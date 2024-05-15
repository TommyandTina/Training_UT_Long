/***********************************************************************************************************************
* Copyright [2020-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.6.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL device configuration header file
***********************************************************************************************************************/

#ifndef OSAL_DEV_CFG_INFO_H
#define OSAL_DEV_CFG_INFO_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************************************************//**
 * @defgroup OSAL_WRAPPER_DEV_CFG OSAL Wrapper Device Configurator
 * OSAL Wrapper Device Configurator
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_DEV_CFG
 * @defgroup OSAL_Public_Defines_DevConfig_info Device Configurator Public macro definitions (info)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3M
 * SoC type is R-Car V3M 
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3M        (0x56334d0000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3H1
 * SoC type is R-Car V3H 1.x
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3H1       (0x5633483100000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3H2
 * SoC type is R-Car V3H 2.x
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3H2       (0x5633483200000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3U
 * SoC type is R-Car V3U
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V3U        (0x5633550000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V4H
 * SoC type is R-Car V3U
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_V4H        (0x5634480000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_S4
 * SoC type is R-Car V3U
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_S4         (0x5334000000000000ULL)

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_SOC_TYPE_V3U
 * SoC type is R-Car V3U
***********************************************************************************************************************/
#define OSAL_DEVICE_SOC_TYPE_INVALID    (0xFFFFFFFFFFFFFFFFULL)

/*******************************************************************************************************************//**
 * @def OSAL_DCFG_INNER_TIMEOUT
 * osal internal timeout[msec] value.
***********************************************************************************************************************/
#define OSAL_DCFG_INNER_TIMEOUT (500L)

/*******************************************************************************************************************//**
 * @def OSAL_DCFG_PM_THREAD_PRIORITY
 * osal power management internal thread priority.
***********************************************************************************************************************/
#define OSAL_DCFG_PM_THREAD_PRIORITY ((uint32_t)OSAL_THREAD_PRIORITY_HIGHEST)

/*******************************************************************************************************************//**
 * @def OSAL_DCFG_PM_TIMEOUT
 * osal power management timeout value[msec].
***********************************************************************************************************************/
#define OSAL_DCFG_PM_TIMEOUT (1000U)

/*******************************************************************************************************************//**
 * @def OSAL_DCFG_PM_POLLING_MAX_NUM
 * osal power management maximum nunber of busy-loop.
***********************************************************************************************************************/
#define OSAL_DCFG_PM_POLLING_MAX_NUM (10U)

/*******************************************************************************************************************//**
 * @def OSAL_DCFG_PM_LOOP_NUM_FOR_RCLK_CYCLE
 * osal power management number of a busy loop to wait for the completion of SoftwareReset.
***********************************************************************************************************************/
#define OSAL_DCFG_PM_LOOP_NUM_FOR_RCLK_CYCLE (40U)


/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_DEV_CFG
 * @defgroup OSAL_Public_Type_DevConfig_info Device Configurator Public type definitions (info)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_device_soc_type_t
 * structure to hold SoC type
***********************************************************************************************************************/
typedef struct
{
    uint64_t    device_soc_type;    /*!< Device SoC type */
} st_osal_device_soc_type_t;

/*******************************************************************************************************************//**
 * @struct st_osal_device_pm_config_t
 * structure to hold power management configuration
***********************************************************************************************************************/
typedef struct
{
    uint32_t task_priority;             /*!< Priority of OSAL PM API task */
    uint32_t timeout_ms;                /*!< Wait time[ms] for return of OS API */
    uint32_t polling_max_num;           /*!< The number of times to check register */
    uint32_t loop_num_for_rclk_cycle;/*!< The number of busy loops to wait for 1 cycle by RCLK */
} st_osal_device_pm_config_t;


/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_DEV_CFG
 * @defgroup OSAL_Public_Defines_Extern_Variables_DevConfig Device Configurator Public extern variables (info)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var g_osal_device_soc_type
 * osal device configuration : soc type identifier
***********************************************************************************************************************/
extern const st_osal_device_soc_type_t g_osal_device_soc_type;

/*******************************************************************************************************************//**
 * @var g_osal_device_pm_config
 * osal device configuration : power management configuration
***********************************************************************************************************************/
extern const st_osal_device_pm_config_t g_osal_device_pm_config;

/*******************************************************************************************************************//**
 * @var g_osal_memory_max_configration
 * osal device configuration : max memory configuration structure
***********************************************************************************************************************/
extern const st_osal_mmngr_config_t g_osal_memory_max_configration;

/*******************************************************************************************************************//**
 * @var g_osal_memory_max_configration
 * osal device configuration : max memory configuration structure
***********************************************************************************************************************/
extern const st_osal_mmngr_region_info_list_t g_osal_memory_region_info_list;
/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* OSAL_DEV_CFG_INFO_H */
