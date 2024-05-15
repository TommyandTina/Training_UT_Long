/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation.
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
#include <aarch64/r-car-gen3.h>

uint8_t detect_ram = 0;

extern uint32_t rcar_detect_ram(uint32_t base);

void init_raminfo()
{
    add_ram(RCAR_GEN3_SDRAM_0, GIG(2));
    add_ram(RCAR_GEN3_SDRAM_1, GIG(2));
    add_ram(RCAR_GEN3_SDRAM_2, GIG(2));
    add_ram(RCAR_GEN3_SDRAM_3, GIG(2));

    /* first 128MB is reserved for secure area. */
    alloc_ram(0x40000000, 0x4800000, 0);
}

