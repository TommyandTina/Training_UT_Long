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
* Version :      1.3.0
* Description  : Interface for OSAL.
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <string.h>
#include "target/common/pma/r_pma.h"
#include "target/common/pma/manager/r_pma_manager.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
#define R_PMA_IF_USER_MAX   (16u)   /*!< Max number of users */

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static st_pma_user_info_t *R_PMA_IF_GetUserInfo(uint32_t userNo);
static e_pma_return_t R_PMA_IF_CheckUser(const st_pma_handle_t pUserInfo);
static e_pma_return_t R_PMA_IF_Exchange_ErrCode(e_result_t innerErr);

/*======================================================================================================================
Private global variables
======================================================================================================================*/
static bool g_pma_if_first_init_flag = true;    /*!< Initialization flag */
e_pma_hwa_id_t g_pma_max_hwa_id_num;            /*!< maximum HWA ID. It is decided by PMA Manager based on SoC type */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function R_PMA_Init()
* XOS2_OSAL_QNX_CD_CD_POW_104
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_104]
***********************************************************************************************************************/
e_pma_return_t R_PMA_Init(st_pma_handle_t       *pHandle,
                          const st_pma_config_t *pConfig)
{
    e_pma_return_t       ret_code = R_PMA_RESULT_OK;
    st_pma_user_info_t * user_info;
    e_result_t           result;

    if (NULL == pHandle)
    {
        ret_code = R_PMA_ERR_INVALID_ARGUMENT;
    }
    else
    {
        /** first time initialization */
        if (g_pma_if_first_init_flag)
        {
            g_pma_if_first_init_flag = false;
            result = R_PMA_MNG_Init(pConfig);
            ret_code = R_PMA_IF_Exchange_ErrCode(result);
        }

        if (R_PMA_RESULT_OK == ret_code)
        {
            ret_code = R_PMA_ERR_NO_VACANT_HANDLE;
            for (uint32_t num = 0U; num < R_PMA_IF_USER_MAX; num++)
            {
                user_info = R_PMA_IF_GetUserInfo(num);

                if (false == user_info->isUsed)
                {
                    user_info->isUsed = true;
                    /* PRQA S 2916 1 # There is no problem because it uses a persistent address */
                    *pHandle = (st_pma_handle_t)user_info;
                    ret_code = R_PMA_RESULT_OK;
                    break;
                }
            }
        }
    }   /* end of if (NULL == pHandle) */
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_Quit()
* XOS2_OSAL_QNX_CD_CD_POW_105
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_105]
***********************************************************************************************************************/
e_pma_return_t R_PMA_Quit(st_pma_handle_t handle)
{
    e_pma_return_t          ret_code;
    e_result_t              result;
    st_pma_user_info_t *    user_info;
    static st_msg_to_task_t lst_msg_to_tsk;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        lst_msg_to_tsk.enProcId    = R_PMA_PROCESS_ID_EXIT_THREAD;
        lst_msg_to_tsk.stPmaHandle = handle;
        result = R_PMA_MNG_SendMessageToTask(&lst_msg_to_tsk);
        
        if (R_PMA_OK == result)
        {
            result = R_PMA_MNG_Quit();
        }

        if (R_PMA_OK == result)
        {
            user_info = (st_pma_user_info_t*)handle;
            user_info->isUsed = false;
        }
    }

    if (R_PMA_RESULT_OK == ret_code)
    {
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    g_pma_if_first_init_flag = true;
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_Quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetLockHwa()
* XOS2_OSAL_QNX_CD_CD_POW_106
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_106]
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetLockHwa(st_pma_handle_t handle,
                                e_pma_hwa_id_t  hwaId)
{
    e_pma_return_t ret_code;
    e_result_t     result;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_GetLockHwa(handle, hwaId);
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_GetLockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_UnlockHwa()
* XOS2_OSAL_QNX_CD_CD_POW_107
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_107]
***********************************************************************************************************************/
e_pma_return_t R_PMA_UnlockHwa(st_pma_handle_t handle,
                               e_pma_hwa_id_t  hwaId)
{
    e_pma_return_t ret_code = R_PMA_RESULT_OK;
    e_result_t     result;
    bool is_enable;

    result = R_PMA_MNG_IsEnableControlPowerClock(hwaId, &is_enable);
    if(R_PMA_OK == result)
    {
        if(true == is_enable)
        {
            ret_code = R_PMA_ReleaseHwa(handle, hwaId);
        }
    }

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_UnlockHwa(handle, hwaId);
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_UnlockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_RequestHwa()
* XOS2_OSAL_QNX_CD_CD_POW_108
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_108]
***********************************************************************************************************************/
e_pma_return_t R_PMA_RequestHwa(st_pma_handle_t handle,
                                e_pma_hwa_id_t  hwaId)
{
    e_pma_return_t   ret_code;
    e_result_t       result;
    st_msg_to_task_t lst_msg_to_tsk;
    bool is_enable;

    lst_msg_to_tsk.enProcId    = R_PMA_PROCESS_ID_REQUEST_HWA;
    lst_msg_to_tsk.stPmaHandle = handle;
    lst_msg_to_tsk.enHwaId     = hwaId;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_IsEnableControlPowerClock(hwaId, &is_enable);
        if(R_PMA_OK == result)
        {
            if(true == is_enable)
            {
                result = R_PMA_MNG_SendMessageToTask(&lst_msg_to_tsk);
                ret_code = R_PMA_IF_Exchange_ErrCode(result);
            }
            else
            {
                ret_code = R_PMA_ERR_UNSUPPORTED_OPERATION;
            }
        }
        else
        {
            ret_code = R_PMA_IF_Exchange_ErrCode(result);
        }
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_RequestHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_ReleaseHwa()
* XOS2_OSAL_QNX_CD_CD_POW_109
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_109]
***********************************************************************************************************************/
e_pma_return_t R_PMA_ReleaseHwa(st_pma_handle_t handle,
                                e_pma_hwa_id_t  hwaId)
{
    e_pma_return_t   ret_code;
    e_result_t       result;
    st_msg_to_task_t lst_msg_to_tsk;
    bool is_enable;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_IsEnableControlPowerClock(hwaId, &is_enable);
        if (R_PMA_OK == result)
        {
            if (true == is_enable)
            {
                lst_msg_to_tsk.enProcId    = R_PMA_PROCESS_ID_RELEASE_HWA;
                lst_msg_to_tsk.stPmaHandle = handle;
                lst_msg_to_tsk.enHwaId     = hwaId;

                result = R_PMA_MNG_SendMessageToTask(&lst_msg_to_tsk);
                ret_code = R_PMA_IF_Exchange_ErrCode(result);
            }
            else
            {
                ret_code = R_PMA_ERR_UNSUPPORTED_OPERATION;
            }
        }
        else
        {
            ret_code = R_PMA_IF_Exchange_ErrCode(result);
        }
    }   /* end of if (R_PMA_RESULT_OK == ret_code) */
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_ReleaseHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetHwaState()
* XOS2_OSAL_QNX_CD_CD_POW_110
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_110]
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetHwaState(st_pma_handle_t     handle,
                                 e_pma_hwa_id_t      hwaId,
                                 st_pma_hwa_state_t *pHwaState)
{
    e_pma_return_t  ret_code;
    e_result_t      result;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_GetHwaState(handle, hwaId, pHwaState);
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_GetHwaState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SetPowerPolicy()
* XOS2_OSAL_QNX_CD_CD_POW_111
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_111]
***********************************************************************************************************************/
e_pma_return_t R_PMA_SetPowerPolicy(st_pma_handle_t      handle,
                                    e_pma_hwa_id_t       hwaId,
                                    e_pma_power_policy_t policy)
{
    e_pma_return_t ret_code;
    e_result_t     result;
    st_msg_to_task_t lst_msg_to_tsk;
    bool is_enable;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_IsEnableControlPowerClock(hwaId, &is_enable);
        if (R_PMA_OK == result)
        {
            if (true == is_enable)
            {
                lst_msg_to_tsk.enProcId    = R_PMA_PROCESS_ID_CHANGE_POLICY;
                lst_msg_to_tsk.stPmaHandle = handle;
                lst_msg_to_tsk.enHwaId     = hwaId;
                lst_msg_to_tsk.enPolicy    = policy;  /* after policy (before policy >>> st_hwa_t.enPwrPolicy) */

                result = R_PMA_MNG_SendMessageToTask(&lst_msg_to_tsk);
                ret_code = R_PMA_IF_Exchange_ErrCode(result);
            }
            else
            {
                ret_code = R_PMA_ERR_UNSUPPORTED_OPERATION;
            }
        }
        else
        {
            ret_code = R_PMA_IF_Exchange_ErrCode(result);
        }
    }   /* end of if (R_PMA_RESULT_OK == ret_code) */
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_SetPowerPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetPowerPolicy()
* XOS2_OSAL_QNX_CD_CD_POW_112
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_112]
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetPowerPolicy(st_pma_handle_t       handle,
                                    e_pma_hwa_id_t        hwaId,
                                    e_pma_power_policy_t *pPolicy)
{
    e_pma_return_t ret_code;
    e_result_t     result;
    bool is_enable;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_IsEnableControlPowerClock(hwaId, &is_enable);
        if (R_PMA_OK == result)
        {
            if (true == is_enable)
            {
                result = R_PMA_MNG_GetPowerPolicy(handle, hwaId, pPolicy);
                ret_code = R_PMA_IF_Exchange_ErrCode(result);
            }
            else
            {
                ret_code = R_PMA_ERR_UNSUPPORTED_OPERATION;
            }
        }
        else
        {
            ret_code = R_PMA_IF_Exchange_ErrCode(result);
        }
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_GetPowerPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_ApplyResetHwa()
* XOS2_OSAL_QNX_CD_CD_POW_113
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_113]
***********************************************************************************************************************/
e_pma_return_t R_PMA_ApplyResetHwa(st_pma_handle_t handle,
                                   e_pma_hwa_id_t  hwaId)
{
    e_pma_return_t   ret_code;
    e_result_t       result;
    st_msg_to_task_t lst_msg_to_tsk;
    e_pma_reset_req_t pRstStat; 

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_GetReset(handle, hwaId, &pRstStat);
        if (R_PMA_OK != result) 
        {
            ret_code = R_PMA_IF_Exchange_ErrCode(result);
        }
        else
        {
            if (R_PMA_UNDEFINED_RESET == pRstStat)
            {
                ret_code = R_PMA_ERR_IO;
            }
            else
            {
                lst_msg_to_tsk.enProcId    = R_PMA_PROCESS_ID_APPLY_RESET;
                lst_msg_to_tsk.stPmaHandle = handle;
                lst_msg_to_tsk.enHwaId     = hwaId;

                result = R_PMA_MNG_SendMessageToTask(&lst_msg_to_tsk);
                ret_code = R_PMA_IF_Exchange_ErrCode(result);
            }
        }
    }   /* end of if (R_PMA_RESULT_OK == ret_code) */
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_ApplyResetHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_ReleaseResetHwa()
* XOS2_OSAL_QNX_CD_CD_POW_114
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_114]
***********************************************************************************************************************/
e_pma_return_t R_PMA_ReleaseResetHwa(st_pma_handle_t handle,
                                     e_pma_hwa_id_t  hwaId)
{
    e_pma_return_t   ret_code;
    e_result_t       result;
    st_msg_to_task_t lst_msg_to_tsk;
    e_pma_reset_req_t pRstStat;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_GetReset(handle, hwaId, &pRstStat);
        if (R_PMA_OK != result)
        {
            ret_code = R_PMA_IF_Exchange_ErrCode(result);
        }
        else
        {
            if (R_PMA_UNDEFINED_RESET == pRstStat)
            {
                ret_code = R_PMA_ERR_IO;
            }
            else
            {
                lst_msg_to_tsk.enProcId    = R_PMA_PROCESS_ID_RELEASE_RESET;
                lst_msg_to_tsk.stPmaHandle = handle;
                lst_msg_to_tsk.enHwaId     = hwaId;

                result = R_PMA_MNG_SendMessageToTask(&lst_msg_to_tsk);
                ret_code = R_PMA_IF_Exchange_ErrCode(result);
            }
        }
    }   /* end of if (R_PMA_RESULT_OK == ret_code) */
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_ReleaseResetHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetReset()
* XOS2_OSAL_QNX_CD_CD_POW_115
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_115]
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetReset(st_pma_handle_t    handle,
                              e_pma_hwa_id_t     hwaId,
                              e_pma_reset_req_t *pRstStat)
{
    e_pma_return_t  ret_code;
    e_result_t      result;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_GetReset(handle, hwaId, pRstStat);
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_GetReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_GetLowestPowerState()
* XOS2_OSAL_QNX_CD_CD_POW_116
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_116]
***********************************************************************************************************************/
e_pma_return_t R_PMA_GetLowestPowerState(st_pma_handle_t handle,
                                         e_pma_hwa_id_t hwaId,
                                         e_pma_power_state_t *pLowestPower)
{
    e_pma_return_t ret_code;
    e_result_t     result;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_GetLowestPowerState(handle, hwaId, pLowestPower);
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_GetLowestPowerState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_WaitForState()
* XOS2_OSAL_QNX_CD_CD_POW_117
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_117]
***********************************************************************************************************************/
e_pma_return_t R_PMA_WaitForState(st_pma_handle_t     const handle,
                                  e_pma_hwa_id_t      hwaId,
                                  st_pma_hwa_state_t  const * pHwaState,
                                  e_pma_reset_req_t   rstStat,
                                  osal_milli_sec_t    sleepTimePeriod,
                                  osal_milli_sec_t    timeoutMsPeriod,
                                  osal_nano_sec_t     timeoutNsPeriod)
{
    e_pma_return_t ret_code;
    e_result_t     result;

    ret_code = R_PMA_IF_CheckUser(handle);

    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_WaitForState(handle, hwaId, pHwaState, rstStat, sleepTimePeriod, timeoutMsPeriod, timeoutNsPeriod);
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    return ret_code;
}/* PRQA S 9108 # The number of parameters of this function is necessary and sufficient. verified OK. */
/***********************************************************************************************************************
* End of function R_PMA_WaitForState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SetPostClock()
* XOS2_OSAL_QNX_CD_CD_POW_120
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_120]
***********************************************************************************************************************/
e_pma_return_t R_PMA_SetPostClock(st_pma_handle_t const handle,
                                  e_pma_clk_req_t clockEnable,
                                  uint32_t        chanIdx,
                                  uint32_t const * pClockDivider)
{
    e_pma_return_t ret_code;
    e_result_t     result;

    ret_code = R_PMA_IF_CheckUser(handle);
   
    if (R_PMA_RESULT_OK == ret_code)
    {
        result = R_PMA_MNG_SetPostClock(clockEnable, chanIdx, pClockDivider);
        ret_code = R_PMA_IF_Exchange_ErrCode(result);
    }
    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_SetPostClock()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/

/***********************************************************************************************************************
* Start of function R_PMA_IF_CheckUser()
* XOS2_OSAL_QNX_CD_CD_POW_123
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_123]
***********************************************************************************************************************/
/* PRQA S 3673 2 # QAC false indication (CCT change). */
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_pma_return_t R_PMA_IF_CheckUser(const st_pma_handle_t pUserInfo)
{
    e_pma_return_t             ret_code;
    const st_pma_user_info_t * user_info;
    const st_pma_user_info_t * user_info_arg = (st_pma_user_info_t *)pUserInfo; 

    ret_code = R_PMA_ERR_INVALID_ARGUMENT;

    if (NULL != user_info_arg)
    {
        for (uint32_t num = 0U; num < R_PMA_IF_USER_MAX; num++)
        {
            user_info = R_PMA_IF_GetUserInfo(num);

            if (user_info == user_info_arg)
            {
                if (true == user_info_arg->isUsed)
                {
                    ret_code = R_PMA_RESULT_OK;
                }
                else
                {
                    ret_code = R_PMA_ERR_INVALID_HANDLE;
                }

                break;
            }
        }
    }
    else
    {
        ret_code = R_PMA_ERR_INVALID_ARGUMENT;
    }   /* end of if (NULL != user_info_arg) */

    return ret_code;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_CheckUser()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_GetUserInfo()
* XOS2_OSAL_QNX_CD_CD_POW_124
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_124]
***********************************************************************************************************************/
static st_pma_user_info_t *R_PMA_IF_GetUserInfo(uint32_t userNo)
{
    static st_pma_user_info_t gs_pma_if_user_info[R_PMA_IF_USER_MAX] =
    {
        /** isUsed */
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false },
        { false }
    };

    st_pma_user_info_t * user_info;

    if (R_PMA_IF_USER_MAX > userNo)
    {
        user_info = &gs_pma_if_user_info[userNo];
    }
    else
    {
        user_info = NULL;
    }

    return user_info;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_GetUserInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* End of function R_PMA_IF_Exchange_ErrCode()
* XOS2_OSAL_QNX_CD_CD_POW_167
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_103]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_167]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_pma_return_t R_PMA_IF_Exchange_ErrCode(e_result_t innerErr)
{
    e_pma_return_t ret_code;

    switch(innerErr)
    {
        case R_PMA_OK:
            ret_code = R_PMA_RESULT_OK;
            break;

        case R_PMA_INVALID_ARGUMENT:
            ret_code = R_PMA_ERR_INVALID_ARGUMENT;
            break;

        case R_PMA_BSP_EINVAL:
        case R_PMA_BSP_ETIMEDOUT:
            ret_code = R_PMA_ERR_DEVICE;
            break;

        case R_PMA_BSP_EIO:
            ret_code = R_PMA_ERR_IO;
            break;

        case R_PMA_ALREADY_IN_USE:
            ret_code = R_PMA_ERR_ALREADY_IN_USE;
            break;

        case R_PMA_INVALID_HANDLE:
            ret_code = R_PMA_ERR_INVALID_HANDLE;
            break;

        case R_PMA_TIMEOUT:
            ret_code = R_PMA_ERR_TIMEOUT;
            break;

        case R_PMA_DEPENDENCY:
            ret_code = R_PMA_ERR_DEPENDENCY;
            break;

        case R_PMA_NG:
        default:
            ret_code = R_PMA_ERR_RESULT_NG;
            break;
    }   /* end of switch(innerErr) */
    return ret_code;
}

/***********************************************************************************************************************
* End of function R_PMA_IF_Exchange_ErrCode()
***********************************************************************************************************************/

/*======================================================================================================================
End of file
======================================================================================================================*/
