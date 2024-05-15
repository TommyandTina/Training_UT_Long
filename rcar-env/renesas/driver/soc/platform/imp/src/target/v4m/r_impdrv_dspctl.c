/***********************************************************************************************************************
* Copyright [2020-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_impdrv_dspctl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_dspctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_dspctl.c
 */

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "rcar-xos/imp/r_impdrv.h"
#include "r_impdrv_private.h"
#include "rcar-xos/osal/r_osal.h"
#include "r_impdrv_cmnctl.h"
#include "r_impdrv_udefctl.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Defines Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_CORENUM_VAL
 * CORENUM_VAL register value for DSP.
***********************************************************************************************************************/
#define IMPDRV_DSP_CORENUM_VAL          (2U)                /**< Maximum number of cores for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_REG_MAX_ADRS_CNFCHK
 *  End of offset address for ConfRegCheck of VDSP.
***********************************************************************************************************************/
#define IMPDRV_DSP_REG_MAX_ADRS_CNFCHK  (0x0019FFFFU)       /** End of offset address for ConfRegCheck of IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_MAIN
 * VDSP device main.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_MAIN        (0u)                /**< vsdp*_core   **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB0
 * VDSP device sub0.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB0        (1u)                /**< vsdp*_csb    **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB1
 * VDSP device sub1.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB1        (2u)                /**< vsdp*_reg    **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB2
 * VDSP device sub2.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB2        (3u)                /**< vsdp*_global **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB3
 * VDSP device sub3.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB3        (4u)                /**< vsdp*_sys    **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB4
 * VDSP device sub4.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB4        (5u)                /**< vsdp*_ocem   **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB5
 * VDSP device sub5.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB5        (6u)                /**< vsdp*_apbdbg **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB6
 * VDSP device sub6.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB6        (7u)                /**< vsdp*_syspo  **/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_VDSP_DEV_SUB7
 * VDSP device sub7.
***********************************************************************************************************************/
#define IMPDRV_DSP_VDSP_DEV_SUB7        (8u)                /**< vsdp*_bus  **/
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK
 * Offset address of ICU_ACU_LOCK register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK (0xC0004U)          /**< Offset address of ICU_ACU_LOCK register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_RST_VEC_ADR
 * Offset address of RESET_VEC register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_RST_VEC_ADR  (0xC0010U)          /**< Offset address of RESET_VEC register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_GPIO_IN
 * Offset address of GPIO_IN register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_GPIO_IN      (0xC0014U)          /**< Offset address of GPIO_IN register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_CONTROL
 * Offset address of DSP_CONTROL register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_CONTROL      (0xC0038U)          /**< Offset address of DSP_CONTROL register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_OUT_INT_EN
 * Offset address of OUT_INT_EN_I register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_OUT_INT_EN   (0xC0040U)          /**< Offset address of OUT_INT_EN_I register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_OUT_INT_CLR
 * Offset address of OUT_INT_CLR_I register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_OUT_INT_CLR  (0xC0044U)          /**< Offset address of OUT_INT_CLR_I register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_OUT_INT_MSK
 * Offset address of OUT_INT_MSK_I register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_OUT_INT_MSK  (0xC0048U)          /**< Offset address of OUT_INT_MSK_I register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_OUT_INT_STA
 * Offset address of OUT_INT_STATE_I register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_OUT_INT_STA  (0xC004CU)          /**< Offset address of OUT_INT_STATE_I register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_PSU_STATE
 * Offset address of PSU_STATE register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_PSU_STATE    (0xC007CU)          /**< Offset address of PSU_STATE register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_GPIO_OUT
 * Offset address of GPIO_OUT register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_GPIO_OUT     (0xC0080U)          /**< Offset address of GPIO_OUT register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_VERSION1
 * Offset address of VERSION1 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_VERSION1     (0xC0094U)          /**< Offset address of VERSION1 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_MSS_PCR
 * Offset address of MSS_PCR register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_MSS_PCR           (0x80400U)          /**< Offset address of MSS_PCR register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_PDEA
 * Offset address of P_PDEA register for VDSP (align 32).
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_PDEA            (0x80410U)          /**< Offset address of P_PDEA register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_PDIA
 * Offset address of P_PDIA register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_PDIA            (0x80414U)          /**< Offset address of P_PDIA register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_PDTC
 * Offset address of P_PDTC register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_PDTC            (0x80418U)          /**< Offset address of P_PDTC register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD0_ATT0
 * Offset address of P_ADD0_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD0_ATT0       (0x80424U)          /**< Offset address of P_ADD0_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD1_START
 * Offset address of P_ADD1_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD1_START      (0x80428U)          /**< Offset address of P_ADD1_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD1_ATT0
 * Offset address of P_ADD1_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD1_ATT0       (0x8042CU)          /**< Offset address of P_ADD1_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD2_START
 * Offset address of P_ADD2_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD2_START      (0x80430U)          /**< Offset address of P_ADD2_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD2_ATT0
 * Offset address of P_ADD2_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD2_ATT0       (0x80434U)          /**< Offset address of P_ADD2_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD3_START
 * Offset address of P_ADD3_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD3_START      (0x80438U)          /**< Offset address of P_ADD2_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD3_ATT0
 * Offset address of P_ADD3_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD3_ATT0       (0x8043CU)          /**< Offset address of P_ADD3_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_ADD4_START
 * Offset address of P_ADD4_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_ADD4_START      (0x80440U)          /**< Offset address of P_ADD4_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_CCOSAR
 * Offset address of P_CCOSAR register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_CCOSAR          (0x804E0U)          /**< Offset address of P_CCOSAR register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_CCOCR
 * Offset address of P_CCOCR register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_CCOCR           (0x804E4U)          /**< Offset address of P_CCOCR register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_MSS_DMBA
 * Offset address of MSS_DMBA register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_MSS_DMBA          (0x80600U)          /**< Offset address of MSS_DMBA register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_DMBE
 * Offset address of MSS_DMBE register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_DMBE              (0x80604U)          /**< Offset address of MSS_DMBE register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_DDTC
 * Offset address of MMS_DDTC register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_DDTC            (0x80658U)          /**< Offset address of MMS_DDTC register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_DDEA
 * Offset address of MMS_DDEA register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_DDEA            (0x80680U)          /**< Offset address of MMS_DDEA register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_DDIA
 * Offset address of MMS_DDIA register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_DDIA            (0x80684U)          /**< Offset address of MMS_DDIA register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_DDCL
 * Offset address of MMS_DDCL register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_DDCL            (0x8068CU)          /**< Offset address of MMS_DDCL register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_DDESC_ID
 * Offset address of MSS_DDESC_ID register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_DDESC_ID        (0x806A0U)          /**< Offset address of MSS_DDESC_ID register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD0_ATT0
 * Offset address of D_ADD0_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD0_ATT0       (0x80924U)          /**< Offset address of D_ADD0_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD0_ATT1
 * Offset address of D_ADD0_ATT1 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD0_ATT1       (0x80928U)          /**< Offset address of D_ADD1_ATT1 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD1_START
 * Offset address of D_ADD1_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD1_START      (0x80930U)          /**< Offset address of D_ADD1_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD1_ATT0
 * Offset address of D_ADD1_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD1_ATT0       (0x80934U)          /**< Offset address of D_ADD1_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD1_ATT1
 * Offset address of D_ADD1_ATT1 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD1_ATT1       (0x80938U)          /**< Offset address of D_ADD1_ATT1 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD2_START
 * Offset address of D_ADD2_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD2_START      (0x80940U)          /**< Offset address of D_ADD2_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD3_START
 * Offset address of D_ADD3_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD3_START      (0x80950U)          /**< Offset address of D_ADD3_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD3_ATT0
 * Offset address of D_ADD3_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD3_ATT0       (0x80954U)          /**< Offset address of D_ADD3_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD3_ATT1
 * Offset address of D_ADD3_ATT1 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD3_ATT1       (0x80958U)          /**< Offset address of D_ADD3_ATT1 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD4_START
 * Offset address of D_ADD4_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD4_START      (0x80960U)          /**< Offset address of D_ADD4_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD5_START
 * Offset address of D_ADD5_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD5_START      (0x80970U)          /**< Offset address of D_ADD5_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD5_ATT0
 * Offset address of D_ADD5_ATT0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD5_ATT0       (0x80974U)          /**< Offset address of D_ADD5_ATT0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD5_ATT1
 * Offset address of D_ADD5_ATT1 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD5_ATT1       (0x80978U)          /**< Offset address of D_ADD5_ATT1 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_ADD6_START
 * Offset address of D_ADD6_START register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_ADD6_START      (0x80980U)          /**< Offset address of D_ADD6_START register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_SFT_SCPD_0
 * Offset address of IMPDRV_REGOFS_D_SFT_SCPD_0 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_SFT_SCPD_0      (0x807E0U)          /**< Offset address of IMPDRV_REGOFS_D_SFT_SCPD_0 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_SFT_SCPD_1
 * Offset address of IMPDRV_REGOFS_D_SFT_SCPD_1 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_SFT_SCPD_1      (0x807E4U)          /**< Offset address of IMPDRV_REGOFS_D_SFT_SCPD_1 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_SFT_SCPD_2
 * Offset address of IMPDRV_REGOFS_D_SFT_SCPD_2 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_SFT_SCPD_2      (0x807E8U)          /**< Offset address of IMPDRV_REGOFS_D_SFT_SCPD_2 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_SFT_SCPD_3
 * Offset address of IMPDRV_REGOFS_D_SFT_SCPD_3 register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_SFT_SCPD_3      (0x807ECU)          /**< Offset address of IMPDRV_REGOFS_D_SFT_SCPD_3 register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_PSVM
 * Offset address of PSVM register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_PSVM              (0x80E50U)          /**< Offset address of PSVM register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_PMASACS
 * Offset address of PMASACS register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_PMASACS           (0x80540U)          /**< Offset address of PMASACS register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_DBGACS
 * Offset address of DBGACS register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_DBGACS            (0x80d04U)          /**< Offset address of DBGACS register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_UCSERR
 * Offset address of P_UCSERR register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_UCSERR          (0x807b0U)          /**< Offset address of P_UCSERR register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_P_CSERR
 * Offset address of P_CSERR register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_P_CSERR           (0x807b8U)          /**< Offset address of P_CSERR register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_UCSERR
 * Offset address of D_UCSERR register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_UCSERR          (0x80580U)          /**< Offset address of D_UCSERR register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_D_CSERR
 * Offset address of D_CSERR register for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_D_CSERR           (0x80588U)          /**< Offset address of D_CSERR register for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMPSLV_DSP0CSTS
 * Offset address of VDSP0CSTS register.
 * Offset address of DSP ch0 Communication STaTus.
***********************************************************************************************************************/
#define IMPDRV_REG_IMPSLV_DSP0CSTS      (0x00004014U)       /**< Offset address of VDSP0CSTS register */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMPSLV_DSP1CSTS
 * Offset address of VDSP1CSTS register.
 * Offset address of DSP ch1 Communication STaTus.
***********************************************************************************************************************/
#define IMPDRV_REG_IMPSLV_DSP1CSTS      (0x00004024U)       /**< Offset address of VDSP1CSTS register */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMPSLV_DSP2CSTS
 * Offset address of VDSP2CSTS register.
 * Offset address of DSP ch2 Communication STaTus.
***********************************************************************************************************************/
#define IMPDRV_REG_IMPSLV_DSP2CSTS      (0x00004034U)       /**< Offset address of VDSP2CSTS register */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMPSLV_DSP3CSTS
 * Offset address of VDSP3CSTS register.
 * Offset address of DSP ch3 Communication STaTus.
***********************************************************************************************************************/
#define IMPDRV_REG_IMPSLV_DSP3CSTS      (0x00004044U)       /**< Offset address of VDSP3CSTS register */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_CLR
 * register clear value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_CLR               (0x00000000U)       /**< register clear value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_OUT_INT_TIMER00
 * I_TIMER00 value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_OUT_INT_TIMER00   (0x0001U)           /**< I_TIMER00 value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_OUT_INT_TIMER01
 * I_TIMER00 value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_OUT_INT_TIMER01   (0x0002U)           /**< I_TIMER01 value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_ACU_LOCK
 * ICU_ACU_LOCK register lock value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_ACU_LOCK     (0x0U)              /**< ICU_ACU_LOCK register lock value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_ACU_UNLOCK
 * ICU_ACU_LOCK register unlock value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_ACU_UNLOCK   (0x8U)              /**< ICU_ACU_LOCK register unlock value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_ICU_LOCK
 * ICU_ACU_LOCK register lock value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_ICU_LOCK     (0x1U)              /**< ICU_ACU_LOCK register lock value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_ACU_UNLOCK
 * ICU_ACU_LOCK register unlock value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_ICU_UNLOCK   (0x0U)              /**< ICU_ACU_LOCK register unlock value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_GPIO_START
 * GPIO_IN register start value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_GPIO_START   (0x00000008U)       /**< GPIO_IN register start value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_GPIO_FSTDBY
 * GPIO_IN register F standby value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_GPIO_FSTDBY  (0x00000800U)       /**< GPIO_IN register F standby value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_CONTROL_PLOAD_STOP
 * DSP_CONTROL register PLOAD_STOP value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_VDSP_CONTROL_PLOAD_STOP  (0x0001U)           /**< DSP_CONTROL register PLOAD_STOP value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_PSU_STATE_SBY
 * PSU_STATE register STANDBY_MD value for VDSP.
 * 1:AXI_LOWPWR_INDI,2:AXI_LOWPWR_ACK,4:STANDBY_MD,8:LIGHT_SLP_MD.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_PSU_STATE_SBY     (0x00000004U)       /**< PSU_STATE register STANDBY_MD value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_PSU_STATE_LSP
 * PSU_STATE register LIGHT_SLP_MD value for VDSP.
 * 1:AXI_LOWPWR_INDI,2:AXI_LOWPWR_ACK,4:STANDBY_MD,8:LIGHT_SLP_MD.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_PSU_STATE_LSP     (0x00000008U)       /**< PSU_STATE register LIGHT_SLP_MD value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_GPIO_STOP
 * GPIO_OUT register stop value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_GPIO_STOP    (0x00000080U)       /**< GPIO_OUT register stop value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_GPIO_STDBY
 * GPIO_OUT register standby value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_GPIO_STDBY   (0x00000001U)       /**< GPIO_OUT register standby value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_GPIO_RESUME
 * GPIO_OUT register resume value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_GPIO_RESUME  (0x00002000U)       /**< GPIO_OUT register resume value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_TRAP
 * GPIO_OUT register TRAP value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_TRAP         (0x00010000U)       /**< GPIO_OUT register TRAP value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_INT
 * GPIO_OUT register INT value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_INT          (0x00020000U)       /**< GPIO_OUT register INT value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_GPIO_RESUME
 * GPIO_OUT register resume value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_RESUME       (0x00020000U)       /**< GPIO_OUT register resume value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_CRC_ERROR
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_CRC_ERROR         (0x80000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_NORMAL_INT
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_NORMAL_INT        (0x40000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_CRITICAL_INT
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_CRITICAL_INT      (0x20000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_SAFETY_ERROR
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_SAFETY_ERROR      (0x10000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_DIV0
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_DIV0              (0x08000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_WDOG_EXPIRE
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_WDOG_EXPIRE       (0x04000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_STACK_VIOLATION
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_STACK_VIOLATION   (0x02000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_TIM_FAULT
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_TIM_FAULT         (0x01000000U)       /**< GPIO_OUT register callback value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_ERROR
 * GPIO_OUT register error value for VDSP.
 * for VDSP error notice.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_ERROR        (0xFF000000U)       /**< GPIO_OUT register error value for VDSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_VERSION1
 * Hardware version value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_VERSION1     (0x20211220U)       /**< Hardware version valuer for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_MSS_PCR
 * MSS_PCR register value for VDSP.
 * CAC_PFE | CLBE | PCAC_EN | HWPF_SZ | CAC_WPE.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_MSS_PCR      (((uint32_t)1U << 2U) | ((uint32_t)1U << 6U) | ((uint32_t)1U << 7U) | \
                                                                        ((uint32_t)3U << 8U) | ((uint32_t)1U << 12U))
                                                            /**< MSS_PCR register value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_REGVAL_PDMA_BUSY
 * P_PDTC register PDTC PDMA BUSY value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_VDSP_REGVAL_PDMA_BUSY    ((uint32_t)1U << 29U)   /**< Hardware version valuer for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_P_CCOSA_MASK
 * P_CCOSAR register P_CCOSA mask for VDSP.
***********************************************************************************************************************/
#define IMPDRV_VDSP_P_CCOSA_MASK        (0xFFFFFFC0U)       /**<P_CCOSAR register P_CCOSA mask for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_P_CCOCR_PF
 * P_CCOCR register Pre-fetched value for VDSP.
 * P_ACTIVE | P_L1ICO | P_OT | P_OS | P_NOBPL (64*1024Byte) (Entire cache, Unlock)
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_P_CCOCR_PF   (((uint32_t)1U << 0U) | ((uint32_t)1U << 1U) | ((uint32_t)3U << 2U) | \
                                                                        ((uint32_t)1U << 7U) | ((uint32_t)1024U << 16U))
                                                            /**< P_CCOCR register Pre-fetched value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_P_CCOCR_IV
 * P_CCOCR register Invalidate value for VDSP.
 * P_ACTIVE | P_L1ICO | P_OT | P_OS | P_NOBPL (64*1024Byte) (Entire cache, Invalidate)
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_P_CCOCR_IV   (((uint32_t)1U << 0U) | ((uint32_t)1U << 1U) | ((uint32_t)4U << 2U) | \
                                                                        ((uint32_t)1U << 7U) | ((uint32_t)1024U << 16U))
                                                            /**< P_CCOCR register Invalidate value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_P_L1ICO_MASK
 * P_CCOCR register P_L1ICO mask for VDSP.
***********************************************************************************************************************/
#define IMPDRV_VDSP_P_L1ICO_MASK        ((uint32_t)1U << 1U)    /**< P_CCOCR register P_L1ICO mask for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_DMBE_INIT
 * MSS_DMBE register value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_DMBE_INIT         (0x00000000U)       /**< DMBA register value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_DMBE_IDLE
 * MSS_DMBE register value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_DMBE_IDLE         (0x00011FFFU)       /**< MSS_DMBE register value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_BSZ_8_INCR
 * MSS_DDCL register BSZ 1000: 128 transfers (INCR) value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_BSZ_8_INCR   (0xA0)              /**< MSS_DDCL register BSZ 8 value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_DDMA_BUSY_MASK
 * MSS_DDCL register DDMA_BUSY mask for VDSP.
***********************************************************************************************************************/
#define IMPDRV_VDSP_DDMA_BUSY_MASK      ((uint32_t)1U<<8U)  /**< MSS_DDCL register DDMA_BUSY mask for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_SPMX_ADDR_START
 * D_ADD3_START register REGION_START value for VDSP.
 * SPMI/SPIC address.
***********************************************************************************************************************/
#define IMPDRV_SPMX_ADDR_START          (0xED200000U)       /**< D_ADD3_START register REGION_START value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_SPMX_ADDR_END
 * D_ADD4_START register REGION_START value for VDSP.
 * SPMI/SPIC address.
***********************************************************************************************************************/
#define IMPDRV_SPMX_ADDR_END            (0xED600000U)       /**< D_ADD4_START register REGION_START value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMPREG_ADDR_START
 * D_ADD5_START register REGION_START value for VDSP.
 * IMP Register address.
***********************************************************************************************************************/
#define IMPDRV_IMPREG_ADDR_START        (0xF1000000U)       /**< D_ADD5_START register REGION_START value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMPREG_ADDR_END
 * D_ADD6_START register REGION_START value for VDSP.
 * IMP Register address.
***********************************************************************************************************************/
#define IMPDRV_IMPREG_ADDR_END          (0xFFC00000U)       /**< D_ADD6_START register REGION_START value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_D_ADDR_SHIFT
 * D_ADDx_START register data shift for VDSP.
***********************************************************************************************************************/
#define IMPDRV_VDSP_D_ADDR_SHIFT        (12U)               /**< D_ADDx_START register data shift for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_REG_EDP
 * D_ADDx_START register REGION_MID EDP value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_REG_EDP      ((uint32_t)0x1U << 20U)
                                                            /**< D_ADDx_START register REGION_MID EDP value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_REG_AXI0
 * D_ADDx_START register REGION_MID AXIm0 value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_REG_AXI0     ((uint32_t)0x2U << 20U)
                                                            /**< D_ADDx_START register REGION_MID AXI0 value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_REG_BLANK
 * D_ADDx_START register REGION_MID Blank region value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_REG_BLANK    ((uint32_t)0x80U << 20U)
                                                            /**< D_ADDx_START reg REGION Blank region value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_REG_ACT
 * D_ADDx_START register INACTIVE active value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_REG_ACT      ((uint32_t)0U << 28U)   /**< D_ADDx_START register active value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_REG_INACT
 * D_ADDx_START register INACTIVE not active value for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_REG_INACT    ((uint32_t)1U << 28U)   /**< D_ADDx_START register not active value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_EXT_DPRAW
 * D_ADDx_START register DPRAW value for VDSP.
 * Data protection Read-after-Write indication. EDP only.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_EXT_DPRAW    ((uint32_t)0x1U << 29U)     /**< D_ADDx_START register DPRAW value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_P_ADDX_ATT0
 * P_ADDx_ATT0 register L1IC value for VDSP.
 * L1IC.
***********************************************************************************************************************/
#define IMPDRV_VDSP_P_ADDX_ATT0         (0x1U)              /**< P_ADDx_ATT0 register L1IC value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_P_L2A_ATT0
 * P_ADDx_ATT0 register P_L2A value for VDSP.
 * L2 Cache Policy for read accesses.
***********************************************************************************************************************/
#define IMPDRV_VDSP_P_L2A_ATT0          ((uint32_t)0x2U << 8U)      /**< P_ADDx_ATT0 register P_L2A value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_P_AP_ATT0
 * P_ADDx_ATT0 register P_AP value for VDSP.
 * L2 Cache Policy for read accesses.
***********************************************************************************************************************/
#define IMPDRV_VDSP_P_AP_ATT0           ((uint32_t)0x3U << 4U)      /**< P_ADDx_ATT0 register P_AP value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_EXT_MOM
 * D_ADDx_ATT0 register MOM value for VDSP.
 * Memory Ordering Model. EDP only.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_EXT_MOM      ((uint32_t)0x1U << 16U)     /**< D_ADDx_ATT0 register MOM value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_EXT_L2A_WR
 * D_ADDx_ATT1 register L2A_WR value for VDSP.
 * L2 Cache Policy for write accesses.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_EXT_L2A_WR   ((uint32_t)0x2U << 1U)      /**< D_ADDx_ATT1 register L2A_WR value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_EXT_L2A_RD
 * D_ADDx_ATT1 register L2A_RD value for VDSP.
 * L2 Cache Policy for read accesses.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_EXT_L2A_RD   ((uint32_t)0x2U << 5U)      /**< D_ADDx_ATT1 register L2A_RD value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_EXT_DABSZ8
 * D_ADDx_ATT1 register DABSZ value for VDSP.
 * 8 transfer/INCR.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_EXT_DABSZ8   ((uint32_t)0xAU << 9U)      /**< D_ADDx_ATT1 register DABSZ value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_EXT_DADOL
 * D_ADDx_ATT1 register DADOL value for VDSP.
 * DDMA Download Outstanding Limit when using this region.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_EXT_DADOL    ((uint32_t)0xFU << 13U)     /**< D_ADDx_ATT1 register DADOL value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_EXT_DAUOL
 * D_ADDx_ATT1 register DAUOL value for VDSP.
 * DDMA Upload Outstanding Limit when using this region.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_EXT_DAUOL    ((uint32_t)0x4U << 18U)     /**< D_ADDx_ATT1 register DAUOL value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_PSVM_PMOD_SBY
 * PSVM register value for VDSP.
 * 00:FreeRun, 01:DPS, 10:LightSleep, 11:Standby.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_PSVM_PMOD_SBY     (0x00000003U)       /**<PSVM register value for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_IMPSLV_DSPNCSTS
 * DSPnCSTS register value.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_IMPSLV_DSPNCSTS   (0x00000000U)       /**< DSPnCSTS register value */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_PMASACS_ECCLR
 * PMASACS register setting value of ECC Clear for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_PMASACS_ECCLR     (0x0000DAC5U)       /**< PMASACS register setting value of ECC Clear for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_DBGACS_ECCLR
 * DBGACS register setting value of ECC Clear for VDSP.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_DBGACS_ECCLR      (0x0000DAC5U)       /**< DBGACS register setting value of ECC Clear for VDSP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_EXT_MEM_START
 * Valid DSP application execution start address.
***********************************************************************************************************************/
#define IMPDRV_VDSP_EXT_MEM_START       (0x40000000U)       /**< Valid DSP application execution start address */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_EXT_MEM_END
 * Valid DSP application execution end address.
***********************************************************************************************************************/
#define IMPDRV_VDSP_EXT_MEM_END         (0xC0000000U)       /**< Valid DSP application execution end address */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_GPIO_WAIT_LOOP
 * The maximum wait loop count for the GPIO register.
***********************************************************************************************************************/
#define IMPDRV_VDSP_GPIO_WAIT_LOOP      (0x00100000U)       /**< The maximum wait loop count for the GPIO register */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_PSU_ST_SLP_LOOP
 * Maximum wait loop count until PSU_STATE register is in sleep mode.
***********************************************************************************************************************/
#define IMPDRV_VDSP_PSU_ST_SLP_LOOP     (100U)              /**< Maximum wait loop count until sleep mode */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_PSU_ST_STD_LOOP
 * Maximum wait loop count until PSU_STATE register is in stand by mode.
***********************************************************************************************************************/
#define IMPDRV_VDSP_PSU_ST_STD_LOOP     (10U)               /**< Maximum wait loop count until stand by mode */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_P_PDTC_WAIT_LOOP
 * The maximum wait loop count for the P_PDTC register.
***********************************************************************************************************************/
#define IMPDRV_VDSP_P_PDTC_WAIT_LOOP    (100000U)           /**< The maximum wait loop count for the P_PDTC register */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_P_CCOCR_WAIT_LOOP
 * The maximum wait loop count for the P_CCOCR register.
***********************************************************************************************************************/
#define IMPDRV_VDSP_P_CCOCR_WAIT_LOOP   (100000U)           /**< The maximum wait loop count for the P_CCOCR register */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_DMBE_WAIT_LOOP
 * The maximum wait loop count for the MSS_DMBE register.
***********************************************************************************************************************/
#define IMPDRV_VDSP_DMBE_WAIT_LOOP      (100U)              /**< The maximum wait loop count for the MSS_DMBE register */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_D_DDCL_WAIT_LOOP
 * The maximum wait loop count for the D_DDCL register.
***********************************************************************************************************************/
#define IMPDRV_VDSP_D_DDCL_WAIT_LOOP    (100000U)           /**< The maximum wait loop count for the D_DDCL register */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_D_DDCL_DDIE_RESET
 * The maximum wait loop count for the D_DDCL register.
***********************************************************************************************************************/
#define IMPDRV_VDSP_D_DDCL_DDIE_RESET   (0U)              /**< The reset DDIE bit for the D_DDCL register */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_D_DDCL_DDIE_ENABLE
 * The maximum wait loop count for the D_DDCL register.
***********************************************************************************************************************/
#define IMPDRV_VDSP_D_DDCL_DDIE_ENABLE   (400U)           /**< The Enable DDIE bit for the D_DDCL register */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_CHANNEL_OPE_BITS
 * Channel operate bits.
***********************************************************************************************************************/
#define IMPDRV_VDSP_CHANNEL_OPE_BITS    (0x0FU)             /**< Channel operate bits */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_CB_CODE_TRAP
 * Callback code TRAP.
***********************************************************************************************************************/
#define IMPDRV_VDSP_CB_CODE_TRAP        (0)                 /**< Callback code TRAP */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_CB_CODE_INT
 * Callback code INT.
***********************************************************************************************************************/
#define IMPDRV_VDSP_CB_CODE_INT         (1)                 /**< Callback code INT */

/*******************************************************************************************************************//**
 * @def IMPDRV_CRC_TABLE_MAX
 * table num for crc rable.
***********************************************************************************************************************/
#define IMPDRV_CRC_TABLE_MAX            (256)              /**< CRC Table Member num */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_D_SFT_SCPD_0_ON
 * table num for crc rable.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_D_SFT_SCPD_0_ON            (0x01U)              /**< SCPD_0 Init Value(CRC ON) */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_D_SFT_SCPD_0_OFF
 * table num for crc rable.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_D_SFT_SCPD_0_OFF            (0x00U)              /**< SCPD_0 Init Value(CRC OFF) */

/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_IDM_FIXED_SIZE_64KB
 * IDM Fixed size
***********************************************************************************************************************/
#define IMPDRV_VDSP_IDM_FIXED_SIZE_64KB  ((uint32_t)0x4000U)     /**< IDM Fixed size */
/*******************************************************************************************************************//**
 * @def IMPDRV_VDSP_IPM_FIXED_SIZE_8KB
 * IPM Fixed size
***********************************************************************************************************************/
#define IMPDRV_VDSP_IPM_FIXED_SIZE_8KB   ((uint32_t)0x2000U)     /**< IPM Fixed size */

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CORECTL
 * @defgroup IMPDRV_CORECTL_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @enum e_impdrv_dspctl_load_type_t
 * VDSP load tyoe.
 **********************************************************************************************************************/
typedef enum
{
    IMPDRV_VDSP_LOAD_TYPE_INT    = (0),     /**< Load type INT       */
    IMPDRV_VDSP_LOAD_TYPE_EXT               /**< Load type EXT       */
}e_impdrv_dspctl_load_type_t;

/** @} */

/*======================================================================================================================
Private file static variables
======================================================================================================================*/

/** @} */
/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
/******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Functions Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief           Initialization of specified core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_device_io           device io handle for DSP
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PM System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dspctl_init_core(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Initialization of DSP core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dspctl_init_dsp(
    st_impdrv_corectl_t         *p_corectl
);
/*******************************************************************************************************************//**
 * @brief           Get DSP device io.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[out]      p_device_io         device io handle for DSP
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_get_device_io(
    st_impdrv_corectl_t         *const p_corectl,
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Check execute data of DSP core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       core_num            Core number
 * @param[in]       callback_func       Callback function of execute request
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_chk_execute_data(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func
);

/*******************************************************************************************************************//**
 * @brief           Preparation before execution of DSP core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       core_num            Core number
 * @param[out]      p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_execute_pre(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);
/*******************************************************************************************************************//**
 * @brief           Application update requested.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in-out]   p_corectl           Control data structure for specific core
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_update_app(
    st_impdrv_corectl_t         *const p_corectl,
    st_impdrv_device_handle_t   *const *p_device_io
);

/*******************************************************************************************************************//**
 * @brief           Reload new dsp_fw to PTCM.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_load_ptcm(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Reset D_ADDx, to new memory map.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_tcm_config_d(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Reset P_ADDx, to new memory map.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_tcm_config_p(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Reload new dsp_dtcm to DTCM.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_load_dtcm(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           The DSP is stopped by a register.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       is_executed         Is the DSP application already executed on DSP
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_stop_reg(
    const bool                  is_executed,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Check that DSP is Stand by Mode.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_standby(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Force that DSP is Stand by Mode.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_foece_standby(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);

/*******************************************************************************************************************//**
 * @brief           Check Interrupt data of DSP core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       core_num            Core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_chk_int_data(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num
);

/***********************************************************************************************************************
 * @brief           CRC calc and set.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       st           Start Address
 * @param[in]       sz           Size of Memory (Long Word)
 * @param[in]       p_device_io         device io handle for DSP
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
***********************************************************************************************************************/
#if defined (IMPDRV_DSPCRC_DETECT_ADDDEF)
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_crc_sub(
    const uint32_t              st,
    const uint32_t              sz,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);
#else
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_crc_sub(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
);
#endif

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_dspctl_is_valid_core()
* CD_PD_02_04_6001
* [Covers: UD_PD_UD02_04_0001]
***********************************************************************************************************************/
bool impdrv_dspctl_is_valid_core(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    bool is_valid_core = false;

    /** Compare core number to valid core number */
    if (core_num < IMPDRV_DSP_CORENUM_VAL)
    {
        is_valid_core = true;
    }

    return is_valid_core;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_is_valid_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_check_state()
* CD_PD_02_04_6002
* [Covers: UD_PD_UD02_04_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core = false;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_STATE_UNINIT != state) && (IMPDRV_STATE_INIT != state)
        && (IMPDRV_STATE_READY != state) && (IMPDRV_STATE_EXEC != state)
        && (IMPDRV_STATE_INT != state))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);

        /** When core number is valid */
        if (true == is_valid_core)
        {
            /** When core state and specified state is not match */
            if (state != p_corectl->core_state)
            {
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
        }
        else
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_check_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_state()
* CD_PD_02_04_6003
* [Covers: UD_PD_UD02_04_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_STATE_UNINIT != state) && (IMPDRV_STATE_INIT != state)
        && (IMPDRV_STATE_READY != state) && (IMPDRV_STATE_EXEC != state)
        && (IMPDRV_STATE_INT != state))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Update for Core state values */
        p_corectl->core_state = state;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_set_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_init_start()
* CD_PD_02_04_6004
* [Covers: UD_PD_UD02_04_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t        ercd_internal;
    bool                        is_valid_core;
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX];
    uint32_t                    l_reg;
    const st_impdrv_core_info_t core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
    st_impdrv_device_handle_t   device_io;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Request osdep to power on core */
    /* prepare device list array and initialize argument. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_get_device_io(&p_coretypectl->core_ctl[core_num], p_device_io);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd_internal = impdrv_osdep_pow_on_imp_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                                        core_info, IMPDRV_DSP_VDSP_DEV_MAIN);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }

        ercd_internal = impdrv_osdep_pow_on_imp_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB0],
                                                        core_info, IMPDRV_DSP_VDSP_DEV_SUB0);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }

        ercd_internal = impdrv_osdep_pow_on_imp_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB1],
                                                        core_info, IMPDRV_DSP_VDSP_DEV_SUB1);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }
    }

    /** Release reset (vdsp_bus). */
    if (IMPDRV_EC_OK == ercd)
    {
        device_io.handle = NULL;
        ercd = impdrv_osdep_dev_open_dsp(&device_io, core_info, IMPDRV_DSP_VDSP_DEV_SUB7);
    }

    if (IMPDRV_EC_OK == ercd)
        {
        ercd = impdrv_osdep_reset_core_dsp(&device_io, false);
        }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_dev_close(&device_io);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB0], true);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB1], true);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB0], false);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB1], false);
    }

    /** Read VCR register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN], IMPDRV_REGOFS_VDSP_VERSION1, &l_reg);
    }

    /** Check core version (unsupported core?) */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_REGVAL_VDSP_VERSION1 != l_reg)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_init_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_init_end()
* CD_PD_02_04_6005
* [Covers: UD_PD_UD02_04_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t        ercd_internal;
    bool                        is_valid_core;
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX];

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /* prepare device list array and initialize argument. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_get_device_io(&p_coretypectl->core_ctl[core_num], p_device_io);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB0], true);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB1], true);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Request osdep to close the core */
        ercd_internal = impdrv_osdep_dev_close(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN]);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }

        ercd_internal = impdrv_osdep_dev_close(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB0]);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }

        ercd_internal = impdrv_osdep_dev_close(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB1]);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_init_end()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_start_pre()
* CD_PD_02_04_6059
* [Covers: UD_PD_UD02_04_0059]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_start_pre(
    st_impdrv_corectl_t                 *p_corectl,
    const uint32_t                      core_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    e_impdrv_errorcode_t        ercd_internal;
    uint32_t                    i;
    const st_impdrv_core_info_t core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX];

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
    }

    /* prepare device list array and initialize argument. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_get_device_io(p_corectl, p_device_io);
    }

    /** Request for power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        /* device open */
        for (i=0; i < IMPDRV_DSP_DEV_NUM_MAX; i++)
        {
            ercd_internal = impdrv_osdep_pow_on_imp_dsp(p_device_io[i], core_info, i);
            if (IMPDRV_EC_OK != ercd_internal)
            {
                ercd = ercd_internal;
            }
        }
    }

    /** Initial setting for VDSP */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_init_core(p_device_io);
    }

    /** register interruot handler */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd_internal = impdrv_osdep_register_dsp_irq(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                core_num, irq_priority, osal_cb_args);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }
    }

    /** enable interruot handler */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd_internal = impdrv_osdep_enable_dsp_irq(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN]);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_start_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_start()
* CD_PD_02_04_6006
* [Covers: UD_PD_UD02_04_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    e_impdrv_errorcode_t        ercd_internal;
    uint32_t                    l_register;
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX];

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /* prepare device list array and initialize argument. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_get_device_io(&p_coretypectl->core_ctl[core_num], p_device_io);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_READY);
    }

    /* Reload new dsp_fw to PTCM */
    if (IMPDRV_EC_OK == ercd)
    {
        if ((IMPDRV_INIT_DSPMEM_ADDR != p_coretypectl->core_ctl[core_num].dsp_info.dsp_app.addr_phys) &&
            (IMPDRV_INIT_DSPMEM_ADDR != p_coretypectl->core_ctl[core_num].dsp_info.dsp_fw.addr_phys))
        {
            ercd = impdrv_dspctl_load_ptcm(&p_coretypectl->core_ctl[core_num], p_device_io);
        }
    }

    /* Application update requested and not loaded */
    if ((ercd == IMPDRV_EC_OK) && 
        (p_coretypectl->core_ctl[core_num].dsp_info.is_update == true) &&
        (p_coretypectl->core_ctl[core_num].dsp_info.is_loaded == false))
    {
        l_register = 0x0;
        /* Enable writing by CPU (Unlock) */
        ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_REGVAL_VDSP_ACU_UNLOCK, false, l_register);

        /* Reset D_ADDx, to new memory map. */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_dspctl_tcm_config_d(&p_coretypectl->core_ctl[core_num], p_device_io);
        }

        /* Reset P_ADDx, to new memory map. */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_dspctl_tcm_config_p(&p_coretypectl->core_ctl[core_num], p_device_io);
        }

        /* Reload new dsp_dtcm to DTCM. */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_dspctl_load_dtcm(&p_coretypectl->core_ctl[core_num], p_device_io);
        }

        /* disable writing by CPU (Lock) */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_REGVAL_VDSP_ACU_LOCK, false, l_register);
        }
        
        /* Reset DMBA */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_MSS_DMBA, IMPDRV_REGVAL_DMBE_INIT, false, l_register);
        }

        /* Clear the ECC status */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_PMASACS, IMPDRV_REGVAL_PMASACS_ECCLR, false, IMPDRV_REGVAL_PMASACS_ECCLR);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_P_UCSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_PMASACS, IMPDRV_REGVAL_PMASACS_ECCLR, false, IMPDRV_REGVAL_PMASACS_ECCLR);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_P_CSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_DBGACS, IMPDRV_REGVAL_DBGACS_ECCLR, false, IMPDRV_REGVAL_DBGACS_ECCLR);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_D_UCSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_DBGACS, IMPDRV_REGVAL_DBGACS_ECCLR, false, IMPDRV_REGVAL_DBGACS_ECCLR);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_D_CSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        /* enable error detection by ECM */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            impdrv_udefctl_set_ecm(core_num, true);
        }

        ercd = ercd_internal;


        /* Application upate request done and it is loaded */
        p_coretypectl->core_ctl[core_num].dsp_info.is_update = false;
        p_coretypectl->core_ctl[core_num].dsp_info.is_loaded = true;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_stop()
* CD_PD_02_04_6007
* [Covers: UD_PD_UD02_04_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    e_impdrv_errorcode_t        ercd_internal;
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX];
    uint32_t                    i;


    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** DSP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
    }

    /* prepare device list array. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_get_device_io(&p_coretypectl->core_ctl[core_num], p_device_io);
    }

    /* The DSP is stopped by a register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_stop_reg(p_coretypectl->core_ctl[core_num].dsp_info.is_executed, p_device_io);
    }

    /** Disable DSP IRQ */
    if ((IMPDRV_EC_OK == ercd) || (IMPDRV_EC_NG_DSP_HALT == ercd)) /* for VDSP error notice */
    {
        ercd_internal = impdrv_osdep_disable_dsp_irq(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN]);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }
    }


    /** Unregister DSP IRQ */
    if ((IMPDRV_EC_OK == ercd) || (IMPDRV_EC_NG_DSP_HALT == ercd)) /* for VDSP error notice */
    {
        ercd_internal = impdrv_osdep_unregister_dsp_irq(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN], core_num);
        if (IMPDRV_EC_OK != ercd_internal)
        {
            ercd = ercd_internal;
        }
    }

    /** Disable error detection by ECM */
    if (IMPDRV_EC_OK == ercd)
    {
        impdrv_udefctl_set_ecm(core_num, false);
    }

    /** Release for Power manager control */
    if ((IMPDRV_EC_OK == ercd) || (IMPDRV_EC_NG_DSP_HALT == ercd)) /* for VDSP error notice */
    {
        for (i=0; i < IMPDRV_DSP_DEV_NUM_MAX; i++)
        {
            /* set reset */
            ercd_internal = impdrv_osdep_reset_core_dsp(p_device_io[i], true);
            if (IMPDRV_EC_OK != ercd_internal)
            {
                ercd = ercd_internal;
            }
            else
            {
                if (i < IMPDRV_DSP_VDSP_DEV_SUB1)
                {
                    ercd_internal = impdrv_osdep_pow_off_imp(p_device_io[i]);
                    if (IMPDRV_EC_OK != ercd_internal)
                    {
                        ercd = ercd_internal;
                    }
                }
                else
                {
                    ercd_internal = impdrv_osdep_dev_close(p_device_io[i]);
                    if (IMPDRV_EC_OK != ercd_internal)
                    {
                        ercd = ercd_internal;
                    }
                }
            }
        }
    }

    /** IMP driver status update */
    if ((IMPDRV_EC_OK == ercd) || (IMPDRV_EC_NG_DSP_HALT == ercd)) /* for VDSP error notice */
    {
        /* Clear DSP status */
        p_coretypectl->core_ctl[core_num].dsp_info.is_update   = false;
        p_coretypectl->core_ctl[core_num].dsp_info.is_loaded   = false;
        p_coretypectl->core_ctl[core_num].dsp_info.is_executed = false;
        ercd = impdrv_dspctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_INIT);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_quit()
* CD_PD_02_04_6024
* [Covers: UD_PD_UD02_04_0024]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_quit(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    e_impdrv_errorcode_t        ercd_internal;
    st_impdrv_device_handle_t   device_io;
    uint32_t                    i;
    const st_impdrv_core_info_t core_info = {IMPDRV_CORE_TYPE_DSP, core_num};


    /** Check core number */
    is_valid_core = impdrv_dspctl_is_valid_core(core_num);
    if (true != is_valid_core)
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        device_io.handle = NULL;
        for (i=0; i < IMPDRV_DSP_VDSP_DEV_SUB1; i++)
        {
            ercd_internal = impdrv_osdep_dev_open_dsp(&device_io, core_info, i);
            if (IMPDRV_EC_OK != ercd_internal)
            {
                ercd = ercd_internal;
            }
            else
            {
                ercd_internal = impdrv_osdep_pow_off_imp(&device_io);
                if (IMPDRV_EC_OK != ercd_internal)
                {
                    ercd = ercd_internal;
                }
            }
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_attr_init()
* CD_PD_02_04_6008
* [Covers: UD_PD_UD02_04_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                i;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
    }

    /** Initialize core control informations(p_corectl) */
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->core_mem_init_enable = IMPDRV_PARAM_OFF;

        for (i = 0U; i < IMPDRV_COREMAP_MAXID; i++)
        {
            p_corectl->cur_core_map[i] = (uint8_t)0U;
        }

        p_corectl->claddr_phys = IMPDRV_INIT_CL_ADDR;

        for (i=0;i<IMPDRV_IRQMASK_MAX; i++)
        {
            p_corectl->irq_mask[i] = true;
        }

        for (i = 0U; i < IMPDRV_EXEPARAM_MAX; i++)
        {
            p_corectl->param[i] = 0U;
        }

        /* Reset dsp_info information */
        p_corectl->dsp_info.dsp_app.addr_phys = IMPDRV_INIT_DSPMEM_ADDR;
        p_corectl->dsp_info.dsp_app.size = 0U;
        p_corectl->dsp_info.dsp_fw.addr_phys = IMPDRV_INIT_DSPMEM_ADDR;
        p_corectl->dsp_info.dsp_fw.size = 0U;
        p_corectl->dsp_info.dsp_data.addr_phys = IMPDRV_INIT_DSPMEM_ADDR;
        p_corectl->dsp_info.dsp_data.size = 0U;
        p_corectl->dsp_info.dsp_dtcm.addr_phys = IMPDRV_INIT_DSPMEM_ADDR;
        p_corectl->dsp_info.dsp_dtcm.size = 0U;
        p_corectl->dsp_info.is_update = false;
        p_corectl->dsp_info.is_loaded = false;
        p_corectl->dsp_info.is_executed = false;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_mem_init()
* CD_PD_02_04_6009
* [Covers: UD_PD_UD02_04_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PARAM_OFF != enable) && (IMPDRV_PARAM_ON != enable))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
    }

    /** Set data to core_mem_init_enable */
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->core_mem_init_enable = enable;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_set_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_core_map()
* CD_PD_02_04_6010
* [Covers: UD_PD_UD02_04_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                i;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == syncc_val))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Set data to cur_core_map */
    if (IMPDRV_EC_OK == ercd)
    {
        for (i = 0U;  i < IMPDRV_COREMAP_MAXID; i++)
        {
            p_corectl->cur_core_map[i] = syncc_val[i];
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_set_core_map()
***********************************************************************************************************************/


/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_dsp()
* CD_PD_02_04_6060
* [Covers: UD_PD_UD02_04_0060]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_dsp(
    st_impdrv_corectl_t         *p_corectl,
    const uint32_t              core_num,
    const st_impdrv_dsp_data_t  *const p_dsp_app,
    const st_impdrv_dsp_data_t  *const p_dsp_fw,
    const st_impdrv_dsp_data_t  *const p_dsp_data,
    const st_impdrv_dsp_data_t  *const p_dsp_dtcm
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U != (p_dsp_app->addr_phys & IMPDRV_CHK_MEM_ALIGN))
        || (0U != (p_dsp_fw->addr_phys & IMPDRV_CHK_MEM_ALIGN))
        || (0U != (p_dsp_data->addr_phys & IMPDRV_CHK_MEM_ALIGN))
        || (0U != (p_dsp_dtcm->addr_phys & IMPDRV_CHK_MEM_ALIGN)))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
        if (IMPDRV_EC_NG_SEQSTATE == ercd)
        {
            ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->dsp_info.dsp_app  = *p_dsp_app;
        p_corectl->dsp_info.dsp_fw   = *p_dsp_fw;
        p_corectl->dsp_info.dsp_data = *p_dsp_data;
        p_corectl->dsp_info.dsp_dtcm = *p_dsp_dtcm;
        if ((0U != p_dsp_app->size)  ||
            (0U != p_dsp_fw->size)   ||
            (0U != p_dsp_data->size) ||
            (0U != p_dsp_dtcm->size))
        {
            /* Application update requested */
            p_corectl->dsp_info.is_update = true;
        }
        else
        {
            /* Continue same application */
            p_corectl->dsp_info.is_update = false;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_set_dsp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_execute()
* CD_PD_02_04_6014
* [Covers: UD_PD_UD02_04_0014]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_execute(
    st_impdrv_corectl_t     *p_corectl,
    const uint32_t          core_num,
    const p_impdrv_cbfunc_t callback_func,
    void                    *const p_callback_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;

    e_impdrv_errorcode_t      ercd_internal;
    st_impdrv_device_handle_t *p_device_io[IMPDRV_DSP_DEV_NUM_MAX];
    uint32_t                  l_register;

    uint32_t gp_val;
    uint32_t l_loop = 0U;

    /** Input parameter confirmation process */
    ercd = impdrv_dspctl_chk_execute_data(p_corectl, core_num, callback_func);

    /** Preparation before execution */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_dsp_execute_pre(p_corectl, core_num, p_device_io);
    }

    /* Application update requested but there is already application running. */
    if (IMPDRV_EC_OK == ercd) 
    {
        /* Application update requested */
        ercd = impdrv_dspctl_dsp_update_app(p_corectl, p_device_io);
    }

    /* Enable TIMER0_INT */
    if (IMPDRV_EC_OK == ercd)
    {
        /* Clear the ECC status */
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_PMASACS, IMPDRV_REGVAL_PMASACS_ECCLR, false, IMPDRV_REGVAL_PMASACS_ECCLR);
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_P_UCSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_PMASACS, IMPDRV_REGVAL_PMASACS_ECCLR, false, IMPDRV_REGVAL_PMASACS_ECCLR);
        }
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_P_CSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_DBGACS, IMPDRV_REGVAL_DBGACS_ECCLR, false, IMPDRV_REGVAL_DBGACS_ECCLR);
        }
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_D_UCSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_DBGACS, IMPDRV_REGVAL_DBGACS_ECCLR, false, IMPDRV_REGVAL_DBGACS_ECCLR);
        }
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_D_CSERR, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
        }

        /* enable error detection by ECM */
        if (ercd == IMPDRV_EC_OK)
        {
            impdrv_udefctl_set_ecm(core_num, true);
        }

        l_register = IMPDRV_REGVAL_OUT_INT_TIMER00;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_VDSP_OUT_INT_EN, l_register, true, l_register);

        l_register = IMPDRV_REGVAL_CLR;
        ercd_internal = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_VDSP_OUT_INT_MSK, l_register, true, l_register);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_internal;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /* set callback function */
        p_corectl->cbinfo.cbfunc = callback_func;
        p_corectl->cbinfo.p_cbarg = p_callback_args;

        /* change state */
        ercd = impdrv_dspctl_set_state(p_corectl, IMPDRV_STATE_EXEC);

        /* start callback */
        core_info.core_type = IMPDRV_CORE_TYPE_DSP;
        core_info.core_num  = core_num;
        (void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1, p_callback_args);
    }
    /* Set ICU_LOCK to 0*/
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_REGVAL_VDSP_ICU_UNLOCK, false, l_register);
    }

    /* 7. release the DSP resets. */
    if (IMPDRV_EC_OK == ercd)
    {
        if (false == p_corectl->dsp_info.is_executed)
        {
            ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN], false);
            p_corectl->dsp_info.is_executed = true;
        }
    }

    /* Trigger to start processing DSP application */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_VDSP_GPIO_START, false ,0U);
    }
    /* Waiting for receiving the start of stop processing from DSP */
    if (IMPDRV_EC_OK == ercd)
    {
        do {
            if ((IMPDRV_EC_OK == ercd) && (l_loop < IMPDRV_VDSP_GPIO_WAIT_LOOP))
            {
                ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],IMPDRV_REGOFS_VDSP_GPIO_OUT, &gp_val);
                l_loop++;
            } else {
                /*sequence error */
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
            /* for VDSP error notice */
            if (0 != (gp_val & IMPDRV_REGVAL_VDSP_ERROR))
            {
                ercd = IMPDRV_EC_NG_DSP_HALT;
            }

            if((IMPDRV_EC_NG_SEQSTATE == ercd) || (IMPDRV_EC_NG_DSP_HALT == ercd))
            {
                break;
            }
        } while (IMPDRV_REGVAL_VDSP_GPIO_START != gp_val);
    }

    /* ICU_LOCK Enable */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_REGVAL_VDSP_ICU_LOCK, false, l_register);
    }
    /* Clear GPIO_IN register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_CLR, false, l_register);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_int_handler()
* CD_PD_02_04_6015
* [Covers: UD_PD_UD02_04_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    uint32_t                intsts_val = 0U;
    uint32_t                int_status_val = 0U;
    uint32_t                ddesc_val = 0U;

    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX] = {IMPDRV_CB_RET_OK};
    uint32_t                cb_ercd_num = 0U;
    uint32_t                cb_num;
    e_impdrv_state_t        state = IMPDRV_STATE_EXEC;
    st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
    uint32_t                l_register = 0U;
    int32_t                 cb_code = IMPDRV_CALLBACK_CODE_DEF;
    uint32_t                loop_num=0;

    /** Input parameter confirmation process */
    ercd = impdrv_dspctl_chk_int_data(p_corectl, core_num);

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_OUT_INT_STA, &intsts_val);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_GPIO_OUT, &int_status_val);
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_EXEC);
    }

    /** Check MSS_DDESC_ID register state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_D_DDESC_ID, &ddesc_val);
        if ((IMPDRV_EC_OK == ercd) && (ddesc_val != 0U))
        {
            l_register |= IMPDRV_VDSP_D_DDCL_DDIE_RESET;
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_D_DDCL, l_register, false, l_register);
        }
    }


    /** Call back occurrence of an interrupt to the CL execute requester */
    if ((IMPDRV_EC_OK == ercd) && (0U != (IMPDRV_VDSP_CHANNEL_OPE_BITS & intsts_val)))
    {
        /* is check Interrupt of timer0_out */
        l_register = IMPDRV_REGVAL_OUT_INT_TIMER00 & (IMPDRV_VDSP_CHANNEL_OPE_BITS & intsts_val);
        
        if (0U == l_register)
        {
            while(loop_num < 100)
            {
                /* is check Interrupt of timer1_out */
                ercd = impdrv_osdep_read_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_OUT_INT_STA, &intsts_val);
                if(IMPDRV_EC_OK == ercd)
                {
                    l_register |= IMPDRV_REGVAL_OUT_INT_TIMER01 & (IMPDRV_VDSP_CHANNEL_OPE_BITS & intsts_val);
                }
                loop_num++;
            }
        }
        if (0U != l_register)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_OUT_INT_CLR, l_register, false, l_register);

            /* Work around Mask interrupt. */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_osdep_read_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_OUT_INT_MSK, &l_register);
                l_register |= IMPDRV_REGVAL_OUT_INT_TIMER00;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_OUT_INT_MSK, l_register, false, l_register);
            }

            if (IMPDRV_REGVAL_VDSP_INT == int_status_val)
            {
                /** When the INT bit of INTSTS register is not 0. */
                cb_ercd[cb_ercd_num] = IMPDRV_CB_RET_INT;
                cb_code = IMPDRV_VDSP_CB_CODE_INT;
                cb_ercd_num ++;
                if (IMPDRV_STATE_EXEC == state)
                {
                    state = IMPDRV_STATE_INT;
                }
            }

            if (IMPDRV_REGVAL_VDSP_TRAP == int_status_val)
            {
                /** When the Timer0_out interrupt. treate as TRAP */
                cb_ercd[cb_ercd_num] = IMPDRV_CB_RET_OK;
                cb_code = IMPDRV_VDSP_CB_CODE_TRAP;
                cb_ercd_num ++;
                if (IMPDRV_STATE_EXEC == state)
                {
                    state = IMPDRV_STATE_READY;
                }
            }

            /* for VDSP error notice */
            if (0 != (int_status_val & IMPDRV_REGVAL_VDSP_ERROR))
            {
                /** When the Timer0_out interrupt. error notice from VDSP */
                switch (int_status_val)
                {
                    case IMPDRV_REGVAL_VDSP_CRC_ERROR:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_CRC_ERROR;
                        break;
                    case IMPDRV_REGVAL_VDSP_NORMAL_INT:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_NORMAL_INT;
                        break;
                    case IMPDRV_REGVAL_VDSP_CRITICAL_INT:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_CRITICAL_INT;
                        break;
                    case IMPDRV_REGVAL_VDSP_SAFETY_ERROR:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_SAFETY_ERROR;
                        break;
                    case IMPDRV_REGVAL_VDSP_DIV0:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_DIV0;
                        break;
                    case IMPDRV_REGVAL_VDSP_WDOG_EXPIRE:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_WDOG_EXPIRE;
                        break;
                    case IMPDRV_REGVAL_VDSP_STACK_VIOLATION:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_STACK_VIOLATION;
                        break;
                    case IMPDRV_REGVAL_VDSP_TIM_FAULT:
                        cb_ercd[cb_ercd_num] = IMPDRV_CB_DSP_TIM_FAULT;
                        break;
                    default:
                        break;
                }
                cb_ercd_num ++;
                if (IMPDRV_STATE_EXEC == state)
                {
                    state = IMPDRV_STATE_READY;
                }
            }
        }

        /** Change core state */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dspctl_set_state(p_corectl, state);
        }

        /** Check callback num */
        if (IMPDRV_EC_OK == ercd)
        {
            if (0U == cb_ercd_num)
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }

        /** Interrupt result callback */
        if (IMPDRV_EC_OK == ercd)
        {
            cb_num = 0;
            while (cb_num < cb_ercd_num)
            {
                (void)((p_corectl->cbinfo.cbfunc)(
                            &core_info,
                            cb_ercd[cb_num],
                            cb_code,
                            p_corectl->cbinfo.p_cbarg
                ));
                cb_num++;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_resume_exe()
* CD_PD_02_04_6016
* [Covers: UD_PD_UD02_04_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_internal = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                l_register;
    uint32_t                gp_val = 0U;
    uint32_t                l_loop;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
    }

    /** CL resume on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /* Enable TIMER0_INT, TIMER1_INT */
    if (IMPDRV_EC_OK == ercd)
    {
        l_register = IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_REGVAL_OUT_INT_TIMER01;
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_VDSP_OUT_INT_EN, l_register, true, l_register);

        l_register = IMPDRV_REGVAL_CLR;
        ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_VDSP_OUT_INT_MSK, l_register, true, l_register);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_internal;
        }
    }

    /** CL resume on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /* Notify DSP that resume processing is completed by writing to GPIN  */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_VDSP_RESUME, false ,0U);

        /* Waiting for receiving the start of stop processing from DSP */
        l_loop = 0U;
        do {
            if ((ercd == IMPDRV_EC_OK) && (l_loop < IMPDRV_VDSP_GPIO_WAIT_LOOP))
            {
                ercd = impdrv_osdep_read_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_GPIO_OUT, &gp_val);
                l_loop++;
            } else {
                /*sequence error */
                ercd = IMPDRV_EC_NG_SEQSTATE;
                break;
            }
        } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_RESUME);

        /* for VDSP error notice */
        if (0 != (gp_val & IMPDRV_REGVAL_VDSP_ERROR))
        {
            ercd = IMPDRV_EC_NG_DSP_HALT;
        }

        /* Clear GPIO_IN register */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_CLR, false ,0U);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_resume_exe()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_pm_set_policy()
* CD_PD_02_04_6017
* [Covers: UD_PD_UD02_04_0017]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    st_impdrv_core_info_t       core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
    st_impdrv_device_handle_t   *p_device_io;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PM_POLICY_CG != policy) 
            && (IMPDRV_PM_POLICY_PG != policy) 
            && (IMPDRV_PM_POLICY_HP != policy))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_STATE_UNINIT == p_coretypectl->core_ctl[core_num].core_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
    }
    
    /* Changes the Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        p_device_io = &p_coretypectl->core_ctl[core_num].device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN];

        ercd = impdrv_osdep_pm_set_policy(p_device_io, core_info, policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_pm_get_policy()
* CD_PD_02_04_6018
* [Covers: UD_PD_UD02_04_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    st_impdrv_core_info_t       core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
    st_impdrv_device_handle_t   *p_device_io;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_policy))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_STATE_UNINIT == p_coretypectl->core_ctl[core_num].core_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
    }
    
    /** Get Power Management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        p_device_io = &p_coretypectl->core_ctl[core_num].device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN];

        ercd = impdrv_osdep_pm_get_policy(p_device_io, core_info, p_policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_pm_get_state()
* CD_PD_02_04_6013
* [Covers: UD_PD_UD02_04_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    st_impdrv_core_info_t       core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
    st_impdrv_device_handle_t   *p_device_io;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_pmstate))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_STATE_UNINIT == p_coretypectl->core_ctl[core_num].core_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
    }
    
    /** Get Power Management state */
    if (IMPDRV_EC_OK == ercd)
    {
        p_device_io = &p_coretypectl->core_ctl[core_num].device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN];

        ercd = impdrv_osdep_pm_get_state(p_device_io, core_info, p_pmstate);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_conf_reg_check()
* CD_PD_02_04_6054
* [Covers: UD_PD_UD02_04_0054]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    uint32_t                    index;
    uint32_t                    reg_val;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_chk_param))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U == param_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(&p_coretypectl->core_ctl[core_num],
                            core_num, IMPDRV_STATE_READY);
    }

    /** Confirmation loop of Configuration register. */
    index = 0;
    while ((IMPDRV_EC_OK == ercd) && (param_num > index))
    {
        /** Input parameter confirmation process */
        if (0U != (p_chk_param[index].offset & IMPDRV_CHK_REG_ALIGN))
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
        else if (IMPDRV_DSP_REG_MAX_ADRS_CNFCHK < p_chk_param[index].offset)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            /** Nothing to do. */
        }

        /** Read the configuration register */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_read_reg(
                &(p_coretypectl->core_ctl[core_num].device_io_dsp[IMPDRV_DSP_VDSP_DEV_MAIN]),
                (uint32_t)p_chk_param[index].offset,
                &reg_val
            );
        }
        if (IMPDRV_EC_OK == ercd)
        {
            /** Configuration register check */
            if ((reg_val & p_chk_param[index].bit_mask)
                != (p_chk_param[index].data & p_chk_param[index].bit_mask))
            {
                /** Error Handring */
                ercd = IMPDRV_EC_NG_CHECKFAIL;
            }
        }

        /** Check the next register */
        index++;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_conf_reg_check()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_init_core()
* CD_PD_02_04_6019
* [Covers: UD_PD_UD02_04_0019]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dspctl_init_core(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    uint32_t l_loop;
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;


   /** Input parameter confirmation process */
    if ((p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN] == NULL) ||
        (p_device_io[IMPDRV_DSP_VDSP_DEV_SUB0] == NULL) ||
        (p_device_io[IMPDRV_DSP_VDSP_DEV_SUB1] == NULL) ||
        (p_device_io[IMPDRV_DSP_VDSP_DEV_SUB2] == NULL) ||
        (p_device_io[IMPDRV_DSP_VDSP_DEV_SUB3] == NULL) ||
        (p_device_io[IMPDRV_DSP_VDSP_DEV_SUB4] == NULL) ||
        (p_device_io[IMPDRV_DSP_VDSP_DEV_SUB5] == NULL) ||
        (p_device_io[IMPDRV_DSP_VDSP_DEV_SUB6] == NULL)
        )
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /* reset all device regarding to the VDSP. */
    if (IMPDRV_EC_OK == ercd)
    {
        for (l_loop = 0; l_loop < IMPDRV_DSP_DEV_NUM_MAX; l_loop++)
        {
            ercd = impdrv_osdep_reset_core_dsp(p_device_io[l_loop], true);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
        }
    }

    /* At first allow to register access. */
    /* release the DSP resets. reg and csd. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB0], false);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB1], false);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_init_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_init_dsp()
* CD_PD_02_04_6061
* [Covers: UD_PD_UD02_04_0061]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dspctl_init_dsp(
    st_impdrv_corectl_t *p_corectl
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** No processing on the HIL */

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_init_dsp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_get_device_io()
* CD_PD_02_04_6062
* [Covers: UD_PD_UD02_04_0062]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_get_device_io(
    st_impdrv_corectl_t         *const p_corectl,
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                cnt;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /* prepare device list array */
    if (IMPDRV_EC_OK == ercd)
    {
        for (cnt = 0U; IMPDRV_DSP_DEV_NUM_MAX > cnt; cnt++)
        {
            p_device_io[cnt] = &p_corectl->device_io_dsp[cnt];
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_get_device_io()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_chk_execute_data()
* CD_PD_02_04_6063
* [Covers: UD_PD_UD02_04_0063]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_chk_execute_data(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == callback_func))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (IMPDRV_INIT_DSPMEM_ADDR == p_corectl->dsp_info.dsp_app.addr_phys)
    {
        ercd = IMPDRV_EC_NG_ATTRIBUTE;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_chk_execute_data()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_execute_pre()
* CD_PD_02_04_6064
* [Covers: UD_PD_UD02_04_0064]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_execute_pre(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    st_impdrv_device_handle_t   *p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    /** Private static variables */
    IMPDRV_STATIC
    const uint32_t  s_reg_addr_tbl[IMPDRV_DSP_CORENUM_VAL] = {
        IMPDRV_REG_IMPSLV_DSP0CSTS,
        IMPDRV_REG_IMPSLV_DSP1CSTS
    };

    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    st_impdrv_device_handle_t   handle;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == p_device_io))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_init_dsp(p_corectl);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_get_io_imptop(&handle);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&handle, s_reg_addr_tbl[core_num],
                                IMPDRV_REGVAL_IMPSLV_DSPNCSTS, false, 0U);
        }
    }

    /* prepare device list array. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_get_device_io(p_corectl, p_device_io);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_execute_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_update_app()
* CD_PD_02_04_6073
* [Covers: UD_PD_UD02_04_0073]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_update_app(
    st_impdrv_corectl_t         *const p_corectl,
    st_impdrv_device_handle_t   *const *p_device_io
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                l_register;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || ( NULL == p_device_io))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /* Set DSP to standby */
    if ((IMPDRV_EC_OK == ercd) &&
        (true == p_corectl->dsp_info.is_loaded) &&
        (true == p_corectl->dsp_info.is_update) &&
        (true == p_corectl->dsp_info.is_executed))
    {
        ercd = impdrv_dspctl_dsp_foece_standby(p_device_io);
        /* reset status */
        p_corectl->dsp_info.is_loaded = false;
        p_corectl->dsp_info.is_executed = false;
    }
 
    /* Application running if not application running. */
    if ((IMPDRV_EC_OK == ercd) &&
        (false == p_corectl->dsp_info.is_loaded) &&
        (true  == p_corectl->dsp_info.is_update) &&
        (false == p_corectl->dsp_info.is_executed))
    {
        /* Reload new dsp_fw to PTCM. */
        ercd = impdrv_dspctl_load_ptcm(p_corectl, p_device_io);

        if (IMPDRV_EC_OK == ercd)
        {
            l_register = 0x0;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_REGVAL_VDSP_ACU_UNLOCK, false, l_register);
            
        }
        /* Reset D_ADDx, to new memory map. */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dspctl_tcm_config_d(p_corectl, p_device_io);
        }
        /* Reset P_ADDx, to new memory map. */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dspctl_tcm_config_p(p_corectl, p_device_io);
        }
        /* Reload new dsp_dtcm to DTCM. */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dspctl_load_dtcm(p_corectl, p_device_io);
        }
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_REGVAL_VDSP_ACU_LOCK, false, l_register);
        }
        /* reset status */
        p_corectl->dsp_info.is_update = false;
        p_corectl->dsp_info.is_loaded = true;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_update_app()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_load_ptcm()
* CD_PD_02_04_6065
* [Covers: UD_PD_UD02_04_0065]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_load_ptcm(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_dspctl_load_type_t load_flag = IMPDRV_VDSP_LOAD_TYPE_INT;
    uint32_t                    l_register;
    uint32_t                    l_loop;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || ( NULL == p_device_io))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    /* Check DSP application address */
    else if ((IMPDRV_VDSP_EXT_MEM_START <= (uint32_t)p_corectl->dsp_info.dsp_fw.addr_phys) &&
        (IMPDRV_VDSP_EXT_MEM_END > (uint32_t)p_corectl->dsp_info.dsp_fw.addr_phys) &&
        (0x0U < (uint32_t)p_corectl->dsp_info.dsp_fw.size) &&
        (IMPDRV_VDSP_INT_PROG_MEM_SIZE >= (uint32_t)p_corectl->dsp_info.dsp_fw.size))
    {
        load_flag = IMPDRV_VDSP_LOAD_TYPE_INT;
    }
    else if ((IMPDRV_VDSP_EXT_MEM_START <= (uint32_t)p_corectl->dsp_info.dsp_app.addr_phys) &&
        (IMPDRV_VDSP_EXT_MEM_END >  (uint32_t)p_corectl->dsp_info.dsp_app.addr_phys))
    {
        load_flag = IMPDRV_VDSP_LOAD_TYPE_EXT;
    }
    else
    {
        ercd = IMPDRV_EC_NG_ATTRIBUTE;
    }

    if ((ercd == IMPDRV_EC_OK) && (p_corectl->dsp_info.is_loaded == false))
    {
        /* Start from the vector address RST_VEC_ADDR */
        /* 1. release the DSP resets. reg and csd. */
        /* 2. Set the PLOAD_STOP bit in register DSP_CONTROL to 0.*/
        ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_VDSP_CONTROL, &l_register);
        if (ercd == IMPDRV_EC_OK)
        {
            l_register &= ~(uint32_t)IMPDRV_VDSP_CONTROL_PLOAD_STOP;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_VDSP_CONTROL, l_register, true, l_register);
        }

        /* 4. Set the reset vector RST_VEC_ADDR. */
        if ((ercd == IMPDRV_EC_OK) && (IMPDRV_VDSP_LOAD_TYPE_EXT == load_flag))
        {
            l_register = (uint32_t)p_corectl->dsp_info.dsp_app.addr_phys;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_RST_VEC_ADR, l_register, true, l_register);
        }
        /* 5. release the DSP resets. global and sys. */
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB6], false);
        }
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB2], false);
        }
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_reset_core_dsp(p_device_io[IMPDRV_DSP_VDSP_DEV_SUB3], false);
        }

        if ((ercd == IMPDRV_EC_OK) && (IMPDRV_VDSP_LOAD_TYPE_INT == load_flag))
        {
            /* Transfer the boot program from external to internal program. */
            l_register = (uint32_t)p_corectl->dsp_info.dsp_fw.addr_phys;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_P_PDEA, l_register, true, l_register);

            /* internal address */
            if (IMPDRV_EC_OK == ercd)
            {
                l_register = IMPDRV_REGVAL_CLR;
                ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_P_PDIA, l_register, true, l_register);
            }
            /* transfer size */
            if (IMPDRV_EC_OK == ercd)
            {
                l_register = p_corectl->dsp_info.dsp_fw.size;
                ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_P_PDTC, l_register, false, l_register);
            }

            if (IMPDRV_EC_OK == ercd)
            {
                l_loop = IMPDRV_VDSP_P_PDTC_WAIT_LOOP;
                do {
                    l_loop--;
                    ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                        IMPDRV_REGOFS_P_PDTC, &l_register);
                    if ((ercd != IMPDRV_EC_OK) || (l_loop == 0U))
                    {
                        ercd = IMPDRV_EC_NG_SYSTEMERROR;
                        break;
                    }
                } while((l_register & IMPDRV_VDSP_REGVAL_PDMA_BUSY) != 0U);
            }
            if(p_corectl->dsp_info.dsp_fw.size < IMPDRV_VDSP_IPM_FIXED_SIZE_8KB)
            {
                /* Transfer the boot program from external to internal program. */
                l_register = (uint32_t)p_corectl->dsp_info.dsp_fw.addr_phys;
                ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_P_PDEA, l_register, true, l_register);

                /* internal address */
                if (IMPDRV_EC_OK == ercd)
                {
                    l_register = p_corectl->dsp_info.dsp_fw.size;
                    ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                        IMPDRV_REGOFS_P_PDIA, l_register, true, l_register);
                }
                /* transfer size */
                if (IMPDRV_EC_OK == ercd)
                {
                    l_register = IMPDRV_VDSP_IPM_FIXED_SIZE_8KB - p_corectl->dsp_info.dsp_fw.size;
                    ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                        IMPDRV_REGOFS_P_PDTC, l_register, false, l_register);
                }

                if (IMPDRV_EC_OK == ercd)
                {
                    l_loop = IMPDRV_VDSP_P_PDTC_WAIT_LOOP;
                    do {
                        l_loop--;
                        ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                            IMPDRV_REGOFS_P_PDTC, &l_register);
                        if ((ercd != IMPDRV_EC_OK) || (l_loop == 0U))
                        {
                            ercd = IMPDRV_EC_NG_SYSTEMERROR;
                            break;
                        }
                    } while((l_register & IMPDRV_VDSP_REGVAL_PDMA_BUSY) != 0U);
                }
            }
        }
    }
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_load_ptcm()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_tcm_config_d()
* CD_PD_02_04_6066
* [Covers: UD_PD_UD02_04_0066]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_tcm_config_d(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                l_register;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || ( NULL == p_device_io))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /* setting external memory config */
    /* D_ADDR0, EDP port */
    /* CEVA DSP Bug #35 W.A. */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_MOM;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD0_ATT0, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_L2A_WR | IMPDRV_REGVAL_VDSP_EXT_L2A_RD |
                        IMPDRV_REGVAL_VDSP_EXT_DADOL | IMPDRV_REGVAL_VDSP_EXT_DAUOL;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD0_ATT1, l_register, false, l_register);
    }

    /* D_ADDR1 : Shared memory, EDP port */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = ((((uint32_t)p_corectl->dsp_info.dsp_data.addr_phys) >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                        IMPDRV_REGVAL_VDSP_REG_EDP | IMPDRV_REGVAL_VDSP_REG_ACT | IMPDRV_REGVAL_VDSP_EXT_DPRAW);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD1_START, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_MOM;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD1_ATT0, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_L2A_WR | IMPDRV_REGVAL_VDSP_EXT_L2A_RD | IMPDRV_REGVAL_VDSP_EXT_DABSZ8 |
                        IMPDRV_REGVAL_VDSP_EXT_DADOL | IMPDRV_REGVAL_VDSP_EXT_DAUOL;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD1_ATT1, l_register, false, l_register);
    }

    /* D_ADDR2 : Inactive the after shared memory */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register  = (uint32_t)p_corectl->dsp_info.dsp_data.addr_phys;
        l_register += p_corectl->dsp_info.dsp_data.size;
        l_register = ((l_register >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                        IMPDRV_REGVAL_VDSP_REG_BLANK | IMPDRV_REGVAL_VDSP_REG_ACT);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD2_START, l_register, false, l_register);
    }

    /* D_ADDR3 : SPMC, EDP port */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = (((uint32_t)IMPDRV_SPMX_ADDR_START >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                        IMPDRV_REGVAL_VDSP_REG_AXI0 | IMPDRV_REGVAL_VDSP_REG_ACT | IMPDRV_REGVAL_VDSP_EXT_DPRAW);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD3_START, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_MOM;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD3_ATT0, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_L2A_WR | IMPDRV_REGVAL_VDSP_EXT_L2A_RD | IMPDRV_REGVAL_VDSP_EXT_DABSZ8 |
                        IMPDRV_REGVAL_VDSP_EXT_DADOL | IMPDRV_REGVAL_VDSP_EXT_DAUOL;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD3_ATT1, l_register, false, l_register);
    }

    /* D_ADDR4 : Inactive the after SPMC */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = (((uint32_t)IMPDRV_SPMX_ADDR_END >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                        IMPDRV_REGVAL_VDSP_REG_BLANK | IMPDRV_REGVAL_VDSP_REG_ACT);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD4_START, l_register, false, l_register);
    }

    /* D_ADDR5 : IMP Register, EDP port */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = (((uint32_t)IMPDRV_IMPREG_ADDR_START >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                        IMPDRV_REGVAL_VDSP_REG_EDP | IMPDRV_REGVAL_VDSP_REG_ACT | IMPDRV_REGVAL_VDSP_EXT_DPRAW);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD5_START, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_MOM;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD5_ATT0, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_EXT_L2A_WR | IMPDRV_REGVAL_VDSP_EXT_L2A_RD |
                        IMPDRV_REGVAL_VDSP_EXT_DADOL | IMPDRV_REGVAL_VDSP_EXT_DAUOL;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD5_ATT1, l_register, false, l_register);
    }

    /* D_ADDR6 : Inactive the after IMP Register */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = (((uint32_t)IMPDRV_IMPREG_ADDR_END >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                        IMPDRV_REGVAL_VDSP_REG_BLANK |IMPDRV_REGVAL_VDSP_REG_ACT);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_ADD6_START, l_register, false, l_register);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_tcm_config_d()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_tcm_config_p()
* CD_PD_02_04_6067
* [Covers: UD_PD_UD02_04_0067]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_tcm_config_p(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                l_register;
    uint32_t                l_loop;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || ( NULL == p_device_io))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /* P_ADDR0 */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_VDSP_P_L2A_ATT0 | IMPDRV_VDSP_P_AP_ATT0;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD0_ATT0, l_register, false, l_register);
    }

    /* P_ADDR1 : Program memory */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = ((((uint32_t)p_corectl->dsp_info.dsp_app.addr_phys) >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                                                                                IMPDRV_REGVAL_VDSP_REG_ACT);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD1_START, l_register, false, l_register);
    }

    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_VDSP_P_ADDX_ATT0 | IMPDRV_VDSP_P_L2A_ATT0;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD1_ATT0, l_register, false, l_register);
    }

    /* P_ADDR2 : Inactive the after Program memory */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register  = (uint32_t)p_corectl->dsp_info.dsp_app.addr_phys;
        l_register += p_corectl->dsp_info.dsp_app.size;
        l_register = ((l_register >> IMPDRV_VDSP_D_ADDR_SHIFT) | IMPDRV_REGVAL_VDSP_REG_INACT);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD2_START, l_register, false, l_register);
    }
    /* P_ADDR2 */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_VDSP_P_L2A_ATT0 | IMPDRV_VDSP_P_AP_ATT0;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD2_ATT0, l_register, false, l_register);
    }
    /* P_ADDR3 : Internal Program memory */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = ((((uint32_t)p_corectl->dsp_info.dsp_fw.addr_phys) >> IMPDRV_VDSP_D_ADDR_SHIFT) |
                                                                                IMPDRV_REGVAL_VDSP_REG_ACT);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD3_START, l_register, false, l_register);
    }
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_VDSP_P_ADDX_ATT0 | IMPDRV_VDSP_P_L2A_ATT0;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD3_ATT0, l_register, false, l_register);
    }

    /* P_ADDR4 : Inactive the Internal Program memory */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register  = (uint32_t)p_corectl->dsp_info.dsp_fw.addr_phys;
        l_register += p_corectl->dsp_info.dsp_fw.size;
        l_register = ((l_register >> IMPDRV_VDSP_D_ADDR_SHIFT) | IMPDRV_REGVAL_VDSP_REG_INACT);
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_ADD4_START, l_register, false, l_register);
    }

    /*** cache enable ***/
    /* P_CCOSAR */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = (uint32_t)p_corectl->dsp_info.dsp_data.addr_phys & IMPDRV_VDSP_P_CCOSA_MASK;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_CCOSAR, l_register, false, l_register);
    }

    /* P_CCOCR cache invalidate */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = IMPDRV_REGVAL_VDSP_P_CCOCR_IV;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_CCOCR, l_register, false, l_register);
        if (ercd == IMPDRV_EC_OK)
        {
            l_loop = 100000U;
            do {
                ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                    IMPDRV_REGOFS_P_CCOCR, &l_register);
                l_loop--;
                if (l_loop == 0U) {
                    ercd = IMPDRV_EC_NG_SYSTEMERROR;
                    break;
                }
            } while ((l_register & (uint32_t)IMPDRV_VDSP_P_L1ICO_MASK) != 0U);
        }
    }

    /* P_CCOCR cache pre-fetch */
    if (ercd == IMPDRV_EC_OK)
    {
        l_register = (uint32_t)IMPDRV_REGVAL_VDSP_P_CCOCR_PF;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_P_CCOCR, l_register, false, l_register);
        if (ercd == IMPDRV_EC_OK)
        {
            l_loop = IMPDRV_VDSP_P_CCOCR_WAIT_LOOP;
            do {
                ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                    IMPDRV_REGOFS_P_CCOCR, &l_register);
                l_loop--;
                if (l_loop == 0U) {
                    ercd = IMPDRV_EC_NG_SYSTEMERROR;
                    break;
                }
            } while ((l_register & (uint32_t)IMPDRV_VDSP_P_L1ICO_MASK) != 0U);
        }
    }

    /* MSS_PCR */
    if (ercd == IMPDRV_EC_OK)
    {
        ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_MSS_PCR, &l_register);
        if (ercd == IMPDRV_EC_OK)
        {
            l_register |= (uint32_t)IMPDRV_REGVAL_VDSP_MSS_PCR;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                            IMPDRV_REGOFS_MSS_PCR, l_register, false, l_register);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_tcm_config_p()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_load_dtcm()
* CD_PD_02_04_6068
* [Covers: UD_PD_UD02_04_0068]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_load_dtcm(
    const st_impdrv_corectl_t   *const p_corectl,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                l_register;
    uint32_t                l_loop;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || ( NULL == p_device_io))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if ((ercd == IMPDRV_EC_OK) && (0U < ((uint32_t)p_corectl->dsp_info.dsp_dtcm.size)))
    {
        /* Transfer the initial value from external to internal data memory(DTCM). */
        l_register = (uint32_t)p_corectl->dsp_info.dsp_dtcm.size;
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_D_DDTC, l_register, false, l_register);
        /* internal address */
        if (IMPDRV_EC_OK == ercd)
        {
            l_register = (uint32_t)p_corectl->dsp_info.dsp_dtcm.addr_phys;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_DDEA, l_register, false, l_register);
        }
        /* transfer size */
        if (IMPDRV_EC_OK == ercd)
        {
            l_register = IMPDRV_REGVAL_CLR;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_DDIA, l_register, false, l_register);
        }
        if (IMPDRV_EC_OK == ercd)
        {
            l_register = IMPDRV_REGVAL_VDSP_BSZ_8_INCR | IMPDRV_VDSP_D_DDCL_DDIE_ENABLE;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_DDCL, l_register, false, l_register);
        }
        if (IMPDRV_EC_OK == ercd)
        {
            l_loop = IMPDRV_VDSP_D_DDCL_WAIT_LOOP;
            do {
                l_loop--;
                ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                    IMPDRV_REGOFS_D_DDCL, &l_register);
                if ((ercd != IMPDRV_EC_OK) || (l_loop == 0U))
                {
                    ercd = IMPDRV_EC_NG_SYSTEMERROR;
                    break;
                }
            } while((l_register & IMPDRV_VDSP_DDMA_BUSY_MASK) != 0U);
        }        
        if(p_corectl->dsp_info.dsp_dtcm.size < IMPDRV_VDSP_IDM_FIXED_SIZE_64KB)
        {
            /* Transfer the initial value from external to internal data memory(DTCM). */
            l_register = IMPDRV_VDSP_IDM_FIXED_SIZE_64KB - (uint32_t)p_corectl->dsp_info.dsp_dtcm.size;
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_DDTC, l_register, false, l_register);
            /* internal address */
            if (IMPDRV_EC_OK == ercd)
            {
                l_register = (uint32_t)p_corectl->dsp_info.dsp_dtcm.addr_phys;
                ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                    IMPDRV_REGOFS_D_DDEA, l_register, false, l_register);
            }
            /* transfer size */
            if (IMPDRV_EC_OK == ercd)
            {
                l_register = p_corectl->dsp_info.dsp_dtcm.size;
                ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                    IMPDRV_REGOFS_D_DDIA, l_register, false, l_register);
            }
            if (IMPDRV_EC_OK == ercd)
            {
                l_register = IMPDRV_REGVAL_VDSP_BSZ_8_INCR;
                ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                    IMPDRV_REGOFS_D_DDCL, l_register, false, l_register);
            }

            /* wait DDMA transfer finished*/
            if (IMPDRV_EC_OK == ercd)
            {
                l_loop = IMPDRV_VDSP_D_DDCL_WAIT_LOOP;
                do {
                    l_loop--;
                    ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                        IMPDRV_REGOFS_D_DDCL, &l_register);
                    if ((ercd != IMPDRV_EC_OK) || (l_loop == 0U))
                    {
                        ercd = IMPDRV_EC_NG_SYSTEMERROR;
                        break;
                    }
                } while((l_register & IMPDRV_VDSP_DDMA_BUSY_MASK) != 0U);
            }
        }
        if (IMPDRV_EC_OK == ercd)
        {
#if defined (IMPDRV_DSPCRC_DETECT_ADDDEF)
            uint32_t  st = (uint32_t)p_corectl->dsp_info.dsp_dtcm.addr_phys;
            uint32_t  sz = p_corectl->dsp_info.dsp_dtcm.size;
            ercd = impdrv_dspctl_crc_sub(st, sz, p_device_io);
#else
            ercd = impdrv_dspctl_crc_sub(p_device_io);
#endif
        }
    }
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_load_dtcm()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_stop_reg()
* CD_PD_02_04_6069
* [Covers: UD_PD_UD02_04_0069]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_stop_reg(
    const bool                  is_executed,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                l_loop;
    uint32_t                gp_val = 0U;
    bool                    is_stopped = true;

    /** Input parameter confirmation process */
    if (NULL == p_device_io)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /* Application is executed */
    if (true == is_executed)
    {

        if (IMPDRV_EC_OK == ercd)
        {
            /* Trigger to stop processing DSP application */
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_VDSP_GPIO_START, false ,0U);
            /* Waiting for receiving the start of stop processing from DSP */
            l_loop = 0U;
            do {
                if ((ercd == IMPDRV_EC_OK) && (l_loop < IMPDRV_VDSP_GPIO_WAIT_LOOP))
                {
                    ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_GPIO_OUT, &gp_val);
                    l_loop++;
                } else {
                    /*sequence error */
                    is_stopped = false;
                    break;
                }
            } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_STOP);
            /* for VDSP error notice */
            if (gp_val == IMPDRV_REGVAL_VDSP_ERROR)
            {
                ercd = IMPDRV_EC_NG_DSP_HALT;
            }
        }

        /* Normal end */
        if ((IMPDRV_EC_OK == ercd) &&
            (true == is_stopped))
        {
            /* Clear GPIO_IN register */
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_CLR, false ,0U);

            /* Wait for receiving the DSP enters STANDBY */
            l_loop = 0U;
            do {
                if ((ercd == IMPDRV_EC_OK) && (l_loop < IMPDRV_VDSP_GPIO_WAIT_LOOP))
                {
                    ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                    IMPDRV_REGOFS_VDSP_GPIO_OUT, &gp_val);
                    l_loop++;
                } else {
                    /*sequence error */
                    ercd = IMPDRV_EC_NG_SEQSTATE;
                    break;
                }
            } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_STDBY);
            /* for VDSP error notice */
            if (0 != (gp_val & IMPDRV_REGVAL_VDSP_ERROR))
            {
                ercd = IMPDRV_EC_NG_DSP_HALT;
            }

            /* Clear GPIO_IN register */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_CLR, false ,0U);
            }
            /* Check that DSP is Stand by Mode */
            if (ercd == IMPDRV_EC_OK)
            {
                ercd = impdrv_dspctl_dsp_standby(p_device_io);
            }
        }
        else
        {
            ercd = impdrv_dspctl_dsp_foece_standby(p_device_io);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_stop_reg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_standby()
* CD_PD_02_04_6070
* [Covers: UD_PD_UD02_04_0070]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_standby(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                reg = 0U;
    uint32_t                l_loop;

    /** Input parameter confirmation process */
    if ( NULL == p_device_io)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /* check that DSP is Light Sleep Mode */
    if (ercd == IMPDRV_EC_OK)
    {
        l_loop = IMPDRV_VDSP_PSU_ST_SLP_LOOP;
        do {
            l_loop--;
            ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN], IMPDRV_REGOFS_VDSP_PSU_STATE, &reg);

            if ((ercd != IMPDRV_EC_OK) || (0U == l_loop))
            {
                /*sequence error */
                ercd = IMPDRV_EC_NG_SEQSTATE;
                break;
            }
        } while (0U == (reg & IMPDRV_REGVAL_PSU_STATE_LSP));
    }
    
    /* If DSP is Light sleep mode, execute CEVA bug#36 W.A. operation */
    if ((ercd == IMPDRV_EC_OK) &&
        ((reg & (uint32_t)IMPDRV_REGVAL_PSU_STATE_SBY) == 0U)) 
    {
        /* check that MSS of DSP is idle */
        l_loop = IMPDRV_VDSP_DMBE_WAIT_LOOP;
        do {
            l_loop--;
            ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN], IMPDRV_REGOFS_DMBE, &reg);

            if ((ercd != IMPDRV_EC_OK) || (0U == l_loop))
            {
                /*sequence error */
                ercd = IMPDRV_EC_NG_SEQSTATE;
                break;
            }
        } while (reg != IMPDRV_REGVAL_DMBE_IDLE);

        /* Set PSVM.PMOD to santby mode */
        if (ercd == IMPDRV_EC_OK)
        {
            ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_PSVM, IMPDRV_REGVAL_PSVM_PMOD_SBY, false, 0U);
        }
        /* check that DSP is Stand by Mode */
        if (ercd == IMPDRV_EC_OK)
        {
            l_loop = IMPDRV_VDSP_PSU_ST_STD_LOOP;
            do {
                l_loop--;
                ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN], IMPDRV_REGOFS_VDSP_PSU_STATE, &reg);

                if ((ercd != IMPDRV_EC_OK) || (0U == l_loop))
                {
                    /*sequence error */
                    ercd = IMPDRV_EC_NG_SEQSTATE;
                    break;
                }
            } while (0U == (reg & (IMPDRV_REGVAL_PSU_STATE_LSP|IMPDRV_REGVAL_PSU_STATE_SBY)));
        }
    }
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_standby()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_foece_standby()
* CD_PD_02_04_6071
* [Covers: UD_PD_UD02_04_0071]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_dsp_foece_standby(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                l_loop;
    uint32_t                gp_val = 0U;

    /** Input parameter confirmation process */
    if ( NULL == p_device_io)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (ercd == IMPDRV_EC_OK)
    {
        /* Trigger to standby processing DSP application */
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_GPIO_IN, IMPDRV_REGVAL_VDSP_GPIO_FSTDBY, false ,0U);
        /* Waiting for receiving the start of standby processing from DSP */
        l_loop = 0U;
        do {
            if ((ercd == IMPDRV_EC_OK) && (l_loop < IMPDRV_VDSP_GPIO_WAIT_LOOP))
            {
                ercd = impdrv_osdep_read_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                                IMPDRV_REGOFS_VDSP_GPIO_OUT, &gp_val);
                l_loop++;
            } else {
                /*sequence error */
                ercd = IMPDRV_EC_NG_SEQSTATE;
                break;
            }
        } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_STDBY);
        /* for VDSP error notice */
        if (0 != (gp_val & IMPDRV_REGVAL_VDSP_ERROR))
        {
            ercd = IMPDRV_EC_NG_DSP_HALT;
        }
    }

    /* Check that DSP is Stand by Mode */
    if (ercd == IMPDRV_EC_OK)
    {
        ercd = impdrv_dspctl_dsp_standby(p_device_io);
    }

    /* Reset all bits */
    if (ercd == IMPDRV_EC_OK)
    {
        ercd = impdrv_dspctl_init_core(p_device_io);
    }
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_foece_standby()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_chk_int_data()
* CD_PD_02_04_6072
* [Covers: UD_PD_UD02_04_0072]
***********************************************************************************************************************/
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_chk_int_data(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_corectl->cbinfo.cbfunc)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_chk_int_data()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_crc_sub
* CD_PD_02_04_xxxx
* [Covers: UD_PD_UD02_04_xxxx]
***********************************************************************************************************************/
#if defined (IMPDRV_DSPCRC_DETECT_ADDDEF)
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_crc_sub(
    const uint32_t              st,
    const uint32_t              sz,
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                crc = 0;

    /** Input parameter confirmation process */
    if (NULL == p_device_io || 0 == st || 0 == sz)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /* Write D_SFT_SCPD_0 */
        ercd = impdrv_osdep_write_reg(
            p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_D_SFT_SCPD_0, 
            IMPDRV_REGVAL_D_SFT_SCPD_0_ON, 
            false, 
            IMPDRV_REGVAL_D_SFT_SCPD_0_ON);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        /* Write D_SFT_SCPD_1 */
        ercd = impdrv_osdep_write_reg(
            p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_D_SFT_SCPD_1, 
            st, 
            false, 
            st);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        /* Write D_SFT_SCPD_2 */
        ercd = impdrv_osdep_write_reg(
            p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_D_SFT_SCPD_2, 
            sz, 
            false, 
            sz);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        crc = impdrv_udefctl_udef_crc(st, sz);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        /* Write D_SFT_SCPD_3 */
        ercd = impdrv_osdep_write_reg(
            p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
            IMPDRV_REGOFS_D_SFT_SCPD_3,
            crc, 
            false, 
            crc);
    }
    /** Set error code in return value */
    return ercd;
}
#else
IMPDRV_STATIC e_impdrv_errorcode_t impdrv_dspctl_crc_sub(
    st_impdrv_device_handle_t   *const p_device_io[IMPDRV_DSP_DEV_NUM_MAX]
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    if (NULL == p_device_io)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /* Clear D_SFT_SCPD_0 */
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_SFT_SCPD_0, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        /* Clear D_SFT_SCPD_1 */
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_SFT_SCPD_1, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        /* Clear D_SFT_SCPD_2 */
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_SFT_SCPD_2, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
    }
    if (IMPDRV_EC_OK == ercd)
    {
        /* Clear D_SFT_SCPD_3 */
        ercd = impdrv_osdep_write_reg(p_device_io[IMPDRV_DSP_VDSP_DEV_MAIN],
                IMPDRV_REGOFS_D_SFT_SCPD_3, IMPDRV_REGVAL_CLR, false, IMPDRV_REGVAL_CLR);
    }

    return ercd;
}
#endif
/***********************************************************************************************************************
* End of function impdrv_dspctl_crc()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
