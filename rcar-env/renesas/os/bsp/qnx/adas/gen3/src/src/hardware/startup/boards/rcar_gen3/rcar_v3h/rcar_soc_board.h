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

#ifndef __RCAR_SOC_BOARD_V3_H_INCLUDED
#define __RCAR_SOC_BOARD_V3_H_INCLUDED

// MISOF3 pins are shared with display
#define USE_DISPLAY                 0
#define USE_MSIOF3                  1
#define USE_DISPLAY_OR_MSIOF3       USE_DISPLAY

// devc (UART) can use SCIF0 or HSCIF0
#define USE_SCIF                    0
#define USE_HSCIF                   1
#define USE_SCIF_HSICF_FOR_UART     USE_SCIF

// SCIF0 SCIF_CLCK pin is shared and CAN clock pin
#define USE_SCIF_CLOCK              0
#define USE_CAN_CLOCK               1
#define USE_CLOCK_PIN_FOR_CAN_SCIF  USE_SCIF_CLOCK

extern void config_dmac(rcar_module *module);
extern void config_impx5(rcar_module *module);

rcar_config v3h_ws10_condor_config[] = {
#if USE_SCIF_HSICF_FOR_UART == USE_SCIF
    { // SCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(7) }, /* SCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 4 }, /* SCL2 -> RX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 4 }, /* SDA2 -> TX0  */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 20, .ipsr.val = 4 }, /* SCIF_CLK_B  */
#endif            
            { { 0 } }
        },
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(9),   .val = 0x200 },         /* Select SCIF_CLK_B pin for SCIF0 */
            { 0 }
        }        
#endif
    },
#else
    { // HSCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(20) }, /* HSCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 5 }, /* SCL2 -> HRX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 5 }, /* SDA2 -> HTX0  */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(9),   .val = 0x200 },         /* Select HRX0_B & HTX0_B pins for HSCIF0 */
            { 0 }
        }        
    },
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
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(13) }, /* GETHER-IF */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BITS(6,21), .gpsr.val = GPSR_PF },
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(22), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(4), .gpio.type = GPIO_OUT, .gpio.val = 1 }, // Reset pin
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(23), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(4), .gpio.type = GPIO_IN,  .gpio.val = 0 }, // Int pin
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(24), .gpsr.val = GPSR_PF },
            { { 0 } }
        }
    },

    { // I2C
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) },   /* I2C-IF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(30) },   /* I2C-IF1 */
            //{ .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(29) },   /* I2C-IF2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(28) },   /* I2C-IF3 */
            //{ .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(27) }, /* I2C-IF4 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 4,  .ipsr.val = 0 }, /* I2C-IF0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 8,  .ipsr.val = 0 }, /* I2C-IF0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 12, .ipsr.val = 0 }, /* I2C-IF1 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 16, .ipsr.val = 0 }, /* I2C-IF1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 0,  .ipsr.val = 0 }, /* I2C-IF3 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(26), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 4,  .ipsr.val = 0 }, /* I2C-IF3 */
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
            { .reg = RCAR_GEN3_CPG_SD0CKCR, .bits = 0xFFFFFFFF, .val = 0x00000004 }, /* SD0 clk */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 16, .ipsr.val = 4 }, /* VI1_DATA0  -> MMC_WP */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 20, .ipsr.val = 4 }, /* VI1_DATA1  -> MMC_CD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 24, .ipsr.val = 4 }, /* VI1_DATA2  -> MMC_DS */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 28, .ipsr.val = 4 }, /* VI1_DATA3  -> MMC_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 0,  .ipsr.val = 4 }, /* VI1_DATA4  -> MMC_D0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 4 }, /* VI1_DATA5  -> MMC_D1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 8,  .ipsr.val = 4 }, /* VI1_DATA6  -> MMC_D2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 12, .ipsr.val = 4 }, /* VI1_DATA7  -> MMC_D3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 4 }, /* VI1_DATA8  -> MMC_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 20, .ipsr.val = 4 }, /* VI1_DATA9  -> MMC_D4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 24, .ipsr.val = 4 }, /* VI1_DATA10 -> MMC_D5 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 4 }, /* VI1_DATA11 -> MMC_D6 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 4 }, /* VI1_FIELD  -> MMC_D7 */
            { { 0 } }
        },

        .settings = (rcar_pin[])
        {
            { .reg = DRVCTRL(7), .bits = BITS(0,7),   .val = 0x33 },         /* IOCTRL7[7:0]    = WP, CD */
            { .reg = DRVCTRL(8), .bits = BITS(0,31),  .val = 0x33333333 },   /* IOCTRL8[31:0]   = SD1_DAT[0:4], SD2_CLK, SD2_DS,SD2_CMD */
            { .reg = DRVCTRL(9), .bits = BITS(20,31), .val = 0x33300000 },   /* IOCTRL9[31:20]  = SD2_DAT[7:5] */
            { .reg = POCCTRL(1), .bits = BITS(11,23), .val = 0 },            /* IOCTRL31[23:11] = all 0 (SD IO voltage=1.8V) */
            { .reg = PUEN(1),    .bits = BITS(29,31), .val = PUEN_DISABLE }, /* PUEN1[31:29]    = all 0 -> Pull-up/down OFF */
            { .reg = PUEN(2),    .bits = BITS(0,9),   .val = PUEN_DISABLE }, /* PUEN2[9:0]      = 0 -> Pull-up/down OFF */
            { 0 }
        }
    },

    {   // PCIe
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(19) }, /* PCIEC0 */
            { {0} }
        },
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

#if USE_DISPLAY_OR_MSIOF3 == USE_DISPLAY
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
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_DR5 */
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
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_HSYNC */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_VSYNC */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(21), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_ODDF */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_IN, .gpio.val = 0 }, /* HDMI Interrupt */
            { { 0 } }
        },
    },
#endif //#if USE_DISPLAY_OR_MSIOF3 == USE_DISPLAY

    { // Capture
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(15) }, /* CSI40 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(16) }, /* CSI41 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(4)  }, /* VIN7 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(5)  }, /* VIN6 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(6)  }, /* VIN5 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(7)  }, /* VIN4 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(8)  }, /* VIN3 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(9)  }, /* VIN2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(10) }, /* VIN1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(11) }, /* VIN0 */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CSI0CKCR, .bits = BIT(8), .val = 0 }, /* CSI0 clk */
            { 0 }
        },
    },

    { // MSIOF0
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(11) }, /* MSIOF0 */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_MSOCKCR, .bits = BITS(0,8), .val = 0x01E }, /* MSIOF clk set to 26Mhz */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(19),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 8,  .ipsr.val = 0 }, /* MSIOF0_RXD */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(20),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 12, .ipsr.val = 0 }, /* MSIOF0_TXD */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 16, .ipsr.val = 0 }, /* MSIOF0_SCK */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 20, .ipsr.val = 0 }, /* MSIOF0_SYNC */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(23),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 24, .ipsr.val = 0 }, /* MSIOF0_SS1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(24),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 28, .ipsr.val = 0 }, /* MSIOF0_SS2 */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = POCCTRL(1), .bits = BITS(26,31), .val = 0xF3000000 },            /* IOCCTRL1[31:26] = all 1 (SD IO voltage=1.8V) */
            { 0 }
        }
    },

#if USE_DISPLAY_OR_MSIOF3 == USE_MSIOF3
    { // MSIOF3
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(8)  }, /* MSIOF3 */
            { {0} }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(16),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 0,  .ipsr.val = 1 }, /* MSIOF3_RXD */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(17),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 4,  .ipsr.val = 1 }, /* MSIOF3_TXD */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(18),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 8,  .ipsr.val = 1 }, /* MSIOF3_SS1 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 1 }, /* MSIOF3_SS2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(20),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 16, .ipsr.val = 1 }, /* MSIOF3_SCK */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 20, .ipsr.val = 1 }, /* MSIOF3_SYNC */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = POCCTRL(0), .bits = BITS(16,21), .val = 0x3F0000 },            /* IOCCTRL0[21:16] = all 1 (SD IO voltage=1.8V) */
            { 0 }
        }
    },
#endif // #if USE_DISPLAY_OR_MSIOF3 == USE_MSIOF3

    { /* CANFD/CAN */
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(14) }, /* CANFD */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CANFDCKCR, .bits = BITS(0,8), .val = 0x009 }, /* CANFD clk set to 80Mhz (classic can only) */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 4,  .ipsr.val = 0 }, /* CANFD0_TX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 8,  .ipsr.val = 0 }, /* CANFD0_RX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 12, .ipsr.val = 0 }, /* CANFD1_TX */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 16, .ipsr.val = 0 }, /* CANFD1_RX */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_CAN_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 20, .ipsr.val = 0 }, /* CAN_CLK_A */
#endif
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(11),   .val = 0 },         /* Select CANFD0_*_A pin for CANFD */
            { 0 }
        }        
    },
};

rcar_config v3h_ws20_condor_config[] = {
#if USE_SCIF_HSICF_FOR_UART == USE_SCIF
    { // SCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(7) }, /* SCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 4 }, /* SCL2 -> RX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 4 }, /* SDA2 -> TX0  */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 20, .ipsr.val = 4 }, /* SCIF_CLK_B  */
#endif
            { { 0 } }
        },
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(9),   .val = 0x200 },         /* Select SCIF_CLK_B pin for SCIF0 */
            { 0 }
        }        
#endif
    },
#else
    { // HSCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(20) }, /* HSCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 5 }, /* SCL2 -> HRX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 5 }, /* SDA2 -> HTX0  */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(9),   .val = 0x200 },         /* Select HRX0_B & HTX0_B pins for HSCIF0 */
            { 0 }
        }
    },
#endif //#if USE_SCIF_HSICF_FOR_UART == USE_SCIF

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
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(13) }, /* GETHER-IF */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BITS(6,21), .gpsr.val = GPSR_PF },
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(22), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(4), .gpio.type = GPIO_OUT, .gpio.val = 1 }, // Reset pin
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(23), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(4), .gpio.type = GPIO_IN,  .gpio.val = 0 }, // Int pin
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(24), .gpsr.val = GPSR_PF },
            { { 0 } }
        }
    },

    { // I2C
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) },   /* I2C-IF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(30) },   /* I2C-IF1 */
            //{ .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(29) },   /* I2C-IF2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(28) },   /* I2C-IF3 */
            //{ .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(27) }, /* I2C-IF4 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 4,  .ipsr.val = 0 }, /* I2C-IF0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 8,  .ipsr.val = 0 }, /* I2C-IF0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 12, .ipsr.val = 0 }, /* I2C-IF1 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 16, .ipsr.val = 0 }, /* I2C-IF1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 0,  .ipsr.val = 0 }, /* I2C-IF3 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(26), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 4,  .ipsr.val = 0 }, /* I2C-IF3 */
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
            { .reg = RCAR_GEN3_CPG_SD0CKCR, .bits = 0xFFFFFFFF, .val = 0x00000004 }, /* SD0 clk */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 16, .ipsr.val = 4 }, /* VI1_DATA0  -> MMC_WP */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 20, .ipsr.val = 4 }, /* VI1_DATA1  -> MMC_CD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 24, .ipsr.val = 4 }, /* VI1_DATA2  -> MMC_DS */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 28, .ipsr.val = 4 }, /* VI1_DATA3  -> MMC_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 0,  .ipsr.val = 4 }, /* VI1_DATA4  -> MMC_D0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 4 }, /* VI1_DATA5  -> MMC_D1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 8,  .ipsr.val = 4 }, /* VI1_DATA6  -> MMC_D2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 12, .ipsr.val = 4 }, /* VI1_DATA7  -> MMC_D3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 4 }, /* VI1_DATA8  -> MMC_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 20, .ipsr.val = 4 }, /* VI1_DATA9  -> MMC_D4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 24, .ipsr.val = 4 }, /* VI1_DATA10 -> MMC_D5 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 4 }, /* VI1_DATA11 -> MMC_D6 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 4 }, /* VI1_FIELD  -> MMC_D7 */
            { { 0 } }
        },

        .settings = (rcar_pin[])
        {
            { .reg = DRVCTRL(7), .bits = BITS(0,7),   .val = 0x33 },         /* IOCTRL7[7:0]    = WP, CD */
            { .reg = DRVCTRL(8), .bits = BITS(0,31),  .val = 0x33333333 },   /* IOCTRL8[31:0]   = SD1_DAT[0:4], SD2_CLK, SD2_DS,SD2_CMD */
            { .reg = DRVCTRL(9), .bits = BITS(20,31), .val = 0x33300000 },   /* IOCTRL9[31:20]  = SD2_DAT[7:5] */
            { .reg = POCCTRL(1), .bits = BITS(11,23), .val = 0 },            /* IOCTRL31[23:11] = all 0 (SD IO voltage=1.8V) */
            { .reg = PUEN(1),    .bits = BITS(29,31), .val = PUEN_DISABLE }, /* PUEN1[31:29]    = all 0 -> Pull-up/down OFF */
            { .reg = PUEN(2),    .bits = BITS(0,9),   .val = PUEN_DISABLE }, /* PUEN2[9:0]      = 0 -> Pull-up/down OFF */
            { 0 }
        }
    },

    {   // PCIe
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(19) }, /* PCIEC0 */
            { {0} }
        },
    },

    { // DMAC
        .mod = (rcar_module[])
        {   /* If enable write/read rate control, please set '.rate = (<wr_rate_val> << 8) | (<rd_rate_val>)' here */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(18), .rate = 0, .base = RCAR_GEN3_SYSDMAC1_BASE   }, /* SYS-DMAC1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(17), .rate = 0, .base = RCAR_GEN3_SYSDMAC2_BASE   }, /* SYS-DMAC2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(21), .rate = 0, .base = RCAR_GEN3_RTDMAC0_BASE   }, /* RT-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(16), .rate = 0, .base = RCAR_GEN3_RTDMAC1_BASE   }, /* RT-DMAC1 */
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

#if USE_DISPLAY_OR_MSIOF3 == USE_DISPLAY
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
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_DR5 */
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
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 0 }, /* DU_HSYNC */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 16, .ipsr.val = 0 }, /* DU_VSYNC */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(21), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 20, .ipsr.val = 0 }, /* DU_ODDF */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_IN, .gpio.val = 0 }, /* HDMI Interrupt */
            { { 0 } }
        },
    },
#endif //#if USE_DISPLAY_OR_MSIOF3 == USE_DISPLAY

    { // Capture
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(15) }, /* CSI40 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(16) }, /* CSI41 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(4)  }, /* VIN7 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(5)  }, /* VIN6 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(6)  }, /* VIN5 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(7)  }, /* VIN4 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(8)  }, /* VIN3 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(9)  }, /* VIN2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(10) }, /* VIN1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(11) }, /* VIN0 */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CSI0CKCR, .bits = BIT(8), .val = 0 }, /* CSI0 clk */
            { 0 }
        },
    },

    { // MSIOF0
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(11) }, /* MSIOF0 */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_MSOCKCR, .bits = BITS(0,8), .val = 0x01E }, /* MSIOF clk set to 26Mhz */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(19),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 8,  .ipsr.val = 0 }, /* MSIOF0_RXD */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(20),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 12, .ipsr.val = 0 }, /* MSIOF0_TXD */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 16, .ipsr.val = 0 }, /* MSIOF0_SCK */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 20, .ipsr.val = 0 }, /* MSIOF0_SYNC */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(23),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 24, .ipsr.val = 0 }, /* MSIOF0_SS1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(24),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 28, .ipsr.val = 0 }, /* MSIOF0_SS2 */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = POCCTRL(1), .bits = BITS(26,31), .val = 0xF3000000 },            /* IOCCTRL1[31:26] = all 1 (SD IO voltage=1.8V) */
            { 0 }
        }
    },

#if USE_DISPLAY_OR_MSIOF3 == USE_MSIOF3
    { // MSIOF3
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(8)  }, /* MSIOF3 */
            { {0} }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(16),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 0,  .ipsr.val = 1 }, /* MSIOF3_RXD */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(17),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 4,  .ipsr.val = 1 }, /* MSIOF3_TXD */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(18),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 8,  .ipsr.val = 1 }, /* MSIOF3_SS1 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(19),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 12, .ipsr.val = 1 }, /* MSIOF3_SS2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(20),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 16, .ipsr.val = 1 }, /* MSIOF3_SCK */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(2), .ipsr.bits = 20, .ipsr.val = 1 }, /* MSIOF3_SYNC */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = POCCTRL(0), .bits = BITS(16,21), .val = 0x3F0000 },            /* IOCCTRL0[21:16] = all 1 (SD IO voltage=1.8V) */
            { 0 }
        }
    },
#endif // #if USE_DISPLAY_OR_MSIOF3 == USE_MSIOF3

    { /* CANFD/CAN */
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(14) }, /* CANFD */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CANFDCKCR, .bits = BITS(0,8), .val = 0x009 }, /* CANFD clk set to 80Mhz (classic can only) */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(19),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 28, .ipsr.val = 2 }, /* CANFD2_TX */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(20),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 0,  .ipsr.val = 2 }, /* CANFD2_RX */            
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 4,  .ipsr.val = 0 }, /* CANFD0_TX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 8,  .ipsr.val = 0 }, /* CANFD0_RX_A*/
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 12, .ipsr.val = 0 }, /* CANFD1_TX */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 16, .ipsr.val = 0 }, /* CANFD1_RX */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_CAN_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 20, .ipsr.val = 0 }, /* CAN_CLK_A */
#endif
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(11),   .val = 0 },         /* Select CANFD0_*_A pin for CANFD */
            { 0 }
        }      
    },
};


rcar_config v3h_ws10_sk_config[] = {
#if USE_SCIF_HSICF_FOR_UART == USE_SCIF
    { // SCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(7) }, /* SCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 4 }, /* SCL2 -> RX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 4 }, /* SDA2 -> TX0  */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 20, .ipsr.val = 4 }, /* SCIF_CLK_B  */
#endif
            { { 0 } }
        },
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_SCIF_CLOCK 
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(9),   .val = 0x200 },         /* Select SCIF_CLK_B pin for SCIF0 */
            { 0 }
        }        
#endif        
    },
#else
    { // HSCIF
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(20) }, /* HSCIF0 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 5 }, /* SCL2 -> HRX0  */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 5 }, /* SDA2 -> HTX0  */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(0), .bits = BIT(9),   .val = 0x200 },         /* Select HRX0_B & HTX0_B pins for HSCIF0 */
            { 0 }
        }
    },
#endif //#if USE_SCIF_HSICF_FOR_UART == USE_SCIF

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
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(13) }, /* GETHER-IF */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BITS(6,21), .gpsr.val = GPSR_PF },
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(22), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(4), .gpio.type = GPIO_OUT, .gpio.val = 1 }, // Reset pin
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(23), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(4), .gpio.type = GPIO_IN,  .gpio.val = 0 }, // Int pin
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(24), .gpsr.val = GPSR_PF },
            { { 0 } }
        }
    },

    { // I2C
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) },   /* I2C-IF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(30) },   /* I2C-IF1 */
            //{ .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(29) },   /* I2C-IF2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(28) },   /* I2C-IF3 */
            //{ .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(27) }, /* I2C-IF4 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 4,  .ipsr.val = 0 }, /* I2C-IF0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 8,  .ipsr.val = 0 }, /* I2C-IF0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 12, .ipsr.val = 0 }, /* I2C-IF1 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 16, .ipsr.val = 0 }, /* I2C-IF1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 0,  .ipsr.val = 0 }, /* I2C-IF3 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(26), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 4,  .ipsr.val = 0 }, /* I2C-IF3 */
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
            { .reg = RCAR_GEN3_CPG_SD0CKCR, .bits = 0xFFFFFFFF, .val = 0x00000004 }, /* SD0 clk */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 16, .ipsr.val = 4 }, /* VI1_DATA0  -> MMC_WP */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 20, .ipsr.val = 4 }, /* VI1_DATA1  -> MMC_CD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 24, .ipsr.val = 4 }, /* VI1_DATA2  -> MMC_DS */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 28, .ipsr.val = 4 }, /* VI1_DATA3  -> MMC_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 0,  .ipsr.val = 4 }, /* VI1_DATA4  -> MMC_D0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 4 }, /* VI1_DATA5  -> MMC_D1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 8,  .ipsr.val = 4 }, /* VI1_DATA6  -> MMC_D2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 12, .ipsr.val = 4 }, /* VI1_DATA7  -> MMC_D3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 4 }, /* VI1_DATA8  -> MMC_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 20, .ipsr.val = 4 }, /* VI1_DATA9  -> MMC_D4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 24, .ipsr.val = 4 }, /* VI1_DATA10 -> MMC_D5 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 4 }, /* VI1_DATA11 -> MMC_D6 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 4 }, /* VI1_FIELD  -> MMC_D7 */
            { { 0 } }
        },

        .settings = (rcar_pin[])
        {
            { .reg = DRVCTRL(7), .bits = BITS(0,7),   .val = 0x33 },         /* IOCTRL7[7:0]    = WP, CD */
            { .reg = DRVCTRL(8), .bits = BITS(0,31),  .val = 0x33333333 },   /* IOCTRL8[31:0]   = SD1_DAT[0:4], SD2_CLK, SD2_DS,SD2_CMD */
            { .reg = DRVCTRL(9), .bits = BITS(20,31), .val = 0x33300000 },   /* IOCTRL9[31:20]  = SD2_DAT[7:5] */
            { .reg = POCCTRL(1), .bits = BITS(11,23), .val = 0 },            /* IOCTRL31[23:11] = all 0 (SD IO voltage=1.8V) */
            { .reg = PUEN(1),    .bits = BITS(29,31), .val = PUEN_DISABLE }, /* PUEN1[31:29]    = all 0 -> Pull-up/down OFF */
            { .reg = PUEN(2),    .bits = BITS(0,9),   .val = PUEN_DISABLE }, /* PUEN2[9:0]      = 0 -> Pull-up/down OFF */
            { 0 }
        }
    },

    {   // PCIe
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(19) }, /* PCIEC0 */
            { {0} }
        },
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
    },

    { // Capture
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(15) }, /* CSI40 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(16) }, /* CSI41 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(4)  }, /* VIN7 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(5)  }, /* VIN6 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(6)  }, /* VIN5 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(7)  }, /* VIN4 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(8)  }, /* VIN3 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(9)  }, /* VIN2 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(10) }, /* VIN1 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(8), .clk.bits = BIT(11) }, /* VIN0 */
            { {0} }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CSI0CKCR, .bits = BIT(8), .val = 0 }, /* CSI0 clk */
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
            { .reg = RCAR_GEN3_CPG_CANFDCKCR, .bits = BITS(0,8), .val = 0x009 }, /* CANFD clk set to 80Mhz (classic can only) */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(21),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 4,  .ipsr.val = 0 }, /* CANFD0_TX_A */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 8,  .ipsr.val = 0 }, /* CANFD0_RX_A*/
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 12, .ipsr.val = 0 }, /* CANFD1_TX */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 16, .ipsr.val = 0 }, /* CANFD1_RX */
#if USE_CLOCK_PIN_FOR_CAN_SCIF == USE_CAN_CLOCK 
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 20, .ipsr.val = 0 }, /* CAN_CLK_A */
#endif
            /* Setup the GPIOs required for CANFD on the CogentEmbedded Videobox V3H */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(12),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(0), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* CAN0Loff */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(27),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(2), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* CAN0STBY */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(29),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(2), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* CAN1Loff */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(2), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* CAN1STBY */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(19),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* CPLD_If_En */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            /* Setup the GPIOs required for CANFD on the CogentEmbedded Videobox V3H */
            { .reg = PUEN(0), .bits = BIT(12), .val = PUEN_DISABLE },  /* CAN0Loff */
            { .reg = PUEN(4), .bits = BIT(20), .val = PUEN_DISABLE },  /* CAN1STBY */
            { .reg = PUEN(4), .bits = BIT(23), .val = PUEN_DISABLE },  /* CAN0STBY */
            { .reg = PUEN(4), .bits = BIT(25), .val = PUEN_DISABLE },  /* CAN1Loff */
            { .reg = PUEN(3), .bits = BIT(2),  .val = PUEN_DISABLE },  /* CPLD_If_En */
            { 0 }
        }
    },
};

#endif //__RCAR_SOC_BOARD_V3_H_INCLUDED
