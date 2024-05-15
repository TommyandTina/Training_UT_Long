/*
* $QNXLicenseC:
* Copyright 2017, QNX Software Systems.
* Copyright 2021, Renesas Electronics Corporation.
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

#ifndef __ADV7180_H__
#define __ADV7180_H__

#include "common.h"
#include "decoder.h"

#define ADV7180_I2C_IO						0x20
#define ADV7180_INPUT_CONTROL_REG			0x00
#define ADV7180_OUTPUT_CONTROL_REG			0x03

#define ADV7180_STAT1_REG					0x10
#define ADV7180_STAT1_IN_LOCK				0x01
#define ADV7180_STAT1_AD_RESULT_MASK		0x70

#define ADV7180_PWR_MAN_REG		0x0f
#define ADV7180_PWR_MAN_UP		0x04
#define ADV7180_PWR_MAN_DOWN	0x24

#define ADV7180_STAT3_REG				0x13
#define ADV7180_STAT3_INTERLACED		1<<6
#define ADV7180_STAT3_FREE_RUN_ACT	    1<<4

/* Contrast control register */
#define ADV7180_CONTRACT_CTRL_REG       	0x08
#define ADV7180_CONTRACT_MINIMUM   			0
#define ADV7180_CONTRACT_MAXIMUM        	255

/* Brightness control register */
#define ADV7180_BRIGHTNESS_CTRL_REG       	0x0a
#define ADV7180_BRIGHTNESS_MINIMUM   		-128
#define ADV7180_BRIGHTNESS_MAXIMUM    		127

/* Hue control register */
#define ADV7180_HUE_CTRL_REG       			0x0b
#define ADV7180_HUE_MINIMUM       			-127
#define ADV7180_HUE_MAXIMUM			        128

/* Saturation control register */
#define ADV7180_SATURATION_CB_REG  		    0xe3
#define ADV7180_SATURATION_CR_REG  		    0xe4
#define ADV7180_SATURATION_MINIMUM        	0
#define ADV7180_SATURATION_MAXIMUM        	255

#define ADV7180_POWER_ON		1
#define ADV7180_POWER_OFF		0

#endif /* __ADV7180_H__ */
