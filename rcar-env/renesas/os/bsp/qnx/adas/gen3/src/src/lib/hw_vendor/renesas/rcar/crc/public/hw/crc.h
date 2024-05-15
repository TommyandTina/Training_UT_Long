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

#ifndef _CRC_H_
#define _CRC_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
//#include "rcar/rcar_crc_common.h"

typedef enum crc_mode {
    INDEPENDENT_CRC_MODE     = 0x00u,  
    E2E_CRC_MODE             = 0x01u,     
    CAIP_Lite_MODE           = 0x02u,   
    DATA_THROUGH_MODE        = 0x03u, 
    E2E_AND_DATA_THROUGH_MODE = 0x04u,     
    CRC_PROHIBITED_MODE
} crc_base_mode_t;

typedef struct {
    uint32_t        crc_base;
    crc_base_mode_t crc_mode;
} CRC_Type;

typedef struct crc_config {
    uint32_t        polynomial;
    bool            reflectIn;
    bool            reflectOut;
    uint32_t        outputXORValue;
    uint32_t        initValue;
}crc_config_t;

/***********************************************
 *	CRC APIs prototype
 ***********************************************/
int CRC_Init(CRC_Type * base, const crc_config_t* config);
int CRC_GetConfig(CRC_Type* base, crc_config_t* config);
int CRC_SetConfig(CRC_Type* base, crc_config_t* config);
int CRC_WriteData(CRC_Type* base, const uint8_t * data,size_t dataSize);
uint32_t CRC_GetResult(CRC_Type * base);
int CRC_Deinit(CRC_Type * base);

#endif /*End of #ifndef _CRC_H_*/
