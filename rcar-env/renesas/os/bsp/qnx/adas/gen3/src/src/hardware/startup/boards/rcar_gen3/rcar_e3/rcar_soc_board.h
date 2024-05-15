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

#ifndef __RCAR_SOC_BOARD_E3_H_INCLUDED
#define __RCAR_SOC_BOARD_E3_H_INCLUDED


#define USE_CANFD

extern void config_usbh(rcar_module *module);
extern void config_usbd(rcar_module *module);
extern void config_usb3h(rcar_module *module);
extern void config_dmac(rcar_module *module);
extern void config_audio(rcar_module *module);
extern void config_gpu(rcar_module *module);

rcar_config e3_config[] = {
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
#ifndef USE_HSCIF2
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(5), .clk.bits = BIT(18) }, /* HSCIF2 */
#else
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(5), .clk.bits = BIT(16) }, /* HSCIF2 */
#endif
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
#ifndef USE_HSCIF2
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 8,   .ipsr.val = 0 }, /* RX2_A */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 12,  .ipsr.val = 0 }, /* TX2_A */
#else
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 8,   .ipsr.val = 1 }, /* HRX0_A */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 12,  .ipsr.val = 1 }, /* HTX0_A */
#endif
            { { 0 } }
        }
    },
    [DEV_ETHERNET] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(12) }, /* EAVB-IF */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(20), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(1), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* AVB_MAGIC */
            { { 0 } }
        },
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
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(6), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* USB0_PWEN */
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IO, .gpio.reg = GPIO(6), .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* USB0_OVC */
            { { 0 } }
        },
        .blk_config = config_usbd,
    },
    [DEV_USB3H] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(3), .clk.bits = BIT(28), .base = RCAR_GEN3_USB30_BASE }, /* USB3.0-IF0 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(27), .base = RCAR_GEN3_USB31_BASE }, /* USB3.0-IF1 */
            { { 0 } }
        },
        .blk_config = config_usb3h,
    },
    [DEV_USB3D] = {
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(30), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(17), .ipsr.bits = 0, .ipsr.val = 0 }, /* USB31_PWEN */
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(31), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(17), .ipsr.bits = 4, .ipsr.val = 0 }, /* USB31_OVC */
            { { 0 } }
        }
    },
    [DEV_MSIOF] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(2), .clk.bits = BIT(11) }, /* MSIOF0 */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_MSOCKCR, .bits = BITS(0,8), .val = 0x01E }, /* MSIOF clk set to 26Mhz */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 16, .ipsr.val = 0 }, /* MSIOF0_SCK */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 20, .ipsr.val = 0 }, /* MSIOF0_RXD */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 24, .ipsr.val = 0 }, /* MSIOF0_TXD */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 28, .ipsr.val = 0 }, /* MSIOF0_SYNC */
            { { 0 } }
        },
    },
    [DEV_I2C] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) }, /* I2C-IF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(28) }, /* I2C-IF3 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(26) }, /* IIC */
            { { 0 } }
        },
    },
    [DEV_AUDIO] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9),  .clk.bits = BIT(22)    }, /* ADG */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(10), .clk.bits = BITS(5,31) }, /* SSI */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(11), .ipsr.bits = 16,  .ipsr.val = 2 }, /* AUDIO_CLKOUT_A */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(11), .ipsr.bits = 20,  .ipsr.val = 2 }, /* AUDIO_CLKOUT1_A */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(12), .ipsr.bits = 4,   .ipsr.val = 2 }, /* AUDIO_CLKB_A */
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(13), .ipsr.bits = 28,  .ipsr.val = 0 }, /* SSI_SCK01239 */
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(13), .ipsr.bits = 24,  .ipsr.val = 0 }, /* SSI_WS01239 */
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_PF, .ipsr.reg = IPSR(14), .ipsr.bits = 0,   .ipsr.val = 0 }, /* SSI_DATA0 */
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_PF, .ipsr.reg = IPSR(14), .ipsr.bits = 4,   .ipsr.val = 0 }, /* SSI_DATA1 */
            { .gpsr.reg = GPSR(6), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_PF }, /* AUDIO_CLKA */
            { { 0 } }
        },
        .blk_config = config_audio,
    },

    [DEV_SDHI] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(14) }, /* SD-IF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(13) }, /* SD-IF1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(12) }, /* GPIO0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(9)  }, /* GPIO3 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(7)  }, /* GPIO5 */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_SD0CKCR, .bits = 0xFFFFFFFF, .val = 1 }, /* SD0 clk */
            { .reg = RCAR_GEN3_CPG_SD1CKCR, .bits = 0xFFFFFFFF, .val = 1 }, /* SD3 clk */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 28, .ipsr.val = 0 }, /* SD0_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 0,  .ipsr.val = 0 }, /* SD0_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 4,  .ipsr.val = 0 }, /* SD0_DAT0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 8,  .ipsr.val = 0 }, /* SD0_DAT1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 12, .ipsr.val = 0 }, /* SD0_DAT2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 16, .ipsr.val = 0 }, /* SD0_DAT3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 20, .ipsr.val = 0 }, /* SD1_CLK */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 24, .ipsr.val = 0 }, /* SD1_CMD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(8),  .ipsr.bits = 28, .ipsr.val = 0 }, /* SD1_DATA0 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 0,  .ipsr.val = 0 }, /* SD1_DATA1 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 4,  .ipsr.val = 0 }, /* SD1_DATA2 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 8,  .ipsr.val = 0 }, /* SD1_DATA3 */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 24, .ipsr.val = 0 }, /* SD0_CD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 28, .ipsr.val = 0 }, /* SD0_WP */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(11), .ipsr.bits = 0,  .ipsr.val = 0 }, /* SD1_CD */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(11), .ipsr.bits = 4,  .ipsr.val = 0 }, /* SD1_WP */

            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(17),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(5),  .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* SD0_PWR */
            { .gpsr.reg = GPSR(5), .gpsr.bits = BIT(18),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(5),  .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* SD0_SIG_PWR */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(4),   .gpsr.val = GPSR_IO, .gpio.reg = GPIO(0),  .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* SD1_PWR */
            { .gpsr.reg = GPSR(3), .gpsr.bits = BIT(15),  .gpsr.val = GPSR_IO, .gpio.reg = GPIO(3),  .gpio.type = GPIO_OUT, .gpio.val = 1 }, /* SD1_SIG_PWR */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = PUEN(2), .bits = BITS(0,1),  .val = PUEN_DISABLE },
            { .reg = PUEN(3), .bits = BITS(7,10)|BITS(22,31), .val = PUEN_DISABLE },
            { .reg = POCCTRL(0), .bits = BITS(0,11), .val = POC_1_8V }, 
            { .reg = POCCTRL(2), .bits = BITS(0,11), .val = POC_1_8V }, 
            { .reg = TDSELCTRL(0), .bits = BITS(0,3), .val = TDSEL(TDSEL_10PF, 0)|TDSEL(TDSEL_10PF, 2) }, 
            { 0 }
        },
    },

    [DEV_MMCIF] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(11) }, /* SD-IF3 */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_SD3CKCR, .bits = 0xFFFFFFFF, .val = 1 },   /* SD3 clk */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 12, .ipsr.val = 0 }, /* SD3_CLK */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 16, .ipsr.val = 0 }, /* SD3_CMD */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 20, .ipsr.val = 0 }, /* SD3_DAT0 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 24, .ipsr.val = 0 }, /* SD3_DAT1 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(9),  .ipsr.bits = 28, .ipsr.val = 0 }, /* SD3_DAT2 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 0,  .ipsr.val = 0 }, /* SD3_DAT3 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 4,  .ipsr.val = 0 }, /* SD3_DAT4 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 8,  .ipsr.val = 0 }, /* SD3_DAT5 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 12, .ipsr.val = 0 }, /* SD3_DAT6 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 16, .ipsr.val = 0 }, /* SD3_DAT7 */
            { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(10), .ipsr.bits = 20, .ipsr.val = 0 }, /* SD3_DS */
            { { 0 } }
        },
        .settings = (rcar_pin[])
        {
            { .reg = PUEN(3), .bits = BITS(11,21), .val = PUEN_DISABLE },
            { .reg = TDSELCTRL(0), .bits = BITS(19,29), .val = POC_1_8V },
            { .reg = TDSELCTRL(0), .bits = BITS(6,7),   .val = TDSEL(TDSEL_10PF, 6) }, 
            { 0 }
        }
    },
    [DEV_DMAC] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(5), .clk.bits = BIT(2),  .base = RCAR_GEN3_AUDIODMAC0_BASE }, /* Audio-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(5), .clk.bits = BIT(1),  .base = RCAR_GEN3_AUDIODMAC1_BASE }, /* Audio-DMAC1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(19), .base = RCAR_GEN3_SYSDMAC0_BASE   }, /* SYS-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(18), .base = RCAR_GEN3_SYSDMAC1_BASE   }, /* SYS-DMAC1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(2), .clk.bits = BIT(17), .base = RCAR_GEN3_SYSDMAC2_BASE   }, /* SYS-DMAC2 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(21), .base = RCAR_GEN3_RTDMAC0_BASE   }, /* RT-DMAC0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(0), .clk.bits = BIT(16), .base = RCAR_GEN3_RTDMAC1_BASE   }, /* RT-DMAC1 */
            { { 0 } }
        },
        .blk_config = config_dmac,
    },
    [DEV_PCI] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(3), .clk.bits = BIT(19) }, /* PCIEC0 */
            { {0} }
        }
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
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(16) }, /* CAN-IF0 */
            { .clk.val = DISABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(15) }, /* CAN-IF1 */
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
            { .reg = RCAR_GEN3_CPG_CANFDCKCR, .bits = 0xFFFFFFFF, .val = 0x9 }, /* CANFD clk at 80 MHz */
            { 0 }
        },
        .pinmux = (rcar_pinmux[])
        {
#if defined USE_CANFD
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 1 }, /* CAN1_TX */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 1 }, /* CAN0_TX_A */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 4,  .ipsr.val = 1 }, /* CAN0_RX_A */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 8,  .ipsr.val = 1 }, /* CAN1_RX */
#else
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 4,  .ipsr.val = 3 }, /* CAN1_TX */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(7),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 16, .ipsr.val = 3 }, /* CAN0_TX_A */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 4,  .ipsr.val = 3 }, /* CAN0_RX_A */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 8,  .ipsr.val = 3 }, /* CAN1_RX */
#endif
            { { 0 } }
        },
    },

    [DEV_GRAPHICS] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE,  .clk.reg = SMSTPCR(1), .clk.bits = BIT(12) }, /* 3DGE */
            { { 0 } }
        },
        .blk_config = config_gpu
    },
    [DEV_DISPLAY] = {
        .pwr = (rcar_pwr[])
        {
            { .base = PWRONCR(8), .setting = 0x1, .status = PWRSR(8), .power_on = (1 << 4) },  /* A3VP */  
            { .base = PWRONCR(9), .setting = 0x1, .status = PWRSR(9), .power_on = (1 << 4) },  /* A3VC */
            { 0 }
        },
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(27) }, /* LVDS*/
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(23) }, /* DU1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(24) }, /* DU0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(31) }, /* VSPI0 */ 
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(26) }, /* VSPBS */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(23) }, /* VSPD0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(22) }, /* VSPD1 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(19) }, /* FCPC_S */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(15) }, /* FCPF0 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(11) }, /* FCPV (IMG0) */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(7) },  /* FCPV (BLD0) */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(3) },  /* FCPV (DU0) */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(6), .clk.bits = BIT(2) },  /* FCPV (DU1 */
            { { 0 } }
        },
        .pinmux = (rcar_pinmux[])
        {
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 20, .ipsr.val = 5 }, /* DU_DR2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 24, .ipsr.val = 5 }, /* DU_DB7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 28, .ipsr.val = 5 }, /* DU_DR3 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 0,  .ipsr.val = 5 }, /* DU_DR4 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(5),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 8,  .ipsr.val = 5 }, /* DU_DR5 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(6),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 12, .ipsr.val = 5 }, /* DU_DR6 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 20, .ipsr.val = 5 }, /* DU_DR7 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 24, .ipsr.val = 5 }, /* DU_DG0 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(6), .ipsr.bits = 28, .ipsr.val = 5 }, /* DU_DG1 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 0,  .ipsr.val = 5 }, /* DU_DG2 */
            { .gpsr.reg = GPSR(0), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7), .ipsr.bits = 16, .ipsr.val = 5 }, /* DU_DG3 */
            
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(2),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 4,  .ipsr.val = 5 }, /* DU_DISP */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(3),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 8,  .ipsr.val = 5 }, /* DU_DOTCLKOUT0 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(4),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 12, .ipsr.val = 5 }, /* DU_DG4 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(8),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(3), .ipsr.bits = 28, .ipsr.val = 5 }, /* DU_HSYNC */
            
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(9),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 0,  .ipsr.val = 5 }, /* DU_DG7 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(10), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 4,  .ipsr.val = 5 }, /* DU_DG5 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(11), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 8,  .ipsr.val = 5 }, /* DU_VSYNC */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(12), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 12, .ipsr.val = 5 }, /* DU_DG6 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(13), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 16, .ipsr.val = 5 }, /* DU_DB2 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(14), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 20, .ipsr.val = 5 }, /* DU_DB3 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(15), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 24, .ipsr.val = 5 }, /* DU_DB4 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(16), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(4), .ipsr.bits = 28, .ipsr.val = 5 }, /* DU_DB5 */
            
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(17), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 0,  .ipsr.val = 5 }, /* DU_DB6 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(18), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 4,  .ipsr.val = 5 }, /* DU_DB0 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(19), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 8,  .ipsr.val = 5 }, /* DU_DB1 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(21), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 12, .ipsr.val = 5 }, /* DU_DR0 */
            { .gpsr.reg = GPSR(1), .gpsr.bits = BIT(22), .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(5), .ipsr.bits = 16, .ipsr.val = 5 }, /* DU_DR1 */
            { { 0 } }
        }
    },

    [DEV_VIN] = {
        .mod = (rcar_module[])
        {
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(7), .clk.bits = BIT(16) }, /* CSI40 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(6) },  /* VIN5 */
            { .clk.val = ENABLE, .clk.reg = SMSTPCR(8), .clk.bits = BIT(7) },  /* VIN4 */
            { { 0 } }
        },
        .cpg = (rcar_pin[])
        {
            { .reg = RCAR_GEN3_CPG_CSI0CKCR, .bits = 0xFFFFFFFF, .val = 0x1F },  /* CSI0 clock */
            { 0 }
        },
    },
};

#endif //__RCAR_SOC_BOARD_E3_H_INCLUDED
