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
* File Name :    r_osal_common.h
* Version :      0.6.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper common for eMCOS header file
***********************************************************************************************************************/

#ifndef OSAL_COMMON_H
#define OSAL_COMMON_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <pthread.h>
#include <errno.h>

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_configuration/target/emcos/r_osal_usr_cfg_info.h"
#include "target/emcos/usr_cfg/r_osal_usr_cfg_control.h"
#include "rcar-xos/osal_configuration/target/emcos/r_osal_dev_cfg_info.h"
#include "target/emcos/dev_cfg/r_osal_dev_cfg_control.h"

/*******************************************************************************************************************//**
 * @defgroup OSAL_WRAPPER_EMCOS OSAL Wrapper For eMCOS
 * OSAL Wrapper for eMCOS
 * @{
 **********************************************************************************************************************/
/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Common Common macro definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_DEBUG
 * debug print.
***********************************************************************************************************************/
#define OSAL_DEBUG(...)

/*******************************************************************************************************************//**
 * @def OSAL_STATIC
***********************************************************************************************************************/
#define OSAL_STATIC static

/*******************************************************************************************************************//**
 * @def OSAL_MAX_SEC
 * Max value of second
***********************************************************************************************************************/
#define OSAL_MAX_SEC   (0x7FFFFFFFL)

/*******************************************************************************************************************//**
 * @def OSAL_MAX_TV_NSEC
 * Max value of nano second
***********************************************************************************************************************/
#define OSAL_MAX_TV_NSEC (1000000000L - 1L)

/*******************************************************************************************************************//**
 * @def OSAL_MSECTONSEC
 * Mask value for converting milli second to nano second
***********************************************************************************************************************/
#define OSAL_MSECTONSEC (1000000L)

/*******************************************************************************************************************//**
 * @def OSAL_SECTOMSEC
 * Mask value for converting second to miili second
***********************************************************************************************************************/
#define OSAL_SECTOMSEC (1000L)

/*******************************************************************************************************************//**
 * @def OSAL_SECTONSEC
 * Mask value for converting second to nano second
***********************************************************************************************************************/
#define OSAL_SECTONSEC (1000000000L)

/*******************************************************************************************************************//**
 * @def OSAL_INNER_TIMEOUT
 * Mask value for timeout used in osal internal exclusive control
***********************************************************************************************************************/
#define OSAL_INNER_TIMEOUT ((osal_milli_sec_t)500U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_CLOCKTIME
 * Initialization bit setting value for Clock&Time Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_CLOCKTIME  (0x01U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_MQ
 * Initialization bit setting value for Message Queue Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_MQ         (0x02U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_THREADSYNC
 * Initialization bit setting value for ThreadSync Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_THREADSYNC (0x04U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_INTERRUPT
 * Initialization bit setting value for Interrupt Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_INTERRUPT  (0x08U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_PM
 * Initialization bit setting value for Power Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_PM         (0x10U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_IO
 * Initialization bit setting value for IO Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_IO         (0x20U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_MEMORY
 * Initialization bit setting value for Memory Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_MEMORY     (0x40U)

/*******************************************************************************************************************//**
 * @def OSAL_ISINIT_THREAD
 * Initialization bit setting value for Thread Manager
***********************************************************************************************************************/
#define OSAL_ISINIT_THREAD     (0x80U)

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Enum_Common Common Enumration.
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @enum e_osal_inner_thread_policy_t
 * Thread Policy definitions.
***********************************************************************************************************************/
typedef enum e_osal_inner_thread_policy
{
    OSAL_SCHED_INVALID = 0,
    OSAL_SCHED_FIFO,
    OSAL_SCHED_RR
}e_osal_inner_thread_policy_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Types_Common Common type definitions.
 *
 * @{
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @typedef osal_inner_thread_attr_t
 * @brief   thread attribute. \n
 *          This type is a typedef definition for a pointer to a struct st_osal_inner_thread_attr.
 **********************************************************************************************************************/
typedef struct st_osal_inner_thread_attr *osal_inner_thread_attr_t;

/*******************************************************************************************************************//**
 * @typedef osal_inner_thread_handle_t
 * @brief   Handle of Thread. \n
 *          This type is a typedef definition for a pointer to a st_osal_inner_thread_control.
 **********************************************************************************************************************/
typedef struct st_osal_inner_thread_control *osal_inner_thread_handle_t;

/*******************************************************************************************************************//**
 * @typedef osal_inner_mutexattr_t
 * @brief   mutex attribute. \n
 *          This type is a typedef definition for a pointer to a struct st_osal_inner_mutexattr.
 **********************************************************************************************************************/
typedef struct st_osal_inner_mutexattr *osal_inner_mutexattr_t;

/*******************************************************************************************************************//**
 * @typedef osal_inner_mutex_handle_t
 * @brief   Handle of Mutex. \n
 *          This type is a typedef definition for a pointer to a struct st_osal_inner_mutex_control.
 **********************************************************************************************************************/
typedef struct st_osal_inner_mutex_control *osal_inner_mutex_handle_t;

/*******************************************************************************************************************//**
 * @typedef osal_inner_cond_handle_t
 * @brief   Handle of condition. \n
 *          This type is a typedef definition for a pointer to a struct st_osal_inner_cond_control.
 **********************************************************************************************************************/
typedef struct st_osal_inner_cond_control *osal_inner_cond_handle_t;

/*******************************************************************************************************************//**
 * @typedef osal_inner_mq_handle_t
 * @brief   Message information. \n
 *          This type is a typedef definition for a pointer to a struct st_osal_inner_mq_control.
 **********************************************************************************************************************/
typedef struct st_osal_inner_mq_control *osal_inner_mq_handle_t;

/*******************************************************************************************************************//**
 * @typedef p_osal_inner_thread_func_t
 * @brief   inner thread worker function.
 **********************************************************************************************************************/
typedef void* (*p_osal_inner_thread_func_t)(void* user_arg);

/*******************************************************************************************************************//**
 * @st_osal_inner_thread_attrinfo_t
 * structure to thread attribute information.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_inner_thread_attrinfo
{
    e_osal_inner_thread_policy_t policy;
    e_osal_thread_priority_t     priority;
    size_t                       stack_size;
} st_osal_inner_thread_attrinfo_t;

/*******************************************************************************************************************//**
 * @struct st_osal_inner_thread_config_t
 * structure to thread configuration.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_inner_thread_config
{
    osal_inner_thread_attr_t    *attr;
    p_osal_inner_thread_func_t  func;      /*!< Worker function */
    void*                       userarg;   /*!< Pointer of Argument for worker function */
} st_osal_inner_thread_config_t;

/*******************************************************************************************************************//**
 * @struct st_osal_inner_mq_config_t
 * structure to Message queue configuration.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_inner_mq_config
{
    uint64_t max_num_msg;
    uint64_t msg_size;
} st_osal_inner_mq_config_t;

/** @} */

extern st_pdev_entry_t g_pdev_entry;  /* TODO: pdev information should be moved to dev_cfg */

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Functions_Common Common function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Initialize Thread Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
extern e_osal_return_t osal_thread_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Thread Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t osal_thread_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Thread Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t osal_thread_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the Thread Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_thread_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Initialize ThreadSync Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
extern e_osal_return_t osal_threadsync_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize ThreadSync Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t osal_threadsync_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize ThreadSync Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t osal_threadsync_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the ThreadSync Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_threadsync_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Initialize Message Queue Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t osal_mq_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Message Queue Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t osal_mq_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Message Queue Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t osal_mq_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the Message Queue Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_mq_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Initialize Clock Time Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t osal_clock_time_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Clock Time Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
extern e_osal_return_t osal_clock_time_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Clock Time Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t osal_clock_time_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the Clock Time Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_clock_time_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Initialize interrupt Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
extern e_osal_return_t osal_interrupt_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize interrupt Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t osal_interrupt_deinitialize(void);

/***********************************************************************************************************************
* @brief            Pre-check for Deinitialize interrupt Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t osal_interrupt_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the interrupt Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_interrupt_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Initialize IO Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t osal_io_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize IO Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t osal_io_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize IO Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t osal_io_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the IO Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_io_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Initialize Power Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t osal_pm_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Power Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t osal_pm_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Power Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t osal_pm_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the Power Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_pm_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Initialize Memory Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t osal_memory_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Memory Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t osal_memory_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Memory Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t osal_memory_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Set the Memory Manager initialization flag to true.
* @return           void
***********************************************************************************************************************/
extern void osal_memory_set_is_init(void);

/*******************************************************************************************************************//**
* @brief            Check time-stamp.
* @param[in]        p_time_stamp  Set the time to be checked.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_CheckTimestamp(const st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Get a time-stamp.
* @param[out]       p_time_stamp  The OSAL sets the timestamp.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_GetTimestamp(struct timespec *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Sleep in milli second.
* @param[in]        milliseconds  Specify the sleep time in milliseconds.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MsSleep(osal_milli_sec_t milliseconds);

/*******************************************************************************************************************//**
* @brief            Lock a mutex(timed lock).
* @param[in]        p_mutex     The pointor of mutex object.
* @param[in]        abs_timeout Specify the timeout time in milliseconds.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_TimedlockMutex(pthread_mutex_t* p_mutex, osal_milli_sec_t timeout);

/*******************************************************************************************************************//**
* @brief            Round up the tiem to milli second.
* @param[in-out]    p_timespec  The OSAL sets the roundup time to milliseconds.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_TimeRoundUp(struct timespec* p_timespec);

/*******************************************************************************************************************//**
* @brief            Add the specified time period to the current time.
* @param[in]        p_time_period  The OSAL sets the current time with the specified "p_time_period" added.
* @param[in-out]    p_timespec     Specifies the time to add to the current time.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_TimespecAdd(struct timespec *p_timespec, osal_milli_sec_t *p_time_period);

/*******************************************************************************************************************//**
* @brief            Create Context Key.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_CreateThreadKey(void);

/*******************************************************************************************************************//**
* @brief            Delete Context Key.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_DeleteThreadKey(void);

/*******************************************************************************************************************//**
* @brief            Get the information associated with context key.
* @return           void pointor (Thread-specific data.)
***********************************************************************************************************************/
extern void * R_OSAL_Internal_GetThreadSpecific(void);

/*******************************************************************************************************************//**
* @brief            Set the information in association with context key.
* @param[in]        p_set_value   A pointer to Thread-specific data.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_SetThreadSpecific(const void* p_set_value);

/*******************************************************************************************************************//**
* @brief            Set the attribute.
* @param[in]        p_attr_info    A pointer to thread attribute information.
* @param[in,out]    p_attr         A pointer to osal_inner_thread_attr_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_SetThreadAttr(const st_osal_inner_thread_attrinfo_t *const p_attr_info,
                                                     osal_inner_thread_attr_t *const p_attr);

/*******************************************************************************************************************//**
* @brief            Create a new thread in the calling process as the configuration specified in p_config.
* @param[in]        p_config       A pointer to thread configuration.
* @param[in,out]    p_handle       A pointer to Handle of Thread.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_ThreadCreate(const st_osal_inner_thread_config_t *const p_config,
                                                    osal_inner_thread_handle_t *const p_handle);

/*******************************************************************************************************************//**
* @brief            Join a thread, with a time limit.
* @param[in]        handle         Handle of Thread.
* @param[out]       p_return_value A pointer to NULL, or a pointer to a location where the function can store the value.
* @param[in]        time_period    Specify the timeout time in milliseconds.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_ThreadTimedJoin(osal_inner_thread_handle_t const handle,
                                                       int64_t *const p_return_value,
                                                       const osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            Set the mutex attribute.
* @param[in,out]    p_mutex_attr   A pointer to mutex attribute.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_SetMutexAttr(osal_inner_mutexattr_t *const p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Initialize mutex assigned and set acquired mutex handle to p_handle.
* @param[in]        p_mutex_attr   A pointer to mutex attribute.
* @param[in,out]    p_handle       The address of osal_inner_mutex_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MutexInit(const osal_inner_mutexattr_t *const p_mutex_attr,
                                                 osal_inner_mutex_handle_t *const p_handle);

/*******************************************************************************************************************//**
* @brief            Lock a mutex(timed lock).
* @param[in]        handle        The address of osal_inner_mutex_handle_t.
* @param[in]        time_period   Specify the timeout time in milliseconds.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MutexTimedLock(osal_inner_mutex_handle_t handle,
                                                      osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            Unlock mutex assigned to handle.
* @param[in]        handle         The address of osal_inner_mutex_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_MutexUnlock(osal_inner_mutex_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Destroy mutex assigned to handle.
* @param[in]        handle         The address of osal_inner_mutex_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MutexDestroy(osal_inner_mutex_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Create the message queue in the configuration specified in p_config.
* @param[in]        p_config       A pointer to Message queue configuration.
* @param[in,out]    p_handle       The address of osal_inner_mq_handle_t. 
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MqCreate(const st_osal_inner_mq_config_t *const p_config,
                                                osal_inner_mq_handle_t *const p_handle);

/*******************************************************************************************************************//**
* @brief            Send a message to message queue with timeout until time stamp.
* @param[in]        handle         The address of osal_inner_mq_handle_t.
* @param[in]        time_period    Specify the timeout time in milliseconds.
* @param[in]        p_buffer       The address of void* for the send data.
* @param[in]        buffer_size    The p_buffer size by Byte. 
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MqTimedSend(osal_inner_mq_handle_t handle,
                                                   osal_milli_sec_t time_period,
                                                   const void * p_buffer,
                                                   size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive a message to message queue with timeout until time stamp.
* @param[in]        handle         The address of osal_inner_mq_handle_t.
* @param[in]        time_period    Specify the timeout time in milliseconds.
* @param[out]       p_buffer       The address of void* for the receive data.
* @param[in]        buffer_size    The p_buffer size by Byte. 
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MqTimedReceive(osal_inner_mq_handle_t handle,
                                                      osal_milli_sec_t time_period,
                                                      void * p_buffer,
                                                      size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Delete message queue assigned to handle.
* @param[in]        handle         The address of osal_inner_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_MqDelete(osal_inner_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Acquire the time stamp on system according to HIGH_PRECISION.
* @param[out]       p_time_stamp   Set the pointer address of st_osal_time_t as timestamp.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_GetHighPrecisTimeStamp(st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Acquire the time stamp on system according to HIGH_RESOLUTION.
* @param[out]       p_time_stamp   Set the pointer address of st_osal_time_t as timestamp.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_GetHighResoTimeStamp(st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief           Acquire the difference between two times.
* @param[in]       p_time2           Set the pointer address of st_osal_time_t as timestamp.
* @param[in]       p_time1           Set the pointer address of st_osal_time_t as timestamp.
* @param[in,out]   p_time_difference To set the address of osal_nano_sec_t.
*                                    The OSAL sets the difference between time2 and time1 with a signed value.
* @return          e_osal_return_t
* @retval          OSAL_RETURN_OK
* @retval          OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_Internal_CalcTimeDifference(const st_osal_time_t *const p_time2, 
                                                          const st_osal_time_t *const p_time1,
                                                          osal_nano_sec_t *const p_time_difference);

/** @} */

/** @} OSAL_WRAPPER_EMCOS */

#endif /* OSAL_COMMON_H */

