/***********************************************************************************************************************
* Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      1.6.0
* Description  : HW Dependent for PM API header file
***********************************************************************************************************************/
#ifndef R_PMA_HWDEPEND_H
/** Multiple inclusion protection macro */
#define R_PMA_HWDEPEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "target/common/pma/r_pma_common.h"
#include "target/common/pma/hw/r_pma_hwa_id.h"
#include "target/common/pma/hw/r_pma_domain_id.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
/* Number of HWA that has dependency */
#define R_PMA_HW_DEP_HWA_DEPEND_V3M_NUM     (13U)   /* V3M2 */
#define R_PMA_HW_DEP_HWA_DEPEND_V3H1_NUM    (29U)   /* V3H1 */
#define R_PMA_HW_DEP_HWA_DEPEND_V3H2_NUM    (29U)   /* V3H2 */
#define R_PMA_HW_DEP_HWA_DEPEND_V4H_NUM     (30U)   /* V4H  */
#define R_PMA_HW_DEP_HWA_DEPEND_V4M_NUM     (37U)   /* V4M  */
#define R_PMA_HW_DEP_HWA_DEPEND_S4_NUM      (0U)    /* S4   */
#define R_PMA_HW_DEP_HWA_BIT_MASK           0x7FFFFFFFFFFFFFFFULL  /* HWA bit assignment mask */
#define R_PMA_HW_DEP_HWA_GROUP_MASK         63ULL  /* HWA group mask */
#define R_PMA_HW_DEP_HWA_GROUP_NUM_0        0ULL   /* Bit assignment group 0 */
#define R_PMA_HW_DEP_HWA_GROUP_NUM_1        1ULL   /* Bit assignment group 1 */
#define R_PMA_HW_DEP_HWA_GROUP_NUM          2ULL   /* Number of groups */

/***********************************************************************************************************************
 * @enum e_cpg_dep_flag_t
 * CPG dependency Flags.
***********************************************************************************************************************/
typedef enum
{
    R_PMA_CPG_DEPEND_SET,      /* CPG Dependency */
    P_PMA_CPG_NO_DEPEND        /* No CPG dependencies */
} e_cpg_dep_flag_t;
/*======================================================================================================================
Type definitions
======================================================================================================================*/
/* Function pointer of callback for UpdateLowestPowerState */
typedef e_result_t (*p_sm_stat_chg_notice_func_t)(e_pma_hwa_id_t hwaId);

/* Function pointer to notice status change event */
typedef e_result_t (*p_chg_hwa_stat_func_t)(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);

/* PRQA S 3630 1 # This definition must be published because the function references the member after calling */
typedef struct
{
    e_pma_hwa_id_t        id;                 /* HWA ID */
    e_sysc_domain_id_t    sysc_id;            /* SYSC ID */
    e_cpg_domain_id_t     cpg_id;             /* CPG ID */
    e_cpg_domain_id_t     reset_id;           /* Reset ID */
    e_pma_reset_req_t     actual_rst_stat;    /* Actual reset status */
    p_chg_hwa_stat_func_t noticeChangeToPG;   /* Function pointer to change to PG mode */
    p_chg_hwa_stat_func_t noticeChangePGtoCG; /* Function pointer to change from PG mode to CG mode */
    p_chg_hwa_stat_func_t noticeChangeHPtoCG; /* Function pointer to change from HP mode to CG mode */
    p_chg_hwa_stat_func_t noticeChangeToHP;   /* Function pointer to change to HP mode */
    e_cpg_dep_flag_t      cpg_dep_flag;       /* CPG dependency Flags */
} st_hwa_t;

typedef struct
{
    uint64_t parent;             /* parent HWAs */
    uint64_t children;           /* child HWAs */
    uint64_t share_sysc_domain;  /* share sysc HWAs */
    uint64_t share_cpg_domain;  /* share sysc HWAs */
} st_hwa_dependent_flag_t;

/* API definition */
e_result_t R_PMA_HW_DEP_Init(uint64_t pma_soc_type, p_sm_stat_chg_notice_func_t pFunc);
e_result_t R_PMA_HW_DEP_GetHwaState(e_pma_hwa_id_t hwaId, e_pma_onoff_t *pClkStat, e_pma_onoff_t *pPwrStat);
e_result_t R_PMA_HW_DEP_GetHwaObject(e_pma_hwa_id_t hwaId, st_hwa_t **pHwa);
e_result_t R_PMA_HW_DEP_ApplyReset(e_pma_hwa_id_t hwaId);
e_result_t R_PMA_HW_DEP_ReleaseReset(e_pma_hwa_id_t hwaId);

e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeA(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeB(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeC(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforTypeAC(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforTypeB(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_GetResetState(e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstStat);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforVDSP(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_IsEnableControlPowerClock(e_pma_hwa_id_t targetHwaId,bool *isEnable);
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR(e_pma_hwa_id_t refHwaId, e_pma_hwa_id_t targetHwaId);
#define R_PMA_HW_DEP_NoticeChangeToPGforA3IMR   R_PMA_HW_DEP_NoticeChangeToPGforTypeC
#define R_PMA_HW_DEP_NoticeChangePGtoCGforA3IMR R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC
#define R_PMA_HW_DEP_NoticeChangeHPtoCGforA3IMR R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC
#define R_PMA_HW_DEP_NoticeChangeToHPforA3IMR   R_PMA_HW_DEP_NoticeChangeToHPforTypeAC
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId);
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId);
	
#ifdef __cplusplus
}
#endif
#endif /** R_PMA_HWDEPEND_H */
