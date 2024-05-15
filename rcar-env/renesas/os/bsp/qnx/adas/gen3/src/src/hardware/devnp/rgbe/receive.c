/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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

#include "gether.h"

#include <bpfilter.h>
#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

void gether_receive (gether_dev_t *gether, struct nw_work_thread *wtp)
{
    struct mbuf    *m, *m2;
    struct ifnet   *ifp;
    uint8_t        *dptr;
    int            idx;

    ifp = &gether->ecom.ec_if;

    gether->pkts_received = 1;

    for (;;) {
        idx = gether->rx_idx;

        if (gether->rx_bd[idx].status & RX_RD0_RACT) {
            /* If it was out of Rx descriptors and stopped, restart it */
            if (in32(gether->base + GETHER_EDRRR)  != GETHER_EDRRR_RR) {
                out32(gether->base + GETHER_EDRRR, GETHER_EDRRR_RR);
            }
            return;
        }

        /* Multicast (RX_RD0_RFS(7)) is not error */
        if (gether->rx_bd[idx].status & (RX_RD0_RFS(0) | RX_RD0_RFS(1) | RX_RD0_RFS(2) | RX_RD0_RFS(3) |
                RX_RD0_RFS(4) | RX_RD0_RFS(5) | RX_RD0_RFS(9))) {
            /* Packet has errors, skip it  */
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Rx error index %d status 0x%x",
                idx, gether->rx_bd[idx].status);
            gether->rx_bd[idx].status |= RX_RD0_RACT;
            ifp->if_ierrors++;
            gether->rx_idx = (idx + 1) % NUM_RX_DESC;
            continue;
        }

        m2 = m_getcl_wtp(M_DONTWAIT, MT_DATA, M_PKTHDR, wtp);
        if (m2 == NULL) {
            /* Failed to get new mbuf, return the old one */
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "gether: Rx index %d failed to retrieve new mbuf", idx);
            gether->rx_bd[idx].status |= RX_RD0_RACT;
            gether->stats.rx_failed_allocs++;
            ifp->if_ierrors++;
            gether->rx_idx = (idx + 1) % NUM_RX_DESC;
            continue;
        }

        m = gether->rx_pkts[idx];
        m->m_pkthdr.len = m->m_len = gether->rx_bd[idx].frame_len;
        m->m_pkthdr.rcvif = ifp;
        CACHE_INVAL(&gether->cachectl, gether->rx_pkts[idx]->m_data,
                gether->rx_bd[idx].addr, m->m_len);

        gether->rx_pkts[idx] = m2;
        gether->rx_bd[idx].addr = mbuf_phys(m2);
        CACHE_FLUSH(&gether->cachectl, m2->m_data, gether->rx_bd[idx].addr,
                        m2->m_ext.ext_size);
        gether->rx_bd[idx].status |= RX_RD0_RACT;

        if (gether->cfg.verbose & VERBOSE_RX) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "gether: Receive packet length %d at index %d",
                m->m_len, idx);
        }

    #if NBPFILTER > 0
        /* Pass this up to any BPF listeners. */
        if (ifp->if_bpf) {
            bpf_mtap(ifp->if_bpf, m);
        }
    #endif

        /* Pass the packet in to the stack */
        ifp->if_ipackets++;
        gether->stats.rxed_ok++;
        gether->stats.octets_rxed_ok += m->m_len;
        dptr = mtod (m, uint8_t *);
        if (ETHER_IS_MULTICAST(dptr)) {
          if (memcmp(dptr, etherbroadcastaddr, ETHER_ADDR_LEN) == 0) {
                gether->stats.rxed_broadcast++;
            } else {
                gether->stats.rxed_multicast++;
            }
        }
        (*ifp->if_input)(ifp, m);

        gether->rx_idx = (idx + 1) % NUM_RX_DESC;
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/receive.c $ $Rev: 865871 $")
#endif
