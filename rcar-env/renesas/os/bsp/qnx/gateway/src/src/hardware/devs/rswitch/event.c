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

void rswitch2_ptp_interrupt (rswitch2_dev_t *rswitch2)
{
}

static void rswitch2_gwca_process_interrupt_rx (rswitch2_dev_t  *rswitch2)
{
    for (int i = 0; i < NUM_RX_QUEUES; i++) {
        mtx_lock(&rswitch2->mtx);
        rswitch2_rxfinish_locked(rswitch2, i); // For queue i
        mtx_unlock(&rswitch2->mtx);
    }
}

static void rswitch2_gwca_process_interrupt_tx (rswitch2_dev_t  *rswitch2)
{
    rswitch2_ptp_add_tx_timestamp(rswitch2);
    rswitch2_ptp_interrupt(rswitch2);
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
            mask |= bus_read_4(rswitch2->res[0], gwca->addr_offset + offs + (i * 0x10));
        }
        mask |= irq[i];
        bus_write_4(rswitch2->res[0], gwca->addr_offset + offs + (i * 0x10), mask);
    }

    return 0;
}

void rswitch2_gwca_isr (void *arg)
{
    rswitch2_dev_t  *rswitch2 = arg;
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             j;
    uint32_t        mask;

    for (j = 0; j < RSWITCH2_IRQ_REG_NUM; j++) {
        /* Get status interrupt */
        rswitch2->dis[j] = bus_read_4(rswitch2->res[0], gwca->addr_offset + GWDIS + (j * 0x10));
        /* Check tx/rx interrupts */
        if (rswitch2->rx_irq_mask[j] & rswitch2->dis[j]) {
            /* Mask interrupt */
            //rswitch2_enadis_data_irq(rswitch2, 0, false);

            /* Process interrupt */
            rswitch2_gwca_process_interrupt_rx(rswitch2);

            /* Clear interrupt */
            mask = rswitch2->dis[j] & rswitch2->rx_irq_mask[j];
            bus_write_4(rswitch2->res[0], gwca->addr_offset + GWDIS + (j * 0x10), mask);
 
            /* Unask interrupt */
            //rswitch2_enadis_data_irq(rswitch2, 0, true);
        }
        if (rswitch2->tx_irq_mask[j] & rswitch2->dis[j]) {
            /* Mask interrupt */
            //rswitch2_enadis_data_irq(rswitch2, 1, false);

            /* Process interrupt */
            rswitch2_gwca_process_interrupt_tx(rswitch2);

            /* Clear interrupt */
            mask = rswitch2->dis[j] & rswitch2->tx_irq_mask[j];
            bus_write_4(rswitch2->res[0], gwca->addr_offset + GWDIS + (j * 0x10), mask);

            /* Unmask interrupt */
            //rswitch2_enadis_data_irq(rswitch2, 1, true);
            
        }
    }
}
