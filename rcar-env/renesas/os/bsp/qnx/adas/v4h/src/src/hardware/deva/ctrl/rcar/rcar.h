/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
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

#ifndef _R_Car_H
#define _R_Car_H

struct  rcar_context;
struct  rcar_audio_channel;

#define  HW_CONTEXT_T           struct rcar_context
#define  PCM_SUBCHN_CONTEXT_T   struct rcar_audio_channel

#include <audio_driver.h>
#include <hw/dma.h>
#include <errno.h>
#include <stdint.h>
#include <mixer/i2s_codec_dll.h>
#include "ssiu.h"

/* RCAR initialization flag definitions */
#define RCAR_INIT_FLAG_SSIU    0x01
#define RCAR_INIT_FLAG_ADG     0x02
#define RCAR_INIT_FLAG_DMAC    0x08

/* RCAR state definitions */
#define RCAR_CHN_IDLE          0 /* state before channel acquire, or after channel release */
#define RCAR_CHN_ACQUIRED      1 /* state after channel acquire */
#define RCAR_CHN_PREPARED      2 /* state after channel prepare */
#define RCAR_CHN_STARTED       3 /* state after trigger start */
#define RCAR_CHN_STOPPED       4 /* state after trigger stop */

/* RCAR max number of PCM devices is based on the max number of SSI instances that can be configured in pin sharing mode;
 * if TDM split mode is supported, this number needs to be multiplied by 4,
 * if TDM explit mode is supported this number needs to be multiplied by 8
 */
#if defined(RCAR_TDMEXSPLIT_SUPPORT)
#define RCAR_MAX_DEVICES (1 * 8)
#elif defined(RCAR_TDMSPLIT_SUPPORT)
#define RCAR_MAX_DEVICES (1 * 4)
#else
#define RCAR_MAX_DEVICES 1
#endif

typedef struct {
   uint32_t dma_irq;
   void* dma_chn;
   dma_addr_t addr;
} rcar_audio_dmac_context_t;

typedef struct rcar_audio_channel
{
    uint32_t                   chan;       /* ADO_PCM_CHANNEL_PLAYBACK or ADO_PCM_CHANNEL_CAPTURE */
    uint32_t                   voices;
    const snd_pcm_chmap_t      *chmap;
    ado_pcm_cap_t              pcm_caps;
    ado_pcm_hw_t               pcm_funcs;
    ado_pcm_subchn_t           *subchn;
    ado_pcm_config_t           *pcm_config;
    rcar_audio_dmac_context_t  dma_context;

    uint32_t                   ssi_subchan; /* SSI BUSIF subchannel used by the audio channel */
    ssi_op_mode_t              ssi_op_mode;

    uint32_t                   state;       /* RCAR_CHN_IDLE, RCAR_ACQUIRED, RCAR_PREPARED, RCAR_STARTED, RCAR_STOPPED */
} rcar_audio_channel_t;

typedef struct rcar_audio_device
{
    ado_pcm_t                *pcm;
    ado_mixer_t              *codec_mixer;
    ado_mixer_t              *mixer; /* points to scu_mixer if created, else to codec_mixer if created, else to simple mixer */
    rcar_audio_channel_t     audio_channels[ADO_PCM_CHANNEL_MAX];  /* Only playback or capture at the sametime*/
} rcar_audio_device_t;

typedef struct rcar_context
{
    ado_mutex_t              hw_lock;
    ado_card_t               *card;
    ado_mixer_t              *codec_mixer;
    ado_mixer_dll_codec_callbacks_t codec_mixer_callbacks;
    rcar_audio_device_t      devices[RCAR_MAX_DEVICES];
    int32_t                  sample_rate_min; /* min sample rate clients can use */
    int32_t                  sample_rate_max; /* max sample rate clients can use */
    uint32_t                 sample_rates;    /* bit mask of sample rates that clients can use */
    uint32_t                 sample_size;     /* number of bits per sample */
    uint32_t                 slot_size;       /* number of bits in a frame includes both phases */

    uint32_t                 active_ssi_subchan_mask;
    ssi_masterslave_mode_t   ssi_masterslave_mode;
    ssi_config_t             ssi_config;
    uint32_t                 ssi_voices;      /* if the TDM split feature is used, this can be different from the host voices */
    uint32_t                 tdm_exsplit_map;

    uint32_t                 mclk;     /* MCLK rates: fixed rates  */
    uint32_t                 mclk2;    /* MCLK rates: fixed rates  */
    uint32_t                 mclk_src; /* MCLK source of input clock to BRGA, BRGB or None of both BRGA & BRGB */
    uint32_t                 mclk_fs;  /* MCLK rate for the case when MCLK is a multiple of the sample rate */

    uint32_t                 debug;
    uint32_t                 init_flags;
    char                     *route_name;
    int                      route_port;
} rcar_context_t;

int rcar_gpio_route(rcar_context_t * rcar, uint32_t chan);

#endif /* _R_Car_H */
