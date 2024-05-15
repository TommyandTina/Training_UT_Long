/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#ifndef _HW_RCAR_TIMER_PROTO_INCLUDED
#define _HW_RCAR_TIMER_PROTO_INCLUDED

#include <hw/rcar_timer.h>

#define RCAR_TIMER_ADD_FUNC(table, entry, func, limit) \
    if (((size_t)&(((rcar_timer_functions_t *)0)->entry)) + \
        sizeof (void (*)()) <= (limit)) \
        (table)->entry = (func);

int cmt_timer_get_funcs(rcar_timer_functions_t *functable, int tablesize);
int tmu_timer_get_funcs(rcar_timer_functions_t *functable, int tablesize);

#endif /* _HW_RCAR_TIMER_PROTO_INCLUDED */
