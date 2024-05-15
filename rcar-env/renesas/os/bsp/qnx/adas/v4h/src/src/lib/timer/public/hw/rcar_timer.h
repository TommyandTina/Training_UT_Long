/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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

#ifndef _HW_RCAR_TIMER_H_INCLUDED
#define _HW_RCAR_TIMER_H_INCLUDED

#include <time.h>

#define TIMER_TMU_MOD   (1)
#define TIMER_CMT_MOD   (2)

typedef struct _rcar_timer_functions {
    void *(*timer_create)(struct sigevent *event);
    int   (*timer_delete)(void *dev);
    int   (*timer_gettime)(void *dev, struct itimerspec *value);
    int   (*timer_settime)(void *dev, const struct itimerspec *value);
    int   (*timer_start)(void *dev);
    int   (*timer_stop)(void *dev);
} rcar_timer_functions_t;

int rcar_timer_get_funcs(int mod, rcar_timer_functions_t *funcs, int tabsize);

#endif /* _HW_RCAR_TIMER_H_INCLUDED */
