/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include "ravb.h"
#include <net/if_vlanvar.h>
#include <netinet/in.h>
#include <netdrvr/avb.h>

#include <bpfilter.h>
#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

#define IS_BROADCAST(dptr) \
    ((dptr)[0] == 0xff && (dptr)[1] == 0xff && \
    (dptr)[2] == 0xff && (dptr)[3] == 0xff && \
    (dptr)[4] == 0xff && (dptr)[5] == 0xff)

int ravb_receive (ravb_dev_t *ravb, struct nw_work_thread *wtp, uint8_t queue)
{
    struct mbuf     *m2;
    struct ifnet    *ifp;
    uint32_t        idx;
    uint8_t         *dptr = 0;
    uint8_t         die_dt;
    ptpv2hdr_t      *ph;
    RAVB_RX_DESC    *bd;
    struct ether_vlan_header    *vlan_hdr;
    const struct sigevent   *evp;

    ifp = &ravb->ecom.ec_if;

    ravb->pkts_received = 1;

    for (;;) {
        idx = ravb->rx_idx[queue];
        bd = &ravb->rx_bd[queue][idx];

        if(bd->die_dt == DT_FEMPTY) {
            /* If it was out of Rx descriptors and stopped, restart it */
            return 0;
        }

        if (bd->msc & (MSC_CRC | MSC_RFE | MSC_RTSF | MSC_RTLF | MSC_CEEF)) {
            bd->die_dt = DT_FEMPTY;
            ifp->if_ierrors++;
            ravb->rx_idx[queue] = (idx + 1) % NUM_RX_DESC;
            continue;
        }

        m2 = m_getcl_wtp(M_DONTWAIT, MT_DATA, M_PKTHDR, wtp);
        if (m2 == NULL) {
            /* Failed to get new mbuf, return the old one */
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: Rx queue %d index %d failed to retrieve new mbuf", queue, idx);
            bd->die_dt = DT_FEMPTY;
            ravb->stats.rx_failed_allocs++;
            ifp->if_ierrors++;
            ravb->rx_idx[queue] = (idx + 1) % NUM_RX_DESC;
            continue;
        }

        // pull rxd packet out of corresponding queue
        if (ravb->rpkt[queue] == NULL) {
            ravb->rpkt[queue] = ravb->rx_pkts[queue][idx];
            ravb->rpkt_tail[queue] = ravb->rpkt[queue];
        } else {
            ravb->rpkt_tail[queue]->m_next = ravb->rx_pkts[queue][idx];
            ravb->rpkt_tail[queue] = ravb->rpkt_tail[queue]->m_next;
        }
       /* If last then this length gets fixed later */
        ravb->rpkt_tail[queue]->m_len = bd->ds_cc & RX_DS;
        die_dt = bd->die_dt;

        CACHE_INVAL(&ravb->cachectl, ravb->rx_pkts[queue][idx]->m_data,
                bd->dptr, ravb->rx_pkts[queue][idx]->m_ext.ext_size);

        ravb->rx_pkts[queue][idx] = m2;
        bd->dptr = mbuf_phys(m2);
        bd->ds_cc = ALIGN(PKT_BUF_SZ, DPTR_ALIGN);
        bd->die_dt = DT_FEMPTY;
        CACHE_FLUSH(&ravb->cachectl, m2->m_data, bd->dptr,
                        m2->m_ext.ext_size);

        if (ravb->options.cfg.verbose & VERBOSE_RX) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO,
              "ravb: Receive packet length %d at queue %d index %d",
              ravb->rpkt_tail[queue]->m_len, queue, idx);
        }

        if (die_dt & DT_FEND) {     // Last descriptor
            if (ravb_ptp_is_eventmsg(ravb->rpkt[queue], &ph)) {
                ravb_ptp_add_rx_timestamp(ravb, ph, bd);
            }

            // update rpkt for this rxd fragment
            ravb->rpkt[queue]->m_pkthdr.rcvif = ifp;

            /* Fix the lengths */
            ravb->rpkt[queue]->m_pkthdr.len = ravb->rpkt_tail[queue]->m_len;
            ravb->rpkt_tail[queue]->m_len -= ravb->length[queue];

#if NBPFILTER > 0
            /* Pass this up to any BPF listeners. */
            if (ifp->if_bpf) {
                bpf_mtap(ifp->if_bpf, ravb->rpkt[queue]);
            }
#endif

            /* Pass the packet in to the stack */
            ifp->if_ipackets++;
            ravb->stats.rxed_ok++;
            ravb->stats.octets_rxed_ok += ravb->rpkt[queue]->m_pkthdr.len;
            dptr = mtod (ravb->rpkt[queue], uint8_t *);
            if (ETHER_IS_MULTICAST(dptr)) {
                if (IS_BROADCAST(dptr)) {
                    ravb->stats.rxed_broadcast++;
                } else {
                    ravb->stats.rxed_multicast++;
                }
            }
            if (!ravb->options.avb) {
                (*ifp->if_input)(ifp, ravb->rpkt[queue]);
            } else {
               vlan_hdr = mtod(ravb->rpkt[queue], struct ether_vlan_header*);
                if ((ntohs(vlan_hdr->evl_encap_proto) == ETHERTYPE_VLAN) &&
                        (ntohs(vlan_hdr->evl_proto) == ETHERTYPE_1722)) {
                    /* 1722 packet, send it straight up for minimum latency */
                    (*ifp->if_input)(ifp, ravb->rpkt[queue]);
                } else {
                /*
                     * Send it up from a stack thread so bridge and
                     * fastforward work. Without this we get logs of "no flow"
                     */
                    pthread_mutex_lock(&ravb->rx_mutex);
                    if (IF_QFULL(&ravb->rx_queue)) {
                        m_freem(ravb->rpkt[queue]);
                        ifp->if_ierrors++;
                        ravb->stats.rx_failed_allocs++;
                    } else {
                        IF_ENQUEUE(&ravb->rx_queue, ravb->rpkt[queue]);
                        if (!ravb->rx_running) {
                            ravb->rx_running = 1;
                            evp = interrupt_queue(ravb->options.iopkt, &ravb->inter_queue);
                            if (evp != NULL) {
                                MsgSendPulsePtr(evp->sigev_coid, evp->sigev_priority,
                                        evp->sigev_code, evp->sigev_value.sival_ptr);
                            }
                        }
                    }
                   pthread_mutex_unlock(&ravb->rx_mutex);
                }
            }
            /* Reset for the next packet */
            ravb->rpkt[queue] = ravb->rpkt_tail[queue] = NULL;
            ravb->length[queue] = 0;
        } else {
            ravb->length[queue] += ravb->rpkt_tail[queue]->m_len;
        }
        ravb->rx_idx[queue] = (idx + 1) % NUM_RX_DESC;
    }
    return 1;
}
