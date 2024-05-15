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
* File Name :    osal_sample_resource_common.h
* Version :      3.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header file of Sample application to use OSAL Thread/Thread Sync/Message API
***********************************************************************************************************************/
#ifndef OSAL_SAMPLE_RESOURCE_COMMON_H_
#define OSAL_SAMPLE_RESOURCE_COMMON_H_

#include <stdio.h>
#include <string.h>
#include "rcar-xos/osal/r_osal.h"

#ifndef OSAL_SAMPLE_NO_LOGS
#define OSAL_SAMPLE_INFO(...) {printf("%s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#define OSAL_SAMPLE_ERR(...)  {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#else
#define OSAL_SAMPLE_INFO(...) {}
#define OSAL_SAMPLE_ERR(...)  {}
#endif

#define OSAL_RESOURCE_ID        0xf000U            /* check, if static env */

/* thread IDs */
#define MQ_MAIN_TASK_ID         OSAL_RESOURCE_ID + 0U
#define THREAD_MAIN_TASK_ID     OSAL_RESOURCE_ID + 1U
#define THREADSYNC_MAIN_TASK_ID OSAL_RESOURCE_ID + 2U
#define SUB_THREAD_ID_NO1       OSAL_RESOURCE_ID + 3U
#define SUB_THREAD_ID_NO2       OSAL_RESOURCE_ID + 4U
#define SUB_THREAD_ID           OSAL_RESOURCE_ID + 5U

/* thread stack size */
#if !defined(DCONFIG_SMALL_STACKSIZE)
#define THREAD_STACKSIZE       2U * 1024U * 1024U
#else
#define THREAD_STACKSIZE       4U * 1024U
#endif

/* define number thread*/
#define NUM_OF_MQ_MAIN_TASK             1
#define NUM_OF_THREAD_MAIN_TASK         1
#define NUM_OF_THREADSYNC_MAIN_TASK     1
#define MQ_TASK_NUM_OF_THREAD           2
#define THREAD_TASK_NUM_OF_THREAD       3
#define THSYNC_THREAD_NUM               2
#define THSYNC_MTX_NUM                  1
#define THSYNC_COND_NUM                 1

/* message queue IDs */
#define MQ_ID_NO1               OSAL_RESOURCE_ID + 0U

/* mutex IDs */
#define MUTEX_ID_NO1            OSAL_RESOURCE_ID + 0U

/* condition variable IDs */
#define COND_ID_NO1             OSAL_RESOURCE_ID + 0U

/* message queue config */
#define MAX_NUM_OF_MSG                  4
#define MSG_SIZE                        sizeof(uint32_t)

/* send message */
#define MSG_CONTENT 1234U

/* timeout (ms) */
#define TIMEOUT_MS 1000 // 1000 millisecond

/* timeout (ns) */
#define TIMEOUT_NS 100000000 // 100 millisecond
#define WAIT_TIME_NS 500000000 // 500 millisecond

/* timeout (s)  */
#define WAIT_TIME_SEC 1 // 1 sec

/* max ns */
#define MAX_NS 1000000000

/* condition variable wait time maximum try */
#define OSAL_COND_WAIT_MAX_TRY (5)

/**
 * @ingroup osal_sample_resource
 * @defgroup OSAL_SAMPLE_RESOURCE OSAL sample resource common type definitions
 * @{
 */
/**
 * @typedef st_arg_cond_mutex_t
 * @brief This type is the typedef definition of struct #st_arg_cond_mutex.
 */
/**
 * @struct st_arg_cond_mutex
 * @brief This struct contains the user's argument information are passed to OSAL threads in ThreadSyn manager sample code
 */
typedef struct st_arg_cond_mutex {
    osal_cond_handle_t cond_handle;     /*!< OSAL ThreadSyn mutex handle */
    osal_mutex_handle_t mtx_handle;     /*!< OSAL ThreadSyn condition handle */
} st_arg_cond_mutex_t;

/**
 * @typedef st_osal_resource_thread_config_t
 * @brief This type is the typedef definition of struct #st_osal_resource_thread_config.
 */
/**
 * @struct st_osal_resource_thread_config
 * @brief This struct contains the information of OSAL threads
 */
typedef struct st_osal_resource_thread_config {
    osal_thread_id_t thread_id;                 /*!< OSAL threads ID */
    st_osal_thread_config_t thread_config;      /*!< OSAL thread configuration */
} st_osal_resource_thread_config_t;

/**
 * @typedef st_osal_resource_mutex_config_t
 * @brief This type is the typedef definition of struct #st_osal_resource_mutex_config.
 */
/**
 * @struct st_osal_resource_mutex_config
 * @brief This struct contains the information of OSAL mutexes
 */
typedef struct st_osal_resource_mutex_config {
    osal_mutex_handle_t list_mutex_handle[THSYNC_MTX_NUM];      /*!< An array contains OSAL mutex handles */
    bool is_mutex_created;                                      /*!< A boolean flag which is used for determining whether OSAL mutex handles are created successfully or not */
} st_osal_resource_mutex_config_t;

/**
 * @typedef st_osal_resource_cond_config_t
 * @brief This type is the typedef definition of struct #st_osal_resource_cond_config.
 */
/**
 * @struct st_osal_resource_cond_config
 * @brief This struct contains the information of OSAL conditions
 */
typedef struct st_osal_resource_cond_config {
    osal_cond_handle_t list_cond_handle[THSYNC_COND_NUM];       /*!< An array contains OSAL condition handles */
    bool is_cond_created;                                       /*!< A boolean flag which is used for determining whether OSAL conditions handles are created successfully or not */
} st_osal_resource_cond_config_t;
/** @} */
#endif /* End of File */
