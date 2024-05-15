/**
 * \addtogroup osal_wrapper
 * @{
 */

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory.hpp"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "rcar-xos/utils/r_allocator_algorithm.hpp"
#include "rcar-xos/utils/r_cb_wrapper.hpp"
#include "rcar-xos/utils/r_standard_monitor_interface.hpp"
#include "rcar-xos/utils/r_utils.hpp"
#include "rcar-xos/vmsav8/r_vmsav8.hpp"
#include "target/simulation/memory_access_router.hpp"
#include "target/simulation/r_xos_osal_memory_interface.hpp"
#include "target/simulation/soc_abstraction/register_interface.hpp"
#include "target/simulation/soc_abstraction/soc_init.hpp"
#include "target/simulation/r_osal_common.hpp"
extern "C" {
#include "rcar-xos/memory_allocator/r_meml.h"
}
#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>
#include <string.h>
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>
#include <windows.h>
#else   //assume posix
#include <stdlib.h>
#include <unistd.h>
#endif

//#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)  /* will be optimized away in release anyway */
#include <cmath>
#include <ios>
#include <limits>
#include <utility>
//#endif

using namespace renesas::rcarxos::osal;
using namespace renesas::rcarxos::utils::literals;
using namespace renesas::rcarxos::utils::allocators;

#define OSAL_MEMORY_HANDLE_ID           (0x4f53414c4d4e4752UL) /* OSALMNGR */
#define OSAL_MEMORY_HANDLE_ID_INVALID   (0)
#define OSAL_MMNGR_GUARD_PAGE_SIZE      (4096U)
#define OSAL_MMNGR_GUARD_PAGE_PATTERN   (0xAAU)
#define OSAL_MMNGR_MEMORY_REGION_SIZE   (0x80000000U)
#define OSAL_MMNGR_MEMORY_RESERVE_SIZE  (2U * 1024U * 1024U)
#define OSAL_MEMORY_DEFAULT_REGION_IDX  (0U)

std::multimap<osal_memory_region_idx_t, std::unique_ptr<st_osal_memory_handle>> g_osal_mmngr;
std::vector<st_osal_hw_end_addr_t> g_list_hw_end_addr;

std::multimap<osal_memory_region_idx_t, std::unique_ptr<st_osal_memory_handle>>::iterator search_mmngr_pos(osal_memory_region_idx_t idx)
{
    std::multimap<osal_memory_region_idx_t, std::unique_ptr<st_osal_memory_handle>>::iterator mmngr_pos;
    for (mmngr_pos = g_osal_mmngr.begin(); mmngr_pos != g_osal_mmngr.end(); mmngr_pos++)
    {
        if (mmngr_pos->first == idx && mmngr_pos->second->status == OSAL_MMNGR_MANAGER_OPENED)
        {
            break;
        }
    }

    return mmngr_pos;
}

template <class Allocator = std::allocator<std::uint8_t>> class OsalMmngr : public MmngrInterface<Allocator>
{
    /* glue class for simulators to access the memory managed by this MMNGR */
    class FastMemoryAccessor : public RegisterInterfaceAxi
    {
    public:
        explicit FastMemoryAccessor(OsalMmngr & parent) : m_parent(parent)
        {
        }

        bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t byte) override
        {
#ifdef HAVE_EASY_PROFILER
            EASY_FUNCTION(profiler::colors::Blue800);
#endif
            return writeRegInternal<uint8_t>(axi_id, physAddr, byte);
        }

        bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t word) override
        {
#ifdef HAVE_EASY_PROFILER
            EASY_FUNCTION(profiler::colors::Blue800);
#endif
            return writeRegInternal<uint16_t>(axi_id, physAddr, word);
        }

        bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t dword) override
        {
#ifdef HAVE_EASY_PROFILER
            EASY_FUNCTION(profiler::colors::Blue800);
#endif
            return writeRegInternal<uint32_t>(axi_id, physAddr, dword);
        }

        bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t qword) override
        {
#ifdef HAVE_EASY_PROFILER
            EASY_FUNCTION(profiler::colors::Blue800);
#endif
            return writeRegInternal<uint64_t>(axi_id, physAddr, qword);
        }

        bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t & val) override
        {
            return readRegInternal(axi_id, physAddr, val);
        }

        bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t & val) override
        {
            return readRegInternal(axi_id, physAddr, val);
        }

        bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t & val) override
        {
            return readRegInternal(axi_id, physAddr, val);
        }

        bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t & val) override
        {
            return readRegInternal(axi_id, physAddr, val);
        }

        bool getVirtAddr(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uintptr_t size, void** pp_cpuPtr) override
        {
#ifdef HAVE_EASY_PROFILER
            EASY_FUNCTION(profiler::colors::Blue800);
#endif
            MemoryBufferObj * bufferObj = nullptr;
            if (false == m_parent.m_translationTable.getValueFromAddr(physAddr, bufferObj))
            {
                return false;
            }

            const void *    vcpu_ptr = nullptr;
            void *          cpu_ptr  = nullptr;
            std::uintptr_t  hw_addr  = 0;
            std::size_t     hw_size  = 0;
            e_osal_return_t ret      = bufferObj->constCpuPtr(&vcpu_ptr); /* base address */
            if (OSAL_RETURN_OK != ret)
            {
                return false;
            }

            ret = bufferObj->hwAddr(axi_id, &hw_addr); /* base address */
            if (OSAL_RETURN_OK != ret)
            {
                return false;
            }

            ret = bufferObj->size(&hw_size);
            if (OSAL_RETURN_OK != ret)
            {
                return false;
            }

            if ((physAddr < hw_addr) || ((hw_addr + (uintptr_t)hw_size) < (physAddr + size)))
            {
                return false;
            }

            /* add offset to cpu_ptr based on the physAddr */
            cpu_ptr =
                reinterpret_cast<void *>(reinterpret_cast<const std::uintptr_t>(vcpu_ptr) + (physAddr - hw_addr));

            /* set value */
            *pp_cpuPtr = cpu_ptr;
            return true;
        }

        virtual uintptr_t startHwAddr() const override
        {
            return m_parent.getAllocationInterface()->managedAddressRange().first;
        }

        virtual uintptr_t endHwAddr() const override
        {
            return m_parent.getAllocationInterface()->managedAddressRange().second;
        }

    private:
        /* avoid code duplication */
        template <class T> inline bool writeRegInternal(osal_axi_bus_id_t axi_id, uintptr_t physAddr, T value)
        {
#ifdef HAVE_EASY_PROFILER
            EASY_FUNCTION(profiler::colors::Blue800);
#endif
            MemoryBufferObj * bufferObj = nullptr;
            if (false == m_parent.m_translationTable.getValueFromAddr(physAddr, bufferObj))
            {
                return false;
            }
            void *          vcpu_ptr = nullptr;
            T *             cpu_ptr  = nullptr;
            std::uintptr_t  hw_addr  = 0;
            e_osal_return_t ret      = bufferObj->cpuPtr(&vcpu_ptr); /* base address */
            if (OSAL_RETURN_OK != ret)
            {
                return false;
            }
            ret = bufferObj->hwAddr(axi_id, &hw_addr); /* base address */
            if (OSAL_RETURN_OK != ret)
            {
                return false;
            }
            if (physAddr < hw_addr)
            {
                return false;
            }
            /* add offset to cpu_ptr based on the physAddr */
            cpu_ptr = reinterpret_cast<T *>(reinterpret_cast<std::uintptr_t>(vcpu_ptr) + (physAddr - hw_addr));
            /* write value */
            *cpu_ptr = value;

            return true;
        }

        /* avoid code duplication */
        template <class T> inline bool readRegInternal(osal_axi_bus_id_t axi_id, uintptr_t physAddr, T & value)
        {
#ifdef HAVE_EASY_PROFILER
            EASY_FUNCTION(profiler::colors::Blue800);
#endif
            MemoryBufferObj * bufferObj = nullptr;
            if (false == m_parent.m_translationTable.getValueFromAddr(physAddr, bufferObj))
            {
                return false;
            }
            const void *    vcpu_ptr = nullptr;
            const T *       cpu_ptr  = nullptr;
            std::uintptr_t  hw_addr  = 0;
            e_osal_return_t ret      = bufferObj->constCpuPtr(&vcpu_ptr); /* base address */
            if (OSAL_RETURN_OK != ret)
            {
                return false;
            }
            ret = bufferObj->hwAddr(axi_id, &hw_addr); /* base address */
            if (OSAL_RETURN_OK != ret)
            {
                return false;
            }
            if (physAddr < hw_addr)
            {
                return false;
            }
            /* add offset to cpu_ptr based on the physAddr */
            cpu_ptr =
                reinterpret_cast<const T *>(reinterpret_cast<const std::uintptr_t>(vcpu_ptr) + (physAddr - hw_addr));
            /* read value */
            value = *cpu_ptr;
            return true;
        }

        OsalMmngr & m_parent;
    };

public:
    class MemoryBufferObj : public BufferInterface
    {
    public:
        static e_osal_return_t cpuPtr_cb(void* p_ptr_self, void ** const cpu_ptr);
        static e_osal_return_t constCpuPtr_cb(void* p_ptr_self, const void ** const cpu_ptr);
        static e_osal_return_t flush_cb(void * p_ptr_self, size_t offset, size_t size);
        static e_osal_return_t invalidate_cb(void * p_ptr_self, size_t offset, size_t size);
        static e_osal_return_t hwAddr_cb(const void * p_ptr_self, osal_axi_bus_id_t axi_id, uintptr_t * const p_hw_addr);
        static e_osal_return_t size_cb(const void * p_ptr_self, size_t * const p_size);
        static e_osal_return_t checkBuffer_cb(const void * p_ptr_self);
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
        static e_osal_return_t printFile_cb(const void * p_ptr_self, FILE * p_output);
#endif
        explicit MemoryBufferObj(OsalMmngr & parent) : m_parent(parent), m_chunkVa(nullptr), m_bufferVa(nullptr)
        {
            using namespace renesas::rcarxos;
            R_OSAL_MmngrInitializeMemoryBufferObj(&m_bufferObjStruct);
            m_bufferObjStruct.p_ptr_self         = this;
            m_bufferObjStruct.mmngr_owner_hndl = m_parent.getOsalHandle();
            m_bufferObjStruct.p_getCpuPtr = cpuPtr_cb;
            m_bufferObjStruct.p_getConstCpuPtr = constCpuPtr_cb;
            m_bufferObjStruct.p_flush = flush_cb;
            m_bufferObjStruct.p_invalidate = invalidate_cb;
            m_bufferObjStruct.p_getHwAddr = hwAddr_cb;
            m_bufferObjStruct.p_getSize = size_cb;

            m_bufferObjStruct.p_checkBuffer = checkBuffer_cb;

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
            m_bufferObjStruct.p_print = printFile_cb;
#else
            m_bufferObjStruct.p_print = nullptr;
#endif
            m_hwMemoryBlockInfo.start_addr = 0;
            m_hwMemoryBlockInfo.size       = 0;
        }

        e_osal_return_t allocate(std::size_t bytes, std::size_t alignment)
        {
            st_osal_mmngr_config_t  conf;
            std::uintptr_t          chunk_va = (uintptr_t)NULL;
            std::size_t             chunk_size = bytes;
            std::uintptr_t          buffer_va;
            std::uintptr_t          buffer_offset;
            std::uint8_t*           prot_addr;

            if (nullptr == m_parent.getAllocationInterface())
            {
                return OSAL_RETURN_DEV;
            }
            if (OSAL_RETURN_OK != m_parent.getConfig(&conf))
            {
                return OSAL_RETURN_FAIL;
            }
            if (!m_parent.getAllocationInterface()->allocateBlock(alignment, bytes, m_hwMemoryBlockInfo))
            {
                return OSAL_RETURN_MEM;
            }
            if (!m_parent.m_translationTable.addEntry(m_hwMemoryBlockInfo.start_addr, m_hwMemoryBlockInfo.size, this))
            {
                m_parent.getAllocationInterface()->deallocateBlock(m_hwMemoryBlockInfo);
                return OSAL_RETURN_MEM;
            }

            if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == conf.mode) ||
                (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == conf.mode))
            {
                /* Top and bottom GUARD_PAGE */
                chunk_size += (OSAL_MMNGR_GUARD_PAGE_SIZE * 2U);
            }
            chunk_size += alignment;

            /* HW Address ensured, now allocate memory block from MEML */
            chunk_va = R_MEML_Alloc(&m_parent.m_meml_handle, chunk_size, alignment);
            if ((std::uintptr_t)nullptr == chunk_va)
            {
                m_parent.m_translationTable.removeEntry(m_hwMemoryBlockInfo.start_addr, m_hwMemoryBlockInfo.size);
                m_parent.getAllocationInterface()->deallocateBlock(m_hwMemoryBlockInfo);
                m_hwMemoryBlockInfo.start_addr = 0;
                m_hwMemoryBlockInfo.size       = 0;
                return OSAL_RETURN_MEM;
            }

            buffer_va = chunk_va;

            if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == conf.mode) ||
                (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == conf.mode))
            {
                /* Top and bottom GUARD_PAGE */
                buffer_va += OSAL_MMNGR_GUARD_PAGE_SIZE;
            }

            buffer_offset = ((buffer_va + alignment - 1U) & ~(alignment - 1U)) - buffer_va;
            buffer_va     += buffer_offset;

            if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == conf.mode) ||
                (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == conf.mode))
            {
                prot_addr = (uint8_t *)chunk_va;
                memset((void *)prot_addr, OSAL_MMNGR_GUARD_PAGE_PATTERN, buffer_va - chunk_va);

                prot_addr = (uint8_t *)(buffer_va + bytes);
                memset((void *)prot_addr, OSAL_MMNGR_GUARD_PAGE_PATTERN, (chunk_va + chunk_size) - (buffer_va + bytes));
            }

            m_chunkVa           = (void *)chunk_va;
            m_bufferVa          = (void *)buffer_va;
            m_chunkSize         = chunk_size;
            m_userBufferSize    = bytes;

            return OSAL_RETURN_OK;
        }

        e_osal_return_t map(std::uintptr_t hw_addr, std::size_t bytes, std::size_t cpu_alignment = 4_KB)
        {
            if (nullptr == m_parent.getAllocationInterface())
            {
                return OSAL_RETURN_DEV;
            }
            if (!m_parent.getAllocationInterface()->mapBlock(hw_addr, bytes, m_hwMemoryBlockInfo))
            {
                return OSAL_RETURN_MEM;
            }
            if (!m_parent.m_translationTable.addEntry(m_hwMemoryBlockInfo.start_addr, m_hwMemoryBlockInfo.size, this))
            {
                m_parent.getAllocationInterface()->deallocateBlock(m_hwMemoryBlockInfo);
                return OSAL_RETURN_MEM;
            }
            if (cpu_alignment < 64 || cpu_alignment % 64 != 0 )
            {
                m_parent.m_translationTable.removeEntry(m_hwMemoryBlockInfo.start_addr, m_hwMemoryBlockInfo.size);
                m_parent.getAllocationInterface()->deallocateBlock(m_hwMemoryBlockInfo);
                return OSAL_RETURN_CONF;
            }
            

            /* HW Address ensured, now allocate memory block from MEML */
            m_chunkVa = (void *)R_MEML_Alloc(&m_parent.m_meml_handle, bytes, cpu_alignment);
            if (nullptr == m_chunkVa)
            {
                m_parent.m_translationTable.removeEntry(m_hwMemoryBlockInfo.start_addr, m_hwMemoryBlockInfo.size);
                m_parent.getAllocationInterface()->deallocateBlock(m_hwMemoryBlockInfo);
                m_hwMemoryBlockInfo.start_addr = 0;
                m_hwMemoryBlockInfo.size       = 0;
                return OSAL_RETURN_MEM;
            }
            return OSAL_RETURN_OK;
        }

        e_osal_return_t deallocate()
        {
            e_meml_return_t meml_ret = MEML_RETURN_OK;

            if (nullptr != m_chunkVa)
            {
                meml_ret = R_MEML_Dealloc(&m_parent.m_meml_handle, (uintptr_t)m_chunkVa);
                if (MEML_RETURN_OK != meml_ret)
                {
                    return OSAL_RETURN_MEM;
                }
                m_chunkVa  = nullptr;
                m_bufferVa = nullptr;

                /* Dealloc and remove HW addr out from translation table */
                if(!m_parent.m_translationTable.removeEntry(m_hwMemoryBlockInfo.start_addr, m_hwMemoryBlockInfo.size))
                {
                    return OSAL_RETURN_MEM;
                }
                if(!m_parent.getAllocationInterface()->deallocateBlock(m_hwMemoryBlockInfo))
                {
                    return OSAL_RETURN_MEM;
                }
                m_hwMemoryBlockInfo.start_addr = 0;
                m_hwMemoryBlockInfo.size       = 0;
            }

            return OSAL_RETURN_OK;
        }

        osal_memory_buffer_handle_t getOsalHandle() final
        {
            return static_cast<osal_memory_buffer_handle_t>(&m_bufferObjStruct);
        }

        e_osal_return_t cpuPtr(void ** const cpu_ptr) final
        {
            if (nullptr == cpu_ptr)
            {
                return OSAL_RETURN_PAR;
            }
            *cpu_ptr = m_bufferVa;

            return OSAL_RETURN_OK;
        }
        e_osal_return_t constCpuPtr(const void ** const const_cpu_ptr) final
        {
            if (nullptr == const_cpu_ptr)
            {
                return OSAL_RETURN_PAR;
            }
            *const_cpu_ptr = m_bufferVa;

            return OSAL_RETURN_OK;
        }

        e_osal_return_t flush(size_t offset, size_t size) final
        {
            if ((m_userBufferSize < offset) || (0U != (offset & 0x3FU)))
            {
                return OSAL_RETURN_PAR;
            }
            else if ((0U == size) || (m_userBufferSize < size) || (0U != (size & 0x3FU)))
            {
                return OSAL_RETURN_PAR;
            }
            else
            {
                /*Do Nothing*/
            }

            return OSAL_RETURN_OK;
        }
        e_osal_return_t invalidate(size_t offset, size_t size) final
        {
            if ((m_userBufferSize < offset) || (0 != (offset & 0x3F)))
            {
                return OSAL_RETURN_PAR;
            }
            else if ((0U == size) || (m_userBufferSize < size) || (0U != (size & 0x3FU)))
            {
                return OSAL_RETURN_PAR;
            }
        	else
            {
                /*Do Nothing*/
            }

            return OSAL_RETURN_OK;
        }
        e_osal_return_t hwAddr(osal_axi_bus_id_t axi_id, uintptr_t * const hw_addr) const final
        {
            if (nullptr == hw_addr || OSAL_AXI_BUS_ID_INVALID == axi_id)
            {
                return OSAL_RETURN_PAR;
            }

            *hw_addr = m_hwMemoryBlockInfo.start_addr;

            return OSAL_RETURN_OK;
        }

        e_osal_return_t size(std::size_t * const size) const final
        {
            if (nullptr == size)
            {
                return OSAL_RETURN_PAR;
            }
            *size = m_userBufferSize;
            return OSAL_RETURN_OK;
        }

        e_osal_return_t checkBuffer(void) const override final
        {
            e_osal_return_t         result = OSAL_RETURN_OK;
            st_osal_mmngr_config_t  conf;
            std::size_t             cnt;
            std::uint8_t*           check_addr;
            std::size_t             check_size;
            std::uintptr_t          chunk_va = (std::uintptr_t)m_chunkVa;
            std::uintptr_t          buffer_va = (std::uintptr_t)m_bufferVa;
            std::size_t             chunk_size = m_chunkSize;

            if (OSAL_RETURN_OK != m_parent.getConfig(&conf))
            {
                return OSAL_RETURN_FAIL;
            }

            if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT != conf.mode) &&
                (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT != conf.mode))
            {
                return OSAL_RETURN_OK;
            }

            check_addr  = (uint8_t *)chunk_va;
            check_size  = buffer_va - chunk_va;
            for (cnt = 0; cnt < check_size; cnt++)
            {
                if (OSAL_MMNGR_GUARD_PAGE_PATTERN != check_addr[cnt])
                {
                    result = OSAL_RETURN_MEM;
                    m_parent.notify(OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED, OSAL_RETURN_OK, (osal_memory_buffer_handle_t)&m_bufferObjStruct);
                    break;
                }
            }

            check_addr  = (uint8_t *)(buffer_va + m_userBufferSize);
            check_size  = (chunk_va + chunk_size) - (buffer_va + m_userBufferSize);
            for (cnt = 0; cnt < check_size; cnt++)
            {
                if (OSAL_MMNGR_GUARD_PAGE_PATTERN != check_addr[cnt])
                {
                    result = OSAL_RETURN_MEM;
                    m_parent.notify(OSAL_MMNGR_EVENT_OVERFLOW_DETECTED, OSAL_RETURN_OK, (osal_memory_buffer_handle_t)&m_bufferObjStruct);
                    break;
                }
            }

            return result;
        }
        /**
         * @brief Print detailed information to the outputstream os
         *
         * @param os output stream to write to
         */
        virtual void print(std::ostream & os) const final
        {
            std::size_t sz;
            this->size(&sz);

            const uint64_t        ptr_max = (uint64_t)(std::numeric_limits<uintptr_t>::max);
            const int     width   = (int)(std::log(ptr_max) / std::log(16)) + 1;
            //constexpr std::size_t p_max   = sizeof("readWrite");

            /* calculcate maximum with for ;d */
            std::size_t requiredBufferSize = 0;
            R_OSAL_IoGetAxiBusNameList(nullptr, 0, &requiredBufferSize);
            char *      buffer       = static_cast<char *>(std::calloc(requiredBufferSize, sizeof(char)));
            std::size_t writtenBytes = 0;
            R_OSAL_IoGetAxiBusNameList(buffer, requiredBufferSize, &writtenBytes);
            auto axi_list = renesas::rcarxos::utils::split(std::string(buffer), '\n');
            std::free(buffer); // do not need the buffer anymore
            buffer = nullptr;

            std::size_t a_max = 0;
            for (auto bus_name : axi_list)
            {
                a_max = std::max(a_max, bus_name.size());
            }
            a_max = std::string("(AXI domain)").size() - a_max;

            auto stored_os_flags = os.flags(); // we change some flags in the next line, store them for recovery.
            /* loop through axi bus ids and check if this buffer is beeing mapped, if yes, print information */
            for (std::size_t i = 1; i < axi_list.size(); i++)
            {
                std::uintptr_t hw_addr = std::numeric_limits<std::uintptr_t>::max();
                if (OSAL_RETURN_OK != this->hwAddr(i, &hw_addr))
                {
                    /* not mapped, so we do not print information for this bus */
                    continue;
                }
                os
                    /* hndl_val */
                    << "0x" << std::setw(width) << std::setfill('0') << std::hex << reinterpret_cast<uintptr_t>(this)
                    << ';'
                    /* address */
                    << "0x" << std::setw(width) << std::setfill('0') << std::hex
                    << reinterpret_cast<uintptr_t>(m_bufferVa) << "--0x" << std::setw(width) << std::setfill('0')
                    << std::hex << reinterpret_cast<uintptr_t>(m_bufferVa) + sz
                    << ';'
                    /* hw_address */
                    << "0x" << std::setw(width) << std::setfill('0') << std::hex << hw_addr << "--0x"
                    << std::setw(width) << std::setfill('0') << std::hex << hw_addr + sz - 1
                    << ";"
                    /* sec */
                    << std::setw(4) << std::setfill(' ') << "ns"
                    << ';'
                    /* d */
                    << std::setw((int)a_max) << std::setfill(' ') << axi_list[i]
                    << ';'
                    /* size */
                    << "0x" << std::setw(width) << std::setfill('0') << std::hex << sz
                    << ';'
                    /* permossions */
                    << "P:" << std::setw(9) << std::setfill(' ') << "readwrite"
                    << " U:" << std::setw(10) << std::setfill(' ') << "readwrite" << ';' << std::endl;
            }
            os.flags(stored_os_flags); // recover original stream flags
        }

        osal_mmngr_buffer_status get_buffer_status() final
        {
            return m_buffer_status;
        }

        void set_buffer_status(osal_mmngr_buffer_status status) final
        {
            m_buffer_status = status;
        }

    private:
        OsalMmngr &                        m_parent;
        AllocatorAlgorithmInterface::Block m_hwMemoryBlockInfo;
        st_osal_memory_buffer_obj          m_bufferObjStruct;
        void *                             m_chunkVa;
        void *                             m_bufferVa;
        std::size_t                        m_chunkSize;
        std::size_t                        m_userBufferSize;
        osal_mmngr_buffer_status           m_buffer_status;
    };

    static e_osal_return_t allocate_cb(void* p_ptr_self, size_t bytes, size_t alignment,
                                       osal_memory_buffer_handle_t * const buffer_obj);
    static e_osal_return_t deallocate_cb (void* p_ptr_self, osal_memory_buffer_handle_t buffer_obj);

    static e_osal_return_t getConfig_cb(const void* p_ptr_self, st_osal_mmngr_config_t * const config);
    static e_osal_return_t registerMonitor_cb(void* p_ptr_self, e_osal_mmngr_event_t     monitored_event,
                                            fp_osal_memory_mngr_cb_t monitor_handler,
                                            void *                   monitor_handler_user_arg);
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    static e_osal_return_t printFile_cb(const void* p_ptr_self, FILE * output);
#endif

    OsalMmngr(st_osal_mmngr_config_t config, osal_memory_region_idx_t region_idx)
        : m_config(config), m_region_idx(region_idx), m_currentRegisteredMonitorCount(0), m_fastMemoryAccessor(*this)
    {
        using namespace renesas::rcarxos;
        R_OSAL_MmngrInitializeMemoryManagerObj(&m_mnngrStruct);
        m_mnngrStruct.p_ptr_self = this;
        m_mnngrStruct.p_allocate = allocate_cb;
        m_mnngrStruct.p_deallocate = deallocate_cb;

        m_mnngrStruct.p_getConfig = getConfig_cb;

        m_mnngrStruct.p_registerMonitor = registerMonitor_cb;

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
        m_mnngrStruct.p_print = printFile_cb;
#else
        m_mnngrStruct.p_print = nullptr;
#endif
    }

    void DeinitOsalMmngr()
    {
        //No need to set exclusion setting in Destracter Function
        //std::lock_guard<std::mutex> lock(m_mtx);
        for (const auto & buf : m_managedBuffers)
        {
            if (OSAL_BUFFER_DEALLOCATED != buf.get()->get_buffer_status())
            {
                if (OSAL_RETURN_OK != deallocate(buf.get()))
                {
                    OSAL_DEBUG("Failed to deallocate buffer before destruction of OsalMmngr");
                    //throw std::runtime_error("Failed to deallocate buffer before destruction of OsalMmngr");
                }
            }
        }
        m_managedBuffers.clear();

        (void)R_MEML_Deinitialize(&m_meml_handle);
        if (nullptr != m_memlPtr)
        {
#if defined(_MSC_VER) || defined(__MINGW32__)
            _aligned_free(m_memlPtr);
#else // assume POSIX
            free(m_memlPtr);
#endif
        }
        R_OSAL_MmngrInitializeMemoryManagerObj(&m_mnngrStruct);
    }

    inline osal_memory_manager_handle_t getMmngrHandle()
    {
        return static_cast<osal_memory_manager_handle_t>(&m_mnngrStruct);
    }

    inline AllocatorAlgorithmInterface * getAllocationInterface()
    {
        return m_allocatorAlgorithm.get();
    }

    virtual e_osal_return_t allocate(std::size_t bytes, std::size_t alignment,
                                     BufferInterface ** const buffer_obj) final
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (nullptr == buffer_obj)
        {
            return OSAL_RETURN_PAR;
        }

        /* check if we are within limits */
        if ((64 > bytes) || (m_config.memory_limit < bytes) || (0 != (bytes & 0x3F)))
        {
            return OSAL_RETURN_CONF;
        }
        else if ((0 == alignment) || (m_config.memory_limit < alignment) ||
                (64 > alignment) || (0 != (alignment & (alignment -1))))
        {
            return OSAL_RETURN_CONF;
        }
        /* Check of number of Allocations */
        if (m_buffer_count >= m_config.max_allowed_allocations)
        {
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, OSAL_RETURN_PROHIBITED, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
            return OSAL_RETURN_PROHIBITED;
        }
        /* Check for total amount of memory allready allocated */
        if ((m_allocatorAlgorithm->spaceUsed() + bytes) > m_config.memory_limit)
        {
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, OSAL_RETURN_MEM, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
            return OSAL_RETURN_MEM; /* using MEM here is ok? Introduce another error code.*/
        }

        std::unique_ptr<MemoryBufferObj> buffer(new MemoryBufferObj(*this));
        auto                             ret = buffer->allocate(bytes, alignment);
        if (OSAL_RETURN_OK != ret)
        {
            // delete buffer;    /* deleted automatically at SCOPE_EXIT by unique_ptr*/
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, ret, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
        }
        else
        {
            buffer->set_buffer_status(OSAL_BUFFER_ALLOCATED);
            *buffer_obj = buffer.get();
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, OSAL_RETURN_OK, buffer->getOsalHandle());
            m_managedBuffers.push_back(std::move(buffer));
            m_buffer_count++;
            auto mmngr_pos = search_mmngr_pos(this->get_region_idx());
            if (mmngr_pos != g_osal_mmngr.end())
            {
                mmngr_pos->second->buffer_p_ptr_self.push_back((*buffer_obj)->getOsalHandle()->p_ptr_self);
            }
            else
            {
                ret = OSAL_RETURN_FAIL;
            }
        }
        return ret;
    }

    virtual e_osal_return_t deallocate(BufferInterface * buffer_obj) final
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        /* sanity checks */
        if (nullptr == buffer_obj)
        {
            return OSAL_RETURN_HANDLE; /* invalid handle */
        }

        /* Update tracing */

        e_osal_return_t ret;
        auto            buffer =
            std::find_if(m_managedBuffers.begin(), m_managedBuffers.end(),
                         [buffer_obj](std::unique_ptr<BufferInterface> & el) { return buffer_obj == el.get(); });

        if (buffer == m_managedBuffers.end())
        {
            /* notify */
            OsalMmngr::notify(OSAL_MMNGR_EVENT_DEALLOCATE, OSAL_RETURN_OK, buffer_obj->getOsalHandle());
            return OSAL_RETURN_HANDLE_NOT_SUPPORTED;
        }

        OsalMmngr::notify(OSAL_MMNGR_EVENT_DEALLOCATE, OSAL_RETURN_OK, buffer_obj->getOsalHandle());
        /* execute deallocation */
        ret = buffer->get()->deallocate();
        if (OSAL_RETURN_OK != ret)
        {
            return ret;
        }

        auto mmngr_pos = search_mmngr_pos(this->get_region_idx());
        if (mmngr_pos != g_osal_mmngr.end())
        {
            auto iterator_ptr_self =
                        std::find(mmngr_pos->second->buffer_p_ptr_self.begin(),
                        mmngr_pos->second->buffer_p_ptr_self.end(),
                        buffer_obj->getOsalHandle()->p_ptr_self);
            if (iterator_ptr_self != mmngr_pos->second->buffer_p_ptr_self.end())
            {
                mmngr_pos->second->buffer_p_ptr_self.erase(iterator_ptr_self);
            }
        }
        else
        {
            return OSAL_RETURN_FAIL;
        }

        buffer->get()->getOsalHandle()->mmngr_owner_hndl = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        buffer->get()->set_buffer_status(OSAL_BUFFER_DEALLOCATED);
        m_buffer_count--;
        return OSAL_RETURN_OK;
    }

    e_osal_return_t map(std::uintptr_t hw_addr, std::size_t bytes, std::size_t cpu_alignment,
                        BufferInterface ** const buffer_obj)
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (nullptr == buffer_obj)
        {
            return OSAL_RETURN_PAR;
        }

        /* check if we are within limits */
        /* Check of number of Allocations */
        if (m_buffer_count >= m_config.max_allowed_allocations)
        {
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, OSAL_RETURN_PROHIBITED, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
            return OSAL_RETURN_PROHIBITED;
        }
        /* Check for total amount of memory allready allocated */
        if ((m_allocatorAlgorithm->spaceUsed() + bytes) > m_config.memory_limit)
        {
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, OSAL_RETURN_MEM, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
            return OSAL_RETURN_MEM; /* using MEM here is ok? Introduce another error code.*/
        }

        std::unique_ptr<MemoryBufferObj> buffer(new MemoryBufferObj(*this));
        auto                             ret = buffer->map(hw_addr, bytes, cpu_alignment);
        if (OSAL_RETURN_OK != ret)
        {
            // delete buffer;   /* shared_ptr takes care about deallocation*/
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, ret, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
        }
        else
        {
            *buffer_obj = buffer.get();
            OsalMmngr::notify(OSAL_MMNGR_EVENT_ALLOCATE, OSAL_RETURN_OK, buffer->getOsalHandle());
            m_managedBuffers.push_back(std::move(buffer));
        }
        return ret;
    }

    e_osal_return_t reserveResources(osal_memory_region_idx_t region_idx) final
    {
        using FreeListAllocatorAlgo = FreeListAllocatorAlgorithm<4_KB, Allocator>;
        e_meml_return_t meml_ret = MEML_RETURN_OK;
        e_meml_alloc_mode_t meml_mode;
        size_t sys_page_size;
        AddressRange_t hw_addr_range;
        std::uintptr_t start_addr_next_region;
        size_t mmngr_count = 0;

        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE; /* should not be reached */
        }

        switch (m_config.mode)
        {
            case OSAL_MMNGR_ALLOC_MODE_FREE_LIST: /* fall through */
            case OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT:
                meml_mode = MEML_ALLOC_MODE_FREE_LIST;
                break;
            case OSAL_MMNGR_ALLOC_MODE_STACK:
            case OSAL_MMNGR_ALLOC_MODE_STACK_PROT:
                meml_mode = MEML_ALLOC_MODE_STACK;
                break;
            default:
                OSAL_DEBUG("Requested OSAL MMNGR Mode not supported");
                return OSAL_RETURN_CONF;
        }

        // case OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT: /* TODO Implement Memory Protection */
        for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
        {
            if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
            {
                mmngr_count++;
            }
        }

        if (0 == mmngr_count)
        {
            hw_addr_range = {0, m_config.memory_limit};
        }
        else if (1 == mmngr_count)
        {
            start_addr_next_region = IMP_SCRATCH_PAD_BASE_ADDR + IMP_SCRATCH_PAD_SIZE;
            hw_addr_range = {start_addr_next_region, start_addr_next_region + m_config.memory_limit};
        }
        else
        {
            start_addr_next_region = g_list_hw_end_addr.back().hw_end_addr;
            hw_addr_range = {start_addr_next_region, start_addr_next_region + m_config.memory_limit};
        }

        g_list_hw_end_addr.push_back({region_idx, hw_addr_range.second});
        m_allocatorAlgorithm = std::shared_ptr<AllocatorAlgorithmInterface>(new FreeListAllocatorAlgo(
                                    hw_addr_range, FreeListAllocatorAlgo::AllocationPolicy::eFindBest));

#if defined(_MSC_VER) || defined(__MINGW32__)
        SYSTEM_INFO sSysInfo;
        GetSystemInfo(&sSysInfo);
        sys_page_size = sSysInfo.dwPageSize;
#else // assume POSIX
        sys_page_size = sysconf(_SC_PAGESIZE);
#endif

        auto limit = (m_config.memory_limit + OSAL_MMNGR_MEMORY_RESERVE_SIZE) & ~(sys_page_size - 1);

#if defined(_MSC_VER) || defined(__MINGW32__)
        m_memlPtr = _aligned_malloc(limit, 64);
#else // assume POSIX
        if (posix_memalign(&m_memlPtr, 64, limit) != 0)
        {
            m_memlPtr = nullptr;
        }
#endif

        if (nullptr == m_memlPtr)
        {
            return OSAL_RETURN_MEM;
        }

        meml_ret = R_MEML_Initialize(&m_meml_handle, (std::uintptr_t)m_memlPtr, (std::uintptr_t)m_memlPtr, limit, meml_mode);
        if (MEML_RETURN_OK != meml_ret)
        {
            return OSAL_RETURN_FAIL;
        }
        return OSAL_RETURN_OK;
    }

    e_osal_return_t getConfig(st_osal_mmngr_config_t * const config) const final
    {
        *config = m_config;
        return OSAL_RETURN_OK;
    }

    e_osal_return_t registerMonitor(e_osal_mmngr_event_t monitored_event, fp_osal_memory_mngr_cb_t monitor_handler,
                                    void * monitor_handler_user_arg) final
    {
        if (m_currentRegisteredMonitorCount >= m_config.max_registered_monitors_cbs)
        {
            return OSAL_RETURN_PROHIBITED;
        }

        for (size_t cnt = 0; cnt < m_currentRegisteredMonitorCount; cnt++)
        {
            if ((monitored_event == m_MonitorEvent[cnt]) &&
                ((void *)monitor_handler == m_MonitorHandler[cnt]))
            {
                return OSAL_RETURN_PAR;
            }
        }

        typename OsalMmngr::observer_key_t observer_hndl;
        auto                               ret = OsalMmngr::registerObserver(
            monitored_event,
            [monitor_handler, monitor_handler_user_arg](typename OsalMmngr::event_type event,
                                                        typename OsalMmngr::error_type error,
                                                        osal_memory_buffer_handle_t    buffer) {
                monitor_handler(monitor_handler_user_arg, event, error, buffer);
            },
            observer_hndl);
        /** @todo add observer hndl to internal list if unregister function is implemented */

        m_MonitorEvent.push_back(std::move(monitored_event));
        m_MonitorHandler.push_back(std::move((void *)monitor_handler));
        m_currentRegisteredMonitorCount++;
        return ret;
    }

    /**
     * @brief Print detailed information about the current memory to the outputstream os
     *
     * @param os output stream to write to
     */
    void print(std::ostream & os) const final
    {
        //constexpr std::size_t conf_width = sizeof("Registered Monitors Current (Limit)") + 1;
        constexpr std::size_t sz_width   = sizeof(std::to_string((uint64_t)std::numeric_limits<size_t>::max)) + 1;

        const uint64_t    ptr_max = (uint64_t)(std::numeric_limits<uintptr_t>::max);
        const int width   = (int)(std::log(ptr_max) / std::log(16)) + 2;

        /* calculcate maximum with for ;d */
        std::size_t requiredBufferSize = 0;
        R_OSAL_IoGetAxiBusNameList(nullptr, 0, &requiredBufferSize);
        char *      buffer       = static_cast<char *>(std::calloc(requiredBufferSize, sizeof(char)));
        std::size_t writtenBytes = 0;
        R_OSAL_IoGetAxiBusNameList(buffer, requiredBufferSize, &writtenBytes);
        auto axi_list = renesas::rcarxos::utils::split(std::string(buffer), '\n');
        std::free(buffer); // do not need the buffer anymore
        buffer = nullptr;

        std::size_t a_max = 0;
        for (auto bus_name : axi_list)
        {
            a_max = std::max(a_max, bus_name.size());
        }
        auto stored_os_flags = os.flags(); // we change some flags in the next line, store them for recovery.
        os << std::left << std::setw(sz_width) << "Memory Current"
           << "(Limit)"
           << " : " << std::setw(sz_width) << size_expr(m_allocatorAlgorithm->spaceUsed()) << " ("
           << size_expr(this->m_config.memory_limit) << ")\n"
           << std::setw(sz_width) << "Allocations Current"
           << "(Limit)"
           << " : " << std::setw(sz_width) << m_buffer_count << " (" << this->m_config.max_allowed_allocations
           << ")\n"
           << std::setw(sz_width) << "Registered Monitors Current"
           << "(Limit)"
           << " : " << std::setw(sz_width) << m_currentRegisteredMonitorCount << " ("
           << this->m_config.max_registered_monitors_cbs << ")\n"
           << std::setw(sz_width) << "Allocation Mode" << std::setw(sizeof("Limit)")) << ' ' << " : "
           << std::setw(sz_width) << printMode(m_config.mode) << '\n'

           << std::setw(width + 1) << "hndl_val" << std::setw(2 * width + 5) << ";address" << std::setw(2 * width + 5)
           << ";hw_address" << std::setw(5) << ";sec" << std::setw((int)a_max) << ";d" << std::setw(width + 2) << ";size"
           << std::setw(sizeof(";P:readWrite U:readWrite")) << ";permossions" << ';' << std::endl;

        for (const auto & buf : m_managedBuffers)
        {
            if (OSAL_BUFFER_ALLOCATED == buf.get()->get_buffer_status())
            {
                buf->print(os);
            }
        }
        os << std::endl;
        os.flags(stored_os_flags); // recover original stream flags
    }

    e_osal_return_t printFile(FILE * output) const final
    {
        std::stringstream ss;
        print(ss);

        int err = 0;
        err = std::fputs(ss.str().c_str(), output);
        if (0 > err)
        {
           return  OSAL_RETURN_IO;
        }

        return OSAL_RETURN_OK;
    }

    osal_memory_manager_handle_t getOsalHandle() final
    {
        return &m_mnngrStruct;
    }

    FastMemoryAccessor & memoryAccessor()
    {
        return m_fastMemoryAccessor;
    }

    void set_handle_id(uint64_t hdl_id)
    {
        handle_id = hdl_id;
    }
    
    uint64_t get_handle_id()
    {
        return handle_id;
    }

    osal_memory_region_idx_t get_region_idx()
    {
        return m_region_idx;
    }
    std::vector<std::unique_ptr<BufferInterface>>& get_managed_buffers()
    {
        return m_managedBuffers;
    }
    
protected:
    uint64_t                                      handle_id;        /*!< Fixed value: OSAL_MEMORY_HANDLE_ID */
    st_osal_memory_manager_obj                    m_mnngrStruct;
    st_osal_mmngr_config_t                        m_config;
    osal_memory_region_idx_t                      m_region_idx;
    std::vector<std::unique_ptr<BufferInterface>> m_managedBuffers;
    std::size_t                                   m_currentRegisteredMonitorCount;
    std::vector<e_osal_mmngr_event_t>             m_MonitorEvent;
    std::vector<void*>                            m_MonitorHandler;
    std::mutex                                    m_mtx;
    st_meml_manage_t                              m_meml_handle;
    void *                                        m_memlPtr;
    size_t                                        m_buffer_count = 0;

    using TranslationTable = renesas::rcarxos::utils::vmsav8_64::TranslationTableSim<MemoryBufferObj *>;

    TranslationTable   m_translationTable;
    FastMemoryAccessor m_fastMemoryAccessor;

    std::shared_ptr<AllocatorAlgorithmInterface> m_allocatorAlgorithm; /* Manager for the HWA Address Space */

private:
    inline constexpr std::string size_expr(std::size_t sz) const
    {

        if (sz >= 1024 * 1024 * 1024)
        {
            return std::to_string((float)sz / (1024 * 1024 * 1024)) + " GB";
        }
        else if (sz >= 1024 * 1024)
        {
            return std::to_string((float)sz / (1024 * 1024)) + " MB";
        }
        else if (sz >= 1024)
        {
            return std::to_string((float)sz / (1024)) + " KB";
        }
        else
        {
            return std::to_string(sz) + " B";
        }
    };

    constexpr const std::string printMode(const e_osal_mmngr_allocator_mode_t & mode) const
    {
        std::string s;
        switch (mode)
        {
            case OSAL_MMNGR_ALLOC_MODE_STACK:
                s = "Stack allocator without Underflow/Overflow detection";
                break;

            case OSAL_MMNGR_ALLOC_MODE_STACK_PROT:
                s = "Stack allocator with Underflow/Overflow detection";
                break;

            case OSAL_MMNGR_ALLOC_MODE_FREE_LIST:
                s = "Free list allocator without Underflow/Overflow detection";
                break;

            case OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT:
                s = "Free list allocator with Underflow/Overflow detection";
                break;

            case OSAL_MMNGR_ALLOC_MODE_UNSPECIFIED:
                s = "Custom allocator without Underflow/Overflow detection";
                break;

            case OSAL_MMNGR_ALLOC_MODE_UNSPECIFIED_PROT:
                s = "Custom allocator with Underflow/Overflow detection";
                break;
            default:
                s = "Invalid allocator";
                break;
        }

        return s;
    }
};

static bool r_osal_internal_memory_check_mmngr_handle_ptr_self(void* p_ptr_self)
{
    bool found = false;
    for (auto it = g_osal_mmngr.begin(); ((it != g_osal_mmngr.end())&&(false == found)); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status && it->second->p_ptr_self == p_ptr_self)
        {
            found = true;
            break;
        }
    }
    return found;
}

static bool r_osal_internal_memory_check_buffer_handle_ptr_self(void* p_ptr_self)
{
    bool found = false;
    for (auto it = g_osal_mmngr.begin(); ((it != g_osal_mmngr.end())&&(false == found)); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            for (size_t i = 0; i <= it->second->buffer_p_ptr_self.size(); i++)
            {
                if(it->second->buffer_p_ptr_self[i] == p_ptr_self )
                {
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::allocate_cb(void* p_ptr_self, size_t bytes, size_t alignment,
                                       osal_memory_buffer_handle_t * const buffer_obj)
{
    BufferInterface * buffer = nullptr;
    size_t mmngr_count = 0;

    for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            mmngr_count++;
        }
    }

    if (0 == mmngr_count)
    {
        return OSAL_RETURN_STATE;
    }

    if (!r_osal_internal_memory_check_mmngr_handle_ptr_self(p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    OsalMmngr<Allocator>* wrapper = (OsalMmngr<Allocator>*)p_ptr_self;
    auto ret = wrapper->allocate(bytes, alignment, &buffer);
    if (OSAL_RETURN_OK != ret)
    {
        return ret;
    }
    else
    {
        *buffer_obj = buffer->getOsalHandle();
    }

    return ret;
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::deallocate_cb(void* p_ptr_self, osal_memory_buffer_handle_t buffer_obj)
{
    size_t mmngr_count = 0;

    for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            mmngr_count++;
        }
    }

    if (0 == mmngr_count)
    {
        return OSAL_RETURN_STATE;
    }

    if (!r_osal_internal_memory_check_mmngr_handle_ptr_self(p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer_obj->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE_NOT_SUPPORTED; /* this implementation expects this value to be set */
    }
    else if (nullptr == buffer_obj->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }
    else
    {
        /* OK */
    }

    OsalMmngr<Allocator>* wrapper = (OsalMmngr<Allocator>*)p_ptr_self;
    if (wrapper->getOsalHandle()->p_ptr_self != buffer_obj->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE_NOT_SUPPORTED; /* this buffer object is not created by this mmngr */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(buffer_obj->p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(buffer_obj->p_ptr_self);
    return wrapper->deallocate(buffer);
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::getConfig_cb(const void* p_ptr_self, st_osal_mmngr_config_t * const config)
{
    size_t mmngr_count = 0;

    for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            mmngr_count++;
        }
    }

    if (0 == mmngr_count)
    {
        return OSAL_RETURN_STATE;
    }

    if (!r_osal_internal_memory_check_mmngr_handle_ptr_self(const_cast<void*>(p_ptr_self)))
    {
        return OSAL_RETURN_HANDLE;
    }

    OsalMmngr<Allocator>* wrapper = (OsalMmngr<Allocator>*)const_cast<void*>(p_ptr_self);

    return wrapper->getConfig(config);
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::registerMonitor_cb(void* p_ptr_self, e_osal_mmngr_event_t     monitored_event,
                                            fp_osal_memory_mngr_cb_t monitor_handler,
                                            void *                   monitor_handler_user_arg)
{
    size_t mmngr_count = 0;

    for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            mmngr_count++;
        }
    }

    if (0 == mmngr_count)
    {
        return OSAL_RETURN_STATE;
    }

    if (!r_osal_internal_memory_check_mmngr_handle_ptr_self(p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    OsalMmngr<Allocator>* wrapper = (OsalMmngr<Allocator>*)p_ptr_self;

    return wrapper->registerMonitor(monitored_event,monitor_handler, monitor_handler_user_arg );
}

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::printFile_cb(const void* p_ptr_self, FILE * output)
{
    size_t mmngr_count = 0;

    for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            mmngr_count++;
        }
    }

    if (0 == mmngr_count)
    {
        return OSAL_RETURN_STATE;
    }

    if (!r_osal_internal_memory_check_mmngr_handle_ptr_self(const_cast<void*>(p_ptr_self)))
    {
        return OSAL_RETURN_HANDLE;
    }
    OsalMmngr<Allocator>* wrapper = (OsalMmngr<Allocator>*)const_cast<void*>(p_ptr_self);
    return wrapper->printFile(output);
}
#endif

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::cpuPtr_cb(void* p_ptr_self, void ** const cpu_ptr)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(p_ptr_self);
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->cpuPtr(cpu_ptr);
    }
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::constCpuPtr_cb(void* p_ptr_self, const void ** const cpu_ptr)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(p_ptr_self);
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->constCpuPtr(cpu_ptr);
    }
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::flush_cb(void* p_ptr_self, size_t offset, size_t size)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(p_ptr_self);
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->flush(offset, size);
    }
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::invalidate_cb(void* p_ptr_self, size_t offset, size_t size)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(p_ptr_self))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(p_ptr_self);
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->invalidate(offset, size);
    }
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::hwAddr_cb(const void * p_ptr_self, osal_axi_bus_id_t axi_id, uintptr_t * const p_hw_addr)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(const_cast<void*>(p_ptr_self)))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(const_cast<void*>(p_ptr_self));
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->hwAddr(axi_id, p_hw_addr);
    }
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::size_cb(const void * p_ptr_self, size_t * const p_size)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(const_cast<void*>(p_ptr_self)))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(const_cast<void*>(p_ptr_self));
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->size(p_size);
    }
}

template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::checkBuffer_cb(const void * p_ptr_self)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(const_cast<void*>(p_ptr_self)))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(const_cast<void*>(p_ptr_self));
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->checkBuffer();
    }
}

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
template<class Allocator> e_osal_return_t OsalMmngr<Allocator>::MemoryBufferObj::printFile_cb(const void * p_ptr_self, FILE * p_output)
{
    if (nullptr == p_ptr_self)
    {
        return OSAL_RETURN_HANDLE; /* this implementation expects this value to be set */
    }

    if (!r_osal_internal_memory_check_buffer_handle_ptr_self(const_cast<void*>(p_ptr_self)))
    {
        return OSAL_RETURN_HANDLE;
    }

    BufferInterface * buffer = static_cast<BufferInterface *>(const_cast<void*>(p_ptr_self));
    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == buffer->getOsalHandle()->mmngr_owner_hndl)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == buffer->getOsalHandle()->mmngr_owner_hndl->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        return buffer->printFile(p_output);
    }
}
#endif

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrOpen")
#endif
e_osal_return_t R_OSAL_MmngrOpen(const st_osal_mmngr_config_t * const config,
                                 osal_memory_manager_handle_t * const hndl_mmngr)
{
    /** @todo replace by central mutex */
    static std::mutex                      mutex; 
    const std::lock_guard<decltype(mutex)> lock(mutex);
    using namespace renesas::rcarxos;

    if (!r_soc_initialized())
    {
        /* is singleton object, can only be open once per process */
        return OSAL_RETURN_STATE;
    }

    auto mmngr_pos = search_mmngr_pos(OSAL_MEMORY_DEFAULT_REGION_IDX);
    if (mmngr_pos != g_osal_mmngr.end())
    {
        return OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (nullptr == hndl_mmngr || nullptr == config || nullptr == hndl_mmngr)
    {
        return OSAL_RETURN_PAR;
    }
    else
    {
        /* OK */
    }

    if (config->max_allowed_allocations > OSAL_MEMORY_MANAGER_CFG_UNLIMITED || config->max_allowed_allocations < 1)
    {
        return OSAL_RETURN_CONF;
    }
    else if (config->max_registered_monitors_cbs > OSAL_MEMORY_MANAGER_CFG_UNLIMITED)
    {
        return OSAL_RETURN_CONF;
    }
    else if (4_KB > config->memory_limit ||
             config->memory_limit > (OSAL_MMNGR_MEMORY_REGION_SIZE - OSAL_MMNGR_MEMORY_RESERVE_SIZE)) /* limit to 32-bit address space */
    {
        return OSAL_RETURN_CONF;
    }
    else if ( (OSAL_MMNGR_ALLOC_MODE_FREE_LIST != config->mode) &&
              (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT != config->mode) &&
              (OSAL_MMNGR_ALLOC_MODE_STACK != config->mode) &&
              (OSAL_MMNGR_ALLOC_MODE_STACK_PROT != config->mode) )
    {
        if ( (OSAL_MMNGR_ALLOC_MODE_UNSPECIFIED == config->mode) ||
            (OSAL_MMNGR_ALLOC_MODE_UNSPECIFIED_PROT == config->mode) )
        {
            return OSAL_RETURN_CONF_UNSUPPORTED;
        }
        return OSAL_RETURN_CONF;
    }
    else
    {
        /* OK */
    }

    // std::unique_ptr<OsalMmngr<>> wrapper = std::unique_ptr<OsalMmngr<>>(new OsalMmngr<>(*config, OSAL_MEMORY_DEFAULT_REGION_IDX));
    auto wrapper = new OsalMmngr<std::allocator<std::uint8_t>>(*config, OSAL_MEMORY_DEFAULT_REGION_IDX);
    if (nullptr == wrapper)
    {
        return OSAL_RETURN_MEM;
    }
    wrapper->set_handle_id(OSAL_MEMORY_HANDLE_ID);

    e_osal_return_t ret = wrapper->reserveResources(OSAL_MEMORY_DEFAULT_REGION_IDX);
    if (OSAL_RETURN_OK != ret)
    {
        return ret;
    }

    MemoryAccessRouter::getInstance().setMemoryResource(&wrapper->memoryAccessor());
    std::unique_ptr<st_osal_memory_handle> local_mmngr_handle = std::unique_ptr<st_osal_memory_handle>(
            new st_osal_memory_handle(wrapper->getMmngrHandle(), OSAL_MEMORY_DEFAULT_REGION_IDX));
    if (nullptr == local_mmngr_handle)
    {
        return OSAL_RETURN_MEM;
    }

    local_mmngr_handle->p_ptr_self = (void*)wrapper;
    local_mmngr_handle->status = OSAL_MMNGR_MANAGER_OPENED;
    *hndl_mmngr = wrapper->getMmngrHandle();
    local_mmngr_handle->wrapper.release();
    local_mmngr_handle->wrapper.reset(wrapper);
    g_osal_mmngr.insert(std::make_pair(OSAL_MEMORY_DEFAULT_REGION_IDX, std::move(local_mmngr_handle)));
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrOpenFromRegionIdx")
#endif
e_osal_return_t R_OSAL_MmngrOpenFromRegionIdx(const st_osal_mmngr_config_t * const p_config,
                                              osal_memory_region_idx_t region_idx,
                                              osal_memory_manager_handle_t * const p_hndl_mmngr)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    static std::mutex                      mutex;
    const std::lock_guard<decltype(mutex)> lock(mutex);

    if(!r_soc_initialized())
    {
        osal_ret = OSAL_RETURN_STATE;
    }

    auto mmngr_pos = search_mmngr_pos(region_idx);
    if (mmngr_pos != g_osal_mmngr.end())
    {
        return OSAL_RETURN_STATE;
    }
    else if((nullptr == p_hndl_mmngr) || (nullptr == p_config))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((p_config->max_allowed_allocations > OSAL_MEMORY_MANAGER_CFG_UNLIMITED) || (p_config->max_allowed_allocations < 1))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else if (p_config->max_registered_monitors_cbs > OSAL_MEMORY_MANAGER_CFG_UNLIMITED)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else if ((4_KB > p_config->memory_limit) || (p_config->memory_limit > (OSAL_MMNGR_MEMORY_REGION_SIZE - OSAL_MMNGR_MEMORY_RESERVE_SIZE)))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            switch(p_config->mode)
            {
                case OSAL_MMNGR_ALLOC_MODE_STACK:
                case OSAL_MMNGR_ALLOC_MODE_STACK_PROT:
                case OSAL_MMNGR_ALLOC_MODE_FREE_LIST:
                case OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT:
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
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        auto wrapper = new OsalMmngr<>(*p_config, region_idx);
        if (nullptr == wrapper)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            wrapper->set_handle_id(OSAL_MEMORY_HANDLE_ID);
            osal_ret = wrapper->reserveResources(region_idx);
            if (OSAL_RETURN_OK != osal_ret)
            {
                delete wrapper;
            }
            else
            {
                MemoryAccessRouter::getInstance().setMemoryResource(&wrapper->memoryAccessor());
                std::unique_ptr<st_osal_memory_handle> local_mmngr_handle = std::unique_ptr<st_osal_memory_handle>(
                                        new st_osal_memory_handle(wrapper->getMmngrHandle(), region_idx));
                if (nullptr == local_mmngr_handle)
                {
                    return OSAL_RETURN_MEM;
                }
                local_mmngr_handle->p_ptr_self = (void*)wrapper;
                local_mmngr_handle->status = OSAL_MMNGR_MANAGER_OPENED;
                local_mmngr_handle->wrapper.reset(wrapper);
                *p_hndl_mmngr = wrapper->getMmngrHandle();
                g_osal_mmngr.insert(std::make_pair(region_idx, std::move(local_mmngr_handle)));
            }
        }
    }
    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrClose")
#endif
e_osal_return_t R_OSAL_MmngrClose(osal_memory_manager_handle_t hndl_mmngr)
{
    /** @todo replace by central mutex */
    static std::mutex                      mutex;
    size_t count = 0;
    size_t mmngr_count = 0;
    std::multimap<osal_memory_region_idx_t, std::unique_ptr<st_osal_memory_handle>>::iterator mmngr_pos;
    size_t i = 0;
    const std::lock_guard<decltype(mutex)> lock(mutex);

    if (!r_soc_initialized())
    {
        /* is singleton object, can only be open once per process */
        return OSAL_RETURN_STATE;
    }

    for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            mmngr_count++;
        }
    }

    if (0 == mmngr_count) // there are no memory handles were opened
    {
        return OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_MEMORY_MANAGER_HANDLE_INVALID == hndl_mmngr)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (nullptr == hndl_mmngr->p_ptr_self)
    {
        return OSAL_RETURN_HANDLE;
    }
    else if (OSAL_MEMORY_HANDLE_ID != ((OsalMmngr<> *)hndl_mmngr->p_ptr_self)->get_handle_id())
    {
        return OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    for (auto it = g_osal_mmngr.begin(); it != g_osal_mmngr.end(); it++)
    {
        if (it->second->mmngr_handle == hndl_mmngr && OSAL_MMNGR_MANAGER_OPENED == it->second->status)
        {
            count++;
            mmngr_pos = it;
            break;
        }
    }

    if (1U != count)
    {
        return OSAL_RETURN_HANDLE_NOT_SUPPORTED; // the input handle was not opened by OSAL Memory Open APIs
    }

    auto wrapper = (OsalMmngr<> *)(hndl_mmngr->p_ptr_self);
    for (auto i = wrapper->get_managed_buffers().begin(); i != wrapper->get_managed_buffers().end(); i++)
    {
        if ((*i)->getOsalHandle()->p_ptr_self == NULL)
        {
            return OSAL_RETURN_HANDLE;
        }
    }
    wrapper->set_handle_id(OSAL_MEMORY_HANDLE_ID_INVALID);
    MemoryAccessRouter::getInstance().setMemoryResource(nullptr);
    wrapper->DeinitOsalMmngr();
    mmngr_pos->second->status = OSAL_MMNGR_MANAGER_CLOSED;
    mmngr_pos->second->mmngr_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    mmngr_pos->second->p_ptr_self = nullptr;
    mmngr_pos->second->buffer_p_ptr_self.clear();
    while(g_list_hw_end_addr.size() > i)
    {
        if (g_list_hw_end_addr[i].index == mmngr_pos->second->region_idx)
        {
            g_list_hw_end_addr.erase(g_list_hw_end_addr.begin() + i);
            g_list_hw_end_addr.shrink_to_fit();
            break;
        }
        i++;
    }

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrGetOsalMaxConfig")
#endif
e_osal_return_t R_OSAL_MmngrGetOsalMaxConfig(st_osal_mmngr_config_t * const config)
{
    if (nullptr == config)
    {
        return OSAL_RETURN_PAR;
    }

    /* static config */
    *config                             = renesas::rcarxos::osal::MMNGR_CONFIG_INIT_VALUE();
    config->max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config->max_registered_monitors_cbs = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    /** @todo we support multiple modes, how to report? */
    config->mode         = OSAL_MMNGR_ALLOC_MODE_FREE_LIST; 
    /* limit in this implementation is the 32-Bit address space for HWA's */
    config->memory_limit = OSAL_MMNGR_MEMORY_REGION_SIZE - OSAL_MMNGR_MEMORY_RESERVE_SIZE;
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx")
#endif
e_osal_return_t R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(osal_memory_region_idx_t region_idx,
                                                          st_osal_mmngr_config_t * const p_config)
{
    (void) region_idx; // TODO: param check?
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_config->mode                          = OSAL_MMNGR_ALLOC_MODE_FREE_LIST;
        p_config->memory_limit                  = OSAL_MMNGR_MEMORY_REGION_SIZE - OSAL_MMNGR_MEMORY_RESERVE_SIZE;
        p_config->max_allowed_allocations       = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
        p_config->max_registered_monitors_cbs   = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    }

    return osal_ret;
}
/**
 * @}
 */
