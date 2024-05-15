/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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


#include "startup.h"
#include "rcar_startup.h"
#include "aarch64/gic.h"
#include <aarch64/r-car.h>


static uintptr_t rcar_gpio_vbase[9];

/*
 * Initialize interrupt controller hardware & intrinfo structure in the
 * system page.
 * This code is hardware dependant and may have to be changed
 * changed by end users.
 */

const static struct startup_intrinfo gpio_intrs[] =
{
    // GPIO0 interrupt
    {   992,                            // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_619,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[0]
    },
    // GPIO1 interrupt
    {   992 + 32,                       // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_623,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[1]
    },
    // GPIO2 interrupt
    {   992 + 32*2,                     // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_627,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[2]
    },
    // GPIO3 interrupt
    {   992 + 32*3,                     // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_631,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[3]
    },
    // GPIO4 interrupt
    {   992 + 32*4,                     // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_635,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[4]
    },
    // GPIO5 interrupt
    {   992 + 32*5,                     // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_639,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[5]
    },
    // GPIO6 interrupt
    {   992 + 32*6,                     // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_643,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[6]
    },
    // GPIO7 interrupt
    {   992 + 32*7,                     // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_647,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[7]
    },
    // GPIO8 interrupt
    {   992 + 32*8,                     // vector base
        32,                             // number of vectors
        RCAR_GEN4_INTCSYS_SPI_651,       // cascade vector
        0,                              // CPU vector base
        0,                              // CPU vector stride
        0,                              // flags
        { 0, 0, &interrupt_id_rcar_gpio},
        { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_rcar_gpio},
        &interrupt_mask_rcar_gpio,      // mask callout
        &interrupt_unmask_rcar_gpio,    // unmask callout
        0,                              // config callout
        &rcar_gpio_vbase[8]
    },
};

void init_intrinfo()
{
    gic_v3_set_paddr(RCAR_GEN4_GICD_BASE, RCAR_GEN4_GICR_LPI_BASE, NULL_PADDR);

    gic_v3_initialize();

    /* Interrupt enable clear */
    out32(RCAR_IRQC0_BASE + RCAR_INTEN_STS0, 0x0000003F);

    /* Clear IRQC interrupts */
    out32(RCAR_IRQC0_BASE + RCAR_DETECT_STATUS, 0x0000003F);

    /* All interrupts high-level */
    for (int i = 0; i < 6; ++i) {
        out32(RCAR_IRQC0_BASE + RCAR_CONFIG_00 + i*4, (in32(RCAR_IRQC0_BASE + RCAR_CONFIG_00 + i*4) & ~0x3F) | 0x2);
    }

    /* Interrupt enable clear */
    out32(RCAR_NMI_BASE + RCAR_INTEN_STS0, 0x000000FF);

    /* Clear IRQC interrupts */
    out32(RCAR_NMI_BASE + RCAR_DETECT_STATUS, 0x000000FF);

    /* All interrupts high-level */
    for (int i = 0; i < 8; ++i) {
        out32(RCAR_NMI_BASE + RCAR_CONFIG_00 + i*4, (in32(RCAR_NMI_BASE + RCAR_CONFIG_00 + i*4) & ~0x3F) | 0x2);
    }

    /* Enable interrupts */
    out32(RCAR_NMI_BASE + RCAR_INTEN_SET0, 0x000000FF);

    rcar_gpio_vbase[0] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP0);
    rcar_gpio_vbase[1] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP1);
    rcar_gpio_vbase[2] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP2);
    rcar_gpio_vbase[3] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP3);
    rcar_gpio_vbase[4] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP4);
    rcar_gpio_vbase[5] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP5);
    rcar_gpio_vbase[6] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP6);
    rcar_gpio_vbase[7] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP7);
    rcar_gpio_vbase[8] = callout_io_map(0x200, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP8);

    add_interrupt_array(gpio_intrs, sizeof(gpio_intrs));
}
