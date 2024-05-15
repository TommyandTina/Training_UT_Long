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

#ifndef __VARIANT_H__
#define __VARIANT_H__

#include "common.h"

#define BOARD_ID            RCAR_V3U_FALCON
#define DECODER_NUMBER      1

codec_info_t decoder_info[DECODER_NUMBER] = {
    {
        .id      = DECODER_MAX96712,
        .dec_lib = "libcapture-decoder-max96712.so",
        .dec_cfg = {
            {
                .type      = CAPTURE_VIA_ISP,
#if MAX96712_COLOR_PATTERN_TEST				
                .vi_format = RCAR_GEN3_VIN_MC_INF_RGB888,
#else
                .vi_format = RCAR_GEN3_VIN_MC_INF_YUV8_BT601,
#endif
                .yuv_input = 1,
                .vin_num   = 4,
                .vin_chan  = 0,
                .csi_chan  = 0,
                .csi_lanes = 4,
                .scale_support = 0,
            },
            {
                .type      = CAPTURE_VIA_ISP,
#if MAX96712_COLOR_PATTERN_TEST				
                .vi_format = RCAR_GEN3_VIN_MC_INF_RGB888,
#else
                .vi_format = RCAR_GEN3_VIN_MC_INF_YUV8_BT601,
#endif
                .yuv_input = 0,
                .vin_num   = 4,
                .vin_chan  = 8,
                .csi_chan  = 1,
                .csi_lanes = 4,
                .scale_support = 0,
            },
            {
                .type      = CAPTURE_VIA_ISP,
#if MAX96712_COLOR_PATTERN_TEST				
                .vi_format = RCAR_GEN3_VIN_MC_INF_RGB888,
#else				
                .vi_format = RCAR_GEN3_VIN_MC_INF_YUV8_BT601,
#endif
                .yuv_input = 1,
                .vin_num   = 4,
                .vin_chan  = 16,
                .csi_chan  = 2,
                .csi_lanes = 4,
                .scale_support = 0,
            },
            {
                .type      = CAPTURE_VIA_ISP,
#if MAX96712_COLOR_PATTERN_TEST				
                .vi_format = RCAR_GEN3_VIN_MC_INF_RGB888,
#else				
                .vi_format = RCAR_GEN3_VIN_MC_INF_YUV8_BT601,
#endif
                .yuv_input = 0,
                .vin_num   = 4,
                .vin_chan  = 24,
                .csi_chan  = 3,
                .csi_lanes = 4,
                .scale_support = 0,
            },
        }
    }
};

#endif // __VARIANT_H__