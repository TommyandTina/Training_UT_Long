/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      1.0.0
* Description  : BSP Wrapper information table for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <string.h>
#include <unistd.h>

#include <hw/rcar_cpg.h>
#include <hw/rcar_sysc.h>

#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"

/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
/* PRQA S 1531,1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const int g_pma_bsp_wrap_sysc_id_tbl_v3h1[] = 
{
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2CN0,        /* R_PMA_SYSC_DOMAIN_ID_A2CN   */ /*  A2CN0  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2DP0,        /* R_PMA_SYSC_DOMAIN_ID_A2DP0  */ /*  A2DP0  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2DP1,        /* R_PMA_SYSC_DOMAIN_ID_A2DP1  */ /*  A2DP1  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2SC0,        /* R_PMA_SYSC_DOMAIN_ID_A2SC0  */ /*  A2SC0  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2SC1,        /* R_PMA_SYSC_DOMAIN_ID_A2SC1  */ /*  A2SC1  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2SC2,        /* R_PMA_SYSC_DOMAIN_ID_A2SC2  */ /*  A2SC2  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2SC3,        /* R_PMA_SYSC_DOMAIN_ID_A2SC3  */ /*  A2SC3  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2SC4,        /* R_PMA_SYSC_DOMAIN_ID_A2SC4  */ /*  A2SC4  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR0,        /* R_PMA_SYSC_DOMAIN_ID_A2IR0  */ /*  A2IR0  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR1,        /* R_PMA_SYSC_DOMAIN_ID_A2IR1  */ /*  A2IR1  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR2,        /* R_PMA_SYSC_DOMAIN_ID_A2IR2  */ /*  A2IR2  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR3,        /* R_PMA_SYSC_DOMAIN_ID_A2IR3  */ /*  A2IR3  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR4,        /* R_PMA_SYSC_DOMAIN_ID_A2IR4  */ /*  A2IR4  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR5,        /* R_PMA_SYSC_DOMAIN_ID_A2IR5  */ /*  A2IR5  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A3IR,         /* R_PMA_SYSC_DOMAIN_ID_A3IR   */ /*  A3IR   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A3VIP0,       /* R_PMA_SYSC_DOMAIN_ID_A3VIP0 */ /*  A3VIP0 */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A3VIP1,       /* R_PMA_SYSC_DOMAIN_ID_A3VIP1 */ /*  A3VIP1 */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A3VIP2        /* R_PMA_SYSC_DOMAIN_ID_A3VIP2 */ /*  A3VIP2 */
};

/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
/* PRQA S 1531,1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const int g_pma_bsp_wrap_cpg_id_tbl_v3h1[] =
{
	/* PRQA S 4542,4543,0499 ++ # These are used according to the QNX specification. */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP0,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP0       *//*  IMP0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP1,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP1       *//*  IMP1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP2,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP2       *//*  IMP2 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP3,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP3       *//*  IMP3 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP,          /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP4       *//*  IMP4 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_SIMP,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_SIMP       *//*  SIMP */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPOCV0,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_OCV0   *//*  IMP-OCV0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPOCV1,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_OCV1   *//*  IMP-OCV1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPOCV2,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_OCV2   *//*  IMP-OCV2 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPOCV3,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_OCV3   *//*  IMP-OCV3 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPOCV4,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_OCV4   *//*  IMP-OCV4 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPDMA0,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_DMA0   *//*  IMPDMA0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPDMA1,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_DMA1   *//*  IMPDMA1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPPSC0,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_PSC0    *//*  IMPPSC */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPSlimDMAC,  /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_PSC1   *//*  Slim-IMPDMA */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPCNN,       /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_CNN    *//*  IMPCNN */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPRAM,       /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMP_RAM    *//*  IMPRAM */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_DOF,          /*  R_PMA_CPG_DOMAIN_ID_V3H1_VIP_DOF    *//*  DOF */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_STV,          /*  R_PMA_CPG_DOMAIN_ID_V3H1_VIP_STV    *//*  STV */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ACF0,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_VIP_ACF0   *//*  ACF0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ACF1,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_VIP_ACF1   *//*  ACF1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ACF2,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_VIP_ACF2   *//*  ACF2 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ACF3,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_VIP_ACF3   *//*  ACF3 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ACF4,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_VIP_ACF4   *//*  ACF4 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_iVCP1E,       /*  R_PMA_CPG_DOMAIN_ID_V3H1_VCP_IVCP1E *//*  iVCP1E */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ISP0,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_ISP_CH0    *//*  ISP0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ISP1,         /*  R_PMA_CPG_DOMAIN_ID_V3H1_ISP_CH1    *//*  ISP1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX40,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMR_LX4_CH0*//*  IMR-LX4 0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX41,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMR_LX4_CH1*//*  IMR-LX4 1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX42,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMR_LX4_CH2*//*  IMR-LX4 2 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX43,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMR_LX4_CH3*//*  IMR-LX4 3 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX44,      /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMR_LX4_CH4*//*  IMR-LX4 4 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX45       /*  R_PMA_CPG_DOMAIN_ID_V3H1_IMR_LX4_CH5*//*  IMR-LX4 5 */
};
