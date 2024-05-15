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

#include "ravb.h"


static void ravb_ptp_interrupt (ravb_dev_t *ravb)
{
    uint32_t gis = in32(ravb->base + GIS);

    gis &= in32(ravb->base + GIC);
    out32(ravb->base + GIS, gis & ~(GIS_PTCF | GIS_PTMF0));
    out32(ravb->base + GIE, GIE_PTMS0);
}

/* Error interrupt handler */
static void ravb_error_interrupt(ravb_dev_t *ravb)
{
    uint32_t eis, ris2;

    eis = in32(ravb->base + EIS);
    out32(ravb->base + EIS, ~(EIS_QFS | EIS_RESERVED));
    if (eis & EIS_QFS) {
        ris2 = in32(ravb->base + RIS2);
        out32(ravb->base + RIS2, ~(RIS2_QFF0 | RIS2_QFF1 | RIS2_RFFF | RIS2_RESERVED));

        /* Receive FIFO Overflow/Receive Descriptor Empty int */
        if (ris2 & (RIS2_QFF0 | RIS2_QFF1 | RIS2_RFFF)) {
            ravb->stats.un.estats.internal_rx_errors++;
        }
    }
}

int ravb_process_interrupt_rx (void *arg, struct nw_work_thread *wtp)
{
    ravb_dev_t  *ravb;

    ravb = arg;

    /* Clear status */
    out32(ravb->base + RIS0, ~(RIS0_FRF0 | RIS0_FRF1 | RIS0_RESERVED));

    /* Received interrupts */
    if (ravb->ris0 & RIS0_FRF0) {
        ravb_receive(ravb, wtp, 0);
    }

    if (ravb->ris0 & RIS0_FRF1) {
        ravb_receive(ravb, wtp, 1);
    }

    return 1;
}

int ravb_dmac_enable_interrupt_rx (void * arg)
{
    ravb_dev_t  *ravb = arg;

    unmask_rx_int(ravb);

    return 0;
}

int ravb_process_interrupt_tx (void *arg, struct nw_work_thread *wtp)
{
    ravb_dev_t      *ravb;
    uint32_t        tis;
    struct ifnet    *ifp;

    ravb = arg;
    ifp = &ravb->ecom.ec_if;

    tis  = in32(ravb->base + TIS);
    
    /* Clear status */
    out32(ravb->base + TIS, tis & ~(TIS_FTF0 | TIS_FTF1 | TIS_TFUF | TIS_TFWF));

    /* TimeStamp Updated Interrupt */
    if (tis & (TIS_TFUF | TIS_TFWF)) {
        ravb_ptp_add_tx_timestamp(ravb);
    }

    ravb_ptp_interrupt(ravb);

    /* Transmitted interrupts */
    if (tis & (TIS_FTF0)) {
        if (ifp->if_flags_tx & IFF_OACTIVE) {
            NW_SIGLOCK(&ifp->if_snd_ex, ravb->options.iopkt);
            ravb_start(ifp);
        }
    }

    return 1;
}

int ravb_dmac_enable_interrupt_tx(void *arg)
{
    ravb_dev_t   *ravb = arg;

    unmask_tx_int(ravb);

    return 0;
}

const struct sigevent * ravb_tx_isr (void *arg, int iid)
{
    ravb_dev_t *ravb = arg;
    uint32_t tis = in32(ravb->base + TIS);
    uint32_t tic = in32(ravb->base + TIC);
    
    /* Mask TX interrupts */
    out32(ravb->base + TID, tis & tic);

    return interrupt_queue(ravb->options.iopkt, &ravb->ient_tx);
}

const struct sigevent * ravb_rx_isr (void *arg, int iid)
{
    ravb_dev_t *ravb = arg;

    ravb->ris0 = in32(ravb->base + RIS0) & in32(ravb->base + RIC0);

    if (ravb->options.avb) {
        if (ravb->ris0 & RIS0_FRF1) {
            out32(ravb->base + RIS0, ravb->ris0 & ~RIS0_FRF1);
            return &ravb->isr_event[1];
        }
        else if (ravb->ris0 & RIS0_FRF0) {
            out32(ravb->base + RIS0, ravb->ris0 & ~RIS0_FRF0);
            return &ravb->isr_event[0];
        }
    }
    else {
        /* Mask RX interrupts */
        out32(ravb->base + RID0, ravb->ris0);
        return interrupt_queue(ravb->options.iopkt, &ravb->ient_rx);
    }

    return NULL;
}

const struct sigevent * ravb_err_ptp_isr (void *arg, int iid)
{
    ravb_dev_t *ravb = arg;
    uint32_t   iss;

    iss = in32(ravb->base + ISS);

    if (iss & ISS_ES) {
        ravb_error_interrupt(ravb);
    }

    if (iss & (ISS_CGIS)) {
        uint32_t gis  = in32(ravb->base + GIS);
        uint32_t gic  = in32(ravb->base + GIC);
    
        /* Mask interrupts */
        out32(ravb->base + GID, gis & gic);
        return interrupt_queue(ravb->options.iopkt, &ravb->ient_tx);
    }

    return NULL;
}

const struct sigevent * ravb_emac_isr (void *arg, int iid)
{
    ravb_dev_t  *ravb;
    uint32_t    ecsr, iss;
    ravb = arg;

    /* Get interrupt status */
    iss = in32(ravb->base + ISS);
    ecsr = in32(ravb->base + ECSR);
    out32(ravb->base + ECSR, ecsr); /* clear interrupt */

    if (ecsr & ECSR_ICD) {
		ravb->stats.un.estats.no_carrier++;
    }
    
    if ((iss & ISS_MS) && (ecsr & ECSR_LCHNG)) {
        /* Link changed */
    }

    return NULL;
}

int ravb_process_queue(void *arg, struct nw_work_thread *wtp)
{
    ravb_dev_t      *ravb = arg;
    struct ifnet    *ifp = &ravb->ecom.ec_if;
    struct mbuf     *m;

    while(1) {
        pthread_mutex_lock(&ravb->rx_mutex);
        IF_DEQUEUE(&ravb->rx_queue, m);
        if (m != NULL) {
            pthread_mutex_unlock(&ravb->rx_mutex);
            (*ifp->if_input)(ifp, m);
        } else {
            /* Leave mutex locked to prevent any enqueues, unlock in enable */
            break;
        }
    }
    return 1;
}

int ravb_enable_queue(void *arg)
{
    ravb_dev_t          *ravb = arg;
    struct ifnet        *ifp = &ravb->ecom.ec_if;

    InterruptLock(&ravb->spinlock);
    if (ifp->if_flags & IFF_RUNNING) {
        unmask_rx_int (ravb);
    }
    InterruptUnlock(&ravb->spinlock);
    ravb->rx_running = 0;
    pthread_mutex_unlock(&ravb->rx_mutex);

  return 1;
}
