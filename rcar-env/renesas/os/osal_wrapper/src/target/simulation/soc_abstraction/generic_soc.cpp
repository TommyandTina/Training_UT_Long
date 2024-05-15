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

#include "target/simulation/soc_abstraction/generic_soc.hpp"
#include <fstream>
#include <iostream>

GenericSoc & GenericSoc::getInstance()
{
    static GenericSoc instance;
    return instance;
}

#ifdef RCAR_OSAL_TRACE_REGWRITES
bool GenericSoc::writeReg(uintptr_t physAddr, uint8_t byte)
{
    traceRegWrite(physAddr, byte);
    return SocUnit::writeReg(physAddr, byte);
}

bool GenericSoc::writeReg(uintptr_t physAddr, uint16_t word)
{
    traceRegWrite(physAddr, word);
    return SocUnit::writeReg(physAddr, word);
}

bool GenericSoc::writeReg(uintptr_t physAddr, uint32_t dword)
{
    GenericSoc::traceRegWrite(physAddr, dword);
    return SocUnit::writeReg(physAddr, dword);
}

bool GenericSoc::writeReg(uintptr_t physAddr, uint64_t qword)
{
    traceRegWrite(physAddr, qword);
    return SocUnit::writeReg(physAddr, qword);
}

bool GenericSoc::readReg(uintptr_t physAddr, uint8_t & val)
{
    bool ret = SocUnit::readReg(physAddr, val);
    traceRegRead(physAddr, val);
    return ret;
}

bool GenericSoc::readReg(uintptr_t physAddr, uint16_t & val)
{
    bool ret = SocUnit::readReg(physAddr, val);
    traceRegRead(physAddr, val);
    return ret;
}

bool GenericSoc::readReg(uintptr_t physAddr, uint32_t & val)
{
    bool ret = SocUnit::readReg(physAddr, val);
    traceRegRead(physAddr, val);
    return ret;
}
bool GenericSoc::readReg(uintptr_t physAddr, uint64_t & val)
{
    bool ret = SocUnit::readReg(physAddr, val);
    traceRegRead(physAddr, val);
    return ret;
}

#endif

/** @}*/