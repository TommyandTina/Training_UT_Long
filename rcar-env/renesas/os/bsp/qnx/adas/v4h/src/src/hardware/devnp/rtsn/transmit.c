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
#include <netdrvr/avb.h>
#include <netinet/ip.h>

#include <bpfilter.h>
#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

static void rtsnes_reap_tx (rtsnes_dev_t *rtsnes, uint8_t queue)
{
    uint32_t idx;

    idx = rtsnes->tx_cidx[queue];
    while ((idx != rtsnes->tx_pidx[queue]) &&
           ((rtsnes->tx_bd[queue][idx].die_dt & DT_MASK) == DT_FEMPTY)) {
        if (rtsnes->cfg.verbose & VERBOSE_TX) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO,
              "rtsnes: Tx reap queue %d index %d", queue, idx);
        }

        m_freem(rtsnes->tx_pkts[queue][idx]);
        rtsnes->tx_pkts[queue][idx] = NULL;
        idx = (idx + 1) % NUM_TX_DESC;
    }

    rtsnes->tx_bd[queue][NUM_TX_DESC].die_dt = DT_LINK;

    rtsnes->tx_cidx[queue] = idx;
}

static int rtsnes_tx (rtsnes_dev_t *rtsnes, struct mbuf *m, uint8_t queue)
{
    struct mbuf     *m2;
    uint32_t        idx;
    RTSNES_TX_DESC  *bd;
    struct ifnet    *ifp = &rtsnes->ecom.ec_if;
    off64_t         m_addr;
    ptpv2hdr_t      *ph;
    uint8_t         msg_type; /* PTP msg type */
    uint16_t        sequence_id;/* PTP message sequence ID */
    int             tx_ph_idx;

    if (queue >= NUM_TX_QUEUES) {
        return EINVAL;
    }

    idx = rtsnes->tx_pidx[queue];
    bd = &rtsnes->tx_bd[queue][idx];
    //if ((bd->die_dt & DT_MASK) != DT_FEMPTY) {
    //    // this should NEVER happen
    //    slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes:  Tx descriptors hit full");
    //    rtsnes->stats.un.estats.internal_tx_errors++;
    //    ifp->if_oerrors++;
    //    m_freem(m);
    //    return EINVAL;
    //}
    //
    //if (rtsnes->tx_pkts[queue][idx]) {
    //    m_freem(rtsnes->tx_pkts[queue][idx]);
    //    rtsnes->tx_pkts[queue][idx] = NULL;
    //}

    /* Hardware cannot do gather DMA */
    if (m->m_next != NULL) {
        m2 = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
        if (m2 == NULL) {
            m_freem(m);
            rtsnes->stats.tx_failed_allocs++;
            ifp->if_oerrors++;
            return ENOMEM;
        }

        m_copydata(m, 0, m->m_pkthdr.len, mtod(m2, caddr_t));
        m2->m_pkthdr.len = m2->m_len = m->m_pkthdr.len;
        m_freem(m);
        m = m2;
    }

    rtsnes->tx_pkts[queue][idx] = m;
    bd->info_ds = m->m_len;
    bd->info1 =  m->m_len;
    m_addr = mbuf_phys(m);
    bd->dptr = m_addr;

    CACHE_FLUSH(&rtsnes->cachectl, m->m_data, m_addr, m->m_len);

    if (rtsnes_ptp_is_eventmsg(m, &ph)) {
        msg_type =  ph->messageId & 0x0f; 
        if ((queue < NUM_PTP_TX_QUEUES) && (msg_type < PTP_TX_MSGTYPE)) {
            sequence_id = ntohs(ph->sequenceId);
            tx_ph_idx = (msg_type * PTP_TX_BUF_LEN) + (sequence_id % PTP_TX_BUF_LEN);
            memcpy(&(tx_ph[queue][tx_ph_idx]), ph, sizeof(ptpv2hdr_t));
        }
        else {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rtsnes: Transmit PTP queue %d or msg type %d not supported", queue, msg_type);
        }
    }

    mem_barrier();
    bd->die_dt = DT_FSINGLE | DIE;

    if (rtsnes->cfg.verbose & VERBOSE_TX) {
        slogf(_SLOGC_NETWORK, _SLOG_INFO,
          "rtsnes: Transmit packet length %d at queue %d index %d", m->m_len, queue, idx);
    }

#if NBPFILTER > 0
    /* Pass the packet to any BPF listeners */
    if (ifp->if_bpf) {
        bpf_mtap(ifp->if_bpf, m);
    }
#endif

    ifp->if_opackets++;
    rtsnes->stats.txed_ok++;
    rtsnes->stats.octets_txed_ok += m->m_len;
    if (m->m_flags & M_MCAST) {
        ifp->if_omcasts++;
        rtsnes->stats.txed_multicast++;
    }
    if (m->m_flags & M_BCAST) {
        ifp->if_omcasts++;
        rtsnes->stats.txed_broadcast++;
    }

    rtsnes->tx_pidx[queue] = (idx + 1) % NUM_TX_DESC;

    mem_barrier();

    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TRCR(0), BIT(queue)); //Transmission Start Request

    return EOK;
}

void rtsnes_start (struct ifnet *ifp)
{
    rtsnes_dev_t  *rtsnes;
    struct mbuf   *m;
    int           idx, next_idx;
    int           timeout = 10000;

    rtsnes = ifp->if_softc;

    if (((ifp->if_flags & IFF_RUNNING) == 0) || ((rtsnes->cfg.flags & NIC_FLAG_LINK_DOWN) != 0)) {
        /* Get rid of any stale traffic */
        IFQ_PURGE(&ifp->if_snd);
        rtsnes->stats.un.estats.no_carrier++;
        NW_SIGUNLOCK(&ifp->if_snd_ex, rtsnes->iopkt);
        return;
    }

    ifp->if_flags_tx |= IFF_OACTIVE;

    while (1) {
        idx = rtsnes->tx_pidx[0];
        next_idx = (idx + 1) % NUM_TX_DESC;

        if (next_idx == rtsnes->tx_cidx[0]) {
            /* Ran out of Tx descriptors, see if we can free some up */
            rtsnes_reap_tx(rtsnes, 0);

            if (next_idx == rtsnes->tx_cidx[0]) {
#if 1
                /*
                 * FIXME: Out of Tx descriptors, should wait for at least one txdone for them to continue ?
                 */
                if(--timeout > 0) {
                    nanospin_ns(100);
                    continue;
                }
#endif
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "TX descritor hits full");
                /* Out of Tx descriptors, leave IFF_OACTIVE set */
                NW_SIGUNLOCK(&ifp->if_snd_ex, rtsnes->iopkt);
                return;
            }
        }

        IFQ_DEQUEUE(&ifp->if_snd, m);
        if (m == NULL) {
            /* Done */
            break;
        }

        rtsnes_tx(rtsnes, m, 0);
    }

    rtsnes->tx_bd[0][NUM_TX_DESC].die_dt = DT_LINK;

    ifp->if_flags_tx &= ~IFF_OACTIVE;
    NW_SIGUNLOCK(&ifp->if_snd_ex, rtsnes->iopkt);
}


int rtsnes_output (struct ifnet *ifp, struct mbuf *m,
         struct sockaddr *dst, struct rtentry *rt)
{
    rtsnes_dev_t   *rtsnes = ifp->if_softc;
    struct m_tag    *tag;
    uint8_t         priority, queue = 0;
    int             error;

    tag = GET_TXQ_TAG(m);
    if (tag == NULL) {
        priority = 0;
    } else {
        priority = EXTRACT_TXQ_TAG(tag);
    }

    switch (priority) {
        case 0:
        case 1:
            /* Do a normal if_output via the stack, will be on TXQ 0 */
            return rtsnes->stack_output(ifp, m, dst, rt);
            break;
        case 2:
            /* Send the mbuf on TXQ 2 */
            break;
        default:
            /* Send the mbuf on TXQ 1 */
            break;
    }

    NW_SIGLOCK_P(&rtsnes->ecom.ec_if.if_snd_ex, rtsnes->iopkt, WTP);

    /*
     * The packet already has an ether header on it but is moved down to let
     * ether_output() do the header as well. As this is going direct, just
     * restore the current header.
     */
    m->m_data -= sizeof(struct ether_header);
    m->m_len += sizeof(struct ether_header);
    m->m_pkthdr.len += sizeof(struct ether_header);

    error = rtsnes_tx(rtsnes, m, queue);

    NW_SIGUNLOCK_P(&rtsnes->ecom.ec_if.if_snd_ex, rtsnes->iopkt, WTP);
    
    return error;
}
