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
* Description  : Internal function for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <string.h>

#include "target/common/pma/manager/r_pma_manager.h"
#include "target/common/pma/statemachine/r_pma_statemachine.h"
#include "target/common/pma/hw/r_pma_hwdepend.h"
#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
/** Task */
#define R_PMA_MNG_TASK_STACK_SIZE       (4096U)     /* Stack size of thread */
#define R_PMA_MNG_TASK_TASK_NAME        "pma_mng_task"
/** Queue */
#define R_PMA_MNG_QUEUE_LENGTH          (333U)      /* Same as maximum number of HWA */
#define R_PMA_MNG_QUEUE_ITEM_SIZE       sizeof(st_msg_to_task_t) /* Size of massage structure */
/** Timeout */
#define R_PMA_MNG_TIMEOUT_MS_MIN_TH     (1U)        /* Minimum time for waiting process (millisecond) */
#define R_PMA_MNG_TIMEOUT_MS_MAX        ((osal_milli_sec_t)9223372036854) /* Maximum time for waiting process (millisecond) */
/** Post Clock */
#define R_PMA_MNG_CLK_POSTCKCR  (4U)    /* Range of channel index */
#define R_PMA_MNG_CLK_DIVIDER   (64U)   /* Range of clock divider */

#define R_PMA_MNG_1MSEC_IN_NSEC_UNIT    (1000000U)  /* Adjust the 1msec to the NSEC unit*/
#define R_PMA_MNG_WAIT_TICK     (10U)   /* Default value of the wait time[ms]   */

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static e_result_t R_PMA_MNG_CheckConfigParam(const st_pma_config_t *pConfig, st_pma_param_t *pParam);
static e_result_t R_PMA_MNG_SetupThreadSync(osal_os_mutex_t *pSetMutex, osal_os_cond_t *pSetCond);
static e_result_t R_PMA_MNG_SetupQueue(osal_os_mq_t *pTaskQueue);
static e_result_t R_PMA_MNG_SetupTask(e_osal_thread_priority_t taskPriority, osal_os_thread_t *pThread);
static e_result_t R_PMA_MNG_GetCurrentState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t const *pHwaState, st_pma_hwa_state_t *pCurHwaState, e_pma_reset_req_t *pCurRstStat);
static e_osal_return_t R_PMA_MNG_GetTimeDiffAndCondWait(osal_os_cond_t osal_os_cond,
                                                        osal_os_mutex_t osal_os_mutex,
                                                        st_osal_time_t const *LstStartTime,
                                                        st_osal_time_t *pDiffTime,
                                                        osal_milli_sec_t sleepTimePeriod);
static void* R_PMA_MNG_Task(void *pvParameters);

/*======================================================================================================================
Private global variables
======================================================================================================================*/
static osal_milli_sec_t  g_pma_mng_timeout;
static osal_os_mutex_t   g_pma_mng_mutex;
static osal_os_mq_t      g_pma_mng_queue;
static osal_os_thread_t  g_pma_mng_thread;
static osal_os_cond_t    g_pma_mng_cond;
static osal_os_mutexattr_t  g_osal_os_mutexattr;
static osal_os_threadattr_t g_osal_os_thread_attr;
static osal_os_condattr_t   g_osal_os_condattr;
/***********************************************************************************************************************
* Start of function R_PMA_MNG_Init()
* XOS3_OSAL_NOS_CD_CD_POW_101
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_126]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_Init(const st_pma_config_t *pConfig)
{
    e_result_t     result;
    st_pma_param_t parameter;
    uint64_t       pma_soc_type;
    
    result = R_PMA_MNG_CheckConfigParam(pConfig, &parameter);
    if (R_PMA_OK == result)
    {
        g_pma_mng_timeout = (osal_milli_sec_t)parameter.timeout_ticks;
        result = R_PMA_MNG_SetupThreadSync(&g_pma_mng_mutex, &g_pma_mng_cond);
        if (R_PMA_OK == result)
        {
            result = R_PMA_MNG_SetupQueue(&g_pma_mng_queue);
            if (R_PMA_OK == result)
            {
                result = R_PMA_MNG_SetupTask(parameter.task_priority, &g_pma_mng_thread);
            }
        }
    }
    
    if (R_PMA_OK == result)
    {
        /* PRQA S 2880,2992,2996 ++ # QAC false indication (CCT change). */
        switch (g_osal_device_soc_type.device_soc_type)
        {
            /* PRQA S 0572,2900 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V3M:
                pma_soc_type         = R_PMA_DEVICE_SOC_TYPE_V3M;
                g_pma_max_hwa_id_num = R_PMA_HWA_ID_V3M_NUM;
                break;
            /* PRQA S 0572,2900 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V3H1:
                pma_soc_type         = R_PMA_DEVICE_SOC_TYPE_V3H1;
                g_pma_max_hwa_id_num = R_PMA_HWA_ID_V3H1_NUM;
                break;
            /* PRQA S 0572,2900 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V3H2:
                pma_soc_type         = R_PMA_DEVICE_SOC_TYPE_V3H2;
                g_pma_max_hwa_id_num = R_PMA_HWA_ID_V3H2_NUM;
                break;
            /* PRQA S 0572,2900 2 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V4H:
            case OSAL_DEVICE_SOC_TYPE_V4H2:
                pma_soc_type         = R_PMA_DEVICE_SOC_TYPE_V4H;
                g_pma_max_hwa_id_num = R_PMA_HWA_ID_V4H_NUM;
                break;
            /* PRQA S 0572,2900 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V4M:
                pma_soc_type         = R_PMA_DEVICE_SOC_TYPE_V4M;
                g_pma_max_hwa_id_num = R_PMA_HWA_ID_V4M_NUM;
                break;
            /* PRQA S 0572,2900 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_S4:
                pma_soc_type         = R_PMA_DEVICE_SOC_TYPE_S4;
                g_pma_max_hwa_id_num = R_PMA_HWA_ID_S4_NUM;
                break;
            /* PRQA S 0572,2900 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_INVALID:
            default:
                result = R_PMA_NG;
                break;
        }
    }   /* end of if (R_PMA_OK == result) */
    if (R_PMA_OK == result)
    {
        result = R_PMA_BSP_WRAP_Init(pma_soc_type);
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_Init(pma_soc_type, &R_PMA_SM_UpdateLowestPowerState);
            if (R_PMA_OK == result)
            {
                result = R_PMA_SM_Init(g_pma_max_hwa_id_num);
            }
        }
    }
    
    if (R_PMA_OK != result)
    {
        if ((R_PMA_BSP_EINVAL == result)
            || (R_PMA_BSP_ETIMEDOUT == result)
            || (R_PMA_BSP_EIO == result))
        {
            /** Returns the BSP error as is. */
        }
        else
        {
            result = R_PMA_NG;
        }
    }
    
    return result;
}/* PRQA S 9104,9107 # The initialization function has already multiple sub-functions to reduce complexity.
                             We have decided that further division is not desirable from perspective of maintainability and readability.*/
/***********************************************************************************************************************
* End of function R_PMA_MNG_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_Quit()
* XOS3_OSAL_NOS_CD_CD_POW_102
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_302]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_Quit(void)
{
    e_result_t      result   = R_PMA_OK;
    e_osal_return_t osal_result;

    osal_result = R_OSAL_OS_ThreadWaitFinish(g_pma_mng_thread);

    if (osal_result != OSAL_RETURN_OK) {
        result = R_PMA_NG;
    }
    
    if (R_PMA_OK == result)
    {
        osal_result = R_OSAL_OS_MqDelete(g_pma_mng_queue);
        if (OSAL_RETURN_OK == osal_result)
        {
            osal_result = R_OSAL_OS_CondDestroy(g_pma_mng_cond);
            if (OSAL_RETURN_OK == osal_result)
            {
                osal_result = R_OSAL_OS_MutexDestroy(g_pma_mng_mutex);
                if (OSAL_RETURN_OK == osal_result)
                {
                    osal_result = R_OSAL_OS_ThreadDeinitAttr(g_osal_os_thread_attr);
                }

                if (OSAL_RETURN_OK == osal_result)
                {
                    osal_result = R_OSAL_OS_CondDeinitAttr(g_osal_os_condattr);
                }

                if (OSAL_RETURN_OK == osal_result)
                {
                    osal_result = R_OSAL_OS_MutexDeinitAttr(g_osal_os_mutexattr);
                }
            }
        }   /* end of if (OSAL_RETURN_OK == osal_result) */
        
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }   /* end of if (R_PMA_OK == result) */
    
    if (R_PMA_OK == result)
    {
        R_PMA_SM_Quit();
        result = R_PMA_BSP_WRAP_Quit();
    }
    
    if (R_PMA_OK != result)
    {
        if ((R_PMA_BSP_EINVAL == result)
            || (R_PMA_BSP_ETIMEDOUT == result)
            || (R_PMA_BSP_EIO == result))
        {
            /** Returns the BSP error as is. */
        }
        else
        {
            result = R_PMA_NG;
        }
    }

    return result;
}/* PRQA S 9104,9107 # The deinitialization function has already multiple sub-functions to reduce complexity.
                            We have decided that further division is not desirable from perspective of maintainability and readability.*/
/***********************************************************************************************************************
* End of function R_PMA_MNG_Quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_SendMessageToTask()
* XOS3_OSAL_NOS_CD_CD_POW_116
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_116]
***********************************************************************************************************************/
/* PRQA S 3673 2 # Implemented in accordance with the requirement. */
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
e_result_t R_PMA_MNG_SendMessageToTask(st_msg_to_task_t *pMsg)
{
    e_result_t      result;
    e_osal_return_t osal_result;

    if (NULL != pMsg)
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if (R_PMA_OK == result)
    {
        /* PRQA S 0315,0314 1 # The 3rd parameter must be (void *) and there is no side side-effect in this case. */
        osal_result = R_OSAL_HIF_MqSendForTimePeriod(g_pma_mng_queue, g_pma_mng_timeout, (void*)pMsg, sizeof(st_msg_to_task_t));
        if (osal_result != OSAL_RETURN_OK)
        {
            if (osal_result == OSAL_RETURN_TIME)
            {
                result = R_PMA_TIMEOUT;
            }
            else
            {
                result = R_PMA_NG;
            }
        }
    }

    return result;
}

/***********************************************************************************************************************
* End of function R_PMA_MNG_SendMessageToTask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_GetLockHwa()
* XOS3_OSAL_NOS_CD_CD_POW_103
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_303]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_GetLockHwa(st_pma_handle_t handle,
                                e_pma_hwa_id_t  hwaId)
{
    e_result_t      result;
    e_osal_return_t osal_result;

    osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
    if (OSAL_RETURN_OK == osal_result)
    {
        result = R_PMA_SM_GetLockHwa(handle, hwaId);
        
        osal_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_GetLockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_UnlockHwa()
* XOS3_OSAL_NOS_CD_CD_POW_104
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_104]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_UnlockHwa(st_pma_handle_t handle,
                               e_pma_hwa_id_t  hwaId)
{
    e_result_t      result;
    e_osal_return_t osal_result;

    osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
    if (OSAL_RETURN_OK == osal_result)
    {
        result = R_PMA_SM_UnlockHwa(handle, hwaId);
        
        osal_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_UnlockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_GetHwaState()
* XOS3_OSAL_NOS_CD_CD_POW_105
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_105]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_GetHwaState(st_pma_handle_t     handle,
                                 e_pma_hwa_id_t      hwaId,
                                 st_pma_hwa_state_t *pHwaState)
{
    e_result_t      result;
    e_osal_return_t osal_result;

    osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
    if (OSAL_RETURN_OK == osal_result)
    {
        result = R_PMA_SM_GetHwaState(handle, hwaId, pHwaState);
        
        osal_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_GetHwaState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_GetPowerPolicy()
* XOS3_OSAL_NOS_CD_CD_POW_106
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_106]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_GetPowerPolicy(st_pma_handle_t       handle,
                                    e_pma_hwa_id_t        hwaId,
                                    e_pma_power_policy_t *pPolicy)
{
    e_result_t      result;
    e_osal_return_t osal_result;

    osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
    if (OSAL_RETURN_OK == osal_result)
    {
        result = R_PMA_SM_GetPowerPolicy(handle, hwaId, pPolicy);
        
        osal_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_GetPowerPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_GetReset()
* XOS3_OSAL_NOS_CD_CD_POW_107
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_107]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_GetReset(st_pma_handle_t    handle,
                              e_pma_hwa_id_t     hwaId,
                              e_pma_reset_req_t *pRstState)
{
    e_result_t      result;
    e_osal_return_t osal_result;

    osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
    if (OSAL_RETURN_OK == osal_result)
    {
        result = R_PMA_SM_GetResetState(handle, hwaId, pRstState);
        
        osal_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_GetReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_GetLowestPowerState()
* XOS3_OSAL_NOS_CD_CD_POW_108
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_108]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_GetLowestPowerState(st_pma_handle_t      handle,
                                         e_pma_hwa_id_t       hwaId,
                                         e_pma_power_state_t *pLowestPower)
{
    e_result_t      result;
    e_osal_return_t osal_result;

    osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
    if (OSAL_RETURN_OK == osal_result)
    {
        result = R_PMA_SM_GetLowestPowerState(handle, hwaId, pLowestPower);
        
        osal_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_GetLowestPowerState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_WaitForState()
* XOS3_OSAL_NOS_CD_CD_POW_109
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_109]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_WaitForState(st_pma_handle_t     handle,
                                  e_pma_hwa_id_t      hwaId,
                                  st_pma_hwa_state_t  const *pHwaState,
                                  e_pma_reset_req_t   rstStat,
                                  osal_milli_sec_t    sleepTimePeriod,
                                  osal_milli_sec_t    timeoutMsPeriod,
                                  osal_nano_sec_t     timeoutNsPeriod)
{
    e_result_t          result  = R_PMA_OK;
    e_osal_return_t     osal_result = OSAL_RETURN_OK;
    e_osal_return_t     osal_mtx_result = OSAL_RETURN_OK;
    st_osal_time_t      LstDiffTime;
    st_osal_time_t      LstWaitTime;
    e_pma_reset_req_t   LenCurRstStat;
    st_osal_time_t      LstStartTime;
    st_pma_hwa_state_t  LstCurHwaState;

    /* PRQA S 0314 4 # The 1st parameter must be (void *) and there is no side side-effect in this case. */
    (void)memset((void*)&LstStartTime,   0, sizeof(st_osal_time_t));
    (void)memset((void*)&LstCurHwaState, 0, sizeof(st_pma_hwa_state_t));
    (void)memset((void*)&LstDiffTime,    0, sizeof(st_osal_time_t));
    (void)memset((void*)&LstWaitTime,    0, sizeof(st_osal_time_t));
    
    if ((g_pma_max_hwa_id_num > hwaId)
        && ((osal_nano_sec_t)0 <= timeoutNsPeriod)
        && ((osal_milli_sec_t)0 <= timeoutMsPeriod)
        && ((osal_milli_sec_t)0 <= sleepTimePeriod)
            && (timeoutNsPeriod < (osal_nano_sec_t)R_PMA_MNG_1MSEC_IN_NSEC_UNIT)
            /* PRQA S 2855 1 # System is requesting an int, so it cannot be changed. */
            && (timeoutMsPeriod < (osal_milli_sec_t)R_PMA_MNG_TIMEOUT_MS_MAX)
            && (sleepTimePeriod <= timeoutMsPeriod))
    {
        osal_result = R_OSAL_OS_ClockTimeGetHighResoTimeStamp(&LstStartTime);
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }

    if ((R_PMA_OK == result)
        && (OSAL_RETURN_OK == osal_result))
    {
        
        /* Timestamp should be calculated as an absolute time from LstStartTime */
        /* PRQA S 4393 1 # System is requesting a time_t, so it cannot be changed. */
        LstWaitTime.tv_sec = (time_t)(timeoutMsPeriod / OSAL_SECTOMSEC);
        LstWaitTime.tv_nsec = (int32_t)(((timeoutMsPeriod % OSAL_SECTOMSEC) * OSAL_MSECTONSEC) + timeoutNsPeriod);
        
        do
        {
            osal_mtx_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
            if (OSAL_RETURN_OK == osal_mtx_result)
            {
                result = R_PMA_MNG_GetCurrentState(handle, hwaId, pHwaState, &LstCurHwaState, &LenCurRstStat);
            }

            if ((R_PMA_OK == result) && (OSAL_RETURN_OK == osal_mtx_result))
            {
                if ((pHwaState->enReqState == LstCurHwaState.enReqState)
                    && ((pHwaState->enActualPwrStat == LstCurHwaState.enActualPwrStat)
                        || (R_PMA_UNDEFINED_STATE == pHwaState->enActualPwrStat))
                    && ((pHwaState->enActualClkStat == LstCurHwaState.enActualClkStat)
                        || (R_PMA_UNDEFINED_STATE == pHwaState->enActualClkStat))
                    && ((R_PMA_HWA_RELEASED == pHwaState->enReqState)
                        || ((R_PMA_HWA_REQUESTED == pHwaState->enReqState)
                            && ((rstStat == LenCurRstStat)
                                || (R_PMA_UNDEFINED_RESET == rstStat)
                                || (R_PMA_UNDEFINED_RESET == LenCurRstStat)))))
                {
                    /** Break out of the loop  (As expected) */
                    osal_mtx_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
                    break;
                }

                osal_result = R_PMA_MNG_GetTimeDiffAndCondWait(g_pma_mng_cond, g_pma_mng_mutex,
                                                               &LstStartTime, &LstDiffTime, sleepTimePeriod);
            }

            if (OSAL_RETURN_OK == osal_mtx_result)
            {
                osal_mtx_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
            }
        } while (((LstWaitTime.tv_sec > LstDiffTime.tv_sec)||
                  ((LstWaitTime.tv_sec == LstDiffTime.tv_sec) && (LstWaitTime.tv_nsec >= LstDiffTime.tv_nsec)))
                    && ((OSAL_RETURN_OK == osal_result) || (OSAL_RETURN_TIME == osal_result))
                    && (OSAL_RETURN_OK == osal_mtx_result)
                    && (R_PMA_OK == result));
    }   /* end of if ((R_PMA_OK == result) && (OSAL_RETURN_OK == osal_result)) */

    if (((OSAL_RETURN_OK == osal_result) || (OSAL_RETURN_TIME == osal_result)) && (OSAL_RETURN_OK == osal_mtx_result))
    {
        if (R_PMA_OK == result)
        {
            if ((LstDiffTime.tv_sec > LstWaitTime.tv_sec)||
                ((LstDiffTime.tv_sec == LstWaitTime.tv_sec) && (LstDiffTime.tv_nsec > LstWaitTime.tv_nsec)))
            {
                result = R_PMA_TIMEOUT;
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
} /* PRQA S 9104,9108 # For better readability this rule is ignored and this function is not split.
                        The number of parameters of this function is necessary and sufficient. verified OK. */
/***********************************************************************************************************************
* End of function R_PMA_MNG_WaitForState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_GetCurrentState()
* XOS3_OSAL_NOS_CD_CD_POW_220
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_120]
***********************************************************************************************************************/
static e_result_t R_PMA_MNG_GetCurrentState(st_pma_handle_t     handle,
                                            e_pma_hwa_id_t      hwaId,
                                            st_pma_hwa_state_t  const *pHwaState,
                                            st_pma_hwa_state_t  *pCurHwaState,
                                            e_pma_reset_req_t   *pCurRstStat)
{
    e_result_t      result;

    result = R_PMA_SM_GetHwaState(handle, hwaId, pCurHwaState);
    if (R_PMA_OK == result)
    {
        if (R_PMA_HWA_REQUESTED == pHwaState->enReqState)
        {
            result = R_PMA_SM_GetResetState(handle, hwaId, pCurRstStat);
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_GetCurrentState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_GetTimeDiffAndCondWait()
* XOS3_OSAL_NOS_CD_CD_POW_221
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_121]
***********************************************************************************************************************/
static e_osal_return_t R_PMA_MNG_GetTimeDiffAndCondWait(osal_os_cond_t       osal_os_cond,
                                                        osal_os_mutex_t      osal_os_mutex,
                                                        st_osal_time_t const *LstStartTime,
                                                        st_osal_time_t       *pDiffTime,
                                                        osal_milli_sec_t     sleepTimePeriod)
{
    e_osal_return_t osal_result;
    st_osal_time_t  current_time;
    st_osal_time_t  time_stamp;

    /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side side-effect in this case. */
    (void)memset((void*)&current_time, 0, sizeof(st_osal_time_t));

    /* Timestamp should be calculated as an absolute time from LstStartTime */
    /* PRQA S 4393 1 # System is requesting a time_t, so it cannot be changed. */
    time_stamp.tv_sec = LstStartTime->tv_sec + (time_t)(sleepTimePeriod / OSAL_SECTOMSEC);
    time_stamp.tv_nsec = LstStartTime->tv_nsec + (int32_t)((sleepTimePeriod % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);

    /* Add elapsed time from the previous wait */
    time_stamp.tv_sec += pDiffTime->tv_sec;
    time_stamp.tv_nsec += pDiffTime->tv_nsec;

    /* Adjust nsec overflow */
    if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
    {
        /* PRQA S 4393 1 # System is requesting a time_t, so it cannot be changed. */
        time_stamp.tv_sec += (time_t)(time_stamp.tv_nsec / OSAL_SECTONSEC);
        time_stamp.tv_nsec %= (int32_t)OSAL_SECTONSEC;
    }

    osal_result = R_OSAL_OS_CondTimedWait(osal_os_cond, osal_os_mutex, &time_stamp);

    if ((OSAL_RETURN_OK == osal_result) || (OSAL_RETURN_TIME == osal_result))
    {
        osal_result = R_OSAL_OS_ClockTimeGetHighResoTimeStamp(&current_time);

        /* Timestamp should be calculated regardless of the result of R_OSAL_OS_ClockTimeGetHighResoTimeStamp() */
        if ((current_time.tv_sec >= LstStartTime->tv_sec) && (current_time.tv_nsec >= LstStartTime->tv_nsec))
        {
            pDiffTime->tv_sec  = current_time.tv_sec - LstStartTime->tv_sec;
            pDiffTime->tv_nsec = current_time.tv_nsec - LstStartTime->tv_nsec;
        }
        else
        {
            pDiffTime->tv_sec  = current_time.tv_sec - (LstStartTime->tv_sec + (time_t)1L);
            pDiffTime->tv_nsec = (int32_t)((OSAL_SECTONSEC + current_time.tv_nsec) - LstStartTime->tv_nsec);
        }
    }

    return osal_result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_GetTimeDiffAndCondWait()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_SetPostClock()
* XOS3_OSAL_NOS_CD_CD_POW_110
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_110]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_SetPostClock(e_pma_clk_req_t  clockEnable,
                                  uint32_t         chanIdx,
                                  uint32_t const * pClockDivider)
{
    e_result_t result = R_PMA_OK;
    e_osal_return_t osal_result = OSAL_RETURN_OK;

    if (((R_PMA_CLK_ENABLE == clockEnable) || (R_PMA_CLK_DISABLE == clockEnable))
          && (R_PMA_MNG_CLK_POSTCKCR >= chanIdx)
          && ((NULL != pClockDivider) && (R_PMA_MNG_CLK_DIVIDER > *pClockDivider)))
    {
        osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
        if (OSAL_RETURN_OK == osal_result)
        {
            result = R_PMA_BSP_WRAP_SetPostDivider(chanIdx, pClockDivider);
            if (R_PMA_OK == result)
            {
                if (R_PMA_CLK_ENABLE == clockEnable)
                {
                    result = R_PMA_BSP_WRAP_EnablePostClock(chanIdx);
                }
                else
                {
                    result = R_PMA_BSP_WRAP_DisablePostClock(chanIdx);
                }
            }
            
            osal_result = R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if (((R_PMA_CLK_ENABLE == clockEnable) || (R_PMA_CLK_DISABLE == clockEnable))... */
    
    if (OSAL_RETURN_OK != osal_result)
    {
        result = R_PMA_NG;
    }

    return result;

}
/***********************************************************************************************************************
* End of function R_PMA_MNG_SetPostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_CheckConfigParam()
* XOS3_OSAL_NOS_CD_CD_POW_117
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_117]
***********************************************************************************************************************/
static e_result_t R_PMA_MNG_CheckConfigParam(const st_pma_config_t *pConfig,
                                             st_pma_param_t        *pParam)
{
    e_result_t result = R_PMA_OK;

    if (NULL != pParam)
    {
        if (NULL != pConfig)
        {
            /** Task priority */
            if ((OSAL_THREAD_PRIORITY_LOWEST <= pConfig->task_priority)
                && (OSAL_THREAD_PRIORITY_HIGHEST >= pConfig->task_priority))
            {
                pParam->task_priority = pConfig->task_priority;
            }
            else
            {
                result = R_PMA_INVALID_ARGUMENT;
            }

            /** Timeout */
            if (R_PMA_OK == result)
            {
                if ((uint32_t)R_PMA_MNG_TIMEOUT_MS_MIN_TH <= pConfig->timeout_ms)
                {
                    pParam->timeout_ticks = pConfig->timeout_ms;
                }
                else
                {
                    result = R_PMA_INVALID_ARGUMENT;
                }
            }
        }
        else
        {
            /* Default value of the thread priority */
            pParam->task_priority = OSAL_THREAD_PRIORITY_TYPE1;
            pParam->timeout_ticks = R_PMA_MNG_WAIT_TICK;
        }   /* end of if (NULL != pConfig) */
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if (NULL != pParam) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_CheckConfigParam()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_SetupThreadSync()
* XOS3_OSAL_NOS_CD_CD_POW_111
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_111]
***********************************************************************************************************************/
static e_result_t R_PMA_MNG_SetupThreadSync(osal_os_mutex_t *pSetMutex, osal_os_cond_t *pSetCond)
{
    e_result_t             result = R_PMA_OK;
    e_osal_return_t        osal_result;

    if ((NULL != pSetMutex) && (NULL != pSetCond))
    {
        osal_result = R_OSAL_OS_MutexInitAttr(&g_osal_os_mutexattr);
        if (OSAL_RETURN_OK == osal_result)
        {
            osal_result = R_OSAL_OS_MutexCreate(g_osal_os_mutexattr, pSetMutex);
        }

        if (OSAL_RETURN_OK == osal_result)
        {
            osal_result = R_OSAL_OS_CondInitAttr(&g_osal_os_condattr);
        }

        if (OSAL_RETURN_OK == osal_result)
        {
            osal_result = R_OSAL_OS_CondStaticCreate(g_osal_os_condattr, pSetCond);
        }

        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((NULL != pSetMutex) && (NULL != pSetCond)) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_SetupThreadSync()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_SetupQueue()
* XOS3_OSAL_NOS_CD_CD_POW_112
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_112]
***********************************************************************************************************************/
static e_result_t R_PMA_MNG_SetupQueue(osal_os_mq_t *pTaskQueue)
{
    e_result_t                result = R_PMA_OK;
    e_osal_return_t           osal_result;
    st_osal_mq_config_t       osal_inner_mq_config;
    osal_os_mqattr_t          mq_attr;

    osal_inner_mq_config.max_num_msg = R_PMA_MNG_QUEUE_LENGTH;
    osal_inner_mq_config.msg_size    = R_PMA_MNG_QUEUE_ITEM_SIZE;

    osal_result = R_OSAL_OS_MqInitAttr(&mq_attr);
    if (OSAL_RETURN_OK == osal_result)
    {
        osal_result = R_OSAL_OS_MqCreate(&osal_inner_mq_config, mq_attr, pTaskQueue);
        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
        (void)R_OSAL_OS_MqDeinitAttr(mq_attr);
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_SetupQueue()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_SetupTask()
* XOS3_OSAL_NOS_CD_CD_POW_113
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_113]
***********************************************************************************************************************/
static e_result_t R_PMA_MNG_SetupTask(e_osal_thread_priority_t taskPriority,
                                      osal_os_thread_t *pThread)
{
    e_result_t                      result = R_PMA_OK;
    e_osal_return_t                 osal_result;
    st_osal_thread_config_t         osal_os_thread_config;
    uint32_t                        arg;

    if (NULL != pThread)
    {
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side side-effect in this case. */
        (void)memset((void*)&osal_os_thread_config, 0, sizeof(st_osal_thread_config_t));
        osal_os_thread_config.task_name  = R_PMA_MNG_TASK_TASK_NAME;
        osal_os_thread_config.stack_size = R_PMA_MNG_TASK_STACK_SIZE;
        osal_result = R_OSAL_OS_ThreadInitAttr(taskPriority, R_PMA_MNG_TASK_STACK_SIZE, &g_osal_os_thread_attr);
        if (OSAL_RETURN_OK == osal_result)
        {
            /* PRQA S 0315 1 # The 3rd parameter must be (void *) and there is no side side-effect in this case. */
            osal_result = R_OSAL_OS_ThreadStaticCreate(&osal_os_thread_config, R_PMA_MNG_Task, &arg, g_osal_os_thread_attr, pThread);
        }

        if (OSAL_RETURN_OK != osal_result)
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if (NULL != pThread) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_SetupTask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_Task()
* XOS3_OSAL_NOS_CD_CD_POW_118
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_101]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_118]
***********************************************************************************************************************/
static void* R_PMA_MNG_Task(void *pvParameters)/* PRQA S 3673 # Implemented in accordance with the requirement. */
{
    static e_result_t result;
    e_osal_return_t   osal_result;
    st_msg_to_task_t  msg_to_tsk;
    bool              is_terminate = false;

    (void)pvParameters; /** this parameter is not used */

    osal_result = R_OSAL_OS_ThreadSetTaskName(R_PMA_MNG_TASK_TASK_NAME);
    if (OSAL_RETURN_OK == osal_result)
    {
        osal_result = R_OSAL_OS_ThreadSetCpuAffinity((osal_thread_id_t)OSAL_THREAD_INVALID_ID);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_result)
    {
        while (true)
        {
            result  = R_PMA_OK;
            /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side side-effect in this case. */
            (void)memset((void*)&msg_to_tsk, 0, sizeof(st_msg_to_task_t));

            /* PRQA S 0315 1 # The 3rd parameter must be (void *) and there is no side side-effect in this case. */
            osal_result = R_OSAL_HIF_MqReceiveForTimePeriod(g_pma_mng_queue, (osal_milli_sec_t)1000, &msg_to_tsk, sizeof(st_msg_to_task_t));
            if (OSAL_RETURN_OK == osal_result)
            {
                osal_result = R_OSAL_HIF_MutexLockForTimePeriod(g_pma_mng_mutex, g_pma_mng_timeout);
                if (OSAL_RETURN_OK == osal_result)
                {
                    switch (msg_to_tsk.enProcId)
                    {
                        case R_PMA_PROCESS_ID_REQUEST_HWA:
                            result = R_PMA_SM_RequestHWA(msg_to_tsk.stPmaHandle, msg_to_tsk.enHwaId);
                            break;

                        case R_PMA_PROCESS_ID_RELEASE_HWA:
                            result = R_PMA_SM_ReleaseHWA(msg_to_tsk.stPmaHandle, msg_to_tsk.enHwaId);
                            break;

                        case R_PMA_PROCESS_ID_APPLY_RESET:
                            result = R_PMA_SM_ApplyResetHWA(msg_to_tsk.stPmaHandle, msg_to_tsk.enHwaId);
                            break;

                        case R_PMA_PROCESS_ID_RELEASE_RESET:
                            result = R_PMA_SM_ReleaseResetHWA(msg_to_tsk.stPmaHandle, msg_to_tsk.enHwaId);
                            break;

                        case R_PMA_PROCESS_ID_CHANGE_POLICY:
                            result = R_PMA_SM_ChangePolicyHWA(msg_to_tsk.stPmaHandle, msg_to_tsk.enHwaId, msg_to_tsk.enPolicy);
                            break;

                        case R_PMA_PROCESS_ID_EXIT_THREAD:
                            is_terminate = true;
                            break;

                        default:
                            result = R_PMA_NG;
                            break;
                    }   /* end of switch (msg_to_tsk.enProcId) */

                    osal_result = R_OSAL_OS_CondBroadcast(g_pma_mng_cond);

                    /**
                     * g_pma_mng_mutex is already locked successfully.
                     * If this unlock is failed, the error can be detected by the next mutex lock timing.
                     * Hence, it is not necessary to check the return code at this point,
                     * in order to prevent osal_result from overwriting by the error of mutex unlock.
                     */
                    (void)R_OSAL_OS_MutexUnlock(g_pma_mng_mutex);

                    if (OSAL_RETURN_OK != osal_result)
                    {
                        result = R_PMA_NG;
                    }
                    if (true ==is_terminate)
                    {
                        break;
                    }
                }
                else
                {
                    /* PRQA S 2982 1 # This flag is referenced in other thread. No problem. */
                    result = R_PMA_NG;
                }   /* end of if (OSAL_RETURN_OK == osal_result) */
            }   /* end of if (OSAL_RETURN_OK == osal_result) */
        }   /* end of while (true) */
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if (OSAL_RETURN_OK == osal_result) */

    return (void*)&result;/* PRQA S 0314 # No problem because it follows the specification of the calling function */
}/* PRQA S 9107,9110 # Since it is a task process, it is necessary to call multiple functions separately.
                  We have decided that further division is not desirable from perspective of maintainability and readability. */

/***********************************************************************************************************************
* End of function R_PMA_MNG_Task()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_MNG_IsEnableControlPowerClock()
* XOS3_OSAL_NOS_CD_CD_POW_195 
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_195]
***********************************************************************************************************************/
e_result_t R_PMA_MNG_IsEnableControlPowerClock(e_pma_hwa_id_t hwaId,
                                               bool *isEnable)
{
    e_result_t result;

    result = R_PMA_HW_DEP_IsEnableControlPowerClock(hwaId, isEnable);

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_MNG_IsEnableControlPowerClock()
***********************************************************************************************************************/

/*======================================================================================================================
End of file
======================================================================================================================*/
