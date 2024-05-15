/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.5.0
* Description  : Dummy state machine for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "target/emcos/pma/r_pma.h"

#include "target/emcos/r_osal_common.h"

typedef struct st_pma_functions
{
    e_pma_return_t (*Quit)(st_pma_handle_t handle);
    e_pma_return_t (*GetLockHwa)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
    e_pma_return_t (*UnlockHwa)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
    e_pma_return_t (*RequestHwa)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
    e_pma_return_t (*ReleaseHwa)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
    e_pma_return_t (*GetHwaState)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState);
    e_pma_return_t (*SetPowerPolicy)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy);
    e_pma_return_t (*GetPowerPolicy)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy);
    e_pma_return_t (*ApplyResetHwa)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
    e_pma_return_t (*ReleaseResetHwa)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
    e_pma_return_t (*GetReset)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstStat);
    e_pma_return_t (*GetLowestPowerState)(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_state_t *pLowestPower);
    e_pma_return_t (*SetPostClock)(st_pma_handle_t const handle, e_pma_clk_req_t clockEnable, uint32_t chanIdx, uint32_t const * pClockDivider);
    e_pma_return_t (*WaitForState)(st_pma_handle_t const handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t const * pHwaState, e_pma_reset_req_t rstStat, osal_milli_sec_t sleepTimePeriod, osal_milli_sec_t timeoutMsPeriod, osal_nano_sec_t timeoutNsPeriod);

}st_pma_functions_t;

typedef struct st_pma_state_table
{
    e_pma_hwa_req_t         required;
    e_pma_power_policy_t    policy;
    e_pma_power_state_t     lowestPower;
    e_pma_reset_req_t       reset;
}st_pma_state_table_t;

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
#define R_PMA_IF_TASK_PRIORITY_MIN_TH            (1u)
#define R_PMA_IF_TASK_PRIORITY_MAX_TH            (63u)
#define R_PMA_IF_TIMEOUT_MS_MIN_TH               (1u)
#define R_PMA_IF_POLLING_NUM_MIN_TH              (1u)
#define R_PMA_IF_LOOP_NUM_FOR_RCLK_CYCLE_MIN_TH  (1u)

/*======================================================================================================================
Private global variables
======================================================================================================================*/
static st_pma_functions_t gs_pma_function;
static st_pma_state_table_t gs_pma_state_table[R_PMA_HWA_ID_NUM];
static st_pma_user_info_t gsUserInfo_Dummy;

/*======================================================================================================================
Public function prototypes
======================================================================================================================*/
extern e_pma_return_t R_PMA_IF_RegInit(uint64_t osal_soc_typev);
extern e_pma_return_t R_PMA_IF_RegDeinit(void);

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static e_pma_return_t R_PMA_IF_CheckConfigParam(const st_pma_config_t *pConfig, st_pma_param_t *pParam);

static e_pma_return_t R_PMA_IF_CheckSoC(st_pma_handle_t *pHandle);


static e_pma_return_t R_PMA_IF_Quit_Dummy(st_pma_handle_t handle);
static e_pma_return_t R_PMA_IF_GetLockHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
static e_pma_return_t R_PMA_IF_UnlockHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
static e_pma_return_t R_PMA_IF_RequestHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
static e_pma_return_t R_PMA_IF_ReleaseHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
static e_pma_return_t R_PMA_IF_GetHwaState_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState);
static e_pma_return_t R_PMA_IF_SetPowerPolicy_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy);
static e_pma_return_t R_PMA_IF_GetPowerPolicy_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy);
static e_pma_return_t R_PMA_IF_ApplyResetHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
static e_pma_return_t R_PMA_IF_ReleaseResetHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId);
static e_pma_return_t R_PMA_IF_GetReset_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstStat);
static e_pma_return_t R_PMA_IF_GetLowestPowerState_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_state_t *pLowestPower);
static e_pma_return_t R_PMA_IF_SetPostClock_Dummy(st_pma_handle_t const handle, e_pma_clk_req_t clockEnable, uint32_t chanIdx, uint32_t const * pClockDivider);
static e_pma_return_t R_PMA_IF_WaitForState_Dummy(st_pma_handle_t const handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t const * pHwaState, e_pma_reset_req_t rstStat, osal_milli_sec_t sleepTimePeriod, osal_milli_sec_t timeoutMsPeriod,osal_nano_sec_t timeoutNsPeriod);

/*======================================================================================================================
Public function
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function R_PMA_Init()
***********************************************************************************************************************/
e_pma_return_t R_PMA_Init(st_pma_handle_t       *pHandle,
                          const st_pma_config_t *pConfig)
{
    e_pma_return_t       LenRetCode;
    st_pma_param_t       LstParam;

    if (NULL == pHandle)
    {
        LenRetCode = R_PMA_ERR_INVALID_ARGUMENT;
    }
    else
    {
        LenRetCode = R_PMA_IF_CheckConfigParam(pConfig, &LstParam);
        if (R_PMA_RESULT_OK == LenRetCode)
        {
            LenRetCode = R_PMA_IF_CheckSoC(pHandle);
        }
    }

    return LenRetCode;
}
/***********************************************************************************************************************
* End of function R_PMA_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_Quit()
***********************************************************************************************************************/
e_pma_return_t R_PMA_Quit(st_pma_handle_t handle)
{
    return gs_pma_function.Quit(handle);
}
/***********************************************************************************************************************
* End of function R_PMA_Quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetLockHwa()
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetLockHwa(st_pma_handle_t handle,
                                e_pma_hwa_id_t  hwaId)
{
    return gs_pma_function.GetLockHwa(handle, hwaId);
}
/***********************************************************************************************************************
* End of function R_PMA_GetLockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_UnlockHwa()
***********************************************************************************************************************/
e_pma_return_t R_PMA_UnlockHwa(st_pma_handle_t handle,
                               e_pma_hwa_id_t  hwaId)
{
    return gs_pma_function.UnlockHwa(handle, hwaId);
}
/***********************************************************************************************************************
* End of function R_PMA_UnlockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_RequestHwa()
***********************************************************************************************************************/
e_pma_return_t R_PMA_RequestHwa(st_pma_handle_t handle,
                                e_pma_hwa_id_t  hwaId)
{
    return gs_pma_function.RequestHwa(handle, hwaId);
}
/***********************************************************************************************************************
* End of function R_PMA_RequestHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_ReleaseHwa()
***********************************************************************************************************************/
e_pma_return_t R_PMA_ReleaseHwa(st_pma_handle_t handle,
                                e_pma_hwa_id_t  hwaId)
{
    return gs_pma_function.ReleaseHwa(handle, hwaId);
}
/***********************************************************************************************************************
* End of function R_PMA_ReleaseHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetHwaState()
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetHwaState(st_pma_handle_t     handle,
                                 e_pma_hwa_id_t      hwaId,
                                 st_pma_hwa_state_t *pHwaState)
{
    return gs_pma_function.GetHwaState(handle, hwaId, pHwaState);
}
/***********************************************************************************************************************
* End of function R_PMA_GetHwaState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SetPowerPolicy()
***********************************************************************************************************************/
e_pma_return_t R_PMA_SetPowerPolicy(st_pma_handle_t      handle,
                                    e_pma_hwa_id_t       hwaId,
                                    e_pma_power_policy_t policy)
{
    return gs_pma_function.SetPowerPolicy(handle, hwaId, policy);
}
/***********************************************************************************************************************
* End of function R_PMA_SetPowerPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetPowerPolicy()
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetPowerPolicy(st_pma_handle_t       handle,
                                    e_pma_hwa_id_t        hwaId,
                                    e_pma_power_policy_t *pPolicy)
{
    return gs_pma_function.GetPowerPolicy(handle, hwaId, pPolicy);
}
/***********************************************************************************************************************
* End of function R_PMA_GetPowerPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_ApplyResetHwa()
***********************************************************************************************************************/
e_pma_return_t R_PMA_ApplyResetHwa(st_pma_handle_t handle,
                                   e_pma_hwa_id_t  hwaId)
{
    return gs_pma_function.ApplyResetHwa(handle, hwaId);
}
/***********************************************************************************************************************
* End of function R_PMA_ApplyResetHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_ReleaseResetHwa()
***********************************************************************************************************************/
e_pma_return_t R_PMA_ReleaseResetHwa(st_pma_handle_t handle,
                                     e_pma_hwa_id_t  hwaId)
{
    return gs_pma_function.ReleaseResetHwa(handle, hwaId);
}
/***********************************************************************************************************************
* End of function R_PMA_ReleaseResetHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetReset()
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetReset(st_pma_handle_t    handle,
                              e_pma_hwa_id_t     hwaId,
                              e_pma_reset_req_t *pRstStat)
{
    return gs_pma_function.GetReset(handle, hwaId, pRstStat);
}
/***********************************************************************************************************************
* End of function R_PMA_GetReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetLowestPowerState()
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetLowestPowerState(st_pma_handle_t handle,
                                         e_pma_hwa_id_t hwaId,
                                         e_pma_power_state_t *pLowestPower)
{
    return gs_pma_function.GetLowestPowerState(handle, hwaId, pLowestPower);
}
/***********************************************************************************************************************
* End of function R_PMA_GetLowestPowerState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SetPostClock()
***********************************************************************************************************************/
e_pma_return_t R_PMA_SetPostClock(st_pma_handle_t const handle,
                                  e_pma_clk_req_t clockEnable,
                                  uint32_t        chanIdx,
                                  uint32_t const * pClockDivider)
{
    return gs_pma_function.SetPostClock(handle, clockEnable, chanIdx, pClockDivider);
}
/***********************************************************************************************************************
* End of function R_PMA_SetPostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_WaitForState()
***********************************************************************************************************************/
e_pma_return_t R_PMA_WaitForState(st_pma_handle_t     const handle,
                                  e_pma_hwa_id_t      hwaId,
                                  st_pma_hwa_state_t  const * pHwaState,
                                  e_pma_reset_req_t   rstStat,
                                  osal_milli_sec_t    sleepTimePeriod,
                                  osal_milli_sec_t    timeoutMsPeriod,
                                  osal_nano_sec_t     timeoutNsPeriod)
{
    return gs_pma_function.WaitForState(handle, hwaId, pHwaState, rstStat, sleepTimePeriod, timeoutMsPeriod, timeoutNsPeriod);
}
/***********************************************************************************************************************
* End of function R_PMA_WaitForState()
***********************************************************************************************************************/

/*======================================================================================================================
Private function
======================================================================================================================*/

/***********************************************************************************************************************
* Start of function R_PMA_IF_CheckConfigParam()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_CheckConfigParam(const st_pma_config_t *pConfig,
                                                st_pma_param_t        *pParam)
{
    e_pma_return_t LenRetCode = R_PMA_RESULT_OK;

    if (NULL != pConfig)
    {
        if (NULL != pParam)
        {
            /** Task priority */
            if ((R_PMA_IF_TASK_PRIORITY_MIN_TH <= pConfig->task_priority)
                && (R_PMA_IF_TASK_PRIORITY_MAX_TH >= pConfig->task_priority))
            {
                pParam->task_priority = pConfig->task_priority;
            }
            else
            {
                LenRetCode = R_PMA_ERR_INVALID_ARGUMENT;
            }

            /** Timeout */
            if (R_PMA_RESULT_OK == LenRetCode)
            {
                /* PRQA S 1842 2 # The comparison between constants and int is not a problem,
                   as QAC seems to be false positives */
                if ((int32_t)R_PMA_IF_TIMEOUT_MS_MIN_TH <= pConfig->timeout_ms)
                {
                    pParam->timeout_ticks = pConfig->timeout_ms;
                }
                else
                {
                    LenRetCode = R_PMA_ERR_INVALID_ARGUMENT;
                }
            }

            /** Polling times */
            if (R_PMA_RESULT_OK == LenRetCode)
            {
                if (R_PMA_IF_POLLING_NUM_MIN_TH <= pConfig->polling_max_num)
                {
                    pParam->polling_max_num = pConfig->polling_max_num;
                }
                else
                {
                    LenRetCode = R_PMA_ERR_INVALID_ARGUMENT;
                }
            }

            /** RCLK 1cycle wait */
            if (R_PMA_RESULT_OK == LenRetCode)
            {
                if (R_PMA_IF_LOOP_NUM_FOR_RCLK_CYCLE_MIN_TH <= pConfig->loop_num_for_rclk_cycle)
                {
                    pParam->loop_num_for_rclk_cycle = pConfig->loop_num_for_rclk_cycle;
                }
                else
                {
                    LenRetCode = R_PMA_ERR_INVALID_ARGUMENT;
                }
            }
        }
        else
        {
            LenRetCode = R_PMA_ERR_RESULT_NG;
        }
    }
    else
    {
        /* PRQA S 2053 4 # Advance start for future updates */
        /**pParam->task_priority           = R_PMA_IF_TASK_PRIORITY;            */
        /**pParam->timeout_ticks           = R_PMA_IF_WAIT_TICK;                */
        /**pParam->polling_max_num         = R_PMA_IF_MAX_POLLING;              */
        /**pParam->loop_num_for_rclk_cycle = R_PMA_IF_LOOP_NUM_FOR_RCLK_CYCLE;  */
    }

    return LenRetCode;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_CheckConfigParam()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_CheckSoC()
***********************************************************************************************************************/
e_pma_return_t R_PMA_IF_CheckSoC(st_pma_handle_t *pHandle)
{
    uint64_t            osal_soc_type = R_OSAL_DCFG_GetSocType();
    e_pma_return_t      ret = R_PMA_RESULT_OK;
    e_pma_return_t      init_ret;

    switch (osal_soc_type)
    {
        /* Dummy PM API supported Soc */
        case OSAL_DEVICE_SOC_TYPE_V3U:
        case OSAL_DEVICE_SOC_TYPE_V4H:
            *pHandle = (st_pma_handle_t)&gsUserInfo_Dummy;
            gs_pma_function.Quit                = R_PMA_IF_Quit_Dummy;
            gs_pma_function.GetLockHwa          = R_PMA_IF_GetLockHwa_Dummy;
            gs_pma_function.UnlockHwa           = R_PMA_IF_UnlockHwa_Dummy;
            gs_pma_function.RequestHwa          = R_PMA_IF_RequestHwa_Dummy;
            gs_pma_function.ReleaseHwa          = R_PMA_IF_ReleaseHwa_Dummy;
            gs_pma_function.GetHwaState         = R_PMA_IF_GetHwaState_Dummy;
            gs_pma_function.SetPowerPolicy      = R_PMA_IF_SetPowerPolicy_Dummy;
            gs_pma_function.GetPowerPolicy      = R_PMA_IF_GetPowerPolicy_Dummy;
            gs_pma_function.ApplyResetHwa       = R_PMA_IF_ApplyResetHwa_Dummy;
            gs_pma_function.ReleaseResetHwa     = R_PMA_IF_ReleaseResetHwa_Dummy;
            gs_pma_function.GetReset            = R_PMA_IF_GetReset_Dummy;
            gs_pma_function.GetLowestPowerState = R_PMA_IF_GetLowestPowerState_Dummy;
            gs_pma_function.SetPostClock        = R_PMA_IF_SetPostClock_Dummy;
            gs_pma_function.WaitForState        = R_PMA_IF_WaitForState_Dummy;

            init_ret = R_PMA_IF_RegInit(osal_soc_type);
            if (R_PMA_RESULT_OK != init_ret)
            {
                ret = R_PMA_ERR_RESULT_NG;
            }
            break;

        default: /* PM API unsupport */
            ret = R_PMA_ERR_RESULT_NG;
    }

    return ret;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_CheckSoC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_Quit_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_Quit_Dummy(st_pma_handle_t handle)
{
    (void)handle;

    R_PMA_IF_RegDeinit();

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_Quit_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_GetLockHwa_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_GetLockHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    (void)handle;
    (void)hwaId;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_GetLockHwa_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_UnlockHwa_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_UnlockHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    (void)handle;
    (void)hwaId;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_UnlockHwa_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_RequestHwa_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_RequestHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    (void)handle;

    gs_pma_state_table[hwaId].required = R_PMA_HWA_REQUESTED;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_RequestHwa_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_ReleaseHwa_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_ReleaseHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    (void)handle;

    gs_pma_state_table[hwaId].required = R_PMA_HWA_RELEASED;

    if (R_PMA_HP_MODE == gs_pma_state_table[hwaId].policy)
    {
        gs_pma_state_table[hwaId].lowestPower = R_PMA_STATE_ENABLED;
    }
    else if (R_PMA_CG_MODE == gs_pma_state_table[hwaId].policy)
    {
        gs_pma_state_table[hwaId].lowestPower = R_PMA_STATE_CG;
    }
    else
    {
        gs_pma_state_table[hwaId].lowestPower = R_PMA_STATE_PG;
    }

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_ReleaseHwa_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_GetHwaState_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_GetHwaState_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState)
{
    (void)handle;

    pHwaState->enReqState = gs_pma_state_table[hwaId].required;

    if (R_PMA_HWA_REQUESTED == gs_pma_state_table[hwaId].required)
    {
        pHwaState->enActualPwrStat = R_PMA_STATE_ON;
        pHwaState->enActualClkStat = R_PMA_STATE_ON;
    }
    else
    {
        if (R_PMA_HP_MODE == gs_pma_state_table[hwaId].policy)
        {
            pHwaState->enActualPwrStat = R_PMA_STATE_ON;
            pHwaState->enActualClkStat = R_PMA_STATE_ON;
        }
        else if (R_PMA_CG_MODE == gs_pma_state_table[hwaId].policy)
        {
            pHwaState->enActualPwrStat = R_PMA_STATE_ON;
            pHwaState->enActualClkStat = R_PMA_STATE_OFF;
        }
        else /* if (R_PMA_PG_MODE == gs_pma_state_table[hwaId].policy) */
        {
            pHwaState->enActualPwrStat = R_PMA_STATE_OFF;
            pHwaState->enActualClkStat = R_PMA_STATE_OFF;
        }
    }

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_GetHwaState_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_SetPowerPolicy_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_SetPowerPolicy_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy)
{
    e_pma_power_state_t tmp_state;
    (void)handle;

    gs_pma_state_table[hwaId].policy = policy;

    if (R_PMA_HWA_RELEASED == gs_pma_state_table[hwaId].required)
    {
        if (R_PMA_HP_MODE == policy)
        {
            tmp_state = R_PMA_STATE_ENABLED;
        }
        else if (R_PMA_CG_MODE == policy)
        {
            tmp_state = R_PMA_STATE_CG;
        }
        else
        {
            tmp_state = R_PMA_STATE_PG;
        }

        if (gs_pma_state_table[hwaId].lowestPower > tmp_state)
        {
            gs_pma_state_table[hwaId].lowestPower = tmp_state;
        }
    }

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_SetPowerPolicy_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_GetPowerPolicy_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_GetPowerPolicy_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy)
{
    (void)handle;

    *pPolicy = gs_pma_state_table[hwaId].policy;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_GetPowerPolicy_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_ApplyResetHwa_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_ApplyResetHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    (void)handle;

    gs_pma_state_table[hwaId].reset = R_PMA_APPLIED_RESET;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_ApplyResetHwa_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_ReleaseResetHwa_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_ReleaseResetHwa_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    (void)handle;

    gs_pma_state_table[hwaId].reset = R_PMA_RELEASED_RESET;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_ReleaseResetHwa_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_GetReset_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_GetReset_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstStat)
{
    (void)handle;

    *pRstStat = gs_pma_state_table[hwaId].reset;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_GetReset_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_GetLowestPowerState_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_GetLowestPowerState_Dummy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_state_t *pLowestPower)
{
    (void)handle;
    (void)hwaId;
    *pLowestPower = gs_pma_state_table[hwaId].lowestPower;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_GetLowestPowerState_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_SetPostClock_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_SetPostClock_Dummy(st_pma_handle_t const handle, e_pma_clk_req_t clockEnable, uint32_t chanIdx, uint32_t const * pClockDivider)
{
    (void)handle;
    (void)clockEnable;
    (void)chanIdx;
    (void)pClockDivider;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_SetPostClock_Dummy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_WaitForState_Dummy()
***********************************************************************************************************************/
static e_pma_return_t R_PMA_IF_WaitForState_Dummy(st_pma_handle_t const handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t const * pHwaState, e_pma_reset_req_t rstStat, osal_milli_sec_t sleepTimePeriod, osal_milli_sec_t timeoutMsPeriod,osal_nano_sec_t timeoutNsPeriod)
{
    (void)handle;
    (void)hwaId;
    (void)pHwaState;
    (void)rstStat;
    (void)sleepTimePeriod;
    (void)timeoutMsPeriod;
    (void)timeoutNsPeriod;

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_WaitForState_Dummy()
***********************************************************************************************************************/

/*======================================================================================================================
End of file
======================================================================================================================*/
