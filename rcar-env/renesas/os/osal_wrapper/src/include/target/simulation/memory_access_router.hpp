/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/** \addtogroup soc_abstraction
 *  @{
 */
#ifndef MEMORY_ACCESS_ROUTER_HPP
#define MEMORY_ACCESS_ROUTER_HPP

#include "rcar-xos/utils/r_utils.hpp"
#include "soc_abstraction/register_interface.hpp"
#include <array>
#include <mutex>
#include <shared_mutex>

#ifdef HAVE_EASY_PROFILER
#include <easy/arbitrary_value.h>
#include <easy/profiler.h>
#endif

#ifdef RCAR_OSAL_TRACE_REGWRITES
#include <fstream>
#include <iomanip>
#include <iostream>
#endif

using namespace renesas::rcarxos::utils::literals;
#if defined(RCAR_V3M2)
#define IMP_SCRATCH_PAD_SIZE (7 * 128_KB)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED000000U)
#define SYSTEM_RAM_SIZE (448_KB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V3H1) || defined(RCAR_V3H2)
#define IMP_SCRATCH_PAD_SIZE (16 * 128_KB)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED000000U)
#define SYSTEM_RAM_SIZE (384_KB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V3U)
#define IMP_SCRATCH_PAD_SIZE (0xEDB00000U - 0xED300000U)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED300000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V4H2)
#define IMP_SCRATCH_PAD_SIZE (0xED680000U - 0xED300000U)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED300000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V4H)
#define IMP_SCRATCH_PAD_SIZE (0xED600000U - 0xED300000U)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED300000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V4M)
#define IMP_SCRATCH_PAD_SIZE (0xED680000U - 0xED200000U) // 4.5MB: imp_spmi_00 (2MB) + imp_spmc_00 (2MB) + imp_spmc_01 (0.5MB)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED200000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#else
#error "unknown SoC. Only V3M2, V3H1 V3H2 and V3U are supported. Please set RCAR_V3M2, RCAR_V3H1, RCAR_V3H2 or RCAR_V3U defintions."
#endif

class MemoryAccessRouter final : public RegisterInterfaceAxi
{
protected:
    MemoryAccessRouter()
    {
#ifdef RCAR_OSAL_TRACE_REGWRITES
        std::string filename = "traceMemAccess.csv";
        m_traceStream.open(filename, std::ios::out);
        if (!m_traceStream.is_open())
        {
            std::runtime_error("unable to open/create file for streaming");
        }
#endif
        m_impScratchPadBuffer.fill(0xCDU);
        m_systemRAMBuffer.fill(0x00U);
    }

public:
    MemoryAccessRouter(const MemoryAccessRouter &) = delete;             // delte copy constructor
    MemoryAccessRouter & operator=(const MemoryAccessRouter &) = delete; // delete assignmnet operator

    static MemoryAccessRouter & getInstance();

    void setMemoryResource(RegisterInterfaceAxi * memoryResource)
    {
        /* modification of memory resources required unique access */
        std::unique_lock<decltype(m_sharedMutex)> l(m_sharedMutex);

        m_memoryMnngr = memoryResource;
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
        std::shared_lock<decltype(m_sharedMutex)> l(m_sharedMutex);
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        if ((physAddr >= IMP_SCRATCH_PAD_BASE_ADDR) && ((physAddr + size) < (IMP_SCRATCH_PAD_BASE_ADDR + IMP_SCRATCH_PAD_SIZE)))
        {
            std::size_t offset  = physAddr % IMP_SCRATCH_PAD_BASE_ADDR;
            void*       p_cpu   = (reinterpret_cast<void *>(m_impScratchPadBuffer.data() + offset));
            *pp_cpuPtr = p_cpu;
            return true;
        }
        if ((physAddr >= SYSTEM_RAM_BASE_ADDR) && ((physAddr + size) < (SYSTEM_RAM_BASE_ADDR + SYSTEM_RAM_SIZE)))
        {
            std::size_t offset  = physAddr % SYSTEM_RAM_BASE_ADDR;
            void*       p_cpu   = (reinterpret_cast<void *>(m_systemRAMBuffer.data() + offset));
            *pp_cpuPtr = p_cpu;
            return true;
        }
        bool ret = false;
        if (nullptr != m_memoryMnngr)
        {
            ret = m_memoryMnngr->getVirtAddr(axi_id, physAddr, size, pp_cpuPtr);
        }
        return ret;
    }

    virtual uintptr_t startHwAddr() const override
    {
        std::shared_lock<decltype(m_sharedMutex)> l(m_sharedMutex);
        uintptr_t                                 ret = 0;
        if (nullptr != m_memoryMnngr)
        {
            ret = m_memoryMnngr->startHwAddr();
        }
        return ret;
    }

    virtual uintptr_t endHwAddr() const override
    {
        std::shared_lock<decltype(m_sharedMutex)> l(m_sharedMutex);
        uintptr_t                                 ret = 0;
        if (nullptr != m_memoryMnngr)
        {
            ret = m_memoryMnngr->endHwAddr();
        }
        return ret;
    }

private:
#ifdef RCAR_OSAL_TRACE_REGWRITES
    template <typename T> void traceRegWrite(osal_axi_bus_id_t axi_id, uintptr_t physAddr, const T val, bool wasSuccess)
    {
        std::lock_guard<std::mutex> lk(m_ioMutex);
        static char                 buffer[255] = {0};
        std::size_t                 numBytes    = 0;
        std::string                 axiBusname;
        e_osal_return_t             ret = R_OSAL_IoGetAxiBusName(axi_id, buffer, 255, &numBytes);
        if (OSAL_RETURN_OK == ret)
        {
            axiBusname = std::string(buffer);
        }

        m_traceStream << axiBusname << "_WU" << std::setw(0) << std::dec << sizeof(T) * 8 << ", 0x" << std::hex
                      << std::setw(8) << std::setfill('0') << physAddr << " = 0x" << std::hex << std::setw(8)
                      << std::setfill('0') << (unsigned int)val << ", " << (wasSuccess ? "success" : "fail")
                      << std::endl;
        m_traceStream.flush();
    }

    template <typename T> void traceRegRead(osal_axi_bus_id_t axi_id, uintptr_t physAddr, const T val, bool wasSuccess)
    {
        std::lock_guard<std::mutex> lk(m_ioMutex);
        static char                 buffer[255] = {0};
        std::size_t                 numBytes    = 0;
        std::string                 axiBusname;
        e_osal_return_t             ret = R_OSAL_IoGetAxiBusName(axi_id, buffer, 255, &numBytes);
        if (OSAL_RETURN_OK == ret)
        {
            axiBusname = std::string(buffer);
        }

        m_traceStream << axiBusname << "_RU" << std::setw(0) << std::dec << sizeof(T) * 8 << ", 0x" << std::hex
                      << std::setw(8) << std::setfill('0') << physAddr << " = 0x" << std::hex << std::setw(8)
                      << std::setfill('0') << (unsigned int)val << ", " << (wasSuccess ? "success" : "fail")
                      << std::endl;
        m_traceStream.flush();
    }
#endif

    /* avoid code duplication */
    template <class T> inline bool writeRegInternal(osal_axi_bus_id_t axi_id, uintptr_t physAddr, T value)
    {
        std::shared_lock<decltype(m_sharedMutex)> l(m_sharedMutex);
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        if (physAddr >= IMP_SCRATCH_PAD_BASE_ADDR && physAddr < (IMP_SCRATCH_PAD_BASE_ADDR + IMP_SCRATCH_PAD_SIZE))
        {
            std::size_t offset                                              = physAddr % IMP_SCRATCH_PAD_BASE_ADDR;
            *(reinterpret_cast<T *>(m_impScratchPadBuffer.data() + offset)) = value;
            return true;
        }
        if (physAddr >= SYSTEM_RAM_BASE_ADDR && physAddr < (SYSTEM_RAM_BASE_ADDR + SYSTEM_RAM_SIZE))
        {
            std::size_t offset                                              = physAddr % SYSTEM_RAM_BASE_ADDR;
            *(reinterpret_cast<T *>(m_systemRAMBuffer.data() + offset)) = value;
            return true;
        }
        bool ret = false;
        if (nullptr != m_memoryMnngr)
        {
            ret = m_memoryMnngr->writeReg(axi_id, physAddr, value);
        }
#ifdef RCAR_OSAL_TRACE_REGWRITES
        traceRegWrite(axi_id, physAddr, value, ret);
#endif
        return ret;
    }

    /* avoid code duplication */
    template <class T> inline bool readRegInternal(osal_axi_bus_id_t axi_id, uintptr_t physAddr, T & value)
    {
        std::shared_lock<decltype(m_sharedMutex)> l(m_sharedMutex);
#ifdef HAVE_EASY_PROFILER
        EASY_FUNCTION(profiler::colors::Blue800);
#endif
        if (physAddr >= IMP_SCRATCH_PAD_BASE_ADDR && physAddr < (IMP_SCRATCH_PAD_BASE_ADDR + IMP_SCRATCH_PAD_SIZE))
        {
            std::size_t offset = physAddr % IMP_SCRATCH_PAD_BASE_ADDR;
            value              = *(reinterpret_cast<T *>(m_impScratchPadBuffer.data() + offset));
            return true;
        }
        if (physAddr >= SYSTEM_RAM_BASE_ADDR && physAddr < (SYSTEM_RAM_BASE_ADDR + SYSTEM_RAM_SIZE))
        {
            std::size_t offset = physAddr % SYSTEM_RAM_BASE_ADDR;
            value              = *(reinterpret_cast<T *>(m_systemRAMBuffer.data() + offset));
            return true;
        }
        bool ret = false;
        if (nullptr != m_memoryMnngr)
        {
            ret = m_memoryMnngr->readReg(axi_id, physAddr, value);
        }

#ifdef RCAR_OSAL_TRACE_REGWRITES
        traceRegRead(axi_id, physAddr, value, ret);
#endif
        return ret;
    }

    mutable std::shared_timed_mutex m_sharedMutex;
#ifdef RCAR_OSAL_TRACE_REGWRITES
    std::fstream       m_traceStream;
    mutable std::mutex m_ioMutex;
#endif

    RegisterInterfaceAxi * m_memoryMnngr = nullptr;
    /** @todo Check if IMPC access is affected by IPMMU settings */
    std::array<std::uint8_t, IMP_SCRATCH_PAD_SIZE>
        m_impScratchPadBuffer; // fast fix to implement IMP ScratchPad 
    std::array<std::uint8_t, SYSTEM_RAM_SIZE>
        m_systemRAMBuffer; // fast fix to implement System RAM
};

#endif // MEMORY_ACCESS_ROUTER_HPP
/** @}*/
