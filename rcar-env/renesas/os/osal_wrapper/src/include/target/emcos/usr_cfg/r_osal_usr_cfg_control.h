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
* File Name :    r_osal_usr_cfg_control.h
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL resource configuration for eMCOS header file
***********************************************************************************************************************/

#ifndef OSAL_USR_CFG_CONTROL_H
#define OSAL_USR_CFG_CONTROL_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <pthread.h>
#include <mcos/uring.h>

#include "rcar-xos/osal/r_osal.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_USR_CFG
 * @defgroup OSAL_Public_Defines_UsrConfig_control User Configurator Public macro definitions (control)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_THREAD_MS_SLEEP_TIME
 * Sleep time[ms].
***********************************************************************************************************************/
#define OSAL_THREAD_MS_SLEEP_TIME    (1)

/*******************************************************************************************************************//**
 * @def OSAL_MESSAGE_MS_SLEEP_TIME
 * Sleep time[ms].
***********************************************************************************************************************/
#define OSAL_MESSAGE_MS_SLEEP_TIME   (1)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_MS_SLEEP_TIME
 * Sleep time[ms].
***********************************************************************************************************************/
#define OSAL_INTERRUPT_MS_SLEEP_TIME (1)

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_PRIO_BASE
 * Thread base priority.
***********************************************************************************************************************/
#define OSAL_THREAD_PRIO_BASE           (1L)

/*******************************************************************************************************************//**
 * @def OSAL_TH_JOIN_TIMEOUT
 * Value for timeout used in thread join wait.
***********************************************************************************************************************/
#define OSAL_TH_JOIN_TIMEOUT            (2500L)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_PRIO_BASE
 * Interrupt thread base priority.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_PRIO_BASE        (16L)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_SPI_OFFSET
 * Offset to match the hardware interrupt number.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_SPI_OFFSET       (32UL)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_ACTIVE_TIMEOUT
 * Value for timeout used in interrupt thread active check.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_ACTIVE_TIMEOUT   (500L)

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_DESTROY_TIMEOUT
 * Mutex destruction timeout value [msec].
***********************************************************************************************************************/
#define OSAL_MUTEX_DESTROY_TIMEOUT      (500L)

/*******************************************************************************************************************//**
 * @def OSAL_RCFG_THREAD_MAX_NUM
 * Max value of the thread control table.
***********************************************************************************************************************/
#define OSAL_RCFG_THREAD_MAX_NUM        (2048)

/*******************************************************************************************************************//**
 * @def OSAL_RCFG_MUTEX_MAX_NUM
 * Max value of the mutex control table.
***********************************************************************************************************************/
#define OSAL_RCFG_MUTEX_MAX_NUM         (1024)

/*******************************************************************************************************************//**
 * @def OSAL_RCFG_COND_MAX_NUM
 * Max value of the condition variable control table.
***********************************************************************************************************************/
#define OSAL_RCFG_COND_MAX_NUM          (1024)

/*******************************************************************************************************************//**
 * @def OSAL_RCFG_MQ_MAX_NUM
 * Max value of the message queue control table.
***********************************************************************************************************************/
#define OSAL_RCFG_MQ_MAX_NUM            (1024)

/*******************************************************************************************************************//**
 * @def OSAL_RCFG_INTERRUPT_MAX_NUM
 * Max value of the interrupt thread control table.
***********************************************************************************************************************/
#define OSAL_RCFG_INTERRUPT_MAX_NUM     (384)

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_HANDLE_ID
 * Handle id for thread.
***********************************************************************************************************************/
#define OSAL_THREAD_HANDLE_ID       (0x4F53414C5F544852ULL) /* ascii code: OSAL_THR */

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_HANDLE_ID
 * Handle id for mutex.
***********************************************************************************************************************/
#define OSAL_MUTEX_HANDLE_ID        (0x4F53414C5F4D5458ULL) /* ascii code: OSAL_MTX */

/*******************************************************************************************************************//**
 * @def OSAL_COND_HANDLE_ID
 * Handle id for mutex.
***********************************************************************************************************************/
#define OSAL_COND_HANDLE_ID         (0x4F53414C5F434E44ULL) /* ascii code: OSAL_CND */

/*******************************************************************************************************************//**
 * @def OSAL_MESSAGE_HANDLE_ID
 * Handle id for message queue.
***********************************************************************************************************************/
#define OSAL_MESSAGE_HANDLE_ID      (0x4F53414C5F4D5347ULL) /* ascii code: OSAL_MSG */

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_HANDLE_ID
 * Handle id for interrupt thread.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_HANDLE_ID    (0x4F53414C5F494E54ULL) /* ascii code: OSAL_INT */

/*******************************************************************************************************************//**
 * @def OSAL_INVALID_USER_HANDLE_ID
 * Invalid User Handle ID
***********************************************************************************************************************/
#define OSAL_INVALID_USER_HANDLE_ID (0U)

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_USR_CFG
 * @defgroup OSAL_Public_Type_UsrConfig_control User Configurator Public type definitions (control)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_thread_control_t
 * structure to control thread.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_thread_control
{
    uint64_t                handle_id;      /*!< Fixed value: OSAL_THREAD_HANDLE_ID */
    bool                    active;         /*!< thread active flag (true: hread is active, false: thread is inactive) */

    pthread_mutex_t         inner_mtx;      /*!< Internal mutex object */
    osal_milli_sec_t        inner_timeout;  /*!< Waiting time of lock inner_mtx */

    pthread_cond_t          create_cond;    /*!< Handle of Condition Variable for create */
    pthread_cond_t          join_cond;      /*!< Handle of Condition Variable for join */

    pthread_t               thread;         /*!< Handle of thread for eMCOS */
    bool                    parent_thread;  /*!< Determine thread has already been joined by another thread or not */

    p_osal_thread_func_t    func;           /*!< User function */
    void*                   userarg;        /*!< Pointer of Argument for user function */
    int64_t                 thread_ret;     /*!< Return value of worker function. refer to join */

    const struct st_osal_rcfg_thread_config *usr_config;      /*!< User configuration for thread */

}st_osal_thread_control_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mutex_control_t
 * structure to control mutex.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mutex_control
{
    uint64_t            handle_id;          /*!< Fixed value: OSAL_MUTEX_HANDLE_ID */
    bool                active;             /*!< Mutex active flag (true: already called create, false: no called create) */

    osal_milli_sec_t    inner_timeout;      /*!< Waiting time of lock mutex object */

    pthread_mutex_t     mutex;              /*!< Handle of Mutex of eMCOS */

    const struct st_osal_rcfg_mutex_config *usr_config; /*!< User configuration for mutex */

}st_osal_mutex_control_t;

/*******************************************************************************************************************//**
 * @struct st_osal_cond_control_t
 * structure to control cond.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_cond_control
{
    uint64_t            handle_id;          /*!< Fixed value: OSAL_COND_HANDLE_ID */
    bool                active;             /*!< Cond active flag (true: already called create, false: no called create) */
    uint32_t            use_count;          /*!< Cond use count */

    pthread_mutex_t     inner_mtx;          /*!< Internal mutex object */
    osal_milli_sec_t    inner_timeout;      /*!< Waiting time of lock inner_mtx */

    pthread_cond_t      cond;               /*!< Handle of Condition Variable for eMCOS */

    const struct st_osal_rcfg_cond_config *usr_config; /*!< User configuration for cndition variable */

}st_osal_cond_control_t;

/*******************************************************************************************************************//**
 * @struct st_osal_queue_t
 * structure to queue.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_queue
{
  void                  *p_buffer;          /*!< Buffer to store messages */

}st_osal_queue_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mq_control_t
 * structure to control message queue.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mq_control
{
    uint64_t            handle_id;          /*!< Fixed value: OSAL_MESSAGE_HANDLE_ID */
    bool                active;             /*!< message queue active flag (true: already called create, false: no called create) */

    bool                send_wait;          /*!< Send wait flag */
    bool                receive_wait;       /*!< Receive wait flag */

    st_osal_queue_t     *queue;             /*!< Pointer of queue */
    int32_t             head;               /*!< Element number of the next queue to read */
    int32_t             tail;               /*!< Element number of the next queue to write */

    pthread_mutex_t     inner_mtx;          /*!< Internal mutex object */
    osal_milli_sec_t    inner_timeout;      /*!< Waiting time of lock inner_mtx */

    pthread_cond_t      send_cond;          /*!< Handle of Condition Variable for waiting to send */
    pthread_cond_t      receive_cond;       /*!< Handle of Condition Variable for waiting to receive */

    uint32_t            numofmsg;           /*!< Number of messages in the queue */

    const struct st_osal_rcfg_mq_config *usr_config; /*!< User configuration for message queue */

}st_osal_mq_control_t;

/*******************************************************************************************************************//**
 * @struct st_osal_interrupt_thread_control_t
 * structure to control interrupt thread.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_interrupt_thread_control
{
    uint64_t              handle_id;        /*!< Fixed value: OSAL_INTERRUPT_HANDLE_ID */
    bool                  thread_active;    /*!< interrupt thread active flag(true: active interrupt hread is active, false: interrupt thread is inactive) */

    pthread_mutex_t       inner_mtx;        /*!< Internal mutex object */
    osal_milli_sec_t      inner_timeout;    /*!< Waiting time of lock inner_mtx */

    osal_device_handle_t  device_handle;    /*!< Handle of device according to interrupt */
    pthread_t             thread_id;        /*!< Handle of interrupt thread for eMCOS */

    p_osal_isr_func_t     irq_handler;      /*!< Pointer of Worker function of Interrupt thread */
    void                  *irq_arg;         /*!< Pointer of Worker function argument */

    uint64_t              irq_channel;      /*!< irq number channel */
    bool                  irq_enable;       /*!< interrupt status (true: Enable interrupt, false: Disable interrupt) */

    pthread_cond_t        cond;             /*!< Condition variable for reception notification */
    bool                  notify;           /*!< Notification flag (true: Interrupt reception notification, false: No interrupt reception) */
    uint32_t              pdev_int_channel; /*!< Interrupt channel number registered in the pdev entry */

    const struct st_osal_interrupt_thread_config *usr_config; /*!< User configuration for message queue */

}st_osal_interrupt_thread_control_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_USR_CFG
 * @defgroup OSAL_Public_Function_UsrConfig User Condifurator Public function definitions (control)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Init Resource Configuration.
* @return           void
***********************************************************************************************************************/
void R_OSAL_RCFG_Init(void);

/*******************************************************************************************************************//**
* @brief            Deinit Resource Configuration.
* @return           void
***********************************************************************************************************************/
void R_OSAL_RCFG_Deinit(void);

/*******************************************************************************************************************//**
* @brief            Get number of Thread Control information.
* @return           size_t  number of thread Control information
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfThread(void);

/*******************************************************************************************************************//**
* @brief            Get Thread Control information.
* @param[in]        num      To set the thread number
* @param[out]       pp_info  To set the address of the thread control structure pointer
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetThreadInfo(uint32_t num, st_osal_thread_control_t **pp_info);

/*******************************************************************************************************************//**
* @brief            Get number of Interrupt Thread Control information.
* @return           size_t  number of Interrupt Thread Control information
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfInterruptThread(void);

/*******************************************************************************************************************//**
* @brief            Get Interrupt Thread Control information.
* @param[in]        num      To set the interrupt thread number
* @param[out]       pp_info  To set the address of the interrupt thread control structure pointer
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetInterruptThreadInfo(uint32_t num, st_osal_interrupt_thread_control_t **pp_info);

/*******************************************************************************************************************//**
* @brief            Get number of Mutex Control information.
* @return           size_t number of Mutex Control information
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfMutex(void);

/*******************************************************************************************************************//**
* @brief            Get Mutex Control information.
* @param[in]        num      To set the mutex number
* @param[out]       pp_info  To set the address of the mutex control structure pointer
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetMutexInfo(uint32_t num, st_osal_mutex_control_t **pp_info);

/*******************************************************************************************************************//**
* @brief            Get number of Condition variable Control information.
* @return           size_t  number of Condition variable Control information
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfCond(void);

/*******************************************************************************************************************//**
* @brief            Get Condition variable Control information.
* @param[in]        num      To set the condition variable number
* @param[out]       pp_info  To set the address of the condition variable control structure pointer
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetCondInfo(uint32_t num, st_osal_cond_control_t **pp_info);

/*******************************************************************************************************************//**
* @brief            Get number of Message queue Control information.
* @return           size_t  number of Message queue Control information
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfMq(void);

/*******************************************************************************************************************//**
* @brief            Get Message queue Control information.
* @param[in]        num      To set the message queue number
* @param[out]       pp_info  To set the address of the message queue control structure pointer
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetMqInfo(uint32_t num, st_osal_mq_control_t **pp_info);

/** @} */

#endif /* OSAL_USR_CFG_CONTROL_H */
