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

#ifndef __RCAR_BOARD_CONFIG_V4H_H_INCLUDED
#define __RCAR_BOARD_CONFIG_V4H_H_INCLUDED

#include "rcar_gen4.h"

#define USE_SCIF     0
#define USE_HSCIF    1

ip_t v4h_scif_debug_config[] =
{
    /* SCIF0 */
    [USE_SCIF] = {
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
    [USE_HSCIF] = {
        /* HSCIF0 */
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
            /* RT-DMAC */
            { .base = RCAR_GEN4_RTDMAC0_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_RTDMAC1_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_RTDMAC2_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_RTDMAC3_COM_BASE,  .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            /* SYS-DMAC */
            { .base = RCAR_GEN4_SYSDMAC1_COM_BASE, .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_GEN4_SYSDMAC2_COM_BASE, .reg = RCAR_DMAC_DMAOR, .val = 1, .reg_len = 16 },
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

    /* Ethernet */
    {
        .pfc = (pfc_t[])
        {
            /* AVB0 */
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP7,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0,  .end_bit = 20, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 10, .end_bit = 10, .val = 0 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = 0 },
                    { 0 }
                },

                /* Settings for drive capability */
                .drvctrl = (pfc_reg_t[])
                {
                    //AVB0_AVTP_PPS, AVB0_AVTP_CAPTURE, AVB0_AVTP_MATCH, AVB0_TD3, AVB0_LINK, AVB0_PHY_INT, AVB0_TD2, AVB0_TD1
                    //{ .reg = RCAR_GEN4_GPIO_DRV0CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
                    //AVB0_RD3, AVB0_TXCREFCLK, AVB0_MAGIC, AVB0_TD0, AVB0_RD2, AVB0_MDC, AVB0_MDIO, AVB0_TXC.
                    //{ .reg = RCAR_GEN4_GPIO_DRV1CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
                    //AVB0_TX_CTL, AVB0_RD1, AVB0_RD0, AVB0_RXC, AVB0_RX_CTL.
                    //{ .reg = RCAR_GEN4_GPIO_DRV2CTRL, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = DRV3_7_8 },
                    { 0 }
                },
                /* Settings for MODSEL, POC, PUEN, PUD */
                .com = (pfc_reg_t[])
                {
                    //AVB0_AVTP_PPS
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 0, .end_bit = 0, .bitlen = 1, .val = 0 },
                    //AVB0_AVTP_MATCH
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 2, .end_bit = 2, .bitlen = 1, .val = 0 },
                    //AVB0_TD3
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 3, .end_bit = 3, .bitlen = 1, .val = 0 },
                    //AVB0_TD2
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 6, .end_bit = 6, .bitlen = 1, .val = 0 },
                    //AVB0_TD1
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 7, .end_bit = 7, .bitlen = 1, .val = 0 },
                    //AVB0_MAGIC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 10, .end_bit = 10, .bitlen = 1, .val = 0 },
                    //AVB0_TD0
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 11, .end_bit = 11, .bitlen = 1, .val = 0 },
                    //AVB0_MDC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 13, .end_bit = 13, .bitlen = 1, .val = 0 },
                    //AVB0_TXC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 15, .end_bit = 15, .bitlen = 1, .val = 0 },
                    //AVB0_TX_CTL
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 16, .end_bit = 16, .bitlen = 1, .val = 0 },

                    //AVB0_AVTP_PPS, AVB0_AVTP_CAPTURE, AVB0_AVTP_MATCH, AVB0_TD3, AVB0_LINK, AVB0_PHY_INT, AVB0_TD2, 
                    //AVB0_TD1, AVB0_RD3, AVB0_TXCREFCLK, AVB0_MAGIC, AVB0_TD0, AVB0_RD2, AVB0_MDC, AVB0_MDIO, AVB0_TXC,
                    //AVB0_TX_CTL, AVB0_RD1, AVB0_RD0 , AVB0_RXC , AVB0_RX_CTL
                    //{ .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 0,  .end_bit = 20,  .bitlen = 1, .val = 1 },
                    //{ .reg = RCAR_GEN4_GPIO_PUD,  .start_bit = 0,  .end_bit = 20,  .bitlen = 1, .val = 0},
                    /*
                     * Limitation for V4H 1.0: If using Tx clock internal Delay Mode, need to change IO voltage to 1.8V at 1Gbps
                     */
                    //{ .reg = RCAR_GEN4_GPIO_POC,  .start_bit = 0,  .end_bit = 0,  .bitlen = 1, .val = POC_1_8V },
                    { 0 }
                },
            },
            /* AVB1 */
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP6,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0,  .end_bit = 20, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 1,  .end_bit = 1,  .val = 0 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = 0 },
                    { 0 }
                },

                /* Settings for drive capability */
                .drvctrl = (pfc_reg_t[])
                {
                    //AVB1_MDIO, AVB1_MAGIC, AVB1_MDC, AVB1_PHY_INT, AVB1_LINK, AVB1_AVTP_MATCH, AVB1_TXC, AVB1_TX_CTL
                    //{ .reg = RCAR_GEN4_GPIO_DRV0CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
                    //AVB1_RXC, AVB1_RX_CTL, AVB1_AVTP_PPS, AVB1_AVTP_CAPTURE, AVB1_TD1, AVB1_TD0, AVB1_RD1, AVB1_RD0.
                    //{ .reg = RCAR_GEN4_GPIO_DRV1CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
                    //AVB1_TD2, AVB1_RD2, AVB1_TD3, AVB1_RD3, AVB1_TXCREFCLK
                    //{ .reg = RCAR_GEN4_GPIO_DRV2CTRL, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = DRV3_7_8 },
                    { 0 }
                },
                /* Settings for MODSEL, POC, PUEN, PUD */
                .com = (pfc_reg_t[])
                {
                    //AVB1_MAGIC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 1, .end_bit = 1, .bitlen = 1, .val = 1 },
                    //AVB1_MDC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 2, .end_bit = 2, .bitlen = 1, .val = 1 },
                    //AVB1_AVTP_MATCH
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 5, .end_bit = 5, .bitlen = 1, .val = 1 },
                    //AVB1_TXC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 6, .end_bit = 6, .bitlen = 1, .val = 1 },
                    //AVB1_TX_CTL
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 7, .end_bit = 7, .bitlen = 1, .val = 1 },
                    //AVB1_AVTP_PPS
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 10, .end_bit = 10, .bitlen = 1, .val = 1 },
                    //AVB1_TD1
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 12, .end_bit = 12, .bitlen = 1, .val = 1 },
                    //AVB1_TD0
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 13, .end_bit = 13, .bitlen = 1, .val = 1 },
                    //AVB1_TD2
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 16, .end_bit = 16, .bitlen = 1, .val = 1 },
                    //AVB1_TD3
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 18, .end_bit = 18, .bitlen = 1, .val = 1 },

                    //AVB1_TXCREFCLK,AVB1_RD3,AVB1_TD3,AVB1_RD2,AVB1_TD2,AVB1_RD0,AVB1_RD1,AVB1_TD0,
                    //AVB1_TD1,AVB1_AVTP_CAPTURE,AVB1_AVTP_PPS,AVB1_RX_CTL,AVB1_RXC,AVB1_TX_CTL,AVB1_TXC,
                    //AVB1_AVTP_MATCH,AVB1_LINK,AVB1_PHY_INT,AVB1_MDC,AVB1_MAGIC,AVB1_MDIO
                    //{ .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 0,  .end_bit = 20,  .bitlen = 1, .val = 1 },
                    //{ .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 0,  .end_bit = 0,  .bitlen = 1, .val = 0 },
                    //{ .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 2,  .end_bit = 2,  .bitlen = 1, .val = 0 },
                    //{ .reg = RCAR_GEN4_GPIO_PUD,  .start_bit = 0,  .end_bit = 20,  .bitlen = 1, .val = 0},
                    /*
                     * Limitation for V4H 1.0: If using Tx clock internal Delay Mode, need to change IO voltage to 1.8V at 1Gbps
                     */
                    //{ .reg = RCAR_GEN4_GPIO_POC,  .start_bit = 0,  .end_bit = 0,  .bitlen = 1, .val = POC_1_8V },
                    { 0 }
                },
            },
            /* AVB2 */
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP5,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0,  .end_bit = 20, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 5,  .end_bit = 5, .val = 0 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                   { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                   { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                   { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = 0 },
                   { 0 }
                },

                /* Settings for drive capability */
                .drvctrl = (pfc_reg_t[])
                {
                    //AVB2_AVTP_PPS, AVB2_AVTP_CAPTURE, AVB2_AVTP_MATCH, AVB2_LINK, AVB2_PHY_INT, AVB2_MAGIC, AVB2_TXCREFCLK
                    //{ .reg = RCAR_GEN4_GPIO_DRV0CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
                    //AVB2_TD3, AVB2_RD3, AVB2_MDIO, AVB2_TD2, AVB2_TD1, AVB2_RD2, AVB2_RD1, AVB2_MDC, AVB2_TD0
                    //{ .reg = RCAR_GEN4_GPIO_DRV1CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
                    //AVB2_TXC, AVB2_RD0, AVB2_RXC, AVB2_TX_CTL, AVB2_RX_CTL.
                    //{ .reg = RCAR_GEN4_GPIO_DRV2CTRL, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = DRV3_7_8 },
                    { 0 }
                },
                /* Settings for MODSEL, POC, PUEN, PUD */
                .com = (pfc_reg_t[])
                {
                    //AVB2_AVTP_PPS
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 0, .end_bit = 0, .bitlen = 1, .val = 1 },
                    //AVB2_AVTP_MATCH
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 2, .end_bit = 2, .bitlen = 1, .val = 1 },
                    //AVB2_MAGIC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 5, .end_bit = 5, .bitlen = 1, .val = 1 },
                    //AVB2_MDC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 6, .end_bit = 6, .bitlen = 1, .val = 1 },
                    //AVB2_TD3
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 8, .end_bit = 8, .bitlen = 1, .val = 1 },
                    //AVB2_TD2
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 11, .end_bit = 11, .bitlen = 1, .val = 1 },
                    //AVB2_TD3
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 12, .end_bit = 12, .bitlen = 1, .val = 1 },
                    //AVB2_TD0
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 15, .end_bit = 15, .bitlen = 1, .val = 1 },
                    //AVB2_TXC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 16, .end_bit = 16, .bitlen = 1, .val = 1 },
                    //AVB2_TX_CTL
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 19, .end_bit = 19, .bitlen = 1, .val = 1 },

                    //AVB2_RX_CTL,AVB2_TX_CTL,AVB2_RXC,AVB2_RD0,AVB2_TXC,AVB2_TD0,AVB2_RD1,AVB2_RD2,
                    //AVB2_TD1,AVB2_TD2,AVB2_MDIO,AVB2_RD3,AVB2_TD3,AVB2_TXCREFCLK,AVB2_MDC,AVB2_MAGIC,
                    //AVB2_PHY_INT,AVB2_LINK,AVB2_AVTP_MATCH,AVB2_AVTP_CAPTURE,AVB2_AVTP_PPS
                    //{ .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 0, .end_bit = 20, .bitlen = 1, .val = 1 },
                    //{ .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 6,  .end_bit = 6,  .bitlen = 1, .val = 0 },
                    //{ .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 10,  .end_bit = 10,  .bitlen = 1, .val = 0 },
                    //{ .reg = RCAR_GEN4_GPIO_PUD,  .start_bit = 0, .end_bit = 20, .bitlen = 1, .val = 0},
                    /*
                     * Limitation for V4H 1.0: If using Tx clock internal Delay Mode, need to change IO voltage to 1.8V at 1Gbps
                     */
                    //{ .reg = RCAR_GEN4_GPIO_POC,  .start_bit = 0, .end_bit = 0, .bitlen = 1, .val = POC_1_8V },
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
                    { .mod_id = CPG_MSTP_GEN4_AVB0, .flags = CPG_FLAGS_EN },
                    { .mod_id = CPG_MSTP_GEN4_AVB1, .flags = CPG_FLAGS_EN },
                    { .mod_id = CPG_MSTP_GEN4_AVB2, .flags = CPG_FLAGS_EN },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* TSN */
    {
        .pfc = (pfc_t[])
        {
            /* TSN0 */
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP4,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0,  .end_bit = 20, .val = 1 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                   { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                   { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
                   { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = 0 },
                   { 0 }
                },
                /* Settings for POC */
                .com = (pfc_reg_t[])
                {

                    //
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 1, .end_bit = 1, .bitlen = 1, .val = 1 },
                    //AVB2_AVTP_MATCH
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 2, .end_bit = 2, .bitlen = 1, .val = 1 },
                    //AVB2_MAGIC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 5, .end_bit = 5, .bitlen = 1, .val = 1 },
                    //AVB2_MDC
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 6, .end_bit = 6, .bitlen = 1, .val = 1 },
                    //AVB2_TD3
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 8, .end_bit = 9, .bitlen = 1, .val = 1 },
                    //AVB2_TD2
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 11, .end_bit = 11, .bitlen = 1, .val = 1 },
                    //AVB2_TD3
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 12, .end_bit = 12, .bitlen = 1, .val = 1 },
                    //AVB2_TD0
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 15, .end_bit = 15, .bitlen = 1, .val = 1 },
                    //TSN0_TD0, TSN0_TD1
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 14, .end_bit = 15, .bitlen = 1, .val = 1 },
                    //TSN0_TD2, TSN0_TD3
                    //{ .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 18, .end_bit = 19, .bitlen = 1, .val = 1 },

                    /*
                     * Limitation for V4H 1.0: If using Tx clock internal Delay Mode, need to change IO voltage to 1.8V at 1Gbps
                     */
                    { .reg = RCAR_GEN4_GPIO_POC,  .start_bit = 0, .end_bit = 0, .bitlen = 1, .val = POC_1_8V },
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
                    { .mod_id = CPG_MSTP_GEN4_TSN, .flags = CPG_FLAGS_EN },
                    { 0 }
                },
            },
            { 0 }
        },        
    },

    /* I2C */
    {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP8,
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
                .com = (pfc_reg_t[])
                {   //Settings for MODSEL
                    { .reg = RCAR_GEN4_GPIO_MODSEL,  .start_bit = 0, .end_bit = 11, .bitlen = 1, .val = 0 },
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
                    { .mod_id = CPG_MSTP_GEN4_I2C0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_I2C1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_I2C2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_I2C3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_I2C4, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_I2C5, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
            },
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
            {
                /* IF Power */
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP8,
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
#if defined(CPG_CLK_DIRECT_ACCESS)
                    { .reg = RCAR_GEN4_CPG_SD0CKCR1, .val = 0x0FFF0000 },
                    { .reg = RCAR_GEN4_CPG_SD0CKCR,  .val = 0x00000001 },
#else
                    // Gen4 SD0  clk max=200Mhz, initial=50Mhz
                    // Gen4 SD0H clk max=800Mhz, initial=stopped
                    { .clk_id = CPG_CLK_SDHI, .idx = 0, .freq = 200000000 },
#endif
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* Display */
    {
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_DU,         .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_DSITXLINK0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_FCPVD0,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VSPD0,      .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
                .ctrl = (cpg_ctrl_t[])
                {
                    { .reg = RCAR_GEN4_CPG_DSICKCR, .val = 0x00000000 },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* VIN */
    {
        .sysc = (sysc_t[])
        {
            {
                .domain = RCAR_GEN4_SYSC_DM0_BASE,
                .pdr = (sysc_pdr_t[])
                {
                    { .index = 60, .status = 0 },
                    { .index = 61, .status = 0 },
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
                    { .mod_id = CPG_MSTP_GEN4_VIN0,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN1,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN2,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN3,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN4,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN5,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN6,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN7,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN10,    .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN11,    .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN12,    .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN13,    .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN14,    .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_VIN15,    .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_ISPCS0,   .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_ISPCS1,   .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_ISP0,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_ISP1,     .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_CSI4LNK0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_CSI4LNK1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
                .ctrl = (cpg_ctrl_t[])
                {
                    { .reg = RCAR_GEN4_CPG_CSI0CKCR, .val = 0x0000001F },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* TMU */
    {
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_TMU0, .flags = CPG_FLAGS_EN },
                    { .mod_id = CPG_MSTP_GEN4_TMU1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_TMU2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_TMU3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_TMU4, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* CMT */
    {
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_CMT0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_CMT1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_CMT2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_CMT3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* TPU */
    {
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_TPU, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
            },
            { 0 }
        },
    },
    /*  CANFD */
    {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP2,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 3, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 7, .end_bit = 19, .val = 1 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    /* IP0SR2[3:0] = 1 CANFD1_TX, IP0SR2[7:4] = 1 CANFD1_RX, IP0SR2[11:8] = 1 CANFD5_TX, 
                       IP0SR2[15:12] = 1 CANFD5_RX, IP0SR2[31:28] = 1 CANFD6_TX */
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 15, .bitlen = 4, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 28, .end_bit = 31, .bitlen = 4, .val = 1 },
                    /* IP1SR2[3:0] = 1 CANFD6_RX, IP1SR2[7:4] = 0 CAN_CLK, IP1SR2[11:8] = 0 CANFD0_TX, 
                       IP1SR2[15:12] =0 CANFD0_RX, IP1SR2[19:16] = 0 CANFD2_TX, IP1SR2[23:20] = 0 CANFD2_RX, 
                       IP1SR2[27:24] = 0 CANFD3_TX, IP1SR2[31:28] = 0 CANFD3_RX */
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 0, .end_bit = 3, .bitlen = 4, .val = 1 },
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 4, .end_bit = 31, .bitlen = 4, .val = 0 },
                    /* IP2SR2[3:0] = 0 CANFD4_TX,IP2SR2[7:4] = 0 CANFD4_RX
                       IP2SR2[11:8] = 0 CANFD7_TX, IP2SR2[15:12] = 0 CANFD7_RX */
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0, .end_bit = 15, .bitlen = 4, .val = 0 },
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
                    { .mod_id = CPG_MSTP_GEN4_CANFD, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
                .ctrl = (cpg_ctrl_t[])
                {
                    { .reg = RCAR_GEN4_CPG_CANFDCKCR, .val = 0x00000009 },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* MSIOF */
    {
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP1,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 0, .end_bit = 5, .val = 1 },   /* MSIOF1_SYNC, _RXD, _TXD, _SCK, _SS1, _SS2 */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP0SR, .start_bit = 0, .end_bit = 23, .bitlen = 4, .val = 0 },
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_POC, .start_bit = 0, .end_bit = 5, .bitlen = 1, .val = POC_1_8V },
                    { 0 }
                },
            },
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP0,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 13, .end_bit = 18, .val = 1 },   /* MSIOF2_SYNC, _RXD, _TXD, _SCK, _SS1, _SS2 */
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP1SR, .start_bit = 20, .end_bit = 31, .bitlen = 4, .val = 0 },
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 0, .end_bit = 11, .bitlen = 4, .val = 0 },
                    { 0 }
                },
                .com = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_POC, .start_bit = 13, .end_bit = 18, .bitlen = 1, .val = POC_1_8V },
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
                    { .mod_id = CPG_MSTP_GEN4_MSI1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_GEN4_MSI2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
                .ctrl = (cpg_ctrl_t[])
                {
                    { .reg = RCAR_GEN4_CPG_MSOCKCR, .val = 0x0000001E },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* Thermal */
    {
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_TSC, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
                },
            },
            { 0 }
        },
    },

    /* Audio */
    {
        .cpg = (cpg_t[])
        {
            {
                .domain = RCAR_GEN4_CPG_DM0_BASE,
                .module = (cpg_module_t[])
                {
                    { .mod_id = CPG_MSTP_GEN4_SSI,  .flags = CPG_FLAGS_EN },
                    { .mod_id = CPG_MSTP_GEN4_SSIU, .flags = CPG_FLAGS_EN },
                    { .mod_id = CPG_MSTP_GEN4_ADG,  .flags = CPG_FLAGS_EN },
                    { 0 }
                },
            },
            { 0 }
        },
        .pfc = (pfc_t[])
        {
            {
                .domain = RCAR_GEN4_GPIO_DM0_BASE,
                .gpio_gp = RCAR_V4H_GPIO_GP1,
                .gpsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_GPSR, .start_bit = 18,  .end_bit = 22, .val = 1 },
                    { 0 }
                },
                .ipsr = (pfc_reg_t[])
                {
                    { .reg = RCAR_GEN4_GPIO_IP2SR, .start_bit = 8, .end_bit = 27, .bitlen = 4, .val = 0 },
                    { 0 }
                },
                //.drvctrl = (pfc_reg_t[])
                //{
                //    { .reg = RCAR_GEN4_GPIO_DRV2CTRL, .start_bit = 8, .end_bit = 27, .bitlen = 4, .val = DRV3_7_8 },
                //    { 0 }
                //},
                //.com = (pfc_reg_t[])
                //{
                //    { .reg = RCAR_GEN4_GPIO_POC,  .start_bit = 18, .end_bit = 22, .bitlen = 1, .val = POC_1_8V },
                //    { .reg = RCAR_GEN4_GPIO_PUEN, .start_bit = 18, .end_bit = 22, .bitlen = 1, .val = 0 },
                //    { 0 }
                //},
            },
            { 0 }
        }
    },
    
    /* Power on A3IR domain for v1.0 */
    {
        .sysc = (sysc_t[])
        {
            {
                .domain = RCAR_GEN4_SYSC_DM0_BASE,
                .pdr = (sysc_pdr_t[])
                {
                    { .index = 43, .status = 0 },
                    { 0 }
                },
            },
            { 0 }
        },
    },
    /* Last Entry */
    { .sysc= (sysc_t *)~0, .pfc = (pfc_t *)~0, .cpg = (cpg_t *)~0, }
};

#endif  //__RCAR_BOARD_CONFIG_V4H_H_INCLUDED

