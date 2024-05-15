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
* Description  : Manager for PM API header file
***********************************************************************************************************************/
#ifndef R_PMA_MANAGER_H
/** Multiple inclusion protection macro */
#define R_PMA_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "target/common/helper/r_osal_helper_if.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_dev_cfg_info.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_res_cfg_info.h"
#include "target/common/pma/r_pma_common.h"
#include "target/common/pma/hw/r_pma_hwa_id.h"
/*======================================================================================================================
Type definitions
======================================================================================================================*/
typedef enum
{
    R_PMA_PROCESS_ID_REQUEST_HWA,    /* Resume HWA */
    R_PMA_PROCESS_ID_RELEASE_HWA,    /* Suspend HWA */
    R_PMA_PROCESS_ID_APPLY_RESET,    /* Apply software reset */
    R_PMA_PROCESS_ID_RELEASE_RESET,  /* Clear software reset */
    R_PMA_PROCESS_ID_CHANGE_POLICY,  /* Change power policy */
    R_PMA_PROCESS_ID_EXIT_THREAD     /* Terminate thread */
} e_task_proc_id_t;

/** structure */
typedef struct
{
    e_task_proc_id_t           enProcId;        /* Process ID of Statemachine */
    e_pma_hwa_id_t             enHwaId;         /* HWA ID */
    e_pma_power_policy_t       enPolicy;        /* Power Policy */
    st_pma_handle_t            stPmaHandle;     /* Handle of PMA */
    osal_thread_handle_t       xUserTaskHandle; /* User task handle */
} st_msg_to_task_t;

typedef struct
{
    e_osal_thread_priority_t task_priority;   /*!< Priority of PMA task */
    uint32_t                 timeout_ticks;   /*!< wait ticks for return of OS API */
} st_pma_param_t;

/* Function definition */
e_result_t R_PMA_MNG_Init(const st_pma_config_t *pConfig);
e_result_t R_PMA_MNG_Quit(void);
e_result_t R_PMA_MNG_SendMessageToTask(st_msg_to_task_t *pMsg);
e_result_t R_PMA_MNG_GetLockHwa(st_pma_handle_t handle, e_pma_hwa_id_t  hwaId);
e_result_t R_PMA_MNG_UnlockHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
e_result_t R_PMA_MNG_GetHwaState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState);
e_result_t R_PMA_MNG_GetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy);
e_result_t R_PMA_MNG_GetReset(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstState);
e_result_t R_PMA_MNG_GetLowestPowerState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_state_t *pLowestPower);
e_result_t R_PMA_MNG_WaitForState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t const *pHwaState, e_pma_reset_req_t rstStat, osal_milli_sec_t sleepTimePeriod, osal_milli_sec_t timeoutMsPeriod, osal_nano_sec_t timeoutNsPeriod);
e_result_t R_PMA_MNG_SetPostClock(e_pma_clk_req_t clockEnable, uint32_t chanIdx, uint32_t const *pClockDivider);
e_result_t R_PMA_MNG_IsEnableControlPowerClock(e_pma_hwa_id_t hwaId, bool *isEnable);


#ifdef __cplusplus
}
#endif
#endif /** R_PMA_MANAGER_H */
