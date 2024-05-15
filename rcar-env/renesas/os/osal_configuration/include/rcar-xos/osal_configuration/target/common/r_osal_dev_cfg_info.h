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
* File Name :    r_osal_dev_cfg_info.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configurator Public Header
***********************************************************************************************************************/

#ifndef OSAL_DEV_CFG_INFO_H
#define OSAL_DEV_CFG_INFO_H

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
 * @def OSAL_DEVICE_SOC_TYPE_V3M
 * @brief SoC type is R-Car V3M 
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3M                (0x56334d0000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3H1
 * @brief SoC type is R-Car V3H 1.x
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3H1               (0x5633483100000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3H2
 * @brief SoC type is R-Car V3H 2.x
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3H2               (0x5633483200000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3U
 * @brief SoC type is R-Car V3U
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3U                (0x5633550000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V4H
 * @brief SoC type is R-Car V4H
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V4H                (0x5634480000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V4H2
 * @brief SoC type is R-Car V4H2
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V4H2               (0x5634483200000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V4M
 * @brief SoC type is R-Car V4M 
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V4M                (0x56344d0000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_S4
 * @brief SoC type is R-Car S4
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_S4                 (0x5334000000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_INVALID
 * @brief Invalid SoC type
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_INVALID            (0xFFFFFFFFFFFFFFFFULL)

/*******************************************************************************************************************//**
 * @def OSAL_INTERRUPT_INVALID_ID
 * @brief Invalid interrupt ID.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_INVALID_ID               (NULL)


/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_device_soc_type_t
 * @brief structure to hold SoC type
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_device_soc_type
 * @brief structure to hold SoC type
***********************************************************************************************************************/
typedef struct st_osal_device_soc_type
{
    uint64_t    device_soc_type;        /*!< Device SoC type */
} st_osal_device_soc_type_t;

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_053
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef st_osal_interrupt_thread_config_t
 * @brief structure to hold interrupt management configuration
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_interrupt_thread_config
 * @brief structure to hold interrupt management configuration
***********************************************************************************************************************/
typedef struct st_osal_interrupt_thread_config
{
    uint64_t                    irq_number;             /*!< IRQ number */
    const char                  *id;                    /*!< Device ID */
    e_osal_interrupt_priority_t priority;               /*!< interrupt thread priority */
    const char                  *p_task_name;           /*!< interrupt thread name */
    size_t                      stack_size;             /*!< stack size of the interrupt thread */
    uint64_t                    interrupt_timeout;      /*!< The timeout time of the hardware interrupt waiting[nsec] */
} st_osal_interrupt_thread_config_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_026]
***********************************************************************************************************************/

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var g_osal_device_soc_type
 * @brief osal device configuration : soc type identifier
***********************************************************************************************************************/
extern const st_osal_device_soc_type_t g_osal_device_soc_type;

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_054
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_config
 * @brief osal device configuration : interrupt thread configuration table
***********************************************************************************************************************/
/* PRQA S 3684 1 # No problem since it is a variable size object and has a terminal symbol. */
extern const st_osal_interrupt_thread_config_t g_osal_interrupt_thread_config[];
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_027]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

#ifdef __cplusplus
}
#endif

#endif /* OSAL_DEV_CFG_INFO_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
