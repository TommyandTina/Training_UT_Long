/***********************************************************************************************************************
* Copyright [2020-2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL device configuration file
***********************************************************************************************************************/
#ifndef OSAL_DEV_CFG_H
#define OSAL_DEV_CFG_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_dev_cfg/freertos/r_osal_dev_cfg_info.h"
#include "rcar-xos/osal_dev_cfg/freertos/r_osal_dev_cfg_control.h"
#include "rcar-xos/pma/r_pma.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_QNX_DEV_CFG
 * @defgroup OSAL_Private_Defines_DevConfig Device Condifurator Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_DCFG_INNER_TIMEOUT
 * osal internal timeout[msec] value.
***********************************************************************************************************************/
#define OSAL_DCFG_INNER_TIMEOUT     (500L)

/*******************************************************************************************************************//**
 * @def OSAL_PMA_PRIORITY
 * osal pma internal thread priority.
***********************************************************************************************************************/
#define OSAL_PMA_PRIORITY ((uint32_t)OSAL_THREAD_PRIORITY_TYPE5 + 1U)

/*******************************************************************************************************************//**
 * @def OSAL_PMA_TIMEOUT
 * osal pma timeout[msec] value.
***********************************************************************************************************************/
#define OSAL_PMA_TIMEOUT (1000U)

/*******************************************************************************************************************//**
 * @def OSAL_PMA_POLLING_MAX_NUM
 * osal pma maximum nunber of busy-loop.
***********************************************************************************************************************/
#define OSAL_PMA_POLLING_MAX_NUM (10U)

/*******************************************************************************************************************//**
 * @def OSAL_PMA_LOOP_NUM_FOR_RCLK_CYCLE
 * osal pma number of a busy loop to wait for the completion of SoftwareReset.
***********************************************************************************************************************/
#define OSAL_PMA_LOOP_NUM_FOR_RCLK_CYCLE (40U)

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_QNX_DEV_CFG
 * @defgroup OSAL_Private_Defines_Static_Variables_DevConfig Device Condifurator Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_control
 * osal device configuration : device control structure
***********************************************************************************************************************/
static st_osal_device_control_t gs_osal_device_control[OSAL_DCFG_DEVICE_MAX_NUM] =
{
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false},
    {.handle_id = 0, .init_state = false}
};

/*******************************************************************************************************************//**
 * @var gs_osal_axi_bus_control
 * osal device configuration : axi bus control structure
***********************************************************************************************************************/
static st_osal_axi_bus_control_t gs_osal_axi_bus_control =
{
    0, NULL
};

/*******************************************************************************************************************//**
 * @var gs_osal_memory_max_configration
 * osal device configuration : max memory configuration structure
***********************************************************************************************************************/
static st_osal_mmngr_config_t gs_osal_memory_max_configration =
{
    .mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT,
    .memory_limit = (128 * OSAL_MMNGR_MB_UNIT),
    .max_allowed_allocations = (4 * 1024),
    .max_registered_monitors_cbs = (4 * 1024)
};

/** @} */

#endif /* OSAL_DEV_CFG_H */
