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

static void rtsnes_ptp_interrupt (rtsnes_dev_t *rtsnes)
{
    return;
}

int rtsnes_axibmi_process_interrupt_rx (void *arg, struct nw_work_thread *wtp)
{
    rtsnes_dev_t  *rtsnes = arg;
    int            i;

    for (i = 0; i < NUM_RX_QUEUES; i++) {
        rtsnes_receive(rtsnes, wtp, i); // For queue i
    }

    return 1;
}

int rtsnes_axibmi_process_interrupt_tx (void *arg, struct nw_work_thread *wtp)
{
    rtsnes_dev_t  *rtsnes = arg;

    rtsnes_ptp_add_tx_timestamp(rtsnes);
    rtsnes_ptp_interrupt(rtsnes);

    return 1;
}

int rtsnes_axibmi_enable_interrupt(rtsnes_dev_t *rtsnes, int tx, int enable)
{        
    /* Only use one TX/RX chain */
    if (enable) {
        if (tx)
            rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TDIE(0), rtsnes->tx_irq);
        else
            rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RDIE(0), rtsnes->rx_irq);
    } else {
        if (tx)
            rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TDID(0), rtsnes->tx_irq);
        else
            rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RDID(0), rtsnes->rx_irq);
    }

    return 0;
}

int rtsnes_axibmi_enable_interrupt_tx (void *arg)
{
    rtsnes_dev_t *rtsnes = (rtsnes_dev_t *) arg;

    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TDIE(0), rtsnes->tx_irq);
    return 0;
}

int rtsnes_axibmi_enable_interrupt_rx (void * arg)
{
    rtsnes_dev_t *rtsnes = (rtsnes_dev_t *) arg;

    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RDIE(0), rtsnes->rx_irq);
    return 0;
}

const struct sigevent * const * rtsnes_axibmi_isr (void *arg, int iid)
{
    rtsnes_dev_t    *rtsnes = arg;
    int             i = 0, j;

    for (j = 0; j < EVP_ARRAY_SIZE; j++)
        rtsnes->evp[j] = NULL;

    /* Get rx status interrupt */
    rtsnes->rx_dis = rtsnes_axibmi_read(rtsnes, TSNAXIBMI_RDIS(0)) | rtsnes->rx_irq;

    /* Clear status interrupt */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RDIS(0), rtsnes->rx_dis);

    if (rtsnes->rx_dis) {
        /* Mask interrupt */
        rtsnes_axibmi_enable_interrupt(rtsnes, 0, 0);
        rtsnes->evp[i] = (struct sigevent *)interrupt_queue(rtsnes->iopkt, &rtsnes->ient_rx);
        if(rtsnes->evp[i]) i++;
    }

    /* Get tx status interrupt */
    rtsnes->tx_dis = rtsnes_axibmi_read(rtsnes, TSNAXIBMI_TDIS(0)) | rtsnes->tx_irq;

    /* Clear status interrupt */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TDIS(0), rtsnes->tx_dis);

    if (rtsnes->tx_dis) {
        /* Mask interrupt */
        rtsnes_axibmi_enable_interrupt(rtsnes, 1, 0);
        rtsnes->evp[i] = (struct sigevent *)interrupt_queue(rtsnes->iopkt, &rtsnes->ient_tx);
        if(rtsnes->evp[i]) i++;
    }

    if (i > 0) {
       return (const struct sigevent * const *) rtsnes->evp;
    } else {
        return NULL;
    }
}

const struct sigevent * rtsnes_axibmi_tx_isr (void *arg, int iid)
{
    rtsnes_dev_t    *rtsnes = arg;

    /* Get status interrupt */
    rtsnes->tx_dis = rtsnes_axibmi_read(rtsnes, TSNAXIBMI_TDIS(0));

    /* Clear status interrupt */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TDIS(0), rtsnes->tx_dis);

    /* Mask interrupt */
    rtsnes_axibmi_enable_interrupt(rtsnes, 1, 0);

    return interrupt_queue(rtsnes->iopkt, &rtsnes->ient_tx);
}

const struct sigevent * rtsnes_axibmi_rx_isr (void *arg, int iid)
{
    rtsnes_dev_t    *rtsnes = arg;

    /* Get status interrupt */
    rtsnes->rx_dis = rtsnes_axibmi_read(rtsnes, TSNAXIBMI_RDIS(0));

    /* Clear status interrupt */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RDIS(0), rtsnes->rx_dis);

    /* Mask interrupt */
    rtsnes_axibmi_enable_interrupt(rtsnes, 0, 0);

    return interrupt_queue(rtsnes->iopkt, &rtsnes->ient_rx);
}

const struct sigevent * rtsnes_axibmi_isr_avb (void *arg, int iid)
{
    rtsnes_dev_t    *rtsnes = arg;

    InterruptLock(&rtsnes->spinlock);

    /* For RX queue */
    rtsnes->rx_dis = rtsnes_axibmi_read(rtsnes, TSNAXIBMI_RDIS(0));
    if (rtsnes->rx_dis) {
        InterruptUnlock(&rtsnes->spinlock);
        //Clear status interrupt
        rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RDIS(0), rtsnes->rx_dis);
        return &rtsnes->isr_event[0]; 
    }

    /* For TX queue */
    rtsnes->tx_dis = rtsnes_axibmi_read(rtsnes, TSNAXIBMI_TDIS(0));
    //Clear status interrupt
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TDIS(0), rtsnes->tx_dis);
    if (rtsnes->tx_dis) {
        InterruptUnlock(&rtsnes->spinlock);
        return interrupt_queue(rtsnes->iopkt, &rtsnes->ient_tx);
    }

    InterruptUnlock(&rtsnes->spinlock);
    return NULL;
}

int rtsnes_process_queue(void *arg, struct nw_work_thread *wtp)
{
    rtsnes_dev_t  *rtsnes = arg;
    struct ifnet    *ifp = &rtsnes->ecom.ec_if;
    struct mbuf     *m;

    while (1) {
        pthread_mutex_lock(&rtsnes->rx_mutex);
        IF_DEQUEUE(&rtsnes->rx_queue, m);
        if (m != NULL) {
            pthread_mutex_unlock(&rtsnes->rx_mutex);
            (*ifp->if_input)(ifp, m);
        } else {
            /* Leave mutex locked to prevent any enqueues, unlock in enable */
            break;
        }
    }

    return 1;
}

int rtsnes_enable_queue(void *arg)
{
    rtsnes_dev_t *rtsnes = arg;
    struct ifnet  *ifp = &rtsnes->ecom.ec_if;

    InterruptLock(&rtsnes->spinlock);
    if (ifp->if_flags & IFF_RUNNING) {
        rtsnes_axibmi_enable_interrupt(rtsnes, 0, 1);
        rtsnes_axibmi_enable_interrupt(rtsnes, 1, 1);
    }
    InterruptUnlock(&rtsnes->spinlock);
    rtsnes->rx_running = 0;
    pthread_mutex_unlock(&rtsnes->rx_mutex);

    return 1;
}

const struct sigevent * rtsnes_rmac_isr (void *arg, int iid)
{
    return EOK;
}

