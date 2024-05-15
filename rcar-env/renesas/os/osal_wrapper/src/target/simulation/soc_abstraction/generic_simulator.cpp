/** \addtogroup soc_abstraction
 *  @{
 */

#include <assert.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stddef.h>
#include <stdexcept>
#include <stdint.h>
#include <string>

#include "rcar-xos/utils/r_utils.hpp"
#include "target/simulation/soc_abstraction/generic_simulator.hpp"

#ifdef HAVE_EASY_PROFILER
#include <easy/arbitrary_value.h>
#include <easy/profiler.h>
#endif

GenericSimulator::GenericSimulator(const std::string & name, osal_axi_bus_id_t axi_id, uintptr_t baseAddr, size_t size,
                                   uint32_t spi, RegisterInterfaceAxi * memory_access_resource)
    : PeripheralInterface({spi}), m_Name(name), m_MemoryAccessResource(memory_access_resource), m_axiId(axi_id),
      m_Base(baseAddr), m_Size(size)

{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif

    m_EnvHandler.userArg   = (void *)this;
    m_EnvHandler.signalWup = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::signalWup)>::make_cb_e<&GenericSimulator::signalWup>();

    m_EnvHandler.assertInterrupt  = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::assertInterrupt)>::make_cb_e<&GenericSimulator::assertInterrupt>();
    m_EnvHandler.releaseInterrupt = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::releaseInterrupt)>::make_cb_e<&GenericSimulator::releaseInterrupt>();

    m_EnvHandler.readMemU8  = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint8_t>)>::make_cb_e<&GenericSimulator::readMem<uint8_t>>();
    m_EnvHandler.readMemU16 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint16_t>)>::make_cb_e<&GenericSimulator::readMem<uint16_t>>();
    m_EnvHandler.readMemU32 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint32_t>)>::make_cb_e<&GenericSimulator::readMem<uint32_t>>();
    m_EnvHandler.readMemU64 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint64_t>)>::make_cb_e<&GenericSimulator::readMem<uint64_t>>();

    m_EnvHandler.writeMemU8  = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint8_t>)>::make_cb_e<&GenericSimulator::writeMem<uint8_t>>();
    m_EnvHandler.writeMemU16 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint16_t>)>::make_cb_e<&GenericSimulator::writeMem<uint16_t>>();
    m_EnvHandler.writeMemU32 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint32_t>)>::make_cb_e<&GenericSimulator::writeMem<uint32_t>>();
    m_EnvHandler.writeMemU64 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint64_t>)>::make_cb_e<&GenericSimulator::writeMem<uint64_t>>();

    m_EnvHandler.getVirtAddr = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::getVirtMem)>::make_cb_e<&GenericSimulator::getVirtMem>();

    m_Hndl = genericSim_OpenDevice(name.c_str(), m_EnvHandler);
    if (GenericSimHndl_Invalid == m_Hndl)
    {
        std::stringstream ss;
        ss << "Failed to open simulator '" << name << "'!" << std::endl;
        throw std::runtime_error(ss.str());
    }
}

GenericSimulator::GenericSimulator(const std::string & name, osal_axi_bus_id_t axi_id, uintptr_t baseAddr, size_t size,
                                   std::vector<uint32_t> spi, RegisterInterfaceAxi * memory_access_resource)
    : PeripheralInterface(spi), m_Name(name), m_MemoryAccessResource(memory_access_resource), m_axiId(axi_id),
      m_Base(baseAddr), m_Size(size)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif

    m_EnvHandler.userArg   = (void *)this;
    m_EnvHandler.signalWup = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::signalWup)>::make_cb_e<&GenericSimulator::signalWup>();

    m_EnvHandler.assertInterrupt  = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::assertInterrupt)>::make_cb_e<&GenericSimulator::assertInterrupt>();
    m_EnvHandler.releaseInterrupt = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::releaseInterrupt)>::make_cb_e<&GenericSimulator::releaseInterrupt>();

    m_EnvHandler.readMemU8  = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint8_t>)>::make_cb_e<&GenericSimulator::readMem<uint8_t>>();
    m_EnvHandler.readMemU16 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint16_t>)>::make_cb_e<&GenericSimulator::readMem<uint16_t>>();
    m_EnvHandler.readMemU32 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint32_t>)>::make_cb_e<&GenericSimulator::readMem<uint32_t>>();
    m_EnvHandler.readMemU64 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::readMem<uint64_t>)>::make_cb_e<&GenericSimulator::readMem<uint64_t>>();

    m_EnvHandler.writeMemU8  = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint8_t>)>::make_cb_e<&GenericSimulator::writeMem<uint8_t>>();
    m_EnvHandler.writeMemU16 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint16_t>)>::make_cb_e<&GenericSimulator::writeMem<uint16_t>>();
    m_EnvHandler.writeMemU32 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint32_t>)>::make_cb_e<&GenericSimulator::writeMem<uint32_t>>();
    m_EnvHandler.writeMemU64 = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::writeMem<uint64_t>)>::make_cb_e<&GenericSimulator::writeMem<uint64_t>>();

    m_EnvHandler.getVirtAddr = renesas::rcarxos::utils::cb_wrapper<decltype(
        &GenericSimulator::getVirtMem)>::make_cb_e<&GenericSimulator::getVirtMem>();

    m_Hndl = genericSim_OpenDevice(name.c_str(), m_EnvHandler);
    if (GenericSimHndl_Invalid == m_Hndl)
    {
        std::stringstream ss;
        ss << "Failed to open simulator '" << name << "'!" << std::endl;
        throw std::runtime_error(ss.str());
    }
}

GenericSimulator::~GenericSimulator()
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    (void)genericSim_CloseDevice(m_Hndl);
    m_Hndl = GenericSimHndl_Invalid;
    // td::cout << "Closed simulator  '" << m_Name << "'." << std::endl;
}

void GenericSimulator::assertInterrupt(size_t channel)
{
#ifdef HAVE_EASY_PROFILER
    EASY_FUNCTION(profiler::colors::Blue800);
#endif
    std::vector<uint32_t> spis = irqSpiList();
    if (spis.size() > channel)
    {
        uint32_t spi = spis[channel];
        signalIrq(spi);
    }
}

void GenericSimulator::releaseInterrupt(size_t channel)
{
    std::vector<uint32_t> spis = irqSpiList();
    if (spis.size() > channel)
    {
        uint32_t spi = spis[channel];
        signalIrqCleared(spi);
    }
}

bool GenericSimulator::signalWup(uint8_t coreId)
{
    /* Wup/Slp not supported by the generic Simulator */
    (void)coreId; /* unused */
    return false;
}
/** @}*/