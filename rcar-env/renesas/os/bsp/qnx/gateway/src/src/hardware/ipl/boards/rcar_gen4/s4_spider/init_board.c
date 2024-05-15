/*
 * $QNXLicenseC:
 * Copyright 2016, 2018 QNX Software Systems.
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

#include "ipl.h"
#include "rcar_ipl.h"
#include <hw/inout.h>
#include <sys/types.h>
#include <stdint.h>
#include <aarch64/gic_v3.h>
#include <aarch64/r-car.h>
#include "board_config.h"

/*
 * This code is board dependant and may have to be
 * changed by end users.
 */

ipl_uart_boot_t s4_uart_boot [] = {
     /*  SCIF0 @115200   */ [UART_USE_SCIF0]  = { 0xE6E60000,  115200,  66666666, 0 },
     /*  SCIF3 @115200   */ [UART_USE_SCIF3]  = { 0xE6C50000,  115200,  66666666, 0 },
     /*  HSCIF0 @1843200 */ [UART_USE_HSCIF0] = { 0xE6540000, 1843200, 266666666, 0 },
     /*  HSCIF1 @1843200 */ [UART_USE_HSCIF1] = { 0xE6550000, 1843200, 266666666, 0 },
};

void init_board(int uart_boot_idx)
{
    ipl_uart_boot_t *uart_boot_cfg = &s4_uart_boot[uart_boot_idx];

	/* Enable TMU0 for delay functions (udelay, nanospin) and SDHI */
	uint32_t val = in32(RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_MSTPCR(7)) & ~(1 << 13) & ~(1 << 6) ;
	out32(RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_CPGWPR, ~val);
	out32(RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_MSTPCR(7), val);
        
    /* Configure SYSC, PFC, CPG */
    soc_config(&s4_uart_config[uart_boot_idx], 1);
	soc_config(s4_config, 0);

    /* Configure uart device */
    init_serscif(uart_boot_cfg->base, uart_boot_cfg->baud, uart_boot_cfg->clk, uart_boot_cfg->extclk);
}
