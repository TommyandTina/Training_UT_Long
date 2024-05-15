/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
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

// Module Description:  board specific header file

#ifndef _BS_H_INCLUDED
#define _BS_H_INCLUDED

#include <internal.h>

#include <sys/utsname.h>

#define SDIO_SOC_SUPPORT

#define SDIO_HC_RCAR_SDMMC

struct _sdio_hc;

typedef struct _sdhi_bs_ext {
    /* SD card VDD power */
    uintptr_t   pwr_vdd_base;   /* GPIO, for VDD power control */
    uint32_t    pwr_vdd_pbase;  /* GPIO Module Physical Addr */
    uint32_t    pwr_vdd_size;
    uint16_t    pwr_vdd_offset; /* Vdd GPIO Offset */
    uint16_t    pwr_vdd_shift;  /* Vdd bit shift*/

    /* SD card IF power */
    uintptr_t   pwr_if_base;    /* GPIO, for signal voltage power control */
    uint32_t    pwr_if_pbase;   /* GPIO Module Physical Addr */
    uint32_t    pwr_if_size;
    uint16_t    pwr_if_offset;  /* Vddqva GPIO Offset */
    uint16_t    pwr_if_shift;   /* Vddqva bit shift*/

    uintptr_t   pfc_base;       /* PIN function control, for SD pin IO voltage */
    uint32_t    pfc_pbase;      /* PFC Physical Addr */
    uint32_t    pfc_size;
    uint16_t    pfc_offset;     /* POCCTRL Offset */
    uint16_t    pfc_pmmr;       /* PMMR Offset */
    uint32_t    pfc_mask;       /* POCCTRL Mask */
    int         (*rcar_pwr)(struct _sdio_hc *, int);
    int         nowp;           /* If non-0, indicates WP is not supported */
    int         pwr_cpld_ctrl;  /* Control VDD and IF power by CPLD */
} sdmmc_bs_ext;

#endif

