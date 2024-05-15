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
* File Name :    r_osal_res_cfg_memory.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions for Memory Manager
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <string.h>
#include "target/common/r_osal_common.h"
#include "target/common/res_cfg/r_osal_res_cfg_if.h"
#include "target/common/helper/r_osal_helper_if.h"
#include "target/common/res_cfg/r_osal_res_cfg_memory.h"

#if defined(NDEBUG) && !defined(R_OSAL_ENABLE_DEBUG)
/* PRQA S 4601 1 # There is no problem because it is the declaration of macro "FILE" when stdio.h is not used. */
#define FILE void
#endif

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private data types
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_memory_manager_table_t
 * @brief structure to memory manager object table.
 **********************************************************************************************************************/
typedef struct
{
    e_osal_init_status_t            osal_memory_is_opened;
    osal_memory_manager_handle_t    osal_memory_manager;
} st_osal_memory_manager_table_t;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var gsp_osal_memory_manager
 * @brief Handle of memory manager object
***********************************************************************************************************************/
OSAL_STATIC st_osal_memory_manager_table_t *gsp_osal_memory_manager = NULL;

/***********************************************************************************************************************
 * @var gspp_osal_mem_mmngr_ptr_self
 * @brief Pointer Self of memory manager object
***********************************************************************************************************************/
OSAL_STATIC st_osal_mem_mmngr_ptr_self_t **gspp_osal_mem_mmngr_ptr_self = NULL;

/***********************************************************************************************************************
 * @var gs_osal_memory_is_initialized_enu
 * @brief Memory Manager initial state flag.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_osal_memory_is_initialized_enu = OSAL_MANAGER_STATE_UNINITIALIZED;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Check the memory resource configuration.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_check_mem_config(void);

/***********************************************************************************************************************
* @brief            Allocate memory manager object.
* @param[in]        num_of_regions  Num of memory regions.
* @param[out]       pp_mmngr_self   A pointer to the memory manager control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_mmngr_obj(size_t num_of_regions,
                                                              st_osal_mem_mmngr_ptr_self_t **pp_mmngr_self);

/***********************************************************************************************************************
* @brief            Initialize memory manager control table.
* @param[in]        p_mmngr_self    A pointer to the memory manager control table.
* @param[in]        num_of_regions  Num of memory regions.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_init_mmngr_self(st_osal_mem_mmngr_ptr_self_t *p_mmngr_self,
                                                              size_t num_of_regions);

/***********************************************************************************************************************
* @brief            Deinitialize memory manager control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_deinit_mmngr_self(void);

/***********************************************************************************************************************
* @brief            Internal configuration check for memory manager open.
* @param[in]        p_config    A pointer to store the configuration.
* @param[in]        region_idx  Memory region index.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_CONF_UNSUPPORTED
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_config_check(st_osal_mmngr_config_t* p_config,
                                                                osal_memory_region_idx_t region_idx);

/***********************************************************************************************************************
* @brief            Internal configuration check for memory name.
* @param[in]        region_idx  Memory region index.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_name_check(osal_memory_region_idx_t region_idx);

/***********************************************************************************************************************
* @brief            Open memory manager.
* @param[in]        p_usr_config    A pointer to configuration to instantiate the OSAL Memory Manager.
* @param[in]        region_idx      Memory region index.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open(const st_osal_mmngr_config_t * const p_usr_config,
                                                   osal_memory_region_idx_t region_idx);

/***********************************************************************************************************************
* @brief            Open memory manager (mapping process).
* @param[in]        p_mngr_control  A pointer to the memory manager control table.
* @param[in]        p_usr_config    A pointer to configuration to instantiate the OSAL Memory Manager.
* @param[in]        region_idx      Memory region index.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_map(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                       const st_osal_mmngr_config_t * const p_usr_config,
                                                       osal_memory_region_idx_t region_idx);

/***********************************************************************************************************************
* @brief            Open memory manager (allocation process).
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        p_usr_config     A pointer to configuration to instantiate the OSAL Memory Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_malloc(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                          const st_osal_mmngr_config_t * const p_usr_config);

/***********************************************************************************************************************
* @brief            Close memory manager.
* @param[in]        hndl_mmngr       Memory Manager handle.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_close(osal_memory_manager_handle_t hndl_mmngr);

/***********************************************************************************************************************
* @brief            Internal deallocate buffers.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_close_dealloc_buffers(st_osal_mem_mmngr_ptr_self_t *p_mngr_control);

/***********************************************************************************************************************
* @brief            Freeing up system resources in memory close.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_close_system_resources(st_osal_mem_mmngr_ptr_self_t *p_mngr_control);

/***********************************************************************************************************************
* @brief            Allocates memory.
* @param[in]        p_ptr_self   A pointer to Memory Manager handle.
* @param[in]        bytes        Requested size of the memory.
* @param[in]        alignment    Alignment of the memory.
* @param[out]       p_buffer_obj Handle to the allocate buffer object.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_PROHIBITED
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc(void * p_ptr_self,
                                                    size_t bytes,
                                                    size_t alignment,
                                                    osal_memory_buffer_handle_t * const p_buffer_obj);

/***********************************************************************************************************************
* @brief            Pre check of memory allocation.
* @param[in]        p_ptr_self     A pointer to Memory Manager handle.
* @param[in]        bytes          Requested size of the memory.
* @param[in]        alignment      Alignment of the memory.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_pre_check(const void * p_ptr_self,
                                                              size_t bytes,
                                                              size_t alignment);

/***********************************************************************************************************************
* @brief            Pre check of memory allocation(configuration).
* @param[in]        p_ptr_self     A pointer to Memory Manager handle.
* @param[in]        bytes          Requested size of the memory.
* @param[in]        alignment      Alignment of the memory.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_pre_config_check(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                                     size_t bytes,
                                                                     size_t alignment);

/***********************************************************************************************************************
* @brief            Allocates memory process.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        bytes            Requested size of the memory.
* @param[in]        alignment        Alignment of the memory.
* @param[in]        p_buffer_info    A pointer to the Memory Buffer Information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                         size_t bytes,
                                                         size_t alignment,
                                                         st_osal_mem_buf_ptr_self_t *p_buffer_info);

/***********************************************************************************************************************
* @brief            Allocates memory process.
* @param[in/out]    p_mngr_control  A pointer to the memory manager control table.
* @param[in/out]    p_buffer_info   A pointer to the buffer information area.
* @param[in]        bytes           Requested size of the memory.
* @param[in]        alignment       Alignment of the memory which has been adjusted in the system page.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_lib(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                        st_osal_mem_buf_ptr_self_t * p_buffer_info,
                                                        size_t bytes,
                                                        size_t alignment);

/***********************************************************************************************************************
* @brief            Find Memory Buffer Information Area.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[out]       pp_buffer_info   A pointer to store the address of the Memory Buffer Information Area.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_find_free_buffer_info(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                                    st_osal_mem_buf_ptr_self_t **pp_buffer_info);

/***********************************************************************************************************************
* @brief            Fill the guard area with the pattern value.
* @param[in]        mode              Memory Manager allocation mode values.
* @param[in/out]    p_buffer_info     A pointer to the buffer information area.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_fill_guard_area(e_osal_mmngr_allocator_mode_t mode,
                                                                    st_osal_mem_buf_ptr_self_t * p_buffer_info);

/***********************************************************************************************************************
* @brief            Send monitor event.
* @param[in]        p_mutex_id     A pointer to mutex object.
* @param[in]        event_id       Event ID.
* @param[in]        error_code     Error code.
* @param[in]        p_mngr_control A pointer to the memory manager control table.
* @param[in]        buffer_hndl    Memory Buffer handle.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_send_monitor_event(osal_os_mutex_t mutex_id,
                                                                 e_osal_mmngr_event_t event_id,
                                                                 e_osal_return_t error_code,
                                                                 st_osal_mem_mmngr_ptr_self_t const * p_mngr_control,
                                                                 osal_memory_buffer_handle_t buffer_hndl);

/***********************************************************************************************************************
* @brief            Deallocates memory.
* @param[in]        p_ptr_self A pointer to Memory Manager handle.
* @param[in]        buffer_obj A pointer to Memory Buffer Object.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_HANDLE_NOT_SUPPORTED
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_dealloc(void * p_ptr_self,
                                                      osal_memory_buffer_handle_t buffer_obj);

/***********************************************************************************************************************
* @brief            Pre check of memory deallocation.
* @param[in]        p_ptr_self   A pointer to Memory Manager handle.
* @param[in]        buffer_obj   A pointer to Memory Buffer Object.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_HANDLE_NOT_SUPPORTED
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_dealloc_pre_check(const void * p_ptr_self,
                                                                const osal_memory_buffer_handle_t buffer_obj);

/***********************************************************************************************************************
* @brief            Deallocates memory process.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        buffer_obj       A pointer to Memory Buffer Object.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_dealloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                           osal_memory_buffer_handle_t buffer_obj);

/***********************************************************************************************************************
* @brief            Get Configuration Pointer.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[out]       p_config   A pointer to store the configuration.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getconfig(const void *p_ptr_self,
                                                        st_osal_mmngr_config_t *const p_config);

/***********************************************************************************************************************
* @brief            Register the user's callback function.
* @param[in]        p_ptr_self      A pointer to Memory Manager handle.
* @param[in]        monitored_event Event that shall be monitored by the provided monitor function.
* @param[in]        monitor_handler Function to be called by the mmngr for the monitored_event.
* @param[in]        p_user_arg      User argument that will be passed without modification/accessed to the monitor handler function.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_PROHIBITED
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_registermonitor(void* p_ptr_self,
                                                              e_osal_mmngr_event_t monitored_event,
                                                              fp_osal_memory_mngr_cb_t monitor_handler,
                                                              void* p_monitor_handler_user_arg);

/***********************************************************************************************************************
* @brief            Pre check of memory register monitor.
* @param[in]        p_ptr_self   A pointer to Memory Manager handle.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_registermonitor_pre_check(const void* p_ptr_self);

/***********************************************************************************************************************
* @brief            Internal memory manager register monitor process.
* @param[in]        monitored_event              Event that shall be monitored by the provided monitor function.
* @param[in]        monitor_handler              Function to be called by the mmngr for the monitored_event.
* @param[in]        p_monitor_handler_user_arg   A pointer to user argument.
* @param[in,out]    p_mngr_control               A pointer to the memory manager control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_PROHIBITED
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_registermonitor_proc(e_osal_mmngr_event_t monitored_event,
                                                                   fp_osal_memory_mngr_cb_t monitor_handler,
                                                                   void* p_monitor_handler_user_arg,
                                                                   st_osal_mem_mmngr_ptr_self_t *p_mngr_control);

/***********************************************************************************************************************
* @brief            Get CPU Pointer.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[out]       pp_cpu_ptr A pointer to the cpu modifiable memory.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getcpuptr(void* p_ptr_self,
                                                        void** const pp_cpu_ptr);

/***********************************************************************************************************************
* @brief            Get Const CPU Pointer.
* @param[in]        p_ptr_self       A pointer to Memory Buffer handle.
* @param[out]       pp_const_cpu_ptr A pointer to the cpu accessible memory.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getconstcpuptr(void* p_ptr_self,
                                                             const void** const pp_const_cpu_ptr);

/***********************************************************************************************************************
* @brief            Synchronize a sub region of the buffer CPU memory to the HW.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[in]        offset     Offset with the buffer object limits.
* @param[in]        size       Size of the region to be synchronized.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_flush(void* p_ptr_self,
                                                    size_t offset,
                                                    size_t size);

/***********************************************************************************************************************
* @brief            Internal memory manager flush process.
* @param[in]        offset         Offset with the buffer object limits.
* @param[in]        size           Size of the region to be synchronized.
* @param[in]        p_buffer       A pointer to the buffer information area.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_flush_proc(size_t offset,
                                                         size_t size,
                                                         st_osal_mem_buf_ptr_self_t *p_buffer);

/***********************************************************************************************************************
* @brief            Synchronize a sub region of the buffer CPU memory to the HW.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[in]        offset     Offset with the buffer object limits.
* @param[in]        size       Size of the region to be synchronized.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_invalidate(void* p_ptr_self,
                                                         size_t offset,
                                                         size_t size);

/***********************************************************************************************************************
* @brief            Internal memory manager invalidate process.
* @param[in]        offset         Offset with the buffer object limits.
* @param[in]        size           Size of the region to be synchronized.
* @param[in]        p_buffer       A pointer to the buffer information area.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_invalidate_proc(size_t offset,
                                                              size_t size,
                                                              st_osal_mem_buf_ptr_self_t *p_buffer);

/***********************************************************************************************************************
* @brief            Get HW Address.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[in]        axi_id     AXI Bus ID.
* @param[out]       p_hw_addr  A pointer to store the hardware address.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_gethwaddr(const void* p_ptr_self,
                                                        osal_axi_bus_id_t axi_id,
                                                        uintptr_t* const p_hw_addr);

/***********************************************************************************************************************
* @brief            Get allocate buffer size.
* @param[in]        p_ptr_self A pointer to Memory Buffer handle.
* @param[out]       p_size     A pointer to store the buffer size.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getsize(const void* p_ptr_self,
                                                      size_t* const p_size);

/***********************************************************************************************************************
* @brief            Check the current buffer object for underflow or overflow.
* @param[in]        p_ptr_self      A pointer to Memory Buffer handle.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_checkbuffer(const void* p_ptr_self);

/***********************************************************************************************************************
* @brief            Internal memory manager check buffer process.
* @param[in]        p_mngr_control   A pointer to the memory manager control table.
* @param[in]        buffer_obj       A pointer to Memory Buffer Object.
* @param[out]       p_check_ret      A pointer to check result.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_checkbuffer_proc(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                               osal_memory_buffer_handle_t buffer_obj,
                                                               e_osal_return_t *p_check_ret);

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
/***********************************************************************************************************************
* @brief            Output buffer information.
* @param[in]        p_ptr_self  A pointer to Memory Manager handle.
* @param[out]       p_output    Output stream to write information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_IO
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_debugmmngrprint(const void* p_ptr_self,
                                                              FILE* p_output);

/***********************************************************************************************************************
* @brief            Performs unit conversion and returns as a string.
* @param[in]        size       Conversion target size.
* @param[out]       p_output   Converted string.
* @param[in]        array_size Array size.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_convert_unit(size_t size,
                                                           char * p_output,
                                                           size_t array_size);

/***********************************************************************************************************************
* @brief            Output buffer information.
* @param[in]        p_ptr_self  A pointer to Memory Buffer handle.
* @param[out]       p_output    Output stream to write information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_IO
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_debugbufferprint(const void* p_ptr_self,
                                                               FILE* p_output);
#endif  /* end of #if !definedNDEBUG || definedR_OSAL_ENABLE_DEBUG. */

/***********************************************************************************************************************
* @brief            Get the Memory Manager handle status.
* @param[in]        hndl_mmngr  Memory Manager handle.
* @param[out]       p_state     A pointer of handle state.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_cmn_get_handle_state(osal_memory_manager_handle_t hndl_mmngr,
                                                                   e_osal_init_status_t *p_state);

/***********************************************************************************************************************
* @brief            Common handle check for functions associated with memory buffer objects.
* @param[in]        p_ptr_self       A pointer to Memory Buffer Object.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_cmn_check_buffer_handle(const void* p_ptr_self);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_041
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetInitStatus()
***********************************************************************************************************************/
e_osal_init_status_t R_OSAL_RCFG_MmngrGetInitStatus(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    return gs_osal_memory_is_initialized_enu;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MmngrGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_042
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_RCFG_MmngrSetInitStatus(e_osal_init_status_t status)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_memory_is_initialized_enu = status;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MmngrSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_043
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrControlInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret       = OSAL_RETURN_OK;
    const st_meml_version_t         *p_meml_version;
    st_osal_mem_mmngr_ptr_self_t    *p_tmp_self;
    size_t                          num_of_regions = g_osal_memory_region_info_list.num_of_memory_regions;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U != num_of_regions)
    {
        osal_ret = r_osal_rcfg_mmngr_check_mem_config();
        if (OSAL_RETURN_OK == osal_ret)
        {
            p_meml_version = R_MEML_GetVersion();
            if ((0U == p_meml_version->major) && (4U > p_meml_version->minor))
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                osal_ret = r_osal_rcfg_mmngr_alloc_mmngr_obj(num_of_regions, &p_tmp_self);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    osal_ret = r_osal_rcfg_mmngr_init_mmngr_self(p_tmp_self, num_of_regions);
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of if ((0U == p_meml_version->major) && (4U > p_meml_version->minor)). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (0U != num_of_regions). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MmngrControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_044
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrControlDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL != gsp_osal_memory_manager) && (NULL != gspp_osal_mem_mmngr_ptr_self))
    {
        osal_ret = r_osal_rcfg_mmngr_deinit_mmngr_self();
        /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gspp_osal_mem_mmngr_ptr_self);
        R_OSAL_OS_Free((void*)gsp_osal_memory_manager);
        gspp_osal_mem_mmngr_ptr_self = NULL;
        gsp_osal_memory_manager      = NULL;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MmngrControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_045
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrOpen()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrOpen(const st_osal_mmngr_config_t* const p_config,
                                      osal_memory_region_idx_t region_idx,
                                      osal_memory_manager_handle_t* const p_hndl_mmngr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    st_osal_mmngr_config_t  local_config = *p_config;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_MANAGER_STATE_INITIALIZED != gsp_osal_memory_manager[region_idx].osal_memory_is_opened)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_MEMORY_HANDLE_ID == gspp_osal_mem_mmngr_ptr_self[region_idx]->handle_id)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            osal_ret = r_osal_rcfg_mmngr_open_config_check(&local_config, region_idx);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mmngr_open(&local_config, region_idx);
            if (OSAL_RETURN_OK == osal_ret)
            {
                *p_hndl_mmngr = gsp_osal_memory_manager[region_idx].osal_memory_manager;
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
    }   /* end of if (OSAL_MANAGER_STATE_INITIALIZED != gsp_osal_memory_manager[region_idx].osal_memory_is_opened). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MmngrOpen()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_046
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrClose()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrClose(osal_memory_manager_handle_t hndl_mmngr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    e_osal_init_status_t               state;
    const st_osal_mem_mmngr_ptr_self_t *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_get_handle_state(hndl_mmngr, &state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MANAGER_STATE_MEMOPENED != state)
        {
            osal_ret = OSAL_RETURN_STATE;
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *) hndl_mmngr->p_ptr_self;

        /* PRQA S 2995,2991,2880 5 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
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

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mmngr_close(hndl_mmngr);
        if (OSAL_RETURN_OK == osal_ret)
        {
            gsp_osal_memory_manager[p_mngr_control->region_idx].osal_memory_is_opened = OSAL_MANAGER_STATE_INITIALIZED;
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
* End of function R_OSAL_RCFG_MmngrClose()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_047
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrAlloc()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrAlloc(osal_memory_manager_handle_t hndl_mmngr,
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
    if (NULL == hndl_mmngr->p_allocate)
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
* End of function R_OSAL_RCFG_MmngrAlloc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_048
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrDealloc()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrDealloc(osal_memory_manager_handle_t hndl_mmngr,
                                         osal_memory_buffer_handle_t buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == hndl_mmngr->p_deallocate)
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
* End of function R_OSAL_RCFG_MmngrDealloc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_049
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetConfig()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrGetConfig(osal_memory_manager_handle_t hndl_mmngr,
                                           st_osal_mmngr_config_t* const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == hndl_mmngr->p_getConfig)
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
* End of function R_OSAL_RCFG_MmngrGetConfig()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_050
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetOsalMaxConfig()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MmngrGetOsalMaxConfig(osal_memory_region_idx_t region_idx,
                                                  st_osal_mmngr_config_t* const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE >= g_osal_memory_region_info_list.p_info[region_idx].range)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        p_config->mode                        = OSAL_MMNGR_ALLOC_MODE_STACK_PROT;
        p_config->memory_limit                = (g_osal_memory_region_info_list.p_info[region_idx].range - OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE);
        p_config->max_allowed_allocations     = g_osal_memory_max_configration.max_allowed_allocations;
        p_config->max_registered_monitors_cbs = g_osal_memory_max_configration.max_registered_monitors_cbs;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MmngrGetOsalMaxConfig()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_035]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_051
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrRegisterMonitor()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrRegisterMonitor(osal_memory_manager_handle_t hndl_mmngr,
                                                 e_osal_mmngr_event_t monitored_event,
                                                 fp_osal_memory_mngr_cb_t monitor_handler,
                                                 void* p_user_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == hndl_mmngr->p_registerMonitor)
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
* End of function R_OSAL_RCFG_MmngrRegisterMonitor()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_052
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetCpuPtr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrGetCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                           void** const pp_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == memory_buffer_obj_hndl->p_getCpuPtr)
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
* End of function R_OSAL_RCFG_MmngrGetCpuPtr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_053
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetConstCpuPtr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrGetConstCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                                const void** const pp_const_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == memory_buffer_obj_hndl->p_getConstCpuPtr)
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
* End of function R_OSAL_RCFG_MmngrGetConstCpuPtr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_054
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrFlush()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                       size_t offset,
                                       size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == memory_buffer_obj_hndl->p_flush)
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
* End of function R_OSAL_RCFG_MmngrFlush()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_055
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrInvalidate()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                            size_t offset,
                                            size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == memory_buffer_obj_hndl->p_invalidate)
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
* End of function R_OSAL_RCFG_MmngrInvalidate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_056
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetHwAddr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrGetHwAddr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                           osal_axi_bus_id_t axi_id,
                                           uintptr_t * const p_hw_addr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == memory_buffer_obj_hndl->p_getHwAddr)
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
* End of function R_OSAL_RCFG_MmngrGetHwAddr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_041]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_057
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetSize()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrGetSize(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                         size_t* const p_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == memory_buffer_obj_hndl->p_getSize)
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
* End of function R_OSAL_RCFG_MmngrGetSize()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_042]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_058
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrCheckBuffer()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrCheckBuffer(osal_memory_buffer_handle_t memory_buffer_obj_hndl)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == memory_buffer_obj_hndl->p_checkBuffer)
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
* End of function R_OSAL_RCFG_MmngrCheckBuffer()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_043]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_084
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetDefaultIdx()
***********************************************************************************************************************/
osal_memory_region_idx_t R_OSAL_RCFG_MmngrGetDefaultIdx(void)
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
* End of function R_OSAL_RCFG_MmngrGetDefaultIdx()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_047]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_085
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrGetNumOfRegions()
***********************************************************************************************************************/
size_t R_OSAL_RCFG_MmngrGetNumOfRegions(void)
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
* End of function R_OSAL_RCFG_MmngrGetNumOfRegions()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_046]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_059
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrDebugMmngrPrint()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrDebugMmngrPrint(osal_memory_manager_handle_t hndl_mmngr,
                                                 FILE* p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == hndl_mmngr->p_print)
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
* End of function R_OSAL_RCFG_MmngrDebugMmngrPrint()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_044]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_060
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MmngrDebugBufferPrint()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MmngrDebugBufferPrint(osal_memory_buffer_handle_t hndl_buffer,
                                                  FILE* p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == hndl_buffer->p_print)
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
* End of function R_OSAL_RCFG_MmngrDebugBufferPrint()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_045]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_087
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_check_mem_config()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MmngrControlInit() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_check_mem_config(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 1281 2 # QAC false indication (CCT change). */
    if ((1U > g_osal_memory_max_configration.max_allowed_allocations) ||
        ((OSAL_MEMORY_MANAGER_CFG_UNLIMITED - 1U) < g_osal_memory_max_configration.max_allowed_allocations))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    /* PRQA S 1281 1 # QAC false indication (CCT change). */
    else if ((OSAL_MEMORY_MANAGER_CFG_UNLIMITED - 1U) < g_osal_memory_max_configration.max_registered_monitors_cbs)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == g_osal_memory_region_info_list.p_info)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else if (g_osal_memory_region_info_list.num_of_memory_regions <= g_osal_memory_region_info_list.default_memory_region)
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_check_mem_config()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_088
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_alloc_mmngr_obj()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MmngrControlInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_mmngr_obj(size_t num_of_regions,
                                                              st_osal_mem_mmngr_ptr_self_t **pp_mmngr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # There is no problem because it returns a void pointer equivalent to the void pointer returned by malloc of the library function. */
    gsp_osal_memory_manager = (st_osal_memory_manager_table_t *)R_OSAL_OS_Malloc(sizeof(st_osal_memory_manager_table_t) * num_of_regions);
    if (NULL == gsp_osal_memory_manager)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* PRQA S 0316 1 # There is no problem because it returns a void pointer equivalent to the void pointer returned by malloc of the library function. */
        gspp_osal_mem_mmngr_ptr_self = (st_osal_mem_mmngr_ptr_self_t **)R_OSAL_OS_Malloc(sizeof(st_osal_mem_mmngr_ptr_self_t*) * num_of_regions);
        if (NULL == gspp_osal_mem_mmngr_ptr_self)
        {
            /* PRQA S 0314 1 # There is no problem because the void pointer obtained by malloc, calloc, and realloc is just reconverted. */
            R_OSAL_OS_Free((void*)gsp_osal_memory_manager);
            gsp_osal_memory_manager = NULL;
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* PRQA S 0316 1 # There is no problem because it returns a void pointer equivalent to the void pointer returned by malloc of the library function. */
            *pp_mmngr_self = (st_osal_mem_mmngr_ptr_self_t *)R_OSAL_OS_Malloc(sizeof(st_osal_mem_mmngr_ptr_self_t) * num_of_regions);
            if (NULL == *pp_mmngr_self)
            {
                /* PRQA S 0314 2 # There is no problem because the void pointer obtained by malloc, calloc, and realloc is just reconverted. */
                R_OSAL_OS_Free((void*)gspp_osal_mem_mmngr_ptr_self);
                R_OSAL_OS_Free((void*)gsp_osal_memory_manager);
                gspp_osal_mem_mmngr_ptr_self = NULL;
                gsp_osal_memory_manager      = NULL;
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (NULL == gspp_osal_mem_mmngr_ptr_self). */
    }   /* end of if (NULL == gsp_osal_memory_manager). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_alloc_mmngr_obj()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_089
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_init_mmngr_self()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MmngrControlInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_init_mmngr_self(st_osal_mem_mmngr_ptr_self_t *p_mmngr_self,
                                                              size_t num_of_regions)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    e_osal_return_t         api_ret;
    osal_os_mutexattr_t     mutex_attr;
    size_t                  loop;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        for (loop = 0; (loop < num_of_regions) && (OSAL_RETURN_OK == osal_ret); loop++)
        {
            gspp_osal_mem_mmngr_ptr_self[loop] = &p_mmngr_self[loop];

            gsp_osal_memory_manager[loop].osal_memory_manager   = (osal_memory_manager_handle_t)&gspp_osal_mem_mmngr_ptr_self[loop]->mmngr_obj;
            gsp_osal_memory_manager[loop].osal_memory_is_opened = OSAL_MANAGER_STATE_INITIALIZED;

            gspp_osal_mem_mmngr_ptr_self[loop]->inner_timeout    = (osal_milli_sec_t)OSAL_RES_CFG_MMNGR_INNER_TIMEOUT;
            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
            gspp_osal_mem_mmngr_ptr_self[loop]->physical_addr    = (uintptr_t)NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->p_virtual_addr   = NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->p_buffers        = NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->num_buffers      = 0;
            gspp_osal_mem_mmngr_ptr_self[loop]->use_buffer_total = 0;
            gspp_osal_mem_mmngr_ptr_self[loop]->p_monitor        = NULL;
            gspp_osal_mem_mmngr_ptr_self[loop]->num_monitor      = 0;
            gspp_osal_mem_mmngr_ptr_self[loop]->handle_id        = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
            gspp_osal_mem_mmngr_ptr_self[loop]->region_idx       = loop;
            /* PRQA S 0314 1 # No problem because it returns to the structure at the time of setting. */
            gsp_osal_memory_manager[loop].osal_memory_manager->p_ptr_self = (void *)gsp_osal_memory_manager[loop].osal_memory_manager;

            osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &gspp_osal_mem_mmngr_ptr_self[loop]->inner_mtx);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }

        api_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_init_mmngr_self()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_090
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_deinit_mmngr_self()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MmngrControlDeinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_deinit_mmngr_self(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret       = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    size_t          num_of_regions = g_osal_memory_region_info_list.num_of_memory_regions;
    size_t          loop;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gspp_osal_mem_mmngr_ptr_self[0])
    {
        for (loop = 0; loop < num_of_regions; loop++)
        {
            /* If it is not closed, it forces the resource to be released. */
            if (OSAL_MANAGER_STATE_MEMOPENED == gsp_osal_memory_manager[loop].osal_memory_is_opened)
            {
                api_ret = r_osal_rcfg_mmngr_close(gsp_osal_memory_manager[loop].osal_memory_manager);
                if (OSAL_RETURN_OK != api_ret)
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

            if (NULL != gspp_osal_mem_mmngr_ptr_self[loop]->inner_mtx)
            {
                api_ret = R_OSAL_OS_MutexDestroy(gspp_osal_mem_mmngr_ptr_self[loop]->inner_mtx);
                if ((OSAL_RETURN_OK == osal_ret) && (OSAL_RETURN_OK != api_ret))
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    gspp_osal_mem_mmngr_ptr_self[loop]->inner_mtx = NULL;
                }
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of for (loop = 0; loop < num_of_regions; loop++) */

        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gspp_osal_mem_mmngr_ptr_self[0]);
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != gspp_osal_mem_mmngr_ptr_self[0]). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_deinit_mmngr_self()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_091
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_open_config_check()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MmngrOpen() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_config_check(st_osal_mmngr_config_t* p_config,
                                                                osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 1281 1 # QAC false indication (CCT change). */
    if (OSAL_MEMORY_MANAGER_CFG_UNLIMITED == p_config->max_allowed_allocations)
    {
        p_config->max_allowed_allocations = g_osal_memory_max_configration.max_allowed_allocations;
    }

    /* PRQA S 1281 1 # QAC false indication (CCT change). */
    if (OSAL_MEMORY_MANAGER_CFG_UNLIMITED == p_config->max_registered_monitors_cbs)
    {
        p_config->max_registered_monitors_cbs = g_osal_memory_max_configration.max_registered_monitors_cbs;
    }

    if ((1U > p_config->max_allowed_allocations) ||
        (g_osal_memory_max_configration.max_allowed_allocations < p_config->max_allowed_allocations))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else if (g_osal_memory_max_configration.max_registered_monitors_cbs < p_config->max_registered_monitors_cbs)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        osal_ret = r_osal_rcfg_mmngr_open_name_check(region_idx);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 1281 2 # QAC false indication (CCT change). */
        if ((OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE >= g_osal_memory_region_info_list.p_info[region_idx].range) ||
            ((OSAL_MEMORY_MANAGER_CFG_UNLIMITED - 1U) < g_osal_memory_region_info_list.p_info[region_idx].range))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        /* PRQA S 1281 1 # QAC false indication (CCT change). */
        else if ((OSAL_MEMORY_MANAGER_CFG_UNLIMITED != p_config->memory_limit) &&
                 ((g_osal_memory_region_info_list.p_info[region_idx].range - OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE) < p_config->memory_limit))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else if (OSAL_RES_CFG_MMNGR_MEM_4K > p_config->memory_limit)
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
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
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_open_config_check()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_092
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_open_name_check()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_open_config_check() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_name_check(osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t         loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (loop_count = 0; OSAL_MEMORY_NAME_SIZE > loop_count; loop_count++)
    {
        if ('\0' == g_osal_memory_region_info_list.p_info[region_idx].name[loop_count])
        {
            break;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_MEMORY_NAME_SIZE == loop_count)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_open_name_check()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_061
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_open()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open(const st_osal_mmngr_config_t * const p_usr_config,
                                                   osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    e_osal_return_t              api_ret;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
    p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *) gsp_osal_memory_manager[region_idx].osal_memory_manager->p_ptr_self;

    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_rcfg_mmngr_open_map(p_mngr_control, p_usr_config, region_idx);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mmngr_open_malloc(p_mngr_control, p_usr_config);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_MEML_Deinitialize(&p_mngr_control->meml_handle);
                (void)R_OSAL_OS_MmngrUnmap(p_mngr_control);
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
            gsp_osal_memory_manager[region_idx].osal_memory_manager->p_allocate        = r_osal_rcfg_mmngr_alloc;
            gsp_osal_memory_manager[region_idx].osal_memory_manager->p_deallocate      = r_osal_rcfg_mmngr_dealloc;
            gsp_osal_memory_manager[region_idx].osal_memory_manager->p_getConfig       = r_osal_rcfg_mmngr_getconfig;
            gsp_osal_memory_manager[region_idx].osal_memory_manager->p_registerMonitor = r_osal_rcfg_mmngr_registermonitor;
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
            gsp_osal_memory_manager[region_idx].osal_memory_manager->p_print           = r_osal_rcfg_mmngr_debugmmngrprint;
#else
            gsp_osal_memory_manager[region_idx].osal_memory_manager->p_print           = NULL;
#endif

            p_mngr_control->config.mode                        = p_usr_config->mode;
            p_mngr_control->config.memory_limit                = p_mngr_control->size - OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE;
            p_mngr_control->config.max_allowed_allocations     = p_usr_config->max_allowed_allocations;
            p_mngr_control->config.max_registered_monitors_cbs = p_usr_config->max_registered_monitors_cbs;
            p_mngr_control->handle_id                          = (uint64_t)OSAL_RES_CFG_MEMORY_HANDLE_ID;

            gsp_osal_memory_manager[region_idx].osal_memory_is_opened = OSAL_MANAGER_STATE_MEMOPENED;
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        api_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_open()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_048]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_062
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_open_map()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_map(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                       const st_osal_mmngr_config_t * const p_usr_config,
                                                       osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    e_meml_return_t     meml_ret;
    e_meml_alloc_mode_t meml_mode;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MmngrMap(p_mngr_control, p_usr_config, region_idx);
    if (OSAL_RETURN_OK == osal_ret)
    {
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

        /* PRQA S 0326 3 # Cast between a pointer to void and uintptr_t is possible. */
        meml_ret = R_MEML_Initialize(&p_mngr_control->meml_handle,
                                     (uintptr_t)p_mngr_control->physical_addr,
                                     (uintptr_t)p_mngr_control->p_virtual_addr,
                                     p_mngr_control->size,
                                     meml_mode);
        if (MEML_RETURN_OK != meml_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)R_OSAL_OS_MmngrUnmap(p_mngr_control);
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_open_map()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_049]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_063
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_open_malloc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_open_malloc(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                          const st_osal_mmngr_config_t * const p_usr_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    e_osal_return_t     api_ret;
    size_t              loop_count    = 0;
    size_t              destroy_count = 0;
    osal_os_mutexattr_t mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (0U != p_usr_config->max_registered_monitors_cbs)
        {
            /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
            p_mngr_control->p_monitor = (st_osal_mem_monitor_desc_t*)R_OSAL_OS_Malloc(sizeof(st_osal_mem_monitor_desc_t) * p_usr_config->max_registered_monitors_cbs);
            if (NULL == p_mngr_control->p_monitor)
            {
                osal_ret = OSAL_RETURN_MEM;
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)R_OSAL_OS_MutexDeinitAttr(mutex_attr);
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
            /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
            p_mngr_control->p_buffers = (st_osal_mem_buf_ptr_self_t*)R_OSAL_OS_Malloc(sizeof(st_osal_mem_buf_ptr_self_t) * p_usr_config->max_allowed_allocations);
            if (NULL == p_mngr_control->p_buffers)
            {
                osal_ret = OSAL_RETURN_MEM;
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                R_OSAL_OS_Free((void*)p_mngr_control->p_monitor);
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)R_OSAL_OS_MutexDeinitAttr(mutex_attr);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */
    }   /* end of if (OSAL_RETURN_OK != osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        while ((p_usr_config->max_allowed_allocations > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            api_ret = R_OSAL_OS_MutexCreate(mutex_attr, &p_mngr_control->p_buffers[loop_count].inner_mtx);
            if (OSAL_RETURN_OK == api_ret)
            {
                p_mngr_control->p_buffers[loop_count].inner_timeout = (osal_milli_sec_t)OSAL_RES_CFG_MMNGR_INNER_TIMEOUT;
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
                (void)R_OSAL_OS_MutexDestroy(p_mngr_control->p_buffers[destroy_count].inner_mtx);

                destroy_count++;
            }
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)R_OSAL_OS_MutexDeinitAttr(mutex_attr);
            /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
            R_OSAL_OS_Free((void*)p_mngr_control->p_monitor);
            R_OSAL_OS_Free((void*)p_mngr_control->p_buffers);
        }
        else
        {
            api_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
            if (OSAL_RETURN_OK != api_ret)
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
* End of function r_osal_rcfg_mmngr_open_malloc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_050]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_064
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_close()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_close(osal_memory_manager_handle_t hndl_mmngr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    e_osal_return_t              api_ret;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
    p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)hndl_mmngr->p_ptr_self;

    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (0U != p_mngr_control->num_buffers)
        {
            osal_ret = r_osal_rcfg_mmngr_close_dealloc_buffers(p_mngr_control);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mmngr_close_system_resources(p_mngr_control);
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret) */

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            R_OSAL_OS_Free((void*)p_mngr_control->p_buffers);
            if (NULL != p_mngr_control->p_monitor)
            {
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                R_OSAL_OS_Free((void*)p_mngr_control->p_monitor);
            }
            else
            {
                /* Do nothing */
            }

            p_mngr_control->handle_id        = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
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

        api_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_close()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_051]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_093
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_close_dealloc_buffers()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_close() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_close_dealloc_buffers(st_osal_mem_mmngr_ptr_self_t *p_mngr_control)
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

            osal_ret = r_osal_rcfg_mmngr_dealloc_proc(p_mngr_control, mngr_buffer_hndl);
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
    }   /* end of for(buff_count = ((int64_t)p_mngr_control->num_buffers - (int64_t)1); 0 <= buff_count; buff_count--). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_close_dealloc_buffers()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_051]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_094
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_close_system_resources()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_close() in order to improve the result of
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_close_system_resources(st_osal_mem_mmngr_ptr_self_t *p_mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_meml_return_t meml_ret;
    size_t          loop_count = 0;

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
        osal_ret = R_OSAL_OS_MmngrUnmap(p_mngr_control);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        while ((p_mngr_control->config.max_allowed_allocations > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = R_OSAL_OS_MutexDestroy(p_mngr_control->p_buffers[loop_count].inner_mtx);
            if (OSAL_RETURN_OK != osal_ret)
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_close_system_resources()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_051]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_065
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_alloc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc(void * p_ptr_self,
                                                    size_t bytes,
                                                    size_t alignment,
                                                    osal_memory_buffer_handle_t * const p_buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    e_osal_return_t              api_ret;
    osal_memory_manager_handle_t mngr_handle;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control;
    st_osal_mem_buf_ptr_self_t   *p_buffer_info   = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_alloc_pre_check(p_ptr_self, bytes, alignment);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        mngr_handle    = (osal_memory_manager_handle_t)p_ptr_self;
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
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
                osal_ret = r_osal_rcfg_mmngr_find_free_buffer_info(p_mngr_control, &p_buffer_info);
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
                /* PRQA S 0314 1 # No problem because it returns to the structure at the time of setting. */
                p_buffer_info->mbuff_obj.p_ptr_self       = (void *)*p_buffer_obj;
                p_buffer_info->mbuff_obj.mmngr_owner_hndl = mngr_handle;

                osal_ret = r_osal_rcfg_mmngr_alloc_proc(p_mngr_control, bytes, alignment, p_buffer_info);
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
            }   /* end of if (OSAL_RETURN_OK == osal_ret) */

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_mngr_control->num_buffers      += (size_t)1;
                p_mngr_control->use_buffer_total += p_buffer_info->size;

                p_buffer_info->handle_id = (uint64_t)OSAL_RES_CFG_BUFFER_HANDLE_ID;
                p_buffer_info->buffer_id = p_mngr_control->num_buffers;
                p_buffer_info->mbuff_obj.p_getCpuPtr      = r_osal_rcfg_mmngr_getcpuptr;
                p_buffer_info->mbuff_obj.p_getConstCpuPtr = r_osal_rcfg_mmngr_getconstcpuptr;
                p_buffer_info->mbuff_obj.p_flush          = r_osal_rcfg_mmngr_flush;
                p_buffer_info->mbuff_obj.p_invalidate     = r_osal_rcfg_mmngr_invalidate;
                p_buffer_info->mbuff_obj.p_getHwAddr      = r_osal_rcfg_mmngr_gethwaddr;
                p_buffer_info->mbuff_obj.p_getSize        = r_osal_rcfg_mmngr_getsize;
                p_buffer_info->mbuff_obj.p_checkBuffer    = r_osal_rcfg_mmngr_checkbuffer;
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
                p_buffer_info->mbuff_obj.p_print          = r_osal_rcfg_mmngr_debugbufferprint;
#else
                p_buffer_info->mbuff_obj.p_print          = NULL;
#endif

                osal_ret = r_osal_rcfg_mmngr_send_monitor_event(p_mngr_control->inner_mtx,
                                                                OSAL_MMNGR_EVENT_ALLOCATE,
                                                                OSAL_RETURN_OK,
                                                                p_mngr_control,
                                                                &p_buffer_info->mbuff_obj);
            }
            else
            {
                /* Do nothing */
            }   /* end of if(OSAL_RETURN_OK == osal_ret). */

            api_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_alloc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_052]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_095
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_alloc_pre_check()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_alloc() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_pre_check(const void * p_ptr_self,
                                                              size_t bytes,
                                                              size_t alignment)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                          osal_ret;
    struct st_osal_memory_manager_obj const  *p_mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t       *p_mngr_control;
    e_osal_init_status_t                     state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316,0311 1 # No problem because it returns to the structure at the time of setting. */
    osal_ret = r_osal_rcfg_mmngr_cmn_get_handle_state((osal_memory_manager_handle_t)p_ptr_self, &state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MANAGER_STATE_MEMOPENED != state)
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_handle = (struct st_osal_memory_manager_obj const *)p_ptr_self;
        if (NULL == p_mngr_handle->p_ptr_self)
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
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)p_mngr_handle->p_ptr_self;

        /* PRQA S 2995,2991,2880 5 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_mngr_control->p_buffers)
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

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mmngr_alloc_pre_config_check(p_mngr_control, bytes, alignment);
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_alloc_pre_check()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_052]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_096
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_alloc_pre_config_check()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_alloc_pre_check() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_pre_config_check(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                                     size_t bytes,
                                                                     size_t alignment)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((0U == bytes) || (p_mngr_control->config.memory_limit < bytes) || (0U != (bytes & 0x3FU)))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else if ((0U == alignment) || (p_mngr_control->config.memory_limit < alignment) || (64U > alignment) || (0U != (alignment & (alignment -1U))))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_alloc_pre_config_check()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_052]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_097
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_alloc_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_alloc() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                         size_t bytes,
                                                         size_t alignment,
                                                         st_osal_mem_buf_ptr_self_t *p_buffer_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    long            page_size;  /* PRQA S 5209 # System is requesting a long, so it cannot be changed. */
    size_t          adjust_alignment;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    page_size = R_OSAL_OS_GetSysPageSize();

    adjust_alignment = ((alignment + ((size_t)page_size - 1U)) & ~((size_t)page_size - 1U));

    if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == p_mngr_control->config.mode) ||
        (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == p_mngr_control->config.mode))
    {
        p_buffer_info->chunk_size = ((((OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE + 
                                     (adjust_alignment - 1U)) & ~(adjust_alignment - 1U)) * 2U) + 
                                     ((bytes + (adjust_alignment - 1U)) & ~(adjust_alignment - 1U)));
    }
    else
    {
        p_buffer_info->chunk_size = ((bytes + (adjust_alignment - 1U)) & ~(adjust_alignment - 1U));
    }

    osal_ret = r_osal_rcfg_mmngr_alloc_lib(p_mngr_control, p_buffer_info, bytes, adjust_alignment);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mmngr_alloc_fill_guard_area(p_mngr_control->config.mode, p_buffer_info);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_alloc_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_052]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_066
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_alloc_lib()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_lib(st_osal_mem_mmngr_ptr_self_t * p_mngr_control,
                                                        st_osal_mem_buf_ptr_self_t * p_buffer_info,
                                                        size_t bytes,
                                                        size_t alignment)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uintptr_t       chunk_max;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
    chunk_max = (uintptr_t)p_mngr_control->p_virtual_addr + p_mngr_control->size;
    if (0U != p_mngr_control->num_buffers)
    {
        chunk_max -= OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE;
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 0306 1 # Cast between a pointer to uint8_t * and uintptr_t is possible. */
    p_buffer_info->p_chunk_va = (uint8_t *)R_MEML_Alloc(&p_mngr_control->meml_handle, p_buffer_info->chunk_size, alignment);
    if (NULL == p_buffer_info->p_chunk_va)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    /* PRQA S 0306 1 # Cast between a pointer to uint8_t * and uintptr_t is possible. */
    else if (chunk_max < ((uintptr_t)p_buffer_info->p_chunk_va + p_buffer_info->chunk_size))
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* PRQA S 0326,0306 3 # Cast between a pointer to void and uintptr_t is possible. */
        p_buffer_info->chunk_pa = p_mngr_control->physical_addr + 
                                  ((uintptr_t)p_buffer_info->p_chunk_va - 
                                  (uintptr_t)p_mngr_control->p_virtual_addr);

        if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == p_mngr_control->config.mode) ||
            (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == p_mngr_control->config.mode))
        {
            p_buffer_info->buffer_pa   = (((p_buffer_info->chunk_pa + OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE) + (alignment - (size_t)1)) & ~(alignment - (size_t)1));
            /* PRQA S 0306 1 # Cast between a pointer to uint8_t * and uintptr_t is possible. */
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
            /* PRQA S 0306 1 # Cast between a pointer to uint8_t * and uintptr_t is possible. */
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
* End of function r_osal_rcfg_mmngr_alloc_lib()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_054]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_067
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_find_free_buffer_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_find_free_buffer_info(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
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
        /* PRQA S 2995,2991 1 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_BUFFER_HANDLE_ID != p_mngr_control->p_buffers[loop_count].handle_id)
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
* End of function r_osal_rcfg_mmngr_find_free_buffer_info()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_055]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_068
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_alloc_fill_guard_area()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_alloc_fill_guard_area(e_osal_mmngr_allocator_mode_t mode,
                                                                    st_osal_mem_buf_ptr_self_t * p_buffer_info)
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
        /* PRQA S 0326,0306 1 # Cast between a pointer to void and uintptr_t is possible. */
        p_guard_area = (void *)((uintptr_t)p_buffer_info->p_buffer_va - OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);

        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        (void)memset(p_guard_area, (int)OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN, OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);

        /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
        osal_ret = R_OSAL_OS_MmngrFlush(&p_buffer_info->mbuff_obj, (uintptr_t)p_guard_area, 0, OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
            osal_ret = R_OSAL_OS_MmngrInvalidate(&p_buffer_info->mbuff_obj, (uintptr_t)p_guard_area, 0, OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0326,0306 1 # Cast between a pointer to void and uintptr_t is possible. */
            p_guard_area = (void *)((uintptr_t)p_buffer_info->p_buffer_va + p_buffer_info->size);

            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            (void)memset(p_guard_area, (int)OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN, OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);

            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
            osal_ret = R_OSAL_OS_MmngrFlush(&p_buffer_info->mbuff_obj, (uintptr_t)p_guard_area, 0, OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);
            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                osal_ret = R_OSAL_OS_MmngrInvalidate(&p_buffer_info->mbuff_obj, (uintptr_t)p_guard_area, 0, OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);
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
    }
    else
    {
        /* Do nothing */
    }   /* end of if((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == mode) || (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == mode)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_alloc_fill_guard_area()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_053]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_069
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_send_monitor_event()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_send_monitor_event(osal_os_mutex_t mutex_id,
                                                                 e_osal_mmngr_event_t event_id,
                                                                 e_osal_return_t error_code,
                                                                 st_osal_mem_mmngr_ptr_self_t const * p_mngr_control,
                                                                 osal_memory_buffer_handle_t buffer_hndl)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          num;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num = p_mngr_control->num_monitor;

    osal_ret = R_OSAL_OS_MutexUnlock(mutex_id);
    if (OSAL_RETURN_OK != osal_ret)
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
    }   /* end of if (OSAL_RETURN_OK != osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(mutex_id, (osal_milli_sec_t)OSAL_RES_CFG_MMNGR_INNER_TIMEOUT);
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
* End of function r_osal_rcfg_mmngr_send_monitor_event()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_067]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_070
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_dealloc()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_dealloc(void * p_ptr_self,
                                                      osal_memory_buffer_handle_t buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                         osal_ret;
    e_osal_return_t                         api_ret;
    struct st_osal_memory_manager_obj const *p_mngr_handle;
    st_osal_mem_mmngr_ptr_self_t            *p_mngr_control;
    st_osal_mem_buf_ptr_self_t const        *p_buffer_info;
    size_t                                  use_size;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_dealloc_pre_check(p_ptr_self, buffer_obj);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 3 # No problem because it returns to the structure at the time of setting. */
        p_mngr_handle  = (struct st_osal_memory_manager_obj const *)p_ptr_self;
        p_buffer_info  = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)p_mngr_handle->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
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
                osal_ret = r_osal_rcfg_mmngr_send_monitor_event(p_mngr_control->inner_mtx,
                                                                OSAL_MMNGR_EVENT_DEALLOCATE,
                                                                OSAL_RETURN_OK,
                                                                p_mngr_control,
                                                                buffer_obj);
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                use_size = p_buffer_info->size;

                osal_ret = r_osal_rcfg_mmngr_dealloc_proc(p_mngr_control, buffer_obj);
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

            api_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_dealloc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_056]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_098
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_dealloc_pre_check()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_dealloc() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
/* PRQA S 3673 2 # From the concept of handles, const is unnecessary. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_dealloc_pre_check(const void * p_ptr_self,
                                                                const osal_memory_buffer_handle_t buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                         osal_ret;
    struct st_osal_memory_manager_obj const *p_mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t      *p_mngr_control;
    st_osal_mem_buf_ptr_self_t const        *p_buffer_info;
    e_osal_init_status_t                    state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316,0311 1 # No problem because it returns to the structure at the time of setting. */
    osal_ret = r_osal_rcfg_mmngr_cmn_get_handle_state((osal_memory_manager_handle_t)p_ptr_self, &state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MANAGER_STATE_MEMOPENED != state)
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_handle = (struct st_osal_memory_manager_obj const *)p_ptr_self;
        if ((NULL == p_mngr_handle->p_ptr_self) || (NULL == buffer_obj->p_ptr_self))
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
    }   /* end of if (OSAL_RETURN_OK != osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_buffer_info  = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

        /* PRQA S 2995,2991,2880 8 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_BUFFER_HANDLE_ID != p_buffer_info->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
            p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)p_mngr_handle->p_ptr_self;

            if (gsp_osal_memory_manager[p_mngr_control->region_idx].osal_memory_manager != buffer_obj->mmngr_owner_hndl)
            {
                osal_ret = OSAL_RETURN_HANDLE_NOT_SUPPORTED;
            }
            else
            {
                /* Do nothing */
            }
        }

        /* PRQA S 2996,2992,2880 4 # QAC false indication (CCT change). */
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2995,2991 1 # QAC false indication (CCT change). */
            if (OSAL_RES_CFG_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else if (NULL == p_mngr_control->p_buffers)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                osal_ret = OSAL_RETURN_OK;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_dealloc_pre_check()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_056]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_071
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_dealloc_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_dealloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                           osal_memory_buffer_handle_t buffer_obj)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t            osal_ret;
    e_osal_return_t            api_ret;
    e_meml_return_t            meml_ret;
    st_osal_mem_buf_ptr_self_t *p_buffer_info;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
    p_buffer_info  = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;

    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer_info->inner_mtx, p_buffer_info->inner_timeout);
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
            /* PRQA S 0306 1 # Cast between a pointer to uint8_t* and uintptr_t is possible. */
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
                p_buffer_info->handle_id     = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                p_buffer_info->buffer_id     = 0;
                p_buffer_info->size          = 0;
                /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                p_buffer_info->buffer_pa     = (uintptr_t)NULL;
                /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                p_buffer_info->chunk_pa      = (uintptr_t)NULL;
                p_buffer_info->chunk_size    = 0;
                p_buffer_info->p_chunk_va    = NULL;
                p_buffer_info->p_buffer_va   = NULL;

                buffer_obj->mmngr_owner_hndl = NULL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(NULL == p_buffer_info->p_chunk_va). */

        api_ret = R_OSAL_OS_MutexUnlock(p_buffer_info->inner_mtx);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_dealloc_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_057]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_072
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_getconfig()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getconfig(const void *p_ptr_self,
                                                        st_osal_mmngr_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                          osal_ret;
    struct st_osal_memory_manager_obj const  *p_mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t       *p_mngr_control;
    e_osal_init_status_t                     state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316,0311 1 # No problem because it returns to the structure at the time of setting. */
    osal_ret = r_osal_rcfg_mmngr_cmn_get_handle_state((osal_memory_manager_handle_t)p_ptr_self, &state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MANAGER_STATE_MEMOPENED != state)
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_handle = (struct st_osal_memory_manager_obj const*)p_ptr_self;
        if (NULL == p_mngr_handle->p_ptr_self)
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
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)p_mngr_handle->p_ptr_self;

        /* PRQA S 2995,2991 1 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
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

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
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

            osal_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_getconfig()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_058]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_073
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_registermonitor()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_registermonitor(void* p_ptr_self,
                                                              e_osal_mmngr_event_t monitored_event,
                                                              fp_osal_memory_mngr_cb_t monitor_handler,
                                                              void* p_monitor_handler_user_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                          osal_ret;
    e_osal_return_t                          api_ret;
    struct st_osal_memory_manager_obj const  *p_mngr_handle;
    st_osal_mem_mmngr_ptr_self_t             *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_registermonitor_pre_check(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        p_mngr_handle  = (struct st_osal_memory_manager_obj const*)p_ptr_self;
        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)p_mngr_handle->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_rcfg_mmngr_registermonitor_proc(monitored_event,
                                                              monitor_handler,
                                                              p_monitor_handler_user_arg,
                                                              p_mngr_control);

            api_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_registermonitor()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_059]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_099
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_registermonitor_pre_check()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_registermonitor() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_registermonitor_pre_check(const void* p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                          osal_ret;
    struct st_osal_memory_manager_obj const  *p_mngr_handle;
    const st_osal_mem_mmngr_ptr_self_t       *p_mngr_control;
    e_osal_init_status_t                     state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316,0311 1 # No problem because it returns to the structure at the time of setting. */
    osal_ret = r_osal_rcfg_mmngr_cmn_get_handle_state((osal_memory_manager_handle_t)p_ptr_self, &state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MANAGER_STATE_MEMOPENED != state)
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_handle = (struct st_osal_memory_manager_obj const*)p_ptr_self;
        if (NULL == p_mngr_handle->p_ptr_self)
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
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)p_mngr_handle->p_ptr_self;

        /* PRQA S 2995,2991,2880 5 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
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
* End of function r_osal_rcfg_mmngr_registermonitor_pre_check()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_059]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_100
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_registermonitor_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_registermonitor() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_registermonitor_proc(e_osal_mmngr_event_t monitored_event,
                                                                   fp_osal_memory_mngr_cb_t monitor_handler,
                                                                   void* p_monitor_handler_user_arg,
                                                                   st_osal_mem_mmngr_ptr_self_t *p_mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (p_mngr_control->config.max_registered_monitors_cbs == p_mngr_control->num_monitor)
    {
        osal_ret = OSAL_RETURN_PROHIBITED;
    }
    else
    {
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
    }   /* end of if (p_mngr_control->config.max_registered_monitors_cbs == p_mngr_control->num_monitor) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_registermonitor_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_059]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_074
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_getcpuptr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getcpuptr(void* p_ptr_self,
                                                        void** const pp_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                        osal_ret;
    struct st_osal_memory_buffer_obj const *p_buffer_obj;
    const st_osal_mem_buf_ptr_self_t       *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_check_buffer_handle(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        p_buffer_obj = (struct st_osal_memory_buffer_obj const *)p_ptr_self;
        p_buffer     = (st_osal_mem_buf_ptr_self_t *)p_buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            *pp_cpu_ptr = (void*)p_buffer->p_buffer_va;

            osal_ret = R_OSAL_OS_MutexUnlock(p_buffer->inner_mtx);
            if (OSAL_RETURN_OK != osal_ret)
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
* End of function r_osal_rcfg_mmngr_getcpuptr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_060]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_075
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_getconstcpuptr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getconstcpuptr(void* p_ptr_self,
                                                             const void** const pp_const_cpu_ptr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                        osal_ret;
    struct st_osal_memory_buffer_obj const *p_buffer_obj;
    const st_osal_mem_buf_ptr_self_t       *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_check_buffer_handle(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        p_buffer_obj = (struct st_osal_memory_buffer_obj const *)p_ptr_self;
        p_buffer     = (st_osal_mem_buf_ptr_self_t *)p_buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            *pp_const_cpu_ptr = (void*)p_buffer->p_buffer_va;

            osal_ret = R_OSAL_OS_MutexUnlock(p_buffer->inner_mtx);
            if (OSAL_RETURN_OK != osal_ret)
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
* End of function r_osal_rcfg_mmngr_getconstcpuptr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_061]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_076
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_flush()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_flush(void* p_ptr_self,
                                                    size_t offset,
                                                    size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                        osal_ret;
    e_osal_return_t                        api_ret;
    struct st_osal_memory_buffer_obj const *p_buffer_obj;
    st_osal_mem_buf_ptr_self_t             *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_check_buffer_handle(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        p_buffer_obj = (struct st_osal_memory_buffer_obj const *)p_ptr_self;
        p_buffer     = (st_osal_mem_buf_ptr_self_t *)p_buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_rcfg_mmngr_flush_proc(offset, size, p_buffer);

            api_ret = R_OSAL_OS_MutexUnlock(p_buffer->inner_mtx);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_flush()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_062]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_flush_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_flush() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_flush_proc(size_t offset,
                                                         size_t size,
                                                         st_osal_mem_buf_ptr_self_t *p_buffer)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

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
        /* PRQA S 0306 1 # Cast between a pointer to uint8_t and uintptr_t is possible. */
        osal_ret = R_OSAL_OS_MmngrFlush(&p_buffer->mbuff_obj, (uintptr_t)p_buffer->p_buffer_va, offset, size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_flush_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_062]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_077
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_invalidate()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_invalidate(void* p_ptr_self,
                                                         size_t offset,
                                                         size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                        osal_ret;
    e_osal_return_t                        api_ret;
    struct st_osal_memory_buffer_obj const *p_buffer_obj;
    st_osal_mem_buf_ptr_self_t             *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_check_buffer_handle(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        p_buffer_obj = (struct st_osal_memory_buffer_obj const *)p_ptr_self;
        p_buffer     = (st_osal_mem_buf_ptr_self_t *)p_buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_rcfg_mmngr_invalidate_proc(offset, size, p_buffer);

            api_ret = R_OSAL_OS_MutexUnlock(p_buffer->inner_mtx);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_rcfg_mmngr_invalidate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_063]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_invalidate_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_invalidate() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_invalidate_proc(size_t offset,
                                                              size_t size,
                                                              st_osal_mem_buf_ptr_self_t *p_buffer)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

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
        /* PRQA S 0306 1 # Cast between a pointer to uint8_t and uintptr_t is possible. */
        osal_ret = R_OSAL_OS_MmngrInvalidate(&p_buffer->mbuff_obj, (uintptr_t)p_buffer->p_buffer_va, offset, size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_invalidate_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_063]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_078
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_gethwaddr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_gethwaddr(const void* p_ptr_self,
                                                        osal_axi_bus_id_t axi_id,
                                                        uintptr_t* const p_hw_addr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                        osal_ret;
    struct st_osal_memory_buffer_obj const *p_buffer_obj;
    const st_osal_mem_buf_ptr_self_t       *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)axi_id;   /* unused */

    osal_ret = r_osal_rcfg_mmngr_cmn_check_buffer_handle(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        p_buffer_obj = (struct st_osal_memory_buffer_obj const*)p_ptr_self;
        p_buffer     = (const st_osal_mem_buf_ptr_self_t *)p_buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            *p_hw_addr = p_buffer->buffer_pa;

            osal_ret = R_OSAL_OS_MutexUnlock(p_buffer->inner_mtx);
            if (OSAL_RETURN_OK != osal_ret)
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
* End of function r_osal_rcfg_mmngr_gethwaddr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_064]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_079
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_getsize()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_getsize(const void* p_ptr_self,
                                                      size_t* const p_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                        osal_ret;
    struct st_osal_memory_buffer_obj const *p_buffer_obj;
    const st_osal_mem_buf_ptr_self_t       *p_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_check_buffer_handle(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        p_buffer_obj = (struct st_osal_memory_buffer_obj const*)p_ptr_self;
        p_buffer     = (const st_osal_mem_buf_ptr_self_t *)p_buffer_obj->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer->inner_mtx, p_buffer->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            *p_size = p_buffer->size;

            osal_ret = R_OSAL_OS_MutexUnlock(p_buffer->inner_mtx);
            if (OSAL_RETURN_OK != osal_ret)
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
* End of function r_osal_rcfg_mmngr_getsize()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_065]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_080
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_checkbuffer()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_checkbuffer(const void* p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    e_osal_return_t                    api_ret;
    e_osal_return_t                    check_ret       = OSAL_RETURN_OK;
    osal_memory_buffer_handle_t        buffer_obj;
    const st_osal_mem_mmngr_ptr_self_t *p_mngr_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_check_buffer_handle(p_ptr_self);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0311 3 # No problem because it is used for arguments without const. */
        /* PRQA S 0316 2 # No problem because it returns to the structure at the time of setting. */
        buffer_obj     = (const osal_memory_buffer_handle_t)p_ptr_self;
        p_mngr_control = (const st_osal_mem_mmngr_ptr_self_t *)buffer_obj->mmngr_owner_hndl->p_ptr_self;

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_rcfg_mmngr_checkbuffer_proc(p_mngr_control, buffer_obj, &check_ret);

            api_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
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
* End of function r_osal_rcfg_mmngr_checkbuffer()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_066]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_checkbuffer_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mmngr_checkbuffer() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_checkbuffer_proc(const st_osal_mem_mmngr_ptr_self_t *p_mngr_control,
                                                               osal_memory_buffer_handle_t buffer_obj,
                                                               e_osal_return_t *p_check_ret)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                  osal_ret     = OSAL_RETURN_OK;
    const st_osal_mem_buf_ptr_self_t *p_buffer;
    const uint8_t                    *p_check_addr;
    size_t                           loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT != p_mngr_control->config.mode) &&
        (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT != p_mngr_control->config.mode))
    {
        /* Do nothing */
    }
    else
    {
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_buffer     = (st_osal_mem_buf_ptr_self_t *)buffer_obj->p_ptr_self;
        /* PRQA S 0306 1 # Cast between a pointer to uint8_t* and uintptr_t is possible. */
        p_check_addr = (uint8_t *)((uintptr_t)p_buffer->p_buffer_va - (uintptr_t)OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE);

        for (loop_count = 0; OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE > loop_count; loop_count++)
        {
            if (OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN != p_check_addr[loop_count])
            {
                osal_ret = r_osal_rcfg_mmngr_send_monitor_event(p_mngr_control->inner_mtx,
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
        /* PRQA S 0306 1 # Cast between a pointer to uint8_t* and uintptr_t is possible. */
        p_check_addr = (uint8_t *)((uintptr_t)p_buffer->p_buffer_va + (uintptr_t)p_buffer->size);

        for (loop_count = 0; OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE > loop_count; loop_count++)
        {
            if (OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN != p_check_addr[loop_count])
            {
                osal_ret = r_osal_rcfg_mmngr_send_monitor_event(p_mngr_control->inner_mtx,
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
    }   /* end of if((OSAL_MMNGR_ALLOC_MODE_STACK_PROT != p_mngr_control->config.mode) &&
                     (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT != p_mngr_control->config.mode)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_checkbuffer_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_066]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_081
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_debugmmngrprint()
***********************************************************************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_debugmmngrprint(const void* p_ptr_self,
                                                              FILE* p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret             = OSAL_RETURN_OK;
    e_osal_return_t              api_ret;
    osal_memory_manager_handle_t mngr_handle          = NULL;
    st_osal_mem_mmngr_ptr_self_t *p_mngr_control      = NULL;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                          os_ret               = 0;
    size_t                       cnt                  = 0;
    char                         bytes_str[20]        = {0};
    char                         memory_limit_str[20] = {0};
    char                         alloc_mode[64]       = {0};
    e_osal_init_status_t         state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mmngr_cmn_get_handle_state((osal_memory_manager_handle_t)p_ptr_self, &state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_MANAGER_STATE_MEMOPENED != state)
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        mngr_handle = (osal_memory_manager_handle_t)p_ptr_self;

        p_mngr_control = (st_osal_mem_mmngr_ptr_self_t *)mngr_handle->p_ptr_self;
        if (OSAL_RES_CFG_MEMORY_HANDLE_ID != p_mngr_control->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_mngr_control->p_buffers)
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
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mngr_control->inner_mtx, p_mngr_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_rcfg_mmngr_convert_unit(p_mngr_control->use_buffer_total,
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

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_mmngr_convert_unit(p_mngr_control->config.memory_limit,
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
            }   /* end of if (OSAL_RETURN_OK == osal_ret) */

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
                    if (OSAL_RES_CFG_BUFFER_HANDLE_ID == p_mngr_control->p_buffers[cnt].handle_id)
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
                    }   /* end of if (OSAL_RES_CFG_BUFFER_HANDLE_ID == p_mngr_control->p_buffers[cnt].handle_id) */
                }   /* end of for (cnt = 0; p_mngr_control->num_buffers > cnt; cnt++) */
            }
            else
            {
                /* Do nothing */
            }   /* end of if (OSAL_RETURN_OK == osal_ret) */

            api_ret = R_OSAL_OS_MutexUnlock(p_mngr_control->inner_mtx);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
#endif /* end of #if !definedNDEBUG || definedR_OSAL_ENABLE_DEBUG. */
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_debugmmngrprint()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_068]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_082
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_convert_unit()
***********************************************************************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_convert_unit(size_t size,
                                                           char * p_output,
                                                           size_t array_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret  = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_RES_CFG_MMNGR_GB_UNIT < size)
    {
        os_ret = snprintf(p_output, array_size, "%ld GB", (size / OSAL_RES_CFG_MMNGR_GB_UNIT));
        if (0 > os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if (OSAL_RES_CFG_MMNGR_MB_UNIT < size)
    {
        os_ret = snprintf(p_output, array_size, "%ld MB", (size / OSAL_RES_CFG_MMNGR_MB_UNIT));
        if (0 > os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if (OSAL_RES_CFG_MMNGR_KB_UNIT < size)
    {
        os_ret = snprintf(p_output, array_size, "%ld KB", (size / OSAL_RES_CFG_MMNGR_KB_UNIT));
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
        os_ret = snprintf(p_output, array_size, "%ld B", size);
        if (0 > os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RES_CFG_MMNGR_GB_UNIT < size). */

    return osal_ret;
}
#endif /* end of #if !definedNDEBUG || definedR_OSAL_ENABLE_DEBUG. */
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_convert_unit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_070]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_083
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_debugbufferprint()
***********************************************************************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_debugbufferprint(const void* p_ptr_self,
                                                               FILE* p_output)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret   = OSAL_RETURN_OK;
    e_osal_return_t             api_ret;
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

            osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_buffer->inner_mtx, p_buffer->inner_timeout);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                os_ret   = fprintf(p_output,
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

                api_ret = R_OSAL_OS_MutexUnlock(p_buffer->inner_mtx);
                if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of if (OSAL_RETURN_OK != osal_ret). */
        }   /* end of if(NULL == buffer_obj->p_ptr_self). */
    }   /* end of if(NULL == p_ptr_self). */

    return osal_ret;
}
#endif /* end of #if !definedNDEBUG || definedR_OSAL_ENABLE_DEBUG. */
/***********************************************************************************************************************
* End of function r_osal_rcfg_mmngr_debugbufferprint()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_069]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_086
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_cmn_get_handle_state()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_cmn_get_handle_state(osal_memory_manager_handle_t hndl_mmngr,
                                                                   e_osal_init_status_t *p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret       = OSAL_RETURN_OK;
    size_t          num_of_regions = g_osal_memory_region_info_list.num_of_memory_regions;
    size_t          loop;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (loop = 0U; loop < num_of_regions; loop++)
    {
        if (hndl_mmngr == gsp_osal_memory_manager[loop].osal_memory_manager)
        {
            *p_state = gsp_osal_memory_manager[loop].osal_memory_is_opened;
            break;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (loop == num_of_regions)
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
* End of function r_osal_rcfg_mmngr_cmn_get_handle_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_071]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MEM_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mmngr_cmn_check_buffer_handle()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mmngr_cmn_check_buffer_handle(const void* p_ptr_self)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                        osal_ret = OSAL_RETURN_OK;
    struct st_osal_memory_buffer_obj const *p_buffer_obj;
    const st_osal_mem_buf_ptr_self_t       *p_buffer;

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
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_buffer_obj = (struct st_osal_memory_buffer_obj const *)p_ptr_self;
        if (NULL == p_buffer_obj->p_ptr_self)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_buffer_obj->mmngr_owner_hndl)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_buffer_obj->mmngr_owner_hndl->p_ptr_self)
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
        /* PRQA S 0316 1 # No problem because it returns to the structure at the time of setting. */
        p_buffer = (const st_osal_mem_buf_ptr_self_t *)p_buffer_obj->p_ptr_self;

        /* PRQA S 2995,2991 1 # QAC false indication (CCT change). */
        if (OSAL_RES_CFG_BUFFER_HANDLE_ID != p_buffer->handle_id)
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
* End of function r_osal_rcfg_mmngr_cmn_check_buffer_handle()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MEM_072]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
