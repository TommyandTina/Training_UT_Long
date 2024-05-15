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
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL CPU Configurator Public Header
***********************************************************************************************************************/

#ifndef OSAL_CPU_CFG_INFO_H
#define OSAL_CPU_CFG_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @def OSAL_THREAD_AFFINITY_INVALID_ID
 * @brief Thread invalid ID for CPU affinity.
***********************************************************************************************************************/
#define OSAL_THREAD_AFFINITY_INVALID_ID     (0xFFFFFFFFFFFFFFFFU)

/***********************************************************************************************************************
 * @def OSAL_INTERRUPT_AFFINITY_INVALID_ID
 * @brief Interrupt Thread invalid ID for CPU affinity.
***********************************************************************************************************************/
#define OSAL_INTERRUPT_AFFINITY_INVALID_ID  (0xFFFFFFFFFFFFFFFFU)

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_thread_affinity_t
 * @brief   This type is the typedef definition of struct #st_osal_thread_affinity.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_thread_affinity
 * @brief   structure to setting CPU affinity of thread manager.
 **********************************************************************************************************************/
typedef struct st_osal_thread_affinity
{
    osal_thread_id_t    id;         /*!< Unique identifier of ID for thread */
    uint64_t            affinity;   /*!< CPU affinity */
} st_osal_thread_affinity_t;

/***********************************************************************************************************************
 * @typedef st_osal_interrupt_thread_affinity_t
 * @brief   This type is the typedef definition of struct #st_osal_interrupt_thread_affinity.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_interrupt_thread_affinity
 * @brief   structure to setting CPU affinity of interrupt manager.
 **********************************************************************************************************************/
typedef struct st_osal_interrupt_thread_affinity
{
    uint64_t        spi_num;        /*!< SPI number of device */
    uint64_t        affinity;       /*!< CPU affinity */
} st_osal_interrupt_thread_affinity_t;

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : CPU affinity value for thread manager.
 **********************************************************************************************************************/
/* PRQA S 3684 1 # No problem since it is a variable size object and has a terminal symbol. */
extern const st_osal_thread_affinity_t g_osal_thread_affinity[];

/***********************************************************************************************************************
 * @var g_osal_interrupt_thread_affinity
 * @brief osal device configuration : CPU affinity value for interrupt manager.
***********************************************************************************************************************/
/* PRQA S 3684 1 # No problem since it is a variable size object and has a terminal symbol. */
extern const st_osal_interrupt_thread_affinity_t g_osal_interrupt_thread_affinity[];

/** @} OSAL_WRAPPER */

#ifdef __cplusplus
}
#endif

#endif /* OSAL_CPU_CFG_INFO_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
