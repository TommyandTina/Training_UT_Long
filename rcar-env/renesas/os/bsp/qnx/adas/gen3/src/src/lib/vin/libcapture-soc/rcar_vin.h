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

#ifndef __VIN_H__
#define __VIN_H__

#include "common.h"
#include <unistd.h>
#include <atomic.h>
#include <pthread.h>
#include "rcar_hdmi_audio.h"

/* Horizontal Scaling Factor bit start */
#define SCALE_HFRAC_SHIFT         16
#define PASS_BW_BWIDTH_H_SHIFT    16

#define SOC_SCALING_UPDATE        0x01
#define SOC_CROPPING_UPDATE       0x02
#define RCAR_VIN_MAX_WIDTH        4096
#define RCAR_VIN_MAX_HEIGHT       4096
#define RCAR_VIN_MAX_BUFFER       3
#define RCAR_VIN_PULSE(i)         (55 + i)
#define RCAR_VIN_END              63

/* Use for analog input only */
#define RCAR_NTSC_TOP_GARBAGE     30
#define RCAR_PAL_TOP_GARBAGE      1

/* Vin pthread attributes */
#define RCAR_VIN_STACK_SIZE       8192
#define RCAR_VIN_PRIORITY         21

enum display_support {
    DISPLAY_ID_LVDS = 1,
    DISPLAY_ID_HDMI0,
    DISPLAY_ID_HDMI1,
    DISPLAY_ID_VGA,
    DISPLAY_ID_NUM
};

enum display_type {   
    DISPLAY_TYPE_INTERNAL                    = 0x7660,
    DISPLAY_TYPE_COMPOSITE                   = 0x7661,
    DISPLAY_TYPE_SVIDEO                      = 0x7662,
    DISPLAY_TYPE_COMPONENT_YPbPr             = 0x7663,
    DISPLAY_TYPE_COMPONENT_RGB               = 0x7664,
    DISPLAY_TYPE_COMPONENT_RGBHV             = 0x7665,
    DISPLAY_TYPE_DVI                         = 0x7666,
    DISPLAY_TYPE_HDMI                        = 0x7667,
    DISPLAY_TYPE_DISPLAYPORT                 = 0x7668,
    DISPLAY_TYPE_OTHER                       = 0x7669,
};

enum hdmi_support {
    OUPUT_VIA_HDMI0 = 1,
    OUPUT_VIA_HDMI1,
    OUPUT_VIA_NUM
};

typedef struct _decoder_info {
    char interlace;
    uint32_t sw;
    uint32_t sh;
    uint32_t csi_rate;
    uint32_t dw;
    uint32_t dh;
    char sfmt[64];
    uint32_t dfmt;
    uint32_t dstride;
    uint32_t cx;
    uint32_t cy;
    uint32_t cw;
    uint32_t ch;
    uint32_t update;
    uint32_t format;
    paddr_t addr[RCAR_VIN_MAX_BUFFER];
    int done[RCAR_VIN_MAX_BUFFER];
    int latest[RCAR_VIN_NUM];
    int release[RCAR_VIN_NUM];    
} decoder_info_t;

typedef struct _rcar_vin {
    int iid[RCAR_VIN_NUM];	
    int tid;
    int	chid;
    int	coid;
    int irq[RCAR_VIN_NUM];
    pthread_cond_t cond[RCAR_VIN_NUM];
    pthread_mutex_t mutex;
    pthread_attr_t attr;
    struct sched_param param;
    struct sigevent event[RCAR_VIN_NUM];
    unsigned frm_end;
    void **frm_bufs[RCAR_VIN_NUM];
    int frm_nbufs;
    int nbufs;
    decoder_info_t dec;
    void *exhdl;
    uintptr_t pbase[RCAR_VIN_NUM];
    uintptr_t vbase[RCAR_VIN_NUM];    
    int csi2_chan;
    int input_type;
    int decoder;
    int board;
    int vin_num;
    int vin_chan;
    int screen_idx;
    int output_type;
    
    /* For CSI */
	uintptr_t csi_pbase;
    uintptr_t csi_vbase;
    uint32_t csi_lanes;
    char csi_interlace;
    uint32_t csi_width;
    uint32_t csi_height;
    int csi_rate;
	
	/* For ISP */
	uintptr_t isp_pbase;
    uintptr_t isp_vbase;
    
    /* Variants are defined in variant.h of libcapture-board */
    int scale_support;      // 0 : No support; 1 : Support 
    int audio_support;      // 0 : No support; 1 : Support HDMI0 only; 2 : Support HDMI0 and HDMI1  
    int audio_ssi_chan;
} rcar_vin_t;

int rcar_vin_init(rcar_vin_t *vin);
int rcar_vin_fini(rcar_vin_t *vin);
int rcar_get_frame(rcar_vin_t *vin, uint64_t timeout, uint32_t idx);
int rcar_release_frame(rcar_vin_t *vin, uint32_t idx);
int rcar_vin_update(rcar_vin_t *vin);

#endif // __VIN_H__
