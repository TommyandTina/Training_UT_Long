/*
 * $QNXLicenseC: 
 * Copyright 2017, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

#include "proto.h"

extern int verbose;

int
rcar_i2c_driver_info(void *hdl, i2c_driver_info_t *info)
{
    rcar_i2c_dev_t  *dev = hdl;

    info->speed_mode = I2C_SPEED_STANDARD | I2C_SPEED_FAST;
    info->addr_mode  = I2C_ADDRFMT_7BIT;
    info->verbosity  = dev->verbose;

    return 0;
}
