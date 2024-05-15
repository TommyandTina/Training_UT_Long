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
* File Name :    r_osal_res_cfg_info.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configurator Public Header
***********************************************************************************************************************/

#ifndef OSAL_RES_CFG_INFO_H
#define OSAL_RES_CFG_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_THREAD_INVALID_ID
 * @brief Invalid thread ID.
***********************************************************************************************************************/
#define    OSAL_THREAD_INVALID_ID       (0xFFFFFFFFFFFFFFFFU)

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_INVALID_ID
 * @brief Invalid mutex ID.
***********************************************************************************************************************/
#define    OSAL_MUTEX_INVALID_ID        (0xFFFFFFFFFFFFFFFFU)

/*******************************************************************************************************************//**
 * @def OSAL_COND_INVALID_ID
 * @brief Invalid condition variable ID.
***********************************************************************************************************************/
#define    OSAL_COND_INVALID_ID         (0xFFFFFFFFFFFFFFFFU)

/*******************************************************************************************************************//**
 * @def OSAL_MQ_INVALID_ID
 * @brief Invalid message queue ID.
***********************************************************************************************************************/
#define    OSAL_MQ_INVALID_ID           (0xFFFFFFFFFFFFFFFFU)

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_056
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef st_osal_rcfg_thread_config_t
 * @brief structure to hold thread configuration
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_rcfg_thread_config
 * @brief structure to hold thread configuration
***********************************************************************************************************************/
typedef struct st_osal_rcfg_thread_config
{
    osal_thread_id_t        id;     /*!< ID that uniquely represents the Thread */
    st_osal_thread_config_t config; /*!< Thread configuration parameter */
} st_osal_rcfg_thread_config_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_022]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * @typedef st_osal_rcfg_mutex_config_t
 * @brief structure to hold mutex configuration
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_rcfg_mutex_config
 * @brief structure to hold mutex configuration
***********************************************************************************************************************/
typedef struct st_osal_rcfg_mutex_config
{
    osal_mutex_id_t     id;         /*!< ID that uniquely represents the Mutex */
} st_osal_rcfg_mutex_config_t;

/***********************************************************************************************************************
 * @typedef st_osal_rcfg_cond_config_t
 * @brief structure to hold condition variable configuration
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_rcfg_cond_config
 * @brief structure to hold condition variable configuration
***********************************************************************************************************************/
typedef struct st_osal_rcfg_cond_config
{
    osal_cond_id_t      id;         /*!< ID that uniquely represents the Condition Variable */
} st_osal_rcfg_cond_config_t;

/***********************************************************************************************************************
 * @typedef st_osal_mq_rcfg_config_t
 * @brief structure to hold message configuration
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mq_rcfg_config
 * @brief structure to hold message configuration
***********************************************************************************************************************/
typedef struct st_osal_rcfg_mq_config
{
    osal_mq_id_t        id;         /*!< ID that uniquely represents the Message queue */
    st_osal_mq_config_t config;     /*!< Message queue configuration parameter */
} st_osal_mq_rcfg_config_t;

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_057
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var g_osal_thread_config
 * @brief osal resource configuration : thread configuration table
***********************************************************************************************************************/
/* PRQA S 3684 1 # No problem since it is a variable size object and has a terminal symbol. */
extern const st_osal_rcfg_thread_config_t g_osal_thread_config[];
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_023]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_070
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var g_osal_mutex_config
 * @brief osal resource configuration : mutex configuration table
***********************************************************************************************************************/
/* PRQA S 3684 1 # No problem since it is a variable size object and has a terminal symbol. */
extern const st_osal_rcfg_mutex_config_t g_osal_mutex_config[];
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_044]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_044]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_071
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var g_osal_cond_config
 * @brief osal resource configuration : condition variable configuration table
***********************************************************************************************************************/
/* PRQA S 3684 1 # No problem since it is a variable size object and has a terminal symbol. */
extern const st_osal_rcfg_cond_config_t g_osal_cond_config[];
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_045]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_045]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_065
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var g_osal_mq_config
 * @brief osal resource configuration : message queue configuration table
***********************************************************************************************************************/
/* PRQA S 3684 1 # No problem since it is a variable size object and has a terminal symbol. */
extern const st_osal_mq_rcfg_config_t g_osal_mq_config[];
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_024]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_042]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

#ifdef __cplusplus
}
#endif

#endif /* OSAL_RES_CFG_INFO_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
