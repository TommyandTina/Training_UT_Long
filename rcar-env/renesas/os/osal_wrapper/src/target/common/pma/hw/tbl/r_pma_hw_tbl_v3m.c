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
#define R_PMA_HW_DEP_HWA_BIT_V3M_NONE        (uint64_t)(0ULL)            /* No dependency */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE0   (uint64_t)(1ULL << 0ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_CORE0 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE2   (uint64_t)(1ULL << 1ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_CORE2 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_OCV_CORE01  (uint64_t)(1ULL << 2ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_OCV_CORE01 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_OCV_CORE23  (uint64_t)(1ULL << 3ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_OCV_CORE23 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_DMA     (uint64_t)(1ULL << 4ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_DMA */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CNN     (uint64_t)(1ULL << 5ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_CNN */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_TOP     (uint64_t)(1ULL << 6ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_TOP */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_SPMI    (uint64_t)(1ULL << 7ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE1   (uint64_t)(1ULL << 8ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_CORE1 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE3   (uint64_t)(1ULL << 9ULL)    /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_CORE3 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_PSC     (uint64_t)(1ULL << 10ULL)   /* Bit assignment of R_PMA_HWA_ID_V3M_IMP_PSC */
#define R_PMA_HW_DEP_HWA_BIT_V3M_CISP        (uint64_t)(1ULL << 11ULL)   /* Bit assignment of R_PMA_HWA_ID_V3M_CISP */
#define R_PMA_HW_DEP_HWA_BIT_V3M_TISP        (uint64_t)(1ULL << 12ULL)   /* Bit assignment of R_PMA_HWA_ID_V3M_TISP */

/* Bit assignment of IMP_RAM's children */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM_CHILDREN  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE0  | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE2  | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_OCV_CORE01 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_OCV_CORE23 | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_IMP_DMA    | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CNN    | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE1  | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE3  | \
                                                              R_PMA_HW_DEP_HWA_BIT_V3M_IMP_PSC)

/* Bit assignment of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_TOP     | \
                                                     R_PMA_HW_DEP_HWA_BIT_V3M_IMP_SPMI) 
/* Share SYSC domain of A2IR0 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2IR0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE1 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE0)

/* Share CPG domain of IMP01 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP01  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE1 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE0)
/* Share SYSC domain of A2IR1 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2IR1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE2 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE3)

/* Share CPG domain of IMP23 */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP23  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE2 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE3)
                                                          
/* Share SYSC domain of A2DP */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2DP  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_DMA | \
                                                          R_PMA_HW_DEP_HWA_BIT_V3M_IMP_PSC)

/* Share CPG domain of IMP_DP */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP_DP  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_DMA | \
                                                          R_PMA_HW_DEP_HWA_BIT_V3M_IMP_PSC)

/* Share SYSC domain of A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A3IR  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_TOP | \
                                                          R_PMA_HW_DEP_HWA_BIT_V3M_IMP_SPMI)

/* Share CPG domain of IMP_RAM */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP_RAM  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_IMP_TOP | \
                                                           R_PMA_HW_DEP_HWA_BIT_V3M_IMP_SPMI)
                                                          
/* Share CPG domain of ISP */
#define R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_ISP  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V3M_CISP | \
                                                       R_PMA_HW_DEP_HWA_BIT_V3M_TISP)


/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3m[R_PMA_HWA_ID_V3M_NUM] = {
    /* parent, children */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2IR0,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP01},   /* R_PMA_HWA_ID_V3M_IMP_CORE0     */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2IR1,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP23},   /* R_PMA_HWA_ID_V3M_IMP_CORE2     */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_IMP_OCV_CORE0 */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_IMP_OCV_CORE1 */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2DP,     R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP_DP},  /* R_PMA_HWA_ID_V3M_IMP_DMA       */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_IMP_CNN       */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A3IR,     R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP_RAM}, /* R_PMA_HWA_ID_V3M_IMP_TOP       */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_VCP_IVCP1E    */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_ISP},     /* R_PMA_HWA_ID_V3M_CISP          */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_IMR0          */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_IMR1          */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_IMR2          */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_NONE},          /* R_PMA_HWA_ID_V3M_IMR3          */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM_CHILDREN ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A3IR,     R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP_RAM}, /* R_PMA_HWA_ID_V3M_IMP_SPMI      */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2IR0,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP01},   /* R_PMA_HWA_ID_V3M_IMP_CORE1     */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2IR1,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP23},   /* R_PMA_HWA_ID_V3M_IMP_CORE3     */
    { R_PMA_HW_DEP_HWA_BIT_V3M_IMP_RAM, R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_A2DP,     R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_IMP_DP},  /* R_PMA_HWA_ID_V3M_IMP_PSC       */
    { R_PMA_HW_DEP_HWA_BIT_V3M_NONE,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE             ,    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,           R_PMA_HW_DEP_HWA_BIT_V3M_SHARE_ISP}      /* R_PMA_HWA_ID_V3M_TISP          */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const uint64_t g_pma_hw_dep_hwa_bit_tbl_v3m[R_PMA_HWA_ID_V3M_NUM] = {
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE0,   /* R_PMA_HWA_ID_V3M_IMP_CORE0     */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE2,   /* R_PMA_HWA_ID_V3M_IMP_CORE2     */
    R_PMA_HW_DEP_HWA_BIT_V3M_OCV_CORE01,  /* R_PMA_HWA_ID_V3M_IMP_OCV_CORE0 */
    R_PMA_HW_DEP_HWA_BIT_V3M_OCV_CORE23,  /* R_PMA_HWA_ID_V3M_IMP_OCV_CORE1 */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_DMA,     /* R_PMA_HWA_ID_V3M_IMP_DMA       */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CNN,     /* R_PMA_HWA_ID_V3M_IMP_CNN       */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_TOP,     /* R_PMA_HWA_ID_V3M_IMP_TOP       */
    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,        /* R_PMA_HWA_ID_V3M_VCP_IVCP1E    */
    R_PMA_HW_DEP_HWA_BIT_V3M_CISP,        /* R_PMA_HWA_ID_V3M_CISP          */
    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,        /* R_PMA_HWA_ID_V3M_IMR0          */
    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,        /* R_PMA_HWA_ID_V3M_IMR1          */
    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,        /* R_PMA_HWA_ID_V3M_IMR2          */
    R_PMA_HW_DEP_HWA_BIT_V3M_NONE,        /* R_PMA_HWA_ID_V3M_IMR3          */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_SPMI,    /* R_PMA_HWA_ID_V3M_IMP_SPMI      */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE1,   /* R_PMA_HWA_ID_V3M_IMP_CORE0     */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_CORE3,   /* R_PMA_HWA_ID_V3M_IMP_CORE2     */
    R_PMA_HW_DEP_HWA_BIT_V3M_IMP_PSC,     /* R_PMA_HWA_ID_V3M_IMP_PSC       */
    R_PMA_HW_DEP_HWA_BIT_V3M_TISP         /* R_PMA_HWA_ID_V3M_TISP          */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v3m[R_PMA_HW_DEP_HWA_DEPEND_V3M_NUM] = {
    R_PMA_HWA_ID_V3M_IMP_CORE0,
    R_PMA_HWA_ID_V3M_IMP_CORE2,
    R_PMA_HWA_ID_V3M_IMP_OCV_CORE0,
    R_PMA_HWA_ID_V3M_IMP_OCV_CORE1,
    R_PMA_HWA_ID_V3M_IMP_DMA,
    R_PMA_HWA_ID_V3M_IMP_CNN,
    R_PMA_HWA_ID_V3M_IMP_TOP,
    R_PMA_HWA_ID_V3M_IMP_SPMI,
    R_PMA_HWA_ID_V3M_IMP_CORE1,
    R_PMA_HWA_ID_V3M_IMP_CORE3,
    R_PMA_HWA_ID_V3M_IMP_PSC,
    R_PMA_HWA_ID_V3M_CISP,
    R_PMA_HWA_ID_V3M_TISP
};  

/* PRQA S 1533,3408 2 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
uint64_t g_pma_hw_dep_sysc_ref_tbl_v3m[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V3M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM] = { 0 };
uint64_t g_pma_hw_dep_cpg_ref_tbl_v3m[(uint64_t)R_PMA_CPG_DOMAIN_ID_V3M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM]   = { 0 };

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3m[R_PMA_HWA_ID_V3M_NUM] = {
    /* hwa id, sysc_id, cpg_id, reset_id, actual_rst_stat, ToPG, PGtoCG, HPtoCG, ToHP, cpg_dep_flag */
    {
        R_PMA_HWA_ID_V3M_IMP_CORE0,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP01,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP01,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_CORE2,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2IR1,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP23,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP23,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_OCV_CORE0,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2SC0,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_OCV0,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_OCV0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_OCV_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2SC1,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_OCV1,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_OCV1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_DMA,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2DP,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_DP,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_DP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_CNN,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2CN,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_CNN,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_CNN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_TOP,
        R_PMA_SYSC_DOMAIN_ID_V3M_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_VCP_IVCP1E,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3M_VCP_IVCP1E,
        R_PMA_CPG_DOMAIN_ID_V3M_VCP_IVCP1E,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_CISP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3M_ISP,
        R_PMA_CPG_DOMAIN_ID_V3M_ISP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR0,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMR1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR1,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMR2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR2,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMR3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR3,
        R_PMA_CPG_DOMAIN_ID_V3M_IMR3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_SPMI,
        R_PMA_SYSC_DOMAIN_ID_V3M_A3IR,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_RAM,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_RAM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_CORE1,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2IR0,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP01,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP01,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_CORE3,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2IR1,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP23,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP23,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_IMP_PSC,
        R_PMA_SYSC_DOMAIN_ID_V3M_A2DP,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_DP,
        R_PMA_CPG_DOMAIN_ID_V3M_IMP_DP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V3M_TISP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V3M_ISP,
        R_PMA_CPG_DOMAIN_ID_V3M_ISP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    }
};
