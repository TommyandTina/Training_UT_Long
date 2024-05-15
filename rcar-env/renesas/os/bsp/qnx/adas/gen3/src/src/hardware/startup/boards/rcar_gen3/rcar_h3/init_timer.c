/*
* $QNXLicenseC:
* Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
* Copyright 2021, Renesas Electronics Corporation.
*
* You must obtain a written license from and pay applicable license fees to QNX
* Software Systems before you may reproduce, modify or distribute this software,
* or any work that includes all or part of this software. Free development
* licenses are available for evaluation and non-commercial purposes. For more
* information visit http://licensing.qnx.com or email licensing@qnx.com.
*
* This file may contain contributions from others. Please review this entire
* file for other proprietary rights or license notices, as well as the QNX
* Development Suite License Guide at http://licensing.qnx.com/license-guide/
* for other information.
* $
*/

#include "startup.h"
#include <aarch64/r-car-gen3.h>
#include "rcar_startup.h"
#include "init_board_type.h"

void
init_timer(void) {
    if(board_type == BOARD_SALVATOR_XS) {
        // Xtal = 16.64 MHz
        board_ext_clk = RCAR_GEN3_EXT_CLK_XS;
    } else {
        // Xtal = 16.6666 MHz
        board_ext_clk = RCAR_GEN3_EXT_CLK;
    }
    timer_freq = board_ext_clk / 2;
    if(debug_flag) {
        kprintf("%s board_ext_clk=%d\n", __FUNCTION__, board_ext_clk);
    }

    if (cpu_freq) {
        cpu_freq_set(cpu_freq);
    }

    cpu_freq = cpu_freq_get(0); // 0 to get the freq of the current cpu
}
