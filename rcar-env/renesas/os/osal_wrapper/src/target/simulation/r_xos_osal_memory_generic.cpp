/**
 * \addtogroup osal_wrapper
 * @{
 */

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "target/simulation/r_xos_osal_memory_interface.hpp"
#include "target/simulation/soc_abstraction/soc_init.hpp"
#include "rcar-xos/osal/r_osal_memory.hpp"

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
#include <stdio.h>
#else
#define FILE void
#endif

extern "C"
{

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrGetCpuPtr")
#endif
    e_osal_return_t R_OSAL_MmngrGetCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl, void ** const cpu_ptr)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (nullptr == cpu_ptr)
        {
            return OSAL_RETURN_PAR;
        }
        else
        {
            /* cpu_ptr output parameter is valid */
        }
        if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
        {
            /* Invalid Handle */
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == memory_buffer_obj_hndl->p_getCpuPtr)
        {
            /* CPU PTR cannot be acquired for this buffer obj. If CPU pointer is supported by this object then the const
             * version can always acquired */
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            /* is valid handle */
        }
        return memory_buffer_obj_hndl->p_getCpuPtr(memory_buffer_obj_hndl->p_ptr_self, cpu_ptr);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrGetConstCpuPtr")
#endif
    e_osal_return_t R_OSAL_MmngrGetConstCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                               const void ** const         const_cpu_ptr)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (nullptr == const_cpu_ptr)
        {
            return OSAL_RETURN_PAR;
        }
        else
        {
            /* cpu_ptr output parameter is valid */
        }
        if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
        {
            /* Invalid Handle */
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == memory_buffer_obj_hndl->p_getConstCpuPtr)
        {
            /* CPU PTR cannot be acquired for this buffer obj. If CPU pointer is supported by this object then the const
             * version can always acquired */
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            /* is valid handle */
        }
        return memory_buffer_obj_hndl->p_getConstCpuPtr(memory_buffer_obj_hndl->p_ptr_self, const_cpu_ptr);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrFlush")
#endif
    e_osal_return_t R_OSAL_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset, size_t size)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
        {
            /* Invalid Handle */
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == memory_buffer_obj_hndl->p_flush)
        {
            /* Cache flush operation not supported by this buffer object*/
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            /* is valid handle */
        }
        return memory_buffer_obj_hndl->p_flush(memory_buffer_obj_hndl->p_ptr_self, offset, size);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrInvalidate")
#endif
    e_osal_return_t R_OSAL_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset,
                                           size_t size)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
        {
            /* Invalid Handle */
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == memory_buffer_obj_hndl->p_invalidate)
        {
            /* Cache flush operation not supported by this buffer object*/
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            /* is valid handle */
        }
        return memory_buffer_obj_hndl->p_invalidate(memory_buffer_obj_hndl->p_ptr_self, offset, size);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrGetHwAddr")
#endif
    e_osal_return_t R_OSAL_MmngrGetHwAddr(osal_memory_buffer_handle_t memory_buffer_obj_hndl, osal_axi_bus_id_t axi_id,
                                          uintptr_t * const hw_addr)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
        {
            /* Invalid Handle */
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == memory_buffer_obj_hndl->p_getHwAddr)
        {
            /* get HwAddr not supported by this buffer object*/
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            /* is valid handle */
        }
        return memory_buffer_obj_hndl->p_getHwAddr(memory_buffer_obj_hndl->p_ptr_self, axi_id, hw_addr);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrGetSize")
#endif
    e_osal_return_t R_OSAL_MmngrGetSize(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t * const size)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == memory_buffer_obj_hndl)
        {
            /* Invalid Handle */
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == memory_buffer_obj_hndl->p_getSize)
        {
            /* get HwAddr not supported by this buffer object*/
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            /* is valid handle */
        }
        return memory_buffer_obj_hndl->p_getSize(memory_buffer_obj_hndl->p_ptr_self, size);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrAlloc")
#endif
    e_osal_return_t R_OSAL_MmngrAlloc(osal_memory_manager_handle_t hndl_mmngr, size_t size, size_t alignment,
                                      osal_memory_buffer_handle_t * const buffer_obj)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
        {
            return OSAL_RETURN_HANDLE;
        }
        if (nullptr == hndl_mmngr->p_allocate)
        {
            return OSAL_RETURN_HANDLE_NOT_SUPPORTED;
        }
        else if (NULL == hndl_mmngr->p_ptr_self)
        {
            return OSAL_RETURN_HANDLE;
        }
        else
        {
            /* OK */
        }

        if (nullptr == buffer_obj)
        {
            return OSAL_RETURN_PAR;
        }

        return hndl_mmngr->p_allocate(hndl_mmngr->p_ptr_self, size, alignment, buffer_obj);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrDealloc")
#endif
    e_osal_return_t R_OSAL_MmngrDealloc(osal_memory_manager_handle_t hndl_mmngr, osal_memory_buffer_handle_t buffer_obj)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }
        if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
        {
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == hndl_mmngr->p_deallocate)
        {
            return OSAL_RETURN_HANDLE_NOT_SUPPORTED;
        }
        else
        {
            /* every thing ok */
        }

        if (nullptr == buffer_obj)
        {
            return OSAL_RETURN_HANDLE;
        }
        return hndl_mmngr->p_deallocate(hndl_mmngr->p_ptr_self, buffer_obj);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrInitializeMmngrConfigSt")
#endif
    e_osal_return_t R_OSAL_MmngrInitializeMmngrConfigSt(st_osal_mmngr_config_t * const config)
    {
        if (nullptr == config)
        {
            return OSAL_RETURN_PAR;
        }
        else
        {
            config->max_allowed_allocations     = 0;
            config->max_registered_monitors_cbs = 0;
            config->memory_limit                = 0;
            config->mode                        = OSAL_MMNGR_ALLOC_MODE_INVALID;
        }
        return OSAL_RETURN_OK;
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrGetConfig")
#endif
    e_osal_return_t R_OSAL_MmngrGetConfig(osal_memory_manager_handle_t   hndl_mmngr,
                                          st_osal_mmngr_config_t * const p_config)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr && nullptr == p_config)
        {
            return OSAL_RETURN_PAR;
        }
        else if (nullptr == p_config)
        {
            return OSAL_RETURN_PAR;
        }
        else if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
        {
            return R_OSAL_MmngrGetOsalMaxConfig(p_config);
        }
        else if (nullptr == hndl_mmngr->p_getConfig)
        {
            return OSAL_RETURN_HANDLE_NOT_SUPPORTED;
        }
        else
        {
            /* OK */
        }

        return hndl_mmngr->p_getConfig(hndl_mmngr->p_ptr_self, p_config);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrRegisterMonitor")
#endif
    e_osal_return_t R_OSAL_MmngrRegisterMonitor(osal_memory_manager_handle_t hndl_mmngr,
                                                e_osal_mmngr_event_t         monitored_event,
                                                fp_osal_memory_mngr_cb_t monitor_handler, void * user_arg)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
        {
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == hndl_mmngr->p_registerMonitor)
        {
            return OSAL_RETURN_HANDLE_NOT_SUPPORTED;
        }
        else if ((OSAL_MMNGR_EVENT_INVALID >= monitored_event) || (OSAL_MMNGR_EVENT_ERROR < monitored_event))
        {
            return OSAL_RETURN_PAR;
        }
        else if (nullptr == monitor_handler)
        {
            return OSAL_RETURN_PAR;
        }
        else
        {
            /* OK */
        }
        return hndl_mmngr->p_registerMonitor(hndl_mmngr->p_ptr_self, monitored_event, monitor_handler, user_arg);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrDebugMmngrPrint")
#endif
    e_osal_return_t R_OSAL_MmngrDebugMmngrPrint(osal_memory_manager_handle_t hndl_mmngr, FILE * output)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
        {
            return OSAL_RETURN_HANDLE;
        }
    	else if (nullptr == output)
        {
            return OSAL_RETURN_PAR;
        }
        else if (nullptr == hndl_mmngr->p_print)
        {
            return OSAL_RETURN_HANDLE_NOT_SUPPORTED;
        }
        else
        {
            /* OK */
        }

        return hndl_mmngr->p_print(hndl_mmngr->p_ptr_self, output);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrDebugBufferPrint")
#endif
    e_osal_return_t R_OSAL_MmngrDebugBufferPrint(osal_memory_buffer_handle_t hndl_buffer, FILE * output)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == hndl_buffer)
        {
            return OSAL_RETURN_HANDLE;
        }
        else if (nullptr == output)
        {
            return OSAL_RETURN_PAR;
        }
        else if (nullptr == hndl_buffer->p_print)
        {
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }


        return hndl_buffer->p_print(hndl_buffer->p_ptr_self, output);
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrInitializeMemoryBufferObj")
#endif
    e_osal_return_t R_OSAL_MmngrInitializeMemoryBufferObj(st_osal_memory_buffer_obj * const obj)
    {
        if (nullptr == obj)
        {
            return OSAL_RETURN_PAR;
        }
        obj->p_ptr_self         = nullptr;
        obj->mmngr_owner_hndl = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        obj->p_getCpuPtr        = nullptr;
        obj->p_getConstCpuPtr   = nullptr;
        obj->p_flush            = nullptr;
        obj->p_invalidate       = nullptr;
        obj->p_getHwAddr        = nullptr;
        obj->p_getSize          = nullptr;
        obj->p_checkBuffer      = nullptr;
        obj->p_print = nullptr;
        return OSAL_RETURN_OK;
    }

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrInitializeMemoryManagerObj")
#endif
    e_osal_return_t R_OSAL_MmngrInitializeMemoryManagerObj(st_osal_memory_manager_obj * const obj)
    {
        if (nullptr == obj)
        {
            return OSAL_RETURN_PAR;
        }
        obj->p_ptr_self        = nullptr;
        obj->p_allocate        = nullptr;
        obj->p_deallocate      = nullptr;
        obj->p_getConfig       = nullptr;
        obj->p_registerMonitor = nullptr;
        obj->p_print = nullptr;
        return OSAL_RETURN_OK;
    }
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrCheckBuffer")
#endif
e_osal_return_t R_OSAL_MmngrCheckBuffer(osal_memory_buffer_handle_t hndl_buffer)
{
    if (!r_soc_initialized())
    {
       return OSAL_RETURN_STATE;
    }

    if (OSAL_MEMORY_BUFFER_HANDLE_INVALID == hndl_buffer)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == hndl_buffer->p_checkBuffer)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        /* OK */
    }

    return hndl_buffer->p_checkBuffer(hndl_buffer->p_ptr_self);
    
}
/**
 * @}
 */
