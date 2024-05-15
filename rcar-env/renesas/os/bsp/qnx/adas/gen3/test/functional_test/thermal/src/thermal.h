/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#ifndef _HW_RCAR_THERMAL_H_INCLUDED
#define _HW_RCAR_THERMAL_H_INCLUDED
#include <inttypes.h>
#include <devctl.h>

/*Resource manager name for sensor's channels*/
#define RESMGR_NAME         "thermal"
#define DEV_NAME            "/dev/" RESMGR_NAME     // do not modify


/* TSC channel max */
#define TSC_MAX_NUM                5 

typedef struct
{
    int    tsc_min;
    int    tsc_max;
    int    tsc_val[TSC_MAX_NUM];
} thermal_packet_t;

/*
    Define the actual devctl commnads
    there are four flavors of commands
    __DIOF      Data IO From the driver
    __DIOT      Data IO To the driver
    __DIOTF     Data IO To and From the driver
    __DION      a command with No associated data
*/
#define DCMD_CMD_CODE                   100
#define DCMD_THSCIVM_GET_TEMPERATURE    __DIOF(_DCMD_MISC,  DCMD_CMD_CODE + 1, thermal_packet_t)
#define DCMD_THSCIVM_GET_VOLTAGE        __DIOF(_DCMD_MISC,  DCMD_CMD_CODE + 2, thermal_packet_t)

#endif /* _HW_RCAR_THERMAL_H_INCLUDED */
