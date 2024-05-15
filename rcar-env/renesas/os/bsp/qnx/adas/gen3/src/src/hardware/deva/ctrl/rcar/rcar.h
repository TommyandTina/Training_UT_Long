/*
 * $QNXLicenseC:
 * Copyright 2014, 2016 QNX Software Systems.
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
#if _NTO_VERSION >= 700
#include <mixer/i2s_codec_dll.h>
#endif
#include "ssiu.h"
#include "rcar_support.h"

/* RCAR initialization flag definitions */
#define RCAR_INIT_FLAG_SSIU    0x01
#define RCAR_INIT_FLAG_ADG     0x02
#define RCAR_INIT_FLAG_SCU     0x04
#define RCAR_INIT_FLAG_DMAC    0x08
#define RCAR_INIT_FLAG_VARIANT 0x10

/* RCAR state definitions */
#define RCAR_CHN_IDLE          0 /* state before channel acquire, or after channel release */
#define RCAR_CHN_ACQUIRED      1 /* state after channel acquire */
#define RCAR_CHN_PREPARED      2 /* state after channel prepare */
#define RCAR_CHN_STARTED       3 /* state after trigger start */
#define RCAR_CHN_STOPPED       4 /* state after trigger stop */

/* RCAR Sample Rate Conversion Unit (SCU) usage definitions */
#define RCAR_USE_SCU_SRC 0x1
#define RCAR_USE_SCU_DVC 0x2
#define RCAR_USE_SCU_MIX 0x4
#define RCAR_USE_SCU_CTU 0x8
#define RCAR_USE_SCU_CMD (RCAR_USE_SCU_DVC|RCAR_USE_SCU_MIX|RCAR_USE_SCU_CTU)

/* RCAR SCU/SRC usage summary across all configured audio channels */
#define RCAR_CHANNELS_NONE 0
#define RCAR_CHANNELS_SOME 1
#define RCAR_CHANNELS_ALL  2

/* RCAR max number of PCM devices is based on the max number of SSI instances that can be configured in pin sharing mode;
 * if TDM split mode is supported, this number needs to be multiplied by 4,
 * if TDM explit mode is supported this number needs to be multiplied by 8
 */
#if defined(RCAR_TDMEXSPLIT_SUPPORT)
#define RCAR_MAX_DEVICES (4 * 8)
#elif defined(RCAR_TDMSPLIT_SUPPORT)
#define RCAR_MAX_DEVICES (4 * 4)
#else
#define RCAR_MAX_DEVICES 4
#endif

typedef struct {
   uint32_t audiodma_irq;
   void* audiodma_chn;
   void* audiodma_pp_chn;
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
    uint32_t                   ssi_chan;    /* successfully reserved ssi channel, SSI_CHANNEL_NUM if entry not used/configured */
    uint32_t                   ssi_subchan; /* SSI BUSIF subchannel used by the audio channel */
    uint32_t                   src_chan;    /* successfully reserved src channel, SRC_CHANNEL_NUM if SRC not used for this channel */
    uint32_t                   cmd_chan;    /* successfully reserved cmd channel, CMD_CHANNEL_NUM if CMD not used for this channel */
    uint32_t                   hdmi_chan;   /* successfully reserved hdmi channel, SSI_HDMI_CHANNEL_NUM if HDMI module not used for this channel
                                             * only applicable to playback channels */
    ssi_transfer_mode_t        ssi_transfer_mode;
    ssi_op_mode_t              ssi_op_mode;
    ssi_start_mode_t           ssi_start_mode;
    uint32_t                   dvc_volume[8];
    uint32_t                   dvc_mute;
    uint32_t                   state;       /* RCAR_CHN_IDLE, RCAR_ACQUIRED, RCAR_PREPARED, RCAR_STARTED, RCAR_STOPPED */
    uint32_t                   use_scu;     /* use the Sampling Rate Convertor Unit */
    uint32_t                   use_mlp;     /* use the Media LB Port (MLP) */
    uint32_t                   use_dtcp;    /* use Digital Transmission Content Protection (available in conjunction with MLP) */
} rcar_audio_channel_t;

typedef struct rcar_audio_device
{
    ado_pcm_t                *pcm;
    ado_mixer_t              *scu_mixer;
    ado_mixer_t              *codec_mixer;
    ado_mixer_t              *mixer; /* points to scu_mixer if created, else to codec_mixer if created, else to simple mixer */
    rcar_audio_channel_t     audio_channels[ADO_PCM_CHANNEL_MAX];  /* Settings for one playback and one capture channel.*/
} rcar_audio_device_t;

typedef struct rcar_context
{
    ado_mutex_t              hw_lock;
    ado_card_t               *card;
    ado_mixer_t              *codec_mixer;
#if _NTO_VERSION >= 700
    ado_mixer_dll_codec_callbacks_t codec_mixer_callbacks;
#endif
    rcar_audio_device_t      devices[RCAR_MAX_DEVICES];
    int32_t                  sample_rate_min; /* min sample rate clients can use */
    int32_t                  sample_rate_max; /* max sample rate clients can use */
    uint32_t                 sample_rates;    /* bit mask of sample rates that clients can use */
    uint32_t                 ssi_sample_rate; /* the sample rate the SSI(s) is (are) locked to; if SCU SRC is used in either direction,
                                               * the SSI sample rate will be locked at all times to the SAMPLE_RATE_SSI value from
                                               * the variant.h, or its command line/config file override; otherwise the SSI sample
                                               * rate is locked while a PCM channel is acquired in either direction; at times when
                                               * the SSI sample rate is not locked, ssi_sample_rate will be set to 0 */
    uint32_t                 sample_size;     /* number of bits per sample */
    uint32_t                 slot_size;       /* number of bits in a frame includes both phases */
    uint32_t                 sharedclk_ssi_chan; /* if pin sharing mode is on, this indicates the SSI whose clock pins are used,
                                                    otherwise set to SSI_CHANNEL_NUM */
    uint32_t                 config_ssi_chan_mask;
    uint32_t                 acquired_ssi_chan_mask;
    uint32_t                 acquired_ssi_subchan_mask[SSI_CHANNEL_NUM];
    uint32_t                 active_ssi_chan_mask;
    uint32_t                 active_ssi_subchan_mask[SSI_CHANNEL_NUM];
    uint32_t                 busif_ssi_chan_mask;
    ssi_masterslave_mode_t   ssi_masterslave_mode;
    uint32_t                 sync_start_mode;  /* start mode (synchronized or normal start) */
    ssi_pin_mode_t           ssi_pin_mode;    /* pin sharing mode */
    ssi_config_t             ssi_config;
    uint32_t                 ssi_voices;      /* if the TDM split feature is used, this can be different from the host voices */
    uint32_t                 use_src_summary;
    uint32_t                 use_scu_summary;
    uint32_t                 use_dtcp_summary;
    uint32_t                 use_mlp_summary;
    uint32_t                 tdm_exsplit_map[SSI_CHANNEL_NUM];

    /* MCLK rates: can be supplied as fixed rates or multiples of the sample rate; in the cases where MCLK rates are supplied as
     * fixed rates, two MCLK sources are required, in order to support for 48 kHz and 44.1 kHz based sample rates - mclk_1 and
     * mclk_2 are used; in the case when MCLK is supplied as a multiple of the sample rate, a single clock source is required -
     * mclk_fs is used */
    uint32_t                 mclk_1; /* MCLK rate for MCLK source 1 when MCLK supplied as fixed rate */
    uint32_t                 mclk_2; /* MCLK rate for MCLK source 2 when MCLK supplied as fixed rate */
    uint32_t                 mclk_source_1;
    uint32_t                 mclk_source_2;
    uint32_t                 mclk_fs; /* MCLK rate for the case when MCLK is a multiple of the sample rate */
    uint32_t                 debug;
    int                      i2c_dev;
    uint32_t                 init_flags;
} rcar_context_t;

int rcar_set_dvc_volume(rcar_context_t * rcar, rcar_audio_channel_t * audio_chan, uint32_t voice, uint32_t vol);
int rcar_get_dvc_volume(rcar_context_t * rcar, rcar_audio_channel_t * audio_chan, uint32_t voice, uint32_t *vol);

int rcar_set_dvc_mute(rcar_context_t * rcar, rcar_audio_channel_t * audio_chan, uint32_t mute);
int rcar_get_dvc_mute(rcar_context_t * rcar, rcar_audio_channel_t * audio_chan, uint32_t *mute);

#endif /* _R_Car_H */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/rcar.h $ $Rev: 911285 $")
#endif
