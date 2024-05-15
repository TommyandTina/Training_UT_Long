
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

#ifndef REGISTERINTERFACE_H
#define REGISTERINTERFACE_H

#include <stddef.h>
#include <stdint.h>
#include "rcar-xos/osal/r_osal.h"

class RegisterInterfaceAxi
{
public:
    explicit RegisterInterfaceAxi()
    {
    }
    virtual ~RegisterInterfaceAxi()
    {
    }

    virtual uintptr_t startHwAddr() const = 0;
    virtual uintptr_t endHwAddr() const   = 0;
    virtual bool      isValidHwAddr(uintptr_t addr) const
    {
        return (addr >= startHwAddr() && addr <= endHwAddr());
    }

    size_t size() const
    {
        return endHwAddr() - startHwAddr() + 1;
    }

    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint8_t byte)   = 0;
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint16_t word)  = 0;
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint32_t dword) = 0;
    virtual bool writeReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint64_t qword) = 0;

    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint8_t & val)  = 0;
    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint16_t & val) = 0;
    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint32_t & val) = 0;
    virtual bool readReg(osal_axi_bus_id_t axi_id, uintptr_t hwAddr, uint64_t & val) = 0;

    virtual bool getVirtAddr(osal_axi_bus_id_t axi_id, uintptr_t physAddr, uintptr_t size, void** pp_cpuPtr) = 0;
};

/** @}*/

#endif   // REGISTERINTERFACE_H
