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
* File Name :    r_osal_res_cfg_memory.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configurator Private Header for Memory Manager
***********************************************************************************************************************/

#ifndef OSAL_RES_CFG_MEMORY_H
#define OSAL_RES_CFG_MEMORY_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal_configuration/target/common/r_osal_mem_cfg_info.h"
#include "rcar-xos/memory_allocator/r_meml.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MEMORY_HANDLE_ID
 * @brief Handle id for memory manager object.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MEMORY_HANDLE_ID           (0x4F53414C4D4E4752ULL) /* ascii code: OSALMNGR */

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_BUFFER_HANDLE_ID
 * @brief Handle id for memory buffer object.
***********************************************************************************************************************/
#define OSAL_RES_CFG_BUFFER_HANDLE_ID           (0x4F53414C42554646ULL) /* ascii code: OSALBUFF */

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_INVALID_RES_HANDLE_ID
 * @brief Invalid User Handle ID
***********************************************************************************************************************/
#define OSAL_RES_CFG_INVALID_RES_HANDLE_ID      (0ULL)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_MEM_RESERVE_SIZE
 * @brief Reserve area size of Continuous Memory.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE     (2U * 1024U * 1024U)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MMNGR_MEM_4K
 * @brief Minimum size of Continuous Memory.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_MEM_4K               (4U * 1024U)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE
 * @brief Guard area size.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE      (4096U)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN
 * @brief Pattern value to be set in the guard area.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN   (0xAAU)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MEMORY_INNER_TIMEOUT
 * @brief Value for timeout used in memory manager internal exclusive control.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_INNER_TIMEOUT        (500L)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MMNGR_GB_UNIT
 * @brief Gigabyte unit.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_GB_UNIT              (1024 * 1024 * 1024)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MMNGR_MB_UNIT
 * @brief Megabyte unit.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_MB_UNIT              (1024 * 1024)

/*******************************************************************************************************************//**
 * @def OSAL_RES_CFG_MMNGR_KB_UNIT
 * @brief kilobyte unit.
***********************************************************************************************************************/
#define OSAL_RES_CFG_MMNGR_KB_UNIT              (1024)

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_mem_buf_ptr_self_t
 * structure to buffer information area
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mem_buf_ptr_self
 * structure to buffer information area
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mem_buf_ptr_self
{
    struct st_osal_memory_buffer_obj    mbuff_obj;          /*!< this area is exposed to interface as memory buffer object*/
    uint64_t                            handle_id;          /*!< Fixed value: OSAL_BUFFER_HANDLE_ID */
    size_t                              buffer_id;          /*!< ID of buffer */
    osal_os_mutex_t                     inner_mtx;          /*!< Internal mutex object */
    osal_milli_sec_t                    inner_timeout;      /*!< Waiting time of lock inner_mtx */
    size_t                              size;               /*!< User-specified size when allocating */
    uintptr_t                           buffer_pa;          /*!< Physical address of the buffer used by the user */
    uintptr_t                           chunk_pa;           /*!< Physical address of the allocated chunk */
    size_t                              chunk_size;         /*!< Size of chunk */
    uint8_t                             *p_chunk_va;        /*!< Virtual address of the allocated chunk */
    uint8_t                             *p_buffer_va;       /*!< Virtual address of the buffer used by the user */

} st_osal_mem_buf_ptr_self_t;

/***********************************************************************************************************************
 * @typedef st_osal_mem_monitor_desc_t
 * structure to monitor information area
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mem_monitor_desc
 * structure to monitor information area
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mem_monitor_desc
{
    e_osal_mmngr_event_t                event_id;           /*!< Event ID */
    fp_osal_memory_mngr_cb_t            *p_handler;         /*!< Function pointer of memory manager callback */
    void                                *p_arg;             /*!< Argument of the callback */
} st_osal_mem_monitor_desc_t;

/***********************************************************************************************************************
 * @typedef st_osal_mem_mmngr_ptr_self_t
 * structure to memory manager control information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_mem_mmngr_ptr_self
 * structure to memory manager control information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mem_mmngr_ptr_self
{
    struct st_osal_memory_manager_obj   mmngr_obj;          /*!< this area is exposed to interface as memory manager object */
    uint64_t                            handle_id;          /*!< Fixed value: OSAL_MEMORY_HANDLE_ID */
    osal_os_mutex_t                     inner_mtx;          /*!< Internal mutex object */
    osal_milli_sec_t                    inner_timeout;      /*!< Waiting time of lock inner_mtx */
    st_meml_manage_t                    meml_handle;        /*!< Handle of the Memory Allocator Library */
    st_osal_mmngr_config_t              config;             /*!< Memory Manager configuration */
    int                                 mem_fd;             /*!< File descriptor */ /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    uintptr_t                           physical_addr;      /*!< Physical Address of the continuous memory */
    void                                *p_virtual_addr;    /*!< Virtual Address of the continuous memory */
    size_t                              size;               /*!< Size of the continuous memory */
    st_osal_mem_buf_ptr_self_t          *p_buffers;         /*!< Pointer to the structure of memory buffer */
    size_t                              num_buffers;        /*!< The number of buffers */
    size_t                              use_buffer_total;   /*!< Size of the user allocate total. */
    st_osal_mem_monitor_desc_t          *p_monitor;         /*!< Pointer to the structure of monitor */
    size_t                              num_monitor;        /*!< The number of monitor */
    osal_memory_region_idx_t            region_idx;         /*!< Memory region ID */
} st_osal_mem_mmngr_ptr_self_t;

/** @} OSAL_WRAPPER */

#endif /* OSAL_RES_CFG_MEMORY_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
