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
* File Name :    r_osal_threadsync.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper Threadsync Manager for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <pthread.h>
#include <mcos/atomic.h>

#include "target/emcos/r_osal_common.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_ThreadSync ThreadSync Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_threadsync_is_initialized
 * Initialize flag
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_threadsync_is_initialized = false;

/** @} */

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Functions_ThreadSync ThreadSync Manager Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Initialize Thread Synchronization Manager (mutex).
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Thread Synchronization Manager (mutex).
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Thread Synchronization Manager (mutex).
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Initialize Thread Synchronization Manager (condition).
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Thread Synchronization Manager (condition).
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Thread Synchronization Manager (condition).
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set mutex attributes.
* @param[in]        p_mutex_attr    The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Internal initialize cond object process.
* @param[in]        p_mutex_attr   The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_initialize_proc(const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Check if exclusive control can be performed with pthread_mutex_trylock.
* @param[in]        mutex_control    The pointor of st_osal_mutex_control_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_trylock(st_osal_mutex_control_t *mutex_control);

/*******************************************************************************************************************//**
* @brief            Gets the information for the specified mutex id.
* @param[in]        mutex_id     The value of mutex id.
* @param[in]        p_control    The pointor of osal_mutex_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_ID
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_mutex_id_info(osal_mutex_id_t mutex_id, osal_mutex_handle_t *p_control);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for R_OSAL_ThsyncMutexLockUntilTimeStamp.
* @param[in]        handle        The pointor of osal_mutex_handle_t.
* @param[in]        p_time_stamp  Set the pointer address of st_osal_time_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_mutex_untiltime(osal_mutex_handle_t const handle,
                                                                const st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Mutex processing on interrupt context.
* @param[in]        handle    The pointor of osal_mutex_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_lock_is_isr(osal_mutex_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Mutex processing on normal context.
* @param[in]        handle        The pointor of osal_mutex_handle_t.
* @param[in]        p_time_stamp  The value of time to wait for mutex unblocked.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_lock_not_isr(osal_mutex_handle_t handle,
                                                             const st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Gets the information for the specified condition id.
* @param[in]        cond_id      The value of condition id.
* @param[in]        p_control    The pointor of osal_cond_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_ID
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_cond_id_info(osal_cond_id_t cond_id, osal_cond_handle_t *p_control);

/*******************************************************************************************************************//**
* @brief            Internal R_OSAL_ThsyncCondDestroy process.
* @param[in]        handle    The pointor of osal_cond_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_destroy(osal_cond_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Reinitialize the condition variable.
* @param[in]        handle    The pointor of osal_cond_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_reinit(osal_cond_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for R_OSAL_ThsyncCondWaitForTimePeriod.
* @param[in]        cond_handle     The pointor of osal_cond_handle_t.
* @param[in]        mutex_handle    The pointor of osal_mutex_handle_t.
* @param[in]        time_period     The value of time to wait for condition variable unblocked.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_cond_timeperiod(osal_cond_handle_t const cond_handle,
                                                                osal_mutex_handle_t const mutex_handle,
                                                                osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for R_OSAL_ThsyncCondWaitUntilTimeStamp.
* @param[in]        cond_handle     The pointor of osal_cond_handle_t.
* @param[in]        mutex_handle    The pointor of osal_mutex_handle_t.
* @param[in]        p_time_stamp    Set the pointer address of st_osal_time_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_cond_untiltime(osal_cond_handle_t const cond_handle,
                                                               osal_mutex_handle_t const mutex_handle,
                                                               const st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Count up the number of condition variables used.
* @param[in]        cond_handle    The pointor of osal_cond_handle_t.
* @param[in]        p_timespec     The value of time to wait for mutex unblocked.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_use_countup(osal_cond_handle_t cond_handle,
                                                           const struct timespec *p_timespec);

/*******************************************************************************************************************//**
* @brief            Internal pthread_cond_signal process.
* @param[in]        handle    The pointor of osal_cond_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_signal(osal_cond_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Internal pthread_cond_broadcast process.
* @param[in]        handle     The pointor of osal_cond_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_broadcast(osal_cond_handle_t handle);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncMutexCreate()
* XOS3_OSAL_EMCOS_CD_CD_THS_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_022]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_023]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexCreate(osal_mutex_id_t mutex_id, osal_mutex_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    osal_mutex_handle_t control  = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
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
        osal_ret = osal_internal_get_mutex_id_info(mutex_id, &control);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2812 1 # No problem because it is checked with osal_internal_get_mutex_id_info function. */
        osal_ret = R_OSAL_Internal_TimedlockMutex(&control->mutex, control->inner_timeout);
        if (OSAL_RETURN_OK == osal_ret)
        {
            if (false == control->active)
            {
                control->active    = true;
                control->handle_id = (uint64_t)OSAL_MUTEX_HANDLE_ID;
                
            }
            else
            {
                osal_ret = OSAL_RETURN_BUSY;
            }

            os_ret = pthread_mutex_unlock(&control->mutex);
            if (0 != os_ret)
            {
                control->active    = false;
                control->handle_id = OSAL_INVALID_USER_HANDLE_ID;
                osal_ret           = OSAL_RETURN_FAIL;
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

    if (OSAL_RETURN_OK != osal_ret)
    {
        if ((OSAL_RETURN_BUSY != osal_ret) && (NULL != control))
        {
            *p_handle = OSAL_MUTEX_HANDLE_INVALID;
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
* End of function R_OSAL_ThsyncMutexCreate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncMutexDestroy()
* XOS3_OSAL_EMCOS_CD_CD_THS_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_024]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexDestroy(osal_mutex_handle_t handle)
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
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        os_ret  = pthread_mutex_trylock(&handle->mutex);
        if ((EBUSY == os_ret) || (EAGAIN == os_ret))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (true == handle->active)
            {
                handle->active    = false;
                handle->handle_id = OSAL_INVALID_USER_HANDLE_ID;
            }
            else
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            os_ret = pthread_mutex_unlock(&handle->mutex);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if((EBUSY == os_ret) || (EAGAIN == os_ret)). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncMutexDestroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncMutexTryLock()
* XOS3_OSAL_EMCOS_CD_CD_THS_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_027]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexTryLock(osal_mutex_handle_t handle)
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
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        os_ret  = pthread_mutex_trylock(&handle->mutex);
        if ((EBUSY == os_ret) || (EAGAIN == os_ret))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Check if the block is released after the de-initialization. */
            if (false == gs_osal_threadsync_is_initialized)
            {
                osal_ret = OSAL_RETURN_STATE;
                (void)pthread_mutex_unlock(&handle->mutex);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (false == handle->active))
            {
                osal_ret = OSAL_RETURN_HANDLE;
                (void)pthread_mutex_unlock(&handle->mutex);
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if((EBUSY == os_ret) || (EAGAIN == os_ret)). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncMutexTryLock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncMutexLockUntilTimeStamp()
* XOS3_OSAL_EMCOS_CD_CD_THS_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_026]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexLockUntilTimeStamp (osal_mutex_handle_t handle,
                                                      const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_check_mutex_untiltime(handle, p_time_stamp);
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_get_value = R_OSAL_Internal_GetThreadSpecific();

        /* Is ISR context. */
        if (NULL != p_get_value)
        {
            osal_ret = osal_internal_mutex_lock_is_isr(handle);
        }
        else
        {
            osal_ret = osal_internal_mutex_lock_not_isr(handle, p_time_stamp);
        }
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_threadsync_is_initialized)
        {
            osal_ret = OSAL_RETURN_STATE;
            (void)pthread_mutex_unlock(&handle->mutex);
        }
        else
        {
            /* Do nothing */
        }

        if ((OSAL_RETURN_OK == osal_ret) && (false == handle->active))
        {
            osal_ret = OSAL_RETURN_HANDLE;
            (void)pthread_mutex_unlock(&handle->mutex);
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncMutexLockUntilTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncMutexLockForTimePeriod()
* XOS3_OSAL_EMCOS_CD_CD_THS_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_025]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexLockForTimePeriod(osal_mutex_handle_t handle,
                                                    osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret;
    const void       *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if (0 > time_period)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2996,2992 21 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        p_get_value = R_OSAL_Internal_GetThreadSpecific();

        /* Is ISR context. */
        if (NULL != p_get_value)
        {
            osal_ret = osal_internal_mutex_lock_is_isr(handle);
        }
        else
        {
            osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->mutex, time_period);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check if the block is released after the de-initialization. */
            if (false == gs_osal_threadsync_is_initialized)
            {
                osal_ret = OSAL_RETURN_STATE;
                (void)pthread_mutex_unlock(&handle->mutex);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (false == handle->active))
            {
                osal_ret = OSAL_RETURN_HANDLE;
                (void)pthread_mutex_unlock(&handle->mutex);
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncMutexLockForTimePeriod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncMutexUnlock()
* XOS3_OSAL_EMCOS_CD_CD_THS_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_028]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexUnlock(osal_mutex_handle_t handle)
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
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if (false == handle->active)
    {
        osal_ret = OSAL_RETURN_HANDLE;
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
        os_ret  = pthread_mutex_unlock(&handle->mutex);
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
* End of function R_OSAL_ThsyncMutexUnlock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncCondCreate()
* XOS3_OSAL_EMCOS_CD_CD_THS_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_013]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_014]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondCreate(osal_cond_id_t cond_id, osal_cond_handle_t* const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                os_ret;
    osal_cond_handle_t control  = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
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
        osal_ret = osal_internal_get_cond_id_info(cond_id, &control);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2812 1 # No problem because it is checked with osal_internal_get_cond_id_info function. */
        osal_ret = R_OSAL_Internal_TimedlockMutex(&control->inner_mtx, control->inner_timeout);
        if (OSAL_RETURN_OK == osal_ret)
        {
            if (false == control->active)
            {
                control->active    = true;
                control->handle_id = (uint64_t)OSAL_COND_HANDLE_ID;
                
            }
            else
            {
                osal_ret = OSAL_RETURN_BUSY;
            }

            os_ret = pthread_mutex_unlock(&control->inner_mtx);
            if (0 != os_ret)
            {
                control->active    = false;
                control->handle_id = OSAL_INVALID_USER_HANDLE_ID;
                osal_ret           = OSAL_RETURN_FAIL;
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

    if (OSAL_RETURN_OK != osal_ret)
    {
        if ((OSAL_RETURN_BUSY != osal_ret) && (NULL != control))
        {
            *p_handle = OSAL_COND_HANDLE_INVALID;
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
* End of function R_OSAL_ThsyncCondCreate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncCondDestroy()
* XOS3_OSAL_EMCOS_CD_CD_THS_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_015]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_016]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondDestroy(osal_cond_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_COND_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_cond_destroy(handle);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncCondDestroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncCondWaitForTimePeriod()
* XOS3_OSAL_EMCOS_CD_CD_THS_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_019]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondWaitForTimePeriod (osal_cond_handle_t cond_handle,
                                                    osal_mutex_handle_t mutex_handle,
                                                    osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec abs_time;
    osal_milli_sec_t time_period_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    time_period_tmp = time_period;
    osal_ret = osal_internal_check_cond_timeperiod(cond_handle, mutex_handle, time_period_tmp);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_GetTimestamp(&abs_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
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
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Checke if the user's mutex object is locked. */
        os_ret = pthread_mutex_trylock(&mutex_handle->mutex);
        if (0 == os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)pthread_mutex_unlock(&mutex_handle->mutex);
        }
        else if (EBUSY == os_ret)
        {
            osal_ret = OSAL_RETURN_OK;
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_cond_use_countup(cond_handle, &abs_time);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_timedwait(&cond_handle->cond, &mutex_handle->mutex, &abs_time);
        switch (os_ret)
        {
            case 0:
                /* Do nothing */
                break;
            case ETIMEDOUT:
                osal_ret = OSAL_RETURN_TIME;
                break;
            default:
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }

        cond_handle->use_count--;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncCondWaitForTimePeriod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncCondWaitUntilTimeStamp()
* XOS3_OSAL_EMCOS_CD_CD_THS_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_020]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondWaitUntilTimeStamp (osal_cond_handle_t cond_handle,
                                                     osal_mutex_handle_t mutex_handle,
                                                     const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec time_spec;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_check_cond_untiltime(cond_handle, mutex_handle, p_time_stamp);
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
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Checke if the user's mutex object is locked. */
        os_ret = pthread_mutex_trylock(&mutex_handle->mutex);
        if (0 == os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)pthread_mutex_unlock(&mutex_handle->mutex);
        }
        else if (EBUSY == os_ret)
        {
            osal_ret = OSAL_RETURN_OK;
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_cond_use_countup(cond_handle, &time_spec);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_timedwait(&cond_handle->cond, &mutex_handle->mutex, &time_spec);
        switch (os_ret)
        {
            case 0:
                /* Do nothing */
                break;
            case ETIMEDOUT:
                osal_ret = OSAL_RETURN_TIME;
                break;
            default:
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }

        cond_handle->use_count--;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncCondWaitUntilTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncCondSignal()
* XOS3_OSAL_EMCOS_CD_CD_THS_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_017]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_018]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondSignal(osal_cond_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_COND_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_cond_signal(handle);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncCondSignal()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncCondBroadcast()
* XOS3_OSAL_EMCOS_CD_CD_THS_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_011]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_012]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondBroadcast(osal_cond_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_COND_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_cond_broadcast(handle);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncCondBroadcast()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ThsyncMemoryBarrier()
* XOS3_OSAL_EMCOS_CD_CD_THS_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_021]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMemoryBarrier(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true != gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        atomic_thread_fence(memory_order_seq_cst);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ThsyncMemoryBarrier()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_threadsync_initialize()
* XOS3_OSAL_EMCOS_CD_CD_THS_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_009]
***********************************************************************************************************************/
e_osal_return_t osal_threadsync_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false != gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_threadsync_is_initialized = true;

        osal_ret = osal_internal_cond_initialize();
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mutex_initialize();
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)osal_threadsync_deinitialize();
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(false != gs_osal_threadsync_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_threadsync_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_threadsync_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_THS_015
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_007]
***********************************************************************************************************************/
e_osal_return_t osal_threadsync_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_threadsync_is_initialized = false;

    osal_ret = osal_internal_cond_deinitialize();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mutex_deinitialize();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_threadsync_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_threadsync_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_THS_016
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_008]
***********************************************************************************************************************/
e_osal_return_t osal_threadsync_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true != gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_threadsync_is_initialized = false;

        osal_ret = osal_internal_cond_deinitialize_pre();
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mutex_deinitialize_pre();
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_BUSY == osal_ret)
    {
        gs_osal_threadsync_is_initialized = true;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_threadsync_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_threadsync_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_THS_017
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_010]
***********************************************************************************************************************/
void osal_threadsync_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_threadsync_is_initialized = true;

    return;
}
/***********************************************************************************************************************
* End of function osal_threadsync_set_is_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mutex_initialize()
* XOS3_OSAL_EMCOS_CD_CD_THS_018
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_006]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    size_t                  mutex_num;
    size_t                  loop_count     = 0;
    st_osal_mutex_control_t *mutex_control = NULL;
    pthread_mutexattr_t     mutex_attr;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                     os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_set_mutex_attr(&mutex_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        mutex_num = R_OSAL_RCFG_GetNumOfMutex();
        if (0U == mutex_num)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* Do nothing */
        }

        while ((OSAL_RETURN_OK == osal_ret) && (mutex_num > loop_count))
        {
            osal_ret = R_OSAL_RCFG_GetMutexInfo((uint32_t)loop_count, &mutex_control);
            if ((OSAL_RETURN_OK == osal_ret) && (NULL != mutex_control))
            {
                /* PRQA S 2736 1 # No problem because osal_mutex_deinitialize is called pthread_mutex_destroy. */
                os_ret = pthread_mutex_init(&mutex_control->mutex, &mutex_attr);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    mutex_control->active = false;
                }
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            loop_count++;
        }   /* end of while((OSAL_RETURN_OK == osal_ret) && (mutex_num > loop_count)). */

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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mutex_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mutex_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_THS_019
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_004]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret       = OSAL_RETURN_OK;
    size_t                  mutex_num;
    size_t                  loop_count     = 0;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                     os_ret;
    st_osal_mutex_control_t *mutex_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    mutex_num = R_OSAL_RCFG_GetNumOfMutex();

    while ((mutex_num > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_RCFG_GetMutexInfo((uint32_t)loop_count, &mutex_control);
        if ((OSAL_RETURN_OK == osal_ret) && (NULL != mutex_control))
        {
            os_ret = pthread_mutex_destroy(&mutex_control->mutex);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                mutex_control->handle_id = OSAL_INVALID_USER_HANDLE_ID;
                mutex_control->active    = false;
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        loop_count++;
    }   /* end of while((mutex_num > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mutex_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mutex_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_THS_020
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_005]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret       = OSAL_RETURN_OK;
    size_t                  mutex_num;
    size_t                  loop_count     = 0;
    st_osal_mutex_control_t *mutex_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    mutex_num = R_OSAL_RCFG_GetNumOfMutex();

    while ((mutex_num > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_RCFG_GetMutexInfo((uint32_t)loop_count, &mutex_control);
        if ((OSAL_RETURN_OK == osal_ret) && (NULL != mutex_control))
        {
            /* Do nothing */
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_check_trylock(mutex_control);
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }   /* end of while((mutex_num > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mutex_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_initialize()
* XOS3_OSAL_EMCOS_CD_CD_THS_021
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_003]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                    os_ret;
    pthread_mutexattr_t    mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_set_mutex_attr(&mutex_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_cond_initialize_proc(&mutex_attr);
        os_ret = pthread_mutexattr_destroy(&mutex_attr);
        if ((OSAL_RETURN_OK == osal_ret) && (os_ret != 0))
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
* End of function osal_internal_cond_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_THS_022
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_001]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret      = OSAL_RETURN_OK;
    st_osal_cond_control_t *cond_control = NULL;
    size_t                 cond_num;
    size_t                 loop_count    = 0;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                    os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    cond_num = R_OSAL_RCFG_GetNumOfCond();

    while ((cond_num > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_RCFG_GetCondInfo((uint32_t)loop_count, &cond_control);
        if ((OSAL_RETURN_OK == osal_ret) && (NULL != cond_control))
        {
            osal_ret = R_OSAL_Internal_TimedlockMutex(&cond_control->inner_mtx, cond_control->inner_timeout);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                os_ret = pthread_cond_destroy(&cond_control->cond);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_mutex_unlock(&cond_control->inner_mtx);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of if(OSAL_RETURN_OK != osal_ret). */

            if (OSAL_RETURN_OK == osal_ret)
            {
                os_ret = pthread_mutex_destroy(&cond_control->inner_mtx);
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
                cond_control->handle_id  = OSAL_INVALID_USER_HANDLE_ID;
                cond_control->active     = false;
                cond_control->use_count  = 0U;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }   /* end of if((OSAL_RETURN_OK == osal_ret) && (NULL != cond_control)) */
        loop_count++;
    }   /* end of while((cond_num > loop_count) && (OSAL_RETURN_OK == osal_ret)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_THS_023
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_002]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret      = OSAL_RETURN_OK;
    st_osal_cond_control_t *cond_control = NULL;
    size_t                 cond_num;
    size_t                 loop_count    = 0;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                    os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    cond_num = R_OSAL_RCFG_GetNumOfCond();

    while ((cond_num > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_RCFG_GetCondInfo((uint32_t)loop_count, &cond_control);
        if ((OSAL_RETURN_OK == osal_ret) && (NULL != cond_control))
        {
            osal_ret = R_OSAL_Internal_TimedlockMutex(&cond_control->inner_mtx, cond_control->inner_timeout);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                if (0U == cond_control->use_count)
                {
                    /* Do nothing */
                }
                else
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }

                os_ret = pthread_mutex_unlock(&cond_control->inner_mtx);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of if(OSAL_RETURN_OK != osal_ret). */
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }   /* end of if((OSAL_RETURN_OK == osal_ret) && (NULL != cond_control)). */
        loop_count++;
    }   /* end of while((cond_num > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_set_mutex_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr)
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
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
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
* End of function osal_internal_set_mutex_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_initialize_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_initialize_proc(const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret      = OSAL_RETURN_OK;
    size_t                 cond_num;
    size_t                 loop_count    = 0;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                    os_ret;
    st_osal_cond_control_t *cond_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    cond_num = R_OSAL_RCFG_GetNumOfCond();
    if (0U == cond_num)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    while ((OSAL_RETURN_OK == osal_ret) && (cond_num > loop_count))
    {
        osal_ret = R_OSAL_RCFG_GetCondInfo((uint32_t)loop_count, &cond_control);
        if ((OSAL_RETURN_OK == osal_ret) && (NULL != cond_control))
        {
            os_ret = pthread_cond_init(&cond_control->cond, NULL);
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
                /* PRQA S 2736 1 # No problem because osal_internal_cond_deinitialize is called pthread_mutex_destroy. */
                os_ret = pthread_mutex_init(&cond_control->inner_mtx, p_mutex_attr);
                if (os_ret != 0)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    cond_control->active    = false;
                    cond_control->use_count = 0U;
                }
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }   /* end of if((OSAL_RETURN_OK == osal_ret) && (NULL != cond_control)) */
        loop_count++;
    }   /* end of while((OSAL_RETURN_OK == osal_ret) && (cond_num > loop_count)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_initialize_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_check_trylock()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_trylock(st_osal_mutex_control_t *mutex_control)
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
    os_ret = pthread_mutex_trylock(&mutex_control->mutex);
    if ((EBUSY == os_ret) || (EAGAIN == os_ret))
    {
        osal_ret = OSAL_RETURN_BUSY;
    }
    else if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutex_unlock(&mutex_control->mutex);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_check_trylock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_get_mutex_id_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_mutex_id_info(osal_mutex_id_t mutex_id, osal_mutex_handle_t *p_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret       = OSAL_RETURN_OK;
    size_t                  mutex_num;
    size_t                  loop_count     = 0;
    st_osal_mutex_control_t *mutex_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    mutex_num = R_OSAL_RCFG_GetNumOfMutex();

    while ((mutex_num > loop_count) && (OSAL_RETURN_OK == osal_ret) && (NULL == *p_control))
    {
        osal_ret = R_OSAL_RCFG_GetMutexInfo((uint32_t)loop_count, &mutex_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == mutex_control) || (NULL == mutex_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (mutex_control->usr_config->id == mutex_id)
            {
                /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
                *p_control = (osal_mutex_handle_t)mutex_control;
            }
            else
            {
                /* Do nothing */
            }
        }
        loop_count++;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == *p_control)
        {
            osal_ret = OSAL_RETURN_ID;
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
* End of function osal_internal_get_mutex_id_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_check_mutex_untiltime()
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_check_mutex_untiltime(osal_mutex_handle_t const handle,
                                                                const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem. 
       True if the correct handle was passed. False detection of the tool. */
    else if (OSAL_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
      True if the correct handle was passed. False detection of the tool. */
    else if (NULL == p_time_stamp)
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_check_mutex_untiltime()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mutex_lock_is_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_lock_is_isr(osal_mutex_handle_t handle)
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
    os_ret = pthread_mutex_trylock(&handle->mutex);
    if ((EBUSY == os_ret) || (EAGAIN == os_ret))
    {
        osal_ret = OSAL_RETURN_BUSY;
    }
    else if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mutex_lock_is_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mutex_lock_not_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mutex_lock_not_isr(osal_mutex_handle_t handle,
                                                             const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec time_spec;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    time_spec.tv_sec  = p_time_stamp->tv_sec;
    time_spec.tv_nsec = p_time_stamp->tv_nsec;

    /* The minimum unit of time is 1 milli second. */
    /* Digits less than 1 milli second is rounded up to 1 milli second. */
    osal_ret = R_OSAL_Internal_TimeRoundUp(&time_spec);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutex_timedlock(&handle->mutex, &time_spec);
        if (ETIMEDOUT == os_ret)
        {
            osal_ret = OSAL_RETURN_TIME;
        }
        else if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mutex_lock_not_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_get_cond_id_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_cond_id_info(osal_cond_id_t cond_id, osal_cond_handle_t *p_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret      = OSAL_RETURN_OK;
    size_t                 cond_num;
    size_t                 loop_count    = 0;
    st_osal_cond_control_t *cond_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    cond_num = R_OSAL_RCFG_GetNumOfCond();

    while ((OSAL_RETURN_OK == osal_ret) && (cond_num > loop_count) && (NULL == *p_control))
    {
        osal_ret = R_OSAL_RCFG_GetCondInfo((uint32_t)loop_count, &cond_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == cond_control) || (NULL == cond_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (cond_control->usr_config->id == cond_id)
            {
                /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
                *p_control = (osal_cond_handle_t)cond_control;
            }
            else
            {
                /* Do nothing */
            }
        }
        loop_count++;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == *p_control)
        {
            osal_ret = OSAL_RETURN_ID;
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
* End of function osal_internal_get_cond_id_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_destroy(osal_cond_handle_t handle)
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
    osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (true == handle->active)
        {
            if (0U == handle->use_count)
            {
                osal_ret = osal_internal_cond_reinit(handle);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    handle->active    = false;
                    handle->handle_id = OSAL_INVALID_USER_HANDLE_ID;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }   /* end of if(true == handle->active). */

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_destroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_reinit()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_reinit(osal_cond_handle_t handle)
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
    os_ret = pthread_cond_destroy(&handle->cond);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_cond_init(&handle->cond, NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_reinit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_check_cond_timeperiod()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_check_cond_timeperiod(osal_cond_handle_t const cond_handle,
                                                                osal_mutex_handle_t const mutex_handle,
                                                                osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL == cond_handle) || (NULL == mutex_handle))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if ((OSAL_COND_HANDLE_ID != cond_handle->handle_id) || (OSAL_MUTEX_HANDLE_ID != mutex_handle->handle_id))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (false == mutex_handle->active)
        {
            osal_ret = OSAL_RETURN_HANDLE;
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
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (0 > time_period)
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_check_cond_timeperiod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_check_cond_untiltime()
***********************************************************************************************************************/
/*PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_check_cond_untiltime(osal_cond_handle_t const cond_handle,
                                                               osal_mutex_handle_t const mutex_handle,
                                                               const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_threadsync_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL == cond_handle) || (NULL == mutex_handle))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if ((OSAL_COND_HANDLE_ID != cond_handle->handle_id) || (OSAL_MUTEX_HANDLE_ID != mutex_handle->handle_id))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (false == mutex_handle->active)
        {
            osal_ret = OSAL_RETURN_HANDLE;
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
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == p_time_stamp)
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
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_check_cond_untiltime()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_use_countup()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_use_countup(osal_cond_handle_t cond_handle, const struct timespec *p_timespec)
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
    os_ret = pthread_mutex_timedlock(&cond_handle->inner_mtx, p_timespec);
    switch (os_ret)
    {
        case 0:
            /* Do nothing */
            break;
        case ETIMEDOUT:
            osal_ret = OSAL_RETURN_TIME;
            break;
        default:
            osal_ret = OSAL_RETURN_FAIL;
            break;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_threadsync_is_initialized)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (true == cond_handle->active)
            {
                cond_handle->use_count++;
            }
            else
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&cond_handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            if (0U < cond_handle->use_count)
            {
                cond_handle->use_count--;
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
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_use_countup()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_signal()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_signal(osal_cond_handle_t handle)
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
    osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (true == handle->active)
        {
            os_ret = pthread_cond_signal(&handle->cond);
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
            osal_ret = OSAL_RETURN_HANDLE;
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
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_signal()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_cond_broadcast()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_cond_broadcast(osal_cond_handle_t handle)
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
    osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (true == handle->active)
        {
            os_ret = pthread_cond_broadcast(&handle->cond);
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
            osal_ret = OSAL_RETURN_HANDLE;
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
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_cond_broadcast()
***********************************************************************************************************************/

