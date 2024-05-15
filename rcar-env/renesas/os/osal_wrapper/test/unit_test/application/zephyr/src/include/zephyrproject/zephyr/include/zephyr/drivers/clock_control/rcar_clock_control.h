/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2016 BayLibre, SAS
 * Copyright (c) 2017 Linaro Limited.
 * Copyright (c) 2017 RnDity Sp. z o.o.
 * Copyright (c) 2022 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_DRIVERS_CLOCK_CONTROL_RCAR_CLOCK_CONTROL_H_
#define ZEPHYR_INCLUDE_DRIVERS_CLOCK_CONTROL_RCAR_CLOCK_CONTROL_H_

#include <zephyr/drivers/clock_control.h>
#include <zephyr/dt-bindings/clock/renesas_rcar_cpg.h>

struct rcar_cpg_clk {
	uint32_t domain;
	uint32_t module;
	uint32_t rate;
#ifdef CONFIG_SOC_SERIES_RCAR_GEN4
	uint32_t reset_state;
#endif
};

#ifdef CONFIG_SOC_SERIES_RCAR_GEN4
#define CPG_MSTP0_IMPPSC                27u      /* MSTPCR0, MSTPSR0, Bit27 */
#define CPG_MSTP0_IMPDMAC0              26u      /* MSTPCR0, MSTPSR0, Bit26 */
#define CPG_MSTP0_IMP1                  25u      /* MSTPCR0, MSTPSR0, Bit25 */
#define CPG_MSTP0_IMP0                  24u      /* MSTPCR0, MSTPSR0, Bit24 */
#define CPG_MSTP0_SPMC                  23u      /* MSTPCR0, MSTPSR0, Bit23 */
#define CPG_MSTP0_IMPCNN                22u      /* MSTPCR0, MSTPSR0, Bit22 */
#define CPG_MSTP0_ISP1                  17u      /* MSTPCR0, MSTPSR0, Bit17 */
#define CPG_MSTP0_ISP0                  16u      /* MSTPCR0, MSTPSR0, Bit16 */
#define CPG_MSTP0_UMFL0                 9u       /* MSTPCR0, MSTPSR0, Bit9 */
#define CPG_MSTP0_SMPS0                 7u       /* MSTPCR0, MSTPSR0, Bit7 */
#define CPG_MSTP0_SMPO0                 5u       /* MSTPCR0, MSTPSR0, Bit5 */
#define CPG_MSTP0_RZX                   0u       /* MSTPCR0, MSTPSR0, Bit0 */

#define CPG_MSTP1_ADVFSC                123u     /* MSTPCR1, MSTPSR1, Bit23 */
#define CPG_MSTP1_ADG                   122u     /* MSTPCR1, MSTPSR1, Bit22 */
#define CPG_MSTP1_SPMI                  120u     /* MSTPCR1, MSTPSR1, Bit20 */
#define CPG_MSTP1_IMPSLV                118u     /* MSTPCR1, MSTPSR1, Bit18 */
#define CPG_MSTP1_IMPDTA                116u     /* MSTPCR1, MSTPSR1, Bit16 */
#define CPG_MSTP1_IMPDMAC1              105u     /* MSTPCR1, MSTPSR1, Bit5 */
#define CPG_MSTP1_IMP3                  104u     /* MSTPCR1, MSTPSR1, Bit4 */
#define CPG_MSTP1_IMP2                  103u     /* MSTPCR1, MSTPSR1, Bit3 */

#define CPG_MSTP2_AVB2                  213u     /* MSTPCR2, MSTPSR2, Bit13 */
#define CPG_MSTP2_AVB1                  212u     /* MSTPCR2, MSTPSR2, Bit12 */
#define CPG_MSTP2_AVB0                  211u     /* MSTPCR2, MSTPSR2, Bit11 */

#define CPG_MSTP3_CSITOP0               331u     /* MSTPCR3, MSTPSR3, Bit31 */
#define CPG_MSTP3_CSDBGPAP              330u     /* MSTPCR3, MSTPSR3, Bit30 */
#define CPG_MSTP3_CR0                   329u     /* MSTPCR3, MSTPSR3, Bit29 */
#define CPG_MSTP3_CCANFD                328u     /* MSTPCR3, MSTPSR3, Bit28 */

#define CPG_MSTP4_DSITXLINK1            416u     /* MSTPCR4, MSTPSR4, Bit16 */
#define CPG_MSTP4_DSITXLINK0            415u     /* MSTPCR4, MSTPSR4, Bit15 */
#define CPG_MSTP4_DOC2CH                414u     /* MSTPCR4, MSTPSR4, Bit14 */
#define CPG_MSTP4_DIS0                  411u     /* MSTPCR4, MSTPSR4, Bit11 */
#define CPG_MSTP4_DDR3_2                410u     /* MSTPCR4, MSTPSR4, Bit10 ddr3:ddr2 */
#define CPG_MSTP4_DDR1_0                409u     /* MSTPCR4, MSTPSR4, Bit9 ddr1:ddr0 */
#define CPG_MSTP4_CSITOP1               400u     /* MSTPCR4, MSTPSR4, Bit0 */

#define CPG_MSTP5_INTAP                 531u     /* MSTPCR5, MSTPSR5, Bit31 */
#define CPG_MSTP5_IMS1                  530u     /* MSTPCR5, MSTPSR5, Bit30 */
#define CPG_MSTP5_IMS0                  529u     /* MSTPCR5, MSTPSR5, Bit29 */
#define CPG_MSTP5_IMR2                  527u     /* MSTPCR5, MSTPSR5, Bit27 */
#define CPG_MSTP5_IMR1                  526u     /* MSTPCR5, MSTPSR5, Bit26 */
#define CPG_MSTP5_IMR0                  525u     /* MSTPCR5, MSTPSR5, Bit25 */
#define CPG_MSTP5_I2C5                  523u     /* MSTPCR5, MSTPSR5, Bit23 */
#define CPG_MSTP5_I2C4                  522u     /* MSTPCR5, MSTPSR5, Bit22 */
#define CPG_MSTP5_I2C3                  521u     /* MSTPCR5, MSTPSR5, Bit21 */
#define CPG_MSTP5_I2C2                  520u     /* MSTPCR5, MSTPSR5, Bit20 */
#define CPG_MSTP5_I2C1                  519u     /* MSTPCR5, MSTPSR5, Bit19 */
#define CPG_MSTP5_I2C0                  518u     /* MSTPCR5, MSTPSR5, Bit18 */
#define CPG_MSTP5_HSCIF3                517u     /* MSTPCR5, MSTPSR5, Bit17 */
#define CPG_MSTP5_HSCIF2                516u     /* MSTPCR5, MSTPSR5, Bit16 */
#define CPG_MSTP5_HSCIF1                515u     /* MSTPCR5, MSTPSR5, Bit15 */
#define CPG_MSTP5_HSCIF0                514u     /* MSTPCR5, MSTPSR5, Bit14 */
#define CPG_MSTP5_FRAY00                513u     /* MSTPCR5, MSTPSR5, Bit13 */
#define CPG_MSTP5_FCPVD1                509u     /* MSTPCR5, MSTPSR5, Bit9 */
#define CPG_MSTP5_FCPVD0                508u     /* MSTPCR5, MSTPSR5, Bit8 */
#define CPG_MSTP5_FCPCS                 507u     /* MSTPCR5, MSTPSR5, Bit7 */

#define CPG_MSTP6_RTDM1                 631u     /* MSTPCR6, MSTPSR6, Bit31 */
#define CPG_MSTP6_RTDM0                 630u     /* MSTPCR6, MSTPSR6, Bit30 */
#define CPG_MSTP6_RPC                   629u     /* MSTPCR6, MSTPSR6, Bit29 */
#define CPG_MSTP6_PWM                   628u     /* MSTPCR6, MSTPSR6, Bit28 */
#define CPG_MSTP6_PCIE1                 625u     /* MSTPCR6, MSTPSR6, Bit25 */
#define CPG_MSTP6_PCIE0                 624u     /* MSTPCR6, MSTPSR6, Bit24 */
#define CPG_MSTP6_MSI5                  623u     /* MSTPCR6, MSTPSR6, Bit23 */
#define CPG_MSTP6_MSI4                  622u     /* MSTPCR6, MSTPSR6, Bit22 */
#define CPG_MSTP6_MSI3                  621u     /* MSTPCR6, MSTPSR6, Bit21 */
#define CPG_MSTP6_MSI2                  620u     /* MSTPCR6, MSTPSR6, Bit20 */
#define CPG_MSTP6_MSI1                  619u     /* MSTPCR6, MSTPSR6, Bit19 */
#define CPG_MSTP6_MSI0                  618u     /* MSTPCR6, MSTPSR6, Bit18 */
#define CPG_MSTP6_IVCP1E                616u     /* MSTPCR6, MSTPSR6, Bit16 */
#define CPG_MSTP6_ISPCS1                613u     /* MSTPCR6, MSTPSR6, Bit13 */
#define CPG_MSTP6_ISPCS0                612u     /* MSTPCR6, MSTPSR6, Bit12 */
#define CPG_MSTP6_IRQC                  611u     /* MSTPCR6, MSTPSR6, Bit11 */
#define CPG_MSTP6_INTTP                 608u     /* MSTPCR6, MSTPSR6, Bit8 */
#define CPG_MSTP6_IPC                   600u     /* MSTPCR6, MSTPSR6, Bit0 */

#define CPG_MSTP7_VIN01                 731u     /* MSTPCR7, MSTPSR7, Bit31 */
#define CPG_MSTP7_VIN00                 730u     /* MSTPCR7, MSTPSR7, Bit30 */
#define CPG_MSTP7_VCPL4                 729u     /* MSTPCR7, MSTPSR7, Bit29 */
#define CPG_MSTP7_TPU0                  718u     /* MSTPCR7, MSTPSR7, Bit18 */
#define CPG_MSTP7_TMU4                  717u     /* MSTPCR7, MSTPSR7, Bit17 */
#define CPG_MSTP7_TMU3                  716u     /* MSTPCR7, MSTPSR7, Bit16 */
#define CPG_MSTP7_TMU2                  715u     /* MSTPCR7, MSTPSR7, Bit15 */
#define CPG_MSTP7_TMU1                  714u     /* MSTPCR7, MSTPSR7, Bit14 */
#define CPG_MSTP7_TMU0                  713u     /* MSTPCR7, MSTPSR7, Bit13 */
#define CPG_MSTP7_SYDM2                 710u     /* MSTPCR7, MSTPSR7, Bit10 */
#define CPG_MSTP7_SYDM1                 709u     /* MSTPCR7, MSTPSR7, Bit9 */
#define CPG_MSTP7_SDHI0                 707u     /* MSTPCR7, MSTPSR7, Bit7 */
#define CPG_MSTP7_SCIF4                 706u     /* MSTPCR7, MSTPSR7, Bit6 */
#define CPG_MSTP7_SCIF3                 705u     /* MSTPCR7, MSTPSR7, Bit5 */
#define CPG_MSTP7_SCIF2                 704u     /* MSTPCR7, MSTPSR7, Bit4 */
#define CPG_MSTP7_SCIF1                 703u     /* MSTPCR7, MSTPSR7, Bit3 */
#define CPG_MSTP7_SCIF0                 702u     /* MSTPCR7, MSTPSR7, Bit2 */
#define CPG_MSTP7_RTDM3                 701u     /* MSTPCR7, MSTPSR7, Bit1 */
#define CPG_MSTP7_RTDM2                 700u     /* MSTPCR7, MSTPSR7, Bit0 */

#define CPG_MSTP8_VSPD1                 831u     /* MSTPCR8, MSTPSR8, Bit31 */
#define CPG_MSTP8_VSPD0                 830u     /* MSTPCR8, MSTPSR8, Bit30 */
#define CPG_MSTP8_VIN17                 813u     /* MSTPCR8, MSTPSR8, Bit13 */
#define CPG_MSTP8_VIN16                 812u     /* MSTPCR8, MSTPSR8, Bit12 */
#define CPG_MSTP8_VIN15                 811u     /* MSTPCR8, MSTPSR8, Bit11 */
#define CPG_MSTP8_VIN14                 810u     /* MSTPCR8, MSTPSR8, Bit10 */
#define CPG_MSTP8_VIN13                 809u     /* MSTPCR8, MSTPSR8, Bit9 */
#define CPG_MSTP8_VIN12                 808u     /* MSTPCR8, MSTPSR8, Bit8 */
#define CPG_MSTP8_VIN11                 807u     /* MSTPCR8, MSTPSR8, Bit7 */
#define CPG_MSTP8_VIN10                 806u     /* MSTPCR8, MSTPSR8, Bit6 */
#define CPG_MSTP8_VIN07                 805u     /* MSTPCR8, MSTPSR8, Bit5 */
#define CPG_MSTP8_VIN06                 804u     /* MSTPCR8, MSTPSR8, Bit4 */
#define CPG_MSTP8_VIN05                 803u     /* MSTPCR8, MSTPSR8, Bit3 */
#define CPG_MSTP8_VIN04                 802u     /* MSTPCR8, MSTPSR8, Bit2 */
#define CPG_MSTP8_VIN03                 801u     /* MSTPCR8, MSTPSR8, Bit1 */
#define CPG_MSTP8_VIN02                 800u     /* MSTPCR8, MSTPSR8, Bit0 */

#define CPG_MSTP9_UCMT                  920u     /* MSTPCR9, MSTPSR9, Bit20 */
#define CPG_MSTP9_TSC4_1                919u     /* MSTPCR9, MSTPSR9, Bit19 tsc4:tsc3:tsc2:tsc1 */
#define CPG_MSTP9_PFC3                  918u     /* MSTPCR9, MSTPSR9, Bit18 */
#define CPG_MSTP9_PFC2                  917u     /* MSTPCR9, MSTPSR9, Bit17 */
#define CPG_MSTP9_PFC1                  916u     /* MSTPCR9, MSTPSR9, Bit16 */
#define CPG_MSTP9_PFC0                  915u     /* MSTPCR9, MSTPSR9, Bit15 */
#define CPG_MSTP9_CMT3                  913u     /* MSTPCR9, MSTPSR9, Bit13 */
#define CPG_MSTP9_CMT2                  912u     /* MSTPCR9, MSTPSR9, Bit12 */
#define CPG_MSTP9_CMT1                  911u     /* MSTPCR9, MSTPSR9, Bit11 */
#define CPG_MSTP9_CMT0                  910u     /* MSTPCR9, MSTPSR9, Bit10 */
#define CPG_MSTP9_WDT1_0                907u     /* MSTPCR9, MSTPSR9, Bit7 wdt1:wdt0 */
#define CPG_MSTP9_WCRC3                 906u     /* MSTPCR9, MSTPSR9, Bit6 */
#define CPG_MSTP9_WCRC2                 905u     /* MSTPCR9, MSTPSR9, Bit5 */
#define CPG_MSTP9_WCRC1                 904u     /* MSTPCR9, MSTPSR9, Bit4 */
#define CPG_MSTP9_WCRC0                 903u     /* MSTPCR9, MSTPSR9, Bit3 */

#define CPG_MSTP10_VSPX1                1029u    /* MSTPCR10, MSTPSR10, Bit29 */
#define CPG_MSTP10_VSPX0                1028u    /* MSTPCR10, MSTPSR10, Bit28 */

#define CPG_MSTP11_AULK4P               1107u    /* MSTPCR11, MSTPSR11, Bit7 */
#define CPG_MSTP11_AULK2P               1106u    /* MSTPCR11, MSTPSR11, Bit6 */
#define CPG_MSTP11_FCPVX1               1101u    /* MSTPCR11, MSTPSR11, Bit1 */
#define CPG_MSTP11_FCPVX0               1100u    /* MSTPCR11, MSTPSR11, Bit0 */

#define CPG_MSTP12_KCRC4                1231u    /* MSTPCR12, MSTPSR12, Bit31 */
#define CPG_MSTP12_FSO                  1230u    /* MSTPCR12, MSTPSR12, Bit30 */
#define CPG_MSTP12_CRC3                 1228u    /* MSTPCR12, MSTPSR12, Bit28 */
#define CPG_MSTP12_CRC2                 1227u    /* MSTPCR12, MSTPSR12, Bit27 */
#define CPG_MSTP12_CRC1                 1226u    /* MSTPCR12, MSTPSR12, Bit26 */
#define CPG_MSTP12_CRC0                 1225u    /* MSTPCR12, MSTPSR12, Bit25 */
#define CPG_MSTP12_ADVFSC               1223u    /* MSTPCR12, MSTPSR12, Bit23 */

#define CPG_MSTP13_MTI                  1304u    /* MSTPCR13, MSTPSR13, Bit4 */
#define CPG_MSTP13_KCRC7                1302u    /* MSTPCR13, MSTPSR13, Bit2 */
#define CPG_MSTP13_KCRC6                1301u    /* MSTPCR13, MSTPSR13, Bit1 */
#define CPG_MSTP13_KCRC5                1300u    /* MSTPCR13, MSTPSR13, Bit0 */

#define CPG_MSTP27_CSBRG_IR_A2          2729u    /* MSTPCR27, MSTPSR27, Bit29 */
#define CPG_MSTP27_CSBRG_IR_A3          2728u    /* MSTPCR27, MSTPSR27, Bit28 */
#define CPG_MSTP27_TSN                  2723u    /* MSTPCR27, MSTPSR27, Bit23 */
#define CPG_MSTP27_IMPSDMAC1            2713u    /* MSTPCR27, MSTPSR27, Bit13 */
#define CPG_MSTP27_IMPSDMAC0            2712u    /* MSTPCR27, MSTPSR27, Bit12 */
#define CPG_MSTP27_CVE3                 2709u    /* MSTPCR27, MSTPSR27, Bit9 */
#define CPG_MSTP27_CVE2                 2708u    /* MSTPCR27, MSTPSR27, Bit8 */
#define CPG_MSTP27_CVE1                 2707u    /* MSTPCR27, MSTPSR27, Bit7 */
#define CPG_MSTP27_CVE0                 2706u    /* MSTPCR27, MSTPSR27, Bit6 */

#define CPG_MSTP28_VDSP1_CSB            2830u    /* MSTPCR28, MSTPSR28, Bit30 */
#define CPG_MSTP28_VDSP0_CSB            2821u    /* MSTPCR28, MSTPSR28, Bit21 */
#define CPG_MSTP28_DSC                  2819u    /* MSTPCR28, MSTPSR28, Bit19 */
#define CPG_MSTP28_FCPRC                2817u    /* MSTPCR28, MSTPSR28, Bit17 */
#define CPG_MSTP28_PAPSDMA              2807u    /* MSTPCR28, MSTPSR28, Bit7 */
#define CPG_MSTP28_PAPTOP               2806u    /* MSTPCR28, MSTPSR28, Bit6 */
#define CPG_MSTP28_PAPBUS               2805u    /* MSTPCR28, MSTPSR28, Bit5 */
#define CPG_MSTP28_VDSP3_BUS            2804u    /* MSTPCR28, MSTPSR28, Bit4 */
#define CPG_MSTP28_VDSP2_BUS            2803u    /* MSTPCR28, MSTPSR28, Bit3 */
#define CPG_MSTP28_VDSP1_BUS            2802u    /* MSTPCR28, MSTPSR28, Bit2 */
#define CPG_MSTP28_VDSP0_BUS            2801u    /* MSTPCR28, MSTPSR28, Bit1 */

#define CPG_MSTP29_SSI                  2927u    /* MSTPCR29, MSTPSR29, Bit27 */
#define CPG_MSTP29_SSIU                 2926u    /* MSTPCR29, MSTPSR29, Bit26 */
#define CPG_MSTP29_VDSP3_CSB            2916u    /* MSTPCR29, MSTPSR29, Bit16 */
#define CPG_MSTP29_VDSP2_CSB            2907u    /* MSTPCR29, MSTPSR29, Bit7 */

#define CPG_SR0_IMPPSC                  27u      /* SRCR0, SRSTCLR0, Bit27 */
#define CPG_SR0_IMPDMAC0                26u      /* SRCR0, SRSTCLR0, Bit26 */
#define CPG_SR0_IMP1                    25u      /* SRCR0, SRSTCLR0, Bit25 */
#define CPG_SR0_IMP0                    24u      /* SRCR0, SRSTCLR0, Bit24 */
#define CPG_SR0_SPMC                    23u      /* SRCR0, SRSTCLR0, Bit23 */
#define CPG_SR0_IMPCNN                  22u      /* SRCR0, SRSTCLR0, Bit22 */
#define CPG_SR0_ISP1                    17u      /* SRCR0, SRSTCLR0, Bit17 */
#define CPG_SR0_ISP0                    16u      /* SRCR0, SRSTCLR0, Bit16 */
#define CPG_SR0_ANRT1                   15u      /* SRCR0, SRSTCLR0, Bit15 */
#define CPG_SR0_UMFL0                   9u       /* SRCR0, SRSTCLR0, Bit9 */
#define CPG_SR0_SMPS0                   7u       /* SRCR0, SRSTCLR0, Bit7 */
#define CPG_SR0_SMPO0                   5u       /* SRCR0, SRSTCLR0, Bit5 */
#define CPG_SR0_RGX                     0u       /* SRCR0, SRSTCLR0, Bit0 */

#define CPG_SR1_ANVI1                   131u     /* SRCR1, SRSTCLR1, Bit31 */
#define CPG_SR1_ANVI0                   130u     /* SRCR1, SRSTCLR1, Bit30 */
#define CPG_SR1_ANVC1                   129u     /* SRCR1, SRSTCLR1, Bit29 */
#define CPG_SR1_ANVC0                   128u     /* SRCR1, SRSTCLR1, Bit28 */
#define CPG_SR1_ANSP0                   126u     /* SRCR1, SRSTCLR1, Bit26 */
#define CPG_SR1_ANRT                    125u     /* SRCR1, SRSTCLR1, Bit25 */
#define CPG_SR1_ANPV0                   124u     /* SRCR1, SRSTCLR1, Bit24 */
#define CPG_SR1_ADVFSC                  123u     /* SRCR1, SRSTCLR1, Bit23 */
#define CPG_SR1_ADG                     122u     /* SRCR1, SRSTCLR1, Bit22 */
#define CPG_SR1_SPMI                    120u     /* SRCR1, SRSTCLR1, Bit20 */
#define CPG_SR1_IMPSLV                  118u     /* SRCR1, SRSTCLR1, Bit18 */
#define CPG_SR1_IMPDTA                  116u     /* SRCR1, SRSTCLR1, Bit16 */
#define CPG_SR1_AXIMP0_AXRS             113u     /* SRCR1, SRSTCLR1, Bit13 aximp0:axrs */
#define CPG_SR1_AXIMP0                  112u     /* SRCR1, SRSTCLR1, Bit12 */
#define CPG_SR1_ANIMP1                  111u     /* SRCR1, SRSTCLR1, Bit11 */
#define CPG_SR1_ANIMP0                  110u     /* SRCR1, SRSTCLR1, Bit10 */
#define CPG_SR1_IMPDMAC1                105u     /* SRCR1, SRSTCLR1, Bit5 */
#define CPG_SR1_IMP3                    104u     /* SRCR1, SRSTCLR1, Bit4 */
#define CPG_SR1_IMP2                    103u     /* SRCR1, SRSTCLR1, Bit3 */

#define CPG_SR2_AXSM_AXRS               231u     /* SRCR2, SRSTCLR2, Bit31 axsm:axrs */
#define CPG_SR2_AXSD0_1                 230u     /* SRCR2, SRSTCLR2, Bit30 axsd0 */
#define CPG_SR2_AXSD0_2                 229u     /* SRCR2, SRSTCLR2, Bit29 axsd0 */
#define CPG_SR2_AXSC                    228u     /* SRCR2, SRSTCLR2, Bit28 */
#define CPG_SR2_AXRS_AXSC               227u     /* SRCR2, SRSTCLR2, Bit27 axrs:axsc */
#define CPG_SR2_AXRT_AXRS               225u     /* SRCR2, SRSTCLR2, Bit25 axrt:axrs */
#define CPG_SR2_AXRT                    224u     /* SRCR2, SRSTCLR2, Bit24 */
#define CPG_SR2_AXRD_1                  223u     /* SRCR2, SRSTCLR2, Bit23 axrd */
#define CPG_SR2_AXRD_2                  222u     /* SRCR2, SRSTCLR2, Bit22 axrd */
#define CPG_SR2_AXRC_1                  221u     /* SRCR2, SRSTCLR2, Bit21 axrc */
#define CPG_SR2_AXRC_2                  220u     /* SRCR2, SRSTCLR2, Bit20 axrc */
#define CPG_SR2_AXRS_AXPV               219u     /* SRCR2, SRSTCLR2, Bit19 axrs:axpv */
#define CPG_SR2_AXPV                    218u     /* SRCR2, SRSTCLR2, Bit18 */
#define CPG_SR2_ANMM_AXMM               217u     /* SRCR2, SRSTCLR2, Bit17 anmm:axmm */
#define CPG_SR2_AVB2                    213u     /* SRCR2, SRSTCLR2, Bit13 */
#define CPG_SR2_AVB1                    212u     /* SRCR2, SRSTCLR2, Bit12 */
#define CPG_SR2_AVB0                    211u     /* SRCR2, SRSTCLR2, Bit11 */
#define CPG_SR2_ARMREG                  210u     /* SRCR2, SRSTCLR2, Bit10 */
#define CPG_SR2_ARMM                    209u     /* SRCR2, SRSTCLR2, Bit9 */
#define CPG_SR2_APSP3_1                 208u     /* SRCR2, SRSTCLR2, Bit8 apsp3 */
#define CPG_SR2_APSP3_2                 207u     /* SRCR2, SRSTCLR2, Bit7 apsp3 */
#define CPG_SR2_APSP4_1                 206u     /* SRCR2, SRSTCLR2, Bit6 apsp4 */
#define CPG_SR2_APSP4_2                 205u     /* SRCR2, SRSTCLR2, Bit5 apsp4 */
#define CPG_SR2_APRT0_1                 204u     /* SRCR2, SRSTCLR2, Bit4 aprt0 */
#define CPG_SR2_APRT0_2                 203u     /* SRCR2, SRSTCLR2, Bit3 aprt0 */
#define CPG_SR2_ANVIP2                  202u     /* SRCR2, SRSTCLR2, Bit2 */
#define CPG_SR2_ANVIP1                  201u     /* SRCR2, SRSTCLR2, Bit1 */
#define CPG_SR2_ANVIP0                  200u     /* SRCR2, SRSTCLR2, Bit0 */

#define CPG_SR3_CSITOP0                 331u     /* SRCR3, SRSTCLR3, Bit31 */
#define CPG_SR3_CSDBGPAP                330u     /* SRCR3, SRSTCLR3, Bit30 */
#define CPG_SR3_CR0                     329u     /* SRCR3, SRSTCLR3, Bit29 */
#define CPG_SR3_CANFD                   328u     /* SRCR3, SRSTCLR3, Bit28 */
#define CPG_SR3_AXVIP1_1                326u     /* SRCR3, SRSTCLR3, Bit26 axvip1 */
#define CPG_SR3_AXVIP1_2                325u     /* SRCR3, SRSTCLR3, Bit25 axvip1 */
#define CPG_SR3_AXVIP0_AXRS             323u     /* SRCR3, SRSTCLR3, Bit23 axvip0:axrs */
#define CPG_SR3_AXVIP0                  322u     /* SRCR3, SRSTCLR3, Bit22 */
#define CPG_SR3_AXVIFF                  321u     /* SRCR3, SRSTCLR3, Bit21 */
#define CPG_SR3_AXVI1                   318u     /* SRCR3, SRSTCLR3, Bit18 */
#define CPG_SR3_AXVI1_AXRS              317u     /* SRCR3, SRSTCLR3, Bit17 axvi1:axrs */
#define CPG_SR3_AXVI                    316u     /* SRCR3, SRSTCLR3, Bit16 */
#define CPG_SR3_AXVC_AXRS               312u     /* SRCR3, SRSTCLR3, Bit12 axvc:axrs */
#define CPG_SR3_AXVC                    311u     /* SRCR3, SRSTCLR3, Bit11 */
#define CPG_SR3_AXTBH                   309u     /* SRCR3, SRSTCLR3, Bit9 */
#define CPG_SR3_AXSTM                   308u     /* SRCR3, SRSTCLR3, Bit8 */
#define CPG_SR3_AXSTM_AXRS              307u     /* SRCR3, SRSTCLR3, Bit7 axstm:axrs */
#define CPG_SR3_AXRS_AXSP0              304u     /* SRCR3, SRSTCLR3, Bit4 axrs:axsp0 */
#define CPG_SR3_AXSP0                   303u     /* SRCR3, SRSTCLR3, Bit3 */
#define CPG_SR3_AXSN                    302u     /* SRCR3, SRSTCLR3, Bit2 */
#define CPG_SR3_AXRS_AXSN               301u     /* SRCR3, SRSTCLR3, Bit1 axrs:axsn */
#define CPG_SR3_AXSM                    300u     /* SRCR3, SRSTCLR3, Bit0 */

#define CPG_SR4_DSITXLINK1              416u     /* SRCR4, SRSTCLR4, Bit16 */
#define CPG_SR4_DSITXLINK0              415u     /* SRCR4, SRSTCLR4, Bit15 */
#define CPG_SR4_DOC2CH                  414u     /* SRCR4, SRSTCLR4, Bit14 */
#define CPG_SR4_DIS0                    411u     /* SRCR4, SRSTCLR4, Bit11 */
#define CPG_SR4_DDR3_2                  410u     /* SRCR4, SRSTCLR4, Bit10 ddr3:ddr2 */
#define CPG_SR4_DDR1_0                  409u     /* SRCR4, SRSTCLR4, Bit9 ddr1:ddr0 */
#define CPG_SR4_CSITOP1                 400u     /* SRCR4, SRSTCLR4, Bit0 */

#define CPG_SR5_INTAP                   531u     /* SRCR5, SRSTCLR5, Bit31 */
#define CPG_SR5_IMS1                    530u     /* SRCR5, SRSTCLR5, Bit30 */
#define CPG_SR5_IMS0                    529u     /* SRCR5, SRSTCLR5, Bit29 */
#define CPG_SR5_IMR2                    527u     /* SRCR5, SRSTCLR5, Bit27 */
#define CPG_SR5_IMR1                    526u     /* SRCR5, SRSTCLR5, Bit26 */
#define CPG_SR5_IMR0                    525u     /* SRCR5, SRSTCLR5, Bit25 */
#define CPG_SR5_I2C5                    523u     /* SRCR5, SRSTCLR5, Bit23 */
#define CPG_SR5_I2C4                    522u     /* SRCR5, SRSTCLR5, Bit22 */
#define CPG_SR5_I2C3                    521u     /* SRCR5, SRSTCLR5, Bit21 */
#define CPG_SR5_I2C2                    520u     /* SRCR5, SRSTCLR5, Bit20 */
#define CPG_SR5_I2C1                    519u     /* SRCR5, SRSTCLR5, Bit19 */
#define CPG_SR5_I2C0                    518u     /* SRCR5, SRSTCLR5, Bit18 */
#define CPG_SR5_HSCIF3                  517u     /* SRCR5, SRSTCLR5, Bit17 */
#define CPG_SR5_HSCIF2                  516u     /* SRCR5, SRSTCLR5, Bit16 */
#define CPG_SR5_HSCIF1                  515u     /* SRCR5, SRSTCLR5, Bit15 */
#define CPG_SR5_HSCIF0                  514u     /* SRCR5, SRSTCLR5, Bit14 */
#define CPG_SR5_FRAY00                  513u     /* SRCR5, SRSTCLR5, Bit13 */
#define CPG_SR5_FCPVD1                  509u     /* SRCR5, SRSTCLR5, Bit9 */
#define CPG_SR5_FCPVD0                  508u     /* SRCR5, SRSTCLR5, Bit8 */
#define CPG_SR5_FCPCS                   507u     /* SRCR5, SRSTCLR5, Bit7 */

#define CPG_SR6_RTDM1                   631u     /* SRCR6, SRSTCLR6, Bit31 */
#define CPG_SR6_RTDM0                   630u     /* SRCR6, SRSTCLR6, Bit30 */
#define CPG_SR6_RPC                     629u     /* SRCR6, SRSTCLR6, Bit29 */
#define CPG_SR6_PWM                     628u     /* SRCR6, SRSTCLR6, Bit28 */
#define CPG_SR6_PCIE1                   625u     /* SRCR6, SRSTCLR6, Bit25 */
#define CPG_SR6_PCIE0                   624u     /* SRCR6, SRSTCLR6, Bit24 */
#define CPG_SR6_MSI5                    623u     /* SRCR6, SRSTCLR6, Bit23 */
#define CPG_SR6_MSI4                    622u     /* SRCR6, SRSTCLR6, Bit22 */
#define CPG_SR6_MSI3                    621u     /* SRCR6, SRSTCLR6, Bit21 */
#define CPG_SR6_MSI2                    620u     /* SRCR6, SRSTCLR6, Bit20 */
#define CPG_SR6_MSI1                    619u     /* SRCR6, SRSTCLR6, Bit19 */
#define CPG_SR6_MSI0                    618u     /* SRCR6, SRSTCLR6, Bit18 */
#define CPG_SR6_IVCP1E                  616u     /* SRCR6, SRSTCLR6, Bit16 */
#define CPG_SR6_ISPCS1                  613u     /* SRCR6, SRSTCLR6, Bit13 */
#define CPG_SR6_ISPCS0                  612u     /* SRCR6, SRSTCLR6, Bit12 */
#define CPG_SR6_IRQC                    611u     /* SRCR6, SRSTCLR6, Bit11 */
#define CPG_SR6_INTTP                   608u     /* SRCR6, SRSTCLR6, Bit8 */
#define CPG_SR6_IPC                     600u     /* SRCR6, SRSTCLR6, Bit0 */

#define CPG_SR7_VIN01                   731u     /* SRCR7, SRSTCLR7, Bit31 */
#define CPG_SR7_VIN00                   730u     /* SRCR7, SRSTCLR7, Bit30 */
#define CPG_SR7_VCPL4                   729u     /* SRCR7, SRSTCLR7, Bit29 */
#define CPG_SR7_TPU0                    718u     /* SRCR7, SRSTCLR7, Bit18 */
#define CPG_SR7_TMU4                    717u     /* SRCR7, SRSTCLR7, Bit17 */
#define CPG_SR7_TMU3                    716u     /* SRCR7, SRSTCLR7, Bit16 */
#define CPG_SR7_TMU2                    715u     /* SRCR7, SRSTCLR7, Bit15 */
#define CPG_SR7_TMU1                    714u     /* SRCR7, SRSTCLR7, Bit14 */
#define CPG_SR7_TMU0                    713u     /* SRCR7, SRSTCLR7, Bit13 */
#define CPG_SR7_SYDM2                   710u     /* SRCR7, SRSTCLR7, Bit10 */
#define CPG_SR7_SYDM1                   709u     /* SRCR7, SRSTCLR7, Bit9 */
#define CPG_SR7_SDHI0                   707u     /* SRCR7, SRSTCLR7, Bit7 */
#define CPG_SR7_SCIF4                   706u     /* SRCR7, SRSTCLR7, Bit6 */
#define CPG_SR7_SCIF3                   705u     /* SRCR7, SRSTCLR7, Bit5 */
#define CPG_SR7_SCIF2                   704u     /* SRCR7, SRSTCLR7, Bit4 */
#define CPG_SR7_SCIF1                   703u     /* SRCR7, SRSTCLR7, Bit3 */
#define CPG_SR7_SCIF0                   702u     /* SRCR7, SRSTCLR7, Bit2 */
#define CPG_SR7_RTDM3                   701u     /* SRCR7, SRSTCLR7, Bit1 */
#define CPG_SR7_RTDM2                   700u     /* SRCR7, SRSTCLR7, Bit0 */

#define CPG_SR8_VSPD1                   831u     /* SRCR8, SRSTCLR8, Bit31 */
#define CPG_SR8_VSPD0                   830u     /* SRCR8, SRSTCLR8, Bit30 */
#define CPG_SR8_VIN17                   813u     /* SRCR8, SRSTCLR8, Bit13 */
#define CPG_SR8_VIN16                   812u     /* SRCR8, SRSTCLR8, Bit12 */
#define CPG_SR8_VIN15                   811u     /* SRCR8, SRSTCLR8, Bit11 */
#define CPG_SR8_VIN14                   810u     /* SRCR8, SRSTCLR8, Bit10 */
#define CPG_SR8_VIN13                   809u     /* SRCR8, SRSTCLR8, Bit9 */
#define CPG_SR8_VIN12                   808u     /* SRCR8, SRSTCLR8, Bit8 */
#define CPG_SR8_VIN11                   807u     /* SRCR8, SRSTCLR8, Bit7 */
#define CPG_SR8_VIN10                   806u     /* SRCR8, SRSTCLR8, Bit6 */
#define CPG_SR8_VIN07                   805u     /* SRCR8, SRSTCLR8, Bit5 */
#define CPG_SR8_VIN06                   804u     /* SRCR8, SRSTCLR8, Bit4 */
#define CPG_SR8_VIN05                   803u     /* SRCR8, SRSTCLR8, Bit3 */
#define CPG_SR8_VIN04                   802u     /* SRCR8, SRSTCLR8, Bit2 */
#define CPG_SR8_VIN03                   801u     /* SRCR8, SRSTCLR8, Bit1 */
#define CPG_SR8_VIN02                   800u     /* SRCR8, SRSTCLR8, Bit0 */

#define CPG_SR9_UCMT                    920u     /* SRCR9, SRSTCLR9, Bit20 */
#define CPG_SR9_TSC4_1                  919u     /* SRCR9, SRSTCLR9, Bit19 sc4:tsc3:tsc2:tsc1 */
#define CPG_SR9_PFC3                    918u     /* SRCR9, SRSTCLR9, Bit18 */
#define CPG_SR9_PFC2                    917u     /* SRCR9, SRSTCLR9, Bit17 */
#define CPG_SR9_PFC1                    916u     /* SRCR9, SRSTCLR9, Bit16 */
#define CPG_SR9_PFC0                    915u     /* SRCR9, SRSTCLR9, Bit15 */
#define CPG_SR9_CMT3                    913u     /* SRCR9, SRSTCLR9, Bit13 */
#define CPG_SR9_CMT2                    912u     /* SRCR9, SRSTCLR9, Bit12 */
#define CPG_SR9_CMT1                    911u     /* SRCR9, SRSTCLR9, Bit11 */
#define CPG_SR9_CMT0                    910u     /* SRCR9, SRSTCLR9, Bit10 */
#define CPG_SR9_WDT1_0                  907u     /* SRCR9, SRSTCLR9, Bit7 wdt1:wdt0 */
#define CPG_SR9_WCRC3                   906u     /* SRCR9, SRSTCLR9, Bit6 */
#define CPG_SR9_WCRC2                   905u     /* SRCR9, SRSTCLR9, Bit5 */
#define CPG_SR9_WCRC1                   904u     /* SRCR9, SRSTCLR9, Bit4 */
#define CPG_SR9_WCRC0                   903u     /* SRCR9, SRSTCLR9, Bit3 */

#define CPG_SR10_VSPX1                  1029u    /* SRCR10, SRSTCLR10, Bit29 */
#define CPG_SR10_VSPX0                  1028u    /* SRCR10, SRSTCLR10, Bit28 */
#define CPG_SR10_RGXC                   1024u    /* SRCR10, SRSTCLR10, Bit24 */
#define CPG_SR10_RGXB                   1023u    /* SRCR10, SRSTCLR10, Bit23 */

#define CPG_SR11_SWDT1_0                1128u    /* SRCR11, SRSTCLR11, Bit28 swdt1:swdt0 */
#define CPG_SR11_RGXPVC                 1126u    /* SRCR11, SRSTCLR11, Bit26 */
#define CPG_SR11_RGXPVDE                1125u    /* SRCR11, SRSTCLR11, Bit25 */
#define CPG_SR11_PCIE1                  1122u    /* SRCR11, SRSTCLR11, Bit22 */
#define CPG_SR11_PCIE0                  1121u    /* SRCR11, SRSTCLR11, Bit21 */
#define CPG_SR11_INTAP                  1119u    /* SRCR11, SRSTCLR11, Bit19 */
#define CPG_SR11_FRAY01                 1118u    /* SRCR11, SRSTCLR11, Bit18 */
#define CPG_SR11_AXVI                   1108u    /* SRCR11, SRSTCLR11, Bit8 */
#define CPG_SR11_AULK4P                 1107u    /* SRCR11, SRSTCLR11, Bit7 */
#define CPG_SR11_AULK2P                 1106u    /* SRCR11, SRSTCLR11, Bit6 */
#define CPG_SR11_FCPVX1                 1101u    /* SRCR11, SRSTCLR11, Bit1 */
#define CPG_SR11_FCPVX0                 1100u    /* SRCR11, SRSTCLR11, Bit0 */

#define CPG_SR12_FSO                    1230u    /* SRCR12, SRSTCLR12, Bit30 */
#define CPG_SR12_SCMT                   1222u    /* SRCR12, SRSTCLR12, Bit22 */
#define CPG_SR12_PFC3_1                 1221u    /* SRCR12, SRSTCLR12, Bit21 pfc3 */
#define CPG_SR12_PFC3_2                 1220u    /* SRCR12, SRSTCLR12, Bit20 pfc3 */
#define CPG_SR12_PFC3_3                 1219u    /* SRCR12, SRSTCLR12, Bit19 pfc3 */
#define CPG_SR12_PFC2_1                 1218u    /* SRCR12, SRSTCLR12, Bit18 pfc2 */
#define CPG_SR12_PFC2_2                 1217u    /* SRCR12, SRSTCLR12, Bit17 pfc2 */
#define CPG_SR12_PFC2_3                 1216u    /* SRCR12, SRSTCLR12, Bit16 pfc2 */
#define CPG_SR12_PFC1_1                 1215u    /* SRCR12, SRSTCLR12, Bit15 pfc1 */
#define CPG_SR12_PFC1_2                 1214u    /* SRCR12, SRSTCLR12, Bit14 pfc1 */
#define CPG_SR12_PFC1_3                 1213u    /* SRCR12, SRSTCLR12, Bit13 pfc1 */
#define CPG_SR12_PFC0_1                 1212u    /* SRCR12, SRSTCLR12, Bit12 pfc0 */
#define CPG_SR12_PFC0_2                 1211u    /* SRCR12, SRSTCLR12, Bit11 pfc0 */
#define CPG_SR12_PFC0_3                 1210u    /* SRCR12, SRSTCLR12, Bit10 pfc0 */
#define CPG_SR12_WWDT9                  1209u    /* SRCR12, SRSTCLR12, Bit9 */
#define CPG_SR12_WWDT8                  1208u    /* SRCR12, SRSTCLR12, Bit8 */
#define CPG_SR12_WWDT7                  1207u    /* SRCR12, SRSTCLR12, Bit7 */
#define CPG_SR12_WWDT6                  1206u    /* SRCR12, SRSTCLR12, Bit6 */
#define CPG_SR12_WWDT5                  1205u    /* SRCR12, SRSTCLR12, Bit5 */
#define CPG_SR12_WWDT4                  1204u    /* SRCR12, SRSTCLR12, Bit4 */
#define CPG_SR12_WWDT3                  1203u    /* SRCR12, SRSTCLR12, Bit3 */
#define CPG_SR12_WWDT2                  1202u    /* SRCR12, SRSTCLR12, Bit2 */
#define CPG_SR12_WWDT1                  1201u    /* SRCR12, SRSTCLR12, Bit1 */
#define CPG_SR12_WWDT0                  1200u    /* SRCR12, SRSTCLR12, Bit0 */

#define CPG_SR13_WWDT9                  1327u    /* SRCR13, SRSTCLR13, Bit27 */
#define CPG_SR13_WWDT8                  1326u    /* SRCR13, SRSTCLR13, Bit26 */
#define CPG_SR13_WWDT7                  1325u    /* SRCR13, SRSTCLR13, Bit25 */
#define CPG_SR13_WWDT6                  1324u    /* SRCR13, SRSTCLR13, Bit24 */
#define CPG_SR13_WWDT5                  1323u    /* SRCR13, SRSTCLR13, Bit23 */
#define CPG_SR13_WWDT4                  1322u    /* SRCR13, SRSTCLR13, Bit22 */
#define CPG_SR13_WWDT3                  1321u    /* SRCR13, SRSTCLR13, Bit21 */
#define CPG_SR13_WWDT2                  1320u    /* SRCR13, SRSTCLR13, Bit20 */
#define CPG_SR13_WWDT1                  1319u    /* SRCR13, SRSTCLR13, Bit19 */
#define CPG_SR13_WWDT0                  1318u    /* SRCR13, SRSTCLR13, Bit18 */
#define CPG_SR13_DBE                    1311u    /* SRCR13, SRSTCLR13, Bit11 */
#define CPG_SR13_MTI                    1304u    /* SRCR13, SRSTCLR13, Bit4 */

#define CPG_SR14_CKMMM                  1412u    /* SRCR14, SRSTCLR14, Bit12 */
#define CPG_SR14_CKMIR                  1411u    /* SRCR14, SRSTCLR14, Bit11 */
#define CPG_SR14_CKMVIP                 1410u    /* SRCR14, SRSTCLR14, Bit10 */
#define CPG_SR14_CKMVC                  1409u    /* SRCR14, SRSTCLR14, Bit9 */
#define CPG_SR14_CKMVIO                 1408u    /* SRCR14, SRSTCLR14, Bit8 */
#define CPG_SR14_CKMPE0                 1406u    /* SRCR14, SRSTCLR14, Bit6 */
#define CPG_SR14_CKMRT                  1405u    /* SRCR14, SRSTCLR14, Bit5 */
#define CPG_SR14_CKM3DG                 1404u    /* SRCR14, SRSTCLR14, Bit4 */

#define CPG_SR15_APVI4_1                1523u    /* SRCR15, SRSTCLR15, Bit23 apvi4 */
#define CPG_SR15_APVI4_2                1522u    /* SRCR15, SRSTCLR15, Bit22 apvi4 */
#define CPG_SR15_APVC1_1                1521u    /* SRCR15, SRSTCLR15, Bit21 apvc1 */
#define CPG_SR15_APVC1_2                1520u    /* SRCR15, SRSTCLR15, Bit20 apvc1 */
#define CPG_SR15_CKMDSP_CKMIR           1519u    /* SRCR15, SRSTCLR15, Bit19 ckmdsp ckmir */
#define CPG_SR15_CKMCNR_CKMIR           1518u    /* SRCR15, SRSTCLR15, Bit18 ckmcnr ckmir */
#define CPG_SR15_CKM                    1517u    /* SRCR15, SRSTCLR15, Bit17 */
#define CPG_SR15_CKMHSC                 1516u    /* SRCR15, SRSTCLR15, Bit16 */
#define CPG_SR15_CKMCR52                1515u    /* SRCR15, SRSTCLR15, Bit15 */
#define CPG_SR15_ANHC                   1503u    /* SRCR15, SRSTCLR15, Bit3 */
#define CPG_SR15_AXHC_AXRS              1502u    /* SRCR15, SRSTCLR15, Bit2 axhc:axrs */
#define CPG_SR15_AXHC                   1501u    /* SRCR15, SRSTCLR15, Bit1 */

#define CPG_SR27_CSBRG_IR_A2            2729u    /* SRCR27, SRSTCLR27, Bit29 */
#define CPG_SR27_CSBRG_IR_A3            2728u    /* SRCR27, SRSTCLR27, Bit28 */
#define CPG_SR27_TSN                    2723u    /* SRCR27, SRSTCLR27, Bit23 */
#define CPG_SR27_IMPSDMAC1              2713u    /* SRCR27, SRSTCLR27, Bit13 */
#define CPG_SR27_IMPSDMAC0              2712u    /* SRCR27, SRSTCLR27, Bit12 */
#define CPG_SR27_CVE3                   2709u    /* SRCR27, SRSTCLR27, Bit9 */
#define CPG_SR27_CVE2                   2708u    /* SRCR27, SRSTCLR27, Bit8 */
#define CPG_SR27_CVE1                   2707u    /* SRCR27, SRSTCLR27, Bit7 */
#define CPG_SR27_CVE0                   2706u    /* SRCR27, SRSTCLR27, Bit6 */

#define CPG_SR28_VDSP1_REG              2831u    /* SRCR28, SRSTCLR28, Bit31 */
#define CPG_SR28_VDSP1_CSB              2830u    /* SRCR28, SRSTCLR28, Bit30 */
#define CPG_SR28_VDSP0_SYSPO            2828u    /* SRCR28, SRSTCLR28, Bit28 */
#define CPG_SR28_VDSP0_APBDBG           2827u    /* SRCR28, SRSTCLR28, Bit27 */
#define CPG_SR28_VDSP0_OCEM             2826u    /* SRCR28, SRSTCLR28, Bit26 */
#define CPG_SR28_VDSP0_CORE             2825u    /* SRCR28, SRSTCLR28, Bit25 */
#define CPG_SR28_VDSP0_SYS              2824u    /* SRCR28, SRSTCLR28, Bit24 */
#define CPG_SR28_VDSP0_GLOBAL           2823u    /* SRCR28, SRSTCLR28, Bit23 */
#define CPG_SR28_VDSP0_REG              2822u    /* SRCR28, SRSTCLR28, Bit22 */
#define CPG_SR28_VDSP0_CSB              2821u    /* SRCR28, SRSTCLR28, Bit21 */
#define CPG_SR28_DSC                    2819u    /* SRCR28, SRSTCLR28, Bit19 */
#define CPG_SR28_FCPRC                  2817u    /* SRCR28, SRSTCLR28, Bit17 */
#define CPG_SR28_PAPSDMA                2807u    /* SRCR28, SRSTCLR28, Bit7 */
#define CPG_SR28_PAPTOP                 2806u    /* SRCR28, SRSTCLR28, Bit6 */
#define CPG_SR28_PAPBUS                 2805u    /* SRCR28, SRSTCLR28, Bit5 */
#define CPG_SR28_VDSP3_BUS              2804u    /* SRCR28, SRSTCLR28, Bit4 */
#define CPG_SR28_VDSP2_BUS              2803u    /* SRCR28, SRSTCLR28, Bit3 */
#define CPG_SR28_VDSP1_BUS              2802u    /* SRCR28, SRSTCLR28, Bit2 */
#define CPG_SR28_VDSP0_BUS              2801u    /* SRCR28, SRSTCLR28, Bit1 */

#define CPG_SR29_SSI                    2927u    /* SRCR29, SRSTCLR29, Bit27 */
#define CPG_SR29_SSIU                   2926u    /* SRCR29, SRSTCLR29, Bit26 */
#define CPG_SR29_VDSP3_SYSPO            2923u    /* SRCR29, SRSTCLR29, Bit23 */
#define CPG_SR29_VDSP3_APBDBG           2922u    /* SRCR29, SRSTCLR29, Bit22 */
#define CPG_SR29_VDSP3_OCEM             2921u    /* SRCR29, SRSTCLR29, Bit21 */
#define CPG_SR29_VDSP3_CORE             2920u    /* SRCR29, SRSTCLR29, Bit20 */
#define CPG_SR29_VDSP3_SYS              2919u    /* SRCR29, SRSTCLR29, Bit19 */
#define CPG_SR29_VDSP3_GLOBAL           2918u    /* SRCR29, SRSTCLR29, Bit18 */
#define CPG_SR29_VDSP3_REG              2917u    /* SRCR29, SRSTCLR29, Bit17 */
#define CPG_SR29_VDSP3_CSB              2916u    /* SRCR29, SRSTCLR29, Bit16 */
#define CPG_SR29_VDSP2_SYSPO            2914u    /* SRCR29, SRSTCLR29, Bit14 */
#define CPG_SR29_VDSP2_APBDBG           2913u    /* SRCR29, SRSTCLR29, Bit13 */
#define CPG_SR29_VDSP2_OCEM             2912u    /* SRCR29, SRSTCLR29, Bit12 */
#define CPG_SR29_VDSP2_CORE             2911u    /* SRCR29, SRSTCLR29, Bit11 */
#define CPG_SR29_VDSP2_SYS              2910u    /* SRCR29, SRSTCLR29, Bit10 */
#define CPG_SR29_VDSP2_GLOBAL           2909u    /* SRCR29, SRSTCLR29, Bit9 */
#define CPG_SR29_VDSP2_REG              2908u    /* SRCR29, SRSTCLR29, Bit8 */
#define CPG_SR29_VDSP2_CSB              2907u    /* SRCR29, SRSTCLR29, Bit7 */
#define CPG_SR29_VDSP1_SYSPO            2905u    /* SRCR29, SRSTCLR29, Bit5 */
#define CPG_SR29_VDSP1_APBDBG           2904u    /* SRCR29, SRSTCLR29, Bit4 */
#define CPG_SR29_VDSP1_OCEM             2903u    /* SRCR29, SRSTCLR29, Bit3 */
#define CPG_SR29_VDSP1_CORE             2902u    /* SRCR29, SRSTCLR29, Bit2 */
#define CPG_SR29_VDSP1_SYS              2901u    /* SRCR29, SRSTCLR29, Bit1 */
#define CPG_SR29_VDSP1_GLOBAL           2900u    /* SRCR29, SRSTCLR29, Bit0 */
#endif /* CONFIG_SOC_SERIES_RCAR_GEN4 */

#endif /* ZEPHYR_INCLUDE_DRIVERS_CLOCK_CONTROL_RCAR_CLOCK_CONTROL_H_ */
