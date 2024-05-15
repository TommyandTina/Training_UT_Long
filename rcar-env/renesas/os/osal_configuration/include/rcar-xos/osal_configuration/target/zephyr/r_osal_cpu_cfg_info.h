/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_cpu_cfg_info.h
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Configurator for specific OS Private Header
***********************************************************************************************************************/

#ifndef OSAL_CPU_CFG_INFO_H
#define OSAL_CPU_CFG_INFO_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/


/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_THREAD_AFFINITY_MAX_BIT
 * @brief Maximum bit of CPU Affinity to set
***********************************************************************************************************************/
#define OSAL_THREAD_AFFINITY_MAX_BIT     (0x8U)

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_AFFINITY_INVALID_ID
 * @brief Invalid thread affinity ID.
***********************************************************************************************************************/
#define OSAL_THREAD_AFFINITY_INVALID_ID (0xFFFFFFFFFFFFFFFFU)

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_012
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef st_osal_thread_affinity_t
 * @brief structure to hold thread CPU affinity configuration
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_thread_affinity
 * @brief structure to hold thread CPU affinity configuration
***********************************************************************************************************************/
typedef struct st_osal_thread_affinity
{
    osal_thread_id_t    id;         /*!< ID that uniquely represents the Thread */
    uint8_t             affinity;   /*!< CPU Affinity */
} st_osal_thread_affinity_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_033]
***********************************************************************************************************************/

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : thread affinity configuration table
***********************************************************************************************************************/
extern const st_osal_thread_affinity_t g_osal_thread_affinity[];

/*******************************************************************************************************************//**
 * @var g_osal_internal_thread_affinity
 * @brief osal resource configuration : thread CPU affinity information for PMA
***********************************************************************************************************************/
extern const uint8_t g_osal_internal_thread_affinity;

#endif /* OSAL_CPU_CFG_INFO_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
