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

/*
 * init_raminfo.c
 *  Tell syspage about our RAM configuration
 */
#include "startup.h"
#include <aarch64/r-car.h>

void init_raminfo()
{
    add_ram(RCAR_SDRAM_0, GIG(2));
    add_ram(RCAR_SDRAM_0_1, GIG(2));
    add_ram(RCAR_SDRAM_2, GIG(4));

    /* first 128MB is reserved for secure area. */
    alloc_ram(0x40000000, 0x8000000, 0);


    /* Reserving:
     * - From 0x5D000000 to 0x5F23FFFF for DSP firmware */
    alloc_ram(0x5D000000, 0x2240000, 0);

    /* Reserving:
     * - From 0x6000_0000, size 512MB for Zepher and MCAL (RTOS on CR core)
     * - From 0x8000_0000, size 560MBfor FCPR (OSAL) */
    alloc_ram(0x60000000, 0x43000000, 0);
}
