/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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

#include <string.h>

#include <hw/rcar_timer.h>
#include "proto.h"

int rcar_timer_get_funcs(int mod, rcar_timer_functions_t *functable, int tablesize)
{
    memset(functable, 0, tablesize);

    switch(mod) {
        case TIMER_TMU_MOD:
            return tmu_timer_get_funcs(functable, tablesize);
        case TIMER_CMT_MOD:
            return cmt_timer_get_funcs(functable, tablesize);
        default:
            break;
    }

    return 0;
}
