
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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/** \addtogroup soc_abstraction
 *  @{
 */

#ifndef __GENERICSIMULATOR_HPP__
#define __GENERICSIMULATOR_HPP__

#include <functional>
#include <initializer_list>
#include <map>
#include <sstream>
#include <stddef.h>
#include <stdint.h>
#include <utility>
#include <vector>

#include "rcar-xos/osal/r_osal.h"
#include "peripheral_interface.hpp"
#include "rcar-xos/generic_sim_api/generic_sim_api.h"
#include "register_interface.hpp"
#if 1 // debug for regread
#include "generic_soc.hpp"
#endif

#ifdef HAVE_EASY_PROFILER
#include <easy/arbitrary_value.h>
#include <easy/profiler.h>
#endif

class GenericSimulator : public PeripheralInterface
{
protected:
    template <class T> bool readMem(uintptr_t physAddr, T * const val)
    {
        if (sizeof(uint32_t) == sizeof(T))
        {
            bool isAvailableAddr;
            bool result = GenericSoc::getInstance().tryReadReg(m_axiId, physAddr, (uint32_t&)*val, &isAvailableAddr);
            if (isAvailableAddr)
            {
                return result;
            }
        }
        return m_MemoryAccessResource->readReg(m_axiId, physAddr, *val);
    }

    template <class T> bool writeMem(uintptr_t physAddr, const T val)
    {
        if (sizeof(uint32_t) == sizeof(T))
        {
            bool isAvailableAddr;
            bool result = GenericSoc::getInstance().tryWriteReg(m_axiId, physAddr, (uint32_t)val, &isAvailableAddr);
            if (isAvailableAddr)
            {
                return result;
            }
        }
        return m_MemoryAccessResource->writeReg(m_axiId, physAddr, val);
    }

    bool getVirtMem(uintptr_t physAddr, uintptr_t size, void** pp_cpuPtr)
    {
        return m_MemoryAccessResource->getVirtAddr(m_axiId, physAddr, size, pp_cpuPtr);
    }

    void assertInterrupt(size_t channel);
    void releaseInterrupt(size_t channel);
    bool signalWup(uint8_t coreId);

public:
    explicit GenericSimulator(const std::string & name, osal_axi_bus_id_t axi_id, uintptr_t baseAddr, size_t size,
                              uint32_t spi, RegisterInterfaceAxi * memory_access_resource);
    explicit GenericSimulator(const std::string & name, osal_axi_bus_id_t axi_id, uintptr_t baseAddr, size_t size,
                              std::vector<uint32_t> spi, RegisterInterfaceAxi * memory_access_resource);

    ~GenericSimulator() override;

    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t byte) override
    {
        (void)axi_id;   /* unused */
        (void)physAddr; /* unused */
        (void)byte;     /* unused */
        return false;
    }
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t word) override
    {
        (void)axi_id;   /* unused */
        (void)physAddr; /* unused */
        (void)word;     /* unused */
        return false;
    }
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t dword) override
    {
        (void)axi_id;   /* unused */
        return genericSim_WriteRegU32(m_Hndl, physAddr - startHwAddr(), dword);
    }
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t qword) override
    {
        (void)axi_id;   /* unused */
        (void)physAddr; /* unused */
        (void)qword;    /* unused */
        return false;
    }

    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t & val) override
    {
        (void)axi_id;   /* unused */
        (void)physAddr; /* unused */
        (void)val;      /* unused */
        return false;
    }

    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t & val) override
    {
        (void)axi_id;   /* unused */
        (void)physAddr; /* unused */
        (void)val;      /* unused */
        return false;
    }

    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t & val) override
    {
        (void)axi_id;   /* unused */
        return genericSim_ReadRegU32(m_Hndl, physAddr - startHwAddr(), &val);
    }

    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t & val) override
    {
        (void)axi_id;   /* unused */
        (void)physAddr; /* unused */
        (void)val;      /* unused */
        return false;
    }

    virtual bool getVirtAddr(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uintptr_t size, void** pp_cpuPtr) override
    {
        (void)axi_id;
        (void)physAddr;
        (void)size;
        (void)pp_cpuPtr;
        return false;
    }

    virtual uintptr_t startHwAddr() const override
    {
        return m_Base;
    }

    virtual uintptr_t endHwAddr() const override
    {
        return m_Base + m_Size - 1;
    }

private:
    std::string                     m_Name;
    GenericSimHndl_t                m_Hndl;
    GenericSimEnvironmentHandlers_t m_EnvHandler;
    RegisterInterfaceAxi *          m_MemoryAccessResource;
    const osal_axi_bus_id_t         m_axiId;

    uintptr_t m_Base;
    uintptr_t m_Size;
};

/** @}*/

#endif // PERIPHERALINTERFACE_H
