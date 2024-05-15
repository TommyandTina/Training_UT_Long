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
* File Name :    r_osal_os_type.h
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IF Private Header
***********************************************************************************************************************/

#ifndef OSAL_OS_TYPE_H
#define OSAL_OS_TYPE_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <zephyr/posix/pthread.h>
#include <zephyr/posix/semaphore.h>

#include "target/common/r_osal_common.h"
#include "target/common/os_if/r_osal_os_if.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg_share.h"
#include "target/common/res_cfg/static/r_osal_res_cfg_share.h"
#include "rcar-xos/osal_configuration/target/zephyr/r_osal_cpu_cfg_info.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_TH_JOIN_TIMEOUT
 * Value for timeout used in thread join wait.
***********************************************************************************************************************/
#define OSAL_TH_JOIN_TIMEOUT                (2500L)

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_PRIO_BASE
 * @brief Thread base priority.
***********************************************************************************************************************/
#define OSAL_THREAD_PRIO_BASE               (1L)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_PRIO_BASE
 * @brief Interrupt thread base priority.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_PRIO_BASE            (16L)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_SPI_OFFSET
 * @brief Offset to match the hardware interrupt number.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_SPI_OFFSET           (32UL)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_ACTIVE_TIMEOUT
 * @brief Value for timeout used in interrupt thread active check.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_ACTIVE_TIMEOUT       (500L)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_TIMEOUT_OFFSET
 * @brief Value for timeout offset
***********************************************************************************************************************/
#define OSAL_INTERRUPT_TIMEOUT_OFFSET       (60000000U)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MUTEX_DESTROY_TIMEOUT
 * @brief Mutex destruction timeout count.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_MUTEX_DESTROY_COUNT  (500L)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_HANDLE_ID
 * @brief Handle id for interrupt thread.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_HANDLE_ID            (0x4F53414C5F494E54ULL) /* ascii code: OSAL_INT */

/*******************************************************************************************************************//**
 * @def OSAL_INVALID_INTERRUPT_HANDLE_ID
 * @brief Invalid Interrupt Handle ID.
***********************************************************************************************************************/
#define OSAL_INVALID_INTERRUPT_HANDLE_ID    (0ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_INVALID_ID
 * @brief Invalid Device ID.
***********************************************************************************************************************/
#define OSAL_DEVICE_INVALID_ID              (NULL)

/*******************************************************************************************************************//**
 * @def R_PMA_CPG_INVALID_PARAM
 * @brief Invalid CPG Parameter.
***********************************************************************************************************************/
#define R_PMA_CPG_INVALID_PARAM 0xFFFF

/*******************************************************************************************************************//**
 * @def R_PMA_SYSC_INVALID_PARAM
 * @brief Invalid SYSC Parameter.
***********************************************************************************************************************/
#define R_PMA_SYSC_INVALID_PARAM 0xFFFF

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_os_threadattr_t
 * @brief structure to Thread Attribute Object
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_threadattr
 * @brief structure to Thread Attribute Object
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_os_threadattr
{
    pthread_attr_t          attr;       /*!< Thread Attribute Object */
} st_osal_os_threadattr_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_mutexattr_t
 * @brief structure to Mutex Attribute Object
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_mutexattr
 * @brief structure to Mutex Attribute Object
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_os_mutexattr
{
    pthread_mutexattr_t     attr;       /*!< Mutex Attribute Object */
} st_osal_os_mutexattr_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_condattr_t
 * @brief structure to Condition Variable Attribute Object
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_condattr
 * @brief structure to Condition Variable Attribute Object
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_os_condattr
{
    pthread_condattr_t      attr;       /*!< Condition Variable Attribute Object */
} st_osal_os_condattr_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_thread_t
 * @brief structure to Thread Object
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_thread
 * @brief structure to Thread Object
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_os_thread
{
    pthread_t               thread;     /*!< Thread Object */
} st_osal_os_thread_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_mutex_t
 * @brief structure to Mutex Object
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_mutex
 * @brief structure to Mutex Object
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_os_mutex
{
    pthread_mutex_t         mutex;       /*!< Mutex Object */
} st_osal_os_mutex_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_cond_t
 * @brief structure to Condition Variable Object
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_cond
 * @brief structure to Condition Variable Object
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_os_cond
{
    pthread_cond_t          cond;           /*!< Condition Variable Object */
} st_osal_os_cond_t;

/***********************************************************************************************************************
 * @typedef st_osal_queue_t
 * @brief structure to queue.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_queue
 * @brief structure to queue.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_queue
{
    void                    *p_buffer;      /*!< Buffer to store messages */
} st_osal_queue_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_mq_t
 * @brief structure to message queue information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_mq
 * @brief structure to message queue information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_os_mq
{
    uint32_t                snd_wait_count; /*!< Count of during send wait */
    uint32_t                rcv_wait_count; /*!< Count of during recv wait */
    st_osal_queue_t         *queue;         /*!< Pointer of queue */
    int32_t                 head;           /*!< Element number of the next queue to read */
    int32_t                 tail;           /*!< Element number of the next queue to write */
    sem_t                   sync_semaphore; /*!< Exclusive receive task and send task */
    struct k_spinlock       spin_lock;      /*!< spin lock object */
    k_spinlock_key_t        spin_key;       /*!< spin key object */
    uint32_t                numofmsg;       /*!< Number of messages in the queue */
    st_osal_mq_config_t     config;         /*!< Structure to Message queue configuration */
} st_osal_os_mq_t;

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
extern st_osal_device_info_t *gp_osal_device_info;
extern size_t g_osal_num_of_device_config;

/** @} OSAL_WRAPPER */

#endif /* OSAL_OS_TYPE_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
