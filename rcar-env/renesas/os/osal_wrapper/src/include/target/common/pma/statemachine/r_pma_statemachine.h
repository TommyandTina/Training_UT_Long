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
* Version :      1.2.0
* Description  : State Machine for PM API header file
***********************************************************************************************************************/
#ifndef R_PMA_STATEMACHINE_H
/** Multiple inclusion protection macro */
#define R_PMA_STATEMACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "target/common/pma/r_pma_common.h"
#include "target/common/pma/hw/r_pma_hwdepend.h"
#include "target/common/pma/hw/r_pma_hwa_id.h"


/*======================================================================================================================
Type definitions
======================================================================================================================*/
typedef struct
{
    st_pma_handle_t       handle;           /* Handle of PMA */
    e_pma_hwa_req_t       enReqStat;        /* Requirement status */
    e_pma_reset_req_t     enRstStat;        /* Reset status */
    uint64_t              refCnt;           /* Reference counter of HWA */
    e_pma_power_policy_t  enPwrPolicy;      /* Power Policy */
    e_pma_power_state_t   enPrevStbMode;    /* Previous stanby mode */
    e_pma_power_state_t   enLowestPwrState; /* Lowest power status */
    st_hwa_t        * targetHwa;            /* Control target HWA */
} st_statemachine_t;
    
/** Function definition */
e_result_t R_PMA_SM_Init(e_pma_hwa_id_t maxHwaId);
void R_PMA_SM_Quit(void);
e_result_t R_PMA_SM_GetLockHwa(st_pma_handle_t handle, e_pma_hwa_id_t  hwaId);
e_result_t R_PMA_SM_UnlockHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
e_result_t R_PMA_SM_RequestHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
e_result_t R_PMA_SM_ReleaseHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
e_result_t R_PMA_SM_ChangePolicyHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy);
e_result_t R_PMA_SM_ApplyResetHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
e_result_t R_PMA_SM_ReleaseResetHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
e_result_t R_PMA_SM_GetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t * pPolicy);
e_result_t R_PMA_SM_GetHwaState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState);
e_result_t R_PMA_SM_GetResetState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstState);
e_result_t R_PMA_SM_GetLowestPowerState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_state_t *pLowestPower);
e_result_t R_PMA_SM_UpdateLowestPowerState(e_pma_hwa_id_t hwaId);

#ifdef __cplusplus
}
#endif
#endif /** R_PMA_STATEMACHINE_H */
