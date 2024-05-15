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
* Description  : Hardware information table for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "target/common/pma/hw/r_pma_hwdepend.h"

/*======================================================================================================================
Private macr definitios
======================================================================================================================*/
/* Bit assignment of HWA that has dependency */
#define R_PMA_HW_DEP_HWA_BIT_S4_NONE    (uint64_t)(0ULL)  /* No dependency */

/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_s4[R_PMA_HWA_ID_S4_NUM] = {
    /* parent, children */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CSDBGPAP            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CR0                 */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_DDR1_DDR0           */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_I2C5                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_I2C4                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_I2C3                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_I2C2                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_I2C1                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_I2C0                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_HSCIF3              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_HSCIF2              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_HSCIF1              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_HSCIF0              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_RTDM1               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_RTDM0               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_RPC                 */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PCIE10              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PCIE00              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_MSI3                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_MSI2                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_MSI1                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_MSI0                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC7            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC6            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC5            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC4            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC3            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC2            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC1            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC0            */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AES_ACC_WRAPPER     */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SHIP_S0             */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_TMU4                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_TMU3                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_TMU2                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_TMU1                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_TMU0                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SYDM2               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SYDM1               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SDHI0               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SDHI                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SCIF4               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SCIF3               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SCIF1               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SCIF0               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_RTDM3               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_RTDM2               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_UCMT                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_TSC4_TSC3_TSC2_TSC1 */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PFC0                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CMT3                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CMT2                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CMT1                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CMT0                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WDT1_WDT0           */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WCRC3               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WCRC2               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WCRC1               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WCRC0               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SWDT1_SWDT0         */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PCIE11              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PCIE01              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_AULK2P              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_KCRC4               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CRC3                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CRC2                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CRC1                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CRC0                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SCMT                */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PFC0D3              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PFC0D2              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_PFC0D1              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT9_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT8_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT7_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT6_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT5_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT4_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT3_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT2_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT1_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_WWDT0_RCLK          */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_KCRC7               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_KCRC6               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_KCRC5               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CKMMM               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CKMPE0              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CKMRT               */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CKM                 */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CKMHSC              */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_CKMCR52             */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_UFS                 */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SHIP_S2             */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_SHIP_S1             */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE},  /* R_PMA_HWA_ID_S4_ETHER_TSN_IF        */
    { R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE,    R_PMA_HW_DEP_HWA_BIT_S4_NONE}   /* R_PMA_HWA_ID_S4_R_SWITCH2           */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const uint64_t g_pma_hw_dep_hwa_bit_tbl_s4[R_PMA_HWA_ID_S4_NUM] = {
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CSDBGPAP            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CR0                 */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_DDR1_DDR0           */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_I2C5                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_I2C4                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_I2C3                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_I2C2                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_I2C1                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_I2C0                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_HSCIF3              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_HSCIF2              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_HSCIF1              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_HSCIF0              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_RTDM1               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_RTDM0               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_RPC                 */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PCIE10              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PCIE00              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_MSI3                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_MSI2                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_MSI1                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_MSI0                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC7            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC6            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC5            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC4            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC3            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC2            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC1            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC0            */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AES_ACC_WRAPPER     */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SHIP_S0             */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_TMU4                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_TMU3                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_TMU2                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_TMU1                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_TMU0                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SYDM2               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SYDM1               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SDHI0               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SDHI                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SCIF4               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SCIF3               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SCIF1               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SCIF0               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_RTDM3               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_RTDM2               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_UCMT                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_TSC4_TSC3_TSC2_TSC1 */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PFC0                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CMT3                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CMT2                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CMT1                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CMT0                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WDT1_WDT0           */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WCRC3               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WCRC2               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WCRC1               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WCRC0               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SWDT1_SWDT0         */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PCIE11              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PCIE01              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_AULK2P              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_KCRC4               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CRC3                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CRC2                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CRC1                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CRC0                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SCMT                */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PFC0D3              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PFC0D2              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_PFC0D1              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT9_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT8_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT7_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT6_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT5_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT4_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT3_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT2_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT1_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_WWDT0_RCLK          */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_KCRC7               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_KCRC6               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_KCRC5               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CKMMM               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CKMPE0              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CKMRT               */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CKM                 */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CKMHSC              */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_CKMCR52             */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_UFS                 */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SHIP_S2             */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_SHIP_S1             */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE,  /* R_PMA_HWA_ID_S4_ETHER_TSN_IF        */
    R_PMA_HW_DEP_HWA_BIT_S4_NONE   /* R_PMA_HWA_ID_S4_R_SWITCH2           */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
uint64_t g_pma_hw_dep_cpg_ref_tbl_s4[(uint64_t)R_PMA_CPG_DOMAIN_ID_S4_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM] = { 0 };

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
st_hwa_t g_pma_hw_dep_hwa_info_tbl_s4[R_PMA_HWA_ID_S4_NUM] = {
    /* hwa id, sysc_id, cpg_id, reset_id, actual_rst_stat, ToPG, PGtoCG, HPtoCG, ToHP, cpg_dep_flag */
    {
        R_PMA_HWA_ID_S4_CSDBGPAP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CSDBGPAP,
        R_PMA_CPG_DOMAIN_ID_S4_CSDBGPAP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CR0,
        R_PMA_CPG_DOMAIN_ID_S4_CR0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_DDR1_DDR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_DDR1_DDR0,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_I2C5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_I2C5,
        R_PMA_CPG_DOMAIN_ID_S4_I2C5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_I2C4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_I2C4,
        R_PMA_CPG_DOMAIN_ID_S4_I2C4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_I2C3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_I2C3,
        R_PMA_CPG_DOMAIN_ID_S4_I2C3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_I2C2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_I2C2,
        R_PMA_CPG_DOMAIN_ID_S4_I2C2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_I2C1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_I2C1,
        R_PMA_CPG_DOMAIN_ID_S4_I2C1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_I2C0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_I2C0,
        R_PMA_CPG_DOMAIN_ID_S4_I2C0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_HSCIF3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF3,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_HSCIF2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF2,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_HSCIF1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF1,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_HSCIF0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF0,
        R_PMA_CPG_DOMAIN_ID_S4_HSCIF0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_RTDM1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM1,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_RTDM0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM0,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_RPC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_RPC,
        R_PMA_CPG_DOMAIN_ID_S4_RPC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PCIE10,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PCIE10,
        R_PMA_CPG_DOMAIN_ID_S4_PCIE10,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PCIE00,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PCIE00,
        R_PMA_CPG_DOMAIN_ID_S4_PCIE00,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_MSI3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_MSI3,
        R_PMA_CPG_DOMAIN_ID_S4_MSI3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_MSI2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_MSI2,
        R_PMA_CPG_DOMAIN_ID_S4_MSI2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_MSI1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_MSI1,
        R_PMA_CPG_DOMAIN_ID_S4_MSI1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_MSI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_MSI0,
        R_PMA_CPG_DOMAIN_ID_S4_MSI0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC7,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC7,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC7,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC6,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC6,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC6,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC5,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC4,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC3,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC2,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC1,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC0,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AES_ACC_WRAPPER,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC_WRAPPER,
        R_PMA_CPG_DOMAIN_ID_S4_AES_ACC_WRAPPER,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SHIP_S0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SHIP_S0,
        R_PMA_CPG_DOMAIN_ID_S4_SHIP_S0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_TMU4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_TMU4,
        R_PMA_CPG_DOMAIN_ID_S4_TMU4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_TMU3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_TMU3,
        R_PMA_CPG_DOMAIN_ID_S4_TMU3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_TMU2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_TMU2,
        R_PMA_CPG_DOMAIN_ID_S4_TMU2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_TMU1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_TMU1,
        R_PMA_CPG_DOMAIN_ID_S4_TMU1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_TMU0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_TMU0,
        R_PMA_CPG_DOMAIN_ID_S4_TMU0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SYDM2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SYDM2,
        R_PMA_CPG_DOMAIN_ID_S4_SYDM2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SYDM1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SYDM1,
        R_PMA_CPG_DOMAIN_ID_S4_SYDM1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SDHI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SDHI0,
        R_PMA_CPG_DOMAIN_ID_S4_SDHI0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SDHI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SDHI,
        R_PMA_CPG_DOMAIN_ID_S4_SDHI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SCIF4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF4,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SCIF3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF3,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SCIF1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF1,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SCIF0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF0,
        R_PMA_CPG_DOMAIN_ID_S4_SCIF0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_RTDM3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM3,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_RTDM2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM2,
        R_PMA_CPG_DOMAIN_ID_S4_RTDM2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_UCMT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_UCMT,
        R_PMA_CPG_DOMAIN_ID_S4_UCMT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_TSC4_TSC3_TSC2_TSC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_TSC4_TSC3_TSC2_TSC1,
        R_PMA_CPG_DOMAIN_ID_S4_TSC4_TSC3_TSC2_TSC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PFC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PFC0,
        R_PMA_CPG_DOMAIN_ID_S4_PFC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CMT3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CMT3,
        R_PMA_CPG_DOMAIN_ID_S4_CMT3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CMT2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CMT2,
        R_PMA_CPG_DOMAIN_ID_S4_CMT2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CMT1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CMT1,
        R_PMA_CPG_DOMAIN_ID_S4_CMT1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CMT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CMT0,
        R_PMA_CPG_DOMAIN_ID_S4_CMT0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WDT1_WDT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WDT1_WDT0,
        R_PMA_CPG_DOMAIN_ID_S4_WDT1_WDT0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WCRC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC3,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WCRC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC2,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WCRC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC1,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WCRC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC0,
        R_PMA_CPG_DOMAIN_ID_S4_WCRC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SWDT1_SWDT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SWDT1_SWDT0,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PCIE11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PCIE11,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PCIE01,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PCIE01,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_AULK2P,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_AULK2P,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_KCRC4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC4,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CRC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CRC3,
        R_PMA_CPG_DOMAIN_ID_S4_CRC3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CRC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CRC2,
        R_PMA_CPG_DOMAIN_ID_S4_CRC2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CRC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CRC1,
        R_PMA_CPG_DOMAIN_ID_S4_CRC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CRC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CRC0,
        R_PMA_CPG_DOMAIN_ID_S4_CRC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SCMT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SCMT,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PFC0D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PFC0D3,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PFC0D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PFC0D2,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_PFC0D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_PFC0D1,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT9_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT9_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT8_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT8_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT7_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT7_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT6_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT6_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT5_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT5_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT4_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT4_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT3_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT3_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT2_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT2_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT1_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT1_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_WWDT0_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_WWDT0_RCLK,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_KCRC7,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC7,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC7,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_KCRC6,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC6,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC6,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_KCRC5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC5,
        R_PMA_CPG_DOMAIN_ID_S4_KCRC5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CKMMM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CKMMM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CKMPE0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CKMPE0,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CKMRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CKMRT,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CKM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CKM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CKMHSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CKMHSC,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_CKMCR52,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_CKMCR52,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_UFS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_UFS,
        R_PMA_CPG_DOMAIN_ID_S4_UFS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SHIP_S2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SHIP_S2,
        R_PMA_CPG_DOMAIN_ID_S4_SHIP_S2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_SHIP_S1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_SHIP_S1,
        R_PMA_CPG_DOMAIN_ID_S4_SHIP_S1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_ETHER_TSN_IF,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_ETHER_TSN_IF,
        R_PMA_CPG_DOMAIN_ID_S4_ETHER_TSN_IF,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_S4_R_SWITCH2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_S4_R_SWITCH2,
        R_PMA_CPG_DOMAIN_ID_S4_R_SWITCH2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    }
};
