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

#ifndef __DECODER_H__
#define __DECODER_H__

#include "common.h"
//#include <hw/i2c.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/rsrcdbmgr.h>
#include "variant.h"
#include "max96712.h"

#define DECODER_COLOR_CON_UPDATE    0x10
#define DECODER_COLOR_BRI_UPDATE    0x20
#define DECODER_COLOR_SAT_UPDATE    0x40
#define DECODER_COLOR_HUE_UPDATE    0x80

typedef enum  {
    VIN_INPUT_STD_HDTV = 0,
    VIN_INPUT_STD_SDTV,
    VIN_INPUT_STD_VGA,
} input_standard;

typedef struct _video_info {
    char signal;
    char interlace;
    char format[64];
    uint32_t width;
    uint32_t height;
    int16_t bri;
    uint8_t sat;
    int16_t hue;
    uint8_t con;
    uint32_t update;
    uint32_t csi_speed;
    int csi_ch;
    int csi_lanes;
    int csi_lanes_max;
    int csi_phy_mode;
    input_standard source_std;
    int source_idx;
    int active_dev;
    int slav_addr;
    int gmsl_mode;
    void *dec_data;
} video_info_t;

typedef struct  _capture_context {
    int enable;
    int is_running;
    int source_idx;
    int active_dev;
    int board_id;
    pthread_mutex_t mutex;
    video_info_t video;
} rcar_context_t;

#endif // __DECODER_H__
