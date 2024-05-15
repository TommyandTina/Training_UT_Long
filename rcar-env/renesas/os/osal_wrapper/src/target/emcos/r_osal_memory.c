/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_memory.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper Memory Manager for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <mcos/pdev.h>
#include <bios/bios_cache.h>

#include "target/emcos/r_osal_common.h"

#if defined(NDEBUG) && !defined(R_OSAL_ENABLE_DEBUG)
/* PRQA S 4601 1 # There is no problem because it is the declaration of macro "FILE" when stdio.h is not used. */
#define FILE void
#endif

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_Memory Memory Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gsp_osal_memory_manager
 * Handle of memory manager object
***********************************************************************************************************************/
OSAL_STATIC osal_memory_manager_handle_t *gsp_osal_memory_manager = NULL;

/*******************************************************************************************************************//**
 * @var gspp_osal_mem_mmngr_ptr_self
 * Pointor Self of memory manager object
***********************************************************************************************************************/
OSAL_STATIC st_osal_mem_mmngr_ptr_self_t **gspp_osal_mem_mmngr_ptr_self = NULL;

/*******************************************************************************************************************//**
 * @var gs_osal_memory_is_initialized
 * Initialize status
***********************************************************************************************************************/
OSAL_STATIC uint32_t gs_osal_memory_is_initialized = OSAL_MMNGR_STATE_NONE;

/** @} */

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Functions_Memory Memory Manager Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Fill the guard area with the pattern value.
* @param[in]        p_ptr_self   A pointer to Memory Manager handle.
* @param[in]        bytes        Requested size of the memory.
* @param[in]        alignment    Alignment of the memory.
* @param[out]       p_buffer_obj Handle to the allocate buffer object.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_PROHIBITED
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc (void * p_ptr_self,
                                                     size_t bytes,
                                                     size_t alignment,
                                                     osal_memory_buffer_handle_t * const p_buffer_obj);

/*******************************************************************************************************************//**
* @brief            Fill the guard area with the pattern value.
* @param[in]        mode              Memory Manager allocation mode values.
* @param[in/out]    p_buffer_info     A pointer to the buffer information area.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_fill_guard_area (e_osal_mmngr_allocator_mode_t mode,
                                                                     const st_osal_mem_buf_ptr_self_t * p_buffer_info);

/*******************************************************************************************************************//**
* @brief            Check the current buffer object for underflow or overflow.
* @param[in/out]    p_mngr_control  A pointer to the memory manager control table.
* @param[in/out]    p_buffer_info   A pointer to the buffer information area.
* @param[in]        bytes           Requested size of the memory.
* @param[in]        alignment       Alignment of the memory which has been adjusted in the system page.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_lib (st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                         st_osal_mem_buf_ptr_self_t * p_buffer_info,
                                                         size_t bytes,
                                                         size_t alignment);

/*******************************************************************************************************************//**
* @brief            Check the current buffer object for underflow or overflow.
* @param[in]        p_ptr_self      A pointer to Memory Buffer handle.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_checkbuffer (const void* p_ptr_self);

/*******************************************************************************************************************//**
* @brief            Close memory manager.
* @param[in]        hndl_mmngr       Memory Manager handle.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_close (osal_memory_manager_handle_t hndl_mmngr);

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
/*******************************************************************************************************************//**
* @brief            Performs unit conversion and returns as a string.
* @param[in]        size       Conversion target size.
* @param[out]       p_output   Converted string.
* @param[in]        array_size Array size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_conv_unit_and_str (size_t size, char * p_output, size_t array_size);
#endif

/*******************************************************************************************************************//**
* @brief            Deallocates memory.
* @param[in]        p_ptr_self A pointer to Memory Manager handle.
* @param[in]        buffer_obj A pointer to Memory Buffer Object.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc (void * p_ptr_self, osal_memory_buffer_handle_t buffer_obj);

/*******************************************************************************************************************//**
* @brief            Deallocates memory process.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        buffer_obj       A pointer to Memory Buffer Object.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                           osal_memory_buffer_handle_t buffer_obj);

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
/*******************************************************************************************************************//**
* @brief            Output buffer information.
* @param[in]        p_ptr_self  A pointer to Memory Buffer handle.
* @param[out]       p_output    Output stream to write information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_IO
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_debugbufferprint (const void* p_ptr_self, FILE* p_output);

/*******************************************************************************************************************//**
* @brief            Output buffer information.
* @param[in]        p_ptr_self  A pointer to Memory Manager handle.
* @param[out]       p_output    Output stream to write information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_IO
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_debugmmngrprint (const void* p_ptr_self, FILE* p_output);
#endif

/*******************************************************************************************************************//**
* @brief            Find Memory Buffer Information Area.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[out]       pp_buffer_info   A pointer to store the address of the Memory Buffer Information Area.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_find_free_buffer_info(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                                    st_osal_mem_buf_ptr_self_t ** pp_buffer_info);

/*******************************************************************************************************************//**
* @brief            Synchronize a sub region of the buffer CPU memory to the HW.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[in]        offset     Offset with the buffer object limits.
* @param[in]        size       Size of the region to be synced.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_flush (void* p_ptr_self, size_t offset, size_t size);

/*******************************************************************************************************************//**
* @brief            Get Configuration Pointer.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[out]       p_config   A pointer to store the configuration.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getconfig (const void* p_ptr_self, st_osal_mmngr_config_t* const p_config);

/*******************************************************************************************************************//**
* @brief            Get Const CPU Pointer.
* @param[in]        p_ptr_self       A pointer to Memory Buffer handle.
* @param[out]       pp_const_cpu_ptr A pointer to the cpu accessible memory.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getconstcpuptr (void* p_ptr_self, const void** const pp_const_cpu_ptr);

/*******************************************************************************************************************//**
* @brief            Get CPU Pointer.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[out]       pp_cpu_ptr A pointer to the cpu modifiable memory.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getcpuptr (void* p_ptr_self, void** const pp_cpu_ptr);

/*******************************************************************************************************************//**
* @brief            Get HW Address.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[in]        axi_id     AXI Bus ID.
* @param[out]       p_hw_addr  A pointer to store the hardware address.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_gethwaddr (const void* p_ptr_self,
                                                         osal_axi_bus_id_t axi_id,
                                                         uintptr_t* const p_hw_addr);

/*******************************************************************************************************************//**
* @brief            Get allocate buffer size.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[out]       p_size     A pointer to store the buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getsize (const void* p_ptr_self, size_t* const p_size);

/*******************************************************************************************************************//**
* @brief            Synchronize a sub region of the buffer CPU memory to the HW.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[in]        offset     Offset with the buffer object limits.
* @param[in]        size       Size of the region to be synced.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_invalidate (void* p_ptr_self, size_t offset, size_t size);

/*******************************************************************************************************************//**
* @brief            Open memory manager.
* @param[in]        p_usr_config     A pointer to configuration to instanciate the OSAL Memory Manager.
* @param[in]        region_idx       Memory region ID.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open(const st_osal_mmngr_config_t * const p_usr_config,
                                                   osal_memory_region_idx_t region_idx);

/*******************************************************************************************************************//**
* @brief            Open memory manager (allocation process).
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        p_usr_config     A pointer to configuration to instanciate the OSAL Memory Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_malloc (st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                           const st_osal_mmngr_config_t * const p_usr_config);

/*******************************************************************************************************************//**
* @brief            Open memory manager (mapping process).
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        p_usr_config     A pointer to configuration to instanciate the OSAL Memory Manager.
* @param[in]        region_idx       Memory region ID.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_map (st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                        const st_osal_mmngr_config_t * const p_usr_config,
                                                        osal_memory_region_idx_t region_idx);

/*******************************************************************************************************************//**
* @brief            Unmap the resources.
* @param[in]        phase_id           Processing phase id.
* @param[in]        mem_fd             File descriptor.
* @param[in]        p_mngr_control     A pointer to the memory manager control table.
* @return           void
***********************************************************************************************************************/
/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC void osal_internal_mem_open_unmap(e_osal_mem_mmap_phase_t phase_id,
                                              int mem_fd,
                                              st_osal_mem_mmngr_ptr_self_t * p_mngr_control);

/*******************************************************************************************************************//**
* @brief            Register the user's callback function.
* @param[in]        p_ptr_self      A pointer to Memory Manager handle.
* @param[in]        monitored_event Event that shall be monitored by the provided monitor function.
* @param[in]        monitor_handler Function to be called by the mmngr for the monitored_event.
* @param[in]        p_user_arg      User argument that will be passed without modification/accessed to the monitor handler function.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_PROHIBITED
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_registermonitor (void* p_ptr_self,
                                                               e_osal_mmngr_event_t monitored_event,
                                                               fp_osal_memory_mngr_cb_t monitor_handler,
                                                               void* p_monitor_handler_user_arg);

/*******************************************************************************************************************//**
* @brief            Send monitor event.
* @param[in]        p_mutex_id     A pointer to mutex object.
* @param[in]        event_id       Event ID.
* @param[in]        error_code     Error code.
* @param[in]        p_mngr_control A pointer to the memory manager control table.
* @param[in]        buffer_hndl    Memory Buffer handle.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_send_monitor_event (pthread_mutex_t * p_mutex_id,
                                                                  e_osal_mmngr_event_t event_id,
                                                                  e_osal_return_t error_code,
                                                                  st_osal_mem_mmngr_ptr_self_t const * p_mngr_control,
                                                                  osal_memory_buffer_handle_t buffer_hndl);

/*******************************************************************************************************************//**
* @brief            Internal set mutex attributes.
* @param[in/out]    p_mutex_attr     The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Initialize memory manager pointer self.
* @param[in]        p_mutex_attr     The pointor of mutex attributes.
* @param[in]        memory_regions   The number of memory region.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_init_mmngr_self(const pthread_mutexattr_t *p_mutex_attr,
                                                              size_t memory_regions);

/*******************************************************************************************************************//**
* @brief            Internal configuration check for memory manager open.
* @param[in]        p_config        A pointer to store the configuration.
* @param[in]        region_idx      Memory region ID.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_CONF_UNSUPPORTED
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_config_check(const st_osal_mmngr_config_t *const p_config,
                                                        osal_memory_region_idx_t region_idx);

/*******************************************************************************************************************//**
* @brief            Internal configuration mode check for memory manager open.
* @param[in]        p_config   A pointer to store the configuration.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_CONF_UNSUPPORTED
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_mode_check(const st_osal_mmngr_config_t *const p_config);

/*******************************************************************************************************************//**
* @brief            Internal meml initialize.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        p_usr_config     A pointer to configuration to instanciate the OSAL Memory Manager.
* @param[in]        region_idx       Memory region ID
* @param[out]       p_mem_fd         File descriptor
* @param[out]       p_phase_id       Processing phase id.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_map_memory(const st_osal_mmngr_config_t * const p_usr_config,
                                                         st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                         osal_memory_region_idx_t region_idx,
                                                         int * p_mem_fd, /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
                                                         e_osal_mem_mmap_phase_t * p_phase_id);

/*******************************************************************************************************************//**
* @brief            Internal meml initialize.
* @param[in]        p_usr_config     A pointer to configuration to instanciate the OSAL Memory Manager.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[out]       p_phase_id       Processing phase id.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_init_meml(const st_osal_mmngr_config_t * const p_usr_config,
                                                        st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                        e_osal_mem_mmap_phase_t * p_phase_id);

/*******************************************************************************************************************//**
* @brief            Allocation st_osal_mem_mmngr_ptr_self_t member.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        p_usr_config     A pointer to configuration to instanciate the OSAL Memory Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_mmngr_ptr_self(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                                   const st_osal_mmngr_config_t * const p_usr_config);

/*******************************************************************************************************************//**
* @brief            Internal deaalocate buffers.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc_buffers(st_osal_mem_mmngr_ptr_self_t *p_mngr_control);

/*******************************************************************************************************************//**
* @brief            Internal meml deinitialize.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_deinit_meml(st_osal_mem_mmngr_ptr_self_t *p_mngr_control);

/*******************************************************************************************************************//**
* @brief            Check the memory manager handle.
* @param[in]        p_ptr_self     A pointer to Memory Manager handle.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_check_ptrself(const void * p_ptr_self);

/*******************************************************************************************************************//**
* @brief            Check the ptr_self in memory manager handle.
* @param[in]        mngr_handle     A pointer to Memory Manager handle.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_check_mngrhndl(osal_memory_manager_handle_t const mngr_handle);

/*******************************************************************************************************************//**
* @brief            Check the memory manager control table.
* @param[in]        p_mngr_control     A pointer to the memory manager control table.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_check_mngrctrl(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control);

/*******************************************************************************************************************//**
* @brief            Pre-processing of memory allocation.
* @param[in]        p_ptr_self      A pointer of Memory Manager handle.
* @param[in]        bytes           Requested size of the memory.
* @param[in]        alignment       Alignment of the memory.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_pre(void * p_ptr_self, size_t bytes, size_t alignment);

/*******************************************************************************************************************//**
* @brief            Check the memory manager configuration.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        bytes            Requested size of the memory.
* @param[in]        alignment        Alignment of the memory.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_config_check(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                                 size_t bytes,
                                                                 size_t alignment);

/*******************************************************************************************************************//**
* @brief            Allocates memory process.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        bytes            Requested size of the memory.
* @param[in]        alignment        Alignment of the memory.
* @param[in]        p_buffer_info    A pointer to the Memory Buffer Information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                         size_t bytes,
                                                         size_t alignment,
                                                         st_osal_mem_buf_ptr_self_t *p_buffer_info);

/*******************************************************************************************************************//**
* @brief            Internal memory manager deallocate pre-processing.
* @param[in]        p_ptr_self   A pointer to Memory Manager handle.
* @param[in]        buffer_obj   A pointer to Memory Buffer Object.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_HANDLE_NOT_SUPPORTED
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc_pre(const void * p_ptr_self,
                                                          osal_memory_buffer_handle_t const buffer_obj);

/*******************************************************************************************************************//**
* @brief            Internal memory manager get configuration pre-processing..
* @param[in]        p_ptr_self   A pointer to Memory Manager handle.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getconfig_pre(const void *p_ptr_self);

/*******************************************************************************************************************//**
* @brief            Internal memory manager register monitor pre-processing.
* @param[in]        p_ptr_self   A pointer to Memory Manager handle.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_registermonitor_pre(const void *p_ptr_self);

/*******************************************************************************************************************//**
* @brief            Internal memory manager register monitor process.
* @param[in]        monitored_event              Event that shall be monitored by the provided monitor function.
* @param[in]        monitor_handler              Function to be called by the mmngr for the monitored_event.
* @param[in]        p_monitor_handler_user_arg   A pointer to user argument.
* @param[in]        p_mngr_control               A pointer to the memory manager control table.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_registermonitor_proc(e_osal_mmngr_event_t monitored_event,
                                                                   fp_osal_memory_mngr_cb_t monitor_handler,
                                                                   void* p_monitor_handler_user_arg,
                                                                   st_osal_mem_mmngr_ptr_self_t *p_mngr_control);

/*******************************************************************************************************************//**
* @brief            Common handle check for functions associated with memory buffer objects.
* @param[in]        p_ptr_self       A pointer to Memory Buffer Object.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_check_buff_hndl(const void* p_ptr_self);

/*******************************************************************************************************************//**
* @brief            Internal memory manager flush process.
* @param[in]        offset         Offset with the buffer object limits.
* @param[in]        size           Size of the region to be synced.
* @param[in]        p_buffer       A pointer to the buffer information area.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_flush_proc(size_t offset,
                                                         size_t size,
                                                         const st_osal_mem_buf_ptr_self_t *p_buffer);

/*******************************************************************************************************************//**
* @brief            Internal memory manager invalidate process.
* @param[in]        offset         Offset with the buffer object limits.
* @param[in]        size           Size of the region to be synced.
* @param[in]        p_buffer       A pointer to the buffer information area.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_invalidate_proc(size_t offset,
                                                              size_t size,
                                                              const st_osal_mem_buf_ptr_self_t *p_buffer);

/*******************************************************************************************************************//**
* @brief            Internal memory manager check buffer process.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        buffer_obj       A pointer to Memory Buffer Object.
* @param[out]       p_check_ret      A pointer to check result.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_checkbuffer_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                               osal_memory_buffer_handle_t buffer_obj,
                                                               e_osal_return_t *p_check_ret);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrInitializeMmngrConfigSt()
* XOS3_OSAL_EMCOS_CD_CD_MEM_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_044]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInitializeMmngrConfigSt(st_osal_mmngr_config_t* const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t local_config;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_config)
    {
        local_config.mode                        = OSAL_MMNGR_ALLOC_MODE_INVALID;
        local_config.memory_limit                = 0;
        local_config.max_allowed_allocations     = 0;
        local_config.max_registered_monitors_cbs = 0;
    }
    else
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_config->mode                        = local_config.mode;
        p_config->memory_limit                = local_config.memory_limit;
        p_config->max_allowed_allocations     = local_config.max_allowed_allocations;
        p_config->max_registered_monitors_cbs = local_config.max_registered_monitors_cbs;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrInitializeMmngrConfigSt()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrInitializeMemoryManagerObj()
* XOS3_OSAL_EMCOS_CD_CD_MEM_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_043]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInitializeMemoryManagerObj(osal_memory_manager_t* const p_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t       osal_ret = OSAL_RETURN_OK;
    osal_memory_manager_t local_obj;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_obj)
    {
        local_obj.p_ptr_self        = NULL;
        local_obj.p_allocate        = NULL;
        local_obj.p_deallocate      = NULL;
        local_obj.p_getConfig       = NULL;
        local_obj.p_registerMonitor = NULL;
        local_obj.p_print           = NULL;
    }
    else
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2916 1 # No problem because it is only set in the pointer argument once the internal variable is moved. */
        p_obj->p_ptr_self        = local_obj.p_ptr_self;
        p_obj->p_allocate        = local_obj.p_allocate;
        p_obj->p_deallocate      = local_obj.p_deallocate;
        p_obj->p_getConfig       = local_obj.p_getConfig;
        p_obj->p_registerMonitor = local_obj.p_registerMonitor;
        p_obj->p_print           = local_obj.p_print;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrInitializeMemoryManagerObj()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrInitializeMemoryBufferObj()
* XOS3_OSAL_EMCOS_CD_CD_MEM_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_041]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_042]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInitializeMemoryBufferObj(osal_memory_buffer_t* const p_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret = OSAL_RETURN_OK;
    osal_memory_buffer_t buffer_obj;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_obj)
    {
        buffer_obj.p_ptr_self       = NULL;
        buffer_obj.mmngr_owner_hndl = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        buffer_obj.p_getCpuPtr      = NULL;
        buffer_obj.p_getConstCpuPtr = NULL;
        buffer_obj.p_flush          = NULL;
        buffer_obj.p_invalidate     = NULL;
        buffer_obj.p_getHwAddr      = NULL;
        buffer_obj.p_getSize        = NULL;
        buffer_obj.p_checkBuffer    = NULL;
        buffer_obj.p_print          = NULL;
    }
    else
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2916 1 # No problem because it is only set in the pointer argument once the internal variable is moved. */
        p_obj->p_ptr_self       = buffer_obj.p_ptr_self;
        /* PRQA S 2916 1 # No problem because it is only set in the pointer argument once the internal variable is moved. */
        p_obj->mmngr_owner_hndl = buffer_obj.mmngr_owner_hndl;
        p_obj->p_getCpuPtr      = buffer_obj.p_getCpuPtr;
        p_obj->p_getConstCpuPtr = buffer_obj.p_getConstCpuPtr;
        p_obj->p_flush          = buffer_obj.p_flush;
        p_obj->p_invalidate     = buffer_obj.p_invalidate;
        p_obj->p_getHwAddr      = buffer_obj.p_getHwAddr;
        p_obj->p_getSize        = buffer_obj.p_getSize;
        p_obj->p_checkBuffer    = buffer_obj.p_checkBuffer;
        p_obj->p_print          = buffer_obj.p_print;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrInitializeMemoryBufferObj()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrOpen()
* XOS3_OSAL_EMCOS_CD_CD_MEM_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_046]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_047]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_018]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrOpen(const st_osal_mmngr_config_t *const p_config,
                                 osal_memory_manager_handle_t *const p_hndl_mmngr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;
    osal_memory_region_idx_t default_idx;
    size_t                   memory_infos;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MMNGR_STATE_INIT != gs_osal_memory_is_initialized)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        default_idx = R_OSAL_DCFG_GetDefaultRegionInfo();
        memory_infos = R_OSAL_DCFG_GetNumOfMemoryRegionInfo();
        if (memory_infos <= default_idx)
        {
            osal_ret = OSAL_RETURN_FAIL;    /* TODO: This error code needs to be considered, e.g. OSAL_RETURN_CONF */
        }
        else
        {
            /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
            if (OSAL_MEMORY_HANDLE_ID == gspp_osal_mem_mmngr_ptr_self[default_idx]->handle_id)
            {
                /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                 True if the correct handle was passed. False detection of the tool. */
                osal_ret = OSAL_RETURN_STATE;
            }
            else if ((NULL == p_config) || (NULL == p_hndl_mmngr))
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                osal_ret = osal_internal_mem_open_config_check(p_config, default_idx);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mem_open(p_config, default_idx);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    *p_hndl_mmngr = gsp_osal_memory_manager[default_idx];
                }
                else
                {
                   /* Do nothing */
                }
            }
            else
            {
               /* Do nothing */
            }
        }   /* end of if(memory_infos <= default_idx). */
    }   /* end of if(OSAL_MMNGR_STATE_INIT != gs_osal_memory_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrOpen()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrOpenFromRegionIdx()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrOpenFromRegionIdx(const st_osal_mmngr_config_t * const p_config,
                                              osal_memory_region_idx_t region_idx,
                                              osal_memory_manager_handle_t * const p_hndl_mmngr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret;
    size_t                 memory_infos;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MMNGR_STATE_INIT != gs_osal_memory_is_initialized)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        memory_infos = R_OSAL_DCFG_GetNumOfMemoryRegionInfo();
        if (memory_infos <= region_idx)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
            if (OSAL_MEMORY_HANDLE_ID == gspp_osal_mem_mmngr_ptr_self[region_idx]->handle_id)
            {
                /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                 True if the correct handle was passed. False detection of the tool. */
                osal_ret = OSAL_RETURN_STATE;
            }
            else if ((NULL == p_config) || (NULL == p_hndl_mmngr))
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                osal_ret = osal_internal_mem_open_config_check(p_config, region_idx);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mem_open(p_config, region_idx);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    *p_hndl_mmngr = gsp_osal_memory_manager[region_idx];
                }
                else
                {
                   /* Do nothing */
                }
            }
            else
            {
               /* Do nothing */
            }
        }   /* end of if(memory_infos <= region_idx). */
    }   /* end of if(OSAL_MMNGR_STATE_INIT != gs_osal_memory_is_initialized). */

    return osal_ret;

}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrOpenFromRegionIdx()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrClose()
* XOS3_OSAL_EMCOS_CD_CD_MEM_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_030]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_018]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrClose(osal_memory_manager_handle_t hndl_mmngr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret = OSAL_RETURN_OK;
    const st_osal_mem_mmngr_ptr_self_t *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MMNGR_STATE_INIT != gs_osal_memory_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == hndl_mmngr)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == hndl_mmngr->p_ptr_self)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *) hndl_mmngr->p_ptr_self;
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (OSAL_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (NULL == p_mngr_control->p_buffers)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_mem_close(hndl_mmngr);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrClose()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrAlloc()
* XOS3_OSAL_EMCOS_CD_CD_MEM_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_028]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrAlloc(osal_memory_manager_handle_t hndl_mmngr,
                                  size_t size,
                                  size_t alignment,
                                  osal_memory_buffer_handle_t* const p_buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == hndl_mmngr)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == p_buffer_obj)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == hndl_mmngr->p_allocate)
    {
        osal_ret = OSAL_RETURN_HANDLE_NOT_SUPPORTED;
    }
    else
    {
        osal_ret = hndl_mmngr->p_allocate(hndl_mmngr->p_ptr_self, size, alignment, p_buffer_obj);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrAlloc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrDealloc()
* XOS3_OSAL_EMCOS_CD_CD_MEM_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_031]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrDealloc(osal_memory_manager_handle_t hndl_mmngr, osal_memory_buffer_handle_t buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == hndl_mmngr) || (NULL == buffer_obj))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == hndl_mmngr->p_deallocate)
    {
        osal_ret = OSAL_RETURN_HANDLE_NOT_SUPPORTED;
    }
    else
    {
        osal_ret = hndl_mmngr->p_deallocate(hndl_mmngr->p_ptr_self, buffer_obj);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrDealloc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrGetConfig()
* XOS3_OSAL_EMCOS_CD_CD_MEM_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_035]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrGetConfig(osal_memory_manager_handle_t hndl_mmngr, st_osal_mmngr_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret;
    st_osal_mmngr_config_t max_config;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
    {
        if (NULL == p_config)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            osal_ret = R_OSAL_DCFG_GetMaxConfigOfMemory(&max_config, 0);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                /* PRQA S 2811 1 # No problem because it is only set in the pointer argument
                   once the internal variable is moved. */
                p_config->mode                        = max_config.mode;
                p_config->memory_limit                = max_config.memory_limit;
                p_config->max_allowed_allocations     = max_config.max_allowed_allocations;
                p_config->max_registered_monitors_cbs = max_config.max_registered_monitors_cbs;
            }
        }
    }
    else if (NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == hndl_mmngr->p_getConfig)
    {
        osal_ret = OSAL_RETURN_HANDLE_NOT_SUPPORTED;
    }
    else
    {
        osal_ret  = hndl_mmngr->p_getConfig(hndl_mmngr->p_ptr_self, p_config);
    }   /* end of if(OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrGetConfig()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrGetOsalMaxConfig()
* XOS3_OSAL_EMCOS_CD_CD_MEM_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_039]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetOsalMaxConfig(st_osal_mmngr_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;
    st_osal_mmngr_config_t   max_config;
    osal_memory_region_idx_t default_idx;
    size_t                   memory_infos;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        default_idx = R_OSAL_DCFG_GetDefaultRegionInfo();
        memory_infos = R_OSAL_DCFG_GetNumOfMemoryRegionInfo();
        if(memory_infos <= default_idx)
        {
            osal_ret = OSAL_RETURN_FAIL;    /* TODO: This error code needs to be considered, e.g. OSAL_RETURN_CONF */
        }
        else
        {
            osal_ret = R_OSAL_DCFG_GetMaxConfigOfMemory(&max_config, default_idx);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if(NULL == p_config). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_config->mode                        = max_config.mode;
        p_config->memory_limit                = max_config.memory_limit;
        p_config->max_allowed_allocations     = max_config.max_allowed_allocations;
        p_config->max_registered_monitors_cbs = max_config.max_registered_monitors_cbs;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrGetOsalMaxConfig()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(osal_memory_region_idx_t region_idx,
                                                          st_osal_mmngr_config_t *const p_config)

{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret;
    st_osal_mmngr_config_t max_config;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = R_OSAL_DCFG_GetMaxConfigOfMemory(&max_config, region_idx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_config->mode                        = max_config.mode;
        p_config->memory_limit                = max_config.memory_limit;
        p_config->max_allowed_allocations     = max_config.max_allowed_allocations;
        p_config->max_registered_monitors_cbs = max_config.max_registered_monitors_cbs;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrRegisterMonitor()
* XOS3_OSAL_EMCOS_CD_CD_MEM_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_048]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_049]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_050]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrRegisterMonitor(osal_memory_manager_handle_t hndl_mmngr,
                                            e_osal_mmngr_event_t monitored_event,
                                            fp_osal_memory_mngr_cb_t monitor_handler,
                                            void * p_user_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == monitor_handler)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((OSAL_MMNGR_EVENT_INVALID >= monitored_event) || (OSAL_MMNGR_EVENT_ERROR <= monitored_event))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == hndl_mmngr->p_registerMonitor)
    {
        osal_ret = OSAL_RETURN_HANDLE_NOT_SUPPORTED;
    }
    else
    {
        osal_ret = hndl_mmngr->p_registerMonitor(hndl_mmngr->p_ptr_self, monitored_event, monitor_handler, p_user_arg);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrRegisterMonitor()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrDebugMmngrPrint()
* XOS3_OSAL_EMCOS_CD_CD_MEM_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_033]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrDebugMmngrPrint(osal_memory_manager_handle_t hndl_mmngr, FILE * p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == hndl_mmngr)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == p_output)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == hndl_mmngr->p_print)
    {
        osal_ret = OSAL_RETURN_HANDLE_NOT_SUPPORTED;
    }
    else
    {
        osal_ret = hndl_mmngr->p_print(hndl_mmngr->p_ptr_self, p_output);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrDebugMmngrPrint()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrGetCpuPtr()
* XOS3_OSAL_EMCOS_CD_CD_MEM_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_037]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrGetCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl, void **const pp_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == pp_cpu_ptr)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == memory_buffer_obj_hndl->p_getCpuPtr)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = memory_buffer_obj_hndl->p_getCpuPtr(memory_buffer_obj_hndl->p_ptr_self, pp_cpu_ptr);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrGetCpuPtr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrGetConstCpuPtr()
* XOS3_OSAL_EMCOS_CD_CD_MEM_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_036]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrGetConstCpuPtr (osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                            const void **const pp_const_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == pp_const_cpu_ptr)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == memory_buffer_obj_hndl->p_getConstCpuPtr)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = memory_buffer_obj_hndl->p_getConstCpuPtr(memory_buffer_obj_hndl->p_ptr_self, pp_const_cpu_ptr);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrGetConstCpuPtr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrFlush()
* XOS3_OSAL_EMCOS_CD_CD_MEM_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_034]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrFlush (osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == memory_buffer_obj_hndl->p_flush)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = memory_buffer_obj_hndl->p_flush(memory_buffer_obj_hndl->p_ptr_self, offset, size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrFlush()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrInvalidate()
* XOS3_OSAL_EMCOS_CD_CD_MEM_015
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_045]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrInvalidate (osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == memory_buffer_obj_hndl->p_invalidate)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = memory_buffer_obj_hndl->p_invalidate(memory_buffer_obj_hndl->p_ptr_self, offset, size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrInvalidate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrGetHwAddr()
* XOS3_OSAL_EMCOS_CD_CD_MEM_016
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_038]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrGetHwAddr (osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                       osal_axi_bus_id_t axi_id,
                                       uintptr_t *const p_hw_addr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == p_hw_addr)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (OSAL_AXI_BUS_ID_INVALID == axi_id)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == memory_buffer_obj_hndl->p_getHwAddr)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = memory_buffer_obj_hndl->p_getHwAddr(memory_buffer_obj_hndl->p_ptr_self, axi_id, p_hw_addr);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrGetHwAddr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrGetSize()
* XOS3_OSAL_EMCOS_CD_CD_MEM_017
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_040]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrGetSize (osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t *const p_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == p_size)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == memory_buffer_obj_hndl->p_getSize)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = memory_buffer_obj_hndl->p_getSize(memory_buffer_obj_hndl->p_ptr_self, p_size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrGetSize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrCheckBuffer()
* XOS3_OSAL_EMCOS_CD_CD_MEM_018
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_029]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrCheckBuffer (osal_memory_buffer_handle_t memory_buffer_obj_hndl)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == memory_buffer_obj_hndl->p_checkBuffer)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = memory_buffer_obj_hndl->p_checkBuffer(memory_buffer_obj_hndl->p_ptr_self);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrCheckBuffer()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MmngrDebugBufferPrint()
* XOS3_OSAL_EMCOS_CD_CD_MEM_019
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_032]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_MmngrDebugBufferPrint (osal_memory_buffer_handle_t hndl_buffer, FILE * p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == hndl_buffer)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (NULL == p_output)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == hndl_buffer->p_print)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = hndl_buffer->p_print(hndl_buffer->p_ptr_self, p_output);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MmngrDebugBufferPrint()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_memory_initialize()
* XOS3_OSAL_EMCOS_CD_CD_MEM_020
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_026]
***********************************************************************************************************************/
e_osal_return_t osal_memory_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                             os_ret;
    pthread_mutexattr_t             mutex_attr;
    size_t                          loop;
    size_t                          memory_regions;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MMNGR_STATE_NONE != gs_osal_memory_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_memory_is_initialized = OSAL_MMNGR_STATE_INIT;

        osal_ret = osal_internal_mem_set_mutex_attr(&mutex_attr);
        if (OSAL_RETURN_OK == osal_ret)
        {
            memory_regions = R_OSAL_DCFG_GetNumOfMemoryRegionInfo();

            /* PRQA S 5118 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initialize. */
            gsp_osal_memory_manager = (osal_memory_manager_handle_t *)malloc(sizeof(osal_memory_manager_handle_t) * memory_regions);
            if (NULL == gsp_osal_memory_manager)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* PRQA S 5118 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initialize. */
                gspp_osal_mem_mmngr_ptr_self = (st_osal_mem_mmngr_ptr_self_t **)malloc(sizeof(st_osal_mem_mmngr_ptr_self_t *) * memory_regions);
                if (NULL == gspp_osal_mem_mmngr_ptr_self)
                {
                    /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
                    free((void*)gsp_osal_memory_manager);
                    gsp_osal_memory_manager = NULL;
                    osal_ret = OSAL_RETURN_MEM;
                }
                else
                {
                    /* Do nothing */
                }
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                for (loop = 0; loop < memory_regions; loop++)
                {
                    gspp_osal_mem_mmngr_ptr_self[loop] = NULL;
                }
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mem_init_mmngr_self(&mutex_attr, memory_regions);
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutexattr_destroy(&mutex_attr);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL; 
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
    }   /* end of if(OSAL_MMNGR_STATE_NONE != gs_osal_memory_is_initialized). */

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* For post-processing in the event of an error, do not perform a return value determination. */
        (void)osal_memory_deinitialize();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_memory_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_memory_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_MEM_021
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_024]
***********************************************************************************************************************/
e_osal_return_t osal_memory_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret; 
    size_t          memory_regions;
    size_t          loop;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_memory_is_initialized = OSAL_MMNGR_STATE_NONE;

    if ((NULL != gsp_osal_memory_manager) && (NULL != gspp_osal_mem_mmngr_ptr_self))
    {
        if (NULL != gspp_osal_mem_mmngr_ptr_self[0])
        {
            memory_regions = R_OSAL_DCFG_GetNumOfMemoryRegionInfo();

            for (loop = 0; loop < memory_regions; loop++)
            {
                /* If it is not closed, it forces the resource to be released. */
                if (OSAL_MEMORY_HANDLE_ID == gspp_osal_mem_mmngr_ptr_self[loop]->handle_id)
                {
                    (void)osal_internal_mem_close(gsp_osal_memory_manager[loop]);   /* TODO change loop*/
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_mutex_destroy(&gspp_osal_mem_mmngr_ptr_self[loop]->inner_mtx);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of for(loop = 0; loop < memory_regions; loop++). */

            /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
            free((void*)gspp_osal_mem_mmngr_ptr_self[0]);
        }
        else
        {
            /* Do nothing */
        }   /* end of if(NULL != gspp_osal_mem_mmngr_ptr_self[0]). */
        /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
        free((void*)gspp_osal_mem_mmngr_ptr_self);
        /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
        free((void*)gsp_osal_memory_manager);
        gspp_osal_mem_mmngr_ptr_self = NULL;
        gsp_osal_memory_manager      = NULL;
    }
    else
    {
        /* Do nothing */
    }   /* end of if((NULL != gsp_osal_memory_manager) && (NULL != gspp_osal_mem_mmngr_ptr_self)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_memory_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_memory_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_MEM_022
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_025]
***********************************************************************************************************************/
e_osal_return_t osal_memory_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MMNGR_STATE_NONE == gs_osal_memory_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_memory_is_initialized = OSAL_MMNGR_STATE_NONE;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_memory_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_memory_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_MEM_023
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_027]
***********************************************************************************************************************/
void osal_memory_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_memory_is_initialized = OSAL_MMNGR_STATE_INIT;

    return;
}
/***********************************************************************************************************************
* End of function osal_memory_set_is_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_set_mutex_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutexattr_init(p_mutex_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_mutexattr_settype(p_mutex_attr, (int)PTHREAD_MUTEX_ERRORCHECK);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_mutexattr_destroy(p_mutex_attr);
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_set_mutex_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_init_mmngr_self()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_init_mmngr_self(const pthread_mutexattr_t *p_mutex_attr,
                                                              size_t memory_regions)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                             os_ret;
    st_osal_mem_mmngr_ptr_self_t    *p_tmp;
    size_t                          loop;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118,2706 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initialize. */
    p_tmp = (st_osal_mem_mmngr_ptr_self_t *)malloc(sizeof(st_osal_mem_mmngr_ptr_self_t) * memory_regions);
    if (NULL == p_tmp)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        for (loop = 0; (loop < memory_regions) && (OSAL_RETURN_OK == osal_ret); loop++)
        {
            gspp_osal_mem_mmngr_ptr_self[loop] = &p_tmp[loop];

            gsp_osal_memory_manager[loop] = (osal_memory_manager_handle_t)&gspp_osal_mem_mmngr_ptr_self[loop]->mmngr_obj;

            gspp_osal_mem_mmngr_ptr_self[loop]->inner_timeout    = (osal_milli_sec_t)OSAL_MEMORY_INNER_TIMEOUT;
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
            gspp_osal_mem_mmngr_ptr_self[loop]->physical_addr    = (uintptr_t)NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->p_virtual_addr   = NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->p_buffers        = NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->num_buffers      = 0;
            gspp_osal_mem_mmngr_ptr_self[loop]->use_buffer_total = 0;
            gspp_osal_mem_mmngr_ptr_self[loop]->p_monitor        = NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->num_monitor      = 0;
            gspp_osal_mem_mmngr_ptr_self[loop]->handle_id        = OSAL_INVALID_DEVICE_HANDLE_ID;
            gspp_osal_mem_mmngr_ptr_self[loop]->region_idx       = loop;
            /* PRQA S 0314 1 # No change on the required cast on the system. */
            gsp_osal_memory_manager[loop]->p_ptr_self = (void *)gsp_osal_memory_manager[loop];

            /* PRQA S 2736 1 # No problem because osal_memory_deinitialize is called pthread_mutex_destroy. */
            os_ret = pthread_mutex_init(&gspp_osal_mem_mmngr_ptr_self[loop]->inner_mtx, p_mutex_attr);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of for(loop = 0; loop < memory_regions && OSAL_RETURN_OK == osal_ret; loop++). */
    }   /* end of if(NULL == p_tmp). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_init_mmngr_self()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_open_config_check()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_config_check(const st_osal_mmngr_config_t *const p_config,
                                                                osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret;
    st_osal_mmngr_config_t max_config;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_DCFG_GetMaxConfigOfMemory(&max_config, region_idx);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (1U > p_config->max_allowed_allocations)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else if (max_config.max_allowed_allocations < p_config->max_allowed_allocations)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (max_config.max_registered_monitors_cbs < p_config->max_registered_monitors_cbs)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 1281 1 # SIZE_MAX in the OSAL_MEMORY_MANAGER_CFG_UNLIMITED are used according to the specifications defined in eMCOS. */
        if ((OSAL_MEMORY_MANAGER_CFG_UNLIMITED != p_config->memory_limit) &&
            (max_config.memory_limit < p_config->memory_limit))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else if (OSAL_MMNGR_MEM_4K > p_config->memory_limit)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mem_open_mode_check(p_config);
    }
    else
    {
       /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_open_config_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_open_mode_check()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_mode_check(const st_osal_mmngr_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    switch (p_config->mode)
    {
        case OSAL_MMNGR_ALLOC_MODE_STACK:
        case OSAL_MMNGR_ALLOC_MODE_STACK_PROT:
        case OSAL_MMNGR_ALLOC_MODE_FREE_LIST:
        case OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT:
            /* Do nothing */
            break;
        case OSAL_MMNGR_ALLOC_MODE_UNSPECIFIED:
        case OSAL_MMNGR_ALLOC_MODE_UNSPECIFIED_PROT:
            osal_ret = OSAL_RETURN_CONF_UNSUPPORTED;
            break;
        default:
            osal_ret = OSAL_RETURN_CONF;
            break;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_open_mode_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_open()
* XOS3_OSAL_EMCOS_CD_CD_MEM_024
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_018]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open(const st_osal_mmngr_config_t * const p_usr_config,
                                                   osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *) gsp_osal_memory_manager[region_idx]->p_ptr_self;

    osal_ret = R_OSAL_Internal_TimedlockMutex(&p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = osal_internal_mem_open_map(p_mngr_control, p_usr_config, region_idx);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mem_open_malloc(p_mngr_control, p_usr_config);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            gsp_osal_memory_manager[region_idx]->p_allocate        = osal_internal_mem_alloc;
            gsp_osal_memory_manager[region_idx]->p_deallocate      = osal_internal_mem_dealloc;
            gsp_osal_memory_manager[region_idx]->p_getConfig       = osal_internal_mem_getconfig;
            gsp_osal_memory_manager[region_idx]->p_registerMonitor = osal_internal_mem_registermonitor;
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
            gsp_osal_memory_manager[region_idx]->p_print           = osal_internal_mem_debugmmngrprint;
#else
            gsp_osal_memory_manager[region_idx]->p_print           = NULL;
#endif

            p_mngr_control->config.mode                        = p_usr_config->mode;
            p_mngr_control->config.memory_limit                = p_usr_config->memory_limit;
            p_mngr_control->config.max_allowed_allocations     = p_usr_config->max_allowed_allocations;
            p_mngr_control->config.max_registered_monitors_cbs = p_usr_config->max_registered_monitors_cbs;

            p_mngr_control->handle_id     = (uint64_t)OSAL_MEMORY_HANDLE_ID;
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        os_ret = pthread_mutex_unlock(&p_mngr_control->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;

}
/***********************************************************************************************************************
* End of function osal_internal_mem_open()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_open_map()
* XOS3_OSAL_EMCOS_CD_CD_MEM_025
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_020]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_map(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                       const st_osal_mmngr_config_t * const p_usr_config,
                                                       osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         mem_fd;
    e_osal_mem_mmap_phase_t     phase_id     = OSAL_MEM_MMAP_PHASE_INVALID;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_map_memory(p_usr_config, p_mngr_control, region_idx, &mem_fd, &phase_id);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mem_init_meml(p_usr_config, p_mngr_control, &phase_id);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = close(mem_fd);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            phase_id = OSAL_MEM_MMAP_PHASE_CLOSE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_internal_mem_open_unmap(phase_id, mem_fd, p_mngr_control);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_open_map()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_map_memory()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_map_memory(const st_osal_mmngr_config_t * const p_usr_config,
                                                         st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                         osal_memory_region_idx_t region_idx,
                                                         int * p_mem_fd, /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
                                                         e_osal_mem_mmap_phase_t * p_phase_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret      = OSAL_RETURN_OK;
    st_osal_mmngr_config_t      max_config;
    size_t                      mem_size;
    pdev_memrange_t             pdevmr;
    uint64_t                    physical_addr;
    st_osal_mmngr_region_info_t info;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         mem_fd;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)R_OSAL_DCFG_GetMemoryRegionInfo(&info, region_idx);

    /* PRQA S 1291 1 # O_RDWR uses octal and is used in accordance with the specification of open(). */
    mem_fd = open(info.name, O_RDWR);
    if (-1 == mem_fd)
    {
        osal_ret = OSAL_RETURN_DEV;
    }
    else
    {
        *p_mem_fd = mem_fd;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2855 2 # It is used according to the specifications defined in eMCOS. */
        /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
        os_ret = posix_devctl(mem_fd, PDEV_MEMRANGE_GET_INFO(0), (void*)&pdevmr, sizeof(pdev_memrange_t), NULL);
        if (0 != os_ret)
        {
            osal_ret    = OSAL_RETURN_DEV;
            *p_phase_id = OSAL_MEM_MMAP_PHASE_DEVCTL;
        }
        else
        {
            physical_addr = pdevmr.base + (uint64_t)info.offset;
            if ((0x400000000ULL <= pdevmr.base) && (0x47FFFFFFFULL >= pdevmr.base)) /* If DDR0, change 32bit address */
            {
                /* Holds the physical address of the mirror. */
                p_mngr_control->physical_addr = (uintptr_t)((physical_addr & 0xFFFFFFFFULL) + 0x40000000ULL);
            }
            else
            {
                p_mngr_control->physical_addr = (uintptr_t)physical_addr;
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MEMORY_MANAGER_CFG_UNLIMITED == p_usr_config->memory_limit)
        {
            osal_ret = R_OSAL_DCFG_GetMaxConfigOfMemory(&max_config, region_idx);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret    = OSAL_RETURN_FAIL;
                *p_phase_id = OSAL_MEM_MMAP_PHASE_GETMAX;
            }
            else
            {
                mem_size = max_config.memory_limit;
            }
        }
        else
        {
            mem_size = p_usr_config->memory_limit;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_mngr_control->size           = (mem_size + OSAL_MMNGR_MEM_RESERVE_SIZE) & ~4095U;
        /* PRQA S 4542 3 # System is requesting an define, so it cannot be changed. */
        p_mngr_control->p_virtual_addr = mmap(NULL,
                                              p_mngr_control->size,
                                              /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
                                              (int)pdevmr.attrs,
                                              /* PRQA S 4447,0499 1 # It is used according to the specifications defined in eMCOS. */
                                              (MAP_SHARED | MAP_POPULATE),
                                              mem_fd,
                                              /* PRQA S 2857 1 # Since the address processes only up to 36 bits, there is no problem of overflow. */
                                              (off_t)physical_addr);
        /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
        if (MAP_FAILED == p_mngr_control->p_virtual_addr)
        {
            osal_ret    = OSAL_RETURN_MEM;
            *p_phase_id = OSAL_MEM_MMAP_PHASE_MMAP;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_map_memory()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_init_meml()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_init_meml(const st_osal_mmngr_config_t * const p_usr_config,
                                                        st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                        e_osal_mem_mmap_phase_t * p_phase_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret   = OSAL_RETURN_OK;
    e_meml_return_t         meml_ret;
    e_meml_alloc_mode_t     meml_mode;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    switch (p_usr_config->mode)
    {
        case OSAL_MMNGR_ALLOC_MODE_STACK:
        case OSAL_MMNGR_ALLOC_MODE_STACK_PROT:
            meml_mode = MEML_ALLOC_MODE_STACK;
            break;
        case OSAL_MMNGR_ALLOC_MODE_FREE_LIST:
        default:
            meml_mode = MEML_ALLOC_MODE_FREE_LIST;
            break;
    }
    /* PRQA S 0326 5 # No problem for casting from void * to object pointer. */
    /* PRQA S 0326 3 # R_MEML_Initialize is request, so it cannot be changed. */
    /* PRQA S 0326 1 # System is request, so it cannot be changed. */
    meml_ret = R_MEML_Initialize(&p_mngr_control->meml_handle,
                                 (uintptr_t)p_mngr_control->physical_addr,
                                 (uintptr_t)p_mngr_control->p_virtual_addr,
                                 p_mngr_control->size,
                                 meml_mode);
    if (MEML_RETURN_OK != meml_ret)
    {
        osal_ret    = OSAL_RETURN_FAIL;
        *p_phase_id = OSAL_MEM_MMAP_PHASE_MEML_INIT;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_init_meml()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_open_unmap()
* XOS3_OSAL_EMCOS_CD_CD_MEM_026
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_021]
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC void osal_internal_mem_open_unmap(e_osal_mem_mmap_phase_t phase_id, int mem_fd,
                                              st_osal_mem_mmngr_ptr_self_t * p_mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    switch (phase_id)
    {
        case OSAL_MEM_MMAP_PHASE_DEVCTL:
        case OSAL_MEM_MMAP_PHASE_GETMAX:
        case OSAL_MEM_MMAP_PHASE_MMAP:
            (void)close(mem_fd);
            break;
        case OSAL_MEM_MMAP_PHASE_MEML_INIT:
            (void)munmap(p_mngr_control->p_virtual_addr, p_mngr_control->size);
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
            p_mngr_control->physical_addr  = (uintptr_t)NULL;
            p_mngr_control->p_virtual_addr = NULL;
            (void)close(mem_fd);
            break;
        case OSAL_MEM_MMAP_PHASE_CLOSE:
            (void)R_MEML_Deinitialize(&p_mngr_control->meml_handle);
            (void)munmap(p_mngr_control->p_virtual_addr, p_mngr_control->size);
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
            p_mngr_control->physical_addr  = (uintptr_t)NULL;
            p_mngr_control->p_virtual_addr = NULL;
            break;
        default:
             /* open() failed do nothing. */
            break;
    }   /* end of switch(phase_id). */

    return;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_open_unmap()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_open_malloc()
* XOS3_OSAL_EMCOS_CD_CD_MEM_027
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_019]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_open_malloc(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                          const st_osal_mmngr_config_t * const p_usr_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    size_t              loop_count    = 0;
    size_t              destroy_count = 0;
    pthread_mutexattr_t mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_set_mutex_attr(&mutex_attr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
         osal_ret = osal_internal_mem_alloc_mmngr_ptr_self(p_mngr_control, p_usr_config);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        while ((p_usr_config->max_allowed_allocations > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            os_ret = pthread_mutex_init(&p_mngr_control->p_buffers[loop_count].inner_mtx, &mutex_attr);
            if (0 == os_ret)
            {
                p_mngr_control->p_buffers[loop_count].inner_timeout = OSAL_MEMORY_INNER_TIMEOUT;
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            loop_count++;
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            while (loop_count >= destroy_count)
            {
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)pthread_mutex_destroy(&p_mngr_control->p_buffers[destroy_count].inner_mtx);

                destroy_count++;
            }
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_mutexattr_destroy(&mutex_attr);
            if (NULL != p_mngr_control->p_monitor)
            {
                /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
                free((void*)p_mngr_control->p_monitor);
                p_mngr_control->p_monitor = NULL;
            }
            /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
            free((void*)p_mngr_control->p_buffers);
            p_mngr_control->p_buffers = NULL;
        }
        else
        {
            os_ret = pthread_mutexattr_destroy(&mutex_attr);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_open_malloc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_alloc_mmngr_ptr_self()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_mmngr_ptr_self(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                                   const st_osal_mmngr_config_t * const p_usr_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U != p_usr_config->max_registered_monitors_cbs)
    {
        /* PRQA S 5118 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initialize. */
        p_mngr_control->p_monitor = malloc(sizeof(st_osal_mem_monitor_desc_t) * p_usr_config->max_registered_monitors_cbs);
        if (NULL == p_mngr_control->p_monitor)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5118 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initialize. */
        p_mngr_control->p_buffers = malloc(sizeof(st_osal_mem_buf_ptr_self_t) * p_usr_config->max_allowed_allocations);
        if (NULL == p_mngr_control->p_buffers)
        {
            osal_ret = OSAL_RETURN_MEM;
            if (NULL != p_mngr_control->p_monitor)
            {
                /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
                free((void*)p_mngr_control->p_monitor);
                p_mngr_control->p_monitor = NULL;
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_alloc_mmngr_ptr_self()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_close()
* XOS3_OSAL_EMCOS_CD_CD_MEM_028
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_005]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_close (osal_memory_manager_handle_t hndl_mmngr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;
    size_t                       loop_count        = 0;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *) hndl_mmngr->p_ptr_self;

    osal_ret = R_OSAL_Internal_TimedlockMutex(&p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = osal_internal_mem_dealloc_buffers(p_mngr_control);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mem_deinit_meml(p_mngr_control);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            while ((p_mngr_control->config.max_allowed_allocations > loop_count) && (OSAL_RETURN_OK == osal_ret))
            {
                os_ret = pthread_mutex_destroy(&p_mngr_control->p_buffers[loop_count].inner_mtx);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
                loop_count++;
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
            free((void*)p_mngr_control->p_buffers);
            if (NULL != p_mngr_control->p_monitor)
            {
                /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
                free((void*)p_mngr_control->p_monitor);
            }

            p_mngr_control->handle_id        = OSAL_INVALID_DEVICE_HANDLE_ID;;
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
            p_mngr_control->physical_addr    = (uintptr_t)NULL;
            p_mngr_control->num_buffers      = 0;
            p_mngr_control->use_buffer_total = 0;
            p_mngr_control->num_monitor      = 0;
            p_mngr_control->p_virtual_addr   = NULL;
            p_mngr_control->p_buffers        = NULL;
            p_mngr_control->p_monitor        = NULL;

            hndl_mmngr->p_allocate           = NULL;
            hndl_mmngr->p_deallocate         = NULL;
            hndl_mmngr->p_getConfig          = NULL;
            hndl_mmngr->p_registerMonitor    = NULL;
            hndl_mmngr->p_print              = NULL;
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        os_ret = pthread_mutex_unlock(&p_mngr_control->inner_mtx);
        if (0 != os_ret)
        {
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_close()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_dealloc_buffers()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc_buffers(st_osal_mem_mmngr_ptr_self_t *p_mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret = OSAL_RETURN_OK;
    int64_t                      buff_count;
    size_t                       use_size;
    osal_memory_buffer_handle_t  mngr_buffer_hndl;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U != p_mngr_control->num_buffers)
    {
        for (buff_count = ((int64_t)p_mngr_control->num_buffers - (int64_t)1); 0 <= buff_count; buff_count--)
        {
            mngr_buffer_hndl = (osal_memory_buffer_handle_t)&p_mngr_control->p_buffers[buff_count].mbuff_obj;
            if (NULL == mngr_buffer_hndl->p_ptr_self)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else
            {
                use_size = p_mngr_control->size;

                osal_ret = osal_internal_mem_dealloc_proc(p_mngr_control, mngr_buffer_hndl);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    break;
                }
                else
                {
                    p_mngr_control->num_buffers      -= (size_t)1;
                    p_mngr_control->use_buffer_total -= use_size;
                }
            }
            /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
        }   /* end of for(buff_count = ((int64_t)p_mngr_control->num_buffers - (int64_t)1);
                          0 <= buff_count; buff_count--). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(0U != p_mngr_control->num_buffers). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_dealloc_buffers()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_deinit_meml()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_deinit_meml(st_osal_mem_mmngr_ptr_self_t *p_mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_meml_return_t meml_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    meml_ret = R_MEML_Deinitialize(&p_mngr_control->meml_handle);
    if (MEML_RETURN_OK != meml_ret)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = munmap(p_mngr_control->p_virtual_addr, p_mngr_control->size);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_deinit_meml()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_check_ptrself()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_check_ptrself(const void * p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MMNGR_STATE_INIT != gs_osal_memory_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_ptr_self)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_check_ptrself()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_check_mngrhndl()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_mem_check_mngrhndl(osal_memory_manager_handle_t const mngr_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mngr_handle->p_ptr_self)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_check_mngrhndl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_check_mngrctrl()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_check_mngrctrl(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_check_mngrctrl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_alloc()
* XOS3_OSAL_EMCOS_CD_CD_MEM_029
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_001]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc(void * p_ptr_self,
                                                    size_t bytes,
                                                    size_t alignment,
                                                    osal_memory_buffer_handle_t * const p_buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    osal_memory_manager_handle_t mngr_handle;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;
    st_osal_mem_buf_ptr_self_t   *p_buffer_info   = NULL;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_alloc_pre(p_ptr_self, bytes, alignment);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        mngr_handle    = (osal_memory_manager_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (p_mngr_control->config.max_allowed_allocations <= p_mngr_control->num_buffers)
            {
                osal_ret = OSAL_RETURN_PROHIBITED;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mem_find_free_buffer_info(p_mngr_control, &p_buffer_info);
                if ((OSAL_RETURN_OK != osal_ret) || (NULL == p_buffer_info))
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    *p_buffer_obj = &p_buffer_info->mbuff_obj;
                }
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mem_alloc_proc(p_mngr_control, bytes, alignment, p_buffer_info);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    *p_buffer_obj = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_mngr_control->num_buffers      += (size_t)1;
                p_mngr_control->use_buffer_total += p_buffer_info->size;

                p_buffer_info->handle_id = (uint64_t)OSAL_BUFFER_HANDLE_ID;
                p_buffer_info->buffer_id = p_mngr_control->num_buffers;
                /* PRQA S 0314 1 # No change on the required cast on the system. */
                p_buffer_info->mbuff_obj.p_ptr_self       = (void *)*p_buffer_obj;
                p_buffer_info->mbuff_obj.mmngr_owner_hndl = mngr_handle;
                p_buffer_info->mbuff_obj.p_getCpuPtr      = osal_internal_mem_getcpuptr;
                p_buffer_info->mbuff_obj.p_getConstCpuPtr = osal_internal_mem_getconstcpuptr;
                p_buffer_info->mbuff_obj.p_flush          = osal_internal_mem_flush;
                p_buffer_info->mbuff_obj.p_invalidate     = osal_internal_mem_invalidate;
                p_buffer_info->mbuff_obj.p_getHwAddr      = osal_internal_mem_gethwaddr;
                p_buffer_info->mbuff_obj.p_getSize        = osal_internal_mem_getsize;
                p_buffer_info->mbuff_obj.p_checkBuffer    = osal_internal_mem_checkbuffer;
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
                p_buffer_info->mbuff_obj.p_print          = osal_internal_mem_debugbufferprint;
#else
                p_buffer_info->mbuff_obj.p_print          = NULL;
#endif

                osal_ret = osal_internal_mem_send_monitor_event(&p_mngr_control->inner_mtx,
                                                                OSAL_MMNGR_EVENT_ALLOCATE,
                                                                OSAL_RETURN_OK,
                                                                p_mngr_control,
                                                                &p_buffer_info->mbuff_obj);
            }
            else
            {
                /* Do nothing */
            }   /* end of if(OSAL_RETURN_OK == osal_ret). */

            os_ret = pthread_mutex_unlock(&p_mngr_control->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_alloc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_alloc_pre()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_pre(void * p_ptr_self, size_t bytes, size_t alignment)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    osal_memory_manager_handle_t mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_ptrself(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        mngr_handle = (osal_memory_manager_handle_t)p_ptr_self;
        osal_ret = osal_internal_mem_check_mngrhndl(mngr_handle);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        osal_ret       = osal_internal_mem_check_mngrctrl(p_mngr_control);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == p_mngr_control->p_buffers)
        {
           osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_mem_alloc_config_check(p_mngr_control, bytes, alignment);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_alloc_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_alloc_config_check()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_config_check(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                                 size_t bytes, size_t alignment)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t        osal_ret;
    st_osal_mmngr_config_t max_config;
    size_t                 memory_limit;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_DCFG_GetMaxConfigOfMemory(&max_config, p_mngr_control->region_idx);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MEMORY_MANAGER_CFG_UNLIMITED == p_mngr_control->config.memory_limit)
        {
            memory_limit = max_config.memory_limit;
        }
        else
        {
            memory_limit = p_mngr_control->config.memory_limit;
        }

        if ((0U == bytes) || (memory_limit < bytes) || (0U != (bytes & 0x3FU)))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else if ((0U == alignment) || (memory_limit < alignment) || (64U > alignment) || (0U != (alignment & (alignment -1U))))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_alloc_config_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_find_free_buffer_info()
* XOS3_OSAL_EMCOS_CD_CD_MEM_030
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_010]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_find_free_buffer_info(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                                    st_osal_mem_buf_ptr_self_t **pp_buffer_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret   = OSAL_RETURN_OK;
    size_t          loop_count = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((p_mngr_control->config.max_allowed_allocations > loop_count) && (NULL == *pp_buffer_info))
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (OSAL_BUFFER_HANDLE_ID != p_mngr_control->p_buffers[loop_count].handle_id)
        {
            *pp_buffer_info = &p_mngr_control->p_buffers[loop_count];
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_find_free_buffer_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_alloc_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                         size_t bytes, size_t alignment,
                                                         st_osal_mem_buf_ptr_self_t *p_buffer_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          adjust_alignment;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    adjust_alignment = ((alignment + (size_t)(4096U - 1U)) & ~(4096U - 1U));

    if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == p_mngr_control->config.mode) ||
        (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == p_mngr_control->config.mode))
    {
        p_buffer_info->chunk_size = ((((OSAL_MMNGR_GUARD_PAGE_SIZE + 
                                     (adjust_alignment - 1U)) & ~(adjust_alignment - 1U)) * 2U) + 
                                     ((bytes + (adjust_alignment - 1U)) & ~(adjust_alignment - 1U)));
    }
    else
    {
        p_buffer_info->chunk_size = ((bytes + (adjust_alignment - 1U)) & ~(adjust_alignment - 1U));
    }

    osal_ret = osal_internal_mem_alloc_lib(p_mngr_control, p_buffer_info, bytes, adjust_alignment);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mem_alloc_fill_guard_area(p_mngr_control->config.mode, p_buffer_info);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_alloc_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_alloc_lib()
* XOS3_OSAL_EMCOS_CD_CD_MEM_031
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_003]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_lib (st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                         st_osal_mem_buf_ptr_self_t * p_buffer_info,
                                                         size_t bytes,
                                                         size_t alignment)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uintptr_t       chunk_max;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0326 1 # Required cast on system, not modifiable. */
    chunk_max = (uintptr_t)p_mngr_control->p_virtual_addr + p_mngr_control->size;
    if (0U != p_mngr_control->num_buffers)
    {
        chunk_max -= OSAL_MMNGR_MEM_RESERVE_SIZE;
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 0306 1 #There is no problem because uint8_t* is a type that represents a pointer type as an unsigned integer.. */
    p_buffer_info->p_chunk_va = (uint8_t *)R_MEML_Alloc(&p_mngr_control->meml_handle, p_buffer_info->chunk_size, alignment);

    if (NULL == p_buffer_info->p_chunk_va)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    /* PRQA S 0306 1 # There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer. */
    else if (chunk_max < ((uintptr_t)p_buffer_info->p_chunk_va + p_buffer_info->chunk_size))
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* PRQA S 0306 2 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer. */
        p_buffer_info->chunk_pa = p_mngr_control->physical_addr + 
                                  ((uintptr_t)p_buffer_info->p_chunk_va - 
                                  /* PRQA S 0326 1 # Required cast on system, not modifiable. */
                                  (uintptr_t)p_mngr_control->p_virtual_addr);

        if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == p_mngr_control->config.mode) ||
            (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == p_mngr_control->config.mode))
        {
            p_buffer_info->buffer_pa   = (((p_buffer_info->chunk_pa + OSAL_MMNGR_GUARD_PAGE_SIZE) + (alignment - (size_t)1)) & ~(alignment - (size_t)1));
            /* PRQA S 0306 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
            p_buffer_info->p_buffer_va = (uint8_t*)((uintptr_t)p_buffer_info->p_chunk_va +
                                                    (p_buffer_info->buffer_pa - p_buffer_info->chunk_pa));
            p_buffer_info->size        = bytes;
        }
        else
        {
            p_buffer_info->buffer_pa   = p_buffer_info->chunk_pa;
            p_buffer_info->p_buffer_va = p_buffer_info->p_chunk_va;
            p_buffer_info->size        = bytes;
        }
    }   /* end of if(NULL == p_buffer_info->p_chunk_va). */

    if (OSAL_RETURN_OK != osal_ret)
    {
        if (NULL != p_buffer_info->p_chunk_va)
        {
            /* PRQA S 0306 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
            (void)R_MEML_Dealloc(&p_mngr_control->meml_handle, (uintptr_t)p_buffer_info->p_chunk_va);
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_alloc_lib()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_alloc_fill_guard_area()
* XOS3_OSAL_EMCOS_CD_CD_MEM_032
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_002]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_alloc_fill_guard_area(e_osal_mmngr_allocator_mode_t mode,
                                                                    const st_osal_mem_buf_ptr_self_t * p_buffer_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    void            *p_guard_area;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == mode) || (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == mode))
    {
        /* Set to the head guard page */
        /* PRQA S 0306,0326 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
        p_guard_area = (void *)((uintptr_t)p_buffer_info->p_buffer_va - OSAL_MMNGR_GUARD_PAGE_SIZE);
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        (void)memset(p_guard_area, (int)OSAL_MMNGR_GUARD_PAGE_PATTERN, OSAL_MMNGR_GUARD_PAGE_SIZE);
        bios_cache_purge(p_guard_area, (size_t)OSAL_MMNGR_GUARD_PAGE_SIZE);

        /* Set to the tail guard page */
        /* PRQA S 0306,0326 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
        p_guard_area = (void *)((uintptr_t)p_buffer_info->p_buffer_va + p_buffer_info->size);
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        (void)memset(p_guard_area, (int)OSAL_MMNGR_GUARD_PAGE_PATTERN, OSAL_MMNGR_GUARD_PAGE_SIZE);
        bios_cache_purge(p_guard_area, (size_t)OSAL_MMNGR_GUARD_PAGE_SIZE);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_alloc_fill_guard_area()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_dealloc()
* XOS3_OSAL_EMCOS_CD_CD_MEM_033
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_007]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc(void * p_ptr_self, osal_memory_buffer_handle_t buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                  osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_manager_handle_t     mngr_handle;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    st_osal_mem_mmngr_ptr_self_t     *p_mngr_control;
    st_osal_mem_buf_ptr_self_t const *p_buffer_info;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                              os_ret;
    size_t                           use_size;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_dealloc_pre(p_ptr_self, buffer_obj);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        mngr_handle    = (osal_memory_manager_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer_info  = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (((OSAL_MMNGR_ALLOC_MODE_STACK == p_mngr_control->config.mode) ||
                (OSAL_MMNGR_ALLOC_MODE_STACK_PROT == p_mngr_control->config.mode)) &&
                (p_mngr_control->num_buffers != p_buffer_info->buffer_id))
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mem_send_monitor_event(&p_mngr_control->inner_mtx,
                                                                OSAL_MMNGR_EVENT_DEALLOCATE,
                                                                OSAL_RETURN_OK,
                                                                p_mngr_control,
                                                                buffer_obj);
            }
            else
            {
                /* Do nothing */
            }
            /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
            if (OSAL_RETURN_OK == osal_ret)
            {
                use_size = p_buffer_info->size;
                osal_ret = osal_internal_mem_dealloc_proc(p_mngr_control, buffer_obj);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    p_mngr_control->num_buffers      -= (size_t)1;
                    p_mngr_control->use_buffer_total -= use_size;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&p_mngr_control->inner_mtx);
            if (0 != os_ret)
            {
                if (OSAL_RETURN_OK == osal_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_dealloc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_dealloc_pre()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc_pre(const void * p_ptr_self, osal_memory_buffer_handle_t const buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_manager_handle_t        mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t  *p_mngr_control;
    st_osal_mem_buf_ptr_self_t const    *p_buffer_info;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_ptrself(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        mngr_handle = (osal_memory_manager_handle_t)p_ptr_self;
        if ((NULL == mngr_handle->p_ptr_self) || (NULL == buffer_obj->p_ptr_self))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880,0316 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer_info  = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (OSAL_BUFFER_HANDLE_ID != p_buffer_info->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (gsp_osal_memory_manager[p_mngr_control->region_idx] != buffer_obj->mmngr_owner_hndl)
        {
            osal_ret = OSAL_RETURN_HANDLE_NOT_SUPPORTED;
        }
        else
        {
            /* Do nothing */
        }
        /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret       = osal_internal_mem_check_mngrctrl(p_mngr_control);
        }
        else
        {
            /* Do nothing */
        }
        /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
            if (NULL == p_mngr_control->p_buffers)
            {
               osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_dealloc_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_dealloc_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_dealloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                           osal_memory_buffer_handle_t buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t            osal_ret;
    e_meml_return_t            meml_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    st_osal_mem_buf_ptr_self_t *p_buffer_info;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                        os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because cast from void * to object is allowed. */
    p_buffer_info  = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

    osal_ret = R_OSAL_Internal_TimedlockMutex(&p_buffer_info->inner_mtx, p_buffer_info->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Already deallocated. */
        if (NULL == p_buffer_info->p_chunk_va)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* PRQA S 0306 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
            meml_ret = R_MEML_Dealloc(&p_mngr_control->meml_handle, (uintptr_t)p_buffer_info->p_chunk_va);
            if (MEML_RETURN_OK != meml_ret)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_buffer_info->handle_id          = OSAL_INVALID_DEVICE_HANDLE_ID;
                p_buffer_info->buffer_id          = 0;
                p_buffer_info->size               = 0;
                /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
                p_buffer_info->buffer_pa          = (uintptr_t)NULL;
                /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
                p_buffer_info->chunk_pa           = (uintptr_t)NULL;
                p_buffer_info->chunk_size         = 0; 
                p_buffer_info->p_chunk_va         = NULL;
                p_buffer_info->p_buffer_va        = NULL;

                buffer_obj->mmngr_owner_hndl = NULL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(NULL == p_buffer_info->p_chunk_va). */

        os_ret = pthread_mutex_unlock(&p_buffer_info->inner_mtx);
        if (0 != os_ret)
        {
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_dealloc_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_getconfig()
* XOS3_OSAL_EMCOS_CD_CD_MEM_034
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_012]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getconfig(const void *p_ptr_self, st_osal_mmngr_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_manager_handle_t mngr_handle;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_getconfig_pre(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        mngr_handle    = (osal_memory_manager_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_config->mode                        = p_mngr_control->config.mode;
            p_config->memory_limit                = p_mngr_control->config.memory_limit;
            p_config->max_allowed_allocations     = p_mngr_control->config.max_allowed_allocations;
            p_config->max_registered_monitors_cbs = p_mngr_control->config.max_registered_monitors_cbs;

            os_ret = pthread_mutex_unlock(&p_mngr_control->inner_mtx);
            if (os_ret != 0)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_getconfig()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_getconfig_pre()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getconfig_pre(const void *p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    osal_memory_manager_handle_t        mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t  *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_ptrself(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        mngr_handle = (osal_memory_manager_handle_t)p_ptr_self;
        osal_ret = osal_internal_mem_check_mngrhndl(mngr_handle);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        osal_ret       = osal_internal_mem_check_mngrctrl(p_mngr_control);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_getconfig_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_registermonitor()
* XOS3_OSAL_EMCOS_CD_CD_MEM_035
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_022]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC e_osal_return_t osal_internal_mem_registermonitor(void* p_ptr_self,
                                                              e_osal_mmngr_event_t monitored_event,
                                                              fp_osal_memory_mngr_cb_t monitor_handler,
                                                              void* p_monitor_handler_user_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_manager_handle_t mngr_handle;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_registermonitor_pre(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        mngr_handle    = (osal_memory_manager_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (p_mngr_control->config.max_registered_monitors_cbs == p_mngr_control->num_monitor)
            {
                osal_ret = OSAL_RETURN_PROHIBITED;
            }
            else
            {
                osal_ret = osal_internal_mem_registermonitor_proc(monitored_event,
                                                                  monitor_handler,
                                                                  p_monitor_handler_user_arg,
                                                                  p_mngr_control);
            }

            os_ret = pthread_mutex_unlock(&p_mngr_control->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_registermonitor()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_registermonitor_pre()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_registermonitor_pre(const void *p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    osal_memory_manager_handle_t mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_ptrself(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        mngr_handle = (osal_memory_manager_handle_t)p_ptr_self;
        osal_ret = osal_internal_mem_check_mngrhndl(mngr_handle);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (OSAL_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_mngr_control->p_monitor)
        {
            /* If p_monitor is NULL, the maximum number of registrations is 0 */
            osal_ret = OSAL_RETURN_PROHIBITED;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_registermonitor_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_registermonitor_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_registermonitor_proc(e_osal_mmngr_event_t monitored_event,
                                                                   fp_osal_memory_mngr_cb_t monitor_handler,
                                                                   void* p_monitor_handler_user_arg,
                                                                   st_osal_mem_mmngr_ptr_self_t *p_mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (loop_count = 0; p_mngr_control->num_monitor > loop_count; loop_count++)
    {
        if ((monitored_event == p_mngr_control->p_monitor[loop_count].event_id) &&
            (monitor_handler == p_mngr_control->p_monitor[loop_count].p_handler))
        {
            osal_ret = OSAL_RETURN_PAR;
            break;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_mngr_control->p_monitor[p_mngr_control->num_monitor].event_id  = monitored_event;
        p_mngr_control->p_monitor[p_mngr_control->num_monitor].p_handler = monitor_handler;
        p_mngr_control->p_monitor[p_mngr_control->num_monitor].p_arg     = p_monitor_handler_user_arg;
        p_mngr_control->num_monitor++;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_registermonitor_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_send_monitor_event()
* XOS3_OSAL_EMCOS_CD_CD_MEM_036
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_023]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_send_monitor_event (pthread_mutex_t * p_mutex_id,
                                                                  e_osal_mmngr_event_t event_id,
                                                                  e_osal_return_t error_code,
                                                                  st_osal_mem_mmngr_ptr_self_t const * p_mngr_control,
                                                                  osal_memory_buffer_handle_t buffer_hndl)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret   = OSAL_RETURN_OK;
    size_t          num;
    size_t          loop_count;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* If p_monitor is NULL, the number is 0 */
    num = p_mngr_control->num_monitor;

    os_ret = pthread_mutex_unlock(p_mutex_id);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        for (loop_count = 0; loop_count < num; loop_count++)
        {
            if (event_id == p_mngr_control->p_monitor[loop_count].event_id)
            {
                p_mngr_control->p_monitor[loop_count].p_handler(p_mngr_control->p_monitor[loop_count].p_arg,
                                                                event_id,
                                                                error_code,
                                                                buffer_hndl);
            }
            else
            {
                /* Do nothing */
            }
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(p_mutex_id, (osal_milli_sec_t)OSAL_MEMORY_INNER_TIMEOUT);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_send_monitor_event()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_debugmmngrprint()
* XOS3_OSAL_EMCOS_CD_CD_MEM_037
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_009]
***********************************************************************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
OSAL_STATIC e_osal_return_t osal_internal_mem_debugmmngrprint(const void* p_ptr_self, FILE* p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret             = OSAL_RETURN_OK;
    osal_memory_manager_handle_t mngr_handle          = NULL;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control      = NULL;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret               = 0;
    size_t                       cnt                  = 0;
    char                         bytes_str[20]        = {0};
    char                         memory_limit_str[20] = {0};
    char                         alloc_mode[64]       = {0};

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_ptr_self)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        mngr_handle = (osal_memory_manager_handle_t)p_ptr_self;
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        if (OSAL_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_mngr_control->p_buffers)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            osal_ret = osal_internal_mem_conv_unit_and_str(p_mngr_control->use_buffer_total,
                                                           bytes_str,
                                                           sizeof(bytes_str));
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_MEMORY_HANDLE_ID != p_mngr_control->handle_id). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mem_conv_unit_and_str(p_mngr_control->config.memory_limit,
                                                           memory_limit_str,
                                                           sizeof(memory_limit_str));
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = fprintf(p_output, "Memory Current(Limit):\t\t%s(%s)\n", bytes_str, memory_limit_str);
            if (0 > os_ret)
            {
                osal_ret = OSAL_RETURN_IO;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = fprintf(p_output,
                             "Allocations Current(Limit):\t\t%ld(%ld)\n",
                             p_mngr_control->num_buffers,
                             p_mngr_control->config.max_allowed_allocations);
            if (0 > os_ret)
            {
                osal_ret = OSAL_RETURN_IO;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = fprintf(p_output,
                             "Registered Monitors Current(Limit):\t\t%ld(%ld)\n",
                             p_mngr_control->num_monitor,
                             p_mngr_control->config.max_registered_monitors_cbs);
            if (0 > os_ret)
            {
                osal_ret = OSAL_RETURN_IO;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            switch (p_mngr_control->config.mode)
            {
                case OSAL_MMNGR_ALLOC_MODE_STACK:
                    (void)strncpy(alloc_mode, "Stack allocator without Underflow/Overflow detection", sizeof(alloc_mode));
                    break;
                case OSAL_MMNGR_ALLOC_MODE_STACK_PROT:
                    (void)strncpy(alloc_mode, "Stack allocator with Underflow/Overflow detection", sizeof(alloc_mode));
                    break;
                case OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT:
                    (void)strncpy(alloc_mode, "Free list allocator with Underflow/Overflow detection", sizeof(alloc_mode));
                    break;
                case OSAL_MMNGR_ALLOC_MODE_FREE_LIST:
                    (void)strncpy(alloc_mode, "Free list allocator without Underflow/Overflow detection", sizeof(alloc_mode));
                    break;
                default:
                    (void)strncpy(alloc_mode, "Invalid allocator", sizeof(alloc_mode));
                    break;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = fprintf(p_output, "Allocation Mode:\t\t%s\n", alloc_mode);
            if (0 > os_ret)
            {
                osal_ret = OSAL_RETURN_IO;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = fprintf(p_output, "%s%s%s",
                             "Buffer Handle Value; address                                 ; ",
                             "address (axi domain)                          ; ",
                             "sec ; d ; size               ; permissions\n");
            if (0 > os_ret)
            {
                osal_ret = OSAL_RETURN_IO;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            for (cnt = 0; p_mngr_control->num_buffers > cnt; cnt++)
            {
                if (OSAL_BUFFER_HANDLE_ID == p_mngr_control->p_buffers[cnt].handle_id)
                {
                    os_ret = fprintf(p_output,
                                     "0x%016lX ; 0x%016lX -- 0x%016lX; 0x%016lX -- 0x%016lX (MM) ; ns  ;   ; 0x%016lX ; P:readWrite U:readWrite\n",
                                     (uintptr_t)&p_mngr_control->p_buffers[cnt],
                                     (uintptr_t) p_mngr_control->p_buffers[cnt].p_buffer_va,
                                     ((uintptr_t) (p_mngr_control->p_buffers[cnt].p_buffer_va + 
                                     p_mngr_control->p_buffers[cnt].size)),
                                     p_mngr_control->p_buffers[cnt].buffer_pa,
                                     p_mngr_control->p_buffers[cnt].buffer_pa + 
                                     p_mngr_control->p_buffers[cnt].size,
                                     p_mngr_control->p_buffers[cnt].size);
                    if (0 > os_ret)
                    {
                        osal_ret = OSAL_RETURN_IO;
                    }
                    else
                    {
                        /* Do nothing */
                    }
                }
                else
                {
                    /* Do nothing */
                }   /* end of if(OSAL_BUFFER_HANDLE_ID == p_mngr_control->p_buffers[cnt].handle_id). */
            }   /* end of for(cnt = 0; p_mngr_control->num_buffers > cnt; cnt++). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
    }   /* end of if(NULL == p_ptr_self). */

    return osal_ret;
}
/* PRQA S 2053 1 # Because of the debugging function, it does not change at present */
#endif /* end of #if !definedNDEBUG || definedR_OSAL_ENABLE_DEBUG. */
/***********************************************************************************************************************
* End of function osal_internal_mem_debugmmngrprint()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_conv_unit_and_str()
* XOS3_OSAL_EMCOS_CD_CD_MEM_038
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_006]
***********************************************************************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
OSAL_STATIC e_osal_return_t osal_internal_mem_conv_unit_and_str(size_t size, char * p_output, size_t array_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret    = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MMNGR_GB_UNIT < size)
    {
        os_ret = snprintf(p_output, array_size, "%ld GB", (size / OSAL_MMNGR_GB_UNIT));
        if (0 > os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if (OSAL_MMNGR_MB_UNIT < size)
    {
        os_ret = snprintf(p_output, array_size, "%ld MB", (size / OSAL_MMNGR_MB_UNIT));
        if (0 > os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if (OSAL_MMNGR_KB_UNIT < size)
    {
        os_ret = snprintf(p_output, array_size, "%ld KB", (size / OSAL_MMNGR_KB_UNIT));
        if (0 > os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        os_ret = snprintf(p_output, array_size, "%ldB", size);
        if (0 > os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_MMNGR_GB_UNIT < size). */

    return osal_ret;
}
/* PRQA S 2053 1 # Because of the debugging function, it does not change at present */
#endif /* end of #if !definedNDEBUG || definedR_OSAL_ENABLE_DEBUG. */
/***********************************************************************************************************************
* End of function osal_internal_mem_conv_unit_and_str()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_check_buff_hndl()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This internal function is intended to be used in common. */
OSAL_STATIC e_osal_return_t osal_internal_mem_check_buff_hndl(const void* p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t buffer_obj;
    const st_osal_mem_buf_ptr_self_t  *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_ptr_self)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        if (NULL == buffer_obj->p_ptr_self)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == buffer_obj->mmngr_owner_hndl)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == buffer_obj->mmngr_owner_hndl->p_ptr_self)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (OSAL_BUFFER_HANDLE_ID != p_buffer->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_check_buff_hndl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_getcpuptr()
* XOS3_OSAL_EMCOS_CD_CD_MEM_039
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_014]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC e_osal_return_t osal_internal_mem_getcpuptr(void* p_ptr_self, void** const pp_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t buffer_obj;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    st_osal_mem_buf_ptr_self_t  *p_buffer;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_buff_hndl(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer   = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
            *pp_cpu_ptr = (void*)p_buffer->p_buffer_va;

            os_ret = pthread_mutex_unlock(&p_buffer->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_getcpuptr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_getconstcpuptr()
* XOS3_OSAL_EMCOS_CD_CD_MEM_040
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_013]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC e_osal_return_t osal_internal_mem_getconstcpuptr(void* p_ptr_self, const void** const pp_const_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t buffer_obj;
    st_osal_mem_buf_ptr_self_t  *p_buffer;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_buff_hndl(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer   = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
            *pp_const_cpu_ptr = (void*)p_buffer->p_buffer_va;

            os_ret = pthread_mutex_unlock(&p_buffer->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_getconstcpuptr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_flush()
* XOS3_OSAL_EMCOS_CD_CD_MEM_041
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_011]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC e_osal_return_t osal_internal_mem_flush(void* p_ptr_self, size_t offset, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t buffer_obj;
    st_osal_mem_buf_ptr_self_t  *p_buffer;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_buff_hndl(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer   = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_mem_flush_proc(offset, size, p_buffer);

            os_ret = pthread_mutex_unlock(&p_buffer->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_flush()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_flush_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_flush_proc(size_t offset, size_t size,
                                                         const st_osal_mem_buf_ptr_self_t *p_buffer)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((p_buffer->size < offset) || (0U != (offset & (size_t)0x3F)))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((0U == size) || (p_buffer->size < size) || (0U != (size & (size_t)0x3F)))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* PRQA S 0306,0326 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
        bios_cache_writeback((void *)((uintptr_t)p_buffer->p_buffer_va + offset), size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_flush_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_invalidate()
* XOS3_OSAL_EMCOS_CD_CD_MEM_042
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_017]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC e_osal_return_t osal_internal_mem_invalidate(void* p_ptr_self, size_t offset, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t buffer_obj;
    st_osal_mem_buf_ptr_self_t  *p_buffer;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_buff_hndl(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer   = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_mem_invalidate_proc(offset, size, p_buffer);

            os_ret = pthread_mutex_unlock(&p_buffer->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_invalidate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_invalidate_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_invalidate_proc(size_t offset, size_t size,
                                                              const st_osal_mem_buf_ptr_self_t *p_buffer)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((p_buffer->size < offset) || (0U != (offset & (size_t)0x3F)))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((0U == size) || (p_buffer->size < size) || (0U != (size & (size_t)0x3F)))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* PRQA S 0306,0326 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
        bios_cache_invalidate((void *)((uintptr_t)p_buffer->p_buffer_va + offset), size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_invalidate_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_gethwaddr()
* XOS3_OSAL_EMCOS_CD_CD_MEM_043
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_015]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_gethwaddr(const void* p_ptr_self,
                                                        osal_axi_bus_id_t axi_id,
                                                        uintptr_t* const p_hw_addr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t buffer_obj;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    st_osal_mem_buf_ptr_self_t  *p_buffer;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_buff_hndl(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer   = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Note: If "axi_id" is other than "OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY", IPMMU is not supported, */
            /* so it returns the same address as if "axi_id" is "OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY". */
            if (OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY == axi_id)
            {
                *p_hw_addr = p_buffer->buffer_pa;
            }
            else
            {
                *p_hw_addr = p_buffer->buffer_pa;
            }

            os_ret = pthread_mutex_unlock(&p_buffer->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_gethwaddr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_getsize()
* XOS3_OSAL_EMCOS_CD_CD_MEM_044
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_016]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_getsize(const void* p_ptr_self, size_t* const p_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t buffer_obj;
    st_osal_mem_buf_ptr_self_t  *p_buffer;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_buff_hndl(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_buffer   = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            *p_size = p_buffer->size;

            os_ret = pthread_mutex_unlock(&p_buffer->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_getsize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_checkbuffer()
* XOS3_OSAL_EMCOS_CD_CD_MEM_045
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_004]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_checkbuffer(const void* p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    e_osal_return_t              check_ret       = OSAL_RETURN_OK;
    /* PRQA S 3679 2 # The actual situation is not rewritten, but because it rewrites the pointer,
     it cannot be changed because it becomes a compile error when Const is granted. */
    osal_memory_buffer_handle_t  buffer_obj;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mem_check_buff_hndl(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316,0311 1 # No problem because cast from void * to object is allowed. */
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)buffer_obj->mmngr_owner_hndl->p_ptr_self;

        osal_ret = R_OSAL_Internal_TimedlockMutex(&p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_mem_checkbuffer_proc(p_mngr_control, buffer_obj, &check_ret);
            os_ret = pthread_mutex_unlock(&p_mngr_control->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK != check_ret)
    {
        osal_ret = check_ret;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_checkbuffer()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_checkbuffer_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mem_checkbuffer_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                               osal_memory_buffer_handle_t buffer_obj,
                                                               e_osal_return_t *p_check_ret)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t            osal_ret  = OSAL_RETURN_OK;
    const uint8_t              *check_addr;
    size_t                     loop_count;
    const st_osal_mem_buf_ptr_self_t *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    p_buffer   = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

    if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT != p_mngr_control->config.mode) &&
        (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT != p_mngr_control->config.mode))
    {
        /* Do nothing */
    }
    else
    {
        /* PRQA S 0306 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
        check_addr = (uint8_t *)((uintptr_t)p_buffer->p_buffer_va - (uintptr_t)OSAL_MMNGR_GUARD_PAGE_SIZE);

        for (loop_count = 0; OSAL_MMNGR_GUARD_PAGE_SIZE > loop_count; loop_count++)
        {
            if (OSAL_MMNGR_GUARD_PAGE_PATTERN != check_addr[loop_count])
            {
                osal_ret = osal_internal_mem_send_monitor_event(&p_mngr_control->inner_mtx,
                                                                OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED,
                                                                OSAL_RETURN_OK,
                                                                p_mngr_control,
                                                                buffer_obj);
                *p_check_ret = OSAL_RETURN_MEM;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
        /* PRQA S 0306 1 #  There is no problem because uintptr_t is a type that represents a pointer type as an unsigned integer.  */
        check_addr = (uint8_t *)((uintptr_t)p_buffer->p_buffer_va + (uintptr_t)p_buffer->size);

        for (loop_count = 0; OSAL_MMNGR_GUARD_PAGE_SIZE > loop_count; loop_count++)
        {
            if (OSAL_MMNGR_GUARD_PAGE_PATTERN != check_addr[loop_count])
            {
                osal_ret = osal_internal_mem_send_monitor_event(&p_mngr_control->inner_mtx,
                                                                OSAL_MMNGR_EVENT_OVERFLOW_DETECTED,
                                                                OSAL_RETURN_OK,
                                                                p_mngr_control,
                                                                buffer_obj);
                *p_check_ret = OSAL_RETURN_MEM;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
        /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_MMNGR_ALLOC_MODE_STACK_PROT != p_mngr_control->config.mode) &&
                     (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT != p_mngr_control->config.mode)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mem_checkbuffer_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mem_debugbufferprint()
* XOS3_OSAL_EMCOS_CD_CD_MEM_046
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MEM_008]
***********************************************************************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
OSAL_STATIC e_osal_return_t osal_internal_mem_debugbufferprint(const void* p_ptr_self, FILE* p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret   = OSAL_RETURN_OK;
    osal_memory_buffer_handle_t buffer_obj = NULL;
    st_osal_mem_buf_ptr_self_t  *p_buffer  = NULL;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret     = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_ptr_self)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        buffer_obj = (osal_memory_buffer_handle_t)p_ptr_self;
        if (NULL == buffer_obj->p_ptr_self)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            p_buffer = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;
            os_ret  = fprintf(p_output,
                              "0x%016lX ; 0x%016lX -- 0x%016lX; 0x%016lX -- 0x%016lX (MM) ; ns ;  ; 0x%016lX ; P:rw U:rw\n",
                              (uintptr_t)p_buffer,
                              (uintptr_t)p_buffer->p_buffer_va,
                              (uintptr_t)p_buffer->p_buffer_va + p_buffer->size,
                              p_buffer->buffer_pa,
                              p_buffer->buffer_pa + p_buffer->size,
                              p_buffer->size);
            if (0 > os_ret)
            {
                osal_ret = OSAL_RETURN_IO;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(NULL == buffer_obj->p_ptr_self). */
    }   /* end of if(NULL == p_ptr_self). */

    return osal_ret;
}
/* PRQA S 2053 1 # Because of the debugging function, it does not change at present */
#endif /* end of #if !definedNDEBUG || definedR_OSAL_ENABLE_DEBUG. */
/***********************************************************************************************************************
* End of function osal_internal_mem_debugbufferprint()
***********************************************************************************************************************/

