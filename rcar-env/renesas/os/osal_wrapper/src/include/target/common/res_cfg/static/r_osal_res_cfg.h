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
* File Name :    r_osal_res_cfg.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configurator Private Header
***********************************************************************************************************************/

#ifndef OSAL_RES_CFG_H
#define OSAL_RES_CFG_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "target/common/r_osal_common.h"
#include "target/common/res_cfg/r_osal_res_cfg_if.h"
#include "target/common/helper/r_osal_helper_if.h"
#include "target/common/res_cfg/static/r_osal_res_cfg_share.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_INNER_TIMEOUT
 * @brief osal internal timeout[msec] value.
***********************************************************************************************************************/
#define OSAL_RES_CFG_INNER_TIMEOUT              (500L)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_THREAD_MS_SLEEP_TIME
 * @brief Sleep time[ms] for Thread Manager.
***********************************************************************************************************************/
#define OSAL_RES_CFG_THREAD_MS_SLEEP_TIME       (1)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MESSAGE_MS_SLEEP_TIME
 * @brief Sleep time[ms] for Message queue Manager.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MESSAGE_MS_SLEEP_TIME      (1)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MUTEX_DESTROY_TIMEOUT
 * @brief Mutex destruction timeout value [msec].
***********************************************************************************************************************/
#define OSAL_RES_CFG_MUTEX_DESTROY_TIMEOUT      (500L)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_THREAD_HANDLE_ID
 * @brief Handle id for thread.
***********************************************************************************************************************/
#define OSAL_RES_CFG_THREAD_HANDLE_ID           (0x4F53414C5F544852ULL) /* ascii code: OSAL_THR */

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MUTEX_HANDLE_ID
 * @brief Handle id for mutex.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MUTEX_HANDLE_ID            (0x4F53414C5F4D5458ULL) /* ascii code: OSAL_MTX */

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_COND_HANDLE_ID
 * @brief Handle id for condition variable.
***********************************************************************************************************************/
#define OSAL_RES_CFG_COND_HANDLE_ID             (0x4F53414C5F434E44ULL) /* ascii code: OSAL_CND */

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MESSAGE_HANDLE_ID
 * @brief Handle id for message queue.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MESSAGE_HANDLE_ID          (0x4F53414C5F4D5347ULL) /* ascii code: OSAL_MSG */

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_INVALID_RES_HANDLE_ID
 * @brief Invalid User Handle ID
***********************************************************************************************************************/
#define OSAL_RES_CFG_INVALID_RES_HANDLE_ID      (0ULL)

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_thread_control_t
 * @brief structure to hold thread control information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_thread_control
 * @brief structure to hold thread control information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_thread_control
{
    uint64_t                                handle_id;      /*!< Fixed value: OSAL_THREAD_HANDLE_ID */
    bool                                    active;         /*!< thread active flag (true: thread is active, false: thread is inactive) */
    osal_os_mutex_t                         inner_mtx;      /*!< Internal mutex object */
    osal_milli_sec_t                        inner_timeout;  /*!< Waiting time of lock inner_mtx */
    osal_os_cond_t                          create_cond;    /*!< Handle of Condition Variable for create */
    osal_os_cond_t                          join_cond;      /*!< Handle of Condition Variable for join */
    osal_os_thread_t                        thread;         /*!< Handle of Thread */
    bool                                    parent_thread;  /*!< Determine thread has already been joined by another thread or not */
    p_osal_thread_func_t                    func;           /*!< User function */
    void                                    *userarg;       /*!< Pointer of Argument for user function */
    int64_t                                 thread_ret;     /*!< Return value of worker function. refer to join */
    const struct st_osal_rcfg_thread_config *usr_config;    /*!< Resource Configuration for thread */
} st_osal_thread_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_mutex_control_t
 * @brief structure to hold mutex control information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mutex_control
 * @brief structure to hold mutex control information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mutex_control
{
    uint64_t                                handle_id;      /*!< Fixed value: OSAL_MUTEX_HANDLE_ID */
    bool                                    active;         /*!< Mutex active flag (true: already called create, false: no called create) */
    osal_milli_sec_t                        inner_timeout;  /*!< Waiting time of lock mutex object */
    osal_os_mutex_t                         mutex;          /*!< Handle of Mutex */
    const struct st_osal_rcfg_mutex_config  *usr_config;    /*!< Resource Configuration for mutex */
} st_osal_mutex_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_cond_control_t
 * @brief structure to hold condition variable control information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_cond_control
 * @brief structure to hold condition variable control information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_cond_control
{
    uint64_t                                handle_id;     /*!< Fixed value: OSAL_COND_HANDLE_ID */
    bool                                    active;        /*!< Cond active flag (true: already called create, false: no called create) */
    uint32_t                                use_count;     /*!< Cond use count */
    osal_os_mutex_t                         inner_mtx;     /*!< Internal mutex object */
    osal_milli_sec_t                        inner_timeout; /*!< Waiting time of lock inner_mtx */
    osal_os_cond_t                          cond;          /*!< Handle of Condition Variable for QNX */
    const struct st_osal_rcfg_cond_config   *usr_config;   /*!< Resource Configuration for condition variable */
} st_osal_cond_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_mq_control_t
 * @brief structure to hold message control information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mq_control
 * @brief structure to hold message control information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mq_control
{
    uint64_t                            handle_id;          /*!< Fixed value: OSAL_MESSAGE_HANDLE_ID */
    bool                                active;             /*!< message queue active flag (true: already called create, false: no called create) */
    osal_os_mq_t                        mq;
    osal_os_mutex_t                     inner_mtx;          /*!< Internal mutex object */
    osal_milli_sec_t                    inner_timeout;      /*!< Waiting time of lock inner_mtx */
    const struct st_osal_rcfg_mq_config *usr_config;        /*!< Resource Configuration for message queue */
} st_osal_mq_control_t;

/** @} OSAL_WRAPPER */

#endif /* OSAL_RES_CFG_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
