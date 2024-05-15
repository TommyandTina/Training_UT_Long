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
#include <netdrvr/avb.h>
#include <netinet/ip.h>

#include <bpfilter.h>
#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

static int rswitch2_tx (rswitch2_dev_t *rswitch2, struct mbuf *m, uint8_t queue)
{
    struct mbuf           *m2;
    RSWITCH2_TX_DESC      *bd;
    struct ifnet          *ifp = &rswitch2->ecom.ec_if;
    struct rswitch2_gwca  *gwca = &rswitch2->gwca;
    struct rswitch2_etha  *etha = &rswitch2->etha;
    off64_t               m_addr;
    ptpv2hdr_t            *ph;
    uint8_t               msg_type; /* PTP msg type */
    uint16_t              sequence_id;/* PTP message sequence ID */
    int                   idx, tx_ph_idx;

    idx = rswitch2->tx_pidx[queue];
    bd = &rswitch2->tx_bd[queue][idx];
    if ((bd->die_dt & DT_MASK) != DT_FEMPTY) {
        // this should NEVER happen
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2:  Tx descriptors hit full");
        rswitch2->stats.un.estats.internal_tx_errors++;
        ifp->if_oerrors++;
        m_freem(m);
        return EINVAL;
    }

    if (rswitch2->tx_pkts[queue][idx]) {
        m_freem(rswitch2->tx_pkts[queue][idx]);
        rswitch2->tx_pkts[queue][idx] = NULL;
    }

    /* Hardware cannot do gather DMA */
    if (m->m_next != NULL) {
        m2 = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
        if (m2 == NULL) {
            m_freem(m);
            rswitch2->stats.tx_failed_allocs++;
            ifp->if_oerrors++;
            return ENOMEM;
        }

        m_copydata(m, 0, m->m_pkthdr.len, mtod(m2, caddr_t));
        m2->m_pkthdr.len = m2->m_len = m->m_pkthdr.len;
        m_freem(m);
        m = m2;
    }

    rswitch2->tx_pkts[queue][idx] = m;
    bd->info_ds = m->m_len;
    m_addr = mbuf_phys(m);
    bd->dptrl = m_addr;
    bd->dptrh = m_addr >> 32;
    if(gwca->index == 0) {
        bd->csd0 = rswitch2->tx_queue_index + queue;
    } else {
        bd->csd1 = rswitch2->tx_queue_index + queue;
    }
    bd->dv = BIT(etha->index);
    bd->info1 = BIT(2) | BIT(1);
    bd->die_dt = DT_FSINGLE | DIE;

    CACHE_FLUSH(&rswitch2->cachectl, m->m_data, m_addr, m->m_len);

    if (rswitch2_ptp_is_eventmsg(m, &ph)) {
        msg_type =  ph->messageId & 0x0f; 
        if ((queue < NUM_PTP_TX_QUEUES) && (msg_type < PTP_TX_MSGTYPE)) {
            sequence_id = ntohs(ph->sequenceId);
            tx_ph_idx = (msg_type * PTP_TX_BUF_LEN) + (sequence_id % PTP_TX_BUF_LEN);

            memcpy(&(tx_ph[queue][tx_ph_idx]), ph, sizeof(ptpv2hdr_t));
        }
        else
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rswitch2: Transmit PTP queue %d or msg type %d not supported", queue, msg_type);
    }

    mem_barrier();

    if (rswitch2->cfg.verbose & VERBOSE_TX) {
        slogf(_SLOGC_NETWORK, _SLOG_INFO,
          "rswitch: Port %d transmit packet length %d at queue %d index %d",
          (&rswitch2->etha)->index, m->m_len, queue, idx);
    }

#if NBPFILTER > 0
    /* Pass the packet to any BPF listeners */
    if (ifp->if_bpf) {
        bpf_mtap(ifp->if_bpf, m);
    }
#endif

    ifp->if_opackets++;
    rswitch2->stats.txed_ok++;
    rswitch2->stats.octets_txed_ok += m->m_len;
    if (m->m_flags & M_MCAST) {
        ifp->if_omcasts++;
        rswitch2->stats.txed_multicast++;
    }
    if (m->m_flags & M_BCAST) {
        ifp->if_omcasts++;
        rswitch2->stats.txed_broadcast++;
    }

    rswitch2->tx_pidx[queue] = (idx + 1) % NUM_TX_DESC;

    mem_barrier();

    queue += rswitch2->tx_queue_index;
    rswitch2_modify(gwca->addr, GWTRC(queue / 32), BIT(queue % 32), BIT(queue % 32));

    return EOK;
}

void rswitch2_start (struct ifnet *ifp)
{
    rswitch2_dev_t      *rswitch2;
    struct mbuf         *m;

    rswitch2 = ifp->if_softc;

    if (((ifp->if_flags & IFF_RUNNING) == 0) || ((rswitch2->cfg.flags & NIC_FLAG_LINK_DOWN) != 0)) {
        /* Get rid of any stale traffic */
        IFQ_PURGE(&ifp->if_snd);
        rswitch2->stats.un.estats.no_carrier++;
        NW_SIGUNLOCK(&ifp->if_snd_ex, rswitch2->iopkt);
        return;
    }

    ifp->if_flags_tx |= IFF_OACTIVE;

    while (1) {
        IFQ_DEQUEUE(&ifp->if_snd, m);
        if (m == NULL) {
            /* Done */
            break;
        }

        rswitch2_tx(rswitch2, m, 0);
    }

    ifp->if_flags_tx &= ~IFF_OACTIVE;
    NW_SIGUNLOCK(&ifp->if_snd_ex, rswitch2->iopkt);
}

int rswitch2_output (struct ifnet *ifp, struct mbuf *m,
         struct sockaddr *dst, struct rtentry *rt)
{
    rswitch2_dev_t  *rswitch2 = ifp->if_softc;
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
            return rswitch2->stack_output(ifp, m, dst, rt);
            break;
        case 2:
            /* Send the mbuf on TXQ 2 */
            queue = 2;
            break;
        case 3:
            /* Send the mbuf on TXQ 3 */
            queue = 3;
            break;
        default:
            /* Send the mbuf on TXQ 1 */
            queue = 1;
            break;
    }

    NW_SIGLOCK_P(&rswitch2->ecom.ec_if.if_snd_ex, rswitch2->iopkt, WTP);

    /*
     * The packet already has an ether header on it but is moved down to let
     * ether_output() do the header as well. As this is going direct, just
     * restore the current header.
     */
    m->m_data -= sizeof(struct ether_header);
    m->m_len += sizeof(struct ether_header);
    m->m_pkthdr.len += sizeof(struct ether_header);

    error = rswitch2_tx(rswitch2, m, queue);

    NW_SIGUNLOCK_P(&rswitch2->ecom.ec_if.if_snd_ex, rswitch2->iopkt, WTP);

    return error;
}
