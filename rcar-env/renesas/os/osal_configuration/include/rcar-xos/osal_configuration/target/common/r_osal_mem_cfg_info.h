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
* File Name :    r_osal_mem_cfg_info.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Memory Configurator Public Header
***********************************************************************************************************************/

#ifndef OSAL_MEM_CFG_INFO_H
#define OSAL_MEM_CFG_INFO_H

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
 * @def OSAL_MEMORY_NAME_SIZE
 * @brief Memory name size
***********************************************************************************************************************/
#define    OSAL_MEMORY_NAME_SIZE        (64)

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_005
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef st_osal_mmngr_region_info_t
 * @brief structure to hold memory region information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mmngr_region_info
 * @brief structure to hold memory region information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mmngr_region_info
{
    char                name[OSAL_MEMORY_NAME_SIZE];    /*!< memory name(e.g. memory driver path) */
    uintptr_t           offset;                         /*!< memory region address offset */
    size_t              range;                          /*!< memory region range */
} st_osal_mmngr_region_info_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_048]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MEM_048]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_006
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef st_osal_mmngr_region_info_list_t
 * @brief structure to hold memory region information list
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mmngr_region_info_list
 * @brief structure to hold memory region information list
***********************************************************************************************************************/
typedef struct st_osal_mmngr_region_info_list
{
    size_t                          num_of_memory_regions;  /*!< Number of memory region information */
    osal_memory_region_idx_t        default_memory_region;  /*!< Default memory region */
    st_osal_mmngr_region_info_t     *p_info;                /*!< memory region information array */
} st_osal_mmngr_region_info_list_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_049]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MEM_049]
***********************************************************************************************************************/

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var g_osal_memory_max_configration
 * @brief osal memory configuration : max memory configuration
***********************************************************************************************************************/
extern const st_osal_mmngr_config_t g_osal_memory_max_configration;

/*******************************************************************************************************************//**
 * @var g_osal_memory_region_info_list
 * @brief osal memory configuration : memory region information list
***********************************************************************************************************************/
extern const st_osal_mmngr_region_info_list_t g_osal_memory_region_info_list;

/** @} OSAL_WRAPPER */

#ifdef __cplusplus
}
#endif

#endif /* OSAL_MEM_CFG_INFO_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
