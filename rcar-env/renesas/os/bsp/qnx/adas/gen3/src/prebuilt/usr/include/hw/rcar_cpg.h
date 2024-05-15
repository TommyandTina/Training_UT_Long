/*
 * $QNXLicenseC:
 * Copyright 2020 QNX Software Systems.
 * Copyright 2021 Renesas Electronics Corporation.
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

#ifndef _RCAR_CPG_H_INCLUDED
#define _RCAR_CPG_H_INCLUDED

#include <stdint.h>
#include <devctl.h>

typedef enum cpg_mstp_cmd_
{
    CMD_MSTP_ENABLE = 1,
    CMD_MSTP_DISABLE,
    CMD_MSTP_RESET,
    CMD_MSTP_ASSERT,
    CMD_MSTP_DEASSERT,
    CMD_MSTP_CHECK,
    CMD_MSTP_RESET_CHECK,
} cpg_mstp_cmd_t;

typedef enum {
    CPG_CLK_PLL0 = 0,
    CPG_CLK_PLL1,
    CPG_CLK_PLL2,
    CPG_CLK_PLL3,
    CPG_CLK_PLL4,
    CPG_CLK_PLL5,
    CPG_CLK_PLL6,
    CPG_CLK_PLL20 = 20, // For V3U only
    CPG_CLK_PLL21 = 21, // For V3U only
    CPG_CLK_PLL30 = 30, // For V3U only
    CPG_CLK_PLL31 = 31, // For V3U only
} CPG_PLL_ID;

typedef enum cpg_clk_id_t_ {
    CPG_CLK_HDMI,
    CPG_CLK_CANFD,
    CPG_CLK_MSIOF,
    CPG_CLK_CSI2,
    CPG_CLK_DSI,
    CPG_CLK_IPC,
    CPG_CLK_FRAY,
    CPG_CLK_RGX,
    CPG_CLK_SDHI,
    CPG_CLK_RPC,
    CPG_CLK_CPU,  // (H3/M3: idx=1 for CA57, idx=0 for CA53. Others only idx=0 is supported)
    CPG_CLK_PLL,
    CPG_CLK_POST,
    CPG_CLK_UNKNOWN,
} cpg_clk_id_t;

typedef enum cpg_clk_cmd_
{
    CMD_CLK_ENABLE = 1,
    CMD_CLK_DISABLE,
    CMD_CLK_RATE_GET,
    CMD_CLK_RATE_SET,
    CMD_CLK_DIV_GET,
    CMD_CLK_DIV_SET,
    CMD_CLK_CHECK,
} cpg_clk_cmd_t;

#define CPG_ATTACH_NAME              "cpg"
#define CPG_DEV_NAME                 "/dev/" CPG_ATTACH_NAME

/*
 * Return codes
 */
#define CPG_RET_OK                   (0)  // OK
#define CPG_RET_INVALPARA            (1)  // Invalid Input parameter 
#define CPG_RET_TIMEOUT              (2)  // Hardware flag not set 
#define CPG_RET_HWFAIL               (3)  // Hardware failure
#define CPG_RET_UNKNOWNSTA           (4)  // Unknown hardware state
#define CPG_RET_ERROR                (-1) // Other errors

/*
 * MSTP registers macros
 */ 
#define CPG_MSTP_REG_BIT_TO_ID(_reg, _bit)   ( ((_reg) * 100) + (_bit) )
#define CPG_MSTP_ID_TO_REG_OFFSET(_id)       ( (_id) / 100 )
#define CPG_MSTP_ID_TO_BIT(_id)              ( (_id) % 100 )

#define CPG_MSTP_GEN3_RTDMAC0          CPG_MSTP_REG_BIT_TO_ID(0, 21)
#define CPG_MSTP_GEN3_RTDMAC1          CPG_MSTP_REG_BIT_TO_ID(0, 16)
#define CPG_MSTP_GEN3_AXVIP012_REG     CPG_MSTP_REG_BIT_TO_ID(0,  4)

#define CPG_MSTP_GEN3_VCP4_VDPB        CPG_MSTP_REG_BIT_TO_ID(1, 31)
#define CPG_MSTP_GEN3_VCP4_VCPLF       CPG_MSTP_REG_BIT_TO_ID(1, 30)
#define CPG_MSTP_GEN3_iVDP1C           CPG_MSTP_REG_BIT_TO_ID(1, 28)
#define CPG_MSTP_GEN3_iVCP1E           CPG_MSTP_REG_BIT_TO_ID(1, 27)
#define CPG_MSTP_GEN3_TMU0             CPG_MSTP_REG_BIT_TO_ID(1, 25)
#define CPG_MSTP_GEN3_TMU1             CPG_MSTP_REG_BIT_TO_ID(1, 24)
#define CPG_MSTP_GEN3_TMU2             CPG_MSTP_REG_BIT_TO_ID(1, 23)
#define CPG_MSTP_GEN3_TMU3             CPG_MSTP_REG_BIT_TO_ID(1, 22)
#define CPG_MSTP_GEN3_TMU4             CPG_MSTP_REG_BIT_TO_ID(1, 21)
#define CPG_MSTP_GEN3_FDP10            CPG_MSTP_REG_BIT_TO_ID(1, 19)
#define CPG_MSTP_GEN3_FDP11            CPG_MSTP_REG_BIT_TO_ID(1, 18)
#define CPG_MSTP_GEN3_KCRC0            CPG_MSTP_REG_BIT_TO_ID(1, 14)
#define CPG_MSTP_GEN3_KCRC1            CPG_MSTP_REG_BIT_TO_ID(1, 13)
#define CPG_MSTP_GEN3_3DGE             CPG_MSTP_REG_BIT_TO_ID(1, 12)
#define CPG_MSTP_GEN3_KCRC2            CPG_MSTP_REG_BIT_TO_ID(1, 11)
#define CPG_MSTP_GEN3_KCRC3            CPG_MSTP_REG_BIT_TO_ID(1, 10)
#define CPG_MSTP_GEN3_SSP1             CPG_MSTP_REG_BIT_TO_ID(1,  9)
#define CPG_MSTP_GEN3_TSIF0            CPG_MSTP_REG_BIT_TO_ID(1,  8)
#define CPG_MSTP_GEN3_TSIF1            CPG_MSTP_REG_BIT_TO_ID(1,  7)
#define CPG_MSTP_GEN3_STBE             CPG_MSTP_REG_BIT_TO_ID(1,  5)
#define CPG_MSTP_GEN3_DOF              CPG_MSTP_REG_BIT_TO_ID(1,  2)
#define CPG_MSTP_GEN3_STV              CPG_MSTP_REG_BIT_TO_ID(1,  1)

#define CPG_MSTP_GEN3_SCEG             CPG_MSTP_REG_BIT_TO_ID(2, 26)
#define CPG_MSTP_GEN3_ARM_REALTIME     CPG_MSTP_REG_BIT_TO_ID(2, 22)
#define CPG_MSTP_GEN3_SYS_DMAC0        CPG_MSTP_REG_BIT_TO_ID(2, 19)
#define CPG_MSTP_GEN3_SYSDMAC1         CPG_MSTP_REG_BIT_TO_ID(2, 18)
#define CPG_MSTP_GEN3_SYSDMAC2         CPG_MSTP_REG_BIT_TO_ID(2, 17)
#define CPG_MSTP_GEN3_RT_SRAM          CPG_MSTP_REG_BIT_TO_ID(2, 15)
#define CPG_MSTP_GEN3_MFIS             CPG_MSTP_REG_BIT_TO_ID(2, 13)
#define CPG_MSTP_GEN3_MSIOF0           CPG_MSTP_REG_BIT_TO_ID(2, 11)
#define CPG_MSTP_GEN3_MSIOF1           CPG_MSTP_REG_BIT_TO_ID(2, 10)
#define CPG_MSTP_GEN3_MSIOF2           CPG_MSTP_REG_BIT_TO_ID(2,  9)
#define CPG_MSTP_GEN3_MSIOF3           CPG_MSTP_REG_BIT_TO_ID(2,  8)
#define CPG_MSTP_GEN3_SCIF0            CPG_MSTP_REG_BIT_TO_ID(2,  7)
#define CPG_MSTP_GEN3_SCIF1            CPG_MSTP_REG_BIT_TO_ID(2,  6)
#define CPG_MSTP_GEN3_SCIF3            CPG_MSTP_REG_BIT_TO_ID(2,  4)
#define CPG_MSTP_GEN3_SCIF4            CPG_MSTP_REG_BIT_TO_ID(2,  3)
#define CPG_MSTP_GEN3_SCIF5            CPG_MSTP_REG_BIT_TO_ID(2,  2)

#define CPG_MSTP_GEN3_USB_DMAC01       CPG_MSTP_REG_BIT_TO_ID(3, 31)
#define CPG_MSTP_GEN3_USB_DMAC00       CPG_MSTP_REG_BIT_TO_ID(3, 30)
#define CPG_MSTP_GEN3_USB_DMAC31       CPG_MSTP_REG_BIT_TO_ID(3, 29)
#define CPG_MSTP_GEN3_USB30_IF0        CPG_MSTP_REG_BIT_TO_ID(3, 28)
#define CPG_MSTP_GEN3_USB_DMAC30       CPG_MSTP_REG_BIT_TO_ID(3, 26)
#define CPG_MSTP_GEN3_WWDT0            CPG_MSTP_REG_BIT_TO_ID(3, 25)
#define CPG_MSTP_GEN3_WWDT1            CPG_MSTP_REG_BIT_TO_ID(3, 24)
#define CPG_MSTP_GEN3_CRC2             CPG_MSTP_REG_BIT_TO_ID(3, 23)
#define CPG_MSTP_GEN3_CRC3             CPG_MSTP_REG_BIT_TO_ID(3, 22)
#define CPG_MSTP_GEN3_WWDT2            CPG_MSTP_REG_BIT_TO_ID(3, 21)
#define CPG_MSTP_GEN3_PCIEC0           CPG_MSTP_REG_BIT_TO_ID(3, 19)
#define CPG_MSTP_GEN3_PCIEC1           CPG_MSTP_REG_BIT_TO_ID(3, 18)
#define CPG_MSTP_GEN3_PCIEC0_PIPE      CPG_MSTP_REG_BIT_TO_ID(3, 17)
#define CPG_MSTP_GEN3_PCIEC1_PIPE      CPG_MSTP_REG_BIT_TO_ID(3, 16)
#define CPG_MSTP_GEN3_IPC              CPG_MSTP_REG_BIT_TO_ID(3, 15)
#define CPG_MSTP_GEN3_SDIF             CPG_MSTP_REG_BIT_TO_ID(3, 14)
#define CPG_MSTP_GEN3_SD_IF1           CPG_MSTP_REG_BIT_TO_ID(3, 13)
#define CPG_MSTP_GEN3_SD_IF2           CPG_MSTP_REG_BIT_TO_ID(3, 12)
#define CPG_MSTP_GEN3_SD_IF3           CPG_MSTP_REG_BIT_TO_ID(3, 11)
#define CPG_MSTP_GEN3_SCIF2            CPG_MSTP_REG_BIT_TO_ID(3, 10)
#define CPG_MSTP_GEN3_WWDT3            CPG_MSTP_REG_BIT_TO_ID(3,  9)
#define CPG_MSTP_GEN3_CRC0             CPG_MSTP_REG_BIT_TO_ID(3,  7)
#define CPG_MSTP_GEN3_CRC1             CPG_MSTP_REG_BIT_TO_ID(3,  6)
#define CPG_MSTP_GEN3_TPU0             CPG_MSTP_REG_BIT_TO_ID(3,  4)
#define CPG_MSTP_GEN3_CMT0             CPG_MSTP_REG_BIT_TO_ID(3,  3)
#define CPG_MSTP_GEN3_CMT1             CPG_MSTP_REG_BIT_TO_ID(3,  2)
#define CPG_MSTP_GEN3_CMT2             CPG_MSTP_REG_BIT_TO_ID(3,  1)
#define CPG_MSTP_GEN3_CMT3             CPG_MSTP_REG_BIT_TO_ID(3,  0)

#define CPG_MSTP_GEN3_SUCMT            CPG_MSTP_REG_BIT_TO_ID(4, 31)
#define CPG_MSTP_GEN3_AXMM_REG         CPG_MSTP_REG_BIT_TO_ID(4, 15)
#define CPG_MSTP_GEN3_RWDT             CPG_MSTP_REG_BIT_TO_ID(4,  2)

#define CPG_MSTP_GEN3_IMPOCV2          CPG_MSTP_REG_BIT_TO_ID(5, 31)
#define CPG_MSTP_GEN3_IMPOCV3          CPG_MSTP_REG_BIT_TO_ID(5, 29)
#define CPG_MSTP_GEN3_IMPOCV4          CPG_MSTP_REG_BIT_TO_ID(5, 28)
#define CPG_MSTP_GEN3_IMPDMA0          CPG_MSTP_REG_BIT_TO_ID(5, 27)
#define CPG_MSTP_GEN3_IMPDMA1          CPG_MSTP_REG_BIT_TO_ID(5, 26)
#define CPG_MSTP_GEN3_IMPPSC0          CPG_MSTP_REG_BIT_TO_ID(5, 25)
#define CPG_MSTP_GEN3_IMPSlimDMAC      CPG_MSTP_REG_BIT_TO_ID(5, 24)
#define CPG_MSTP_GEN3_PWM              CPG_MSTP_REG_BIT_TO_ID(5, 23)
#define CPG_MSTP_GEN3_THS              CPG_MSTP_REG_BIT_TO_ID(5, 22)
#define CPG_MSTP_GEN3_IMP              CPG_MSTP_REG_BIT_TO_ID(5, 21)
#define CPG_MSTP_GEN3_HSCIF0           CPG_MSTP_REG_BIT_TO_ID(5, 20)
#define CPG_MSTP_GEN3_HSCIF1           CPG_MSTP_REG_BIT_TO_ID(5, 19)
#define CPG_MSTP_GEN3_HSCIF2           CPG_MSTP_REG_BIT_TO_ID(5, 18)
#define CPG_MSTP_GEN3_HSCIF3           CPG_MSTP_REG_BIT_TO_ID(5, 17)
#define CPG_MSTP_GEN3_HSCIF4           CPG_MSTP_REG_BIT_TO_ID(5, 16)
#define CPG_MSTP_GEN3_DRIF0            CPG_MSTP_REG_BIT_TO_ID(5, 15)
#define CPG_MSTP_GEN3_DRIF1            CPG_MSTP_REG_BIT_TO_ID(5, 14)
#define CPG_MSTP_GEN3_DRIF2            CPG_MSTP_REG_BIT_TO_ID(5, 13)
#define CPG_MSTP_GEN3_DRIF3            CPG_MSTP_REG_BIT_TO_ID(5, 12)
#define CPG_MSTP_GEN3_DRIF4            CPG_MSTP_REG_BIT_TO_ID(5, 11)
#define CPG_MSTP_GEN3_DRIF5            CPG_MSTP_REG_BIT_TO_ID(5, 10)
#define CPG_MSTP_GEN3_DRIF6            CPG_MSTP_REG_BIT_TO_ID(5,  9)
#define CPG_MSTP_GEN3_DRIF7            CPG_MSTP_REG_BIT_TO_ID(5,  8)
#define CPG_MSTP_GEN3_ADSP             CPG_MSTP_REG_BIT_TO_ID(5,  6)
#define CPG_MSTP_GEN3_FLEXRAY          CPG_MSTP_REG_BIT_TO_ID(5,  5)
#define CPG_MSTP_GEN3_ADC              CPG_MSTP_REG_BIT_TO_ID(5,  3)
#define CPG_MSTP_GEN3_Audio_DMAC0      CPG_MSTP_REG_BIT_TO_ID(5,  2)
#define CPG_MSTP_GEN3_Audio_DMAC1      CPG_MSTP_REG_BIT_TO_ID(5,  1)
#define CPG_MSTP_GEN3_SIMP             CPG_MSTP_REG_BIT_TO_ID(5,  0)

#define CPG_MSTP_GEN3_VSPI0            CPG_MSTP_REG_BIT_TO_ID(6, 31)
#define CPG_MSTP_GEN3_VSPI1            CPG_MSTP_REG_BIT_TO_ID(6, 30)
#define CPG_MSTP_GEN3_VIN8             CPG_MSTP_REG_BIT_TO_ID(6, 28)
#define CPG_MSTP_GEN3_VIN9             CPG_MSTP_REG_BIT_TO_ID(6, 27)
#define CPG_MSTP_GEN3_VSPBD            CPG_MSTP_REG_BIT_TO_ID(6, 26)
#define CPG_MSTP_GEN3_VIN10            CPG_MSTP_REG_BIT_TO_ID(6, 25)
#define CPG_MSTP_GEN3_VSPBC            CPG_MSTP_REG_BIT_TO_ID(6, 24)
#define CPG_MSTP_GEN3_VSPD0            CPG_MSTP_REG_BIT_TO_ID(6, 23)
#define CPG_MSTP_GEN3_VSPD1            CPG_MSTP_REG_BIT_TO_ID(6, 22)
#define CPG_MSTP_GEN3_VSPD2            CPG_MSTP_REG_BIT_TO_ID(6, 21)
#define CPG_MSTP_GEN3_FCPCS            CPG_MSTP_REG_BIT_TO_ID(6, 19)
#define CPG_MSTP_GEN3_VIN11            CPG_MSTP_REG_BIT_TO_ID(6, 18)
#define CPG_MSTP_GEN3_FCPF0            CPG_MSTP_REG_BIT_TO_ID(6, 15)
#define CPG_MSTP_GEN3_FCPF1            CPG_MSTP_REG_BIT_TO_ID(6, 14)
#define CPG_MSTP_GEN3_VIN12            CPG_MSTP_REG_BIT_TO_ID(6, 12)
#define CPG_MSTP_GEN3_FCPVI0           CPG_MSTP_REG_BIT_TO_ID(6, 11)
#define CPG_MSTP_GEN3_FCPVI1           CPG_MSTP_REG_BIT_TO_ID(6, 10)
#define CPG_MSTP_GEN3_VIN13            CPG_MSTP_REG_BIT_TO_ID(6,  8)
#define CPG_MSTP_GEN3_FCPVVB0          CPG_MSTP_REG_BIT_TO_ID(6,  7)
#define CPG_MSTP_GEN3_FCPVVB1          CPG_MSTP_REG_BIT_TO_ID(6,  6)
#define CPG_MSTP_GEN3_VIN14            CPG_MSTP_REG_BIT_TO_ID(6,  5)
#define CPG_MSTP_GEN3_VIN15            CPG_MSTP_REG_BIT_TO_ID(6,  4)
#define CPG_MSTP_GEN3_FCPVD0           CPG_MSTP_REG_BIT_TO_ID(6,  3)
#define CPG_MSTP_GEN3_FCPVD1           CPG_MSTP_REG_BIT_TO_ID(6,  2)
#define CPG_MSTP_GEN3_FCPVD2           CPG_MSTP_REG_BIT_TO_ID(6,  1)

#define CPG_MSTP_GEN3_HDMI_IF0         CPG_MSTP_REG_BIT_TO_ID(7, 29)
#define CPG_MSTP_GEN3_HDMI_IF1         CPG_MSTP_REG_BIT_TO_ID(7, 28)
#define CPG_MSTP_GEN3_LVDS             CPG_MSTP_REG_BIT_TO_ID(7, 27)
#define CPG_MSTP_GEN3_DU0              CPG_MSTP_REG_BIT_TO_ID(7, 24)
#define CPG_MSTP_GEN3_DU1              CPG_MSTP_REG_BIT_TO_ID(7, 23)
#define CPG_MSTP_GEN3_DU2              CPG_MSTP_REG_BIT_TO_ID(7, 22)
#define CPG_MSTP_GEN3_DU3              CPG_MSTP_REG_BIT_TO_ID(7, 21)
#define CPG_MSTP_GEN3_TCON0            CPG_MSTP_REG_BIT_TO_ID(7, 20)
#define CPG_MSTP_GEN3_TCON1            CPG_MSTP_REG_BIT_TO_ID(7, 19)
#define CPG_MSTP_GEN3_DOC0             CPG_MSTP_REG_BIT_TO_ID(7, 18)
#define CPG_MSTP_GEN3_DOC1             CPG_MSTP_REG_BIT_TO_ID(7, 17)
#define CPG_MSTP_GEN3_CSI40            CPG_MSTP_REG_BIT_TO_ID(7, 16)
#define CPG_MSTP_GEN3_CSI41            CPG_MSTP_REG_BIT_TO_ID(7, 15)
#define CPG_MSTP_GEN3_CSI20            CPG_MSTP_REG_BIT_TO_ID(7, 14)
#define CPG_MSTP_GEN3_DCU              CPG_MSTP_REG_BIT_TO_ID(7, 12)
#define CPG_MSTP_GEN3_CMM0             CPG_MSTP_REG_BIT_TO_ID(7, 11)
#define CPG_MSTP_GEN3_CMM1             CPG_MSTP_REG_BIT_TO_ID(7, 10)
#define CPG_MSTP_GEN3_CMM2             CPG_MSTP_REG_BIT_TO_ID(7,  9)
#define CPG_MSTP_GEN3_CMM3             CPG_MSTP_REG_BIT_TO_ID(7,  8)
#define CPG_MSTP_GEN3_IMRLX44          CPG_MSTP_REG_BIT_TO_ID(7,  7)
#define CPG_MSTP_GEN3_IMRLX45          CPG_MSTP_REG_BIT_TO_ID(7,  6)
#define CPG_MSTP_GEN3_HS_USB_IF3       CPG_MSTP_REG_BIT_TO_ID(7,  5)
#define CPG_MSTP_GEN3_HS_USB_IF0       CPG_MSTP_REG_BIT_TO_ID(7,  4)
#define CPG_MSTP_GEN3_EHCI_OHCI0       CPG_MSTP_REG_BIT_TO_ID(7,  3)
#define CPG_MSTP_GEN3_EHCI_OHCI1       CPG_MSTP_REG_BIT_TO_ID(7,  2)
#define CPG_MSTP_GEN3_EHCI_OHCI2       CPG_MSTP_REG_BIT_TO_ID(7,  1)
#define CPG_MSTP_GEN3_EHCI_OHCI3       CPG_MSTP_REG_BIT_TO_ID(7,  0)

#define CPG_MSTP_GEN3_IMPCNN           CPG_MSTP_REG_BIT_TO_ID(8, 31)
#define CPG_MSTP_GEN3_IMPRAM           CPG_MSTP_REG_BIT_TO_ID(8, 30)
#define CPG_MSTP_GEN3_IMPOCV0          CPG_MSTP_REG_BIT_TO_ID(8, 29)
#define CPG_MSTP_GEN3_IMPOCV1          CPG_MSTP_REG_BIT_TO_ID(8, 28)
#define CPG_MSTP_GEN3_IMP0             CPG_MSTP_REG_BIT_TO_ID(8, 27)
#define CPG_MSTP_GEN3_IMP1             CPG_MSTP_REG_BIT_TO_ID(8, 26)
#define CPG_MSTP_GEN3_IMP2             CPG_MSTP_REG_BIT_TO_ID(8, 25)
#define CPG_MSTP_GEN3_IMP3             CPG_MSTP_REG_BIT_TO_ID(8, 24)
#define CPG_MSTP_GEN3_IMRLX40          CPG_MSTP_REG_BIT_TO_ID(8, 23)
#define CPG_MSTP_GEN3_IMRLX41          CPG_MSTP_REG_BIT_TO_ID(8, 22)
#define CPG_MSTP_GEN3_IMRLX42          CPG_MSTP_REG_BIT_TO_ID(8, 21)
#define CPG_MSTP_GEN3_IMRLX43          CPG_MSTP_REG_BIT_TO_ID(8, 20)
#define CPG_MSTP_GEN3_ISP0             CPG_MSTP_REG_BIT_TO_ID(8, 17)
#define CPG_MSTP_GEN3_SATA             CPG_MSTP_REG_BIT_TO_ID(8, 15)
#define CPG_MSTP_GEN3_ISP1             CPG_MSTP_REG_BIT_TO_ID(8, 14)
#define CPG_MSTP_GEN3_GIGABIT_ETHERNET CPG_MSTP_REG_BIT_TO_ID(8, 13)
#define CPG_MSTP_GEN3_EAVBIF           CPG_MSTP_REG_BIT_TO_ID(8, 12)
#define CPG_MSTP_GEN3_VIN0             CPG_MSTP_REG_BIT_TO_ID(8, 11)
#define CPG_MSTP_GEN3_VIN1             CPG_MSTP_REG_BIT_TO_ID(8, 10)
#define CPG_MSTP_GEN3_VIN2             CPG_MSTP_REG_BIT_TO_ID(8,  9)
#define CPG_MSTP_GEN3_VIN3             CPG_MSTP_REG_BIT_TO_ID(8,  8)
#define CPG_MSTP_GEN3_VIN4             CPG_MSTP_REG_BIT_TO_ID(8,  7)
#define CPG_MSTP_GEN3_VIN5             CPG_MSTP_REG_BIT_TO_ID(8,  6)
#define CPG_MSTP_GEN3_VIN6             CPG_MSTP_REG_BIT_TO_ID(8,  5)
#define CPG_MSTP_GEN3_VIN7             CPG_MSTP_REG_BIT_TO_ID(8,  4)
#define CPG_MSTP_GEN3_MLP              CPG_MSTP_REG_BIT_TO_ID(8,  2)
#define CPG_MSTP_GEN3_DAVE_HD          CPG_MSTP_REG_BIT_TO_ID(8,  1)
#define CPG_MSTP_GEN3_R_NANDC          CPG_MSTP_REG_BIT_TO_ID(8,  0)

#define CPG_MSTP_GEN3_I2C0             CPG_MSTP_REG_BIT_TO_ID(9, 31)
#define CPG_MSTP_GEN3_I2C1             CPG_MSTP_REG_BIT_TO_ID(9, 30)
#define CPG_MSTP_GEN3_I2C2             CPG_MSTP_REG_BIT_TO_ID(9, 29)
#define CPG_MSTP_GEN3_I2C3             CPG_MSTP_REG_BIT_TO_ID(9, 28)
#define CPG_MSTP_GEN3_I2C4             CPG_MSTP_REG_BIT_TO_ID(9, 27)
#define CPG_MSTP_GEN3_IIC_DVFS         CPG_MSTP_REG_BIT_TO_ID(9, 26)
#define CPG_MSTP_GEN3_MLM              CPG_MSTP_REG_BIT_TO_ID(9, 24)
#define CPG_MSTP_GEN3_DCTP             CPG_MSTP_REG_BIT_TO_ID(9, 23)
#define CPG_MSTP_GEN3_ADG              CPG_MSTP_REG_BIT_TO_ID(9, 22)
#define CPG_MSTP_GEN3_SIM              CPG_MSTP_REG_BIT_TO_ID(9, 20)
#define CPG_MSTP_GEN3_I2C5             CPG_MSTP_REG_BIT_TO_ID(9, 19)
#define CPG_MSTP_GEN3_I2C6             CPG_MSTP_REG_BIT_TO_ID(9, 18)
#define CPG_MSTP_GEN3_RPC              CPG_MSTP_REG_BIT_TO_ID(9, 17)
#define CPG_MSTP_GEN3_CAN_IF0          CPG_MSTP_REG_BIT_TO_ID(9, 16)
#define CPG_MSTP_GEN3_CAN_IF1          CPG_MSTP_REG_BIT_TO_ID(9, 15)
#define CPG_MSTP_GEN3_CANFD            CPG_MSTP_REG_BIT_TO_ID(9, 14)
#define CPG_MSTP_GEN3_GPIO0            CPG_MSTP_REG_BIT_TO_ID(9, 12)
#define CPG_MSTP_GEN3_GPIO1            CPG_MSTP_REG_BIT_TO_ID(9, 11)
#define CPG_MSTP_GEN3_GPIO2            CPG_MSTP_REG_BIT_TO_ID(9, 10)
#define CPG_MSTP_GEN3_GPIO3            CPG_MSTP_REG_BIT_TO_ID(9,  9)
#define CPG_MSTP_GEN3_GPIO4            CPG_MSTP_REG_BIT_TO_ID(9,  8)
#define CPG_MSTP_GEN3_GPIO5            CPG_MSTP_REG_BIT_TO_ID(9,  7)
#define CPG_MSTP_GEN3_GPIO6            CPG_MSTP_REG_BIT_TO_ID(9,  6)
#define CPG_MSTP_GEN3_GPIO7            CPG_MSTP_REG_BIT_TO_ID(9,  5)
#define CPG_MSTP_GEN3_FM               CPG_MSTP_REG_BIT_TO_ID(9,  4)
#define CPG_MSTP_GEN3_IR               CPG_MSTP_REG_BIT_TO_ID(9,  3)
#define CPG_MSTP_GEN3_SPIF             CPG_MSTP_REG_BIT_TO_ID(9,  2)
#define CPG_MSTP_GEN3_GRYOIF           CPG_MSTP_REG_BIT_TO_ID(9,  1)

#define CPG_MSTP_GEN3_SCU_SRC0         CPG_MSTP_REG_BIT_TO_ID(10, 31)
#define CPG_MSTP_GEN3_SCU_SRC1         CPG_MSTP_REG_BIT_TO_ID(10, 30)
#define CPG_MSTP_GEN3_SCU_SRC2         CPG_MSTP_REG_BIT_TO_ID(10, 29)
#define CPG_MSTP_GEN3_SCU_SRC3         CPG_MSTP_REG_BIT_TO_ID(10, 28)
#define CPG_MSTP_GEN3_SCU_SRC4         CPG_MSTP_REG_BIT_TO_ID(10, 27)
#define CPG_MSTP_GEN3_SCU_SRC5         CPG_MSTP_REG_BIT_TO_ID(10, 26)
#define CPG_MSTP_GEN3_SCU_SRC6         CPG_MSTP_REG_BIT_TO_ID(10, 25)
#define CPG_MSTP_GEN3_SCU_SRC7         CPG_MSTP_REG_BIT_TO_ID(10, 24)
#define CPG_MSTP_GEN3_SCU_SRC8         CPG_MSTP_REG_BIT_TO_ID(10, 23)
#define CPG_MSTP_GEN3_SCU_SRC9         CPG_MSTP_REG_BIT_TO_ID(10, 22)
#define CPG_MSTP_GEN3_SCU0             CPG_MSTP_REG_BIT_TO_ID(10, 21)
#define CPG_MSTP_GEN3_SCU1             CPG_MSTP_REG_BIT_TO_ID(10, 20)
#define CPG_MSTP_GEN3_SCU_DCV0         CPG_MSTP_REG_BIT_TO_ID(10, 19)
#define CPG_MSTP_GEN3_SCU_DCV1         CPG_MSTP_REG_BIT_TO_ID(10, 18)
#define CPG_MSTP_GEN3_SCU_ALL          CPG_MSTP_REG_BIT_TO_ID(10, 17)
#define CPG_MSTP_GEN3_SSI0             CPG_MSTP_REG_BIT_TO_ID(10, 15)
#define CPG_MSTP_GEN3_SSI1             CPG_MSTP_REG_BIT_TO_ID(10, 14)
#define CPG_MSTP_GEN3_SSI2             CPG_MSTP_REG_BIT_TO_ID(10, 13)
#define CPG_MSTP_GEN3_SSI3             CPG_MSTP_REG_BIT_TO_ID(10, 12)
#define CPG_MSTP_GEN3_SSI4             CPG_MSTP_REG_BIT_TO_ID(10, 11)
#define CPG_MSTP_GEN3_SSI5             CPG_MSTP_REG_BIT_TO_ID(10, 10)
#define CPG_MSTP_GEN3_SSI6             CPG_MSTP_REG_BIT_TO_ID(10,  9)
#define CPG_MSTP_GEN3_SSI7             CPG_MSTP_REG_BIT_TO_ID(10,  8)
#define CPG_MSTP_GEN3_SSI8             CPG_MSTP_REG_BIT_TO_ID(10,  7)
#define CPG_MSTP_GEN3_SSI9             CPG_MSTP_REG_BIT_TO_ID(10,  6)
#define CPG_MSTP_GEN3_SSI_ALL          CPG_MSTP_REG_BIT_TO_ID(10,  5)
#define CPG_MSTP_GEN3_ACF0             CPG_MSTP_REG_BIT_TO_ID(10,  4)
#define CPG_MSTP_GEN3_ACF1             CPG_MSTP_REG_BIT_TO_ID(10,  3)
#define CPG_MSTP_GEN3_ACF2             CPG_MSTP_REG_BIT_TO_ID(10,  2)
#define CPG_MSTP_GEN3_ACF3             CPG_MSTP_REG_BIT_TO_ID(10,  1)
#define CPG_MSTP_GEN3_ACF4             CPG_MSTP_REG_BIT_TO_ID(10,  0)

#define CPG_MSTP_GEN3_AXSM             CPG_MSTP_REG_BIT_TO_ID(11, 17)
#define CPG_MSTP_GEN3_AXSM_REG         CPG_MSTP_REG_BIT_TO_ID(11, 16)

#define CPG_MSTP_V3U_OCV3              CPG_MSTP_REG_BIT_TO_ID(0, 31)
#define CPG_MSTP_V3U_OCV2              CPG_MSTP_REG_BIT_TO_ID(0, 30)
#define CPG_MSTP_V3U_OCV1              CPG_MSTP_REG_BIT_TO_ID(0, 29)
#define CPG_MSTP_V3U_OCV0              CPG_MSTP_REG_BIT_TO_ID(0, 28)
#define CPG_MSTP_V3U_IMPPSC0           CPG_MSTP_REG_BIT_TO_ID(0, 27)
#define CPG_MSTP_V3U_IMPDMA0           CPG_MSTP_REG_BIT_TO_ID(0, 26)
#define CPG_MSTP_V3U_IMP1              CPG_MSTP_REG_BIT_TO_ID(0, 25)
#define CPG_MSTP_V3U_IMP0              CPG_MSTP_REG_BIT_TO_ID(0, 24)
#define CPG_MSTP_V3U_SPMC0             CPG_MSTP_REG_BIT_TO_ID(0, 23)
#define CPG_MSTP_V3U_IMPCNN0           CPG_MSTP_REG_BIT_TO_ID(0, 22)
#define CPG_MSTP_V3U_ISP3              CPG_MSTP_REG_BIT_TO_ID(0, 19)
#define CPG_MSTP_V3U_ISP2              CPG_MSTP_REG_BIT_TO_ID(0, 18)
#define CPG_MSTP_V3U_ISP1              CPG_MSTP_REG_BIT_TO_ID(0, 17)
#define CPG_MSTP_V3U_ISP0              CPG_MSTP_REG_BIT_TO_ID(0, 16)
#define CPG_MSTP_V3U_CLE3              CPG_MSTP_REG_BIT_TO_ID(0, 14)
#define CPG_MSTP_V3U_CLE2              CPG_MSTP_REG_BIT_TO_ID(0, 13)
#define CPG_MSTP_V3U_CLE1              CPG_MSTP_REG_BIT_TO_ID(0, 12)
#define CPG_MSTP_V3U_CLE0              CPG_MSTP_REG_BIT_TO_ID(0, 11)
#define CPG_MSTP_V3U_UMFL1             CPG_MSTP_REG_BIT_TO_ID(0, 10)
#define CPG_MSTP_V3U_UMFL0             CPG_MSTP_REG_BIT_TO_ID(0,  9)
#define CPG_MSTP_V3U_DISP0_0           CPG_MSTP_REG_BIT_TO_ID(0,  2)
#define CPG_MSTP_V3U_DISP0_1           CPG_MSTP_REG_BIT_TO_ID(0,  1)
#define CPG_MSTP_V3U_RGX               CPG_MSTP_REG_BIT_TO_ID(0,  0)

#define CPG_MSTP_V3U_ADVFSC            CPG_MSTP_REG_BIT_TO_ID(1, 23)
#define CPG_MSTP_V3U_SPMI1             CPG_MSTP_REG_BIT_TO_ID(1, 21)
#define CPG_MSTP_V3U_SPMI0             CPG_MSTP_REG_BIT_TO_ID(1, 20)
#define CPG_MSTP_V3U_IPMMUIR           CPG_MSTP_REG_BIT_TO_ID(1, 19)
#define CPG_MSTP_V3U_IMPSLV            CPG_MSTP_REG_BIT_TO_ID(1, 18)
#define CPG_MSTP_V3U_IMPLDMA           CPG_MSTP_REG_BIT_TO_ID(1, 17)
#define CPG_MSTP_V3U_IMPDTA            CPG_MSTP_REG_BIT_TO_ID(1, 16)
#define CPG_MSTP_V3U_SPMC1             CPG_MSTP_REG_BIT_TO_ID(1,  9)
#define CPG_MSTP_V3U_IMPCNN1           CPG_MSTP_REG_BIT_TO_ID(1,  8)
#define CPG_MSTP_V3U_OCV5              CPG_MSTP_REG_BIT_TO_ID(1,  7)
#define CPG_MSTP_V3U_IMPPSC1           CPG_MSTP_REG_BIT_TO_ID(1,  6)
#define CPG_MSTP_V3U_IMPDMA1           CPG_MSTP_REG_BIT_TO_ID(1,  5)
#define CPG_MSTP_V3U_IMP3              CPG_MSTP_REG_BIT_TO_ID(1,  4)
#define CPG_MSTP_V3U_IMP2              CPG_MSTP_REG_BIT_TO_ID(1,  3)
#define CPG_MSTP_V3U_SPMC2             CPG_MSTP_REG_BIT_TO_ID(1,  2)
#define CPG_MSTP_V3U_IMPCNN2           CPG_MSTP_REG_BIT_TO_ID(1,  1)
#define CPG_MSTP_V3U_OCV4              CPG_MSTP_REG_BIT_TO_ID(1,  0)

#define CPG_MSTP_V3U_AVB5              CPG_MSTP_REG_BIT_TO_ID(2, 16)
#define CPG_MSTP_V3U_AVB4              CPG_MSTP_REG_BIT_TO_ID(2, 15)
#define CPG_MSTP_V3U_AVB3              CPG_MSTP_REG_BIT_TO_ID(2, 14)
#define CPG_MSTP_V3U_AVB2              CPG_MSTP_REG_BIT_TO_ID(2, 13)
#define CPG_MSTP_V3U_AVB1              CPG_MSTP_REG_BIT_TO_ID(2, 12)
#define CPG_MSTP_V3U_AVB0              CPG_MSTP_REG_BIT_TO_ID(2, 11)

#define CPG_MSTP_V3U_CSI4LNK0          CPG_MSTP_REG_BIT_TO_ID(3, 31)
#define CPG_MSTP_V3U_CSD               CPG_MSTP_REG_BIT_TO_ID(3, 30)
#define CPG_MSTP_V3U_CR0               CPG_MSTP_REG_BIT_TO_ID(3, 29)
#define CPG_MSTP_V3U_CANFD             CPG_MSTP_REG_BIT_TO_ID(3, 28)
#define CPG_MSTP_V3U_OCV7              CPG_MSTP_REG_BIT_TO_ID(3, 14)
#define CPG_MSTP_V3U_OCV6              CPG_MSTP_REG_BIT_TO_ID(3, 13)

#define CPG_MSTP_V3U_DSITXLINK1        CPG_MSTP_REG_BIT_TO_ID(4, 16)
#define CPG_MSTP_V3U_DSITXLINK0        CPG_MSTP_REG_BIT_TO_ID(4, 15)
#define CPG_MSTP_V3U_DOC               CPG_MSTP_REG_BIT_TO_ID(4, 14)
#define CPG_MSTP_V3U_IPMMUVI1          CPG_MSTP_REG_BIT_TO_ID(4, 13)
#define CPG_MSTP_V3U_IPMMUVI0          CPG_MSTP_REG_BIT_TO_ID(4, 12)
#define CPG_MSTP_V3U_DU                CPG_MSTP_REG_BIT_TO_ID(4, 11)
#define CPG_MSTP_V3U_DDR0_DDR1         CPG_MSTP_REG_BIT_TO_ID(4,  9)
#define CPG_MSTP_V3U_CSI4LNK3          CPG_MSTP_REG_BIT_TO_ID(4,  2)
#define CPG_MSTP_V3U_CSI4LNK2          CPG_MSTP_REG_BIT_TO_ID(4,  1)
#define CPG_MSTP_V3U_CSI4LNK1          CPG_MSTP_REG_BIT_TO_ID(4,  0)

#define CPG_MSTP_V3U_INTAP_5_31        CPG_MSTP_REG_BIT_TO_ID(5, 31)
#define CPG_MSTP_V3U_IMR1              CPG_MSTP_REG_BIT_TO_ID(5, 30)
#define CPG_MSTP_V3U_IMR0              CPG_MSTP_REG_BIT_TO_ID(5, 29)
#define CPG_MSTP_V3U_IMR5              CPG_MSTP_REG_BIT_TO_ID(5, 28)
#define CPG_MSTP_V3U_IMR4              CPG_MSTP_REG_BIT_TO_ID(5, 27)
#define CPG_MSTP_V3U_IMR3              CPG_MSTP_REG_BIT_TO_ID(5, 26)
#define CPG_MSTP_V3U_IMR2              CPG_MSTP_REG_BIT_TO_ID(5, 25)
#define CPG_MSTP_V3U_I2C6              CPG_MSTP_REG_BIT_TO_ID(5, 24)
#define CPG_MSTP_V3U_I2C5              CPG_MSTP_REG_BIT_TO_ID(5, 23)
#define CPG_MSTP_V3U_I2C4              CPG_MSTP_REG_BIT_TO_ID(5, 22)
#define CPG_MSTP_V3U_I2C3              CPG_MSTP_REG_BIT_TO_ID(5, 21)
#define CPG_MSTP_V3U_I2C2              CPG_MSTP_REG_BIT_TO_ID(5, 20)
#define CPG_MSTP_V3U_I2C1              CPG_MSTP_REG_BIT_TO_ID(5, 19)
#define CPG_MSTP_V3U_I2C0              CPG_MSTP_REG_BIT_TO_ID(5, 18)
#define CPG_MSTP_V3U_HSCIF3            CPG_MSTP_REG_BIT_TO_ID(5, 17)
#define CPG_MSTP_V3U_HSCIF2            CPG_MSTP_REG_BIT_TO_ID(5, 16)
#define CPG_MSTP_V3U_HSCIF1            CPG_MSTP_REG_BIT_TO_ID(5, 15)
#define CPG_MSTP_V3U_HSCIF0            CPG_MSTP_REG_BIT_TO_ID(5, 14)
#define CPG_MSTP_V3U_FRAY              CPG_MSTP_REG_BIT_TO_ID(5, 13)
#define CPG_MSTP_V3U_FCPVD1            CPG_MSTP_REG_BIT_TO_ID(5,  9)
#define CPG_MSTP_V3U_FCPVD0            CPG_MSTP_REG_BIT_TO_ID(5,  8)
#define CPG_MSTP_V3U_FCPCS             CPG_MSTP_REG_BIT_TO_ID(5,  7)

#define CPG_MSTP_V3U_RTDM1             CPG_MSTP_REG_BIT_TO_ID(6, 31)
#define CPG_MSTP_V3U_RTDM0             CPG_MSTP_REG_BIT_TO_ID(6, 30)
#define CPG_MSTP_V3U_RPC               CPG_MSTP_REG_BIT_TO_ID(6, 29)
#define CPG_MSTP_V3U_PWM0              CPG_MSTP_REG_BIT_TO_ID(6, 28)
#define CPG_MSTP_V3U_PCI31             CPG_MSTP_REG_BIT_TO_ID(6, 27)
#define CPG_MSTP_V3U_PCI21             CPG_MSTP_REG_BIT_TO_ID(6, 26)
#define CPG_MSTP_V3U_PCI11             CPG_MSTP_REG_BIT_TO_ID(6, 25)
#define CPG_MSTP_V3U_PCI01             CPG_MSTP_REG_BIT_TO_ID(6, 24)
#define CPG_MSTP_V3U_MSI5              CPG_MSTP_REG_BIT_TO_ID(6, 23)
#define CPG_MSTP_V3U_MSI4              CPG_MSTP_REG_BIT_TO_ID(6, 22)
#define CPG_MSTP_V3U_MSI3              CPG_MSTP_REG_BIT_TO_ID(6, 21)
#define CPG_MSTP_V3U_MSI2              CPG_MSTP_REG_BIT_TO_ID(6, 20)
#define CPG_MSTP_V3U_MSI1              CPG_MSTP_REG_BIT_TO_ID(6, 19)
#define CPG_MSTP_V3U_MSI0              CPG_MSTP_REG_BIT_TO_ID(6, 18)
#define CPG_MSTP_V3U_MFI               CPG_MSTP_REG_BIT_TO_ID(6, 17)
#define CPG_MSTP_V3U_IVCP1E            CPG_MSTP_REG_BIT_TO_ID(6, 16)
#define CPG_MSTP_V3U_ISPCS3            CPG_MSTP_REG_BIT_TO_ID(6, 15)
#define CPG_MSTP_V3U_ISPCS2            CPG_MSTP_REG_BIT_TO_ID(6, 14)
#define CPG_MSTP_V3U_ISPCS1            CPG_MSTP_REG_BIT_TO_ID(6, 13)
#define CPG_MSTP_V3U_ISPCS0            CPG_MSTP_REG_BIT_TO_ID(6, 12)
#define CPG_MSTP_V3U_IRQC              CPG_MSTP_REG_BIT_TO_ID(6, 11)
#define CPG_MSTP_V3U_IPMMUVIP1         CPG_MSTP_REG_BIT_TO_ID(6, 10)
#define CPG_MSTP_V3U_IPMMUVIP0         CPG_MSTP_REG_BIT_TO_ID(6,  9)
#define CPG_MSTP_V3U_IPMMUVC           CPG_MSTP_REG_BIT_TO_ID(6,  7)
#define CPG_MSTP_V3U_IPMMURT1          CPG_MSTP_REG_BIT_TO_ID(6,  6)
#define CPG_MSTP_V3U_IPMMURT0          CPG_MSTP_REG_BIT_TO_ID(6,  5)
#define CPG_MSTP_V3U_IPMMUPV0          CPG_MSTP_REG_BIT_TO_ID(6,  4)
#define CPG_MSTP_V3U_IPMMUMM           CPG_MSTP_REG_BIT_TO_ID(6,  3)
#define CPG_MSTP_V3U_IPMMUDS1          CPG_MSTP_REG_BIT_TO_ID(6,  2)
#define CPG_MSTP_V3U_IPMMUDS0          CPG_MSTP_REG_BIT_TO_ID(6,  1)
#define CPG_MSTP_V3U_IPC               CPG_MSTP_REG_BIT_TO_ID(6,  0)

#define CPG_MSTP_V3U_VIN1              CPG_MSTP_REG_BIT_TO_ID(7, 31)
#define CPG_MSTP_V3U_VIN0              CPG_MSTP_REG_BIT_TO_ID(7, 30)
#define CPG_MSTP_V3U_VCP4L             CPG_MSTP_REG_BIT_TO_ID(7, 29)
#define CPG_MSTP_V3U_TPU0              CPG_MSTP_REG_BIT_TO_ID(7, 18)
#define CPG_MSTP_V3U_TMU4              CPG_MSTP_REG_BIT_TO_ID(7, 17)
#define CPG_MSTP_V3U_TMU3              CPG_MSTP_REG_BIT_TO_ID(7, 16)
#define CPG_MSTP_V3U_TMU2              CPG_MSTP_REG_BIT_TO_ID(7, 15)
#define CPG_MSTP_V3U_TMU1              CPG_MSTP_REG_BIT_TO_ID(7, 14)
#define CPG_MSTP_V3U_TMU0              CPG_MSTP_REG_BIT_TO_ID(7, 13)
#define CPG_MSTP_V3U_BKBUF             CPG_MSTP_REG_BIT_TO_ID(7, 12)
#define CPG_MSTP_V3U_RTRAM0            CPG_MSTP_REG_BIT_TO_ID(7, 11)  // S4: RTRAM0, V3U: SYSRAM
#define CPG_MSTP_V3U_SYDM2             CPG_MSTP_REG_BIT_TO_ID(7, 10)
#define CPG_MSTP_V3U_SYDM1             CPG_MSTP_REG_BIT_TO_ID(7,  9)
#define CPG_MSTP_V3U_SECROM            CPG_MSTP_REG_BIT_TO_ID(7,  8)
#define CPG_MSTP_V3U_SDHI0             CPG_MSTP_REG_BIT_TO_ID(7,  6)
#define CPG_MSTP_V3U_SCIF4             CPG_MSTP_REG_BIT_TO_ID(7,  5)
#define CPG_MSTP_V3U_SCIF3             CPG_MSTP_REG_BIT_TO_ID(7,  4)
#define CPG_MSTP_V3U_SCIF1             CPG_MSTP_REG_BIT_TO_ID(7,  3)
#define CPG_MSTP_V3U_SCIF0             CPG_MSTP_REG_BIT_TO_ID(7,  2)
#define CPG_MSTP_V3U_RTDM3             CPG_MSTP_REG_BIT_TO_ID(7,  1)
#define CPG_MSTP_V3U_RTDM2             CPG_MSTP_REG_BIT_TO_ID(7,  0)

#define CPG_MSTP_V3U_VSPD1             CPG_MSTP_REG_BIT_TO_ID(8, 31)
#define CPG_MSTP_V3U_VSPD0             CPG_MSTP_REG_BIT_TO_ID(8, 30)
#define CPG_MSTP_V3U_VIN37             CPG_MSTP_REG_BIT_TO_ID(8, 29)
#define CPG_MSTP_V3U_VIN36             CPG_MSTP_REG_BIT_TO_ID(8, 28)
#define CPG_MSTP_V3U_VIN35             CPG_MSTP_REG_BIT_TO_ID(8, 27)
#define CPG_MSTP_V3U_VIN34             CPG_MSTP_REG_BIT_TO_ID(8, 26)
#define CPG_MSTP_V3U_VIN33             CPG_MSTP_REG_BIT_TO_ID(8, 25)
#define CPG_MSTP_V3U_VIN32             CPG_MSTP_REG_BIT_TO_ID(8, 24)
#define CPG_MSTP_V3U_VIN31             CPG_MSTP_REG_BIT_TO_ID(8, 23)
#define CPG_MSTP_V3U_VIN30             CPG_MSTP_REG_BIT_TO_ID(8, 22)
#define CPG_MSTP_V3U_VIN27             CPG_MSTP_REG_BIT_TO_ID(8, 21)
#define CPG_MSTP_V3U_VIN26             CPG_MSTP_REG_BIT_TO_ID(8, 20)
#define CPG_MSTP_V3U_VIN25             CPG_MSTP_REG_BIT_TO_ID(8, 19)
#define CPG_MSTP_V3U_VIN24             CPG_MSTP_REG_BIT_TO_ID(8, 18)
#define CPG_MSTP_V3U_VIN23             CPG_MSTP_REG_BIT_TO_ID(8, 17)
#define CPG_MSTP_V3U_VIN22             CPG_MSTP_REG_BIT_TO_ID(8, 16)
#define CPG_MSTP_V3U_VIN21             CPG_MSTP_REG_BIT_TO_ID(8, 15)
#define CPG_MSTP_V3U_VIN20             CPG_MSTP_REG_BIT_TO_ID(8, 14)
#define CPG_MSTP_V3U_VIN17             CPG_MSTP_REG_BIT_TO_ID(8, 13)
#define CPG_MSTP_V3U_VIN16             CPG_MSTP_REG_BIT_TO_ID(8, 12)
#define CPG_MSTP_V3U_VIN15             CPG_MSTP_REG_BIT_TO_ID(8, 11)
#define CPG_MSTP_V3U_VIN14             CPG_MSTP_REG_BIT_TO_ID(8, 10)
#define CPG_MSTP_V3U_VIN13             CPG_MSTP_REG_BIT_TO_ID(8,  9)
#define CPG_MSTP_V3U_VIN12             CPG_MSTP_REG_BIT_TO_ID(8,  8)
#define CPG_MSTP_V3U_VIN11             CPG_MSTP_REG_BIT_TO_ID(8,  7)
#define CPG_MSTP_V3U_VIN10             CPG_MSTP_REG_BIT_TO_ID(8,  6)
#define CPG_MSTP_V3U_VIN07             CPG_MSTP_REG_BIT_TO_ID(8,  5)
#define CPG_MSTP_V3U_VIN06             CPG_MSTP_REG_BIT_TO_ID(8,  4)
#define CPG_MSTP_V3U_VIN05             CPG_MSTP_REG_BIT_TO_ID(8,  3)
#define CPG_MSTP_V3U_VIN04             CPG_MSTP_REG_BIT_TO_ID(8,  2)
#define CPG_MSTP_V3U_VIN03             CPG_MSTP_REG_BIT_TO_ID(8,  1)
#define CPG_MSTP_V3U_VIN02             CPG_MSTP_REG_BIT_TO_ID(8,  0)

#define CPG_MSTP_V3U_UCMT              CPG_MSTP_REG_BIT_TO_ID(9, 20)
#define CPG_MSTP_V3U_TSC               CPG_MSTP_REG_BIT_TO_ID(9, 19)
#define CPG_MSTP_V3U_PFC3              CPG_MSTP_REG_BIT_TO_ID(9, 18)
#define CPG_MSTP_V3U_PFC2              CPG_MSTP_REG_BIT_TO_ID(9, 17)
#define CPG_MSTP_V3U_PFC1              CPG_MSTP_REG_BIT_TO_ID(9, 16)
#define CPG_MSTP_V3U_PFC0              CPG_MSTP_REG_BIT_TO_ID(9, 15)
#define CPG_MSTP_V3U_CMT3              CPG_MSTP_REG_BIT_TO_ID(9, 13)
#define CPG_MSTP_V3U_CMT2              CPG_MSTP_REG_BIT_TO_ID(9, 12)
#define CPG_MSTP_V3U_CMT1              CPG_MSTP_REG_BIT_TO_ID(9, 11)
#define CPG_MSTP_V3U_CMT0              CPG_MSTP_REG_BIT_TO_ID(9, 10)
#define CPG_MSTP_V3U_RWDT              CPG_MSTP_REG_BIT_TO_ID(9,  7)   // S4: WDT, V3U: RWDT
#define CPG_MSTP_V3U_WCRC3             CPG_MSTP_REG_BIT_TO_ID(9,  6)
#define CPG_MSTP_V3U_WCRC2             CPG_MSTP_REG_BIT_TO_ID(9,  5)
#define CPG_MSTP_V3U_WCRC1             CPG_MSTP_REG_BIT_TO_ID(9,  4)
#define CPG_MSTP_V3U_WCRC0             CPG_MSTP_REG_BIT_TO_ID(9,  3)

#define CPG_MSTP_V3U_VSPX3             CPG_MSTP_REG_BIT_TO_ID(10, 31)
#define CPG_MSTP_V3U_VSPX2             CPG_MSTP_REG_BIT_TO_ID(10, 30)
#define CPG_MSTP_V3U_VSPX1             CPG_MSTP_REG_BIT_TO_ID(10, 29)
#define CPG_MSTP_V3U_VSPX0             CPG_MSTP_REG_BIT_TO_ID(10, 28)

#define CPG_MSTP_V3U_SWDT0             CPG_MSTP_REG_BIT_TO_ID(11, 28)  // S4: SWDT0/1, V3U: SWDT0
#define CPG_MSTP_V3U_SWDT1             CPG_MSTP_REG_BIT_TO_ID(11, 27)
#define CPG_MSTP_V3U_RGXPVC            CPG_MSTP_REG_BIT_TO_ID(11, 26)
#define CPG_MSTP_V3U_RGXPVD            CPG_MSTP_REG_BIT_TO_ID(11, 25)
#define CPG_MSTP_V3U_PCI32P            CPG_MSTP_REG_BIT_TO_ID(11, 24)
#define CPG_MSTP_V3U_PCI22P            CPG_MSTP_REG_BIT_TO_ID(11, 23)
#define CPG_MSTP_V3U_PCI12P            CPG_MSTP_REG_BIT_TO_ID(11, 22)
#define CPG_MSTP_V3U_PCI02P            CPG_MSTP_REG_BIT_TO_ID(11, 21)
#define CPG_MSTP_V3U_FRAY_P            CPG_MSTP_REG_BIT_TO_ID(11, 18)
#define CPG_MSTP_V3U_AULK4P            CPG_MSTP_REG_BIT_TO_ID(11,  7)
#define CPG_MSTP_V3U_AULK2P            CPG_MSTP_REG_BIT_TO_ID(11,  6)
#define CPG_MSTP_V3U_FCPVX3            CPG_MSTP_REG_BIT_TO_ID(11,  3)
#define CPG_MSTP_V3U_FCPVX2            CPG_MSTP_REG_BIT_TO_ID(11,  2)
#define CPG_MSTP_V3U_FCPVX1            CPG_MSTP_REG_BIT_TO_ID(11,  1)
#define CPG_MSTP_V3U_FCPVX0            CPG_MSTP_REG_BIT_TO_ID(11,  0)

#define CPG_MSTP_V3U_FSO               CPG_MSTP_REG_BIT_TO_ID(12, 30)  // S4: FSO, V3U: RFSO
#define CPG_MSTP_V3U_SCMT              CPG_MSTP_REG_BIT_TO_ID(12, 22)
#define CPG_MSTP_V3U_PFC3_2            CPG_MSTP_REG_BIT_TO_ID(12, 21)
#define CPG_MSTP_V3U_PFC3_1            CPG_MSTP_REG_BIT_TO_ID(12, 20)
#define CPG_MSTP_V3U_PFC3_0            CPG_MSTP_REG_BIT_TO_ID(12, 19)
#define CPG_MSTP_V3U_PFC2_2            CPG_MSTP_REG_BIT_TO_ID(12, 18)
#define CPG_MSTP_V3U_PFC2_1            CPG_MSTP_REG_BIT_TO_ID(12, 17)
#define CPG_MSTP_V3U_PFC2_0            CPG_MSTP_REG_BIT_TO_ID(12, 16)
#define CPG_MSTP_V3U_PFC1_2            CPG_MSTP_REG_BIT_TO_ID(12, 15)
#define CPG_MSTP_V3U_PFC1_1            CPG_MSTP_REG_BIT_TO_ID(12, 14)
#define CPG_MSTP_V3U_PFC1_0            CPG_MSTP_REG_BIT_TO_ID(12, 13)
#define CPG_MSTP_V3U_PFC0_2            CPG_MSTP_REG_BIT_TO_ID(12, 12)
#define CPG_MSTP_V3U_PFC0_1            CPG_MSTP_REG_BIT_TO_ID(12, 11)
#define CPG_MSTP_V3U_PFC0_0            CPG_MSTP_REG_BIT_TO_ID(12, 10)
#define CPG_MSTP_V3U_WWDT90            CPG_MSTP_REG_BIT_TO_ID(12,  9)
#define CPG_MSTP_V3U_WWDT80            CPG_MSTP_REG_BIT_TO_ID(12,  8)
#define CPG_MSTP_V3U_WWDT70            CPG_MSTP_REG_BIT_TO_ID(12,  7)
#define CPG_MSTP_V3U_WWDT60            CPG_MSTP_REG_BIT_TO_ID(12,  6)
#define CPG_MSTP_V3U_WWDT50            CPG_MSTP_REG_BIT_TO_ID(12,  5)
#define CPG_MSTP_V3U_WWDT40            CPG_MSTP_REG_BIT_TO_ID(12,  4)
#define CPG_MSTP_V3U_WWDT30            CPG_MSTP_REG_BIT_TO_ID(12,  3)
#define CPG_MSTP_V3U_WWDT20            CPG_MSTP_REG_BIT_TO_ID(12,  2)
#define CPG_MSTP_V3U_WWDT10            CPG_MSTP_REG_BIT_TO_ID(12,  1)
#define CPG_MSTP_V3U_WWDT00            CPG_MSTP_REG_BIT_TO_ID(12,  0)

#define CPG_MSTP_V3U_PFC1_             CPG_MSTP_REG_BIT_TO_ID(13, 31)
#define CPG_MSTP_V3U_PFC0_             CPG_MSTP_REG_BIT_TO_ID(13, 30)
#define CPG_MSTP_V3U_WWDT91            CPG_MSTP_REG_BIT_TO_ID(13, 27)
#define CPG_MSTP_V3U_WWDT81            CPG_MSTP_REG_BIT_TO_ID(13, 26)
#define CPG_MSTP_V3U_WWDT71            CPG_MSTP_REG_BIT_TO_ID(13, 25)
#define CPG_MSTP_V3U_WWDT61            CPG_MSTP_REG_BIT_TO_ID(13, 24)
#define CPG_MSTP_V3U_WWDT51            CPG_MSTP_REG_BIT_TO_ID(13, 23)
#define CPG_MSTP_V3U_WWDT41            CPG_MSTP_REG_BIT_TO_ID(13, 22)
#define CPG_MSTP_V3U_WWDT31            CPG_MSTP_REG_BIT_TO_ID(13, 21)
#define CPG_MSTP_V3U_WWDT21            CPG_MSTP_REG_BIT_TO_ID(13, 20)
#define CPG_MSTP_V3U_WWDT11            CPG_MSTP_REG_BIT_TO_ID(13, 19)
#define CPG_MSTP_V3U_WWDT01            CPG_MSTP_REG_BIT_TO_ID(13, 18)
#define CPG_MSTP_V3U_DBE               CPG_MSTP_REG_BIT_TO_ID(13, 11)
#define CPG_MSTP_V3U_MTI               CPG_MSTP_REG_BIT_TO_ID(13,  4)

#define CPG_MSTP_V3U_CKMMM             CPG_MSTP_REG_BIT_TO_ID(14, 12)
#define CPG_MSTP_V3U_CKMPE             CPG_MSTP_REG_BIT_TO_ID(14,  6)
#define CPG_MSTP_V3U_CKMRT             CPG_MSTP_REG_BIT_TO_ID(14,  5)
#define CPG_MSTP_V3U_SYS               CPG_MSTP_REG_BIT_TO_ID(14,  3)
#define CPG_MSTP_V3U_SWC               CPG_MSTP_REG_BIT_TO_ID(14,  2)
#define CPG_MSTP_V3U_PFC3_             CPG_MSTP_REG_BIT_TO_ID(14,  1)
#define CPG_MSTP_V3U_PFC2_             CPG_MSTP_REG_BIT_TO_ID(14,  0)

#define CPG_MSTP_V3U_CKM               CPG_MSTP_REG_BIT_TO_ID(15, 17)
#define CPG_MSTP_V3U_CKMHSC            CPG_MSTP_REG_BIT_TO_ID(15, 16)
#define CPG_MSTP_V3U_CKMCR52           CPG_MSTP_REG_BIT_TO_ID(15, 15)
#define CPG_MSTP_V3U_UFS0              CPG_MSTP_REG_BIT_TO_ID(15, 14)
#define CPG_MSTP_V3U_SYSRAM1           CPG_MSTP_REG_BIT_TO_ID(15,  7)
#define CPG_MSTP_V3U_ETHPHY            CPG_MSTP_REG_BIT_TO_ID(15,  6)
#define CPG_MSTP_V3U_RSW2              CPG_MSTP_REG_BIT_TO_ID(15,  5)

#define CPG_MSTP_V3U_INTAP_26_27       CPG_MSTP_REG_BIT_TO_ID(26, 27)
#define CPG_MSTP_V3U_INTAP_26_24       CPG_MSTP_REG_BIT_TO_ID(26, 24)
#define CPG_MSTP_V3U_INTAP_26_21       CPG_MSTP_REG_BIT_TO_ID(26, 21)
#define CPG_MSTP_V3U_INTAP_26_18       CPG_MSTP_REG_BIT_TO_ID(26, 18)

typedef struct cpg_mstpctl_ {
    int            mod_id;
    cpg_mstp_cmd_t cmd;
    int            is_enabled;
} cpg_mstpctl_t;

typedef struct cpg_clkctl_ {
    int           clk_id;
    int           clk_idx;
    uint32_t      clk_val;
    cpg_clk_cmd_t cmd;
    uint8_t       rev[20];
} cpg_clkctl_t;

/* 
 * Commands interface between resource manager and client 
 */
#define CPG_CMD_CODE                          1
#define RCAR_CPG_DEVCTL_MODCTL               __DIOTF(_DCMD_MISC,  CPG_CMD_CODE + 2, cpg_mstpctl_t)
#define RCAR_CPG_DEVCTL_CLKCTL               __DIOTF(_DCMD_MISC,  CPG_CMD_CODE + 2, cpg_clkctl_t)

/*
 * Hardware Access Function Table
 */
typedef struct {

    /* size of this structure */
    size_t size;

    /*
     * open a instant of cpg resourse manager (start #cpg_resmgr-rcar)
     *
     * Input Parameters:
     *  vbose : Debug verbose level
     *
     * Return Value :
     *    EOK - Success
     *    Another - Failure if cpg_resmgr-rcar is not run
     */
    int (*init) (int vbose);

    /*
     * Close the instant of cpg resourse manager (start #cpg_resmgr-rcar)
     *
     * Input Parameters:
     *
     * Output Parameters:
     *
     * Return Value :
     *    EOK - Success
     *    Another - Failure
     */
    int (*deinit) (void);

    /*
     * Function mstp_is_enabled used to check the current clock state of a module
     *
     * Input Parameters:
     *    int mod_id: CPG MSTP id of the requested module
     *
     * Return Value :
     *    1 - Module is enabled
     *    0 - Module is not enabled
     *  < 0 - Failure
     */
    int (*mstp_is_enabled) (int mod_id);

    /*
     * The mstp_enable function is used to enable
     * clock of a module.
     *
     * Input Parameters:
     *    int mod_id: CPG MSTP id of the requested module
     *
     * Return Value :
     *    1 - Enabled
     *  0 - Disable
     *    < 0 - Failure
     *
     */
    int (*mstp_enable) (int mod_id);

    /*
     * The mstp_disable function is used to disable
     * clock of a module.
     *
     * Input Parameters:
     *    int mod_id: CPG MSTP id of the requested module
     *
     * Return Value :
     *    EOK - Success
     *    < 0 - Failure
     *
     */
    int (*mstp_disable) (int mod_id);

    /*
     * The mstp_reset function is used to reset
     * a module.
     *
     * Input Parameters:
     *    int mod_id: CPG MSTP id of the requested module
     *
     * Return Value :
     *    EOK - Success
     *    < 0 - Failure
     *
     */
    int (*mstp_reset) (int mod_id);

    /*
     * The mstp_assert function is used to assert reset to
     * a module.
     *
     * Input Parameters:
     *    int mod_id: CPG MSTP id of the requested module
     *
     * Return Value :
     *    EOK - Success
     *    < 0 - Failure
     *
     */
    int (*mstp_assert) (int mod_id);

    /*
     * The mstp_deassert function is used to deassert reset to
     * a module.
     *
     * Input Parameters:
     *    int mod_id: CPG MSTP id of the requested module
     *
     * Return Value :
     *    EOK - Success
     *    < 0 - Failure
     *
     */
    int (*mstp_deassert) (int mod_id);

    /*
     * Function mstp_reset_state used to check the current reset state of a module
     *
     * Input Parameters:
     *    int mod_id: CPG MSTP id of the requested module
     *
     * Return Value :
     *    1 - Module is under reset state
     *    0 - Module is under reset release
     *  < 0 - Failure
     */
    int (*mstp_reset_state) (int mod_id);

    /*
     * Function clk_is_enabled used to check the current input clock state of a module
     *
     * Input Parameters:
     *  id : Module ID
     *  idx: channel index
     *
     * Return Value :
     * Return Value :
     *    1 - Module is enabled
     *    0 - Module is not enabled
     *  other - Failure
     */
    int (*clk_is_enabled) (cpg_clk_id_t id, const int idx);

    /*
     * Function clk_enable used to enable input clock of a module
     *
     * Input Parameters:
     *  id : Module ID
     *  idx: channel index
     *
     * Return Value :
     *    EOK - Success
     *    -ENOMEM - Failure
     */
    int (*clk_enable) (cpg_clk_id_t id, const int idx);


    /*
     * Function clk_disable used to disable input clock of a module
     *
     * Input Parameters:
     *  id : Module ID
     *  idx: channel index
     *
     * Return Value :
     *    EOK - Success
     *   -ENOMEM - Failure
     */
    int (*clk_disable) (cpg_clk_id_t id, const int idx);

    /*
     * Function clk_rate_get used to get the current clock rate of a module
     *
     * Input Parameters:
     *  id : Module ID
     *    idx: channel index
     *
     * Output Parameters:
     *  rate : Current clock rate
     *
     * Return Value :
     *    EOK - Success
     *    -ENOMEM - Failure
     */
    int (*clk_rate_get) (cpg_clk_id_t id, const int idx, uint32_t *rate);
    /*
     * Function clk_rate_set used to change rate of a module
     *
     * Input Parameters:
     *  id : Module ID
     *  idx: channel index
     *  *rate : Clock rate need to change to
     *
     * Output Parameters:
     *  *rate : Real Clock rate
     *
     * Return Value :
     *    EOK - Success
     *    Other - Failure
     */
    int (*clk_rate_set) (cpg_clk_id_t id, const int idx, uint32_t *rate);

    /*
     * Function clk_divider_get used to get the current clock divider of a module
     *
     * Input Parameters:
     *  id : Module ID
     *    idx: channel index
     *
     * Output Parameters:
     *  div : Current clock divider
     *
     * Return Value :
     *    EOK - Success
     *    -ENOMEM - Failure
     */
    int (*clk_divider_get) (cpg_clk_id_t id, const int idx, uint32_t *div);

    /*
     * Function clk_divider_set used to change clock divider of a module
     *
     * Input Parameters:
     *  id : Module ID
     *  idx: channel index
     *  *div : Clock divider need to change to
     *
     * Output Parameters:
     *  *div : Real Clock divider
     *
     * Return Value :
     *    EOK - Success
     *    Other - Failure
     */
    int (*clk_divider_set) (cpg_clk_id_t id, const int idx, uint32_t *div);
} cpg_mgr_funcs_t;

/*
 * Fills in the given table with the hardware-specific functions.
 * If a function table entry is unimplemented, it should
 * be left unchanged (and not set to NULL, etc.).
 * "funcs->size" should be set by whomever has allocated the
 * table. Don't change this.
 * Input Parameters:
 *    cpg_mgr_funcs_t *funcs      Function table
 *    int tabsize                 size of "funcs" in bytes
 *
 * Returns:
 * true       success
 * false    failure
 */

int cpg_mgr_getfuncs(cpg_mgr_funcs_t *funcs, const int tabsize);

#endif /* _RCAR_CPG_H_INCLUDED */

