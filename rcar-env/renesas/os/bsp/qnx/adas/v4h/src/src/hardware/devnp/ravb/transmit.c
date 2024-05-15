/*
 * Copyright (c) 2016, 2022, BlackBerry Limited.
 * Copyright 2022, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ravb.h"
#include <netdrvr/avb.h>
#include <netinet/ip.h>

#include <bpfilter.h>
#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

#if defined(VARIANT_ipmmu)
#include <hw/rcar_ipmmu-api.h>
#endif /* defined(VARIANT_ipmmu) */

void ravb_reap_tx (ravb_dev_t *ravb, uint8_t queue)
{
    uint32_t idx;

    idx = ravb->tx_cidx[queue];
    while ((idx != ravb->tx_pidx[queue]) &&
           (ravb->tx_bd[queue][idx].die_dt == DT_FEMPTY)) {
        if (ravb->options.cfg.verbose & VERBOSE_TX) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO,
              "ravb%d: Tx reap queue %d index %d", ravb->options.cfg.device_index, queue, idx);
        }

        /* Clear timestamp request flag */
        if (ravb->tx_bd[queue][idx].tagh_tsr & TX_TSR) {
            ravb->tx_bd[queue][idx].tagh_tsr &= ~TX_TSR;
        }
        m_freem(ravb->tx_pkts[queue][idx]);
        ravb->tx_pkts[queue][idx] = NULL;
        idx = (idx + 1) % NUM_TX_DESC;
    }

    ravb->tx_cidx[queue] = idx;
}

void ravb_start (struct ifnet *ifp)
{
    ravb_dev_t    *ravb;
    struct mbuf   *m;
    uint32_t      idx, next_idx;
    int           timeout = 10000;

    ravb = ifp->if_softc;

    if (((ifp->if_flags & IFF_RUNNING) == 0) || ((ravb->options.cfg.flags & NIC_FLAG_LINK_DOWN) != 0)) {
        /* Get rid of any stale traffic */
        IFQ_PURGE(&ifp->if_snd);
        ravb->stats.un.estats.no_carrier++;
        NW_SIGUNLOCK(&ifp->if_snd_ex, ravb->options.iopkt);
        return;
    }

    ifp->if_flags_tx |= IFF_OACTIVE;

    while (1) {
        idx = ravb->tx_pidx[0];
        next_idx = (idx + 1) % NUM_TX_DESC;

        if (next_idx == ravb->tx_cidx[0]) {
            /* Ran out of Tx descriptors, see if we can free some up */
            ravb_reap_tx(ravb, 0);

            if (next_idx == ravb->tx_cidx[0]) {
#if 1
                /*
                 * FIXME: Out of Tx descriptors, should wait for at least one txdone for them to continue ?
                 */
                if(--timeout > 0) {
                    nanospin_ns(100);
                    continue;
                }
#endif
                /* Out of Tx descriptors, leave IFF_OACTIVE set */
                NW_SIGUNLOCK(&ifp->if_snd_ex, ravb->options.iopkt);
                return;
            }
        }

        IFQ_DEQUEUE(&ifp->if_snd, m);
        if (m == NULL) {
            /* Done */
            break;
        }

        ravb_tx(ravb, m, 0);
    }

    ifp->if_flags_tx &= ~IFF_OACTIVE;
    NW_SIGUNLOCK(&ifp->if_snd_ex, ravb->options.iopkt);
}

int ravb_tx (ravb_dev_t *ravb, struct mbuf *m, uint8_t queue)
{
    struct mbuf     *m2;
    uint32_t        idx, next_idx, tag, req;
    RAVB_TX_DESC    *bd;
    struct ifnet    *ifp = &ravb->ecom.ec_if;
    ptpv2hdr_t      *ph;
    uint8_t         msg_type; /* PTP msg type */
    uint16_t        sequence_id;/* PTP message sequence ID */
    int             tx_ph_idx;

    idx = ravb->tx_pidx[queue];
    next_idx = (idx + 1) % NUM_TX_DESC;

    /* Hardware cannot do gather DMA */
    if (m->m_next != NULL) {
        m2 = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
        if (m2 == NULL) {
            m_freem(m);
            ravb->stats.tx_failed_allocs++;
            ifp->if_oerrors++;
            return ENOMEM;
        }

        m_copydata(m, 0, m->m_pkthdr.len, mtod(m2, caddr_t));
        m2->m_pkthdr.len = m2->m_len = m->m_pkthdr.len;
        m_freem(m);
        m = m2;
    }
    bd = &ravb->tx_bd[queue][idx];
    ravb->tx_pkts[queue][idx] = m;
    bd->ds_tagl = m->m_len;

#if !defined(VARIANT_ipmmu)
    bd->dptr = mbuf_phys(m);
    CACHE_FLUSH(&ravb->cachectl, m->m_data, bd->dptr,
                    m->m_len);

#else /* defined(VARIANT_ipmmu) */
    uint64_t m_phys = mbuf_phys(m);
    iommu_node_t *node = iommu_get_node(ravb, m_phys, m->m_len);
    if (node == NULL) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: iommu failed to get node, Tx phys 0x%lx, size %d",
                m_phys, m->m_len);
        return ENOMEM;
    }
    bd->dptr = node->iova + (/*start offset*/m_phys - node->paddr);
    CACHE_FLUSH(&ravb->cachectl, m->m_data, m_phys, m->m_len);

#endif /* !defined(VARIANT_ipmmu) */

    if (ravb_ptp_is_eventmsg(m, &ph)) {
        msg_type =  ph->messageId & 0x0f;
        if ((queue < NUM_PTP_TX_QUEUES) && (msg_type < PTP_TX_MSGTYPE)) {
            sequence_id = ntohs(ph->sequenceId);
            tx_ph_idx = (msg_type * PTP_TX_BUF_LEN) + (sequence_id % PTP_TX_BUF_LEN);

            memcpy(&(tx_ph[queue][tx_ph_idx]), ph, sizeof(ptpv2hdr_t));

            tag = ((queue << 8) | tx_ph_idx) & 0x3FF;
            bd->ds_tagl |= (tag << 12) & TX_TAGL;
            bd->tagh_tsr = tag >> 4;
            bd->tagh_tsr |= TX_TSR;
        }
        else {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "ravb%d: Transmit PTP queue %d or msg type %d not supported",
                  ravb->options.cfg.device_index, queue, msg_type);
        }
    }

    /*
     * Descriptor Type must be set last
     * DMAC will begin transfer if type
     * is not DT_FEMPTY
     */
    mem_barrier();
    bd->die_dt = DT_FSINGLE;

    if (ravb->options.cfg.verbose & VERBOSE_TX) {
        slogf(_SLOGC_NETWORK, _SLOG_INFO,
          "ravb%d: Transmit packet length %d at queue %d index %d",
          ravb->options.cfg.device_index, m->m_len, queue, idx);
    }

#if NBPFILTER > 0
    /* Pass the packet to any BPF listeners */
    if (ifp->if_bpf) {
        bpf_mtap(ifp->if_bpf, m);
    }
#endif
    ifp->if_opackets++;
    ravb->stats.txed_ok++;
    ravb->stats.octets_txed_ok += m->m_len;
    if (m->m_flags & M_MCAST) {
        ifp->if_omcasts++;
        ravb->stats.txed_multicast++;
    }
    if (m->m_flags & M_BCAST) {
        ifp->if_omcasts++;
        ravb->stats.txed_broadcast++;
    }

    ravb->tx_pidx[queue] = next_idx;

    req = TCCR_TSRQ0 << queue;
    mem_barrier();

    /* Request transmit */
    out32(ravb->base + TCCR, in32(ravb->base + TCCR) | (req));

    return EOK;
}

int ravb_output (struct ifnet *ifp, struct mbuf *m,
         struct sockaddr *dst, struct rtentry *rt)
{
    ravb_dev_t    *ravb = ifp->if_softc;
    struct m_tag  *tag;
    uint8_t       priority, queue = 0;
    int           error;
    uint32_t      idx, next_idx;

    tag = GET_TXQ_TAG(m);
    if (tag == NULL) {
        priority = 0;
    } else {
        priority = EXTRACT_TXQ_TAG(tag);
    }

    /*
     * Map the eight priorities to the 4 queues
     * According to 802.1Q-2011 Table 34-1
     */
    switch (priority) {
        case 0:
        case 1:
            /* Do a normal if_output via the stack, will be on TXQ 0 */
            return ravb->stack_output(ifp, m, dst, rt);
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

    NW_SIGLOCK_P(&ravb->ecom.ec_if.if_snd_ex, ravb->options.iopkt, WTP);

    idx = ravb->tx_pidx[queue];
    next_idx = (idx + 1) % NUM_TX_DESC;

    if (next_idx == ravb->tx_cidx[queue]) {
        /* Ran out of Tx descriptors, see if we can free some up */
        ravb_reap_tx(ravb, queue);
        if (next_idx == ravb->tx_cidx[queue]) {
            /* Out of Tx descriptors, leave IFF_OACTIVE set */
            NW_SIGUNLOCK_P(&ifp->if_snd_ex, ravb->options.iopkt, WTP);
            return ENOBUFS;
        }
    }

    /*
     * The packet already has an ether header on it but is moved down to let
     * ether_output() do the header as well. As this is going direct, just
     * restore the current header.
     */
    m->m_data -= sizeof(struct ether_header);
    m->m_len += sizeof(struct ether_header);
    m->m_pkthdr.len += sizeof(struct ether_header);

    error = ravb_tx(ravb, m, queue);
    NW_SIGUNLOCK_P(&ravb->ecom.ec_if.if_snd_ex, ravb->options.iopkt, WTP);
    return error;
}

