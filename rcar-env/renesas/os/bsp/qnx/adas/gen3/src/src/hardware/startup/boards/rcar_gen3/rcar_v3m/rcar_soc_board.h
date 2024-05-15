/*
* $QNXLicenseC:
* Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
* Copyright 2021, Renesas Electronics Corporation.
*
* You must obtain a written license from and pay applicable license fees to QNX
* Software Systems before you may reproduce, modify or distribute this software,
* or any work that includes all or part of this software. Free development
* licenses are available for evaluation and non-commercial purposes. For more
* information visit http://licensing.qnx.com or email licensing@qnx.com.
*
* This file may contain contributions from others. Please review this entire
* file for other proprietary rights or license notices, as well as the QNX
* Development Suite License Guide at http://licensing.qnx.com/license-guide/
* for other information.
* $
*/

#ifndef __RCAR_SOC_BOARD_V3M_H_INCLUDED
#define __RCAR_SOC_BOARD_V3M_H_INCLUDED

#define USE_DISPLAY             0
#define USE_SCIF                0
#define USE_HSCIF               1

#define USE_SCIF_HSICF_FOR_UART USE_SCIF
#define USE_DISPLAY_OR_HSCIF0   USE_DISPLAY

// SCIF0 SCIF_CLCK pin is shared and CAN clock pin
#define USE_SCIF_CLOCK              0
#define USE_CAN_CLOCK               1
#define USE_CLOCK_PIN_FOR_CAN_SCIF  USE_SCIF_CLOCK


extern void config_dmac(rcar_module *module);
extern void config_impx5(rcar_module *module);

rcar_config v3m_eagle_config[] = {
    { // Timer
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(1), .clk.bits = BIT(25) }, /* TMU0 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(24) }, /* TMU1 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(23) }, /* TMU2 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(22) }, /* TMU3 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(21) }, /* TMU4 */
            { { 0 } }
        }
    },

#if USE_SCIF_HSICF_FOR_UART == USE_SCIF
    { // SCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(7) }, /* SCIF0 */
#ifdef USE_SCIF1
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2),  .clk.bits = BIT(6) }, /* SCIF1 */
#endif //USE_SCIF1
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 4 }, /* RX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 4 }, /* TX0  */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 16, .ipsr.val = 4 }, /* SCIF_CLK_B  */
#endif
#ifdef USE_SCIF1
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 8,  .ipsr.val = 4 }, /* TX1_B */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 12, .ipsr.val = 4 }, /* RX1_B */
#endif //USE_SCIF1
            { { 0 } }
        },
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(10),   .val = 0x400 },         /* Select SCIF_CLK_B pin for SCIF0 */
            { 0 }
        }        
#endif
    },
#else
#if USE_DISPLAY_OR_HSCIF0 == USE_HSCIF
    { // HSCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(20) }, /* HSCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 1 }, /* HRX0  */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 12, .ipsr.val = 1 }, /* HTX0  */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(10),   .val = 0x400 },         /* Select SCIF_CLK_B pin */
            { 0 }
        }        
    },
#endif
#endif //#if  USE_SCIF_HSICF_FOR_UART == USE_SCIF
    { // GPIO
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(7)  }, /* GPIO5 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(8)  }, /* GPIO4 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(9)  }, /* GPIO3 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(10) }, /* GPIO2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(11) }, /* GPIO1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(12) }, /* GPIO0 */
            { { 0 } }
        },
    },

    { // ETHERNET
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(12) }, /* EAVB-IF */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BITS(1,15), .gpsr.val = GPSR_PF },
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(17), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_IN,  .gpio.val = 0 }, /* Int pin */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* Reset pin */
            { { 0 } }
        }
    },

    { // I2C
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) },  /* I2C-IF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(30) },  /* I2C-IF1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(29) },  /* I2C-IF2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(28) },  /* I2C-IF3 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(27) },  /* I2C-IF4 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 4,  .ipsr.val = 0 }, /* SCL0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 8,  .ipsr.val = 0 }, /* SDA0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 12, .ipsr.val = 0 }, /* SCL1 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 16, .ipsr.val = 0 }, /* SDA1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 20, .ipsr.val = 2 }, /* SDA3_A */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 24, .ipsr.val = 2 }, /* SCL3_A */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 1 }, /* SCL4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 1 }, /* SDA4 */
            { { 0 } }
        }
    },

    { // MMCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(14) }, /* SD-IF */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_SD0CKCR, .bits = 0xFFFFFFFF, .val = 0x00000240 }, /* SD0 clk */
            { 0 } 
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 20, .ipsr.val = 4 }, /* MMC_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 24, .ipsr.val = 4 }, /* MMC_D0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 28, .ipsr.val = 4 }, /* MMC_D1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 0,  .ipsr.val = 4 }, /* MMC_D2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 4 }, /* MMC_D3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 8,  .ipsr.val = 4 }, /* MMC_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 12, .ipsr.val = 4 }, /* MMC_D4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 4 }, /* MMC_D5 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 20, .ipsr.val = 4 }, /* MMC_D6 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 24, .ipsr.val = 4 }, /* MMC_D7 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 4 }, /* MMC_WP */ 
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 4 }, /* MMC_CD */ 
            { { 0 } }
        },

        .settings = (rcar_pin[])
        {
            { .reg = POCCTRL(1), .bits = BITS(12,23), .val = POC_1_8V },     /* Set SD POCCTRL to 1.8V */
            { .reg = PUEN(1),    .bits = BITS(21,31), .val = PUEN_DISABLE }, /* PUEN1[31:21] = all 0 -> Pull-up/down OFF */
            { .reg = PUEN(2),    .bits = BIT(0),      .val = PUEN_DISABLE }, /* PUEN2[0] = 0 -> Pull-up/down OFF */
            { 0 } 
        }
    },

    { // DMAC
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(18), .base = RCAR_GEN3_SYSDMAC1_BASE   }, /* SYS-DMAC1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(17), .base = RCAR_GEN3_SYSDMAC2_BASE   }, /* SYS-DMAC2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(21), .base = RCAR_GEN3_RTDMAC0_BASE   }, /* RT-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(16), .base = RCAR_GEN3_RTDMAC1_BASE   }, /* RT-DMAC1 */
            { { 0 } }
        },
        .blk_config = config_dmac,
    },

    { // QSPI
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(5), .gpsr.bits = BITS(0,5),  .gpsr.val = GPSR_PF }, // QSPI0
            { .gpsr.reg = GPSR(5), .gpsr.bits = BITS(6,11), .gpsr.val = GPSR_PF }, // QSPI1
            { { 0 } }
        }
    },

    { // Thermal
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(22) }, /* THS */
            { {0} }
        }
    },

    { // Display
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(27) }, /* LVDS */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(24) }, /* DU0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(6), .clk.bits = BIT(23) }, /* VSPD0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(6), .clk.bits = BIT(3)  }, /* FCPV (DU0) */
            { {0} }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 0,  .ipsr.val = 0 }, /* DU_DR2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 4,  .ipsr.val = 0 }, /* DU_DR3 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 8,  .ipsr.val = 0 }, /* DU_DR4 */
#if USE_DISPLAY_OR_HSCIF0 == USE_DISPLAY
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_DR5 */
#endif
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_DR6 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_DR7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 24, .ipsr.val = 0 }, /* DU_DG2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 28, .ipsr.val = 0 }, /* DU_DG3 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 0,  .ipsr.val = 0 }, /* DU_DG4 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 4,  .ipsr.val = 0 }, /* DU_DG5 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 8,  .ipsr.val = 0 }, /* DU_DG6 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_DG7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_DB2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_DB3 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 24, .ipsr.val = 0 }, /* DU_DB4 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 28, .ipsr.val = 0 }, /* DU_DB5 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 0,  .ipsr.val = 0 }, /* DU_DB6 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(17), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 4,  .ipsr.val = 0 }, /* DU_DB7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(18), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 8,  .ipsr.val = 0 }, /* DU_DOTCLKOUT */
#if USE_DISPLAY_OR_HSCIF0 == USE_DISPLAY
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_HSYNC */
#endif
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_VSYNC */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(21), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_ODDF */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_IN, .gpio.val = 0 }, /* HDMI Interrupt */
            { { 0 } }
        },
    },

    { // IMPX5
        .pwr = (rcar_pwr[])
        {
            { .base = PWRONCR(4), .setting = 0x1, .status = PWRSR(4), .power_on = BIT(4) },
            { 0 }
        },
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(30) }, /* IMP-RAM */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(29) }, /* IMP-OCV0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(28) }, /* IMP-OCV1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(27) }, /* IMP-IMP0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(26) }, /* IMP-IMP1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(25) }, /* IMP-IMP2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(24) }, /* IMP-IMP3 */
            { {0} }
        },
        //.blk_config = config_impx5,
    },

    { // Capture
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(16) }, /* CSI40 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(8)  }, /* VIN3 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(9)  }, /* VIN2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(10) }, /* VIN1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(11) }, /* VIN0 */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CSI0CKCR, .bits = 0xFFFFFFFF, .val = 0x1F }, /* CSI0 clock */
            { 0 }
        },
    },

    { /* CANFD/CAN */
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(14) }, /* CANFD */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CANFDCKCR, .bits = 0xFFFFFFFF, .val = 0x00000009 }, /* CANFD clock */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 0,  .ipsr.val = 0 }, /* CANFD0_TX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 4,  .ipsr.val = 0 }, /* CANFD0_RX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 8,  .ipsr.val = 0 }, /* CANFD1_TX */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 12, .ipsr.val = 0 }, /* CANFD1_RX */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_CAN_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 16, .ipsr.val = 0 }, /* CAN_CLK_A */
#endif
            { { 0 } }
        },
         .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(8),   .val = 0x000 },         /* Select CANFD_*_A pin */
            { 0 }
        }
    },
};

rcar_config v3m_sk_config[] = {
    { // Timer
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(1), .clk.bits = BIT(25) }, /* TMU0 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(24) }, /* TMU1 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(23) }, /* TMU2 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(22) }, /* TMU3 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(1), .clk.bits = BIT(21) }, /* TMU4 */
            { { 0 } }
        }
    },

#if USE_SCIF_HSICF_FOR_UART == USE_SCIF
    { // SCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(7) }, /* SCIF0 */
#ifdef USE_SCIF1
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2),  .clk.bits = BIT(6) }, /* SCIF1 */
#endif //USE_SCIF1
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 4 }, /* RX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 4 }, /* TX0  */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 16, .ipsr.val = 4 }, /* SCIF_CLK_B  */
#endif 
#ifdef USE_SCIF1
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 8,  .ipsr.val = 4 }, /* TX1_B */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 12, .ipsr.val = 4 }, /* RX1_B */
#endif //USE_SCIF1
            { { 0 } }
        },
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(10),   .val = 0x400 },         /* Select SCIF_CLK_B pin for SCIF0 */
            { 0 }
        }        
#endif
    },
#else
#if USE_DISPLAY_OR_HSCIF0 == USE_HSCIF0
    { // HSCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(20) }, /* HSCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 1 }, /* HRX0  */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 12, .ipsr.val = 1 }, /* HTX0  */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(10),   .val = 0x400 },          /* Select SCIF_CLK_B pin */
            { { 0 } }
        }        
    },
#endif
#endif //#if  USE_SCIF_HSICF_FOR_UART == USE_SCIF
    { // GPIO
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(7)  }, /* GPIO5 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(8)  }, /* GPIO4 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(9)  }, /* GPIO3 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(10) }, /* GPIO2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(11) }, /* GPIO1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(12) }, /* GPIO0 */
            { { 0 } }
        },
    },

    { // ETHERNET
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(12) }, /* EAVB-IF */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BITS(1,15), .gpsr.val = GPSR_PF },
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(17), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_IN,  .gpio.val = 0 }, /* Int pin */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* Reset pin */
            { { 0 } }
        }
    },

    { // I2C
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) },  /* I2C-IF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(30) },  /* I2C-IF1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(29) },  /* I2C-IF2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(28) },  /* I2C-IF3 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(27) },  /* I2C-IF4 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 4,  .ipsr.val = 0 }, /* SCL0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 8,  .ipsr.val = 0 }, /* SDA0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 12, .ipsr.val = 0 }, /* SCL1 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 16, .ipsr.val = 0 }, /* SDA1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 20, .ipsr.val = 2 }, /* SDA3_A */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 24, .ipsr.val = 2 }, /* SCL3_A */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 1 }, /* SCL4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 1 }, /* SDA4 */
            { { 0 } }
        }
    },

    { // MMCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(14) }, /* SD-IF */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_SD0CKCR, .bits = 0xFFFFFFFF, .val = 0x00000240 }, /* SD0 clk */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 20, .ipsr.val = 4 }, /* MMC_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 24, .ipsr.val = 4 }, /* MMC_D0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 28, .ipsr.val = 4 }, /* MMC_D1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 0,  .ipsr.val = 4 }, /* MMC_D2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 4 }, /* MMC_D3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 8,  .ipsr.val = 4 }, /* MMC_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 12, .ipsr.val = 4 }, /* MMC_D4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 4 }, /* MMC_D5 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 20, .ipsr.val = 4 }, /* MMC_D6 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 24, .ipsr.val = 4 }, /* MMC_D7 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 4 }, /* MMC_WP */ 
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 4 }, /* MMC_CD */ 
            { { 0 } }
        },

        .settings = (rcar_pin[])
        {
            { .reg = POCCTRL(1), .bits = BITS(12,23), .val = POC_1_8V },     /* Set SD POCCTRL to 1.8V */
            { .reg = PUEN(1),    .bits = BITS(21,31), .val = PUEN_DISABLE }, /* PUEN1[31:21] = all 0 -> Pull-up/down OFF */
            { .reg = PUEN(2),    .bits = BIT(0),      .val = PUEN_DISABLE }, /* PUEN2[0] = 0 -> Pull-up/down OFF */
            { 0 }
        }
    },

    { // DMAC
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(18), .base = RCAR_GEN3_SYSDMAC1_BASE   }, /* SYS-DMAC1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(17), .base = RCAR_GEN3_SYSDMAC2_BASE   }, /* SYS-DMAC2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(21), .base = RCAR_GEN3_RTDMAC0_BASE   }, /* RT-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(16), .base = RCAR_GEN3_RTDMAC1_BASE   }, /* RT-DMAC1 */
            { { 0 } }
        },
        .blk_config = config_dmac,
    },

    { // QSPI
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(5), .gpsr.bits = BITS(0,5),  .gpsr.val = GPSR_PF }, // QSPI0
            { .gpsr.reg = GPSR(5), .gpsr.bits = BITS(6,11), .gpsr.val = GPSR_PF }, // QSPI1
            { { 0 } }
        }
    },

    { // Thermal
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(22) }, /* THS */
            { {0} }
        }
    },

    { // Display
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(27) }, /* LVDS */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(24) }, /* DU0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(6), .clk.bits = BIT(23) }, /* VSPD0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(6), .clk.bits = BIT(3)  }, /* FCPV (DU0) */
            { {0} }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 0,  .ipsr.val = 0 }, /* DU_DR2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 4,  .ipsr.val = 0 }, /* DU_DR3 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 8,  .ipsr.val = 0 }, /* DU_DR4 */
#if USE_DISPLAY_OR_HSCIF0 == USE_DISPLAY
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_DR5 */
#endif
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_DR6 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_DR7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 24, .ipsr.val = 0 }, /* DU_DG2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 28, .ipsr.val = 0 }, /* DU_DG3 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 0,  .ipsr.val = 0 }, /* DU_DG4 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 4,  .ipsr.val = 0 }, /* DU_DG5 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 8,  .ipsr.val = 0 }, /* DU_DG6 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_DG7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_DB2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_DB3 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 24, .ipsr.val = 0 }, /* DU_DB4 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(1), .ipsr.bits = 28, .ipsr.val = 0 }, /* DU_DB5 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 0,  .ipsr.val = 0 }, /* DU_DB6 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(17), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 4,  .ipsr.val = 0 }, /* DU_DB7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(18), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 8,  .ipsr.val = 0 }, /* DU_DOTCLKOUT */
#if USE_DISPLAY_OR_HSCIF0 == USE_DISPLAY
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_HSYNC */
#endif
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_VSYNC */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(21), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_ODDF */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_IN, .gpio.val = 0 }, /* HDMI Interrupt */
            { { 0 } }
        },
    },

    { // IMPX5
        .pwr = (rcar_pwr[])
        {
            { .base = PWRONCR(4), .setting = 0x1, .status = PWRSR(4), .power_on = BIT(4) },
            { 0 }
        },
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(30) }, /* IMP-RAM */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(29) }, /* IMP-OCV0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(28) }, /* IMP-OCV1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(27) }, /* IMP-IMP0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(26) }, /* IMP-IMP1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(25) }, /* IMP-IMP2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(24) }, /* IMP-IMP3 */
            { {0} }
        },
        //.blk_config = config_impx5,
    },

    { // Capture
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(16) }, /* CSI40 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(8)  }, /* VIN3 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(9)  }, /* VIN2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(10) }, /* VIN1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(11) }, /* VIN0 */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CSI0CKCR, .bits = 0xFFFFFFFF, .val = 0x1F }, /* CSI0 clock */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(9), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* MAX9286 power up */
            { { 0 } }
        }
    },

    { /* CANFD/CAN */
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(14) }, /* CANFD */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CANFDCKCR, .bits = 0xFFFFFFFF, .val = 0x00000009 }, /* CANFD clock */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 0,  .ipsr.val = 0 }, /* CANFD0_TX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 4,  .ipsr.val = 0 }, /* CANFD0_RX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 8,  .ipsr.val = 0 }, /* CANFD1_TX */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 12, .ipsr.val = 0 }, /* CANFD1_RX */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_CAN_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 16, .ipsr.val = 0 }, /* CAN_CLK_A */
#endif
            { { 0 } }
        },
         .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(8),   .val = 0x000 },         /* Select CANFD_*_A pin */
            { 0 }
        }
    },
};

#endif //__RCAR_SOC_BOARD_V3M_H_INCLUDED
