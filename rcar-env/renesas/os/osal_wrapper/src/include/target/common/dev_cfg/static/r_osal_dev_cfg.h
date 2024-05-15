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
* File Name :    r_osal_dev_cfg.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configurator Private Header
***********************************************************************************************************************/

#ifndef OSAL_DEV_CFG_H
#define OSAL_DEV_CFG_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "target/common/r_osal_common.h"
#include "target/common/dev_cfg/r_osal_dev_cfg_if.h"
#include "target/common/helper/r_osal_helper_if.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg_share.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_INNER_TIMEOUT
 * @brief osal internal timeout[msec] value.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_INNER_TIMEOUT              (500L)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH
 * @brief Length of the AXI bus name.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH    (31UL)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_BUS_DOMAIN_SIZE
 * @brief Size of the AXI bus domain.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_BUS_DOMAIN_SIZE            (32U)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_DOMAIN_UTLB_MAX_STRINGS
 * @brief Length of Domain and Utlb strings(AXI bus name).
***********************************************************************************************************************/
#define OSAL_DEV_CFG_DOMAIN_UTLB_MAX_STRINGS    (224U)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_AXI_BUS_NAME_LAST
 * @brief Value indicating the last data.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_AXI_BUS_NAME_LAST          (-1)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_INVALID_UTLB
 * @brief uTLB Invalid value
***********************************************************************************************************************/
#define OSAL_DEV_CFG_INVALID_UTLB               (-2)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_POWER_MS_SLEEP_TIME
 * @brief Sleep time[ms].
***********************************************************************************************************************/
#define OSAL_DEV_CFG_POWER_MS_SLEEP_TIME        (1)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_POWER_MS_SLEEP_RETRY
 * @brief Sleep time[ms].
***********************************************************************************************************************/
#define OSAL_DEV_CFG_POWER_MS_SLEEP_RETRY       (1000)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_PM_TIMEOUT
 * @brief osal power management timeout value[msec].
***********************************************************************************************************************/
#define OSAL_DEV_CFG_PM_TIMEOUT                 (1000U)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_PM_POLLING_MAX_NUM
 * @brief osal power management maximum number of busy-loop.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_PM_POLLING_MAX_NUM         (10U)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_PM_LOOP_NUM_FOR_RCLK_CYCLE
 * @brief osal power management number of a busy loop to wait for the completion of SoftwareReset.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_PM_LOOP_NUM_FOR_RCLK_CYCLE (40U)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_DEVICE_HANDLE_ID
 * @brief Handle id for device.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_DEVICE_HANDLE_ID           (0x4F53414C5F444556ULL) /* ascii code: OSAL_DEV */

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_INVALID_DEVICE_HANDLE_ID
 * @brief Invalid Device Handle ID
***********************************************************************************************************************/
#define OSAL_DEV_CFG_INVALID_DEVICE_HANDLE_ID   (0ULL)

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_axi_bus_list_t
 * @brief structure to hold AXI bus information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_axi_bus_list
 * @brief structure to hold AXI bus information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_axi_bus_list
{
    char    axi_bus_name[OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH]; /*!< AXI Bus name */
} st_osal_axi_bus_list_t;

/***********************************************************************************************************************
 * @typedef st_osal_axi_bus_control_t
 * @brief structure to control axi bus list.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_axi_bus_control
 * @brief structure to control axi bus list.
***********************************************************************************************************************/
typedef struct st_osal_axi_bus_control
{
    uint64_t                num;                /*!< number of registrations with "axi_bus_list" */
    st_osal_axi_bus_list_t  *axi_bus_list;      /*!< AXI Bus information */
} st_osal_axi_bus_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_axi_bus_name_info_t
 * @brief structure to control axi bus information.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_axi_bus_name_info
 * @brief structure to control axi bus information.
***********************************************************************************************************************/
typedef struct st_osal_axi_bus_name_info
{
    char    domain_name[OSAL_DEV_CFG_BUS_DOMAIN_SIZE];  /*!< AXI Bus domain name */
    int32_t utlb;                                       /*!< uTLB number  */
    char    *dev_names;                                 /*!< Associated Device Name */
    size_t  dev_names_length;                           /*!< Length of "dev_names" */
} st_osal_axi_bus_name_info_t;

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gp_osal_device_control
 * @brief The value for the user to operate the device without being aware of the device information.
***********************************************************************************************************************/
extern st_osal_device_control_t *gp_osal_device_control;

/*******************************************************************************************************************//**
 * @var g_osal_num_of_device_table
 * @brief Number of the device control information table.
***********************************************************************************************************************/
extern size_t g_osal_num_of_device_table;

/** @} OSAL_WRAPPER */

#endif /* OSAL_DEV_CFG_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
