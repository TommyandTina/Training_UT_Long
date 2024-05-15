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

static void rswitch2_txfinished_lock(rswitch2_dev_t *rswitch2, int queue)
{
    uint32_t idx;

    mtx_assert(&(rswitch2)->tx_mtx, MA_OWNED);

    idx = rswitch2->tx_cidx[queue];
    while ((idx != rswitch2->tx_pidx[queue]) &&
            ((rswitch2->tx_ring[queue][idx].die_dt & DT_MASK) == DT_FEMPTY)) {

        if (rswitch2->verbose & VERBOSE_TX) {
            device_printf(rswitch2->dev,"rswitch2_tx_reap queue %d index %d\n", queue, idx);
        }
        /*
         * Hardware-specific code here: exit the loop when all
         * transferred packets have been cleared
         */
        if (rswitch2->tx_buf_map[queue][idx].m) {
            bus_dmamap_sync(rswitch2->tx_buf_tag[queue], rswitch2->tx_buf_map[queue][idx].map,
                    BUS_DMASYNC_POSTWRITE);
            bus_dmamap_unload(rswitch2->tx_buf_tag[queue], rswitch2->tx_buf_map[queue][idx].map);
            m_freem(rswitch2->tx_buf_map[queue][idx].m);
            rswitch2->tx_buf_map[queue][idx].m = NULL;
            /*
            * If we use software packet counters, we can increase them here.
            * If we use hardware counters, we can use the sample_harvest_stats() function.
            * For example: if_inc_counter(rswitch2->ifp, IFCOUNTER_OPACKETS, 1);
            */
            if_inc_counter(rswitch2->ifp, IFCOUNTER_OPACKETS, 1);
        }
        idx = (idx + 1) % TX_DESC_COUNT;
    }

    if (idx != rswitch2->tx_cidx[queue]) {
        rswitch2->tx_cidx[queue] = idx;
    }
}

static int rswitch2_txstart_locked(rswitch2_dev_t *rswitch2, int queue)
{
    struct ifnet            *ifp;
    uint32_t                next_idx;
    struct mbuf             *m0, *m2;
    struct mbuf             *m_temp;
    int                     error = 0;
    int                     nsegs;
    struct bus_dma_segment  segs[TX_MAX_SEGS];
    int                     enqueued = 0;
    uint32_t                idx;
    RSWITCH2_TX_DESC        *bd;
    rswitch2_gwca_t         *gwca = &rswitch2->gwca;
    ptpv2hdr_t              *ph;
    uint8_t                 msg_type; /* PTP msg type */
    uint16_t                sequence_id;/* PTP message sequence ID */
    int                     tx_ph_idx;
    int                     tx_idx;

    mtx_assert(&(rswitch2)->tx_mtx, MA_OWNED);

    if (((rswitch2->ifp->if_drv_flags & IFF_DRV_RUNNING) == 0) ||
        !rswitch2->link_is_up) {
        return EOK;
    }

    ifp = rswitch2->ifp;

    while(1) {
        idx = rswitch2->tx_pidx[queue];
        next_idx = (idx + 1) % TX_DESC_COUNT;

       /*
        * Check if hardware can transfer, return if not
        */
        rswitch2_txfinished_lock(rswitch2, queue);

        if (next_idx == rswitch2->tx_cidx[queue]) {
            error = ENOBUFS;
            device_printf(rswitch2->dev,"%s Out of Tx descriptors\n", __func__);
            break;
        }

        m0 = drbr_peek(rswitch2->ifp, rswitch2->br);
        if (m0 == NULL) {
            break;
        }

        /* Hardware cannot do gather DMA */
        if (m0->m_next != NULL)
        {
            m2 = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
            if (m2 == NULL)
            {
                m_freem(m0);
                if_inc_counter(ifp, IFCOUNTER_OERRORS, 1);
                break;
            }
        
            if (m0->m_pkthdr.len < 0) {
                break; 
            }
            m_copydata(m0, 0, m0->m_pkthdr.len, mtod(m2, caddr_t));
            m2->m_pkthdr.len = m2->m_len = m0->m_pkthdr.len;
            m_freem(m0);
            m0 = m2;

        }

        error = bus_dmamap_load_mbuf_sg(rswitch2->tx_buf_tag[queue],
            rswitch2->tx_buf_map[queue][idx].map,
            m0, segs, &nsegs, 0);
        /*
        * If bus_dmamap_load_mbuf_sg() wasn't successfull in mapping all
        * elements, we will first try to "collapse" the list,
        * and if collapse didn't help, we will try to defragment it.
        */
        if (error == EFBIG) {
            m_temp = m_collapse(m0, M_NOWAIT, TX_MAX_SEGS);
            if (m_temp != NULL) {
                m0 = m_temp;
            }
            error = bus_dmamap_load_mbuf_sg(rswitch2->tx_buf_tag[queue],
                rswitch2->tx_buf_map[queue][idx].map,
                m0, segs, &nsegs, 0);
            if (error == EFBIG) {
                m_temp = m_defrag(m0, M_NOWAIT);
                if (m_temp == NULL) {
                    device_printf(rswitch2->dev,"%s Defrag failed\n",__func__);
                    drbr_putback(rswitch2->ifp, rswitch2->br, m0);
                    break;
                }
                m0 = m_temp;
                error = bus_dmamap_load_mbuf_sg(rswitch2->tx_buf_tag[queue],
                    rswitch2->tx_buf_map[queue][idx].map,
                    m0, segs, &nsegs, 0);
                /*
                 * We try to remap only once.
                 * If we fail, we try defrag
                 * (which is time consuming).
                 * After that, bus_dmamap_load_mbuf_sg()
                 * should not return EFBIG error
                 */
                KASSERT((error != EFBIG),("%s: Too many segments after defrag",__func__));
            }
        }
        if (error != 0) {
            device_printf(rswitch2->dev,"%s an attempt to load mbuf failed\n",__func__);
            drbr_putback(rswitch2->ifp, rswitch2->br, m0);
            break;
        }

        KASSERT(nsegs == 1, ("%s: %s: many DMA segments",__func__device_get_nameunit(rswitch2->dev)));
        bus_dmamap_sync(rswitch2->tx_buf_tag[queue],rswitch2->tx_buf_map[queue][idx].map,
            BUS_DMASYNC_PREWRITE);
        BPF_MTAP(ifp, m0);
        drbr_advance(rswitch2->ifp, rswitch2->br);
        m_temp = m0;
        rswitch2->tx_buf_map[queue][idx].m = m0;

        /*
         * Prepare memory for the transfer from the hardware
         */
        bd = &rswitch2->tx_ring[queue][idx];
        bd->info_ds = m0->m_len;
        bd->dptrh = segs[0].ds_addr >> 32;
        bd->dptrl = (uint32_t)segs[0].ds_addr;

        /* FIXME : The following 'csd' setting is correct ? */
        if (rswitch2->gwca.index == 0)
            bd->csd0 = rswitch2->tx_queue_index + queue;
        else
            bd->csd1 = rswitch2->tx_queue_index + queue;

        bd->dv = BIT(rswitch2->etha.index);
        bd->info1 = BIT(2) | BIT(1);
        bd->die_dt = DT_FSINGLE;// | DIE;

        mb();

        if (rswitch2_ptp_is_eventmsg(m0, &ph)) {
            msg_type =  ph->messageId & 0x0f; 
            if ((queue < NUM_PTP_TX_QUEUES) && (msg_type < PTP_TX_MSGTYPE)) {
                sequence_id = ntohs(ph->sequenceId);
                tx_ph_idx = (msg_type * PTP_TX_BUF_LEN) + (sequence_id % PTP_TX_BUF_LEN);

                memcpy(&(tx_ph[queue][tx_ph_idx]), ph, sizeof(ptpv2hdr_t));
            } else
                device_printf(rswitch2->dev,
                    "rswitch2: Transmit PTP queue %d or msg type %d not supported", queue, msg_type);
        }

        mb();

        if (rswitch2->verbose & VERBOSE_TX) {
            device_printf(rswitch2->dev,"Tx queue %d index %d packet length %d\n", queue, idx, m0->m_len);
        }

        ++enqueued;
        rswitch2->tx_pidx[queue] = next_idx;

        /* Start transfer on device */
        tx_idx = queue + rswitch2->tx_queue_index;
        rswitch2_modify(rswitch2->res[0], gwca->addr_offset + GWTRC(tx_idx / 32), BIT(tx_idx % 32), BIT(tx_idx % 32));
    }

    if (!enqueued) {
        rswitch2->tx_watchdog_count = WATCHDOG_TIMEOUT_SECS;
    }

    if (!drbr_empty(rswitch2->ifp, rswitch2->br)) {
        taskqueue_enqueue(rswitch2->tq, &rswitch2->tx);
    }
    return error;
}

int rswitch2_transmit(struct ifnet *ifp, struct mbuf *m)
{
    rswitch2_dev_t *rswitch2 = ifp->if_softc;
    int error = 0;
   
    if (rswitch2->br == NULL) {
        error = EINVAL;
       return (error);
    }
    error = drbr_enqueue(ifp, rswitch2->br, m);
    if (error != 0) {
        device_printf(rswitch2->dev,"%s: drbr_enqueue failed \n",__func__);
        goto done;
    }
    
    if (mtx_trylock(&rswitch2->tx_mtx)) {
        error = rswitch2_txstart_locked(rswitch2, 0);
        mtx_unlock(&rswitch2->tx_mtx);
    } else {
        /* Wake up the task queue */
        taskqueue_enqueue(rswitch2->tq, &rswitch2->tx);
    }
    
done:
     return error;
}

void rswitch2_tq_tx(void *arg, int pending)
{
    rswitch2_dev_t *rswitch2 = arg;

    mtx_lock(&rswitch2->tx_mtx);
    if (!drbr_empty(rswitch2->ifp, rswitch2->br)) {
        rswitch2_txstart_locked(rswitch2, 0);
    }
    mtx_unlock(&rswitch2->tx_mtx);
}

void rswitch2_qflush(struct ifnet *ifp)
{
    rswitch2_dev_t *rswitch2 = ifp->if_softc;
    struct mbuf  *m;

    mtx_lock(&rswitch2->tx_mtx);
    while ((m = buf_ring_dequeue_sc(rswitch2->br)) != NULL) {
        m_freem(m);
    }
    mtx_unlock(&rswitch2->tx_mtx);

    if_qflush(ifp);
}
