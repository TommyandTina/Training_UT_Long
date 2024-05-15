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
* File Name :    r_osal_os_types.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS Specific Type Definition Header
***********************************************************************************************************************/
#ifndef OSAL_OS_TYPES_H
#define OSAL_OS_TYPES_H

#include "rcar-xos/osal_configuration/target/linux/r_osal_configuration.h"
#include "rcar-xos/osal_configuration/target/linux/r_osal_cpu_cfg_info.h"
#include "target/common/r_osal_common.h"
#include "target/common/os_if/r_osal_os_if.h"
#include "target/common/res_cfg/dynamic/r_osal_res_cfg_share.h"
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg_share.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Definition
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @def     OSAL_THREAD_PRIORITY_BASE
 * @brief   Base priority for setting to Linux in thread manager.
            Thread priority should be higher than or equal 1 and lower than the lowest priority of the interrupt thread.
***********************************************************************************************************************/
#define OSAL_THREAD_PRIORITY_BASE  (1)

/***********************************************************************************************************************
 * @def     OSAL_INTERRUPT_PRIORITY_BASE
 * @brief   Base priority for setting to Linux in interrupt manager.
            Interrupt priority should be higher than the highest priority of thread manager and lower than or equal 85.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_PRIORITY_BASE  (16)

/***********************************************************************************************************************
 * @typedef st_osal_os_thread_t
 * @brief   This type is the typedef definition of struct #st_osal_os_thread.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_thread
 * @brief   structure for thread of pthread.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_os_thread
{
    pthread_t thread;        /*!< Thread ID of Linux */
} st_osal_os_thread_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_mutexattr_t
 * @brief   This type is the typedef definition of struct #st_osal_os_mutexattr.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_mutexattr
 * @brief  structure for mutex attribute of pthread.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_os_mutexattr
{
    pthread_mutexattr_t attr;   /*!< Mutex attribute object of Linux */
} st_osal_os_mutexattr_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_mutex_t
 * @brief   This type is the typedef definition of struct #st_osal_os_mutex.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_mutex
 * @brief   structure for mutex of pthread.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_os_mutex
{
    pthread_mutex_t mutex;   /*!< Mutex object of Linux */
} st_osal_os_mutex_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_cond_t
 * @brief   This type is the typedef definition of struct #st_osal_os_cond.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_cond
 * @brief   structure for condition variable of pthread.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_os_cond
{
    pthread_cond_t cond;     /*!< Condition variable object of Linux */
} st_osal_os_cond_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_mqattr_t
 * @brief   This type is the typedef definition of struct #st_osal_os_mqattr.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_os_mqattr
 * @brief  structure for message queue attribute.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_os_mqattr
{
    osal_os_mutexattr_t mutex_attr;   /*!< Mutex attribute object of Linux */
} st_osal_os_mqattr_t;

/***********************************************************************************************************************
 * @typedef st_osal_mq_node_t
 * @brief   This type is the typedef definition of struct #st_osal_mq_node.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_mq_node
 * @brief   structure for message queue.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_mq_node
{
    void*                   buffer; /*!< Buffer to store the message */
    struct st_osal_mq_node* next;   /*!< Pointer to the next message in queue */
} st_osal_mq_node_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_mq_t
 * @brief   This type is the typedef definition of struct #st_osal_os_mq.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_mq
 * @brief   structure to control message queue information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_os_mq
{
    const st_osal_mq_config_t*  p_osal_config;      /*!< Message Queue configuration parameter */
    st_osal_mq_node_t*          head;               /*!< The first message in queue */
    st_osal_mq_node_t*          tail;               /*!< The last message in queue */
    uint64_t                    numofmsg;           /*!< Current number of message in queue */
    bool                        send_wait_flag;     /*!< true: waiting for a send message */
    bool                        receive_wait_flag;  /*!< true: waiting for a receive message */
    osal_os_mutex_t             mq_mtx;             /*!< Mutex of message queue */
    osal_os_cond_t              send_cond;          /*!< Send condition variable of message queue */
    osal_os_cond_t              receive_cond;       /*!< Receive condition variable of message queue */
} st_osal_os_mq_t;

/** @} OSAL_WRAPPER */
#endif /* OSAL_OS_TYPES_H */
