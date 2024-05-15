/*
* $QNXLicenseC:
* Copyright 2017, QNX Software Systems.
* Copyright 2017, Renesas Electronics Corporation.
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

#ifndef __MAX9286_MAX9271_H__
#define __MAX9286_MAX9271_H__

#include "common.h"
#include "decoder.h"

/* MAX9286 */
#define MAX9271_SLAVE_ADDR              0x80
#define MAX9271_ID                      0x9
#define MAX9286_ID                      0x40
#define MAXIM_FRAME_SYNC_PERIOD		    3200000
#define MAXIM_BROADCAST_ADDR            0x6f

/* OV490 */
#define OV490_I2C_ADDR          		0x48
#define OV490_HIGH_OFFSET          		0xFFFD
#define OV490_LOW_OFFSET          		0xFFFE
#define OV490_PID               		0x300a
#define OV490_VER               		0x300b
#define OV490_VERSION       		    0x0490
#define OV490_ISP_IMAGE0_OHSIZE0  		0x60
#define OV490_ISP_IMAGE0_OHSIZE1   		0x61
#define OV490_ISP_IMAGE0_OVSIZE0   		0x62
#define OV490_ISP_IMAGE0_OVSIZE1   		0x63
#define OV490_ISP_DVP_CTRL3				0x6009
#define OV490_DVP_ORDER         		1
#define OV490_ISP_IMAGE0_FORMAT1		0x0071
#define OV490_ISP_IMAGE1_FORMAT1		0x0075
#define OV490_IMAGE_CONTROl				0x6010
#define OV490_EMBEDLINE_CTRL			0xe000
#define OV490_DVP_CTRL0					0x6000
#define OV490_DVP_CTRL1					0x6004
#define OV490_DVP_CTRL2					0x6008
#define OV490_IMAGE_PATH_CONTROL1		0x0091
#define OV490_MIPI_CONTROL_F			0x00bb
#define OV490_HOST_COMMAND				0x00c0
/* OV10640 */
#define OV10640_WRITE_OPERATION			0x5000
#define OV10640_HIGH_OFFSET				0x5001
#define OV10640_LOW_OFFSET				0x5002
#define OV10640_REGISTER_VALUE			0x5003
#define OV10640_SENSOR_CTRL				0x308C


int ov490_init(uint8_t max9286_addr, uint8_t max9271_addr, int port);

#endif // __MAX9286_MAX9271_H__
