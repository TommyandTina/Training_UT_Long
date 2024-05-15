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

void gether_reap_tx (gether_dev_t *gether)
{
    int idx;

    idx = gether->tx_cidx;
    while ((idx != gether->tx_pidx) &&
       ((gether->tx_bd[idx].status & TX_TD0_TACT) == 0)) {

    if (gether->cfg.verbose & VERBOSE_TX) {
        slogf(_SLOGC_NETWORK, _SLOG_INFO, "gether: Tx reap index %d", idx);
    }
        if(gether->tx_pkts[idx]) {
            m_freem(gether->tx_pkts[idx]);
            gether->tx_pkts[idx] = NULL;
        }
        idx = (idx + 1) % NUM_TX_DESC;
    }

    gether->tx_cidx = idx;
}

void gether_start (struct ifnet *ifp)
{
    gether_dev_t            *gether;
    struct nw_work_thread    *wtp;
    struct mbuf            *m, *m2;
    uint32_t            idx, next_idx;

    gether = ifp->if_softc;
    wtp = WTP;

    if (((ifp->if_flags & IFF_RUNNING) == 0) || ((gether->cfg.flags & NIC_FLAG_LINK_DOWN) != 0)) {
        /* Get rid of any stale traffic */
        IFQ_PURGE(&ifp->if_snd);
        gether->stats.un.estats.no_carrier++;
        NW_SIGUNLOCK(&ifp->if_snd_ex, gether->iopkt);
        return;
    }

    ifp->if_flags_tx |= IFF_OACTIVE;

    while (1) {
        idx = gether->tx_pidx;
        next_idx = (idx + 1) % NUM_TX_DESC;
        if (next_idx == gether->tx_cidx) {
            /* Ran out of Tx descriptors, see if we can free some up */
            gether_reap_tx(gether);
            if (next_idx == gether->tx_cidx) {
            /* Out of Tx descriptors, leave IFF_OACTIVE set */
            unmask_tx_int(gether);
            NW_SIGUNLOCK_P(&ifp->if_snd_ex, gether->iopkt, wtp);
            return;
            }
        }

        IFQ_DEQUEUE(&ifp->if_snd, m);
        if (m == NULL) {
            /* Done */
            ifp->if_flags_tx &= ~IFF_OACTIVE;
            NW_SIGUNLOCK_P(&ifp->if_snd_ex, gether->iopkt, wtp);
            return;
        }

        ifp->if_opackets++;

    #if NBPFILTER > 0
        /* Pass the packet to any BPF listeners */
        if (ifp->if_bpf) {
            bpf_mtap(ifp->if_bpf, m);
        }
    #endif

        /* Hardware cannot do gather DMA */
        if (m->m_next != NULL) {
            m2 = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
            if (m2 == NULL) {
                m_freem(m);
                gether->stats.tx_failed_allocs++;
                ifp->if_oerrors++;
                continue;
            }
            m_copydata(m, 0, m->m_pkthdr.len, mtod(m2, caddr_t));
            m2->m_pkthdr.len = m2->m_len = m->m_pkthdr.len;
            m_freem(m);
            m = m2;
        }

        gether->tx_pkts[idx] = m;
        gether->tx_bd[idx].buf_len = m->m_len;
        gether->tx_bd[idx].addr = mbuf_phys(m);
        CACHE_FLUSH(&gether->cachectl, m->m_data, gether->tx_bd[idx].addr, m->m_len);

        if (next_idx != 0) {
            gether->tx_bd[idx].status = TX_TD0_TACT | TX_TD0_TFP(3);
        } else {
            gether->tx_bd[idx].status = TX_TD0_TACT | TX_TD0_TFP (3)| TX_TD0_TDLE;
        }

        if (gether->cfg.verbose & VERBOSE_TX) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "gether: Transmit packet length %d at index %d",
                            m->m_len, idx);
        }

        gether->stats.txed_ok++;
        gether->stats.octets_txed_ok += m->m_len;
        if (m->m_flags & M_MCAST) {
            ifp->if_omcasts++;
            gether->stats.txed_multicast++;
        }
        if (m->m_flags & M_BCAST) {
            ifp->if_omcasts++;
            gether->stats.txed_broadcast++;
        }

        gether->tx_pidx = next_idx;
        mem_barrier();

        out32(gether->base + GETHER_EDTRR, GETHER_EDTRR_TR_DMA);
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/transmit.c $ $Rev: 878393 $")
#endif
