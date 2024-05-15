/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <hw/inout.h>
#include <string.h>
#include <vcapture/capture.h>
#include <hw/hwinfo_rcar.h>
#include <drvr/hwinfo.h>
#include <sys/mman.h>
#include <aarch64/r-car.h>
#include <vin.h>

#define CAPTURE_VIA_CSI     (1 << 3)
#define CAPTURE_VIA_ISP     (1 << 4)

#define CSI2_PHY_MODE_DPHY   1
#define CSI2_PHY_MODE_CPHY   2

#define CAPTURE_PROPERTY_CSI2_PHY_MODE      CAPTURE_PROPERTY('Q','C','P','M')
#define CAPTURE_PROPERTY_BOARD_ID           CAPTURE_PROPERTY('Q','B','I','D')
#define CAPTURE_PROPERTY_DECODER_ID         CAPTURE_PROPERTY('Q','D','I','D')

#define RCAR_VIN_NUM        32

enum board_support {
    RCAR_V3H_CONDOR,
    RCAR_V3M_STATERKIT,
    RCAR_V3M_EAGLE,
    RCAR_V3U_FALCON,
    RCAR_V3_SUPPORT_NUM,
    RCAR_H3_SALVATOR,
    RCAR_M3_SALVATOR,
    RCAR_E3_EBISU,
    RCAR_D3_DRAAK,
    RCAR_V4H_WHITEHAWK
};

enum csi2_channel_support {
    RCAR_CSI2_CH0,
    RCAR_CSI2_CH1,
    RCAR_CSI2_CH2,
    RCAR_CSI2_CH3,
    RCAR_CSI2_NUM
};

enum decoder_support {
    DECODER_MAX9286,
    DECODER_ADV7482,
    DECODER_ADV7612,
    DECODER_ADV7180,
    DECODER_MAX96712,
    DECODER_SUPPORT_NUM
};

enum input_support {
    INPUT_FROM_CAMERA,
    INPUT_FROM_HDMI,
    INPUT_FROM_CVBS,
    INPUT_FROM_NUM
};

typedef struct {
    enum input_support type;
    uint32_t vi_format[2];
    int  yuv_input;
    int  vin_num;
    int  vin_chan;
    int  csi_chan;
    int  csi_lanes;
    int  csi_phy_mode;
    int  scale_support;
} input_attribute_t;

typedef struct _codec_dll_info{
    enum decoder_support id;
    char dec_lib[256];
    int  dec_src_num;
    input_attribute_t dec_cfg[RCAR_CSI2_NUM];
} codec_info_t;

typedef struct __board__ {
    enum board_support board;
} board_t;

typedef struct __decoder__ {
    enum decoder_support decoder;
} decoder_t;

#endif // __COMMON_H__
