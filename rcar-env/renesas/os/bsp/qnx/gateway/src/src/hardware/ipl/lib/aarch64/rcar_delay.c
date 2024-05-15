/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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


#include <hw/inout.h>
#include <aarch64/r-car.h>
#include "ipl.h"
#include "private/rcar_delay.h"

void rcar_usec_delay(uint32_t usec)
{
    uint32_t tmu_base = RCAR_TMU_0_2_BASE;
	uint8_t start_reg;

	/* Stop TMU0 before changing settings */
	start_reg = in8(tmu_base + RCAR_TMU_TSTR);
	out8(tmu_base + RCAR_TMU_TSTR, start_reg & ~RCAR_TMU_START0);

	out16(tmu_base + RCAR_TMU_TCR0, 0);
	out32(tmu_base + RCAR_TMU_TCOR0, usec);
	out32(tmu_base + RCAR_TMU_TCNT0, usec);

	/* Start TMU0 */
	out8(tmu_base + RCAR_TMU_TSTR, start_reg | RCAR_TMU_START0);

	/* Check the countdown is 0 */
	while (!(in16(tmu_base + RCAR_TMU_TCR0) & RCAR_TMU_UNF));

	/* stop count */
	out8(tmu_base + RCAR_TMU_TSTR, start_reg & ~RCAR_TMU_START0);
}

