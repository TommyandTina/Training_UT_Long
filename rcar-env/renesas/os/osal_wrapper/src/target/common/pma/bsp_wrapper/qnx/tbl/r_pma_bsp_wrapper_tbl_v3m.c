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
const int g_pma_bsp_wrap_sysc_id_tbl_v3m[] =
{
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR0,        /* R_PMA_SYSC_DOMAIN_ID_A2IR0*/   /*  A2IR0  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2IR1,        /* R_PMA_SYSC_DOMAIN_ID_A2IR1*/   /*  A2IR1  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2SC0,        /* R_PMA_SYSC_DOMAIN_ID_A2SC0*/   /*  A2SC0  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2SC1,        /* R_PMA_SYSC_DOMAIN_ID_A2SC1*/   /*  A2SC1  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2DP,         /* R_PMA_SYSC_DOMAIN_ID_A2DP0*/   /*  A2DP0  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A2CN0,        /* R_PMA_SYSC_DOMAIN_ID_A2CN */   /*  A2CN   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN3_A3IR,         /* R_PMA_SYSC_DOMAIN_ID_A3IR */   /*  A3IR   */
};

/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
/* PRQA S 1531,1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const int g_pma_bsp_wrap_cpg_id_tbl_v3m[] =
{
    /* PRQA S 4542,4543,0499 ++ # These are used according to the QNX specification. */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP0,         /*  R_PMA_CPG_DOMAIN_ID_IMP01       *//*  IMP0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP1,         /*  R_PMA_CPG_DOMAIN_ID_IMP23       *//*  IMP1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPOCV0,      /*  R_PMA_CPG_DOMAIN_ID_IMP_OCV0    *//*  IMP-OCV0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPOCV1,      /*  R_PMA_CPG_DOMAIN_ID_IMP_OCV1    *//*  IMP-OCV1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP2,         /*  R_PMA_CPG_DOMAIN_ID_IMP_DP      *//*  IMP-DP */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMP3,         /*  R_PMA_CPG_DOMAIN_ID_IMP_CNN     *//*  IMPCNN */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMPRAM,       /*  R_PMA_CPG_DOMAIN_ID_IMP_RAM     *//*  IMPRAM */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_iVCP1E,       /*  R_PMA_CPG_DOMAIN_ID_VCP_IVCP1E  *//*  iVCP1E */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_ISP0,         /*  R_PMA_CPG_DOMAIN_ID_ISP         *//*  ISP0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX40,      /*  R_PMA_CPG_DOMAIN_ID_IMR0        *//*  IMR0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX41,      /*  R_PMA_CPG_DOMAIN_ID_IMR1        *//*  IMR1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX42,      /*  R_PMA_CPG_DOMAIN_ID_IMR2        *//*  IMR2 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN3_IMRLX43       /*  R_PMA_CPG_DOMAIN_ID_IMR3        *//*  IMR3 */
};
