/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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


/*
 * Renesas R-Car processor with ARMv8
 * Gen3 shared definitions: H3, M3, V3, E3, D3
 * Gen4 shared definitions: V3U, S4, V4H 
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

#define IS_RCAR_GEN4(_id)         ((_id == PRODUCT_ID_RCAR_V3U) || (_id == PRODUCT_ID_RCAR_S4) || \
                                   (_id == PRODUCT_ID_RCAR_V4H))

#define RCAR_PRODUCT_REV(x)        (x & 0xFF)
 #define PRODUCT_REV_1_0                0x00
 #define PRODUCT_REV_1_1                0x01
 #define PRODUCT_REV_1_2                0x02
 #define PRODUCT_REV_1_3                0x03 
 #define PRODUCT_REV_2_0                0x10
 #define PRODUCT_REV_2_1                0x11
 #define PRODUCT_REV_2_2                0x12
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
 #define PRODUCT_S4_REV_1_0             0x00
 #define PRODUCT_S4_REV_1_1             0x01
 #define PRODUCT_S4_REV_1_2             0x02
 #define PRODUCT_V4H_REV_1_0            0x00
 #define PRODUCT_V4H_REV_1_1            0x01
 #define PRODUCT_V4H_REV_2_0            0x10
 #define PRODUCT_V4H_REV_2_1            0x11
 #define PRODUCT_V4H_REV_2_2            0x12

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
 * Macros is for Gen4 (V3U, S4, V4H)
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

#define RCAR_V4H_GPIO_GP0                0x00000
#define RCAR_V4H_GPIO_GP1                0x00800
#define RCAR_V4H_GPIO_GP2                0x08000
#define RCAR_V4H_GPIO_GP3                0x08800
#define RCAR_V4H_GPIO_GP4                0x10000
#define RCAR_V4H_GPIO_GP5                0x10800
#define RCAR_V4H_GPIO_GP6                0x11000
#define RCAR_V4H_GPIO_GP7                0x11800
#define RCAR_V4H_GPIO_GP8                0x18000
#define RCAR_V4H_GPIO_SYSGP              0x28000

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
#define RCAR_GEN4_CPG_PLL6CR0             0x084C
#define RCAR_GEN4_CPG_PLL1CR1             0x08B0
#define RCAR_GEN4_CPG_PLL1CR2             0x08B4
#define RCAR_GEN4_CPG_PLL2CR1             0x08B8
#define RCAR_GEN4_CPG_PLL2CR2             0x08BC
#define RCAR_GEN4_CPG_PLL3CR1             0x08C0
#define RCAR_GEN4_CPG_PLL3CR2             0x08C0
#define RCAR_GEN4_CPG_PLL5CR0             0x0848
#define RCAR_GEN4_CPG_PLL5CR1             0x08D0
#define RCAR_GEN4_CPG_PLL5CR2             0x08D4
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
#define RCAR_GEN4_CPG_RAHSR(n)            (0x2D80 + (n * 4))
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
#define RCAR_GEN4_CPG_CLK_S0D3            ( 266666666)
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
 * Reset registers for Gen4 (V3U, S4, V4H)
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
 * System Controller for Gen4 (V3U, S4, V4H)
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
 * GIC-600 for Gen4 (V3U, S4, V4H)
 */
#define RCAR_GEN4_GICD_BASE               0xF1000000
#define RCAR_GEN4_GICA_BASE               0xF1010000
#define RCAR_GEN4_GICT_BASE               0xF1020000
#define RCAR_GEN4_GICP_BASE               0xF1030000
#define RCAR_GEN4_GICS_BASE               0xF1040000
#define RCAR_GEN4_GICR_LPI_BASE           0xF1060000
#define RCAR_GEN4_GICR_SGI_BASE           0xF1070000
#define RCAR_GEN4_GICDA_BASE              0xF1160000

/* Interrupt SPI number */
#define RCAR_GEN4_INTCSYS_SPI_0           (0   + 32)
#define RCAR_GEN4_INTCSYS_SPI_1           (1   + 32)
#define RCAR_GEN4_INTCSYS_SPI_2	          (2   + 32)
#define RCAR_GEN4_INTCSYS_SPI_3	          (3   + 32)
#define RCAR_GEN4_INTCSYS_SPI_4	          (4   + 32)
#define RCAR_GEN4_INTCSYS_SPI_5	          (5   + 32)
#define RCAR_GEN4_INTCSYS_SPI_6	          (6   + 32)
#define RCAR_GEN4_INTCSYS_SPI_7	          (7   + 32)
#define RCAR_GEN4_INTCSYS_SPI_8	          (8   + 32)
#define RCAR_GEN4_INTCSYS_SPI_9	          (9   + 32)
#define RCAR_GEN4_INTCSYS_SPI_10	      (10  + 32)
#define RCAR_GEN4_INTCSYS_SPI_11	      (11  + 32)
#define RCAR_GEN4_INTCSYS_SPI_12	      (12  + 32)
#define RCAR_GEN4_INTCSYS_SPI_13	      (13  + 32)
#define RCAR_GEN4_INTCSYS_SPI_14	      (14  + 32)
#define RCAR_GEN4_INTCSYS_SPI_15	      (15  + 32)
#define RCAR_GEN4_INTCSYS_SPI_16	      (16  + 32)
#define RCAR_GEN4_INTCSYS_SPI_17	      (17  + 32)
#define RCAR_GEN4_INTCSYS_SPI_18	      (18  + 32)
#define RCAR_GEN4_INTCSYS_SPI_19	      (19  + 32)
#define RCAR_GEN4_INTCSYS_SPI_20	      (20  + 32)
#define RCAR_GEN4_INTCSYS_SPI_21	      (21  + 32)
#define RCAR_GEN4_INTCSYS_SPI_22	      (22  + 32)
#define RCAR_GEN4_INTCSYS_SPI_23	      (23  + 32)
#define RCAR_GEN4_INTCSYS_SPI_24	      (24  + 32)
#define RCAR_GEN4_INTCSYS_SPI_25	      (25  + 32)
#define RCAR_GEN4_INTCSYS_SPI_26	      (26  + 32)
#define RCAR_GEN4_INTCSYS_SPI_27	      (27  + 32)
#define RCAR_GEN4_INTCSYS_SPI_28	      (28  + 32)
#define RCAR_GEN4_INTCSYS_SPI_29	      (29  + 32)
#define RCAR_GEN4_INTCSYS_SPI_30	      (30  + 32)
#define RCAR_GEN4_INTCSYS_SPI_31	      (31  + 32)
#define RCAR_GEN4_INTCSYS_SPI_32	      (32  + 32)
#define RCAR_GEN4_INTCSYS_SPI_33	      (33  + 32)
#define RCAR_GEN4_INTCSYS_SPI_34	      (34  + 32)
#define RCAR_GEN4_INTCSYS_SPI_35	      (35  + 32)
#define RCAR_GEN4_INTCSYS_SPI_36	      (36  + 32)
#define RCAR_GEN4_INTCSYS_SPI_37	      (37  + 32)
#define RCAR_GEN4_INTCSYS_SPI_38	      (38  + 32)
#define RCAR_GEN4_INTCSYS_SPI_39	      (39  + 32)
#define RCAR_GEN4_INTCSYS_SPI_40	      (40  + 32)
#define RCAR_GEN4_INTCSYS_SPI_41	      (41  + 32)
#define RCAR_GEN4_INTCSYS_SPI_42	      (42  + 32)
#define RCAR_GEN4_INTCSYS_SPI_43	      (43  + 32)
#define RCAR_GEN4_INTCSYS_SPI_44	      (44  + 32)
#define RCAR_GEN4_INTCSYS_SPI_45	      (45  + 32)
#define RCAR_GEN4_INTCSYS_SPI_46	      (46  + 32)
#define RCAR_GEN4_INTCSYS_SPI_47	      (47  + 32)
#define RCAR_GEN4_INTCSYS_SPI_48	      (48  + 32)
#define RCAR_GEN4_INTCSYS_SPI_49	      (49  + 32)
#define RCAR_GEN4_INTCSYS_SPI_50	      (50  + 32)
#define RCAR_GEN4_INTCSYS_SPI_51	      (51  + 32)
#define RCAR_GEN4_INTCSYS_SPI_52	      (52  + 32)
#define RCAR_GEN4_INTCSYS_SPI_53	      (53  + 32)
#define RCAR_GEN4_INTCSYS_SPI_54	      (54  + 32)
#define RCAR_GEN4_INTCSYS_SPI_55	      (55  + 32)
#define RCAR_GEN4_INTCSYS_SPI_56	      (56  + 32)
#define RCAR_GEN4_INTCSYS_SPI_57	      (57  + 32)
#define RCAR_GEN4_INTCSYS_SPI_58	      (58  + 32)
#define RCAR_GEN4_INTCSYS_SPI_59	      (59  + 32)
#define RCAR_GEN4_INTCSYS_SPI_60	      (60  + 32)
#define RCAR_GEN4_INTCSYS_SPI_61	      (61  + 32)
#define RCAR_GEN4_INTCSYS_SPI_62	      (62  + 32)
#define RCAR_GEN4_INTCSYS_SPI_63	      (63  + 32)
#define RCAR_GEN4_INTCSYS_SPI_64	      (64  + 32)
#define RCAR_GEN4_INTCSYS_SPI_65	      (65  + 32)
#define RCAR_GEN4_INTCSYS_SPI_66	      (66  + 32)
#define RCAR_GEN4_INTCSYS_SPI_67	      (67  + 32)
#define RCAR_GEN4_INTCSYS_SPI_68	      (68  + 32)
#define RCAR_GEN4_INTCSYS_SPI_69	      (69  + 32)
#define RCAR_GEN4_INTCSYS_SPI_70	      (70  + 32)
#define RCAR_GEN4_INTCSYS_SPI_71	      (71  + 32)
#define RCAR_GEN4_INTCSYS_SPI_72	      (72  + 32)
#define RCAR_GEN4_INTCSYS_SPI_73	      (73  + 32)
#define RCAR_GEN4_INTCSYS_SPI_74	      (74  + 32)
#define RCAR_GEN4_INTCSYS_SPI_75	      (75  + 32)
#define RCAR_GEN4_INTCSYS_SPI_76	      (76  + 32)
#define RCAR_GEN4_INTCSYS_SPI_77	      (77  + 32)
#define RCAR_GEN4_INTCSYS_SPI_78	      (78  + 32)
#define RCAR_GEN4_INTCSYS_SPI_79	      (79  + 32)
#define RCAR_GEN4_INTCSYS_SPI_80	      (80  + 32)
#define RCAR_GEN4_INTCSYS_SPI_81	      (81  + 32)
#define RCAR_GEN4_INTCSYS_SPI_82	      (82  + 32)
#define RCAR_GEN4_INTCSYS_SPI_83	      (83  + 32)
#define RCAR_GEN4_INTCSYS_SPI_84	      (84  + 32)
#define RCAR_GEN4_INTCSYS_SPI_85	      (85  + 32)
#define RCAR_GEN4_INTCSYS_SPI_86	      (86  + 32)
#define RCAR_GEN4_INTCSYS_SPI_87	      (87  + 32)
#define RCAR_GEN4_INTCSYS_SPI_88	      (88  + 32)
#define RCAR_GEN4_INTCSYS_SPI_89	      (89  + 32)
#define RCAR_GEN4_INTCSYS_SPI_90	      (90  + 32)
#define RCAR_GEN4_INTCSYS_SPI_91	      (91  + 32)
#define RCAR_GEN4_INTCSYS_SPI_92	      (92  + 32)
#define RCAR_GEN4_INTCSYS_SPI_93	      (93  + 32)
#define RCAR_GEN4_INTCSYS_SPI_94	      (94  + 32)
#define RCAR_GEN4_INTCSYS_SPI_95	      (95  + 32)
#define RCAR_GEN4_INTCSYS_SPI_96	      (96  + 32)
#define RCAR_GEN4_INTCSYS_SPI_97	      (97  + 32)
#define RCAR_GEN4_INTCSYS_SPI_98	      (98  + 32)
#define RCAR_GEN4_INTCSYS_SPI_99	      (99  + 32)
#define RCAR_GEN4_INTCSYS_SPI_100	      (100 + 32)
#define RCAR_GEN4_INTCSYS_SPI_101	      (101 + 32)
#define RCAR_GEN4_INTCSYS_SPI_102	      (102 + 32)
#define RCAR_GEN4_INTCSYS_SPI_103	      (103 + 32)
#define RCAR_GEN4_INTCSYS_SPI_104	      (104 + 32)
#define RCAR_GEN4_INTCSYS_SPI_105	      (105 + 32)
#define RCAR_GEN4_INTCSYS_SPI_106	      (106 + 32)
#define RCAR_GEN4_INTCSYS_SPI_107	      (107 + 32)
#define RCAR_GEN4_INTCSYS_SPI_108	      (108 + 32)
#define RCAR_GEN4_INTCSYS_SPI_109	      (109 + 32)
#define RCAR_GEN4_INTCSYS_SPI_110	      (110 + 32)
#define RCAR_GEN4_INTCSYS_SPI_111	      (111 + 32)
#define RCAR_GEN4_INTCSYS_SPI_112	      (112 + 32)
#define RCAR_GEN4_INTCSYS_SPI_113	      (113 + 32)
#define RCAR_GEN4_INTCSYS_SPI_114	      (114 + 32)
#define RCAR_GEN4_INTCSYS_SPI_115	      (115 + 32)
#define RCAR_GEN4_INTCSYS_SPI_116	      (116 + 32)
#define RCAR_GEN4_INTCSYS_SPI_117	      (117 + 32)
#define RCAR_GEN4_INTCSYS_SPI_118	      (118 + 32)
#define RCAR_GEN4_INTCSYS_SPI_119	      (119 + 32)
#define RCAR_GEN4_INTCSYS_SPI_120	      (120 + 32)
#define RCAR_GEN4_INTCSYS_SPI_121	      (121 + 32)
#define RCAR_GEN4_INTCSYS_SPI_122	      (122 + 32)
#define RCAR_GEN4_INTCSYS_SPI_123	      (123 + 32)
#define RCAR_GEN4_INTCSYS_SPI_124	      (124 + 32)
#define RCAR_GEN4_INTCSYS_SPI_125	      (125 + 32)
#define RCAR_GEN4_INTCSYS_SPI_126	      (126 + 32)
#define RCAR_GEN4_INTCSYS_SPI_127	      (127 + 32)
#define RCAR_GEN4_INTCSYS_SPI_128	      (128 + 32)
#define RCAR_GEN4_INTCSYS_SPI_129	      (129 + 32)
#define RCAR_GEN4_INTCSYS_SPI_130	      (130 + 32)
#define RCAR_GEN4_INTCSYS_SPI_131	      (131 + 32)
#define RCAR_GEN4_INTCSYS_SPI_132	      (132 + 32)
#define RCAR_GEN4_INTCSYS_SPI_133	      (133 + 32)
#define RCAR_GEN4_INTCSYS_SPI_134	      (134 + 32)
#define RCAR_GEN4_INTCSYS_SPI_135	      (135 + 32)
#define RCAR_GEN4_INTCSYS_SPI_136	      (136 + 32)
#define RCAR_GEN4_INTCSYS_SPI_137	      (137 + 32)
#define RCAR_GEN4_INTCSYS_SPI_138	      (138 + 32)
#define RCAR_GEN4_INTCSYS_SPI_139	      (139 + 32)
#define RCAR_GEN4_INTCSYS_SPI_140	      (140 + 32)
#define RCAR_GEN4_INTCSYS_SPI_141	      (141 + 32)
#define RCAR_GEN4_INTCSYS_SPI_142	      (142 + 32)
#define RCAR_GEN4_INTCSYS_SPI_143	      (143 + 32)
#define RCAR_GEN4_INTCSYS_SPI_144	      (144 + 32)
#define RCAR_GEN4_INTCSYS_SPI_145	      (145 + 32)
#define RCAR_GEN4_INTCSYS_SPI_146	      (146 + 32)
#define RCAR_GEN4_INTCSYS_SPI_147	      (147 + 32)
#define RCAR_GEN4_INTCSYS_SPI_148	      (148 + 32)
#define RCAR_GEN4_INTCSYS_SPI_149	      (149 + 32)
#define RCAR_GEN4_INTCSYS_SPI_150	      (150 + 32)
#define RCAR_GEN4_INTCSYS_SPI_151	      (151 + 32)
#define RCAR_GEN4_INTCSYS_SPI_152	      (152 + 32)
#define RCAR_GEN4_INTCSYS_SPI_153	      (153 + 32)
#define RCAR_GEN4_INTCSYS_SPI_154	      (154 + 32)
#define RCAR_GEN4_INTCSYS_SPI_155	      (155 + 32)
#define RCAR_GEN4_INTCSYS_SPI_156	      (156 + 32)
#define RCAR_GEN4_INTCSYS_SPI_157	      (157 + 32)
#define RCAR_GEN4_INTCSYS_SPI_158	      (158 + 32)
#define RCAR_GEN4_INTCSYS_SPI_159	      (159 + 32)
#define RCAR_GEN4_INTCSYS_SPI_160	      (160 + 32)
#define RCAR_GEN4_INTCSYS_SPI_161	      (161 + 32)
#define RCAR_GEN4_INTCSYS_SPI_162	      (162 + 32)
#define RCAR_GEN4_INTCSYS_SPI_163	      (163 + 32)
#define RCAR_GEN4_INTCSYS_SPI_164	      (164 + 32)
#define RCAR_GEN4_INTCSYS_SPI_165	      (165 + 32)
#define RCAR_GEN4_INTCSYS_SPI_166	      (166 + 32)
#define RCAR_GEN4_INTCSYS_SPI_167	      (167 + 32)
#define RCAR_GEN4_INTCSYS_SPI_168	      (168 + 32)
#define RCAR_GEN4_INTCSYS_SPI_169	      (169 + 32)
#define RCAR_GEN4_INTCSYS_SPI_170	      (170 + 32)
#define RCAR_GEN4_INTCSYS_SPI_171	      (171 + 32)
#define RCAR_GEN4_INTCSYS_SPI_172	      (172 + 32)
#define RCAR_GEN4_INTCSYS_SPI_173	      (173 + 32)
#define RCAR_GEN4_INTCSYS_SPI_174	      (174 + 32)
#define RCAR_GEN4_INTCSYS_SPI_175	      (175 + 32)
#define RCAR_GEN4_INTCSYS_SPI_176	      (176 + 32)
#define RCAR_GEN4_INTCSYS_SPI_177	      (177 + 32)
#define RCAR_GEN4_INTCSYS_SPI_178	      (178 + 32)
#define RCAR_GEN4_INTCSYS_SPI_179	      (179 + 32)
#define RCAR_GEN4_INTCSYS_SPI_180	      (180 + 32)
#define RCAR_GEN4_INTCSYS_SPI_181	      (181 + 32)
#define RCAR_GEN4_INTCSYS_SPI_182	      (182 + 32)
#define RCAR_GEN4_INTCSYS_SPI_183	      (183 + 32)
#define RCAR_GEN4_INTCSYS_SPI_184	      (184 + 32)
#define RCAR_GEN4_INTCSYS_SPI_185	      (185 + 32)
#define RCAR_GEN4_INTCSYS_SPI_186	      (186 + 32)
#define RCAR_GEN4_INTCSYS_SPI_187	      (187 + 32)
#define RCAR_GEN4_INTCSYS_SPI_188	      (188 + 32)
#define RCAR_GEN4_INTCSYS_SPI_189	      (189 + 32)
#define RCAR_GEN4_INTCSYS_SPI_190	      (190 + 32)
#define RCAR_GEN4_INTCSYS_SPI_191	      (191 + 32)
#define RCAR_GEN4_INTCSYS_SPI_192	      (192 + 32)
#define RCAR_GEN4_INTCSYS_SPI_193	      (193 + 32)
#define RCAR_GEN4_INTCSYS_SPI_194	      (194 + 32)
#define RCAR_GEN4_INTCSYS_SPI_195	      (195 + 32)
#define RCAR_GEN4_INTCSYS_SPI_196	      (196 + 32)
#define RCAR_GEN4_INTCSYS_SPI_197	      (197 + 32)
#define RCAR_GEN4_INTCSYS_SPI_198	      (198 + 32)
#define RCAR_GEN4_INTCSYS_SPI_199	      (199 + 32)
#define RCAR_GEN4_INTCSYS_SPI_200	      (200 + 32)
#define RCAR_GEN4_INTCSYS_SPI_201	      (201 + 32)
#define RCAR_GEN4_INTCSYS_SPI_202	      (202 + 32)
#define RCAR_GEN4_INTCSYS_SPI_203	      (203 + 32)
#define RCAR_GEN4_INTCSYS_SPI_204	      (204 + 32)
#define RCAR_GEN4_INTCSYS_SPI_205	      (205 + 32)
#define RCAR_GEN4_INTCSYS_SPI_206	      (206 + 32)
#define RCAR_GEN4_INTCSYS_SPI_207	      (207 + 32)
#define RCAR_GEN4_INTCSYS_SPI_208	      (208 + 32)
#define RCAR_GEN4_INTCSYS_SPI_209	      (209 + 32)
#define RCAR_GEN4_INTCSYS_SPI_210	      (210 + 32)
#define RCAR_GEN4_INTCSYS_SPI_211	      (211 + 32)
#define RCAR_GEN4_INTCSYS_SPI_212	      (212 + 32)
#define RCAR_GEN4_INTCSYS_SPI_213	      (213 + 32)
#define RCAR_GEN4_INTCSYS_SPI_214	      (214 + 32)
#define RCAR_GEN4_INTCSYS_SPI_215	      (215 + 32)
#define RCAR_GEN4_INTCSYS_SPI_216	      (216 + 32)
#define RCAR_GEN4_INTCSYS_SPI_217	      (217 + 32)
#define RCAR_GEN4_INTCSYS_SPI_218	      (218 + 32)
#define RCAR_GEN4_INTCSYS_SPI_219	      (219 + 32)
#define RCAR_GEN4_INTCSYS_SPI_220	      (220 + 32)
#define RCAR_GEN4_INTCSYS_SPI_221	      (221 + 32)
#define RCAR_GEN4_INTCSYS_SPI_222	      (222 + 32)
#define RCAR_GEN4_INTCSYS_SPI_223	      (223 + 32)
#define RCAR_GEN4_INTCSYS_SPI_224	      (224 + 32)
#define RCAR_GEN4_INTCSYS_SPI_225	      (225 + 32)
#define RCAR_GEN4_INTCSYS_SPI_226	      (226 + 32)
#define RCAR_GEN4_INTCSYS_SPI_227	      (227 + 32)
#define RCAR_GEN4_INTCSYS_SPI_228	      (228 + 32)
#define RCAR_GEN4_INTCSYS_SPI_229	      (229 + 32)
#define RCAR_GEN4_INTCSYS_SPI_230	      (230 + 32)
#define RCAR_GEN4_INTCSYS_SPI_231	      (231 + 32)
#define RCAR_GEN4_INTCSYS_SPI_232	      (232 + 32)
#define RCAR_GEN4_INTCSYS_SPI_233	      (233 + 32)
#define RCAR_GEN4_INTCSYS_SPI_234	      (234 + 32)
#define RCAR_GEN4_INTCSYS_SPI_235	      (235 + 32)
#define RCAR_GEN4_INTCSYS_SPI_236	      (236 + 32)
#define RCAR_GEN4_INTCSYS_SPI_237	      (237 + 32)
#define RCAR_GEN4_INTCSYS_SPI_238	      (238 + 32)
#define RCAR_GEN4_INTCSYS_SPI_239	      (239 + 32)
#define RCAR_GEN4_INTCSYS_SPI_240	      (240 + 32)
#define RCAR_GEN4_INTCSYS_SPI_241	      (241 + 32)
#define RCAR_GEN4_INTCSYS_SPI_242	      (242 + 32)
#define RCAR_GEN4_INTCSYS_SPI_243	      (243 + 32)
#define RCAR_GEN4_INTCSYS_SPI_244	      (244 + 32)
#define RCAR_GEN4_INTCSYS_SPI_245	      (245 + 32)
#define RCAR_GEN4_INTCSYS_SPI_246	      (246 + 32)
#define RCAR_GEN4_INTCSYS_SPI_247	      (247 + 32)
#define RCAR_GEN4_INTCSYS_SPI_248	      (248 + 32)
#define RCAR_GEN4_INTCSYS_SPI_249	      (249 + 32)
#define RCAR_GEN4_INTCSYS_SPI_250	      (250 + 32)
#define RCAR_GEN4_INTCSYS_SPI_251	      (251 + 32)
#define RCAR_GEN4_INTCSYS_SPI_252	      (252 + 32)
#define RCAR_GEN4_INTCSYS_SPI_253	      (253 + 32)
#define RCAR_GEN4_INTCSYS_SPI_254	      (254 + 32)
#define RCAR_GEN4_INTCSYS_SPI_255	      (255 + 32)
#define RCAR_GEN4_INTCSYS_SPI_256	      (256 + 32)
#define RCAR_GEN4_INTCSYS_SPI_257	      (257 + 32)
#define RCAR_GEN4_INTCSYS_SPI_258	      (258 + 32)
#define RCAR_GEN4_INTCSYS_SPI_259	      (259 + 32)
#define RCAR_GEN4_INTCSYS_SPI_260	      (260 + 32)
#define RCAR_GEN4_INTCSYS_SPI_261	      (261 + 32)
#define RCAR_GEN4_INTCSYS_SPI_262	      (262 + 32)
#define RCAR_GEN4_INTCSYS_SPI_263	      (263 + 32)
#define RCAR_GEN4_INTCSYS_SPI_264	      (264 + 32)
#define RCAR_GEN4_INTCSYS_SPI_265	      (265 + 32)
#define RCAR_GEN4_INTCSYS_SPI_266	      (266 + 32)
#define RCAR_GEN4_INTCSYS_SPI_267	      (267 + 32)
#define RCAR_GEN4_INTCSYS_SPI_268	      (268 + 32)
#define RCAR_GEN4_INTCSYS_SPI_269	      (269 + 32)
#define RCAR_GEN4_INTCSYS_SPI_270	      (270 + 32)
#define RCAR_GEN4_INTCSYS_SPI_271	      (271 + 32)
#define RCAR_GEN4_INTCSYS_SPI_272	      (272 + 32)
#define RCAR_GEN4_INTCSYS_SPI_273	      (273 + 32)
#define RCAR_GEN4_INTCSYS_SPI_274	      (274 + 32)
#define RCAR_GEN4_INTCSYS_SPI_275	      (275 + 32)
#define RCAR_GEN4_INTCSYS_SPI_276	      (276 + 32)
#define RCAR_GEN4_INTCSYS_SPI_277	      (277 + 32)
#define RCAR_GEN4_INTCSYS_SPI_278	      (278 + 32)
#define RCAR_GEN4_INTCSYS_SPI_279	      (279 + 32)
#define RCAR_GEN4_INTCSYS_SPI_280	      (280 + 32)
#define RCAR_GEN4_INTCSYS_SPI_281	      (281 + 32)
#define RCAR_GEN4_INTCSYS_SPI_282	      (282 + 32)
#define RCAR_GEN4_INTCSYS_SPI_283	      (283 + 32)
#define RCAR_GEN4_INTCSYS_SPI_284	      (284 + 32)
#define RCAR_GEN4_INTCSYS_SPI_285	      (285 + 32)
#define RCAR_GEN4_INTCSYS_SPI_286	      (286 + 32)
#define RCAR_GEN4_INTCSYS_SPI_287	      (287 + 32)
#define RCAR_GEN4_INTCSYS_SPI_288	      (288 + 32)
#define RCAR_GEN4_INTCSYS_SPI_289	      (289 + 32)
#define RCAR_GEN4_INTCSYS_SPI_290	      (290 + 32)
#define RCAR_GEN4_INTCSYS_SPI_291	      (291 + 32)
#define RCAR_GEN4_INTCSYS_SPI_292	      (292 + 32)
#define RCAR_GEN4_INTCSYS_SPI_293	      (293 + 32)
#define RCAR_GEN4_INTCSYS_SPI_294	      (294 + 32)
#define RCAR_GEN4_INTCSYS_SPI_295	      (295 + 32)
#define RCAR_GEN4_INTCSYS_SPI_296	      (296 + 32)
#define RCAR_GEN4_INTCSYS_SPI_297	      (297 + 32)
#define RCAR_GEN4_INTCSYS_SPI_298	      (298 + 32)
#define RCAR_GEN4_INTCSYS_SPI_299	      (299 + 32)
#define RCAR_GEN4_INTCSYS_SPI_300	      (300 + 32)
#define RCAR_GEN4_INTCSYS_SPI_301	      (301 + 32)
#define RCAR_GEN4_INTCSYS_SPI_302	      (302 + 32)
#define RCAR_GEN4_INTCSYS_SPI_303	      (303 + 32)
#define RCAR_GEN4_INTCSYS_SPI_304	      (304 + 32)
#define RCAR_GEN4_INTCSYS_SPI_305	      (305 + 32)
#define RCAR_GEN4_INTCSYS_SPI_306	      (306 + 32)
#define RCAR_GEN4_INTCSYS_SPI_307	      (307 + 32)
#define RCAR_GEN4_INTCSYS_SPI_308	      (308 + 32)
#define RCAR_GEN4_INTCSYS_SPI_309	      (309 + 32)
#define RCAR_GEN4_INTCSYS_SPI_310	      (310 + 32)
#define RCAR_GEN4_INTCSYS_SPI_311	      (311 + 32)
#define RCAR_GEN4_INTCSYS_SPI_312	      (312 + 32)
#define RCAR_GEN4_INTCSYS_SPI_313	      (313 + 32)
#define RCAR_GEN4_INTCSYS_SPI_314	      (314 + 32)
#define RCAR_GEN4_INTCSYS_SPI_315	      (315 + 32)
#define RCAR_GEN4_INTCSYS_SPI_316	      (316 + 32)
#define RCAR_GEN4_INTCSYS_SPI_317	      (317 + 32)
#define RCAR_GEN4_INTCSYS_SPI_318	      (318 + 32)
#define RCAR_GEN4_INTCSYS_SPI_319	      (319 + 32)
#define RCAR_GEN4_INTCSYS_SPI_320	      (320 + 32)
#define RCAR_GEN4_INTCSYS_SPI_321	      (321 + 32)
#define RCAR_GEN4_INTCSYS_SPI_322	      (322 + 32)
#define RCAR_GEN4_INTCSYS_SPI_323	      (323 + 32)
#define RCAR_GEN4_INTCSYS_SPI_324	      (324 + 32)
#define RCAR_GEN4_INTCSYS_SPI_325	      (325 + 32)
#define RCAR_GEN4_INTCSYS_SPI_326	      (326 + 32)
#define RCAR_GEN4_INTCSYS_SPI_327	      (327 + 32)
#define RCAR_GEN4_INTCSYS_SPI_328	      (328 + 32)
#define RCAR_GEN4_INTCSYS_SPI_329	      (329 + 32)
#define RCAR_GEN4_INTCSYS_SPI_330	      (330 + 32)
#define RCAR_GEN4_INTCSYS_SPI_331	      (331 + 32)
#define RCAR_GEN4_INTCSYS_SPI_332	      (332 + 32)
#define RCAR_GEN4_INTCSYS_SPI_333	      (333 + 32)
#define RCAR_GEN4_INTCSYS_SPI_334	      (334 + 32)
#define RCAR_GEN4_INTCSYS_SPI_335	      (335 + 32)
#define RCAR_GEN4_INTCSYS_SPI_336	      (336 + 32)
#define RCAR_GEN4_INTCSYS_SPI_337	      (337 + 32)
#define RCAR_GEN4_INTCSYS_SPI_338	      (338 + 32)
#define RCAR_GEN4_INTCSYS_SPI_339	      (339 + 32)
#define RCAR_GEN4_INTCSYS_SPI_340	      (340 + 32)
#define RCAR_GEN4_INTCSYS_SPI_341	      (341 + 32)
#define RCAR_GEN4_INTCSYS_SPI_342	      (342 + 32)
#define RCAR_GEN4_INTCSYS_SPI_343	      (343 + 32)
#define RCAR_GEN4_INTCSYS_SPI_344	      (344 + 32)
#define RCAR_GEN4_INTCSYS_SPI_345	      (345 + 32)
#define RCAR_GEN4_INTCSYS_SPI_346	      (346 + 32)
#define RCAR_GEN4_INTCSYS_SPI_347	      (347 + 32)
#define RCAR_GEN4_INTCSYS_SPI_348	      (348 + 32)
#define RCAR_GEN4_INTCSYS_SPI_349	      (349 + 32)
#define RCAR_GEN4_INTCSYS_SPI_350	      (350 + 32)
#define RCAR_GEN4_INTCSYS_SPI_351	      (351 + 32)
#define RCAR_GEN4_INTCSYS_SPI_352	      (352 + 32)
#define RCAR_GEN4_INTCSYS_SPI_353	      (353 + 32)
#define RCAR_GEN4_INTCSYS_SPI_354	      (354 + 32)
#define RCAR_GEN4_INTCSYS_SPI_355	      (355 + 32)
#define RCAR_GEN4_INTCSYS_SPI_356	      (356 + 32)
#define RCAR_GEN4_INTCSYS_SPI_357	      (357 + 32)
#define RCAR_GEN4_INTCSYS_SPI_358	      (358 + 32)
#define RCAR_GEN4_INTCSYS_SPI_359	      (359 + 32)
#define RCAR_GEN4_INTCSYS_SPI_360	      (360 + 32)
#define RCAR_GEN4_INTCSYS_SPI_361	      (361 + 32)
#define RCAR_GEN4_INTCSYS_SPI_362	      (362 + 32)
#define RCAR_GEN4_INTCSYS_SPI_363	      (363 + 32)
#define RCAR_GEN4_INTCSYS_SPI_364	      (364 + 32)
#define RCAR_GEN4_INTCSYS_SPI_365	      (365 + 32)
#define RCAR_GEN4_INTCSYS_SPI_366	      (366 + 32)
#define RCAR_GEN4_INTCSYS_SPI_367	      (367 + 32)
#define RCAR_GEN4_INTCSYS_SPI_368	      (368 + 32)
#define RCAR_GEN4_INTCSYS_SPI_369	      (369 + 32)
#define RCAR_GEN4_INTCSYS_SPI_370	      (370 + 32)
#define RCAR_GEN4_INTCSYS_SPI_371	      (371 + 32)
#define RCAR_GEN4_INTCSYS_SPI_372	      (372 + 32)
#define RCAR_GEN4_INTCSYS_SPI_373	      (373 + 32)
#define RCAR_GEN4_INTCSYS_SPI_374	      (374 + 32)
#define RCAR_GEN4_INTCSYS_SPI_375	      (375 + 32)
#define RCAR_GEN4_INTCSYS_SPI_376	      (376 + 32)
#define RCAR_GEN4_INTCSYS_SPI_377	      (377 + 32)
#define RCAR_GEN4_INTCSYS_SPI_378	      (378 + 32)
#define RCAR_GEN4_INTCSYS_SPI_379	      (379 + 32)
#define RCAR_GEN4_INTCSYS_SPI_380	      (380 + 32)
#define RCAR_GEN4_INTCSYS_SPI_381	      (381 + 32)
#define RCAR_GEN4_INTCSYS_SPI_382	      (382 + 32)
#define RCAR_GEN4_INTCSYS_SPI_383	      (383 + 32)
#define RCAR_GEN4_INTCSYS_SPI_384	      (384 + 32)
#define RCAR_GEN4_INTCSYS_SPI_385	      (385 + 32)
#define RCAR_GEN4_INTCSYS_SPI_386	      (386 + 32)
#define RCAR_GEN4_INTCSYS_SPI_387	      (387 + 32)
#define RCAR_GEN4_INTCSYS_SPI_388	      (388 + 32)
#define RCAR_GEN4_INTCSYS_SPI_389	      (389 + 32)
#define RCAR_GEN4_INTCSYS_SPI_390	      (390 + 32)
#define RCAR_GEN4_INTCSYS_SPI_391	      (391 + 32)
#define RCAR_GEN4_INTCSYS_SPI_392	      (392 + 32)
#define RCAR_GEN4_INTCSYS_SPI_393	      (393 + 32)
#define RCAR_GEN4_INTCSYS_SPI_394	      (394 + 32)
#define RCAR_GEN4_INTCSYS_SPI_395	      (395 + 32)
#define RCAR_GEN4_INTCSYS_SPI_396	      (396 + 32)
#define RCAR_GEN4_INTCSYS_SPI_397	      (397 + 32)
#define RCAR_GEN4_INTCSYS_SPI_398	      (398 + 32)
#define RCAR_GEN4_INTCSYS_SPI_399	      (399 + 32)
#define RCAR_GEN4_INTCSYS_SPI_400	      (400 + 32)
#define RCAR_GEN4_INTCSYS_SPI_401	      (401 + 32)
#define RCAR_GEN4_INTCSYS_SPI_402	      (402 + 32)
#define RCAR_GEN4_INTCSYS_SPI_403	      (403 + 32)
#define RCAR_GEN4_INTCSYS_SPI_404	      (404 + 32)
#define RCAR_GEN4_INTCSYS_SPI_405	      (405 + 32)
#define RCAR_GEN4_INTCSYS_SPI_406	      (406 + 32)
#define RCAR_GEN4_INTCSYS_SPI_407	      (407 + 32)
#define RCAR_GEN4_INTCSYS_SPI_408	      (408 + 32)
#define RCAR_GEN4_INTCSYS_SPI_409	      (409 + 32)
#define RCAR_GEN4_INTCSYS_SPI_410	      (410 + 32)
#define RCAR_GEN4_INTCSYS_SPI_411	      (411 + 32)
#define RCAR_GEN4_INTCSYS_SPI_412	      (412 + 32)
#define RCAR_GEN4_INTCSYS_SPI_413	      (413 + 32)
#define RCAR_GEN4_INTCSYS_SPI_414	      (414 + 32)
#define RCAR_GEN4_INTCSYS_SPI_415	      (415 + 32)
#define RCAR_GEN4_INTCSYS_SPI_416	      (416 + 32)
#define RCAR_GEN4_INTCSYS_SPI_417	      (417 + 32)
#define RCAR_GEN4_INTCSYS_SPI_418	      (418 + 32)
#define RCAR_GEN4_INTCSYS_SPI_419	      (419 + 32)
#define RCAR_GEN4_INTCSYS_SPI_420	      (420 + 32)
#define RCAR_GEN4_INTCSYS_SPI_421	      (421 + 32)
#define RCAR_GEN4_INTCSYS_SPI_422	      (422 + 32)
#define RCAR_GEN4_INTCSYS_SPI_423	      (423 + 32)
#define RCAR_GEN4_INTCSYS_SPI_424	      (424 + 32)
#define RCAR_GEN4_INTCSYS_SPI_425	      (425 + 32)
#define RCAR_GEN4_INTCSYS_SPI_426	      (426 + 32)
#define RCAR_GEN4_INTCSYS_SPI_427	      (427 + 32)
#define RCAR_GEN4_INTCSYS_SPI_428	      (428 + 32)
#define RCAR_GEN4_INTCSYS_SPI_429	      (429 + 32)
#define RCAR_GEN4_INTCSYS_SPI_430	      (430 + 32)
#define RCAR_GEN4_INTCSYS_SPI_431	      (431 + 32)
#define RCAR_GEN4_INTCSYS_SPI_432	      (432 + 32)
#define RCAR_GEN4_INTCSYS_SPI_433	      (433 + 32)
#define RCAR_GEN4_INTCSYS_SPI_434	      (434 + 32)
#define RCAR_GEN4_INTCSYS_SPI_435	      (435 + 32)
#define RCAR_GEN4_INTCSYS_SPI_436	      (436 + 32)
#define RCAR_GEN4_INTCSYS_SPI_437	      (437 + 32)
#define RCAR_GEN4_INTCSYS_SPI_438	      (438 + 32)
#define RCAR_GEN4_INTCSYS_SPI_439	      (439 + 32)
#define RCAR_GEN4_INTCSYS_SPI_440	      (440 + 32)
#define RCAR_GEN4_INTCSYS_SPI_441	      (441 + 32)
#define RCAR_GEN4_INTCSYS_SPI_442	      (442 + 32)
#define RCAR_GEN4_INTCSYS_SPI_443	      (443 + 32)
#define RCAR_GEN4_INTCSYS_SPI_444	      (444 + 32)
#define RCAR_GEN4_INTCSYS_SPI_445	      (445 + 32)
#define RCAR_GEN4_INTCSYS_SPI_446	      (446 + 32)
#define RCAR_GEN4_INTCSYS_SPI_447	      (447 + 32)
#define RCAR_GEN4_INTCSYS_SPI_448	      (448 + 32)
#define RCAR_GEN4_INTCSYS_SPI_449	      (449 + 32)
#define RCAR_GEN4_INTCSYS_SPI_450	      (450 + 32)
#define RCAR_GEN4_INTCSYS_SPI_451	      (451 + 32)
#define RCAR_GEN4_INTCSYS_SPI_452	      (452 + 32)
#define RCAR_GEN4_INTCSYS_SPI_453	      (453 + 32)
#define RCAR_GEN4_INTCSYS_SPI_454	      (454 + 32)
#define RCAR_GEN4_INTCSYS_SPI_455	      (455 + 32)
#define RCAR_GEN4_INTCSYS_SPI_456	      (456 + 32)
#define RCAR_GEN4_INTCSYS_SPI_457	      (457 + 32)
#define RCAR_GEN4_INTCSYS_SPI_458	      (458 + 32)
#define RCAR_GEN4_INTCSYS_SPI_459	      (459 + 32)
#define RCAR_GEN4_INTCSYS_SPI_460	      (460 + 32)
#define RCAR_GEN4_INTCSYS_SPI_461	      (461 + 32)
#define RCAR_GEN4_INTCSYS_SPI_462	      (462 + 32)
#define RCAR_GEN4_INTCSYS_SPI_463	      (463 + 32)
#define RCAR_GEN4_INTCSYS_SPI_464	      (464 + 32)
#define RCAR_GEN4_INTCSYS_SPI_465	      (465 + 32)
#define RCAR_GEN4_INTCSYS_SPI_466	      (466 + 32)
#define RCAR_GEN4_INTCSYS_SPI_467	      (467 + 32)
#define RCAR_GEN4_INTCSYS_SPI_468	      (468 + 32)
#define RCAR_GEN4_INTCSYS_SPI_469	      (469 + 32)
#define RCAR_GEN4_INTCSYS_SPI_470	      (470 + 32)
#define RCAR_GEN4_INTCSYS_SPI_471	      (471 + 32)
#define RCAR_GEN4_INTCSYS_SPI_472	      (472 + 32)
#define RCAR_GEN4_INTCSYS_SPI_473	      (473 + 32)
#define RCAR_GEN4_INTCSYS_SPI_474	      (474 + 32)
#define RCAR_GEN4_INTCSYS_SPI_475	      (475 + 32)
#define RCAR_GEN4_INTCSYS_SPI_476	      (476 + 32)
#define RCAR_GEN4_INTCSYS_SPI_477	      (477 + 32)
#define RCAR_GEN4_INTCSYS_SPI_478	      (478 + 32)
#define RCAR_GEN4_INTCSYS_SPI_479	      (479 + 32)
#define RCAR_GEN4_INTCSYS_SPI_480	      (480 + 32)
#define RCAR_GEN4_INTCSYS_SPI_481	      (481 + 32)
#define RCAR_GEN4_INTCSYS_SPI_482	      (482 + 32)
#define RCAR_GEN4_INTCSYS_SPI_483	      (483 + 32)
#define RCAR_GEN4_INTCSYS_SPI_484	      (484 + 32)
#define RCAR_GEN4_INTCSYS_SPI_485	      (485 + 32)
#define RCAR_GEN4_INTCSYS_SPI_486	      (486 + 32)
#define RCAR_GEN4_INTCSYS_SPI_487	      (487 + 32)
#define RCAR_GEN4_INTCSYS_SPI_488	      (488 + 32)
#define RCAR_GEN4_INTCSYS_SPI_489	      (489 + 32)
#define RCAR_GEN4_INTCSYS_SPI_490	      (490 + 32)
#define RCAR_GEN4_INTCSYS_SPI_491	      (491 + 32)
#define RCAR_GEN4_INTCSYS_SPI_492	      (492 + 32)
#define RCAR_GEN4_INTCSYS_SPI_493	      (493 + 32)
#define RCAR_GEN4_INTCSYS_SPI_494	      (494 + 32)
#define RCAR_GEN4_INTCSYS_SPI_495	      (495 + 32)
#define RCAR_GEN4_INTCSYS_SPI_496	      (496 + 32)
#define RCAR_GEN4_INTCSYS_SPI_497	      (497 + 32)
#define RCAR_GEN4_INTCSYS_SPI_498	      (498 + 32)
#define RCAR_GEN4_INTCSYS_SPI_499	      (499 + 32)
#define RCAR_GEN4_INTCSYS_SPI_500	      (500 + 32)
#define RCAR_GEN4_INTCSYS_SPI_501	      (501 + 32)
#define RCAR_GEN4_INTCSYS_SPI_502	      (502 + 32)
#define RCAR_GEN4_INTCSYS_SPI_503	      (503 + 32)
#define RCAR_GEN4_INTCSYS_SPI_504	      (504 + 32)
#define RCAR_GEN4_INTCSYS_SPI_505	      (505 + 32)
#define RCAR_GEN4_INTCSYS_SPI_506	      (506 + 32)
#define RCAR_GEN4_INTCSYS_SPI_507	      (507 + 32)
#define RCAR_GEN4_INTCSYS_SPI_508	      (508 + 32)
#define RCAR_GEN4_INTCSYS_SPI_509	      (509 + 32)
#define RCAR_GEN4_INTCSYS_SPI_510	      (510 + 32)
#define RCAR_GEN4_INTCSYS_SPI_511	      (511 + 32)
#define RCAR_GEN4_INTCSYS_SPI_512	      (512 + 32)
#define RCAR_GEN4_INTCSYS_SPI_513	      (513 + 32)
#define RCAR_GEN4_INTCSYS_SPI_514	      (514 + 32)
#define RCAR_GEN4_INTCSYS_SPI_515	      (515 + 32)
#define RCAR_GEN4_INTCSYS_SPI_516	      (516 + 32)
#define RCAR_GEN4_INTCSYS_SPI_517	      (517 + 32)
#define RCAR_GEN4_INTCSYS_SPI_518	      (518 + 32)
#define RCAR_GEN4_INTCSYS_SPI_519	      (519 + 32)
#define RCAR_GEN4_INTCSYS_SPI_520	      (520 + 32)
#define RCAR_GEN4_INTCSYS_SPI_521	      (521 + 32)
#define RCAR_GEN4_INTCSYS_SPI_522	      (522 + 32)
#define RCAR_GEN4_INTCSYS_SPI_523	      (523 + 32)
#define RCAR_GEN4_INTCSYS_SPI_524	      (524 + 32)
#define RCAR_GEN4_INTCSYS_SPI_525	      (525 + 32)
#define RCAR_GEN4_INTCSYS_SPI_526	      (526 + 32)
#define RCAR_GEN4_INTCSYS_SPI_527	      (527 + 32)
#define RCAR_GEN4_INTCSYS_SPI_528	      (528 + 32)
#define RCAR_GEN4_INTCSYS_SPI_529	      (529 + 32)
#define RCAR_GEN4_INTCSYS_SPI_530	      (530 + 32)
#define RCAR_GEN4_INTCSYS_SPI_531	      (531 + 32)
#define RCAR_GEN4_INTCSYS_SPI_532	      (532 + 32)
#define RCAR_GEN4_INTCSYS_SPI_533	      (533 + 32)
#define RCAR_GEN4_INTCSYS_SPI_534	      (534 + 32)
#define RCAR_GEN4_INTCSYS_SPI_535	      (535 + 32)
#define RCAR_GEN4_INTCSYS_SPI_536	      (536 + 32)
#define RCAR_GEN4_INTCSYS_SPI_537	      (537 + 32)
#define RCAR_GEN4_INTCSYS_SPI_538	      (538 + 32)
#define RCAR_GEN4_INTCSYS_SPI_539	      (539 + 32)
#define RCAR_GEN4_INTCSYS_SPI_540	      (540 + 32)
#define RCAR_GEN4_INTCSYS_SPI_541	      (541 + 32)
#define RCAR_GEN4_INTCSYS_SPI_542	      (542 + 32)
#define RCAR_GEN4_INTCSYS_SPI_543	      (543 + 32)
#define RCAR_GEN4_INTCSYS_SPI_544	      (544 + 32)
#define RCAR_GEN4_INTCSYS_SPI_545	      (545 + 32)
#define RCAR_GEN4_INTCSYS_SPI_546	      (546 + 32)
#define RCAR_GEN4_INTCSYS_SPI_547	      (547 + 32)
#define RCAR_GEN4_INTCSYS_SPI_548	      (548 + 32)
#define RCAR_GEN4_INTCSYS_SPI_549	      (549 + 32)
#define RCAR_GEN4_INTCSYS_SPI_550	      (550 + 32)
#define RCAR_GEN4_INTCSYS_SPI_551	      (551 + 32)
#define RCAR_GEN4_INTCSYS_SPI_552	      (552 + 32)
#define RCAR_GEN4_INTCSYS_SPI_553	      (553 + 32)
#define RCAR_GEN4_INTCSYS_SPI_554	      (554 + 32)
#define RCAR_GEN4_INTCSYS_SPI_555	      (555 + 32)
#define RCAR_GEN4_INTCSYS_SPI_556	      (556 + 32)
#define RCAR_GEN4_INTCSYS_SPI_557	      (557 + 32)
#define RCAR_GEN4_INTCSYS_SPI_558	      (558 + 32)
#define RCAR_GEN4_INTCSYS_SPI_559	      (559 + 32)
#define RCAR_GEN4_INTCSYS_SPI_560	      (560 + 32)
#define RCAR_GEN4_INTCSYS_SPI_561	      (561 + 32)
#define RCAR_GEN4_INTCSYS_SPI_562	      (562 + 32)
#define RCAR_GEN4_INTCSYS_SPI_563	      (563 + 32)
#define RCAR_GEN4_INTCSYS_SPI_564	      (564 + 32)
#define RCAR_GEN4_INTCSYS_SPI_565	      (565 + 32)
#define RCAR_GEN4_INTCSYS_SPI_566	      (566 + 32)
#define RCAR_GEN4_INTCSYS_SPI_567	      (567 + 32)
#define RCAR_GEN4_INTCSYS_SPI_568	      (568 + 32)
#define RCAR_GEN4_INTCSYS_SPI_569	      (569 + 32)
#define RCAR_GEN4_INTCSYS_SPI_570	      (570 + 32)
#define RCAR_GEN4_INTCSYS_SPI_571	      (571 + 32)
#define RCAR_GEN4_INTCSYS_SPI_572	      (572 + 32)
#define RCAR_GEN4_INTCSYS_SPI_573	      (573 + 32)
#define RCAR_GEN4_INTCSYS_SPI_574	      (574 + 32)
#define RCAR_GEN4_INTCSYS_SPI_575	      (575 + 32)
#define RCAR_GEN4_INTCSYS_SPI_576	      (576 + 32)
#define RCAR_GEN4_INTCSYS_SPI_577	      (577 + 32)
#define RCAR_GEN4_INTCSYS_SPI_578	      (578 + 32)
#define RCAR_GEN4_INTCSYS_SPI_579	      (579 + 32)
#define RCAR_GEN4_INTCSYS_SPI_580	      (580 + 32)
#define RCAR_GEN4_INTCSYS_SPI_581	      (581 + 32)
#define RCAR_GEN4_INTCSYS_SPI_582	      (582 + 32)
#define RCAR_GEN4_INTCSYS_SPI_583	      (583 + 32)
#define RCAR_GEN4_INTCSYS_SPI_584	      (584 + 32)
#define RCAR_GEN4_INTCSYS_SPI_585	      (585 + 32)
#define RCAR_GEN4_INTCSYS_SPI_586	      (586 + 32)
#define RCAR_GEN4_INTCSYS_SPI_587	      (587 + 32)
#define RCAR_GEN4_INTCSYS_SPI_588	      (588 + 32)
#define RCAR_GEN4_INTCSYS_SPI_589	      (589 + 32)
#define RCAR_GEN4_INTCSYS_SPI_590	      (590 + 32)
#define RCAR_GEN4_INTCSYS_SPI_591	      (591 + 32)
#define RCAR_GEN4_INTCSYS_SPI_592	      (592 + 32)
#define RCAR_GEN4_INTCSYS_SPI_593	      (593 + 32)
#define RCAR_GEN4_INTCSYS_SPI_594	      (594 + 32)
#define RCAR_GEN4_INTCSYS_SPI_595	      (595 + 32)
#define RCAR_GEN4_INTCSYS_SPI_596	      (596 + 32)
#define RCAR_GEN4_INTCSYS_SPI_597	      (597 + 32)
#define RCAR_GEN4_INTCSYS_SPI_598	      (598 + 32)
#define RCAR_GEN4_INTCSYS_SPI_599	      (599 + 32)
#define RCAR_GEN4_INTCSYS_SPI_600	      (600 + 32)
#define RCAR_GEN4_INTCSYS_SPI_601	      (601 + 32)
#define RCAR_GEN4_INTCSYS_SPI_602	      (602 + 32)
#define RCAR_GEN4_INTCSYS_SPI_603	      (603 + 32)
#define RCAR_GEN4_INTCSYS_SPI_604	      (604 + 32)
#define RCAR_GEN4_INTCSYS_SPI_605	      (605 + 32)
#define RCAR_GEN4_INTCSYS_SPI_606	      (606 + 32)
#define RCAR_GEN4_INTCSYS_SPI_607	      (607 + 32)
#define RCAR_GEN4_INTCSYS_SPI_608	      (608 + 32)
#define RCAR_GEN4_INTCSYS_SPI_609	      (609 + 32)
#define RCAR_GEN4_INTCSYS_SPI_610	      (610 + 32)
#define RCAR_GEN4_INTCSYS_SPI_611	      (611 + 32)
#define RCAR_GEN4_INTCSYS_SPI_612	      (612 + 32)
#define RCAR_GEN4_INTCSYS_SPI_613	      (613 + 32)
#define RCAR_GEN4_INTCSYS_SPI_614	      (614 + 32)
#define RCAR_GEN4_INTCSYS_SPI_615	      (615 + 32)
#define RCAR_GEN4_INTCSYS_SPI_616	      (616 + 32)
#define RCAR_GEN4_INTCSYS_SPI_617	      (617 + 32)
#define RCAR_GEN4_INTCSYS_SPI_618	      (618 + 32)
#define RCAR_GEN4_INTCSYS_SPI_619	      (619 + 32)
#define RCAR_GEN4_INTCSYS_SPI_620	      (620 + 32)
#define RCAR_GEN4_INTCSYS_SPI_621	      (621 + 32)
#define RCAR_GEN4_INTCSYS_SPI_622	      (622 + 32)
#define RCAR_GEN4_INTCSYS_SPI_623	      (623 + 32)
#define RCAR_GEN4_INTCSYS_SPI_624	      (624 + 32)
#define RCAR_GEN4_INTCSYS_SPI_625	      (625 + 32)
#define RCAR_GEN4_INTCSYS_SPI_626	      (626 + 32)
#define RCAR_GEN4_INTCSYS_SPI_627	      (627 + 32)
#define RCAR_GEN4_INTCSYS_SPI_628	      (628 + 32)
#define RCAR_GEN4_INTCSYS_SPI_629	      (629 + 32)
#define RCAR_GEN4_INTCSYS_SPI_630	      (630 + 32)
#define RCAR_GEN4_INTCSYS_SPI_631	      (631 + 32)
#define RCAR_GEN4_INTCSYS_SPI_632	      (632 + 32)
#define RCAR_GEN4_INTCSYS_SPI_633	      (633 + 32)
#define RCAR_GEN4_INTCSYS_SPI_634	      (634 + 32)
#define RCAR_GEN4_INTCSYS_SPI_635	      (635 + 32)
#define RCAR_GEN4_INTCSYS_SPI_636	      (636 + 32)
#define RCAR_GEN4_INTCSYS_SPI_637	      (637 + 32)
#define RCAR_GEN4_INTCSYS_SPI_638	      (638 + 32)
#define RCAR_GEN4_INTCSYS_SPI_639	      (639 + 32)
#define RCAR_GEN4_INTCSYS_SPI_640	      (640 + 32)
#define RCAR_GEN4_INTCSYS_SPI_641	      (641 + 32)
#define RCAR_GEN4_INTCSYS_SPI_642	      (642 + 32)
#define RCAR_GEN4_INTCSYS_SPI_643	      (643 + 32)
#define RCAR_GEN4_INTCSYS_SPI_644	      (644 + 32)
#define RCAR_GEN4_INTCSYS_SPI_645	      (645 + 32)
#define RCAR_GEN4_INTCSYS_SPI_646	      (646 + 32)
#define RCAR_GEN4_INTCSYS_SPI_647	      (647 + 32)
#define RCAR_GEN4_INTCSYS_SPI_648	      (648 + 32)
#define RCAR_GEN4_INTCSYS_SPI_649	      (649 + 32)
#define RCAR_GEN4_INTCSYS_SPI_650	      (650 + 32)
#define RCAR_GEN4_INTCSYS_SPI_651	      (651 + 32)
#define RCAR_GEN4_INTCSYS_SPI_652	      (652 + 32)
#define RCAR_GEN4_INTCSYS_SPI_653	      (653 + 32)
#define RCAR_GEN4_INTCSYS_SPI_654	      (654 + 32)
#define RCAR_GEN4_INTCSYS_SPI_655	      (655 + 32)
#define RCAR_GEN4_INTCSYS_SPI_656	      (656 + 32)
#define RCAR_GEN4_INTCSYS_SPI_657	      (657 + 32)
#define RCAR_GEN4_INTCSYS_SPI_658	      (658 + 32)
#define RCAR_GEN4_INTCSYS_SPI_659	      (659 + 32)
#define RCAR_GEN4_INTCSYS_SPI_660	      (660 + 32)
#define RCAR_GEN4_INTCSYS_SPI_661	      (661 + 32)
#define RCAR_GEN4_INTCSYS_SPI_662	      (662 + 32)
#define RCAR_GEN4_INTCSYS_SPI_663	      (663 + 32)
#define RCAR_GEN4_INTCSYS_SPI_664	      (664 + 32)
#define RCAR_GEN4_INTCSYS_SPI_665	      (665 + 32)
#define RCAR_GEN4_INTCSYS_SPI_666	      (666 + 32)
#define RCAR_GEN4_INTCSYS_SPI_667	      (667 + 32)
#define RCAR_GEN4_INTCSYS_SPI_668	      (668 + 32)
#define RCAR_GEN4_INTCSYS_SPI_669	      (669 + 32)
#define RCAR_GEN4_INTCSYS_SPI_670	      (670 + 32)
#define RCAR_GEN4_INTCSYS_SPI_671	      (671 + 32)
#define RCAR_GEN4_INTCSYS_SPI_672	      (672 + 32)
#define RCAR_GEN4_INTCSYS_SPI_673	      (673 + 32)
#define RCAR_GEN4_INTCSYS_SPI_674	      (674 + 32)
#define RCAR_GEN4_INTCSYS_SPI_675	      (675 + 32)
#define RCAR_GEN4_INTCSYS_SPI_676	      (676 + 32)
#define RCAR_GEN4_INTCSYS_SPI_677	      (677 + 32)
#define RCAR_GEN4_INTCSYS_SPI_678	      (678 + 32)
#define RCAR_GEN4_INTCSYS_SPI_679	      (679 + 32)
#define RCAR_GEN4_INTCSYS_SPI_680	      (680 + 32)
#define RCAR_GEN4_INTCSYS_SPI_681	      (681 + 32)
#define RCAR_GEN4_INTCSYS_SPI_682	      (682 + 32)
#define RCAR_GEN4_INTCSYS_SPI_683	      (683 + 32)
#define RCAR_GEN4_INTCSYS_SPI_684	      (684 + 32)
#define RCAR_GEN4_INTCSYS_SPI_685	      (685 + 32)
#define RCAR_GEN4_INTCSYS_SPI_686	      (686 + 32)
#define RCAR_GEN4_INTCSYS_SPI_687	      (687 + 32)
#define RCAR_GEN4_INTCSYS_SPI_688	      (688 + 32)
#define RCAR_GEN4_INTCSYS_SPI_689	      (689 + 32)
#define RCAR_GEN4_INTCSYS_SPI_690	      (690 + 32)
#define RCAR_GEN4_INTCSYS_SPI_691	      (691 + 32)
#define RCAR_GEN4_INTCSYS_SPI_692	      (692 + 32)
#define RCAR_GEN4_INTCSYS_SPI_693	      (693 + 32)
#define RCAR_GEN4_INTCSYS_SPI_694	      (694 + 32)
#define RCAR_GEN4_INTCSYS_SPI_695	      (695 + 32)
#define RCAR_GEN4_INTCSYS_SPI_696	      (696 + 32)
#define RCAR_GEN4_INTCSYS_SPI_697	      (697 + 32)
#define RCAR_GEN4_INTCSYS_SPI_698	      (698 + 32)
#define RCAR_GEN4_INTCSYS_SPI_699	      (699 + 32)
#define RCAR_GEN4_INTCSYS_SPI_700	      (700 + 32)
#define RCAR_GEN4_INTCSYS_SPI_701	      (701 + 32)
#define RCAR_GEN4_INTCSYS_SPI_702	      (702 + 32)
#define RCAR_GEN4_INTCSYS_SPI_703	      (703 + 32)
#define RCAR_GEN4_INTCSYS_SPI_704	      (704 + 32)
#define RCAR_GEN4_INTCSYS_SPI_705	      (705 + 32)
#define RCAR_GEN4_INTCSYS_SPI_706	      (706 + 32)
#define RCAR_GEN4_INTCSYS_SPI_707	      (707 + 32)
#define RCAR_GEN4_INTCSYS_SPI_708	      (708 + 32)
#define RCAR_GEN4_INTCSYS_SPI_709	      (709 + 32)
#define RCAR_GEN4_INTCSYS_SPI_710	      (710 + 32)
#define RCAR_GEN4_INTCSYS_SPI_711	      (711 + 32)
#define RCAR_GEN4_INTCSYS_SPI_712	      (712 + 32)
#define RCAR_GEN4_INTCSYS_SPI_713	      (713 + 32)
#define RCAR_GEN4_INTCSYS_SPI_714	      (714 + 32)
#define RCAR_GEN4_INTCSYS_SPI_715	      (715 + 32)
#define RCAR_GEN4_INTCSYS_SPI_716	      (716 + 32)
#define RCAR_GEN4_INTCSYS_SPI_717	      (717 + 32)
#define RCAR_GEN4_INTCSYS_SPI_718	      (718 + 32)
#define RCAR_GEN4_INTCSYS_SPI_719	      (719 + 32)
#define RCAR_GEN4_INTCSYS_SPI_720	      (720 + 32)
#define RCAR_GEN4_INTCSYS_SPI_721	      (721 + 32)
#define RCAR_GEN4_INTCSYS_SPI_722	      (722 + 32)
#define RCAR_GEN4_INTCSYS_SPI_723	      (723 + 32)
#define RCAR_GEN4_INTCSYS_SPI_724	      (724 + 32)
#define RCAR_GEN4_INTCSYS_SPI_725	      (725 + 32)
#define RCAR_GEN4_INTCSYS_SPI_726	      (726 + 32)
#define RCAR_GEN4_INTCSYS_SPI_727	      (727 + 32)
#define RCAR_GEN4_INTCSYS_SPI_728	      (728 + 32)
#define RCAR_GEN4_INTCSYS_SPI_729	      (729 + 32)
#define RCAR_GEN4_INTCSYS_SPI_730	      (730 + 32)
#define RCAR_GEN4_INTCSYS_SPI_731	      (731 + 32)
#define RCAR_GEN4_INTCSYS_SPI_732	      (732 + 32)
#define RCAR_GEN4_INTCSYS_SPI_733	      (733 + 32)
#define RCAR_GEN4_INTCSYS_SPI_734	      (734 + 32)
#define RCAR_GEN4_INTCSYS_SPI_735	      (735 + 32)
#define RCAR_GEN4_INTCSYS_SPI_736	      (736 + 32)
#define RCAR_GEN4_INTCSYS_SPI_737	      (737 + 32)
#define RCAR_GEN4_INTCSYS_SPI_738	      (738 + 32)
#define RCAR_GEN4_INTCSYS_SPI_739	      (739 + 32)
#define RCAR_GEN4_INTCSYS_SPI_740	      (740 + 32)
#define RCAR_GEN4_INTCSYS_SPI_741	      (741 + 32)
#define RCAR_GEN4_INTCSYS_SPI_742	      (742 + 32)
#define RCAR_GEN4_INTCSYS_SPI_743	      (743 + 32)
#define RCAR_GEN4_INTCSYS_SPI_744	      (744 + 32)
#define RCAR_GEN4_INTCSYS_SPI_745	      (745 + 32)
#define RCAR_GEN4_INTCSYS_SPI_746	      (746 + 32)
#define RCAR_GEN4_INTCSYS_SPI_747	      (747 + 32)
#define RCAR_GEN4_INTCSYS_SPI_748	      (748 + 32)
#define RCAR_GEN4_INTCSYS_SPI_749	      (749 + 32)
#define RCAR_GEN4_INTCSYS_SPI_750	      (750 + 32)
#define RCAR_GEN4_INTCSYS_SPI_751	      (751 + 32)
#define RCAR_GEN4_INTCSYS_SPI_752	      (752 + 32)
#define RCAR_GEN4_INTCSYS_SPI_753	      (753 + 32)
#define RCAR_GEN4_INTCSYS_SPI_754	      (754 + 32)
#define RCAR_GEN4_INTCSYS_SPI_755	      (755 + 32)
#define RCAR_GEN4_INTCSYS_SPI_756	      (756 + 32)
#define RCAR_GEN4_INTCSYS_SPI_757	      (757 + 32)
#define RCAR_GEN4_INTCSYS_SPI_758	      (758 + 32)
#define RCAR_GEN4_INTCSYS_SPI_759	      (759 + 32)
#define RCAR_GEN4_INTCSYS_SPI_760	      (760 + 32)
#define RCAR_GEN4_INTCSYS_SPI_761	      (761 + 32)
#define RCAR_GEN4_INTCSYS_SPI_762	      (762 + 32)
#define RCAR_GEN4_INTCSYS_SPI_763	      (763 + 32)
#define RCAR_GEN4_INTCSYS_SPI_764	      (764 + 32)
#define RCAR_GEN4_INTCSYS_SPI_765	      (765 + 32)
#define RCAR_GEN4_INTCSYS_SPI_766	      (766 + 32)
#define RCAR_GEN4_INTCSYS_SPI_767	      (767 + 32)
#define RCAR_GEN4_INTCSYS_SPI_768	      (768 + 32)
#define RCAR_GEN4_INTCSYS_SPI_769	      (769 + 32)
#define RCAR_GEN4_INTCSYS_SPI_770	      (770 + 32)
#define RCAR_GEN4_INTCSYS_SPI_771	      (771 + 32)
#define RCAR_GEN4_INTCSYS_SPI_772	      (772 + 32)
#define RCAR_GEN4_INTCSYS_SPI_773	      (773 + 32)
#define RCAR_GEN4_INTCSYS_SPI_774	      (774 + 32)
#define RCAR_GEN4_INTCSYS_SPI_775	      (775 + 32)
#define RCAR_GEN4_INTCSYS_SPI_776	      (776 + 32)
#define RCAR_GEN4_INTCSYS_SPI_777	      (777 + 32)
#define RCAR_GEN4_INTCSYS_SPI_778	      (778 + 32)
#define RCAR_GEN4_INTCSYS_SPI_779	      (779 + 32)
#define RCAR_GEN4_INTCSYS_SPI_780	      (780 + 32)
#define RCAR_GEN4_INTCSYS_SPI_781	      (781 + 32)
#define RCAR_GEN4_INTCSYS_SPI_782	      (782 + 32)
#define RCAR_GEN4_INTCSYS_SPI_783	      (783 + 32)
#define RCAR_GEN4_INTCSYS_SPI_784	      (784 + 32)
#define RCAR_GEN4_INTCSYS_SPI_785	      (785 + 32)
#define RCAR_GEN4_INTCSYS_SPI_786	      (786 + 32)
#define RCAR_GEN4_INTCSYS_SPI_787	      (787 + 32)
#define RCAR_GEN4_INTCSYS_SPI_788	      (788 + 32)
#define RCAR_GEN4_INTCSYS_SPI_789	      (789 + 32)
#define RCAR_GEN4_INTCSYS_SPI_790	      (790 + 32)
#define RCAR_GEN4_INTCSYS_SPI_791	      (791 + 32)
#define RCAR_GEN4_INTCSYS_SPI_792	      (792 + 32)
#define RCAR_GEN4_INTCSYS_SPI_793	      (793 + 32)
#define RCAR_GEN4_INTCSYS_SPI_794	      (794 + 32)
#define RCAR_GEN4_INTCSYS_SPI_795	      (795 + 32)
#define RCAR_GEN4_INTCSYS_SPI_796	      (796 + 32)
#define RCAR_GEN4_INTCSYS_SPI_797	      (797 + 32)
#define RCAR_GEN4_INTCSYS_SPI_798	      (798 + 32)
#define RCAR_GEN4_INTCSYS_SPI_799	      (799 + 32)
#define RCAR_GEN4_INTCSYS_SPI_800	      (800 + 32)
#define RCAR_GEN4_INTCSYS_SPI_801	      (801 + 32)
#define RCAR_GEN4_INTCSYS_SPI_802	      (802 + 32)
#define RCAR_GEN4_INTCSYS_SPI_803	      (803 + 32)
#define RCAR_GEN4_INTCSYS_SPI_804	      (804 + 32)
#define RCAR_GEN4_INTCSYS_SPI_805	      (805 + 32)
#define RCAR_GEN4_INTCSYS_SPI_806	      (806 + 32)
#define RCAR_GEN4_INTCSYS_SPI_807	      (807 + 32)
#define RCAR_GEN4_INTCSYS_SPI_808	      (808 + 32)
#define RCAR_GEN4_INTCSYS_SPI_809	      (809 + 32)
#define RCAR_GEN4_INTCSYS_SPI_810	      (810 + 32)
#define RCAR_GEN4_INTCSYS_SPI_811	      (811 + 32)
#define RCAR_GEN4_INTCSYS_SPI_812	      (812 + 32)
#define RCAR_GEN4_INTCSYS_SPI_813	      (813 + 32)
#define RCAR_GEN4_INTCSYS_SPI_814	      (814 + 32)
#define RCAR_GEN4_INTCSYS_SPI_815	      (815 + 32)
#define RCAR_GEN4_INTCSYS_SPI_816	      (816 + 32)
#define RCAR_GEN4_INTCSYS_SPI_817	      (817 + 32)
#define RCAR_GEN4_INTCSYS_SPI_818	      (818 + 32)
#define RCAR_GEN4_INTCSYS_SPI_819	      (819 + 32)
#define RCAR_GEN4_INTCSYS_SPI_820	      (820 + 32)
#define RCAR_GEN4_INTCSYS_SPI_821	      (821 + 32)
#define RCAR_GEN4_INTCSYS_SPI_822	      (822 + 32)
#define RCAR_GEN4_INTCSYS_SPI_823	      (823 + 32)
#define RCAR_GEN4_INTCSYS_SPI_824	      (824 + 32)
#define RCAR_GEN4_INTCSYS_SPI_825	      (825 + 32)
#define RCAR_GEN4_INTCSYS_SPI_826	      (826 + 32)
#define RCAR_GEN4_INTCSYS_SPI_827	      (827 + 32)
#define RCAR_GEN4_INTCSYS_SPI_828	      (828 + 32)
#define RCAR_GEN4_INTCSYS_SPI_829	      (829 + 32)
#define RCAR_GEN4_INTCSYS_SPI_830	      (830 + 32)
#define RCAR_GEN4_INTCSYS_SPI_831	      (831 + 32)
#define RCAR_GEN4_INTCSYS_SPI_832	      (832 + 32)
#define RCAR_GEN4_INTCSYS_SPI_833	      (833 + 32)
#define RCAR_GEN4_INTCSYS_SPI_834	      (834 + 32)
#define RCAR_GEN4_INTCSYS_SPI_835	      (835 + 32)
#define RCAR_GEN4_INTCSYS_SPI_836	      (836 + 32)
#define RCAR_GEN4_INTCSYS_SPI_837	      (837 + 32)
#define RCAR_GEN4_INTCSYS_SPI_838	      (838 + 32)
#define RCAR_GEN4_INTCSYS_SPI_839	      (839 + 32)
#define RCAR_GEN4_INTCSYS_SPI_840	      (840 + 32)
#define RCAR_GEN4_INTCSYS_SPI_841	      (841 + 32)
#define RCAR_GEN4_INTCSYS_SPI_842	      (842 + 32)
#define RCAR_GEN4_INTCSYS_SPI_843	      (843 + 32)
#define RCAR_GEN4_INTCSYS_SPI_844	      (844 + 32)
#define RCAR_GEN4_INTCSYS_SPI_845	      (845 + 32)
#define RCAR_GEN4_INTCSYS_SPI_846	      (846 + 32)
#define RCAR_GEN4_INTCSYS_SPI_847	      (847 + 32)
#define RCAR_GEN4_INTCSYS_SPI_848	      (848 + 32)
#define RCAR_GEN4_INTCSYS_SPI_849	      (849 + 32)
#define RCAR_GEN4_INTCSYS_SPI_850	      (850 + 32)
#define RCAR_GEN4_INTCSYS_SPI_851	      (851 + 32)
#define RCAR_GEN4_INTCSYS_SPI_852	      (852 + 32)
#define RCAR_GEN4_INTCSYS_SPI_853	      (853 + 32)
#define RCAR_GEN4_INTCSYS_SPI_854	      (854 + 32)
#define RCAR_GEN4_INTCSYS_SPI_855	      (855 + 32)
#define RCAR_GEN4_INTCSYS_SPI_856	      (856 + 32)
#define RCAR_GEN4_INTCSYS_SPI_857	      (857 + 32)
#define RCAR_GEN4_INTCSYS_SPI_858	      (858 + 32)
#define RCAR_GEN4_INTCSYS_SPI_859	      (859 + 32)
#define RCAR_GEN4_INTCSYS_SPI_860	      (860 + 32)
#define RCAR_GEN4_INTCSYS_SPI_861	      (861 + 32)
#define RCAR_GEN4_INTCSYS_SPI_862	      (862 + 32)
#define RCAR_GEN4_INTCSYS_SPI_863	      (863 + 32)
#define RCAR_GEN4_INTCSYS_SPI_864	      (864 + 32)
#define RCAR_GEN4_INTCSYS_SPI_865	      (865 + 32)
#define RCAR_GEN4_INTCSYS_SPI_866	      (866 + 32)
#define RCAR_GEN4_INTCSYS_SPI_867	      (867 + 32)
#define RCAR_GEN4_INTCSYS_SPI_868	      (868 + 32)
#define RCAR_GEN4_INTCSYS_SPI_869	      (869 + 32)
#define RCAR_GEN4_INTCSYS_SPI_870	      (870 + 32)
#define RCAR_GEN4_INTCSYS_SPI_871	      (871 + 32)
#define RCAR_GEN4_INTCSYS_SPI_872	      (872 + 32)
#define RCAR_GEN4_INTCSYS_SPI_873	      (873 + 32)
#define RCAR_GEN4_INTCSYS_SPI_874	      (874 + 32)
#define RCAR_GEN4_INTCSYS_SPI_875	      (875 + 32)
#define RCAR_GEN4_INTCSYS_SPI_876	      (876 + 32)
#define RCAR_GEN4_INTCSYS_SPI_877	      (877 + 32)
#define RCAR_GEN4_INTCSYS_SPI_878	      (878 + 32)
#define RCAR_GEN4_INTCSYS_SPI_879	      (879 + 32)
#define RCAR_GEN4_INTCSYS_SPI_880	      (880 + 32)
#define RCAR_GEN4_INTCSYS_SPI_881	      (881 + 32)
#define RCAR_GEN4_INTCSYS_SPI_882	      (882 + 32)
#define RCAR_GEN4_INTCSYS_SPI_883	      (883 + 32)
#define RCAR_GEN4_INTCSYS_SPI_884	      (884 + 32)
#define RCAR_GEN4_INTCSYS_SPI_885	      (885 + 32)
#define RCAR_GEN4_INTCSYS_SPI_886	      (886 + 32)
#define RCAR_GEN4_INTCSYS_SPI_887	      (887 + 32)
#define RCAR_GEN4_INTCSYS_SPI_888	      (888 + 32)
#define RCAR_GEN4_INTCSYS_SPI_889	      (889 + 32)
#define RCAR_GEN4_INTCSYS_SPI_890	      (890 + 32)
#define RCAR_GEN4_INTCSYS_SPI_891	      (891 + 32)
#define RCAR_GEN4_INTCSYS_SPI_892	      (892 + 32)
#define RCAR_GEN4_INTCSYS_SPI_893	      (893 + 32)
#define RCAR_GEN4_INTCSYS_SPI_894	      (894 + 32)
#define RCAR_GEN4_INTCSYS_SPI_895	      (895 + 32)
#define RCAR_GEN4_INTCSYS_SPI_896	      (896 + 32)
#define RCAR_GEN4_INTCSYS_SPI_897	      (897 + 32)
#define RCAR_GEN4_INTCSYS_SPI_898	      (898 + 32)
#define RCAR_GEN4_INTCSYS_SPI_899	      (899 + 32)
#define RCAR_GEN4_INTCSYS_SPI_900	      (900 + 32)
#define RCAR_GEN4_INTCSYS_SPI_901	      (901 + 32)
#define RCAR_GEN4_INTCSYS_SPI_902	      (902 + 32)
#define RCAR_GEN4_INTCSYS_SPI_903	      (903 + 32)
#define RCAR_GEN4_INTCSYS_SPI_904	      (904 + 32)
#define RCAR_GEN4_INTCSYS_SPI_905	      (905 + 32)
#define RCAR_GEN4_INTCSYS_SPI_906	      (906 + 32)
#define RCAR_GEN4_INTCSYS_SPI_907	      (907 + 32)
#define RCAR_GEN4_INTCSYS_SPI_908	      (908 + 32)
#define RCAR_GEN4_INTCSYS_SPI_909	      (909 + 32)
#define RCAR_GEN4_INTCSYS_SPI_910	      (910 + 32)
#define RCAR_GEN4_INTCSYS_SPI_911	      (911 + 32)
#define RCAR_GEN4_INTCSYS_SPI_912	      (912 + 32)
#define RCAR_GEN4_INTCSYS_SPI_913	      (913 + 32)
#define RCAR_GEN4_INTCSYS_SPI_914	      (914 + 32)
#define RCAR_GEN4_INTCSYS_SPI_915	      (915 + 32)
#define RCAR_GEN4_INTCSYS_SPI_916	      (916 + 32)
#define RCAR_GEN4_INTCSYS_SPI_917	      (917 + 32)
#define RCAR_GEN4_INTCSYS_SPI_918	      (918 + 32)
#define RCAR_GEN4_INTCSYS_SPI_919	      (919 + 32)
#define RCAR_GEN4_INTCSYS_SPI_920	      (920 + 32)
#define RCAR_GEN4_INTCSYS_SPI_921	      (921 + 32)
#define RCAR_GEN4_INTCSYS_SPI_922	      (922 + 32)
#define RCAR_GEN4_INTCSYS_SPI_923	      (923 + 32)
#define RCAR_GEN4_INTCSYS_SPI_924	      (924 + 32)
#define RCAR_GEN4_INTCSYS_SPI_925	      (925 + 32)
#define RCAR_GEN4_INTCSYS_SPI_926	      (926 + 32)
#define RCAR_GEN4_INTCSYS_SPI_927	      (927 + 32)
#define RCAR_GEN4_INTCSYS_SPI_928	      (928 + 32)
#define RCAR_GEN4_INTCSYS_SPI_929	      (929 + 32)
#define RCAR_GEN4_INTCSYS_SPI_930	      (930 + 32)
#define RCAR_GEN4_INTCSYS_SPI_931	      (931 + 32)
#define RCAR_GEN4_INTCSYS_SPI_932	      (932 + 32)
#define RCAR_GEN4_INTCSYS_SPI_933	      (933 + 32)
#define RCAR_GEN4_INTCSYS_SPI_934	      (934 + 32)
#define RCAR_GEN4_INTCSYS_SPI_935	      (935 + 32)
#define RCAR_GEN4_INTCSYS_SPI_936	      (936 + 32)
#define RCAR_GEN4_INTCSYS_SPI_937	      (937 + 32)
#define RCAR_GEN4_INTCSYS_SPI_938	      (938 + 32)
#define RCAR_GEN4_INTCSYS_SPI_939	      (939 + 32)
#define RCAR_GEN4_INTCSYS_SPI_940	      (940 + 32)
#define RCAR_GEN4_INTCSYS_SPI_941	      (941 + 32)
#define RCAR_GEN4_INTCSYS_SPI_942	      (942 + 32)
#define RCAR_GEN4_INTCSYS_SPI_943	      (943 + 32)
#define RCAR_GEN4_INTCSYS_SPI_944	      (944 + 32)
#define RCAR_GEN4_INTCSYS_SPI_945	      (945 + 32)
#define RCAR_GEN4_INTCSYS_SPI_946	      (946 + 32)
#define RCAR_GEN4_INTCSYS_SPI_947	      (947 + 32)
#define RCAR_GEN4_INTCSYS_SPI_948	      (948 + 32)
#define RCAR_GEN4_INTCSYS_SPI_949	      (949 + 32)
#define RCAR_GEN4_INTCSYS_SPI_950	      (950 + 32)
#define RCAR_GEN4_INTCSYS_SPI_951	      (951 + 32)
#define RCAR_GEN4_INTCSYS_SPI_952	      (952 + 32)
#define RCAR_GEN4_INTCSYS_SPI_953	      (953 + 32)
#define RCAR_GEN4_INTCSYS_SPI_954	      (954 + 32)
#define RCAR_GEN4_INTCSYS_SPI_955	      (955 + 32)
#define RCAR_GEN4_INTCSYS_SPI_956	      (956 + 32)
#define RCAR_GEN4_INTCSYS_SPI_957	      (957 + 32)
#define RCAR_GEN4_INTCSYS_SPI_958	      (958 + 32)
#define RCAR_GEN4_INTCSYS_SPI_959	      (959 + 32)

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
 * ECM offset for S4 and V4H
 */
 #define RCAR_GEN4_ECM_ERRCTLR(n)         (0x0000 + (n * 4))
 #define RCAR_GEN4_ECM_ERRSTSR(n)         (0x0100 + (n * 4))
 #define RCAR_GEN4_ECM_ERRTGTR(n)         (0x0200 + (n * 4))
 #define RCAR_GEN4_ECM_ERRCNTR(n)         (0x0400 + (n * 4))
 #define RCAR_GEN4_ECM_ERRFATALR(n)       (0x0600 + (n * 4))
 #define RCAR_GEN4_ECM_PSSTATCTLRA(n)     (0x0700 + (n * 4))
 #define RCAR_GEN4_ECM_PSSTATCTLRB(n)     (0x0800 + (n * 4))
 #define RCAR_GEN4_ECM_PSSTATCTLRM         0x0900
 #define RCAR_GEN4_ECM_GEIIDR              0x0928
 #define RCAR_GEN4_ECM_SAFSTSR             0x0940
 #define RCAR_GEN4_ECM_SAFSTERRENR         0x0944
 #define RCAR_GEN4_ECM_SAFCLERRENR         0x0948
 #define RCAR_GEN4_ECM_EDCERRSIDPADDR      0x095C
 #define RCAR_GEN4_ECM_EDCERRSIDPWDATAR    0x0960
 #define RCAR_GEN4_ECM_DCLSERMON00R        0x0984
 #define RCAR_GEN4_ECM_DCLSERMON01R        0x0988
 #define RCAR_GEN4_ECM_WPCNTR              0x0A00
 #define RCAR_GEN4_ECM_WACNTR              0x0A04
/*
 * ECM offset for only V4H
 *
*/
 #define RCAR_GEN4_ECM_EXTRQHLDCNTR        0x0A10
 #define RCAR_GEN4_ECM_EXTRQMSKCNTR        0x0A14
 #define RCAR_GEN4_ECM_EXTRQSTSR           0x0A18  
 #define RCAR_GEN4_ECM_ERRSTSINR           0x0A28
 #define RCAR_GEN4_ECM_ERROUTCTLR          0x0A2C
 #define RCAR_GEN4_ECM_DYNCTRLR            0x0A30
 #define RCAR_GEN4_ECM_DYNFREQSELR         0x0A34
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

/* V4H SSIU registers */
#define RCAR_SSIU_SSI0_0_BUSIF_MODE    0x000   // SSI[n]_0 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_0_BUSIF_ADINR   0x004   // SSI[n]_0 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_0_BUSIF_DALIGN  0x008   // SSI[n]_0 BUSIF Data Align Register(n=0) 
#define RCAR_SSIU_SSI0_0_MODE          0x00C   // SSI[n]_0 Mode  Register(n=0) 
#define RCAR_SSIU_SSI0_0_CONTROL       0x010   // SSI[n]_0 Control Register (n=0)
#define RCAR_SSIU_SSI0_0_STATUS        0x014    // SSI[n]_0 Status  Register(n=0) 
#define RCAR_SSIU_SSI0_0_INT_ENABLE_MAIN    0x018     // SSI[n]_0 Interrupt Enable Register (n=0)
#define RCAR_SSIU_SSI0_1_BUSIF_MODE    0x020   // SSI[n]_1 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_1_BUSIF_ADINR   0x024   // SSI[n]_1 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_1_BUSIF_DALIGN  0x028   // SSI[n]_1 BUSIF Data Align Register(n=0) 
#define RCAR_SSIU_SSI0_2_BUSIF_MODE    0x040   // SSI[n]_2 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_2_BUSIF_ADINR   0x044   // SSI[n]_2 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_2_BUSIF_DALIGN  0x048   // SSI[n]_2 BUSIF Data Align Register(n=0) 
#define RCAR_SSIU_SSI0_3_BUSIF_MODE    0x060   // SSI[n]_3 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_3_BUSIF_ADINR   0x064   // SSI[n]_3 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_3_BUSIF_DALIGN  0x068   // SSI[n]_3 BUSIF Data Align Register(n=0) 
#define RCAR_SSIU_SSI0_4_BUSIF_MODE    0x500   // SSI[n]_4 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_4_BUSIF_ADINR   0x504   // SSI[n]_4 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_4_BUSIF_DALIGN  0x508   // SSI[n]_4 BUSIF Data Align Register(n=0) 
#define RCAR_SSIU_SSI0_5_BUSIF_MODE    0x520   // SSI[n]_5 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_5_BUSIF_ADINR   0x524   // SSI[n]_5 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_5_BUSIF_DALIGN  0x528   // SSI[n]_5 BUSIF Data Align Register(n=0) 
#define RCAR_SSIU_SSI0_6_BUSIF_MODE    0x540   // SSI[n]_6 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_6_BUSIF_ADINR   0x544   // SSI[n]_6 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_6_BUSIF_DALIGN  0x548   // SSI[n]_6 BUSIF Data Align Register(n=0) 
#define RCAR_SSIU_SSI0_7_BUSIF_MODE    0x560   // SSI[n]_7 BUSIF Mode Register (n=0)
#define RCAR_SSIU_SSI0_7_BUSIF_ADINR   0x564   // SSI[n]_7 BUSIF Audio Information Register (n=0) 
#define RCAR_SSIU_SSI0_7_BUSIF_DALIGN  0x568   // SSI[n]_7 BUSIF Data Align Register(n=0)
#define RCAR_SSIU_SSI0_0_BUSIF_DALIGN2 0xA08   // SSI[n]_0 BUSIF Data Align2 Register(n=0) 
#define RCAR_SSIU_SSI0_0_MODE2         0xA0C   // SSI[n]_0 Mode2  Register(n=0) 
#define RCAR_SSIU_SSI0_0_STATUS2       0xA14   // SSI[n]_0 Status2  Register(n=0) 

#define RCAR_SSIU_SSI_SYSTEM_STATUS0       0x840    // SSI System Status Register 0 
#define RCAR_SSIU_SSI_SYSTEM_STATUS2       0x848    // SSI System Status Register 2 
#define RCAR_SSIU_SSI_SYSTEM_INT_ENABLE0   0x850    // SSI System Interrupt Enable Register 0
#define RCAR_SSIU_SSI_SYSTEM_INT_ENABLE2   0x858    // SSI System Interrupt Enable Register 2
#define RCAR_SSIU_SSI_SYSTEM_INT_ENABLE4   0x890    // SSI System Interrupt Enable Register 4
#define RCAR_SSIU_SSI_SYSTEM_INT_ENABLE6   0x898    // SSI System Interrupt Enable Register 6
#define RCAR_SSIU_SSI_SYSTEM_STATUS4       0x880    // SSI System Status Register 4 
#define RCAR_SSIU_SSI_SYSTEM_STATUS6       0x888    // SSI System Status Register 6 
#define RCAR_SSIU_SSI_INT_ENABLE_MAIN2     0xA18    // SSI[n]_0 Interrupt Enable2 Register(n=0)

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
 * SYS-DMAC for Gen4 (V3U, S4, V4H)
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
 * RT-DMAC for Gen4 (V3U, S4, V4H)
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
#define RCAR_DREQ_SSI00_RXI         0x6E
#define RCAR_DREQ_SSI00_TXI         0x6F
#define RCAR_DREQ_SSI01_RXI         0x6C
#define RCAR_DREQ_SSI01_TXI         0x6D
#define RCAR_DREQ_SSI02_RXI         0x6A
#define RCAR_DREQ_SSI02_TXI         0x6B
#define RCAR_DREQ_SSI03_RXI         0x68
#define RCAR_DREQ_SSI03_TXI         0x69
#define RCAR_DREQ_SSI04_RXI         0x66
#define RCAR_DREQ_SSI04_TXI         0x67
#define RCAR_DREQ_SSI05_RXI         0x64
#define RCAR_DREQ_SSI05_TXI         0x65
#define RCAR_DREQ_SSI06_RXI         0x62
#define RCAR_DREQ_SSI06_TXI         0x63
#define RCAR_DREQ_SSI07_RXI         0x60
#define RCAR_DREQ_SSI07_TXI         0x61

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
                                    (n == 2 ? RCAR_DREQ_MSIOF2_TXI: \
                                    (n == 3 ? RCAR_DREQ_MSIOF3_TXI: \
                                    (n == 4 ? RCAR_DREQ_MSIOF4_TXI: RCAR_DREQ_MSIOF5_TXI)))))
#define RCAR_DREQ_MSIOF_RXI(n)      (n == 0 ? RCAR_DREQ_MSIOF0_RXI: \
                                    (n == 1 ? RCAR_DREQ_MSIOF1_RXI: \
                                    (n == 2 ? RCAR_DREQ_MSIOF2_RXI: \
                                    (n == 3 ? RCAR_DREQ_MSIOF3_RXI: \
                                    (n == 4 ? RCAR_DREQ_MSIOF4_RXI: RCAR_DREQ_MSIOF5_RXI)))))

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
#define RCAR_TSC_THCODE1                 0x050
#define RCAR_TSC_THCODE2                 0x054
#define RCAR_TSC_THCODE3                 0x058
 #define RCAR_TSC_THCODE_MASK            0xFFF
#define RCAR_TSC_PTAT1                   0x05C
#define RCAR_TSC_PTAT2                   0x060
#define RCAR_TSC_PTAT3                   0x064
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
#define RCAR_VIN8_BASE              0xE6EF8000 /* V3H, V3U & V4H only */ 
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
 #define RCAR_VIN_MC_INF_RAW10           (7 << 16)
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
 #define RCAR_VIN_DMR_RMODE_RAW8	       (0 << 19)
 #define RCAR_VIN_DMR_RMODE_RAW10      (2 << 19)
 #define RCAR_VIN_DMR_RMODE_RAW12      (3 << 19)
 #define RCAR_VIN_DMR_RMODE_RAW14      (4 << 19)
 #define RCAR_VIN_DMR_RMODE_RAW20      (5 << 19)
 #define RCAR_VIN_DMR_YC_THR           (1 << 11)
 #define RCAR_VIN_DMR_EXRGB                (1 << 8)
 #define RCAR_VIN_DMR_BPSM                 (1 << 4)
 #define RCAR_VIN_DMR_ABIT                 (1 << 2)
 #define RCAR_VIN_DMR_DTMD_YCSEP           (1 << 1)
 #define RCAR_VIN_DMR_DTMD_ARGB            (1 << 0)
 #define RCAR_VIN_DMR_A8BIT                (0xFF << 24)
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
 * V4H Camera Serial Interface 2
 */
#define RCAR_GEN4_CSI20_BASE             0xFE500000 
#define RCAR_GEN4_CSI21_BASE             0xFE540000
#define RCAR_GEN4_CSI2_SIZE              0x30000

/* V4H offset from base */
/* Register accessed in 32 bits */
#define RCAR_GEN4_CSI2_VERSION                          0x0000
#define RCAR_GEN4_CSI2_N_LANES                          0x0004
#define RCAR_GEN4_CSI2_RESETN                           0x0008
#define RCAR_GEN4_CSI2_INT_ST_MAIN                      0x000C
#define RCAR_GEN4_CSI2_DATA_IDS_1                       0x0010
#define RCAR_GEN4_CSI2_PHY_MODE                         0x001C
#define RCAR_GEN4_CSI2_DATA_IDS_VC_1                    0x0030
#define RCAR_GEN4_CSI2_PHY_SHUTDOWNZ                    0x0040
#define RCAR_GEN4_CSI2_DPHY_RSTZ                        0x0044
#define RCAR_GEN4_CSI2_PHY_RX                           0x0048
#define RCAR_GEN4_CSI2_PHY_STOPSTATE                    0x004C
#define RCAR_GEN4_CSI2_PHY_CAL                          0x00CC
#define RCAR_GEN4_CSI2_INT_ST_PHY_FATAL	                0x00E0
#define RCAR_GEN4_CSI2_INT_MSK_PHY_FATAL                0x00E4
#define RCAR_GEN4_CSI2_INT_FORCE_PHY_FATAL	            0x00E8
#define RCAR_GEN4_CSI2_INT_ST_PKT_FATAL	                0x00F0
#define RCAR_GEN4_CSI2_INT_MSK_PKT_FATAL                0x00F4
#define RCAR_GEN4_CSI2_INT_FORCE_PKT_FATAL	            0x00F8
#define RCAR_GEN4_CSI2_INT_ST_PHY	                    0x0110
#define RCAR_GEN4_CSI2_INT_MSK_PHY	       	            0x0114
#define RCAR_GEN4_CSI2_INT_FORCE_PHY                    0x0118
#define RCAR_GEN4_CSI2_INT_ST_LINE	                    0x0130
#define RCAR_GEN4_CSI2_INT_MSK_LINE	                    0x0134
#define RCAR_GEN4_CSI2_INT_FORCE_LINE	                0x0138
#define RCAR_GEN4_CSI2_INT_ST_PLD_CRC_FATAL	            0x02B0
#define RCAR_GEN4_CSI2_INT_MSK_PLD_CRC_FATAL	        0x02B4
#define RCAR_GEN4_CSI2_INT_FORCE_PLD_CRC_FATAL	        0x02B8
#define RCAR_GEN4_CSI2_INT_ST_DATA_ID	                0x02C0
#define RCAR_GEN4_CSI2_INT_MSK_DATA_ID	                0x02C4
#define RCAR_GEN4_CSI2_INT_FORCE_DATA_ID	            0x02C8
#define RCAR_GEN4_CSI2_INT_ST_ECC_CORRECTED	            0x02D0
#define RCAR_GEN4_CSI2_INT_MSK_ECC_CORRECTED	        0x02D4
#define RCAR_GEN4_CSI2_INT_FORCE_ECC_CORRECTED	        0x02D8
#define RCAR_GEN4_CSI2_SCRAMBLING_SEED1	                0x0304
#define RCAR_GEN4_CSI2_SCRAMBLING_SEED2	                0x0308
#define RCAR_GEN4_CSI2_SCRAMBLING_SEED3	                0x030C
#define RCAR_GEN4_CSI2_SCRAMBLING_SEED4	                0x0310
#define RCAR_GEN4_CSI2_FLDC	                            0x0804
#define RCAR_GEN4_CSI2_FLDD	                            0x0808
#define RCAR_GEN4_CSI2_IDIC	                            0x0810
#define RCAR_GEN4_CSI2_PHY_EN                           0x2000
#define RCAR_GEN4_CSI2_FRXM	                            0x2004
#define RCAR_GEN4_CSI2_RDTW	                            0x2008
#define RCAR_GEN4_CSI2_TXESC0	                        0x2040
#define RCAR_GEN4_CSI2_TXESC1	                        0x2044
#define RCAR_GEN4_CSI2_TXESC2	                        0x2048
#define RCAR_GEN4_CSI2_TXESC3	                        0x204C
#define RCAR_GEN4_CSI2_ST_RXESC	                        0x2800
#define RCAR_GEN4_CSI2_ESCERR	                        0x2804
#define RCAR_GEN4_CSI2_ST_RXACT	                        0x280C
#define RCAR_GEN4_CSI2_ST_PXSYNC	                    0x2810
#define RCAR_GEN4_CSI2_ST_PHYST                         0x2814

/* Registers accessed in 16 bits */
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_0                 0x21800
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_1                 0x21802
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_2                 0x21804
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_3                 0x21806
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_4                 0x21808
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_5                 0x2180A
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_6                 0x2180C
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_7                 0x2180E
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_8                 0x21810
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_9                 0x21812
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_A                 0x21814
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_10                0x21820
#define RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_STARTUP_1_1            0x21822
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_RW_COMMON_CALIBCTRL_2_0        0x21840
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_R_COMMON_CALIBCTRL_2_1         0x21842
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_R_COMMON_CALIBCTRL_2_2         0x21844
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_R_COMMON_CALIBCTRL_2_3         0x21846
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_R_COMMON_CALIBCTRL_2_4         0x21848
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_R_COMMON_CALIBCTRL_2_5         0x2184A
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_RW_COMMON_BG_0                 0x2184C
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_RW_COMMON_CALIBCTRL_2_7        0x2184E
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_RW_ADC_CFG_0                   0x21850
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_RW_ADC_CFG_1                   0x21852
#define RCAR_GEN4_CSI2_PPI_CALIBCTRL_R_ADC_DEBUG                    0x21854

#define RCAR_GEN4_CSI2_PPI_RW_TERMCAL_CFG_0                        0x21C80
#define RCAR_GEN4_CSI2_PPI_RW_OFFSETCAL_CFG_0                      0x21CA0
#define RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_TIMEBASE                    0x21C02
#define RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_NREF                        0x21C04
#define RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_NREF_RANGE                  0x21C06
#define RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_TWAIT_CONFIG                0x21C0A
#define RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_VT_CONFIG                   0x21C0C
#define RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_COARSE_CFG                  0x21C10
#define RCAR_GEN4_CSI2_PPI_RW_COMMON_CFG                           0x21C6C

#define RCAR_GEN4_CSI2_PPI_R_LB_START_CMU_RB                        0x21C26
#define RCAR_GEN4_CSI2_PPI_RW_LB_DPHY_BURST_START                   0x21C28
#define RCAR_GEN4_CSI2_PPI_RW_LB_CPHY_BURST_START                   0x21C2A
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_0                          0x21C40
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_1                          0x21C42
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_2                          0x21C44
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_3                          0x21C46
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_4                          0x21C48
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_5                          0x21C4A
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_6                          0x21C4C
#define RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_7                          0x21C4E
#define RCAR_GEN4_CSI2_PPI_R_DDLCAL_DEBUG_0                         0x21C50
#define RCAR_GEN4_CSI2_PPI_R_DDLCAL_DEBUG_1                         0x21C52
#define RCAR_GEN4_CSI2_PPI_RW_PARITY_TEST                           0x21C60
#define RCAR_GEN4_CSI2_PPI_RW_STARTUP_OVR_0                         0x21C62
#define RCAR_GEN4_CSI2_PPI_RW_STARTUP_STATE_OVR_1                   0x21C64
#define RCAR_GEN4_CSI2_PPI_RW_DTB_SELECTOR                          0x21C66
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_CLK_SPARE                        0x21C6A
#define RCAR_GEN4_CSI2_PPI_RW_COMMON_CFG                            0x21C6C
#define RCAR_GEN4_CSI2_PPI_RW_TERMCAL_CFG_0                         0x21C80
#define RCAR_GEN4_CSI2_PPI_R_TERMCAL_DEBUG_0                        0x21C82
#define RCAR_GEN4_CSI2_PPI_RW_OFFSETCAL_CFG_0                       0x21CA0
#define RCAR_GEN4_CSI2_PPI_R_OFFSETCAL_DEBUG_LANE0                  0x21CA2
#define RCAR_GEN4_CSI2_PPI_R_OFFSETCAL_DEBUG_LANE1                  0x21CA4
#define RCAR_GEN4_CSI2_PPI_R_OFFSETCAL_DEBUG_LANE2                  0x21CA6
#define RCAR_GEN4_CSI2_PPI_R_OFFSETCAL_DEBUG_LANE3                  0x21CA8
#define RCAR_GEN4_CSI2_PPI_R_OFFSETCAL_DEBUG_LANE4                  0x21CAA
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_0    0x22000
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_1    0x22002
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_2    0x22004
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_3    0x22006
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_4    0x22008
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_5    0x2200A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_6    0x2200C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_7    0x2200E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE0_OVR_0_8    0x22010
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE0_OVR_0_9     0x22012
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE0_OVR_0_10    0x22014
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE0_OVR_0_11    0x22016
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE0_OVR_0_12    0x22018
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE0_OVR_0_13    0x2201A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE0_OVR_0_14    0x2201C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE0_OVR_0_15    0x2201E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_0    0x22020
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_1    0x22022
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_2    0x22024
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_3    0x22026
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_4    0x22028
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_5    0x2202A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_6    0x2202C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_7    0x2202E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_8    0x22030
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_1_9    0x22032
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_1_10    0x22034
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_1_11    0x22036
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_1_12    0x22038
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_1_13    0x2203A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_1_14    0x2203C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_1_15    0x2203E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_0        0x22040
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_1        0x22042
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_2        0x22044
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_3        0x22046
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_4        0x22048
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_5        0x2204A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_6        0x2204C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_7        0x2204E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_8        0x22050
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_9        0x22052
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_10       0x22054
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_11       0x22056
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_12       0x22058
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_13       0x2205A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_14       0x2205C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_15       0x2205E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_0        0x22060
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_1        0x22062
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_2        0x22064
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_3        0x22066
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_4        0x22068
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_5        0x2206A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_6        0x2206C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_3_7        0x2206E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_8         0x22070
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_9         0x22072
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_10        0x22074
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_11        0x22076
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_12        0x22078
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_13        0x2207A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_14        0x2207C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_3_15        0x2207E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_4_0         0x22080
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_4_1         0x22082
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_4_2         0x22084
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_4_3         0x22086
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE0_CTRL_4_4         0x22088
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_5_0     0x220A0
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_5_1     0x220A2
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_5_2    0x220A4
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE0_OVR_5_3    0x220A6
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE0_OVR_5_4     0x220A8
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO0_0                          0x22100
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO0_1                          0x22102
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO0_2                          0x22104
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_0    0x22400
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_1    0x22402
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_2    0x22404
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_3    0x22406
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_4    0x22408
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_5    0x2240A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_6    0x2240C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_7    0x2240E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_8    0x22410
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE1_OVR_0_9     0x22412
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE1_OVR_0_10    0x22414
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE1_OVR_0_11    0x22416
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE1_OVR_0_12    0x22418
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE1_OVR_0_13    0x2241A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE1_OVR_0_14    0x2241C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE1_OVR_0_15    0x2241E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_0    0x22420
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_1    0x22422
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_2    0x22424
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_3    0x22426
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_4    0x22428
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_5    0x2242A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_6    0x2242C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_7    0x2242E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_8    0x22430
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_1_9    0x22432
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_1_10    0x22434
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_1_11    0x22436
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_1_12    0x22438
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_1_13    0x2243A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_1_14    0x2243C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_1_15    0x2243E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_0        0x22440
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_1        0x22442
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_2        0x22444
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_3        0x22446
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_4        0x22448
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_5        0x2244A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_6        0x2244C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_7        0x2244E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_8        0x22450
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_9        0x22452
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_10       0x22454
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_11       0x22456
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_12       0x22458
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_13       0x2245A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_14       0x2245C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_15       0x2245E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_0        0x22460
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_1        0x22462
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_2        0x22464
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_3        0x22466
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_4        0x22468
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_5        0x2246A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_6        0x2246C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_3_7        0x2246E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_8         0x22470
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_9         0x22472
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_10        0x22474
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_11        0x22476
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_12        0x22478
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_13        0x2247A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_14        0x2247C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_3_15        0x2247E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_4_0         0x22480
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_4_1         0x22482
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_4_2         0x22484
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_4_3         0x22486
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE1_CTRL_4_4         0x22488
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_5_0     0x224A0
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_5_1     0x224A2
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_5_2    0x224A4
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE1_OVR_5_3    0x224A6
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE1_OVR_5_4     0x224A8
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO1_0                          0x22500
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO1_1                          0x22502
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO1_2                          0x22504
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_0    0x22800
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_1    0x22802
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_2    0x22804
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_3    0x22806
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_4    0x22808
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_5    0x2280A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_6    0x2280C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_7    0x2280E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE2_OVR_0_8    0x22810
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE2_OVR_0_9     0x22812
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE2_OVR_0_10    0x22814
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE2_OVR_0_11    0x22816
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE2_OVR_0_12    0x22818
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE2_OVR_0_13    0x2281A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE2_OVR_0_14    0x2281C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE2_OVR_0_15    0x2281E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_0    0x22820
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_1    0x22822
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_2    0x22824
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_3    0x22826
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_4    0x22828
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_5    0x2282A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_6    0x2282C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_7    0x2282E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_8    0x22830
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_1_9    0x22832
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_1_10    0x22834
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_1_11    0x22836
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_1_12    0x22838
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_1_13    0x2283A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_1_14    0x2283C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_1_15    0x2283E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_0        0x22840
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_1        0x22842
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_2        0x22844
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_3        0x22846
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_4        0x22848
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_5        0x2284A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_6        0x2284C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_7        0x2284E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_8        0x22850
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_9        0x22852
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_10       0x22854
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_11       0x22856
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_12       0x22858
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_13       0x2285A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_14       0x2285C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_15       0x2285E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_0        0x22860
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_1        0x22862
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_2        0x22864
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_3        0x22866
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_4        0x22868
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_5        0x2286A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_6        0x2286C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_3_7        0x2286E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_8         0x22870
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_9         0x22872
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_10        0x22874
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_11        0x22876
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_12        0x22878
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_13        0x2287A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_14        0x2287C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_3_15        0x2287E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_4_0         0x22880
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_4_1         0x22882
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_4_2         0x22884
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_4_3         0x22886
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE2_CTRL_4_4         0x22888
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_5_0     0x228A0
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_5_1     0x228A2
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_5_2    0x228A4
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_CPHY_PPI_LANE2_OVR_5_3    0x228A6
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_CPHY_PPI_LANE2_OVR_5_4     0x228A8
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO2_0                          0x22900
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO2_1                          0x22902
#define RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO2_2                          0x22904
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_0    0x22C00
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_1    0x22C02
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_2    0x22C04
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_3    0x22C06
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_4    0x22C08
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_5    0x22C0A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_6    0x22C0C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_7    0x22C0E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE3_OVR_0_8    0x22C10
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE3_OVR_0_9     0x22C12
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE3_OVR_0_10    0x22C14
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE3_OVR_0_11    0x22C16
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE3_OVR_0_12    0x22C18
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE3_OVR_0_13    0x22C1A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE3_OVR_0_14    0x22C1C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_LANE3_OVR_0_15    0x22C1E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_0        0x22C40
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_1        0x22C42
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_2        0x22C44
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_3        0x22C46
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_4        0x22C48
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_5        0x22C4A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_6        0x22C4C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_7        0x22C4E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_8        0x22C50
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_9        0x22C52
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_10       0x22C54
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_11       0x22C56
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_12       0x22C58
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_13       0x22C5A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_14       0x22C5C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_15       0x22C5E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_0        0x22C60
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_1        0x22C62
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_2        0x22C64
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_3        0x22C66
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_4        0x22C68
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_5        0x22C6A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_6        0x22C6C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_3_7        0x22C6E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_8         0x22C70
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_9         0x22C72
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_10        0x22C74
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_11        0x22C76
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_12        0x22C78
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_13        0x22C7A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_14        0x22C7C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_3_15        0x22C7E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_4_0         0x22C80
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_4_1         0x22C82
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_4_2         0x22C84
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_4_3         0x22C86
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE3_CTRL_4_4         0x22C88
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_0        0x23040
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_1        0x23042
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_2        0x23044
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_3        0x23046
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_4        0x23048
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_5        0x2304A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_6        0x2304C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_7        0x2304E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_8        0x23050
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_9        0x23052
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_10       0x23054
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_11       0x23056
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_12       0x23058
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_13       0x2305A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_14       0x2305C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_15       0x2305E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_0        0x23060
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_1        0x23062
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_2        0x23064
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_3        0x23066
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_4        0x23068
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_5        0x2306A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_6        0x2306C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_3_7        0x2306E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_8         0x23070
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_9         0x23072
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_10        0x23074
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_11        0x23076
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_12        0x23078
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_13        0x2307A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_14        0x2307C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_3_15        0x2307E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_4_0         0x23080
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_4_1         0x23082
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_4_2         0x23084
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_4_3         0x23086
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_LANE4_CTRL_4_4         0x23088
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_CLK_OVR_0_0      0x23400
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_DPHY_PPI_CLK_OVR_0_1      0x23402
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_DPHY_PPI_CLK_OVR_0_2       0x23404
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_0        0x23800
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_1        0x23802
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_2        0x23804
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_3        0x23806
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_4        0x23808
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_5        0x2380A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_6        0x2380C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_7        0x2380E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_8        0x23810
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_9        0x23812
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_COMMON_PPI_OVR_0_10       0x23814
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_0_11        0x23816
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_0_12        0x23818
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_0_13        0x2381A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_0_14        0x2381C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_0_15        0x2381E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_1_0         0x23820
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_1_1         0x23822
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_1_2         0x23824
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_1_3         0x23826
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_COMMON_PPI_OVR_1_4         0x23828
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_0           0x23840
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_1           0x23842
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_2           0x23844
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_3           0x23846
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_4           0x23848
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_5           0x2384A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_6           0x2384C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_7           0x2384E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_8           0x23850
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_9           0x23852
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_10          0x23854
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_11          0x23856
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_CB_CTRL_2_12           0x23858
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_CB_CTRL_2_13           0x2385A
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_CB_CTRL_2_14           0x2385C
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_CB_CTRL_2_15           0x2385E
#define RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_R_AFE_CB_CTRL_3_0            0x23860
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_0                         0x23880
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_1                         0x23882
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_2                         0x23884
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_3                         0x23886
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_4                         0x23888
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_5                         0x2388A
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_6                         0x2388C
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_7                         0x2388E
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_8                         0x23890
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_9                         0x23892
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_10                        0x23894
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_11                        0x23896
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_12                        0x23898
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_13                        0x2389A
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_14                        0x2389C
#define RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_15                        0x2389E
#define RCAR_GEN4_CSI2_CORE_DIG_ANACTRL_RW_COMMON_ANACTRL_0         0x239E0
#define RCAR_GEN4_CSI2_CORE_DIG_ANACTRL_RW_COMMON_ANACTRL_1         0x239E2
#define RCAR_GEN4_CSI2_CORE_DIG_ANACTRL_RW_COMMON_ANACTRL_2         0x239E4
#define RCAR_GEN4_CSI2_CORE_DIG_ANACTRL_RW_COMMON_ANACTRL_3         0x239E6
#define RCAR_GEN4_CSI2_CORE_DIG_COMMON_RW_DESKEW_FINE_MEM           0x23FE0
#define RCAR_GEN4_CSI2_CORE_DIG_COMMON_R_DESKEW_FINE_MEM            0x23FE2
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE0_LBERT_0                    0x24000
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE0_LBERT_1                    0x24002
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE0_LBERT_0                     0x24004
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE0_LBERT_1                     0x24006
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE0_SPARE                      0x24008
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE1_LBERT_0                    0x24400
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE1_LBERT_1                    0x24402
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE1_LBERT_0                     0x24404
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE1_LBERT_1                     0x24406
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE1_SPARE                      0x24408
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE2_LBERT_0                    0x24800
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE2_LBERT_1                    0x24802
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE2_LBERT_0                     0x24804
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE2_LBERT_1                     0x24806
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE2_SPARE                      0x24808
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE3_LBERT_0                    0x24C00
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE3_LBERT_1                    0x24C02
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE3_LBERT_0                     0x24C04
#define RCAR_GEN4_CSI2_PPI_R_DPHY_LANE3_LBERT_1                     0x24C06
#define RCAR_GEN4_CSI2_PPI_RW_DPHY_LANE3_SPARE                      0x24C08
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_CFG_0                    0x26000
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_CFG_1                    0x26002
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_CFG_2                    0x26004
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_LP_0                     0x26080
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_LP_1                     0x26082
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_LP_0                      0x260A0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_LP_1                      0x260A2
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_HS_TX_0                   0x260E0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_0                  0x26100
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_1                  0x26102
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_2                  0x26104
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_3                  0x26106
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_4                  0x26108
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_5                  0x2610A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_6                  0x2610C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_7                  0x2610E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_8                  0x26110
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_HS_RX_0                   0x26120
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_HS_RX_1                   0x26122
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_HS_RX_2                   0x26124
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_HS_RX_3                   0x26126
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_R_HS_RX_4                   0x26128
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_0                  0x26200
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_1                  0x26202
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_2                  0x26204
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_3                  0x26206
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_4                  0x26208
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_5                  0x2620A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_6                  0x2620C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_7                  0x2620E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_8                  0x26210
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_9                  0x26212
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_10                 0x26214
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_11                 0x26216
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_TX_12                 0x26218
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_CFG_0                    0x26400
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_CFG_1                    0x26402
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_CFG_2                    0x26404
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_LP_0                     0x26480
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_LP_1                     0x26482
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_LP_0                      0x264A0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_LP_1                      0x264A2
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_HS_TX_0                   0x264E0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_0                  0x26500
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_1                  0x26502
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_2                  0x26504
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_3                  0x26506
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_4                  0x26508
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_5                  0x2650A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_6                  0x2650C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_7                  0x2650E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_8                  0x26510
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_HS_RX_0                   0x26520
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_HS_RX_1                   0x26522
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_HS_RX_2                   0x26524
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_HS_RX_3                   0x26526
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_R_HS_RX_4                   0x26528
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_0                  0x26600
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_1                  0x26602
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_2                  0x26604
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_3                  0x26606
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_4                  0x26608
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_5                  0x2660A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_6                  0x2660C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_7                  0x2660E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_8                  0x26610
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_9                  0x26612
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_10                 0x26614
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_11                 0x26616
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_TX_12                 0x26618
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_CFG_0                    0x26800
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_CFG_1                    0x26802
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_CFG_2                    0x26804
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_LP_0                     0x26880
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_LP_1                     0x26882
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_LP_0                      0x268A0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_LP_1                      0x268A2
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_HS_TX_0                   0x268E0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_0                  0x26900
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_1                  0x26902
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_2                  0x26904
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_3                  0x26906
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_4                  0x26908
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_5                  0x2690A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_6                  0x2690C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_7                  0x2690E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_8                  0x26910
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_HS_RX_0                   0x26920
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_HS_RX_1                   0x26922
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_HS_RX_2                   0x26924
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_HS_RX_3                   0x26926
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_R_HS_RX_4                   0x26928
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_0                  0x26A00
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_1                  0x26A02
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_2                  0x26A04
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_3                  0x26A06
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_4                  0x26A08
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_5                  0x26A0A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_6                  0x26A0C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_7                  0x26A0E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_8                  0x26A10
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_9                  0x26A12
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_10                 0x26A14
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_11                 0x26A16
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_TX_12                 0x26A18
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_CFG_0                    0x26C00
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_CFG_1                    0x26C02
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_CFG_2                    0x26C04
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_LP_0                     0x26C80
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_LP_1                     0x26C82
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_LP_0                      0x26CA0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_LP_1                      0x26CA2
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_HS_TX_0                   0x26CE0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_0                  0x26D00
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_1                  0x26D02
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_2                  0x26D04
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_3                  0x26D06
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_4                  0x26D08
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_5                  0x26D0A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_6                  0x26D0C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_7                  0x26D0E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_8                  0x26D10
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_HS_RX_0                   0x26D20
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_HS_RX_1                   0x26D22
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_HS_RX_2                   0x26D24
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_HS_RX_3                   0x26D26
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_R_HS_RX_4                   0x26D28
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_0                  0x26E00
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_1                  0x26E02
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_2                  0x26E04
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_3                  0x26E06
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_4                  0x26E08
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_5                  0x26E0A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_6                  0x26E0C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_7                  0x26E0E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_8                  0x26E10
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_9                  0x26E12
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_10                 0x26E14
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_11                 0x26E16
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_TX_12                 0x26E18
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_CFG_0                  0x27000
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_CFG_1                  0x27002
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_CFG_2                  0x27004
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_LP_0                   0x27080
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_LP_1                   0x27082
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_LP_0                    0x270A0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_LP_1                    0x270A2
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_HS_TX_0                 0x270E0
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_0                0x27100
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_1                0x27102
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_2                0x27104
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_3                0x27106
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_4                0x27108
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_5                0x2710A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_6                0x2710C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_7                0x2710E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_8                0x27110
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_HS_RX_0                 0x27120
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_HS_RX_1                 0x27122
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_HS_RX_2                 0x27124
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_HS_RX_3                 0x27126
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_R_HS_RX_4                 0x27128
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_0                0x27200
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_1                0x27202
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_2                0x27204
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_3                0x27206
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_4                0x27208
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_5                0x2720A
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_6                0x2720C
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_7                0x2720E
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_8                0x27210
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_9                0x27212
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_10               0x27214
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_11               0x27216
#define RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_TX_12               0x27218
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO0_LBERT_0                    0x28000
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO0_LBERT_1                    0x28002
#define RCAR_GEN4_CSI2_PPI_R_CPHY_TRIO0_LBERT_0                     0x28004
#define RCAR_GEN4_CSI2_PPI_R_CPHY_TRIO0_LBERT_1                     0x28006
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO0_SPARE                      0x28008
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO1_LBERT_0                    0x28400
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO1_LBERT_1                    0x28402
#define RCAR_GEN4_CSI2_PPI_R_CPHY_TRIO1_LBERT_0                     0x28404
#define RCAR_GEN4_CSI2_PPI_R_CPHY_TRIO1_LBERT_1                     0x28406
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO1_SPARE                      0x28408
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO2_LBERT_0                    0x28800
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO2_LBERT_1                    0x28802
#define RCAR_GEN4_CSI2_PPI_R_CPHY_TRIO2_LBERT_0                     0x28804
#define RCAR_GEN4_CSI2_PPI_R_CPHY_TRIO2_LBERT_1                     0x28806
#define RCAR_GEN4_CSI2_PPI_RW_CPHY_TRIO2_SPARE                      0x28808
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_CFG_0                    0x2A000
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_CFG_2                    0x2A004
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_LP_0                     0x2A080
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_LP_1                     0x2A082
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_R_LP_0                      0x2A0A0
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_R_LP_1                      0x2A0A2
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_0                  0x2A100
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_1                  0x2A102
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_2                  0x2A104
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_3                  0x2A106
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_4                  0x2A108
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_5                  0x2A10A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_6                  0x2A10C
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_R_RX_0                      0x2A120
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_R_RX_1                      0x2A122
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_R_TX_0                      0x2A124
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_R_RX_2                      0x2A126
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_R_RX_3                      0x2A128
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_0                  0x2A200
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_1                  0x2A202
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_2                  0x2A204
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_3                  0x2A206
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_4                  0x2A208
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_5                  0x2A20A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_6                  0x2A20C
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_7                  0x2A20E
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_8                  0x2A210
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_9                  0x2A212
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_10                 0x2A214
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_11                 0x2A216
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_12                 0x2A218
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_TX_13                 0x2A21A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_CFG_0                    0x2A400
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_CFG_2                    0x2A404
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_LP_0                     0x2A480
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_LP_1                     0x2A482
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_R_LP_0                      0x2A4A0
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_R_LP_1                      0x2A4A2
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_0                  0x2A500
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_1                  0x2A502
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_2                  0x2A504
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_3                  0x2A506
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_4                  0x2A508
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_5                  0x2A50A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_6                  0x2A50C
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_R_RX_0                      0x2A520
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_R_RX_1                      0x2A522
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_R_TX_0                      0x2A524
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_R_RX_2                      0x2A526
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_R_RX_3                      0x2A528
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_0                  0x2A600
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_1                  0x2A602
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_2                  0x2A604
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_3                  0x2A606
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_4                  0x2A608
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_5                  0x2A60A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_6                  0x2A60C
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_7                  0x2A60E
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_8                  0x2A610
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_9                  0x2A612
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_10                 0x2A614
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_11                 0x2A616
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_12                 0x2A618
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_13                 0x2A61A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_CFG_0                    0x2A800
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_CFG_2                    0x2A804
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_LP_0                     0x2A880
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_LP_1                     0x2A882
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_R_LP_0                      0x2A8A0
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_R_LP_1                      0x2A8A2
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_0                  0x2A900
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_1                  0x2A902
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_2                  0x2A904
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_3                  0x2A906
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_4                  0x2A908
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_5                  0x2A90A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_6                  0x2A90C
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_R_RX_0                      0x2A920
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_R_RX_1                      0x2A922
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_R_TX_0                      0x2A924
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_R_RX_2                      0x2A926
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_R_RX_3                      0x2A928
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_0                  0x2AA00
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_1                  0x2AA02
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_2                  0x2AA04
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_3                  0x2AA06
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_4                  0x2AA08
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_5                  0x2AA0A
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_6                  0x2AA0C
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_7                  0x2AA0E
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_8                  0x2AA10
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_9                  0x2AA12
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_10                 0x2AA14
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_11                 0x2AA16
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_12                 0x2AA18
#define RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_TX_13                 0x2AA1A

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
