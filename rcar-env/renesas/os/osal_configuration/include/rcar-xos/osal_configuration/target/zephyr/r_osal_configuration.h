/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_configuration.h
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Configurator for specific OS Private Header
***********************************************************************************************************************/

#ifndef OSAL_CONFIGURATION_H
#define OSAL_CONFIGURATION_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_dev_cfg_info.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_res_cfg_info.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_mem_cfg_info.h"
#include "rcar-xos/osal_configuration/target/zephyr/r_osal_cpu_cfg_info.h"

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_THREAD_STACK_INVALID_ID
 * @brief Invalid thread stack ID.
***********************************************************************************************************************/
#define OSAL_THREAD_STACK_INVALID_ID (0xFFFFFFFFFFFFFFFFU)


/***********************************************************************************************************************
 * @typedef st_osal_thread_stack_t
 * @brief structure to hold thread stack configuration
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_thread_stack
 * @brief structure to hold thread stack configuration
***********************************************************************************************************************/
typedef struct st_osal_thread_stack
{
    osal_thread_id_t    id;
    void                * addr_stack;
}st_osal_thread_stack_t;

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_011
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var g_osal_thread_stack_config
 * @brief osal resource configuration : thread stack configuration table
***********************************************************************************************************************/
/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const st_osal_thread_stack_t g_osal_thread_stack_config[];
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_030]
***********************************************************************************************************************/

#endif /* OSAL_CONFIGURATION_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
