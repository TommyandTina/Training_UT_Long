/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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
 * RCar-V3U specific WDT support.
 * RWDT is used.
 */

#include "startup.h"
#include <aarch64/r-car-gen3.h>
#include <hw/rcar_cpg_support.h>

int wdt_enable()
{
	/* Supply clock to WDT */
	cpg_support_funcs.mstp_config(CPG_MSTP_V3U_RWDT, 1);

	/* Enable Generating internal reset when RWDT overflow */
	out32(RCAR_V3U_RST_DM0_BASE + RCAR_V3U_RST_WDTRSTCR, (0xA55A << 16) | 0x1000);

	/* for RWDT */
	out32(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRA, (0xA5A5A5 << 8) | (in8(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRA) & ~(1 << 7))); // stop timer
	out32(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CNT,  0x5A5A0000); // reset counter
	out32(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRA, (0xA5A5A5 << 8) | (in8(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRA) & ~(1 << 4)) | 0x7);
	out32(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRB, (0xA5A5A5 << 8) | 0x21); // overflow after 1 minute
	while (in8(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRA) & (1 << 5));

	out32(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRA, (0xA5A5A5 << 8) | (in8(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CSRA) | (1 << 7)));  // start RWDT
	out32(RCAR_GEN3_RWDT_BASE + RCAR_GEN3_WDT_CNT, (0x5A5A << 16) | 0xff00); // write first value to WDT

	return 1;
}

