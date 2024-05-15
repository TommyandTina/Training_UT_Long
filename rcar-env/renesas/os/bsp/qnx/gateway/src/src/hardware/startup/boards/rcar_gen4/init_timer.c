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
#include <aarch64/r-car.h>
#include "rcar_startup.h"

/*
 * This code is board dependant and may have to be
 * changed by end users.
 */
void
init_timer(void) {
    timer_freq = RCAR_GEN4_CPG_CLK_CL16M;

    cpu_freq = cpu_freq_get(0); // 0 to get the freq of the current cpu
}

