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

#ifndef __VARIANT_H__
#define __VARIANT_H__

#include "common.h"

#define BOARD_ID                RCAR_V3M_EAGLE
#define DECODER_NUMBER          3
#define RCAR_VIN_PIN_CONFIG     1

codec_info_t decoder_info[DECODER_NUMBER] = {
    {
        .id      = DECODER_ADV7482,
        .dec_lib = "libcapture-decoder-adv7482.so",
        .dec_cfg = {
            {
                .type      = INPUT_FROM_HDMI | CAPTURE_VIA_CSI,
                .vi_format = RCAR_GEN3_VIN_MC_INF_RGB888,
                .yuv_input = 0,
                .vin_num   = 1,
                .vin_chan  = 0,
                .csi_chan  = 0,
                .csi_lanes = 4,
            },
            {
                .type      = INPUT_FROM_CVBS | CAPTURE_VIA_CSI,
                .vi_format = RCAR_GEN3_VIN_MC_INF_YUV8_BT656,
                .yuv_input = 1,
                .vin_num   = 1,
                .vin_chan  = 0,
                .csi_chan  = 0,
                .csi_lanes = 1,
            }
        }
    },
    {
        .id = DECODER_MAX9286,
        .dec_lib = "libcapture-decoder-max9286.so",
        .dec_cfg = {
            {
                .type = INPUT_FROM_CAMERA | CAPTURE_VIA_CSI,
                .vi_format = RCAR_GEN3_VIN_MC_INF_YUV8_BT656,
                .yuv_input = 1,
                .vin_num   = 4,
                .vin_chan  = 0,
                .csi_chan  = 0,
                .csi_lanes = 4,
            },
        }
    },
    {
        .id = DECODER_ADV7612,
        .dec_lib = "libcapture-decoder-adv7612.so",
        .dec_cfg = {
            {
                .type = INPUT_FROM_HDMI,
                .vi_format = RCAR_GEN3_VIN_MC_INF_YUV10_BT656 | (3 << 12),
                .yuv_input = 1,
                .vin_num   = 1,
                .vin_chan  = 0,
            },
        }
    }
};

#ifdef RCAR_VIN_PIN_CONFIG
/* This VIN's pins is shared with another module */
void vin_pin_config()
{
    uint32_t tmp;
    uintptr_t   PFC_vbase = mmap_device_io(0x1000, RCAR_GEN3_PFC_BASE);
    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_GPSR2);
    tmp |= 0x0001ffff;
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_GPSR2, tmp);

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_IPSR2);
    tmp &= ~(0xF0000000);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_IPSR2, tmp);

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_IPSR3);
    tmp &= ~(0xFFFFFFFF);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_IPSR3, tmp);

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_IPSR4);
    tmp &= ~(0xFFFFFFFF);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_IPSR4, tmp);   

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_IOCTRL30);
    tmp &= ~(0xFFC00000);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_IOCTRL30, tmp);

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_IOCTRL31);
    tmp &= ~(0x0000027F);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_IOCTRL31, tmp);

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_PUEN0);
    tmp &= ~(0x80000000);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_PUEN0, tmp);

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_PUEN1);
    tmp &= ~(0x0000FFFF);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_PUEN1, tmp);

    munmap_device_io(PFC_vbase, 0x1000);
}

/* This I2C3 pins is shared with another module */
void i2c_pin_config()
{
    uint32_t tmp;
    uintptr_t   PFC_vbase = mmap_device_io(0x1000, RCAR_GEN3_PFC_BASE);
    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_GPSR2);
    tmp |= 0x000000C1;
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_GPSR2, tmp);

    tmp = in32(PFC_vbase + RCAR_GEN3_PFC_IPSR3);
    tmp &= ~(0x0F000000);
    tmp |=  (0x02200000);
    out32(PFC_vbase + RCAR_GEN3_PFC_PMMR, ~tmp);
    out32(PFC_vbase + RCAR_GEN3_PFC_IPSR3, tmp);

    munmap_device_io(PFC_vbase, 0x1000);
}
#endif

#endif // __VARIANT_H__
