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
* Description :  OSAL Resource Configurator Public Header
***********************************************************************************************************************/
#ifndef OSAL_RES_CFG_H
#define OSAL_RES_CFG_H

#include "rcar-xos/osal_configuration/target/linux/r_osal_configuration.h"
#include "target/common/r_osal_common.h"
#include "target/common/res_cfg/r_osal_res_cfg_if.h"
#include "target/common/helper/r_osal_helper_if.h"
#include "target/common/res_cfg/dynamic/r_osal_res_cfg_share.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * @typedef st_osal_thread_control_t
 * @brief   This type is the typedef definition of struct #st_osal_thread_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_thread_control
 * @brief   structure to control thread information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_thread_control
{
    osal_thread_id_t        thread_id;      /*!< Store the created Thread id */
    osal_os_thread_t        thread;         /*!< Handle of Thread for Linux */
    st_osal_thread_config_t osal_config;    /*!< Thread configuration parameter */
    bool                    parent_thread;  /*!< Determine thread has already been joined by another thread or not */
    int64_t                 thread_ret;     /*!< Return value of worker function. refer to join */
} st_osal_thread_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_mutex_control_t
 * @brief   This type is the typedef definition of struct #st_osal_mutex_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_mutex_control
 * @brief   structure to control thread information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_mutex_control
{
    osal_mutex_id_t         mutex_id;   /*!< Store the created Mutex id */
    osal_os_mutex_t         mutex;      /*!< Handle of Mutex for Linux */
} st_osal_mutex_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_cond_control_t
 * @brief   This type is the typedef definition of struct #st_osal_cond_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_cond_control
 * @brief   structure to control condition variable information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_cond_control
{
    osal_cond_id_t      cond_id;    /*!< Store the created Condition Variable id */
    osal_os_cond_t      cond;       /*!< Handle of Condition Variable for Linux */
    bool                active;     /*!< Determine the condition variable is referencing or not */
} st_osal_cond_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_mq_control_t
 * @brief   This type is the typedef definition of struct #st_osal_mq_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_mq_control
 * @brief   structure to control message queue information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_mq_control
{
    osal_mq_id_t        mq_id;              /*!< Store the created Message Queue id */
    osal_os_mq_t        mq;                 /*!< Handle of message queue for Linux */
    st_osal_mq_config_t osal_config;        /*!< Message Queue configuration parameter */
} st_osal_mq_control_t;

/** @} OSAL_WRAPPER */
#endif /* OSAL_RES_CFG_H */
