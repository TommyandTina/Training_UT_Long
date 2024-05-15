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

#include <unistd.h>
#include <string.h>
#include "mem_map.h"
#include "rcar.h"
#include "ssiu.h"
#include "dmac.h"
#include "adg.h"
#include "rcar_mixer.h"
#include "variant.h"

static inline uint32_t count_set_bits( uint32_t bitmask ) {
    uint32_t count = 0;
    while (bitmask) { bitmask &= (bitmask-1); count++; }
    return count;
}

/*  Supported_rate_list is a bitmask of V4H hardware supported rates */
static const uint32_t supported_rate_list = SND_PCM_RATE_8000  |
                                            SND_PCM_RATE_11025 |
#ifdef SND_PCM_RATE_12000
                                            SND_PCM_RATE_12000 |
#endif
                                            SND_PCM_RATE_16000 |
                                            SND_PCM_RATE_22050 |
                                            SND_PCM_RATE_24000 |
                                            SND_PCM_RATE_32000 |
                                            SND_PCM_RATE_44100 |
                                            SND_PCM_RATE_48000 |
#ifdef SND_PCM_RATE_64000
                                            SND_PCM_RATE_64000 |
#endif
                                            SND_PCM_RATE_88200 |
                                            SND_PCM_RATE_96000 |
                                            SND_PCM_RATE_176400 |
                                            SND_PCM_RATE_192000;

#define RCAR_NUM_SUPPORTED_RATES (count_set_bits(supported_rate_list))

static int rcar_set_clock_rate_codec_ssi (rcar_context_t * rcar, uint32_t sample_rate);

static void rcar_register_dump( HW_CONTEXT_T * rcar );

static int rcar_playback_dma_setup ( rcar_context_t * rcar, rcar_audio_channel_t* playback, ado_pcm_dmabuf_t* dmabuf ) {

    int status = EOK;

    audio_peripheral_t dest;

    /* DMA transfer from Memory to SSI0-y (y = 0 to 7) */
    dest = AUDIO_PERIPHERAL_SSI_BUSIF(playback->ssi_subchan);

    status = audio_dmac_tx_setup ( playback->dma_context.dma_chn,
                                   dest,
                                   dmabuf->phys_addr,
                                   dmabuf->size );
    if ( status != EOK ) {
        ado_error_fmt ( "failed setting up dmac tx to peripheral %x", dest );
    }

    return status;
}

static int rcar_capture_dma_setup ( rcar_context_t * rcar, rcar_audio_channel_t* capture, ado_pcm_dmabuf_t* dmabuf ) {

    int status = EOK;

    audio_peripheral_t src;

    /* DMA transfer from SSI0-y (y = 0 to 7) to Memory */
    src = AUDIO_PERIPHERAL_SSI_BUSIF(capture->ssi_subchan);

    status = audio_dmac_rx_setup ( capture->dma_context.dma_chn,
                                   src,
                                   dmabuf->phys_addr,
                                   dmabuf->size );
    if( status != EOK ) {
        ado_error_fmt( "failed setting up dmac rx from peripheral %x", src );
    }

    return status;
}

static int32_t rcar_capabilities(HW_CONTEXT_T* rcar, ado_pcm_t *pcm, snd_pcm_channel_info_t* info)
{
    rcar_audio_channel_t* audio_chan = NULL;
    ado_debug( DB_LVL_DRIVER, "CAPABILITIES" );

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        if ( rcar->devices[i].pcm == pcm ) {
            if ( rcar->devices[i].audio_channels[info->channel].state != RCAR_CHN_IDLE ) {
                audio_chan = &rcar->devices[i].audio_channels[info->channel];
            }
            audio_chan = &rcar->devices[i].audio_channels[info->channel];
            break;
        }
    }

    if ( !audio_chan ) {
        ado_error_fmt( "invalid pcm" );
        return EINVAL;
    }

    if ( audio_chan->subchn ) {
        info->formats       = 0;
        info->rates         = 0;
        info->min_rate      = 0;
        info->max_rate      = 0;
        info->min_voices    = 0;
        info->max_voices    = 0;
        info->min_fragment_size = 0;
        info->max_fragment_size = 0;
    }

    return EOK;
}

static int32_t rcar_acquire (HW_CONTEXT_T * rcar, PCM_SUBCHN_CONTEXT_T ** pc,
        ado_pcm_config_t * config, ado_pcm_subchn_t * subchn, uint32_t * why_failed)
{
    int status;
    uint32_t chan;
    rcar_audio_channel_t* audio_chan = NULL;

    ado_debug ( DB_LVL_DRIVER, "ACQUIRE" );

    ado_mutex_lock( &rcar->hw_lock );

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        if ( ado_pcm_subchn_is_channel ( subchn, rcar->devices[i].pcm, ADO_PCM_CHANNEL_PLAYBACK ) ) {
            audio_chan = &rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK];
            chan = ADO_PCM_CHANNEL_PLAYBACK;
            *pc = audio_chan;
            break;
        }
        if ( ado_pcm_subchn_is_channel ( subchn, rcar->devices[i].pcm, ADO_PCM_CHANNEL_CAPTURE ) ) {
            audio_chan = &rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE];
            chan = ADO_PCM_CHANNEL_CAPTURE;
            *pc = audio_chan;
            break;
        }
    }

    if ( !audio_chan ) {
        ado_mutex_unlock( &rcar->hw_lock );
        return EINVAL;
    }

    if( audio_chan->subchn ) {
        *why_failed = SND_PCM_PARAMS_NO_CHANNEL;
        ado_mutex_unlock( &rcar->hw_lock );
        ado_error_fmt( "no channel available" );
        return EAGAIN;
    }

    if( rcar->sample_rate_min == rcar->sample_rate_max ) {
        if( config->format.rate != rcar->sample_rate_min ) {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "rate not supported: %d", config->format.rate );
            return EINVAL;
        }
    } else {
        if((config->format.rate > rcar->sample_rate_max) ||
            (config->format.rate < rcar->sample_rate_min)) {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "rate not in range: %d", config->format.rate );
            return EINVAL;
        }
    }

    status = rcar_gpio_route(rcar, chan);
    if(status != EOK) {
        ado_error_fmt("failed to switch to %s route", (chan == ADO_PCM_CHANNEL_PLAYBACK) ? "playback" : "capture");
        return status;
    }

    /* Configure SSIU for either playback or capture */
    status = ssiu_setup( (chan == ADO_PCM_CHANNEL_PLAYBACK) ? 1 : 0, audio_chan->ssi_op_mode,
        rcar->ssi_masterslave_mode, rcar->sample_size, rcar->ssi_voices,
        &rcar->ssi_config, rcar->tdm_exsplit_map);
    if (status != EOK) {
        ado_error_fmt("failed setting up SSIU");
        return status;
    }
    
    status = rcar_set_clock_rate_codec_ssi( rcar, config->format.rate );
    if (status != EOK) {
        ado_mutex_unlock (&rcar->hw_lock);
        ado_error_fmt( "failed setting the codec/SSI clock rate" );
        return status;
    }

    /* Allocate DMA transfer buffer*/
    if (ado_dma_pool_info ( NULL, NULL, NULL, 0) != EOK)
    {
        /* If not using the io-audio memory pool then allocate via the DMA library */
        status = audio_dmac_alloc_buf( audio_chan->dma_context.dma_chn,
                                       &audio_chan->dma_context.addr, config->dmabuf.size, DMA_BUF_FLAG_SHARED );
        if (status != EOK)
        {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "failed allocating shared memory" );
            return status;
       }
       /* Map the allocated DMA buffer to our PCM buffer */
       if (ADO_PCM_BUF_MAP(rcar->card, config, audio_chan->dma_context.addr.paddr,
                           audio_chan->dma_context.addr.len, ADO_BUF_CACHE) == NULL)
       {
            audio_dmac_free_buf(audio_chan->dma_context.dma_chn, &audio_chan->dma_context.addr);
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "failed allocating shared memory" );
            return ENOMEM;
        }
    }
    else {
        // added ADO_BUF_CACHE flag to flags used in old driver
        if( ADO_PCM_BUF_ALLOC( rcar->card, config, config->dmabuf.size,
                               ADO_BUF_DMA_SAFE | ADO_BUF_CACHE ) == NULL ) {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "failed allocating shared memory" );
            return ENOMEM;
        }
    }
    ado_debug( DB_LVL_DRIVER, "dmabuf.size = %d", config->dmabuf.size );

    if ( chan == ADO_PCM_CHANNEL_PLAYBACK ) {
        status = rcar_playback_dma_setup(rcar, audio_chan, &config->dmabuf);
    } else {
        status = rcar_capture_dma_setup(rcar, audio_chan, &config->dmabuf);
    }

    if( status == EOK ) {
        audio_chan->subchn = subchn;
        audio_chan->pcm_config = config;
        *pc = audio_chan;
        audio_chan->state = RCAR_CHN_ACQUIRED;
    } else {
        if (audio_chan->dma_context.addr.paddr) {
            audio_dmac_free_buf(audio_chan->dma_context.dma_chn, &audio_chan->dma_context.addr);
        }
        ADO_PCM_BUF_FREE (rcar->card, config);
        config->dmabuf.addr = NULL;
    }

    ado_mutex_unlock (&rcar->hw_lock);

    return EOK;
}

/* */
/*  Playback release*/
/* */
static int32_t rcar_release (HW_CONTEXT_T * rcar, PCM_SUBCHN_CONTEXT_T * pc,
    ado_pcm_config_t * config)
{
    rcar_audio_channel_t* audio_chan = pc;

    ado_debug( DB_LVL_DRIVER, "RELEASE" );

    ado_mutex_lock (&rcar->hw_lock);

    audio_chan->state = RCAR_CHN_IDLE;
    audio_chan->subchn = NULL;

    if( config->dmabuf.addr ) {
        if (audio_chan->dma_context.addr.paddr) {
            audio_dmac_free_buf(audio_chan->dma_context.dma_chn, &audio_chan->dma_context.addr);
        }
        (void)ADO_PCM_BUF_FREE (rcar->card, config);
        config->dmabuf.addr = NULL;
    }

    ado_mutex_unlock (&rcar->hw_lock);

    return (EOK);
}

static int32_t rcar_prepare (HW_CONTEXT_T * rcar, PCM_SUBCHN_CONTEXT_T * pc, ado_pcm_config_t * config)
{
    int status = EOK;
    rcar_audio_channel_t* audio_chan = pc;

    ado_debug (DB_LVL_DRIVER, "PREPARE");

    ado_mutex_lock (&rcar->hw_lock);

    if ( audio_chan->state == RCAR_CHN_STOPPED ) {
        /* If the audio channel was capturing and got stopped repeat 
         * the DMA setup (although already done in the acquire callback);
         * without doing this, overrun recovery would hang occasionally
         * TODO: investigate with Renesas why this is required
         */
        if ( audio_chan->chan == ADO_PCM_CHANNEL_PLAYBACK ) {
            status = rcar_playback_dma_setup ( rcar, audio_chan, &config->dmabuf );
        } else {
            status = rcar_capture_dma_setup ( rcar, audio_chan, &config->dmabuf );
        }
        if( status != EOK ) {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "failed setting up DMA" );
            return status;
        }
    }

    audio_chan->state = RCAR_CHN_PREPARED;

    ado_mutex_unlock (&rcar->hw_lock);

    return status;
}

static int32_t rcar_playback_trigger (HW_CONTEXT_T * rcar, PCM_SUBCHN_CONTEXT_T * pc, uint32_t cmd)
{
    int idst_status;
    rcar_audio_channel_t* playback = pc;

    ado_debug (DB_LVL_DRIVER, "TRIGGER - cmd : %d", cmd);

    if( cmd == ADO_PCM_TRIGGER_GO ) {
        ado_debug( DB_LVL_DRIVER, "ADO_PCM_TRIGGER_GO" );

        /* Start DMAC */
        ado_debug( DB_LVL_DRIVER, "Start DMAC" );
        audio_dmac_start( playback->dma_context.dma_chn );

        /* Start SSI */
        ssi_start();

        /* Start busif SSI0-y */
        ado_debug( DB_LVL_DRIVER, "Start BUSIF for SSI subchan %d", playback->ssi_subchan );
        ssiu_busif_start(playback->ssi_subchan);
        rcar->active_ssi_subchan_mask |= SSI_SUBCHANNEL_MASK(playback->ssi_subchan);

        playback->state = RCAR_CHN_STARTED;

        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_START complete");

    } else {
        bool skipped_ssi_stop = false;

        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_STOP cmd %d", cmd);

        playback->state = RCAR_CHN_STOPPED;

        /* Stop busif SSI0-y if in TDM split mode or ex-split mode, before stopping the SSI */
        if( playback->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
            playback->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
            playback->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
            playback->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            ado_debug( DB_LVL_DRIVER, "Stop BUSIF for subchan %d", playback->ssi_subchan );
            ssiu_busif_stop(playback->ssi_subchan);
            rcar->active_ssi_subchan_mask &= ~SSI_SUBCHANNEL_MASK(playback->ssi_subchan);
        }

        if ( rcar->active_ssi_subchan_mask )
            skipped_ssi_stop = true;
        else {
            ado_debug( DB_LVL_DRIVER, "Stop SSI");
            ssi_stop();
        }

        if ( !skipped_ssi_stop ) {
            /* Wait for idle mode*/
            ado_debug( DB_LVL_DRIVER, "Waiting for IDST on SSI" );
            idst_status = ssi_wait_status( SSISR_IDST_MASK);
            if (idst_status != EOK) {
                ado_error_fmt("SSI timed out waiting for IDST");
            }
            ssi_stop_idle_int();
        }

        /* Stop busif if busif not tdm split/exsplit mode, after stopping the SSI */
        if( playback->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XMONO &&
            playback->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XSTEREO &&
            playback->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_8CHAN &&
            playback->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            ado_debug( DB_LVL_DRIVER, "Stop BUSIF subchan %d", playback->ssi_subchan );
            ssiu_busif_stop( playback->ssi_subchan);
            rcar->active_ssi_subchan_mask &= ~SSI_SUBCHANNEL_MASK(playback->ssi_subchan);
        }

        ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC" );
        audio_dmac_stop(playback->dma_context.dma_chn);

        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_STOP complete");
    }

    if( rcar->debug ) {
        rcar_register_dump( rcar );
    }

    return EOK;
}

static int32_t rcar_capture_trigger (HW_CONTEXT_T * rcar, PCM_SUBCHN_CONTEXT_T * pc, uint32_t cmd)
{
    rcar_audio_channel_t* capture = pc;

    ado_debug (DB_LVL_DRIVER, "TRIGGER - cmd: %d", cmd);

    if (cmd == ADO_PCM_TRIGGER_GO) {
        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_GO");

        /* Start DMAC */
        ado_debug( DB_LVL_DRIVER, "Start DMAC" );
        audio_dmac_start(capture->dma_context.dma_chn);

        /* Start busif SSI0-y if in TDM split mode or TDM ex-split mode, after starting the SSI */
        if( capture->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XMONO &&
            capture->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XSTEREO &&
            capture->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_8CHAN &&
            capture->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            /* Start BUSIF0-0 */
            ado_debug( DB_LVL_DRIVER, "Start BUSIF for SSI subchan 0");
            ssiu_busif_start(0);
            rcar->active_ssi_subchan_mask = SSI_SUBCHANNEL_MASK(0);
        }

        /* Start SSI */
        ado_debug( DB_LVL_DRIVER, "Start of SSI" );
        ssi_start();

         /* start busif SSI0-y if in TDM split mode or TDM ex-split mode, after starting the SSI */
        if( capture->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
            capture->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
            capture->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
            capture->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            ado_debug( DB_LVL_DRIVER, "Start BUSIF for SSI subchan %d", capture->ssi_subchan );
            ssiu_busif_start(capture->ssi_subchan);
            rcar->active_ssi_subchan_mask |= SSI_SUBCHANNEL_MASK(capture->ssi_subchan);
        }

        capture->state = RCAR_CHN_STARTED;

    } else {
        bool skipped_ssi_stop = false;

        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_STOP cmd %d", cmd);

        capture->state = RCAR_CHN_STOPPED;

        /* Stop busif SSI0-y */
        ado_debug( DB_LVL_DRIVER, "Stop BUSIF for SSI subchan %d",  capture->ssi_subchan );
        ssiu_busif_stop(capture->ssi_subchan);
        rcar->active_ssi_subchan_mask &= ~SSI_SUBCHANNEL_MASK(capture->ssi_subchan);

        if ( rcar->active_ssi_subchan_mask )
            skipped_ssi_stop = true;
        else {
            ado_debug( DB_LVL_DRIVER, "Stop SSI" );
            ssi_stop();
        }

        if ( !skipped_ssi_stop ) {
            /* Wait for idle mode*/
            ado_debug( DB_LVL_DRIVER, "Waiting for IDST on SSI");
            ssi_wait_status(SSISR_IDST_MASK);
        }

        ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC" );
        audio_dmac_stop(capture->dma_context.dma_chn);
    }

    if( rcar->debug ) {
        rcar_register_dump( rcar );
    }

    return EOK;
}

static uint32_t rcar_position
(
    HW_CONTEXT_T * rcar,
    PCM_SUBCHN_CONTEXT_T * pc,
    ado_pcm_config_t * config,
    uint32_t * hw_buffer_level
)
{
    uint32_t bytes_left = 0;
    uint32_t pos;

    if( hw_buffer_level ) {
        *hw_buffer_level = 0;
    }

    ado_mutex_lock (&rcar->hw_lock);

    audio_dmac_count_register_get(pc->dma_context.dma_chn, &bytes_left);

    ado_mutex_unlock (&rcar->hw_lock);

    pos = config->dmabuf.size - bytes_left;

    ado_debug (DB_LVL_PCM, "position=%d/%d", pos, config->dmabuf.size);

    return pos;
}

static void rcar_interrupt ( HW_CONTEXT_T * rcar, int32_t irq )
{
    int status = EOK;
    rcar_audio_channel_t* audio_chan = NULL;

    /* NOTE: do not use locks in this handler!! If the interrupt flag is
     * not cleared before the next interrupt should occur the hardware
     * will just stop sending interrupts without notifying the driver.
     * TODO: detect this state and call ado_pcm_error
     */
    ado_debug ( DB_LVL_INTERRUPT, "irq=%d", irq );

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            if ( rcar->devices[i].audio_channels[chan].dma_context.dma_irq == irq ) {
                audio_chan = &rcar->devices[i].audio_channels[chan];
                break;
            }
        }
    }

    if ( !audio_chan ) {
        ado_error_fmt ( "irq %d not in use", irq );
        return;
    }

    /* Clear Interrupt status */
    status = audio_dmac_cleanup ( audio_chan->dma_context.dma_chn );

    if ( status == EOK ) {
        /* Signal to io-audio (DMA transfer was completed) */
        dma_interrupt ( audio_chan->subchn );
    } else {
        ado_pcm_error ( audio_chan->subchn, ADO_PCM_STATUS_ERROR );
    }
}

static rcar_audio_channel_t* rcar_new_audio_chan
(
    rcar_context_t * rcar,
    uint32_t is_transmit
)
{
    rcar_audio_channel_t* audio_chan = NULL;
    uint32_t chan = is_transmit ? ADO_PCM_CHANNEL_PLAYBACK : ADO_PCM_CHANNEL_CAPTURE;

    // Pick the next available channel
    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        if ( rcar->devices[i].audio_channels[chan].state == RCAR_CHN_IDLE ) {
            audio_chan = &rcar->devices[i].audio_channels[chan];
            break;
        }
    }

    return audio_chan;
}

static int rcar_audio_chan_config
(
    rcar_context_t * rcar,
    uint32_t is_transmit,
    uint32_t voice_map,
    ssi_op_mode_t ssi_op_mode
)
{
    uint32_t voices_exsplit_mode = 0;
    uint32_t max_ssi_subchan = 0;
    uint32_t voices = 0;
    uint32_t ssi_voices = rcar->ssi_voices;
    uint32_t sample_size = rcar->sample_size;
    uint32_t slot_size = rcar->slot_size;

    ado_debug ( DB_LVL_DRIVER, "is_transmit=%d, voice_map=%d, ssi_op_mode=%d",
        is_transmit, voice_map, ssi_op_mode);

    voices_exsplit_mode = ( voice_map & 0xFFFFFFE0 ) != 0 ? 1 : 0;
    if ( !voices_exsplit_mode ) {
        voices = voice_map;
    } else {
        voices = 0;
        for ( uint32_t i = 0; i < SSI_SUB_CHANNEL_NUM*4; i+=4 ) {
            voices += ( voice_map >> i ) & 0xF;
        }
    }

    /* Refine TDM modes and validate against number of voices if specified */
    if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT ) {
        if ( !voices ) {
            ado_error_fmt ( "voices need to be specified in TDM exsplit mode" );
            return EINVAL;
        }
        if ( !voices_exsplit_mode ) {
            ado_error_fmt ( "voice map 0x%x incompatible with TDM exsplit mode", voice_map );
            return EINVAL;
        }
        if ( voices == 8 ) {
            ssi_op_mode = SSI_OP_MODE_TDMEXSPLIT_8CHAN;
        } else if ( voices == 16 ) {
            ssi_op_mode = SSI_OP_MODE_TDMEXSPLIT_16CHAN;
        } else {
            ado_error_fmt ( "%d voices not supported in TDM exsplit mode", voices );
            return EINVAL;
        }
        if ( !ssi_voices ) {
            ssi_voices = voices;
        } else {
            if ( ssi_voices != voices ) {
                ado_error_fmt ( "voices %d does not match previously determined ssi_voices %d",
                    voices, ssi_voices );
                return EINVAL;
            }
        }
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT ) {
        if ( !voices ) {
            ado_error_fmt ( "voices need to be specified in TDM split mode" );
            return EINVAL;
        }
        if ( voices_exsplit_mode ) {
            ado_error_fmt ( "voice map 0x%x incompatible with TDM split mode", voice_map );
            return EINVAL;
        }
        if ( voices == 4 ) {
            ssi_op_mode = SSI_OP_MODE_TDMSPLIT_4XMONO;
        } else if ( voices == 8 ) {
            ssi_op_mode = SSI_OP_MODE_TDMSPLIT_4XSTEREO;
        } else {
            ado_error_fmt ( "%d voices not supported in TDM split mode", voices );
            return EINVAL;
        }
        if ( !ssi_voices ) {
            ssi_voices = voices;
        } else {
            if ( ssi_voices != voices ) {
                ado_error_fmt ( "voices %d does not match previously determined ssi_voices %d",
                    voices, ssi_voices );
                return EINVAL;
            }
        }
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMEXT && voices != 0 ) {
        if ( !voices ) {
            ado_error_fmt ( "voices need to be specified in TDM exsplit mode" );
            return EINVAL;
        }
        if ( voices_exsplit_mode ) {
            ado_error_fmt ( "voice map 0x%x incompatible with TDM ext mode", voice_map );
            return EINVAL;
        }
        if ( voices != 6 && voices != 8 ) {
            ado_error_fmt( "%d voices not supported in TDM ext mode", voices );
            return EINVAL;
        }
        if ( !ssi_voices ) {
            ssi_voices = ( voices == 6 ? 8 : 6 );
        } else {
            if ( ssi_voices != ( voices == 6 ? 8 : 6 ) ) {
                ado_error_fmt( "voices %d does not match previously determined ssi_voices %d",
                    ( voices == 6 ? 8 : 6 ), ssi_voices );
                return EINVAL;
            }
        }
    } else {
        if ( voices ) {
            ssi_voices = voices;
        } else {
            /* if voices not inferred from cmd line params, use variant.h default value,
             * except for the case that the op_mode was specified or inferred from cmd line params,
             * and it is different from the variant.h default op_mode */
            if ( ssi_op_mode != SSI_OP_MODE_NUM && ssi_op_mode != DEFAULT_SSI_OP_MODE ) {
                ado_error_fmt( "Number of voices needs to be specified for op_mode %d", ssi_op_mode );
                return EINVAL;
            } else {
                ssi_voices = voices = DEFAULT_VOICES;
            }
        }
    }

    /* if ssi_op_mode not specified or inferred from cmd line params, use variant.h default value */
    if ( ssi_op_mode == SSI_OP_MODE_NUM ) {
        if ( ssi_voices == DEFAULT_VOICES ) {
            ssi_op_mode = DEFAULT_SSI_OP_MODE;
        } else if (ssi_voices == 1) {
            ssi_op_mode = SSI_OP_MODE_MONO;
        } else if (ssi_voices == 2) {
            ssi_op_mode = SSI_OP_MODE_STEREO;
        } else if (ssi_voices == 4 || ssi_voices == 6 || ssi_voices == 8) {
            ssi_op_mode = SSI_OP_MODE_TDM;
        } else if (ssi_voices == 16) {
            ssi_op_mode = SSI_OP_MODE_TDM_16CHAN;
        } else {
            ado_error_fmt( "op_mode can not be determined for voices %d", ssi_voices );
            return EINVAL;
        }
    }

    /* If voices and op_mode were specified or inferred from command line params, they should be fine.
       However, if voices and/or op_mode use variant.h default values, we need to validate
       that voices and op_mode are consistent */
    if ( ( ( ssi_op_mode == SSI_OP_MODE_TDMEXT ) && ( ssi_voices != 6 && ssi_voices != 8 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ) && ( ssi_voices != 4 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
             ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) && ( ssi_voices != 8 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ||
             ssi_op_mode == SSI_OP_MODE_TDM_16CHAN ) && ( ssi_voices != 16 ) ) ||
         (  ssi_op_mode == SSI_OP_MODE_STEREO &&
         ( ssi_voices != 2 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_MONO ) && ( ssi_voices != 1 ) ) ) {
        ado_error_fmt( "op_mode %d and voices %d are inconsistent", ssi_op_mode, ssi_voices );
        return EINVAL;
    }

    /* Check if the sample_size is supported for the determined op mode */
    if ( ssi_op_mode == SSI_OP_MODE_TDMEXT ||
         ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
         ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
        if ( sample_size != 0 && sample_size != 16 && sample_size != 24 ) {
            ado_error_fmt("Sample size %d is not supported with op mode %d", sample_size, ssi_op_mode);
            return EINVAL;
        }
        if ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
             ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
            if ( slot_size == 0 ) {
                slot_size = 32;
            } else if ( slot_size != 32 ) {
                ado_error_fmt("Slot size %d is not supported with op mode %d", slot_size, ssi_op_mode);
                return EINVAL;
            }
        }
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ||
                ssi_op_mode == SSI_OP_MODE_TDM_16CHAN ) {
        if ( sample_size == 0 ) {
            sample_size = 16;
        } else if (( sample_size != 8 ) || (sample_size != 16)) {
            ado_error_fmt("Sample size %d is not supported with op mode %d", sample_size, ssi_op_mode);
            return EINVAL;
        }
        if ( slot_size == 0 ) {
            slot_size = 16;
        } else if ( slot_size != 16 ) {
            ado_error_fmt("Slot size %d is not supported with op mode %d", slot_size, ssi_op_mode);
            return EINVAL;
        }
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) {
        if ( sample_size != 0 && sample_size != 8 && sample_size != 16 && sample_size != 24 ) {
            ado_error_fmt("Sample size %d is not supported with op mode %d", sample_size, ssi_op_mode);
            return EINVAL;
        }
    }

    /* if sample_size and slot_size not specified among options, use the default values; handle the case that
     * one of sample_size and slot_size specified and make sure the defaulted one satisfies the condition
     * sample_size is less equal the slot size
     */
    if ( !sample_size && !slot_size ) {
        sample_size = DEFAULT_SAMPLE_SIZE;
        slot_size = DEFAULT_SLOT_SIZE;
    } else {
        if ( !sample_size ) {
            sample_size = slot_size >= DEFAULT_SAMPLE_SIZE ? DEFAULT_SAMPLE_SIZE : slot_size;
        }
        if ( !rcar->slot_size ) {
            slot_size = sample_size <= DEFAULT_SLOT_SIZE ? DEFAULT_SLOT_SIZE : sample_size;
        }
    }

    /* Validate that the sample_size is less equal the slot size */
    if ( sample_size > slot_size ) {
        ado_error_fmt("Sample size %d is greater than the slot size %d", sample_size, slot_size);
        return EINVAL;
    }

    rcar->ssi_voices = ssi_voices;
    rcar->sample_size = sample_size;
    rcar->slot_size = slot_size;

    if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        max_ssi_subchan = SSI_SUB_CHANNEL_7;
        rcar->tdm_exsplit_map = voice_map;
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) {
        max_ssi_subchan = SSI_SUB_CHANNEL_3;
        rcar->tdm_exsplit_map = voice_map;
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
                ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
        max_ssi_subchan = SSI_SUB_CHANNEL_3;
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        max_ssi_subchan = SSI_SUB_CHANNEL_7;
    } else {
        max_ssi_subchan = SSI_SUB_CHANNEL_0;
    }

    for ( uint32_t i = SSI_SUB_CHANNEL_0; i <= max_ssi_subchan; i++ ) {
        uint32_t subchan_voices = 0;
        if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
             ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            subchan_voices = ( voice_map >> (i << 2) ) & 0xF;
        } else if ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ) {
            subchan_voices = 1;
        } else if ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
            subchan_voices = 2;
        } else {
            subchan_voices = voices;
        }
        if ( subchan_voices ) {
            rcar_audio_channel_t* audio_chan = rcar_new_audio_chan ( rcar, is_transmit );
            audio_chan->ssi_op_mode = ssi_op_mode;
            audio_chan->voices = subchan_voices;
            audio_chan->ssi_subchan = i;
            audio_chan->state = RCAR_CHN_STOPPED;
        }
    }

    return EOK;
}

static int rcar_parse_commandline (rcar_context_t * rcar, char *args)
{
    int      opt = 0;
    char     *value;
    int      numvalue = 0;

    /* Local variables to hold ssi options specified in io-audio conf file, or
     * their default value from the variant.h file
     */
    ssi_op_mode_t tx_ssi_op_mode = SSI_OP_MODE_NUM;
    ssi_op_mode_t rx_ssi_op_mode = SSI_OP_MODE_NUM;
    uint32_t tx_voice_map = 0;
    uint32_t rx_voice_map = 0;

    char     *opts[] = {
    #define RCAR_OPT_TX_VOICES         0
        "tx_voices",        // e.g. tx_voices=8, tx_voices=2, tx_voices=4:0:2:2, specifies number of playback voices used
    #define RCAR_OPT_RX_VOICES         1
        "rx_voices",        // e.g. rx_voices=8, rx_voices=2, rx_voices=4:0:2:2, specifies number of capture voices used
    #define RCAR_OPT_TX_OP_MODE        2
        "tx_op_mode",       // "tx_op_mode" indicates a special TDM op mode used for playback, possible values are "tdm_ext",
                            // "tdm_split", tdm_exsplit, while other op modes are inferred from the number of voices
    #define RCAR_OPT_RX_OP_MODE        3
        "rx_op_mode",       // "rx_op_mode" indicates a special TDM op mode used for capture, possible values are "tdm_ext",
                            // "tdm_split", tdm_exsplit, while other op modes are inferred from the number of voices
    #define RCAR_OPT_CLK_MODE          4
        "clk_mode",         // clk_mode="master" indicates that SSI configured as master, clk_mode="slave" indicates that SSI
                            // configured as slave
    #define RCAR_OPT_SAMPLE_SIZE       5
        "sample_size",
    #define RCAR_OPT_CLK_POL           6
        "clk_pol",
    #define RCAR_OPT_FSYNC_POL         7
        "fsync_pol",
    #define RCAR_OPT_BIT_DELAY         8
        "bit_delay",
    #define RCAR_OPT_SAMPLE_RATE       9
        "sample_rate",
    #define RCAR_OPT_SAMPLE_RATE_LIST  10
        "sample_rate_list",
    #define RCAR_OPT_SLOT_SIZE         11
        "slot_size",
    #define RCAR_OPT_MCLK              12
        "mclk",
    #define RCAR_OPT_MCLK2             13
        "mclk2",
    #define RCAR_OPT_MCLK_SRC          14
        "mclk_src",
    #define RCAR_OPT_MCLK_FS           15
        "mclk_fs",
    #define RCAR_OPT_ROUTE_NAME        16
        "route_name",
    #define RCAR_OPT_ROUTE_PORT        17
        "route_bit",
    #define RCAR_OPT_DEBUG             18
        "debug",            // no params, refers to the use of register dumps at the start and end of
                            // playback or capture sessions
        NULL
    };

    ado_debug ( DB_LVL_DRIVER, "" );

    /* Driver option related data members */
    rcar->ssi_masterslave_mode             = SSI_MS_MODE_NUM;
    rcar->sample_rate_min                  = SAMPLE_RATE_MIN;  /* see variant.h */
    rcar->sample_rate_max                  = SAMPLE_RATE_MAX;  /* see variant.h */
    rcar->sample_rates                     = 0;
    rcar->slot_size                        = 0;
    rcar->sample_size                      = 0;
    rcar->ssi_config.clk_pol               = SSI_BIT_CLK_POL_RISING;
    rcar->ssi_config.ws_pol                = SSI_WS_POL_0;
    //rcar->ssi_config.bit_delay             = SSI_BIT_DELAY_ONE;
    rcar->ssi_config.bit_delay             = SSI_BIT_DELAY_NONE;
    rcar->ssi_config.padding_pol           = SSI_PADDING_POL_LOW;
    rcar->ssi_config.serial_data_alignment = SSI_SER_DATA_ALIGN_DATA_FIRST;
    rcar->ssi_config.sys_word_length       = SSI_SYS_WORD_LEN_16BIT_STEREO;
    rcar->ssi_config.data_word_length      = SSI_DATA_WORD_LEN_16BIT;
    rcar->mclk                             = MCLK;          /* see variant.h */
    rcar->mclk2                            = 0;
    rcar->mclk_fs                          = MCLK_FS;       /* see variant.h */
    rcar->mclk_src                         = AUDIO_CLK_NONE;
    rcar->debug                            = 0;             /* by default, no register dumps */
    rcar->init_flags                       = 0;
    rcar->ssi_voices                       = 0;
    rcar->tdm_exsplit_map                  = 0;
    rcar->route_name                       = NULL;
    rcar->route_port                       = -1;

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].ssi_op_mode             = SSI_OP_MODE_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].ssi_op_mode              = SSI_OP_MODE_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].state                   = RCAR_CHN_IDLE;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].state                    = RCAR_CHN_IDLE;
    }

    /* Sets options to each values */
    while ( args != NULL && args[0] != 0 ) {
        opt = getsubopt ( &args, opts, &value );

        /* Handle in a single place the case where an option that would need a value
           to be specified is missing the value */
        switch ( opt ) {
            case RCAR_OPT_TX_VOICES:
            case RCAR_OPT_RX_VOICES:
            case RCAR_OPT_TX_OP_MODE:
            case RCAR_OPT_RX_OP_MODE:
            case RCAR_OPT_CLK_MODE:
            case RCAR_OPT_SAMPLE_SIZE:
            case RCAR_OPT_CLK_POL:
            case RCAR_OPT_FSYNC_POL:
            case RCAR_OPT_BIT_DELAY:
            case RCAR_OPT_SAMPLE_RATE:
            case RCAR_OPT_SAMPLE_RATE_LIST:
            case RCAR_OPT_SLOT_SIZE:
            case RCAR_OPT_MCLK:
            case RCAR_OPT_MCLK2:
            case RCAR_OPT_MCLK_FS:
            case RCAR_OPT_MCLK_SRC:
            case RCAR_OPT_ROUTE_NAME:
            case RCAR_OPT_ROUTE_PORT:
                if ( value == NULL ) {
                    ado_error_fmt ( "Option %s requires a value", opts[opt] );
                    return EINVAL;
                }
                break;
            default:
                break;
        }
        switch ( opt ) {
            case RCAR_OPT_TX_VOICES:
            {
                /* tx_voices can be either a single numeric value, or a number of numeric values separated by ':';
                 * the latter option is used to specify a voice map for the tdm exsplit mode; the option value is
                 * saved in the tx_voice_map that can hold a unique value that can be one of 1, 2, 4, 6, 8 or 16, or
                 * can hold 8 numeric values that can be 0, 2, 4, 6, 8 or 10, each stored in a 4-bit field, and each
                 * corresponding to a busif subchannel voices allocation for tdm exsplit mode
                 * Note: In TDM exsplit mode, the voice map can have 0 voices for some subchannels (meaning subchannels
                   not used)
                 */
                char *str = value;
                uint32_t i = 0;
                uint32_t tdm_exsplit_mode = (strchr(str, ':') != NULL);
                tx_voice_map = 0;
                if ( tdm_exsplit_mode ) {
                    while ( i < 32 ) {
                        numvalue = strtol( str, &str, 0 );
                        if ( numvalue == 0 || numvalue == 2 || numvalue == 4 ||
                             numvalue == 6 || numvalue == 8 || numvalue == 10 ) {
                            tx_voice_map |= ( numvalue << i );
                            if ( !str || !*str ) break;
                            if ( *str != ':' ) {
                                ado_error_fmt( "Invalid character %c in playback voices", *str );
                                return EINVAL;
                            }
                            str++;
                            i += 4;
                        } else {
                            ado_error_fmt( "Invalid subchannel voices %d for tdm exsplit mode", numvalue );
                            return EINVAL;
                        }
                    }
                } else {
                    numvalue = strtol( str, &str, 0 );
                    if ( numvalue == 1 || numvalue == 2 || numvalue == 4 ||
                         numvalue == 6 || numvalue == 8 || numvalue == 16 ) {
                        tx_voice_map = numvalue;
                    } else {
                        ado_error_fmt( "Invalid voices %d", numvalue );
                        return EINVAL;
                    }
                }
                ado_debug( DB_LVL_DRIVER, "playback voices map 0x%x", tx_voice_map );
                break;
            }
            case RCAR_OPT_RX_VOICES:
            {
                /* rx_voices can be either a single numeric value, or a number of numeric values separated by ':';
                 * the latter option is used to specify a voice map for the tdm exsplit mode; the option value is
                 * saved in the rx_voice_map that can hold a unique value that can be one of 1, 2, 4, 6, 8 or 16, or
                 * can hold 8 numeric values that can be 0, 2, 4, 6, 8 or 10, each stored in a 4-bit field, and each
                 * corresponding to a busif subchannel voices allocation for tdm exsplit mode
                 * Note: In TDM exsplit mode, the voice map can have 0 voices for some subchannels (meaning subchannels
                   not used)
                 */
                char *str = value;
                uint32_t i = 0;
                uint32_t tdm_exsplit_mode = (strchr(str, ':') != NULL);
                rx_voice_map = 0;
                if ( tdm_exsplit_mode ) {
                    while ( i < 32 ) {
                        numvalue = strtol( str, &str, 0 );
                        if ( numvalue == 0 || numvalue == 2 || numvalue == 4 ||
                             numvalue == 6 || numvalue == 8 || numvalue == 10 ) {
                            rx_voice_map |= ( numvalue << i );
                            if ( !str || !*str ) break;
                            if ( *str != ':' ) {
                                ado_error_fmt( "Invalid character %c in capture voices", *str );
                                return EINVAL;
                            }
                            str++;
                            i += 4;
                        } else {
                            ado_error_fmt( "Invalid subchannel voices %d for tdm exsplit mode", numvalue );
                            return EINVAL;
                        }
                    }
                } else {
                    numvalue = strtol( str, &str, 0 );
                    if ( numvalue == 1 || numvalue == 2 || numvalue == 4 ||
                         numvalue == 6 || numvalue == 8 || numvalue == 16 ) {
                        rx_voice_map = numvalue;
                    } else {
                        ado_error_fmt( "Invalid voices %d", numvalue );
                        return EINVAL;
                    }
                }
                ado_debug( DB_LVL_DRIVER, "capture voices map 0x%x", rx_voice_map );
                break;
            }
            case RCAR_OPT_CLK_MODE:
                if ( strcmp( value, "master") == 0 ) {
                    rcar->ssi_masterslave_mode = SSI_MS_MODE_MASTER;
                } else if ( strcmp( value, "slave") == 0 ) {
                    rcar->ssi_masterslave_mode = SSI_MS_MODE_SLAVE;
                } else {
                    ado_error_fmt( "Invalid clk_mode %s", value );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "clk_mode %s: %d", value, rcar->ssi_masterslave_mode );
                break;
            case RCAR_OPT_TX_OP_MODE:
                if ( strcmp( value, "tdm_ext") == 0 ) {
                    tx_ssi_op_mode = SSI_OP_MODE_TDMEXT;
                } else if ( strcmp( value, "tdm_split") == 0 ) {
                    tx_ssi_op_mode = SSI_OP_MODE_TDMSPLIT;
                } else if ( strcmp( value, "tdm_exsplit") == 0 ) {
                    tx_ssi_op_mode = SSI_OP_MODE_TDMEXSPLIT;
                } else {
                    ado_error_fmt( "Invalid tx_op_mode %s", value );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "tx_op_mode %s: %d", value, tx_ssi_op_mode );
                break;
            case RCAR_OPT_RX_OP_MODE:
                if ( strcmp( value, "tdm_ext") == 0 ) {
                    rx_ssi_op_mode = SSI_OP_MODE_TDMEXT;
                } else if ( strcmp( value, "tdm_split") == 0 ) {
                    rx_ssi_op_mode = SSI_OP_MODE_TDMSPLIT;
                } else if ( strcmp( value, "tdm_exsplit") == 0 ) {
                    rx_ssi_op_mode = SSI_OP_MODE_TDMEXSPLIT;
                } else {
                    ado_error_fmt( "Invalid rx_op_mode %s", value );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "rx_op_mode %s: %d", value, rx_ssi_op_mode );
                break;
            case RCAR_OPT_SAMPLE_SIZE:
                numvalue = strtol( value, NULL, 0 );
                if ( numvalue == 16 || numvalue == 24 || numvalue == 32 ) {
                    rcar->sample_size = numvalue;
                } else {
                    ado_error_fmt( "Invalid sample size: %d", numvalue );
                    return EINVAL;
                }
                break;
            case RCAR_OPT_CLK_POL:
                numvalue = strtol( value, NULL, 0 );

                if ( numvalue == 0 ) {
                    rcar->ssi_config.clk_pol = SSI_BIT_CLK_POL_RISING;
                } else if( numvalue == 1 ) {
                    rcar->ssi_config.clk_pol = SSI_BIT_CLK_POL_FALLING;
                } else {
                    ado_error_fmt( "Invalid clk pol: %d", numvalue );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "Set clock polarity %s",
                           numvalue ? "FALLING" : "RISING" );
                break;
            case RCAR_OPT_FSYNC_POL:
                numvalue = strtol( value, NULL, 0 );

                if ( numvalue == 0 ) {
                    rcar->ssi_config.ws_pol = SSI_WS_POL_0;
                } else if ( numvalue == 1 ) {
                    rcar->ssi_config.ws_pol = SSI_WS_POL_1;
                } else {
                    ado_error_fmt( "Invalid ws pol: %d", numvalue );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "Set WS polarity %d", rcar->ssi_config.ws_pol );
                break;
            case RCAR_OPT_BIT_DELAY:
                numvalue = strtol( value, NULL, 0 );

                if ( numvalue == 0 ) {
                    rcar->ssi_config.bit_delay = SSI_BIT_DELAY_NONE;
                } else if ( numvalue == 1 ) {
                    rcar->ssi_config.bit_delay = SSI_BIT_DELAY_ONE;
                } else {
                    ado_error_fmt( "Invalid bit delay: %d", numvalue );
                    return EINVAL;
                }
                ado_debug (DB_LVL_DRIVER, "Set bit delay %s",
                           numvalue ? "ONE" : "NONE");
                break;
            case RCAR_OPT_SAMPLE_RATE: // min and max sample rate, separated by ':'
            {
                char *value2;
                int32_t rates[2];
                uint32_t rate_flag;
                int32_t rate;

                rates[0] = rates[1] = strtoul(value, 0, 0);
                if ((value2 = strchr(value, ':')) != NULL) {
                     rates[1] = strtoul(value2 + 1, 0, 0);
                }

                if ( ( rate_flag = ado_pcm_rate2flag(rates[0]) ) == 0 ) {
                    ado_error_fmt( "Invalid min_rate in sample_rate: %d", rates[0] );
                    return EINVAL;
                }

                if ( ( rate_flag & supported_rate_list ) == 0 ) {
                    ado_error_fmt( "Unsupported min_rate in sample_rate: %d", rates[0] );
                    return EINVAL;
                }

                if ( rates[1] < rates[0] ||
                     ( rate_flag = ado_pcm_rate2flag(rates[1]) ) == 0 ) {
                    ado_error_fmt( "Invalid max_rate in sample_rate: %d", rates[1] );
                    return EINVAL;
                }

                if ( ( rate_flag & supported_rate_list ) == 0 ) {
                    ado_error_fmt( "Unsupported max_rate in sample_rate: %d", rates[0] );
                    return EINVAL;
                }

                rcar->sample_rate_min = rates[0];
                rcar->sample_rate_max = rates[1];

                rate_flag = SND_PCM_RATE_8000;
                while (true) {
                    rate = ado_pcm_flag2rate(rate_flag);
                    if (!rate) break;
                    if ( (rate_flag & supported_rate_list) &&
                         (rate >= rcar->sample_rate_min) && (rate <= rcar->sample_rate_max) ) {
                        rcar->sample_rates |= rate_flag;
                    }
                    rate_flag <<= 1;
                }

                ado_debug( DB_LVL_DRIVER, "sample rate min %d, max %d, rate_list %x",
                           rcar->sample_rate_min, rcar->sample_rate_max, rcar->sample_rates );
                break;
            }
            case RCAR_OPT_SAMPLE_RATE_LIST: // all supported sample rates, separated by ':'
            {
                int32_t rates[RCAR_NUM_SUPPORTED_RATES];
                uint32_t n = 0, i = 0;
                uint32_t rate_flag;

                while (value && n < sizeof(rates)/sizeof(uint32_t)) {
                    if (n > 0) value++;  // skip over separator
                    rates[n++] = strtoul(value, &value, 0);
                    value = strchr(value, ':');  // find next separator
                }

                if ( n == 0 ) {
                    ado_error_fmt( "No valid rate in sample_rate_list: %s", value );
                    return EINVAL;
                }

                rcar->sample_rate_min = rates[0];
                rcar->sample_rate_max = rates[0];

                for ( i = 0; i < n; i++ ) {
                    if ( ( rate_flag = ado_pcm_rate2flag(rates[i]) ) == 0 ) {
                        ado_error_fmt( "Invalid rate in sample_rate_list: %d", rates[i] );
                        return EINVAL;
                    }
                    if ( ( rate_flag & supported_rate_list ) == 0 ) {
                        ado_error_fmt( "Unsupported rate in sample_rate_list: %d", rates[i] );
                        return EINVAL;
                    }
                    rcar->sample_rates |= rate_flag;
                    if ( rates[i] < rcar->sample_rate_min ) {
                        rcar->sample_rate_min = rates[i];
                    } else if( rates[i] > rcar->sample_rate_max ) {
                        rcar->sample_rate_max = rates[i];
                    }
                }

                ado_debug( DB_LVL_DRIVER, "sample rate min %d, max %d, rate_list %x",
                           rcar->sample_rate_min, rcar->sample_rate_max, rcar->sample_rates );
                break;
            }
            case RCAR_OPT_SLOT_SIZE:
                numvalue = strtol( value, NULL, 0 );
                if ( numvalue == 16 || numvalue == 32 ) {
                    rcar->slot_size = numvalue;
                } else {
                    ado_error_fmt( "Invalid slot_size %d", numvalue );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "slot_size %d", rcar->slot_size );
                break;
            case RCAR_OPT_MCLK:
                numvalue = strtol( value, NULL, 0 );
                rcar->mclk = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk %d", rcar->mclk );
                break;
            case RCAR_OPT_MCLK2:
                numvalue = strtol( value, NULL, 0 );
                rcar->mclk2 = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk2 %d", rcar->mclk2 );
                break;
            case RCAR_OPT_MCLK_SRC:
                numvalue = strtol( value, NULL, 0 );
                if (numvalue < AUDIO_CLK_NONE || numvalue > AUDIO_CLKB) {
                    ado_error_fmt("Invalid mclk source value %d", numvalue);
                    return EINVAL;
                }
                rcar->mclk_src = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk_src %d", rcar->mclk_src );
                break;
            case RCAR_OPT_MCLK_FS:
                numvalue = strtol( value, NULL, 0 );
                rcar->mclk_fs = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk_fs %d", rcar->mclk_fs );
                break;
            case RCAR_OPT_ROUTE_NAME:
                if (strlen(value)) {
                    int len = strlen(value) + 1;
                    rcar->route_name = (char *)malloc(len);
                    snprintf(rcar->route_name, len, "%s", value);
                    ado_debug( DB_LVL_DRIVER, "route_name %s", rcar->route_name );
                }
                break;
            case RCAR_OPT_ROUTE_PORT:
                numvalue = strtol( value, NULL, 0 );
                rcar->route_port = numvalue;
                ado_debug( DB_LVL_DRIVER, "route_port %d", rcar->route_port );
                break;
            case RCAR_OPT_DEBUG:
                rcar->debug = 1;
                ado_debug( DB_LVL_DRIVER, "debug mode is on" );
                break;
            default:
                ado_error_fmt ( "invalid option %s", value );
                return EINVAL;
        }
    }
    
    /* For playing back */
    if ( rcar_audio_chan_config ( rcar, 1, tx_voice_map, tx_ssi_op_mode ) != EOK ) {
        return EINVAL;
    }

    /* For capturing */
    if ( rcar_audio_chan_config ( rcar, 0, rx_voice_map, rx_ssi_op_mode ) != EOK ) {
        return EINVAL;
    }

    /* If masterslave_mode not specified among options, use the default value */
    if ( rcar->ssi_masterslave_mode == SSI_MS_MODE_NUM ) {
        rcar->ssi_masterslave_mode = DEFAULT_SSI_MASTERSLAVE_MODE;
    }

    /* If RCAR_OPT_SAMPLE_RATE or RCAR_OPT_SAMPLE_RATE_LIST options not specified,
     * the sample rate range is dictated by SAMPLE_RATE_MIN and SAMPLE_RATE_MAX
     * defined in variant.h, rcar->sample_rate_min and rcar->sample_rate_max are
     * initialized to these values */
    if ( !rcar->sample_rates ) {
        int32_t rate;
        uint32_t rate_flag;

        rate_flag = SND_PCM_RATE_8000;
        while (true) {
            rate = ado_pcm_flag2rate(rate_flag);
            if (!rate) break;
            if ( (rate_flag & supported_rate_list) &&
                 (rate >= rcar->sample_rate_min) && (rate <= rcar->sample_rate_max) ) {
                rcar->sample_rates |= rate_flag;
            }
            rate_flag <<= 1;
        }

        ado_debug( DB_LVL_DRIVER, "sample rate min %d, max %d, rate_list %x",
            rcar->sample_rate_min, rcar->sample_rate_max, rcar->sample_rates );
    }

    uint32_t is_mono = 0;

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan ++ ) {
            rcar_audio_channel_t* audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->ssi_op_mode == SSI_OP_MODE_MONO ) {
                is_mono = 1;
            }
        }
    }

    if ( rcar->mclk != 0 &&
         rcar->mclk / rcar->sample_rate_max * rcar->sample_rate_max == rcar->mclk &&
         rcar->sample_rate_max * rcar->ssi_voices * rcar->slot_size > rcar->mclk ) {
        ado_error_fmt("Max sample rate %d, slot_size %d and voices %d not supported by MCLK rate %d",
                      rcar->sample_rate_max, rcar->slot_size, rcar->ssi_voices, rcar->mclk);
        return EINVAL;
    }

    /* determine the sys_word_length and data_word_length settings for the SWL, DWL bit fields
       of the SSICR register; note that the override of SWL and DWL with special values for
       the TDM split modes is done in ssi_set_mode_bits */
    switch( rcar->slot_size ) {
        case 16:
            rcar->ssi_config.sys_word_length = ( is_mono ?
                 SSI_SYS_WORD_LEN_16BIT_MONO : SSI_SYS_WORD_LEN_16BIT_STEREO );
            break;
        case 32:
            rcar->ssi_config.sys_word_length = ( is_mono ?
                 SSI_SYS_WORD_LEN_32BIT_MONO : SSI_SYS_WORD_LEN_32BIT_STEREO );
            break;
        case 48:
            rcar->ssi_config.sys_word_length = ( is_mono ?
                 SSI_SYS_WORD_LEN_48BIT_MONO : SSI_SYS_WORD_LEN_48BIT_STEREO );
            break;
        case 64:
            rcar->ssi_config.sys_word_length = ( is_mono ?
                 SSI_SYS_WORD_LEN_64BIT_MONO : SSI_SYS_WORD_LEN_64BIT_STEREO );
            break;
        case 128:
            rcar->ssi_config.sys_word_length = ( is_mono ?
                 SSI_SYS_WORD_LEN_128BIT_MONO : SSI_SYS_WORD_LEN_128BIT_STEREO );
            break;
        case 256:
            rcar->ssi_config.sys_word_length = ( is_mono ?
                 SSI_SYS_WORD_LEN_256BIT_MONO : SSI_SYS_WORD_LEN_256BIT_STEREO );
            break;
        case 512:
            rcar->ssi_config.sys_word_length = ( is_mono ?
                 SSI_SYS_WORD_LEN_512BIT_MONO : SSI_SYS_WORD_LEN_NOSUP_STEREO );
            break;
        default:
            ado_error_fmt( "Unsupported slot size %d", rcar->slot_size );
            return EINVAL;
    }

    if (rcar->ssi_config.sys_word_length == SSI_SYS_WORD_LEN_NOSUP_MONO ||
        rcar->ssi_config.sys_word_length == SSI_SYS_WORD_LEN_NOSUP_STEREO) {
        ado_error_fmt( "Unsupported Unsupported slot size %d in %s mode", rcar->slot_size, is_mono ? "mono" : "stereo");
        return EINVAL;
    }

    if (is_mono && rcar->sample_size > 16) {
        ado_error_fmt( "Unsupported sample size %d in mono mode", rcar->sample_size );
        return EINVAL;
    }

    switch( rcar->sample_size ) {
        case 8:
            rcar->ssi_config.data_word_length = SSI_DATA_WORD_LEN_8BIT;
            break;
        case 16:
            rcar->ssi_config.data_word_length = SSI_DATA_WORD_LEN_16BIT;
            break;
        case 24:
            rcar->ssi_config.data_word_length = SSI_DATA_WORD_LEN_16BIT;
            break;
        case 32:
            rcar->ssi_config.data_word_length = SSI_DATA_WORD_LEN_32BIT;
            break;
        default:
            ado_error_fmt( "Unsupported sample size %d", rcar->sample_size );
            return EINVAL;
    }

    if (rcar->sample_size > rcar->slot_size) {
        ado_error_fmt( "Slot size %d cannot be less sample size %d ", rcar->slot_size, rcar->sample_size );
        return EINVAL;
    }

    if (rcar->route_name != NULL && (rcar->route_port < 0 || rcar->route_port > 31)) {
        ado_error_fmt( "Specify invalid route port (%s_%d)", rcar->route_name, rcar->route_port );
        return EINVAL;
    }

    return EOK;
}

static void rcar_register_dump ( HW_CONTEXT_T * rcar )
{
    ado_debug (DB_LVL_DRIVER, "rcar");

    ssiu_common_register_dump();

    adg_register_dump();
}

static int rcar_set_clock_rate_codec_ssi( rcar_context_t * rcar, uint32_t sample_rate )
{
    int ret = EOK;
    uint32_t divisor = 0;
    uint32_t ssi_divisor = 0;
    uint32_t adg_divisor = 0;
    uint32_t ssi_mclk_rate = 0;
    uint32_t ssi_mclk_src = rcar->mclk_src;

    ado_debug( DB_LVL_DRIVER, "sample_rate=%d", sample_rate );

    if ( rcar->mclk != 0 && rcar->mclk / sample_rate * sample_rate == rcar->mclk ) {
        ssi_mclk_rate = rcar->mclk;
    } else if ( rcar->mclk2 != 0 && rcar->mclk2 / sample_rate * sample_rate == rcar->mclk2 ) {
        ssi_mclk_rate = rcar->mclk2;
    } else if ( rcar->mclk_fs != 0 ) {
        ssi_mclk_rate = sample_rate * rcar->mclk_fs;
    } else {
        ssi_mclk_rate = rcar->mclk;
        ado_error_fmt("Using an invalid mclk %d for sample_rate %d", ssi_mclk_rate, sample_rate);
    }

    divisor = ssi_mclk_rate / rcar->slot_size / rcar->ssi_voices / sample_rate;

    // Divisors in two different audio blocks (ADG and SSI) must be used to
    // achieve the desired range of clock rates. A second, more subtle
    // restriction dictates that the SSI divisor must be a value other than
    // '1' if a continuous frame clock (i.e. even without data streaming) is
    // configured.
    // ADG divisor possible values are 1, 2, 4, 8, 16, 32
    // SSI divisor possible values are 1, 2, 4, 8, 16, 6, 12
    switch ( divisor ) {
        case 1:
        case 2:
        case 4:
        case 6:
        case 8:
        case 12:
        case 16:
            ssi_divisor = divisor;
            adg_divisor = 1;
            ssi_mclk_src = AUDIO_CLK_NONE;
            break;
        case 24:
        case 32:
            ssi_divisor = divisor>>1; // divisor / 2
            adg_divisor = 2;
            break;
        case 48:
        case 64:
            ssi_divisor = divisor>>2; // divisor / 4
            adg_divisor = 4;
            break;
        case 96:
        case 128:
            ssi_divisor = divisor>>3; // divisor / 8
            adg_divisor = 8;
            break;
        case 192:
            ssi_divisor = divisor>>4; // divisor / 16
            adg_divisor = 16;
            break;
        case 0:
            ado_error_fmt("Could not find a valid divisor for sample_rate %d", sample_rate);
            return EINVAL;
        default:
            ssi_divisor = 2;
            adg_divisor = divisor>>1;
            ssi_mclk_src = AUDIO_CLKA;
            break;
    }

    ado_debug( DB_LVL_DRIVER, "Set SSI divisor to %d, ADG divisor to %d", ssi_divisor, adg_divisor );

    ret = ssi_set_divisor( ssi_divisor );
    if (ret != EOK) {
        ado_error_fmt( "failed setting the SSI divisor for SSI");
        return ret;
    }

    ret = adg_set_ssi_clksrc (ssi_mclk_src);
    if (ret != EOK) {
        ado_error_fmt( "failed setting the ADG Clock Select for SSI" );
        return ret;
    }

    ret = adg_set_ssi_clkdiv (adg_divisor, ssi_mclk_src);
    if (ret != EOK) {
        ado_error_fmt( "failed setting the ADG divisor for SSI" );
        return ret;
    }

    /* Set the mixer/codec clock rate if applicable */
    ret = rcar_mixer_set_clock_rate( rcar, sample_rate );
    if (ret != EOK) {
        ado_error_fmt( "failed setting the mixer clock rate" );
        return ret;
    }

    return EOK;
}

ado_dll_version_t ctrl_version;

void ctrl_version ( int *major, int *minor, const char **date )
{
    *major = ADO_MAJOR_VERSION;
    *minor = 1;
    *date = __DATE__;
}

static void ctrl_init_cleanup(rcar_context_t * rcar)
{
    ado_debug (DB_LVL_DRIVER, "rcar");

    ado_mutex_destroy (&rcar->hw_lock);

    if (rcar->init_flags & RCAR_INIT_FLAG_SSIU) {
        ssiu_deinit();
        rcar->init_flags &= ~RCAR_INIT_FLAG_SSIU;
    }
    if (rcar->init_flags & RCAR_INIT_FLAG_ADG) {
        adg_deinit();
        rcar->init_flags &= ~RCAR_INIT_FLAG_ADG;
    }
    if (rcar->init_flags & RCAR_INIT_FLAG_DMAC) {
        audio_dmac_deinit(rcar);
        rcar->init_flags &= ~RCAR_INIT_FLAG_DMAC;
    }
    if (rcar->route_name) {
        free(rcar->route_name);
    }

    ado_free (rcar);

    rcar = NULL;
}

/* */
/*  Initialize */
/* */

int ctrl_init (HW_CONTEXT_T ** hw_context, ado_card_t * card, char *args)
{
    rcar_context_t *rcar;
    int status;

    ado_debug (DB_LVL_DRIVER, "rcar : CTRL_DLL_INIT");

    if ( (rcar = (rcar_context_t *) ado_calloc (1, sizeof (rcar_context_t))) == NULL ) {
        ado_error_fmt ("Unable to allocate memory (%s)", strerror (errno));
        return ENOMEM;
    }

    memset ( rcar, 0, sizeof(rcar_context_t) );

    *hw_context = rcar;
    rcar->card = card;

    if ( (status = rcar_parse_commandline (rcar, args)) != EOK ) {
        ado_free (rcar);
        return -status;
    }

    ado_card_set_shortname ( card, "rcar" );

    ado_mutex_init ( &rcar->hw_lock) ;

    /* Map DMAC */
    if ( (status = audio_dmac_init(rcar)) != EOK ) {
        ado_error_fmt ("rcar: Audio DMAC init failed (%s)", strerror (errno));
        ctrl_init_cleanup(rcar);
        rcar = NULL;
        return -status;
    }
    rcar->init_flags |= RCAR_INIT_FLAG_DMAC;

    /* Map Common SSIU base register */
    if ( (status = ssiu_init()) != EOK ) {
        ado_error_fmt ("rcar: SSIU init failed");
        ctrl_init_cleanup(rcar);
        rcar = NULL;
        return -status;
    }
    rcar->init_flags |= RCAR_INIT_FLAG_SSIU;

    /* Map ADG */
    if ( (status = adg_init()) != EOK ) {
        ado_error_fmt ("rcar: ADG init failed");
        ctrl_init_cleanup(rcar);
        rcar = NULL;
        return -status;
    }
    rcar->init_flags |= RCAR_INIT_FLAG_ADG;

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        rcar_audio_channel_t* playback = NULL;
        rcar_audio_channel_t* capture = NULL;
        uint32_t codec_rates;

        if ( rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].state != RCAR_CHN_IDLE ) {
            playback = &rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK];
            playback->chmap = ado_pcm_get_default_chmap ( playback->voices );
        }
        if ( rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].state != RCAR_CHN_IDLE ) {
            capture = &rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE];
            capture->chmap = ado_pcm_get_default_chmap ( capture->voices );
        }

        if ( !playback && !capture ) break;

        /* Perform the mixer init before populating the capabilities:
         * if the codec supports the codec_capabilities callback, it will allow to update
         * rcar->sample_rates based on codec rate capabilities
         */
        if ( (status = rcar_mixer_init ( card, rcar, &rcar->devices[i] ) ) != EOK ) {
            ado_error_fmt ("rcar: Unable to init a mixer");
            ctrl_init_cleanup(rcar);
            return -status;
        }

        rcar_mixer_supported_rates (rcar, &codec_rates);

        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            rcar_audio_channel_t* audio_chan;
            if ( rcar->devices[i].audio_channels[chan].state == RCAR_CHN_IDLE ) {
                continue;
            }
            audio_chan = &rcar->devices[i].audio_channels[chan];

            ado_debug (DB_LVL_DRIVER, "Attaching DMA interrupt %d", audio_chan->dma_context.dma_irq);

            if ( ( status = ado_attach_interrupt ( card, audio_chan->dma_context.dma_irq,
                       rcar_interrupt, rcar ) ) != EOK ) {
                /* ado_attach_interrupt returns -errno, no need to swap the status sign before returning status */
                ado_error_fmt ( "rcar: Unable to attach DMA interrupt %d (%s)",
                                audio_chan->dma_context.dma_irq, strerror (errno) );
                ctrl_init_cleanup ( rcar );
                return status;
            }

            audio_chan->pcm_caps.chn_flags = SND_PCM_CHNINFO_BLOCK |
                SND_PCM_CHNINFO_INTERLEAVE | SND_PCM_CHNINFO_BLOCK_TRANSFER |
                SND_PCM_CHNINFO_MMAP | SND_PCM_CHNINFO_MMAP_VALID;

            if( rcar->sample_size == 8 || rcar->sample_size == 16 ) {
                /* for sample size 8 bit, advertise S16_LE to io-audio, as on Rcar the 8 bit
                 * need to occupy the three most significant bytes of a 16 bit word */
                audio_chan->pcm_caps.formats = SND_PCM_FMT_S16_LE;
            } else {
                /* for sample size 24 bit, advertise S32_LE to io-audio, as on Rcar the 24 bit
                 * need to occupy the three most significant bytes of a 32 bit word */
                audio_chan->pcm_caps.formats = SND_PCM_FMT_S32_LE;
            }

            audio_chan->pcm_caps.rates = rcar->sample_rates;
            audio_chan->pcm_caps.min_rate = rcar->sample_rate_min;
            audio_chan->pcm_caps.max_rate = rcar->sample_rate_max;
            audio_chan->pcm_caps.min_voices = audio_chan->voices;
            audio_chan->pcm_caps.max_voices = audio_chan->voices;
            audio_chan->pcm_caps.min_fragsize = 64;
            audio_chan->pcm_caps.max_fragsize = 64 * 1024;
            audio_chan->pcm_caps.max_frags = 2;

            if ( ( audio_chan->pcm_caps.rates & codec_rates) != audio_chan->pcm_caps.rates ) {
                ado_debug ( DB_LVL_DRIVER, "Codec rate capabilities 0x%x restrict the driver rate capabilities 0x%x to 0x%x",
                    codec_rates, audio_chan->pcm_caps.rates, codec_rates & audio_chan->pcm_caps.rates );
                audio_chan->pcm_caps.rates &= codec_rates;
                if ( ado_pcm_flags2rates ( audio_chan->pcm_caps.rates,
                         &audio_chan->pcm_caps.min_rate, &audio_chan->pcm_caps.max_rate ) != EOK ) {
                    audio_chan->pcm_caps.min_rate = 0;
                    audio_chan->pcm_caps.max_rate = 0;
                    ctrl_init_cleanup(rcar);
                    return -EINVAL;
                }
            }

            audio_chan->pcm_funcs.aquire = rcar_acquire;
            audio_chan->pcm_funcs.release = rcar_release;
            audio_chan->pcm_funcs.prepare = rcar_prepare;

            if ( chan == ADO_PCM_CHANNEL_PLAYBACK ) {
                audio_chan->pcm_funcs.trigger = rcar_playback_trigger;
            } else {
                audio_chan->pcm_funcs.trigger = rcar_capture_trigger;
            }

            audio_chan->pcm_funcs.position = rcar_position;
            audio_chan->pcm_funcs.capabilities = rcar_capabilities;
        }

        /* Create a PCM audio device */
        if( ado_pcm_create (card, "R-Car SSI", 0, "rcar",
                playback ? 1 : 0, playback ? &playback->pcm_caps : NULL, playback ? &playback->pcm_funcs : NULL,
                capture ? 1 : 0, capture ? &capture->pcm_caps : NULL, capture ? &capture->pcm_funcs : NULL,
                rcar->devices[i].mixer, &rcar->devices[i].pcm ) != EOK ) {
            /* ado_pcm_create returns -1 on error, assign status to errno if set, else EINVAL */
            status = errno ? errno : EINVAL;
            ado_error_fmt ( "rcar: Unable to create pcm devices (%s)", strerror (errno) );
            ctrl_init_cleanup ( rcar );
            return -status;
        }

        if ( playback ) {
            rcar_mixer_set_default_group( rcar, rcar->devices[i].pcm, ADO_PCM_CHANNEL_PLAYBACK, 0 );
        }
        if ( capture ) {
            rcar_mixer_set_default_group( rcar, rcar->devices[i].pcm, ADO_PCM_CHANNEL_CAPTURE, 0 );
        }
    }

    if ( rcar->debug ) {
        rcar_register_dump ( rcar );
    }

    ado_debug (DB_LVL_DRIVER, "rcar initialization complete.....");

    return EOK;
}

ado_ctrl_dll_destroy_t ctrl_destroy;
int ctrl_destroy (HW_CONTEXT_T * rcar)
{
    ado_debug (DB_LVL_DRIVER, "rcar : CTRL_DLL_DESTROY");

    ctrl_init_cleanup(rcar);

    rcar = NULL;

    return EOK;
}

