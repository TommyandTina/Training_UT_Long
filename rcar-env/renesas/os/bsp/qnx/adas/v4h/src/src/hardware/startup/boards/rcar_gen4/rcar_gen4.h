/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#include <aarch64/r-car.h>
#include <hw/rcar_cpg_support.h>
#include <hw/rcar_sysc_support.h>

#ifdef BIT
 #undef BIT
#endif
#define BIT(x)              (1 << x)
#define BITS(x,y)           (((2ULL << (y-x)) - 1) << x)
#define BITMASK(x,y)        (((2ULL << (y-x)) - 1) << x)

#define DISABLE             0
#define ENABLE              1

#define GPSR_IO             0 /* GPIO */
#define GPSR_IP             1 /* Peripheral Function, mux with IPSR */
#define GPSR_PF             2 /* Peripheral Function, no mux */

#define GPIO_IN             0
#define GPIO_OUT            1
#define GPIO_INT            2

#define PUEN_DISABLE        0
#define PUEN_ENABLE         0xFFFFFFFF

#define PUD_PULL_DOWN       0
#define PUD_PULL_UP         0xFFFFFFFF

#define DRV(mask,shift)     (mask << shift)
#define DRV3                0x7
#define DRV2                0x3

#define DRVCAP(val, shift)  (val << shift)
#define DRV3_1_8            0x0
#define DRV3_2_8            0x1
#define DRV3_3_8            0x2
#define DRV3_1_2            0x3
#define DRV3_5_8            0x4
#define DRV3_6_8            0x5
#define DRV3_7_8            0x6
#define DRV3_FULL           0x7

#define DRV2_1_4            0x0
#define DRV2_1_2            0x1
#define DRV2_3_4            0x2
#define DRV2_FULL           0x3

#define POC_1_8V            0x0
#define POC_3_3V            0xFFFFFFFF

#define TDSEL(val, shift)   (val << shift)
#define TDSEL_10PF          0x0
#define TDSEL_20PF          0x2
#define TDSEL_30PF          0x1
#define TDSEL_40PF          0x3

/* Register settings */
#define CPG_FLAGS_DIS       (1 << 0)    /* module disable */
#define CPG_FLAGS_EN        (1 << 1)    /* module enable */
#define CPG_FLAGS_RST       (1 << 2)    /* module reset */


//	TODO: to be removed
#define CPG_CLK_DIRECT_ACCESS

typedef struct _cpg_module
{
    int      mod_id;
    uint32_t flags;
} cpg_module_t;

typedef struct _cpg_ctrl
{
#if defined(CPG_CLK_DIRECT_ACCESS)
    uint32_t reg;
    uint32_t val;
#else
    cpg_clk_id_t clk_id;
    int          idx;
    uint32_t     freq;
#endif
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

