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

static void rswitch2_ptp_interrupt (rswitch2_dev_t *rswitch2)
{
}

static void rswitch2_qwca_complete_tx (rswitch2_dev_t *rswitch2, uint8_t queue)
{
    int idx;

    idx = rswitch2->tx_pidx[queue] - 1;
    if (idx < 0) idx = NUM_TX_DESC - 1;
    if ((rswitch2->tx_bd[queue][idx].die_dt & DT_MASK) == DT_FEMPTY) {
        m_freem(rswitch2->tx_pkts[queue][idx]);
        rswitch2->tx_pkts[queue][idx] = NULL;
    }
}

int rswitch2_gwca_process_interrupt_rx (void *arg, struct nw_work_thread *wtp)
{
    rswitch2_dev_t  *rswitch2 = arg;
    int              i;

    for (i = 0; i < NUM_RX_QUEUES; i++) {
        rswitch2_receive(rswitch2, wtp, i); // For queue i
    }

    return 1;
}

int rswitch2_gwca_process_interrupt_tx (void *arg, struct nw_work_thread *wtp)
{
    rswitch2_dev_t  *rswitch2 = arg;
    int              i;

    for (i = 0; i < NUM_TX_QUEUES; i++) {
        rswitch2_qwca_complete_tx(rswitch2, i); // For queue i
    }

    rswitch2_ptp_add_tx_timestamp(rswitch2);
    rswitch2_ptp_interrupt(rswitch2);

    return 1;
}

int rswitch2_enadis_data_irq(rswitch2_dev_t *rswitch2, int tx, bool enable)
{
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
	uint32_t        offs = (enable ? GWDIE : GWDID);
    uint32_t        *irq = (tx ? &rswitch2->tx_irq_mask[0] : &rswitch2->rx_irq_mask[0]);
    int             i = 0;
    uint32_t        mask = 0;

    for (i = 0; i < RSWITCH2_IRQ_REG_NUM; i++) {
        mask = 0;
        if (enable) {
            mask = in32(gwca->addr + offs + (i * 0x10));
        }
        mask |= irq[i];
        out32(gwca->addr + offs + (i * 0x10), mask);
    }

    return 0;
}


int rswitch2_gwca_enable_interrupt_rx (void * arg)
{
    rswitch2_dev_t *rswitch2 = arg;

    rswitch2_enadis_data_irq(rswitch2, 0, true);

    return 0;
}

int rswitch2_gwca_enable_interrupt_tx (void *arg)
{
    rswitch2_dev_t *rswitch2 = arg;

    rswitch2_enadis_data_irq(rswitch2, 1, true);

    return 0;
}

const struct sigevent * const * rswitch2_gwca_isr (void *arg, int iid)
{
    rswitch2_dev_t  *rswitch2 = arg;
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             i = 0, j;

    for (j = 0; j < EVP_ARRAY_SIZE; j++)
        rswitch2->evp[j] = NULL;

    for (j = 0; j < RSWITCH2_IRQ_REG_NUM; j++) {
        /* Get status interrupt */
        rswitch2->dis[j] = in32(gwca->addr + GWDIS + (j * 0x10));
        /* Clear innterrupt */
        out32(gwca->addr + GWDIS + (j * 0x10), rswitch2->dis[j] & (rswitch2->tx_irq_mask[j] | rswitch2->rx_irq_mask[j]));
    }

    for (j = 0; j < RSWITCH2_IRQ_REG_NUM; j++) {
        /* Check rx interrupts */
        if (rswitch2->rx_irq_mask[j] & rswitch2->dis[j]) {
            rswitch2->evp[i] = (struct sigevent *)interrupt_queue(rswitch2->iopkt, &rswitch2->ient_rx);
            if(rswitch2->evp[i]) {
                i++;
                /* Mask interrupt */
                rswitch2_enadis_data_irq(rswitch2, 0, false);
                break;
            }
        }
    }

    for (j = 0; j < RSWITCH2_IRQ_REG_NUM; j++) {
        /* Check tx interrupts */
        if (rswitch2->tx_irq_mask[j] & rswitch2->dis[j]) {
            rswitch2->evp[i] = (struct sigevent *)interrupt_queue(rswitch2->iopkt, &rswitch2->ient_tx);
            if(rswitch2->evp[i]) {
                i++;
                /* Mask interrupt */
                rswitch2_enadis_data_irq(rswitch2, 1, false);
                break;
            }
        }
    }
            
    if (i > 0) {
       return (const struct sigevent * const *) rswitch2->evp;
    } else {
        return NULL;
    }
}

const struct sigevent * rswitch2_gwca_isr_avb (void *arg, int iid)
{
    rswitch2_dev_t    *rswitch2 = arg;
    rswitch2_gwca_t   *gwca = &rswitch2->gwca;
    int               j, i;
    uint32_t          mask;

    InterruptLock(&rswitch2->spinlock);

    for (j = 0; j < RSWITCH2_IRQ_REG_NUM; j++) {
        rswitch2->dis[j] = in32(gwca->addr + GWDIS + (j * 0x10));
        /* For RX queue */
        for (i = 0; i < NUM_RX_QUEUES; i++) {
            mask = rswitch2->rx_irq_mask[j] & rswitch2->dis[j] & BIT((rswitch2->rx_queue_index + i) % 32) ;
            if (mask) {
                out32(gwca->addr + GWDIS + (j * 0x10), mask);
                InterruptUnlock(&rswitch2->spinlock);
                return &rswitch2->isr_event[i];
            }
        }

        /* For TX queue */
        for (i = 0; i < NUM_TX_QUEUES; i++) {
            mask = rswitch2->tx_irq_mask[j] & rswitch2->dis[j] & BIT((rswitch2->tx_queue_index + i) % 32) ;
            if (mask) {
                out32(gwca->addr + GWDIS + (j * 0x10), mask);
                InterruptUnlock(&rswitch2->spinlock);
                return interrupt_queue(rswitch2->iopkt, &rswitch2->ient_tx);
            }
        }
    }

    InterruptUnlock(&rswitch2->spinlock);
    return NULL;
}

int rswitch2_process_queue(void *arg, struct nw_work_thread *wtp)
{
    rswitch2_dev_t  *rswitch2 = arg;
    struct ifnet    *ifp = &rswitch2->ecom.ec_if;
    struct mbuf     *m;

    while (1) {
        pthread_mutex_lock(&rswitch2->rx_mutex);
        IF_DEQUEUE(&rswitch2->rx_queue, m);
        if (m != NULL) {
            pthread_mutex_unlock(&rswitch2->rx_mutex);
            (*ifp->if_input)(ifp, m);
        } else {
            /* Leave mutex locked to prevent any enqueues, unlock in enable */
            break;
        }
    }

    return 1;
}

int rswitch2_enable_queue(void *arg)
{
    rswitch2_dev_t *rswitch2 = arg;
    struct ifnet  *ifp = &rswitch2->ecom.ec_if;

    InterruptLock(&rswitch2->spinlock);
    if (ifp->if_flags & IFF_RUNNING) {
        //rswitch2_enadis_data_irq(rswitch2, 0, true);
    }
    InterruptUnlock(&rswitch2->spinlock);
    rswitch2->rx_running = 0;
    pthread_mutex_unlock(&rswitch2->rx_mutex);

    return 1;
}

const struct sigevent * rswitch2_etha_isr (void *arg, int iid)
{
    return EOK;
}
