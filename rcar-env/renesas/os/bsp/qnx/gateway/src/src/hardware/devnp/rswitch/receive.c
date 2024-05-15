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

#include "rswitch.h"
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

int rswitch2_receive (rswitch2_dev_t *rswitch2, struct nw_work_thread *wtp, uint8_t queue)
{
    struct mbuf               *m2;
    struct ifnet              *ifp;
    uint32_t                  idx;
    uint8_t                   *dptr = 0;
    uint8_t                   dt;
    RSWITCH2_RX_DESC          *bd;
    struct ether_vlan_header  *vlan_hdr;
    const struct sigevent     *evp;
    off64_t                   m_addr;
    ptpv2hdr_t                *ph;

    if (queue >= NUM_RX_QUEUES)
        return -EINVAL;

    ifp = &rswitch2->ecom.ec_if;
    rswitch2->pkts_received = 1;
    for (;;) {
        idx = rswitch2->rx_idx[queue];
        bd = &rswitch2->rx_bd[queue][idx];
        if((bd->die_dt & DT_MASK) == DT_FEMPTY) {
            break;
        }

        m2 = m_getcl_wtp(M_DONTWAIT, MT_DATA, M_PKTHDR, wtp);
        if (m2 == NULL) {
            /* Failed to get new mbuf, return the old one */
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Rx queue %d index %d failed to retrieve new mbuf", queue, idx);
            bd->die_dt = DT_FEMPTY | DIE;
            rswitch2->stats.rx_failed_allocs++;
            ifp->if_ierrors++;
            rswitch2->rx_idx[queue] = (idx + 1) % NUM_RX_DESC;
            continue;
        }

        /* pull rxd packet out of corresponding queue */
        if (rswitch2->rpkt[queue] == NULL) {
            rswitch2->rpkt[queue] = rswitch2->rx_pkts[queue][idx];
            rswitch2->rpkt_tail[queue] = rswitch2->rpkt[queue];
        } else {
            rswitch2->rpkt_tail[queue]->m_next = rswitch2->rx_pkts[queue][idx];
            rswitch2->rpkt_tail[queue] = rswitch2->rpkt_tail[queue]->m_next;
        }
        /* If last then this length gets fixed later */
        rswitch2->rpkt_tail[queue]->m_len = bd->info_ds & RX_DS;
        dt = bd->die_dt & DT_MASK;

        CACHE_INVAL(&rswitch2->cachectl, rswitch2->rx_pkts[queue][idx]->m_data,
                (off64_t)bd->dptrh << 32 | bd->dptrl, rswitch2->rx_pkts[queue][idx]->m_ext.ext_size);

        rswitch2->rx_pkts[queue][idx] = m2;
        m_addr = mbuf_phys(m2);
        bd->dptrl = m_addr;
        bd->dptrh = m_addr >> 32;
        bd->info_ds = ALIGN(PKT_BUF_SZ, DPTR_ALIGN);
        bd->die_dt = DT_FEMPTY | DIE;
        CACHE_FLUSH(&rswitch2->cachectl, m2->m_data, m_addr,  m2->m_ext.ext_size);

        if (rswitch2->cfg.verbose & VERBOSE_RX) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO,  "rswitch2: Port %d receive packet length %d at queue %d index %d",
                            (&rswitch2->etha)->index, rswitch2->rpkt_tail[queue]->m_len, queue, idx);
        }

        /* Frame End or Single? If so, pass the packet upto the stack */
        if ((dt == (DT_FEND & DT_MASK)) || (dt == (DT_FSINGLE & DT_MASK))) {
            if (rswitch2_ptp_is_eventmsg(rswitch2->rpkt[queue], &ph)) {
                rswitch2_ptp_add_rx_timestamp(rswitch2, ph, bd);
            }

            /* update rpkt for this rxd fragment */
            rswitch2->rpkt[queue]->m_pkthdr.rcvif = ifp;
            rswitch2->rpkt[queue]->m_pkthdr.len = rswitch2->rpkt_tail[queue]->m_len + rswitch2->length[queue];

#if NBPFILTER > 0
            /* Pass this up to any BPF listeners. */
            if (ifp->if_bpf) {
                bpf_mtap(ifp->if_bpf, rswitch2->rpkt[queue]);
            }
#endif

            /* Pass the packet upto the stack */
            ifp->if_ipackets++;
            rswitch2->stats.rxed_ok++;
            rswitch2->stats.octets_rxed_ok += rswitch2->rpkt[queue]->m_pkthdr.len;
            dptr = mtod (rswitch2->rpkt[queue], uint8_t *);
            if (ETHER_IS_MULTICAST(dptr)) {
                if (IS_BROADCAST(dptr)) {
                    rswitch2->stats.rxed_broadcast++;
                } else {
                    rswitch2->stats.rxed_multicast++;
                }
            }
            if (!rswitch2->avb) {
                (*ifp->if_input)(ifp, rswitch2->rpkt[queue]);
            } else {
               vlan_hdr = mtod(rswitch2->rpkt[queue], struct ether_vlan_header*);
                if ((ntohs(vlan_hdr->evl_encap_proto) == ETHERTYPE_VLAN) && (ntohs(vlan_hdr->evl_proto) == ETHERTYPE_1722)) {
                    /* 1722 packet, send it straight up for minimum latency */
                    (*ifp->if_input)(ifp, rswitch2->rpkt[queue]);
                } else {
                    /*
                     * Send it up from a stack thread so bridge and
                     * fastforward work. Without this we get logs of "no flow"
                     */
                    pthread_mutex_lock(&rswitch2->rx_mutex);
                    if (IF_QFULL(&rswitch2->rx_queue)) {
                        m_freem(rswitch2->rpkt[queue]);
                        ifp->if_ierrors++;
                        rswitch2->stats.rx_failed_allocs++;
                    } else {
                        IF_ENQUEUE(&rswitch2->rx_queue, rswitch2->rpkt[queue]);
                        if (!rswitch2->rx_running) {
                            rswitch2->rx_running = 1;
                            evp = interrupt_queue(rswitch2->iopkt, &rswitch2->inter_queue);
                            if (evp != NULL) {
                                MsgSendPulsePtr(evp->sigev_coid, evp->sigev_priority,
                                        evp->sigev_code, evp->sigev_value.sival_ptr);
                            }
                        }
                    }
                   pthread_mutex_unlock(&rswitch2->rx_mutex);
                }
            }

            /* Reset for the next packet */
            rswitch2->rpkt[queue] = rswitch2->rpkt_tail[queue] = NULL;
            rswitch2->length[queue] = 0;
        } else {
            rswitch2->length[queue] += rswitch2->rpkt_tail[queue]->m_len;
        }
        rswitch2->rx_idx[queue] = (idx + 1) % NUM_RX_DESC;
    }

    rswitch2->pkts_received = 0;
    return 1;
}
