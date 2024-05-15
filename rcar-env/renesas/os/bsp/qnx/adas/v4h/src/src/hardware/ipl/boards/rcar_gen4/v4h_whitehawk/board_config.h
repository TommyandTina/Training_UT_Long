/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#ifndef __RCAR_BOARD_CONFIG_S4_H_INCLUDED
#define __RCAR_BOARD_CONFIG_S4_H_INCLUDED

#include "rcar_gen4.h"
#include "board.h"

ip_t v4h_uart_config[] = {
    /* SCIF0 */
    [UART_USE_SCIF0] = {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP1,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 12, .end_bit = 17, .val = 1 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 16, .end_bit = 31, .bitlen = 4, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0 , .end_bit = 3 , .bitlen = 4, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 4 , .end_bit = 7 , .bitlen = 4, .val = 0 },
                    { 0 }
                },
            },
            { 0 }
        },
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_SCIF0, .flags = CPG_FLAGS_EN },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* HSCIF0 */
    [UART_USE_HSCIF0] = {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP1,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 12, .end_bit = 17, .val = 1 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 16, .end_bit = 31, .bitlen = 4, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0 , .end_bit = 7 , .bitlen = 4, .val = 0 },
                    { 0 }
                },
            },
            { 0 }
        },
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_HSCIF0, .flags = CPG_FLAGS_EN },
                    { 0 }
                },
            },
            { 0 }
        },
    },
};


ip_t v4h_config[] =
{
    /* DMAC */
    {
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    /* RT-DMAC */
                    { .mod_id = CPG_MSTP_GEN4_RTDM0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_RTDM1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_RTDM2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_RTDM3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    /* SYS-DMAC */
                    { .mod_id = CPG_MSTP_GEN4_SYDM1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_SYDM2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
            },
            { 0 }
        },
        .other = (other_t[])
        {
            /* SYS-DMAC */
            { .base = RCAR_GEN4_SYSDMAC1_COM_BASE, .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_SYSDMAC2_COM_BASE, .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            /* RT-DMAC */
            { .base = RCAR_GEN4_RTDMAC0_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_RTDMAC1_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_RTDMAC2_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_RTDMAC3_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { 0 }
        },
    },

    /* 	MMC/SDHI */
    {
        .pfc = (pfc_t[])
        {
            {
                /* MMC/SDHI */
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP3,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 11, .val = 1 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 0, .end_bit = 15, .bitlen = 4, .val = 0 },
                    { 0 }
                },				
                .drvctrl = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_DRV0CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
                    { .reg = RCAR_GEN4_GPIO_DRV1CTRL, .start_bit = 0, .end_bit = 15, .bitlen = 4, .val = DRV3_7_8 },
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_POC,  .start_bit = 0, .end_bit = 11, .bitlen = 1, .val = POC_1_8V },
                    { .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 0, .end_bit = 11, .bitlen = 1, .val = 0 },
                    { 0 }
                },
            },
            {
                /* SD VDD */
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP3,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 12, .end_bit = 12, .val = 0 },
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IOINTSEL, .start_bit = 12, .end_bit = 12, .bitlen = 1, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_INOUTSEL, .start_bit = 12, .end_bit = 12, .bitlen = 1, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_OUTDT,    .start_bit = 12, .end_bit = 12, .bitlen = 1, .val = 1 },
                    { 0 }
                },
            },
            { 0 }
        },
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_SDHI0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
                .ctrl = (cpg_ctrl_t[])
                {
                    { .reg = RCAR_GEN4_CPG_SD0CKCR1, .val = 0x0FFF0000 },
                    { .reg = RCAR_GEN4_CPG_SD0CKCR,  .val = 0x00000001 },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* RPC */
    {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP3,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 13, .end_bit = 29, .val = 1 },
                    { 0 }
                },
            },
            { 0 }
        },
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_RPC, .flags = CPG_FLAGS_EN },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* SYSC */
    {
        .sysc = (sysc_t[])
        {
            {
                .domain = RCAR_GEN4_SYSC_DM0_BASE,
                .pdr = (sysc_pdr_t[])
                {
                    { .index = SYSC_PDR_GEN4_A3E0, .status = 1 },
                    { .index = SYSC_PDR_GEN4_A3E1, .status = 1 },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* Last Entry */
    { .sysc= (sysc_t *)~0, .pfc = (pfc_t *)~0, .cpg = (cpg_t *)~0, }
};

#endif  //__RCAR_BOARD_CONFIG_S4_H_INCLUDED

