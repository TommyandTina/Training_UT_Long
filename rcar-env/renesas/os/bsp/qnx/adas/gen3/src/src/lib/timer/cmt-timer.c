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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/queue.h>
#include <hw/rcar_cmt.h>
#include <hw/rcar_timer.h>

#include "proto.h"

#define CMT_TEST
#define CMT_DEBUG

#define CMT_ENV_CHAN      "CMT_FORCE_CHAN"
#define CMT_ENV_CLKDIV    "CMT_FORCE_CLKDIV"
#define CMT_ENV_BITSZ     "CMT_FORCE_BITSZ"

#define CMT_LIB_NAME      "rcar-cmt.so"
#define CMT_LOG_NAME      "cmt-timer"

#ifndef UINT48_MAX 
#define UINT48_MAX        (0xffffffffffff)
#endif

#define DEF_TV_SEC        (1.0)
#define MAX_TV_SEC        (0xfffffffff)
#define MAX_TV_NSEC       (999999999)
#define MIN_TICK_DIV      (100)

typedef struct cmt_hwpar {
    bool has_48bit;
    bool has_hires;
    uint32_t clock_freq;
    uint32_t bit_size;
    uint32_t clock_div;
} cmt_hwpar_t;

typedef struct cmt_task {
    bool notify;
    bool reload;
    uint64_t repeat_left;
    uint64_t repeat_count;
    uint64_t const_tick;
    SLIST_ENTRY(cmt_task) next;
} cmt_task_t;

typedef struct cmt_timer {
    bool periodic;
    double initial_sec;
    double interval_sec;
    double elapsed_sec;
    struct sigevent *event;
    cmt_hwpar_t hwpar;
    cmt_task_t *curr_task;
    SLIST_HEAD(, cmt_task) tasks;
    void *hw_hdl;
} cmt_timer_t;

static int forced_chan = -1;
static int forced_clkdiv = -1;
static int forced_bitsz = -1; 

static int refcount = 0;
static void *cmt_lib;
static const struct cmt_factory *cmt_sfp;

static int cmt_timer_get_div(int clkdiv)
{
    switch(clkdiv) {
        case CMT_CKS_RCLK_DIV_128:
        case CMT_CKS_PSEU_DIV_128:
        case CMT_CKS_CPEX_DIV_128:
            return 128;
        case CMT_CKS_RCLK_DIV_32:
        case CMT_CKS_PSEU_DIV_32:
        case CMT_CKS_CPEX_DIV_32:
            return 32;
        case CMT_CKS_RCLK_DIV_8:
        case CMT_CKS_PSEU_DIV_8:
        case CMT_CKS_CPEX_DIV_8:
            return 8;
        default:
            break;
    }

    return 1;
}

#ifdef CMT_DEBUG
static void cmt_timer_print_param(cmt_timer_t *cmt)
{
    cmt_task_t *task;

    cmt_slogf(_SLOG_DEBUG1, 0, "%s: cmt_param", CMT_LOG_NAME);
    cmt_slogf(_SLOG_DEBUG1, 0, "%s:   init_period : %.9lf sec", CMT_LOG_NAME, cmt->initial_sec);
    cmt_slogf(_SLOG_DEBUG1, 0, "%s:   intv_period : %.9lf sec", CMT_LOG_NAME, cmt->interval_sec);
    cmt_slogf(_SLOG_DEBUG1, 0, "%s:   clock_freq  : %d Hz", CMT_LOG_NAME, cmt->hwpar.clock_freq);
    cmt_slogf(_SLOG_DEBUG1, 0, "%s:   timer_size  : %d bits", CMT_LOG_NAME, cmt->hwpar.bit_size);
    cmt_slogf(_SLOG_DEBUG1, 0, "%s:   clock_presc : %d", CMT_LOG_NAME, cmt_timer_get_div(cmt->hwpar.clock_div));

    cmt_slogf(_SLOG_DEBUG1, 0, "%s: cmt_task", CMT_LOG_NAME);

    SLIST_FOREACH(task, &cmt->tasks, next) {
        cmt_slogf(_SLOG_DEBUG1, 0, "%s:   notify: %d, intv: %d, repeat: %ld, tick: %d", CMT_LOG_NAME,
            task->notify, task->reload, task->repeat_count, task->const_tick);
    }
}
#endif

static void cmt_timer_load_task(cmt_timer_t *cmt)
{
    cmt_task_t *curr = cmt->curr_task;
    cmt_task_t *temp;

    if(!curr) {
        cmt->curr_task = SLIST_FIRST(&cmt->tasks);
    } else {
        cmt->curr_task = SLIST_NEXT(curr, next);
    }

    if(!cmt->curr_task) {
        if(!cmt->periodic) {
            return;
        } else {
            temp = SLIST_FIRST(&cmt->tasks);
            while(!temp->reload) {
                temp = SLIST_NEXT(temp, next);
            }
            cmt->curr_task = temp;
            if(temp == curr) {
                return;
            }
        }
    }

    if(cmt->curr_task) {
        cmt_sfp->set_const(cmt->hw_hdl, cmt->curr_task->const_tick);
    }
}

static const struct sigevent *cmt_timer_irq_handler(void *arg, int id)
{
    double elapsed = 0;

    cmt_timer_t *cmt = (cmt_timer_t*)arg;
    cmt_task_t *task = cmt->curr_task;

    cmt_sfp->overflow(cmt->hw_hdl);

    if(!task) {
        return NULL;
    }

    elapsed += cmt_timer_get_div(cmt->hwpar.clock_div);
    elapsed *= (task->const_tick / (double)cmt->hwpar.clock_freq);

    cmt->elapsed_sec += elapsed;

    if(!(--task->repeat_left)) {
        task->repeat_left = task->repeat_count;
        cmt_timer_load_task(cmt);
    }

    if(task->notify) {
        return cmt->event;
    }

    return NULL;
}

static int cmt_timer_create_task(cmt_timer_t *cmt, double period, uint64_t count, bool reload, bool notify)
{
    cmt_task_t *curr;
    cmt_task_t *last, *temp;

    if(!(curr = calloc(1, sizeof(cmt_task_t)))) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: calloc() failed", CMT_LOG_NAME);
        return -1;
    }

    uint32_t div = cmt_timer_get_div(cmt->hwpar.clock_div);

    curr->notify = notify;
    curr->reload = reload;
    curr->const_tick = (period / div) * cmt->hwpar.clock_freq;
    curr->repeat_count = count;
    curr->repeat_left = count;

    if(SLIST_EMPTY(&cmt->tasks)) {
        SLIST_INSERT_HEAD(&cmt->tasks, curr, next);
        return 0;
    }

    temp = SLIST_FIRST(&cmt->tasks);

    while(temp) {
        last = temp;
        temp = SLIST_NEXT(last, next);
    }

    SLIST_INSERT_AFTER(last, curr, next);

    return 0;
}

static int cmt_timer_set_period(cmt_timer_t *cmt, double set_period, bool reload, bool notify)
{
    uint64_t repeat = 0;
    double max_period;

    switch(cmt->hwpar.bit_size) {
        case 16:
            max_period = (cmt_timer_get_div(cmt->hwpar.clock_div) * (double)UINT16_MAX) / cmt->hwpar.clock_freq;
            break;
        case 32:
            max_period = (cmt_timer_get_div(cmt->hwpar.clock_div) * (double)UINT32_MAX) / cmt->hwpar.clock_freq;
            break;
        default:
            max_period = (cmt_timer_get_div(cmt->hwpar.clock_div) * (double)UINT48_MAX) / cmt->hwpar.clock_freq;
            break;
    }

    while(set_period > max_period) {
        repeat++; set_period -= max_period;
    }

    if(repeat) {
        if(cmt_timer_create_task(cmt, max_period, repeat, reload, false) < 0) {
            return -1;
        }
    }

    if(cmt_timer_create_task(cmt, set_period, 1, reload, notify) < 0) {
        return -1;
    }

    return 0;
}

static int cmt_timer_request_48bit(cmt_timer_t *cmt)
{
    void *hdl;

    if((hdl = cmt_sfp->channel_attach(NULL, cmt_timer_irq_handler, cmt, 48, 0, CMT_ATTACH_ANY_CHANNEL | CMT_ATTACH_ISR_ON_OVERFLOW))) {
        cmt_sfp->channel_release(cmt->hw_hdl);
        cmt->hw_hdl = hdl;
        return 0;
    }

    return -1;
}                           

static int cmt_timer_select_bitsz(cmt_timer_t *cmt, double set_period)
{
    double max_period;

    max_period = (cmt_timer_get_div(cmt->hwpar.clock_div) * (double)UINT16_MAX) / cmt->hwpar.clock_freq;

    if(set_period < max_period) {
        cmt->hwpar.bit_size = 16; return 0;
    }

    max_period = (cmt_timer_get_div(cmt->hwpar.clock_div) * (double)UINT32_MAX) / cmt->hwpar.clock_freq;

    if(set_period < max_period) {
        cmt->hwpar.bit_size = 32; return 0;
    }

    max_period = (cmt_timer_get_div(cmt->hwpar.clock_div) * (double)UINT48_MAX) / cmt->hwpar.clock_freq;

    if(set_period < max_period) {
        cmt->hwpar.bit_size = 48; return 0;
    }

    return -1;
}

static int cmt_timer_select_clkdiv(cmt_timer_t *cmt, double set_period)
{
    uint64_t tick_x_div;

    tick_x_div = (uint64_t)roundl(set_period * cmt->hwpar.clock_freq);

    if(tick_x_div < MIN_TICK_DIV) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: too small time (%ld ns)", CMT_LOG_NAME, (int)round(set_period * 1000000000.0));
        errno = EINVAL;
        return -1;
    }

    if(cmt->hwpar.has_48bit) {
        if(tick_x_div <= UINT48_MAX) {
            cmt->hwpar.clock_div = (cmt->hwpar.has_hires) ? CMT_CKS_CPEX_DIV_1 : CMT_CKS_RCLK_DIV_1;
            return 0;
        }
    } else {
        if(tick_x_div <= UINT32_MAX) {
            cmt->hwpar.clock_div = (cmt->hwpar.has_hires) ? CMT_CKS_CPEX_DIV_1 : CMT_CKS_RCLK_DIV_1;
            return 0;
        }
    }

    if(cmt->hwpar.has_48bit) {
        if(tick_x_div <= (8 * UINT48_MAX)) {
            cmt->hwpar.clock_div = (cmt->hwpar.has_hires) ? CMT_CKS_CPEX_DIV_8 : CMT_CKS_RCLK_DIV_8;
            return 0;
        }
    } else {
        if(tick_x_div <= (8 * UINT32_MAX)) {
            cmt->hwpar.clock_div = (cmt->hwpar.has_hires) ? CMT_CKS_CPEX_DIV_8 : CMT_CKS_RCLK_DIV_8;
            return 0;
        }
    }

    if(cmt->hwpar.has_48bit) {
        if(tick_x_div <= (64 * UINT48_MAX)) {
            cmt->hwpar.clock_div = (cmt->hwpar.has_hires) ? CMT_CKS_CPEX_DIV_32 : CMT_CKS_RCLK_DIV_32;
            return 0;
        }
    } else {
        if(tick_x_div <= (64 * UINT32_MAX)) {
            cmt->hwpar.clock_div = (cmt->hwpar.has_hires) ? CMT_CKS_CPEX_DIV_32 : CMT_CKS_RCLK_DIV_32;
            return 0;
        }
    }

    cmt->hwpar.clock_div = (cmt->hwpar.has_hires) ? CMT_CKS_CPEX_DIV_128 : CMT_CKS_RCLK_DIV_128;

    return 0;
}

static void cmt_timer_reset_data(cmt_timer_t *cmt)
{
    cmt_task_t *task;

    cmt_sfp->stop_count(cmt->hw_hdl);

    if(!SLIST_EMPTY(&cmt->tasks)) {
        SLIST_FOREACH(task, &cmt->tasks, next) {
            free(task);
        }
    }

    SLIST_INIT(&cmt->tasks);

    cmt->curr_task = NULL;
    cmt->periodic = true;
    cmt->initial_sec = 0;
    cmt->interval_sec = 0;
    cmt->elapsed_sec = 0;
}

static int cmt_timer_init_defaul(cmt_timer_t *cmt)
{
    cmt_channel_query_t query;
    cmt_channel_info_t info;

    cmt_sfp->query_channel(cmt->hw_hdl, &query);
    cmt_sfp->channel_info(query.cmt_idx, query.chan_idx, &info);

    cmt->hwpar.bit_size = 32;

    if(info.counter_size_max == 48) {
        cmt->hwpar.has_48bit = true;
    }

    if(info.clock_src & CMT_CLOCK_SRC_CPEX) {
        cmt->hwpar.clock_div = CMT_CKS_CPEX_DIV_1;
        cmt->hwpar.has_hires = true;
    } else {
        cmt->hwpar.clock_div = CMT_CKS_RCLK_DIV_1;
        cmt->hwpar.has_hires = false;
    }

    cmt_sfp->setup_counter(cmt->hw_hdl, ~0L, cmt->hwpar.bit_size, CMT_MODE_FREE_RUNING, cmt->hwpar.clock_div);
    cmt_sfp->set_count(cmt->hw_hdl, 0);
    cmt_sfp->query_channel(cmt->hw_hdl, &query);

    cmt->hwpar.clock_freq = query.frequency;

    cmt_slogf(_SLOG_INFO, 0, "%s: minimum accepted time is %d ns", CMT_LOG_NAME,
            (int)round((1000000000.0 / cmt->hwpar.clock_freq) * MIN_TICK_DIV));

    cmt_timer_set_period(cmt, DEF_TV_SEC, true, cmt->event ? true : false);
    cmt_timer_load_task(cmt);

    return 0;
}

#ifdef CMT_TEST
static void cmt_timer_check_env(void)
{
    char *env = NULL;

    if(getenv(CMT_ENV_CHAN)) {
        env = strdup(getenv(CMT_ENV_CHAN));
        if((env[0] >= '0') || (env[0] <= '9')) {
            forced_chan = atoi(env);
            free(env);
        }
    }

    if(getenv(CMT_ENV_CLKDIV)) {
        env = strdup(getenv(CMT_ENV_CLKDIV));
        if((env[0] >= '0') || (env[0] <= '9')) {
            forced_clkdiv = atoi(env);
            free(env);
        }
    }

    if(getenv(CMT_ENV_BITSZ)) {
        env = strdup(getenv(CMT_ENV_BITSZ));
        if((env[0] >= '0') || (env[0] <= '9')) {
            forced_bitsz = atoi(env);
            free(env);
        }
    }
}
#endif

static void *cmt_timer_create(struct sigevent *event)
{
    int channel;
    int aflags = CMT_ATTACH_ISR_ON_OVERFLOW;
    void *cmt_hdl = NULL;
    cmt_timer_t *cmt = NULL;

    if(!(cmt = calloc(1, sizeof(cmt_timer_t)))) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: calloc() failed", CMT_LOG_NAME);
        return NULL;
    }

    if(!cmt_lib) {
        if(!(cmt_lib = dlopen(CMT_LIB_NAME, RTLD_NOW|RTLD_GROUP|RTLD_WORLD))) {
            cmt_slogf(_SLOG_ERROR, 0, "%s: unable to load support file %s", CMT_LOG_NAME, CMT_LIB_NAME);
            goto failed;
        }
    }

    if(!cmt_sfp) {
        if(!(cmt_sfp = dlsym(cmt_lib, "cmt_support"))) {
            cmt_slogf(_SLOG_ERROR, 0, "%s: support symbols not found in %s", CMT_LOG_NAME, CMT_LIB_NAME);
            goto failed;
        }
        if(cmt_sfp->init("verbose=1") < 0) {
            goto failed;
        }
    }

#ifdef CMT_TEST
    cmt_timer_check_env();
#endif

    if(forced_chan < 0) {
        channel = 0;
        aflags |= CMT_ATTACH_ANY_CHANNEL;
    } else {
        channel = forced_chan;
        aflags |= CMT_ATTACH_PRIORITY_STRICT;
    }

    if(!(cmt_hdl = cmt_sfp->channel_attach(NULL, cmt_timer_irq_handler, cmt, channel, 32, aflags))) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: unable to attach a CMT channel", CMT_LOG_NAME);
        goto failed;
    }

    cmt->periodic = true;
    cmt->event = event;
    cmt->hw_hdl = cmt_hdl;

    SLIST_INIT(&cmt->tasks);

    if(cmt_timer_init_defaul(cmt) < 0) {
        goto failed;
    }

    refcount++; return cmt;

failed:
    if(cmt_hdl) {
        cmt_sfp->channel_release(cmt_hdl);
    }
    if(cmt_sfp && !refcount) {
        cmt_sfp->fini(); cmt_sfp = NULL;
    }
    if(cmt_lib && !refcount) {
        dlclose(cmt_lib); cmt_lib = NULL;
    }
    return NULL;
}

static int cmt_timer_delete(void *dev)
{
    cmt_timer_t *cmt;
    cmt_task_t *task;

    if(!(cmt = (cmt_timer_t*)dev)) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: invalid argument", CMT_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    if(!SLIST_EMPTY(&cmt->tasks)) {
        SLIST_FOREACH(task, &cmt->tasks, next) {
            free(task);
        }
    }

    if(cmt_sfp) {
        cmt_sfp->channel_release(cmt->hw_hdl);
        if(!(--refcount)) {
            cmt_sfp->fini(); cmt_sfp = NULL;
        }
    }

    if(!refcount) {
        dlclose(cmt_lib); cmt_lib = NULL;
    }

    free(cmt); cmt = NULL;

    return 0;
}

static int cmt_timer_gettime(void *dev, struct itimerspec *value)
{
    cmt_timer_t *cmt;

    if(!value || !(cmt = (cmt_timer_t*)dev)) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: invalid argument", CMT_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    double elapsed = 0;

    elapsed += cmt_sfp->get_count(cmt->hw_hdl, false) / (double)cmt->hwpar.clock_freq;
    elapsed *= cmt_timer_get_div(cmt->hwpar.clock_div);
    elapsed += cmt->elapsed_sec;

    value->it_value.tv_sec = (time_t)elapsed;
    value->it_value.tv_nsec = (uint64_t)((elapsed - (time_t)elapsed) * 1000000000);

    return 0;
}

static int cmt_timer_settime(void *dev, const struct itimerspec *value)
{
    double init_period = 0;
    double intv_period = 0;
    double set_period;

    cmt_timer_t *cmt;

    if(!(cmt = (cmt_timer_t*)dev)) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: invalid argument", CMT_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    if((value->it_value.tv_sec < 0) || (value->it_value.tv_sec > MAX_TV_SEC)) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: invalid argument", CMT_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    if((value->it_value.tv_nsec < 0) || (value->it_value.tv_nsec > MAX_TV_NSEC)) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: invalid argument", CMT_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    cmt_timer_reset_data(cmt);

    init_period += value->it_value.tv_sec;
    init_period += value->it_value.tv_nsec / 1000000000.f;
    intv_period += value->it_interval.tv_sec;
    intv_period += value->it_interval.tv_nsec / 1000000000.f;

    cmt->initial_sec = init_period;
    cmt->interval_sec = intv_period;

    if(!value->it_interval.tv_sec && !value->it_interval.tv_nsec) {
        cmt->periodic = false;
    } else {
        cmt->periodic = true;;
    }

    set_period = (init_period > intv_period) ? init_period : intv_period;

    if((cmt_timer_select_clkdiv(cmt, set_period) < 0) || 
       (cmt_timer_select_bitsz(cmt, set_period) < 0)) {
        errno = EINVAL;
        return -1;
    }

    if((cmt->hwpar.bit_size == 48) && (!cmt->hwpar.has_48bit)) {
        if(cmt_timer_request_48bit(cmt) < 0) {
            cmt->hwpar.bit_size = 32;
        }
    }

    if(forced_clkdiv > 0) {
        cmt->hwpar.clock_div = forced_clkdiv;
    }

    if(forced_bitsz > 0) {
        if((forced_bitsz != 48) || ((forced_bitsz == 48) && (cmt->hwpar.has_48bit))) {
            cmt->hwpar.bit_size = forced_bitsz;
        }
    }

    bool notify = cmt->event ? true : false;

    if(cmt->periodic) {
        if((cmt_timer_set_period(cmt, init_period, false, notify) < 0) ||
           (cmt_timer_set_period(cmt, intv_period, true, notify) < 0)) {
            return -1;
        }
    } else {
        if(cmt_timer_set_period(cmt, init_period, false, notify) < 0) {
            return -1;
        }
    }

#ifdef CMT_DEBUG
    cmt_timer_print_param(cmt);
#endif

    cmt_sfp->setup_counter(cmt->hw_hdl, ~0L, cmt->hwpar.bit_size, CMT_MODE_FREE_RUNING, cmt->hwpar.clock_div);
    cmt_sfp->set_count(cmt->hw_hdl, 0);

    cmt_timer_load_task(cmt);

    return 0;
}

static int cmt_timer_start(void *dev)
{
    cmt_timer_t *cmt;

    if(!(cmt = (cmt_timer_t*)dev)) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: invalid argument", CMT_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    cmt_sfp->start_count(cmt->hw_hdl);

    return 0;
}

static int cmt_timer_stop(void *dev)
{
    cmt_timer_t *cmt;

    if(!(cmt = (cmt_timer_t*)dev)) {
        cmt_slogf(_SLOG_ERROR, 0, "%s: invalid argument", CMT_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    cmt_sfp->stop_count(cmt->hw_hdl);

    return 0;
}

int cmt_timer_get_funcs(rcar_timer_functions_t *functable, int tablesize)
{
    RCAR_TIMER_ADD_FUNC(functable, timer_create, cmt_timer_create, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_delete, cmt_timer_delete, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_gettime, cmt_timer_gettime, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_settime, cmt_timer_settime, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_start, cmt_timer_start, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_stop, cmt_timer_stop, tablesize);

    return 0;
}
