/***********************************************************************************************************************
* Copyright [2020-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
 * XOS3_OSAL_NOS_AD_HED_COM_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_memory_impl.h
* Version :      3.7.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL public header file for memory implementation
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_023]
***********************************************************************************************************************/
#ifndef R_OSAL_MEMORY_IMPL_H /* R_OSAL_MEMORY_IMPL_H */
#define R_OSAL_MEMORY_IMPL_H

/* This file contains non-OS specific OSAL API implementation type declarations,
 * it is to be used by Memory Managers that are supposed to compatible with the OSAL API.
 * This enables the usage of different Memory Managers without adapting OSAL Based SW Components to the specific MMNGR.
 * Additional Functionality the MMNGR may provide must be usable by its provided API. OSAL only provides common &
 * mandatory functionality.
 */
#if !defined(_STDIO_H_INCLUDED) && !defined(_STDIO_H) && !defined(_STDIO_H_) && !defined(_INC_STDIO) && !defined(__stdio_h)
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
#include <stdio.h>
#else
/* PRQA S 4601 1 # There is no problem because it is the declaration of macro "FILE" when stdio.h is not used. */
#define FILE void
#endif
#endif

#include "rcar-xos/osal/r_osal_types.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_GENERIC_TYPES
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_025
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct  st_osal_memory_manager_obj
 * @brief   memory manager object.
 **********************************************************************************************************************/
struct st_osal_memory_manager_obj
{
    void * p_ptr_self; /*!< user argument */
    e_osal_return_t (*p_allocate)(void * p_ptr_self, size_t bytes, size_t alignment,
                                osal_memory_buffer_handle_t * const p_buffer_obj);
    /*!< Registered at R_OSAL_MmngrOpen() and called by the R_OSAL_MmngrAlloc(). */
    e_osal_return_t (*p_deallocate)(void * p_ptr_self, osal_memory_buffer_handle_t buffer_obj);
    /*!< Registered at R_OSAL_MmngrOpen() and called by the R_OSAL_MmngrDealloc(). */
    e_osal_return_t (*p_getConfig)(const void * p_ptr_self, st_osal_mmngr_config_t * const p_config);
    /*!< Registered at R_OSAL_MmngrOpen() and called by the R_OSAL_MmngrGetConfig(). */
    e_osal_return_t (*p_registerMonitor)(void * p_ptr_self, e_osal_mmngr_event_t monitored_event,
                                       fp_osal_memory_mngr_cb_t monitor_handler, void * p_monitor_handler_user_arg);
    /*!< Registered at R_OSAL_MmngrOpen() and called by the R_OSAL_MmngrRegisterMonitor(). */
    e_osal_return_t (*p_print)(const void * p_ptr_self, FILE * p_output);
    /*!< Registered at R_OSAL_MmngrOpen() and called by the R_OSAL_MmngrDebugMmngrPrint(). */
};
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_029]
***********************************************************************************************************************/

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_GENERIC_TYPES
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_026
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct  st_osal_memory_buffer_obj
 * @brief   memory buffer object.
 **********************************************************************************************************************/
struct st_osal_memory_buffer_obj
{
    void *                         p_ptr_self;      /*!< user argument */
    osal_memory_manager_handle_t mmngr_owner_hndl;  /*!< owner mmngr of the buffer object */
    e_osal_return_t (*p_getCpuPtr)(void * p_ptr_self, void ** const pp_cpu_ptr);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrGetCpuPtr(). */
    e_osal_return_t (*p_getConstCpuPtr)(void * p_ptr_self, const void ** const pp_const_cpu_ptr);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrGetConstCpuPtr(). */
    e_osal_return_t (*p_flush)(void * p_ptr_self, size_t offset, size_t size);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrFlush(). */
    e_osal_return_t (*p_invalidate)(void * p_ptr_self, size_t offset, size_t size);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrInvalidate(). */
    e_osal_return_t (*p_getHwAddr)(const void * p_ptr_self, osal_axi_bus_id_t axi_id, uintptr_t * const p_hw_addr);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrGetHwAddr(). */
    e_osal_return_t (*p_getSize)(const void * p_ptr_self, size_t * const p_size);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrGetSize(). */
    e_osal_return_t (*p_checkBuffer)(const void * p_ptr_self);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrCheckBuffer(). */
    e_osal_return_t (*p_print)(const void * p_ptr_self, FILE * p_output);
    /*!< Registered at R_OSAL_MmngrAlloc() and called by the R_OSAL_MmngrDebugBufferPrint(). */
};
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_031]
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @typedef osal_memory_manager_t
 * @brief   memory manager handle structure. \n
 *          This type is the typedef definition of struct #st_osal_memory_manager_obj.
 **********************************************************************************************************************/
typedef struct st_osal_memory_manager_obj osal_memory_manager_t; /* forward declaration */

/*******************************************************************************************************************//**
 * @typedef osal_memory_buffer_t
 * @brief   memory buffer handle structure. \n
 *          This type is the typedef definition of struct #st_osal_memory_buffer_obj.
 **********************************************************************************************************************/
typedef struct st_osal_memory_buffer_obj osal_memory_buffer_t; /* forward declaration */

/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************************************************//**
 * @ingroup OSAL_Memory_Manager
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_019
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Initializes all members of #st_osal_memory_buffer_obj to invalid/defined values.
 *          - p_obj->p_ptr_self is NULL
 *          - p_obj->mmngr_owner_hndl is #OSAL_MEMORY_MANAGER_HANDLE_INVALID
 *          - p_obj->p_getCpuPtr is NULL
 *          - p_obj->p_getConstCpuPtr is NULL
 *          - p_obj->p_flush is NULL
 *          - p_obj->p_invalidate is NULL
 *          - p_obj->p_getHwAddr is NULL
 *          - p_obj->p_getSize is NULL
 *          - p_obj->p_checkBuffer is NULL
 *          - p_obj->p_print is NULL
 * @param[in]   p_obj   object to initialize
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK  All arguments are valid, and all members of #st_osal_memory_manager_obj was initialized 
 *                          successfully.
 * @retval  OSAL_RETRUN_PAR Invalid argument.
 *                          - p_obj is NULL.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInitializeMemoryBufferObj(osal_memory_buffer_t * const p_obj);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_015]
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_MEM_020
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Initializes all members of #st_osal_memory_manager_obj to invalid/defined values.
 *          - p_obj->p_ptr_self is NULL
 *          - p_obj->p_allocate is NULL
 *          - p_obj->p_deallocate is NULL
 *          - p_obj->p_getConfig is NULL
 *          - p_obj->p_registerMonitor is NULL
 *          - p_obj->p_print is NULL
 * @param[in]   p_obj   object to initialize
 * @return  #e_osal_return_t
 * @retval  OSAL_RETURN_OK  All arguments are valid, and all members of #st_osal_memory_manager_obj was initialized 
 *                          successfully.
 * @retval  OSAL_RETRUN_PAR Invalid argument.
 *                          - p_obj is NULL.
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInitializeMemoryManagerObj(osal_memory_manager_t * const p_obj);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_MEM_017]
***********************************************************************************************************************/

/**@} MMNGR*/

#ifdef __cplusplus
}
#endif

#if !defined(_STDIO_H_INCLUDED) && !defined(_STDIO_H) && !defined(_STDIO_H_) && !defined(_INC_STDIO) && !defined(__stdio_h)
#if defined(NDEBUG) && !defined(R_OSAL_ENABLE_DEBUG)
/* PRQA S 4601,0841 1 # Since it is the macro "FILE" declared when stdio.h is not used, undefining it is no problem. */
#undef FILE
#endif
#endif

#endif /* R_OSAL_MEMORY_IMPL_H */

