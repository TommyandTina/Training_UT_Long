/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#include <aarch64/r-car-gen3.h>

//#define SOC_DEBUG
#ifdef SOC_DEBUG
 #define SOC_DEBUG_PRINT(format, ...) kprintf(format, ##__VA_ARGS__)
#else
 #define SOC_DEBUG_PRINT(format, ...)
#endif

#ifndef BIT
#define BIT(x)              (1 << x)
#endif
#define BITMASK(x,y)        (((2ULL << (y-x)) - 1) << x)



/* Register settings */
#define CPG_FLAGS_DIS       (1 << 0)    /* module disable */
#define CPG_FLAGS_EN        (1 << 1)    /* module enable */
#define CPG_FLAGS_RST       (1 << 2)    /* module reset */

typedef struct _cpg_module
{
    int mod_id;
    uint32_t flags;
} cpg_module_t;

typedef struct _cpg_ctrl
{
    uint32_t reg;
    uint32_t val;
} cpg_ctrl_t;

typedef struct _cpg{
    uint32_t domain;
    cpg_module_t *module;
    cpg_ctrl_t *ctrl;
} cpg_t;

typedef struct _pfc_reg
{
    uint32_t reg;           /* register index */
    uint32_t start_bit;
    uint32_t end_bit;
    uint32_t bitlen;
    uint32_t val;
} pfc_reg_t;

typedef struct _pfc
{
    uint32_t domain;        /* domain index */
    uint32_t gpio_gp;       /* gpio group index */
    pfc_reg_t *gpsr;
    pfc_reg_t *ipsr;
    pfc_reg_t *drvctrl;
    pfc_reg_t *com;
} pfc_t;

typedef struct _sysc_pdr
{
    uint32_t index;         /* Power Domain Register Number */
    uint32_t status;        /* 0: ON, 1: OFF */
} sysc_pdr_t;

typedef struct _sysc
{
    uint32_t domain;        /* sysc domain index */
    sysc_pdr_t *pdr;
} sysc_t;

typedef struct _other
{
    uint32_t base;
    uint32_t reg;
    uint32_t val;
    uint32_t reg_len;
} other_t;

typedef struct _ip
{
    sysc_t *sysc;
    pfc_t *pfc;
    cpg_t *cpg;
    other_t *other;
} ip_t;

void sysc_config(sysc_t *cfg);
void pfc_config(pfc_t *cfg);
void cpg_config(cpg_t *cfg);

void soc_config(ip_t *cfg);
void udelay(uint32_t usec);
void mynanospin_ns(unsigned long nsec);

