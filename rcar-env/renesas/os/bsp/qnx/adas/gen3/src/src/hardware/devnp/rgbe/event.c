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

void * gether_isr_thread(void *arg)
{
    struct sigevent		event;
    gether_dev_t		*gether;
    const struct sigevent	*evp;
    uint32_t        val;
    int rc;
    gether = arg;

    pthread_setname_np(0, "RGBE ISR");

    SIGEV_INTR_INIT(&event);

    gether->iid = InterruptAttachEvent_r(gether->cfg.irq[0], &event, _NTO_INTR_FLAGS_TRK_MSK);
    if (gether->iid < 0) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to attach to the interrupt: %s",
              strerror(errno));
        dev_remove(&gether->dev);
    }

    while (1) {
        InterruptWait(0, NULL);

        val = in32(gether->base + GETHER_EESR);

        if ((gether->ecom.ec_if.if_flags & IFF_RUNNING) == 0) {
            /* Interface down. Ignore it */
            out32(gether->base + GETHER_EESR, val);
            InterruptUnmask(gether->cfg.irq[0], gether->iid);
            continue;
        }

        if ((val & GETHER_EESR_FR) || (val & (GETHER_EESR_RABT | GETHER_EESR_RFOF))) {
            mask_rx_int(gether);
            gether->rx_eesr = val;
            evp = interrupt_queue(gether->iopkt, &gether->rx_ient);
            if (evp != NULL) {
                MsgSendPulsePtr(evp->sigev_coid, evp->sigev_priority,
                                evp->sigev_code,
                                evp->sigev_value.sival_ptr);
            }
        }

        if (((val & GETHER_EESR_TC0) && (val & GETHER_EESR_TC1)) ||
             (val & (GETHER_EESR_TABT | GETHER_EESR_TFUF))) {
            mask_tx_int(gether);
            gether->tx_eesr = val;
            evp = interrupt_queue(gether->iopkt, &gether->tx_ient);
            if (evp != NULL) {
                MsgSendPulsePtr(evp->sigev_coid, evp->sigev_priority,
                                evp->sigev_code,
                                evp->sigev_value.sival_ptr);
            }
        }

        out32(gether->base + GETHER_EESR, val);

        do {
               rc = InterruptUnmask(gether->cfg.irq[0], gether->iid);
           } while (rc > 0);
        if (rc == -1) {
             slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                   "gether: Failed to unmask interrupt: %s", strerror(errno));
        }
    }
    return NULL;
}

int gether_process_interrupt_rx (void *arg, struct nw_work_thread *wtp)
{
    gether_dev_t        *gether;

    gether = arg;

    if (gether->rx_eesr & GETHER_EESR_FR) {
       gether_receive(gether, wtp);
    }

    if (gether->rx_eesr & (GETHER_EESR_RABT | GETHER_EESR_RFOF)) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Error rx interrupt 0x%x", gether->rx_eesr);
    }

    gether->rx_eesr &= ~(gether->rx_eesr);

    return 1;
}

int gether_enable_interrupt_rx (void * arg)
{
    gether_dev_t        *gether;

    gether = arg;

    unmask_rx_int(gether);
    __cpu_membarrier();

    return 1;
}


int gether_process_interrupt_tx (void *arg, struct nw_work_thread *wtp)
{
    return 1;
}

int gether_enable_interrupt_tx (void * arg)
{
    gether_dev_t        *gether;
    struct ifnet    *ifp;
    struct nw_work_thread	*wtp;

    gether = arg;

    ifp = &gether->ecom.ec_if;
    wtp = WTP;

    if ((gether->tx_eesr & GETHER_EESR_TC0) && (gether->tx_eesr & GETHER_EESR_TC1)) {
        NW_SIGLOCK_P(&ifp->if_snd_ex, gether->iopkt, wtp);
        if (ifp->if_flags_tx & IFF_OACTIVE) {
           gether_start(ifp);
        } else {
           NW_SIGUNLOCK_P(&ifp->if_snd_ex, gether->iopkt, wtp);
        }
    }

    if (gether->tx_eesr & (GETHER_EESR_TABT | GETHER_EESR_TFUF)) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Error tx interrupt 0x%x", gether->tx_eesr);
    }

    gether->tx_eesr &= ~(gether->tx_eesr);

    unmask_tx_int(gether);
    __cpu_membarrier();

    return 1;
}

const struct sigevent * const *  gether_isr(void *arg, int iid)
{
    gether_dev_t        *gether;
    uint32_t        val;
    int i;

    gether = arg;

    for (i = 0; i < EVP_ARRAY_SIZE; i++)
        gether->evp[i] = NULL;
 
    val = in32(gether->base + GETHER_EESR);

    i = 0;
    if ((val & GETHER_EESR_FR) || (val & (GETHER_EESR_RABT | GETHER_EESR_RFOF))) {
       mask_rx_int(gether);
       gether->rx_eesr = val;
       gether->evp[i] = (struct sigevent *)interrupt_queue(gether->iopkt, &gether->rx_ient);
       if(gether->evp[i]) i++;
    }

    if (((val & GETHER_EESR_TC0) && (val & GETHER_EESR_TC1)) ||
         (val & (GETHER_EESR_TABT | GETHER_EESR_TFUF))) {
       mask_tx_int(gether);
       gether->tx_eesr = val;
       gether->evp[i] = (struct sigevent *)interrupt_queue(gether->iopkt, &gether->tx_ient);
       if(gether->evp[i]) i++;
    }

    out32(gether->base + GETHER_EESR, val);

    if (i > 0)
       return (const struct sigevent * const *) gether->evp;
    else
        return NULL;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/event.c $ $Rev: 878393 $")
#endif
