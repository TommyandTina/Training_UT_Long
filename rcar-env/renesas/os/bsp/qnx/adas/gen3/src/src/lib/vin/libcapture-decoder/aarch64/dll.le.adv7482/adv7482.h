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

#ifndef __ADV7482_H__
#define __ADV7482_H__

#include "common.h"
#include "decoder.h"

#define ADV7482_I2C_MAP_WAIT        0xFF
#define ADV7482_I2C_MAP_EOR         0xFE
#define ADV7482_I2C_MAP_NOT_ADDR    0xFD

/*****************I2C slave address**********************/
#define ADV7482_I2C_MAP_IO          0xE0
#define ADV7482_I2C_MAP_DPLL        0x4C
#define ADV7482_I2C_MAP_CP          0x44
#define ADV7482_I2C_MAP_HDMI        0x68
#define ADV7482_I2C_MAP_REPEATER    0x64
#define ADV7482_I2C_MAP_INFORFRAME  0x62
#define ADV7482_I2C_MAP_EDID        0x6C
#define ADV7482_I2C_MAP_CBUS        0xF0
#define ADV7482_I2C_MAP_CEC         0x82
#define ADV7482_I2C_MAP_SDP         0xF2
#define ADV7482_I2C_MAP_TXB         0x90
#define ADV7482_I2C_MAP_TXA         0x94

/*****************IO registers**********************/
#define ADV7482_VIDEO_STANDARD_480I      0x40
#define ADV7482_VIDEO_STANDARD_576I      0x41
#define ADV7482_VIDEO_STANDARD_480P      0x4A
#define ADV7482_VIDEO_STANDARD_576P      0x4B
#define ADV7482_VIDEO_STANDARD_720P      0x53
#define ADV7482_VIDEO_STANDARD_1080I     0x54
#define ADV7482_VIDEO_STANDARD_1080P     0x5E
#define ADV7482_VIDEO_STANDARD_SVGA56    0x80
#define ADV7482_VIDEO_STANDARD_SVGA60    0x81
#define ADV7482_VIDEO_STANDARD_SVGA72    0x82
#define ADV7482_VIDEO_STANDARD_SVGA75    0x83
#define ADV7482_VIDEO_STANDARD_SVGA85    0x84
#define ADV7482_VIDEO_STANDARD_SXGA60    0x85
#define ADV7482_VIDEO_STANDARD_SXGA75    0x86
#define ADV7482_VIDEO_STANDARD_VGA60     0x88
#define ADV7482_VIDEO_STANDARD_VGA72     0x89
#define ADV7482_VIDEO_STANDARD_VGA75     0x8A
#define ADV7482_VIDEO_STANDARD_VGA85     0x8B
#define ADV7482_VIDEO_STANDARD_XGA60     0x8C
#define ADV7482_VIDEO_STANDARD_XGA70     0x8D
#define ADV7482_VIDEO_STANDARD_XGA75     0x8E
#define ADV7482_VIDEO_STANDARD_XGA85     0x8F
#define ADV7482_VIDEO_STANDARD_UXGA60    0x96

/*****************CP registers**********************/
/* Contrast control register */
#define ADV7482_CP_CONTRACT_CTRL_REG  		0x3a
#define ADV7482_CP_CONTRACT_MINIMUM    		0
#define ADV7482_CP_CONTRACT_MAXIMUM    		255

/* Saturation control register */
#define ADV7482_CP_SATURATION_CTRL_REG   	0x3b
#define ADV7482_CP_SATURATION_MINIMUM      	0
#define ADV7482_CP_SATURATION_MAXIMUM      	255

/* Brightness control register */
#define ADV7482_CP_BRIGHTNEST_CTRL_REG   	0x3c
#define ADV7482_CP_BRIGHTNEST_MINIMUM      	127
#define ADV7482_CP_BRIGHTNEST_MAXIMUM      	-128

/* Hue control register */
#define ADV7482_CP_HUE_CTRL_REG    			0x3d
#define ADV7482_CP_HUE_MINIMUM		        0
#define ADV7482_CP_HUE_MAXIMUM	          	255

/* Video adjustment 0 register */
#define ADV7482_CP_VID_ADJ_0_REG            0x3e
/* Enable color controls */
#define ADV7482_CP_VID_ADJ_EN               0x80

#define ADV7482_CP_DE_POS_HIGH_REG		    0x8b	
#define ADV7482_CP_DE_POS_END_LOW_REG	    0x8c
#define ADV7482_CP_DE_POS_START_LOW_REG	    0x8d

/*****************HDMI registers**********************/
#define ADV7482_HDMI_LINE_WIDTH_1_REG           0x07
#define ADV7482_HDMI_VERT_FILTER_LOCKED         0x80
#define ADV7482_HDMI_DE_REGEN_FILTER_LOCKED     0x20
#define ADV7482_HDMI_LINE_WIDTH_MSBS_MASK       0x1F
#define ADV7482_HDMI_LINE_WIDTH_2_REG           0x08
#define ADV7482_HDMI_FIELD0_HEIGHT_1_REG        0x09
#define ADV7482_HDMI_FIELD0_HEIGHT_1_MSBS_MASK  0x1F
#define ADV7482_HDMI_FIELD0_HEIGHT_2_REG        0x0A
#define ADV7482_HDMI_FIELD1_HEIGHT_1_REG        0x0B
#define ADV7482_HDMI_INTERLACED                 0x20
#define ADV7482_HDMI_FIELD1_HEIGHT_1_MSBS_MASK  0x1F
#define ADV7482_HDMI_FIELD1_HEIGHT_2_REG        0x0C

/*****************SDP registers**********************/
#define ADV7482_SDP_MAIN_MAP        0x00
#define ADV7482_SDP_MAP1            0x20
#define ADV7482_SDP_MAP2            0x40
#define ADV7482_SDP_NO_RO_MAP       0x00
#define ADV7482_SDP_RO_MAIN_MAP     0x01
#define ADV7482_SDP_RO_MAP1         0x02
#define ADV7482_SDP_RO_MAP2         0x03
#define ADV7482_SDP_MAP_RW         (ADV7482_SDP_MAIN_MAP | ADV7482_SDP_NO_RO_MAP)


#define ADV7482_SDP_USR_MAP_RW_REG   0x0e

/* Contrast control register */
#define ADV7482_SDP_CONTRACT_CTRL_REG       	0x08
#define ADV7482_SDP_CONTRACT_MINIMUM   			0
#define ADV7482_SDP_CONTRACT_MAXIMUM        	255

/* Brightness control register */
#define ADV7482_SDP_BRIGHTNESS_CTRL_REG       	0x0a
#define ADV7482_SDP_BRIGHTNESS_MINIMUM   		-128
#define ADV7482_SDP_BRIGHTNESS_MAXIMUM    		127

/* Hue control register */
#define ADV7482_SDP_HUE_CTRL_REG       			0x0b
#define ADV7482_SDP_HUE_MINIMUM       			-127
#define ADV7482_SDP_HUE_MAXIMUM			        128

/* Saturation control register */
#define ADV7482_SDP_SD_SATURATION_U_REG  		0xe3
#define ADV7482_SDP_SD_SATURATION_V_REG  		0xe4
#define ADV7482_SDP_SATURATION_MINIMUM        	0
#define ADV7482_SDP_SATURATION_MAXIMUM        	255

#define ADV7482_SDP_USR_MAP_R10_REG             0x10
#define ADV7482_SDP_USR_MAP_R10_AD_MASK         0x70
#define ADV7482_SDP_AD_RESULT_NTSM_M_J          0x00
#define ADV7482_SDP_AD_RESULT_NTSC_4_43         0x10
#define ADV7482_SDP_AD_RESULT_PAL_M             0x20
#define ADV7482_SDP_AD_RESULT_PAL_60            0x30
#define ADV7482_SDP_AD_RESULT_PAL_B_G           0x40
#define ADV7482_SDP_AD_RESULT_SECAM             0x50
#define ADV7482_SDP_AD_RESULT_PAL_COMB          0x60
#define ADV7482_SDP_AD_RESULT_SECAM_525         0x70
#define ADV7482_SDP_IN_LOCK                     0x01

#define ADV7482_PWR_UP_FLG                      1
#define ADV7482_PWR_DOWN_FLG                    0

struct _video_info;

int adv7482_init(struct _video_info *video, int channel);
int	adv7482_fini(struct _video_info *video, int channel);
int adv7482_update(struct _video_info *video, int channel);
int adv7482_power(struct _video_info* video, int channel, int enable);


#endif // __ADV7482_H__
