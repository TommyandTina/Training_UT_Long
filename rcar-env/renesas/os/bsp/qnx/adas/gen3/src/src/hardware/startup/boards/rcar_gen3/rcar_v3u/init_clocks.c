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

#include "startup.h"
#include "rcar_startup.h"
#include <hw/rcar_cpg.h>
#include <aarch64/r-car-gen3.h>

unsigned board_ext_clk;

/*
 * Get EXTAL clock, init CPG clock and SYSC power
 */
void init_board_clock()
{
    switch ((in32(RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR) >> 13) & 3) {
		case 0: board_ext_clk = RCAR_GEN3_EXT_CLK_16MHZ; break;
		case 1: board_ext_clk = RCAR_GEN3_EXT_CLK_20MHZ; break;
		case 2: board_ext_clk = RCAR_GEN3_EXT_CLK_25MHZ; break;
		case 3: board_ext_clk = RCAR_GEN3_EXT_CLK_33MHZ / 2; break;
	}

    if(debug_flag) {
		kprintf("%s board_ext_clk=%d\n", __FUNCTION__, board_ext_clk);
	}

	/* init CPG lib */
	const cpg_opts_t cpg_opts = {
		.base    = RCAR_V3U_CPG_DM0_BASE,
		.size    = RCAR_V3U_CPG_SIZE,
		.ext_clk = board_ext_clk,
	};
	cpg_support_funcs.init(&cpg_opts, NULL);

	/* init SYSC lib */
	const sysc_opts_t sysc_opts = {
		.base    = RCAR_V3U_SYSC_DM0_BASE,
		.size    = RCAR_V3U_SYSC_SIZE,
	};
	sysc_support_funcs.init(&sysc_opts);

   /* Enable TMU0 for delay functions (udelay, nanospin) */
	cpg_support_funcs.mstp_config(CPG_MSTP_V3U_TMU0, 1);
}

/*
 * Get CPU frequency
 */
uint32_t cpu_freq_get(uint32_t midr_cpu_id) 
{
    uint32_t freq = 0;
    
    midr_cpu_id = rcar_soc_id;

    if (cpg_support_funcs.clk_rate(CPG_CLK_CPU, 0, &freq, 0))
        kprintf("%s: Warning: Unknown CPU ID %x. Set cpu_freq = 0\n", __func__, midr_cpu_id);

    return freq;
}

/*
 * Set CPU frequency
 */
void cpu_freq_set(uint32_t cpufreq) 
{
}

