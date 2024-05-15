/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation
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

#ifndef _RCAR_KCRC_
#define _RCAR_KCRC_

#include "rcar_crc_common.h"
#include <hw/crc.h>

int RCAR_KCRC_Open(CRC_DEVICE_Type* dev);
int RCAR_KCRC_GetConfig(CRC_DEVICE_Type* dev, crc_config_t* config);
int RCAR_KCRC_SetConfig(CRC_DEVICE_Type* dev, crc_config_t* config);
int RCAR_KCRC_WriteData(CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize);
uint32_t RCAR_KCRC_GetResult(CRC_DEVICE_Type * dev);
int RCAR_KCRC_Close(CRC_DEVICE_Type* dev);

#endif
