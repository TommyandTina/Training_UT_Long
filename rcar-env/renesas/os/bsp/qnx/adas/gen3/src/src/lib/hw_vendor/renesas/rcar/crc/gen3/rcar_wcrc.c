/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include "rcar_crc.h"
#include "rcar_wcrc.h"
#include <sys/mman.h>
#include <hw/inout.h>
#include <unistd.h>
#include <aarch64/r-car-gen3.h>
#include <hw/crc.h>

/***********************************************
 *	MACROS/DEFINES
 ***********************************************/

/***********************************************
 *	GLOBAL VARIABLE
 ***********************************************/

/***********************************************
 *	APIs IMPLEMENATION
 ***********************************************/

/*!
 *    @brief        Write value to specific register in WCRC module
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    uint32_t reg
 *    @param[in]    Value need to to write
 *    @return       None
 */
void RCAR_WCRC_REG_WRITE (CRC_DEVICE_Type* dev, uint32_t reg, uint32_t dwValue)
{
    out32(dev->wcrc_base_ptr + reg, dwValue);
}

/*!
 *    @brief        Read value to specific register in WCRC module
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    uint32_t reg
 *    @return       Value of register
 */
uint32_t  RCAR_WCRC_REG_READ (CRC_DEVICE_Type* dev, uint32_t reg)
{
    return in32(dev->wcrc_base_ptr + reg);
}

/*!
 *    @brief        Swap bit with size 32
 *    @param[in]    uint32_t dwValue_In
 *    @return       Value of swapped data
 */

uint32_t bitswap32 (uint32_t dwValue_In)
{
    uint32_t dwValue;

    dwValue = 0;
    for(int i = 0; i < 32; i++) {
        if ((1 << i) & (dwValue_In)) {
            dwValue |= 1 << (31 -i);
        }
    }

    return dwValue;
}

/*!
 *    @brief        Swap bit with size 16
 *    @param[in]    uint32_t dwValue_In
 *    @return       Value of swapped data
 */

uint32_t bitswap16 (uint32_t dwValue_In)
{
    uint32_t dwValue;

    dwValue = 0;
    for(int i  =0; i < 16; i++) {
        if ((1 << i) & (dwValue_In)) {
            dwValue |= 1 << (15 -i);
        }
    }

    return dwValue;
}


/*!
 *    @brief        Swap bit with size 8
 *    @param[in]    uint32_t dwValue_In
 *    @return       Value of swapped data
 */

uint32_t bitswap8 (uint32_t dwValue_In)
{
    uint32_t dwValue;

    dwValue = 0;
    for(int i = 0; i < 8; i++) {
        if ((1 << i) & (dwValue_In)) {
            dwValue |= 1 << (7 -i);
        }
    }

    return dwValue;
}

