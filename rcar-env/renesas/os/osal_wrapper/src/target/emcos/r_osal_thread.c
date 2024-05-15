/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_thread.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper Thread Manager for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <pthread.h>

#include "target/emcos/r_osal_common.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_Thread Thread Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_thread_is_initialized
 * Initialize flag
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_thread_is_initialized = false;

/** @} */

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Functions_Thread Thread Manager Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Internal thread of the Thread Manager.
* @param[in]        p_arg  The address of st_osal_thread_control_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void* osal_internal_thread(void* p_arg);

/*******************************************************************************************************************//**
* @brief            Initialize mutex attributes.
* @param[in]        p_mutex_attr    The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_init_mutex_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Internal initialize process.
* @param[in]        p_mutex_attr    The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_initialize_proc(const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Initialize mutex and cond object.
* @param[in]        p_thread_control  The pointor of thread control information.
* @param[in]        p_mutex_attr      The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_init_mutex_cond(st_osal_thread_control_t *p_thread_control,
                                                             pthread_mutexattr_t const *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Set thread attributes.
* @param[in]        p_thread_attr      The pointor of thread attributes.
* @param[in]        p_usr_config       Specify internal management data for thread config.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_set_thread_attr(pthread_attr_t *p_thread_attr,
                                                             const st_osal_thread_config_t *p_config);

/*******************************************************************************************************************//**
* @brief            Internal cond signal function used in deinitialize.
* @param[in]        p_thread_control  The pointor of thread control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_cond_signal(st_osal_thread_control_t *p_thread_control);

/*******************************************************************************************************************//**
* @brief            Internal mutex destroy function used in deinitialize.
* @param[in]        p_thread_control  The pointor of thread control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_mutex_destory(st_osal_thread_control_t *p_thread_control);

/*******************************************************************************************************************//**
* @brief            Internal cond destroy function used in deinitialize.
* @param[in]        p_thread_control  The pointor of thread control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_cond_destory(st_osal_thread_control_t *p_thread_control);

/*******************************************************************************************************************//**
* @brief            Deinitialize Thread Manager process.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_pre_proc(void);

/*******************************************************************************************************************//**
* @brief            Parameter check for R_OSAL_ThreadCreate.
* @param[in]        p_config  The address of st_osal_thread_config_t.
* @param[in]        p_handle  The address of osal_thread_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_check_create_param(const st_osal_thread_config_t *const p_config,
                                                                const osal_thread_handle_t *const p_handle);

/*******************************************************************************************************************//**
* @brief            Acquires the thread information of the specified thread ID.
* @param[in]        thread_id  Thread id.
* @param[in,out]    p_control  The address of osal_thread_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_ID
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_get_thread_id_info(osal_thread_id_t thread_id,
                                                                osal_thread_handle_t *p_control);

/*******************************************************************************************************************//**
* @brief            Set the initial information in the thread handle.
* @param[in]        control   The address of osal_thread_handle_t setting source.
* @param[in]        p_config  The address of st_osal_thread_config_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_set_create_handle(osal_thread_handle_t control,
                                                               const st_osal_thread_config_t *const p_config);

/*******************************************************************************************************************//**
* @brief            Internal pthread_cond_wait for R_OSAL_ThreadJoin.
* @param[in]        handle    The address of osal_thread_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_cond_wait_for_join(osal_thread_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Send signal to pthread_cond_wait for R_OSAL_ThreadJoin.
* @param[in]        p_control  The address of st_osal_thread_control_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_signal_for_join_cond(st_osal_thread_control_t *p_control);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_ThreadCreate()
* XOS3_OSAL_EMCOS_CD_CD_TH_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_006]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_007]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_008]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_009]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadCreate (const st_osal_thread_config_t *const p_config,
                                     osal_thread_id_t thread_id,
                                     osal_thread_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret;
    osal_thread_handle_t control  = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_th_check_create_param(p_config, p_handle);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_th_get_thread_id_info(thread_id, &control);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_th_set_create_handle(control, p_config);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        if ((OSAL_RETURN_BUSY != osal_ret) && (NULL != control))
        {
            *p_handle = OSAL_THREAD_HANDLE_INVALID;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
        *p_handle = control;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThreadCreate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThreadEqual()
* XOS3_OSAL_EMCOS_CD_CD_TH_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_010]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_011]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_012]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_ThreadEqual (osal_thread_handle_t handle1, osal_thread_handle_t handle2, bool *const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    bool            result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == handle1) || (NULL == handle2))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_THREAD_HANDLE_ID != handle1->handle_id) || (OSAL_THREAD_HANDLE_ID != handle2->handle_id))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (NULL == p_result)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        if (handle1 == handle2)
        {
            result = true;
        }
        else
        {
            result = false;
        }
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        *p_result = result;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThreadEqual()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThreadInitializeThreadConfigSt()
* XOS3_OSAL_EMCOS_CD_CD_TH_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_013]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadInitializeThreadConfigSt (st_osal_thread_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_config->func       = NULL;
        p_config->userarg    = NULL;
        p_config->priority   = OSAL_THREAD_PRIORITY_TYPE0;
        p_config->task_name  = "";
        p_config->stack_size = 0;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThreadInitializeThreadConfigSt()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThreadJoin()
* XOS3_OSAL_EMCOS_CD_CD_TH_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_014]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_015]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_016]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadJoin (osal_thread_handle_t handle, int64_t *const p_return_value)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    pthread_t       self_id;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             id_result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_THREAD_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        self_id   = pthread_self();
        id_result = pthread_equal(handle->thread, self_id);
        if (0 != id_result)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            osal_ret = OSAL_RETURN_OK;
        }
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK == osal_ret)
        {

            osal_ret = osal_internal_th_cond_wait_for_join(handle);
            if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_return_value))
            {
                *p_return_value = handle->thread_ret;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->handle_id = OSAL_INVALID_USER_HANDLE_ID;
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThreadJoin()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThreadSelf()
* XOS3_OSAL_EMCOS_CD_CD_TH_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_017]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_018]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadSelf (osal_thread_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    size_t                   th_num;
    size_t                   loop_count      = 0;
    pthread_t                self_id;
    st_osal_thread_control_t *thread_control = NULL;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_thread_handle_t     control         = NULL;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      id_result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_handle)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        self_id = pthread_self();
        th_num  = R_OSAL_RCFG_GetNumOfThread();

        while ((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret) && (NULL == control))
        {
            osal_ret = R_OSAL_RCFG_GetThreadInfo((uint32_t)loop_count, &thread_control);
            if ((OSAL_RETURN_OK == osal_ret) && (NULL != thread_control))
            {
                id_result = pthread_equal(thread_control->thread, self_id);
                if (0 != id_result)
                {
                    control = (osal_thread_handle_t)thread_control;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            loop_count++;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == control)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
            *p_handle = control;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThreadSelf()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThreadSleepForTimePeriod()
* XOS3_OSAL_EMCOS_CD_CD_TH_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_019]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadSleepForTimePeriod (osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec abs_time;
    osal_milli_sec_t time_period_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    time_period_tmp = time_period;
    if (false == gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (0 > time_period_tmp)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_GetTimestamp(&abs_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimespecAdd(&abs_time, &time_period_tmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209,2855 1 # System is requesting an int, so it cannot be changed. */
        os_ret = clock_nanosleep(CLOCK_REALTIME, (int)TIMER_ABSTIME, &abs_time, NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThreadSleepForTimePeriod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThreadSleepUntilTimeStamp()
* XOS3_OSAL_EMCOS_CD_CD_TH_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_020]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec time_spec;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_CheckTimestamp(p_time_stamp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        time_spec.tv_sec  = p_time_stamp->tv_sec;
        time_spec.tv_nsec = p_time_stamp->tv_nsec;

        osal_ret = R_OSAL_Internal_TimeRoundUp(&time_spec);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209,2855 1 # System is requesting an int, so it cannot be changed. */
        os_ret = clock_nanosleep(CLOCK_MONOTONIC, (int)TIMER_ABSTIME, &time_spec, NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThreadSleepUntilTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_thread_initialize()
* XOS3_OSAL_EMCOS_CD_CD_TH_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_004]
***********************************************************************************************************************/
e_osal_return_t osal_thread_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    pthread_mutexattr_t      mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false != gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_thread_is_initialized = true;

        osal_ret = osal_internal_th_init_mutex_attr(&mutex_attr);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_th_initialize_proc(&mutex_attr);

            os_ret = pthread_mutexattr_destroy(&mutex_attr);
            if ((OSAL_RETURN_OK == osal_ret) && (0 != os_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)osal_thread_deinitialize();
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(false != gs_osal_thread_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_thread_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_thread_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_TH_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_002]
***********************************************************************************************************************/
e_osal_return_t osal_thread_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    size_t                   th_num;
    size_t                   loop_count      = 0;
    st_osal_thread_control_t *thread_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_thread_is_initialized = false;

    th_num = R_OSAL_RCFG_GetNumOfThread();

    while ((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_RCFG_GetThreadInfo((uint32_t)loop_count, &thread_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == thread_control))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_th_deinit_cond_signal(thread_control);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2810 1 # Following the specification. */
            os_ret = pthread_join(thread_control->thread, NULL);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_th_deinit_mutex_destory(thread_control);
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_th_deinit_cond_destory(thread_control);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            thread_control->handle_id     = OSAL_INVALID_USER_HANDLE_ID;
            thread_control->active        = false;
            thread_control->parent_thread = false;
            thread_control->thread_ret    = 0;
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }   /* end of while((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_thread_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_thread_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_TH_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_003]
***********************************************************************************************************************/
e_osal_return_t osal_thread_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true != gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_thread_is_initialized = false;
        osal_ret = osal_internal_th_deinit_pre_proc();
    }

    if (OSAL_RETURN_BUSY == osal_ret)
    {
        gs_osal_thread_is_initialized = true;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_thread_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_thread_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_TH_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_005]
***********************************************************************************************************************/
void osal_thread_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_thread_is_initialized = true;

    return;
}
/***********************************************************************************************************************
* End of function osal_thread_set_is_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_thread()
* XOS3_OSAL_EMCOS_CD_CD_TH_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_001]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the 1st parameter of pthread_create(). */
OSAL_STATIC void* osal_internal_thread(void* p_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    st_osal_thread_control_t *control    = (st_osal_thread_control_t *)p_arg;
    e_osal_return_t          local_error = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((NULL != control) && (true == gs_osal_thread_is_initialized) && (OSAL_RETURN_OK == local_error))
    {
        local_error = R_OSAL_Internal_TimedlockMutex(&control->inner_mtx, control->inner_timeout);
        if (OSAL_RETURN_OK == local_error)
        {
            if (true != control->active)
            {
                os_ret = pthread_cond_wait(&control->create_cond, &control->inner_mtx);
                if (0 != os_ret)
                {
                    local_error = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&control->inner_mtx);
            if (0 != os_ret)
            {
                local_error = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            local_error = OSAL_RETURN_FAIL;
        }   /* end of if(OSAL_RETURN_OK == local_error). */

        if ((OSAL_RETURN_OK == local_error) && (NULL != control->func))
        {
            control->thread_ret = control->func(control->userarg);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == local_error)
        {
            local_error = osal_internal_th_signal_for_join_cond(control);
        }
        else
        {
            /* Do nothing */
        }
        /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of while((NULL != control) && (true == gs_osal_thread_is_initialized) &&
                        (OSAL_RETURN_OK == local_error)). */

    return NULL;
}
/***********************************************************************************************************************
* End of function osal_internal_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_init_mutex_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_init_mutex_attr(pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutexattr_init(p_mutex_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_mutexattr_settype(p_mutex_attr, (int)PTHREAD_MUTEX_ERRORCHECK);
        if (os_ret != 0)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_mutexattr_destroy(p_mutex_attr);
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_init_mutex_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_initialize_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_initialize_proc(const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    size_t                   loop_count      = 0;
    size_t                   th_num;
    st_osal_thread_control_t *thread_control = NULL;
    pthread_attr_t           thread_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    th_num = R_OSAL_RCFG_GetNumOfThread();
    if (0U == th_num)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    while ((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_RCFG_GetThreadInfo((uint32_t)loop_count, &thread_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == thread_control) || (NULL == thread_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_th_init_mutex_cond(thread_control, p_mutex_attr);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_th_set_thread_attr(&thread_attr, &thread_control->usr_config->config);
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 0314 2 # The parameter must be (void *) and there is no side side-effect in this case. */
                os_ret = pthread_create(&thread_control->thread,
                                        &thread_attr, osal_internal_thread, (void *)thread_control);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
                os_ret = pthread_attr_destroy(&thread_attr);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }

            }
            else
            {
                /* Do nothing */
            }   /* end of if(OSAL_RETURN_OK == osal_ret). */

            if (OSAL_RETURN_OK == osal_ret)
            {
                thread_control->active        = false;
                thread_control->parent_thread = false;
                thread_control->func          = NULL;
                thread_control->userarg       = NULL;
                thread_control->thread_ret    = 0;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        loop_count++;
    }   /* end of while((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_initialize_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_init_mutex_cond()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_init_mutex_cond(st_osal_thread_control_t *p_thread_control,
                                                             pthread_mutexattr_t const *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 2736 1 # No problem because osal_thread_deinitialize is called pthread_mutex_destroy. */
    os_ret = pthread_mutex_init(&p_thread_control->inner_mtx, p_mutex_attr);
    if (os_ret != 0)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_init(&p_thread_control->create_cond, NULL);
        if(os_ret != 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_init(&p_thread_control->join_cond, NULL);
        if(os_ret != 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_init_mutex_cond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_set_thread_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_set_thread_attr(pthread_attr_t *p_thread_attr, const st_osal_thread_config_t *p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret    = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                os_ret;
    struct sched_param th_priority;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_attr_init(p_thread_attr);
    if(os_ret != 0)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_attr_setinheritsched(p_thread_attr, (int)PTHREAD_EXPLICIT_SCHED);
        if(os_ret != 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            os_ret = pthread_attr_setschedpolicy(p_thread_attr, (int)SCHED_RR);
            if(os_ret != 0)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((OSAL_THREAD_PRIORITY_HIGHEST < p_config->priority) || (OSAL_THREAD_PRIORITY_LOWEST > p_config->priority))
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
                th_priority.sched_priority = (int)((int)p_config->priority + (int)OSAL_THREAD_PRIO_BASE);
                os_ret = pthread_attr_setschedparam(p_thread_attr, &th_priority);
                if(0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (0U == p_config->stack_size)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                os_ret = pthread_attr_setstacksize(p_thread_attr, (size_t)(p_config->stack_size + (size_t)PTHREAD_STACK_MIN));
                if(0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        if (OSAL_RETURN_OK != osal_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_attr_destroy(p_thread_attr);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(os_ret != 0). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_set_thread_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_deinit_cond_signal()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_cond_signal(st_osal_thread_control_t *p_thread_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_Internal_TimedlockMutex(&p_thread_control->inner_mtx, p_thread_control->inner_timeout);
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_thread_control->func    = NULL;
        p_thread_control->userarg = NULL;
        os_ret = pthread_cond_signal(&p_thread_control->create_cond);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_thread_control->active = true;
        }

        os_ret = pthread_mutex_unlock(&p_thread_control->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_deinit_cond_signal()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_deinit_mutex_destory()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_mutex_destory(st_osal_thread_control_t *p_thread_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret          = EBUSY;
    size_t              waiting_time;
    osal_milli_sec_t    milliseconds    = OSAL_THREAD_MS_SLEEP_TIME;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    waiting_time = 0;
    while (((size_t)OSAL_MUTEX_DESTROY_TIMEOUT >= waiting_time) && (EBUSY == os_ret))
    {
        (void)R_OSAL_Internal_MsSleep(milliseconds);
        waiting_time += (size_t)milliseconds;
        os_ret       = pthread_mutex_destroy(&p_thread_control->inner_mtx);
    }

    if (((size_t)OSAL_MUTEX_DESTROY_TIMEOUT <= waiting_time) || (0 != os_ret))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_deinit_mutex_destory()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_deinit_cond_destory()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_cond_destory(st_osal_thread_control_t *p_thread_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_cond_destroy(&p_thread_control->create_cond);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_destroy(&p_thread_control->join_cond);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_deinit_cond_destory()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_deinit_pre_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_deinit_pre_proc(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    size_t                   th_num;
    size_t                   loop_count      = 0;
    st_osal_thread_control_t *thread_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    th_num = R_OSAL_RCFG_GetNumOfThread();

    while ((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_RCFG_GetThreadInfo((uint32_t)loop_count, &thread_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == thread_control))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_Internal_TimedlockMutex(&thread_control->inner_mtx, thread_control->inner_timeout);
            if (OSAL_RETURN_OK == osal_ret)
            {
                if (true == thread_control->active)
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_mutex_unlock(&thread_control->inner_mtx);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        loop_count++;
    }   /* end of while((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_deinit_pre_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_check_create_param()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_check_create_param(const st_osal_thread_config_t *const p_config,
                                                                const osal_thread_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_thread_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_config) || (NULL == p_handle))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((OSAL_THREAD_PRIORITY_LOWEST > p_config->priority) ||
             (OSAL_THREAD_PRIORITY_HIGHEST < p_config->priority) || 
             (0U == p_config->stack_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_check_create_param()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_get_thread_id_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_get_thread_id_info(osal_thread_id_t thread_id,
                                                                osal_thread_handle_t *p_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret = OSAL_RETURN_OK;
    size_t                   th_num;
    size_t                   loop_count      = 0;
    st_osal_thread_control_t *thread_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    th_num = R_OSAL_RCFG_GetNumOfThread();

    while ((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret) && (NULL == *p_control))
    {
        osal_ret = R_OSAL_RCFG_GetThreadInfo((uint32_t)loop_count, &thread_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == thread_control) || (NULL == thread_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (thread_control->usr_config->id == thread_id)
            {
                /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
                *p_control = (osal_thread_handle_t)thread_control;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }   /* end of while((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret) && (NULL == *p_control)). */

    if ((OSAL_RETURN_OK == osal_ret) && (NULL == *p_control))
    {
        osal_ret = OSAL_RETURN_ID;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_get_thread_id_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_set_create_handle()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_set_create_handle(osal_thread_handle_t control,
                                                               const st_osal_thread_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_Internal_TimedlockMutex(&control->inner_mtx, control->inner_timeout);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_thread_is_initialized)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            /* Do nothing */
        }

        if ((OSAL_RETURN_OK == osal_ret) && (false != control->active))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            control->func    = p_config->func;
            control->userarg = p_config->userarg;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            control->thread_ret = 0;

            os_ret = pthread_cond_signal(&control->create_cond);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                control->handle_id = (uint64_t)OSAL_THREAD_HANDLE_ID;
                control->active    = true;
            }
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&control->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret        = OSAL_RETURN_FAIL;
            control->active = false;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_set_create_handle()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_cond_wait_for_join()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_cond_wait_for_join(osal_thread_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true == handle->active)
    {
        if (false == handle->parent_thread)
        {
            handle->parent_thread = true;

            os_ret = pthread_cond_wait(&handle->join_cond, &handle->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                handle->parent_thread = false;
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(true == handle->active). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_th_cond_wait_for_join()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_th_signal_for_join_cond()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_th_signal_for_join_cond(st_osal_thread_control_t *p_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_error;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    local_error = R_OSAL_Internal_TimedlockMutex(&p_control->inner_mtx, p_control->inner_timeout);
    if (OSAL_RETURN_OK == local_error)
    {
        p_control->active = false;

        if (false != p_control->parent_thread)
        {
            os_ret = pthread_cond_signal(&p_control->join_cond);
            if (0 != os_ret)
            {
                local_error = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&p_control->inner_mtx);
        if (0 != os_ret)
        {
            local_error = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        local_error = OSAL_RETURN_FAIL;
    }   /* end of if(OSAL_RETURN_OK == local_error). */

    return local_error;
}
/***********************************************************************************************************************
* End of function osal_internal_th_signal_for_join_cond()
***********************************************************************************************************************/

