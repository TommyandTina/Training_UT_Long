/***********************************************************************************************************************
 * Copyright [2020] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
 * @Version      : 0.0.1
 * @Description  : This file is intended to be moved to public in upcomming xOS Releases
 ***********************************************************************************************************************/
/**
 * \addtogroup osal_wrapper
 * @{
 */

#ifndef R_XOS_OSAL_MEMORY_INTERFACE_HPP
#define R_XOS_OSAL_MEMORY_INTERFACE_HPP

#include "rcar-xos/osal/r_osal_def.h"
#include "rcar-xos/osal/r_osal_types.h"
#include "rcar-xos/utils/r_standard_monitor_interface.hpp"
#include "rcar-xos/utils/r_utils.hpp"
#include <iostream>
#include <memory>
#include <sstream>

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include <map>

namespace renesas
{
namespace rcarxos
{
namespace osal
{
template <class Allocator = std::allocator<void>>
using MmngrMonitorInterface =
    renesas::rcarxos::utils::StandardMonitorInterface<e_osal_mmngr_event_t, e_osal_return_t, OSAL_RETURN_OK,
                                                      OSAL_RETURN_PAR, OSAL_RETURN_MEM, OSAL_RETURN_FAIL, Allocator,
                                                      osal_memory_buffer_handle_t>;

enum osal_mmngr_buffer_status
{
    OSAL_BUFFER_ALLOCATED,
    OSAL_BUFFER_DEALLOCATED
};

class BufferInterface
{
public:
    virtual ~BufferInterface() = default;

    virtual e_osal_return_t cpuPtr(void ** const cpu_ptr)            = 0;
    virtual e_osal_return_t constCpuPtr(const void ** const cpu_ptr) = 0;

    virtual e_osal_return_t flush(std::size_t offset, std::size_t size)      = 0;
    virtual e_osal_return_t invalidate(std::size_t offset, std::size_t size) = 0;

    virtual e_osal_return_t hwAddr(osal_axi_bus_id_t axi_id, uintptr_t * const hw_addr) const = 0;
    virtual e_osal_return_t size(std::size_t * const size) const                              = 0;

    virtual e_osal_return_t checkBuffer() const = 0;

    virtual e_osal_return_t allocate(std::size_t bytes, std::size_t alignment) = 0;
    virtual e_osal_return_t deallocate() = 0;

    /**
     * @brief Print detailed information to the outputstream os
     *
     * @param os output stream to write to
     */
    virtual void print(std::ostream & os) const = 0;

    e_osal_return_t printFile(FILE * output) const
    {
        if (nullptr == output)
        {
            return OSAL_RETURN_PAR;
        }
        std::stringstream ss;
        print(ss);

        int err = 0;
        err = std::fputs(ss.str().c_str(), output);
        if( 0 > err )
        {
           return  OSAL_RETURN_IO;
        }

        return OSAL_RETURN_OK;
    }

    virtual osal_memory_buffer_handle_t getOsalHandle() = 0;
    static constexpr BufferInterface *    fromOsalHandle(osal_memory_buffer_handle_t osal_handle)
    {
        return static_cast<BufferInterface *>(static_cast<st_osal_memory_buffer_obj *>(osal_handle)->p_ptr_self);
    }
    virtual osal_mmngr_buffer_status get_buffer_status() = 0;
    virtual void set_buffer_status(osal_mmngr_buffer_status status) = 0;
};

/**
 * @brief Memory Manager Interface
 *
 * @tparam Allocator Allocator Type for allocating internal memory. Default is std::allocator but should not be used in
 * safety critical systems.
 */
template <class Allocator = std::allocator<void>> class MmngrInterface : public MmngrMonitorInterface<Allocator>
{
public:
    /**
     * @brief
     *
     * @param bytes
     * @param alignment
     * @param buffer_obj
     * @return e_osal_return_t
     */
    virtual ~MmngrInterface() = default;
    virtual e_osal_return_t allocate(std::size_t bytes, std::size_t alignment, BufferInterface ** const buffer_obj) = 0;

    virtual e_osal_return_t deallocate(BufferInterface * buffer_obj) = 0;

    virtual e_osal_return_t getConfig(st_osal_mmngr_config_t * const config) const = 0;

    virtual e_osal_return_t registerMonitor(e_osal_mmngr_event_t     monitored_event,
                                            fp_osal_memory_mngr_cb_t monitor_handler,
                                            void *                   monitor_handler_user_arg)             = 0;

    virtual e_osal_return_t reserveResources(osal_memory_region_idx_t region_idx) = 0;

    /**
     * @brief Print detailed information about the current memory to the outputstream os
     *
     * @param os output stream to write to
     */
    virtual void print(std::ostream & os) const = 0;

    virtual e_osal_return_t printFile(FILE * output) const = 0;

    virtual osal_memory_manager_handle_t getOsalHandle() = 0;
    static constexpr MmngrInterface *      fromOsalHandle(osal_memory_manager_handle_t osal_handle)
    {
        return static_cast<MmngrInterface *>(static_cast<MmngrInterface *>(osal_handle)->user_arg);
    }
};

} // namespace osal
} // namespace rcarxos
} // namespace renesas

#endif // R_XOS_OSAL_MEMORY_HPP

/**
 * @}
 */
