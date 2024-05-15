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

/*
 * Xilinx Versal processor with ARMv8 Cortex-A72
 */

#ifndef    __AARCH64_VERSAL_H_INCLUDED
#define    __AARCH64_VERSAL_H_INCLUDED

/*
 * hwinfo Devices
 */
#define VERSAL_HWI_REF_CLK                  "ref_clk"
#define VERSAL_HWI_PMC_ALT_REF_CLK          "pmc_alt_ref_clk"
#define VERSAL_HWI_LPD_ALT_REF_CLK          "lpd_alt_ref_clk"
#define VERSAL_HWI_FPD_ALT_REF_CLK          "fpd_alt_ref_clk"
#define VERSAL_HWI_ENET                     "gem"
#define VERSAL_HWI_ENET_MDIO                "gem,mdio"

/*
 * GIC-500 Registers
 */
#define VERSAL_GICD_BASE_ADDR       0xF9000000
#define VERSAL_GICR_BASE_ADDR       0xF9080000

/*
 * DDR configuration Info
 */
#define VERSAL_DDR0_REGION0_ADDR        0x00000000
#define VERSAL_DDR0_REGION0_SIZE        GIG(2)
#define VERSAL_DDR0_REGION1_ADDR        0x800000000
#define VERSAL_DDR0_REGION1_SIZE        GIG(32)
#define VERSAL_DDR0_REGION2_ADDR        0x00C000000000
#define VERSAL_DDR0_REGION2_SIZE        GIG(256)
#define VERSAL_DDR0_REGION3_ADDR        0x010000000000
#define VERSAL_DDR0_REGION3_SIZE        MEG(734)

/*
 * CRF Registers
 */
#define VERSAL_CRF_BASE                 0xFD1A0000
#define VERSAL_CRF_APLL_CTRL            0x00000040
#define VERSAL_CRF_APLL_TO_XPD_CTRL     0x00000100
#define VERSAL_CRF_ACPU_CTRL            0x0000010C
    #define GET_ACPU_DIVISOR0(x)            ((x >> 8) & 0x3FF)
    #define GET_ACPU_SRCSEL(x)              (x & 0x7)
        #define ACPU_SRCSEL_PPLL                0x0
        #define ACPU_SRCSEL_RPLL                0x1
        #define ACPU_SRCSEL_APLL                0x2
        #define ACPU_SRCSEL_NPLL                0x3

/*
 * CRL registers
 */
#define VERSAL_CRL_BASE                 0xFF5E0000
#define VERSAL_CRL_RPLL_CTRL            0x00000040
#define VERSAL_CRL_RPLL_TO_XPD_CTRL     0x00000100

/*
 * CRP registers
 */
#define VERSAL_CRP_BASE                 0xF1260000
#define VERSAL_CRP_PMCPLL_CTRL          0x00000040
#define VERSAL_CRP_NOCPLL_CTRL          0x00000050
#define VERSAL_CRP_PPLL_TO_XPD_CTRL     0x00000100
#define VERSAL_CRP_NPLL_TO_XPD_CTRL     0x00000104

/*
 * PLL_CTRL register macros
 */
#define GET_PLL_POST_SRC(x)             ((x >> 24) & 0x7)
#define GET_PLL_PRE_SRC(x)              ((x >> 20) & 0x7)
#define GET_PLL_CLKOUTDIV(x)            ((x >> 16) & 0x3)
#define GET_PLL_FBDIV(x)                ((x >> 8) & 0xFF)
#define GET_PLL_BYPASS(x)               ((x >> 3) & 0x1)
#define GET_PLL_RESET(x)                ((x >> 0) & 0x1)

/*
 * PLL_TO_XPD_CTRL register macros
 */
#define GET_PLL_TO_XPD_DIVISOR0(x)      ((x >> 8) & 0x3FF)

/*
 * GEM info
 */
#define VERSAL_GEM0_BASE                0xFF0C0000
#define VERSAL_GEM1_BASE                0xFF0D0000

#define VERSAL_GEM0_IRQ                 88
#define VERSAL_GEM1_IRQ                 90

/*
 * SDMMC info
 */
#define VERSAL_SDIO0_BASE               0xF1040000
#define VERSAL_SDIO1_BASE               0xF1050000
#define VERSAL_SDIO_SWRST_OFFSET        0x2F
    #define VERSAL_SDIO_SWRST_FOR_ALL       0x1

/*
 * RTC info
 */
#define VERSAL_RTC_BASE                 0xF12A0000

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://svn.ott.qnx.com/product/mainline/hardware/startup/lib/public/aarch64/versal.h $ $Rev: 928459 $"
#else
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/mainline/hardware/startup/lib/public/aarch64/versal.h $ $Rev: 928459 $" )
#endif
#endif
