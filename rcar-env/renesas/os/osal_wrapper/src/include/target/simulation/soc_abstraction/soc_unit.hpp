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

#ifndef SOCUNIT_H
#define SOCUNIT_H

#include "irq_interface.hpp"
#include "register_interface.hpp"
#include "tlb.hpp"
#include <map>
#include <memory>

class SocUnit : public RegisterInterfaceAxi {
    std::map<uintptr_t, RegisterInterfaceAxi*>      m_StandardAdressRegions;
    IrqControllerInterface&                      m_Gic;
    std::shared_ptr<Tlb<RegisterInterfaceAxi>>      m_tlb;

    template<class T>
    bool writeRegInternal(osal_axi_bus_id_t axi_id, uintptr_t physAddr, T value)
    {
        RegisterInterfaceAxi* regBlkPtr = m_tlb->getRefFromPhysAddr(physAddr);
        if (regBlkPtr == NULL)
        {
            return false;
        }
        return regBlkPtr->writeReg(axi_id, physAddr, value);
    }

public:
    explicit SocUnit(IrqControllerInterface& gic);
    ~SocUnit();

    bool registerPeripheralAddressSpace(RegisterInterfaceAxi& registerInterface, bool autoConnectIrq = true);
    bool unregisterPeripheralAddressSpace(const RegisterInterfaceAxi& registerInterface);
    bool tryReadReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t& val, bool* const p_isAvailableAddr);
    bool tryWriteReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t val, bool* const p_isAvailableAddr);

    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t byte)   override;
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t word)  override;
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t dword) override;
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t qword) override;

    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint8_t& val) override;
    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint16_t& val) override;
    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint32_t& val) override;
    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uint64_t& val) override;

    virtual bool getVirtAddr(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uintptr_t size, void** pp_cpuPtr) override;

    virtual uintptr_t startHwAddr() const override;
    virtual uintptr_t endHwAddr() const override;


    IrqControllerInterface& gic()
    {
        return m_Gic;
    }

    std::shared_ptr<Tlb<RegisterInterfaceAxi>> tlb()
    {
        return m_tlb;
    }
};

/** @}*/

#endif   // SOCUNIT_H
