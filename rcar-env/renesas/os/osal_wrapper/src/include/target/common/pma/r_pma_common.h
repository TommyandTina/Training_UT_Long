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
* Version :      1.5.0
* Description  : Common definition for OSAL
***********************************************************************************************************************/
#ifndef R_PMA_COMMON_H
/** Multiple inclusion protection macro */
#define R_PMA_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include "rcar-xos/osal/r_osal_types.h"
#include "target/common/pma/hw/r_pma_hwa_id.h"

/*======================================================================================================================
Macro definitions
======================================================================================================================*/
#define R_PMA_HWA_ID_NUM (333U)         /*!< Maximum number of HWA ID among various SoC */
#define R_PMA_DEVICE_SOC_TYPE_V3M  0    /*!< SoC type is R-Car V3M  */ 
#define R_PMA_DEVICE_SOC_TYPE_V3H1 1    /*!< SoC type is R-Car V3H1 */ 
#define R_PMA_DEVICE_SOC_TYPE_V3H2 2    /*!< SoC type is R-Car V3H2 */ 
#define R_PMA_DEVICE_SOC_TYPE_V4H  3    /*!< SoC type is R-Car V4H  */ 
#define R_PMA_DEVICE_SOC_TYPE_S4   4    /*!< SoC type is R-Car S4   */ 
#define R_PMA_DEVICE_SOC_TYPE_V4M  5    /*!< SoC type is R-Car V4H  */ 


/*======================================================================================================================
Type definitions
======================================================================================================================*/
typedef enum 
{
    R_PMA_RESULT_OK,                    /*!< Success */
    R_PMA_ERR_RESULT_NG,                /*!< Internal error */
    R_PMA_ERR_ALREADY_IN_USE,           /*!< HWA is already occupied. */
    R_PMA_ERR_INVALID_HANDLE,           /*!< Handle value is Incorrect. */
    R_PMA_ERR_NO_VACANT_HANDLE,         /*!< Number of users exceeds the limit. */
    R_PMA_ERR_INVALID_ARGUMENT,         /*!< Parameter value is Incorrect. */
    R_PMA_ERR_DEVICE,                   /*!< Failure on the device side. (HW failure)   */
    R_PMA_ERR_TIMEOUT,                  /*!< Timeout situation. */
    R_PMA_ERR_IO,                       /*!< Failure related to input/output operation. */
    R_PMA_ERR_DEPENDENCY,               /*!< Unable to control reset due to HWA's dependency. */
    R_PMA_ERR_UNSUPPORTED_OPERATION     /*!< Target HWA is not supported */
} e_pma_return_t;

typedef enum
{
    R_PMA_OK,                     /*!< Success */
    R_PMA_NG,                     /*!< Fail */
    R_PMA_ALREADY_IN_USE,         /*!< HWA is already locked */
    R_PMA_INVALID_ARGUMENT,       /*!< Argument is invalid */
    R_PMA_INVALID_HANDLE,         /*!< Handle is unmatch with locked HWA */
    R_PMA_TIMEOUT,                /*!< Timeout of OS API or polling register */
    R_PMA_BSP_EINVAL,             /*!< Invalid situation of BSP */
    R_PMA_BSP_ETIMEDOUT,          /*!< Timeout situation of BSP */
    R_PMA_BSP_EIO,                /*!< Failure related to input/output operation of BSP */
    R_PMA_BSP_ECPG,               /*!< Unable to get CPG status */
    R_PMA_DEPENDENCY              /*!< Unable to control reset due to HWA's dependency */
} e_result_t;

typedef enum 
{
    R_PMA_STATE_OFF,              /*!< Power or clock domain status is OFF. */
    R_PMA_STATE_ON,               /*!< Power or clock domain status is ON. */
    R_PMA_UNDEFINED_STATE         /*!< Power or clock domain status is not defined */
} e_pma_onoff_t;

typedef enum 
{
    R_PMA_HWA_RELEASED,           /*!< HWA is not ready. */
    R_PMA_HWA_REQUESTED           /*!< HWA is ready. */
} e_pma_hwa_req_t;

typedef enum 
{
    R_PMA_PG_MODE,                /*!< Power Policy is "PG-Mode". */
    R_PMA_CG_MODE,                /*!< Power Policy is "CG-Mode". */
    R_PMA_HP_MODE                 /*!< Power Policy is "HP-Mode". */
} e_pma_power_policy_t;

typedef enum 
{
    R_PMA_RELEASED_RESET,         /*!< Software Reset is released. */
    R_PMA_APPLIED_RESET,          /*!< Software Reset is applied. */
    R_PMA_UNDEFINED_RESET         /*!< Software Reset is not defined. */
} e_pma_reset_req_t;
typedef enum 
{
    R_PMA_STATE_PG,               /*!< Power State is Power Gated. */
    R_PMA_STATE_CG,               /*!< Power State is Clock Gated. */
    R_PMA_STATE_ENABLED,          /*!< Power State is Enabled. */
    R_PMA_STATE_UNKNOWN           /*!< Power State is unknown. */
} e_pma_power_state_t;

typedef enum 
{
    R_PMA_CLK_DISABLE,            /*!< Disable the POST clock. */
    R_PMA_CLK_ENABLE,             /*!< Enable the POST clock. */
} e_pma_clk_req_t;


/** structure */
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct
{
    e_pma_hwa_req_t enReqState;         /*!< User's requirement of HWA. */
    e_pma_onoff_t   enActualPwrStat;    /*!< Actual status of SYSC register. */
    e_pma_onoff_t   enActualClkStat;    /*!< Actual status of CPG register. */
} st_pma_hwa_state_t;
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct
{
    e_osal_thread_priority_t task_priority;             /*!< Priority of OSAL PM API task */
    uint32_t                 timeout_ms;                /*!< Wait time[ms] for return of OS API */
    uint32_t                 polling_max_num;           /*!< Reserved */
    uint32_t                 loop_num_for_rclk_cycle;   /*!< Reserved */
} st_pma_config_t;
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct
{
    bool isUsed;    /** flag to indicate that the handle is used */
} st_pma_user_info_t;

typedef st_pma_user_info_t * st_pma_handle_t;  /*!< Type of OSAL PM API handle */

/*======================================================================================================================
Variable Definitions
======================================================================================================================*/
extern e_pma_hwa_id_t g_pma_max_hwa_id_num;  /*!< maximum HWA ID. It is decided by PMA Manager based on SoC type */

#ifdef __cplusplus
}
#endif
#endif /** R_PMA_COMMON_H */
