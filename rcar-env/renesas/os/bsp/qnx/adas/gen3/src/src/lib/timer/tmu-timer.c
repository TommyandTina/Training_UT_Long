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
#include <hw/rcar_tmu.h>
#include <hw/rcar_timer.h>

#include "proto.h"

#define TMU_TEST
#define TMU_DEBUG

#define TMU_ENV_CHAN      "TMU_FORCE_CHAN"
#define TMU_ENV_CLKDIV    "TMU_FORCE_CLKDIV"

#define TMU_LIB_NAME      "rcar-tmu.so"
#define TMU_LOG_NAME      "tmu-timer"

#define DEF_TV_SEC        (1.0)
#define MAX_TV_SEC        (0xfffffffff)
#define MAX_TV_NSEC       (999999999)
#define MIN_TICK_DIV      (400)

typedef struct tmu_hwpar {
    uint32_t clock_freq;
    uint32_t bit_size;
    uint32_t clock_div;
} tmu_hwpar_t;

typedef struct tmu_task {
    bool notify;
    bool reload;
    uint64_t repeat_left;
    uint64_t repeat_count;
    uint32_t const_tick;
    SLIST_ENTRY(tmu_task) next;
} tmu_task_t;

typedef struct tmu_timer {
    bool periodic;
    double initial_sec;
    double interval_sec;
    double elapsed_sec;
    struct sigevent *event;
    tmu_hwpar_t hwpar;
    tmu_task_t *curr_task;
    SLIST_HEAD(, tmu_task) tasks;
    void *hw_hdl;
} tmu_timer_t;

static int forced_chan = -1;
static int forced_clkdiv = -1;

static int refcount = 0;
static void *tmu_lib;
static const struct tmu_factory *tmu_sfp;

static int tmu_timer_get_div(int clkdiv)
{
    switch(clkdiv) {
        case TMU_CLOCK_DIV_1024:
            return 1024;
        case TMU_CLOCK_DIV_256:
            return 256;
        case TMU_CLOCK_DIV_64:
            return 64;
        case TMU_CLOCK_DIV_16:
            return 16;
        default:
            break;
    }

    return 4;
}

#ifdef TMU_DEBUG
static void tmu_timer_print_param(tmu_timer_t *tmu)
{
    tmu_task_t *task;

    tmu_slogf(_SLOG_DEBUG1, 0, "%s: tmu_param", TMU_LOG_NAME);
    tmu_slogf(_SLOG_DEBUG1, 0, "%s:   init_period : %.9lf sec", TMU_LOG_NAME, tmu->initial_sec);
    tmu_slogf(_SLOG_DEBUG1, 0, "%s:   intv_period : %.9lf sec", TMU_LOG_NAME, tmu->interval_sec);
    tmu_slogf(_SLOG_DEBUG1, 0, "%s:   clock_freq  : %d Hz", TMU_LOG_NAME, tmu->hwpar.clock_freq);
    tmu_slogf(_SLOG_DEBUG1, 0, "%s:   timer_size  : %d bits", TMU_LOG_NAME, tmu->hwpar.bit_size);
    tmu_slogf(_SLOG_DEBUG1, 0, "%s:   clock_presc : %d", TMU_LOG_NAME, tmu_timer_get_div(tmu->hwpar.clock_div));

    tmu_slogf(_SLOG_DEBUG1, 0, "%s: tmu_task", TMU_LOG_NAME);

    SLIST_FOREACH(task, &tmu->tasks, next) {
        tmu_slogf(_SLOG_DEBUG1, 0, "%s:   notify: %d, reload: %d, repeat: %ld, tick: %d", TMU_LOG_NAME,
             task->notify, task->reload, task->repeat_count, task->const_tick);
    }
}
#endif

static void tmu_timer_load_task(tmu_timer_t *tmu)
{
    tmu_task_t *curr = tmu->curr_task;
    tmu_task_t *temp;

    if(!curr) {
        tmu->curr_task = SLIST_FIRST(&tmu->tasks);
    } else {
        tmu->curr_task = SLIST_NEXT(curr, next);
    }

    if(!tmu->curr_task) {
        if(!tmu->periodic) {
            return;
        } else {
            temp = SLIST_FIRST(&tmu->tasks);
            while(!temp->reload) {
                temp = SLIST_NEXT(temp, next);
            }
            tmu->curr_task = temp;
            if(temp == curr) {
                return;
            }
        }
    }

    if(tmu->curr_task) {
        tmu_sfp->set_constant(tmu->hw_hdl, tmu->curr_task->const_tick);
    }
}

static const struct sigevent *tmu_timer_irq_handler(void *arg, int id)
{
    double elapsed = 0;

    tmu_timer_t *tmu = (tmu_timer_t*)arg;
    tmu_task_t *task = tmu->curr_task;

    tmu_sfp->underflow_isr(tmu->hw_hdl);

    if(!task) {
        return NULL;
    }

    elapsed += tmu_timer_get_div(tmu->hwpar.clock_div);
    elapsed *= (task->const_tick / (double)tmu->hwpar.clock_freq);

    tmu->elapsed_sec += elapsed;

    if(!(--task->repeat_left)) {
        task->repeat_left = task->repeat_count;
        tmu_timer_load_task(tmu);
    }

    if(task->notify) {
        return tmu->event;
    }

    return NULL;
}

static int tmu_timer_create_task(tmu_timer_t *tmu, double period, uint64_t repeat, bool reload, bool notify)
{
    tmu_task_t *curr;
    tmu_task_t *last, *temp;

    if(!(curr = calloc(1, sizeof(tmu_task_t)))) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: calloc() failed", TMU_LOG_NAME);
        return -1;
    }

    uint32_t div = tmu_timer_get_div(tmu->hwpar.clock_div);

    curr->notify = notify;
    curr->reload = reload;
    curr->const_tick = (period / div) * tmu->hwpar.clock_freq;
    curr->repeat_count = repeat;
    curr->repeat_left = repeat;

    if(SLIST_EMPTY(&tmu->tasks)) {
        SLIST_INSERT_HEAD(&tmu->tasks, curr, next);
        return 0;
    }

    temp = SLIST_FIRST(&tmu->tasks);

    while(temp) {
        last = temp;
        temp = SLIST_NEXT(last, next);
    }

    SLIST_INSERT_AFTER(last, curr, next);

    return 0;
}

static int tmu_timer_set_period(tmu_timer_t *tmu, double set_period, bool reload, bool notify)
{
    uint64_t repeat = 0;
    double max_period;

    max_period = (tmu_timer_get_div(tmu->hwpar.clock_div) * (double)UINT32_MAX) / tmu->hwpar.clock_freq;

    while(set_period > max_period) {
        repeat++; set_period -= max_period;
    }

    if(repeat) {
        if(tmu_timer_create_task(tmu, max_period, repeat, reload, false) < 0) {
            return -1;
        }
    }

    if(tmu_timer_create_task(tmu, set_period, 1, reload, notify) < 0) {
        return -1;
    }

    return 0;
}

static int tmu_timer_select_clkdiv(tmu_timer_t *tmu, double set_period)
{
    uint64_t tick_x_div;

    tick_x_div = (uint64_t)roundl(set_period * tmu->hwpar.clock_freq);

    if(tick_x_div < MIN_TICK_DIV) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: too small time (%d ns)", TMU_LOG_NAME, (int)round(set_period * 1000000000.0));
        errno = EINVAL;
        return -1;
    }

    if(tick_x_div <= (4 * UINT32_MAX)) {
        tmu->hwpar.clock_div = TMU_CLOCK_DIV_4;
        return 0;
    }

    if(tick_x_div <= (16 * UINT32_MAX)) {
        tmu->hwpar.clock_div = TMU_CLOCK_DIV_16;
        return 0;
    }

    if(tick_x_div <= (64 * UINT32_MAX)) {
        tmu->hwpar.clock_div = TMU_CLOCK_DIV_64;
        return 0;
    }

    if(tick_x_div <= (256 * UINT32_MAX)) {
        tmu->hwpar.clock_div = TMU_CLOCK_DIV_256;
        return 0;
    }

    tmu->hwpar.clock_div = TMU_CLOCK_DIV_1024;

    return 0;
}

static void tmu_timer_reset_data(tmu_timer_t *tmu)
{
    tmu_task_t *task;

    tmu_sfp->stop_counter(tmu->hw_hdl);

    if(!SLIST_EMPTY(&tmu->tasks)) {
        SLIST_FOREACH(task, &tmu->tasks, next) {
            free(task);
        }
    }

    SLIST_INIT(&tmu->tasks);

    tmu->curr_task = NULL;
    tmu->periodic = true;
    tmu->initial_sec = 0;
    tmu->interval_sec = 0;
    tmu->elapsed_sec = 0;
}

static int tmu_timer_init_defaul(tmu_timer_t *tmu)
{
    tmu_clock_t clk;

    tmu_sfp->get_clock(tmu->hw_hdl, &clk);

    tmu->hwpar.bit_size = 32;
    tmu->hwpar.clock_div = TMU_CLOCK_DIV_4;
    tmu->hwpar.clock_freq = clk.parent;

    clk.div = TMU_CLOCK_DIV_4;
    clk.clk_edge = TMU_CLOCK_EDGE_RISE;

    tmu_sfp->set_clock(tmu->hw_hdl, &clk);
    tmu_sfp->set_constant(tmu->hw_hdl, UINT32_MAX);

    tmu_slogf(_SLOG_INFO, 0, "%s: minimum accepted time is %d ns", TMU_LOG_NAME,
            (int)round((1000000000.0 / tmu->hwpar.clock_freq) * MIN_TICK_DIV));

    tmu_timer_set_period(tmu, DEF_TV_SEC, true, tmu->event ? true : false);
    tmu_timer_load_task(tmu);

    return 0;
}

#ifdef TMU_TEST
static void tmu_timer_check_env(void)
{
    char *env = NULL;

    if(getenv(TMU_ENV_CHAN)) {
        env = strdup(getenv(TMU_ENV_CHAN));
        if((env[0] >= '0') || (env[0] <= '9')) {
            forced_chan = atoi(env);
            free(env);
        }
    }

    if(getenv(TMU_ENV_CLKDIV)) {
        env = strdup(getenv(TMU_ENV_CLKDIV));
        if((env[0] >= '0') || (env[0] <= '9')) {
            forced_clkdiv = atoi(env);
            free(env);
        }
    }
}
#endif

static void *tmu_timer_create(struct sigevent *event)
{
    int channel;
    int aflags = TMU_ATTACH_UNDR_ISR;
    void *tmu_hdl = NULL;
    tmu_timer_t *tmu = NULL;

    if(!(tmu = calloc(1, sizeof(tmu_timer_t)))) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: calloc() failed", TMU_LOG_NAME);
        return NULL;
    }

    if(!tmu_lib) {
        if(!(tmu_lib = dlopen(TMU_LIB_NAME, RTLD_NOW|RTLD_GROUP|RTLD_WORLD))) {
            tmu_slogf(_SLOG_ERROR, 0, "%s: unable to load support file %s", TMU_LOG_NAME, TMU_LIB_NAME);
            goto failed;
        }
    }

    if(!tmu_sfp) {
        if(!(tmu_sfp = dlsym(tmu_lib, "tmu_support"))) {
            tmu_slogf(_SLOG_ERROR, 0, "%s: support symbols not found in %s", TMU_LOG_NAME, TMU_LIB_NAME);
            goto failed;
        }
        if(tmu_sfp->init("verbose=1") < 0) {
            goto failed;
        }
    }

#ifdef TMU_TEST
    tmu_timer_check_env();
#endif

    if(forced_chan < 0) {
        channel = 0;
        aflags |= TMU_ATTACH_ANY_CHANNEL;
    } else {
        channel = forced_chan;
        aflags |= TMU_ATTACH_PRIORITY_STRICT;
    }

    if(!(tmu_hdl = tmu_sfp->channel_attach(NULL, tmu_timer_irq_handler, NULL, NULL, tmu, channel, aflags))) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: unable to attach a TMU channel", TMU_LOG_NAME);
        goto failed;
    }

    tmu->periodic = true;
    tmu->event = event;
    tmu->hw_hdl = tmu_hdl;

    SLIST_INIT(&tmu->tasks);

    if(tmu_timer_init_defaul(tmu) < 0) {
        goto failed;
    }

    refcount++; return tmu;

failed:
    if(tmu_hdl) {
        tmu_sfp->channel_release(tmu_hdl);
    }
    if(tmu_sfp && !refcount) {
        tmu_sfp->fini(); tmu_sfp = NULL;
    }
    if(tmu_lib && !refcount) {
        dlclose(tmu_lib); tmu_lib = NULL;
    }
    return NULL;
}

static int tmu_timer_delete(void *dev)
{
    tmu_timer_t *tmu;
    tmu_task_t *task;

    if(!(tmu = (tmu_timer_t*)dev)) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: invalid argument", TMU_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    if(!SLIST_EMPTY(&tmu->tasks)) {
        SLIST_FOREACH(task, &tmu->tasks, next) {
            free(task);
        }
    }

    if(tmu_sfp) {
        tmu_sfp->stop(tmu->hw_hdl, NULL);
        tmu_sfp->channel_release(tmu->hw_hdl);
        if(!(--refcount)) {
            tmu_sfp->fini(); tmu_sfp = NULL;
        }
    }

    if(!refcount) {
        dlclose(tmu_lib); tmu_lib = NULL;
    }

    free(tmu); tmu = NULL;

    return 0;
}

static int tmu_timer_gettime(void *dev, struct itimerspec *value)
{
    double elapsed = 0;
    uint32_t curr_tick;

    tmu_timer_t *tmu;

    if(!value || !(tmu = (tmu_timer_t*)dev)) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: invalid argument", TMU_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    tmu_sfp->get_count(tmu->hw_hdl, &curr_tick);

    elapsed += (tmu->curr_task->const_tick - curr_tick) / (double)tmu->hwpar.clock_freq;
    elapsed *= tmu_timer_get_div(tmu->hwpar.clock_div);
    elapsed += tmu->elapsed_sec;

    value->it_value.tv_sec = (time_t)elapsed;
    value->it_value.tv_nsec = (uint64_t)((elapsed - (time_t)elapsed) * 1000000000);

    return 0;
}

static int tmu_timer_settime(void *dev, const struct itimerspec *value)
{
    double init_period = 0;
    double intv_period = 0;
    double set_period;

    tmu_timer_t *tmu;

    if(!(tmu = (tmu_timer_t*)dev)) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: invalid argument", TMU_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    if((value->it_value.tv_sec < 0) || (value->it_value.tv_sec > MAX_TV_SEC)) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: invalid argument", TMU_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    if((value->it_value.tv_nsec < 0) || (value->it_value.tv_nsec > MAX_TV_NSEC)) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: invalid argument", TMU_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    tmu_timer_reset_data(tmu);

    init_period += value->it_value.tv_sec;
    init_period += value->it_value.tv_nsec / 1000000000.f;
    intv_period += value->it_interval.tv_sec;
    intv_period += value->it_interval.tv_nsec / 1000000000.f;

    tmu->initial_sec = init_period;
    tmu->interval_sec = intv_period;

    if(!value->it_interval.tv_sec && !value->it_interval.tv_nsec) {
        tmu->periodic = false;
    } else {
        tmu->periodic = true;;
    }

    set_period = (init_period > intv_period) ? init_period : intv_period;

    if(tmu_timer_select_clkdiv(tmu, set_period) < 0) {
        return -1;
    }

    if(forced_clkdiv > 0) {
        tmu->hwpar.clock_div = forced_clkdiv;
    }

    bool notify = tmu->event ? true : false;

    if(tmu->periodic) {
        if((tmu_timer_set_period(tmu, init_period, false, notify) < 0) ||
           (tmu_timer_set_period(tmu, intv_period, true, notify) < 0)) {
            return -1;
        }
    } else {
        if(tmu_timer_set_period(tmu, init_period, false, notify) < 0) {
            return -1;
        }
    }

#ifdef TMU_DEBUG
    tmu_timer_print_param(tmu);
#endif

    tmu_clock_t clk = {
        .parent = tmu->hwpar.clock_freq,
        .clk_edge = TMU_CLOCK_EDGE_RISE,
        .div = tmu->hwpar.clock_div
    };

    tmu_sfp->set_clock(tmu->hw_hdl, &clk);

    tmu_timer_load_task(tmu);

    return 0;
}

static int tmu_timer_start(void *dev)
{
    tmu_timer_t *tmu;

    if(!(tmu = (tmu_timer_t*)dev)) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: invalid argument", TMU_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    tmu_sfp->start_counter(tmu->hw_hdl);

    return 0;
}

static int tmu_timer_stop(void *dev)
{
    tmu_timer_t *tmu;

    if(!(tmu = (tmu_timer_t*)dev)) {
        tmu_slogf(_SLOG_ERROR, 0, "%s: invalid argument", TMU_LOG_NAME);
        errno = EINVAL;
        return -1;
    }

    tmu_sfp->stop_counter(tmu->hw_hdl);

    return 0;
}

int tmu_timer_get_funcs(rcar_timer_functions_t *functable, int tablesize)
{
    RCAR_TIMER_ADD_FUNC(functable, timer_create, tmu_timer_create, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_delete, tmu_timer_delete, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_gettime, tmu_timer_gettime, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_settime, tmu_timer_settime, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_start, tmu_timer_start, tablesize);
    RCAR_TIMER_ADD_FUNC(functable, timer_stop, tmu_timer_stop, tablesize);

    return 0;
}
