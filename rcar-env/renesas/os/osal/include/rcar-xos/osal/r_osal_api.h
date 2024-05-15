/***********************************************************************************************************************
* Copyright [2020-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
 * XOS3_OSAL_NOS_AD_HED_COM_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_api.h
* Version :      3.7.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL public header file
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_012]
***********************************************************************************************************************/

#ifndef OSAL_API_H /* OSAL_API_H */
#define OSAL_API_H

/* Proposal: Enforce C99 ANSI C standard compatibility in header */
#if !defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/** Compiler is compatible to C99 or newer standard */
#error "Compiler must be compatible with at least C99 standard"
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

/* ################################################################################### */

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#if !defined(_STDIO_H_INCLUDED) && !defined(_STDIO_H) && !defined(_STDIO_H_) && !defined(_INC_STDIO) && !defined(__stdio_h)
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
#include <stdio.h>
#else
/*******************************************************************************************************************//**
 * @ingroup macro_definitions
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def FILE
 * @brief This definition is only defined in release mode. \n
 * "FILE" type is used as argument of R_OSAL_MmngrDebugMmngrPrint() and R_OSAL_MmngrDebugBufferPrint().
 * stdio.h is only included in debug mode, so this definition should be defined in release mode.
 * In release mode, R_OSAL_MmngrDebugMmngrPrint() and R_OSAL_MmngrDebugBufferPrint() are not supported.
 * Therefore, the "FILE" type in release mode is defined as "void".
 **********************************************************************************************************************/
/* PRQA S 4601 1 # There is no problem because it is the declaration of macro "FILE" when stdio.h is not used. */
#define FILE void
/** @} */
#endif
#endif

#include "rcar-xos/osal/r_osal_def.h"
#include "rcar-xos/osal/r_osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_COM_011
***********************************************************************************************************************/
/******************************************************************************************************************//**
 * @defgroup OSAL_API OSAL API
 * OSAL API has 8 managers and common functions. For detail refer to each section.
 * @{
 *********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_008]
***********************************************************************************************************************/

/**
 * @defgroup OSAL_Common_Function OSAL Common Function
 * OSAL API for Common
 * @{
 */


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_COM_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Initialize the resources provided by OSAL.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      Successful initialization of resources supported by OSAL.
 * @retval  OSAL_RETURN_MEM     Resource allocation failed.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_CONF    Wrong configuration value.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Already initialized in the process.
 * @note    This function must call only one time in the process.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_Initialize(void);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_015]
***********************************************************************************************************************/


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_COM_003
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Deinitialize the initialized resource.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      Successful deinitialized the initialized resource.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY    Cannot deinitialize because resource is in use.
 * @retval  OSAL_RETURN_STATE   If R_OSAL_Deinitialize() is called again after R_OSAL_Deinitialize().
 * @note    This function must use after the initialize API called.
 *          - This function only release resources created by OSAL.
 *          - This function works on the assumption that all threads created by OSAL are terminated. \n
 *            If there are any threads blocked on it, those threads should be unblocked before calling this function.
 *          - This function works on the assumption that the HW control is stopped. Before calling this function, 
 *            the interrupt processing should be stopped. \n
 *            i.e. all interrupt should be disabled and all interrupt handler should be unregistered, and also all 
 *            IO handle should be closed.
 *          - If there are any resources created by OSAL are remaining before calling this function, this function 
 *            may return #OSAL_RETURN_FAIL, #OSAL_RETURN_BUSY or Undefined behavior.
 *          - User should ensure to close/delete all resources before calling this function.
 * @see
 *      R_OSAL_Initialize()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_Deinitialize(void);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_013]
 * [Covers: XOS3_OSAL_NOS_AD_FD_COM_002]
***********************************************************************************************************************/

/**@} Common*/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_001
***********************************************************************************************************************/
/**
 * @defgroup OSAL_Thread_Manager OSAL Thread Manager
 * OSAL API for Thread Management \n
 * OSAL Thread Manager provides the following functions. \n
 * - Provide thread operation function: thread create, thread join and sleep.
 * - There are 15 level of thread priority, and OSAL maps to thread priorities of the OS.
 * - Provide utility function: compare thread IDs and get thread ID by oneself.
 * @{
 */
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_012]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Create a new thread in the calling process as the configuration specified in p_config.
 * @param[in]       p_config    The address of #st_osal_thread_config_t.
 * @param[in]       thread_id   The value of thread id.
 * @param[in,out]   p_handle    The address of osal_thread_handle_t. The OSAL sets the handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the thread was created successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_config is NULL.
 *                              - p_handle is NULL.
 *                              - p_config->priority is out of range. please see also #e_osal_thread_priority_t.
 *                              - p_config->stack_size is 0.
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
 * @see
 *      - #st_osal_thread_config_t
 *      - #e_osal_thread_priority_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadCreate(const st_osal_thread_config_t* const p_config, osal_thread_id_t thread_id,
                                    osal_thread_handle_t* const p_handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_001]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_002]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_014]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_027]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_029]
 * [Covers: XOS3_OSAL_NOS_AD_FD_TH_001]
***********************************************************************************************************************/


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_003
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Suspend the execution of the current thread until an absolute time value specified by p_time_stamp is 
 *          reached. \n
 *          The value of time stamp should be acquired by R_OSAL_ClockTimeGetTimeStamp().
 * @param[in]   p_time_stamp    The pointer addresses of #st_osal_time_t that holds the timestamp on which 
 *                              the thread will continue.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK          All arguments are valid, and the thread was suspended successfully.
 * @retval  OSAL_RETURN_PAR         Invalid argument.
 *                                  - p_time_stamp is NULL.
 *                                  - p_time_stamp->tv_sec is out of range. please see also #st_osal_time_t.
 *                                  - p_time_stamp->tv_nsec is out of range. please see also #st_osal_time_t.
 * @retval  OSAL_RETURN_FAIL        An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE       Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to 
 *          1 milli second. If the time stamp is past time, use the current time stamp. \n
 *          The waiting time may be longer than specified due to the influence of other threads.
 * @see
 *      - #st_osal_time_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_026]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_004
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Suspend the execution of the current thread for specified by time_period.
 * @param[in]   time_period To set the value for sleep time by msec. (valid values are >= 0)
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the thread was suspended successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - time_period is less than 0.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    The waiting time may be longer than specified due to the influence of other threads.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadSleepForTimePeriod(osal_milli_sec_t time_period);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_009]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_005
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Obtain handle of the calling thread and set that thread to p_handle.
 * @param[in,out]   p_handle    The address of osal_thread_handle_t. The OSAL sets current thread handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and succeeded in getting the handle of the calling thread.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_handle is NULL.
 * @retval  OSAL_RETURN_FAIL    
 *                              - An error occurred in OS or related API.
 *                              - Called from the thread which was not created by OSAL.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_ThreadCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadSelf(osal_thread_handle_t* const p_handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_008]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_030]
***********************************************************************************************************************/


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_006
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Compares two thread handles and set the return value to p_result that is they refer to the same thread or 
 *          not.
 * @param[in]       handle1     The handle acquired at R_OSAL_ThreadCreate().
 * @param[in]       handle2     The handle acquired at R_OSAL_ThreadCreate().
 * @param[in,out]   p_result    The result compares the 2 handles.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and two thread handles was compared successfully. \n \n \n \n 
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_result is NULL.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE  handle is invalid.
 *                              - handle1 or handle2 is not acquired by R_OSAL_ThreadCreate().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_ThreadCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadEqual(osal_thread_handle_t handle1, osal_thread_handle_t handle2, bool* const p_result);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_003]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_004]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_015]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_031]
***********************************************************************************************************************/


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_007
***********************************************************************************************************************/
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
 * @see
 *      - R_OSAL_ThreadCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadJoin(osal_thread_handle_t handle, int64_t* const p_return_value);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_005]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_006]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_007]
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_032]
***********************************************************************************************************************/


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_TH_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Initialize all configuration structure members to a default/invalid values.
 *          - p_config->func is NULL
 *          - p_config->userarg is NULL
 *          - p_config->priority is OSAL_THREAD_PRIORITY_LOWEST
 *          - p_config->task_name is ""
 *          - p_config->stack_size is 0
 * @param[in,out]   p_config    thread configuration.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK  All arguments are valid, and successfully initialized the thread settings.
 * @retval  OSAL_RETURN_PAR Invalid argument.
 *                          - p_config is NULL.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThreadInitializeThreadConfigSt(st_osal_thread_config_t* const p_config);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_TH_033]
***********************************************************************************************************************/

/**@} Thread*/

/**
 * @defgroup OSAL_Thread_Synchronization OSAL Thread Synchronization
 * OSAL API for Thread Synchronization Management
 * @{
 */


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_001
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Create a new mutex assigned to mutex_id and set acquired mutex handle to p_handle.
 * @param[in]       mutex_id    The value of mutex id.
 * @param[in,out]   p_handle    The address of osal_mutex_handle_t. The OSAL sets the handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was created successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_handle is NULL.
 * @retval  OSAL_RETURN_MEM     Resource allocation failed.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY    Specified mutex_id is already handled. 
 *                              If this error occurs, p_handle is not overwritten.
 * @retval  OSAL_RETURN_ID      Specified mutex_id is unknown.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    The initial state of the mutex is unlocked. \n
 *          If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
 *          #OSAL_MUTEX_HANDLE_INVALID.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexCreate(osal_mutex_id_t mutex_id, osal_mutex_handle_t* const p_handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_007]
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_013]
 * [Covers: XOS3_OSAL_NOS_AD_FD_THS_003]
***********************************************************************************************************************/


/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_002
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    After calling this function, the handle is invalid and cannot be used.
 * @see
 *      R_OSAL_ThsyncMutexCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexDestroy(osal_mutex_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_003
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_time_stamp is NULL.
 *                              - p_time_stamp->tv_sec is out of range. please see also #st_osal_time_t.
 *                              - p_time_stamp->tv_nsec is out of range. please see also #st_osal_time_t.
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
 * @see
 *      - R_OSAL_ThsyncMutexCreate()
 *      - R_OSAL_ThsyncMutexUnlock()
 *      - #st_osal_time_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexLockUntilTimeStamp(osal_mutex_handle_t         handle,
                                                     const st_osal_time_t* const p_time_stamp);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_004
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Lock a mutex assigned to handle with timeout for specified time. \n
 *          The function blocks the current thread until the mutex assigned to mutex handle is locked by the current 
 *          thread or until the time period has been elapsed.
 * @param[in]   handle      The handle acquired at R_OSAL_ThsyncMutexCreate().
 * @param[in]   time_period Sets the value of time to wait for the mutex is unlocked, in msec. (valid values are >= 0)
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was locked successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - time_period is less than 0.
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
 * @see
 *      - R_OSAL_ThsyncMutexCreate()
 *      - R_OSAL_ThsyncMutexUnlock()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexLockForTimePeriod(osal_mutex_handle_t handle, osal_milli_sec_t time_period);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_005
***********************************************************************************************************************/
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
 * @see
 *      - R_OSAL_ThsyncMutexCreate()
 *      - R_OSAL_ThsyncMutexUnlock()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexTryLock(osal_mutex_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_006
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Unlock mutex assigned to handle.
 * @param[in]   handle  The handle acquired at R_OSAL_ThsyncMutexCreate().
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the mutex was unlocked successfully.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API or Mutex has already unlocked.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle was not acquired by R_OSAL_ThsyncMutexCreate().
 *                              - handle was already destroyed by R_OSAL_ThsyncMutexDestroy().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_ThsyncMutexCreate()
 *      - R_OSAL_ThsyncMutexLockUntilTimeStamp()
 *      - R_OSAL_ThsyncMutexLockForTimePeriod()
 *      - R_OSAL_ThsyncMutexTryLock()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMutexUnlock(osal_mutex_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_007
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Create the condition variable assigned to cond_id and set acquired cond handle to p_handle.
 * @param[in]       cond_id     The value of condition variable id.
 * @param[in,out]   p_handle    The address of osal_cond_handle_t. The OSAL sets the handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the condition variable was created successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_handle is NULL.
 * @retval  OSAL_RETURN_MEM     Resource allocation failed.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY    Specified cond_id is already handled. 
 *                              If this error occurs, p_handle is not overwritten.
 * @retval  OSAL_RETURN_ID      Specified cond_id is unknown.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
 *          #OSAL_COND_HANDLE_INVALID.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondCreate(osal_cond_id_t cond_id, osal_cond_handle_t* const p_handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_002]
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_008
***********************************************************************************************************************/
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
 * @see
 *      R_OSAL_ThsyncCondCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondDestroy(osal_cond_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_009
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Wait on a condition variable with timeout until time stamp. \n
 *          This function blocks the calling thread on the condition variable and unlocks the associated mutex. \n
 *          The calling thread is blocked until either unblocked from other thread or the time stamp is reached. \n
 *          The value of time stamp should be acquired by R_OSAL_ClockTimeGetTimeStamp(). \n
 *          Return from this function, the mutex is re-locked and owned by the calling thread.\n\n\n\n
 * @param[in]   cond_handle     The handle acquired at R_OSAL_ThsyncCondCreate().
 * @param[in]   mutex_handle    The handle acquired at R_OSAL_ThsyncMutexCreate().
 * @param[in]   p_time_stamp    Set the pointer address of #st_osal_time_t as timestamp that wait until 
 *                              condition variable unblocked.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the condition variable was waited successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_time_stamp is NULL.
 *                              - p_time_stamp->tv_sec is out of range. please see also #st_osal_time_t.
 *                              - p_time_stamp->tv_nsec is out of range. please see also #st_osal_time_t.
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
 * @see
 *      - R_OSAL_ThsyncMutexCreate()
 *      - R_OSAL_ThsyncCondCreate()
 *      - R_OSAL_ThsyncCondBroadcast()
 *      - R_OSAL_ThsyncCondSignal()
 *      - #st_osal_time_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondWaitUntilTimeStamp(osal_cond_handle_t cond_handle, osal_mutex_handle_t mutex_handle,
                                                    const st_osal_time_t* const p_time_stamp);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_010
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - time_period is less than 0.
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
 * @see
 *      - R_OSAL_ThsyncMutexCreate()
 *      - R_OSAL_ThsyncCondCreate()
 *      - R_OSAL_ThsyncCondBroadcast()
 *      - R_OSAL_ThsyncCondSignal()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondWaitForTimePeriod(osal_cond_handle_t cond_handle, osal_mutex_handle_t mutex_handle,
                                                   osal_milli_sec_t time_period);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_011
***********************************************************************************************************************/
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
 * @see
 *      - R_OSAL_ThsyncCondCreate()
 *      - R_OSAL_ThsyncCondWaitUntilTimeStamp()
 *      - R_OSAL_ThsyncCondWaitForTimePeriod()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondBroadcast(osal_cond_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_001]
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_012
***********************************************************************************************************************/
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
 *                              \n \n \n
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    Before calling this function, lock the mutex. And after calling this function, unlock the mutex.
 * @see
 *      - R_OSAL_ThsyncCondCreate()
 *      - R_OSAL_ThsyncCondWaitUntilTimeStamp()
 *      - R_OSAL_ThsyncCondWaitForTimePeriod()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncCondSignal(osal_cond_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_004]
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_THS_013
***********************************************************************************************************************/
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
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ThsyncMemoryBarrier(void);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_THS_018]
***********************************************************************************************************************/

/**@} Thread_Synchronization*/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_001
***********************************************************************************************************************/
/**
 * @defgroup OSAL_Message_Queue_Manager OSAL_Message_Queue_Manager
 * OSAL API for Message Queue Management \n
 * OSAL Message Queue Manager provides the following functions. \n
 * - A message queue that sends messages from one thread to another thread within the same process.
 * - A message queue that works according to the first in first out (FIFO) pattern.
 * @{
 */
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_017]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Create the message queue in the calling process as the mq_Id and configuration specified in p_config.
 * @param[in]       p_config    The address of #st_osal_mq_config_t
 * @param[in]       mq_Id       The value of message queue id.
 * @param[in,out]   p_handle    The address of #osal_mq_handle_t. The OSAL sets the handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the message queue was created successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_config is NULL.
 *                              - p_handle is NULL.
 *                              - p_config->max_num_msg is 0.
 *                              - p_config->msg_size is 0.
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
 * @see
 *      - #st_osal_mq_config_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqCreate(const st_osal_mq_config_t* const p_config, osal_mq_id_t mq_Id,
                                osal_mq_handle_t* const p_handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_001]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_018]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_019]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_020]
 * [Covers: XOS3_OSAL_NOS_AD_FD_MSG_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_003
***********************************************************************************************************************/
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
 * @see
 *      - R_OSAL_MqCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqDelete(osal_mq_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_002]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_003]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_004
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_buffer is NULL.
 *                              - p_time_stamp is NULL.
 *                              - p_time_stamp->tv_sec is out of range. please see also #st_osal_time_t.
 *                              - p_time_stamp->tv_nsec is out of range. please see also #st_osal_time_t.
 *                              - buffer_size is not equal msg_size.
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
 * @see
 *      - R_OSAL_MqCreate()
 *      - R_OSAL_MqReceiveUntilTimeStamp()
 *      - R_OSAL_MqReceiveForTimePeriod()
 *      - #st_osal_time_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqSendUntilTimeStamp(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp,
                                            const void* p_buffer, size_t buffer_size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_015]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_016]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_029]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_005
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - time_period is less than 0.
 *                              - p_buffer is NULL.
 *                              - buffer_size is not equal msg_size.
 * @retval  OSAL_RETURN_TIME    Could not send message within the specified time.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY    Could not send message. This error occurs only interrupt context.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle was not acquired by R_OSAL_MqCreate().
 *                              - handle was already deleted by R_OSAL_MqDelete().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    The waiting time may be longer than specified due to the influence of other threads.
 * @see
 *      - R_OSAL_MqCreate()
 *      - R_OSAL_MqReceiveUntilTimeStamp()
 *      - R_OSAL_MqReceiveForTimePeriod()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqSendForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, const void* p_buffer,
                                           size_t buffer_size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_012]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_013]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_014]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_028]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_035]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_006
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_buffer is NULL.
 *                              - p_time_stamp is NULL.
 *                              - p_time_stamp->tv_sec is out of range. please see also #st_osal_time_t.
 *                              - p_time_stamp->tv_nsec is out of range. please see also #st_osal_time_t.
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
 * @see
 *      - R_OSAL_MqCreate()
 *      - R_OSAL_MqSendUntilTimeStamp()
 *      - R_OSAL_MqSendForTimePeriod()
 *      - #st_osal_time_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqReceiveUntilTimeStamp(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp,
                                               void* p_buffer, size_t buffer_size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_009]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_010]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_026]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_007
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - time_period is less than 0.
 *                              - p_buffer is NULL.
 *                              - buffer_size is not equal msg_size.
 * @retval  OSAL_RETURN_TIME    Could not receive message within the specified time.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY    Could not receive message. This error occurs only interrupt context.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle was not acquired by R_OSAL_MqCreate().
 *                              - handle was already deleted by R_OSAL_MqDelete().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    The waiting time may be longer than specified due to the influence of other threads.
 * @see
 *      - R_OSAL_MqCreate()
 *      - R_OSAL_MqSendUntilTimeStamp()
 *      - R_OSAL_MqSendForTimePeriod()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqReceiveForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, void* p_buffer,
                                              size_t buffer_size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_007]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_008]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_025]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Check if a message queue is full or not and set the result value to p_result.
 * @param[in]       handle      The handle acquired at R_OSAL_MqCreate().
 * @param[in,out]   p_result    The address of bool for result. The OSAL sets result of the confirmation.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and get successfully whether the message queue is full.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_result is NULL.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle was not acquired by R_OSAL_MqCreate().
 *                              - handle was already deleted by R_OSAL_MqDelete().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_MqCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqIsFull(osal_mq_handle_t handle, bool* const p_result);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_006]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_024]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_009
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Check if a message queue is empty or not and set the result value to p_result.
 * @param[in]       handle      The handle acquired at R_OSAL_MqCreate().
 * @param[in,out]   p_result    The address of bool for result. The OSAL sets result of the confirmation.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and get successfully whether the message queue is empty.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_result is NULL.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle was not acquired by R_OSAL_MqCreate().
 *                              - handle was already deleted by R_OSAL_MqDelete().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_MqCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqIsEmpty(osal_mq_handle_t handle, bool* const p_result);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_005]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_023]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_010
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get a configuration corresponding to handle of the message queue and set that configuration to p_config.
 * @param[in]       handle      The handle acquired at R_OSAL_MqCreate().
 * @param[in,out]   p_config    The address of #st_osal_mq_config_t.
 *                              The OSAL sets the Message Queue configuration corresponding to handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and get successfully configuration of the message queue.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_config is NULL.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle was not acquired by R_OSAL_MqCreate().
 *                              - handle was already deleted by R_OSAL_MqDelete().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_MqCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqGetConfig(osal_mq_handle_t handle, st_osal_mq_config_t* const p_config);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_004]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_011
***********************************************************************************************************************/
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
 *          , and the message queue can be used.
 * @see
 *      - R_OSAL_MqCreate()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqReset(osal_mq_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_011]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MSG_012
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Initialize all configuration structure members to a default/invalid values.
 *          - p_config->max_num_msg is 0
 *          - p_config->msg_size is 0
 * @param[in,out]   p_config    message queue configuration
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK  All arguments are valid, and successfully initialized the configuration structure for 
 *                          Message queue.
 * @retval  OSAL_RETURN_PAR Invalid argument.
 *                          - p_config is NULL.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MqInitializeMqConfigSt(st_osal_mq_config_t* const p_config);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MSG_040]
***********************************************************************************************************************/
/**@} Message_Queue*/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_CLK_001
***********************************************************************************************************************/
/**
 * @defgroup OSAL_Clock_Time_Manager OSAL Clock & Time Manager
 * OSAL API for Clock & Time Management \n
 * OSAL Clock & Time Manager provides the following functions.
 * - Supports two types, HIGH_RESOLUTION_CLOCK and HIGH_PRECISION_CLOCK.
 * - OSAL provides similar to the clock type CLOCK_MONOTONIC of the POSIX standard.
 * @{
 */
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_005]
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_CLK_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Acquire the time stamp on system according to clock_type.
 * @param[in]       clock_type      The value for clock type.
 *                                  - #OSAL_CLOCK_TYPE_HIGH_RESOLUTION: High resolution clock type.
 *                                  - #OSAL_CLOCK_TYPE_HIGH_PRECISION: High precision clock type.
 * @param[in,out]   p_time_stamp    The address of #st_osal_time_t. The OSAL sets the p_time_stamp.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the time stamp on system was acquired successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_time_stamp is NULL.
 *                              - clock_type is not supported.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeGetTimeStamp(e_osal_clock_type_t clock_type, st_osal_time_t* const p_time_stamp);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_004]
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_CLK_003
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Acquire the difference between two times. \n
 *          This function calculates the elapsed time from time1 to time2. (p_time2 - p_time1)
 * @param[in]       p_time2             The time acquired at R_OSAL_ClockTimeGetTimeStamp().
 * @param[in]       p_time1             The time acquired at R_OSAL_ClockTimeGetTimeStamp().
 * @param[in,out]   p_time_difference   To set the address of osal_nano_sec_t.
 *                                      The OSAL sets the difference between time2 and time1 with a signed value.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the difference between two times was acquired successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_time_difference is NULL.
 *                              - p_time1 or p_time2 is NULL.
 *                              - p_time1->tv_sec or p_time2->tv_sec is out of range. please see also #st_osal_time_t.
 *                              - p_time1->tv_nsec or p_time2->tv_nsec is out of range. please see also #st_osal_time_t.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    The result of p_time_difference is not guaranteed if clock type has different between time1 and 
 *          time2. Regarding to time2, please set the value that after time of time1.
 * @see
 *      - R_OSAL_ClockTimeGetTimeStamp()
 *      - #st_osal_time_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeCalculateTimeDifference(const st_osal_time_t* const p_time2,
                                                        const st_osal_time_t* const p_time1,
                                                        osal_nano_sec_t* const      p_time_difference);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_001]
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_002]
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_003]
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_007]
 * [Covers: XOS2_OSAL_NOS_AD_FD_CLK_012]
***********************************************************************************************************************/

/**@} Clock_Time*/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_001
***********************************************************************************************************************/
/**
 * @defgroup OSAL_Interrupt_Manager OSAL Interrupt Manager
 * OSAL API for Interrupt Management 
 * OSAL Interrupt Manager provides the following functions. \n
 * Following OSAL Interrupt Manager functions are callable from interrupt context:
 * - R_OSAL_InterruptEnableIsr()
 * - R_OSAL_InterruptDisableIsr()
 * - R_OSAL_InterruptRegisterIsr()
 * - R_OSAL_InterruptUnregisterIsr()
 * Interrupt priority can be changed by calling the following function:
 * - R_OSAL_InterruptSetIsrPriority()
 * @{
 */
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_016]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the number of interrupt channels for a specific device and set the value to p_numOfChannels. \n
 *          The interrupt channel is used to specify the interrupt information associated with handle in each 
 *          OSAL interrupt API.
 * @param[in]       handle          To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_numOfChannels To set the address of size_t. The OSAL set the number of irq channels.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the number of interrupt channels for a specific device was 
 *                              got successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_numOfChannels is NULL.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle is corrupted.
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - R_OSAL_InterruptRegisterIsr()
 *      - R_OSAL_InterruptUnregisterIsr()
 *      - R_OSAL_InterruptEnableIsr()
 *      - R_OSAL_InterruptDisableIsr()
 *      - R_OSAL_InterruptSetIsrPriority()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptGetNumOfIrqChannels(osal_device_handle_t handle, size_t* const p_numOfChannels);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_003]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_017]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_003
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Registers the interrupt handler service routine called when an interrupt occurs from device. 
 * @param[in,out]   handle              To set the handle unique value of device handle acquired at 
 *                                      R_OSAL_IoDeviceOpen().
 * @param[in]       irq_channel         To set the value of Irq channel number.
 *                                      This value is from 0 to less than the value
 *                                      acquired with R_OSAL_InterruptGetNumOfIrqChannels().
 * @param[in]       priority_value      the value of interrupt priority.
 * @param[in]       irqHandlerFunction  pointer of CallBackFunction.
 * @param[in]       p_irq_arg           interrupt handler function's argument.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the function to be called when the 
 *                                              interrupt occurs has been successfully registered.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - irq_channel is the number of interrupt channel or more.
 *                                              - irqHandlerFunction is NULL.
 *                                              - priority_value is out of range. 
 *                                                please see also #e_osal_interrupt_priority_t.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY                    Already registered or interrupt remains enabled.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - the number of interrupt channel is 0.
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   An irq channel of interrupt not defined in the configuration 
 *                                              was specified.
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function works on the assumption that the interrupt is stopped before it is called.
 *          Before calling this function, finish the interrupt processing and make sure that no interrupt 
 *          occurs. When setting the same interrupt resource from different threads and interrupts, 
 *          ensure that the settings do not conflict on the user side.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - p_osal_isr_func_t
 *      - #e_osal_interrupt_priority_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel,
                                            e_osal_interrupt_priority_t priority_value,
                                            p_osal_isr_func_t irqHandlerFunction, void* p_irq_arg);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_005]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_012]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_004
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Unregisters the interrupt handler service routine called when an interrupt occurs from device.
 * @param[in]   handle              To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   irq_channel         To set the value of Irq channel number.
 *                                  This value is from 0 to less than the value
 *                                  acquired with R_OSAL_InterruptGetNumOfIrqChannels().
 * @param[in]   irqHandlerFunction  pointer of CallBackFunction.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the function to be called when the 
 *                                              interrupt occurs has been successfully unregistered.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - irq_channel is the number of interrupt channel or more.
 *                                              - irqHandlerFunction is NULL.
 *                                              - irqHandlerFunction has not been registered by 
 *                                                R_OSAL_InterruptRegisterIsr().
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY                    Interrupt remains enabled.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - the number of interrupt channel is 0.
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   An irq channel of interrupt not defined in the configuration 
 *                                              was specified.
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function works on the assumption that the interrupt is stopped before it is called. \n
 *          Before calling this function, finish the interrupt processing and make sure that no interrupt occurs. \n
 *          When setting the same interrupt resource from different threads and interrupts, 
 *          ensure that the settings do not conflict on the user side.
 * @see
 *      - R_OSAL_InterruptRegisterIsr()
 *      - R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptUnregisterIsr(osal_device_handle_t handle, uint64_t irq_channel,
                                              p_osal_isr_func_t irqHandlerFunction);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_007]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_008]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_009]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_013]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_005
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Enables the interrupt handler already registered to the specified irq_channel.
 * @param[in]   handle      To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   irq_channel To set the value of Irq channel number.
 *                          This value is from 0 to less than the value
 *                          acquired with R_OSAL_InterruptGetNumOfIrqChannels().
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the handler already registered was 
 *                                              successfully enabled.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - irq_channel is the number of interrupt channel or more.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL
 *                                              -# An error occurred in OS or related API.
 *                                              -# If the function is not registered.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - the number of interrupt channel is 0.
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   An irq channel of interrupt not defined in the configuration 
 *                                              was specified.
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    When setting the same interrupt resource from different threads and interrupts, ensure that the 
 *          settings do not conflict on the user side.
 * @see
 *      - R_OSAL_InterruptRegisterIsr()
 *      - R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_002]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_011]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_006
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Disables the interrupt handler already registered to the specified irq_channel.
 * @param[in]   handle      To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   irq_channel To set the value of Irq channel number.
 *                          This value is from 0 to less than the value
 *                          acquired with R_OSAL_InterruptGetNumOfIrqChannels().
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the handler already registered was 
 *                                              successfully disabled.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - irq_channel is the number of interrupt channel or more.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL
 *                                              -# An error occurred in OS or related API.
 *                                              -# If the function is not registered.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - the number of interrupt channel is 0.
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   An irq channel of interrupt not defined in the configuration 
 *                                              was specified.
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    When setting the same interrupt resource from different threads and interrupts, ensure that the 
 *          settings do not conflict on the user side.
 *          This function needs to use before register/unregister the function with 
 *          R_OSAL_InterruptRegisterIsr() and R_OSAL_InterruptUnregisterIsr().
 * @see
 *      - R_OSAL_InterruptRegisterIsr()
 *      - R_OSAL_InterruptEnableIsr()
 *      - R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptDisableIsr(osal_device_handle_t handle, uint64_t irq_channel);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_001]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_010]
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_007
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Checks where the context was called and set the context to p_is_isr.
 * @param[in,out]   p_is_isr    To set the address of bool. The OSAL sets the context.
 *                              p_is_isr is set the result of judge.
 *                              - true: interrupt context
 *                              - false: not interrupt context
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully confirm the context of original calling.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_is_isr is NULL.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      None. \n\n\n\n\n
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptIsISRContext(bool* const p_is_isr);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_INT_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Changes priority already registered handler to priority_value. \n \n \n \n 
 * @param[in]   handle          To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   irq_channel     To set the value of Irq channel number.
 *                              This value is from 0 to less than the value 
 *                              acquired with R_OSAL_InterruptGetNumOfIrqChannels().
 * @param[in]   priority_value  To set the value of priority.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and successfully changed priority already 
 *                                              registered handler.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                               - irq_channel is the number of interrupt channel or more.
 *                                               - priority_value is out of range.
 *                                                 please see also #e_osal_interrupt_priority_t.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - the number of interrupt channel is 0.
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   An irq channel of interrupt not defined in the configuration 
 *                                              was specified.
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    None.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - #e_osal_interrupt_priority_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptSetIsrPriority(osal_device_handle_t handle, uint64_t irq_channel,
                                               e_osal_interrupt_priority_t priority_value);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_INT_006]
***********************************************************************************************************************/

/**@} IRQ*/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_001
***********************************************************************************************************************/
/**
 * @defgroup OSAL_IO_Manager OSAL I/O Manager
 * OSAL API for I/O Management \n
 * OSAL IO Manager provides the following function.
 * - Abstraction of device information such as register address and interrupt number.
 * - Registers read / write per access size.
 * - Support block read / write.
 * - All IO functions are relative to device's base address.
 * @{
 */
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_030]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_031]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_032]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the number of devices identifier for a specific device type (p_device_type) and set the value to 
 *          p_numOfDevices. \n \n \n
 * @param[in]       p_deviceType    To set the string of device type.
 * @param[in,out]   p_numOfDevices  To set the address of size_t.
 *                                  The OSAL set the number of devices corresponding to deviceType.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the number of device identifier was got successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_deviceType is NULL.
 *                              - p_deviceType is more than #OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
 *                              - p_numOfDevices is NULL.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    In case of the device specified in p_deviceType does not exist, this API returns the following.
 *          - p_numOfDevices: 0
 *          - return value: OSAL_RETURN_OK
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetNumOfDevices(const char* p_deviceType, size_t* const p_numOfDevices);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_019]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_034]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_003
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the list of device identifier names for a specific device type and set the names to p_buffer. \n \n \n 
 * @param[in]       p_device_type_name  To set the string of device type.
 * @param[in,out]   p_buffer            To set the buffer address.
 *                                      The OSAL sets the device id corresponding to the device TYPE 
 *                                      as a separated string to p_buffer.
 *                                      Needs to acquire the memory area according to list_size.
 *                                      The buffer shall use the '\\n' character as a delimiter for the string 
 *                                      instances. The buffer shall be null-terminated, meaning it shall use '\0' 
 *                                      at the end of the last instance.
 * @param[in]       buffer_size         To set the value of p_buffer size by byte.
 * @param[in,out]   p_num_of_byte       To set the address of size_t.
 *                                      The OSAL sets the number of characters in p_buffer.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK              All arguments are valid, and the list of device identifier was got successfully.
 * @retval  OSAL_RETURN_PAR             Invalid argument.
 *                                      - p_device_type_name is NULL.
 *                                      - p_device_type_name is more than #OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
 *                                      - p_num_of_byte is NULL.
 *                                      - p_buffer is NULL and buffer_size is not 0.
 *                                      - if provided buffer is too small.
 * @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNKNOWN_NAME    No device was found related to p_device_type_name.
 * @retval  OSAL_RETURN_OK_RQST         on success. Only the number of characters in p_buffer could be obtained.
 *                                      - p_buffer is NULL and buffer_size is 0.
 * @note
 *          1. This function shall return an error if the given buffer is too small to hold all device ID
 *             strings which are must be returned.
 *          2. This function shall return the OSAL_RETURN_OK_RQST in case p_buffer==NULL && buffer_size==0
 *             and update written_bytes to the required buffer size.
 *          3. This function shall modify the written_bytes output both on success and in the error case
 *             (if the provided buffer is too small).
 *          4. In case the provided buffer is too small this output variable shall be set to the required 
 *             buffer size for the operation.
 *          Example: Requested device type is "abc" and it has 3 separate instances. \n
 *          Output of the function:
 *          Buffer with content: "abc_00\nabc_01\nabc_02\0". \n
 *          Value of written bytes is 21.
 * @see
 *      R_OSAL_IoGetNumOfDevices()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetDeviceList(const char* p_device_type_name, char* p_buffer, size_t buffer_size, 
                                       size_t* const p_num_of_byte);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_018]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_041]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_004
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Open an IO device handle corresponding to p_id and set the handle to p_handle.
 * @param[in]       p_id        To set the value of target device name (a.k.a device id) acquired by 
 *                              R_OSAL_IoGetDeviceList().
 * @param[in,out]   p_handle    To set the address of osal_device_handle_t.
 *                              The OSAL sets the handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the IO handle was opened successfully. 
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_id is NULL.
 *                              - p_handle is NULL.
 * @retval  OSAL_RETURN_MEM     Resource allocation failed.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_CONF    Wrong configuration value.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY    Specified id is already handled.
 *                              If this error occurs, p_handle will not be overwritten.
 * @retval  OSAL_RETURN_ID      Specified id is unknown.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    The setting items related to the Device ID are described the following.
 *          <table>
 *          <tr>
 *          <th> elements </th> <th> description </th>
 *          </tr>
 *          <tr>
 *          <td> Device Type </td> <td> One HW IP usually has multiple instances. 
 *                                      Device ID is assigned to each instance, bundle with Device Type. 
 *                                      For example, the device type is HW IP name.</td>
 *          </tr>
 *          <tr>
 *          <td> Device ID </td> <td> device identifier </td>
 *          </tr>
 *          </table>
 *          for example: IMR IP in the R-Car V3H, there are 6 different HW IP core. \n
 *          Device Type: "imr" \n
 *          Device ID : "imr_00", "imr_01", "imr_02", "imr_03", "imr_04", "imr_05" \n \n
 *          After calling this function the OSAL Manager can access the target device via the device p_handle. \n
 *          If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
 *          #OSAL_DEVICE_HANDLE_INVALID.
 * @see
 *      R_OSAL_IoGetDeviceList()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoDeviceOpen(const char* p_id, osal_device_handle_t* const p_handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_012]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_033]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_042]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_005
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Close an IO device handle specified by handle.
 * @param[in,out]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the IO handle was closed successfully. 
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_BUSY    The specified handle is still in use by another manager.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    After calling this function, the handle is invalid and cannot be used.
 *          This function works on the assumption that the interrupt is stopped before it is called. \n
 *          Before calling this function, finish the interrupt processing and make sure that no interrupt 
 *          occurs.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoDeviceClose(osal_device_handle_t handle);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_011]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_043]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_006
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a value from register by 8bit alignment and set the read value to p_data.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint8_t. The OSAL sets the register data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read a value from register by 8bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + sizeof(uint8_t) is more than register address range.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 8bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t* const p_data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_023]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_024]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_044]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_007
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a value from register by 16bit alignment and set the read value to p_data.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint16_t. The OSAL sets the register data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read a value from register by 16bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + sizeof(uint16_t) is more than register address range.
 *                              - offset is not aligned 16 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 16bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead16(osal_device_handle_t handle, uintptr_t offset, uint16_t* const p_data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_020]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_045]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a value from register by 32bit alignment and set the read value to p_data.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint32_t. The OSAL sets the register data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read a value from register by 32bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + sizeof(uint32_t) is more than register address range.
 *                              - offset is not aligned 32 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 32bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t* const p_data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_021]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_046]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_009
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a value from register by 64bit alignment and set the read value to p_data.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint64_t. The OSAL sets the register data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read a value from register by 64bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + sizeof(uint64_t) is more than register address range.
 *                              - offset is not aligned 64 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 64bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead64(osal_device_handle_t handle, uintptr_t offset, uint64_t* const p_data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_022]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_047]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_010
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a value (data) to register by 8bit alignment.
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   data    To set the value of uint8_t for write data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write a value to register by 8bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - offset + sizeof(uint8_t) is more than register address range.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 8bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite8(osal_device_handle_t handle, uintptr_t offset, uint8_t data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_028]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_029]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_048]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_011
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a value (data) to register by 16bit alignment.
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   data    To set the value of uint16_t for write data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write a value to register by 16bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - offset + sizeof(uint16_t) is more than register address range.
 *                              - offset is not aligned 16 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 16bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite16(osal_device_handle_t handle, uintptr_t offset, uint16_t data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_025]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_049]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_012
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a value (data) to register by 32bit alignment.
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   data    To set the value of uint32_t for write data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write a value to register by 32bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - offset + sizeof(uint32_t) is more than register address range.
 *                              - offset is not aligned 32 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 32bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite32(osal_device_handle_t handle, uintptr_t offset, uint32_t data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_026]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_050]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_013
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a value (data) to register by 64bit alignment.
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   data    To set the value of uint64_t for write data.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write a value to register by 64bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - offset + sizeof(uint64_t) is more than register address range.
 *                              - offset is not aligned 64 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 64bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite64(osal_device_handle_t handle, uintptr_t offset, uint64_t data);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_027]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_051]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_014
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a group of 8bit registers from the device and set the read value to p_data.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint8_t array.
 *                          The OSAL sets the register data.
 * @param[in]       size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read some values from register 
 *                              by 8bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 8bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Read of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t* p_data, size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_004]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_005]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_052]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_015
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a group of 16bit registers from the device and set the read value to p_data.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint16_t array.
 *                          The OSAL sets the register data.
 * @param[in]       size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read some values from register 
 *                              by 16bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 *                              - offset or size is not aligned 16 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 16bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          Blocking read can improve performance when reading a large amount of data. Note that this blocking read 
 *          function does not return until reading the specified amount of data completes while 
 *          R_OSAL_IoRead[8/16/32/64] returns after reading the data whose size is specified by the name of API. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Read of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead16(osal_device_handle_t handle, uintptr_t offset, uint16_t* p_data, size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_001]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_037]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_053]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_016
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a group of 32bit registers from the device and set the read value to p_data.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint32_t array.
 *                          The OSAL sets the register data.
 * @param[in]       size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read some values from register 
 *                              by 32bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 *                              - offset or size is not aligned 32 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 32bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Read of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t* p_data, size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_002]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_054]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_017
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Read a group of 64bit registers from the device and set the read value to p_data. \n\n
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       offset  To set the value of offset from the base address of handle by byte.
 * @param[in,out]   p_data  To set the address of uint64_t array.
 *                          The OSAL sets the register data.
 * @param[in]       size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read some values from register 
 *                              by 64bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 *                              - offset or size is not aligned 64 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 64bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Read of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead64(osal_device_handle_t handle, uintptr_t offset, uint64_t* p_data, size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_003]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_055]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_018
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a group of 8bit registers to the device. \n \n \n
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   p_data  To set the address of uint8_t array for write data.
 *                      The OSAL does not change the value.
 * @param[in]   size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write some values to register 
 *                              by 8bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 8bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Write of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite8(osal_device_handle_t handle, uintptr_t offset, const uint8_t* p_data, size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_009]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_010]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_056]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_019
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a group of 16bit registers to the device.
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   p_data  To set the address of uint16_t array for write data.
 *                      The OSAL does not change the value.
 * @param[in]   size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write some values to register 
 *                              by 16bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 *                              - offset or size is not aligned 16 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 16bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          Blocking write can improve performance when writing a large amount of data. Note that this blocking write 
 *          function does not return until writing the specified amount of data completes while 
 *          R_OSAL_IoWrite[8/16/32/64] returns after writing the data whose size is specified by the name of API. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Write of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite16(osal_device_handle_t handle, uintptr_t offset, const uint16_t* p_data, 
                                      size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_006]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_038]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_057]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_020
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a group of 32bit registers to the device. \n \n \n \n 
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   p_data  To set the address of uint32_t array for write data.
 *                      The OSAL does not change the value.
 * @param[in]   size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write some values to register 
 *                              by 32bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 *                              - offset or size is not aligned 32 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 32bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Write of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite32(osal_device_handle_t handle, uintptr_t offset, const uint32_t* p_data, 
                                      size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_007]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_058]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_021
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Write a group of 64bit registers to the device.
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   offset  To set the value of offset from the base address of handle by byte.
 * @param[in]   p_data  To set the address of uint64_t array for write data.
 *                      The OSAL does not change the value.
 * @param[in]   size    To set the value of register data size by byte.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write some values to register 
 *                              by 64bit alignment.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_data is NULL.
 *                              - offset + size is more than register address range.
 *                              - offset or size is not aligned 64 bit.
 * @retval  OSAL_RETURN_DEV     HW fault detected.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_IO      Input / Output operation failed.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function provides only access to registers under the assumption that access size is 64bit. \n
 *          When accessing the same register from different threads or interrupts, ensure that there is no data 
 *          race on the user side. \n
 *          In case of any argument other than size is valid and size = 0, this API exits without doing anything and 
 *          returns #OSAL_RETURN_OK. (= Write of 0 bytes was performed.)
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite64(osal_device_handle_t handle, uintptr_t offset, const uint64_t* p_data, 
                                      size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_008]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_059]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_022
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the axi id for the given device. The id may refer to a axi bus name and may be shared with 
 *          other peripherals. The axi id obtained by this API can be converted to a human readable name by 
 *          R_OSAL_IoGetAxiBusName().
 * @param[in]       device      Valid handle to a device.
 * @param[in,out]   p_axi_id    This parameter is identifier of bus name about the device.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the axi bus id for the given device was got successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_axi_id is NULL.
 * @retval  OSAL_RETURN_FAIL
 *                              -# An error occurred in OS or related API.
 *                              -# if the axi id for the given device could not be acquired.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - device is not acquired by R_OSAL_IoDeviceOpen().
 *                              - device was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @note    This function will be deprecated and shall not be used.
 * @see
 *      - R_OSAL_IoGetAxiBusName()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetDeviceAxiBusId(osal_device_handle_t device, osal_axi_bus_id_t * const p_axi_id);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_023
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the axi id for the given device. The id may refer to a axi bus name and may be shared with 
 *          other peripherals. The axi id obtained by this API can be converted to a human readable name by 
 *          R_OSAL_IoGetAxiBusName().
 * @param[in]       p_device_name   To set the value of target device name (a.k.a device id).
 * @param[in,out]   p_axi_id        To set the address of osal_axi_bus_id_t.
 *                                  The OSAL sets the identifier of bus name.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK              All arguments are valid, and the axi bus id for the given device name was 
 *                                      got successfully.
 * @retval  OSAL_RETURN_PAR             Invalid argument.
 *                                      - p_device_name is NULL.
 *                                      - p_axi_id is NULL.
 * @retval  OSAL_RETURN_FAIL
 *                                      -# An error occurred in OS or related API.
 *                                      -# if the axi id for the given device name could not be acquired.
 * @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNKNOWN_NAME    No device was found related to p_device_name.
 * @note    This function have limited usage in drivers or middleware to limit string compares and improved 
 *          performance. It may be used for debugging/tracing purposes in drivers/middlewares. 
 *          Drivers/Middlewares are typically owner of the device handles and shall use 
 *          R_OSAL_IoGetDeviceAxiBusId() instead.\n
 *          This function will be deprecated and shall not be used.
 * @see
 *      - R_OSAL_IoGetAxiBusName()
 *      - R_OSAL_IoGetDeviceAxiBusId()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiBusIdFromDeviceName(const char * p_device_name, osal_axi_bus_id_t * const p_axi_id);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_024
***********************************************************************************************************************/
/* PRQA S 2053 23 # False positive. */
/*******************************************************************************************************************//**
 * @brief   Get the string list of all axi bus names. The string list is a null terminated string with each entry 
 *          delimited by character. The number of axi ids included in the axi bus name obtained by this API can be
 *          obtained by R_OSAL_IoGetNumOfAxiIdsForAxiBusName(). osal_axi_bus_id_t list can be obtained by using
 *          axi bus names obtained by this API and R_OSAL_IoGetAxiIdListForAxiBusName().
 *          In case a null pointer for buffer and buffer_size = 0 is provided as input to this function, 
 *          it will provided the required buffer size for the function to successfully execute.
 * @param[in,out]   p_buffer        Pointer to a buffer to write the string list to.
 * @param[in]       buffer_size     Size of the provided buffer
 * @param[in,out]   p_num_of_byte   Amount of bytes (to be) written to the buffer
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the string list of all axi bus names was got successfully. 
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_num_of_byte is NULL.
 *                              - p_buffer is NULL and buffer_size is not 0.
 *                              - if provided buffer is too small.
 * @retval  OSAL_RETURN_MEM     Resource allocation failed.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_OK_RQST on success. Only the number of characters in p_buffer could be obtained.
 *                              - p_buffer is NULL and buffer_size is 0.
 * @note    In case axi ids are assigned to specific peripherals instead of a bus domain, this list may also
 *          contain device ids. This API returuns the following example.\n
 *          Example: \n
 *                   - p_buffer is not NULL
 *                   - buffer_size is 75
 *                   - p_num_of_byte is not NULL
 *                   Buffer with content: "invalid\0mm(ipa)\0ir_00-01\0vi0_04_10-11_13-14\0vc0_00-06\0vip0_01\0vip1\0ds1\0rt0\0"
 * @see
 *      - R_OSAL_IoGetNumOfAxiIdsForAxiBusName()
 *      - R_OSAL_IoGetAxiIdListForAxiBusName()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiBusNameList(char * p_buffer, size_t buffer_size, size_t * const p_num_of_byte);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_016]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_060]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_025
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Return the axi bus name for the given axi_id as a null terminated string. 
 *          This API returns a string for bus name and utlb separated by '_'.
 * @param[in]       axi_id          This parameter is identifier of bus name.
 * @param[in,out]   p_buffer        Pointer to a buffer to write the string list.
 * @param[in]       buffer_size     Size of the provided buffer.
 * @param[in,out]   p_num_of_byte   Amount of bytes (to be) written to the buffer.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the string list of all axi bus names was got successfully.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_num_of_byte is NULL.
 *                              - p_buffer is NULL and buffer_size is not 0.
 *                              - if provided buffer is too small.
 *                              - axi_id is out of range.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_OK_RQST on success. Only the number of characters in p_buffer could be obtained.
 *                              - p_buffer is NULL and buffer_size is 0.
 * @note    Axi bus name obtained by this API. \n
 *          Example: \n
 *                   - axi_id is 2
 *                   - p_buffer is not NULL
 *                   - buffer_size is 6
 *                   Buffer with content: "ir_00\0"
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiBusName(osal_axi_bus_id_t axi_id, char * p_buffer, size_t buffer_size,
                                       size_t * const p_num_of_byte);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_026
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the number of all axi ids that correspond to the specified p_axi_bus_name. 
 * @param[in]       p_axi_bus_name      To set the pointer to character that identifies the AXI Bus Name.
 * @param[in,out]   p_num_of_axi_ids    To set the pointer that contains the number of available AXI IDs 
 *                                      for that AXI Bus Name
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK              All arguments are valid, and the number of all axi ids corresponding to 
 *                                      the entered.
 * @retval  OSAL_RETURN_PAR             Invalid argument.
 *                                      - p_axi_bus_name is NULL.
 *                                      - p_num_of_axi_ids is NULL.
 * @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNKNOWN_NAME    If the entered axi_bus_name contains a combination of bus domain 
 *                                      and uTLB to which axi_id is not assigned. \n
 *                                      If the entered AXI Bus Name does not follow the format. \n
 *                                      Please refer to this API "Note"
 * @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
 * @note    axi_bus_name follows the format.
 *          - bus domain and uTLB are connected by "_"
 *          - If uTLBs are consecutive, connect the first and last uTLB values with "-"
 *          - If uTLBs are discontinuous, connect with "_"
 *          - uTLBs are described by 2 digit decimal number
 *          - uTLB numbers are arranged in ascending order. \n
 *          example) \n
 *          When bus domain is "ir" and uTLBs are 0, 1, 2, 3, 4, 8, 9, 10,12 \n
 *          "ir_00-04_08-10_12".
 * @see
 *      - R_OSAL_IoGetAxiBusNameList()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForAxiBusName(const char* p_axi_bus_name,  size_t *p_num_of_axi_ids);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_061]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_027
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the list of all axi ids that correspond to the specified axi_bus_name
 * @param[in]       p_axi_bus_name              To set the pointer to character that identifies the AXI Bus Name.
 * @param[in,out]   p_array                     To set the buffer address.
 *                                              OSAL sets all axi ids that correspond to axi_bus_name
 * @param[in]       array_size                  Maximum number of axi ids that can be written to p_array
 * @param[in,out]   p_num_of_written_axi_ids    Number of axi ids stored in p_array
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK              All arguments are valid, and the list of all axi ids corresponding
 *                                      to the entered.
 * @retval  OSAL_RETURN_PAR             Invalid argument.
 *                                      - p_axi_bus_name is NULL.
 *                                      - p_num_of_written_axi_ids is NULL.
 *                                      - p_array is NULL and array_size is not 0.
 *                                      - if provided buffer is too small.
 * @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNKNOWN_NAME    If the entered axi_bus_name contains a combination of bus domain
 *                                      and uTLB to which axi_id is not assigned\n
 *                                      If the entered AXI Bus Name does not follow the format. \n
 *                                      Please refer to this API "Note"
 * @retval  OSAL_RETURN_OK_RQST         on success. This API inputs the number of axi ids associated 
 *                                      with p_axi_bus_name to p_num_of_written_axi_ids.
 *                                      - only p_array is NULL and array_size is 0.
 * @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
 * @note    axi_bus_name follows the format following.
 *          - bus domain and uTLB are connected by "_"
 *          - If uTLBs are consecutive, connect the first and last uTLB values with "-"
 *          - If uTLBs are discontinuous, connect with "_"
 *          - uTLBs are described by 2 digit decimal number
 *          - uTLB numbers are arranged in ascending order. \n
 *          example) \n
 *          When bus domain is "ir" and uTLBs are 0, 1, 2, 3, 4, 8, 9, 10,12 \n
 *          "ir_00-04_08-10_12".
 * @see
 *      - R_OSAL_IoGetAxiBusNameList()
 *      - R_OSAL_IoGetNumOfAxiIdsForAxiBusName()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiIdListForAxiBusName(const char* p_axi_bus_name, osal_axi_bus_id_t* p_array,
                                                   size_t array_size, size_t* const p_num_of_written_axi_ids);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_062]
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_067]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_028
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the number of all axi ids that correspond to the specified device handle.
 * @param[in]       device_hdl          To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_num_of_axi_ids    To set the pointer that store the number of available AXI IDs
 *                                      for that AXI Bus Name.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the number of all axi ids corresponding to the entered.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_num_of_axi_ids is NULL.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - device is not acquired by R_OSAL_IoDeviceOpen().
 *                              - device was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @note    Even if use the same device, the value that can be obtained varies depending on the SoC.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(osal_device_handle_t device_hdl, size_t* const p_num_of_axi_ids);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_063]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_029
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the list of all axi ids that correspond to the specified device handle. \n \n
 * @param[in]       device_hdl                  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_array                     To set the buffer address. OSAL sets all axi ids that correspond
 *                                              to axi_bus_name. 
 * @param[in]       array_size                  Maximum number of axi ids that can be written to p_array.
 * @param[in,out]   p_num_of_written_axi_ids    To set the pointer that store number of axi ids stored in p_array.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK      All arguments are valid, and the number of all axi ids corresponding to the entered.
 * @retval  OSAL_RETURN_PAR     Invalid argument.
 *                              - p_num_of_written_axi_ids is NULL.
 *                              - p_array is NULL and array_size is not 0.
 *                              - if provided buffer is too small.
 * @retval  OSAL_RETURN_HANDLE  Invalid handle.
 *                              - device is not acquired by R_OSAL_IoDeviceOpen().
 *                              - device was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_OK_RQST on success. This API inputs the number of axi ids associated 
 *                              with device_hdl to p_num_of_written_axi_ids.
 *                              - only p_array is NULL and array_size is 0.
 * @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
 * @note    Even if use the same device, the value that can be obtained varies depending on the SoC.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - R_OSAL_IoGetNumOfAxiIdsForDeviceHdl()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiIdListForDeviceHdl(osal_device_handle_t device_hdl, osal_axi_bus_id_t* p_array, 
                                                  size_t array_size, size_t* const p_num_of_written_axi_ids);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_064]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_030
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the number of all axi ids that correspond to the specified device name.
 * @param[in]       p_device_name       To set the value of target device name (a.k.a device id) acquired
 *                                      by R_OSAL_IoGetDeviceList().
 * @param[in,out]   p_num_of_axi_ids    To set the pointer that store the number of available AXI IDs 
 *                                      for the device handle.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK              All arguments are valid, and the number of all axi ids corresponding
 *                                      to the entered.
 * @retval  OSAL_RETURN_PAR             Invalid argument.
 *                                      - p_device_name is NULL.
 *                                      - p_num_of_axi_ids is NULL.
 * @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNKNOWN_NAME    If device name that is not supported by OSAL is entered.
 * @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
 * @note    Even if use the same device, the value that can be obtained varies depending on the SoC. \n
 *          Also, "DeviceName" included in the API name is the device id. \n
 *          If device name supported by OSAL don't support axi id, OSAL_RETURN_OK and p_num_of_axi_ids=0 are returned.
 * @see
 *      - R_OSAL_IoGetDeviceList()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForDeviceName(const char* p_device_name, size_t* const p_num_of_axi_ids);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_065]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_IO_031
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Returns the list of all axi ids that correspond to the specified device name \n
 * @param[in]       p_device_name               To set the value of target device name (a.k.a device id) acquired
 *                                              by R_OSAL_IoGetDeviceList().
 * @param[in,out]   p_array                     To set the buffer address. OSAL sets all axi ids
 *                                              that correspond to axi_bus_name. 
 * @param[in]       array_size                  Maximum number of axi ids that can be written to p_array.
 * @param[in,out]   p_num_of_written_axi_ids    To set the pointer that store number of axi ids stored in p_array.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK              All arguments are valid, and the number of all axi ids corresponding to the entered.
 * @retval  OSAL_RETURN_PAR             Invalid argument.
 *                                      - p_device_name is NULL.
 *                                      - p_num_of_written_axi_ids is NULL.
 *                                      - p_array is NULL and array_size is not 0.
 *                                      - if provided buffer is too small.
 * @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNKNOWN_NAME    If device name that is not supported by OSAL is entered.
 * @retval  OSAL_RETURN_OK_RQST         on success. This API inputs the number of axi ids associated 
 *                                      with p_device_name to p_num_of_written_axi_ids.
 *                                      - only p_array is NULL and array_size is 0.
 * @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
 * @note    Even if use the same device, the value that can be obtained varies depending on the SoC. \n
 *          Also, "DeviceName" included in the API name is the device id. \n
 *          If device_id supported by OSAL don't support axi id, OSAL_RETURN_OK and p_num_of_axi_ids=0 are returned.
 * @see
 *      - R_OSAL_IoGetDeviceList()
 *      - R_OSAL_IoGetNumOfAxiIdsForDeviceName()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiIdListForDeviceName(const char* p_device_name, osal_axi_bus_id_t* p_array,
                                                   size_t array_size, size_t* const p_num_of_written_axi_ids);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_IO_066]
 * [Covers: XOS3_OSAL_NOS_AD_FD_IO_001]
***********************************************************************************************************************/

/**@} IO*/

/**
 * @defgroup OSAL_Power_Manager OSAL Power Manager
 * OSAL API for Power Management 
 * @{
 */

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_001
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Set the value of required-state to a device.
 * @param[in,out]   handle          To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]       state           To set the value of required status. \n
 *                                  - OSAL_PM_REQUIRED: Switching power state to required. \n
 *                                  - OSAL_PM_RELEASED: Switching power state to released.
 * @param[in]       applyImmediate  To set the value of the function's role. \n
 *                                  - true: Sync function, the state is applied immediately on the HW. \n
 *                                  - false: Async function, the state is applied late to the HW.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and required status was set to a device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - state is out of range. please see also #e_osal_pm_required_state_t.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_TIME                    applyImmediate is true and power state did not change during 
 *                                              the specified time.
 *                                              Please refer to each Wrapper User Manual for how to specify time.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    When control the same handle from different threads and interrupts, ensure that the power state do 
 *          not conflict on the user side.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - #e_osal_pm_required_state_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t state,
                                          bool applyImmediate);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the value of required-state of a device.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_state To set the address of #e_osal_pm_required_state_t.
 *                          The OSAL sets the device's required status.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and required status was got from device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_state is NULL.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    None.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t* const p_state);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_003
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Wait until to apply the specified state or the specified time has elapsed.
 * @param[in]   handle      To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   state       To set the value of power status.
 * @param[in]   time_period Sets the value of time to wait for the power state is changed, in msec.
 *                          (valid values are >= 0)
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the PmState was changed to the specified 
 *                                              value successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - time_period is less than 0.
 *                                              - state is out of range. please see also #e_osal_pm_state_t.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_TIME                    Not changed state within the specified time.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    The waiting time may be longer than specified due to the influence of other threads.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - #e_osal_pm_state_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmWaitForState(osal_device_handle_t handle, e_osal_pm_state_t state,
                                      osal_milli_sec_t time_period);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_004
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the current power state of a device and set the value to p_state.
 * @param[in]       handle      To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_state     To set the address #e_osal_pm_state_t.
 *                              The OSAL sets the device's power status.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the power status was got from device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_state is NULL.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    None.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetState(osal_device_handle_t handle, e_osal_pm_state_t* const p_state);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_005
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the lowest intermediate all possible states of the Power Management State Machine (power/clock) 
 *          that was reached since the last change to non-required status and set to p_state.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_state To set the address #e_osal_pm_state_t.
 *                          The OSAL sets the device's power status.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the power status was got from device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_state is NULL.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    The state of the Power Management State Machine is defined in #e_osal_pm_state_t. 
 *          Higher values mean higher states. \n
 *          Example: The OSAL_PM_STATE_PG is lower than the OSAL_PM_STATE_CG state.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetLowestIntermediatePowerState(osal_device_handle_t handle, e_osal_pm_state_t* const p_state);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_006
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Set the requested power policy for a device.
 * @param[in]   handle          To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   policy          To set the value of device power policy.
 *                              - #OSAL_PM_POLICY_HP: Clock domain ON / Power domain ON
 *                              - #OSAL_PM_POLICY_CG: Clock domain OFF / Power domain ON
 *                              - #OSAL_PM_POLICY_PG: Clock domain OFF / Power domain OFF
 * @param[in]   applyImmediate  To set the value of the function's role.
 *                              - true: Sync function, when the required state is "Released" the policy is applied on 
 *                              the HW. The applyImmediate flag has no meaning when the Required State is "Required".
 *                              - false: Async function, the policy is applied late to the HW.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the power policy was set to device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - policy is out of range. please see also #e_osal_pm_policy_t.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_TIME                    When applyImmediate is true, power state did not change during 
 *                                              the specified time.
 *                                              Please refer to each Wrapper User Manual for how to specify time.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    The policy setting is reflected when the required state switches from "Required" to "Released" or 
 *          during "Released". 
 *          When control the same handle from different threads and interrupts, ensure that the power state do 
 *          not conflict on the user side.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - #e_osal_pm_policy_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t policy, bool applyImmediate);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_007
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the power policy of a device and set the value to p_policy.
 * @param[in]       handle      To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_policy    To set the address #e_osal_pm_policy_t.
 *                              The OSAL sets the device's power policy.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the power policy was set to device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_policy is NULL.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    None.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t* const p_policy);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Set the reset state value for a device.
 * @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in]   state   To set value of type #e_osal_pm_reset_state_t that identifies.
 *                      the requested reset status for the device.
 *                      - #OSAL_PM_RESET_STATE_APPLIED: Reset.
 *                      - #OSAL_PM_RESET_STATE_RELEASED: Release reset.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and reset status was set to a device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
                                                - state is out of range. please see also #e_osal_pm_reset_state_t.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    When control the same handle from different threads and interrupts, ensure that the power state do 
 *          not conflict on the user side.
 * @see
 *      - R_OSAL_IoDeviceOpen()
 *      - #e_osal_pm_reset_state_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t state);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_009
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the reset state value of a device and set the value to p_state.
 * @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
 * @param[in,out]   p_state To set the address of #e_osal_pm_reset_state_t.
 *                          The OSAL sets the reset status.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the reset status was got from device 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_state is NULL.
 * @retval  OSAL_RETURN_DEV                     HW fault detected.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - handle was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
 * @note    None.
 * @see
 *      R_OSAL_IoDeviceOpen()
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_state);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_POW_010
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Set the POST clock and modify its clock divider for a given hierarchy the device is in.
 * @param[in]   device          Device to configure the POST Clock for
 * @param[in]   clock_enable    Enable POST Clock for the hierarchy the device is in.
 * @param[in]   clock_divider   Clock divider setting. [clock_divider < 64]
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the POST clock was set and modify 
 *                                              its clock divider successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - clock_divider is out of range.
 * @retval  OSAL_RETURN_DEV                     in case the underlying device cannot be accessed.
 * @retval  OSAL_RETURN_IO                      in case IO operation failed
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - device is not acquired by R_OSAL_IoDeviceOpen().
 *                                              - device was already closed by R_OSAL_IoDeviceClose().
 * @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device does not support POST Clock settings.
 * @note    Only FBC/FBA devices do support this operation. Any other device will return \n
 *          #OSAL_RETURN_UNSUPPORTED_OPERATION. \n
 *          Due to HW the POST Clock settings may be shared across multiple devices if they belong to the same 
 *          Clock hierarchy. It is users Responsibility to ensure FFI.
 * @see
 *      None \n\n\n\n
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetPostClock(osal_device_handle_t device, bool clock_enable, uint32_t clock_divider);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_POW_007]
***********************************************************************************************************************/

/**@} PM*/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_001
***********************************************************************************************************************/
/**
 * @defgroup OSAL_Memory_Manager OSAL Memory Manager
 * OSAL API for Memory Management \n
 * OSAL Memory Manager work with contiguous, non-peripheral, non-persistent and non-moveable memory regions. \n
 * The memory is allocated by the consecutive memory allocation function and the allocated memory is not remapped to 
 * keep exposed physical memory valid.  All bookkeeping memories are allocated during initialization unless 
 * maximum values are set to unlimited in the configuration.
 * @{
 */
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_024]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_025]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_002
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Factory method to create the OSAL Memory Manager Instance with the given configuration.
 *          To change configuration, the manager must be closed and re-opened. Closing and re-open the OSAL 
 *          Memory Manager will potentially result in allocation/de-allocation of bookkeeping memory resources 
 *          and should be limited to initialization phase in restricted environments.
 * @param[in]       p_config        Configuration to instantiate the OSAL Memory Manager with
 * @param[in,out]   p_hndl_mmngr    Handle to the OSAL MMNGR instance
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                  All arguments are valid, and the OSAL Memory Manager was created 
 *                                          with the given configuration successfully.
 * @retval  OSAL_RETURN_PAR                 Invalid argument.
 *                                          - p_config is NULL.
 *                                          - p_hndl_mmngr is NULL.
 * @retval  OSAL_RETURN_MEM                 Internal Bookkeeping memory could not be allocated.
 * @retval  OSAL_RETURN_DEV                 OS specific memory resource could not be opened/accessed.
 * @retval  OSAL_RETURN_TIME                Operation took too long.
 * @retval  OSAL_RETURN_CONF                Requested Configuration is invalid. please see also #st_osal_mmngr_config_t.
 * @retval  OSAL_RETURN_FAIL                An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE
 *                                          -# A memory region is already opened.
 *                                          -# OSAL is not initialized. (Before calling R_OSAL_Initialize() or after 
 *                                          calling R_OSAL_Deinitialize())
 * @retval  OSAL_RETURN_CONF_UNSUPPORTED    Requested allocate mode is not supported. 
 *                                          please see also #e_osal_mmngr_allocator_mode_t.
 * @note    In case of calling this API with a config initialized with R_OSAL_MmngrInitializeMmngrConfigSt(), 
 *          #OSAL_RETURN_CONF is returned. Before calling this API, the config value  should be initialized with 
 *          valid value. \n
 *          OSAL Memory Manager will always allocate memory that is Cached, CPU read/write accessible. 
 *          IOMMU/IPMMU configuration is not known by OSAL Memory Manager and it is assumed HW has read/write 
 *          access. IOMMU/IPMMU configuration that changes Virtual address spaces of the HW shall implemented 
 *          as Specific Memory Manager that is create Buffer objects that can consider IOMMU/IPMMU 
 *          configurations. This user Memory Manager shall take ownership of the OSAL Memory Manager to prevent
 *          other SW from accessing the OSAL MMNGR instance.
 *          OSAL Memory Manager cannot be opened twice! \n
 *          Default capabilities can be get by calling R_OSAL_MmngrGetConfig() or R_OSAL_MmngrGetOsalMaxConfig().
 * @see
 *      - #st_osal_mmngr_config_t
 *      - #e_osal_mmngr_allocator_mode_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrOpen(const st_osal_mmngr_config_t * const p_config,
                                 osal_memory_manager_handle_t * const p_hndl_mmngr);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_020]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_044]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_027
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Factory method to create the OSAL Memory Manager Instance with the given configuration.
 *          This API specifies IDs for different memory region and creates an instance for each memory region.
 *          To change configuration, the manager must be closed and re-opened. Closing and re-open the OSAL 
 *          Memory Manager will potentially result in allocation/de-allocation of bookkeeping memory resources 
 *          and should be limited to initialization phase in restricted environments.
 * @param[in]       p_config        Configuration to instantiate the OSAL Memory Manager with
 * @param[in]       region_idx      ID of memory region
 * @param[in,out]   p_hndl_mmngr    Handle to the OSAL MMNGR instance
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                  All arguments are valid, and the OSAL Memory Manager was created 
 *                                          with the given configuration successfully.
 * @retval  OSAL_RETURN_PAR                 Invalid argument.
 *                                          - p_config is NULL.
 *                                          - Specified region_idx is unknown.
 *                                          - p_hndl_mmngr is NULL.
 * @retval  OSAL_RETURN_MEM                 Internal Bookkeeping memory could not be allocated.
 * @retval  OSAL_RETURN_DEV                 OS specific memory resource could not be opened/accessed.
 * @retval  OSAL_RETURN_TIME                Operation took too long.
 * @retval  OSAL_RETURN_CONF                Requested Configuration is invalid. please see also st_osal_mmngr_config_t.
 * @retval  OSAL_RETURN_FAIL                An error occurred in OS or related API.
 * @retval  OSAL_RETURN_STATE
 *                                          -# A memory region is already opened.
 *                                          -# OSAL is not initialized. (Before calling R_OSAL_Initialize() or after 
 *                                          calling R_OSAL_Deinitialize())
 * @retval  OSAL_RETURN_CONF_UNSUPPORTED    Requested allocate mode is not supported. 
 *                                          please see also #e_osal_mmngr_allocator_mode_t.
 * @note    In case of calling this API with a config initialized with R_OSAL_MmngrInitializeMmngrConfigSt(), 
 *          #OSAL_RETURN_CONF is returned. Before calling this API, the config value  should be initialized with 
 *          valid value. \n
 *          OSAL Memory Manager will always allocate memory that is Cached, CPU read/write accessible. 
 *          IOMMU/IPMMU configuration is not known by OSAL Memory Manager and it is assumed HW has read/write 
 *          access. IOMMU/IPMMU configuration that changes Virtual address spaces of the HW shall implemented 
 *          as Specific Memory Manager that is create Buffer objects that can consider IOMMU/IPMMU 
 *          configurations. This user Memory Manager shall take ownership of the OSAL Memory Manager to prevent
 *          other SW from accessing the OSAL MMNGR instance.
 *          OSAL Memory Manager cannot be opened twice which same memory region. \n
 *          Default capabilities can be get by calling R_OSAL_MmngrGetConfig() or R_OSAL_MmngrGetOsalMaxConfig().
 * @see
 *      - st_osal_mmngr_config_t
 *      - #e_osal_mmngr_allocator_mode_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrOpenFromRegionIdx(const st_osal_mmngr_config_t * const p_config,
                                              osal_memory_region_idx_t region_idx,
                                              osal_memory_manager_handle_t * const p_hndl_mmngr);
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_AD_FD_MEM_030]
 * [Covers: XOS3_OSAL_NOS_AD_FD_MEM_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_003
***********************************************************************************************************************/
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
 *                                              - hndl_mmngr is NULL.
 *                                              - hndl_mmngr (Not NULL) has not been opened by R_OSAL_MmngrOpen()/
 *                                              R_OSAL_MmngrOpenFromRegionIdx() yet.
 *                                              - hndl_mmngr->p_ptr_self is invalid.
 *                                              - buffer_obj->p_ptr_self is invalid (NULL). \n
 *                                              buffer_obj manage the buffer allocated by using hndl_mmngr
 * @retval  OSAL_RETURN_STATE
 *                                              -# hndl_mmngr has already been closed.
 *                                              -# OSAL is not initialized. (Before calling R_OSAL_Initialize() or 
 *                                              after calling R_OSAL_Deinitialize())
 * @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The handle is not an handle of type OSAL Memory Manager. 
 *                                              Use the Memory Manager specific function to close it.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrClose(osal_memory_manager_handle_t hndl_mmngr);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_004
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Allocate memory from the memory manager hndl_mngr with the given size and alignment and default 
 *          memory attributes of the memory manager. \n 
 * @param[in]       hndl_mmngr      Handle to the Memory Manager to allocate the memory from.
 * @param[in]       size            Requested size of the memory: [range: n \* minimumCachelineSize ...
 *                                  maxBufferSize] \(e.g. n \* 64 Byte\) where n = 1,2,3...
 * @param[in]       alignment       Alignment of the memory [range: 2^6, 2^7, ..., 2^n with n limited
 *                                  by the system or MMNGR ]
 * @param[in,out]   p_buffer_obj    Handle to the allocate buffer object
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the memory was allocated successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_buffer_obj is NULL.
 * @retval  OSAL_RETURN_MEM                     Memory manager is not capable of allocating requested memory
 *                                              with the given parameters. E.g. run out of memory
 * @retval  OSAL_RETURN_DEV                     Underlying memory manager cannot be accessed.
 * @retval  OSAL_RETURN_TIME                    Operation took too long.
 * @retval  OSAL_RETURN_CONF                    The given size and alignment values to not match with 
 *                                              the memory managers requirements for allocation memory. 
 *                                              please see also #st_osal_mmngr_config_t.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - hndl_mmngr is NULL
 *                                              - hndl_mmngr->p_ptr_self is invalid
 * @retval  OSAL_RETURN_STATE
 *                                              - hndl_mmngr (Not NULL) has not been opened by R_OSAL_MmngrOpen()/
 *                                              R_OSAL_MmngrOpenFromRegionIdx() yet.
 * @retval  OSAL_RETURN_PROHIBITED              The maximum amount of allowed allocations is reached.
 * @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
 * @note    This function will always use the default memory attributes of the memory manager. OSAL Memory 
 *          Manager only supports one set of attributes. If you want to allocate memory with different 
 *          attributes, please use the Specific Memory Manager function (e.g. future HWA Memory Manager). \n
 *          Similar to standard allocations the actually allocated memory may be larger than the requested size
 *          to fit alignments, avoid fragmentations and underflow/overflow detections. The reported allocated
 *          size of the buffer. \n
 *          The reported allocated size of the buffer (R_OSAL_MmngrGetSize()) will always match the user requested size.
 * @see
 *      - #st_osal_mmngr_config_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrAlloc(osal_memory_manager_handle_t hndl_mmngr, size_t size, size_t alignment,
                                  osal_memory_buffer_handle_t * const p_buffer_obj);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_005
***********************************************************************************************************************/
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
 *                                              - hndl_mmngr is NULL.
 *                                              - buffer_obj is NULL.
 *                                              - hndl_mmngr->p_ptr_self or buffer_obj->p_ptr_self is invalid 
 *                                              (NULL/not NULL). handle might be corrupted.
 * @retval  OSAL_RETURN_STATE
 *                                              - hndl_mmngr (Not NULL) has not been opened by R_OSAL_MmngrOpen()/
 *                                              R_OSAL_MmngrOpenFromRegionIdx() yet.
 * @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED
 *                                              -# The provided mmngr does not support this operation.
 *                                              -# The buffer object is not managed by Memory Manager hndl_mngr.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrDealloc(osal_memory_manager_handle_t hndl_mmngr,
                                    osal_memory_buffer_handle_t  buffer_obj);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_006
***********************************************************************************************************************/
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
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_config is NULL.
 * @retval  OSAL_RETURN_CONF                    Wrong configuration value.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  
 *                                              - hndl_mmngr is invalid. (not #OSAL_MEMORY_MANAGER_HANDLE_INVALID).
 *                                              e.g. internal handle parameters are invalid.
 *                                              - hndl_mmngr->p_ptr_self is invalid
 *                                              handle might be corrupted.
 * @retval  OSAL_RETURN_STATE
 *                                              - hndl_mmngr (Not NULL) has not been opened by R_OSAL_MmngrOpen()/
 *                                              R_OSAL_MmngrOpenFromRegionIdx() yet.
 * @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
 * @note    None.
 * @see
 *      - #st_osal_mmngr_config_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetConfig(osal_memory_manager_handle_t hndl_mmngr,
                                      st_osal_mmngr_config_t * const p_config);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_007
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the default/maximum configuration of the OSAL Memory Manager and set to p_config.
 * @param[in,out]   p_config    Will contain the requested configuration of the provided mmngr 
 *                              or the default/maximum configuration of the OSAL MMNGR.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the maximum configuration of the provided 
 *                                              memory manager was returned successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_config is NULL.
 * @retval  OSAL_RETURN_CONF                    Wrong configuration value.
 * @note    None.
 * @see
 *      - #st_osal_mmngr_config_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetOsalMaxConfig(st_osal_mmngr_config_t * const p_config);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_028
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the default/maximum configuration of the specified memory region and set to p_config.
 * @param[in]       region_idx  ID of memory region
 * @param[in,out]   p_config    Will contain the requested configuration of the provided mmngr 
 *                              or the default/maximum configuration of the OSAL MMNGR.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the maximum configuration of the provided 
 *                                              memory manager was returned successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - Specified region_idx is unknown.
 *                                              - p_config is NULL.
 * @retval  OSAL_RETURN_CONF                    Wrong configuration value.
 * @note    None.
 * @see
 *      - #st_osal_mmngr_config_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(osal_memory_region_idx_t region_idx,
                                                          st_osal_mmngr_config_t * const p_config);
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_AD_FD_MEM_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Initializes all members of #st_osal_mmngr_config to invalid/defined values.
 *          - p_config->mode is #OSAL_MMNGR_ALLOC_MODE_INVALID
 *          - p_config->memory_limit is 0
 *          - p_config->max_allowed_allocations is 0
 *          - p_config->max_registered_monitors_cbs is 0
 * @param[in,out]   p_config    structure to initialize.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and all members of #st_osal_mmngr_config was 
 *                                              initialized successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_config is NULL.
 * @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInitializeMmngrConfigSt(st_osal_mmngr_config_t * const p_config);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_009
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Registers a monitor function to a mmngr for monitoring a specific event of the MMNGR
 *          This functions registers a monitor/observer to a memory manager for monitoring events 
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
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - monitor_handler is NULL.
 *                                              - monitored_event is out of range. please see also 
 *                                              #e_osal_mmngr_event_t.
 * @retval  OSAL_RETURN_MEM                     Internal bookkeeping memory allocation failed.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - hndl_mmngr is NULL
 *                                              - hndl_mmngr->p_ptr_self is invalid
 *                                              handle might be corrupted.
 *                                              - hndl_mmngr was already closed by R_OSAL_MmngrClose().
 * @retval  OSAL_RETURN_STATE
 *                                              - hndl_mmngr (Not NULL) has not been opened by R_OSAL_MmngrOpen()/
 *                                              R_OSAL_MmngrOpenFromRegionIdx() yet.
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
 * @see
 *      - #e_osal_mmngr_event_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrRegisterMonitor(osal_memory_manager_handle_t hndl_mmngr,
                                            e_osal_mmngr_event_t monitored_event,
                                            fp_osal_memory_mngr_cb_t monitor_handler, void * p_user_arg);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_021]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_022]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_045]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_010
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the CPU accessible (read/write) pointer of the buffer object and set to pp_cpu_ptr.
 * @param[in]       memory_buffer_obj_hndl  Handle of the buffer object
 * @param[in,out]   pp_cpu_ptr              pointer to the cpu modifiable memory
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the CPU accessible (read/write) pointer 
 *                                              of the buffer object was returned successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid handle.
 *                                              - pp_cpu_ptr is NULL.
 * @retval  OSAL_RETURN_MEM                     Mapping/copying the buffer content to CPU failed.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - memory_buffer_obj_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->p_ptr_self is invalid. 
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self is NULL.
 * @retval  OSAL_RETURN_PROHIBITED              Read/write access by CPU is prohibited to the buffer object.
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    Be aware that acquiring CPU accessible pointer may not be permitted. Call to this function shall be
 *          restricted to driver/middleware implementations if modifying memory content is not mandatory. \n
 *          Be aware that any modification done by the CPU must be synced back to HW by calling 
 *          R_OSAL_MmngrFlush(). If this is not done the modified memory content may not be or only partially 
 *          visible to the HW. \n
 *          This operation may require to reserve memory on first call and may fail with #OSAL_RETURN_MEM. See
 *          documentation of the Memory Manager that created the buffer object for more details.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl, void ** const pp_cpu_ptr);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_011
***********************************************************************************************************************/
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
 *                                              - memory_buffer_obj_hndl->p_ptr_self is invalid. 
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self is NULL.
 * @retval  OSAL_RETURN_PROHIBITED              Read access by CPU is prohibited to the buffer object.
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    Be aware that acquiring CPU accessible pointer may not be permitted. Call to this function shall be
 *          restricted to driver/middleware implementations if reading memory content is not mandatory. \n
 *          Be aware that any expected modification done by the HW from the point in time this cpu_ptr is 
 *          acquired must be synced to CPU by calling R_OSAL_MmngrInvalidate(). If this is not done the 
 *          modified memory content may not be or only partially updated to the CPU. \n
 *          This operation may require to reserve memory on first call and may fail with #OSAL_RETURN_MEM. See
 *          documentation of the Memory Manager that created the buffer object for more details.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetConstCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                           const void ** const pp_const_cpu_ptr);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_012
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Synchronizes a sub region of the buffer CPU memory to the HW. \n
 *          Synchronizes CPU memory to the HW by making sure the memory content accessible by the HW is 
 *          identical with the CPU memory view.
 * @param[in]   memory_buffer_obj_hndl  Handle of the buffer object
 * @param[in]   offset                  Offset with the buffer object limits [range: 0, 
 *                                      n*minimumCachelineSize ... maxBufferSize] n = 1,2,3,... 
 *                                      (e.g. 0, n* 64 Byte)
 * @param[in]   size                    Size of the region to be synced [range: [buffer_specific,
 *                                      typical 64Bytes]--buffer_size].
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the sub region of the buffer CPU memory 
 *                                              was synchronized to the HW successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - offset is out of range for the given buffer object.
 *                                              - offset is not aligned 64 byte.
 *                                              - size is 0.
 *                                              - size is out of range for the given buffer object.
 *                                              - size is not aligned 64 byte.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - memory_buffer_obj_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->p_ptr_self is invalid. 
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self is NULL.
 * @retval  OSAL_RETURN_PROHIBITED              Write access by CPU is prohibited to the buffer object.
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    This operation may overwrite memory modified by the HW since the last call to R_OSAL_MmngrFlush().
 *          User must make sure this is avoided or will not have impact. \n
 *          Provided size must be multiple of the minimal size. Typically multiple of 64Byte 
 *          (ARM Cache Line Size). \n
 *          This operation will return #OSAL_RETURN_OK also in case no synchronization is required due to CPU 
 *          memory view and HW memory view is ensured to be same. Example: Memory is uncached.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset, size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_013
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Synchronizes a sub region of the buffer HW memory to the CPU memory. \n
 *          Synchronizes HW memory with the CPU by making sure the memory content accessible by the HW is 
 *          identical with the CPU memory view.
 * @param[in]   memory_buffer_obj_hndl  Handle of the buffer object
 * @param[in]   offset                  Offset with the buffer object limits [range: 0, 
 *                                      n*minimumCachelineSize ... maxBufferSize] n = 1,2,3, ... 
 *                                      (e.g. 0, n* 64 Byte)
 * @param[in]   size                    Size of the region to be synced [range: [buffer_specific, 
 *                                      typical 64Bytes]--buffer_size].
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the sub region of the buffer HW memory 
 *                                              was synchronized to the CPU memory successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                               - offset is out of range for the given buffer object.
 *                                               - offset is not aligned 64 byte.
 *                                               - size is 0.
 *                                               - size is out of range for the given buffer object.
 *                                               - size is not aligned 64 byte.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - memory_buffer_obj_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->p_ptr_self is invalid. 
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self is NULL.
 * @retval  OSAL_RETURN_PROHIBITED              Write access by CPU is prohibited to the buffer object.
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    This operation may overwrite memory modified by the CPU since the last call to 
 *          R_OSAL_MmngrInvalidate(). User must make sure this is avoided or will not have impact. \n
 *          Provided size must be multiple of the minimal size. Typically multiple of 64Byte
 *          (ARM Cache Line Size). \n
 *          This operation will return #OSAL_RETURN_OK also in case no synchronization is required due to CPU 
 *          memory view and HW memory view is ensured to be same. Example: Memory is uncached.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset,
                                       size_t size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_014
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get the hw/peripheral/axi bus domain specific address of the buffer and set to p_hw_addr.
 * @param[in]       memory_buffer_obj_hndl  Handle of the buffer object
 * @param[in]       axi_id                  axi id
 * @param[in,out]   p_hw_addr               To set the address of uintptr_t. 
 *                                          The OSAL sets the hardware address.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the axi bus domain specific address of 
 *                                              the buffer was returned successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - axi_id is #OSAL_AXI_BUS_ID_INVALID.
 *                                              - p_hw_addr is NULL.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - memory_buffer_obj_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->p_ptr_self is invalid. 
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self is NULL.
 * @retval  OSAL_RETURN_PROHIBITED              The peripheral is not allowed to access this buffer object.
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    The hw_addr may differ between devices/peripherals due to external factors (e.g. IOMMU/IPMMU
 *          configuration). It must be ensured the hw_addr provided by this function is only used by the 
 *          peripheral it was requested from. See documentation of the Memory Manager that created the buffer 
 *          object for more details.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetHwAddr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                      osal_axi_bus_id_t axi_id, uintptr_t * const p_hw_addr);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_015
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Get this size of the buffer and set to p_size.
 * @param[in]       memory_buffer_obj_hndl  Handle of the buffer object
 * @param[in,out]   p_size                  To set the address of size_t.
 *                                          The OSAL sets the buffer size.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the size of buffer was returned 
 *                                              successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_size is NULL.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - memory_buffer_obj_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->p_ptr_self is invalid. 
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self is NULL.
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetSize(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t * const p_size);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_016
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Triggers a checks the current buffer object for integrity. E.g. by checking for underflow or 
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
 * mmngr to be modified. This situation would be automatically be reported by the MMNGR by calling the matching
 * monitor. This function would trigger this event again.
 *
 * @param[in]   memory_buffer_obj_hndl  Handle of the buffer object.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the current buffer object for integrity 
 *                                              is ok. (no overflow/underflow)
 * @retval  OSAL_RETURN_MEM                     The buffer integrity is not ok (overflow/underflow)
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - memory_buffer_obj_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->p_ptr_self is invalid. 
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl is NULL.
 *                                              - memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self is NULL.
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    This operation must be supported by the buffer object and mmngr.
 * @see
 *      - #e_osal_mmngr_event_t
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrCheckBuffer(osal_memory_buffer_handle_t memory_buffer_obj_hndl);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_002]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_026]
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Debug Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_017
***********************************************************************************************************************/
/* PRQA S 2053 32 # False positive. */
/*******************************************************************************************************************//**
 * @brief   Prints human readable information about the current mmngr state.
 *          This function prints Memory Manager informations in following format: \n
 *          (Generic Manager Information) \n
 *          (Manager Specific information, format undefined in OSAL API) \n
 *          (Table Header) \n
 *          (Output from R_OSAL_MmngrDebugBufferPrint()) \n
 *
 * Example:
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
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_output is NULL.
 * @retval  OSAL_RETURN_IO                      on write to output stream failed.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - hndl_mmngr is NULL.
 *                                              - hndl_mmngr->p_ptr_self is invalid
 *                                              handle might be corrupted.
 * @retval  OSAL_RETURN_STATE
 *                                              - hndl_mmngr (Not NULL) has not been opened by R_OSAL_MmngrOpen()/
 *                                              R_OSAL_MmngrOpenFromRegionIdx() yet.
 * @retval  OSAL_RETURN_HANDLE_NOT_SUPPORTED    The provided mmngr does not support this operation.
 * @note    this function will only be available if debug builds are enabled or the specific 
 *          R_OSAL_ENABLE_DEBUG flag is set. \n
 *          this function shall not be used in production environment.
 * @see
 *      - #FILE
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrDebugMmngrPrint(osal_memory_manager_handle_t hndl_mmngr, FILE * p_output);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_018
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Prints human readable information about the current mmngr state.
 *
 * ```bash
 * The function prints buffer information in following format if Buffer is mapped identical in all HWA domains \n
 * "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE (MM)   ; ns 
 * ;   ; 0x123456743; P:readonly U:readonly ... ;" \n
 *
 * In case of different mappings, multiple lines are dumped with information for each domains. Entries with same
 * mappings can be grouped: \n
 * "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE ; 
 * 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE (MM)      ; ns  ;   ; 0x123456743; P:readonly U:readonly ... ;" \n
 * "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE ; 0x1234567891234567--0x1234567891234567 (A3IR,VI) ; 
 * ns  ;   ; 0x123456743; P:readonly U:readonly ... ;" \n
 * "0xAFFEBABEAFFEBABE ; 0xAFFEBABEAFFEBABE--0xAFFEBABEAFFEBABE 
 * ; 0x9876543219876543--0x9876543219876543 (VO)      ; ns  ;   ; 0x123456743; P:readonly U:readonly ... ;"
 * ```
 * @param[in]   hndl_buffer Handle of an buffer object
 * @param[in]   p_output    Output stream to write the information to.
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK                      All arguments are valid, and the human readable information about 
 *                                              the current mmngr state was printed successfully.
 * @retval  OSAL_RETURN_PAR                     Invalid argument.
 *                                              - p_output is NULL.
 * @retval  OSAL_RETURN_IO                      on write to output stream failed.
 * @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
 * @retval  OSAL_RETURN_HANDLE                  Invalid handle.
 *                                              - hndl_buffer is NULL.
 *                                              - hndl_buffer->p_ptr_self is invalid. 
 * @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The provider buffer object does not support this operation.
 * @note    This function will only be available if debug builds are enabled or the specific 
 *          R_OSAL_ENABLE_DEBUG flag is set. \n
 *          This function shall not be used in production environment.
 * @see
 *      - #FILE
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrDebugBufferPrint(osal_memory_buffer_handle_t hndl_buffer, FILE * p_output);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_005]
***********************************************************************************************************************/

/**@} MMNGR */
/** @} OSAL_API */

#ifdef __cplusplus
}
#endif

#if !defined(_STDIO_H_INCLUDED) && !defined(_STDIO_H) && !defined(_STDIO_H_) && !defined(_INC_STDIO) && !defined(__stdio_h)
#if defined(NDEBUG) && !defined(R_OSAL_ENABLE_DEBUG)
/* PRQA S 4601,0841 1 # Since it is the macro "FILE" declared when stdio.h is not used, undefining it is no problem. */
#undef FILE
#endif
#endif

#endif /* OSAL_API_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

