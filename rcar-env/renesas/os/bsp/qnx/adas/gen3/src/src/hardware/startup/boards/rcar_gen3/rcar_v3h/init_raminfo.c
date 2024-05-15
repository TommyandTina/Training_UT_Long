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

/*
 * init_raminfo.c
 *  Tell syspage about our RAM configuration
 */
#include "startup.h"
#include "aarch64/r-car-gen3.h"
#include <rcar_board.h>

extern void rcar_add_ram(rcar_ram *ram, uint8_t detect_ram);

uint8_t detect_ram = 0;

void init_raminfo()
{
    rcar_ram ram[] = {
      {.base=RCAR_GEN3_SDRAM_0, .rank={RCAR_GEN3_DBSC4_DBMEMCONF_0_0, RCAR_GEN3_DBSC4_DBMEMCONF_0_1}, .size=GIG(2)},
      { 0 }
    };

    rcar_add_ram(ram, detect_ram);

    // Remove Region (0x40000000-0x47FFFFFF) for CR7 TP
    alloc_ram(0x40000000, 0x8000000, 0);
}

