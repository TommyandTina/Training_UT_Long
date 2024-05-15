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
* File Name :    r_osal_dev_cfg.c
* Version :      0.6.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL device configuration interface
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <string.h>

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_configuration/target/emcos/r_osal_configuration.h"
#include "target/emcos/dev_cfg/r_osal_dev_cfg.h"

#include "target/emcos/dev_cfg/r_osal_dev_cfg_tbl_v4h.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Private_Defines_Static_Variables_DevConfig Device Condifurator Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gsp_osal_device_info
 * osal device configuration : device information pointer
***********************************************************************************************************************/
static const st_osal_device_info_t *gsp_osal_device_info;

/*******************************************************************************************************************//**
 * @var gs_osal_device_control
 * osal device configuration : device control structure
***********************************************************************************************************************/
static st_osal_device_control_t gs_osal_device_control[OSAL_DCFG_DEVICE_MAX_NUM] =
{
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID},
    {.handle_id = OSAL_INVALID_DEVICE_HANDLE_ID}
};

/*******************************************************************************************************************//**
 * @var gs_osal_axi_bus_control
 * osal device configuration : axi bus control structure
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
static st_osal_axi_bus_control_t gs_osal_axi_bus_control =
{
    0, NULL
};

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_Init()
* XOS3_OSAL_EMCOS_CD_CD_NON_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_NON_005]
***********************************************************************************************************************/
void R_OSAL_DCFG_Init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    uint64_t osal_soc_type;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_soc_type = R_OSAL_DCFG_GetSocType();

    switch (osal_soc_type)
    {
        /* PRQA S 0572,2900 2 # There is no problem because it is implemented based on the design.
         The branch condition by this switch is uniquely determined at compile time, so the tool cannot detect it correctly. */
        case OSAL_DEVICE_SOC_TYPE_V4H:
        {
            gsp_osal_device_info = gs_osal_device_info_v4h;
            break;
        }
        /* PRQA S 0572,2900 2 # There is no problem because it is implemented based on the design.
         The branch condition by this switch is uniquely determined at compile time, so the tool cannot detect it correctly. */
        case OSAL_DEVICE_SOC_TYPE_INVALID:
        default:
        {
            gsp_osal_device_info = NULL;
            break;
        }
    }   /* end of switch(osal_soc_type). */

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_Deinit()
* XOS3_OSAL_EMCOS_CD_CD_NON_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_NON_003]
***********************************************************************************************************************/
void R_OSAL_DCFG_Deinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gsp_osal_device_info = NULL;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_Deinit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetSocType()
* XOS3_OSAL_EMCOS_CD_CD_NON_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_NON_004]
***********************************************************************************************************************/
uint64_t R_OSAL_DCFG_GetSocType(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    return g_osal_device_soc_type.device_soc_type;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetSocType()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetNumOfDevice()
* XOS3_OSAL_EMCOS_CD_CD_NON_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_NON_002]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
size_t R_OSAL_DCFG_GetNumOfDevice(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;
    size_t count;
    bool   last_data = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0;
         (((sizeof(gs_osal_device_control) / sizeof(st_osal_device_control_t)) > count) && (false == last_data));
         count++)
    {
        if(OSAL_DEVICE_INVALID_ID == gsp_osal_device_info[count].id)
        {
            last_data = true;
            num = count;
        }
        else
        {
            /* Do nothing */
        }
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetNumOfDevice()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetDeviceInfo()
* XOS3_OSAL_EMCOS_CD_CD_NON_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_NON_001]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_DCFG_GetDeviceInfo(uint32_t num,
                                          st_osal_device_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool   last_data         = false;
    size_t count;
    size_t num_of_config     = 0;
    size_t num_of_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        num_of_control = sizeof(gs_osal_device_control) / sizeof(st_osal_device_control_t);

        for (count = 0;
             (num_of_control > count) && (false == last_data);
             count++)
        {
            if(OSAL_DEVICE_INVALID_ID == gsp_osal_device_info[count].id)
            {
                last_data = true;
                num_of_config = count;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (num < num_of_config)
        {
            gs_osal_device_control[num].dev_info      = &gsp_osal_device_info[num];
            gs_osal_device_control[num].inner_timeout = (osal_milli_sec_t)OSAL_DEV_CFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_device_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }   /* end of if(NULL == pp_info). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetDeviceInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetNumOfPdevEntry()
***********************************************************************************************************************/
size_t R_OSAL_DCFG_GetNumOfPdevEntry(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    uint64_t    osal_soc_type;
    size_t      num_of_entry;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_soc_type = R_OSAL_DCFG_GetSocType();

    switch (osal_soc_type)
    {
        case OSAL_DEVICE_SOC_TYPE_V4H:
        {
            num_of_entry = sizeof(gs_pdev_group_name_v4h) / sizeof(char *);
            break;
        }
        case OSAL_DEVICE_SOC_TYPE_INVALID:
        default:
        {
            num_of_entry = 0;
            break;
        }
    }

    return num_of_entry;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetNumOfPdevEntry()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetPdevEntryName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetPdevEntryName(uint32_t num, const char **name)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          num_of_entry;
    uint64_t        osal_soc_type;
    const char      **p_pdev_group_name;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_soc_type = R_OSAL_DCFG_GetSocType();

    switch (osal_soc_type)
    {
        case OSAL_DEVICE_SOC_TYPE_V4H:
        {
            p_pdev_group_name = gs_pdev_group_name_v4h;
            num_of_entry = sizeof(gs_pdev_group_name_v4h) / sizeof(char *);
            break;
        }
        case OSAL_DEVICE_SOC_TYPE_INVALID:
        default:
        {
            p_pdev_group_name = NULL;
            break;
        }
    }

    if (NULL == name)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == p_pdev_group_name)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (num < num_of_entry)
        {
            *name = p_pdev_group_name[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }   /* end of if(NULL == name). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetPdevEntryName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetPMAConfiguration()
* XOS3_OSAL_EMCOS_CD_CD_POW_015
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_016]
***********************************************************************************************************************/
void R_OSAL_DCFG_GetPMAConfiguration(st_pma_config_t * const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_config->task_priority           = g_osal_device_pm_config.task_priority;
    p_config->timeout_ms              = g_osal_device_pm_config.timeout_ms;
    p_config->polling_max_num         = g_osal_device_pm_config.polling_max_num;
    p_config->loop_num_for_rclk_cycle = g_osal_device_pm_config.loop_num_for_rclk_cycle;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetPMAConfiguration()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetAxiBusInfo()
* XOS3_OSAL_EMCOS_CD_CD_IO_038
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_064]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetAxiBusInfo(st_osal_axi_bus_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        *pp_info = &gs_osal_axi_bus_control;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetAxiBusInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetMaxConfigOfMemory()
* XOS3_OSAL_EMCOS_CD_CD_MEM_047
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_051]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_DCFG_GetMaxConfigOfMemory(st_osal_mmngr_config_t * const p_config,
                                                 osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_mmngr_region_info_t info;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = R_OSAL_DCFG_GetMemoryRegionInfo(&info, region_idx);
        if (OSAL_RETURN_OK == osal_ret)
        {
            p_config->mode                        = g_osal_memory_max_configration.mode;
            p_config->memory_limit                = info.range;
            p_config->max_allowed_allocations     = g_osal_memory_max_configration.max_allowed_allocations;
            p_config->max_registered_monitors_cbs = g_osal_memory_max_configration.max_registered_monitors_cbs;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetMaxConfigOfMemory()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetDefaultRegionInfo()
***********************************************************************************************************************/
osal_memory_region_idx_t R_OSAL_DCFG_GetDefaultRegionInfo(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    return g_osal_memory_region_info_list.default_memory_region;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetDefaultRegionInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetNumOfMemoryRegionInfo()
***********************************************************************************************************************/
size_t R_OSAL_DCFG_GetNumOfMemoryRegionInfo(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    return g_osal_memory_region_info_list.num_of_memory_regions;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetNumOfMemoryRegionInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_GetMemoryRegionInfo()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetMemoryRegionInfo(st_osal_mmngr_region_info_t * const pp_info,
                                                osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (g_osal_memory_region_info_list.num_of_memory_regions <= region_idx)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        pp_info->offset         = g_osal_memory_region_info_list.p_info[region_idx].offset;
        pp_info->range          = g_osal_memory_region_info_list.p_info[region_idx].range;

        (void)strncpy(pp_info->name, g_osal_memory_region_info_list.p_info[region_idx].name, 64);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_GetMemoryRegionInfo()
***********************************************************************************************************************/
