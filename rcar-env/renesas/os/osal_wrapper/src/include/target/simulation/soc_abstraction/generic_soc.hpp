/** \addtogroup soc_abstraction
 *  @{
 */

#ifndef GENERIC_SOC_HPP
#define GENERIC_SOC_HPP

#include "soc_unit.hpp"
#include <fstream>
#include <iostream>

#ifdef RCAR_OSAL_DEBUG_IRQ_INTERFACE
class DebugIrqControllerInterface : public IrqControllerInterface
{
public:
    bool registerIrqSource(PeripheralInterface & irqSource, std::vector<uint32_t> spi)
    {
        std::clog << "DebugIrqControllerInterface.registerIrqSources : [ ";
        for (auto i : spi)
        {
            std::clog << i << " ";
        }
        std::clog << "]" << std::endl;
        return IrqControllerInterface::registerIrqSource(irqSource, spi);
    }

    bool enableIrq(uint32_t spi)
    {
        std::clog << "DebugIrqControllerInterface.enableIrq " << spi << std::endl;
        return IrqControllerInterface::enableIrq(spi);
    }
};
#endif // RCAR_OSAL_DEBUG_IRQ_INTERFACE

class GenericSoc final : public SocUnit
{
#ifdef RCAR_OSAL_DEBUG_IRQ_INTERFACE
    DebugIrqControllerInterface m_Gic;
#else
    IrqControllerInterface m_Gic;
#endif //#ifdef#ifdef RCAR_OSAL_DEBUG_IRQ_INTERFACE

#ifdef RCAR_OSAL_TRACE_REGWRITES
    std::fstream m_traceStream;
    std::mutex   m_ioMutex;
#endif

protected:
    GenericSoc() : SocUnit(m_Gic)
    {
#ifdef RCAR_OSAL_TRACE_REGWRITES
        std::string filename = "traceRegWrites.csv";
        m_traceStream.open(filename, std::ios::out);
        if (!m_traceStream.is_open())
        {
            std::runtime_error("unable to open/create file for streaming");
        }
#endif
    }

#ifdef RCAR_OSAL_TRACE_REGWRITES
    template <typename T> void traceRegWrite(uintptr_t physAddr, const T val)
    {
        std::lock_guard<std::mutex> lk(m_ioMutex);

        m_traceStream << "WU" << std::setw(0) << std::dec << sizeof(T) * 8 << ", 0x" << std::hex << std::setw(8)
                      << std::setfill('0') << physAddr << " = 0x" << std::hex << std::setw(8) << std::setfill('0')
                      << (unsigned int)val << std::endl;
    }

    template <typename T> T traceRegRead(uintptr_t physAddr, const T val)
    {
        std::lock_guard<std::mutex> lk(m_ioMutex);

        m_traceStream << "RU" << std::setw(0) << std::dec << sizeof(T) * 8 << ", 0x" << std::hex << std::setw(8)
                      << std::setfill('0') << physAddr << " = 0x" << std::hex << std::setw(8) << std::setfill('0')
                      << (unsigned int)val << std::endl;
        m_traceStream.flush();
        return val;
    }
#endif

public:
    GenericSoc(const GenericSoc &) = delete;             // delte copy constructor
    GenericSoc & operator=(const GenericSoc &) = delete; // delete assignmnet operator

    ~GenericSoc()  override 
    {
    }

    static GenericSoc & getInstance();

#ifdef RCAR_OSAL_TRACE_REGWRITES
    bool writeReg(uintptr_t physAddr, uint8_t byte) override;

    bool writeReg(uintptr_t physAddr, uint16_t word) override;

    bool writeReg(uintptr_t physAddr, uint32_t dword) override;

    bool writeReg(uintptr_t physAddr, uint64_t qword) override;

    bool readReg(uintptr_t physAddr, uint8_t & val) override;

    bool readReg(uintptr_t physAddr, uint16_t & val) override;

    bool readReg(uintptr_t physAddr, uint32_t & val) override;
    bool readReg(uintptr_t physAddr, uint64_t & val) override;
#endif
};

#endif // GENERIC_SOC_HPP
/** @}*/