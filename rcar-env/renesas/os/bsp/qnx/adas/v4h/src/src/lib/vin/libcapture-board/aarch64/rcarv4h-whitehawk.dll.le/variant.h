/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#ifndef __VARIANT_H__
#define __VARIANT_H__

#include "common.h"

#define BOARD_ID            RCAR_V4H_WHITEHAWK
#define DECODER_NUMBER      2

codec_info_t decoder_info[DECODER_NUMBER] = {
    {
        .id      = DECODER_MAX96712,
        .dec_lib = "libcapture-decoder-max96712.so",
        .dec_cfg = {
            {
                .type      = CAPTURE_VIA_ISP,
#ifdef MAX96712_COLOR_PATTERN_TEST
                .vi_format[0]  = RCAR_VIN_MC_INF_RGB888,
                .vin_num       = 1,
#else
                .vi_format[0]  = RCAR_VIN_MC_INF_YUV8_BT601,
                .vi_format[1]  = RCAR_VIN_MC_INF_RAW10,
                .vin_num       = 4,
#endif
                .yuv_input     = 0,
                .csi_lanes     = 3,
                .vin_chan      = 8,
                .csi_chan      = 1,
                .csi_phy_mode  = CSI2_PHY_MODE_CPHY,
                .scale_support = 0,
            },
            { 0 },
        }
	},
    {
        .id      = DECODER_MAX96712,
        .dec_lib = "libcapture-decoder-max96712.so",
        .dec_cfg = {
            {
                .type      = CAPTURE_VIA_ISP,
#ifdef MAX96712_COLOR_PATTERN_TEST
                .vi_format[0]  = RCAR_VIN_MC_INF_RGB888,
                .vin_num       = 1,
#else
		        .vi_format[0] = RCAR_VIN_MC_INF_YUV8_BT601,
                .vi_format[1]  = RCAR_VIN_MC_INF_RAW10,
                .vin_num       = 4,
#endif
                .yuv_input     = 0,
                .csi_lanes     = 3,
                .vin_chan      = 0,
                .csi_chan      = 0,
                .csi_phy_mode  = CSI2_PHY_MODE_CPHY,
                .scale_support = 0,
            },
            { 0 },
        },
    },
};

#endif // __VARIANT_H__
