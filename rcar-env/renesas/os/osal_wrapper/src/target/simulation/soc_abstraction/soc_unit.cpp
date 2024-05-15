/** \addtogroup soc_abstraction
 *  @{
 */

#include "target/simulation/soc_abstraction/soc_unit.hpp"
#include <iomanip>
#include <iostream>

#ifdef HAVE_EASY_PROFILER
#include <easy/arbitrary_value.h>
#include <easy/profiler.h>
#endif

SocUnit::SocUnit(IrqControllerInterface & gic) : m_Gic(gic)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    m_tlb           = std::shared_ptr<Tlb<RegisterInterfaceAxi>>(new Tlb<RegisterInterfaceAxi>());
    if (m_tlb == nullptr)
    {
        throw std::runtime_error("Could not allocate instance of Tlb<RegisterInterfaceAxi>");
    }
}

SocUnit::~SocUnit()
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
}

bool SocUnit::registerPeripheralAddressSpace(RegisterInterfaceAxi & registerBlock, bool autoConnectIrq)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif

    if (!m_tlb->addEntry(registerBlock.startHwAddr(), nullptr, registerBlock.size(), &registerBlock))
    {
        return false;
    }

    m_StandardAdressRegions[registerBlock.startHwAddr()] = &registerBlock;

    // Check if there is an interrupt to register
    PeripheralInterface * peripheral = dynamic_cast<PeripheralInterface *>(&registerBlock);
    if (peripheral != nullptr && autoConnectIrq == true)
    {
        return m_Gic.registerIrqSource(*peripheral);
    }
    return true;
}

bool SocUnit::unregisterPeripheralAddressSpace(const RegisterInterfaceAxi & registerBlock)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif

    if (!m_tlb->removeEntry(registerBlock.startHwAddr(), nullptr, registerBlock.size()))
    {
        return false;
    }

    auto el = m_StandardAdressRegions.find(registerBlock.startHwAddr());
    if (m_StandardAdressRegions.end() != el)
    {
        auto ret = m_StandardAdressRegions.erase(registerBlock.startHwAddr());
        if (ret != 1)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool SocUnit::writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t byte)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    return writeRegInternal<uint8_t>(axi_id, physAddr, byte);
}

bool SocUnit::writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t word)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    return writeRegInternal<uint16_t>(axi_id, physAddr, word);
}

bool SocUnit::writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t dword)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    return writeRegInternal<uint32_t>(axi_id, physAddr, dword);
}

bool SocUnit::writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t qword)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    return writeRegInternal<uint64_t>(axi_id, physAddr, qword);
}

bool SocUnit::readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t & val)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    RegisterInterfaceAxi * regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
    if (regBlkPtr == nullptr)
    {
        return false;
    }
    return regBlkPtr->readReg(axi_id, physAddr, val);
}

bool SocUnit::readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t & val)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    RegisterInterfaceAxi * regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
    if (regBlkPtr == nullptr)
    {
        return false;
    }
    return regBlkPtr->readReg(axi_id, physAddr, val);
}

bool SocUnit::readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t & val)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    RegisterInterfaceAxi * regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
    if (regBlkPtr == nullptr)
    {
        return false;
    }
    return regBlkPtr->readReg(axi_id, physAddr, val);
}

bool SocUnit::readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t & val)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    RegisterInterfaceAxi * regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
    if (regBlkPtr == nullptr)
    {
        return false;
    }
    return regBlkPtr->readReg(axi_id, physAddr, val);
}

bool SocUnit::getVirtAddr(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uintptr_t size, void** pp_cpuPtr)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    RegisterInterfaceAxi * regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
    if (regBlkPtr == nullptr)
    {
        return false;
    }
    return regBlkPtr->getVirtAddr(axi_id, physAddr, size, pp_cpuPtr);
}

uintptr_t SocUnit::startHwAddr() const 
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
 return m_StandardAdressRegions.begin()->second->startHwAddr();
}

uintptr_t SocUnit::endHwAddr() const 
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    auto it = m_StandardAdressRegions.end()--;
    return it->second->endHwAddr();
}

bool SocUnit::tryReadReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t& val, bool* const p_isAvailableAddr)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    *p_isAvailableAddr = false;
    RegisterInterfaceAxi * regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
    if(regBlkPtr == nullptr)
    {
        return false;
    }
    *p_isAvailableAddr = true;
    return regBlkPtr->readReg(axi_id, physAddr, val);
}

bool SocUnit::tryWriteReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t val, bool* const p_isAvailableAddr)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    *p_isAvailableAddr = false;
    RegisterInterfaceAxi * regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
    if(regBlkPtr == nullptr)
    {
        return false;
    }
    *p_isAvailableAddr = true;
    return regBlkPtr->writeReg(axi_id, physAddr, val);
}
/** @}*/