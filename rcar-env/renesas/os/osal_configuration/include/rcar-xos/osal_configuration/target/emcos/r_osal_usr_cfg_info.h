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
* File Name :    r_osal_usr_cfg_info.h
* Version :      0.6.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL resource configuration header file
***********************************************************************************************************************/

#ifndef OSAL_USR_CFG_INFO_H
#define OSAL_USR_CFG_INFO_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************************************************//**
 * @defgroup OSAL_WRAPPER_USR_CFG OSAL Wrapper User Configurator
 * OSAL Wrapper User Configurator
 * @{
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_USR_CFG
 * @defgroup OSAL_Public_Defines_UsrConfig_info User Configurator Public macro definitions (info)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_THREAD_INVALID_ID
 * Invalid thread ID.
***********************************************************************************************************************/
#define    OSAL_THREAD_INVALID_ID       (0xFFFF)

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_INVALID_ID
 * Invalid mutex ID.
***********************************************************************************************************************/
#define    OSAL_MUTEX_INVALID_ID        (0xFFFF)

/*******************************************************************************************************************//**
 * @def OSAL_COND_INVALID_ID
 * Invalid condition variable ID.
***********************************************************************************************************************/
#define    OSAL_COND_INVALID_ID         (0xFFFF)

/*******************************************************************************************************************//**
 * @def OSAL_MQ_INVALID_ID
 * Invalid message queue ID.
***********************************************************************************************************************/
#define    OSAL_MQ_INVALID_ID           (0xFFFF)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_INVALID_ID
 * Invalid interrupt ID.
***********************************************************************************************************************/
#define    OSAL_INTERRUPT_INVALID_ID    (NULL)

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_USR_CFG
 * @defgroup OSAL_Public_Type_UsrConfig_info User Configurator Public type definitions (info)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_rcfg_thread_config_t
 * structure to hold thread configuration
 * XOS2_OSAL_QNX_CD_CD_TH_013
 * [Covers: XOS2_OSAL_QNX_UD_DD_TH_025]
 * XOS3_OSAL_EMCOS_CD_CD_TH_015
 * [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_023]
***********************************************************************************************************************/
typedef struct st_osal_rcfg_thread_config
{
    osal_thread_id_t        id;         /*!< thread ID */
    st_osal_thread_config_t config;     /*!< thread configuration */
#if defined(CONFIG_ADDR_STACK)
    void*                   addr_stack; /*!< Pointer to the lowest address of stack buffer */
#endif
} st_osal_rcfg_thread_config_t;

/*******************************************************************************************************************//**
 * @struct st_osal_interrupt_thread_config_t
 * structure to hold interrupt thread configuration
 * XOS2_OSAL_QNX_CD_CD_INT_016
 * [Covers: XOS2_OSAL_QNX_UD_DD_INT_024]
 * XOS3_OSAL_EMCOS_CD_CD_INT_018
 * [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_024]
***********************************************************************************************************************/
typedef struct st_osal_interrupt_thread_config
{
    uint64_t                    irq_number;     /*!< IRQ number */
    const char                  *id;            /*!< Device ID */
    e_osal_interrupt_priority_t priority;       /*!< interrupt thread priority */
    const char                  *task_name;     /*!< interrupt thread name */
    size_t                      stack_size;     /*!< stack size of the interrupt thread */
    uint64_t                    interrupt_timeout;  /*!< The timeout time of the hardware interrupt waiting[nsec] */
} st_osal_interrupt_thread_config_t;

/*******************************************************************************************************************//**
 * @struct st_osal_rcfg_mutex_config_t
 * structure to hold mutex configuration
***********************************************************************************************************************/
typedef struct st_osal_rcfg_mutex_config
{
    osal_mutex_id_t     id;     /*!< mutex ID */
} st_osal_rcfg_mutex_config_t;

/*******************************************************************************************************************//**
 * @struct st_osal_rcfg_cond_config_t
 * structure to hold condition variable configuration
***********************************************************************************************************************/
typedef struct st_osal_rcfg_cond_config
{
    osal_cond_id_t      id;     /*!< cond ID */
} st_osal_rcfg_cond_config_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mq_rcfg_config_t
 * structure to hold message queue configuration
***********************************************************************************************************************/
typedef struct st_osal_rcfg_mq_config
{
    osal_mq_id_t        id;     /*!< message queue ID */
    st_osal_mq_config_t config; /*!< message queue configuration */
} st_osal_mq_rcfg_config_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mq_rcfg_config_t
 * structure to hold message queue configuration
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mmngr_region_info
{
    char                            name[64];       /*!< memory name(e.g. memory driver path) */
    uintptr_t                       offset;         /*!< memory region address offest */
    size_t                          range;          /*!< memory region range */
} st_osal_mmngr_region_info_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mq_rcfg_config_t
 * structure to hold message queue configuration
***********************************************************************************************************************/
typedef struct st_osal_mmngr_region_info_list
{
    size_t                          num_of_memory_regions;  /*!< Number of memory region informations */
    osal_memory_region_idx_t        default_memory_region;  /*!< Default memory region */
    st_osal_mmngr_region_info_t     *p_info;                /*!< memory region information array */
} st_osal_mmngr_region_info_list_t;
/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_USR_CFG
 * @defgroup OSAL_Public_Defines_Extern_Variables_UsrConfig User Configurator Public extern variables (info)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_thread_config
 * osal user configuration : thread configuration table
***********************************************************************************************************************/
/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const st_osal_rcfg_thread_config_t g_osal_thread_config[];

/*******************************************************************************************************************//**
 * @var gs_osal_mutex_config
 * osal user configuration : mutex configuration table
***********************************************************************************************************************/
/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const st_osal_rcfg_mutex_config_t g_osal_mutex_config[];

/*******************************************************************************************************************//**
 * @var gs_osal_cond_config
 * osal user configuration : cond configuration table
***********************************************************************************************************************/
/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const st_osal_rcfg_cond_config_t g_osal_cond_config[];

/*******************************************************************************************************************//**
 * @var gs_osal_mq_config
 * osal user configuration : message queue configuration table
***********************************************************************************************************************/
/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const st_osal_mq_rcfg_config_t g_osal_mq_config[];

/*******************************************************************************************************************//**
 * @var gs_osal_interrupt_thread_config
 * osal user configuration : interrupt thread configuration table
***********************************************************************************************************************/
/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const st_osal_interrupt_thread_config_t g_osal_interrupt_thread_config[];

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* OSAL_USR_CFG_INFO_H */
