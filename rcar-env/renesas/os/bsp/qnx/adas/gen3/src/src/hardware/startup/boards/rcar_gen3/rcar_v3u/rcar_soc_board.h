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

#ifndef __RCAR_SOC_BOARD_V3U_H_INCLUDED
#define __RCAR_SOC_BOARD_V3U_H_INCLUDED

#include "rcar_soc.h"
#include "rcar_board.h"

// devc (UART) can use SCIF0 or HSCIF0
#define USE_SCIF                    0
#define USE_HSCIF                   1
#define USE_SCIF_HSICF_FOR_UART     USE_SCIF

ip_t v3u_config[] =
{
	/* I2C */
	{
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP2,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 2, .end_bit = 15, .val = 0 },
					{ 0 }
				},
				.com = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IOINTSEL, .start_bit = 2, .end_bit = 15, .bitlen = 1, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_INOUTSEL, .start_bit = 2, .end_bit = 15, .bitlen = 1, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_MODSEL,   .start_bit = 2, .end_bit = 15, .bitlen = 1, .val = 1 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_I2C0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_I2C1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_I2C2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_I2C3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_I2C6, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
	/* SDHI */
	{
		.pfc = (pfc_t[])
		{
			{
				/* SDHI */
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP0,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 16, .end_bit = 27, .val = 1 },
					{ 0 }
				},
				.drvctrl = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_DRV2CTRL, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = DRV3_7_8 },
					{ .reg = RCAR_V3U_GPIO_DRV3CTRL, .start_bit = 0, .end_bit = 14, .bitlen = 4, .val = DRV3_7_8 },
					{ 0 }
				},
				.com = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_POC,  .start_bit = 16, .end_bit = 27, .bitlen = 1, .val = POC_1_8V },
					{ .reg = RCAR_V3U_GPIO_PUEN, .start_bit = 16, .end_bit = 27, .bitlen = 1, .val = 0 },
					{ 0 }
				},
			},
			{
				/* SD VDD */
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP0,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 15, .end_bit = 15, .val = 0 },
					{ 0 }
				},
				.com = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IOINTSEL, .start_bit = 15, .end_bit = 15, .bitlen = 1, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_INOUTSEL, .start_bit = 15, .end_bit = 15, .bitlen = 1, .val = 1 },
					{ .reg = RCAR_V3U_GPIO_OUTDT,    .start_bit = 15, .end_bit = 15, .bitlen = 1, .val = 1 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_SDHI0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ 0 }
				},
				.ctrl = (cpg_ctrl_t[])
				{
					{ .reg = RCAR_V3U_CPG_SD0CKCR, .val = 0x00000001 },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
	/* DMAC */
	{
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					/* RT-DMAC */
                    { .mod_id = CPG_MSTP_V3U_RTDM0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_V3U_RTDM1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_V3U_RTDM2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_V3U_RTDM3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    /* SYS-DMAC */
                    { .mod_id = CPG_MSTP_V3U_SYDM1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { .mod_id = CPG_MSTP_V3U_SYDM2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
                    { 0 }
				},
			},
			{ 0 }
		},

        .other = (other_t[])
        {
            /* Enable SYS-DMAC */
            { .base = RCAR_V3U_SYSDMAC1_COM_BASE, .reg = RCAR_GEN3_SYSDMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_V3U_SYSDMAC2_COM_BASE, .reg = RCAR_GEN3_SYSDMAC_DMAOR, .val = 1, .reg_len = 16 },

            /* If enable read rate control, please set one or more groups here: 
             * - Replace <READ_RATE_VALUE> to real value 
             * - And uncomment code
             */
            //{ .base = RCAR_V3U_SYSDMAC1_COM_BASE, .reg = RCAR_GEN3_SYSDMAC_RATE_RD, .val = BIT(31) | <READ_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_SYSDMAC2_COM_BASE, .reg = RCAR_GEN3_SYSDMAC_RATE_RD, .val = BIT(31) | <READ_RATE_VALUE>, .reg_len = 32 },

            /* If enable write rate control, please set one or more group here: 
             * - Replace <WRITE_RATE_VALUE> to real value 
             * - And uncomment code
             */
            //{ .base = RCAR_V3U_SYSDMAC1_COM_BASE, .reg = RCAR_GEN3_SYSDMAC_RATE_WR, .val = BIT(31) | <WRITE_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_SYSDMAC2_COM_BASE, .reg = RCAR_GEN3_SYSDMAC_RATE_WR, .val = BIT(31) | <WRITE_RATE_VALUE>, .reg_len = 32 },

            /* Enable RT-DMAC */
            { .base = RCAR_V3U_RTDMAC0_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_V3U_RTDMAC1_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_V3U_RTDMAC2_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_DMAOR, .val = 1, .reg_len = 16 },
            { .base = RCAR_V3U_RTDMAC3_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_DMAOR, .val = 1, .reg_len = 16 },

            /* If enable read rate control, please set one or more groups here: 
             * - Replace <READ_RATE_VALUE> to real value 
             * - And uncomment code
             */
            //{ .base = RCAR_V3U_RTDMAC0_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_RD, .val = BIT(31) | <READ_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_RTDMAC1_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_RD, .val = BIT(31) | <READ_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_RTDMAC2_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_RD, .val = BIT(31) | <READ_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_RTDMAC3_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_RD, .val = BIT(31) | <READ_RATE_VALUE>, .reg_len = 32 },

            /* If enable write rate control, please set one or more group here: 
             * - Replace <WRITE_RATE_VALUE> to real value 
             * - And uncomment code
             */
            //{ .base = RCAR_V3U_RTDMAC0_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_WR, .val = BIT(31) | <WRITE_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_RTDMAC1_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_WR, .val = BIT(31) | <WRITE_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_RTDMAC2_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_WR, .val = BIT(31) | <WRITE_RATE_VALUE>, .reg_len = 32 },
            //{ .base = RCAR_V3U_RTDMAC3_COM_BASE,  .reg = RCAR_GEN3_SYSDMAC_RATE_WR, .val = BIT(31) | <WRITE_RATE_VALUE>, .reg_len = 32 },

            { 0 }
        },
	},
	/* Ethernet */
	{
		.pfc = (pfc_t[])
		{
			/* AVB0 */
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP4,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0,  .end_bit = 20, .val = 1 },
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 15, .end_bit = 15, .val = 0 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP1SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP2SR, .start_bit = 4, .end_bit = 19, .bitlen = 4, .val = 0 },
					{ 0 }
				},

			},
			/* AVB1 */
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP5,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 20, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP1SR, .start_bit = 0, .end_bit = 31, .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP2SR, .start_bit = 4, .end_bit = 19, .bitlen = 4, .val = 0 },
					{ 0 }
				},
			},
			/* AVB2 */
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP6,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 20, .val = 1 },
					{ 0 }
				},
			},
			/* AVB3 */
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP7,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 20, .val = 1 },
					{ 0 }
				},
			},
			/* AVB4 */
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP8,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 20, .val = 1 },
					{ 0 }
				},
			},
			/* AVB5 */
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP9,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 20, .val = 1 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_AVB0, .flags = CPG_FLAGS_EN },
					{ .mod_id = CPG_MSTP_V3U_AVB1, .flags = CPG_FLAGS_EN },
					{ .mod_id = CPG_MSTP_V3U_AVB2, .flags = CPG_FLAGS_EN },
					{ .mod_id = CPG_MSTP_V3U_AVB3, .flags = CPG_FLAGS_EN },
					{ .mod_id = CPG_MSTP_V3U_AVB4, .flags = CPG_FLAGS_EN },
					{ .mod_id = CPG_MSTP_V3U_AVB5, .flags = CPG_FLAGS_EN },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
#if USE_SCIF_HSICF_FOR_UART == USE_SCIF
	/* SCIF0 */
	{
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP1,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 5, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 0, .end_bit = 3,  .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 4, .end_bit = 23, .bitlen = 4, .val = 1 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_SCIF0, .flags = CPG_FLAGS_EN },
					{ 0 }
				},
			},
			{ 0 }
		},
	},	
#else
	{
		/* HSCIF0 */
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP1,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 5, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 4, .end_bit = 23, .bitlen = 4, .val = 0 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_HSCIF0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
#endif
	/* RPC */
	{
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP0,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 14, .val = 1 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_RPC, .flags = CPG_FLAGS_EN },
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
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP1,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 6, .end_bit = 23, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 24, .end_bit = 31, .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP1SR, .start_bit = 0,  .end_bit = 31, .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP2SR, .start_bit = 0,  .end_bit = 31, .bitlen = 4, .val = 0 },
					{ 0 }
				},
				.com = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_POC, .start_bit = 12, .end_bit = 23, .bitlen = 1, .val = POC_1_8V },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_MSI0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_MSI1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_MSI2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_MSI3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_MSI4, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_MSI5, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ 0 }
				},
				.ctrl = (cpg_ctrl_t[])
				{
					{ .reg = RCAR_V3U_CPG_MSOCKCR, .val = 0x0000001E },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
	/* TSC */
	{
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_TSC, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
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
				.domain = RCAR_V3U_SYSC_DM0_BASE,
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
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_VIN0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN02, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN03, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN04, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN05, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN06, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN07, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN10, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN11, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN12, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN13, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN14, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN15, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN16, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN17, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN20, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN21, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN22, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN23, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN24, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN25, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN26, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN27, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN30, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN31, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN32, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN33, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN34, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN35, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN36, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VIN37, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISPCS0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISPCS1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISPCS2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISPCS3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISP0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISP1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISP2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_ISP3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_CSI4LNK0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_CSI4LNK1, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_CSI4LNK2, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_CSI4LNK3, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ 0 }
				},
				.ctrl = (cpg_ctrl_t[])
				{
					{ .reg = RCAR_V3U_CPG_CSI0CKCR, .val = 0x0000001F },
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
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_DU, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_DSITXLINK0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_FCPVD0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ .mod_id = CPG_MSTP_V3U_VSPD0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ 0 }
				},
				.ctrl = (cpg_ctrl_t[])
				{
					{ .reg = RCAR_V3U_CPG_DSICKCR, .val = 0x00000000 },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
	/* CANFD */
	{
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP3,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 16, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 4,  .end_bit = 11,  .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 20, .end_bit = 31,  .bitlen = 4, .val = 0 },
					{ .reg = RCAR_V3U_GPIO_IP1SR, .start_bit = 0,  .end_bit = 23,  .bitlen = 4, .val = 0 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_CANFD, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST},
					{ 0 }
				},
				.ctrl = (cpg_ctrl_t[])
				{
					{ .reg = RCAR_V3U_CPG_CANFDCKCR, .val = 0x00000009 },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
	/* FlexRay */
	{
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP2,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 16, .end_bit = 20, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP2SR, .start_bit = 0, .end_bit = 19, .bitlen = 4, .val = 0 },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
	/* PWM (Do not configure PWM while Falcon board do not output to the external pins) */
#if 0
	{
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP3,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 5, .end_bit = 9, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 20, .end_bit = 31, .bitlen = 4, .val = 2 },
					{ .reg = RCAR_V3U_GPIO_IP1SR, .start_bit = 0,  .end_bit = 7,  .bitlen = 4, .val = 2 },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
#endif
	/* HSCIF (Do not configure HSCIF0 while using SCIF0) */
#if 0
	{
		/* HSCIF0 */
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP1,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 0, .end_bit = 5, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP0SR, .start_bit = 4, .end_bit = 23, .bitlen = 4, .val = 0 },
					{ 0 }
				},
			},
			{ 0 }
		},
		.cpg = (cpg_t[])
		{
			{
				.domain = RCAR_V3U_CPG_DM0_BASE,
				.module = (cpg_module_t[])
				{
					{ .mod_id = CPG_MSTP_V3U_HSCIF0, .flags = CPG_FLAGS_EN | CPG_FLAGS_RST },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
#endif
	/* TPU (Do not configure) */
#if 0
	{
		.pfc = (pfc_t[])
		{
			{
				.domain = RCAR_V3U_GPIO_DM0_BASE,
				.gpio_gp = RCAR_V3U_GPIO_GP2,
				.gpsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_GPSR, .start_bit = 21, .end_bit = 24, .val = 1 },
					{ 0 }
				},
				.ipsr = (pfc_reg_t[])
				{
					{ .reg = RCAR_V3U_GPIO_IP2SR, .start_bit = 20, .end_bit = 31, .bitlen = 4, .val = 0 },
					{ 0 }
				},
			},
			{ 0 }
		},
	},
#endif
	/* Last Entry */
	{ .sysc= (sysc_t *)~0, .pfc = (pfc_t *)~0, .cpg = (cpg_t *)~0, }
};

#endif

