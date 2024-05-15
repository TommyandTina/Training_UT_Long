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

#include "rswitch.h"

#define IS_BROADCAST(dptr) \
    ((dptr)[0] == 0xff && (dptr)[1] == 0xff && \
    (dptr)[2] == 0xff && (dptr)[3] == 0xff && \
    (dptr)[4] == 0xff && (dptr)[5] == 0xff)

static void rswitch2_rxfinish_one(rswitch2_dev_t *rswitch2, RSWITCH2_RX_DESC *bd, int idx, int queue)
{
    struct ifnet *ifp;
    struct mbuf *m;
    struct rswitch2_bufmap *map;
    uint8_t dt;
    ptpv2hdr_t *ph;

    ifp = rswitch2->ifp;
    map = &rswitch2->rx_buf_map[queue][idx];
    m = map->m;

    if (m == NULL) {
        if_inc_counter(ifp, IFCOUNTER_IERRORS, 1);
        device_printf(rswitch2->dev,"No mbuf in in queue %d chain descriptor %d\n", queue, idx);
        return;
    }

    // pull rxd packet out of corresponding queue
    if (rswitch2->rpkt[queue] == NULL) {
        rswitch2->rpkt[queue] = m;
        rswitch2->rpkt_tail[queue] = rswitch2->rpkt[queue];
    } else {
        rswitch2->rpkt_tail[queue]->m_next = m;
        rswitch2->rpkt_tail[queue] = rswitch2->rpkt_tail[queue]->m_next;
    }
   /* If last then this length gets fixed later */
    rswitch2->rpkt_tail[queue]->m_len = bd->info_ds & RX_DS;
    dt = bd->die_dt & DT_MASK;

    /* Synchronize DMA for newly received packet */
    bus_dmamap_sync(rswitch2->rx_buf_tag[queue], map->map, BUS_DMASYNC_POSTREAD);
    bus_dmamap_unload(rswitch2->rx_buf_tag[queue], map->map);

    /* Frame End or Single? If so, pass the packet upto the stack */
    if ((dt == (DT_FEND & DT_MASK)) || (dt == (DT_FSINGLE & DT_MASK))) {
        if (rswitch2_ptp_is_eventmsg(rswitch2->rpkt[queue], &ph)) {
            rswitch2_ptp_add_rx_timestamp(rswitch2, ph, bd);
        }

        /* assign packet to this interface interface */
        rswitch2->rpkt[queue]->m_pkthdr.rcvif = ifp;

        /* Fix the lengths */
        // rswitch2->rpkt[queue]->m_pkthdr.len = rswitch2->rpkt_tail[queue]->m_len;
        // rswitch2->rpkt_tail[queue]->m_len -= rswitch2->length[queue];
        rswitch2->rpkt[queue]->m_pkthdr.len = rswitch2->rpkt_tail[queue]->m_len + rswitch2->length[queue];

        //rswitch2->rpkt[queue]->m_pkthdr.flowid = 0;
        //M_HASHTYPE_SET(rswitch2->rpkt[queue], M_HASHTYPE_OPAQUE);

        BPF_MTAP(ifp, rswitch2->rpkt[queue]);

        /*
         * If we use software packet counters, we can increase them here.
         * If we use hardware counters, we can use rswitch2_harvest_stats().
         * For example: if_inc_counter(ifp, IFCOUNTER_IPACKETS, 1);
         */
        if_inc_counter(ifp, IFCOUNTER_IPACKETS, 1);

        if (rswitch2->verbose & VERBOSE_RX) {
            device_printf(rswitch2->dev,"Rx queue %d index %d packet length %d\n", queue, idx, rswitch2->rpkt_tail[queue]->m_len);
        }

        /* pass the frame to the stack */
        mtx_unlock(&rswitch2->mtx);
        (*ifp->if_input)(ifp, rswitch2->rpkt[queue]);
        mtx_lock(&rswitch2->mtx);

        /* Reset for the next packet */
        rswitch2->rpkt[queue] = rswitch2->rpkt_tail[queue] = NULL;
        rswitch2->length[queue] = 0;
    } else {
        rswitch2->length[queue] += rswitch2->rpkt_tail[queue]->m_len;
    }
}

void rswitch2_rxfinish_locked(rswitch2_dev_t *rswitch2, int queue)
{
    RSWITCH2_RX_DESC *bd;
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int idx, i = 0;

    mtx_assert(&rswitch2->mtx, MA_OWNED);

    for (;;) {
        idx = rswitch2->rx_idx[queue];
        bd = &rswitch2->rx_ring[queue][idx];
        if ((bd->die_dt & DT_MASK) == DT_FEMPTY) {
            /*
             * FIXME: Some next descriptors that can raise next interrupt might be process already 
             * to avoid this interrupt we need to clear interrupt here ???
             */ 
            /* Clear status interrupt */
            i = rswitch2->rx_queue_index + queue;
            bus_write_4(rswitch2->res[0], gwca->addr_offset + GWDIS + ((i/32) * 0x10), BIT(i%32));
            break;
        }

        rswitch2_rxfinish_one(rswitch2, bd, idx, queue);

        /* Allocate new buffer */
        if (rswitch2_setup_rxbuf(rswitch2, queue, idx)) {
            /* no new mbuf available, recycle old */
            /* Failed to get new mbuf, return the old one */
            device_printf(rswitch2->dev,"Rx queue %d index %d failed to retrieve new mbuf\n", queue, idx);
            bd->die_dt = DT_FEMPTY | DIE;
            if_inc_counter(rswitch2->ifp, IFCOUNTER_OERRORS, 1);
            rswitch2->rx_idx[queue] = (idx + 1) % RX_DESC_COUNT;
            continue;
        }

        rswitch2->rx_idx[queue] = (idx + 1) % RX_DESC_COUNT;
    }
}
