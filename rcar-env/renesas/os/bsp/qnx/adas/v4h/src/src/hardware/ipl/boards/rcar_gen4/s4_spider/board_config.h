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

ip_t s4_uart_config[] = {
    /* SCIF0 */
    [UART_USE_SCIF0] = {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_S4_GPIO_GP0,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 6, .end_bit = 10, .val = 1 },   /* RX0, TX0, SCK0, RTS0#, CTS0# */
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 0,  .val = 1 },   /* SCIF_CLK  */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit =  0, .end_bit =  3, .bitlen = 4, .val = 0 },   /* SCIF_CLK  */
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 24, .end_bit = 31, .bitlen = 4, .val = 0 },   /* RX0, TX0, SCK0 */
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit =  0, .end_bit = 11, .bitlen = 4, .val = 0 },   /* RTS0#, CTS0# */
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

    /* SCIF3 */
    [UART_USE_SCIF3] = {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_S4_GPIO_GP0,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 5, .val = 1 },  /* SCIF_CLK, SCK3, RX3, TX3, CTS3#, RTS3#, */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 3, .bitlen = 4, .val = 0 },   /* SCIF_CLK */
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 4, .end_bit = 23, .bitlen = 4, .val = 1 },   /* SCK3, RX3, TX3, CTS3#, RTS3# */
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
                    { .mod_id = CPG_MSTP_GEN4_SCIF3, .flags = CPG_FLAGS_EN },
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
                .gpio_gp = RCAR_S4_GPIO_GP0,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 5, .val = 1 },  /* SCIF_CLK, HSCK0, HRX0, HTX0, HCTS0#, HRTS0#, */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 3, .bitlen = 4, .val = 0 },   /* SCIF_CLK */
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 4, .end_bit = 23, .bitlen = 4, .val = 0 },   /* HSCK0, HRX0, HTX0, HCTS0#, HRTS0# */
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_POC, .start_bit = 2, .end_bit = 3, .bitlen = 1, .val = 0 },   /* HRX0, HTX0 : 1.8V */
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

    /* HSCIF1 */
    [UART_USE_HSCIF1] = {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_S4_GPIO_GP0,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 6, .end_bit = 10, .val = 1 },   /* HRX1, HTX1, HSCK1, HRTS1#, HCTS1# */
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 0,  .val = 1 },   /* SCIF_CLK  */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit =  0, .end_bit =  3, .bitlen = 4, .val = 0 },   /* SCIF_CLK  */
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 24, .end_bit = 31, .bitlen = 4, .val = 1 },   /* HRX1, HTX1, HSCK1 */
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit =  0, .end_bit = 11, .bitlen = 4, .val = 1 },   /* HRTS1#, HCTS1# */
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_POC, .start_bit = 6, .end_bit = 7, .bitlen = 1, .val = 0 },   /* HRX1, HTX1 : 1.8V */
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
                    { .mod_id = CPG_MSTP_GEN4_HSCIF1, .flags = CPG_FLAGS_EN },
                    { 0 }
                },
            },
            { 0 }
        },
    },
};


ip_t s4_config[] =
{
#if 0
    /* SCIF0/HSCIF1 */
    {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_S4_GPIO_GP0,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 6, .end_bit = 10, .val = 1 },   /* RX0, TX0, SCK0, RTS0#, CTS0# */
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 0,  .val = 1 },   /* SCIF_CLK  */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit =  0, .end_bit =  3, .bitlen = 4, .val = 0 },   /* SCIF_CLK  */
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 24, .end_bit = 31, .bitlen = 4, .val = USE_SCIF0_OR_HSCIF1 },   /* RX0, TX0, SCK0 */
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit =  0, .end_bit = 11, .bitlen = 4, .val = USE_SCIF0_OR_HSCIF1 },   /* RTS0#, CTS0# */
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
#if USE_SCIF0_OR_HSCIF1==USE_SCIF0
                    { .mod_id = CPG_MSTP_GEN4_SCIF0, .flags = CPG_FLAGS_EN },   /* SCIF0  */
#else
                    { .mod_id = CPG_MSTP_GEN4_HSCIF1, .flags = CPG_FLAGS_EN },   /* HSCIF1  */
#endif
                    { 0 }
                },
            },
            { 0 }
        },
    },
    /* HSCIF0/SCIF3 */
    {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_S4_GPIO_GP0,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 5, .val = 1 },  /* SCIF_CLK, HSCK0, HRX,0 HTX,0 HCTS0#, HRTS0#, */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 3, .bitlen = 4, .val = 0 },   /* SCIF_CLK */
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 4, .end_bit = 23, .bitlen = 4, .val = USE_HSCIF0_OR_SCIF3 },   /* HSCK0, HRX3 HTX3 / HCTS0#, HRTS0# */
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
#if USE_HSCIF0_OR_SCIF3==USE_HSCIF0
                    { .mod_id = CPG_MSTP_GEN4_HSCIF0, .flags = CPG_FLAGS_EN },  /* HSCIF0  */
#else
                    { .mod_id = CPG_MSTP_GEN4_SCIF3, .flags = CPG_FLAGS_EN },  /* SCIF3  */
#endif
                    { 0 }
                },
            },
            { 0 }
        },
    },
#endif
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

    /* SDHI */
    {
        .pfc = (pfc_t[])
        {
            {
                /* SDHI */
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_S4_GPIO_GP1,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 12, .end_bit = 23, .val = 1 },
                    { 0 }
                },
                .drvctrl = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_DRV2CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_4_8 },
                    { .reg = RCAR_GEN4_GPIO_DRV3CTRL, .start_bit = 0, .end_bit = 14, .bitlen = 4, .val = DRV3_4_8 },
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_POC,  .start_bit = 12, .end_bit = 23, .bitlen = 1, .val = POC_1_8V },
                    { .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 21, .end_bit = 23, .bitlen = 1, .val = 0 },
                    { 0 }
                },
            },
#if 0  // No SD card slot on S4 Spider board
            {
                /* SD VDD */
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_S4_GPIO_GP1,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 24, .end_bit = 24, .val = 0 },
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IOINTSEL, .start_bit = 24, .end_bit = 24, .bitlen = 1, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_INOUTSEL, .start_bit = 24, .end_bit = 24, .bitlen = 1, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_OUTDT,    .start_bit = 24, .end_bit = 24, .bitlen = 1, .val = 1 },
                    { 0 }
                },
            },
#endif
            { 0 }
        },
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_SDHI0, .flags = CPG_FLAGS_EN },
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
                .gpio_gp = RCAR_S4_GPIO_GP2,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 14, .val = 1 },
                    { 0 }
                },
                .drvctrl = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_DRV0CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV2_1_2 },
                    { .reg = RCAR_GEN4_GPIO_DRV1CTRL, .start_bit = 0, .end_bit = 27, .bitlen = 4, .val = DRV2_1_2 },
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

