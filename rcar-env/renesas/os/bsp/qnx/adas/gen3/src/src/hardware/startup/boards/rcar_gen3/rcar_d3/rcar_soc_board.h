/*
* $QNXLicenseC:
* Copyright (c) 2017, QNX Software Systems. All Rights Reserved.
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

#ifndef __RCAR_SOC_BOARD_D3_H_INCLUDED
#define __RCAR_SOC_BOARD_D3_H_INCLUDED
#define USE_CANFD

extern void config_usbh(rcar_module *module);
extern void config_usbd(rcar_module *module);
extern void config_usb3h(rcar_module *module);
extern void config_dmac(rcar_module *module);
extern void config_audio_d3(rcar_module *module);

rcar_config d3_config[] = {
    [DEV_TIMER] = {
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
    [DEV_SCIF] = {
        .mod = (rcar_module[])
        {
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(7) }, /* SCIF0 */
#ifndef USE_HSCIF1
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(6) }, /* SCIF1 */
#else
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(6) }, /* SCIF1 */
#endif
            /* SCIF2 is always enabled */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(4) }, /* SCIF3 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(3) }, /* SCIF4 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(2) }, /* SCIF5 */
            { { 0 } }
        },
    },
    [DEV_ETHERNET] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(12) }, /* EAVB-IF */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(7) },  /* GPIO5 */
            { { 0 } }
        },
        /* Use pfc setting from IPL/UBoot */
    },
    [DEV_USBH] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(7), .clk.bits = BIT(3), .base = RCAR_GEN3_USB0_BASE }, /* EHCI0/OHCI0 */
            { { 0 } }
        },
        .blk_config = config_usbh,
    },
    [DEV_USBD] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(4), .base = RCAR_GEN3_HSUSB_BASE }, /* HS-USB-IF */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(30) },                              /* USB_DMAC0-0*/
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(31) },                              /* USB_DMAC0-1*/
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(6)  },                              /* GPIO6 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(0), .gpsr.val = GPSR_PF }, /* USB0_PWEN */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(1), .gpsr.val = GPSR_PF }, /* USB0_OVC */
            { { 0 } }
        },
        .blk_config = config_usbd,
    },
    [DEV_MSIOF] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(9)  }, /* MSIOF2 */
            { { 0 } }
        },

        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_MSOCKCR, .bits = BITS(0,8), .val = 0x01E }, /* MSIOF clk set to 26Mhz */
            { 0 }
        },
        
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(2), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 0, .ipsr.val = 1 }, /* IRQ0_A/MSIOF2_SYNC_B */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3), .gpsr.val = GPSR_PF }, /* MSIOF2_SCK */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(4), .gpsr.val = GPSR_PF }, /* MSIOF2_TXD */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(5), .gpsr.val = GPSR_PF }, /* MSIOF2_RXD */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(6), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(0), .ipsr.bits = 16, .ipsr.val = 1 }, /* MLB_CLK/MSIOF2_SYNC_A/SCK5_A */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
#if defined USE_SYNC_A
            { .reg = MODSEL(0), .bits = BIT(30), .val = 0 }, /* MSIOF2_SYNC_A */
#elif defined USE_SYNC_B
            { .reg = MODSEL(0), .bits = BIT(30), .val = 1 }, /* MSIOF2_SYNC_B */
#endif
            { 0 }
        }
    },
    [DEV_I2C] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) }, /* I2C-IF0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(30) }, /* I2C-IF1 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(29) }, /* I2C-IF2 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(28) }, /* I2C-IF3 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(27) }, /* I2C-IF4 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(8), .gpsr.val = GPSR_PF }, /* SCL0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(9), .gpsr.val = GPSR_PF }, /* SDA0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 28,  .ipsr.val = 0 }, /* SCL1/CTS1# */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(11), .ipsr.bits = 0,  .ipsr.val = 0 }, /* SDA1/RTS1# */
            { { 0 } }
        },
    },
    [DEV_AUDIO] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9),  .clk.bits = BIT(22)  },    /* ADG */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(10), .clk.bits = BITS(11,12) }, /* SSI 34*/
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(10), .clk.bits = BIT(5)   },    /* SSI All*/
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(27), .gpsr.val = GPSR_IP,  .ipsr.reg = IPSR(8),  .ipsr.bits = 0,  .ipsr.val = 1 }, /* AUDIO_CLKB */

            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 0,  .ipsr.val = 0 }, /* AUDIO_CLKA */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 4,  .ipsr.val = 0 }, /* SSI_SCK34 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 8,  .ipsr.val = 0 }, /* SSI_SDATA3 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 12, .ipsr.val = 0 }, /* SSI_WS34 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 16, .ipsr.val = 2 }, /* AUDIO_CLKOUT */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 20, .ipsr.val = 0 }, /* SSI_SDATA4 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 24, .ipsr.val = 0 }, /* SSI_WS4 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(22),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(11), .ipsr.bits = 28, .ipsr.val = 4 }, /* AUDIO_CLKOUT1 */
            { { 0 } }
        },
        .blk_config = config_audio_d3,
    },
    [DEV_MMCIF] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(12) }, /* SD-IF2 */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_SD2CKCR, .bits = 0xFFFFFFFF, .val = 1 },   /* SD2 clk */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(0), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 24, .ipsr.val = 1 }, /* MMC_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(1), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 28, .ipsr.val = 1 }, /* MMC_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(2), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 0,  .ipsr.val = 1 }, /* MMC_D0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(3), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 4,  .ipsr.val = 1 }, /* MMC_D1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(4), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 8,  .ipsr.val = 1 }, /* MMC_D2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(5), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 12, .ipsr.val = 1 }, /* MMC_D3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(6), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 16, .ipsr.val = 1 }, /* MMC_D4 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(7), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 20, .ipsr.val = 1 }, /* MMC_D5 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(8), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 24, .ipsr.val = 1 }, /* MMC_D6 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(9), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9), .ipsr.bits = 28, .ipsr.val = 1 }, /* MMC_D7 */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = PUEN(2), .bits = BITS(0,8), .val = PUEN_DISABLE },       /* Disable all  SD2 Pins Pull-up/down */
            { .reg = PUEN(3), .bits = BIT(31),   .val = PUEN_DISABLE },       /* Disable all SD2 Pins Pull-u/down */
            { .reg = POCCTRL(0), .bits = BITS(20, 29),  .val = POC_1_8V  },   /* Set SD1, SD2 POCCTRL to 1.8V */
            { 0 }
        }
    },
    [DEV_DMAC] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(5), .clk.bits = BIT(2),  .base = RCAR_GEN3_AUDIODMAC0_BASE }, /* Audio-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(19), .base = RCAR_GEN3_SYSDMAC0_BASE   }, /* SYS-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(18), .base = RCAR_GEN3_SYSDMAC1_BASE   }, /* SYS-DMAC1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(17), .base = RCAR_GEN3_SYSDMAC2_BASE   }, /* SYS-DMAC2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(21), .base = RCAR_GEN3_RTDMAC0_BASE   }, /* RT-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(16), .base = RCAR_GEN3_RTDMAC1_BASE   }, /* RT-DMAC1 */
            { { 0 } }
        },
        .blk_config = config_dmac,
    },
    [DEV_THERMAL] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(22) }, /* THS */
            { {0} }
        }
    },
    [DEV_CAN] = {
        .mod = (rcar_module[])
        {
#if defined USE_CANFD
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(14) }, /* CAN-FD */
#else
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(16) }, /* CAN-IF0 */
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(9), .clk.bits = BIT(15) }, /* CAN-IF1 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(14) }, /* CAN-FD */
#endif
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CANFDCKCR, .bits = 0xFFFFFFFF, .val = 0x18 }, /* CANFD clk at 40 MHz */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
#if defined USE_CANFD
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(28), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 24, .ipsr.val = 1 }, /* CAN0_RX_A/CANFD0_RX/RX0_B */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(29), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 28, .ipsr.val = 1 }, /* CAN0_TX_A/CANFD0_TX/TX0_B */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(30), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(13), .ipsr.bits = 0,  .ipsr.val = 1 }, /* CAN1_RX_A/CANFD1_RX/TPU0TO2_A */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(31), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(13), .ipsr.bits = 4,  .ipsr.val = 1 }, /* CAN1_TX_A/CANFD1_TX/TPU0TO3_A */
#else
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(28), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 24, .ipsr.val = 0 }, /* CAN0_RX_A/CANFD0_RX/RX0_B */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(29), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 28, .ipsr.val = 0 }, /* CAN0_TX_A/CANFD0_TX/TX0_B */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(30), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(13), .ipsr.bits = 0,  .ipsr.val = 0 }, /* CAN1_RX_A/CANFD1_RX/TPU0TO2_A */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(31), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(13), .ipsr.bits = 4,  .ipsr.val = 0 }, /* CAN1_TX_A/CANFD1_TX/TPU0TO3_A */
#endif
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = MODSEL(1), .bits = BIT(30), .val = 0 }, /* sel_can0 = CAN0_RX0_A/CAN0_TX_A */
            { .reg = MODSEL(1), .bits = BIT(31), .val = 0 }, /* sel_can1 = CAN1_RX0_A/CAN1_TX_A */
            { 0 }
        }
    },
    [DEV_DISPLAY] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(24) }, /* DU0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(23) }, /* DU1*/
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(27) }, /* LVDS */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(2) },  /* FCPV (DU1) */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(3) },  /* FCPV (DU0) */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(7) },  /* FCPVBS */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(22) }, /* VSPD1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(23) }, /* VSPD0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(27) }, /* VSPBS */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_PF }, /* DU_DB0 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_PF }, /* DU_DB1*/
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_PF }, /* DU_DB2 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_PF }, /* DU_DB3 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_PF }, /* DU_DB4 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_PF }, /* DU_DB5 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_PF }, /* DU_DB6 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_PF }, /* DU_DB7 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_PF }, /* DU_DG0 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_PF }, /* DU_DG1 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(10), .gpsr.val = GPSR_PF }, /* DU_DG2 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(11), .gpsr.val = GPSR_PF }, /* DU_DG3 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(12), .gpsr.val = GPSR_PF }, /* DU_DG4 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(13), .gpsr.val = GPSR_PF }, /* DU_DG5 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(14), .gpsr.val = GPSR_PF }, /* DU_DG6 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(15), .gpsr.val = GPSR_PF }, /* DU_DG7 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(16), .gpsr.val = GPSR_PF }, /* DU_DR0 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(17), .gpsr.val = GPSR_PF }, /* DU_DR1 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(18), .gpsr.val = GPSR_PF }, /* DU_DR2 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(19), .gpsr.val = GPSR_PF }, /* DU_DR3 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(20), .gpsr.val = GPSR_PF }, /* DU_DR4 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(21), .gpsr.val = GPSR_PF }, /* DU_DR5 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(22), .gpsr.val = GPSR_PF }, /* DU_DR6 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(23), .gpsr.val = GPSR_PF }, /* DU_DR7 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(24), .gpsr.val = GPSR_PF }, /* DU_DOTCLKOUT */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(25), .gpsr.val = GPSR_PF }, /* DU_HSYNC */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(26), .gpsr.val = GPSR_PF }, /* DU_VSYNC */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(27), .gpsr.val = GPSR_PF }, /* DU_DISP */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(31), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(2), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* Backlight Control for LVDS0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(4), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* Backlight Control for LVDS1 */
            { { 0 } }
        },
    },

    [DEV_VIN] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(7) },  /* VIN4 */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CSI0CKCR, .bits = 0xFFFFFFFF, .val = 0x1F },   /* CSI0 clock */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_PF },                                                      /* VI4_CLK   */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 28, .ipsr.val = 0 }, /* VI4_DATA0 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 0,  .ipsr.val = 0 }, /* VI4_DATA1 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 4,  .ipsr.val = 0 }, /* VI4_DATA2 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 8,  .ipsr.val = 0 }, /* VI4_DATA3 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_PF },                                                      /* VI4_DATA4 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 12, .ipsr.val = 0 }, /* VI4_DATA5 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 16, .ipsr.val = 0 }, /* VI4_DATA6 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 20, .ipsr.val = 0 }, /* VI4_DATA7 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 24, .ipsr.val = 0 }, /* VI4_DATA8 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 28, .ipsr.val = 0 }, /* VI4_DATA9 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 0,  .ipsr.val = 0 }, /* VI4_DATA10 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 0 }, /* VI4_DATA11 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 8,  .ipsr.val = 0 }, /* VI4_DATA12 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 12, .ipsr.val = 0 }, /* VI4_DATA13 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 0 }, /* VI4_DATA14 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 20, .ipsr.val = 0 }, /* VI4_DATA15 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(17), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 24, .ipsr.val = 0 }, /* VI4_DATA16 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(18), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 0 }, /* VI4_DATA17 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 0 }, /* VI4_DATA18 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 4,  .ipsr.val = 0 }, /* VI4_DATA19 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(21), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 8,  .ipsr.val = 0 }, /* VI4_DATA20 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(22), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 12, .ipsr.val = 0 }, /* VI4_DATA21 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(23), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 16, .ipsr.val = 0 }, /* VI4_DATA22 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(24), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 20, .ipsr.val = 0 }, /* VI4_DATA23 */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(25), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 24, .ipsr.val = 0 }, /* VI4_VSYNC  */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(26), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 28, .ipsr.val = 0 }, /* VI4_HSYNC  */
            { .gpsr.reg = GPSR(2), .gpsr.bits = BIT(28), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8), .ipsr.bits = 4,  .ipsr.val = 0 }, /* VI4_CLKENB */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            /* disable all eavb pins pull-up/down */
            { .reg = PUEN(1), .bits = BITS(0,9),   .val = PUEN_DISABLE }, /* PUEN1[9:0]    = all 0 -> Pull-up/down OFF */
            { .reg = PUEN(2), .bits = BITS(15,31), .val = PUEN_DISABLE }, /* PUEN2[31:15]  = all 0 -> Pull-up/down OFF */
            { .reg = PUEN(2), .bits = BIT(13),     .val = PUEN_DISABLE }, /* PUEN2[13]     = all 0 -> Pull-up/down OFF */
            { 0 }
        }
    },
};

#endif //__RCAR_SOC_BOARD_D3_H_INCLUDED

