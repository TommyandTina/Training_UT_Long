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


/*
 * Renesas R-Car processor with ARMv8
 * Gen3 shared definitions: H3, M3, V3, E3, D3
 * Gen4 shared definitions: S4,V3U 
 */

#ifndef __AARCH64_RCAR_REG_H_INCLUDED
#define __AARCH64_RCAR_REG_H_INCLUDED

#define BIT(nr)                          (1UL << (nr))

/*
 * Product Register (PRR)
 */
#define RCAR_PRODUCT_REGISTER      0xFFF00044
#define RCAR_PRODUCT_ID(x)         ((x & 0x7F00) >> 8)
 #define PRODUCT_ID_RCAR_H3             0x4F
 #define PRODUCT_ID_RCAR_M3W            0x52
 #define PRODUCT_ID_RCAR_M3N            0x55
 #define PRODUCT_ID_RCAR_V3M            0x54
 #define PRODUCT_ID_RCAR_V3H            0x56
 #define PRODUCT_ID_RCAR_E3             0x57
 #define PRODUCT_ID_RCAR_D3             0x58
 #define PRODUCT_ID_RCAR_V3U            0x59
 #define PRODUCT_ID_RCAR_S4             0x5A
 #define PRODUCT_ID_RCAR_V4H            0x5C
 #define PRODUCT_ID_RCAR_UNKNOWN        (~0)

#define IS_RCAR_GEN3(_id)         ((_id == PRODUCT_ID_RCAR_H3)  || \
                                   (_id == PRODUCT_ID_RCAR_M3W) || (_id == PRODUCT_ID_RCAR_M3N) || \
                                   (_id == PRODUCT_ID_RCAR_V3M) || (_id == PRODUCT_ID_RCAR_V3H) || \
                                   (_id == PRODUCT_ID_RCAR_D3)  || (_id == PRODUCT_ID_RCAR_E3))

#define IS_RCAR_GEN4(_id)         ((_id == PRODUCT_ID_RCAR_V3U) || (_id == PRODUCT_ID_RCAR_S4))

#define RCAR_PRODUCT_REV(x)        (x & 0xFF)
 #define PRODUCT_REV_1_0                0x00
 #define PRODUCT_REV_1_1                0x01
 #define PRODUCT_REV_1_2                0x10
 #define PRODUCT_REV_1_3                0x11 
 #define PRODUCT_REV_2_0                0x10
 #define PRODUCT_REV_3_0                0x20
 #define PRODUCT_REV_UNKNOWN           (~0)
 
 #define PRODUCT_H3_REV_1_0             0x00
 #define PRODUCT_H3_REV_1_1             0x01
 #define PRODUCT_H3_REV_2_0             0x10
 #define PRODUCT_H3_REV_3_0             0x20
 #define PRODUCT_M3_REV_1_0             0x00
 #define PRODUCT_M3_REV_1_1             0x10
 #define PRODUCT_M3_REV_1_2             0x10
 #define PRODUCT_M3_REV_1_3             0x11
 #define PRODUCT_M3_REV_3_0             0x20
 #define PRODUCT_M3N_REV_1_0            0x00
 #define PRODUCT_M3N_REV_1_1            0x01
 #define PRODUCT_E3_REV_1_0             0x00
 #define PRODUCT_E3_REV_1_1             0x01
 #define PRODUCT_D3_REV_1_0             0x00
 #define PRODUCT_D3_REV_1_1             0x10
 #define PRODUCT_V3M_REV_1_0            0x00
 #define PRODUCT_V3M_REV_2_0            0x10
 #define PRODUCT_V3H_REV_1_0            0x00
 #define PRODUCT_V3H_REV_1_1            0x01
 #define PRODUCT_V3H_REV_2_0            0x10
 #define PRODUCT_V3H_REV_2_1            0x11

#define MPIDR_A76_CLUSTER_ID(x)         (((x) >> 16) & 0xFF)
#define MPIDR_CLUSTER_ID(x)             (((x) >>  8) & 0xFF)
#define MIDR_CPU_ID(x)                  (((x) >>  4) & 0xFFF)
#define MIDR_CPU_A53_ID                 0xD03
#define MIDR_CPU_A55_ID                 0xD05
#define MIDR_CPU_A57_ID                 0xD07
#define MIDR_CPU_A76_ID                 0xD0B
 
#define RCAR_GEN3_PRODUCT_CA57EN(x)     ((x >> 27) & 0xF)
#define RCAR_GEN3_PRODUCT_CA53EN(x)     ((x >> 22) & 0xF)

#define RCAR_GEN4_CLUSTERS_NUM_MAX      4
#define RCAR_GEN4_CORES_NUM_MAX         8
#define RCAR_GEN4_MAX_CORE_PER_CLUSTER  2
#define RCAR_GEN4_PRODUCT_CAEN(x,y)     ((((x >> 20) & 0xFFF) >> (y * 3)) & 3)


/*--------------------------------------------------
 * 
 * Macros is for Gen3 (H3, M3, V3, E3, D3)
 * 
 *--------------------------------------------------*/

/*
 * Board-Specific Defines for Gen3 Reference Boards
 */
/* External Crystal Clock */
#define RCAR_GEN3_EXT_CLK_16MHZ          16666667
#define RCAR_GEN3_EXT_CLK_20MHZ          20000000
#define RCAR_GEN3_EXT_CLK_25MHZ          25000000
#define RCAR_GEN3_EXT_CLK_33MHZ          33333333
#define RCAR_GEN3_EXT_CLK                16666667        /* External crystal clock */
#define RCAR_GEN3_EXT_CLK_XS             16640000        /* Salvator-XS */
#define RCAR_GEN3_EXT_CLK_D3             48000000        /* D3*/

/*
 * GPIO
 */
#define RCAR_GEN3_GPIO_BASE              0xE6050000
#define RCAR_GEN3_GPIO_SIZE              0x1000

#define RCAR_GEN3_GPIO0_BASE             0xE6050000
#define RCAR_GEN3_GPIO1_BASE             0xE6051000
#define RCAR_GEN3_GPIO2_BASE             0xE6052000
#define RCAR_GEN3_GPIO3_BASE             0xE6053000
#define RCAR_GEN3_GPIO4_BASE             0xE6054000
#define RCAR_GEN3_GPIO5_BASE             0xE6055000
#define RCAR_GEN3_GPIO6_BASE             0xE6055400
#define RCAR_GEN3_GPIO7_BASE             0xE6055800

 // GPIO Registers, offset from GPIO_BASE
 #define RCAR_GEN3_GPIO_IOINTSEL         0x0000
 #define RCAR_GEN3_GPIO_INOUTSEL         0x0004
 #define RCAR_GEN3_GPIO_OUTDT            0x0008
 #define RCAR_GEN3_GPIO_INDT             0x000C
 #define RCAR_GEN3_GPIO_INTDT            0x0010
 #define RCAR_GEN3_GPIO_INTCLR           0x0014
 #define RCAR_GEN3_GPIO_INTMSK           0x0018
 #define RCAR_GEN3_GPIO_MSKCLR           0x001C
 #define RCAR_GEN3_GPIO_POSNEG           0x0020
 #define RCAR_GEN3_GPIO_EDGLEVEL         0x0024
 #define RCAR_GEN3_GPIO_FILONOFF         0x0028
 #define RCAR_GEN3_GPIO_INTMSKS          0x0038
 #define RCAR_GEN3_GPIO_MSKCLRS          0x003C
 #define RCAR_GEN3_GPIO_OUTDTSEL         0x0040
 #define RCAR_GEN3_GPIO_OUTDTH           0x0044
 #define RCAR_GEN3_GPIO_OUTDTL           0x0048
 #define RCAR_GEN3_GPIO_BOTHEDGE         0x004C


/*
 * Pin Function Control
 */
#define RCAR_GEN3_PFC_BASE               0xE6060000
#define RCAR_GEN3_PFC_SIZE               0x1000

/* PFC registers, offset from RCAR_GEN3_PFC_BASE */
#define RCAR_GEN3_PFC_PMMR              0x000
#define RCAR_GEN3_PFC_GPSR0             0x100
#define RCAR_GEN3_PFC_GPSR1             0x104
#define RCAR_GEN3_PFC_GPSR2             0x108
#define RCAR_GEN3_PFC_GPSR3             0x10C
#define RCAR_GEN3_PFC_GPSR4             0x110
#define RCAR_GEN3_PFC_GPSR5             0x114
#define RCAR_GEN3_PFC_GPSR6             0x118
#define RCAR_GEN3_PFC_GPSR7             0x11C
#define RCAR_GEN3_PFC_IPSR0             0x200
#define RCAR_GEN3_PFC_IPSR1             0x204
#define RCAR_GEN3_PFC_IPSR2             0x208
#define RCAR_GEN3_PFC_IPSR3             0x20C
#define RCAR_GEN3_PFC_IPSR4             0x210
#define RCAR_GEN3_PFC_IPSR5             0x214
#define RCAR_GEN3_PFC_IPSR6             0x218
#define RCAR_GEN3_PFC_IPSR7             0x21C
#define RCAR_GEN3_PFC_IPSR8             0x220
#define RCAR_GEN3_PFC_IPSR9             0x224
#define RCAR_GEN3_PFC_IPSR10            0x228
#define RCAR_GEN3_PFC_IPSR11            0x22C
#define RCAR_GEN3_PFC_IPSR12            0x230
#define RCAR_GEN3_PFC_IPSR13            0x234
#define RCAR_GEN3_PFC_IPSR14            0x238
#define RCAR_GEN3_PFC_IPSR15            0x23C
#define RCAR_GEN3_PFC_IPSR16            0x240
#define RCAR_GEN3_PFC_IPSR17            0x244
#define RCAR_GEN3_PFC_IPSR18            0x248
#define RCAR_GEN3_PFC_DRVCTRL0          0x300
#define RCAR_GEN3_PFC_DRVCTRL1          0x304
#define RCAR_GEN3_PFC_DRVCTRL2          0x308
#define RCAR_GEN3_PFC_DRVCTRL3          0x30C
#define RCAR_GEN3_PFC_DRVCTRL4          0x310
#define RCAR_GEN3_PFC_DRVCTRL5          0x314
#define RCAR_GEN3_PFC_DRVCTRL6          0x318
#define RCAR_GEN3_PFC_DRVCTRL7          0x31C
#define RCAR_GEN3_PFC_DRVCTRL8          0x320
#define RCAR_GEN3_PFC_DRVCTRL9          0x324
#define RCAR_GEN3_PFC_DRVCTRL10         0x328
#define RCAR_GEN3_PFC_DRVCTRL11         0x32C
#define RCAR_GEN3_PFC_DRVCTRL12         0x330
#define RCAR_GEN3_PFC_DRVCTRL13         0x334
#define RCAR_GEN3_PFC_DRVCTRL14         0x338
#define RCAR_GEN3_PFC_DRVCTRL15         0x33C
#define RCAR_GEN3_PFC_DRVCTRL16         0x340
#define RCAR_GEN3_PFC_DRVCTRL17         0x344
#define RCAR_GEN3_PFC_DRVCTRL18         0x348
#define RCAR_GEN3_PFC_DRVCTRL19         0x34C
#define RCAR_GEN3_PFC_DRVCTRL20         0x350
#define RCAR_GEN3_PFC_DRVCTRL21         0x354
#define RCAR_GEN3_PFC_DRVCTRL22         0x358
#define RCAR_GEN3_PFC_DRVCTRL23         0x35C
#define RCAR_GEN3_PFC_DRVCTRL24         0x360
#define RCAR_GEN3_PFC_IOCTRL0           0x300
#define RCAR_GEN3_PFC_IOCTRL1           0x304
#define RCAR_GEN3_PFC_IOCTRL2           0x308
#define RCAR_GEN3_PFC_IOCTRL3           0x30C
#define RCAR_GEN3_PFC_IOCTRL4           0x310
#define RCAR_GEN3_PFC_IOCTRL5           0x314
#define RCAR_GEN3_PFC_IOCTRL6           0x318
#define RCAR_GEN3_PFC_IOCTRL7           0x31C
#define RCAR_GEN3_PFC_IOCTRL8           0x320
#define RCAR_GEN3_PFC_IOCTRL9           0x324
#define RCAR_GEN3_PFC_IOCTRL10          0x328
#define RCAR_GEN3_PFC_IOCTRL11          0x32C
#define RCAR_GEN3_PFC_IOCTRL12          0x330
#define RCAR_GEN3_PFC_IOCTRL13          0x334
#define RCAR_GEN3_PFC_IOCTRL14          0x338
#define RCAR_GEN3_PFC_IOCTRL15          0x33C
#define RCAR_GEN3_PFC_IOCTRL16          0x340
#define RCAR_GEN3_PFC_IOCTRL17          0x344
#define RCAR_GEN3_PFC_IOCTRL18          0x348
#define RCAR_GEN3_PFC_IOCTRL19          0x34C
#define RCAR_GEN3_PFC_IOCTRL30          0x380
#define RCAR_GEN3_PFC_IOCTRL31          0x384
#define RCAR_GEN3_PFC_IOCTRL32          0x388
#define RCAR_GEN3_PFC_IOCTRL33          0x38C
#define RCAR_GEN3_PFC_IOCTRL40          0x3C0
#define RCAR_GEN3_PFC_POCCTRL0          0x380
#define RCAR_GEN3_PFC_POCCTRL1          0x384
#define RCAR_GEN3_PFC_POCCTRL2          0x388
#define RCAR_GEN3_PFC_TDSELCTRL0        0x3C0
#define RCAR_GEN3_PFC_IOCTRL            0x3E0
#define RCAR_GEN3_PFC_FUSEMON0          0x3E4
#define RCAR_GEN3_PFC_FUSEMON1          0x3E8        // Fuse Monitor Register 1
#define RCAR_GEN3_PFC_FUSEMON2          0x3EC
#define RCAR_GEN3_PFC_FUSEMON3          0x3F0
#define RCAR_GEN3_PFC_PUEN0             0x400
#define RCAR_GEN3_PFC_PUEN1             0x404
#define RCAR_GEN3_PFC_PUEN2             0x408
#define RCAR_GEN3_PFC_PUEN3             0x40C
#define RCAR_GEN3_PFC_PUEN4             0x410
#define RCAR_GEN3_PFC_PUEN5             0x414
#define RCAR_GEN3_PFC_PUEN6             0x418
#define RCAR_GEN3_PFC_PUD0              0x440
#define RCAR_GEN3_PFC_PUD1              0x444
#define RCAR_GEN3_PFC_PUD2              0x448
#define RCAR_GEN3_PFC_PUD3              0x44C
#define RCAR_GEN3_PFC_PUD4              0x450
#define RCAR_GEN3_PFC_PUD5              0x454
#define RCAR_GEN3_PFC_PUD6              0x458
#define RCAR_GEN3_PFC_MODSEL0           0x500
#define RCAR_GEN3_PFC_MODSEL1           0x504
#define RCAR_GEN3_PFC_MODSEL2           0x508

#define RCAR_GEN3_PFC_PUPR0             0x100
#define RCAR_GEN3_PFC_PUPR1             0x104
#define RCAR_GEN3_PFC_PUPR2             0x108
#define RCAR_GEN3_PFC_PUPR3             0x10C
#define RCAR_GEN3_PFC_PUPR4             0x110
#define RCAR_GEN3_PFC_PUPR5             0x114
#define RCAR_GEN3_PFC_PUPR6             0x118
#define RCAR_GEN3_PFC_IOCTL             0x70
#define RCAR_GEN3_PFC_IOCTL0            0x60
#define RCAR_GEN3_PFC_IOCTL1            0x64
#define RCAR_GEN3_PFC_IOCTL4            0x84
#define RCAR_GEN3_PFC_IOCTL5            0x88
#define RCAR_GEN3_PFC_IOCTL6            0x8C
#define RCAR_GEN3_PFC_DDR3GPEN          0x240
#define RCAR_GEN3_PFC_DDR3GPOE          0x244
#define RCAR_GEN3_PFC_DDR3GPOD          0x248
#define RCAR_GEN3_PFC_DDR3GPID          0x24C

/* MFIS */
#define RCAR_GEN3_MFIS_ADDR_0 (0xE62600C0U)   /* MFI lock register top addr         */
#define RCAR_GEN3_MFIS_ADDR_1 (0xE6260200U)   /* MFI error control register top addr*/
#define RCAR_GEN3_MFIS_ADDR_2 (0xE6260300U)   /* MFI safety register top addr       */
#define RCAR_GEN3_MFIS_ADDR_3 (0xE6260400U)   /* MFI communication register top addr*/
#define RCAR_GEN3_MFIS_ADDR_4 (0xE62608A0U)   /* MFI external register top addr     */
#define RCAR_GEN3_MFIS_ADDR_5 (0xE6260900U)   /* MFI write control register top addr*/
#define RCAR_GEN3_MFIS_CODE    0xACCE
#define RCAR_GEN3_MFIS_WPD     0x1

/* MFIS */
#define RCAR_GEN3_MFIS_BASE              (0xE6260000)
/* MFIS registers */
#define RCAR_GEN3_MFIS_WPCNTR            (0x900)   /* Write Protection Control Register*/
#define RCAR_GEN3_MFIS_WPCNTR_CODE       (0xACCE << 16)
#define RCAR_GEN3_MFIS_WPCNTR_WPD        (0x1 << 0)

/*
 * CPG
 */
#define RCAR_GEN3_CPG_BASE               0xE6150000
#define RCAR_GEN3_CPG_SIZE               0x1000

/* CPG registers, offset from RCAR_GEN3_CPG_BASE */
#define RCAR_GEN3_CPG_CPGWPCR           0x904       // CPG Write Protect Control Register
#define RCAR_GEN3_CPG_CPGWPR            0x900       // CPG Write Protect Register
#define RCAR_GEN3_CPG_FRQCRB            0x004       // Frequency Control Register B
#define RCAR_GEN3_CPG_MSOCKCR           0x014       // MSIOF clock frequency control register
#define RCAR_GEN3_CPG_FRQCRC            0x0E0       // Frequency Control Register C
#define RCAR_GEN3_CPG_PLLECR            0x0D0       // PLL Enable Control Register
#define RCAR_GEN3_CPG_PLL0CR            0x0D8       // PLL0 Control Register
#define RCAR_GEN3_CPG_PLL1CR            0x028       // PLL1 Control Register
#define RCAR_GEN3_CPG_PLL2CR            0x02C       // PLL2 Control Register
#define RCAR_GEN3_CPG_PLL3CR            0x0DC       // PLL3 Control Register
#define RCAR_GEN3_CPG_PLL4CR            0x1F4       // PLL4 Control Register
#define RCAR_GEN3_CPG_PLL0STPCR         0x0F0       // PLL0 Stop Condition Register
#define RCAR_GEN3_CPG_PLL2STPCR         0x0F8       // PLL2 Stop Condition Register
#define RCAR_GEN3_CPG_PLL3STPCR         0x0FC       // PLL3 Stop Condition Register
#define RCAR_GEN3_CPG_PLL4STPCR         0x1F8       // PLL4 Stop Condition Register
#define RCAR_GEN3_CPG_CSI0CKCR          0x00C       // CSI0 clock frequency control register
#define RCAR_GEN3_CPG_POST3CKCR         0x01C       // Power-on self test 3 clock frequency control register
#define RCAR_GEN3_CPG_RGXCR             0x0B4       // RGX Control Register
/* SD0CKCR, SD1CKCR and SD3CKCR are not applied to D3 */
#define RCAR_GEN3_CPG_SD0CKCR           0x074       // SDHI 0 Clock Frequency Control Register
#define RCAR_GEN3_CPG_SD1CKCR           0x078       // SDHI 1 Clock Frequency Control Register
#define RCAR_GEN3_CPG_POSTCKCR          0x08C       // Power-on self test clock frequency control register
#define RCAR_GEN3_CPG_POST2CKCR         0x09C       // Power-on self test 2 clock frequency control register
#define RCAR_GEN3_CPG_RCKCR             0x240       // RCLK frequency control Register
#define RCAR_GEN3_CPG_SD2CKCR           0x268       // SDHI 2 Clock Frequency Control Register
#define RCAR_GEN3_CPG_SD3CKCR           0x26C       // SDHI 3 Clock Frequency Control Register
#define RCAR_GEN3_CPG_GPUCKCR           0x234       // GPU Clock Frequency Control Register
#define RCAR_GEN3_CPG_RPCCKCR           0x238       // R/W  32  RPC clock frequency control register
#define RCAR_GEN3_CPG_CANFDCKCR         0x244       // CAN-FD Clock Frequency Control Register
#define RCAR_GEN3_CPG_POST4CKCR         0x260       // Power-on self test 4 clock frequency control register
#define RCAR_GEN3_CPG_ZA2CKCR           0x4DC       // ZA2CKCR Clock Frequency Control Register
#define RCAR_GEN3_CPG_ADSPCKCR          0x25C       // ADSP Clock Frequency Control Register
#define RCAR_GEN3_CPG_SSPCKCR           0x248       // SSP Clock Frequency Control Register
#define RCAR_GEN3_CPG_SSPRSCKCR         0x24C       // SSPRS Clock Frequency Control Register
#define RCAR_GEN3_CPG_HDMICKCR          0x250       // HDMI-IF Clock Frequency Control Register
#define RCAR_GEN3_CPG_DVFSCR0           0x058       // DVFS Control Register 0
#define RCAR_GEN3_CPG_DVFSCR1           0x05C       // DVFS Control Register 1
#define RCAR_GEN3_CPG_ADSPCKCR          0x25C       // ADSP Clock Frequency Control Register
#define RCAR_GEN3_CPG_SSPCKCR           0x248       // SSP Clock Frequency Control Register
#define RCAR_GEN3_CPG_HDMICKCR          0x250       // SSP Clock Frequency Control Register
#define RCAR_GEN3_CPG_CSI0CKCR          0x00C       // CSI0 Clock Frequency Control Register
#define RCAR_GEN3_CPG_SSPRSCKCR         0x24C       // SSPRS Clock Frequency Control Register
#define RCAR_GEN3_CPG_RCANCKCR          0x244       // RCAN Clock Frequency Control Register
#define RCAR_GEN3_CPG_FMMCKCR           0x274       // FMM Clock Frequency Control Register
#define RCAR_GEN3_CPG_DVFSCR0           0x058       // DVFS Control Register 0
#define RCAR_GEN3_CPG_DVFSCR1           0x05C       // DVFS Control Register 1

#define RCAR_GEN3_CPG_MSTPSR0           0x030       // Module Stop Status Register 0
#define RCAR_GEN3_CPG_MSTPSR1           0x038       // Module Stop Status Register 1
#define RCAR_GEN3_CPG_MSTPSR2           0x040       // Module Stop Status Register 2
#define RCAR_GEN3_CPG_MSTPSR3           0x048       // Module Stop Status Register 3
#define RCAR_GEN3_CPG_MSTPSR4           0x04C       // Module Stop Status Register 4
#define RCAR_GEN3_CPG_MSTPSR5           0x03C       // Module Stop Status Register 5
#define RCAR_GEN3_CPG_MSTPSR6           0x1C0       // Module Stop Status Register 6
#define RCAR_GEN3_CPG_MSTPSR7           0x1C4       // Module Stop Status Register 7
#define RCAR_GEN3_CPG_MSTPSR8           0x9A0       // Module Stop Status Register 8
#define RCAR_GEN3_CPG_MSTPSR9           0x9A4       // Module Stop Status Register 9
#define RCAR_GEN3_CPG_MSTPSR10          0x9A8       // Module Stop Status Register 10
#define RCAR_GEN3_CPG_MSTPSR11          0x9AC       // Module Stop Status Register 11

#define RCAR_GEN3_CPG_RMSTPCR0          0x110       // Realtime Module Stop Control Register 0
#define RCAR_GEN3_CPG_RMSTPCR1          0x114       // Realtime Module Stop Control Register 1
#define RCAR_GEN3_CPG_RMSTPCR2          0x118       // Realtime Module Stop Control Register 2
#define RCAR_GEN3_CPG_RMSTPCR3          0x11C       // Realtime Module Stop Control Register 3
#define RCAR_GEN3_CPG_RMSTPCR4          0x120       // Realtime Module Stop Control Register 4
#define RCAR_GEN3_CPG_RMSTPCR5          0x124       // Realtime Module Stop Control Register 5
#define RCAR_GEN3_CPG_RMSTPCR6          0x128       // Realtime Module Stop Control Register 6
#define RCAR_GEN3_CPG_RMSTPCR7          0x12C       // Realtime Module Stop Control Register 7
#define RCAR_GEN3_CPG_RMSTPCR8          0x980       // Realtime Module Stop Control Register 8
#define RCAR_GEN3_CPG_RMSTPCR9          0x984       // Realtime Module Stop Control Register 9
#define RCAR_GEN3_CPG_RMSTPCR10         0x988       // Realtime Module Stop Control Register 10
#define RCAR_GEN3_CPG_RMSTPCR11         0x98C       // Realtime Module Stop Control Register 11

#define RCAR_GEN3_CPG_SMSTPCR0          0x130       // System Module Stop Control Register 0
#define RCAR_GEN3_CPG_SMSTPCR1          0x134       // System Module Stop Control Register 1
#define RCAR_GEN3_CPG_SMSTPCR2          0x138       // System Module Stop Control Register 2
#define RCAR_GEN3_CPG_SMSTPCR3          0x13C       // System Module Stop Control Register 3
#define RCAR_GEN3_CPG_SMSTPCR4          0x140       // System Module Stop Control Register 4
#define RCAR_GEN3_CPG_SMSTPCR5          0x144       // System Module Stop Control Register 5
#define RCAR_GEN3_CPG_SMSTPCR6          0x148       // System Module Stop Control Register 6
#define RCAR_GEN3_CPG_SMSTPCR7          0x14C       // System Module Stop Control Register 7
#define RCAR_GEN3_CPG_SMSTPCR8          0x990       // System Module Stop Control Register 8
#define RCAR_GEN3_CPG_SMSTPCR9          0x994       // System Module Stop Control Register 9
#define RCAR_GEN3_CPG_SMSTPCR10         0x998       // System Module Stop Control Register 10
#define RCAR_GEN3_CPG_SMSTPCR11         0x99C       // System Module Stop Control Register 11

#define RCAR_GEN3_CPG_SRCR0             0x0A0       // Software Reset Register 0
#define RCAR_GEN3_CPG_SRCR1             0x0A8       // Software Reset Register 1
#define RCAR_GEN3_CPG_SRCR2             0x0B0       // Software Reset Register 2
#define RCAR_GEN3_CPG_SRCR3             0x0B8       // Software Reset Register 3
#define RCAR_GEN3_CPG_SRCR4             0x0BC       // Software Reset Register 4
#define RCAR_GEN3_CPG_SRCR5             0x0C4       // Software Reset Register 5
#define RCAR_GEN3_CPG_SRCR6             0x1C8       // Software Reset Register 6
#define RCAR_GEN3_CPG_SRCR7             0x1CC       // Software Reset Register 7
#define RCAR_GEN3_CPG_SRCR8             0x920       // Software Reset Register 8
#define RCAR_GEN3_CPG_SRCR9             0x924       // Software Reset Register 9
#define RCAR_GEN3_CPG_SRCR10            0x928       // Software Reset Register 10
#define RCAR_GEN3_CPG_SRCR11            0x92C       // Software Reset Register 11

#define RCAR_GEN3_CPG_SRSTCLR0          0x940       // Software Reset Clear Register 0
#define RCAR_GEN3_CPG_SRSTCLR1          0x944       // Software Reset Clear Register 1
#define RCAR_GEN3_CPG_SRSTCLR2          0x948       // Software Reset Clear Register 2
#define RCAR_GEN3_CPG_SRSTCLR3          0x94C       // Software Reset Clear Register 3
#define RCAR_GEN3_CPG_SRSTCLR4          0x950       // Software Reset Clear Register 4
#define RCAR_GEN3_CPG_SRSTCLR5          0x954       // Software Reset Clear Register 5
#define RCAR_GEN3_CPG_SRSTCLR6          0x958       // Software Reset Clear Register 6
#define RCAR_GEN3_CPG_SRSTCLR7          0x95C       // Software Reset Clear Register 7
#define RCAR_GEN3_CPG_SRSTCLR8          0x960       // Software Reset Clear Register 8
#define RCAR_GEN3_CPG_SRSTCLR9          0x964       // Software Reset Clear Register 9
#define RCAR_GEN3_CPG_SRSTCLR10         0x968       // Software Reset Clear Register 10
#define RCAR_GEN3_CPG_SRSTCLR11         0x96C       // Software Reset Clear Register 11

/* 
 * IPMMU for Gen3
 */
#define RCAR_GEN3_IPMMU_MM_BASE                    (0xE67B0000U)   /* IPMMU-MM */
#define RCAR_GEN3_IPMMUMM_IMSCTLR                  (RCAR_GEN3_IPMMU_MM_BASE + 0x0500U)
#define RCAR_GEN3_IPMMUMM_IMAUXCTLR                (RCAR_GEN3_IPMMU_MM_BASE + 0x0504U)
 #define RCAR_GEN3_IPMMUMM_IMSCTLR_ENABLE           (0xC0000000U)
 #define RCAR_GEN3_IPMMUMM_IMAUXCTLR_NMERGE40_BIT   (0x01000000U)
 #define RCAR_GEN3_IMSCTLR_DISCACHE                 (0xE0000000U)

#define RCAR_GEN3_IPMMU_VI0_BASE                   (0xFEBD0000U)   /* IPMMU-VI0 */
#define RCAR_GEN3_IPMMUVI0_IMSCTLR                 (RCAR_GEN3_IPMMU_VI0_BASE + 0x0500U) 

#define RCAR_GEN3_IPMMU_VI1_BASE                   (0xFEBE0000U)   /* IPMMU-VI1 */
#define RCAR_GEN3_IPMMUVI1_IMSCTLR                 (RCAR_GEN3_IPMMU_VI1_BASE + 0x0500U)

#define RCAR_GEN3_IPMMU_PV0_BASE                   (0xFD800000U)   /* IPMMU-PV0 */
#define RCAR_GEN3_IPMMUPV0_IMSCTLR                 (RCAR_GEN3_IPMMU_PV0_BASE + 0x0500U) 

#define RCAR_GEN3_IPMMU_PV1_BASE                   (0xFD950000U)   /* IPMMU-PV1 */
#define RCAR_GEN3_IPMMUPV1_IMSCTLR                 (RCAR_GEN3_IPMMU_PV1_BASE + 0x0500U) 

#define RCAR_GEN3_IPMMU_PV2_BASE                   (0xFD960000U)   /* IPMMU-PV2 */
#define RCAR_GEN3_IPMMUPV2_IMSCTLR                 (RCAR_GEN3_IPMMU_PV2_BASE + 0x0500U)

#define RCAR_GEN3_IPMMU_PV3_BASE                   (0xFD970000U)   /* IPMMU-PV3 */
#define RCAR_GEN3_IPMMUPV3_IMSCTLR                 (RCAR_GEN3_IPMMU_PV3_BASE + 0x0500U)

#define RCAR_GEN3_IPMMU_HC_BASE                    (0xE6570000U)   /* IPMMU-HC */
#define RCAR_GEN3_IPMMUHC_IMSCTLR                  (RCAR_GEN3_IPMMU_HC_BASE + 0x0500U)

#define RCAR_GEN3_IPMMU_RT_BASE                    (0xFFC80000U)   /* IPMMU-RT */
#define RCAR_GEN3_IPMMURT_IMSCTLR                  (RCAR_GEN3_IPMMU_RT_BASE + 0x0500U)

#define RCAR_GEN3_IPMMU_MP_BASE                    (0xEC670000U)   /* IPMMU-MP */
#define RCAR_GEN3_IPMMUMP_IMSCTLR                  (RCAR_GEN3_IPMMU_MP_BASE + 0x0500U)

#define RCAR_GEN3_IPMMU_DS0_BASE                   (0xE6740000U)   /* IPMMU-DS0 */
#define RCAR_GEN3_IPMMUDS0_IMSCTLR                 (RCAR_GEN3_IPMMU_DS0_BASE + 0x0500U)

#define RCAR_GEN3_IPMMU_DS1_BASE                   (0xE7740000U)   /* IPMMU-DS1 */
#define RCAR_GEN3_IPMMUDS1_IMSCTLR                 (RCAR_GEN3_IPMMU_DS1_BASE + 0x0500U)

/*
 * Advanced Power Management Unit registers
 */
#define RCAR_GEN3_APMU_BASE               0xE6150000
#define RCAR_GEN3_APMU_SIZE               0x4000

#define RCAR_GEN3_APMU_CA53WUPCR         0x1010
#define RCAR_GEN3_APMU_CA57WUPCR         0x2010

#define RCAR_GEN3_APMU_CA53CPUCMCR       0x1184      // Common power control register for A53
#define RCAR_GEN3_APMU_CA57CPUCMCR       0x2184      // Common power control register for A57

/*
 * Reset registers
 */
#define RCAR_GEN3_RESET_BASE                   0xE6160000

/* Offset from base */
#define RCAR_GEN3_WDTRSTCR                     0x54        // Watchdog reset control register
#define RCAR_GEN3_MODEMR                       0x60        // Mode Monitor Register
#define RCAR_GEN3_MODEMR_BOOT_MASK             0x1E
 #define RCAR_GEN3_MODEMR_BOOT_HYPERFLASH160   0x4
 #define RCAR_GEN3_MODEMR_BOOT_HYPERFLASH80    0x6
 #define RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH40    0x8
 #define RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH80    0xC
#define RCAR_GEN3_RESET_CA57RESCNT             0x40
#define RCAR_GEN3_RESET_CA53RESCNT             0x44
#define RCAR_GEN3_WDTRSTCR                     0x54        // Watchdog reset control register
#define RCAR_GEN3_MODEMR                       0x60        // Mode Monitor Register
 #define RCAR_GEN3_MODEMR_BOOT_MASK            0x1E
 #define RCAR_GEN3_MODEMR_BOOT_HYPERFLASH160   0x4
 #define RCAR_GEN3_MODEMR_BOOT_HYPERFLASH80    0x6
 #define RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH40    0x8
 #define RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH80    0xC
#define RCAR_GEN3_RESET_CA53CPUxBARH(x)        (0x80 + (0x10 * x))
#define RCAR_GEN3_RESET_CA53CPUxBARL(x)        (0x84 + (0x10 * x))
#define RCAR_GEN3_RESET_CA57CPUxBARH(x)        (0xC0 + (0x10 * x))
#define RCAR_GEN3_RESET_CA57CPUxBARL(x)        (0xC4 + (0x10 * x))


/*
 * System Controller
 */
#define RCAR_GEN3_SYSC_BASE              0xE6180000
#define RCAR_GEN3_SYSC_SIZE              0x1000
#define RCAR_GEN3_SYSC_SR               0x00        // Status Register
#define RCAR_GEN3_SYSC_ISR              0x04        // Interrupt Status Register
#define RCAR_GEN3_SYSC_ISCR             0x08        // Interrupt Status Clear Register
#define RCAR_GEN3_SYSC_IER              0x0C        // Interrupt Enable Register
#define RCAR_GEN3_SYSC_IMR              0x10        // Interrupt Mask Register
#define RCAR_GEN3_SYSC_WUPMSKCA57       0x14
#define RCAR_GEN3_SYSC_WUPMSKCA53       0x18
#define RCAR_GEN3_SYSC_EERSR            0x20        // External Event Request Status Register
#define RCAR_GEN3_SYSC_EERSCR           0x24        // External Event Request Status Clear Register
#define RCAR_GEN3_SYSC_EERSER           0x28        // External Event Request Status Enable Register
#define RCAR_GEN3_SYSC_EERSR2           0x2C        // External Event Request Status Register 2
#define RCAR_GEN3_SYSC_EERSCR2          0x30        // External Event Request Status Clear register 2
#define RCAR_GEN3_SYSC_EERSER2          0x34        // External Event Request Status Enable Register 2
#define RCAR_GEN3_SYSC_EERSR3           0x40        // External Event Request Status Register 3
#define RCAR_GEN3_SYSC_EERSCR3          0x44        // External Event Request Status Clear register 3
#define RCAR_GEN3_SYSC_EERSER3          0x48        // External Event Request Status Enable Register 3
#define RCAR_GEN3_SYSC_PCPR             0x4C        // Power control protection register
#define RCAR_GEN3_SYSC_PCPER            0x50        // Power control protection error status register
#define RCAR_GEN3_SYSC_PCPERR           0x54        // Power control protection error request register
#define RCAR_GEN3_SYSC_CPDMD            0x64        // Power Domain Grouping Mode Control Register
 #define RCAR_GEN3_SYSC_CPDMD_PDMODE_0  0           // Power mode 0
 #define RCAR_GEN3_SYSC_CPDMD_PDMODE_1  1           // Power mode 1
 #define RCAR_GEN3_SYSC_CPDMD_PDMODE_3  3           // Power mode 3
/* CA57 */
#define RCAR_GEN3_SYSC_PWRSR0           0x80        // Power status register 0 (CA57)
#define RCAR_GEN3_SYSC_PWROFFSR0        0x88        // Power shutoff status register 0 (CA57)
#define RCAR_GEN3_SYSC_PWRONSR0         0x90        // Power resume status register 0 (CA57)
#define RCAR_GEN3_SYSC_PWRER0           0x94        // Power shutoff/resume error register 0 (CA57)
#define RCAR_GEN3_SYSC_PWRPSEU0         0xb8        // Power pseudo shutoff register 0 (CA57)
#define RCAR_GEN3_SYSC_PWRISOER0        0xbc        // Power isolation error detection register 0 (CA57)

/* 3DG */
#define RCAR_GEN3_SYSC_PWRSR2           0x100        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR2        0x104        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR2        0x108        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR2         0x10C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR2         0x110        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER2           0x114        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU2         0x138        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER2        0x13C        // Power shutoff/resume Error Register

/* CA53-SCU */
#define RCAR_GEN3_SYSC_PWRSR3           0x140        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR3        0x144        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR3        0x148        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR3         0x14C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR3         0x150        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER3           0x154        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU3         0x178        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER3        0x17C        // Power shutoff/resume Error Register

/* IMP */
#define RCAR_GEN3_SYSC_PWRSR4           0x180        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR4        0x184        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR4        0x188        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR4         0x18C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR4         0x190        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER4           0x194        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_EXTMASK          0x1B0        // Power control protection error register
#define RCAR_GEN3_SYSC_PWRPSEU4         0x1B8        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER4        0x1BC        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_EXTMASK_V3H      0x138        // External Request Mask register for V3M
#define RCAR_GEN3_SYSC_EXTMASK_V3M      0x1B0        // External Request Mask register for V3H
#define RCAR_GEN3_SYSC_EXTMASK_H3M3NE3  0x2F8        // External Request Mask register for H3, H3N, M3W+, M3N, E3

/* CA57-SCU */
#define RCAR_GEN3_SYSC_PWRSR5           0x1C0        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR5        0x1C4        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR5        0x1C8        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR5         0x1CC        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR5         0x1D0        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER5           0x1D4        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU5         0x1F8        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER5        0x1FC        // Power shutoff/resume Error Register

/* CA53-SCU */
#define RCAR_GEN3_SYSC_PWRSR6           0x200        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFSR6        0x208        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONSR6         0x210        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER6           0x214        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU6         0x238        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER6        0x23C        // Power shutoff/resume Error Register

/* CR7 */
#define RCAR_GEN3_SYSC_PWRSR7           0x240        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR7        0x244        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR7        0x248        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR7         0x24C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR7         0x250        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER7           0x254        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU7         0x278        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER7        0x27C        // Power shutoff/resume Error Register

/* A3VP */
#define RCAR_GEN3_SYSC_PWRSR8           0x340        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR8        0x344        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR8        0x348        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR8         0x34C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR8         0x350        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER8           0x354        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU8         0x378        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER8        0x37C        // Power shutoff/resume Error Register

/* A3VC */
#define RCAR_GEN3_SYSC_PWRSR9           0x380        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR9        0x384        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR9        0x388        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR9         0x38C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR9         0x390        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER9           0x394        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU9         0x3B8        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER9        0x3BC        // Power shutoff/resume Error Register

/* A2VC */
#define RCAR_GEN3_SYSC_PWRSR10          0x3C0        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR10       0x3C4        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR10       0x3C8        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR10        0x3CC        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR10        0x3D0        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER10          0x3D4        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU10        0x3F8        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER10       0x3FC        // Power shutoff/resume Error Register

/* A2IR/A2SC */
#define RCAR_GEN3_SYSC_PWRSR11          0x400        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR11       0x404        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR11       0x408        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR11        0x40C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR11        0x410        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER11          0x414        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU11        0x438        // Power pseudo shutoff Register
#define RCAR_GEN3_SYSC_PWRISOER11       0x43C        // Power isolation error detection register

/* A3VIP0 */
#define RCAR_GEN3_SYSC_PWRSR12          0x2C0        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR12       0x2C4        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR12       0x2C8        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR12        0x2CC        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR12        0x2D0        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER12          0x2D4        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU12        0x2F8        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER12       0x2FC        // Power shutoff/resume Error Register

/* A3VIP1 */
#define RCAR_GEN3_SYSC_PWRSR13          0x300        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR13       0x304        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR13       0x308        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR13        0x30C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR13        0x310        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER13          0x314        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU13        0x318        // Power pseudo shutoff Register
#define RCAR_GEN3_SYSC_PWRISOER13       0x33C        // Power isolation error detection register

/* A3VIP2 */
#define RCAR_GEN3_SYSC_PWRSR14          0x280        // Power Status Register
#define RCAR_GEN3_SYSC_PWROFFCR14       0x284        // Power Shutoff Control Register
#define RCAR_GEN3_SYSC_PWROFFSR14       0x288        // Power Shutoff Status Register
#define RCAR_GEN3_SYSC_PWRONCR14        0x28C        // Power Resume Control Register
#define RCAR_GEN3_SYSC_PWRONSR14        0x290        // Power Resume Status Register
#define RCAR_GEN3_SYSC_PWRER14          0x294        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRPSEU14        0x298        // Power shutoff/resume Error Register
#define RCAR_GEN3_SYSC_PWRISOER14       0x2BC        // Power shutoff/resume Error Register

#define RCAR_GEN3_SYSC_PWRSRx(_x)       (_x==  0 ? RCAR_GEN3_SYSC_PWRSR0 : \
                                        (_x==  2 ? RCAR_GEN3_SYSC_PWRSR2 : \
                                        (_x==  3 ? RCAR_GEN3_SYSC_PWRSR3 : \
                                        (_x==  4 ? RCAR_GEN3_SYSC_PWRSR4 : \
                                        (_x==  5 ? RCAR_GEN3_SYSC_PWRSR5 : \
                                        (_x==  6 ? RCAR_GEN3_SYSC_PWRSR6 : \
                                        (_x==  7 ? RCAR_GEN3_SYSC_PWRSR7 : \
                                        (_x==  8 ? RCAR_GEN3_SYSC_PWRSR8 : \
                                        (_x==  9 ? RCAR_GEN3_SYSC_PWRSR9 : \
                                        (_x== 10 ? RCAR_GEN3_SYSC_PWRSR10 : \
                                        (_x== 11 ? RCAR_GEN3_SYSC_PWRSR11 : \
                                        (_x== 12 ? RCAR_GEN3_SYSC_PWRSR12 : \
                                        (_x== 13 ? RCAR_GEN3_SYSC_PWRSR13 : RCAR_GEN3_SYSC_PWRSR14)))))))))))))
#define RCAR_GEN3_SYSC_PWROFFCRx(_x)    (_x==  2 ? RCAR_GEN3_SYSC_PWROFFCR2 : \
                                        (_x==  3 ? RCAR_GEN3_SYSC_PWROFFCR3 : \
                                        (_x==  4 ? RCAR_GEN3_SYSC_PWROFFCR4 : \
                                        (_x==  5 ? RCAR_GEN3_SYSC_PWROFFCR5 : \
                                        (_x==  7 ? RCAR_GEN3_SYSC_PWROFFCR7 : \
                                        (_x==  8 ? RCAR_GEN3_SYSC_PWROFFCR8 : \
                                        (_x==  9 ? RCAR_GEN3_SYSC_PWROFFCR9 : \
                                        (_x== 10 ? RCAR_GEN3_SYSC_PWROFFCR10 : \
                                        (_x== 11 ? RCAR_GEN3_SYSC_PWROFFCR11 : \
                                        (_x== 12 ? RCAR_GEN3_SYSC_PWROFFCR12 : \
                                        (_x== 13 ? RCAR_GEN3_SYSC_PWROFFCR13 : RCAR_GEN3_SYSC_PWROFFCR14)))))))))))
#define RCAR_GEN3_SYSC_PWROFFSRx(_x)    (_x==  0 ? RCAR_GEN3_SYSC_PWROFFSR0 : \
                                        (_x==  2 ? RCAR_GEN3_SYSC_PWROFFSR2 : \
                                        (_x==  3 ? RCAR_GEN3_SYSC_PWROFFSR3 : \
                                        (_x==  4 ? RCAR_GEN3_SYSC_PWROFFSR4 : \
                                        (_x==  5 ? RCAR_GEN3_SYSC_PWROFFSR5 : \
                                        (_x==  6 ? RCAR_GEN3_SYSC_PWROFFSR6 : \
                                        (_x==  7 ? RCAR_GEN3_SYSC_PWROFFSR7 : \
                                        (_x==  8 ? RCAR_GEN3_SYSC_PWROFFSR8 : \
                                        (_x==  9 ? RCAR_GEN3_SYSC_PWROFFSR9 : \
                                        (_x== 10 ? RCAR_GEN3_SYSC_PWROFFSR10 : \
                                        (_x== 11 ? RCAR_GEN3_SYSC_PWROFFSR11 : \
                                        (_x== 12 ? RCAR_GEN3_SYSC_PWROFFSR12 : \
                                        (_x== 13 ? RCAR_GEN3_SYSC_PWROFFSR13 : RCAR_GEN3_SYSC_PWROFFSR14)))))))))))))
#define RCAR_GEN3_SYSC_PWRONCRx(_x)     (_x==  2 ? RCAR_GEN3_SYSC_PWRONCR2 : \
                                        (_x==  3 ? RCAR_GEN3_SYSC_PWRONCR3 : \
                                        (_x==  4 ? RCAR_GEN3_SYSC_PWRONCR4 : \
                                        (_x==  5 ? RCAR_GEN3_SYSC_PWRONCR5 : \
                                        (_x==  7 ? RCAR_GEN3_SYSC_PWRONCR7 : \
                                        (_x==  8 ? RCAR_GEN3_SYSC_PWRONCR8 : \
                                        (_x==  9 ? RCAR_GEN3_SYSC_PWRONCR9 : \
                                        (_x== 10 ? RCAR_GEN3_SYSC_PWRONCR10 : \
                                        (_x== 11 ? RCAR_GEN3_SYSC_PWRONCR11 : \
                                        (_x== 12 ? RCAR_GEN3_SYSC_PWRONCR12 : \
                                        (_x== 13 ? RCAR_GEN3_SYSC_PWRONCR13 : RCAR_GEN3_SYSC_PWRONCR14)))))))))))
#define RCAR_GEN3_SYSC_PWRONSRx(_x)     (_x==  0 ? RCAR_GEN3_SYSC_PWRONSR0 : \
                                        (_x==  2 ? RCAR_GEN3_SYSC_PWRONSR2 : \
                                        (_x==  3 ? RCAR_GEN3_SYSC_PWRONSR3 : \
                                        (_x==  4 ? RCAR_GEN3_SYSC_PWRONSR4 : \
                                        (_x==  5 ? RCAR_GEN3_SYSC_PWRONSR5 : \
                                        (_x==  6 ? RCAR_GEN3_SYSC_PWRONSR6 : \
                                        (_x==  7 ? RCAR_GEN3_SYSC_PWRONSR7 : \
                                        (_x==  8 ? RCAR_GEN3_SYSC_PWRONSR8 : \
                                        (_x==  9 ? RCAR_GEN3_SYSC_PWRONSR9 : \
                                        (_x== 10 ? RCAR_GEN3_SYSC_PWRONSR10 : \
                                        (_x== 11 ? RCAR_GEN3_SYSC_PWRONSR11 : \
                                        (_x== 12 ? RCAR_GEN3_SYSC_PWRONSR12 : \
                                        (_x== 13 ? RCAR_GEN3_SYSC_PWRONSR13 : RCAR_GEN3_SYSC_PWRONSR14)))))))))))))
#define RCAR_GEN3_SYSC_PWRERx(_x)       (_x==  0 ? RCAR_GEN3_SYSC_PWRER0 : \
                                        (_x==  2 ? RCAR_GEN3_SYSC_PWRER2 : \
                                        (_x==  3 ? RCAR_GEN3_SYSC_PWRER3 : \
                                        (_x==  4 ? RCAR_GEN3_SYSC_PWRER4 : \
                                        (_x==  5 ? RCAR_GEN3_SYSC_PWRER5 : \
                                        (_x==  6 ? RCAR_GEN3_SYSC_PWRER6 : \
                                        (_x==  7 ? RCAR_GEN3_SYSC_PWRER7 : \
                                        (_x==  8 ? RCAR_GEN3_SYSC_PWRER8 : \
                                        (_x==  9 ? RCAR_GEN3_SYSC_PWRER9 : \
                                        (_x== 10 ? RCAR_GEN3_SYSC_PWRER10 : \
                                        (_x== 11 ? RCAR_GEN3_SYSC_PWRER11 : \
                                        (_x== 12 ? RCAR_GEN3_SYSC_PWRER12 : \
                                        (_x== 13 ? RCAR_GEN3_SYSC_PWRER13 : RCAR_GEN3_SYSC_PWRER14)))))))))))))

/*
 * L2 Cache
 */
#define RCAR_GEN3_L2CPL310_BASE          0xF0100000

/*
 * GIC
 */
#define RCAR_GEN3_GIC_BASE               0xF1000000
#define RCAR_GEN3_GIC_CPU_BASE           0xF1020000
#define RCAR_GEN3_GIC_DIST_BASE          0xF1010000

/*
 * Interrupt ID
 */
#define RCAR_GEN3_INTCSYS_IRQ0           (  0 + 32)
#define RCAR_GEN3_INTCSYS_IRQ1           (  1 + 32)
#define RCAR_GEN3_INTCSYS_IRQ2           (  2 + 32)
#define RCAR_GEN3_INTCSYS_IRQ3           (  3 + 32)
#define RCAR_GEN3_INTCSYS_GPIO0          (  4 + 32)
#define RCAR_GEN3_INTCSYS_GPIO1          (  5 + 32)
#define RCAR_GEN3_INTCSYS_GPIO2          (  6 + 32)
#define RCAR_GEN3_INTCSYS_GPIO3          (  7 + 32)
#define RCAR_GEN3_INTCSYS_GPIO4          (  8 + 32)
#define RCAR_GEN3_INTCSYS_GPIO5          (  9 + 32)
#define RCAR_GEN3_INTCSYS_GPIO6          ( 10 + 32)
#define RCAR_GEN3_INTCSYS_GPIO7          ( 11 + 32)
#define RCAR_GEN3_INTCSYS_SCIF4          ( 16 + 32)
#define RCAR_GEN3_INTCSYS_SCIF5          ( 17 + 32)
#define RCAR_GEN3_INTCSYS_IRQ4           ( 18 + 32)
#define RCAR_GEN3_INTCSYS_I2C4           ( 19 + 32)
#define RCAR_GEN3_INTCSYS_I2C5           ( 20 + 32)
#define RCAR_GEN3_INTCSYS_I2C6           ( 21 + 32)
#define RCAR_GEN3_INTCSYS_DBSC4          ( 22 + 32)
#define RCAR_GEN3_INTCSYS_SCIF3          ( 23 + 32)
#define RCAR_GEN3_INTCSYS_ISP0_CH0       ( 25 + 32)
#define RCAR_GEN3_INTCSYS_ISP0_CH1       ( 26 + 32)
#define RCAR_GEN3_INTCSYS_ISP1_CH0       ( 27 + 32)
#define RCAR_GEN3_INTCSYS_FRAY           ( 28 + 32)
#define RCAR_GEN3_INTCSYS_CANFD_CH       ( 29 + 32)
#define RCAR_GEN3_INTCSYS_CANFD_GL       ( 30 + 32)
#define RCAR_GEN3_INTCSYS_SPI36          ( 36 + 32)
#define RCAR_GEN3_INTCSYS_RPC            ( 38 + 32)
#define RCAR_GEN3_INTCSYS_RAVB0          ( 39 + 32)
#define RCAR_GEN3_INTCSYS_RAVB1          ( 40 + 32)
#define RCAR_GEN3_INTCSYS_RAVB2          ( 41 + 32)
#define RCAR_GEN3_INTCSYS_RAVB3          ( 42 + 32)
#define RCAR_GEN3_INTCSYS_RAVB4          ( 43 + 32)
#define RCAR_GEN3_INTCSYS_RAVB5          ( 44 + 32)
#define RCAR_GEN3_INTCSYS_RAVB6          ( 45 + 32)
#define RCAR_GEN3_INTCSYS_RAVB7          ( 46 + 32)
#define RCAR_GEN3_INTCSYS_RAVB8          ( 47 + 32)
#define RCAR_GEN3_INTCSYS_RAVB9          ( 48 + 32)
#define RCAR_GEN3_INTCSYS_RAVB10         ( 49 + 32)
#define RCAR_GEN3_INTCSYS_RAVB11         ( 50 + 32)
#define RCAR_GEN3_INTCSYS_RAVB12         ( 51 + 32)
#define RCAR_GEN3_INTCSYS_RAVB13         ( 52 + 32)
#define RCAR_GEN3_INTCSYS_RAVB14         ( 53 + 32)
#define RCAR_GEN3_INTCSYS_RAVB15         ( 54 + 32)
#define RCAR_GEN3_INTCSYS_RAVB16         ( 55 + 32)
#define RCAR_GEN3_INTCSYS_RAVB17         ( 56 + 32)
#define RCAR_GEN3_INTCSYS_RAVB18         ( 57 + 32)
#define RCAR_GEN3_INTCSYS_RAVB19         ( 58 + 32)
#define RCAR_GEN3_INTCSYS_RAVB20         ( 59 + 32)
#define RCAR_GEN3_INTCSYS_RAVB21         ( 60 + 32)
#define RCAR_GEN3_INTCSYS_RAVB22         ( 61 + 32)
#define RCAR_GEN3_INTCSYS_RAVB23         ( 62 + 32)
#define RCAR_GEN3_INTCSYS_RAVB24         ( 63 + 32)
#define RCAR_GEN3_INTCSYS_THERMAL0       ( 67 + 32)
#define RCAR_GEN3_INTCSYS_THERMAL1       ( 68 + 32)
#define RCAR_GEN3_INTCSYS_THERMAL2       ( 69 + 32)
#define RCAR_GEN3_INTCSYS_USB3_1         ( 98 + 32)
#define RCAR_GEN3_INTCSYS_USB3_0         (102 + 32)
#define RCAR_GEN3_INTCSYS_SATA0          (105 + 32)
#define RCAR_GEN3_INTCSYS_EHCI0          (108 + 32)
#define RCAR_GEN3_INTCSYS_EHCI1          (112 + 32)
#define RCAR_GEN3_INTCSYS_EHCI2          (113 + 32)
#define RCAR_GEN3_INTCSYS_PCIE0          (116 + 32)
#define RCAR_GEN3_INTCSYS_V3_TMU40       (116 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_TMU41       (117 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_TMU42       (118 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_CMT10          (120 + 32)
#define RCAR_GEN3_INTCSYS_CMT11          (121 + 32)
#define RCAR_GEN3_INTCSYS_CMT12          (122 + 32)
#define RCAR_GEN3_INTCSYS_CMT13          (123 + 32)
#define RCAR_GEN3_INTCSYS_CMT14          (124 + 32)
#define RCAR_GEN3_INTCSYS_CMT15          (125 + 32)
#define RCAR_GEN3_INTCSYS_CMT16          (126 + 32)
#define RCAR_GEN3_INTCSYS_CMT17          (127 + 32)
#define RCAR_GEN3_INTCSYS_TMU00          (136 + 32)
#define RCAR_GEN3_INTCSYS_TMU01          (137 + 32)
#define RCAR_GEN3_INTCSYS_TMU02          (138 + 32)
#define RCAR_GEN3_INTCSYS_TMU10          (128 + 32)
#define RCAR_GEN3_INTCSYS_TMU11          (129 + 32)
#define RCAR_GEN3_INTCSYS_TMU12          (130 + 32)
#define RCAR_GEN3_INTCSYS_TMU13          (147 + 32)
#define RCAR_GEN3_INTCSYS_TMU20          (303 + 32)
#define RCAR_GEN3_INTCSYS_TMU21          (304 + 32)
#define RCAR_GEN3_INTCSYS_TMU22          (305 + 32)
#define RCAR_GEN3_INTCSYS_TMU23          (306 + 32)
#define RCAR_GEN3_INTCSYS_TMU30          (131 + 32)
#define RCAR_GEN3_INTCSYS_TMU31          (132 + 32)
#define RCAR_GEN3_INTCSYS_TMU32          (133 + 32)
#define RCAR_GEN3_INTCSYS_TMU33          (134 + 32)
#define RCAR_GEN3_INTCSYS_TPU            (135 + 32)
#define RCAR_GEN3_INTCSYS_CMT00          (142 + 32)
#define RCAR_GEN3_INTCSYS_CMT01          (143 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF2         (144 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF3         (145 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF4         (146 + 32)
#define RCAR_GEN3_INTCSYS_PCIE1          (148 + 32)
#define RCAR_GEN3_INTCSYS_SCIF0          (152 + 32)
#define RCAR_GEN3_INTCSYS_SCIF1          (153 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF0         (154 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF1         (155 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF0         (156 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF1         (157 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF2         (158 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF3         (159 + 32)
#define RCAR_GEN3_INTCSYS_IRQ5           (161 + 32)
#define RCAR_GEN3_INTCSYS_SCIF2          (164 + 32)
#define RCAR_GEN3_INTCSYS_SDHI0          (165 + 32)
#define RCAR_GEN3_INTCSYS_SDHI1          (166 + 32)
#define RCAR_GEN3_INTCSYS_SDHI2          (167 + 32)
#define RCAR_GEN3_INTCSYS_SDHI3          (168 + 32)
#define RCAR_GEN3_INTCSYS_MMC0           (169 + 32)
#define RCAR_GEN3_INTCSYS_MMC1           (170 + 32)
#define RCAR_GEN3_INTCSYS_VIN7           (171 + 32)
#define RCAR_GEN3_INTCSYS_IIC            (173 + 32)
#define RCAR_GEN3_INTCSYS_VIN4           (174 + 32)
#define RCAR_GEN3_INTCSYS_VIN5           (175 + 32)
#define RCAR_GEN3_INTCSYS_VIN6           (176 + 32)
#define RCAR_GEN3_INTCSYS_QSPI           (184 + 32)
#define RCAR_GEN3_INTCSYS_CAN0           (186 + 32)
#define RCAR_GEN3_INTCSYS_CAN1           (187 + 32)
#define RCAR_GEN3_INTCSYS_VIN0           (188 + 32)
#define RCAR_GEN3_INTCSYS_VIN1           (189 + 32)
#define RCAR_GEN3_INTCSYS_VIN2           (190 + 32)
#define RCAR_GEN3_INTCSYS_VIN3           (191 + 32)
#define RCAR_GEN3_INTCSYS_IMRLX40        (192 + 32)
#define RCAR_GEN3_INTCSYS_IMRLX41        (193 + 32)
#define RCAR_GEN3_INTCSYS_IMRLX42        (194 + 32)
#define RCAR_GEN3_INTCSYS_IMRLX43        (195 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_0     (200 + 32)  // For V3M & V3H RTDMAC0-0
#define RCAR_GEN3_INTCSYS_SYSDMAC0_1     (201 + 32)  // For V3M & V3H RTDMAC0-1
#define RCAR_GEN3_INTCSYS_SYSDMAC0_2     (202 + 32)  // For V3M & V3H RTDMAC0-2
#define RCAR_GEN3_INTCSYS_SYSDMAC0_3     (203 + 32)  // For V3M & V3H RTDMAC0-3
#define RCAR_GEN3_INTCSYS_SYSDMAC0_4     (204 + 32)  // For V3M & V3H RTDMAC0-4
#define RCAR_GEN3_INTCSYS_SYSDMAC0_5     (205 + 32)  // For V3M & V3H RTDMAC0-5
#define RCAR_GEN3_INTCSYS_SYSDMAC0_6     (206 + 32)  // For V3M & V3H RTDMAC0-6
#define RCAR_GEN3_INTCSYS_SYSDMAC0_7     (207 + 32)  // For V3M & V3H RTDMAC0-7
#define RCAR_GEN3_INTCSYS_SYSDMAC0_8     (208 + 32)  // For V3M & V3H RTDMAC0-8
#define RCAR_GEN3_INTCSYS_SYSDMAC0_9     (209 + 32)  // For V3M & V3H RTDMAC0-9
#define RCAR_GEN3_INTCSYS_SYSDMAC0_10    (210 + 32)  // For V3M & V3H RTDMAC0-10
#define RCAR_GEN3_INTCSYS_SYSDMAC0_11    (211 + 32)  // For V3M & V3H RTDMAC0-11
#define RCAR_GEN3_INTCSYS_SYSDMAC0_12    (212 + 32)  // For V3M & V3H RTDMAC0-12
#define RCAR_GEN3_INTCSYS_SYSDMAC0_13    (213 + 32)  // For V3M & V3H RTDMAC0-13
#define RCAR_GEN3_INTCSYS_SYSDMAC0_14    (214 + 32)  // For V3M & V3H RTDMAC0-14
#define RCAR_GEN3_INTCSYS_SYSDMAC0_15    (215 + 32)  // For V3M & V3H RTDMAC0-15
#define RCAR_GEN3_INTCSYS_SYSDMAC1_0     (216 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_1     (217 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_2     (218 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_3     (219 + 32)
/* For V3H and V3M */
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_0   (200 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_1   (201 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_2   (202 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_3   (203 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_4   (204 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_5   (205 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_6   (206 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_7   (207 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_8   (208 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_9   (209 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_10  (210 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_11  (211 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_12  (212 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_13  (213 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_14  (214 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC0_15  (215 + 32)
#define RCAR_GEN3_INTCSYS_V3_CMT20       (258 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT21       (259 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT22       (260 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT23       (261 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT24       (262 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT25       (263 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT26       (264 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT27       (265 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_VIN8           (268 + 32)
#define RCAR_GEN3_INTCSYS_VIN9           (269 + 32)
#define RCAR_GEN3_INTCSYS_V3_CMT30       (273 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT31       (274 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT32       (275 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT33       (276 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT34       (277 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT35       (278 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT36       (279 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_V3_CMT37       (280 + 32) // For V3M and V3H
#define RCAR_GEN3_INTCSYS_I2C2           (286 + 32)
#define RCAR_GEN3_INTCSYS_I2C0           (287 + 32)
#define RCAR_GEN3_INTCSYS_I2C1           (288 + 32)
#define RCAR_GEN3_INTCSYS_VIN10          (289 + 32)
#define RCAR_GEN3_INTCSYS_I2C3           (290 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI0       (291 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI1       (292 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI2       (293 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI3       (294 + 32)
#define RCAR_GEN3_INTCSYS_VIN11          (296 + 32)
#define RCAR_GEN3_INTCSYS_VIN12          (298 + 32)
#define RCAR_GEN3_INTCSYS_VIN13          (299 + 32)
#define RCAR_GEN3_INTCSYS_VIN14          (301 + 32)
#define RCAR_GEN3_INTCSYS_VIN15          (302 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_4     (308 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_5     (309 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_6     (310 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_7     (311 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_8     (312 + 32)  // For V3M & V3H SYSDMAC2-0
#define RCAR_GEN3_INTCSYS_SYSDMAC1_9     (313 + 32)  // For V3M & V3H SYSDMAC2-1
#define RCAR_GEN3_INTCSYS_SYSDMAC1_10    (314 + 32)  // For V3M & V3H SYSDMAC2-2 
#define RCAR_GEN3_INTCSYS_SYSDMAC1_11    (315 + 32)  // For V3M & V3H SYSDMAC2-3 
#define RCAR_GEN3_INTCSYS_SYSDMAC1_12    (316 + 32)  // For V3M & V3H SYSDMAC2-4
#define RCAR_GEN3_INTCSYS_SYSDMAC1_13    (317 + 32)  // For V3M & V3H SYSDMAC2-5
#define RCAR_GEN3_INTCSYS_SYSDMAC1_14    (318 + 32)  // For V3M & V3H SYSDMAC2-6
#define RCAR_GEN3_INTCSYS_SYSDMAC1_15    (319 + 32)  // For V3M & V3H SYSDMAC2-7 
/* For V3H and V3M */
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_0  (312 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_1  (313 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_2  (314 + 32) 
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_3  (315 + 32) 
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_4  (316 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_5  (317 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_6  (318 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_7  (319 + 32) 
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_0   (321 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_1   (322 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_2   (323 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_3   (324 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_4   (325 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_5   (326 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_6   (327 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_7   (328 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_8   (329 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_9   (330 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_10  (331 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_11  (332 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_12  (333 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_13  (334 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_14  (335 + 32)
#define RCAR_GEN3_INTCSYS_V3_RTDMAC1_15  (336 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_0   (320 + 32)  
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_1   (321 + 32)  // For V3M & V3H RTDMAC1-0
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_2   (322 + 32)  // For V3M & V3H RTDMAC1-1
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_3   (323 + 32)  // For V3M & V3H RTDMAC1-2
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_4   (324 + 32)  // For V3M & V3H RTDMAC1-3
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_5   (325 + 32)  // For V3M & V3H RTDMAC1-4
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_6   (326 + 32)  // For V3M & V3H RTDMAC1-5
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_7   (327 + 32)  // For V3M & V3H RTDMAC1-6
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_8   (328 + 32)  // For V3M & V3H RTDMAC1-7
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_9   (329 + 32)  // For V3M & V3H RTDMAC1-8
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_10  (330 + 32)  // For V3M & V3H RTDMAC1-9
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_11  (331 + 32)  // For V3M & V3H RTDMAC1-10
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_12  (332 + 32)  // For V3M & V3H RTDMAC1-11
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_13  (333 + 32)  // For V3M & V3H RTDMAC1-12
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_14  (334 + 32)  // For V3M & V3H RTDMAC1-13
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_15  (335 + 32)  // For V3M & V3H RTDMAC1-14
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_0   (336 + 32)  // For V3M & V3H RTDMAC1-15
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_1   (337 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_2   (338 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_3   (339 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_4   (340 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_5   (341 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_6   (342 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_7   (343 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_8   (344 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_9   (345 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_10  (346 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_11  (347 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_12  (348 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_13  (349 + 32)
#define RCAR_GEN3_INTCSYS_SCU0           (352 + 32)
#define RCAR_GEN3_INTCSYS_SCU1           (353 + 32)  // For V3H SYSDMAC1-8
#define RCAR_GEN3_INTCSYS_SCU2           (354 + 32)  // For V3H SYSDMAC1-9
#define RCAR_GEN3_INTCSYS_SCU3           (355 + 32)  // For V3H SYSDMAC1-10
#define RCAR_GEN3_INTCSYS_SCU4           (356 + 32)  // For V3H SYSDMAC1-11 
#define RCAR_GEN3_INTCSYS_SCU5           (357 + 32)  // For V3H SYSDMAC1-12 
#define RCAR_GEN3_INTCSYS_SCU6           (358 + 32)  // For V3H SYSDMAC1-13 
#define RCAR_GEN3_INTCSYS_SCU7           (359 + 32)  // For V3H SYSDMAC1-14 
#define RCAR_GEN3_INTCSYS_SCU8           (360 + 32)  // For V3H SYSDMAC1-15
#define RCAR_GEN3_INTCSYS_SCU9           (361 + 32)  // For V3H SYSDMAC2-8
#define RCAR_GEN3_INTCSYS_MLM0           (362 + 32)  // For V3H SYSDMAC2-9 
#define RCAR_GEN3_INTCSYS_MLM1           (363 + 32)  // For V3H SYSDMAC2-10
#define RCAR_GEN3_INTCSYS_MLM2           (364 + 32)  // For V3H SYSDMAC2-11 
#define RCAR_GEN3_INTCSYS_MLM3           (365 + 32)  // For V3H SYSDMAC2-12 
#define RCAR_GEN3_INTCSYS_MLM4           (366 + 32)  // For V3H SYSDMAC2-13 
#define RCAR_GEN3_INTCSYS_MLM5           (367 + 32)  // For V3H SYSDMAC2-14 
#define RCAR_GEN3_INTCSYS_MLM6           (368 + 32)  // For V3H SYSDMAC2-15 
#define RCAR_GEN3_INTCSYS_TMU43          (369 + 32)
#define RCAR_GEN3_INTCSYS_CMT20          (398 + 32)
#define RCAR_GEN3_INTCSYS_CMT21          (399 + 32)
#define RCAR_GEN3_INTCSYS_CMT22          (400 + 32)
#define RCAR_GEN3_INTCSYS_CMT23          (401 + 32)
#define RCAR_GEN3_INTCSYS_CMT24          (402 + 32)
#define RCAR_GEN3_INTCSYS_CMT25          (403 + 32)
#define RCAR_GEN3_INTCSYS_CMT26          (404 + 32)
#define RCAR_GEN3_INTCSYS_CMT27          (405 + 32)
#define RCAR_GEN3_INTCSYS_CMT30          (470 + 32)
#define RCAR_GEN3_INTCSYS_CMT31          (471 + 32)
#define RCAR_GEN3_INTCSYS_CMT32          (472 + 32)
#define RCAR_GEN3_INTCSYS_CMT33          (473 + 32)
#define RCAR_GEN3_INTCSYS_CMT34          (474 + 32)
#define RCAR_GEN3_INTCSYS_CMT35          (475 + 32)
#define RCAR_GEN3_INTCSYS_CMT36          (476 + 32)
#define RCAR_GEN3_INTCSYS_CMT37          (477 + 32)
/* For V3H */
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_8  (353 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_9  (354 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_10 (355 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_11 (356 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_12 (357 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_13 (358 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_14 (359 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC1_15 (360 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_8  (361 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_9  (362 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_10 (363 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_11 (364 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_12 (365 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_13 (366 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_14 (367 + 32)
#define RCAR_GEN3_INTCSYS_V3_SYSDMAC2_15 (368 + 32)
#define RCAR_GEN3_INTCSYS_MLM7           (369 + 32) 
#define RCAR_GEN3_INTCSYS_SSI0           (370 + 32)  
#define RCAR_GEN3_INTCSYS_SSI1           (371 + 32)
#define RCAR_GEN3_INTCSYS_SSI2           (372 + 32)
#define RCAR_GEN3_INTCSYS_SSI3           (373 + 32)
#define RCAR_GEN3_INTCSYS_SSI4           (374 + 32)
#define RCAR_GEN3_INTCSYS_SSI5           (375 + 32)
#define RCAR_GEN3_INTCSYS_SSI6           (376 + 32)
#define RCAR_GEN3_INTCSYS_SSI7           (377 + 32)
#define RCAR_GEN3_INTCSYS_SSI8           (378 + 32)
#define RCAR_GEN3_INTCSYS_SSI9           (379 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_14  (382 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_15  (383 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_15    (397 + 32)
#define RCAR_GEN3_INTCSYS_TMU40          (406 + 32)
#define RCAR_GEN3_INTCSYS_TMU41          (407 + 32)
#define RCAR_GEN3_INTCSYS_TMU42          (408 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_0     (417 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_1     (418 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_2     (419 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_3     (420 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_4     (421 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_5     (422 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_6     (423 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_7     (424 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_8     (425 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_9     (426 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_10    (427 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_11    (428 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_12    (429 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_13    (430 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_14    (431 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_0      (449 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_1      (450 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_2      (451 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_3      (452 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_4      (453 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_5      (454 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_6      (455 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC0_7      (456 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_0      (457 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_1      (458 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_2      (459 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_3      (460 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_4      (461 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_5      (462 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_6      (463 + 32)
#define RCAR_GEN3_INTCSYS_RTDMAC1_7      (464 + 32)

/*
 * Adaptive Voltage Scaling
 */
#define RCAR_GEN3_AVS_BASE               0xE60A0000  // Adaptive Voltage Scaling
#define RCAR_GEN3_AVS_SIZE               0x200

#define RCAR_GEN3_ADVADJP                0x13C       // register offset address

/*--------------------------------------------------
 * 
 * Macros is for Gen4 (V3U, S4)
 * 
 *--------------------------------------------------*/

/*
 * PFC/GPIO
 */

/* Domain base address */
#define RCAR_GEN4_GPIO_DM0_BASE          0xE6050000
#define RCAR_GEN4_GPIO_DM1_BASE          0xE6052000
#define RCAR_GEN4_GPIO_DM2_BASE          0xE6054000
#define RCAR_GEN4_GPIO_DM3_BASE          0xE6056000
#define RCAR_GEN4_GPIO_SIZE              0x2000
#define RCAR_GEN4_GPIO_GP_SIZE           0x200

/* Group offset */
#define RCAR_V3U_GPIO_GP0                0x08000
#define RCAR_V3U_GPIO_GP1                0x00000
#define RCAR_V3U_GPIO_GP2                0x00800
#define RCAR_V3U_GPIO_GP3                0x08800
#define RCAR_V3U_GPIO_GP4                0x10000
#define RCAR_V3U_GPIO_GP5                0x10800
#define RCAR_V3U_GPIO_GP6                0x18000
#define RCAR_V3U_GPIO_GP7                0x18800
#define RCAR_V3U_GPIO_GP8                0x19000
#define RCAR_V3U_GPIO_GP9                0x19800
#define RCAR_V3U_GPIO_SYSGP              0x28000

#define RCAR_S4_GPIO_GP0                 0x00000
#define RCAR_S4_GPIO_GP1                 0x00800
#define RCAR_S4_GPIO_GP2                 0x01000
#define RCAR_S4_GPIO_GP3                 0x01800
#define RCAR_S4_GPIO_SYSGP0              0x28000
#define RCAR_S4_GPIO_GP4                 0x19D40000
#define RCAR_S4_GPIO_GP5                 0x19D40800
#define RCAR_S4_GPIO_GP6                 0x19D41000
#define RCAR_S4_GPIO_GP7                 0x19D41800
#define RCAR_S4_GPIO_SYSGP1              0x19D48000


/* Group operation offset */
#define RCAR_GEN4_GPIO_RW                0x0000
#define RCAR_GEN4_GPIO_SET               0x0200
#define RCAR_GEN4_GPIO_CLR               0x0400

/* Register offset for Group n */
#define RCAR_GEN4_GPIO_PMMR              0x000
#define RCAR_GEN4_GPIO_PMMER             0x004
#define RCAR_GEN4_GPIO_DM0PR             0x020
#define RCAR_GEN4_GPIO_DM1PR             0x024
#define RCAR_GEN4_GPIO_DM2PR             0x028
#define RCAR_GEN4_GPIO_DM3PR             0x02C
#define RCAR_GEN4_GPIO_GPSR              0x040
#define RCAR_GEN4_GPIO_IP0SR             0x060
#define RCAR_GEN4_GPIO_IP1SR             0x064
#define RCAR_GEN4_GPIO_IP2SR             0x068
#define RCAR_GEN4_GPIO_IP3SR             0x06C
#define RCAR_GEN4_GPIO_DRV0CTRL          0x080
#define RCAR_GEN4_GPIO_DRV1CTRL          0x084
#define RCAR_GEN4_GPIO_DRV2CTRL          0x088
#define RCAR_GEN4_GPIO_DRV3CTRL          0x08C
#define RCAR_GEN4_GPIO_POC               0x0A0
#define RCAR_GEN4_GPIO_PUEN              0x0C0
#define RCAR_GEN4_GPIO_PUD               0x0E0
#define RCAR_GEN4_GPIO_MODSEL            0x100
#define RCAR_GEN4_GPIO_TD0SEL            0x120
#define RCAR_GEN4_GPIO_TD1SEL            0x124
#define RCAR_GEN4_GPIO_BIP0SR            0x140
#define RCAR_GEN4_GPIO_BIP1SR            0x144
#define RCAR_GEN4_GPIO_BIP2SR            0x148
#define RCAR_GEN4_GPIO_BIP3SR            0x14C
#define RCAR_GEN4_GPIO_PSER              0x160
#define RCAR_GEN4_GPIO_PS0SR             0x164
#define RCAR_GEN4_GPIO_PS1SR             0x168
#define RCAR_GEN4_GPIO_IOINTSEL          0x180
#define RCAR_GEN4_GPIO_INOUTSEL          0x184
#define RCAR_GEN4_GPIO_OUTDT             0x188
#define RCAR_GEN4_GPIO_INDT              0x18C
#define RCAR_GEN4_GPIO_INTDT             0x190
#define RCAR_GEN4_GPIO_INTCLR            0x194
#define RCAR_GEN4_GPIO_INTMSK            0x198
#define RCAR_GEN4_GPIO_MSKCLR            0x19C
#define RCAR_GEN4_GPIO_POSNEG            0x1A0
#define RCAR_GEN4_GPIO_EDGLEVEL          0x1A4
#define RCAR_GEN4_GPIO_FILONOFF          0x1A8
#define RCAR_GEN4_GPIO_FILCLKSEL         0x1AC
#define RCAR_GEN4_GPIO_OUTDTSEL          0x1C0
#define RCAR_GEN4_GPIO_OUTDTH            0x1C4
#define RCAR_GEN4_GPIO_OUTDTL            0x1C8
#define RCAR_GEN4_GPIO_BOTHEDGE          0x1CC
#define RCAR_GEN4_GPIO_INEN              0x1D0

/* Register offset for SYS Group */
#define RCAR_GEN4_GPIO_PMMRSYS           0x000
#define RCAR_GEN4_GPIO_PMMERSYS          0x004
#define RCAR_GEN4_GPIO_DM0PRSYS          0x020
#define RCAR_GEN4_GPIO_DM1PRSYS          0x024
#define RCAR_GEN4_GPIO_DM2PRSYS          0x028
#define RCAR_GEN4_GPIO_DM3PRSYS          0x02C
#define RCAR_GEN4_GPIO_DRV0CTRLSYS       0x080
#define RCAR_GEN4_GPIO_DRV1CTRLSYS       0x084
#define RCAR_GEN4_GPIO_DRV2CTRLSYS       0x088
#define RCAR_GEN4_GPIO_PUENSYS           0x0C0
#define RCAR_GEN4_GPIO_PUDSYS            0x0E0
#define RCAR_GEN4_GPIO_FUSE_MON0         0x0C0
#define RCAR_GEN4_GPIO_FUSE_MON1         0x0C4
#define RCAR_GEN4_GPIO_FUSE_MON2         0x0C8
#define RCAR_GEN4_GPIO_FUSE_MON3         0x0CC
#define RCAR_GEN4_GPIO_FUSE_MON4         0x0D0
#define RCAR_GEN4_GPIO_FUSE_MON5         0x0D4
#define RCAR_GEN4_GPIO_FUSE_MON6         0x0D8
#define RCAR_GEN4_GPIO_FUSE_MON7         0x0DC
#define RCAR_GEN4_GPIO_FUSE_MON8         0x0E0
#define RCAR_GEN4_GPIO_FUSE_MON9         0x0E4
#define RCAR_GEN4_GPIO_FUSE_CTR          0x080
#define RCAR_GEN4_GPIO_LTM0_CTR          0x100
#define RCAR_GEN4_GPIO_LTM0_MON0         0x140
#define RCAR_GEN4_GPIO_LTM0_MON1         0x144
#define RCAR_GEN4_GPIO_LTM0_MON2         0x148
#define RCAR_GEN4_GPIO_LTM0_MON3         0x14C
#define RCAR_GEN4_GPIO_LTM0_MON4         0x150
#define RCAR_GEN4_GPIO_LTM1_CTR          0x180
#define RCAR_GEN4_GPIO_LTM1_MON0         0x1C0
#define RCAR_GEN4_GPIO_LTM1_MON1         0x1C4
#define RCAR_GEN4_GPIO_LTM1_MON2         0x1C8
#define RCAR_GEN4_GPIO_LTM1_MON3         0x1CC
#define RCAR_GEN4_GPIO_LTM1_MON4         0x1D0

/*
 * CPG
 */

/* Domain base address */
#define RCAR_GEN4_CPG_DM0_BASE      0xE6150000
#define RCAR_GEN4_CPG_DM1_BASE      0xE6154000
#define RCAR_GEN4_CPG_DM2_BASE      0xE6158000
#define RCAR_GEN4_CPG_DM3_BASE      0xE615C000
#define RCAR_GEN4_CPG_SIZE          0x4000

/* Group offset */
#define RCAR_GEN4_CPG_GP0            0x0000
#define RCAR_GEN4_CPG_GP1            0x0400
#define RCAR_GEN4_CPG_GP2            0x0800
#define RCAR_GEN4_CPG_GP3            0x0C00
#define RCAR_GEN4_CPG_GP4            0x1000
#define RCAR_GEN4_CPG_GP5            0x1400
#define RCAR_GEN4_CPG_GP6            0x1800
#define RCAR_GEN4_CPG_GP7            0x1C00
#define RCAR_GEN4_CPG_GP8            0x2000
#define RCAR_GEN4_CPG_GP9            0x2400
#define RCAR_GEN4_CPG_GPA            0x2800
#define RCAR_GEN4_CPG_GPB            0x2C00
#define RCAR_GEN4_CPG_GPC            0x3000
#define RCAR_GEN4_CPG_GPD            0x3400
#define RCAR_GEN4_CPG_GPE            0x3800
#define RCAR_GEN4_CPG_GPF            0x3C00

/* Register offset */
#define RCAR_GEN4_CPG_CPGWPR              0x0000
#define RCAR_GEN4_CPG_CPGWPCR             0x0004
#define RCAR_GEN4_CPG_RSSDER              0x0008
#define RCAR_GEN4_CPG_CLSTAT              0x0108
#define RCAR_GEN4_CPG_ATSTPOFFR           0x0278
#define RCAR_GEN4_CPG_RTSRAMCR            0x027C
#define RCAR_GEN4_CPG_BKBAPR              0x0280
#define RCAR_GEN4_CPG_FSRCHKRA(n)        (0x0400 + (n * 4))
#define RCAR_GEN4_CPG_FSRCHKRB(n)        (0x0480 + (n * 4))
#define RCAR_GEN4_CPG_FSRCHKSETR(n)      (0x0500 + (n * 4))
#define RCAR_GEN4_CPG_FSRCHKCLRR(n)      (0x0580 + (n * 4))
#define RCAR_GEN4_CPG_ECMERRINJR          0x0740
#define RCAR_GEN4_CPG_FRQCRA              0x0800
#define RCAR_GEN4_CPG_FRQCRB              0x0804
#define RCAR_GEN4_CPG_FRQCRC              0x0808
#define RCAR_GEN4_CPG_FRQCRD0             0x080C
#define RCAR_GEN4_CPG_FRQCRD1             0x0810
#define RCAR_GEN4_CPG_PLLECR              0x0820
#define RCAR_GEN4_CPG_PLL1CR0             0x0830
#define RCAR_GEN4_CPG_PLL1CR1             0x08B0
#define RCAR_GEN4_CPG_PLL1CR2             0x08B4
#define RCAR_GEN4_CPG_PLL2CR0             0x0834
#define RCAR_GEN4_CPG_PLL2CR1             0x08B8
#define RCAR_GEN4_CPG_PLL2CR2             0x08BC
#define RCAR_GEN4_CPG_PLL20CR             0x0834
#define RCAR_GEN4_CPG_PLL21CR             0x0838
#define RCAR_GEN4_CPG_PLL3CR0             0x083C
#define RCAR_GEN4_CPG_PLL3CR1             0x08C0
#define RCAR_GEN4_CPG_PLL3CR2             0x08C0
#define RCAR_GEN4_CPG_PLL30CR             0x083C
#define RCAR_GEN4_CPG_PLL31CR             0x0840
#define RCAR_GEN4_CPG_PLL4CR              0x0844
#define RCAR_GEN4_CPG_PLL5CR0             0x0848
#define RCAR_GEN4_CPG_PLL5CR1             0x08D0
#define RCAR_GEN4_CPG_PLL5CR2             0x08D4
#define RCAR_GEN4_CPG_PLL6CR0             0x084C
#define RCAR_GEN4_CPG_PLL6CR1             0x08D8
#define RCAR_GEN4_CPG_PLL6CR2             0x08DC
#define RCAR_GEN4_CPG_PLL1STPCR           0x0850
#define RCAR_GEN4_CPG_PLL20STPCR          0x0854
#define RCAR_GEN4_CPG_PLL21STPCR          0x0858
#define RCAR_GEN4_CPG_PLL30STPCR          0x085C
#define RCAR_GEN4_CPG_PLL31STPCR          0x0860
#define RCAR_GEN4_CPG_PLL4STPCR           0x0864
#define RCAR_GEN4_CPG_PLL5STPCR           0x0868
#define RCAR_GEN4_CPG_SD0CKCR             0x0870
#define RCAR_GEN4_CPG_SD0CKCR1            0x08A4
#define RCAR_GEN4_CPG_RPCCKCR             0x0874
#define RCAR_GEN4_CPG_CANFDCKCR           0x0878
#define RCAR_GEN4_CPG_MSOCKCR             0x087C
#define RCAR_GEN4_CPG_CSI0CKCR            0x0880
#define RCAR_GEN4_CPG_DSICKCR             0x0884
#define RCAR_GEN4_CPG_IPCCKCR             0x0888
#define RCAR_GEN4_CPG_FRAYCKCR            0x088C
#define RCAR_GEN4_CPG_POSTCKCR            0x0890
#define RCAR_GEN4_CPG_POST2CKCR           0x0894
#define RCAR_GEN4_CPG_POST3CKCR           0x0898
#define RCAR_GEN4_CPG_POST4CKCR           0x089C
#define RCAR_GEN4_CPG_OSCCKCR             0x08A0
#define RCAR_GEN4_CPG_CPGMTCR             0x08A4
#define RCAR_GEN4_CPG_Z0CKKSCR            0x08A8
#define RCAR_GEN4_CPG_Z1CKKSCR            0x08AC
#define RCAR_GEN4_CPG_RSW2CKCR            0x08E8
#define RCAR_GEN4_CPG_PLL1FBCKMCSR        0x0C00
#define RCAR_GEN4_CPG_PLL1FBCKMECR        0x0C04
#define RCAR_GEN4_CPG_PLL1FBCKMLCH        0x0C08
#define RCAR_GEN4_CPG_PLL1FBCKMLCL        0x0C0C
#define RCAR_GEN4_CPG_PLL1FBCKMCNT        0x0C10
#define RCAR_GEN4_CPG_PLL1FBCKMCNTE       0x0C14
#define RCAR_GEN4_CPG_PLL1FBCKMMDR        0x0C18
#define RCAR_GEN4_CPG_PLL2_0FBCKMCSR      0x0C20
#define RCAR_GEN4_CPG_PLL2_0FBCKMECR      0x0C24
#define RCAR_GEN4_CPG_PLL2_0FBCKMLCH      0x0C28
#define RCAR_GEN4_CPG_PLL2_0FBCKMLCL      0x0C2C
#define RCAR_GEN4_CPG_PLL2_0FBCKMCNT      0x0C30
#define RCAR_GEN4_CPG_PLL2_0FBCKMCNTE     0x0C34
#define RCAR_GEN4_CPG_PLL2_0FBCKMMDR      0x0C38
#define RCAR_GEN4_CPG_PLL2_1FBCKMCSR      0x0C40
#define RCAR_GEN4_CPG_PLL2_1FBCKMECR      0x0C44
#define RCAR_GEN4_CPG_PLL2_1FBCKMLCH      0x0C48
#define RCAR_GEN4_CPG_PLL2_1FBCKMLCL      0x0C4C
#define RCAR_GEN4_CPG_PLL2_1FBCKMCNT      0x0C50
#define RCAR_GEN4_CPG_PLL2_1FBCKMCNTE     0x0C54
#define RCAR_GEN4_CPG_PLL2_1FBCKMMDR      0x0C58
#define RCAR_GEN4_CPG_PLL3_0FBCKMCSR      0x0C60
#define RCAR_GEN4_CPG_PLL3_0FBCKMECR      0x0C64
#define RCAR_GEN4_CPG_PLL3_0FBCKMLCH      0x0C68
#define RCAR_GEN4_CPG_PLL3_0FBCKMLCL      0x0C6C
#define RCAR_GEN4_CPG_PLL3_0FBCKMCNT      0x0C70
#define RCAR_GEN4_CPG_PLL3_0FBCKMCNTE     0x0C74
#define RCAR_GEN4_CPG_PLL3_0FBCKMMDR      0x0C78
#define RCAR_GEN4_CPG_PLL3_1FBCKMCSR      0x0C80
#define RCAR_GEN4_CPG_PLL3_1FBCKMECR      0x0C84
#define RCAR_GEN4_CPG_PLL3_1FBCKMLCH      0x0C88
#define RCAR_GEN4_CPG_PLL3_1FBCKMLCL      0x0C8C
#define RCAR_GEN4_CPG_PLL3_1FBCKMCNT      0x0C90
#define RCAR_GEN4_CPG_PLL3_1FBCKMCNTE     0x0C94
#define RCAR_GEN4_CPG_PLL3_1FBCKMMDR      0x0C98
#define RCAR_GEN4_CPG_PLL4FBCKMCSR        0x0CA0
#define RCAR_GEN4_CPG_PLL4FBCKMECR        0x0CA4
#define RCAR_GEN4_CPG_PLL4FBCKMLCH        0x0CA8
#define RCAR_GEN4_CPG_PLL4FBCKMLCL        0x0CAC
#define RCAR_GEN4_CPG_PLL4FBCKMCNT        0x0CB0
#define RCAR_GEN4_CPG_PLL4FBCKMCNTE       0x0CB4
#define RCAR_GEN4_CPG_PLL4FBCKMMDR        0x0CB8
#define RCAR_GEN4_CPG_PLL5FBCKMCSR        0x0CC0
#define RCAR_GEN4_CPG_PLL5FBCKMECR        0x0CC4
#define RCAR_GEN4_CPG_PLL5FBCKMLCH        0x0CC8
#define RCAR_GEN4_CPG_PLL5FBCKMLCL        0x0CCC
#define RCAR_GEN4_CPG_PLL5FBCKMCNT        0x0CD0
#define RCAR_GEN4_CPG_PLL5FBCKMCNTE       0x0CD4
#define RCAR_GEN4_CPG_PLL5FBCKMMDR        0x0CD8
#define RCAR_GEN4_CPG_CPCKMCSR            0x0CE0
#define RCAR_GEN4_CPG_CPCKMECR            0x0CE4
#define RCAR_GEN4_CPG_CPCKMLCH            0x0CE8
#define RCAR_GEN4_CPG_CPCKMLCL            0x0CEC
#define RCAR_GEN4_CPG_CPCKMCNT            0x0CF0
#define RCAR_GEN4_CPG_CPCKMCNTE           0x0CF4
#define RCAR_GEN4_CPG_CPCKMMDR            0x0CF8
#define RCAR_GEN4_CPG_CBFUSACKMCSR        0x0D00
#define RCAR_GEN4_CPG_CBFUSACKMECR        0x0D04
#define RCAR_GEN4_CPG_CBFUSACKMLCH        0x0D08
#define RCAR_GEN4_CPG_CBFUSACKMLCL        0x0D0C
#define RCAR_GEN4_CPG_CBFUSACKMCNT        0x0D10
#define RCAR_GEN4_CPG_CBFUSACKMCNTE       0x0D14
#define RCAR_GEN4_CPG_CBFUSACKMMDR        0x0D18
#define RCAR_GEN4_CPG_CPGACKMSR           0x0D1C
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2CKMCSR          0x0D20
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2CKMECR          0x0D24
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2CKMLCH          0x0D28
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2CKMLCL          0x0D2C
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2CKMCNT          0x0D30
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2CKMCNTE         0x0D34
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2CKMMDR          0x0D38
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2DDRL0CKMCSR     0x0D40
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2DDRL0CKMECR     0x0D44
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2DDRL0CKMLCH     0x0D48
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2DDRL0CKMLCL     0x0D4C
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2DDRL0CKMCNT     0x0D50
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2DDRL0CKMCNTE    0x0D54
#define RCAR_GEN4_CPG_CKMDDR0ZB3D2DDRL0CKMMDR     0x0D58
#define RCAR_GEN4_CPG_CKMDDR0CL16MCKMCSR          0x0D60
#define RCAR_GEN4_CPG_CKMDDR0CL16MCKMECR          0x0D64
#define RCAR_GEN4_CPG_CKMDDR0CL16MCKMLCH          0x0D68
#define RCAR_GEN4_CPG_CKMDDR0CL16MCKMLCL          0x0D6C
#define RCAR_GEN4_CPG_CKMDDR0CL16MCKMCNT          0x0D70
#define RCAR_GEN4_CPG_CKMDDR0CL16MCKMCNTE         0x0D74
#define RCAR_GEN4_CPG_CKMDDR0CL16MCKMMDR          0x0D78
#define RCAR_GEN4_CPG_CKMDDR0ZB3D1CKMCSR          0x0D80
#define RCAR_GEN4_CPG_CKMDDR0ZB3D1CKMECR          0x0D84
#define RCAR_GEN4_CPG_CKMDDR0ZB3D1CKMLCH          0x0D88
#define RCAR_GEN4_CPG_CKMDDR0ZB3D1CKMLCL          0x0D8C
#define RCAR_GEN4_CPG_CKMDDR0ZB3D1CKMCNT          0x0D90
#define RCAR_GEN4_CPG_CKMDDR0ZB3D1CKMCNTE         0x0D94
#define RCAR_GEN4_CPG_CKMDDR0ZB3D1CKMMDR          0x0D98
#define RCAR_GEN4_CPG_CKMDDR0ZB3D4CKMCSR          0x0DA0
#define RCAR_GEN4_CPG_CKMDDR0ZB3D4CKMECR          0x0DA4
#define RCAR_GEN4_CPG_CKMDDR0ZB3D4CKMLCH          0x0DA8
#define RCAR_GEN4_CPG_CKMDDR0ZB3D4CKMLCL          0x0DAC
#define RCAR_GEN4_CPG_CKMDDR0ZB3D4CKMCNT          0x0DB0
#define RCAR_GEN4_CPG_CKMDDR0ZB3D4CKMCNTE         0x0DB4
#define RCAR_GEN4_CPG_CKMDDR0ZB3D4CKMMDR          0x0DB8
#define RCAR_GEN4_CPG_CKMDDR0ACKMSR               0x0DBC
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2CKMCSR          0x0DC0
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2CKMECR          0x0DC4
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2CKMLCH          0x0DC8
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2CKMLCL          0x0DCC
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2CKMCNT          0x0DD0
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2CKMCNTE         0x0DD4
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2CKMMDR          0x0DD8
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2DDRL0CKMCSR     0x0DE0
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2DDRL0CKMECR     0x0DE4
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2DDRL0CKMLCH     0x0DE8
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2DDRL0CKMLCL     0x0DEC
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2DDRL0CKMCNT     0x0DF0
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2DDRL0CKMCNTE    0x0DF4
#define RCAR_GEN4_CPG_CKMDDR1ZB3D2DDRL0CKMMDR     0x0DF8
#define RCAR_GEN4_CPG_CKMDDR1CL16MCKMCSR          0x0E00
#define RCAR_GEN4_CPG_CKMDDR1CL16MCKMECR          0x0E04
#define RCAR_GEN4_CPG_CKMDDR1CL16MCKMLCH          0x0E08
#define RCAR_GEN4_CPG_CKMDDR1CL16MCKMLCL          0x0E0C
#define RCAR_GEN4_CPG_CKMDDR1CL16MCKMCNT          0x0E10
#define RCAR_GEN4_CPG_CKMDDR1CL16MCKMCNTE         0x0E14
#define RCAR_GEN4_CPG_CKMDDR1CL16MCKMMDR          0x0E18
#define RCAR_GEN4_CPG_CKMDDR1ZB3D1CKMCSR          0x0E20
#define RCAR_GEN4_CPG_CKMDDR1ZB3D1CKMECR          0x0E24
#define RCAR_GEN4_CPG_CKMDDR1ZB3D1CKMLCH          0x0E28
#define RCAR_GEN4_CPG_CKMDDR1ZB3D1CKMLCL          0x0E2C
#define RCAR_GEN4_CPG_CKMDDR1ZB3D1CKMCNT          0x0E30
#define RCAR_GEN4_CPG_CKMDDR1ZB3D1CKMCNTE         0x0E34
#define RCAR_GEN4_CPG_CKMDDR1ZB3D1CKMMDR          0x0E38
#define RCAR_GEN4_CPG_CKMDDR1ZB3D4CKMCSR          0x0E40
#define RCAR_GEN4_CPG_CKMDDR1ZB3D4CKMECR          0x0E44
#define RCAR_GEN4_CPG_CKMDDR1ZB3D4CKMLCH          0x0E48
#define RCAR_GEN4_CPG_CKMDDR1ZB3D4CKMLCL          0x0E4C
#define RCAR_GEN4_CPG_CKMDDR1ZB3D4CKMCNT          0x0E50
#define RCAR_GEN4_CPG_CKMDDR1ZB3D4CKMCNTE         0x0E54
#define RCAR_GEN4_CPG_CKMDDR1ZB3D4CKMMDR          0x0E58
#define RCAR_GEN4_CPG_CKMDDR1ACKMSR               0x0E5C

/* MSTP resgister offset */
#define RCAR_GEN4_CPG_SRCR(n)             (0x2C00 + (n * 4))
#define RCAR_GEN4_CPG_SRSTCLR(n)          (0x2C80 + (n * 4))
#define RCAR_GEN4_CPG_MSTPCR(n)           (0x2D00 + (n * 4))
#define RCAR_GEN4_CPG_NSIRCR(n)           (0x2D80 + (n * 4))
#define RCAR_GEN4_CPG_MSTPSR(n)           (0x2E00 + (n * 4))
#define RCAR_GEN4_CPG_D0WACR_MSTPCR(n)    (0x3000 + (n * 4))
#define RCAR_GEN4_CPG_D0WACR_NSIRCR(n)    (0x3080 + (n * 4))
#define RCAR_GEN4_CPG_D1WACR_MSTPCR(n)    (0x3100 + (n * 4))
#define RCAR_GEN4_CPG_D1WACR_NSIRCR(n)    (0x3180 + (n * 4))
#define RCAR_GEN4_CPG_D2WACR_MSTPCR(n)    (0x3200 + (n * 4))
#define RCAR_GEN4_CPG_D2WACR_NSIRCR(n)    (0x3280 + (n * 4))
#define RCAR_GEN4_CPG_D3WACR_MSTPCR(n)    (0x3300 + (n * 4))
#define RCAR_GEN4_CPG_D3WACR_NSIRCR(n)    (0x3380 + (n * 4))
#define RCAR_GEN4_CPG_D0WACR_SRCR(n)      (0x3400 + (n * 4))
#define RCAR_GEN4_CPG_D0WACR_SRSTCLR(n)   (0x3480 + (n * 4))
#define RCAR_GEN4_CPG_D1WACR_SRCR(n)      (0x3500 + (n * 4))
#define RCAR_GEN4_CPG_D1WACR_SRSTCLR(n)   (0x3580 + (n * 4))
#define RCAR_GEN4_CPG_D2WACR_SRCR(n)      (0x3600 + (n * 4))
#define RCAR_GEN4_CPG_D2WACR_SRSTCLR(n)   (0x3680 + (n * 4))
#define RCAR_GEN4_CPG_D3WACR_SRCR(n)      (0x3700 + (n * 4))
#define RCAR_GEN4_CPG_D3WACR_SRSTCLR(n)   (0x3780 + (n * 4))
#define RCAR_GEN4_CPG_D0WACRA(n)          (0x3800 + (n * 4))
#define RCAR_GEN4_CPG_D1WACRA(n)          (0x3900 + (n * 4))
#define RCAR_GEN4_CPG_D2WACRA(n)          (0x3A00 + (n * 4))
#define RCAR_GEN4_CPG_D3WACRA(n)          (0x3B00 + (n * 4))
#define RCAR_GEN4_CPG_D0WACRB(n)          (0x3C00 + (n * 4))
#define RCAR_GEN4_CPG_D1WACRB(n)          (0x3D00 + (n * 4))
#define RCAR_GEN4_CPG_D3WACRB(n)          (0x3F00 + (n * 4))

/* 
 * List of supported extal clocks
 */
#define RCAR_GEN4_EXT_CLK_16MHZ           (16666667)
#define RCAR_GEN4_EXT_CLK_20MHZ           (20000000)
#define RCAR_GEN4_EXT_CLK_40MHZ           (40000000)

/* Clock out */
#define RCAR_GEN4_CPG_CLK_Z0              (1800000000)
#define RCAR_GEN4_CPG_CLK_ZX              ( 900000000)
#define RCAR_GEN4_CPG_CLK_Z1              (1800000000)
#define RCAR_GEN4_CPG_CLK_ZR              (1066000000)
#define RCAR_GEN4_CPG_CLK_ZS              ( 266000000)
#define RCAR_GEN4_CPG_CLK_ZT              ( 533000000)
#define RCAR_GEN4_CPG_CLK_ZTR             ( 533000000)
#define RCAR_GEN4_CPG_CLK_S1              ( 533330000)
#define RCAR_GEN4_CPG_CLK_S3              ( 266660000)
#define RCAR_GEN4_CPG_CLK_S0D2            ( 400000000)
#define RCAR_GEN4_CPG_CLK_S0D3            ( 266660000)
#define RCAR_GEN4_CPG_CLK_S0D4            ( 200000000)
#define RCAR_GEN4_CPG_CLK_S0D2_MM         ( 400000000)
#define RCAR_GEN4_CPG_CLK_S0D3_MM         ( 266660000)
#define RCAR_GEN4_CPG_CLK_S0D4_MM         ( 200000000)
#define RCAR_GEN4_CPG_CLK_S0D2_RT         ( 400000000)
#define RCAR_GEN4_CPG_CLK_S0D3_RT         ( 266660000)
#define RCAR_GEN4_CPG_CLK_S0D4_RT         ( 200000000)
#define RCAR_GEN4_CPG_CLK_S0D6_RT         ( 133330000)
#define RCAR_GEN4_CPG_CLK_S0D24_RT        (  33330000)
#define RCAR_GEN4_CPG_CLK_S0D3_PER        ( 266660000)
#define RCAR_GEN4_CPG_CLK_S0D6_PER        ( 133330000)
#define RCAR_GEN4_CPG_CLK_S0D12_PER       (  66660000)
#define RCAR_GEN4_CPG_CLK_S0D24_PER       (  33330000)
#define RCAR_GEN4_CPG_CLK_S0D2_HSC        ( 400000000)
#define RCAR_GEN4_CPG_CLK_S0D3_HSC        ( 266660000)
#define RCAR_GEN4_CPG_CLK_S0D4_HSC        ( 200000000)
#define RCAR_GEN4_CPG_CLK_S0D6_HSC        ( 133330000)
#define RCAR_GEN4_CPG_CLK_S0D12_HSC       (  66660000)
#define RCAR_GEN4_CPG_CLK_S0D2_CC         ( 400000000)
#define RCAR_GEN4_CPG_CLK_S1D1            ( 533330000)
#define RCAR_GEN4_CPG_CLK_S1D2            ( 266660000)
#define RCAR_GEN4_CPG_CLK_S1D4            ( 133330000)
#define RCAR_GEN4_CPG_CLK_S1D8            (  66660000)
#define RCAR_GEN4_CPG_CLK_S1D12           (  33330000)
#define RCAR_GEN4_CPG_CLK_S2D1            ( 400000000)
#define RCAR_GEN4_CPG_CLK_S2D2            ( 200000000)
#define RCAR_GEN4_CPG_CLK_S2D4            ( 100000000)
#define RCAR_GEN4_CPG_CLK_S3D1            ( 266660000)
#define RCAR_GEN4_CPG_CLK_S3D2            ( 133330000)
#define RCAR_GEN4_CPG_CLK_S3D4            (  66660000)
#define RCAR_GEN4_CPG_CLK_LB              (  66660000)
#define RCAR_GEN4_CPG_CLK_CP              (  16660000)
#define RCAR_GEN4_CPG_CLK_CL              (  33330000)
#define RCAR_GEN4_CPG_CLK_CL16M           (  16666666)
#define RCAR_GEN4_CPG_CLK_ZB30            (1066000000)
#define RCAR_GEN4_CPG_CLK_ZB30D2          ( 533000000)
#define RCAR_GEN4_CPG_CLK_ZB30D4          ( 266000000)
#define RCAR_GEN4_CPG_CLK_ZB31            (1066000000)
#define RCAR_GEN4_CPG_CLK_ZB31D2          ( 533000000)
#define RCAR_GEN4_CPG_CLK_ZB31D4          ( 266000000)
#define RCAR_GEN4_CPG_CLK_SDSRC           ( 800000000)
#define RCAR_GEN4_CPG_CLK_SD0H            ( 800000000)
#define RCAR_GEN4_CPG_CLK_SD0             ( 200000000)
#define RCAR_GEN4_CPG_CLK_RPC             ( 320000000)
#define RCAR_GEN4_CPG_CLK_RPCD2           ( 160000000)
#define RCAR_GEN4_CPG_CLK_MS0             ( 133330000)
#define RCAR_GEN4_CPG_CLK_CANFD           (  80000000)
#define RCAR_GEN4_CPG_CLK_CSI0            (  25000000)
#define RCAR_GEN4_CPG_CLK_FRAY            (  80000000)
#define RCAR_GEN4_CPG_CLK_POST2           ( 133330000)
#define RCAR_GEN4_CPG_CLK_POST3           (  66660000)
#define RCAR_GEN4_CPG_CLK_DSI             ( 800000000)
#define RCAR_GEN4_CPG_CLK_VIP             ( 640000000)
#define RCAR_GEN4_CPG_CLK_ADGH            ( 800000000)
#define RCAR_GEN4_CPG_CLK_CNNDSP          ( 800000000)
#define RCAR_GEN4_CPG_CLK_ICU             ( 400000000)
#define RCAR_GEN4_CPG_CLK_ICUD2           ( 200000000)
#define RCAR_GEN4_CPG_CLK_VCBUS           ( 800000000)
#define RCAR_GEN4_CPG_CLK_CBFUSA          (  16660000)
#define RCAR_GEN4_CPG_CLK_CPEX            (  16660000)
#define RCAR_GEN4_CPG_CLK_RCLK            (     32800)
#define RCAR_GEN4_CPG_CLK_OSCCLK          (    131570)
#define RCAR_GEN4_CPG_CLK_SASYNCRT        (  16660000)
#define RCAR_GEN4_CPG_CLK_SASYNCPERD1     ( 266660000)
#define RCAR_GEN4_CPG_CLK_SASYNCPERD2     ( 133330000)
#define RCAR_GEN4_CPG_CLK_SASYNCPERD4     (  66660000)

/*
 * APMU
 */
#define RCAR_GEN4_APMU_DM0_BASE            0xE6170000
#define RCAR_GEN4_APMU_DM1_BASE            0xE6171000
#define RCAR_GEN4_APMU_DM2_BASE            0xE6172000
#define RCAR_GEN4_APMU_DM3_BASE            0xE6173000

#define RCAR_GEN4_APMU_ADMIN_GP            0x000
#define RCAR_GEN4_APMU_R52_GP              0x300
#define RCAR_GEN4_APMU_AP_CLUSTER0_GP      0x400
#define RCAR_GEN4_APMU_AP_CLUSTER1_GP      0x440
#define RCAR_GEN4_APMU_AP_CLUSTER2_GP      0x480
#define RCAR_GEN4_APMU_AP_CLUSTER3_GP      0x4C0
#define RCAR_GEN4_APMU_AP_CORE0_GP         0x800
#define RCAR_GEN4_APMU_AP_CORE1_GP         0x840
#define RCAR_GEN4_APMU_AP_CORE2_GP         0xA00
#define RCAR_GEN4_APMU_AP_CORE3_GP         0xA40
#define RCAR_GEN4_APMU_AP_CORE4_GP         0xC00
#define RCAR_GEN4_APMU_AP_CORE5_GP         0xC40
#define RCAR_GEN4_APMU_AP_CORE6_GP         0xE00
#define RCAR_GEN4_APMU_AP_CORE7_GP         0xE40

/* Admin Group */
#define RCAR_GEN4_APMU_WPCR                0x00
#define RCAR_GEN4_APMU_WPR                 0x04
#define RCAR_GEN4_APMU_D0ACCENR            0x10
#define RCAR_GEN4_APMU_D1ACCENR            0x14
#define RCAR_GEN4_APMU_D2ACCENR            0x18
#define RCAR_GEN4_APMU_D3ACCENR            0x1C
#define RCAR_GEN4_APMU_PTCSR               0x20
#define RCAR_GEN4_APMU_PTERADR             0x24
#define RCAR_GEN4_APMU_DCLSEIJTR           0x28
#define RCAR_GEN4_APMU_SYSMONCTRL          0x30
#define RCAR_GEN4_APMU_SYSMONSTSR          0x34
#define RCAR_GEN4_APMU_A3PWRCTRL0          0x40
#define RCAR_GEN4_APMU_A3PWRCTRL1          0x44
#define RCAR_GEN4_APMU_A3FSMSTSR0          0x48
#define RCAR_GEN4_APMU_A3FSMSTSR1          0x4C
#define RCAR_GEN4_APMU_A3FSMLOCKR0         0x50
#define RCAR_GEN4_APMU_A3FSMLOCKR1         0x54
#define RCAR_GEN4_APMU_INTSTSR             0x58
#define RCAR_GEN4_APMU_ERRSTSR             0x5C
#define RCAR_GEN4_APMU_FRSTR               0x60
#define RCAR_GEN4_APMU_FRSTD               0x64
#define RCAR_GEN4_APMU_FRSTCTRL            0x68
#define RCAR_GEN4_APMU_PADDCHKSTSR         0x80
#define RCAR_GEN4_APMU_PWDATACHKSTSR       0x84
#define RCAR_GEN4_APMU_APRTMGINTMASK       0x98
#define RCAR_GEN4_APMU_RSCTRL              0x9C
#define RCAR_GEN4_APMU_AA64nAA32CR         0xA0

/* R52 Group */
#define RCAR_GEN4_APMU_CR52CR              0x00
#define RCAR_GEN4_APMU_CR52RSTCTRL         0x04
#define RCAR_GEN4_APMU_FSMLOCKRCR52        0x0C
#define RCAR_GEN4_APMU_FSMSTSRCR52         0x18
#define RCAR_GEN4_APMU_G2GPRCR52           0x1C
#define RCAR_GEN4_APMU_CR52CMPEN           0x28
#define RCAR_GEN4_APMU_GCNTERRENCR52       0x2C
#define RCAR_GEN4_APMU_CR52RVBAR           0x30
#define RCAR_GEN4_APMU_CR52BAR             0x34
#define RCAR_GEN4_APMU_CR52RVBARP          0x38
#define RCAR_GEN4_APMU_CR52BARP            0x3C

/* A76/A55 Cluster Group */
#define RCAR_GEN4_APMU_PWRCTRLCL           0x00
 #define RCAR_GEN4_APMU_PWRCTRLCL_WUP_REQ          (1 << 0)
 #define RCAR_GEN4_APMU_PWRCTRLCL_PCHPDNEN         (1 << 17) 
#define RCAR_GEN4_APMU_L3CTRLCL            0x04
 #define RCAR_GEN4_APMU_L3CTRLCL_L3STS_MSK		  (7 << 16)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3STS_ALL_ON	  (4 << 16)	
 #define RCAR_GEN4_APMU_L3CTRLCL_L3STS_34_ON      (3 << 16)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3STS_24_ON      (2 << 16)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3STS_14_ON      (1 << 16)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3STS_OFF		  (0 << 16)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3DENY           (1 << 9)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3ACCEPT         (1 << 8)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3REQ		      (1 << 4)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_MSK       (7 << 0)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_ALL_ON    (4 << 0)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_34_ON	  (3 << 0)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_24_ON	  (2 << 0)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_14_ON	  (1 << 0)
 #define RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_OFF       (0 << 0)
#define RCAR_GEN4_APMU_FSMLOCKRCL          0x0C
#define RCAR_GEN4_APMU_PDENYSTSRCL         0x10
#define RCAR_GEN4_APMU_PDENYINTRCL         0x14
#define RCAR_GEN4_APMU_FSMSTSRCL           0x18
 #define RCAR_GEN4_APMU_FSMSTSRCL_STATE_MSK        0xFF
 #define RCAR_GEN4_APMU_FSMSTSRCL_STATE_ON         0x12
 #define RCAR_GEN4_APMU_FSMSTSRCL_STATE_OFF        0x00
 #define RCAR_GEN4_APMU_FSMSTSRCL_STATE_FUNC_RET   0x21
 #define RCAR_GEN4_APMU_FSMSTSRCL_STATE_MEM_RET    0x31
 #define RCAR_GEN4_APMU_A3PWRCTRL_A3_WUP_REQ           (1 << 0)
 #define RCAR_GEN4_APMU_A3PWRCTRL_A3_PDN_EN            (1 << 4)
 #define RCAR_GEN4_APMU_A3FSMSTSR_STATE_MSK        0xFF
 #define RCAR_GEN4_APMU_A3FSMSTSR_STATE_ON         0x04
 #define RCAR_GEN4_APMU_A3FSMSTSR_STATE_OFF        0x00
 
#define RCAR_GEN4_APMU_G2GPRCL             0x1C
#define RCAR_GEN4_APMU_SAFECTRLCL          0x20
#define RCAR_GEN4_APMU_DCLSENCL            0x24
#define RCAR_GEN4_APMU_DCLSCMPENCL         0x28
#define RCAR_GEN4_APMU_GCNTERRENCL         0x2C
#define RCAR_GEN4_APMU_CA55BAR             0x30
#define RCAR_GEN4_APMU_CA55BARP            0x38

/* A76/A55 Core Group */
#define RCAR_GEN4_APMU_PWRCTRLC            0x00
 #define RCAR_GEN4_APMU_PWRCTRLC_WUP_REQ           (1 << 0)
 #define RCAR_GEN4_APMU_PWRCTRLC_PCHPDNEN          (1 << 17) 
#define RCAR_GEN4_APMU_FSMLOCKRC           0x0C
#define RCAR_GEN4_APMU_PDENYSTSRC          0x10
#define RCAR_GEN4_APMU_PDENYINTRC          0x14
#define RCAR_GEN4_APMU_FSMSTSRC            0x18
 #define RCAR_GEN4_APMU_FSMSTSRC_STATE_MSK         0xFF
 #define RCAR_GEN4_APMU_FSMSTSRC_STATE_ON          0x0D
 #define RCAR_GEN4_APMU_FSMSTSRC_STATE_OFF         0x00
 #define RCAR_GEN4_APMU_FSMSTSRC_STATE_OFF_EMU	   0x1C
#define RCAR_GEN4_APMU_G2GPRC              0x1C
#define RCAR_GEN4_APMU_SAFECTRLC           0x20
 #define RCAR_GEN4_APMU_SAFECTRLC_DBGGEN_PPDN      (1 << 13)
#define RCAR_GEN4_APMU_RVBARLC             0x30
#define RCAR_GEN4_APMU_RVBARHC             0x34
#define RCAR_GEN4_APMU_RVBARPLC            0x38
#define RCAR_GEN4_APMU_RVBARPHC            0x3C

#define RCAR_GEN4_APMU_CLUSTER_OFFSET             0x200
#define RCAR_GEN4_APMU_CLUSTER_SIZE               0x40
#define RCAR_GEN4_APMU_CORE_IN_CLUSTER_SIZE       0x40
#define RCAR_GEN4_APMU_CORE_PER_CLUSTER           2
#define RCAR_GEN4_APMU_CORE_PER_DOMAIN            4

/*
 * Reset registers for Gen4 (V3U, S4)
 */
#define RCAR_GEN4_RST_DM0_BASE            0xE6160000
#define RCAR_GEN4_RST_DM1_BASE            0xE6164000
#define RCAR_GEN4_RST_DM2_BASE            0xE6168000
#define RCAR_GEN4_RST_DM3_BASE            0xE616C000
#define RCAR_GEN4_RST_SIZE                0x4000

/* Reset register offset */
#define RCAR_GEN4_RST_MODEMR0             0x0000
#define RCAR_GEN4_RST_MODEMR1             0x0004
#define RCAR_GEN4_RST_WDTRSTCR            0x0010
#define RCAR_GEN4_RST_RSTOUTCR            0x0014
#define RCAR_GEN4_RST_SRESCR0             0x0018
#define RCAR_GEN4_RST_SRESCR1             0x001C
#define RCAR_GEN4_RST_RSTFR0              0x0020
#define RCAR_GEN4_RST_RSTFR1              0x0024
#define RCAR_GEN4_RST_RSTFR2              0x0028
#define RCAR_GEN4_RST_RSTFR3              0x002C
#define RCAR_GEN4_RST_STBCHR0             0x0030
#define RCAR_GEN4_RST_STBCHR1             0x0034
#define RCAR_GEN4_RST_STBCHR2             0x0038
#define RCAR_GEN4_RST_STBCHR3             0x003C
#define RCAR_GEN4_RST_STBCHR4             0x0040
#define RCAR_GEN4_RST_STBCHR5             0x0044
#define RCAR_GEN4_RST_STBCHR6             0x0048
#define RCAR_GEN4_RST_STBCHR7             0x004C
#define RCAR_GEN4_RST_APBSFTYCHKR         0x0050
#define RCAR_GEN4_RST_ICUMXBAR            0x0054
#define RCAR_GEN4_RST_ICUMXCPCR           0x0058
#define RCAR_GEN4_RST_RSTPTCSR            0x0068
#define RCAR_GEN4_RST_RSTPTERADR          0x006C
#define RCAR_GEN4_RST_D0WACR              0x3800
#define RCAR_GEN4_RST_D1WACR              0x3A00
#define RCAR_GEN4_RST_D2WACR              0x3C00
#define RCAR_GEN4_RST_D3WACR              0x3E00

/*
 * System Controller for Gen4 (V3U, S4)
 */
#define RCAR_GEN4_SYSC_DM0_BASE          0xE6180000
#define RCAR_GEN4_SYSC_DM1_BASE          0xE6184000
#define RCAR_GEN4_SYSC_DM2_BASE          0xE6188000
#define RCAR_GEN4_SYSC_DM3_BASE          0xE618C000
#define RCAR_GEN4_SYSC_SIZE              0x4000

#define RCAR_GEN4_SYSC_SR                0x0000
#define RCAR_GEN4_SYSC_PTCSR             0x0010
#define RCAR_GEN4_SYSC_PTERADR           0x0014
#define RCAR_GEN4_SYSC_EFCR              0x0034
#define RCAR_GEN4_SYSC_PONSR0            0x0800
#define RCAR_GEN4_SYSC_PONSR1            0x0804
#define RCAR_GEN4_SYSC_POFFSR0           0x0808
#define RCAR_GEN4_SYSC_POFFSR1           0x080C
#define RCAR_GEN4_SYSC_ISCR0             0x0810
#define RCAR_GEN4_SYSC_ISCR1             0x0814
#define RCAR_GEN4_SYSC_IER0              0x0820
#define RCAR_GEN4_SYSC_IER1              0x0824
#define RCAR_GEN4_SYSC_IMR0              0x0830
#define RCAR_GEN4_SYSC_IMR1              0x0834

#define RCAR_GEN4_SYSC_PDRSR(n)          (0x1000 + ((n) * 64))
#define RCAR_GEN4_SYSC_PDRONCR(n)        (0x1004 + ((n) * 64))
#define RCAR_GEN4_SYSC_PDROFFCR(n)       (0x1008 + ((n) * 64))
#define RCAR_GEN4_SYSC_PDRESR(n)         (0x100C + ((n) * 64)) // Must be checked


#define RCAR_GEN4_SYSC_D0WACR0           0x3000
#define RCAR_GEN4_SYSC_D0WACR1           0x3004
#define RCAR_GEN4_SYSC_D0WACR2           0x3008
#define RCAR_GEN4_SYSC_D0WACR3           0x300C
#define RCAR_GEN4_SYSC_D0WACR4           0x3010
#define RCAR_GEN4_SYSC_D0WACR5           0x3014
#define RCAR_GEN4_SYSC_D1WACR0           0x3020
#define RCAR_GEN4_SYSC_D1WACR1           0x3024
#define RCAR_GEN4_SYSC_D1WACR2           0x3028
#define RCAR_GEN4_SYSC_D1WACR3           0x302C
#define RCAR_GEN4_SYSC_D1WACR4           0x3030
#define RCAR_GEN4_SYSC_D1WACR5           0x3034
#define RCAR_GEN4_SYSC_D2WACR0           0x3040
#define RCAR_GEN4_SYSC_D2WACR1           0x3044
#define RCAR_GEN4_SYSC_D2WACR2           0x3048
#define RCAR_GEN4_SYSC_D2WACR3           0x304C
#define RCAR_GEN4_SYSC_D2WACR4           0x3050
#define RCAR_GEN4_SYSC_D2WACR5           0x3054
#define RCAR_GEN4_SYSC_D3WACR0           0x3060
#define RCAR_GEN4_SYSC_D3WACR1           0x3064
#define RCAR_GEN4_SYSC_D3WACR2           0x3068
#define RCAR_GEN4_SYSC_D3WACR3           0x306C
#define RCAR_GEN4_SYSC_D3WACR4           0x3070
#define RCAR_GEN4_SYSC_D3WACR5           0x3074

/* SYSC for FuSa */
#define RCAR_GEN4_SYSC_RDNCSR            0x0020
#define RCAR_GEN4_SYSC_RDNIR             0x0024
#define RCAR_GEN4_SYSC_APBSR             0x0030
#define RCAR_GEN4_SYSC_ISOEHSR0          0x0860
#define RCAR_GEN4_SYSC_ISOEHSR1          0x0864
#define RCAR_GEN4_SYSC_ISOELSR0          0x0868
#define RCAR_GEN4_SYSC_ISOELSR1          0x086C
#define RCAR_GEN4_SYSC_ISOEHIR0          0x0870
#define RCAR_GEN4_SYSC_ISOEHIR1          0x0874
#define RCAR_GEN4_SYSC_ISOELIR0          0x0878
#define RCAR_GEN4_SYSC_ISOELIR1          0x087C
#define RCAR_GEN4_SYSC_ISOEHMR0          0x0C00
#define RCAR_GEN4_SYSC_ISOEHMR1          0x0C04
#define RCAR_GEN4_SYSC_ISOELMR0          0x0C08
#define RCAR_GEN4_SYSC_ISOELMR1          0x0C0C

/*
 * GIC-600 for Gen4 (V3U, S4)
 */
#define RCAR_GEN4_GICD_BASE               0xF1000000
#define RCAR_GEN4_GICA_BASE               0xF1010000
#define RCAR_GEN4_GICT_BASE               0xF1020000
#define RCAR_GEN4_GICP_BASE               0xF1030000
#define RCAR_GEN4_GICS_BASE               0xF1040000
#define RCAR_GEN4_GICR_LPI_BASE           0xF1060000
#define RCAR_GEN4_GICR_SGI_BASE           0xF1070000
#define RCAR_GEN4_GICDA_BASE              0xF1160000


/*
 * Interrupt ID for V3U
 */
#define RCAR_V3U_INTCSYS_IRQ0              (  0 + 32)
#define RCAR_V3U_INTCSYS_IRQ1              (  1 + 32)
#define RCAR_V3U_INTCSYS_IRQ2              (  2 + 32)
#define RCAR_V3U_INTCSYS_IRQ3              (  3 + 32)
#define RCAR_V3U_INTCSYS_IRQ4              (  4 + 32)
#define RCAR_V3U_INTCSYS_IRQ5              (  5 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_ERR      (  6 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_ERR      (  7 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_ERR       (  8 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_ERR       (  9 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_ERR       ( 10 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_ERR       ( 11 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_SEC_ERR   ( 12 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_SEC_ERR   ( 13 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_SEC_ERR   ( 14 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_SEC_ERR   ( 15 + 32)
#define RCAR_V3U_INTCSYS_SYSTEM_CONTROL    ( 18 + 32)
#define RCAR_V3U_INTCSYS_DBSC0             ( 19 + 32)
#define RCAR_V3U_INTCSYS_DBSC1             ( 20 + 32)
#define RCAR_V3U_INTCSYS_IPMMU             ( 21 + 32)
#define RCAR_V3U_INTCSYS_IPMMU_SEC         ( 22 + 32)
#define RCAR_V3U_INTCSYS_RCLK_WDT          ( 23 + 32)
#define RCAR_V3U_INTCSYS_SYSTEM_WDT        ( 24 + 32)
#define RCAR_V3U_INTCSYS_CANFD_IRQ_CH      ( 25 + 32)
#define RCAR_V3U_INTCSYS_CANFD_IRQ_GL      ( 26 + 32)
#define RCAR_V3U_INTCSYS_FRAY              ( 27 + 32)
#define RCAR_V3U_INTCSYS_HSCIF0            ( 28 + 32)
#define RCAR_V3U_INTCSYS_HSCIF1            ( 29 + 32)
#define RCAR_V3U_INTCSYS_HSCIF2            ( 30 + 32)
#define RCAR_V3U_INTCSYS_HSCIF3            ( 31 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_0        ( 32 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_1        ( 33 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_2        ( 34 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_3        ( 35 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_4        ( 36 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_5        ( 37 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_6        ( 38 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_7        ( 39 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC1_8        ( 40 + 32)  
#define RCAR_V3U_INTCSYS_SYSDMAC1_9        ( 41 + 32)  
#define RCAR_V3U_INTCSYS_SYSDMAC1_10       ( 42 + 32)   
#define RCAR_V3U_INTCSYS_SYSDMAC1_11       ( 43 + 32)   
#define RCAR_V3U_INTCSYS_SYSDMAC1_12       ( 44 + 32)  
#define RCAR_V3U_INTCSYS_SYSDMAC1_13       ( 45 + 32)  
#define RCAR_V3U_INTCSYS_SYSDMAC1_14       ( 46 + 32)  
#define RCAR_V3U_INTCSYS_SYSDMAC1_15       ( 47 + 32) 
#define RCAR_V3U_INTCSYS_SYSDMAC2_0        ( 48 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_1        ( 49 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_2        ( 50 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_3        ( 51 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_4        ( 52 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_5        ( 53 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_6        ( 54 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_7        ( 55 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_8        ( 1000 + 32) //Just for compile
#define RCAR_V3U_INTCSYS_SYSDMAC2_9        ( 1000 + 32) //Just for compile
#define RCAR_V3U_INTCSYS_SYSDMAC2_10       ( 1000 + 32) //Just for compile
#define RCAR_V3U_INTCSYS_SYSDMAC2_11       ( 1000 + 32) //Just for compile
#define RCAR_V3U_INTCSYS_SYSDMAC2_12       ( 1000 + 32) //Just for compile
#define RCAR_V3U_INTCSYS_SYSDMAC2_13       ( 1000 + 32) //Just for compile
#define RCAR_V3U_INTCSYS_SYSDMAC2_14       ( 1000 + 32) //Just for compile
#define RCAR_V3U_INTCSYS_SYSDMAC2_15       ( 1000 + 32) //Just for compile

#define RCAR_V3U_INTCSYS_SYSDMAC1_SEC_ERR  ( 56 + 32)
#define RCAR_V3U_INTCSYS_SYSDMAC2_SEC_ERR  ( 57 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_0         ( 64 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_1         ( 65 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_2         ( 66 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_3         ( 67 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_4         ( 68 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_5         ( 69 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_6         ( 70 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_7         ( 71 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_8         ( 72 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_9         ( 73 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_10        ( 74 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_11        ( 75 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_12        ( 76 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_13        ( 77 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_14        ( 78 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC0_15        ( 79 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_0         ( 80 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_1         ( 81 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_2         ( 82 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_3         ( 83 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_4         ( 84 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_5         ( 85 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_6         ( 86 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_7         ( 87 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_8         ( 88 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_9         ( 89 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_10        ( 90 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_11        ( 91 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_12        ( 92 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_13        ( 93 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_14        ( 94 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC1_15        ( 95 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_0         ( 96 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_1         ( 97 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_2         ( 98 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_3         ( 99 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_4         (100 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_5         (101 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_6         (102 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_7         (103 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_8         (104 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_9         (105 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_10        (106 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_11        (107 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_12        (108 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_13        (109 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_14        (110 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC2_15        (111 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_0         (112 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_1         (113 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_2         (114 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_3         (115 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_4         (116 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_5         (117 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_6         (118 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_7         (119 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_8         (120 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_9         (121 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_10        (122 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_11        (123 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_12        (124 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_13        (125 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_14        (126 + 32)
#define RCAR_V3U_INTCSYS_RTDMAC3_15        (127 + 32)
#define RCAR_V3U_INTCSYS_VSP2_VSPD_0       (128 + 32)
#define RCAR_V3U_INTCSYS_VSP2_VSPD_1       (129 + 32)
#define RCAR_V3U_INTCSYS_IV1E_0            (133 + 32)
#define RCAR_V3U_INTCSYS_IV1E_1            (134 + 32)
#define RCAR_V3U_INTCSYS_RPC               (135 + 32)
#define RCAR_V3U_INTCSYS_CPG               (136 + 32)
#define RCAR_V3U_INTCSYS_RST               (137 + 32)
#define RCAR_V3U_INTCSYS_CCI               (138 + 32)
#define RCAR_V3U_INTCSYS_ENYO0_PMU         (139 + 32)
#define RCAR_V3U_INTCSYS_ENYO1_PMU         (140 + 32)
#define RCAR_V3U_INTCSYS_ENYO2_PMU         (141 + 32)
#define RCAR_V3U_INTCSYS_ENYO3_PMU         (142 + 32)
#define RCAR_V3U_INTCSYS_DU0               (143 + 32)
#define RCAR_V3U_INTCSYS_DU1               (144 + 32)
#define RCAR_V3U_INTCSYS_IMRLX40           (146 + 32)
#define RCAR_V3U_INTCSYS_IMRLX41           (147 + 32)
#define RCAR_V3U_INTCSYS_IMRLX42           (148 + 32)
#define RCAR_V3U_INTCSYS_IMRLX43           (149 + 32)
#define RCAR_V3U_INTCSYS_IMS0              (150 + 32)
#define RCAR_V3U_INTCSYS_IMS1              (151 + 32)
#define RCAR_V3U_INTCSYS_ISP00             (152 + 32)
#define RCAR_V3U_INTCSYS_ISP01             (153 + 32)
#define RCAR_V3U_INTCSYS_ISP10             (154 + 32)
#define RCAR_V3U_INTCSYS_ISP11             (155 + 32)
#define RCAR_V3U_INTCSYS_ISP20             (156 + 32)
#define RCAR_V3U_INTCSYS_ISP21             (157 + 32)
#define RCAR_V3U_INTCSYS_ISP30             (158 + 32)
#define RCAR_V3U_INTCSYS_ISP31             (159 + 32)
#define RCAR_V3U_INTCSYS_VIN0              (160 + 32)
#define RCAR_V3U_INTCSYS_VIN1              (161 + 32)
#define RCAR_V3U_INTCSYS_VIN2              (162 + 32)
#define RCAR_V3U_INTCSYS_VIN3              (163 + 32)
#define RCAR_V3U_INTCSYS_VIN4              (164 + 32)
#define RCAR_V3U_INTCSYS_VIN5              (165 + 32)
#define RCAR_V3U_INTCSYS_VIN6              (166 + 32)
#define RCAR_V3U_INTCSYS_VIN7              (167 + 32)
#define RCAR_V3U_INTCSYS_VIN8              (168 + 32)
#define RCAR_V3U_INTCSYS_VIN9              (169 + 32)
#define RCAR_V3U_INTCSYS_VIN10             (170 + 32) 
#define RCAR_V3U_INTCSYS_VIN11             (171 + 32)
#define RCAR_V3U_INTCSYS_VIN12             (172 + 32)
#define RCAR_V3U_INTCSYS_VIN13             (173 + 32)
#define RCAR_V3U_INTCSYS_VIN14             (174 + 32)
#define RCAR_V3U_INTCSYS_VIN15             (175 + 32)
#define RCAR_V3U_INTCSYS_VIN16             (176 + 32)
#define RCAR_V3U_INTCSYS_VIN17             (177 + 32)
#define RCAR_V3U_INTCSYS_VIN18             (178 + 32)
#define RCAR_V3U_INTCSYS_VIN19             (179 + 32)
#define RCAR_V3U_INTCSYS_VIN20             (180 + 32)
#define RCAR_V3U_INTCSYS_VIN21             (181 + 32)
#define RCAR_V3U_INTCSYS_VIN22             (182 + 32)
#define RCAR_V3U_INTCSYS_VIN23             (183 + 32)
#define RCAR_V3U_INTCSYS_VIN24             (184 + 32)
#define RCAR_V3U_INTCSYS_VIN25             (185 + 32)
#define RCAR_V3U_INTCSYS_VIN26             (186 + 32) 
#define RCAR_V3U_INTCSYS_VIN27             (187 + 32)
#define RCAR_V3U_INTCSYS_VIN28             (188 + 32)
#define RCAR_V3U_INTCSYS_VIN29             (189 + 32)
#define RCAR_V3U_INTCSYS_VIN30             (190 + 32)
#define RCAR_V3U_INTCSYS_VIN31             (191 + 32)
#define RCAR_V3U_INTCSYS_CSI0              (192 + 32)
#define RCAR_V3U_INTCSYS_CSI1              (193 + 32)
#define RCAR_V3U_INTCSYS_CSI2              (194 + 32)
#define RCAR_V3U_INTCSYS_CSI3              (195 + 32)
#define RCAR_V3U_INTCSYS_DSI0_0            (196 + 32)
#define RCAR_V3U_INTCSYS_DSI0_1            (197 + 32)
#define RCAR_V3U_INTCSYS_DSI0_2            (198 + 32)
#define RCAR_V3U_INTCSYS_DSI0_3            (199 + 32)
#define RCAR_V3U_INTCSYS_DSI0_4            (200 + 32)
#define RCAR_V3U_INTCSYS_DSI0_5            (201 + 32)
#define RCAR_V3U_INTCSYS_DSI0_6            (202 + 32)
#define RCAR_V3U_INTCSYS_DSI0_7            (203 + 32)
#define RCAR_V3U_INTCSYS_DSI0_8            (204 + 32)
#define RCAR_V3U_INTCSYS_DSI1_0            (205 + 32)
#define RCAR_V3U_INTCSYS_DSI1_1            (206 + 32)
#define RCAR_V3U_INTCSYS_DSI1_2            (207 + 32)
#define RCAR_V3U_INTCSYS_DSI1_3            (208 + 32)
#define RCAR_V3U_INTCSYS_DSI1_4            (209 + 32)
#define RCAR_V3U_INTCSYS_DSI1_5            (210 + 32)
#define RCAR_V3U_INTCSYS_DSI1_6            (211 + 32)
#define RCAR_V3U_INTCSYS_DSI1_7            (212 + 32)
#define RCAR_V3U_INTCSYS_DSI1_8            (213 + 32)
#define RCAR_V3U_INTCSYS_APMU              (214 + 32)
#define RCAR_V3U_INTCSYS_CCI_HOST          (216 + 32)
#define RCAR_V3U_INTCSYS_3DGE              (223 + 32)
#define RCAR_V3U_INTCSYS_PCIE0             (224 + 32)
#define RCAR_V3U_INTCSYS_PCIE0_DMA         (225 + 32)
#define RCAR_V3U_INTCSYS_PCIE0_ERR         (226 + 32)
#define RCAR_V3U_INTCSYS_PCIE1             (227 + 32)
#define RCAR_V3U_INTCSYS_PCIE1_DMA         (228 + 32)
#define RCAR_V3U_INTCSYS_PCIE1_ERR         (229 + 32)
#define RCAR_V3U_INTCSYS_PCIE2             (230 + 32)
#define RCAR_V3U_INTCSYS_PCIE2_DMA         (231 + 32)
#define RCAR_V3U_INTCSYS_PCIE2_ERR         (232 + 32)
#define RCAR_V3U_INTCSYS_PCIE3             (233 + 32)
#define RCAR_V3U_INTCSYS_PCIE3_DMA         (234 + 32)
#define RCAR_V3U_INTCSYS_PCIE3_ERR         (235 + 32)                    
#define RCAR_V3U_INTCSYS_SDHI0             (236 + 32)
#define RCAR_V3U_INTCSYS_SDHI0_OTHER       (237 + 32)               
#define RCAR_V3U_INTCSYS_I2C0              (238 + 32)
#define RCAR_V3U_INTCSYS_I2C1              (239 + 32)
#define RCAR_V3U_INTCSYS_I2C2              (240 + 32)
#define RCAR_V3U_INTCSYS_I2C3              (241 + 32)
#define RCAR_V3U_INTCSYS_I2C4              (242 + 32)
#define RCAR_V3U_INTCSYS_I2C5              (243 + 32)
#define RCAR_V3U_INTCSYS_I2C6              (244 + 32)          
#define RCAR_V3U_INTCSYS_MSIOF0            (245 + 32)
#define RCAR_V3U_INTCSYS_MSIOF1            (246 + 32)
#define RCAR_V3U_INTCSYS_MSIOF2            (247 + 32)
#define RCAR_V3U_INTCSYS_MSIOF3            (248 + 32)
#define RCAR_V3U_INTCSYS_MSIOF4            (249 + 32) 
#define RCAR_V3U_INTCSYS_MSIOF5            (250 + 32)                      
#define RCAR_V3U_INTCSYS_SCIF0             (251 + 32)
#define RCAR_V3U_INTCSYS_SCIF1             (252 + 32)
#define RCAR_V3U_INTCSYS_SCIF3             (253 + 32)
#define RCAR_V3U_INTCSYS_SCIF4             (254 + 32)                              
#define RCAR_V3U_INTCSYS_LBSC              (255 + 32)               
#define RCAR_V3U_INTCSYS_RAVB0_0           (256 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_1           (257 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_2           (258 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_3           (259 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_4           (260 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_5           (261 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_6           (262 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_7           (263 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_8           (264 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_9           (265 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_10          (266 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_11          (267 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_12          (268 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_13          (269 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_14          (270 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_15          (271 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_16          (272 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_17          (273 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_18          (274 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_19          (275 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_20          (276 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_21          (277 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_22          (278 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_23          (279 + 32)
#define RCAR_V3U_INTCSYS_RAVB0_24          (280 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_0           (281 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_1           (282 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_2           (283 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_3           (284 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_4           (285 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_5           (286 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_6           (287 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_7           (288 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_8           (289 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_9           (290 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_10          (291 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_11          (292 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_12          (293 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_13          (294 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_14          (295 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_15          (296 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_16          (297 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_17          (298 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_18          (299 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_19          (300 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_20          (301 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_21          (302 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_22          (303 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_23          (304 + 32)
#define RCAR_V3U_INTCSYS_RAVB1_24          (305 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_0           (306 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_1           (307 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_2           (308 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_3           (309 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_4           (310 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_5           (311 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_6           (312 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_7           (313 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_8           (314 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_9           (315 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_10          (316 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_11          (317 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_12          (318 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_13          (319 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_14          (320 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_15          (321 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_16          (322 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_17          (323 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_18          (324 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_19          (325 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_20          (326 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_21          (327 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_22          (328 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_23          (329 + 32)
#define RCAR_V3U_INTCSYS_RAVB2_24          (330 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_0           (331 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_1           (332 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_2           (333 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_3           (334 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_4           (335 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_5           (336 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_6           (337 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_7           (338 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_8           (339 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_9           (340 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_10          (341 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_11          (342 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_12          (343 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_13          (344 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_14          (345 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_15          (346 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_16          (347 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_17          (348 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_18          (349 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_19          (350 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_20          (351 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_21          (352 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_22          (353 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_23          (354 + 32)
#define RCAR_V3U_INTCSYS_RAVB3_24          (355 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_0           (356 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_1           (357 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_2           (358 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_3           (359 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_4           (360 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_5           (361 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_6           (362 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_7           (363 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_8           (364 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_9           (365 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_10          (366 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_11          (367 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_12          (368 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_13          (369 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_14          (370 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_15          (371 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_16          (372 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_17          (373 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_18          (374 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_19          (375 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_20          (376 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_21          (377 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_22          (378 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_23          (379 + 32)
#define RCAR_V3U_INTCSYS_RAVB4_24          (380 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_0           (381 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_1           (382 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_2           (383 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_3           (384 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_4           (385 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_5           (386 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_6           (387 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_7           (388 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_8           (389 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_9           (390 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_10          (391 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_11          (392 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_12          (393 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_13          (394 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_14          (395 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_15          (396 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_16          (397 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_17          (398 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_18          (399 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_19          (400 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_20          (401 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_21          (402 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_22          (403 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_23          (404 + 32)
#define RCAR_V3U_INTCSYS_RAVB5_24          (405 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_CR520S    (406 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO0S    (407 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO1S    (408 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO2S    (409 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO3S    (410 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_CR521S    (411 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO4S    (412 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO5S    (413 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO6S    (414 + 32)
#define RCAR_V3U_INTCSYS_ICU_IMX_ENYO7S    (415 + 32)
#define RCAR_V3U_INTCSYS_PCI0_FATAL        (416 + 32)
#define RCAR_V3U_INTCSYS_PCI0_NONE_FATAL   (417 + 32)
#define RCAR_V3U_INTCSYS_PCI0_LP           (418 + 32)
#define RCAR_V3U_INTCSYS_PCI0_VNDMSG       (419 + 32)
#define RCAR_V3U_INTCSYS_PCI1_FATAL        (420 + 32)
#define RCAR_V3U_INTCSYS_PCI1_NONE_FATAL   (421 + 32)
#define RCAR_V3U_INTCSYS_PCI1_LP           (422 + 32)
#define RCAR_V3U_INTCSYS_PCI1_VNDMSG       (423 + 32)
#define RCAR_V3U_INTCSYS_PCI2_FATAL        (424 + 32)
#define RCAR_V3U_INTCSYS_PCI2_NONE_FATAL   (425 + 32)
#define RCAR_V3U_INTCSYS_PCI2_LP           (426 + 32)
#define RCAR_V3U_INTCSYS_PCI2_VNDMSG       (427 + 32)
#define RCAR_V3U_INTCSYS_PCI3_FATAL        (428 + 32)
#define RCAR_V3U_INTCSYS_PCI3_NONE_FATAL   (429 + 32)
#define RCAR_V3U_INTCSYS_PCI3_LP           (430 + 32)
#define RCAR_V3U_INTCSYS_PCI3_VNDMSG       (431 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_432      (432 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_433      (433 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_434      (434 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_435      (435 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_436      (436 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_437      (437 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_438      (438 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_439      (439 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR0    (440 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR1    (441 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR2    (442 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR3    (443 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR4    (444 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR5    (445 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR6    (446 + 32)
#define RCAR_V3U_INTCSYS_MFIS_AR_EI_CR7    (447 + 32)
#define RCAR_V3U_INTCSYS_CMT10             (448 + 32)
#define RCAR_V3U_INTCSYS_CMT11             (449 + 32)
#define RCAR_V3U_INTCSYS_CMT12             (450 + 32)
#define RCAR_V3U_INTCSYS_CMT13             (451 + 32)
#define RCAR_V3U_INTCSYS_CMT14             (452 + 32)
#define RCAR_V3U_INTCSYS_CMT15             (453 + 32)
#define RCAR_V3U_INTCSYS_CMT16             (454 + 32)
#define RCAR_V3U_INTCSYS_CMT17             (455 + 32)
#define RCAR_V3U_INTCSYS_CMT20             (456 + 32)
#define RCAR_V3U_INTCSYS_CMT21             (457 + 32)
#define RCAR_V3U_INTCSYS_CMT22             (458 + 32)
#define RCAR_V3U_INTCSYS_CMT23             (459 + 32)
#define RCAR_V3U_INTCSYS_CMT24             (460 + 32)
#define RCAR_V3U_INTCSYS_CMT25             (461 + 32)
#define RCAR_V3U_INTCSYS_CMT26             (462 + 32)
#define RCAR_V3U_INTCSYS_CMT27             (463 + 32)
#define RCAR_V3U_INTCSYS_CMT30             (464 + 32)
#define RCAR_V3U_INTCSYS_CMT31             (465 + 32)
#define RCAR_V3U_INTCSYS_CMT32             (466 + 32)
#define RCAR_V3U_INTCSYS_CMT33             (467 + 32)
#define RCAR_V3U_INTCSYS_CMT34             (468 + 32)
#define RCAR_V3U_INTCSYS_CMT35             (469 + 32)
#define RCAR_V3U_INTCSYS_CMT36             (470 + 32)
#define RCAR_V3U_INTCSYS_CMT37             (471 + 32)
#define RCAR_V3U_INTCSYS_TMU30             (472 + 32)
#define RCAR_V3U_INTCSYS_TMU31             (473 + 32)
#define RCAR_V3U_INTCSYS_TMU32             (474 + 32)
#define RCAR_V3U_INTCSYS_TMU33             (475 + 32)
#define RCAR_V3U_INTCSYS_TMU40             (476 + 32)
#define RCAR_V3U_INTCSYS_TMU41             (477 + 32)
#define RCAR_V3U_INTCSYS_TMU42             (478 + 32)
#define RCAR_V3U_INTCSYS_TMU43             (479 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT0       (480 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT0_ERR   (481 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT1       (482 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT1_ERR   (483 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT2       (484 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT2_ERR   (485 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT3       (486 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT3_ERR   (487 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT4       (488 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT4_ERR   (489 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT5       (490 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT5_ERR   (491 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT6       (492 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT6_ERR   (493 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT7       (494 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT7_ERR   (495 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT8       (496 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT8_ERR   (497 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT9       (498 + 32)
#define RCAR_V3U_INTCSYS_WINDOW_WDT9_ERR   (499 + 32)
#define RCAR_V3U_INTCSYS_CMT00             (500 + 32)
#define RCAR_V3U_INTCSYS_CMT01             (501 + 32)
#define RCAR_V3U_INTCSYS_SCMT              (502 + 32)
#define RCAR_V3U_INTCSYS_UCMT              (503 + 32)
#define RCAR_V3U_INTCSYS_TMU10             (504 + 32)
#define RCAR_V3U_INTCSYS_TMU11             (505 + 32)
#define RCAR_V3U_INTCSYS_TMU12             (506 + 32)
#define RCAR_V3U_INTCSYS_TMU13             (507 + 32)
#define RCAR_V3U_INTCSYS_TMU20             (508 + 32)
#define RCAR_V3U_INTCSYS_TMU21             (509 + 32)
#define RCAR_V3U_INTCSYS_TMU22             (510 + 32)
#define RCAR_V3U_INTCSYS_TMU23             (511 + 32)
#define RCAR_V3U_INTCSYS_TMU00             (512 + 32)
#define RCAR_V3U_INTCSYS_TMU01             (513 + 32)
#define RCAR_V3U_INTCSYS_TMU02             (514 + 32)
#define RCAR_V3U_INTCSYS_TPU               (515 + 32)
#define RCAR_V3U_INTCSYS_DOC               (516 + 32)
#define RCAR_V3U_INTCSYS_DOC_ACTMON        (517 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_518      (518 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_519      (519 + 32)
#define RCAR_V3U_INTCSYS_ADVFS0            (520 + 32)
#define RCAR_V3U_INTCSYS_ADVFS1            (521 + 32)
#define RCAR_V3U_INTCSYS_GIC_PMU           (522 + 32)
#define RCAR_V3U_INTCSYS_IREQ_CPU0         (523 + 32)
#define RCAR_V3U_INTCSYS_RFSO0             (524 + 32)
#define RCAR_V3U_INTCSYS_RFSO1             (525 + 32)
#define RCAR_V3U_INTCSYS_RFSO2             (526 + 32)
#define RCAR_V3U_INTCSYS_RFSO3             (527 + 32)
#define RCAR_V3U_INTCSYS_RFSO4             (528 + 32)
#define RCAR_V3U_INTCSYS_RFSO5             (529 + 32)
#define RCAR_V3U_INTCSYS_RFSO6             (530 + 32)
#define RCAR_V3U_INTCSYS_RFSO7             (531 + 32)
#define RCAR_V3U_INTCSYS_RFSO8             (532 + 32)
#define RCAR_V3U_INTCSYS_RFSO9             (533 + 32)
#define RCAR_V3U_INTCSYS_RFSO10            (534 + 32)
#define RCAR_V3U_INTCSYS_IPC               (535 + 32)
#define RCAR_V3U_INTCSYS_MTI1              (536 + 32)
#define RCAR_V3U_INTCSYS_MTI2              (537 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH        (538 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE           (539 + 32)
#define RCAR_V3U_INTCSYS_STAT              (540 + 32)
#define RCAR_V3U_INTCSYS_CSD               (541 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_542      (542 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_543      (543 + 32)
#define RCAR_V3U_INTCSYS_CLE0_STS          (544 + 32)
#define RCAR_V3U_INTCSYS_CLE0_ERR          (545 + 32)
#define RCAR_V3U_INTCSYS_CLE1_STS          (546 + 32)
#define RCAR_V3U_INTCSYS_CLE1_ERR          (547 + 32)
#define RCAR_V3U_INTCSYS_CLE2_STS          (548 + 32)
#define RCAR_V3U_INTCSYS_CLE2_ERR          (549 + 32)
#define RCAR_V3U_INTCSYS_CLE3_STS          (550 + 32)
#define RCAR_V3U_INTCSYS_CLE3_ERR          (551 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_552      (552 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_553      (553 + 32)
#define RCAR_V3U_INTCSYS_DISP0_STS         (554 + 32)
#define RCAR_V3U_INTCSYS_DISP0_ERR         (555 + 32)
#define RCAR_V3U_INTCSYS_DISP1_STS         (556 + 32)
#define RCAR_V3U_INTCSYS_DISP1_ERR         (557 + 32)
#define RCAR_V3U_INTCSYS_SMES0_STS         (558 + 32)
#define RCAR_V3U_INTCSYS_SMES0_ERR         (559 + 32)
#define RCAR_V3U_INTCSYS_SMES1_STS         (560 + 32)
#define RCAR_V3U_INTCSYS_SMES1_ERR         (561 + 32)
#define RCAR_V3U_INTCSYS_SMPO0_STS         (562 + 32)
#define RCAR_V3U_INTCSYS_SMPO0_ERR         (563 + 32)
#define RCAR_V3U_INTCSYS_SMPO1_STS         (564 + 32)
#define RCAR_V3U_INTCSYS_SMPO1_ERR         (565 + 32)
#define RCAR_V3U_INTCSYS_SMPS0_STS         (566 + 32)
#define RCAR_V3U_INTCSYS_SMPS0_ERR         (567 + 32)
#define RCAR_V3U_INTCSYS_SMPS1_STS         (568 + 32)
#define RCAR_V3U_INTCSYS_SMPS1_ERR         (569 + 32)
#define RCAR_V3U_INTCSYS_UMFI0_STS         (570 + 32)
#define RCAR_V3U_INTCSYS_UMFI0_ERR         (571 + 32)
#define RCAR_V3U_INTCSYS_UMFI1_STS         (572 + 32)
#define RCAR_V3U_INTCSYS_UMFI1_ERR         (573 + 32)
#define RCAR_V3U_INTCSYS_VCP_CINT          (574 + 32)
#define RCAR_V3U_INTCSYS_VCP_VINT          (575 + 32)
#define RCAR_V3U_INTCSYS_IMP00             (576 + 32)
#define RCAR_V3U_INTCSYS_IMP01             (577 + 32)
#define RCAR_V3U_INTCSYS_IMP02             (578 + 32)
#define RCAR_V3U_INTCSYS_IMP03             (579 + 32)
#define RCAR_V3U_INTCSYS_IMP10             (580 + 32)
#define RCAR_V3U_INTCSYS_IMP11             (581 + 32)
#define RCAR_V3U_INTCSYS_IMP12             (582 + 32)
#define RCAR_V3U_INTCSYS_IMP13             (583 + 32)
#define RCAR_V3U_INTCSYS_IMPL_DMAC         (584 + 32)
#define RCAR_V3U_INTCSYS_RADSP0            (585 + 32)
#define RCAR_V3U_INTCSYS_RADSP1            (586 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_587      (587 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_588      (588 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_589      (589 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_590      (590 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_591      (591 + 32)
#define RCAR_V3U_INTCSYS_VSP00             (592 + 32)
#define RCAR_V3U_INTCSYS_VSP01             (593 + 32)
#define RCAR_V3U_INTCSYS_VSP02             (594 + 32)
#define RCAR_V3U_INTCSYS_VSP03             (595 + 32)
#define RCAR_V3U_INTCSYS_VSP10             (596 + 32)
#define RCAR_V3U_INTCSYS_VSP11             (597 + 32)
#define RCAR_V3U_INTCSYS_VSP12             (598 + 32)
#define RCAR_V3U_INTCSYS_VSP13             (599 + 32)
#define RCAR_V3U_INTCSYS_VSPX0             (600 + 32)
#define RCAR_V3U_INTCSYS_VSPX1             (601 + 32)
#define RCAR_V3U_INTCSYS_VSPX2             (602 + 32)
#define RCAR_V3U_INTCSYS_VSPX3             (603 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_604      (604 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_605      (605 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_606      (606 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_607      (607 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH0       (608 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH1       (609 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH2       (610 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH3       (611 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH4       (612 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH5       (613 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH6       (614 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH7       (615 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH8       (616 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH9       (617 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH10      (618 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH11      (619 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH12      (620 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH13      (621 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH14      (622 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH15      (623 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH16      (624 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH17      (625 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH18      (626 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH19      (627 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH20      (628 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH21      (629 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH22      (630 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH23      (631 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH24      (632 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH25      (633 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH26      (634 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH27      (635 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH28      (636 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH29      (637 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH30      (638 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH31      (639 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH32      (640 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH33      (641 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH34      (642 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH35      (643 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH36      (644 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH37      (645 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH38      (646 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH39      (647 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH40      (648 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH41      (649 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH42      (650 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH43      (651 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH44      (652 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH45      (653 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH46      (654 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH47      (655 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH48      (656 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH49      (657 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH50      (658 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH51      (659 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH52      (660 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH53      (661 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH54      (662 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH55      (663 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH56      (664 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH57      (665 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH58      (666 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH59      (667 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH60      (668 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH61      (669 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH62      (670 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH63      (671 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH64      (672 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH65      (673 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH66      (674 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH67      (675 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH68      (676 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH69      (677 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH70      (678 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH71      (679 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH72      (680 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH73      (681 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH74      (682 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH75      (683 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH76      (684 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH77      (685 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH78      (686 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH79      (687 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH80      (688 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH81      (689 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH82      (690 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH83      (691 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH84      (692 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH85      (693 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH86      (694 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH87      (695 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH88      (696 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH89      (697 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH90      (698 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH91      (699 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH92      (700 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH93      (701 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH94      (702 + 32)
#define RCAR_V3U_INTCSYS_FBC_FINISH95      (703 + 32)     
#define RCAR_V3U_INTCSYS_FBC_PRE0          (704 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE1          (705 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE2          (706 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE3          (707 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE4          (708 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE5          (709 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE6          (710 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE7          (711 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE8          (712 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE9          (713 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE10         (714 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE11         (715 + 32) // Reserved
#define RCAR_V3U_INTCSYS_FBC_PRE12         (716 + 32) // Reserved
#define RCAR_V3U_INTCSYS_FBC_PRE13         (717 + 32) // Reserved
#define RCAR_V3U_INTCSYS_FBC_PRE14         (718 + 32) // Reserved
#define RCAR_V3U_INTCSYS_FBC_PRE15         (719 + 32) // Reserved
#define RCAR_V3U_INTCSYS_FBC_PRE16         (720 + 32) // Reserved
#define RCAR_V3U_INTCSYS_FBC_PRE17         (721 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE18         (722 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE19         (723 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE20         (724 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE21         (725 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE22         (726 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE23         (727 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE24         (728 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE25         (729 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE26         (730 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE27         (731 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE28         (732 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE29         (733 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE30         (734 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE31         (735 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE32         (736 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE33         (737 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE34         (738 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE35         (739 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE36         (740 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE37         (741 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE38         (742 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE39         (743 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE40         (744 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE41         (745 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE42         (746 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE43         (747 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE44         (748 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE45         (749 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE46         (750 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE47         (751 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE48         (752 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE49         (753 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE50         (754 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE51         (755 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE52         (756 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE53         (757 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE54         (758 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE55         (759 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE56         (760 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE57         (761 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE58         (762 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE59         (763 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE60         (764 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE61         (765 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE62         (766 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE63         (767 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE64         (768 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE65         (769 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE66         (770 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE67         (771 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE68         (772 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE69         (773 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE70         (774 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE71         (775 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE72         (776 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE73         (777 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE74         (778 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE75         (779 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE76         (780 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE77         (781 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE78         (782 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE79         (783 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE80         (784 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE81         (785 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE82         (786 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE83         (787 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE84         (788 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE85         (789 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE86         (790 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE87         (791 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE88         (792 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE89         (793 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE90         (794 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE91         (795 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE92         (796 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE93         (797 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE94         (798 + 32)
#define RCAR_V3U_INTCSYS_FBC_PRE95         (799 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR0          (800 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR1          (801 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR2          (802 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR3          (803 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR4          (804 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR5          (805 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR6          (806 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR7          (807 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR8          (808 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR9          (809 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR10         (810 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR11         (811 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR12         (812 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR13         (813 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR14         (814 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR15         (815 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR16         (816 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR17         (817 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR18         (818 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR19         (819 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR20         (820 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR21         (821 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR22         (822 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR23         (823 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR24         (824 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR25         (825 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR26         (826 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR27         (827 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR28         (828 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR29         (829 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR30         (830 + 32)
#define RCAR_V3U_INTCSYS_ECM_ERR31         (831 + 32)
#define RCAR_V3U_INTCSYS_GPIO00            (832 + 32)
#define RCAR_V3U_INTCSYS_GPIO01            (833 + 32)
#define RCAR_V3U_INTCSYS_GPIO02            (834 + 32)
#define RCAR_V3U_INTCSYS_GPIO03            (835 + 32)
#define RCAR_V3U_INTCSYS_GPIO10            (836 + 32)
#define RCAR_V3U_INTCSYS_GPIO11            (837 + 32)
#define RCAR_V3U_INTCSYS_GPIO12            (838 + 32)
#define RCAR_V3U_INTCSYS_GPIO13            (839 + 32)
#define RCAR_V3U_INTCSYS_GPIO20            (840 + 32)
#define RCAR_V3U_INTCSYS_GPIO21            (841 + 32)
#define RCAR_V3U_INTCSYS_GPIO22            (842 + 32)
#define RCAR_V3U_INTCSYS_GPIO23            (843 + 32)
#define RCAR_V3U_INTCSYS_GPIO30            (844 + 32)
#define RCAR_V3U_INTCSYS_GPIO31            (845 + 32)
#define RCAR_V3U_INTCSYS_GPIO32            (846 + 32)
#define RCAR_V3U_INTCSYS_GPIO33            (847 + 32)
#define RCAR_V3U_INTCSYS_GPIO40            (848 + 32)
#define RCAR_V3U_INTCSYS_GPIO41            (849 + 32)
#define RCAR_V3U_INTCSYS_GPIO42            (850 + 32)
#define RCAR_V3U_INTCSYS_GPIO43            (851 + 32)
#define RCAR_V3U_INTCSYS_GPIO50            (852 + 32)
#define RCAR_V3U_INTCSYS_GPIO51            (853 + 32)
#define RCAR_V3U_INTCSYS_GPIO52            (854 + 32)
#define RCAR_V3U_INTCSYS_GPIO53            (855 + 32)
#define RCAR_V3U_INTCSYS_GPIO60            (856 + 32)
#define RCAR_V3U_INTCSYS_GPIO61            (857 + 32)
#define RCAR_V3U_INTCSYS_GPIO62            (858 + 32)
#define RCAR_V3U_INTCSYS_GPIO63            (859 + 32)
#define RCAR_V3U_INTCSYS_GPIO70            (860 + 32)
#define RCAR_V3U_INTCSYS_GPIO71            (861 + 32)
#define RCAR_V3U_INTCSYS_GPIO72            (862 + 32)
#define RCAR_V3U_INTCSYS_GPIO73            (863 + 32)
#define RCAR_V3U_INTCSYS_GPIO80            (864 + 32)
#define RCAR_V3U_INTCSYS_GPIO81            (865 + 32)
#define RCAR_V3U_INTCSYS_GPIO82            (866 + 32)
#define RCAR_V3U_INTCSYS_GPIO83            (867 + 32)
#define RCAR_V3U_INTCSYS_GPIO90            (868 + 32)
#define RCAR_V3U_INTCSYS_GPIO91            (869 + 32)
#define RCAR_V3U_INTCSYS_GPIO92            (870 + 32)
#define RCAR_V3U_INTCSYS_GPIO93            (871 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_872      (872 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_873      (873 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_874      (874 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_875      (875 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_876      (876 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_877      (877 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_878      (878 + 32)
#define RCAR_V3U_INTCSYS_RESERVED_879      (879 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC0       (880 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC1       (881 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC2       (882 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC3       (883 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC4       (884 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC5       (885 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC6       (886 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC7       (887 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC8       (888 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC9       (889 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC10      (890 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC11      (891 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC12      (892 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC13      (893 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC14      (894 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DMAC15      (895 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC0       (896 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC1       (897 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC2       (898 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC3       (899 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC4       (900 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC5       (901 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC6       (902 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC7       (903 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC8       (904 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC9       (905 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC10      (906 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC11      (907 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC12      (908 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC13      (909 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC14      (910 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DMAC15      (911 + 32)
#define RCAR_V3U_INTCSYS_RADSP0DSPW        (912 + 32)
#define RCAR_V3U_INTCSYS_RADSP1DSPW        (913 + 32)
#define RCAR_V3U_INTCSYS_ECM_RT            (914 + 32)
#define RCAR_V3U_INTCSYS_ECM_PER1          (915 + 32)
#define RCAR_V3U_INTCSYS_ECM_MM            (916 + 32)
#define RCAR_V3U_INTCSYS_ECM_PER0          (917 + 32)
#define RCAR_V3U_INTCSYS_ECM_VIP           (918 + 32)
#define RCAR_V3U_INTCSYS_ECM_3DGE          (919 + 32)
#define RCAR_V3U_INTCSYS_ECM_VC            (920 + 32)
#define RCAR_V3U_INTCSYS_ECM_VIO           (921 + 32)
#define RCAR_V3U_INTCSYS_ECM_TOP           (922 + 32)
#define RCAR_V3U_INTCSYS_ECM_IR            (923 + 32)
#define RCAR_V3U_INTCSYS_WCRC0             (924 + 32)
#define RCAR_V3U_INTCSYS_WCRC1             (925 + 32)
#define RCAR_V3U_INTCSYS_WCRC2             (926 + 32)
#define RCAR_V3U_INTCSYS_WCRC3             (927 + 32)
#define RCAR_V3U_INTCSYS_TSIP00            (928 + 32)
#define RCAR_V3U_INTCSYS_TSIP01            (929 + 32)
#define RCAR_V3U_INTCSYS_TSIP10            (930 + 32)
#define RCAR_V3U_INTCSYS_TSIP11            (931 + 32)
#define RCAR_V3U_INTCSYS_TSIP20            (932 + 32)
#define RCAR_V3U_INTCSYS_TSIP21            (933 + 32)
#define RCAR_V3U_INTCSYS_TSIP30            (934 + 32)
#define RCAR_V3U_INTCSYS_TSIP31            (935 + 32)
#define RCAR_V3U_INTCSYS_TSIP40            (936 + 32)
#define RCAR_V3U_INTCSYS_TSIP41            (937 + 32)
#define RCAR_V3U_INTCSYS_TSIP50            (938 + 32)
#define RCAR_V3U_INTCSYS_TSIP51            (939 + 32)
#define RCAR_V3U_INTCSYS_TSIP60            (940 + 32)
#define RCAR_V3U_INTCSYS_TSIP61            (941 + 32)
#define RCAR_V3U_INTCSYS_TSIP70            (942 + 32)
#define RCAR_V3U_INTCSYS_TSIP71            (943 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_CR520       (944 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO0       (945 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO1       (946 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO2       (947 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO3       (948 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_CR521       (949 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO4       (950 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO5       (951 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO6       (952 + 32)
#define RCAR_V3U_INTCSYS_ICUMX_ENYO7       (953 + 32)


/*
 * Interrupt ID for S4
 */
#define RCAR_S4_INTCSYS_IRQ0                        32
#define RCAR_S4_INTCSYS_IRQ1                        33
#define RCAR_S4_INTCSYS_IRQ2                        34
#define RCAR_S4_INTCSYS_IRQ3                        35
#define RCAR_S4_INTCSYS_IRQ4                        36
#define RCAR_S4_INTCSYS_IRQ5                        37
#define RCAR_S4_INCTSYS_PMU                         38
#define RCAR_S4_INCTSYS_DBSC                        51
#define RCAR_S4_INCTSYS_IPMMUMM_ERR_NSEC            52
#define RCAR_S4_INCTSYS_IPMMUMM_ERR_SEC             53
#define RCAR_S4_INTCSYS_RTDM0_SEC_ERR	            60
#define RCAR_S4_INTCSYS_RTDM0_NSEC_ERR	            61
#define RCAR_S4_INTCSYS_RTDM00	                    62
#define RCAR_S4_INTCSYS_RTDM01	                    63
#define RCAR_S4_INTCSYS_RTDM02	                    64
#define RCAR_S4_INTCSYS_RTDM03	                    65
#define RCAR_S4_INTCSYS_RTDM04	                    66
#define RCAR_S4_INTCSYS_RTDM05	                    67
#define RCAR_S4_INTCSYS_RTDM06	                    68
#define RCAR_S4_INTCSYS_RTDM07	                    69
#define RCAR_S4_INTCSYS_RTDM08	                    70
#define RCAR_S4_INTCSYS_RTDM09	                    71
#define RCAR_S4_INTCSYS_RTDM010	                    72
#define RCAR_S4_INTCSYS_RTDM011	                    73
#define RCAR_S4_INTCSYS_RTDM012	                    74
#define RCAR_S4_INTCSYS_RTDM013	                    75
#define RCAR_S4_INTCSYS_RTDM014	                    76
#define RCAR_S4_INTCSYS_RTDM015	                    77
#define RCAR_S4_INTCSYS_RTDM1_SEC_ERR	            78
#define RCAR_S4_INTCSYS_RTDM11_NSEC_ERR	            79
#define RCAR_S4_INTCSYS_RTDM10	                    80
#define RCAR_S4_INTCSYS_RTDM11	                    81
#define RCAR_S4_INTCSYS_RTDM12	                    82
#define RCAR_S4_INTCSYS_RTDM13	                    83
#define RCAR_S4_INTCSYS_RTDM14	                    84
#define RCAR_S4_INTCSYS_RTDM15	                    85
#define RCAR_S4_INTCSYS_RTDM16	                    86
#define RCAR_S4_INTCSYS_RTDM17	                    87
#define RCAR_S4_INTCSYS_RTDM18	                    88
#define RCAR_S4_INTCSYS_RTDM19	                    89
#define RCAR_S4_INTCSYS_RTDM110	                    90
#define RCAR_S4_INTCSYS_RTDM111	                    91
#define RCAR_S4_INTCSYS_RTDM112	                    92
#define RCAR_S4_INTCSYS_RTDM113	                    93
#define RCAR_S4_INTCSYS_RTDM114	                    94
#define RCAR_S4_INTCSYS_RTDM115	                    95
#define RCAR_S4_INTCSYS_RTDM2_SEC_ERR	            96
#define RCAR_S4_INTCSYS_RTDM2_NSEC_ERR	            97
#define RCAR_S4_INTCSYS_RTDM20	                    98
#define RCAR_S4_INTCSYS_RTDM21	                    99
#define RCAR_S4_INTCSYS_RTDM22	                    100
#define RCAR_S4_INTCSYS_RTDM23	                    101
#define RCAR_S4_INTCSYS_RTDM24	                    102
#define RCAR_S4_INTCSYS_RTDM25	                    103
#define RCAR_S4_INTCSYS_RTDM26	                    104
#define RCAR_S4_INTCSYS_RTDM27	                    105
#define RCAR_S4_INTCSYS_RTDM28	                    106
#define RCAR_S4_INTCSYS_RTDM29	                    107
#define RCAR_S4_INTCSYS_RTDM210	                    108
#define RCAR_S4_INTCSYS_RTDM211	                    109
#define RCAR_S4_INTCSYS_RTDM212	                    110
#define RCAR_S4_INTCSYS_RTDM213	                    111
#define RCAR_S4_INTCSYS_RTDM214	                    112
#define RCAR_S4_INTCSYS_RTDM215	                    113
#define RCAR_S4_INTCSYS_RTDM3_SEC_ERR	            114
#define RCAR_S4_INTCSYS_RTDM3_NSEC_ERR	            115
#define RCAR_S4_INTCSYS_RTDM30	                    116
#define RCAR_S4_INTCSYS_RTDM31	                    117
#define RCAR_S4_INTCSYS_RTDM32	                    118
#define RCAR_S4_INTCSYS_RTDM33	                    119
#define RCAR_S4_INTCSYS_RTDM34	                    120
#define RCAR_S4_INTCSYS_RTDM35	                    121
#define RCAR_S4_INTCSYS_RTDM36	                    122
#define RCAR_S4_INTCSYS_RTDM37	                    123
#define RCAR_S4_INTCSYS_RTDM38	                    124
#define RCAR_S4_INTCSYS_RTDM39	                    125
#define RCAR_S4_INTCSYS_RTDM310	                    126
#define RCAR_S4_INTCSYS_RTDM311	                    127
#define RCAR_S4_INTCSYS_RTDM312	                    128
#define RCAR_S4_INTCSYS_RTDM313	                    129
#define RCAR_S4_INTCSYS_RTDM314	                    130
#define RCAR_S4_INTCSYS_RTDM315	                    131
#define RCAR_S4_INTCSYS_SYDM1_SEC_ERR	            132
#define RCAR_S4_INTCSYS_SYDM1_NSEC_ERR	            133
#define RCAR_S4_INTCSYS_SYDM10	                    134
#define RCAR_S4_INTCSYS_SYDM11	                    135
#define RCAR_S4_INTCSYS_SYDM12	                    136
#define RCAR_S4_INTCSYS_SYDM13	                    137
#define RCAR_S4_INTCSYS_SYDM14	                    138
#define RCAR_S4_INTCSYS_SYDM15	                    139
#define RCAR_S4_INTCSYS_SYDM16	                    140
#define RCAR_S4_INTCSYS_SYDM17	                    141
#define RCAR_S4_INTCSYS_SYDM18	                    142
#define RCAR_S4_INTCSYS_SYDM19	                    143
#define RCAR_S4_INTCSYS_SYDM110	                    144
#define RCAR_S4_INTCSYS_SYDM111	                    145
#define RCAR_S4_INTCSYS_SYDM112	                    146
#define RCAR_S4_INTCSYS_SYDM113	                    147
#define RCAR_S4_INTCSYS_SYDM114	                    148
#define RCAR_S4_INTCSYS_SYDM115	                    149
#define RCAR_S4_INTCSYS_SYDM2_SEC_ERR               150
#define RCAR_S4_INTCSYS_SYDM2_NSEC_ERR	            151
#define RCAR_S4_INTCSYS_SYDM20	                    152
#define RCAR_S4_INTCSYS_SYDM21	                    153
#define RCAR_S4_INTCSYS_SYDM22	                    154
#define RCAR_S4_INTCSYS_SYDM23	                    155
#define RCAR_S4_INTCSYS_SYDM24	                    156
#define RCAR_S4_INTCSYS_SYDM25	                    157
#define RCAR_S4_INTCSYS_SYDM26	                    158
#define RCAR_S4_INTCSYS_SYDM27	                    159
#define RCAR_S4_INTCSYS_CR0	                        165
#define RCAR_S4_INTCSYS_SYSC	                    166
#define RCAR_S4_INTCSYS_RPC	                        167
#define RCAR_S4_INTCSYS_CPG	                        168
#define RCAR_S4_INTCSYS_RESET	                    169
#define RCAR_S4_INTCSYS_CCI500SS	                170
#define RCAR_S4_INTCSYS_CA55SS0_0	                171
#define RCAR_S4_INTCSYS_CA55SS0_1	                172
#define RCAR_S4_INTCSYS_CA55SS1_0	                173
#define RCAR_S4_INTCSYS_CA55SS1_1	                174
#define RCAR_S4_INTCSYS_STAT	                    175
#define RCAR_S4_INTCSYS_APMU	                    176
#define RCAR_S4_INTCSYS_CSD	                        177
#define RCAR_S4_INTCSYS_SYDM28	                    179
#define RCAR_S4_INTCSYS_SYDM29	                    180
#define RCAR_S4_INTCSYS_SYDM210	                    181
#define RCAR_S4_INTCSYS_SYDM211	                    182
#define RCAR_S4_INTCSYS_SYDM212	                    183
#define RCAR_S4_INTCSYS_SYDM213	                    184
#define RCAR_S4_INTCSYS_SYDM214	                    185
#define RCAR_S4_INTCSYS_SYDM215	                    186
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM0	    224
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM1     225
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM2	    226
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM3	    227
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM4	    228
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM5	    229
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM6	    230
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM7	    231
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM8	    232
#define RCAR_S4_INTCSYS_ICUMX_SINGLE_ICUP_2_ARM9	    233
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM0	        234
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM1	        235
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM2	        236
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM3	        237
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM4	        238
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM5	        239
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM6	        240
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM7	        241
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM8	        242
#define RCAR_S4_INTCSYS_ICUMX_ICUP_2_ARM9	        243
#define RCAR_S4_INTCSYS_RFSO0	                    250
#define RCAR_S4_INTCSYS_RFSO1	                    251
#define RCAR_S4_INTCSYS_RFSO2	                    252
#define RCAR_S4_INTCSYS_RFSO3	                    253
#define RCAR_S4_INTCSYS_RFSO4	                    254
#define RCAR_S4_INTCSYS_RFSO5	                    255
#define RCAR_S4_INTCSYS_RFSO6	                    256
#define RCAR_S4_INTCSYS_RFSO7	                    257
#define RCAR_S4_INTCSYS_RFSO8	                    258
#define RCAR_S4_INTCSYS_RFSO9	                    259
#define RCAR_S4_INTCSYS_RFSO10	                    260
#define RCAR_S4_INTCSYS_MSIOF0	                    262
#define RCAR_S4_INTCSYS_MSIOF1	                    263
#define RCAR_S4_INTCSYS_MSIOF2	                    264
#define RCAR_S4_INTCSYS_MSIOF3	                    265
#define RCAR_S4_INTCSYS_UFS_DEBUG	                266
#define RCAR_S4_INTCSYS_UFS_HC	                    267
#define RCAR_S4_INTCSYS_SDHI0	                    268
#define RCAR_S4_INTCSYS_I2C0	                    270
#define RCAR_S4_INTCSYS_I2C1	                    271
#define RCAR_S4_INTCSYS_I2C2	                    272
#define RCAR_S4_INTCSYS_I2C3	                    273
#define RCAR_S4_INTCSYS_I2C4	                    274
#define RCAR_S4_INTCSYS_I2C5	                    275
#define RCAR_S4_INTCSYS_HSCIF0	                    277
#define RCAR_S4_INTCSYS_HSCIF1	                    278
#define RCAR_S4_INTCSYS_HSCIF2	                    279
#define RCAR_S4_INTCSYS_HSCIF3	                    280
#define RCAR_S4_INTCSYS_SCIF0	                    281
#define RCAR_S4_INTCSYS_SCIF1	                    282
#define RCAR_S4_INTCSYS_SCIF3	                    284
#define RCAR_S4_INTCSYS_SCIF4	                    285
#define RCAR_S4_INTCSYS_RSW2_MFWD_ERR               288
#define RCAR_S4_INTCSYS_RSW2_RACE_ERR               289
#define RCAR_S4_INTCSYS_RSW2_COMA_ERR               290
#define RCAR_S4_INTCSYS_RSW2_GWCA0_ERR              291
#define RCAR_S4_INTCSYS_RSW2_GWCA1_ERR              292
#define RCAR_S4_INTCSYS_RSW2_ETHA0_ERR              293
#define RCAR_S4_INTCSYS_RSW2_ETHA1_ERR              294
#define RCAR_S4_INTCSYS_RSW2_ETHA2_ERR              295
#define RCAR_S4_INTCSYS_RSW2_GPTP0_STS	            297
#define RCAR_S4_INTCSYS_RSW2_GPTP1_STS	            298
#define RCAR_S4_INTCSYS_RSW2_MFWD_STS	            299
#define RCAR_S4_INTCSYS_RSW2_RACE_STS	            300
#define RCAR_S4_INTCSYS_RSW2_COMA_STS	            301
#define RCAR_S4_INTCSYS_RSW2_GWCA0_STS	            302
#define RCAR_S4_INTCSYS_RSW2_GWCA1_STS	            303
#define RCAR_S4_INTCSYS_RSW2_ETHA0_STS	            304
#define RCAR_S4_INTCSYS_RSW2_ETHA1_STS	            305
#define RCAR_S4_INTCSYS_RSW2_ETHA2_STS	            306
#define RCAR_S4_INTCSYS_RSW2_RMAC0_STS	            308
#define RCAR_S4_INTCSYS_RSW2_RMAC1_STS	            309
#define RCAR_S4_INTCSYS_RSW2_RMAC2_STS	            310
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_0           312
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_1           313
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_2           314
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_3           315
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_4           316
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_5           317
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_6           318
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTX_7           319
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_0           320
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_1           321
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_2           322
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_3           323
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_4           324
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_5           325
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_6           326
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTX_7           327
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTS_0           328
#define RCAR_S4_INTCSYS_RSW2_GWCA0_RXTS_1           329
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTS_0           330
#define RCAR_S4_INTCSYS_RSW2_GWCA1_RXTS_1           331
#define RCAR_S4_INTCSYS_RSW2_RAMC0_MDIO             332
#define RCAR_S4_INTCSYS_RSW2_RAMC1_MDIO             333
#define RCAR_S4_INTCSYS_RSW2_RAMC2_MDIO             334
#define RCAR_S4_INTCSYS_RSW2_RAMC0_PHY	            336
#define RCAR_S4_INTCSYS_RSW2_RAMC1_PHY	            337
#define RCAR_S4_INTCSYS_RSW2_RAMC2_PHY	            338
#define RCAR_S4_INTCSYS_MFI_AR_2_AP0_E              352
#define RCAR_S4_INTCSYS_MFI_AR_2_AP0_I              353
#define RCAR_S4_INTCSYS_MFI_AR_2_AP1_E              354
#define RCAR_S4_INTCSYS_MFI_AR_2_AP1_I              355
#define RCAR_S4_INTCSYS_MFI_AR_2_AP2_E              356
#define RCAR_S4_INTCSYS_MFI_AR_2_AP2_I              357
#define RCAR_S4_INTCSYS_MFI_AR_2_AP3_E              358
#define RCAR_S4_INTCSYS_MFI_AR_2_AP3_I              359
#define RCAR_S4_INTCSYS_MFI_AR_2_AP4_E              360
#define RCAR_S4_INTCSYS_MFI_AR_2_AP4_I              361
#define RCAR_S4_INTCSYS_MFI_AR_2_AP5_E              362
#define RCAR_S4_INTCSYS_MFI_AR_2_AP5_I              363
#define RCAR_S4_INTCSYS_MFI_AR_2_AP6_E              364
#define RCAR_S4_INTCSYS_MFI_AR_2_AP6_I              365
#define RCAR_S4_INTCSYS_MFI_AR_2_AP7_E              366
#define RCAR_S4_INTCSYS_MFI_AR_2_AP7_I              367
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP0_E       368
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP1_E      	370
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP2_E      	372
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP3_E      	374
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP4_E      	376
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP5_E      	378
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP6_E      	380
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP7_E      	382
#define RCAR_S4_INTCSYS_MFI_G4MH_PER0_2_AP8_E      	385
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP0_E      	386
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP1_E      	388
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP2_E      	390
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP3_E      	392
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP4_E      	394
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP5_E      	396
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP6_E      	398
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP7_E      	400
#define RCAR_S4_INTCSYS_MFI_G4MH_PER1_2_AP8_E      	403
#define RCAR_S4_INTCSYS_CMT00	                    480
#define RCAR_S4_INTCSYS_CMT01	                    481
#define RCAR_S4_INTCSYS_CMT10	                    482
#define RCAR_S4_INTCSYS_CMT11	                    483
#define RCAR_S4_INTCSYS_CMT12	                    484
#define RCAR_S4_INTCSYS_CMT13	                    485
#define RCAR_S4_INTCSYS_CMT14	                    486
#define RCAR_S4_INTCSYS_CMT15	                    487
#define RCAR_S4_INTCSYS_CMT16	                    488
#define RCAR_S4_INTCSYS_CMT17	                    489
#define RCAR_S4_INTCSYS_CMT20	                    490
#define RCAR_S4_INTCSYS_CMT21	                    491
#define RCAR_S4_INTCSYS_CMT22	                    492
#define RCAR_S4_INTCSYS_CMT23	                    493
#define RCAR_S4_INTCSYS_CMT24	                    494
#define RCAR_S4_INTCSYS_CMT25	                    495
#define RCAR_S4_INTCSYS_CMT26	                    496
#define RCAR_S4_INTCSYS_CMT27	                    497
#define RCAR_S4_INTCSYS_CMT30	                    498
#define RCAR_S4_INTCSYS_CMT31	                    499
#define RCAR_S4_INTCSYS_CMT32	                    500
#define RCAR_S4_INTCSYS_CMT33	                    501
#define RCAR_S4_INTCSYS_CMT34	                    502
#define RCAR_S4_INTCSYS_CMT35	                    503
#define RCAR_S4_INTCSYS_CMT36	                    504
#define RCAR_S4_INTCSYS_CMT37	                    505
#define RCAR_S4_INTCSYS_TMU00	                    506
#define RCAR_S4_INTCSYS_TMU01	                    507
#define RCAR_S4_INTCSYS_TMU02	                    508
#define RCAR_S4_INTCSYS_TMU10	                    509
#define RCAR_S4_INTCSYS_TMU11	                    510
#define RCAR_S4_INTCSYS_TMU12	                    511
#define RCAR_S4_INTCSYS_TMU13	                    512
#define RCAR_S4_INTCSYS_TMU20	                    513
#define RCAR_S4_INTCSYS_TMU21	                    514
#define RCAR_S4_INTCSYS_TMU22	                    515
#define RCAR_S4_INTCSYS_TMU23	                    516
#define RCAR_S4_INTCSYS_TMU30	                    517
#define RCAR_S4_INTCSYS_TMU31	                    518
#define RCAR_S4_INTCSYS_TMU32	                    519
#define RCAR_S4_INTCSYS_TMU33	                    520
#define RCAR_S4_INTCSYS_TMU40	                    521
#define RCAR_S4_INTCSYS_TMU41	                    522
#define RCAR_S4_INTCSYS_TMU42	                    523
#define RCAR_S4_INTCSYS_TMU43	                    524
#define RCAR_S4_INTCSYS_WWDT0_NMI	                525
#define RCAR_S4_INTCSYS_WWDT0_75P	                526
#define RCAR_S4_INTCSYS_WWDT1_NMI	                527
#define RCAR_S4_INTCSYS_WWDT1_75P	                528
#define RCAR_S4_INTCSYS_WWDT2_NMI	                529
#define RCAR_S4_INTCSYS_WWDT2_75P	                530
#define RCAR_S4_INTCSYS_WWDT3_NMI	                531
#define RCAR_S4_INTCSYS_WWDT3_75P	                532
#define RCAR_S4_INTCSYS_WWDT4_NMI	                533
#define RCAR_S4_INTCSYS_WWDT4_75P	                534
#define RCAR_S4_INTCSYS_WWDT5_NMI	                535
#define RCAR_S4_INTCSYS_WWDT5_75P	                536
#define RCAR_S4_INTCSYS_WWDT6_NMI	                537
#define RCAR_S4_INTCSYS_WWDT6_75P	                538
#define RCAR_S4_INTCSYS_WWDT7_NMI	                539
#define RCAR_S4_INTCSYS_WWDT7_75P	                540
#define RCAR_S4_INTCSYS_WWDT8_NMI	                541
#define RCAR_S4_INTCSYS_WWDT8_75P	                542
#define RCAR_S4_INTCSYS_WWDT9_NMI	                543
#define RCAR_S4_INTCSYS_WWDT9_75P	                544
#define RCAR_S4_INTCSYS_SCMT	                    545
#define RCAR_S4_INTCSYS_SUCMT	                    546
#define RCAR_S4_INTCSYS_RWDT	                    547
#define RCAR_S4_INTCSYS_SWDT	                    548
#define RCAR_S4_INTCSYS_MCU_TOP_0	                554
#define RCAR_S4_INTCSYS_MCU_TOP_1	                555
#define RCAR_S4_INTCSYS_MCU_TOP_2	                556
#define RCAR_S4_INTCSYS_MCU_TOP_3	                557
#define RCAR_S4_INTCSYS_MCU_TOP_4	                558
#define RCAR_S4_INTCSYS_MCU_TOP_5	                559
#define RCAR_S4_INTCSYS_MCU_TOP_6	                560
#define RCAR_S4_INTCSYS_MCU_TOP_7	                561
#define RCAR_S4_INTCSYS_MCU_TOP_8	                562
#define RCAR_S4_INTCSYS_MCU_TOP_9	                563
#define RCAR_S4_INTCSYS_MCU_TOP_10	                564
#define RCAR_S4_INTCSYS_MCU_TOP_11	                565
#define RCAR_S4_INTCSYS_MCU_TOP_12	                566
#define RCAR_S4_INTCSYS_MCU_TOP_13	                567
#define RCAR_S4_INTCSYS_MCU_TOP_14	                568
#define RCAR_S4_INTCSYS_MCU_TOP_15	                569
#define RCAR_S4_INTCSYS_MCU_TOP_16	                570
#define RCAR_S4_INTCSYS_MCU_TOP_17	                571
#define RCAR_S4_INTCSYS_MCU_TOP_18	                572
#define RCAR_S4_INTCSYS_MCU_TOP_19	                573
#define RCAR_S4_INTCSYS_MCU_TOP_20	                574
#define RCAR_S4_INTCSYS_MCU_TOP_21	                575
#define RCAR_S4_INTCSYS_MCU_TOP_22	                576
#define RCAR_S4_INTCSYS_MCU_TOP_23	                577
#define RCAR_S4_INTCSYS_MCU_TOP_24	                578
#define RCAR_S4_INTCSYS_MCU_TOP_25	                579
#define RCAR_S4_INTCSYS_MCU_TOP_26	                580
#define RCAR_S4_INTCSYS_MCU_TOP_27	                581
#define RCAR_S4_INTCSYS_MCU_TOP_28	                582
#define RCAR_S4_INTCSYS_MCU_TOP_29	                583
#define RCAR_S4_INTCSYS_MCU_TOP_30	                584
#define RCAR_S4_INTCSYS_MCU_TOP_31	                585
#define RCAR_S4_INTCSYS_MCU_TOP_32	                586
#define RCAR_S4_INTCSYS_MCU_TOP_33	                587
#define RCAR_S4_INTCSYS_MCU_TOP_34	                588
#define RCAR_S4_INTCSYS_MCU_TOP_35	                589
#define RCAR_S4_INTCSYS_MCU_TOP_36	                590
#define RCAR_S4_INTCSYS_MCU_TOP_37	                591
#define RCAR_S4_INTCSYS_MCU_TOP_38	                592
#define RCAR_S4_INTCSYS_MCU_TOP_39	                593
#define RCAR_S4_INTCSYS_MCU_TOP_40	                594
#define RCAR_S4_INTCSYS_MCU_TOP_41	                595
#define RCAR_S4_INTCSYS_MCU_TOP_42	                596
#define RCAR_S4_INTCSYS_MCU_TOP_43	                597
#define RCAR_S4_INTCSYS_MCU_TOP_44	                598
#define RCAR_S4_INTCSYS_MCU_TOP_45	                599
#define RCAR_S4_INTCSYS_MCU_TOP_46	                600
#define RCAR_S4_INTCSYS_MCU_TOP_47	                601
#define RCAR_S4_INTCSYS_MCU_TOP_48	                602
#define RCAR_S4_INTCSYS_MCU_TOP_49	                603
#define RCAR_S4_INTCSYS_MCU_TOP_50	                604
#define RCAR_S4_INTCSYS_MCU_TOP_51	                605
#define RCAR_S4_INTCSYS_MCU_TOP_52	                606
#define RCAR_S4_INTCSYS_MCU_TOP_53	                607
#define RCAR_S4_INTCSYS_MCU_TOP_54	                608
#define RCAR_S4_INTCSYS_MCU_TOP_55	                609
#define RCAR_S4_INTCSYS_MCU_TOP_56	                610
#define RCAR_S4_INTCSYS_MCU_TOP_57	                611
#define RCAR_S4_INTCSYS_MCU_TOP_58	                612
#define RCAR_S4_INTCSYS_MCU_TOP_59	                613
#define RCAR_S4_INTCSYS_MCU_TOP_60                  614
#define RCAR_S4_INTCSYS_MCU_TOP_61                  615
#define RCAR_S4_INTCSYS_MCU_TOP_62                  616
#define RCAR_S4_INTCSYS_MCU_TOP_63                  617
#define RCAR_S4_INTCSYS_MCU_TOP_64                  618
#define RCAR_S4_INTCSYS_MCU_TOP_65                  619
#define RCAR_S4_INTCSYS_MCU_TOP_66                  620
#define RCAR_S4_INTCSYS_MCU_TOP_67                  621
#define RCAR_S4_INTCSYS_MCU_TOP_68                  622
#define RCAR_S4_INTCSYS_MCU_TOP_69                  623
#define RCAR_S4_INTCSYS_MCU_TOP_70                  624
#define RCAR_S4_INTCSYS_MCU_TOP_71                  625
#define RCAR_S4_INTCSYS_MCU_TOP_72                  626
#define RCAR_S4_INTCSYS_MCU_TOP_73                  627
#define RCAR_S4_INTCSYS_MCU_TOP_74                  628
#define RCAR_S4_INTCSYS_MCU_TOP_75                  629
#define RCAR_S4_INTCSYS_MCU_TOP_76                  630
#define RCAR_S4_INTCSYS_MCU_TOP_77                  631
#define RCAR_S4_INTCSYS_MCU_TOP_78                  632
#define RCAR_S4_INTCSYS_MCU_TOP_79                  633
#define RCAR_S4_INTCSYS_FBC_END_0	                644
#define RCAR_S4_INTCSYS_FBC_END_1	                645
#define RCAR_S4_INTCSYS_FBC_END_2	                646
#define RCAR_S4_INTCSYS_FBC_END_3	                647
#define RCAR_S4_INTCSYS_FBC_END_4	                648
#define RCAR_S4_INTCSYS_FBC_END_5	                649
#define RCAR_S4_INTCSYS_FBC_END_6	                650
#define RCAR_S4_INTCSYS_FBC_END_7	                651
#define RCAR_S4_INTCSYS_FBC_END_8	                652
#define RCAR_S4_INTCSYS_FBC_END_9	                653
#define RCAR_S4_INTCSYS_FBC_END_10	                654
#define RCAR_S4_INTCSYS_FBC_END_11	                655
#define RCAR_S4_INTCSYS_FBC_END_12	                656
#define RCAR_S4_INTCSYS_FBC_END_13	                657
#define RCAR_S4_INTCSYS_FBC_END_14	                658
#define RCAR_S4_INTCSYS_FBC_END_15	                659
#define RCAR_S4_INTCSYS_FBC_END_16	                660
#define RCAR_S4_INTCSYS_FBC_END_17	                661
#define RCAR_S4_INTCSYS_FBC_END_18	                662
#define RCAR_S4_INTCSYS_FBC_END_19	                663
#define RCAR_S4_INTCSYS_FBC_END_20    	            664
#define RCAR_S4_INTCSYS_FBC_END_21    	            665
#define RCAR_S4_INTCSYS_FBC_END_22    	            666
#define RCAR_S4_INTCSYS_FBC_END_23    	            667
#define RCAR_S4_INTCSYS_FBC_END_24    	            668
#define RCAR_S4_INTCSYS_FBC_END_25    	            669
#define RCAR_S4_INTCSYS_FBC_END_26    	            670
#define RCAR_S4_INTCSYS_FBC_END_27                  671
#define RCAR_S4_INTCSYS_FBC_END_28                  672
#define RCAR_S4_INTCSYS_FBC_END_29                  673
#define RCAR_S4_INTCSYS_FBC_END_30                  674
#define RCAR_S4_INTCSYS_FBC_END_31                  675
#define RCAR_S4_INTCSYS_FBC_END_32                  676
#define RCAR_S4_INTCSYS_FBC_END_33                  677
#define RCAR_S4_INTCSYS_FBC_END_34                  678
#define RCAR_S4_INTCSYS_FBC_END_35                  679
#define RCAR_S4_INTCSYS_FBC_END_36                  680
#define RCAR_S4_INTCSYS_FBC_END_37                  681
#define RCAR_S4_INTCSYS_FBC_END_38                  682
#define RCAR_S4_INTCSYS_FBC_END_39                  683
#define RCAR_S4_INTCSYS_FBC_END_40                  684
#define RCAR_S4_INTCSYS_FBC_END_41                  685
#define RCAR_S4_INTCSYS_FBC_END_42                  686
#define RCAR_S4_INTCSYS_FBC_END_43                  687
#define RCAR_S4_INTCSYS_FBC_END_44                  688
#define RCAR_S4_INTCSYS_FBC_END_45                  689
#define RCAR_S4_INTCSYS_FBC_END_46                  690
#define RCAR_S4_INTCSYS_FBC_END_47                  691
#define RCAR_S4_INTCSYS_FBC_END_48                  692
#define RCAR_S4_INTCSYS_FBC_END_49                  693
#define RCAR_S4_INTCSYS_FBC_END_50                  694
#define RCAR_S4_INTCSYS_FBC_END_51                  695
#define RCAR_S4_INTCSYS_FBC_END_52                  696
#define RCAR_S4_INTCSYS_FBC_END_53                  697
#define RCAR_S4_INTCSYS_FBC_END_54                  698
#define RCAR_S4_INTCSYS_FBC_END_55                  699
#define RCAR_S4_INTCSYS_FBC_END_56                  700
#define RCAR_S4_INTCSYS_FBC_END_57                  701
#define RCAR_S4_INTCSYS_FBC_END_58                  702
#define RCAR_S4_INTCSYS_FBC_END_59                  703
#define RCAR_S4_INTCSYS_FBC_END_60                  704
#define RCAR_S4_INTCSYS_FBC_END_61                  705
#define RCAR_S4_INTCSYS_FBC_END_62                  706
#define RCAR_S4_INTCSYS_FBC_END_63                  707
#define RCAR_S4_INTCSYS_FBC_START_0	                708
#define RCAR_S4_INTCSYS_FBC_START_1	                709
#define RCAR_S4_INTCSYS_FBC_START_2	                710
#define RCAR_S4_INTCSYS_FBC_START_3	                711
#define RCAR_S4_INTCSYS_FBC_START_4	                712
#define RCAR_S4_INTCSYS_FBC_START_5	                713
#define RCAR_S4_INTCSYS_FBC_START_6	                714
#define RCAR_S4_INTCSYS_FBC_START_7	                715
#define RCAR_S4_INTCSYS_FBC_START_8	                716
#define RCAR_S4_INTCSYS_FBC_START_9	                717
#define RCAR_S4_INTCSYS_FBC_START_10	            718
#define RCAR_S4_INTCSYS_FBC_START_11	            719
#define RCAR_S4_INTCSYS_FBC_START_12	            720
#define RCAR_S4_INTCSYS_FBC_START_13	            721
#define RCAR_S4_INTCSYS_FBC_START_14	            722
#define RCAR_S4_INTCSYS_FBC_START_15	            723
#define RCAR_S4_INTCSYS_FBC_START_16	            724
#define RCAR_S4_INTCSYS_FBC_START_17	            725
#define RCAR_S4_INTCSYS_FBC_START_18	            726
#define RCAR_S4_INTCSYS_FBC_START_19	            727
#define RCAR_S4_INTCSYS_FBC_START_20                728
#define RCAR_S4_INTCSYS_FBC_START_21                729
#define RCAR_S4_INTCSYS_FBC_START_22                730
#define RCAR_S4_INTCSYS_FBC_START_23                731
#define RCAR_S4_INTCSYS_FBC_START_24                732
#define RCAR_S4_INTCSYS_FBC_START_25                733
#define RCAR_S4_INTCSYS_FBC_START_26                734
#define RCAR_S4_INTCSYS_FBC_START_27                735
#define RCAR_S4_INTCSYS_FBC_START_28                736
#define RCAR_S4_INTCSYS_FBC_START_29                737
#define RCAR_S4_INTCSYS_FBC_START_30                738
#define RCAR_S4_INTCSYS_FBC_START_31                739
#define RCAR_S4_INTCSYS_FBC_START_32                740
#define RCAR_S4_INTCSYS_FBC_START_33                741
#define RCAR_S4_INTCSYS_FBC_START_34                742
#define RCAR_S4_INTCSYS_FBC_START_35                743
#define RCAR_S4_INTCSYS_FBC_START_36                744
#define RCAR_S4_INTCSYS_FBC_START_37                745
#define RCAR_S4_INTCSYS_FBC_START_38                746
#define RCAR_S4_INTCSYS_FBC_START_39                747
#define RCAR_S4_INTCSYS_FBC_START_40                748
#define RCAR_S4_INTCSYS_FBC_START_41                749
#define RCAR_S4_INTCSYS_FBC_START_42                750
#define RCAR_S4_INTCSYS_FBC_START_43                751
#define RCAR_S4_INTCSYS_FBC_START_44                752
#define RCAR_S4_INTCSYS_FBC_START_45                753
#define RCAR_S4_INTCSYS_FBC_START_46                754
#define RCAR_S4_INTCSYS_FBC_START_47                755
#define RCAR_S4_INTCSYS_FBC_START_48                756
#define RCAR_S4_INTCSYS_FBC_START_49                757
#define RCAR_S4_INTCSYS_FBC_START_50                758
#define RCAR_S4_INTCSYS_FBC_START_51                759
#define RCAR_S4_INTCSYS_FBC_START_52                760
#define RCAR_S4_INTCSYS_FBC_START_53                761
#define RCAR_S4_INTCSYS_FBC_START_54                762
#define RCAR_S4_INTCSYS_FBC_START_55                763
#define RCAR_S4_INTCSYS_FBC_START_56                764
#define RCAR_S4_INTCSYS_FBC_START_57                765
#define RCAR_S4_INTCSYS_FBC_START_58                766
#define RCAR_S4_INTCSYS_FBC_START_59                767
#define RCAR_S4_INTCSYS_FBC_START_60                768
#define RCAR_S4_INTCSYS_FBC_START_61                769
#define RCAR_S4_INTCSYS_FBC_START_62                770
#define RCAR_S4_INTCSYS_FBC_START_63	            771
#define RCAR_S4_INTCSYS_FBC_START_OR	            772
#define RCAR_S4_INTCSYS_FBC_FINISH_OR	            773
#define RCAR_S4_INTCSYS_ADVFS_0	                    777
#define RCAR_S4_INTCSYS_ADVFS_1	                    778
#define RCAR_S4_INTCSYS_ECM_ERR0	                784
#define RCAR_S4_INTCSYS_ECM_ERR1	                785
#define RCAR_S4_INTCSYS_ECM_ERR2	                786
#define RCAR_S4_INTCSYS_ECM_ERR3	                787
#define RCAR_S4_INTCSYS_ECM_ERR4	                788
#define RCAR_S4_INTCSYS_ECM_ERR5	                789
#define RCAR_S4_INTCSYS_ECM_ERR6	                790
#define RCAR_S4_INTCSYS_ECM_ERR7	                791
#define RCAR_S4_INTCSYS_ECM_ERR8	                792
#define RCAR_S4_INTCSYS_ECM_ERR9	                793
#define RCAR_S4_INTCSYS_ECM_ERR10	                794
#define RCAR_S4_INTCSYS_ECM_ERR11	                795
#define RCAR_S4_INTCSYS_ECM_ERR12	                796
#define RCAR_S4_INTCSYS_ECM_ERR13	                797
#define RCAR_S4_INTCSYS_ECM_ERR14	                798
#define RCAR_S4_INTCSYS_ECM_ERR15	                799
#define RCAR_S4_INTCSYS_ECM_ERR16	                800
#define RCAR_S4_INTCSYS_ECM_ERR17	                801
#define RCAR_S4_INTCSYS_ECM_ERR18	                802
#define RCAR_S4_INTCSYS_ECM_ERR19	                803
#define RCAR_S4_INTCSYS_ECM_ERR20	                804
#define RCAR_S4_INTCSYS_ECM_ERR21	                805
#define RCAR_S4_INTCSYS_ECM_ERR22	                806
#define RCAR_S4_INTCSYS_ECM_ERR23	                807
#define RCAR_S4_INTCSYS_ECM_ERR24	                808
#define RCAR_S4_INTCSYS_ECM_ERR25	                809
#define RCAR_S4_INTCSYS_ECM_ERR26	                810
#define RCAR_S4_INTCSYS_ECM_ERR27	                811
#define RCAR_S4_INTCSYS_ECM_ERR28	                812
#define RCAR_S4_INTCSYS_ECM_ERR29	                813
#define RCAR_S4_INTCSYS_ECM_ERR30	                814
#define RCAR_S4_INTCSYS_ECM_ERR31	                815
#define RCAR_S4_INTCSYS_ECM_ERR32	                816
#define RCAR_S4_INTCSYS_ECM_ERR33	                817
#define RCAR_S4_INTCSYS_ECM_ERR34	                818
#define RCAR_S4_INTCSYS_ECM_ERR35	                819
#define RCAR_S4_INTCSYS_ECM_ERR36	                820
#define RCAR_S4_INTCSYS_ECM_ERR37	                821
#define RCAR_S4_INTCSYS_ECM_ERR38	                822
#define RCAR_S4_INTCSYS_ECM_ERR39	                823
#define RCAR_S4_INTCSYS_ECM_ERR40	                824
#define RCAR_S4_INTCSYS_ECM_ERR41	                825
#define RCAR_S4_INTCSYS_ECM_ERR42	                826
#define RCAR_S4_INTCSYS_ECM_ERR43	                827
#define RCAR_S4_INTCSYS_ECM_ERR44	                828
#define RCAR_S4_INTCSYS_ECM_ERR45	                829
#define RCAR_S4_INTCSYS_ECM_ERR46	                830
#define RCAR_S4_INTCSYS_ECM_ERR47	                831
#define RCAR_S4_INTCSYS_ECM_ERR48	                832
#define RCAR_S4_INTCSYS_ECM_ERR49	                833
#define RCAR_S4_INTCSYS_ECM_ERR50	                834
#define RCAR_S4_INTCSYS_ECM_ERR51	                835
#define RCAR_S4_INTCSYS_ECM_ERR52	                836
#define RCAR_S4_INTCSYS_ECM_ERR53	                837
#define RCAR_S4_INTCSYS_ECM_ERR54	                838
#define RCAR_S4_INTCSYS_ECM_ERR55	                839
#define RCAR_S4_INTCSYS_ECM_ERR56	                840
#define RCAR_S4_INTCSYS_ECM_ERR57	                841
#define RCAR_S4_INTCSYS_ECM_ERR58	                842
#define RCAR_S4_INTCSYS_ECM_ERR59	                843
#define RCAR_S4_INTCSYS_ECM_ERR60	                844
#define RCAR_S4_INTCSYS_ECM_ERR61	                845
#define RCAR_S4_INTCSYS_ECM_ERR62	                846
#define RCAR_S4_INTCSYS_ECM_ERR63	                847
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_0	            854
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_1	            855
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_2	            856
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_3	            857
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_4	            858
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_5	            859
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_6	            860
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_7	            861
#define RCAR_S4_INTCSYS_ECM_ECMPER0	                941
#define RCAR_S4_INTCSYS_ECM_ECMHSC	                942
#define RCAR_S4_INTCSYS_ECM_ECMMM	                943
#define RCAR_S4_INTCSYS_ECM_ECMRT	                944
#define RCAR_S4_INTCSYS_ECM_ECMTOP	                945
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_8	            948
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_9	            949
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_10	            950
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_11	            951
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_12	            952
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_13	            953
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_14	            954
#define RCAR_S4_INTCSYS_PFCSOC0_PORT_15	            955
#define RCAR_S4_INTCSYS_ECM_ECMRC	                956
#define RCAR_S4_INTCSYS_ECM_ECMRD	                957
#define RCAR_S4_INTCSYS_ECM_ECMRT0	                958
#define RCAR_S4_INTCSYS_ECM_ECMSTM	                959
#define RCAR_S4_INTCSYS_ECM_ECMSD0	                960
#define RCAR_S4_INTCSYS_ECM_ECMSP3	                961
#define RCAR_S4_INTCSYS_ECM_ECMSP4	                962
#define RCAR_S4_INTCSYS_ECM_ECMPS0	                963
#define RCAR_S4_INTCSYS_TSIPL_CH0_0	                964
#define RCAR_S4_INTCSYS_TSIPL_CH0_1	                965
#define RCAR_S4_INTCSYS_TSIPL_CH1_0	                966
#define RCAR_S4_INTCSYS_TSIPL_CH1_1	                967
#define RCAR_S4_INTCSYS_TSIPL_CH2_0	                968
#define RCAR_S4_INTCSYS_TSIPL_CH2_1	                969
#define RCAR_S4_INTCSYS_TSIPL_CH3_0	                970
#define RCAR_S4_INTCSYS_TSIPL_CH3_1	                971
#define RCAR_S4_INTCSYS_TSIPL_CH4_0	                972
#define RCAR_S4_INTCSYS_TSIPL_CH4_1	                973
#define RCAR_S4_INTCSYS_TSIPL_CH5_0	                974
#define RCAR_S4_INTCSYS_TSIPL_CH5_1	                975
#define RCAR_S4_INTCSYS_TSIPL_CH6_0	                976
#define RCAR_S4_INTCSYS_TSIPL_CH6_1	                977
#define RCAR_S4_INTCSYS_TSIPL_CH7_0	                978
#define RCAR_S4_INTCSYS_TSIPL_CH7_1	                979
#define RCAR_S4_INTCSYS_TSIP_CH0_0	                980
#define RCAR_S4_INTCSYS_TSIP_CH0_1	                981
#define RCAR_S4_INTCSYS_TSIP_CH1_0	                982
#define RCAR_S4_INTCSYS_TSIP_CH1_1	                983
#define RCAR_S4_INTCSYS_TSIP_CH2_0	                984
#define RCAR_S4_INTCSYS_TSIP_CH2_1	                985
#define RCAR_S4_INTCSYS_WCRC0	                    986
#define RCAR_S4_INTCSYS_WCRC1	                    987
#define RCAR_S4_INTCSYS_WCRC2	                    988
#define RCAR_S4_INTCSYS_WCRC3	                    989

/*
 * ECM base for V3U and S4
 */
#define RCAR_ECM_BASE              0xE6250000
#define RCAR_ECM_SIZE              0x1000

/*
 * ECM offset for V3U
 */
 #define RCAR_V3U_ECM_ERRCTLR0         0x200
 #define RCAR_V3U_ECM_ERRCTLR1         0x204
 #define RCAR_V3U_ECM_ERRCTLR2         0x208
 #define RCAR_V3U_ECM_ERRCTLR3         0x20C
 #define RCAR_V3U_ECM_ERRCTLR4         0x210
 #define RCAR_V3U_ECM_ERRCTLR5         0x214
 #define RCAR_V3U_ECM_ERRCTLR6         0x218
 #define RCAR_V3U_ECM_ERRCTLR7         0x260
 #define RCAR_V3U_ECM_ERRCTLR8         0x26C
 #define RCAR_V3U_ECM_ERRCTLR9         0x804
 #define RCAR_V3U_ECM_ERRCTLR10        0x808
 #define RCAR_V3U_ECM_ERRCTLR11        0x80C
 #define RCAR_V3U_ECM_ERRCTLR12        0x908
 #define RCAR_V3U_ECM_ERRCTLR13        0x918
 #define RCAR_V3U_ECM_ERRCTLR14        0x944
 #define RCAR_V3U_ECM_ERRCTLR15        0x954
 #define RCAR_V3U_ECM_ERRCTLR16        0x964
 #define RCAR_V3U_ECM_ERRCTLR17        0x974
 #define RCAR_V3U_ECM_ERRCTLR18        0x984
 #define RCAR_V3U_ECM_ERRCTLR19        0x994
 #define RCAR_V3U_ECM_ERRCTLR20        0x9A4
 #define RCAR_V3U_ECM_ERRCTLR21        0x9B4
 #define RCAR_V3U_ECM_ERRCTLR22        0x9C4
 #define RCAR_V3U_ECM_ERRCTLR23        0x9D4
 #define RCAR_V3U_ECM_ERRCTLR24        0x9E4
 #define RCAR_V3U_ECM_ERRCTLR25        0x9F4
 #define RCAR_V3U_ECM_ERRCTLR26        0xA04
 #define RCAR_V3U_ECM_ERRCTLR27        0xA14
 #define RCAR_V3U_ECM_ERRCTLR28        0xA24
 #define RCAR_V3U_ECM_ERRCTLR29        0xA34
 #define RCAR_V3U_ECM_ERRCTLR30        0xA44
 #define RCAR_V3U_ECM_ERRCTLR31        0xA54
 #define RCAR_V3U_ECM_ERRSTSR0         0x240
 #define RCAR_V3U_ECM_ERRSTSR1         0x244
 #define RCAR_V3U_ECM_ERRSTSR2         0x248
 #define RCAR_V3U_ECM_ERRSTSR3         0x24C
 #define RCAR_V3U_ECM_ERRSTSR4         0x250
 #define RCAR_V3U_ECM_ERRSTSR5         0x254
 #define RCAR_V3U_ECM_ERRSTSR6         0x258
 #define RCAR_V3U_ECM_ERRSTSR7         0x264
 #define RCAR_V3U_ECM_ERRSTSR8         0x270
 #define RCAR_V3U_ECM_ERRSTSR9         0x810
 #define RCAR_V3U_ECM_ERRSTSR10        0x814
 #define RCAR_V3U_ECM_ERRSTSR11        0x818
 #define RCAR_V3U_ECM_ERRSTSR12        0x90C
 #define RCAR_V3U_ECM_ERRSTSR13        0x91C
 #define RCAR_V3U_ECM_ERRSTSR14        0x948
 #define RCAR_V3U_ECM_ERRSTSR15        0x958
 #define RCAR_V3U_ECM_ERRSTSR16        0x968
 #define RCAR_V3U_ECM_ERRSTSR17        0x978
 #define RCAR_V3U_ECM_ERRSTSR18        0x988
 #define RCAR_V3U_ECM_ERRSTSR19        0x998
 #define RCAR_V3U_ECM_ERRSTSR20        0x9A8
 #define RCAR_V3U_ECM_ERRSTSR21        0x9B8
 #define RCAR_V3U_ECM_ERRSTSR22        0x9C8
 #define RCAR_V3U_ECM_ERRSTSR23        0x9D8
 #define RCAR_V3U_ECM_ERRSTSR24        0x9E8
 #define RCAR_V3U_ECM_ERRSTSR25        0x9F8
 #define RCAR_V3U_ECM_ERRSTSR26        0xA08
 #define RCAR_V3U_ECM_ERRSTSR27        0xA18
 #define RCAR_V3U_ECM_ERRSTSR28        0xA28
 #define RCAR_V3U_ECM_ERRSTSR29        0xA38
 #define RCAR_V3U_ECM_ERRSTSR30        0xA48
 #define RCAR_V3U_ECM_ERRSTSR31        0xA58
 #define RCAR_V3U_ECM_ERRTGTR0         0x280
 #define RCAR_V3U_ECM_ERRTGTR1         0x284
 #define RCAR_V3U_ECM_ERRTGTR2         0x288
 #define RCAR_V3U_ECM_ERRTGTR3         0x28C
 #define RCAR_V3U_ECM_ERRTGTR4         0x290
 #define RCAR_V3U_ECM_ERRTGTR5         0x294
 #define RCAR_V3U_ECM_ERRTGTR6         0x25C
 #define RCAR_V3U_ECM_ERRTGTR7         0x268
 #define RCAR_V3U_ECM_ERRTGTR8         0x274
 #define RCAR_V3U_ECM_ERRTGTR9         0x81C
 #define RCAR_V3U_ECM_ERRTGTR10        0x820
 #define RCAR_V3U_ECM_ERRTGTR11        0x824
 #define RCAR_V3U_ECM_ERRTGTR12        0x910
 #define RCAR_V3U_ECM_ERRTGTR13        0x920
 #define RCAR_V3U_ECM_ERRTGTR14        0x94C
 #define RCAR_V3U_ECM_ERRTGTR15        0x95C
 #define RCAR_V3U_ECM_ERRTGTR16        0x96C
 #define RCAR_V3U_ECM_ERRTGTR17        0x97C
 #define RCAR_V3U_ECM_ERRTGTR18        0x98C
 #define RCAR_V3U_ECM_ERRTGTR19        0x99C
 #define RCAR_V3U_ECM_ERRTGTR20        0x9AC
 #define RCAR_V3U_ECM_ERRTGTR21        0x9BC
 #define RCAR_V3U_ECM_ERRTGTR22        0x9CC
 #define RCAR_V3U_ECM_ERRTGTR23        0x9DC
 #define RCAR_V3U_ECM_ERRTGTR24        0x9EC
 #define RCAR_V3U_ECM_ERRTGTR25        0xAFC
 #define RCAR_V3U_ECM_ERRTGTR26        0xA0C
 #define RCAR_V3U_ECM_ERRTGTR27        0xA1C
 #define RCAR_V3U_ECM_ERRTGTR28        0xA2C
 #define RCAR_V3U_ECM_ERRTGTR29        0xA3C
 #define RCAR_V3U_ECM_ERRTGTR30        0xA4C
 #define RCAR_V3U_ECM_ERRTGTR31        0xA5C
 #define RCAR_V3U_ECM_SECCTLR          0x5F0
 #define RCAR_V3U_ECM_SECSTSRR         0x5F4
 #define RCAR_V3U_ECM_SECSADR          0x5FC
 #define RCAR_V3U_ECM_ERRCNTR0         0x834
 #define RCAR_V3U_ECM_ERRCNTR1         0x838
 #define RCAR_V3U_ECM_ERRCNTR2         0x83C
 #define RCAR_V3U_ECM_ERRCNTR3         0x840
 #define RCAR_V3U_ECM_ERRCNTR4         0x844
 #define RCAR_V3U_ECM_ERRCNTR5         0x848
 #define RCAR_V3U_ECM_ERRCNTR6         0x84C
 #define RCAR_V3U_ECM_ERRCNTR7         0x850
 #define RCAR_V3U_ECM_ERRCNTR8         0x854
 #define RCAR_V3U_ECM_ERRCNTR9         0x858
 #define RCAR_V3U_ECM_ERRCNTR10         0x85C
 #define RCAR_V3U_ECM_ERRCNTR11         0x860
 #define RCAR_V3U_ECM_ERRCNTR12         0x864
 #define RCAR_V3U_ECM_ERRCNTR13         0x868
 #define RCAR_V3U_ECM_ERRCNTR14         0x86C
 #define RCAR_V3U_ECM_ERRCNTR15         0x870
 #define RCAR_V3U_ECM_ERRCNTR16         0x874
 #define RCAR_V3U_ECM_ERRCNTR17         0x878
 #define RCAR_V3U_ECM_ERRCNTR18         0x87C
 #define RCAR_V3U_ECM_ERRCNTR19         0x880
 #define RCAR_V3U_ECM_ERRCNTR20         0x884
 #define RCAR_V3U_ECM_ERRCNTR21         0x888
 #define RCAR_V3U_ECM_ERRCNTR22         0x88C
 #define RCAR_V3U_ECM_ERRCNTR23         0x890
 #define RCAR_V3U_ECM_ERRCNTR24         0x894
 #define RCAR_V3U_ECM_ERRCNTR25         0x898
 #define RCAR_V3U_ECM_ERRCNTR26         0x918
 #define RCAR_V3U_ECM_ERRCNTR27         0x91C
 #define RCAR_V3U_ECM_ERRCNTR28         0x920
 #define RCAR_V3U_ECM_ERRCNTR29         0x924
 #define RCAR_V3U_ECM_ERRCNTR30         0x928
 #define RCAR_V3U_ECM_ERRCNTR31         0x92C
 #define RCAR_V3U_ECM_ERRCNTR32         0x930
 #define RCAR_V3U_ECM_ERRCNTR33         0x934
 #define RCAR_V3U_ECM_ERRCNTR34         0x938
 #define RCAR_V3U_ECM_ERRCNTR35         0x93C
 #define RCAR_V3U_SAFGPRR               0x300
 #define RCAR_V3U_SAFSTERRENR           0x304
 #define RCAR_V3U_SAFCLERRENR           0x308
 #define RCAR_V3U_SAFSTSR               0x30C
 #define RCAR_V3U_SAFCTLR               0x310
 #define RCAR_V3U_ECM_GEIIDR            0x89C
 #define RCAR_V3U_ECM_EXTRQHLDCNTR      0x8A0
 #define RCAR_V3U_ECM_EXTRQMSKCNTR      0x8A4
 #define RCAR_V3U_ECM_EXTRQSTSR         0x8A8
 #define RCAR_V3U_ECM_EXTTMHLDCNTR      0x8AC
 #define RCAR_V3U_ECM_EXTTMMSKCNTR      0x8B0
 #define RCAR_V3U_ECM_EXTSEQMONR        0x8B4
 #define RCAR_V3U_ECM_WPCNTR            0x900
 #define RCAR_V3U_ECM_WACNTR            0x904
 #define RCAR_V3U_ECM_PSSTATCTLRM       0x0BC
 #define RCAR_V3U_ECM_PSSTATCTLRA0      0x0C0
 #define RCAR_V3U_ECM_PSSTATCTLRA1      0x0C4
 #define RCAR_V3U_ECM_PSSTATCTLRA2      0x0C8
 #define RCAR_V3U_ECM_PSSTATCTLRA3      0x0CC
 #define RCAR_V3U_ECM_PSSTATCTLRA4      0x0D0
 #define RCAR_V3U_ECM_PSSTATCTLRA5      0x0D4
 #define RCAR_V3U_ECM_PSSTATCTLRA6      0x0D8
 #define RCAR_V3U_ECM_PSSTATCTLRA7      0x0DC
 #define RCAR_V3U_ECM_PSSTATCTLRA8      0x724
 #define RCAR_V3U_ECM_PSSTATCTLRA9      0x728
 #define RCAR_V3U_ECM_PSSTATCTLRA10     0x72C
 #define RCAR_V3U_ECM_PSSTATCTLRA11     0x730
 #define RCAR_V3U_ECM_PSSTATCTLRA12     0x734
 #define RCAR_V3U_ECM_PSSTATCTLRA13     0x738
 #define RCAR_V3U_ECM_PSSTATCTLRA14     0x73C
 #define RCAR_V3U_ECM_PSSTATCTLRA15     0x740
 #define RCAR_V3U_ECM_PSSTATCTLRA16     0x744
 #define RCAR_V3U_ECM_PSSTATCTLRA17     0x748
 #define RCAR_V3U_ECM_PSSTATCTLRA18     0x74C
 #define RCAR_V3U_ECM_PSSTATCTLRA19     0x750
 #define RCAR_V3U_ECM_PSSTATCTLRA20     0x754
 #define RCAR_V3U_ECM_PSSTATCTLRA21     0x758
 #define RCAR_V3U_ECM_PSSTATCTLRA22     0x75C
 #define RCAR_V3U_ECM_PSSTATCTLRA23     0x760
 #define RCAR_V3U_ECM_PSSTATCTLRA24     0x764
 #define RCAR_V3U_ECM_PSSTATCTLRA25     0x768
 #define RCAR_V3U_ECM_PSSTATCTLRA26     0x76C
 #define RCAR_V3U_ECM_PSSTATCTLRA27     0x770
 #define RCAR_V3U_ECM_PSSTATCTLRA28     0x774
 #define RCAR_V3U_ECM_PSSTATCTLRA29     0x778
 #define RCAR_V3U_ECM_PSSTATCTLRA30     0x77C
 #define RCAR_V3U_ECM_PSSTATCTLRA31     0x780
 #define RCAR_V3U_ECM_PSSTATCTLRB0      0x784
 #define RCAR_V3U_ECM_PSSTATCTLRB1      0x788
 #define RCAR_V3U_ECM_PSSTATCTLRB2      0x78C
 #define RCAR_V3U_ECM_PSSTATCTLRB3      0x790
 #define RCAR_V3U_ECM_PSSTATCTLRB4      0x794
 #define RCAR_V3U_ECM_PSSTATCTLRB5      0x798
 #define RCAR_V3U_ECM_PSSTATCTLRB6      0x79C
 #define RCAR_V3U_ECM_PSSTATCTLRB7      0x7A0
 #define RCAR_V3U_ECM_PSSTATCTLRB8      0x7A4
 #define RCAR_V3U_ECM_PSSTATCTLRB9      0x7A8
 #define RCAR_V3U_ECM_PSSTATCTLRB10     0x7AC
 #define RCAR_V3U_ECM_PSSTATCTLRB11     0x7B0
 #define RCAR_V3U_ECM_PSSTATCTLRB12     0x7B4
 #define RCAR_V3U_ECM_PSSTATCTLRB13     0x7B8
 #define RCAR_V3U_ECM_PSSTATCTLRB14     0x7BC
 #define RCAR_V3U_ECM_PSSTATCTLRB15     0x7C0
 #define RCAR_V3U_ECM_PSSTATCTLRB16     0x7C4
 #define RCAR_V3U_ECM_PSSTATCTLRB17     0x7C8
 #define RCAR_V3U_ECM_PSSTATCTLRB18     0x7CC
 #define RCAR_V3U_ECM_PSSTATCTLRB19     0x7D0
 #define RCAR_V3U_ECM_PSSTATCTLRB20     0x7D4
 #define RCAR_V3U_ECM_PSSTATCTLRB21     0x7D8
 #define RCAR_V3U_ECM_PSSTATCTLRB22     0x7DC
 #define RCAR_V3U_ECM_PSSTATCTLRB23     0x7E0
 #define RCAR_V3U_ECM_PSSTATCTLRB24     0x7E4
 #define RCAR_V3U_ECM_PSSTATCTLRB25     0x7E8
 #define RCAR_V3U_ECM_PSSTATCTLRB26     0x7EC
 #define RCAR_V3U_ECM_PSSTATCTLRB27     0x7F0
 #define RCAR_V3U_ECM_PSSTATCTLRB28     0x7F4
 #define RCAR_V3U_ECM_PSSTATCTLRB29     0x7F8
 #define RCAR_V3U_ECM_PSSTATCTLRB30     0x7FC
 #define RCAR_V3U_ECM_PSSTATCTLRB31     0x800

/*
 * ECM offset for S4
 */
 #define RCAR_S4_ECM_ERRCTLR(n)         (0x0000 + (n * 4))
 #define RCAR_S4_ECM_ERRSTSR(n)         (0x0100 + (n * 4))
 #define RCAR_S4_ECM_ERRTGTR(n)         (0x0200 + (n * 4))
 #define RCAR_S4_ECM_ERRCNTR(n)         (0x0400 + (n * 4))
 #define RCAR_S4_ECM_ERRFATALR(n)       (0x0600 + (n * 4))
 #define RCAR_S4_ECM_PSSTATCTLRA(n)     (0x0700 + (n * 4))
 #define RCAR_S4_ECM_PSSTATCTLRB(n)     (0x0800 + (n * 4))
 #define RCAR_S4_ECM_PSSTATCTLRM         0x0900
 #define RCAR_S4_ECM_GEIIDR              0x0928
 #define RCAR_S4_ECM_SAFSTSR             0x0940
 #define RCAR_S4_ECM_SAFSTERRENR         0x0944
 #define RCAR_S4_ECM_SAFCLERRENR         0x0948
 #define RCAR_S4_ECM_EDCERRSIDPADDR      0x095C
 #define RCAR_S4_ECM_EDCERRSIDPWDATAR    0x0960
 #define RCAR_S4_ECM_DCLSERMON00R        0x0984
 #define RCAR_S4_ECM_DCLSERMON01R        0x0988
 #define RCAR_S4_ECM_WPCNTR              0x0A00
 #define RCAR_S4_ECM_WACNTR              0x0A04

/*--------------------------------------------------
 * 
 * The following is for Gen3 and Gen4 (All R-Car SoC)
 * 
 *--------------------------------------------------*/

/*
 * CMT 
 */ 
#define RCAR_CMT0_BASE              0xE60F0000
#define RCAR_CMT1_BASE              0xE6130000
#define RCAR_CMT2_BASE              0xE6140000
#define RCAR_CMT3_BASE              0xE6148000

#define RCAR_CMT0_REG_OFFSET        0x500
#define RCAR_CMT1_REG_OFFSET        0x000
#define RCAR_CMT_REG_SIZE           0x100
#define RCAR_CMT_SIZE               (0x1000 + RCAR_CMT_REG_SIZE)

 #define RCAR_CMT_CMCLKE            0x1000
 #define RCAR_CMT_CMSTR             0x0000
 #define RCAR_CMT_CMCSR             0x0010
 #define RCAR_CMT_CMCNT             0x0014
 #define RCAR_CMT_CMCOR             0x0018
 #define RCAR_CMT_CMCSRH            0x0020
 #define RCAR_CMT_CMCNTH            0x0024
 #define RCAR_CMT_CMCORH            0x0028
 #define RCAR_CMT_CMCSRM            0x0040
 #define RCAR_CMT_CMCNTM            0x0044

/*
 * PWM
 */
#define RCAR_PWM0_BASE              0xE6E30000
#define RCAR_PWM1_BASE              0xE6E31000
#define RCAR_PWM2_BASE              0xE6E32000
#define RCAR_PWM3_BASE              0xE6E33000
#define RCAR_PWM4_BASE              0xE6E34000
#define RCAR_PWM5_BASE              0xE6E35000
#define RCAR_PWM6_BASE              0xE6E36000
#define RCAR_PWM_SIZE               0x1000

#define RCAR_PWM_PWMCR              0x0000
#define RCAR_PWM_PWMCNT             0x0004

/* V3H only */ 
#define RCAR_PWM_PWMEI              0x000C

/*
 * Timer Unit (TMU)
 */
#define RCAR_TMU_0_2_BASE          0xE61E0000
#define RCAR_TMU_3_5_BASE          0xE6FC0000
#define RCAR_TMU_6_8_BASE          0xE6FD0000
#define RCAR_TMU_9_11_BASE         0xE6FE0000
#define RCAR_TMU_12_14_BASE        0xFFC00000
#define RCAR_TMU_SIZE              0x30

#define RCAR_TMU_TSTR              0x04
    #define RCAR_TMU_START0        (1 << 0) // for channel 0, 3, 6, 9, 12
    #define RCAR_TMU_START1        (1 << 1) // for channel 1, 4, 7, 10, 13
    #define RCAR_TMU_START2        (1 << 2) // for channel 2, 5, 8, 11, 14

#define RCAR_TMU_TCOR0             0x08 // for channel 0, 3, 6, 9, 12
#define RCAR_TMU_TCOR1             0x14 // for channel 1, 4, 7, 10, 13
#define RCAR_TMU_TCOR2             0x20 // for channel 2, 5, 8, 11, 14
#define RCAR_TMU_TCNT0             0x0C // for channel 0, 3, 6, 9, 12
#define RCAR_TMU_TCNT1             0x18 // for channel 1, 4, 7, 10, 13
#define RCAR_TMU_TCNT2             0x24 // for channel 2, 5, 8, 11, 14
#define RCAR_TMU_TCR0              0x10 // for channel 0, 3, 6, 9, 12
#define RCAR_TMU_TCR1              0x1C // for channel 1, 4, 7, 10, 13
#define RCAR_TMU_TCR2              0x28 // for channel 2, 5, 8, 11, 14
#define RCAR_TMU_TCPR2             0x2C // for channel 5, 8, 11, 14
 #define RCAR_TMU_UNIE             (1 << 5)
 #define RCAR_TMU_UNF              (1 << 8)

/*
 * CAN
 */
#define RCAR_CAN0_BASE              0xE6C30000
#define RCAR_CAN1_BASE              0xE6C38000
#define RCAR_CAN_SIZE               0x600
#define RCAR_CAN_REG_OFFSET         0x400

/*
 * CANFD
 */
#define RCAR_CANFD_BASE             0xE66C0000
#define RCAR_CANFD_SIZE             0x10000

 /*
 * CANFD for V3U
 */
 #define RCAR_GEN4_CANFD_BASE             0xE6660000

/*
 * Watchdog Timer
 */
#define RCAR_RWDT_BASE              0xE6020000  // RCLK Watchdog Timer
#define RCAR_SWDT_BASE              0xE6030000  // Secure Watchdog Timer
 #define RCAR_WDT_CNT               0x00        // Count Register
 #define RCAR_WDT_CSRA              0x04        // Control/Status Register A
 #define RCAR_WDT_CSRB              0x08        // Control/Status Register B
 #define RCAR_WDT_SIZE              0x0C

/*
 * ARM Generic Timer
 */
#define RCAR_ARM_CNTR_BASE          0xE6080000  // ARM Generic Timer
 #define RCAR_ARM_CNTR_CNTCR        0x00
  #define RCAR_ARM_CNTR_CNTCR_EN    (1 << 0)
 #define RCAR_ARM_CNTR_CNTFID       0x20

/*
 * Audio modules
 */


/*
 * Audio Clock Generator
 */
#define RCAR_ADG_BASE               0xEC5A0000
#define RCAR_ADG_SIZE               0x200

/* Offset from base */
#define RCAR_ADG_BRRA              0x00
#define RCAR_ADG_BRRB              0x04
#define RCAR_ADG_BRGCKR            0x08
#define RCAR_ADG_SSICKR            0x08
#define RCAR_ADG_CKSEL0            0x0C
#define RCAR_ADG_CKSEL1            0x10
#define RCAR_ADG_CKSEL2            0x14
#define RCAR_ADG_TIM_EN            0x30
#define RCAR_ADG_SRCIN_TIMSEL0     0x34
#define RCAR_ADG_SRCIN_TIMSEL1     0x38
#define RCAR_ADG_SRCIN_TIMSEL2     0x3C
#define RCAR_ADG_SRCIN_TIMSEL3     0x40
#define RCAR_ADG_SRCIN_TIMSEL4     0x44
#define RCAR_ADG_SRCOUT_TIMSEL0    0x48
#define RCAR_ADG_SRCOUT_TIMSEL1    0x4C
#define RCAR_ADG_SRCOUT_TIMSEL2    0x50
#define RCAR_ADG_SRCOUT_TIMSEL3    0x54
#define RCAR_ADG_SRCOUT_TIMSEL4    0x58
#define RCAR_ADG_CMDOUT_TIMSEL     0x5C
#define RCAR_ADG_DTCP_TIMSEL       0x64
#define RCAR_ADG_ADSP_TIMSEL1      0x80
#define RCAR_ADG_ADSP_TIMSEL2      0x84
#define RCAR_ADG_ADSP_TIMSEL3      0x88
#define RCAR_ADG_SSICKR1           0x100
#define RCAR_ADG_AVBCKR            0x100
#define RCAR_ADG_AVB_SYNC_SEL0     0x104
#define RCAR_ADG_AVB_SYNC_SEL1     0x10C
#define RCAR_ADG_AVB_SYNC_SEL2     0x110
#define RCAR_ADG_AVB_SYNC_DIV0     0x114
#define RCAR_ADG_AVB_SYNC_DIV1     0x118
#define RCAR_ADG_AVB_CLK_DIV0      0x11C
#define RCAR_ADG_AVB_CLK_DIV1      0x120
#define RCAR_ADG_AVB_CLK_DIV2      0x124
#define RCAR_ADG_AVB_CLK_DIV3      0x128
#define RCAR_ADG_AVB_CLK_DIV4      0x12C
#define RCAR_ADG_AVB_CLK_DIV5      0x130
#define RCAR_ADG_AVB_CLK_DIV6      0x134
#define RCAR_ADG_AVB_CLK_DIV7      0x138
#define RCAR_ADG_AVB_CLK_CONFIG    0x13C
#define RCAR_ADG_AVB_CLK_STATUS    0x140

/* SCU */
#define RCAR_SCU_BASE               0xEC500000
#define RCAR_SCU_SIZE               0x1000

/* SSIU */
#define RCAR_SSIU_BASE             0xEC540000
#define RCAR_SSIU_SIZE             0x1000
 #define RCAR_SSIU_MODE0            0x800
 #define RCAR_SSIU_MODE1            0x804
 #define RCAR_SSIU_MODE2            0x808
 #define RCAR_SSIU_MODE3            0x80C
 #define RCAR_SSIU_CONTROL          0x810

/* SSI */
#define RCAR_SSI_BASE              0xEC541000
#define RCAR_SSI_SIZE              0x280       // 10 Modules * 0x40 each
 #define RCAR_SSI_CR                0x00        // Control Register
 #define RCAR_SSI_SR                0x04        // Status Register
 #define RCAR_SSI_TDR               0x08        // Transmit Data Register
 #define RCAR_SSI_RDR               0x0C        // Receive Data Register
 #define RCAR_SSI_WSR               0x20        // WS Mode Register
 #define RCAR_SSI_FMR               0x24        // FS Mode Register
 #define RCAR_SSI_FSR               0x28        // FS Status Register

/* AUDIO-DMAC */
 #define RCAR_AUDIODMAC0_BASE       0xEC700000
 #define RCAR_AUDIODMAC1_BASE       0xEC720000

/* AUDIO-DMAC-PP */
#define RCAR_AUDIODMACPP_BASE       0xEC740000
#define RCAR_AUDIODMACPP_EXT_BASE   0xEC760000
#define RCAR_AUDIODMACPP_SIZE       0x1000
 #define RCAR_PDMASAR(x)            (0x20 + 0x10 * (x))
 #define RCAR_PDMADAR(x)            (0x24 + 0x10 * (x))
 #define RCAR_PDMACHCR(x)           (0x2C + 0x10 * (x))

/*
 * Interrupt Controller (INTC-EX)
 */
#define RCAR_IRQC0_BASE             0xE61C0000
#define RCAR_NMI_BASE               0xE61C0400
 #define RCAR_INTREQ_STS0           0x000       /* R */
 #define RCAR_INTEN_STS0            0x004       /* R/WC1 */
 #define RCAR_INTEN_SET0            0x008       /* W */
 #define RCAR_INTREQ_STS1           0x010       /* R */
 #define RCAR_INTEN_STS1            0x014       /* R/WC1 */
 #define RCAR_INTEN_SET1            0x018       /* W */
 #define RCAR_DETECT_STATUS         0x100       /* R/WC1 */
 #define RCAR_CONFIG_00             0x180       /* R/W */

/*
 * SDHI
 */
#define RCAR_SDHI0_BASE             0xEE100000  // SDHI0 Base
#define RCAR_SDHI1_BASE             0xEE120000  // SDHI1 Base
#define RCAR_SDHI2_BASE             0xEE140000  // SDHI2 Base
#define RCAR_SDHI3_BASE             0xEE160000  // SDHI3 Base
#define RCAR_SDHI_SIZE              0x2000

/*
 * MMCIF
 */
#define RCAR_MMCIF0_BASE            0xEE140000
#define RCAR_MMCIF1_BASE            0xEE160000
#define RCAR_MMCIF_SIZE             0x2000

/*
 * SYS-DMAC for Gen3 
 */
#define RCAR_GEN3_SYSDMAC0_BASE          0xE6700000
#define RCAR_GEN3_SYSDMAC1_BASE          0xE7300000
#define RCAR_GEN3_SYSDMAC2_BASE          0xE7310000
#define RCAR_GEN3_SYSDMAC_SIZE           0x3000      // BASE+REGS to DESC

/* 
 * RT-DMAC for Gen3
 */
#define RCAR_GEN3_RTDMAC0_BASE           0xFFC10000
#define RCAR_GEN3_RTDMAC1_BASE           0xFFC20000
#define RCAR_GEN3_RTDMAC2_BASE           0xFFC30000
#define RCAR_GEN3_RTDMAC3_BASE           0xFFC40000 

/* SYS-DMAC register, offset from BASE + REGS */
#define RCAR_GEN3_SYSDMAC_REGS          0x8000
#define RCAR_GEN3_SYSDMAC_REGSIZE       0x80
#define RCAR_GEN3_SYSDMAC_DESC          (0xA000 - RCAR_GEN3_SYSDMAC_REGS)

/*
 * SYS-DMAC for Gen4 (V3U, S4)
 */
#define RCAR_GEN4_SYSDMAC1_COM_BASE       0xE7350000
#define RCAR_GEN4_SYSDMAC2_COM_BASE       0xE7351000
#define RCAR_GEN4_SYSDMAC_COM_SIZE        0x1000

/* SYSDMAC register, offset from BASE to REGS */
#define RCAR_GEN4_SYSDMAC_DESC           0x0800
#define RCAR_GEN4_SYSDMAC_REGS           0x1000
#define RCAR_GEN4_SYSDMAC_CHREG_SIZE     0x200
#define RCAR_GEN4_SYSDMAC_SIZE           0xF200 // Size of 16 channels

/*
 * RT-DMAC for Gen4 (V3U, S4)
 */
#define RCAR_GEN4_RTDMAC0_BASE            0xFFC10000
#define RCAR_GEN4_RTDMAC1_BASE            0xFFC20000
#define RCAR_GEN4_RTDMAC2_BASE            0xFFD70000
#define RCAR_GEN4_RTDMAC3_BASE            0xFFD80000
 
#define RCAR_GEN4_RTDMAC0_COM_BASE        0xFFD60000
#define RCAR_GEN4_RTDMAC1_COM_BASE        0xFFD61000
#define RCAR_GEN4_RTDMAC2_COM_BASE        0xFFD62000
#define RCAR_GEN4_RTDMAC3_COM_BASE        0xFFD63000
#define RCAR_GEN4_RTDMAC_COM_SIZE         0x1000

/*
 * Offset for SYS-DMAC and RT-DMAC
 */

/* SYSDMAC registers, offset from BASE + SYSDMAC_REG */
#define RCAR_DMAC_DMASAR        0x000
#define RCAR_DMAC_DMADAR        0x004
#define RCAR_DMAC_DMATCR        0x008
#define RCAR_DMAC_DMACHCR       0x00C
#define RCAR_DMAC_DMAFIXSAR     0x010
#define RCAR_DMAC_DMAFIXDAR     0x014
#define RCAR_DMAC_DMATCRB       0x018
#define RCAR_DMAC_DMACHCRB      0x01C
#define RCAR_DMAC_DMASART       0x020
#define RCAR_DMAC_DMADART       0x024
#define RCAR_DMAC_DMATSR        0x028
#define RCAR_DMAC_DMACHCRT      0x02C
#define RCAR_DMAC_DMATSRB       0x038
#define RCAR_DMAC_DMARS         0x040
#define RCAR_DMAC_DMABUFCR      0x048
#define RCAR_DMAC_DMADPBASE     0x050
#define RCAR_DMAC_DMADPCR       0x054
#define RCAR_DMAC_DMAFIXDPBASE  0x060
#define RCAR_DMAC_DMAREGIONID   0x078
#define RCAR_DMAC_DMACHID       0x080
#define RCAR_DMAC_DMADMSEC      0x0B0
#define RCAR_DMAC_CHCLR         0x100

/* SYSDMAC global registers, offset from BASE */
#define RCAR_DMAC_DMAISTA       0x0020      // DMA interrupt status register
#define RCAR_DMAC_DMASEC        0x0030      // DMA secure control register
#define RCAR_DMAC_DMAOR         0x0060      // DMA operation register
#define RCAR_DMAC_DMACHCLR      0x0080      // DMA channel clear register
#define RCAR_DMAC_DMADPSEC      0x00A0      // DPRAM secure control register
#define RCAR_DMAC_ADRFB         0x00CC
#define RCAR_DMAC_APBEDC        0x00D0
#define RCAR_DMAC_APB_CH_PADDR          0x00D4
#define RCAR_DMAC_APB_CH_PWDATA         0x00D8
#define RCAR_DMAC_APB_PUBLIC_PADDR      0x00DC
#define RCAR_DMAC_APB_PUBLIC_PWDATA     0x00E0
#define RCAR_DMAC_DMCMP_STAUS   0x00F0
#define RCAR_DMAC_RATE_RD       0x00F4
#define RCAR_DMAC_RATE_WR       0x00F8
#define RCAR_DMAC_DPRAM_EDC     0x00FC
#define RCAR_DMAC_DPSAR         0x0800
#define RCAR_DMAC_DPDAR         0x0804
#define RCAR_DMAC_DPTCR         0x0808
#define RCAR_DMAC_DPCHCR        0x080C

/*
 * DMA request ID
 */
#define RCAR_DREQ_IIC_DVFS_RXI      0x10
#define RCAR_DREQ_IIC_DVFS_TXI      0x11
#define RCAR_DREQ_SIM_RXI           0x14
#define RCAR_DREQ_SIM_TXI           0x15
#define RCAR_DREQ_TPU0_TXI          0x17
#define RCAR_DREQ_TSIF0_RXI         0x18
#define RCAR_DREQ_TSIF1_RXI         0x1A
#define RCAR_DREQ_HSCIF0_RXI        0x30
#define RCAR_DREQ_HSCIF0_TXI        0x31
#define RCAR_DREQ_HSCIF1_RXI        0x32
#define RCAR_DREQ_HSCIF1_TXI        0x33
#define RCAR_DREQ_HSCIF2_RXI        0x34
#define RCAR_DREQ_HSCIF2_TXI        0x35
#define RCAR_DREQ_HSCIF3_RXI        0x36
#define RCAR_DREQ_HSCIF3_TXI        0x37
#define RCAR_DREQ_HSCIF4_RXI        0x38
#define RCAR_DREQ_HSCIF4_TXI        0x39
#define RCAR_DREQ_MSIOF0_RXI        0x40
#define RCAR_DREQ_MSIOF0_TXI        0x41
#define RCAR_DREQ_MSIOF1_RXI        0x42
#define RCAR_DREQ_MSIOF1_TXI        0x43
#define RCAR_DREQ_MSIOF2_RXI        0x44
#define RCAR_DREQ_MSIOF2_TXI        0x45
#define RCAR_DREQ_MSIOF3_RXI        0x46
#define RCAR_DREQ_MSIOF3_TXI        0x47
#define RCAR_DREQ_MSIOF4_RXI        0x48
#define RCAR_DREQ_MSIOF4_TXI        0x49
#define RCAR_DREQ_MSIOF5_RXI        0x4A
#define RCAR_DREQ_MSIOF5_TXI        0x4B
#define RCAR_DREQ_SCIF0_RXI         0x50
#define RCAR_DREQ_SCIF0_TXI         0x51
#define RCAR_DREQ_SCIF1_RXI         0x52
#define RCAR_DREQ_SCIF1_TXI         0x53
#define RCAR_DREQ_SCIF2_RXI         0x12
#define RCAR_DREQ_SCIF2_TXI         0x13
#define RCAR_DREQ_SCIF3_RXI         0x56
#define RCAR_DREQ_SCIF3_TXI         0x57
#define RCAR_DREQ_SCIF4_RXI         0x58
#define RCAR_DREQ_SCIF4_TXI         0x59
#define RCAR_DREQ_I2C0_RXI          0x90
#define RCAR_DREQ_I2C0_TXI          0x91
#define RCAR_DREQ_I2C1_RXI          0x92
#define RCAR_DREQ_I2C1_TXI          0x93
#define RCAR_DREQ_I2C2_RXI          0x94
#define RCAR_DREQ_I2C2_TXI          0x95
#define RCAR_DREQ_I2C3_RXI          0x96
#define RCAR_DREQ_I2C3_TXI          0x97
#define RCAR_DREQ_I2C4_RXI          0x98
#define RCAR_DREQ_I2C4_TXI          0x99
#define RCAR_DREQ_I2C5_RXI          0x9A
#define RCAR_DREQ_I2C5_TXI          0x9B
#define RCAR_DREQ_I2C6_RXI          0x9C
#define RCAR_DREQ_I2C6_TXI          0x9D

#define RCAR_DREQ_SCIF_TXI(n)       (n == 0 ? RCAR_DREQ_SCIF0_TXI: \
                                    (n == 1 ? RCAR_DREQ_SCIF1_TXI: \
                                    (n == 2 ? RCAR_DREQ_SCIF2_TXI: \
                                    (n == 3 ? RCAR_DREQ_SCIF3_TXI: RCAR_DREQ_SCIF4_TXI))))
#define RCAR_DREQ_SCIF_RXI(n)       (n == 0 ? RCAR_DREQ_SCIF0_RXI: \
                                    (n == 1 ? RCAR_DREQ_SCIF1_RXI: \
                                    (n == 2 ? RCAR_DREQ_SCIF2_RXI: \
                                    (n == 3 ? RCAR_DREQ_SCIF3_RXI: RCAR_DREQ_SCIF4_RXI))))
#define RCAR_DREQ_HSCIF_TXI(n)      (n == 0 ? RCAR_DREQ_HSCIF0_TXI: \
                                    (n == 1 ? RCAR_DREQ_HSCIF1_TXI: \
                                    (n == 2 ? RCAR_DREQ_HSCIF2_TXI: RCAR_DREQ_HSCIF3_TXI)))
#define RCAR_DREQ_HSCIF_RXI(n)      (n == 0 ? RCAR_DREQ_HSCIF0_RXI: \
                                    (n == 1 ? RCAR_DREQ_HSCIF1_RXI: \
                                    (n == 2 ? RCAR_DREQ_HSCIF2_RXI: RCAR_DREQ_HSCIF3_RXI)))
#define RCAR_DREQ_I2C_TXI(n)        (n == 0 ? RCAR_DREQ_I2C0_TXI: \
                                    (n == 1 ? RCAR_DREQ_I2C1_TXI: \
                                    (n == 2 ? RCAR_DREQ_I2C2_TXI: \
                                    (n == 3 ? RCAR_DREQ_I2C3_TXI: \
                                    (n == 4 ? RCAR_DREQ_I2C4_TXI: RCAR_DREQ_I2C5_TXI)))))
#define RCAR_DREQ_I2C_RXI(n)        (n == 0 ? RCAR_DREQ_I2C0_RXI: \
                                    (n == 1 ? RCAR_DREQ_I2C1_RXI: \
                                    (n == 2 ? RCAR_DREQ_I2C2_RXI: \
                                    (n == 3 ? RCAR_DREQ_I2C3_RXI: \
                                    (n == 3 ? RCAR_DREQ_I2C4_RXI: RCAR_DREQ_I2C5_RXI)))))
#define RCAR_DREQ_MSIOF_TXI(n)      (n == 0 ? RCAR_DREQ_MSIOF0_TXI: \
                                    (n == 1 ? RCAR_DREQ_MSIOF1_TXI: \
                                    (n == 2 ? RCAR_DREQ_MSIOF2_TXI: RCAR_DREQ_MSIOF3_TXI)))
#define RCAR_DREQ_MSIOF_RXI(n)      (n == 0 ? RCAR_DREQ_MSIOF0_RXI: \
                                    (n == 1 ? RCAR_DREQ_MSIOF1_RXI: \
                                    (n == 2 ? RCAR_DREQ_MSIOF2_RXI: RCAR_DREQ_MSIOF3_RXI)))

/*
 * Flexray
 */
#define RCAR_FLEXRAY_BASE           0xE6B00000
#define RCAR_FLEXRAY_SIZE           0x1000

/*
 * I2C
 */
#define RCAR_I2C0_BASE              0xE6500000
#define RCAR_I2C1_BASE              0xE6508000
#define RCAR_I2C2_BASE              0xE6510000
#define RCAR_I2C3_BASE              0xE66D0000
#define RCAR_I2C4_BASE              0xE66D8000
#define RCAR_I2C5_BASE              0xE66E0000
#define RCAR_I2C6_BASE              0xE66E8000
#define RCAR_I2C7_BASE              0xE6690000
#define RCAR_I2C_SIZE               0x100

/* I2C registers, offset from I2C base */
#define RCAR_I2C_ICSCR             0x00        // Slave Control Register
 #define RCAR_ICSCR_SDBS            (1 << 3)
 #define RCAR_ICSCR_SIE             (1 << 2)
 #define RCAR_ICSCR_GCAE            (1 << 1)
 #define RCAR_ICSCR_FNA             (1 << 0)

#define RCAR_I2C_ICMCR             0x04        // Master Control Register
 #define RCAR_ICMCR_MDBS            (1 << 7)
 #define RCAR_ICMCR_FSCL            (1 << 6)
 #define RCAR_ICMCR_FSDA            (1 << 5)
 #define RCAR_ICMCR_OBPC            (1 << 4)
 #define RCAR_ICMCR_MIE             (1 << 3)
 #define RCAR_ICMCR_TSBE            (1 << 2)
 #define RCAR_ICMCR_FSB             (1 << 1)
 #define RCAR_ICMCR_ESG             (1 << 0)

#define RCAR_I2C_ICSSR             0x08        // Slave Status Register
 #define RCAR_ICSSR_GCAR            (1 << 6)
 #define RCAR_ICSSR_STM             (1 << 5)
 #define RCAR_ICSSR_SSR             (1 << 4)
 #define RCAR_ICSSR_SDE             (1 << 3)
 #define RCAR_ICSSR_SDT             (1 << 2)
 #define RCAR_ICSSR_SDR             (1 << 1)
 #define RCAR_ICSSR_SAR             (1 << 0)

#define RCAR_I2C_ICMSR             0x0C        // Master Status Register
 #define RCAR_ICMSR_MNR             (1 << 6)
 #define RCAR_ICMSR_MAL             (1 << 5)
 #define RCAR_ICMSR_MST             (1 << 4)
 #define RCAR_ICMSR_MDE             (1 << 3)
 #define RCAR_ICMSR_MDT             (1 << 2)
 #define RCAR_ICMSR_MDR             (1 << 1)
 #define RCAR_ICMSR_MAT             (1 << 0)

#define RCAR_I2C_ICSIER            0x10        // Slave Interrupt Enable Register
 #define RCAR_ICSIER_SSRE           (1 << 4)
 #define RCAR_ICSIER_SDEE           (1 << 3)
 #define RCAR_ICSIER_SDTE           (1 << 2)
 #define RCAR_ICSIER_SDRE           (1 << 1)
 #define RCAR_ICSIER_SARE           (1 << 0)

#define RCAR_I2C_ICMIER            0x14        // Master Interrupt Enable Register
 #define RCAR_ICMIER_MNRE           (1 << 6)
 #define RCAR_ICMIER_MALE           (1 << 5)
 #define RCAR_ICMIER_MSTE           (1 << 4)
 #define RCAR_ICMIER_MDEE           (1 << 3)
 #define RCAR_ICMIER_MDTE           (1 << 2)
 #define RCAR_ICMIER_MDRE           (1 << 1)
 #define RCAR_ICMIER_MATE           (1 << 0)

#define RCAR_I2C_ICCCR             0x18        // Clock Control Register
#define RCAR_I2C_ICSAR             0x1C        // Slave Address Register
#define RCAR_I2C_ICMAR             0x20        // Master Address Register
 #define RCAR_GEN3_ICMAR_STM1            (1 << 0)

#define RCAR_I2C_ICRXD             0x24        // Receive Data Register
#define RCAR_I2C_ICTXD             0x24        // Transmit Data Register
#define RCAR_I2C_ICCCR2            0x28
#define RCAR_I2C_ICMPR             0x2C
#define RCAR_I2C_ICHPR             0x30
#define RCAR_I2C_ICLPR             0x34
#define RCAR_I2C_ICDMAER           0x3C        //DMA enable register
 #define RCAR_ICDMAER_RSDMAE        (1 << 3)
 #define RCAR_ICDMAER_TSDMAE        (1 << 2)
 #define RCAR_ICDMAER_RMDMAE        (1 << 1)
 #define RCAR_ICDMAER_TMDMAE        (1 << 0)

#define RCAR_I2C_ICFBSCR           0x38
 #define RCAR_ICFBSCR_TCYC17        0x07

/*
 * I2C Bus Interface for DVFS IIC
 */
#define RCAR_IIC_BASE               0xE60B0000
#define RCAR_IIC_SIZE               0x100

/* IIC registers, offset from base */
#define RCAR_IIC_ICDR              0x0000      // Data Register
#define RCAR_IIC_ICCR              0x0004      // Control Register
 #define RCAR_ICCR_ICE              (1 << 7)    // IIC Interface Enable
 #define RCAR_ICCR_RACK             (1 << 6)    // Receive Acknowledge
 #define RCAR_ICCR_MTM              (1 << 4)    // Master Transmit Mode
 #define RCAR_ICCR_MRM              (0 << 4)    // Master Receive Mode
 #define RCAR_ICCR_BBSY             (1 << 2)    // Bus Busy
 #define RCAR_ICCR_SCP              (1 << 0)    // START/STOP Condition Prohibit

#define RCAR_IIC_ICSR              0x0008      // Status Register
 #define RCAR_ICSR_DTE              (1 << 0)    // Data Transmit Enable
 #define RCAR_ICSR_WAIT             (1 << 1)    // Wait
 #define RCAR_ICSR_TACK             (1 << 2)    // Transmit Acknowledge
 #define RCAR_ICSR_AL               (1 << 3)    // Arbitration Lost
 #define RCAR_ICSR_BUSY             (1 << 4)    // Transmit Busy

#define RCAR_IIC_ICIC              0x000C      // Interrupt Control Register
 #define RCAR_ICIC_DTE              (1 << 0)    // Data Transmit Enable
 #define RCAR_ICIC_WAIT             (1 << 1)    // Wait
 #define RCAR_ICIC_TACK             (1 << 2)    // Transmit Acknowledge
 #define RCAR_ICIC_AL               (1 << 3)    // Arbitration Lost

#define RCAR_IIC_ICCL              0x0010      // Clock Control Register Low
#define RCAR_IIC_ICCH              0x0014      // Clock Control Register High

/*
 * MSIOF
 */
#define RCAR_MSIOF0_BASE            0xE6E90000
#define RCAR_MSIOF1_BASE            0xE6EA0000
#define RCAR_MSIOF2_BASE            0xE6C00000
#define RCAR_MSIOF3_BASE            0xE6C10000
#define RCAR_MSIOF4_BASE            0xE6C20000
#define RCAR_MSIOF5_BASE            0xE6C28000

#define RCAR_MSIOF_SIZE             0x100

/* MSIOF registers, offset from base */
#define RCAR_MSIOF_TMDR1           0x00
#define RCAR_MSIOF_TMDR2           0x04
#define RCAR_MSIOF_TMDR3           0x08
#define RCAR_MSIOF_RMDR1           0x10
#define RCAR_MSIOF_RMDR2           0x14
#define RCAR_MSIOF_RMDR3           0x18
#define RCAR_MSIOF_TSCR            0x20
#define RCAR_MSIOF_RSCR            0x22
#define RCAR_MSIOF_CTR             0x28
#define RCAR_MSIOF_FCTR            0x30
#define RCAR_MSIOF_STR             0x40
#define RCAR_MSIOF_IER             0x44
#define RCAR_MSIOF_TFDR            0x50
#define RCAR_MSIOF_RFDR            0x60

/*
 * SCIF
 */
#define RCAR_SCIF0_BASE             0xE6E60000
#define RCAR_SCIF1_BASE             0xE6E68000
#define RCAR_SCIF2_BASE             0xE6E88000
#define RCAR_SCIF3_BASE             0xE6C50000
#define RCAR_SCIF4_BASE             0xE6C40000
#define RCAR_SCIF5_BASE             0xE6F30000

#define RCAR_SCIF_SIZE              0x100

#define RCAR_SCIF_FIFO_SIZE         16

/* Offset from SCIF_BASE */
#define RCAR_SCIF_SCSMR            0x00
 #define RCAR_SCIF_SCSMR_CHR        (1 << 6)
 #define RCAR_SCIF_SCSMR_PE         (1 << 5)
 #define RCAR_SCIF_SCSMR_OE         (1 << 4)
 #define RCAR_SCIF_SCSMR_STOP       (1 << 3)
 #define RCAR_SCIF_SCSMR_CKS_0      (0 << 0)
 #define RCAR_SCIF_SCSMR_CKS_4      (1 << 0)
 #define RCAR_SCIF_SCSMR_CKS_16     (2 << 0)
 #define RCAR_SCIF_SCSMR_CKS_64     (3 << 0)

#define RCAR_SCIF_SCBRR            0x04
#define RCAR_SCIF_SCSCR            0x08
  #define RCAR_SCIF_SCSCR_TEIE      (1 << 11)
  #define RCAR_SCIF_SCSCR_TIE       (1 << 7)
  #define RCAR_SCIF_SCSCR_RIE       (1 << 6)
  #define RCAR_SCIF_SCSCR_TE        (1 << 5)
  #define RCAR_SCIF_SCSCR_RE        (1 << 4)
  #define RCAR_SCIF_SCSCR_REIE      (1 << 3)
  #define RCAR_SCIF_SCSCR_CKE_MASK  (3 << 0)

#define RCAR_SCIF_SCFTDR           0x0C
#define RCAR_SCIF_SCFSR            0x10
  #define RCAR_SCIF_SCFSR_PERF(x)   (((x) >> 12) & 0xF)
  #define RCAR_SCIF_SCFSR_FERF(x)   (((x) >> 8) & 0xF)
  #define RCAR_SCIF_SCFSR_ER        (1 << 7)
  #define RCAR_SCIF_SCFSR_TEND      (1 << 6)
  #define RCAR_SCIF_SCFSR_TDFE      (1 << 5)
  #define RCAR_SCIF_SCFSR_BRK       (1 << 4)
  #define RCAR_SCIF_SCFSR_FER       (1 << 3)
  #define RCAR_SCIF_SCFSR_PER       (1 << 2)
  #define RCAR_SCIF_SCFSR_RDF       (1 << 1)
  #define RCAR_SCIF_SCFSR_DR        (1 << 0)

#define RCAR_SCIF_SCFRDR           0x14
#define RCAR_SCIF_SCFCR            0x18
  #define RCAR_SCIF_SCFCR_RTRG_1    (0 << 6)
  #define RCAR_SCIF_SCFCR_RTRG_4    (1 << 6)
  #define RCAR_SCIF_SCFCR_RTRG_8    (2 << 6)
  #define RCAR_SCIF_SCFCR_RTRG_14   (3 << 6)
  #define RCAR_SCIF_SCFCR_TTRG_0    (3 << 4)
  #define RCAR_SCIF_SCFCR_TTRG_2    (2 << 4)
  #define RCAR_SCIF_SCFCR_TTRG_4    (1 << 4)
  #define RCAR_SCIF_SCFCR_TTRG_8    (0 << 4)
  #define RCAR_SCIF_SCFCR_MCE       (1 << 3)
  #define RCAR_SCIF_SCFCR_TFRST     (1 << 2)
  #define RCAR_SCIF_SCFCR_RFRST     (1 << 1)
  #define RCAR_SCIF_SCFCR_LOOP      (1 << 0)
#define RCAR_SCIF_SCFDR            0x1C
  #define RCAR_SCIF_SCFDR_TX(x)     (((x) >> 8) & 0x1F)
  #define RCAR_SCIF_SCFDR_RX(x)     ((x) & 0x1F)

#define RCAR_SCIF_SCSPTR           0x20
  #define RCAR_SCIF_SCSPTR_RTSIO    (1 << 7)
  #define RCAR_SCIF_SCSPTR_RTSDT    (1 << 6)
  #define RCAR_SCIF_SCSPTR_CTSIO    (1 << 5)
  #define RCAR_SCIF_SCSPTR_CTSDT    (1 << 4)
  #define RCAR_SCIF_SCSPTR_SPB2IO   (1 << 1)
  #define RCAR_SCIF_SCSPTR_SPB2DT   (1 << 0)

#define RCAR_SCIF_SCLSR            0x24
  #define RCAR_SCIF_SCLSR_ORER      (1 << 0)

#define RCAR_SCIF_BRG_DL           0x30
#define RCAR_SCIF_BRG_CKS          0x34

/*
 * HSCIF
 */
#define RCAR_HSCIF0_BASE            0xE6540000
#define RCAR_HSCIF1_BASE            0xE6550000
#define RCAR_HSCIF2_BASE            0xE6560000
#define RCAR_HSCIF3_BASE            0xE66A0000
#define RCAR_HSCIF4_BASE            0xE66B0000
#define RCAR_HSCIF_SIZE             0x100

/*
* RPC (SPI)
*/
#define RCAR_RPC_BASE              0xEE200000
#define RCAR_RPC_SIZE              0x200

#define RCAR_RPC_BUFFER_BASE       0xEE208000
#define RCAR_RPC_BUFFER_SIZE       0x100
#define RCAR_RPC_BUFFER            0x8000

/* Hyperflash */
#define RCAR_RPC_DEV_ADDR           0x08000000
#define RCAR_RPC_DEV_SIZE           0x04000000

/* RPC registers, offset from RCAR_GEN3_RPC_BASE */
#define RCAR_V3M_RPC_DIV_REG_1_0       0xEE2000A8       /* Divider clock register V3M 1.0 only */
#define RCAR_V3M_RPC_DIV_REG           0xE627100C       /* Divider clock register V3M 1.1 or later*/


/*
 * HSUSB
 */
#define RCAR_HSUSB_BASE             0xE6590000
#define RCAR_HSUSB3_BASE            0xE659C000    /* R-CarH3 only */
#define RCAR_HSUSB_SYSCFG           0x0000
#define RCAR_HSUSB_INTENB0          0x0030
#define RCAR_HSUSB_LPSTS            0x0102
#define RCAR_HSUSB_UGCTRL           0x0180
#define RCAR_HSUSB_UGCTRL2          0x0184
#define RCAR_HSUSB_LPSTS_SUSPM      0x4000
#define RCAR_HSUSB_USB0SEL          0x00000030
#define RCAR_HSUSB_USB0SEL_EHCI     0x00000010
#define RCAR_HSUSB_SUSPM           (1 << 14)
#define RCAR_HSUSB_SUSPM_NORMAL    (1 << 14)

/* Register offset */
/* AHB bridge registers */
#define RCAR_USB_AHB_INT_ENABLE    0x200
#define RCAR_USB_AHB_USBCTR        0x20C
 #define RCAR_USB_AHB_PLL_RST       (1 << 1)
 #define RCAR_USB_AHB_USBH_INTBEN   (1 << 2)
 #define RCAR_USB_AHB_USBH_INTAEN   (1 << 1)

/* UCOM registers */
#define RCAR_USB_UCOM_COMMCTRL		 0x800
 #define RCAR_USB_UCOM_OTG_PERI      (1 << 31)
 #define RCAR_USB_UCOM_OTG_PERI_HOST (0 << 31)
 #define RCAR_USB_UCOM_OTG_PERI_PERI (1 << 31)

 /* Core defined registers */
#define RCAR_USB_CORE_SPD_RSM_TIMSET     0x30C
#define RCAR_USB_CORE_OC_TIMSET          0x310

/*
 * USB 2.0
 */
#define RCAR_USB0_BASE              0xEE080000     // USB2.0 ch0
#define RCAR_USB1_BASE              0xEE0A0000     // USB2.0 ch1
#define RCAR_USB2_BASE              0xEE0C0000     // USB2.0 ch2
#define RCAR_USB3_BASE              0xEE0E0000     // USB2.0 ch3

#define RCAR_PCI_CONF_EHCI          0x10100        // Offset to PCI configuration Register

/* Core defined registers */
#define RCAR_USB_CORE_SPD_RSM_TIMSET     0x30C
#define RCAR_USB_CORE_OC_TIMSET          0x310

/*
 * USB 3.0
 */
#define RCAR_USB30_BASE             0xEE000000  /* USB3.0 ch0 */
#define RCAR_USB31_BASE             0xEE040000  /* USB3.0 ch1 */
#define RCAR_USB3_SIZE              0xBFF

/* Register Offset */
#define RCAR_USB3_CLASSCODE        0x204
#define RCAR_USB3_RELEASE_NUMBER   0x210
#define RCAR_USB3_INT_ENA          0x224      /* Interrupt Enable */
 #define RCAR_USB3_INT_XHC_ENA       0x00000001
 #define RCAR_USB3_INT_PME_ENA       0x00000002
 #define RCAR_USB3_INT_HSE_ENA       0x00000004
 #define RCAR_USB3_INT_LTM_ENA       0x00000008
 #define RCAR_USB3_INT_SMI_ENA       0x00000010
 #define RCAR_USB3_INT_ENA_VAL       (RCAR_USB3_INT_XHC_ENA | RCAR_USB3_INT_PME_ENA | RCAR_USB3_INT_HSE_ENA)

#define RCAR_USB3_DL_CTRL          0x250      /* FW Download Control & Status */
 #define RCAR_USB3_DL_CTRL_ENABLE        0x00000001
 #define RCAR_USB3_DL_CTRL_FW_SUCCESS    0x00000010
 #define RCAR_USB3_DL_CTRL_FW_SET_DATA0  0x00000100

#define RCAR_USB3_FW_DATA0         0x258      /* FW Data0 */
#define RCAR_USB3_LCLK             0xa44      /* LCLK Select */
 #define RCAR_USB3_PTPWER_CTRL      (1 << 24)
 #define RCAR_USB3_RENESAS_PRIV     (0x3 << 16)
 #define RCAR_USB3_BC_MODE          (1 << 0)
 #define RCAR_USB3_LCLK_ENA_VAL     RCAR_USB3_PTPWER_CTRL | RCAR_USB3_RENESAS_PRIV | RCAR_USB3_BC_MODE

#define RCAR_USB3_CONF1            0xa48      /* USB3.0 Configuration1 */
#define RCAR_USB3_CONF2            0xa5c      /* USB3.0 Configuration2 */
#define RCAR_USB3_CONF3            0xaa8      /* USB3.0 Configuration3 */
 #define RCAR_USB3_CONF1_VAL        0x00030204
 #define RCAR_USB3_CONF2_VAL        0x00030300
 #define RCAR_USB3_CONF3_VAL        0x13802007

#define RCAR_USB3_RX_POL           0xab0      /* USB3.0 RX Polarity */
#define RCAR_USB3_TX_POL           0xab8      /* USB3.0 TX Polarity */
 #define RCAR_USB3_RX_POL_VAL        BIT(21)
 #define RCAR_USB3_TX_POL_VAL        BIT(4)

/*
 * DBSC4 Controller
 */
 //#define RCAR_SDRAM_0               0x48000000
 #define RCAR_SDRAM_0               0x40000000
 #define RCAR_SDRAM_0_0             0x400000000 // First 2GB of this area is shadow area of RCAR_GEN3_SDRAM_0 
 #define RCAR_SDRAM_0_1             0x480000000 
 #define RCAR_SDRAM_1               0x500000000
 #define RCAR_SDRAM_2               0x600000000
 #define RCAR_SDRAM_3               0x700000000
 #define RCAR_MAX_RANK              4

#define RCAR_DBSC4_BASE             0xE6790000
/* DBSC4 registers, offset from DBSC4_BASE */
#define RCAR_DBSC4_DBSYSCONF1      0x0004U
#define RCAR_DBSC4_DBPHYCONF0      0x0010U
#define RCAR_DBSC4_DBKIND          0x0020U
#define RCAR_DBSC4_DBMEMCONF_0_0   0x0030U
#define RCAR_DBSC4_DBMEMCONF_0_1   0x0034U
#define RCAR_DBSC4_DBMEMCONF_1_0   0x0040U
#define RCAR_DBSC4_DBMEMCONF_1_1   0x0044U
#define RCAR_DBSC4_DBMEMCONF_2_0   0x0050U
#define RCAR_DBSC4_DBMEMCONF_2_1   0x0054U
#define RCAR_DBSC4_DBMEMCONF_3_0   0x0060U
#define RCAR_DBSC4_DBMEMCONF_3_1   0x0064U
#define RCAR_DBSC4_DBMEMCONF_0_2   0x0038
#define RCAR_DBSC4_DBMEMCONF_0_3   0x003C
#define RCAR_DBSC4_DBMEMCONF_1_2   0x0048
#define RCAR_DBSC4_DBMEMCONF_1_3   0x004C
#define RCAR_DBSC4_DBMEMCONF_2_2   0x0058
#define RCAR_DBSC4_DBMEMCONF_2_3   0x005C
#define RCAR_DBSC4_DBMEMCONF_3_2   0x0068
#define RCAR_DBSC4_DBMEMCONF_3_3   0x006C
#define RCAR_DBSC4_DBSYSCNT0       0x0100U
#define RCAR_DBSC4_DBSVCR1         0x0104U
#define RCAR_DBSC4_DBSTATE0        0x0108U
#define RCAR_DBSC4_DBSTATE1        0x010CU
#define RCAR_DBSC4_DBACEN          0x0200U
#define RCAR_DBSC4_DBRFEN          0x0204U
#define RCAR_DBSC4_DBCMD           0x0208U
#define RCAR_DBSC4_DBWAIT          0x0210U //wait DBCMD 1=busy, 0=ready
#define RCAR_DBSC4_DBSYSCTRL0      0x0280U
#define RCAR_DBSC4_DBTR0           0x0300U
#define RCAR_DBSC4_DBTR1           0x0304U
#define RCAR_DBSC4_DBTR2           0x0308U
#define RCAR_DBSC4_DBTR3           0x030CU
#define RCAR_DBSC4_DBTR4           0x0310U
#define RCAR_DBSC4_DBTR5           0x0314U
#define RCAR_DBSC4_DBTR6           0x0318U
#define RCAR_DBSC4_DBTR7           0x031CU
#define RCAR_DBSC4_DBTR8           0x0320U
#define RCAR_DBSC4_DBTR9           0x0324U
#define RCAR_DBSC4_DBTR10          0x0328U
#define RCAR_DBSC4_DBTR11          0x032CU
#define RCAR_DBSC4_DBTR12          0x0330U
#define RCAR_DBSC4_DBTR13          0x0334U
#define RCAR_DBSC4_DBTR14          0x0338U
#define RCAR_DBSC4_DBTR15          0x033CU
#define RCAR_DBSC4_DBTR16          0x0340U
#define RCAR_DBSC4_DBTR17          0x0344U
#define RCAR_DBSC4_DBTR18          0x0348U
#define RCAR_DBSC4_DBTR19          0x034CU
#define RCAR_DBSC4_DBTR20          0x0350U
#define RCAR_DBSC4_DBTR21          0x0354U
#define RCAR_DBSC4_DBTR22          0x0358U
#define RCAR_DBSC4_DBTR23          0x035CU
#define RCAR_DBSC4_DBTR24          0x0360U
#define RCAR_DBSC4_DBTR25          0x0364U
#define RCAR_DBSC4_DBBL            0x0400U
#define RCAR_DBSC4_DBRFCNF1        0x0414U
#define RCAR_DBSC4_DBRFCNF2        0x0418U
#define RCAR_DBSC4_DBCALCNF        0x0424U
#define RCAR_DBSC4_DBRNK2          0x0438U
#define RCAR_DBSC4_DBRNK3          0x043CU
#define RCAR_DBSC4_DBRNK4          0x0440U
#define RCAR_DBSC4_DBRNK5          0x0444U
#define RCAR_DBSC4_DBPDNCNF        0x0450U
#define RCAR_DBSC4_DBODT0          0x0460U
#define RCAR_DBSC4_DBODT1          0x0464U
#define RCAR_DBSC4_DBODT2          0x0468U
#define RCAR_DBSC4_DBODT3          0x046CU
#define RCAR_DBSC4_DBADJ0          0x0500U
#define RCAR_DBSC4_DBDBICNT        0x0518U
#define RCAR_DBSC4_DBDFIPMSTRCNF   0x0520
#define RCAR_DBSC4_DBDFICUPDCNF    0x052CU
#define RCAR_DBSC4_DBDFISTAT_0     0x0600
#define RCAR_DBSC4_DBDFICNT0       0x0604U
#define RCAR_DBSC4_DBPDLK0         0x0620U
#define RCAR_DBSC4_DBPDRGA0        0x0624U
#define RCAR_DBSC4_DBPDRGD0        0x0628U
#define RCAR_DBSC4_DBPDSTAT00      0x0630U
#define RCAR_DBSC4_DBDFISTAT_1     0x0640
#define RCAR_DBSC4_DBDFICNT1       0x0644U
#define RCAR_DBSC4_DBPDLK1         0x0660U
#define RCAR_DBSC4_DBPDRGA1        0x0664U
#define RCAR_DBSC4_DBPDRGD1        0x0668U
#define RCAR_DBSC4_DBDFISTAT_2     0x0680
#define RCAR_DBSC4_DBDFICNT2       0x0684U
#define RCAR_DBSC4_DBDFISTAT_3     0x06C0
#define RCAR_DBSC4_DBPDLK2         0x06A0U
#define RCAR_DBSC4_DBPDRGA2        0x06A4U
#define RCAR_DBSC4_DBPDRGD2        0x06A8U
#define RCAR_DBSC4_DBPDSTAT20      0x06B0U
#define RCAR_DBSC4_DBDFICNT3       0x06C4U
#define RCAR_DBSC4_DBPDLK3         0x06E0U
#define RCAR_DBSC4_DBPDRGA3        0x06E4U
#define RCAR_DBSC4_DBPDRGD3        0x06E8U
#define RCAR_DBSC4_DBDFICNT_0      0x0604
#define RCAR_DBSC4_DBDFICNT_1      0x0644
#define RCAR_DBSC4_DBDFICNT_2      0x0684
#define RCAR_DBSC4_DBDFICNT_3      0x06C4
#define RCAR_DBSC4_DBPDCNT0_0      0x0610
#define RCAR_DBSC4_DBPDCNT0_1      0x0650
#define RCAR_DBSC4_DBPDCNT0_2      0x0690
#define RCAR_DBSC4_DBPDCNT0_3      0x06D0
#define RCAR_DBSC4_DBPDCNT3_0      0x061C
#define RCAR_DBSC4_DBPDCNT3_1      0x065C
#define RCAR_DBSC4_DBPDCNT3_2      0x069C
#define RCAR_DBSC4_DBPDCNT3_3      0x06DC
#define RCAR_DBSC4_DBPDRGA_0       0x0624
#define RCAR_DBSC4_DBPDRGD_0       0x0628
#define RCAR_DBSC4_DBPDRGA_1       0x0664
#define RCAR_DBSC4_DBPDRGD_1       0x0668
#define RCAR_DBSC4_DBPDRGA_2       0x06A4
#define RCAR_DBSC4_DBPDRGD_2       0x06A8
#define RCAR_DBSC4_DBPDRGA_3       0x06E4
#define RCAR_DBSC4_DBPDRGD_3       0x06E8
#define RCAR_DBSC4_DBBUS0CNF0      0x0800
#define RCAR_DBSC4_DBBUS0CNF1      0x0804
#define RCAR_DBSC4_DBCAM0CNF1      0x0904 //CAM Unit Configuration Register 1
#define RCAR_DBSC4_DBCAM0CNF2      0x0908 //CAM Unit Configuration Register 2
#define RCAR_DBSC4_DBCAM0CNF3      0x090C //CAM Unit Configuration Register 3
#define RCAR_DBSC4_DBCAM0STAT0     0x0980U
#define RCAR_DBSC4_DBCAM1STAT0     0x0990U
#define RCAR_DBSC4_DBSCHCNT0       0x1000 //Scheduler Unit Operation Setting Register 0
#define RCAR_DBSC4_DBSCHCNT1       0x1004 //Scheduler Unit Operation Setting Register 1
#define RCAR_DBSC4_DBSCHSZ0        0x1010 //Size Miss Scheduling Setting Register 0
#define RCAR_DBSC4_DBSCHRW0        0x1020 //Read/Write Scheduling Setting Register 0
#define RCAR_DBSC4_DBSCHRW1        0x1024 //Read/Write Scheduling Setting Register 1
#define RCAR_DBSC4_DBSCHQOS00      0x1030U
#define RCAR_DBSC4_DBSCHQOS01      0x1034U
#define RCAR_DBSC4_DBSCHQOS02      0x1038U
#define RCAR_DBSC4_DBSCHQOS03      0x103CU
#define RCAR_DBSC4_DBSCHQOS10      0x1040U
#define RCAR_DBSC4_DBSCHQOS11      0x1044U
#define RCAR_DBSC4_DBSCHQOS12      0x1048U
#define RCAR_DBSC4_DBSCHQOS13      0x104CU
#define RCAR_DBSC4_DBSCHQOS20      0x1050U
#define RCAR_DBSC4_DBSCHQOS21      0x1054U
#define RCAR_DBSC4_DBSCHQOS22      0x1058U
#define RCAR_DBSC4_DBSCHQOS23      0x105CU
#define RCAR_DBSC4_DBSCHQOS30      0x1060U
#define RCAR_DBSC4_DBSCHQOS31      0x1064U
#define RCAR_DBSC4_DBSCHQOS32      0x1068U
#define RCAR_DBSC4_DBSCHQOS33      0x106CU
#define RCAR_DBSC4_DBSCHQOS40      0x1070U
#define RCAR_DBSC4_DBSCHQOS41      0x1074U
#define RCAR_DBSC4_DBSCHQOS42      0x1078U
#define RCAR_DBSC4_DBSCHQOS43      0x107CU
#define RCAR_DBSC4_DBSCHQOS50      0x1080U 
#define RCAR_DBSC4_DBSCHQOS51      0x1084U
#define RCAR_DBSC4_DBSCHQOS52      0x1088U
#define RCAR_DBSC4_DBSCHQOS53      0x108CU
#define RCAR_DBSC4_DBSCHQOS60      0x1090U
#define RCAR_DBSC4_DBSCHQOS61      0x1094U
#define RCAR_DBSC4_DBSCHQOS62      0x1098U
#define RCAR_DBSC4_DBSCHQOS63      0x109CU
#define RCAR_DBSC4_DBSCHQOS70      0x10A0U
#define RCAR_DBSC4_DBSCHQOS71      0x10A4U
#define RCAR_DBSC4_DBSCHQOS72      0x10A8U
#define RCAR_DBSC4_DBSCHQOS73      0x10ACU
#define RCAR_DBSC4_DBSCHQOS80      0x10B0U
#define RCAR_DBSC4_DBSCHQOS81      0x10B4U
#define RCAR_DBSC4_DBSCHQOS82      0x10B8U
#define RCAR_DBSC4_DBSCHQOS83      0x10BCU
#define RCAR_DBSC4_DBSCHQOS90      0x10C0U
#define RCAR_DBSC4_DBSCHQOS91      0x10C4U
#define RCAR_DBSC4_DBSCHQOS92      0x10C8U
#define RCAR_DBSC4_DBSCHQOS93      0x10CCU
#define RCAR_DBSC4_DBSCHQOS100     0x10D0U
#define RCAR_DBSC4_DBSCHQOS101     0x10D4U
#define RCAR_DBSC4_DBSCHQOS102     0x10D8U
#define RCAR_DBSC4_DBSCHQOS103     0x10DCU
#define RCAR_DBSC4_DBSCHQOS110     0x10E0U
#define RCAR_DBSC4_DBSCHQOS111     0x10E4U
#define RCAR_DBSC4_DBSCHQOS112     0x10E8U
#define RCAR_DBSC4_DBSCHQOS113     0x10ECU
#define RCAR_DBSC4_DBSCHQOS120     0x10F0U
#define RCAR_DBSC4_DBSCHQOS121     0x10F4U
#define RCAR_DBSC4_DBSCHQOS122     0x10F8U
#define RCAR_DBSC4_DBSCHQOS123     0x10FCU
#define RCAR_DBSC4_DBSCHQOS130     0x1100U
#define RCAR_DBSC4_DBSCHQOS131     0x1104U
#define RCAR_DBSC4_DBSCHQOS132     0x1108U
#define RCAR_DBSC4_DBSCHQOS133     0x110CU
#define RCAR_DBSC4_DBSCHQOS140     0x1110U
#define RCAR_DBSC4_DBSCHQOS141     0x1114U
#define RCAR_DBSC4_DBSCHQOS142     0x1118U
#define RCAR_DBSC4_DBSCHQOS143     0x111CU
#define RCAR_DBSC4_DBSCHQOS150     0x1120U
#define RCAR_DBSC4_DBSCHQOS151     0x1124U
#define RCAR_DBSC4_DBSCHQOS152     0x1128U
#define RCAR_DBSC4_DBSCHQOS153     0x112CU
#define RCAR_DBSC4_SCFCTST0        0x1700 //Schedule timing setting register 0
#define RCAR_DBSC4_SCFCTST1        0x1708 //Schedule timing setting register 1
#define RCAR_DBSC4_SCFCTST2        0x170C //Schedule timing setting register 2
#define RCAR_DBSC4_DBMRRDR0        0x1800U
#define RCAR_DBSC4_DBMRRDR1        0x1804U
#define RCAR_DBSC4_DBMRRDR2        0x1808U
#define RCAR_DBSC4_DBMRRDR3        0x180CU
#define RCAR_DBSC4_DBMRRDR4        0x1810U
#define RCAR_DBSC4_DBMRRDR5        0x1814U
#define RCAR_DBSC4_DBMRRDR6        0x1818U
#define RCAR_DBSC4_DBMRRDR7        0x181CU


/*
 * Thermal Sensor
 */
#define RCAR_TSC0_BASE              0xE6190000 
#define RCAR_TSC1_BASE              0xE6198000
#define RCAR_TSC2_BASE              0xE61A0000
#define RCAR_TSC3_BASE              0xE61A8000
#define RCAR_TSC4_BASE              0xE61B0000
#define RCAR_TSC_SIZE               0x1000
#define RCAR_THERMAL_TYPE_A         1
#define RCAR_THERMAL_TYPE_B         2
#define RCAR_THERMAL_TYPE_C         3

#define RCAR_TSC_IRQSTR                  0x004
#define RCAR_TSC_IRQMSK                  0x008
#define RCAR_TSC_IRQCTL                  0x00C
#define RCAR_TSC_IRQEN                   0x010
 #define RCAR_TSC_IRQ_TEMP1_EN               (1 << 0)
 #define RCAR_TSC_IRQ_TEMP2_EN               (1 << 1)
 #define RCAR_TSC_IRQ_TEMP3_EN               (1 << 2)
 #define RCAR_TSC_IRQ_TEMPD1_EN              (1 << 3)
 #define RCAR_TSC_IRQ_TEMPD2_EN              (1 << 4)
 #define RCAR_TSC_IRQ_TEMPD3_EN              (1 << 5)
#define RCAR_TSC_IRQTEMP1                0x014
#define RCAR_TSC_IRQTEMP2                0x018
#define RCAR_TSC_IRQTEMP3                0x01C
#define RCAR_TSC_THCTR                   0x020
 #define RCAR_TSC_THCTR_THSST                (1 << 0)
 #define RCAR_TSC_THCTR_THEN                 (1 << 5)
 #define RCAR_TSC_THCTR_PONM                 (1 << 6)
 #define RCAR_TSC_THCTR_SENSSEL              (3 << 8)
 #define RCAR_TSC_THCTR_CIVMTST              (1 << 20)
 #define RCAR_TSC_THCTR_CTCTL                (1 << 24)
#define RCAR_TSC_THSTR                   0x024
#define RCAR_TSC_TEMP                    0x028
 #define TEMP_CODE_MASK                       0xFFF
#define RCAR_TSC_VOLT                    0x02C 
 #define VOLT_CODE_MASK                       0x3FF
#define RCAR_TSC_TEMP_X                  0x030
#define RCAR_TSC_TEMP_YN1                0x034
#define RCAR_TSC_TEMP_X1                 0x038
#define RCAR_TSC_THINITSTR               0x03C
 #define RCAR_TSC_THINITSTR_NRME_ND          (1 << 3)
 #define RCAR_TSC_THINITSTR_THTMP3           (1 << 2)
 #define RCAR_TSC_THINITSTR_THTMP2           (1 << 1)
 #define RCAR_TSC_THINITSTR_THTMP1           (1 << 0)
 #define RCAR_TSC_THINITSTR_THTMP_MASK       0x7
 #define RCAR_TSC_THINITSTR_MASK             0xF
#define RCAR_TSC_THCODE_INT1             0x040
#define RCAR_TSC_THCODE_INT2             0x044
#define RCAR_TSC_THCODE_INT3             0x048
#define RCAR_TSC_CVCREG                  0x04C
#define RCAR3_TSC_THCODE1                0x050
#define RCAR3_TSC_THCODE2                0x054
#define RCAR3_TSC_THCODE3                0x058
#define RCAR_TSC_THCODE3(_id)	         ((_id == PRODUCT_ID_RCAR_S4) ? 0x180 : 0x58) /*THSFMON00*/
#define RCAR_TSC_THCODE1(_id)	         ((_id == PRODUCT_ID_RCAR_S4) ? 0x184 : 0x50) /*THSFMON01*/
#define RCAR_TSC_THCODE2(_id)	         ((_id == PRODUCT_ID_RCAR_S4) ? 0x188 : 0x54) /*THSFMON02*/
 #define RCAR_TSC_THCODE_MASK                 0xFFF
#define RCAR3_TSC_PTAT1                  0x05C
#define RCAR3_TSC_PTAT2                  0x060
#define RCAR3_TSC_PTAT3                  0x064
#define RCAR_TSC_PTAT3(_id)              ((_id == PRODUCT_ID_RCAR_S4) ? 0x1BC : 0x064) /*THSFMON15*/
#define RCAR_TSC_PTAT1(_id)              ((_id == PRODUCT_ID_RCAR_S4) ? 0x1C0 : 0x05C) /*THSFMON16*/
#define RCAR_TSC_PTAT2(_id)              ((_id == PRODUCT_ID_RCAR_S4) ? 0x1C4 : 0x060) /*THSFMON17*/
 #define RCAR_TSC_PTAT_MASK              0xFFF
#define RCAR_TSC_THSCP                   0x068
 #define COR_PARA_VLD                        (0x03 << 14)
#define RCAR_TSC_IRQ_INJECTION           0x06C
#define RCAR_TSC_ACK_TIMEOUT             0x070
#define RCAR_TSC_THERMAL_ERROR_CTL       0x074
#define RCAR_TSC_THSOUTCTL               0x078
#define RCAR_TSC_SEQ_RESET               0x07C
#define RCAR_TSC_PON_TEMP                0x080
#define RCAR_TSC_NML_TEMP                0x084
#define RCAR_TSC_NML_VOLT                0x088
#define RCAR_TSC_CIVMTST_VOLT1           0x08C
#define RCAR_TSC_CIVMTST_VOLT2           0x090
#define RCAR_TSC_CIVMTST_VOLT3           0x094
#define RCAR_TSC_CIVMTST_VOLT4           0x098
#define RCAR_TSC_MANTST_VOLT             0x09C
#define RCAR_TSC_VOLT_YN                 0x0A8
#define RCAR_TSC_VOLT_X                  0x0B0
#define RCAR_TSC_VOLT_X1                 0x0B8
#define RCAR_TSC_THS_MANUAL_SET          0x0C0
#define RCAR_TSC_VMCODE1                 0x0D0
#define RCAR_TSC_VMCODE2                 0x0D4
#define RCAR_TSC_VMCODE3                 0x0D8
 #define RCAR_TSC_VMCODE_MASK            0x3FF
#define RCAR_TSC_HVMCODE1                0x0DC
#define RCAR_TSC_HVMCODE2                0x0E0
#define RCAR_TSC_HVMCODE3                0x0E4
#define RCAR_TSC_INBUF_OFF1              0x0E8
#define RCAR_TSC_INBUF_OFF2              0x0EC
#define RCAR_TSC_INBUF_OFF3              0x0F0
#define RCAR_TSC_CVM_LOCK                0x0F8
#define RCAR_TSC_CVM_LOCK_BK             0x0FC
#define RCAR_TSC_CVM_LATCH_BK            0x100
#define RCAR_TSC_CVM_CTRL                0x104
 #define RCAR_TSC_CVM_CTRL_OUTPC_CVM                (1 << 8)
 #define RCAR_TSC_CVM_CTRL_EXTERNAL_FS_CVM_OUTPUT   (0 << 4)
 #define RCAR_TSC_CVM_CTRL_EXTERNAL_FS_THS_OUTPUT   (1 << 4)
 #define RCAR_TSC_CVM_CTRL_CHOICE_PLACE_CVM4_EN     (1 << 3)
 #define RCAR_TSC_CVM_CTRL_CHOICE_PLACE_CVM3_EN     (1 << 2)
 #define RCAR_TSC_CVM_CTRL_CHOICE_PLACE_CVM2_EN     (1 << 1)
 #define RCAR_TSC_CVM_CTRL_CHOICE_PLACE_CVM1_EN     (1 << 0)
#define RCAR_TSC_CVM_CTRL_BK              0x108
 #define RCAR_TSC_CVM_CTRL_BK_THEN_BK          (1 << 12)
#define RCAR_TSC_CVM_DETECT_MON          0x10C
#define RCAR_TSC_CVM_DETECT_MANUAL_SET   0x110
#define RCAR_TSC_CVM_DETECT_SET_BK       0x114
#define RCAR_TSC_CVM_TOFF_MON            0x118
#define RCAR_TSC_CVM_TOFF_MANUAL_SET     0x11C
#define RCAR_TSC_CVM_TOFF_BK             0x120
#define RCAR_TSC_CVM_VMOUT_BK            0x124
#define RCAR_TSC_STATMON1                0x128
#define RCAR_TSC_STATMON2                0x12C
#define RCAR_TSC_THERMAL_ERROR_MON       0x130
#define RCAR_TSC_THCODE_MON              0x134
#define RCAR_TSC_MANTST_SET_DT           0x138
#define RCAR_TSC_TMR_DT                  0x13C
#define RCAR_TSC_SEQ_ACT_MON             0x140
 #define RCAR_TSC_SEQ_ACT_MON_MANTSTSEQ_ACT   (1 << 4)
 #define RCAR_TSC_SEQ_ACT_MON_CIVMTSTSEQ_ACT  (1 << 3)
 #define RCAR_TSC_SEQ_ACT_MON_NMLSEQ_ACT      (1 << 2)
 #define RCAR_TSC_SEQ_ACT_MON_PONSEQ_ACT      (1 << 1)
 #define RCAR_TSC_SEQ_ACT_MON_TSTSEQ_ACT      (1 << 0)
 #define RCAR_TSC_SEQ_ACT_MON_ACT_MASK        (0x1F << 0)
#define RCAR_TSC_VMMSK_CTRL              0x144
#define RCAR_TSC_VMMSK_CTRL_MON          0x148
#define RCAR_TSC_VM_MON                  0x14C
#define RCAR_TSC_VM06CODE1               0x150
#define RCAR_TSC_VM06CODE2               0x154
#define RCAR_TSC_VM06CODE3               0x158
#define RCAR_TSC_VM11CODE1               0x15C
#define RCAR_TSC_VM11CODE2               0x160
#define RCAR_TSC_VM11CODE3               0x164
#define RCAR_TSC_VMCVMCODE1              0x168
#define RCAR_TSC_VMCVMCODE2              0x16C
#define RCAR_TSC_VMCVMCODE3              0x170
#define RCAR_TSC_THSFMON00               0x180
#define RCAR_TSC_THSFMON01               0x184
#define RCAR_TSC_THSFMON02               0x188
#define RCAR_TSC_THSFMON15               0x1BC
#define RCAR_TSC_THSFMON16               0x1C0
#define RCAR_TSC_THSFMON17               0x1C4

/* Thermal type B */
#define RCAR_TSC_B_STR                   0x00
#define RCAR_TSC_B_ENR                   0x04
#define RCAR_TSC_B_INT_MASK              0x0C
 #define RCAR_TSC_B_Tj00INT                 (1 << 0)
 #define RCAR_TSC_B_Tj01INT                 (1 << 1)
 #define RCAR_TSC_B_Tj02INT                 (1 << 2)
#define RCAR_TSC_B_POSNEG                0x120
#define RCAR_TSC_B_THSCR                 0x12C
 #define RCAR_TSC_B_THSCR_CPCTL             (1 << 12)
#define RCAR_TSC_B_THSSR                 0x130
 #define RCAR_TSC_THSSR_CTEMP_MASK          0x3F
#define RCAR_TSC_B_INTCTLR               0x134
 #define INTCTLR_CTEMP0_MASK                (0x3F << 0)
 #define INTCTLR_CTEMP1_MASK                (0x3F << 8)
 #define INTCTLR_CTEMP2_MASK                (0x3F << 16)
 #define INTCTLR_CTEMP0_VAL(val)            (((val) & 0x3F) << 0)
 #define INTCTLR_CTEMP1_VAL(val)            (((val) & 0x3F) << 8)
 #define INTCTLR_CTEMP2_VAL(val)            (((val) & 0x3F) << 16)
#define RCAR_TSC_B_CIVM_SR               0x21C
 #define RCAR_TSC_VOLT_CODE_B_MASK       0x3F


/*
 * Video Capture
 */
#define RCAR_VIN0_BASE              0xE6EF0000
#define RCAR_VIN1_BASE              0xE6EF1000
#define RCAR_VIN2_BASE              0xE6EF2000
#define RCAR_VIN3_BASE              0xE6EF3000
#define RCAR_VIN4_BASE              0xE6EF4000
#define RCAR_VIN5_BASE              0xE6EF5000
#define RCAR_VIN6_BASE              0xE6EF6000
#define RCAR_VIN7_BASE              0xE6EF7000
#define RCAR_VIN8_BASE              0xE6EF8000 /* V3H & V3U only */ 
#define RCAR_VIN9_BASE              0xE6EF9000
#define RCAR_VIN10_BASE             0xE6EFA000
#define RCAR_VIN11_BASE             0xE6EFB000
#define RCAR_VIN12_BASE             0xE6EFC000
#define RCAR_VIN13_BASE             0xE6EFD000
#define RCAR_VIN14_BASE             0xE6EFE000
#define RCAR_VIN15_BASE             0xE6EFF000
#define RCAR_VIN16_BASE             0xE6ED0000 /* V3U only */ 
#define RCAR_VIN17_BASE             0xE6ED1000
#define RCAR_VIN18_BASE             0xE6ED2000
#define RCAR_VIN19_BASE             0xE6ED3000
#define RCAR_VIN20_BASE             0xE6ED4000
#define RCAR_VIN21_BASE             0xE6ED5000
#define RCAR_VIN22_BASE             0xE6ED6000
#define RCAR_VIN23_BASE             0xE6ED7000
#define RCAR_VIN24_BASE             0xE6ED8000
#define RCAR_VIN25_BASE             0xE6ED9000
#define RCAR_VIN26_BASE             0xE6EDA000
#define RCAR_VIN27_BASE             0xE6EDB000
#define RCAR_VIN28_BASE             0xE6EDC000
#define RCAR_VIN29_BASE             0xE6EDD000
#define RCAR_VIN30_BASE             0xE6EDE000
#define RCAR_VIN31_BASE             0xE6EDF000
     
#define RCAR_VIN_SIZE               0x1000

#define RCAR_VIN_MC                 0x00000
 #define RCAR_VIN_MC_MASK            0xFFFFF
 #define RCAR_VIN_MC_DPINE               (1 << 27)
 #define RCAR_VIN_MC_SCLE                (1 << 26)
 #define RCAR_VIN_MC_FCPE                (1 << 24)
 #define RCAR_VIN_MC_FOC                 (1 << 21)
 #define RCAR_VIN_MC_YCAL                (1 << 19)
 #define RCAR_VIN_MC_INF_YUV8_BT656      (0 << 16)
 #define RCAR_VIN_MC_INF_YUV8_BT601      (1 << 16)
 #define RCAR_VIN_MC_INF_YUV10_BT656     (2 << 16)
 #define RCAR_VIN_MC_INF_YUV10_BT601     (3 << 16)
 #define RCAR_VIN_MC_INF_YUV16           (5 << 16)
 #define RCAR_VIN_MC_INF_RGB888          (6 << 16)
 #define RCAR_VIN_MC_INF_MASK            (7 << 16)
 #define RCAR_VIN_MC_VUP                 (1 << 10)
 #define RCAR_VIN_MC_IM_MSK              (3 << 3)
 #define RCAR_VIN_MC_IM_ODD              (0 << 3)
 #define RCAR_VIN_MC_IM_ODD_EVEN         (1 << 3)
 #define RCAR_VIN_MC_IM_EVEN             (2 << 3)
 #define RCAR_VIN_MC_IM_FULL             (3 << 3)
 #define RCAR_VIN_MC_BPS                 (1 << 1)
 #define RCAR_VIN_MC_ME                  (1 << 0)

#define RCAR_VIN_MS                     0x0004
 #define RCAR_VIN_MS_FBS_MASK            (3 << 3)
 #define RCAR_VIN_MS_FBS_SHIFT           3
 #define RCAR_VIN_MS_AV                  (1 << 1)
 #define RCAR_VIN_MS_CA                  (1 << 0)

#define RCAR_VIN_FC                     0x0008
 #define RCAR_VIN_FC_C_FRAME             (1 << 1)
 #define RCAR_VIN_FC_S_FRAME             (1 << 0)
#define RCAR_VIN_SLPRC             0x000C
#define RCAR_VIN_ELPRC             0x0010
#define RCAR_VIN_SPPRC             0x0014
#define RCAR_VIN_EPPRC             0x0018
#define RCAR_VIN_CSI_IFMD          0x0020
 #define RCAR_VIN_CSI_IFMD_DES2     (1 << 27) 
 #define RCAR_VIN_CSI_IFMD_DES1     (1 << 26) 
 #define RCAR_VIN_CSI_IFMD_DES0     (1 << 25)
 #define RCAR_VIN_CSI_IFMD_CSI_CHSEL(n)  (n << 0)
#define RCAR_VIN_IS                0x002C
#define RCAR_VIN_LC                0x003C
#define RCAR_VIN_IE                0x0040
 #define RCAR_VIN_IE_FIE            (1 << 4)
 #define RCAR_VIN_IE_EFE            (1 << 1)
 #define RCAR_VIN_IE_FOE            (1 << 0)
#define RCAR_VIN_INTS              0x0044
 #define RCAR_VIN_INTS_FIS          (1 << 4)
 #define RCAR_VIN_INTS_EFS          (1 << 1)
 #define RCAR_VIN_INTS_FOS          (1 << 0)
#define RCAR_VIN_SI                0x0048
#define RCAR_VIN_DMR               0x0058
 #define RCAR_VIN_DMR_EXRGB                (1 << 8)
 #define RCAR_VIN_DMR_BPSM                 (1 << 4)
 #define RCAR_VIN_DMR_ABIT                 (1 << 2)
 #define RCAR_VIN_DMR_DTMD_YCSEP           (1 << 1)
 #define RCAR_VIN_DMR_DTMD_ARGB            (1 << 0)
 #define RCAR_VIN_DMR_A8BIT              (0xFF << 24)
 #define RCAR_VIN_DMR_DTMD_YCSEP_YCBCR420  (3 << 0)
#define RCAR_VIN_DMR2              0x005C
 #define RCAR_VIN_DMR2_VPS          (1 << 30)
 #define RCAR_VIN_DMR2_HPS          (1 << 29)
 #define RCAR_VIN_DMR2_FTEV         (1 << 17)
 #define RCAR_VIN_DMR2_VLV(n)       ((n & 0xf) << 12)
#define RCAR_VIN_UVAOF             0x0060
#define RCAR_VIN_UDS_CTRL          0x0080
 #define RCAR_VIN_UDS_CTRL_AMD      (1 << 30)
 #define RCAR_VIN_UDS_CTRL_BC       (1 << 20)
 #define RCAR_VIN_UDS_CTRL_TDIPC    (1 << 1)
#define RCAR_VIN_UDS_SCALE         0x0084
#define RCAR_VIN_UDS_PASS_BW       0x0090
#define RCAR_VIN_UDS_IPC           0x0098
#define RCAR_VIN_UDS_CLIPSIZE      0x00A4
#define RCAR_VIN_LUTP              0x0100
#define RCAR_VIN_LUTD              0x0104
#define RCAR_VIN_MB(x)             0x0030 + 0x04*x
#define RCAR_VIN_CSCC(x)           0x0064 + 0x04*x
#define RCAR_VIN_YCCR(x)           0x0228 + 0x04*x
#define RCAR_VIN_CBCCR(x)          0x0234 + 0x04*x
#define RCAR_VIN_CRCCR(x)          0x0240 + 0x04*x
#define RCAR_VIN_CSCE(x)           0x0300 + 0x04*x

/*
 * Camera Serial Interface 2
 */
#define RCAR_CSI40_BASE             0xFEAA0000
#define RCAR_CSI41_BASE             0xFEAB0000
#define RCAR_CSI42_BASE             0xFED60000
#define RCAR_CSI43_BASE             0xFED70000
#define RCAR_CSI20_BASE             0xFEA80000
#define RCAR_CSI21_BASE             0xFEA90000
#define RCAR_CSI2_SIZE              0x10000

/*  Offset from base */
#define RCAR_CSI2_TREF             0x00
#define RCAR_CSI2_SRST             0x04
#define RCAR_CSI2_PHYCNT           0x08
#define RCAR_CSI2_CHKSUM           0x0C
#define RCAR_CSI2_VCDT             0x10
#define RCAR_CSI2_VCDT2            0x14
#define RCAR_CSI2_FRDT             0x18
#define RCAR_CSI2_FLD              0x1C
#define RCAR_CSI2_ASTBY            0x20
#define RCAR_CSI2_LNGDT0           0x28
#define RCAR_CSI2_LNGDT1           0x2C
#define RCAR_CSI2_INTEN            0x30
#define RCAR_CSI2_INTCLOSE         0x34
#define RCAR_CSI2_INTSTATE         0x38
#define RCAR_CSI2_INTERRSTATE      0x3C
#define RCAR_CSI2_SHPDAT           0x40
#define RCAR_CSI2_SHPCNT           0x44
#define RCAR_CSI2_LINKCNT          0x48
#define RCAR_CSI2_LSWAP            0x4C
#define RCAR_CSI2_PHTW             0x50
#define RCAR_CSI2_PHTC             0x58
#define RCAR_CSI2_PHYPLL           0x68
#define RCAR_CSI2_PHEERM           0x74
#define RCAR_CSI2_PHCLM            0x78
#define RCAR_CSI2_PHDLM            0x7C

/*
 * ISP
 */
#define RCAR_ISP0_BASE              0xFED00000
#define RCAR_ISP0_CORE_BASE         0xFEC00000
#define RCAR_ISP1_BASE              0xFED20000
#define RCAR_ISP1_CORE_BASE         0xFEE00000
#define RCAR_ISP2_BASE              0xFED30000
#define RCAR_ISP2_CORE_BASE         0xFEF00000
#define RCAR_ISP3_BASE              0xFED40000
#define RCAR_ISP3_CORE_BASE         0xFE400000
#define RCAR_ISP_SIZE               0x20000

//Basic control registers 
#define RCAR_ISPVCR				 (0x0000)
#define RCAR_ISPFIFOCTRL		 (0x0004)	
#define RCAR_ISPINPUTSEL0		 (0x0008)	
#define RCAR_ISPSTART			 (0x0014)	
#define RCAR_ISPINT_STATUS		 (0x0040)	
#define RCAR_ISPERR0_STATUS		 (0x0044)	
#define RCAR_ISPERR1_STATUS		 (0x0048)	
#define RCAR_ISPERR2_STATUS		 (0x004C)	
#define RCAR_ISPERR3_STATUS		 (0x0080)	
#define RCAR_ISPERR4_STATUS		 (0x0084)	
#define RCAR_ISPINT_CLEAR		 (0x0050)	
#define RCAR_ISPERR0_CLEAR		 (0x0054)	
#define RCAR_ISPERR1_CLEAR		 (0x0058)	
#define RCAR_ISPERR2_CLEAR		 (0x005C)	
#define RCAR_ISPERR3_CLEAR		 (0x0090)	
#define RCAR_ISPERR4_CLEAR		 (0x0094)
#define RCAR_ISPINT_ENABLE		 (0x0060)
#define RCAR_ISPERR0_ENABLE		 (0x0064)
#define RCAR_ISPERR1_ENABLE		 (0x0068)
#define RCAR_ISPERR2_ENABLE		 (0x006C)
#define RCAR_ISPERR3_ENABLE		 (0x00A0)
#define RCAR_ISPERR4_ENABLE		 (0x00A4)
#define RCAR_ISP_PADDING_CTRL		 (0x00C0)
//Pixel reconstructor register
#define RCAR_ISPPROC_CUSTOM_FORMATn_CTRL(n)			(0x1010 + (4 * n))
#define RCAR_ISP_PROCMODE_DT(n)						(0x1100 + (4 * n))	
#define RCAR_ISPPROC_CUSTOM_FORMAT_LUT0_Wn_B3b(n, b)	(0x8000 + (0x20 * n) + (4 * b))	
//Synchronized system control register 	
#define RCAR_ISPWUP_CONFIG0		 		(0x2000)
#define RCAR_ISPWUP_CONFIG1		 		(0x2004)
#define RCAR_ISPWUP_EOF_CONFIG	 		(0x2008)
#define RCAR_ISPWUP_STATUS		 		(0x200C)
#define RCAR_ISPWUP_EOF_MATCH_ADRESSn(n) 	(0x2100 + (4 * n))
#define RCAR_ISPWUP_EOF_MATCH_IDn(n) 		(0x2200 + (4 * n))
//Pixel count checker control register
#define RCAR_PIXEL_COUNT_MAX_CHn(n)		(0x4000 + (0x10 * n))
#define RCAR_PIXEL_COUNT_MIN_CHn(n)		(0x4004 + (0x10 * n))
#define RCAR_PIXEL_COUNT_MONITOR_CHn(n)	(0x4008 + (0x10 * n))
//Channel selector register 
#define RCAR_ISPCS_FILTER_ID_CHn(n)			(0x3000 + (0x100 * n))
#define RCAR_ISPCS_LC_MODULO_CHn(n)			(0x3004 + (0x100 * n))
#define RCAR_ISPCS_DT_CODE03_CHn(n)			(0x3008 + (0x100 * n))	
#define RCAR_ISPCS_DT_CODE47_CHn(n)			(0x300C + (0x100 * n))	
#define RCAR_ISPCS_H_CLIP_DT_CODEm_CHn(m, n)	(0x3020 + (4 * m) + (0x100 * n))	
#define RCAR_ISPCS_V_CLIP_DT_CODEm_CHn(m, n)	(0x3030 + (4 * m) + (0x100 * n))	
#define RCAR_ISPCS_OUTPUT_0TO3_MODE_CHn(n)	(0x0020 + (4 * n))	
#define RCAR_ISPCS_OUTPUT_4to7_MODE_CHn(n)	(0x0120 + (4 * n))	
#define RCAR_ISPCS_DI_FILTER_CTRL_CHn(n)		(0x3040 + (0x100 * n))
#define RCAR_ISPCS_DI_FILTER_LUTp_CHn(p, n)	(0x3080 + (4 * p) + (0x100 * n))	
//Write protect control register
#define RCAR_ISPWP_CTRL			  (0x0100)
//Streaming IP register					
#define RCAR_ISPSTREAMER_MODE		  (0x7000)	
#define RCAR_ISPSTREAMER_V_BLANK	  (0x7004)	
#define RCAR_ISPSTREAMER_H_BLANK	  (0x7008)	
	

/*
* IMR-LX4
*/
#define RCAR_IMRLX40_BASE             0xFE860000
#define RCAR_IMRLX41_BASE             0xFE870000
#define RCAR_IMRLX42_BASE             0xFE880000
#define RCAR_IMRLX43_BASE             0xFE890000
#define RCAR_IMRLX4_SIZE              2048
    
#define RCAR_IMRLX4_PULSE             58
#define RCAR_IMRLX4_END               59
            
//Control registers         
#define RCAR_IMRLX4_CR                0x0008
#define RCAR_IMRLX4_SR                0x000C
#define RCAR_IMRLX4_SRCR              0x0010
#define RCAR_IMRLX4_ICR               0x0014
#define RCAR_IMRLX4_IMR               0x0018
#define RCAR_IMRLX4_DLSP              0x001C
#define RCAR_IMRLX4_DLPR              0x0020
#define RCAR_IMRLX4_FUSAR             0x0024
#define RCAR_IMRLX4_EDLR              0x0028
//Memory control registers
#define RCAR_IMRLX4_DLSAR             0x0030
#define RCAR_IMRLX4_DSAR              0x0034
#define RCAR_IMRLX4_SSAR              0x0038
#define RCAR_IMRLX4_DSTR              0x003C
#define RCAR_IMRLX4_SSTR              0x0040
#define RCAR_IMRLX4_DSOR              0x0050
//Rendering control registers
#define RCAR_IMRLX4_CMRCR             0x0054
#define RCAR_IMRLX4_CMRCSR            0x0058
#define RCAR_IMRLX4_CMRCCR            0x005C
#define RCAR_IMRLX4_TRIMR             0x0060
#define RCAR_IMRLX4_TRIMSR            0x0064
#define RCAR_IMRLX4_TRIMCR            0x0068
#define RCAR_IMRLX4_TRICR             0x006C
#define RCAR_IMRLX4_UVDPOR            0x0070
#define RCAR_IMRLX4_SUSR              0x0074
#define RCAR_IMRLX4_SVSR              0x0078
#define RCAR_IMRLX4_XMINR             0x0080
#define RCAR_IMRLX4_YMINR             0x0084
#define RCAR_IMRLX4_XMAXR             0x0088
#define RCAR_IMRLX4_YMAXR             0x008c
#define RCAR_IMRLX4_AMXSR             0x0090
#define RCAR_IMRLX4_AMYSR             0x0094
#define RCAR_IMRLX4_AMXOR             0x0098
#define RCAR_IMRLX4_AMYOR             0x009C
#define RCAR_IMRLX4_TRICR2            0x00A0
#define RCAR_IMRLX4_YLMINR            0x00B0
#define RCAR_IMRLX4_UBMINR            0x00B4
#define RCAR_IMRLX4_VRMINR            0x00B8
#define RCAR_IMRLX4_YLMAXR            0x00BC
#define RCAR_IMRLX4_UBMAXR            0x00C0
#define RCAR_IMRLX4_VRMAXR            0x00C4
#define RCAR_IMRLX4_CPDPOR            0x00D0
#define RCAR_IMRLX4_YLCPR             0x00D4
#define RCAR_IMRLX4_UBCPR             0x00D8
#define RCAR_IMRLX4_VRCPR             0x00DC
#define RCAR_IMRLX4_CMRCR2            0x00E4
#define RCAR_IMRLX4_CMRCSR2           0x00E8
#define RCAR_IMRLX4_CMRCCR2           0x00EC
#define RCAR_IMRLX4_LUTDR(n)          (0x1000 | n)
//Display list instruction
#define RCAR_IMRLX4_INST_TRI          0x8A
#define RCAR_IMRLX4_INST_NOP          0x80
#define RCAR_IMRLX4_INST_TRAP         0x8F
#define RCAR_IMRLX4_INST_WTL          0x81
#define RCAR_IMRLX4_INST_WTL2         0x83
#define RCAR_IMRLX4_INST_WTS          0x82
#define RCAR_IMRLX4_INST_INT          0x88
#define RCAR_IMRLX4_INST_SYNCM        0x86
#define RCAR_IMRLX4_INST_GOBSUB       0x8C
#define RCAR_IMRLX4_INST_RET          0x8D

#endif /* __AARCH64_RCAR_GEN3_H_INCLUDED */
