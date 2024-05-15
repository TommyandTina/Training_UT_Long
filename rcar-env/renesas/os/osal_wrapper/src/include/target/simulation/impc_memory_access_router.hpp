
#include "soc_abstraction/register_interface.hpp"

/** \addtogroup soc_abstraction
 *  @{
 */

#ifndef IMPCMEMORYACCESSROUTER_HPP
#define IMPCMEMORYACCESSROUTER_HPP

/* ------------- WORKAROUND TO GET IMPC working with the IO interface ----------- */

class ImpcMemoryAccessRouter : public RegisterInterfaceAxi
{
public:
    ImpcMemoryAccessRouter()
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
        return MemoryAccessRouter::getInstance().getVirtAddr(axi_id, physAddr, size, pp_cpuPtr);
    }

    virtual uintptr_t startHwAddr() const override
    {
        return IMP_SCRATCH_PAD_BASE_ADDR;
    }

    virtual uintptr_t endHwAddr() const override
    {
        return IMP_SCRATCH_PAD_BASE_ADDR + IMP_SCRATCH_PAD_SIZE - 1;
    }

private:
    /* avoid code duplication */
    template <class T> inline bool writeRegInternal(osal_axi_bus_id_t axi_id, uintptr_t physAddr, T value)
    {
        return MemoryAccessRouter::getInstance().writeReg(axi_id, physAddr, value);
    }

    /* avoid code duplication */
    template <class T> inline bool readRegInternal(osal_axi_bus_id_t axi_id, uintptr_t physAddr, T & value)
    {
        return MemoryAccessRouter::getInstance().readReg(axi_id, physAddr, value);
    }
};

/* ------------- ENDOF WORKAROUND TO GET IMPC working with the IO interface ----------- */

# endif /* IMPCMEMORYACCESSROUTER_HPP */

/** @}*/