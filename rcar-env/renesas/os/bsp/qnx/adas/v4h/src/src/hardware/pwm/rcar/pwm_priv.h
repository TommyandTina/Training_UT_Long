/*
* $QNXLicenseC:
* Copyright 2022, QNX Software Systems.
* Copyright 2022, Renesas Electronics Corporation.
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

#ifndef _PWM_PRIV_H_INCLUDED
#define _PWM_PRIV_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <hw/inout.h>
#include <hw/rcar_cpg.h>
#include <sys/queue.h>
#include <sys/resmgr.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include <hw/rcar_cpg.h>
#include <hw/rcar_pwm.h>

#define NSEC_PER_SEC            1000000000L

#define RCAR_PWM_MAX_DIVISION   24
#define RCAR_PWM_MIN_CYCLE      2
#define RCAR_PWM_MAX_CYCLE      1023

#define RCAR_PWMCNT_CYC0_MASK   0x03ff0000
#define RCAR_PWMCNT_CYC0_SHIFT  16

#define RCAR_PWMCNT_PH0_MASK    0x000003ff
#define RCAR_PWMCNT_PH0_SHIFT   0

#define RCAR_PWMCR_CC0_MASK     0x000f0000
#define RCAR_PWMCR_CC0_SHIFT    16

#define RCAR_PWMCR_CCMD         BIT(15)
#define RCAR_PWMCR_SYNC         BIT(11)
#define RCAR_PWMCR_SS0          BIT(4)
#define RCAR_PWMCR_EN0          BIT(0)

typedef struct {
    int linkid;
    dispatch_t *dpp;
    thread_pool_t *tpp;
    dispatch_context_t *ctp;
    resmgr_connect_funcs_t connect_funcs;
    resmgr_io_funcs_t io_funcs;
    resmgr_attr_t rattr;
    iofunc_attr_t ioattr;
    thread_pool_attr_t tpattr;
} pwm_resmgr_t;

typedef struct {
    bool active;
    pid_t owner;
    unsigned index;
    uintptr_t iobase;
    pthread_mutex_t lock;
} pwm_channel_t;

typedef struct {
    bool force_quit;
    unsigned product;
    unsigned n_chan;
    unsigned refclk;
    uintptr_t iobase;
    pwm_resmgr_t resmgr;
    pwm_channel_t *chans;
    cpg_mgr_funcs_t clk;
} pwm_device_t;

int pwm_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
void pwm_pfc_config(void);

#endif /* _PWM_PRIV_H_INCLUDED */
