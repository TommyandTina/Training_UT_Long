/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

#include "rtsn.h"
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

int rtsnes_receive (rtsnes_dev_t *rtsnes, struct nw_work_thread *wtp, uint8_t queue)
{
    struct mbuf                 *m2;
    struct ifnet                *ifp;
    uint32_t                    idx;
    uint8_t                     *dptr = 0;
    uint8_t                     die_dt;
    RTNSES_RX_EXT_DESC          *bd;
    struct ether_vlan_header    *vlan_hdr;
    const struct sigevent       *evp;
    off64_t                     m_addr;
    ptpv2hdr_t                  *ph;

    if (queue >= NUM_RX_QUEUES)
        return -EINVAL;

    ifp = &rtsnes->ecom.ec_if;
    rtsnes->pkts_received = 1;

    for (;;) {
        idx = rtsnes->rx_idx[queue];
        bd = &rtsnes->rx_bd[queue][idx];
        if((bd->die_dt & DT_MASK) == DT_FEMPTY) {
            /* If it was out of Rx descriptors and stopped, restart it */
            break;
        }

        m2 = m_getcl_wtp(M_DONTWAIT, MT_DATA, M_PKTHDR, wtp);
        if (m2 == NULL) {
            /* Failed to get new mbuf, return the old one */
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Rx queue %d index %d failed to retrieve new mbuf", queue, idx);
            bd->die_dt = DT_FEMPTY | DIE;
            rtsnes->stats.rx_failed_allocs++;
            ifp->if_ierrors++;
            rtsnes->rx_idx[queue] = (idx + 1) % NUM_RX_DESC;
            continue;
        }

        // pull rxd packet out of corresponding queue
        if (rtsnes->rpkt[queue] == NULL) {
            rtsnes->rpkt[queue] = rtsnes->rx_pkts[queue][idx];
            rtsnes->rpkt_tail[queue] = rtsnes->rpkt[queue];
        } else {
            rtsnes->rpkt_tail[queue]->m_next = rtsnes->rx_pkts[queue][idx];
            rtsnes->rpkt_tail[queue] = rtsnes->rpkt_tail[queue]->m_next;
        }
       /* If last then this length gets fixed later */
        rtsnes->rpkt_tail[queue]->m_len = bd->info_ds & RX_DS;
        die_dt = bd->die_dt & DT_MASK;

        CACHE_INVAL(&rtsnes->cachectl, rtsnes->rx_pkts[queue][idx]->m_data,
                (off64_t)bd->dptr, rtsnes->rx_pkts[queue][idx]->m_ext.ext_size);

        rtsnes->rx_pkts[queue][idx] = m2;
        m_addr = mbuf_phys(m2);
        bd->dptr = m_addr;
        bd->info_ds = ALIGN(PKT_BUF_SZ, DPTR_ALIGN);
        bd->die_dt = DT_FEMPTY | DIE;
        CACHE_FLUSH(&rtsnes->cachectl, m2->m_data, m_addr,
                        m2->m_ext.ext_size);

        if (rtsnes->cfg.verbose & VERBOSE_RX) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO,
              "rtsnes: Receive packet length %d at queue %d index %d",
              rtsnes->rpkt_tail[queue]->m_len, queue, idx);
        }

        if ((die_dt == DT_FEND) || (die_dt == DT_FSINGLE)) {
            if (rtsnes_ptp_is_eventmsg(rtsnes->rpkt[queue], &ph)) {
                rtsnes_ptp_add_rx_timestamp(rtsnes, ph, bd);
            }
            // update rpkt for this rxd fragment
            rtsnes->rpkt[queue]->m_pkthdr.rcvif = ifp;

            /* Fix the lengths */
            rtsnes->rpkt[queue]->m_pkthdr.len = rtsnes->rpkt_tail[queue]->m_len + rtsnes->length[queue];
            //rtsnes->rpkt_tail[queue]->m_len -= rtsnes->length[queue];

#if NBPFILTER > 0
            /* Pass this up to any BPF listeners. */
            if (ifp->if_bpf) {
                bpf_mtap(ifp->if_bpf, rtsnes->rpkt[queue]);
            }
#endif

            /* Pass the packet in to the stack */
            ifp->if_ipackets++;
            rtsnes->stats.rxed_ok++;
            rtsnes->stats.octets_rxed_ok += rtsnes->rpkt[queue]->m_pkthdr.len;
            dptr = mtod (rtsnes->rpkt[queue], uint8_t *);
            if (ETHER_IS_MULTICAST(dptr)) {
                if (IS_BROADCAST(dptr)) {
                    rtsnes->stats.rxed_broadcast++;
                } else {
                    rtsnes->stats.rxed_multicast++;
                }
            }
            if (!rtsnes->avb) {
                (*ifp->if_input)(ifp, rtsnes->rpkt[queue]);
            } else {
               vlan_hdr = mtod(rtsnes->rpkt[queue], struct ether_vlan_header*);
                if ((ntohs(vlan_hdr->evl_encap_proto) == ETHERTYPE_VLAN) &&
                        (ntohs(vlan_hdr->evl_proto) == ETHERTYPE_1722)) {
                    /* 1722 packet, send it straight up for minimum latency */
                    (*ifp->if_input)(ifp, rtsnes->rpkt[queue]);
                } else {
                    /*
                     * Send it up from a stack thread so bridge and
                     * fastforward work. Without this we get logs of "no flow"
                     */
                    pthread_mutex_lock(&rtsnes->rx_mutex);
                    if (IF_QFULL(&rtsnes->rx_queue)) {
                        m_freem(rtsnes->rpkt[queue]);
                        ifp->if_ierrors++;
                        rtsnes->stats.rx_failed_allocs++;
                    } else {
                        IF_ENQUEUE(&rtsnes->rx_queue, rtsnes->rpkt[queue]);
                        if (!rtsnes->rx_running) {
                            rtsnes->rx_running = 1;
                            evp = interrupt_queue(rtsnes->iopkt, &rtsnes->inter_queue);
                            if (evp != NULL) {
                                MsgSendPulsePtr(evp->sigev_coid, evp->sigev_priority,
                                        evp->sigev_code, evp->sigev_value.sival_ptr);
                            }
                        }
                    }
                   pthread_mutex_unlock(&rtsnes->rx_mutex);
                }
            }
            /* Reset for the next packet */
            rtsnes->rpkt[queue] = rtsnes->rpkt_tail[queue] = NULL;
            rtsnes->length[queue] = 0;
        } else {
            rtsnes->length[queue] += rtsnes->rpkt_tail[queue]->m_len;
        }

        rtsnes->rx_idx[queue] = (idx + 1) % NUM_RX_DESC;
    }

    bd = &rtsnes->rx_bd[queue][NUM_RX_DESC];
    bd->die_dt = DT_LINK;

    rtsnes->pkts_received = 0;

    return 1;
}
