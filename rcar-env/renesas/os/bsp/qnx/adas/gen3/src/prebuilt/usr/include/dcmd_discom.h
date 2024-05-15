/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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

#ifndef _DCMD_DISCOM_H_
#define _DCMD_DISCOM_H_

#include <stdint.h>
#include <stdbool.h>
#include <devctl.h>

/* resmgr name */
#define RESMGR_NAME                     "discom/dpy-"
#define DEV_NAME                        "/dev/" RESMGR_NAME

#define _DCMD_DISCOM                    _DCMD_MISC
#define DCMD_DISCOM_ATTACH              __DIOT (_DCMD_DISCOM, 1, dc_data_packet_t)
#define DCMD_DISCOM_DETACH              __DIOT (_DCMD_DISCOM, 2, dc_data_packet_t)
#define DCMD_DISCOM_SET_EXPECTED_CRC    __DIOT (_DCMD_DISCOM, 3, dc_data_packet_t)
#define DCMD_DISCOM_GET_CURRENT_CRC     __DIOTF (_DCMD_DISCOM, 4, dc_data_packet_t)
#define DCMD_DISCOM_COMPARE_VALUE       __DIOTF (_DCMD_DISCOM, 5, dc_data_packet_t)
#define DCMD_DISCOM_SOURCE_RECTANGLE    __DIOT (_DCMD_DISCOM, 6, dc_data_packet_t)
#define DCMD_DISCOM_START               __DIOT (_DCMD_DISCOM, 7, dc_data_packet_t)
#define DCMD_DISCOM_STOP                __DIOT (_DCMD_DISCOM, 8, dc_data_packet_t)

typedef struct {
	uint32_t    hor_off;    // the horizontal offset
	uint32_t    ver_off;    // the vertical offset
	uint32_t    hor_size;   // the horizontal size
	uint32_t    ver_size;   // the vertical size
} rect_t;

typedef struct
{
    int         pipe;
    rect_t      src;
    uint32_t    exp_crc;        // the expectation value of the CRC code
    uint32_t    cur_crc;        // the CRC value of current image
    bool       compare_result;    // the compare result of the CRC code
} dc_data_packet_t;

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://f27svn.qnx.com/svn/repos/h3-wfd/trunk/hardware/wfd/rcar3/discom_if.h $ $Rev: 69 $")
#endif

