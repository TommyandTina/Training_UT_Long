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
* File Name :    r_osal_res_cfg_if.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configurator Public Header
***********************************************************************************************************************/

#ifndef OSAL_RES_CFG_IF_H
#define OSAL_RES_CFG_IF_H

#if !defined(_STDIO_H_INCLUDED) && !defined(_STDIO_H) && !defined(_STDIO_H_) && !defined(_INC_STDIO) && !defined(__stdio_h)
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
#include <stdio.h>
#else
/* PRQA S 4601 1 # There is no problem because it is the declaration of macro "FILE" when stdio.h is not used. */
#define FILE void
#endif
#endif

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Public global functions
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the Thread manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_ThreadGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the Thread manager.
* @return           void
* @param[in]        status        Initialization status of the thread manager
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_RCFG_ThreadSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes Thread related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function prepare all threads used in 
*                   the environment and resources to control them. (Inner mutex, for example)
*                   For dynamic resource environment, this function only prepare resources to control threads.

* @see              Refer to following the function in this specification
*                   R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadControlInit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes Thread related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function finishes all threads used in the environment and 
*                   destroy resources to control them. (Inner mutex, for example)
*                   For dynamic resource environment, this function only destroy resources to control threads.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadControlDeinit(void);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active Thread resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             This function is used to judge if the thread manager can be deinitialized.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadCheckBusy(void);

/*******************************************************************************************************************//**
* @brief   Create a new thread in the calling process as the configuration specified in p_config.
* @param[in]       p_config    The address of #st_osal_thread_config_t.
* @param[in]       thread_id   The value of thread id.
* @param[in,out]   p_handle    The address of osal_thread_handle_t. The OSAL sets the handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the thread was created successfully.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Specified thread_id is already handled.
*                              If this error occurs, p_handle is not overwritten.
* @retval  OSAL_RETURN_ID      Specified thread_id is unknown.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    In case of calling this API with a config initialized with R_OSAL_ThreadInitializeThreadConfigSt(), 
*          #OSAL_RETURN_PAR is returned. Before calling this API, the config value should be initialized with 
*          valid value. \n
*          If the func and userarg in p_config are NULL, successfully and return #OSAL_RETURN_OK.
*          OSAL does not provide the thread cancellation. Please prepare termination logic in the thread 
*          function. \n
*          If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
*          #OSAL_THREAD_HANDLE_INVALID.
* @see     Refer to following the function in this specification
*          R_OSAL_ThreadCreate
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadCreate(const st_osal_thread_config_t* const p_config,
                                         osal_thread_id_t thread_id,
                                         osal_thread_handle_t* const p_handle);

/*******************************************************************************************************************//**
* @brief   Obtain handle of the calling thread and set that thread to p_handle.
* @param[in,out]   p_handle    The address of osal_thread_handle_t. The OSAL sets current thread handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and succeeded in getting the handle of the calling thread.
* @retval  OSAL_RETURN_FAIL    
*                              - An error occurred in OS or related API.
*                              - Called from the thread which was not created by OSAL.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_ThreadSelf
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSelf(osal_thread_handle_t* const p_handle);

/*******************************************************************************************************************//**
* @brief   Compares two thread handles and set the return value to p_result that is they refer to the same thread or 
*          not.
* @param[in]       handle1     The handle acquired at R_OSAL_ThreadCreate().
* @param[in]       handle2     The handle acquired at R_OSAL_ThreadCreate().
* @param[in,out]   p_result    The result compares the 2 handles.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and two thread handles was compared successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  handle is invalid.
*                              - handle1 or handle2 is not acquired by R_OSAL_ThreadCreate().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_ThreadEqual
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadEqual(osal_thread_handle_t handle1,
                                        osal_thread_handle_t handle2,
                                        bool* const p_result);

/*******************************************************************************************************************//**
* @brief   Wait for the specified by thread to finish.
* @param[in]       handle          The handle acquired at R_OSAL_ThreadCreate().
* @param[in,out]   p_return_value  The address of int64_t for result.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and successfully waited for the specified thread to finish.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE
*                              -# Invalid handle.
*                                  - handle is not acquired by R_OSAL_ThreadCreate().
*                              -# Thread that cannot join.
*                                  - own thread.
*                                  - no corresponding thread.
*                                  - another thread is already joining.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_ThreadJoin
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadJoin(osal_thread_handle_t handle,
                                       int64_t* const p_return_value);

/*******************************************************************************************************************//**
* @brief   Suspend the execution of the current thread for specified by time_period.
* @param[in]   time_period To set the value for sleep time by msec. (valid values are >= 0)
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the thread was suspended successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_ThreadSleepForTimePeriod
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSleepForTimePeriod(osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief   Suspend the execution of the current thread until an absolute time value specified by p_time_stamp is 
*          reached. \n
*          The value of time stamp should be acquired by R_OSAL_ClockTimeGetTimeStamp().
* @param[in]   p_time_stamp    The pointer addresses of #st_osal_time_t that holds the timestamp on which 
*                              the thread will continue.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK          All arguments are valid, and the thread was suspended successfully.
* @retval  OSAL_RETURN_FAIL        An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE       Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to 
*          1 milli second. If the time stamp is past time, use the current time stamp. \n
*          The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_ThreadSleepUntilTimeStamp
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the Thread Synchronization manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_ThsyncGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the Thread Synchronization manager.
* @param[in]        status        Initialization status of the thread synchronization manager
* @return           void
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_RCFG_ThsyncSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes Thread Synchronization related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function prepare all mutexes used in the environment 
*                   and resources to control them. (Inner mutex, for example)
*                   For dynamic resource environment, this function only prepare resources to control thread synchronization.
* @see              Refer to following the function in this specification
*                   R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncControlInit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes Thread Synchronization related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function destroys all mutexes used in the environment
*                   and resources to control them. (Inner mutex, for example)
*                    For dynamic resource environment, this function only destroy resources to control thread synchronization.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncControlDeinit(void);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active Thread Synchronization resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             This function is used to judge if the thread synchronization manager can be deinitialized.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncCheckBusy(void);

/*******************************************************************************************************************//**
* @brief   Create a new mutex assigned to mutex_id and set acquired mutex handle to p_handle.
* @param[in]       mutex_id    The value of mutex id.
* @param[in,out]   p_handle    The address of osal_mutex_handle_t. The OSAL sets the handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was created successfully.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Specified mutex_id is already handled. 
*                              If this error occurs, p_handle is not overwritten.
* @retval  OSAL_RETURN_ID      Specified mutex_id is unknown.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The initial state of the mutex is unlocked. \n
*          If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
*          #OSAL_MUTEX_HANDLE_INVALID.
* @see     Refer to following the function in this specification
*          R_OSAL_MutexCreate
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexCreate(osal_mutex_id_t mutex_id,
                                        osal_mutex_handle_t *const p_handle);

/*******************************************************************************************************************//**
* @brief   Destroy mutex assigned to handle.
* @param[in]   handle  The handle acquired at R_OSAL_ThsyncMutexCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was destroyed successfully.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    The mutex for the specified handle is in use.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncMutexCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
* @note    After calling this function, the handle is invalid and cannot be used.
* @see     Refer to following the function in this specification
*          R_OSAL_MutexDestroy
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexDestroy(osal_mutex_handle_t handle);

/*******************************************************************************************************************//**
* @brief   Lock a mutex assigned to handle with timeout for specified time. \n
*          The function blocks the current thread until the mutex assigned to mutex handle is locked by the current 
*          thread or until the time period has been elapsed.
* @param[in]   handle      The handle acquired at R_OSAL_ThsyncMutexCreate().
* @param[in]   time_period Sets the value of time to wait for the mutex is unlocked, in msec. (valid values are >= 0)
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was locked successfully.
* @retval  OSAL_RETURN_TIME    Could not lock within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Failed to lock Mutex. This error occurs only in an interrupt context.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncMutexCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The waiting time may be longer than specified due to the influence of other threads. \n
*          In case of the mutex is already locked, the calling thread is blocked until the mutex becomes available. \n
*          In case of the mutex cannot be locked, blocking is terminated when the specified time has passed.
* @see     Refer to following the function in this specification
*          R_OSAL_MutexLockForTimePeriod
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexLockForTimePeriod(osal_mutex_handle_t handle,
                                                   osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief   Lock a mutex assigned to handle with timeout until time stamp. \n
*          The function blocks the current thread until the mutex assigned to mutex handle is locked by the current 
*          thread or until the given absolute time value has been reached. \n
*          The value of time stamp should be acquired by R_OSAL_ClockTimeGetTimeStamp().
* @param[in]   handle          The handle acquired at R_OSAL_ThsyncMutexCreate().
* @param[in]   p_time_stamp    Set the pointer address of #st_osal_time_t as timestamp that wait until 
*                              mutex is unlocked.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was locked successfully.
* @retval  OSAL_RETURN_TIME    Could not lock within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Failed to lock Mutex. This error occurs only in an interrupt context.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncMutexCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to 
*          1 milli second. If the time stamp is past time, use the current time stamp. \n
*          The waiting time may be longer than specified due to the influence of other threads. \n
*          In case of the mutex is already locked, the calling thread is blocked until the mutex becomes available. \n
*          In case of the mutex cannot be locked, blocking is terminated when the specified time stamp has been 
*          reached.
* @see     Refer to following the function in this specification
*          R_OSAL_MutexLockUntilTimeStamp
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexLockUntilTimeStamp(osal_mutex_handle_t handle,
                                                    const st_osal_time_t* const p_time_stamp);

/*******************************************************************************************************************//**
* @brief   Lock a mutex assigned to handle.
* @param[in]   handle  The handle acquired at R_OSAL_ThsyncMutexCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was locked successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    The mutex is in use.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncMutexCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    In case of the mutex is not locked, it is locked and owned by the calling thread. \n
*          In case of the mutex is already locked, return #OSAL_RETURN_BUSY and exit immediately.
* @see     Refer to following the function in this specification
*          R_OSAL_MutexTryLock
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexTryLock(osal_mutex_handle_t handle);

/*******************************************************************************************************************//**
* @brief   Unlock mutex assigned to handle.
* @param[in]   handle  The handle acquired at R_OSAL_ThsyncMutexCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was unlocked successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncMutexCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MutexUnlock
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexUnlock(osal_mutex_handle_t handle);

/*******************************************************************************************************************//**
* @brief   Create the condition variable assigned to cond_id and set acquired cond handle to p_handle.
* @param[in]       cond_id     The value of condition variable id.
* @param[in,out]   p_handle    The address of osal_cond_handle_t. The OSAL sets the handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the condition variable was created successfully.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Specified cond_id is already handled. 
*                              If this error occurs, p_handle is not overwritten.
* @retval  OSAL_RETURN_ID      Specified cond_id is unknown.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
*          #OSAL_COND_HANDLE_INVALID.
* @see     Refer to following the function in this specification
*          R_OSAL_CondCreate
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondCreate(osal_cond_id_t cond_id,
                                       osal_cond_handle_t* const p_handle);

/*******************************************************************************************************************//**
* @brief   Destroy condition variable assigned to handle.
* @param[in]   handle  The handle acquired at R_OSAL_ThsyncCondCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the condition variable was destroyed successfully.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY
*                              -# The specified handle is Active.
*                              -# The condition variable for the specified handle is in use.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncCondCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncCondDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    After calling this function, the handle is invalid and cannot be used further.
* @see     Refer to following the function in this specification
*          R_OSAL_CondDestroy
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondDestroy(osal_cond_handle_t handle);

/*******************************************************************************************************************//**
* @brief   Wait on a condition variable with timeout for specified time. \n
*          This function blocks the calling thread on the condition variable and unlocks the associated mutex. \n
*          The calling thread is blocked until either it is unblocked from other thread or the period time has passed.
*           \n Return from this function, the mutex is re-locked and owned by the calling thread.
* @param[in]   cond_handle     The handle acquired at R_OSAL_ThsyncCondCreate().
* @param[in]   mutex_handle    The handle acquired at R_OSAL_ThsyncMutexCreate().
* @param[in]   time_period     Sets the value of time to wait for condition variable unblocked, in msec.
*                              (valid values are >= 0)
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the condition variable was waited successfully.
* @retval  OSAL_RETURN_TIME    Not unblocked within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - cond_handle was not acquired by R_OSAL_ThsyncCondCreate().
*                              - mutex_handle was not acquired by R_OSAL_ThsyncMutexCreate().
*                              - cond_handle was already destroyed by R_OSAL_ThsyncCondDestroy().
*                              - mutex_handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    This function works on the assumption that the mutex is locked before it is called.
*          The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_CondWaitForTimePeriod
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondWaitForTimePeriod(osal_cond_handle_t cond_handle,
                                                  osal_mutex_handle_t mutex_handle, 
                                                  osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief   Wait on a condition variable with timeout until time stamp. \n
*          This function blocks the calling thread on the condition variable and unlocks the associated mutex. \n
*          The calling thread is blocked until either unblocked from other thread or the time stamp is reached. \n
*          The value of time stamp should be acquired by R_OSAL_ClockTimeGetTimeStamp(). \n
*          Return from this function, the mutex is re-locked and owned by the calling thread.
* @param[in]   cond_handle     The handle acquired at R_OSAL_ThsyncCondCreate().
* @param[in]   mutex_handle    The handle acquired at R_OSAL_ThsyncMutexCreate().
* @param[in]   p_time_stamp    Set the pointer address of #st_osal_time_t as timestamp that wait until
*                              condition variable unblocked.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the condition variable was waited successfully.
* @retval  OSAL_RETURN_TIME    Not unblocked within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - cond_handle was not acquired by R_OSAL_ThsyncCondCreate().
*                              - mutex_handle was not acquired by R_OSAL_ThsyncMutexCreate().
*                              - cond_handle was already destroyed by R_OSAL_ThsyncCondDestroy().
*                              - mutex_handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    This function works on the assumption that the mutex is locked before it is called.
*          The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to 
*          1 milli second. If the time stamp is past time, use the current time stamp.
*          The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_CondWaitUntilTimeStamp
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondWaitUntilTimeStamp(osal_cond_handle_t cond_handle,
                                                   osal_mutex_handle_t mutex_handle,
                                                   const st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief   Unblock one thread that's waiting on a condition variable assigned to the specified handle.
* @param[in]   handle  The handle acquired at R_OSAL_ThsyncCondCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the one threads that's waiting on a condition variable 
*                              was unblocked successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncCondCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncCondDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    Before calling this function, lock the mutex. And after calling this function, unlock the mutex.
* @see     Refer to following the function in this specification
*          R_OSAL_CondSignal
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondSignal(osal_cond_handle_t handle);

/*******************************************************************************************************************//**
* @brief   Unblock all threads that's waiting on the condition variable assigned to the specified handle.
* @param[in]   handle  The handle acquired at R_OSAL_ThsyncCondCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the all threads that's waiting on a condition variable 
*                              was unblocked successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_ThsyncCondCreate().
*                              - handle was already destroyed by R_OSAL_ThsyncCondDestroy().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    Before calling this function, lock the mutex. And after calling this function, unlock the mutex.
* @see     Refer to following the function in this specification
*          R_OSAL_CondBroadcast
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondBroadcast(osal_cond_handle_t handle);

/*******************************************************************************************************************//**
* @brief   The OSAL memory barrier guarantee that operations called before the barrier are executed before operations 
*          after the barrier are executed.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      The memory barrier was issued successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    When the memory is shared with multiple devices, such as other CPUs in a multiprocessor system, 
*          or memory-mapped peripherals, out-of-order execution may affect program behavior. \n
*          For example, a second CPU may see memory changes made by the first CPU in a sequence which differs from 
*          program order. This applies to two or more software threads running in a single process whereby these 
*          multiple software threads share a single memory space. \n
*          Multiple software threads, within a single process, may run concurrently in a multi-core processor 
*          environment. Together with out-of-order execution this may lead to unpredictable behavior of the system.
* @see     Refer to following the function in this specification
*          R_OSAL_MemoryBarrier
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MemoryBarrier(void);

/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the Message Queue Manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_MqGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the Message Queue Manager.
* @return           void
* @param[in]        status        Initialization status of the message queue manager
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_RCFG_MqSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes Message related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function prepare all message queues used in the environment 
*                   and resources to control them. (Inner mutex, for example)
*                   For dynamic resource environment, this function only prepare resources to control message queues.
* @see              Refer to following the function in this specification
*                   R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqControlInit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes Message related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function destroys all message queues used in the environment 
*                   and destroy resources to control them. (Inner mutex, for example).
*                   For dynamic resource environment, this function only destroys resources to control message queues.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqControlDeinit(void);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active Message resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             This function is used to judge if the message manager can be deinitialized.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqCheckBusy(void);

/*******************************************************************************************************************//**
* @brief   Create the message queue in the calling process as the mq_Id and configuration specified in p_config.
* @param[in]       p_config    The address of #st_osal_mq_config_t
* @param[in]       mq_Id       The value of message queue id.
* @param[in,out]   p_handle    The address of #osal_mq_handle_t. The OSAL sets the handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the message queue was created successfully.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Specified mq_id is already handled.
*                              If this error occurs, p_handle will not be overwritten.
* @retval  OSAL_RETURN_ID      Specified mq_id is unknown.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    In case of calling this API with a config initialized with R_OSAL_MqInitializeMqConfigSt(), 
*          #OSAL_RETURN_PAR is returned. Before calling this API, the config value should be initialized with valid 
*          value.
*          If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
*          #OSAL_MQ_HANDLE_INVALID.
* @see     Refer to following the function in this specification
*          R_OSAL_MqCreate
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqCreate(const st_osal_mq_config_t * const p_config,
                                     osal_mq_id_t mq_Id,
                                     osal_mq_handle_t * const p_handle);

/*******************************************************************************************************************//**
* @brief   Delete message queue assigned to handle.
* @param[in]   handle  The handle acquired at R_OSAL_MqCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the message queue was deleted successfully.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    The message queue for the specified handle is in use.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    After calling this function, the handle is invalid and cannot be used.
* @see     Refer to following the function in this specification
*          R_OSAL_MqDelete
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqDelete(osal_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief   Send a message to message queue. \n
*          In case of the message queue is full, wait for the time specified by time_period.
* @param[in]   handle      The handle acquired at R_OSAL_MqCreate().
* @param[in]   time_period Sets the value of time to wait for the queue be able to send message,
*                          in msec. (valid values are >= 0)
* @param[in]   p_buffer    The address of void* for the send data.
* @param[in]   buffer_size The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the message was sent successfully to the message queue.
* @retval  OSAL_RETURN_TIME    Could not send message within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Could not send message. This error occurs only interrupt context.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_MqSendForTimePeriod
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqSendForTimePeriod(osal_mq_handle_t handle,
                                                osal_milli_sec_t time_period,
                                                const void* p_buffer,
                                                size_t buffer_size);

/*******************************************************************************************************************//**
* @brief   Send a message to message queue. \n
*          In case of the message queue is full, wait until the time specified by the time stamp is reached. \n
*          The value of time stamp should be acquired by R_OSAL_ClockTimeGetTimeStamp().
* @param[in]   handle          he handle acquired at R_OSAL_MqCreate().
* @param[in]   p_time_stamp    Set the pointer address of #st_osal_time_t as timestamp that wait until
*                              the queue be able to send message.
* @param[in]   p_buffer        The address of void* for the send data.
* @param[in]   buffer_size     The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the message was sent successfully to the message queue.
* @retval  OSAL_RETURN_TIME    Could not send message within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Could not send message. This error occurs only interrupt context.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to 
*          1 milli second. If the time stamp is past time, use the current time stamp.
*          The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_MqSendUntilTimeStamp
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqSendUntilTimeStamp(osal_mq_handle_t handle,
                                                 const st_osal_time_t* const p_time_stamp,
                                                 const void* p_buffer,
                                                 size_t buffer_size);

/*******************************************************************************************************************//**
* @brief   Receive a message from the message queue. \n
*          In case of the message queue is empty, wait for the time specified by time_period.
* @param[in]       handle      The handle acquired at R_OSAL_MqCreate().
* @param[in]       time_period Sets the value of time to wait for the queue be able to receive message, 
*                              in msec. (valid values are >= 0)
* @param[in,out]   p_buffer    The address of void* for the receives data. The OSAL sets the receive data.
* @param[in]       buffer_size The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the message was received successfully from the message 
*                              queue.
* @retval  OSAL_RETURN_TIME    Could not receive message within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Could not receive message. This error occurs only interrupt context.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_MqReceiveForTimePeriod
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqReceiveForTimePeriod(osal_mq_handle_t handle,
                                                   osal_milli_sec_t time_period,
                                                   void* p_buffer,
                                                   size_t buffer_size);

/*******************************************************************************************************************//**
* @brief   Receive a message from the message queue. \n
*          In case of the message queue is empty, wait until the time that is specified by the time stamp is reached.
*          \n The value of time stamp should be acquired by R_OSAL_ClockTimeGetTimeStamp().
* @param[in]       handle          The handle acquired at R_OSAL_MqCreate().
* @param[in]       p_time_stamp    Set the pointer address of #st_osal_time_t as timestamp that wait until
*                                  the queue be able to receive message.
* @param[in,out]   p_buffer        The address of void* for the receive data. The OSAL sets the receive data.
* @param[in]       buffer_size     The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the message was received successfully from the message 
*                              queue.
*                              - buffer_size is not equal msg_size.
* @retval  OSAL_RETURN_TIME    Could not receive message within the specified time.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Could not receive message. This error occurs only interrupt context.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to 
*          1 milli second. If the time stamp is past time, use the current time stamp.
*          The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_MqReceiveUntilTimeStamp
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqReceiveUntilTimeStamp(osal_mq_handle_t handle,
                                                    const st_osal_time_t* const p_time_stamp,
                                                    void* p_buffer,
                                                    size_t buffer_size);

/*******************************************************************************************************************//**
* @brief   Check if a message queue is full or not and set the result value to p_result.
* @param[in]       handle      The handle acquired at R_OSAL_MqCreate().
* @param[in,out]   p_result    The address of bool for result. The OSAL sets result of the confirmation.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and get successfully whether the message queue is full.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MqIsFull
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqIsFull(osal_mq_handle_t handle,
                                     bool* const p_result);

/*******************************************************************************************************************//**
* @brief   Check if a message queue is empty or not and set the result value to p_result.
* @param[in]       handle      The handle acquired at R_OSAL_MqCreate().
* @param[in,out]   p_result    The address of bool for result. The OSAL sets result of the confirmation.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and get successfully whether the message queue is empty.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MqIsEmpty
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqIsEmpty(osal_mq_handle_t handle,
                                      bool* const p_result);

/*******************************************************************************************************************//**
* @brief   Get a configuration corresponding to handle of the message queue and set that configuration to p_config.
* @param[in]       handle      The handle acquired at R_OSAL_MqCreate().
* @param[in,out]   p_config    The address of #st_osal_mq_config_t.
*                              The OSAL sets the Message Queue configuration corresponding to handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and get successfully configuration of the message queue.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MqGetConfig
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqGetConfig(osal_mq_handle_t handle,
                                        st_osal_mq_config_t *const p_config);

/*******************************************************************************************************************//**
* @brief   Delete all messages in the message queue. \n
*          After this call, the message queue is emptied and the thread waiting to send the message is unblocked.
* @param[in]   handle  The handle acquired at R_OSAL_MqCreate().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and delete successfully all messages in the message queue.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle was not acquired by R_OSAL_MqCreate().
*                              - handle was already deleted by R_OSAL_MqDelete().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The message queue itself is not deleted. After calling this function, the handle is still valid
*          and the message queue can be used.
* @see     Refer to following the function in this specification
*          R_OSAL_MqReset
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqReset(osal_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the Clock and Time manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_init_status_t R_OSAL_RCFG_ClockTimeGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the Clock and Time manager.
* @return           void
* @param[in]        status        Initialization status of the  clock and time manager
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_RCFG_ClockTimeSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes Clock and Time related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ClockTimeControlInit(void);

/*******************************************************************************************************************//**
* @brief   Acquire the time stamp on system according to clock_type.
* @param[in]       clock_type      The value for clock type.
*                                  - #OSAL_CLOCK_TYPE_HIGH_RESOLUTION: High resolution clock type.
*                                  - #OSAL_CLOCK_TYPE_HIGH_PRECISION: High precision clock type.
* @param[in,out]   p_time_stamp    The address of #st_osal_time_t. The OSAL sets the p_time_stamp.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the time stamp on system was acquired successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_ClockTimeGetTimeStamp
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ClockTimeGetTimeStamp(e_osal_clock_type_t clock_type,
                                                  st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the Memory Manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @retval           OSAL_MANAGER_STATE_MEMOPENED
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_init_status_t R_OSAL_RCFG_MmngrGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the Memory Manager.
* @return           void
* @param[in]        status        Initialization status of the memory manager
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_RCFG_MmngrSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes Memory related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function prepare memory used in the environment 
*                   and resources to control them. (Inner mutex, for example)
*                   For dynamic resource environment, this function only prepare resources to control message queues.

* @see              Refer to following the function in this specification
*                   R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrControlInit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes Memory related controls in Resource Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function deinitializes memory related controls in the environment
*                   and destroy resources to control them. (Inner mutex, for example)
*                   For dynamic resource environment, this function only destroys resources to control message queues.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrControlDeinit(void);

/*******************************************************************************************************************//**
* @brief   Factory method to create the OSAL Memory Manager Instance with the given configuration.
*          To change configuration, the manager must be closed and re-opened. Closing and re-open the OSAL 
*          Memory Manager will potentially result in allocation/de-allocation of bookkeeping memory resources 
*          and should be limited to initialization phase in restricted environments.
* @param[in]       p_config        Configuration to instantiate the OSAL Memory Manager with
* @param[in]       region_idx      ID of memory region
* @param[in,out]   p_hndl_mmngr    Handle to the OSAL MMNGR instance
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                  All arguments are valid, and the OSAL Memory Manager was created 
*                                          with the given configuration successfully.
* @retval  OSAL_RETURN_MEM                 Internal Bookkeeping memory could not be allocated.
* @retval  OSAL_RETURN_DEV                 OS specific memory resource could not be opened/accessed.
* @retval  OSAL_RETURN_TIME                Operation took too long.
* @retval  OSAL_RETURN_CONF                Requested Configuration is invalid. please see also #st_osal_mmngr_config_t.
* @retval  OSAL_RETURN_FAIL                An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE
*                                          -# The OSAL Memory Manager is already opened.
*                                          -# Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_CONF_UNSUPPORTED    Requested allocate mode is not supported. 
*                                          please see also #e_osal_mmngr_allocator_mode_t.
* @note    In case of calling this API with a config initialized with R_OSAL_MmgrInitializeMmngrConfigSt(), 
*          #OSAL_RETURN_CONF is returned. Before calling this API, the config value  should be initialized with 
*          valid value. \n
*          OSAL Memory Manager will always allocate memory that is Cached, CPU read/write accessible. 
*          IOMMU/IPMMU configuration is not known by OSAL Memory Manager and it is assumed HW has read/write 
*          access. IOMMU/IPMMU configuration that changes Virtual address spaces of the HW shall implemented 
*          as Specific Memory Manager that is create Buffer objects that can consider IOMMU/IPMMU 
*          configurations. This user Memory Manager shall take ownership of the OSAL Memory Manager to prevent
*          other SW from accessing the OSAL MMNGR instance.
*          OSAL Memory Manager cannot be opened twice!
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrOpen
*          R_OSAL_MmngrOpenFromRegionIdx
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrOpen(const st_osal_mmngr_config_t* const p_config,
                                      osal_memory_region_idx_t region_idx,
                                      osal_memory_manager_handle_t* const p_hndl_mmngr);

/*******************************************************************************************************************//**
* @brief   Destroy/close the OSAL Memory Manager Instance. In case of any allocated space remains, it is released 
*          by this API.
* @param[in]   hndl_mmngr  Handle of the Manager to close
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the OSAL Memory Manager was closed 
*                                              successfully.
* @retval  OSAL_RETURN_MEM                     Internal Bookkeeping memory could not be deallocated.
* @retval  OSAL_RETURN_DEV                     OS specific memory resource could not be closed or timed out.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - hndl_mmngr is not acquired by R_OSAL_MmngrOpen().
*                                              - hndl_mmngr was already closed by R_OSAL_MmngrClose().
* @retval  OSAL_RETURN_STATE
*                                              -# The Memory Manager is not opened.
*                                              -# Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The handle is not a handle of type OSAL Memory Manager. 
*                                              Use the Memory Manager specific function to close it.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrOpen
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrClose(osal_memory_manager_handle_t hndl_mmngr);

/*******************************************************************************************************************//**
* @brief   Allocate memory from the memory manager hndl_mngr with the given size and alignment and default 
*          memory attributes of the memory manager.
* @param[in]       hndl_mmngr      Handle to the Memory Manager to allocate the memory from.
* @param[in]       size            Requested size of the memory: [range: n \* minimumCachelineSize ...
*                                  maxBufferSize] \(e.g. n \* 64 Byte\) where n = 1,2,3...
* @param[in]       alignment       Alignment of the memory [range: 2^6, 2^7, ..., 2^n with n limited
*                                  by the system or MMNGR ]
* @param[in,out]   p_buffer_obj    Handle to the allocate buffer object
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the memory was allocated successfully.
* @retval  OSAL_RETURN_MEM                     Memory manager is not capable of allocating requested memory
*                                              with the given parameters. E.g. run out of memory
* @retval  OSAL_RETURN_DEV                     Underlying memory manager cannot be accessed.
* @retval  OSAL_RETURN_TIME                    Operation took too long.
* @retval  OSAL_RETURN_CONF                    The given size and alignment values to not match with 
*                                              the memory managers requirements for allocation memory. 
*                                              please see also #st_osal_mmngr_config_t.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - hndl_mmngr is not acquired by R_OSAL_MmngrOpen().
*                                              - hndl_mmngr was already closed by R_OSAL_MmngrClose().
* @retval  OSAL_RETURN_STATE
*                                              -# Not initialized. (Before calling R_OSAL_Initialize().)
*                                              -# The Memory Manager is not opened. (Before calling 
*                                              R_OSAL_MmngrOpen().)
* @retval  OSAL_RETURN_PROHIBITED              The maximum amount of allowed allocations is reached.
* @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
* @note    This function will always use the default memory attributes of the memory manager. OSAL Memory 
*          Manager only supports one set of attributes. If you want to allocate memory with different 
*          attributes, please use the Specific Memory Manager function (e.g. future HWA Memory Manager). \n
*          Similar to standard allocations the actually allocated memory may be larger than the requested size
*          to fit alignments, avoid fragmentations and underflow/overflow detections. The reported allocated
*          size of the buffer.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrAlloc
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrAlloc(osal_memory_manager_handle_t hndl_mmngr,
                                       size_t size,
                                       size_t alignment,
                                       osal_memory_buffer_handle_t* const p_buffer_obj);

/*******************************************************************************************************************//**
* @brief   Deallocate memory from the given memory manager.
* @param[in]   hndl_mmngr  Handle to the Memory Manager to deallocate the memory from.
* @param[in]   buffer_obj  Handle to the deallocate buffer object.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the memory was deallocated successfully.
* @retval  OSAL_RETURN_MEM                     Memory manager is not capable of deallocating requested 
*                                              memory.
* @retval  OSAL_RETURN_DEV                     Underlying memory manager cannot be accessed.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - buffer_obj is NULL.
*                                              - hndl_mmngr is not acquired by R_OSAL_MmngrOpen().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
*                                              - hndl_mmngr was already closed by R_OSAL_MmngrClose().
* @retval  OSAL_RETURN_STATE
*                                              -# Not initialized. (Before calling R_OSAL_Initialize().)
*                                              -# The Memory Manager is not opened. (Before calling 
*                                              R_OSAL_MmngrOpen().)
* @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED
*                                              -# The provided mmngr does not support this operation.
*                                              -# The buffer object is not managed by Memory Manager hndl_mngr.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrDealloc
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrDealloc(osal_memory_manager_handle_t hndl_mmngr,
                                         osal_memory_buffer_handle_t buffer_obj);

/*******************************************************************************************************************//**
* @brief   Get the configuration of the memory manager and set to p_config.
*          In case #OSAL_MEMORY_MANAGER_HANDLE_INVALID is passed to this 
*          function the default/maximum configuration of the OSAL Memory Manager is returned. 
*          (please see R_OSAL_MmngrGetOsalMaxConfig())
* @param[in]       hndl_mmngr  Handle to an opened manager
* @param[in,out]   p_config    Will contain the requested configuration of the provided mmngr 
*                              or the default/maximum configuration of the OSAL MMNGR
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the configuration of the provided 
*                                              memory manager was returned successfully.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  hndl_mmngr is invalid. (not #OSAL_MEMORY_MANAGER_HANDLE_INVALID).
*                                              e.g. internal handle parameters are invalid.
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
*                                              - hndl_mmngr was already closed by R_OSAL_MmngrClose().
* @retval  OSAL_RETURN_STATE
*                                              -# Not initialized. (Before calling R_OSAL_Initialize().)
*                                              -# The Memory Manager is not opened. (Before calling 
*                                              R_OSAL_MmngrOpen().)
* @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrGetConfig
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrGetConfig(osal_memory_manager_handle_t hndl_mmngr,
                                           st_osal_mmngr_config_t* const p_config);

/*******************************************************************************************************************//**
* @brief   Get the default/maximum configuration of the OSAL Memory Manager and set to p_config.
* @param[in]       region_idx  ID of memory region
* @param[in,out]   p_config    Will contain the requested configuration of the provided mmngr 
*                              or the default/maximum configuration of the OSAL MMNGR.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the maximum configuration of the provided 
*                                              memory manager was returned successfully.
* @retval  OSAL_RETURN_CONF                    Requested Configuration is invalid.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrGetOsalMaxConfig
*          R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrGetOsalMaxConfig(osal_memory_region_idx_t region_idx,
                                                  st_osal_mmngr_config_t* const p_config);

/*******************************************************************************************************************//**
* @brief   Registers a monitor function to a mmngr for monitoring a specific event of the MMNGR
*          This function registers a monitor/observer to a memory manager for monitoring events 
*          (#e_osal_mmngr_event_t).
*          Use this monitor functionality to monitoring allocation/deallocation or error events. In safety 
*          critical system use this to connect a manager that can handle/detect systematic faults, under- 
*          or over-flowing of memory blocks according to ISO26262.
* @param[in]   hndl_mmngr      Handle to the memory manager
* @param[in]   monitored_event Event that shall be monitored by the provided monitor function
* @param[in]   monitor_handler function to be called by the mmngr for the monitored_event
* @param[in]   p_user_arg      User argument that will be passed without modification/accessed
*                              to the monitor handler function.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the monitor function was registered 
*                                              successfully.
* @retval  OSAL_RETURN_MEM                     Internal bookkeeping memory allocation failed.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - hndl_mmngr is not acquired by R_OSAL_MmngrOpen().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
*                                              - hndl_mmngr was already closed by R_OSAL_MmngrClose().
* @retval  OSAL_RETURN_STATE
*                                              -# Not initialized. (Before calling R_OSAL_Initialize().)
*                                              -# The Memory Manager is not opened. (Before calling 
*                                              R_OSAL_MmngrOpen().)
* @retval  OSAL_RETURN_PROHIBITED              The maximum amount of allowed registered monitor handlers 
*                                              is reached.
* @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
* @note    Any monitoring function shall meet similar requirements as an interrupt handler: Minimal Execution
*          times, non-blocking. \n Reason: They are called synchronously from the memory manager and the calling 
*          thread and have impact on timing. \n
*          User must make sure calling threads that may issue an event have enough stack available for the 
*          registered monitor function and the required stack of the mmngr. \n
*          The monitor functions may also be used for fault injection during testing. Example: artificially 
*          introduce delays. \n
*          Same monitor function may be registered for different events but shall not be registered twice 
*          for the same event!
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrRegisterMonitor
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrRegisterMonitor(osal_memory_manager_handle_t hndl_mmngr,
                                                 e_osal_mmngr_event_t monitored_event,
                                                 fp_osal_memory_mngr_cb_t monitor_handler,
                                                 void* p_user_arg);

/*******************************************************************************************************************//**
* @brief   Get the CPU accessible (read/write) pointer of the buffer object and set to pp_cpu_ptr.
* @param[in]       memory_buffer_obj_hndl  Handle of the buffer object
* @param[in,out]   pp_cpu_ptr              pointer to the cpu modifiable memory
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the CPU accessible (read/write) pointer 
*                                              of the buffer object was returned successfully.
* @retval  OSAL_RETURN_MEM                     Mapping/copying the buffer content to CPU failed.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - memory_buffer_obj_hndl is NULL.
*                                              - memory_buffer_obj_hndl is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_PROHIBITED              Read/write access by CPU is prohibited to the buffer object.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    Be aware that acquiring CPU accessible pointer may not be permitted. Call to this function shall be
*          restricted to driver/middleware implementations if modifying memory content is not mandatory. \n
*          Be aware that any modification done by the CPU must be synchronized back to HW by calling 
*          R_OSAL_MmngrFlush(). If this is not done the modified memory content may not be or only partially 
*          visible to the HW. \n
*          This operation may require to reserve memory on first call and may fail with #OSAL_RETURN_MEM. See
*          documentation of the Memory Manager that created the buffer object for more details.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrGetCpuPtr
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrGetCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                           void** const pp_cpu_ptr);

/*******************************************************************************************************************//**
* @brief   Get the CPU accessible (read-only) pointer of the buffer object and set to pp_const_cpu_ptr.
* @param[in]       memory_buffer_obj_hndl  Handle of the buffer object
* @param[in,out]   pp_const_cpu_ptr        pointer to the cpu accessible memory
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the CPU read accessible pointer of 
*                                              the buffer object was returned successfully.
* @retval  OSAL_RETURN_PAR                     Invalid handle.
*                                              - pp_const_cpu_ptr is NULL.
* @retval  OSAL_RETURN_MEM                     Mapping/copying the buffer content to CPU failed.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - memory_buffer_obj_hndl is NULL.
*                                              - memory_buffer_obj_hndl is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_PROHIBITED              Read access by CPU is prohibited to the buffer object.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    Be aware that acquiring CPU accessible pointer may not be permitted. Call to this function shall be
*          restricted to driver/middleware implementations if reading memory content is not mandatory. \n
*          Be aware that any expected modification done by the HW from the point in time this cpu_ptr is 
*          acquired must be synchronized to CPU by calling R_OSAL_MmngrInvalidate(). If this is not done the 
*          modified memory content may not be or only partially updated to the CPU. \n
*          This operation may require to reserve memory on first call and may fail with #OSAL_RETURN_MEM. See
*          documentation of the Memory Manager that created the buffer object for more details.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrGetConstCpuPtr
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrGetConstCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                                const void** const pp_const_cpu_ptr);

/*******************************************************************************************************************//**
* @brief   Synchronizes a sub region of the buffer CPU memory to the HW. \n
*          Synchronizes CPU memory to the HW by making sure the memory content accessible by the HW is 
*          identical with the CPU memory view.
* @param[in]   memory_buffer_obj_hndl  Handle of the buffer object
* @param[in]   offset                  Offset with the buffer object limits [range: 0, 
*                                      n*minimumCachelineSize ... maxBufferSize] n = 1,2,3,... 
*                                      (e.g. 0, n* 64 Byte)
* @param[in]   size                    Size of the region to be synchronized [range: [buffer_specific,
*                                      typical 64Bytes]--buffer_size].
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the sub region of the buffer CPU memory 
*                                              was synchronized to the HW successfully.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - memory_buffer_obj_hndl is NULL.
*                                              - memory_buffer_obj_hndl is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_PROHIBITED              Write access by CPU is prohibited to the buffer object.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    This operation may overwrite memory modified by the HW since the last call to R_OSAL_MmngrFlush().
*          User must make sure this is avoided or will not have impact. \n
*          Provided size must be multiple of the minimal size. Typically multiple of 64Byte
*          (ARM Cache Line Size). \n
*          This operation will return #OSAL_RETURN_OK also in case no synchronization is required due to CPU 
*          memory view and HW memory view is ensured to be same. Example: Memory is uncached.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrFlush
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                       size_t offset,
                                       size_t size);

/*******************************************************************************************************************//**
* @brief   Synchronizes a sub region of the buffer HW memory to the CPU memory. \n
*          Synchronizes HW memory with the CPU by making sure the memory content accessible by the HW is 
*          identical with the CPU memory view.
* @param[in]   memory_buffer_obj_hndl  Handle of the buffer object
* @param[in]   offset                  Offset with the buffer object limits [range: 0, 
*                                      n*minimumCachelineSize ... maxBufferSize] n = 1,2,3, ... 
*                                      (e.g. 0, n* 64 Byte)
* @param[in]   size                    Size of the region to be synchronized [range: [buffer_specific, 
*                                      typical 64Bytes]--buffer_size].
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the sub region of the buffer HW memory 
*                                              was synchronized to the CPU memory successfully.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - memory_buffer_obj_hndl is NULL.
*                                              - memory_buffer_obj_hndl is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_PROHIBITED              Write access by CPU is prohibited to the buffer object.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    This operation may overwrite memory modified by the CPU since the last call to 
*          R_OSAL_MmngrInvalidate(). User must make sure this is avoided or will not have impact. \n
*          Provided size must be multiple of the minimal size. Typically multiple of 64Byte
*          (ARM Cache Line Size). \n
*          This operation will return #OSAL_RETURN_OK also in case no synchronization is required due to CPU 
*          memory view and HW memory view is ensured to be same. Example: Memory is uncached.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrInvalidate
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                            size_t offset,
                                            size_t size);

/*******************************************************************************************************************//**
* @brief   Get the hw/peripheral/axi bus domain specific address of the buffer and set to p_hw_addr.
* @param[in]       memory_buffer_obj_hndl  Handle of the buffer object
* @param[in]       axi_id                  axi id
* @param[in,out]   p_hw_addr               To set the address of uintptr_t. 
*                                          The OSAL sets the hardware address.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the axi bus domain specific address of 
*                                              the buffer was returned successfully.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - memory_buffer_obj_hndl is NULL.
*                                              - memory_buffer_obj_hndl is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_PROHIBITED              The peripheral is not allowed to access this buffer object.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    The hw_addr may differ between devices/peripherals due to external factors (e.g. IOMMU/IPMMU
*          configuration). It must be ensured the hw_addr provided by this function is only used by the 
*          peripheral it was requested from. See documentation of the Memory Manager that created the buffer 
*          object for more details.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrGetHwAddr
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrGetHwAddr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                           osal_axi_bus_id_t axi_id,
                                           uintptr_t * const p_hw_addr);

/*******************************************************************************************************************//**
* @brief   Get this size of the buffer and set to p_size.
* @param[in]       memory_buffer_obj_hndl  Handle of the buffer object
* @param[in,out]   p_size                  To set the address of size_t.
*                                          The OSAL sets the buffer size.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the size of buffer was returned 
*                                              successfully.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - memory_buffer_obj_hndl is NULL.
*                                              - memory_buffer_obj_hndl is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrGetSize
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrGetSize(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                         size_t* const p_size);

/*******************************************************************************************************************//**
* @brief   Triggers a check the current buffer object for integrity. E.g. by checking for underflow or 
*          overflow. Behavior may differ between memory buffer objects and their manager implementation.
*
* Example 1: \n
*   The buffer is CPU read/write accessible and mmngr has underflow/overflow detection enabled: \n
*     This will function will trigger check of any unexpected modifications of the underflow/overflow memory
* regions and will result in issuing a monitor event call. (Please see #e_osal_mmngr_event_t)
*
* Example 2: \n
*   The buffer is not CPU read/write accessible but IOMMU/IPMMU Guard pages are used to detect underflow/overflow of
* memory regions by HW. This will check if regions assigned/neighbored to this buffer object are marked by the
* mmngr to be modified. This situation would automatically be reported by the MMNGR by calling the matching
* monitor. This function would trigger this event again.
*
* @param[in]   memory_buffer_obj_hndl  Handle of the buffer object.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the current buffer object for integrity 
*                                              is ok. (no overflow/underflow)
* @retval  OSAL_RETURN_MEM                     The buffer integrity is not ok (overflow/underflow)
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - memory_buffer_obj_hndl is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    This operation must be supported by the buffer object and mmngr.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrCheckBuffer
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrCheckBuffer(osal_memory_buffer_handle_t memory_buffer_obj_hndl);

/*******************************************************************************************************************//**
* @brief   Get the default memory region index.
* @return  osal_memory_region_idx_t
* @retval  default region index
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrOpen
*          R_OSAL_MmngrGetOsalMaxConfig
**********************************************************************************************************************/
osal_memory_region_idx_t R_OSAL_RCFG_MmngrGetDefaultIdx(void);

/*******************************************************************************************************************//**
* @brief   Get the num of memory region.
* @return  size_t
* @retval  Num of memory resgion
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrOpen
*          R_OSAL_MmngrOpenFromRegionIdx
*          R_OSAL_MmngrGetOsalMaxConfig
*          R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx
**********************************************************************************************************************/
size_t R_OSAL_RCFG_MmngrGetNumOfRegions(void);

/*******************************************************************************************************************//**
 * Debug Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
* @brief   Prints human readable information about the current mmngr state.
*          This function prints Memory Manager information in following format: \n
*          (Generic Manager Information) \n
*          (Manager Specific information, format undefined in OSAL API) \n
*          (Table Header) \n
*          (Output from R_OSAL_MMNGRDebugBufferPrint) \n
*
* Example: \n
* ```bash
* Memory Current(Limit):              548KB(100MB)
* Allocations Current(Limit):         30(100)
* Registered Monitors Current(Limit): 6(6)
* Allocation Mode:                    FreeList with Underflow/Overflow detection
* <Manager Specifics>
* Buffer Handle Value; address                                ; address (axi domain)                          ; sec ;
*  d    ;  size      ; permissions               ; \n
* 0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE (A3IR) ; ns  ;
*  A3IR ; 0x123456743; P:readonly U:readonly ... ;
* ```
* @param[in]   hndl_mmngr  Handle of the manager
* @param[in]   p_output    Output stream to write the information to.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the human readable information about 
*                                              the current mmngr state was printed successfully.
* @retval  OSAL_RETURN_IO                      on write to output stream failed.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - hndl_mmngr is not acquired by R_OSAL_MmngrOpen().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
*                                              - hndl_mmngr was already closed by R_OSAL_MmngrClose().
* @retval  OSAL_RETURN_STATE
*                                              -# Not initialized. (Before calling R_OSAL_Initialize().)
*                                              -# The Memory Manager is not opened. (Before calling 
*                                              R_OSAL_MmngrOpen().)
* @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
* @note    this function will only be available if debug builds are enabled or the specific 
*          R_OSAL_ENABLE_DEBUG flag is set. \n
*          this function shall not be used in production environment.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrDebugMmngrPrint
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrDebugMmngrPrint(osal_memory_manager_handle_t hndl_mmngr,
                                                 FILE* p_output);

/*******************************************************************************************************************//**
* @brief   Prints human readable information about the current mmngr state.
*
* ```bash
* The function prints buffer information in following format if Buffer is mapped identical in all HWA domains \n
* "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE (MM)   ; ns 
* ;   ; 0x123456743; P:readonly U:readonly ... ;" \n
*
* In case of different mappings, multiple lines are dumped with information for each domain. Entries with same
* mappings can be grouped: \n
* "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE ; 
* 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE (MM)      ; ns  ;   ; 0x123456743; P:readonly U:readonly ... ;" \n
* "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE ; 0x1234567891234567--0x1234567891234567 (A3IR,VI) ; 
* ns  ;   ; 0x123456743; P:readonly U:readonly ... ;" \n
* "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE 
* ; 0x9876543219876543--0x9876543219876543 (VO)      ; ns  ;   ; 0x123456743; P:readonly U:readonly ... ;"
* ```
* @param[in]   hndl_buffer Handle of a buffer object
* @param[in]   p_output    Output stream to write the information to.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the human readable information about 
*                                              the current mmngr state was printed successfully.
* @retval  OSAL_RETURN_IO                      on write to output stream failed.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - hndl_buffer is not acquired by R_OSAL_MmngrAlloc().
*                                              - handle->p_ptr_self is invalid. handle might be corrupted or it is 
*                                              not acquired by OSAL memory manager.
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
* @note    This function will only be available if debug builds are enabled or the specific 
*          R_OSAL_ENABLE_DEBUG flag is set. \n
*          This function shall not be used in production environment.
* @see     Refer to following the function in this specification
*          R_OSAL_MmngrDebugBufferPrint
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrDebugBufferPrint(osal_memory_buffer_handle_t hndl_buffer,
                                                  FILE* p_output);

/** @} OSAL_WRAPPER */

#if !defined(_STDIO_H_INCLUDED) && !defined(_STDIO_H) && !defined(_STDIO_H_) && !defined(_INC_STDIO) && !defined(__stdio_h)
#if defined(NDEBUG) && !defined(R_OSAL_ENABLE_DEBUG)
/* PRQA S 4601,0841 1 # Since it is the macro "FILE" declared when stdio.h is not used, undefining it is no problem. */
#undef FILE
#endif
#endif

#endif /* OSAL_RES_CFG_IF_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
