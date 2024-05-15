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
* File Name :    r_osal_dev_cfg.h
* Version :      0.6.0
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
#include "rcar-xos/osal_configuration/target/emcos/r_osal_dev_cfg_info.h"
#include "target/emcos/dev_cfg/r_osal_dev_cfg_control.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Private_Defines_DevConfig Device Condifurator Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_INNER_TIMEOUT
 * osal internal timeout[msec] value.
***********************************************************************************************************************/
#define OSAL_DEV_CFG_INNER_TIMEOUT     (500L)


/** @} */

#endif /* OSAL_DEV_CFG_H */
