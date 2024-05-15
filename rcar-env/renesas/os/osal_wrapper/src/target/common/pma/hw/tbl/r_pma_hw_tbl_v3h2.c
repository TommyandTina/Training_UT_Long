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
#define R_PMA_HW_DEP_HWA_BIT_V3H2_NONE           (uint64_t)(0ULL)           /* No dependency */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE0      (uint64_t)(1ULL << 0ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_CORE0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE1      (uint64_t)(1ULL << 1ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_CORE1 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE2      (uint64_t)(1ULL << 2ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_CORE2 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE3      (uint64_t)(1ULL << 3ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_CORE3 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE4      (uint64_t)(1ULL << 4ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_CORE4 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_CORE      (uint64_t)(1ULL << 5ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_SIMP_CORE */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE0  (uint64_t)(1ULL << 6ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE1  (uint64_t)(1ULL << 7ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE2  (uint64_t)(1ULL << 8ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE3  (uint64_t)(1ULL << 9ULL)   /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE4  (uint64_t)(1ULL << 10ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC0      (uint64_t)(1ULL << 11ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_DMAC0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC1      (uint64_t)(1ULL << 12ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_DMAC1 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_PSC        (uint64_t)(1ULL << 13ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_PSC */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_DMA       (uint64_t)(1ULL << 14ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_SIMP_DMA */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CNN        (uint64_t)(1ULL << 15ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_CNN */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP        (uint64_t)(1ULL << 16ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_TOP */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_DOF        (uint64_t)(1ULL << 17ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_VIP_DOF */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_STV        (uint64_t)(1ULL << 18ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_VIP_STV */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF0       (uint64_t)(1ULL << 19ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_VIP_ACF0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF1       (uint64_t)(1ULL << 20ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_VIP_ACF1 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF2       (uint64_t)(1ULL << 21ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_VIP_ACF2 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF3       (uint64_t)(1ULL << 22ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_VIP_ACF3 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF4       (uint64_t)(1ULL << 23ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_VIP_ACF4 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI       (uint64_t)(1ULL << 24ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_IMP_SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_CISP0          (uint64_t)(1ULL << 25ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_CISP0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_TISP0          (uint64_t)(1ULL << 26ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_TISP0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_CISP1          (uint64_t)(1ULL << 27ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_CISP1 */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_TISP1          (uint64_t)(1ULL << 28ULL)  /* Bit assignment of R_PMA_HWA_ID_V3H2_TISP1 */

/* Bit assignment of IMP_RAM's children */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM_CHILDREN  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE0     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE1     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE2     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE3     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE4     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_CORE     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE0 | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE1 | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE2 | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE3 | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE4 | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC0     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC1     | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_PSC       | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_DMA      | \
                                                               R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CNN)

/* Bit assignment of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP     | \
                                                      R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI) 

/* PD0 macro */
/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A2DP0_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC0 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_PSC)

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A2DP1_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC1 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_DMA)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_PSC_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC1 | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_PSC)

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI)

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF0 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF1 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF2 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF3 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF4)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_CISP0 | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_TISP0)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_CISP1 | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_TISP1)

/* PD1 macro */
/* Share SYSC domain of A2IR0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE0 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE1 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE2 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE3 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE4)

/* Share SYSC domain of A2SC0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2SC0_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE0 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE1 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE2 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE3 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE4)

/* Share SYSC domain of A2SC0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2DP0_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC0 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC1 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_PSC   | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_DMA)

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF0 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF1 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF2 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF3 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF4)

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_CISP0 | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_TISP0)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_CISP1 | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_TISP1)

/* PD3 macro */
/* Share SYSC domain of A2IR0 */
#define R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE0     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE1     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE2     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE3     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE4     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_CORE     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE0 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE1 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE2 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE3 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE4 | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC0     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC1     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_PSC       | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_DMA      | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CNN)      

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF0 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF1 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF2 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF3 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF4)

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP | \
                                                             R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_CISP0 | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_TISP0)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3H2_CISP1 | \
                                                                R_PMA_HW_DEP_HWA_BIT_V3H2_TISP1)


/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd0[R_PMA_HWA_ID_V3H2_NUM] = {
    /* parent, children */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE0       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE1       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE2       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE3       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE4       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_SIMP_CORE       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A2DP0_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_DMAC0       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A2DP1_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_PSC_PD0},  /* R_PMA_HWA_ID_V3H2_IMP_DMAC1       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A2DP0_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_PSC_PD0},  /* R_PMA_HWA_ID_V3H2_IMP_PSC         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A2DP1_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_SIMP_DMA        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CNN         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD0,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD0},  /* R_PMA_HWA_ID_V3H2_IMP_TOP         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_DOF         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_STV         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF0        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF1        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF2        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF3        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD0,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF4        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VCP_IVCP1E      */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD0, R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD0},  /* R_PMA_HWA_ID_V3H2_CISP0           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD0},  /* R_PMA_HWA_ID_V3H2_CISP1           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH0     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH1     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH2     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH3     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH4     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH5     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD0,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD0},  /* R_PMA_HWA_ID_V3H2_IMP_SPMI        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD0, R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD0},  /* R_PMA_HWA_ID_V3H2_TISP0           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,              R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD0}   /* R_PMA_HWA_ID_V3H2_TISP1           */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd1[R_PMA_HWA_ID_V3H2_NUM] = {
    /* parent, children */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE0       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE1       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE2       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE3       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE4       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_SIMP_CORE       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2SC0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2SC0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2SC0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2SC0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2SC0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2DP0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_DMAC0       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2DP0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_DMAC1       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2DP0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_PSC         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2DP0_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_SIMP_DMA        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CNN         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD1},  /* R_PMA_HWA_ID_V3H2_IMP_TOP         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_DOF         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_STV         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD1,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF0        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD1,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF1        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD1,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF2        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD1,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF3        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD1,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF4        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VCP_IVCP1E      */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD1},  /* R_PMA_HWA_ID_V3H2_CISP0           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD1},  /* R_PMA_HWA_ID_V3H2_CISP1           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH0     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH1     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH2     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH3     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH4     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH5     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD1,   R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD1},  /* R_PMA_HWA_ID_V3H2_IMP_SPMI        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD1},  /* R_PMA_HWA_ID_V3H2_TISP0           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD1}   /* R_PMA_HWA_ID_V3H2_TISP1           */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd3[R_PMA_HWA_ID_V3H2_NUM] = {
    /* parent, children */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE0       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE1       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE2       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE3       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CORE4       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_SIMP_CORE       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4   */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_DMAC0       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_DMAC1       */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_PSC         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_SIMP_DMA        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A2IR0_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMP_CNN         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD3},  /* R_PMA_HWA_ID_V3H2_IMP_TOP         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_DOF         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_STV         */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD3,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF0        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD3,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF1        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD3,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF2        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD3,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF3        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H_SHARE_A3VIP2_PD3,  R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VIP_ACF4        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_VCP_IVCP1E      */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD3},  /* R_PMA_HWA_ID_V3H2_CISP0           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD3},  /* R_PMA_HWA_ID_V3H2_CISP1           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH0     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH1     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH2     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH3     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH4     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_NONE},               /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH5     */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_A3IR_PD3,   R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_IMP_RAM_PD3},  /* R_PMA_HWA_ID_V3H2_IMP_SPMI        */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH0_PD3},  /* R_PMA_HWA_ID_V3H2_TISP0           */
    { R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             R_PMA_HW_DEP_HWA_BIT_V3H2_SHARE_ISP_CH1_PD3}   /* R_PMA_HWA_ID_V3H2_TISP1           */
};


/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const uint64_t g_pma_hw_dep_hwa_bit_tbl_v3h2[R_PMA_HWA_ID_V3H2_NUM] = {
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE0,        /* R_PMA_HWA_ID_V3H2_IMP_CORE0       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE1,        /* R_PMA_HWA_ID_V3H2_IMP_CORE1       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE2,        /* R_PMA_HWA_ID_V3H2_IMP_CORE2       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE3,        /* R_PMA_HWA_ID_V3H2_IMP_CORE3       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CORE4,        /* R_PMA_HWA_ID_V3H2_IMP_CORE4       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_CORE,        /* R_PMA_HWA_ID_V3H2_SIMP_CORE       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE0,    /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0   */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE1,    /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1   */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE2,    /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2   */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE3,    /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3   */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_OCV_CORE4,    /* R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4   */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC0,        /* R_PMA_HWA_ID_V3H2_IMP_DMAC0       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_DMAC1,        /* R_PMA_HWA_ID_V3H2_IMP_DMAC1       */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_PSC,          /* R_PMA_HWA_ID_V3H2_IMP_PSC         */
    R_PMA_HW_DEP_HWA_BIT_V3H2_SIMP_DMA,         /* R_PMA_HWA_ID_V3H2_SIMP_DMA        */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_CNN,          /* R_PMA_HWA_ID_V3H2_IMP_CNN         */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_TOP,          /* R_PMA_HWA_ID_V3H2_IMP_TOP         */
    R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_DOF,          /* R_PMA_HWA_ID_V3H2_VIP_DOF         */
    R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_STV,          /* R_PMA_HWA_ID_V3H2_VIP_STV         */
    R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF0,         /* R_PMA_HWA_ID_V3H2_VIP_ACF0        */
    R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF1,         /* R_PMA_HWA_ID_V3H2_VIP_ACF1        */
    R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF2,         /* R_PMA_HWA_ID_V3H2_VIP_ACF2        */
    R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF3,         /* R_PMA_HWA_ID_V3H2_VIP_ACF3        */
    R_PMA_HW_DEP_HWA_BIT_V3H2_VIP_ACF4,         /* R_PMA_HWA_ID_V3H2_VIP_ACF4        */
    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             /* R_PMA_HWA_ID_V3H2_VCP_IVCP1E      */
    R_PMA_HW_DEP_HWA_BIT_V3H2_CISP0,            /* R_PMA_HWA_ID_V3H2_CISP0           */
    R_PMA_HW_DEP_HWA_BIT_V3H2_CISP1,            /* R_PMA_HWA_ID_V3H2_CISP1           */
    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH0     */
    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH1     */
    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH2     */
    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH3     */
    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH4     */
    R_PMA_HW_DEP_HWA_BIT_V3H2_NONE,             /* R_PMA_HWA_ID_V3H2_IMR_LX4_CH5     */
    R_PMA_HW_DEP_HWA_BIT_V3H2_IMP_SPMI,         /* R_PMA_HWA_ID_V3H2_IMP_SPMI        */
    R_PMA_HW_DEP_HWA_BIT_V3H2_TISP0,            /* R_PMA_HWA_ID_V3H2_TISP0           */
    R_PMA_HW_DEP_HWA_BIT_V3H2_TISP1             /* R_PMA_HWA_ID_V3H2_TISP1           */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v3h2[R_PMA_HW_DEP_HWA_DEPEND_V3H2_NUM] = {
    R_PMA_HWA_ID_V3H2_IMP_CORE0,
    R_PMA_HWA_ID_V3H2_IMP_CORE1,
    R_PMA_HWA_ID_V3H2_IMP_CORE2,
    R_PMA_HWA_ID_V3H2_IMP_CORE3,
    R_PMA_HWA_ID_V3H2_IMP_CORE4,
    R_PMA_HWA_ID_V3H2_SIMP_CORE,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4,
    R_PMA_HWA_ID_V3H2_IMP_DMAC0,
    R_PMA_HWA_ID_V3H2_IMP_DMAC1,
    R_PMA_HWA_ID_V3H2_IMP_PSC,
    R_PMA_HWA_ID_V3H2_SIMP_DMA,
    R_PMA_HWA_ID_V3H2_IMP_CNN,
    R_PMA_HWA_ID_V3H2_IMP_TOP,
    R_PMA_HWA_ID_V3H2_VIP_DOF,
    R_PMA_HWA_ID_V3H2_VIP_STV,
    R_PMA_HWA_ID_V3H2_VIP_ACF0,
    R_PMA_HWA_ID_V3H2_VIP_ACF1,
    R_PMA_HWA_ID_V3H2_VIP_ACF2,
    R_PMA_HWA_ID_V3H2_VIP_ACF3,
    R_PMA_HWA_ID_V3H2_VIP_ACF4,
    R_PMA_HWA_ID_V3H2_IMP_SPMI,
    R_PMA_HWA_ID_V3H2_CISP0,
    R_PMA_HWA_ID_V3H2_TISP0,
    R_PMA_HWA_ID_V3H2_CISP1,
    R_PMA_HWA_ID_V3H2_TISP1
};

/* PRQA S 1533,3408 2 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
uint64_t g_pma_hw_dep_sysc_ref_tbl_v3h2[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V3H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM] = { 0 };
uint64_t g_pma_hw_dep_cpg_ref_tbl_v3h2[(uint64_t)R_PMA_CPG_DOMAIN_ID_V3H2_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM]  = { 0 };

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3h2_pd0[R_PMA_HWA_ID_V3H2_NUM] = {
    /* hwa id, sysc_id, cpg_id, reset_id, actual_rst_stat, ToPG, PGtoCG, HPtoCG, ToHP, cpg_dep_flag */
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_SIMP_CORE,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR5,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_DMAC0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_DMAC1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_PSC,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_SIMP_DMA,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP_DMA,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP_DMA,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CNN,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2CN,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_CNN,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_CNN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_TOP,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_DOF,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_DOF,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_DOF,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_STV,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_STV,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_STV,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF0,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF1,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF3,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF4,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VCP_IVCP1E,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_VCP_IVCP1E,
        R_PMA_CPG_DOMAIN_ID_V3H2_VCP_IVCP1E,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_CISP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_CISP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH5,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_SPMI,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_TISP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_TISP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    }
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3h2_pd1[R_PMA_HWA_ID_V3H2_NUM] = {
    /* hwa id, sysc_id, cpg_id, reset_id, actual_rst_stat, ToPG, PGtoCG, HPtoCG, ToHP, cpg_dep_flag */
    {
          R_PMA_HWA_ID_V3H2_IMP_CORE0,
          R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
          R_PMA_CPG_DOMAIN_ID_V3H2_IMP0,
          R_PMA_CPG_DOMAIN_ID_V3H2_IMP0,
          R_PMA_UNDEFINED_RESET,
          &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
          &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
          &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
          &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
          R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_SIMP_CORE,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR5,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2SC0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_DMAC0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_DMAC1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_PSC,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_SIMP_DMA,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2DP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP_DMA,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP_DMA,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CNN,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2CN,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_CNN,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_CNN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_TOP,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_DOF,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_DOF,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_DOF,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_STV,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_STV,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_STV,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF0,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF1,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF3,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF4,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VCP_IVCP1E,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_VCP_IVCP1E,
        R_PMA_CPG_DOMAIN_ID_V3H2_VCP_IVCP1E,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_CISP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_CISP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH5,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_SPMI,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_TISP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_TISP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    }
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3h2_pd3[R_PMA_HWA_ID_V3H2_NUM] = {
    /* hwa id, sysc_id, cpg_id, reset_id, actual_rst_stat, ToPG, PGtoCG, HPtoCG, ToHP, cpg_dep_flag */
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CORE4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_SIMP_CORE,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_OCV4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_DMAC0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_DMAC1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_DMA1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_PSC,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_PSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_SIMP_DMA,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP_DMA,
        R_PMA_CPG_DOMAIN_ID_V3H2_SIMP_DMA,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_CNN,
        R_PMA_SYSC_DOMAIN_ID_V3H_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_CNN,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_CNN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_TOP,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_DOF,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP1,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_DOF,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_DOF,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_STV,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_STV,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_STV,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF0,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF0,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF1,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF1,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF2,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF3,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF3,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VIP_ACF4,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF4,
        R_PMA_CPG_DOMAIN_ID_V3H2_VIP_ACF4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_VCP_IVCP1E,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_VCP_IVCP1E,
        R_PMA_CPG_DOMAIN_ID_V3H2_VCP_IVCP1E,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_CISP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_CISP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH2,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH3,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH4,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMR_LX4_CH5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH5,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMR_LX4_CH5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_IMP_SPMI,
        R_PMA_SYSC_DOMAIN_ID_V3H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3H2_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_TISP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3H2_TISP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_CPG_DOMAIN_ID_V3H2_ISP_CH1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    }
};
