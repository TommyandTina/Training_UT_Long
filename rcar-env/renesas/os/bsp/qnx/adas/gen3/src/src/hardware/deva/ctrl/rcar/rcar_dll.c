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

#include <unistd.h>
#include <string.h>
#include "mem_map.h"
#include "rcar.h"
#include "ssiu.h"
#include "scu.h"
#include "dmac.h"
#include "adg.h"
#include "rcar_support.h"
#include "rcar_rsrc.h"
#include "rcar_mixer.h"

#include "variant.h"

static inline uint32_t count_set_bits( uint32_t bitmask ) {
    uint32_t count = 0;
    while (bitmask) { bitmask &= (bitmask-1); count++; }
    return count;
}

/* supported_rate_list is a bitmask of RCAR-H2 hardware supported rates */
static const uint32_t supported_rate_list = SND_PCM_RATE_8000 |
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
static int rcar_set_clock_rate_scu (rcar_context_t * rcar, rcar_audio_channel_t* audio_chan, uint32_t sample_rate);

static void rcar_register_dump( HW_CONTEXT_T * rcar );

static int rcar_playback_dma_setup ( rcar_context_t * rcar, rcar_audio_channel_t* playback, ado_pcm_dmabuf_t* dmabuf ) {

    int status = EOK;

    if ( !playback->use_scu ) {
        audio_peripheral_t dest;
        /* Don't use SCU: DMA transfer from Memory to SSIx or SSIx-0 */
        if ( playback->ssi_transfer_mode == SSI_BUSIF_TRANSFER ) {
            dest = AUDIO_PERIPHERAL_SSI_BUSIF(playback->ssi_chan, playback->ssi_subchan);
        } else {
            dest = AUDIO_PERIPHERAL_SSI(playback->ssi_chan);
        }
        status = audio_dmac_mp_setup ( playback->dma_context.audiodma_chn,
                                       dest,
                                       dmabuf->phys_addr,
                                       dmabuf->size );
        if ( status != EOK ) {
            ado_error_fmt ( "failed setting up dmac mp to peripheral %x", dest );
        }
    } else {
        audio_peripheral_t dest;
        /* Use SCU: DMA transfer from Memory to SRCy, and from SRCy or CMDz to SSIx-0
         * Notes: Regardless whether the SRC function of the SCU is used, the DMA transfer
         * from memory is always to SRCy input; the peripheral-to-peripheral DMA transfer
         * from the SCU to SSIx-0 is either from SRCy output, if no CMD is used, or from
         * CMDz output, if CMD is used
         */
        dest = AUDIO_PERIPHERAL_SCUSRC(playback->src_chan);
        status = audio_dmac_mp_setup( playback->dma_context.audiodma_chn,
                                      dest,
                                      dmabuf->phys_addr,
                                      dmabuf->size );
        if( status != EOK ) {
            ado_error_fmt ( "failed setting up dmac mp to peripheral %x", dest );
        } else {
            audio_peripheral_t pp_src;
            audio_peripheral_t pp_dest;
            if ( playback->use_scu & RCAR_USE_SCU_CMD ) {
                pp_src = AUDIO_PERIPHERAL_SCUCMD(playback->cmd_chan);
            } else {
                pp_src = AUDIO_PERIPHERAL_SCUSRC(playback->src_chan);
            }
            pp_dest = AUDIO_PERIPHERAL_SSI_BUSIF(playback->ssi_chan, playback->ssi_subchan);
            status = audio_dmac_pp_setup ( playback->dma_context.audiodma_pp_chn, pp_src, pp_dest );
            if( status != EOK ) {
                ado_error_fmt ( "failed setting up dmac pp between peripherals %x and %x", pp_src, pp_dest );
            }
        }
    }

    return status;
}

static int rcar_capture_dma_setup ( rcar_context_t * rcar, rcar_audio_channel_t* capture, ado_pcm_dmabuf_t* dmabuf ) {

    int status = EOK;

    if ( !capture->use_scu ) {
        audio_peripheral_t src;
        /* Don't use SCU: DMA transfer from SSI to Memory */
        if ( capture->ssi_transfer_mode == SSI_BUSIF_TRANSFER ) {
            src = AUDIO_PERIPHERAL_SSI_BUSIF(capture->ssi_chan, capture->ssi_subchan);
        } else {
            src = AUDIO_PERIPHERAL_SSI(capture->ssi_chan);
        }
        status = audio_dmac_pm_setup ( capture->dma_context.audiodma_chn,
                                       src,
                                       dmabuf->phys_addr,
                                       dmabuf->size );
        if( status != EOK ) {
            ado_error_fmt( "failed setting up dmac pm from peripheral %x", src );
        }
    } else {
        audio_peripheral_t src;
        /* Use SCU: DMA transfer from SRC or CMD to memory */
        if ( capture->use_scu & RCAR_USE_SCU_CMD ) {
            src = AUDIO_PERIPHERAL_SCUCMD(capture->cmd_chan);
        } else {
            src = AUDIO_PERIPHERAL_SCUSRC(capture->src_chan);
        }
        status = audio_dmac_pm_setup ( capture->dma_context.audiodma_chn,
                                       src,
                                       dmabuf->phys_addr,
                                       dmabuf->size );
        if( status != EOK ) {
            ado_error_fmt( "failed setting up dmac pm from peripheral %x", src );
        } else {
            audio_peripheral_t pp_src;
            audio_peripheral_t pp_dest;
            pp_src = AUDIO_PERIPHERAL_SSI_BUSIF(capture->ssi_chan, capture->ssi_subchan);
            pp_dest = AUDIO_PERIPHERAL_SCUSRC(capture->src_chan);
            status = audio_dmac_pp_setup ( capture->dma_context.audiodma_pp_chn, pp_src, pp_dest );
            if( status != EOK ) {
                ado_error_fmt( "failed setting up dmac pp between peripherals %x and %x", pp_src, pp_dest );
            }
        }
    }

    return status;
}

static int32_t rcar_capabilities(HW_CONTEXT_T* rcar, ado_pcm_t *pcm, snd_pcm_channel_info_t* info)
{
    int chn_avail;
    rcar_audio_channel_t* audio_chan = NULL;

    ado_debug( DB_LVL_DRIVER, "CAPABILITIES" );

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        if ( rcar->devices[i].pcm == pcm ) {
            if ( rcar->devices[i].audio_channels[info->channel].ssi_chan != SSI_CHANNEL_NUM ) {
                audio_chan = &rcar->devices[i].audio_channels[info->channel];
            }
            break;
        }
    }

    if ( !audio_chan ) {
        ado_error_fmt( "invalid pcm" );
        return EINVAL;
    }

    chn_avail = audio_chan->subchn ? 0 : 1;

    if ( chn_avail ) {
        if (rcar->sample_rate_min != rcar->sample_rate_max) {

            ado_mutex_lock(&rcar->hw_lock);

            /* If channel doesn't use SRC, it uses directly the SSI sample rate;
             * if the SSI sample rate is locked by an acquired channel or by the fact
             * that SRC is used for another channel, adjust rate capabilities to reflect that
             */
            if ( rcar->ssi_sample_rate && !( audio_chan->use_scu & RCAR_USE_SCU_SRC ) ) {
                info->min_rate = info->max_rate = rcar->ssi_sample_rate;
                info->rates = ado_pcm_rate2flag(rcar->ssi_sample_rate);
            }

            ado_mutex_unlock(&rcar->hw_lock);
        }
    } else {
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
        /* if the SSI sample rate is locked by either acquired capture PCM channel or use of SRC,
         * and playback does not use SRC, the sample_rate of the playback channel to be acquired
         * must match the locked SSI sample rate
         */
        if ( rcar->ssi_sample_rate && !(audio_chan->use_scu & RCAR_USE_SCU_SRC) &&
             (config->format.rate != rcar->ssi_sample_rate) ) {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "rate is locked by another session or SRC use for another session: locked rate: %d, requested rate: %d",
                       rcar->ssi_sample_rate, config->format.rate );
            return EBUSY;
        }
    }

    if ( rcar->use_src_summary == RCAR_CHANNELS_NONE ) {
        rcar->ssi_sample_rate = config->format.rate;
    }

    if ( !rcar->acquired_ssi_chan_mask ) {
        status = rcar_set_clock_rate_codec_ssi( rcar, config->format.rate );
        if ( status != EOK ) {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "failed setting the codec/SSI clock rate" );
            return status;
        }
    }

    status = rcar_set_clock_rate_scu ( rcar, audio_chan, config->format.rate );
    if ( status != EOK ) {
        ado_mutex_unlock (&rcar->hw_lock);
        ado_error_fmt( "failed setting the SCU clock rate" );
        return status;
    }

    /* Allocate DMA transfer buffer*/
    if (ado_dma_pool_info ( NULL, NULL, NULL, 0) != EOK)
    {
        /* If not using the io-audio memory pool then allocate via the DMA library */
        status = audio_dmac_alloc_buf( audio_chan->dma_context.audiodma_chn,
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
            audio_dmac_free_buf(audio_chan->dma_context.audiodma_chn, &audio_chan->dma_context.addr);
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "failed allocating shared memory" );
            return ENOMEM;
        }
    }
    else
    {
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
        rcar->acquired_ssi_chan_mask |= SSI_CHANNEL_MASK(audio_chan->ssi_chan);
        rcar->acquired_ssi_subchan_mask[audio_chan->ssi_chan] |= SSI_SUBCHANNEL_MASK(audio_chan->ssi_subchan);
    } else {
        if (audio_chan->dma_context.addr.paddr) {
            audio_dmac_free_buf(audio_chan->dma_context.audiodma_chn, &audio_chan->dma_context.addr);
        }
        ADO_PCM_BUF_FREE (rcar->card, config);
        config->dmabuf.addr = NULL;
        if ( !audio_chan->subchn && rcar->use_src_summary == RCAR_CHANNELS_NONE ) {
            rcar->ssi_sample_rate = 0;
        }
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

    rcar->acquired_ssi_subchan_mask[audio_chan->ssi_chan] &= ~SSI_SUBCHANNEL_MASK(audio_chan->ssi_subchan);
    if (rcar->acquired_ssi_subchan_mask[audio_chan->ssi_chan] == 0) {
        rcar->acquired_ssi_chan_mask &= ~SSI_CHANNEL_MASK(audio_chan->ssi_chan);
    }

    if( !rcar->acquired_ssi_chan_mask &&
        rcar->use_src_summary == RCAR_CHANNELS_NONE ) {
        rcar->ssi_sample_rate = 0;
    }

    if( config->dmabuf.addr ) {
        if (audio_chan->dma_context.addr.paddr) {
            audio_dmac_free_buf(audio_chan->dma_context.audiodma_chn, &audio_chan->dma_context.addr);
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
        /* If the audio channel was capturing and got stopped repeat the clock rate config
         * for the SCU and the DMA setup (although already done in the acquire callback);
         * without doing this, overrun recovery would hang occasionally
         * TODO: investigate with Renesas why this is required
         */
#if 0
        /* Note: based on testing, repeating the clock rate config for the codec and SSIs
         * is not required
         */
        if ( ! rcar->active_ssi_chan_mask ) {
            status = rcar_set_clock_rate_codec_ssi( rcar, config->format.rate );
            if( status != EOK ) {
                ado_mutex_unlock (&rcar->hw_lock);
                ado_error_fmt( "failed setting the codec/SSI clock rate" );
                return status;
            }
        }
#endif
        status = rcar_set_clock_rate_scu ( rcar, audio_chan, config->format.rate );
        if( status != EOK ) {
            ado_mutex_unlock (&rcar->hw_lock);
            ado_error_fmt( "failed setting the SCU clock rate" );
            return status;
        }

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

        /* For start order on 3rd generation R-Car SoCs, refer to
         * "R-Car Series, 3rd Generation User's Manual: Hardware"
         * version 1.50, section 40.3.5 Transfer Flow - figure 40.4 and
         * notes 1, 2, as well as section 41.3.13 Procedure of SSIU Transfer -
         * figures 41.36 and 41.40
         * For start order on 2nd generation R-Car SoCs, refer to
         * "R-Car Series, 2nd Generation User's Manual: Hardware"
         * version 2.0, section 38.1.6 Transfer Flow - figure 38.4
         */

        if( playback->use_scu ) {
            /* Start Peripheral-Peripheral DMAC */
            ado_debug( DB_LVL_DRIVER, "Start Audio DMAC PP" );
            audio_dmac_pp_start( playback->dma_context.audiodma_pp_chn );
        }

        /* Start Audio-DMAC */
        ado_debug( DB_LVL_DRIVER, "Start Audio DMAC" );
        audio_dmac_start( playback->dma_context.audiodma_chn );

        if ( playback->ssi_start_mode == SSI_SYNC_SSI0129_START ) {
            /* Multichannel SSI */

            /* start the individual SSIs */
            ado_debug( DB_LVL_DRIVER, "Start SSI 0,1,2 (no CR.EN)" );
            ssi_start( SSI_CHANNEL_0, SSI_SYNC_SSI0129_START );
            ssi_start( SSI_CHANNEL_1, SSI_SYNC_SSI0129_START );
            ssi_start( SSI_CHANNEL_2, SSI_SYNC_SSI0129_START );
            rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK_012;
            if( playback->voices == 8 ) {
                ado_debug( DB_LVL_DRIVER, "Start SSI 9 (no CR.EN)" );
                ssi_start( SSI_CHANNEL_9, SSI_SYNC_SSI0129_START );
                rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK_9;
            }
            /* start in a synchronized fashion the SSI0129 or SSI012 channels */
            ado_debug( DB_LVL_DRIVER, "Synchronized start of SSI 0,1,2(,9)" );
            ssiu_start( SSI_SYNC_SSI0129_START );
        } else if ( playback->ssi_start_mode == SSI_SYNC_SSI34_START ) {
            /* SSI 3,4 configured for synchronized start */

            /* start the individual SSIs */
            ado_debug( DB_LVL_DRIVER, "Start SSI 3,4 (no CR.EN)" );
            ssi_start( playback->ssi_chan, SSI_SYNC_SSI34_START );

            rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK (playback->ssi_chan);

            /* start in a synchronized fashion the SSI34 */
            if ( ( rcar->active_ssi_chan_mask & SSI_CHANNEL_MASK_34 ) == SSI_CHANNEL_MASK_34 ) {
                ado_debug( DB_LVL_DRIVER, "Synchronized start of SSI 3,4" );
                ssiu_start( SSI_SYNC_SSI34_START );
            }
        } else {
            /* start the shared clock source SSI if this is the first channel that starts */
            if ( !rcar->active_ssi_chan_mask && playback->ssi_chan != rcar->sharedclk_ssi_chan ) {
                ado_debug ( DB_LVL_DRIVER, "Start SSI %d (CR.EN)", rcar->sharedclk_ssi_chan );
                ssi_start ( rcar->sharedclk_ssi_chan, SSI_INDEPENDENT_START );
            }
            /* start SSIx as independent SSI, if not already started */
            if ( ! ( rcar->active_ssi_chan_mask & SSI_CHANNEL_MASK (playback->ssi_chan) ) ) {
                ado_debug( DB_LVL_DRIVER, "Start SSI %d (CR.EN)", playback->ssi_chan );
                ssi_start( playback->ssi_chan, SSI_INDEPENDENT_START );
                rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK (playback->ssi_chan);
            }
        }

        /* start busif SSIx-y */
        if( playback->ssi_transfer_mode == SSI_BUSIF_TRANSFER ) {
            ado_debug( DB_LVL_DRIVER, "Start BUSIF for SSI %d subchan %d",
                playback->ssi_chan, playback->ssi_subchan );
            ssiu_busif_start(playback->ssi_chan, playback->ssi_subchan);
            rcar->active_ssi_subchan_mask[playback->ssi_chan] |= SSI_SUBCHANNEL_MASK(playback->ssi_subchan);
        }

        if (playback->use_scu) {
            /* start SRC if any SCU block is used */
            ado_debug( DB_LVL_DRIVER, "Start SRC %d",
                       playback->src_chan );
            scu_src_start(playback->src_chan);

            if (playback->use_scu & RCAR_USE_SCU_CMD) {
                /* start CMD if any CMD block is used */
                ado_debug( DB_LVL_DRIVER, "Start CMD %d",
                           playback->cmd_chan );
                scu_cmd_start(playback->cmd_chan);
            }
        }

        playback->state = RCAR_CHN_STARTED;

        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_START complete");

    } else {
        bool skipped_ssi_stop = false;

        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_STOP cmd %d", cmd);

        playback->state = RCAR_CHN_STOPPED;

        /* For stop order on 3rd generation R-Car SoCs, refer to
         * "R-Car Series, 3rd Generation User's Manual: Hardware"
         * version 1.50, section 40.3.5 Transfer Flow - figure 40.4 and
         * notes 3, 4, as well as section 41.3.13 Procedure of SSIU Transfer -
         * figures 41.37 and 41.41
         * For stop order on 2nd generation R-Car SoCs, refer to
         * "R-Car Series, 2nd Generation User's Manual: Hardware"
         * version 2.0, section 38.1.6 Transfer Flow - figure 38.4
         */

        if (rcar_generation_get() == RCAR_GENERATION_2) {
            /* stop dmac */
            if (playback->use_scu) {
                /* stop pp dmac */
                ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC PP" );
                audio_dmac_pp_stop(playback->dma_context.audiodma_pp_chn);
            }

            ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC" );
            audio_dmac_stop(playback->dma_context.audiodma_chn);
        }

        /* stop busif SSIx-y if in TDM split mode or ex-split mode, before stopping the SSI chanel */
        if( playback->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
            playback->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
            playback->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
            playback->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            ado_debug( DB_LVL_DRIVER, "Stop BUSIF for SSI %d subchan %d",
                       playback->ssi_chan, playback->ssi_subchan );
            ssiu_busif_stop(playback->ssi_chan, playback->ssi_subchan);
            rcar->active_ssi_subchan_mask[playback->ssi_chan] &= ~SSI_SUBCHANNEL_MASK(playback->ssi_subchan);
        }

        if ( playback->ssi_start_mode == SSI_SYNC_SSI0129_START ) {
            /* Multichannel SSI */

            /* stop the individual SSIs */
            ado_debug( DB_LVL_DRIVER, "Stop SSI 0,1,2 (no CR.EN)" );
            if( playback->voices == 8 ) {
                ado_debug( DB_LVL_DRIVER, "Stop SSI 9 (no CR.EN)" );
                ssi_stop( SSI_CHANNEL_9 );
                rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK_9;
            }
            ssi_stop( SSI_CHANNEL_2 );
            ssi_stop( SSI_CHANNEL_1 );
            ssi_stop( SSI_CHANNEL_0 );
            rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK_012;

            ado_debug( DB_LVL_DRIVER, "Synchronized stop of SSI 0,1,2(,9)" );
            ssiu_stop( SSI_SYNC_SSI0129_START );
        } else if ( playback->ssi_start_mode == SSI_SYNC_SSI34_START ) {
            // TODO: need to handle split and exsplit modes here?
            /* SSI 3,4 configured for synchronized start */
            if ( rcar->active_ssi_chan_mask & SSI_CHANNEL_MASK_34 ) {
                /* stop the individual SSIs */
                ado_debug( DB_LVL_DRIVER, "Stop SSI 3,4 (no CR.EN)" );
                ssi_stop( SSI_CHANNEL_3 );
                ssi_stop( SSI_CHANNEL_4 );

                ado_debug( DB_LVL_DRIVER, "Synchronized stop of SSI 3,4" );
                ssiu_stop( SSI_SYNC_SSI34_START );

                rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK_34;
            } else {
                skipped_ssi_stop = true;
            }
        } else {
            /* stop SSIx */

            /* SSIx can be stopped if it doesn't have any active subchannels, and it
             * is not the shared clk source SSI or if it is the last SSI that is still active */
            if ( rcar->active_ssi_subchan_mask[playback->ssi_chan] ||
                 ( playback->ssi_chan == rcar->sharedclk_ssi_chan &&
                   SSI_CHANNEL_MASK(playback->ssi_chan) != rcar->active_ssi_chan_mask ) ) {
                skipped_ssi_stop = true;
            } else {
                ado_debug( DB_LVL_DRIVER, "Stop SSI %d (CR.EN)", playback->ssi_chan );
                ssi_stop( playback->ssi_chan );
                rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK(playback->ssi_chan);

                /* stop the shared clock source SSI if this is the last channel that was active */
                if ( playback->ssi_chan != rcar->sharedclk_ssi_chan &&
                     !rcar->active_ssi_chan_mask ) {
                    ado_debug( DB_LVL_DRIVER, "Stop SSI %d (CR.EN)", rcar->sharedclk_ssi_chan );
                    ssi_stop( rcar->sharedclk_ssi_chan );
                }
            }
        }

        if ( !skipped_ssi_stop ) {
            /* Wait for idle mode*/
            ado_debug( DB_LVL_DRIVER, "Waiting for IDST on SSI %d", playback->ssi_chan );
            idst_status = ssi_wait_status(playback->ssi_chan, SSISR_IDST_MASK);
            if (idst_status != EOK) {
                ado_error_fmt("SSI%d timed out waiting for IDST");
            }
            ssi_stop_idle_int(playback->ssi_chan);
        }

        /* stop busif SSIx-0 if busif transfer mode and not tdm split/exsplit mode, after stopping the SSI channel */
        if( playback->ssi_transfer_mode == SSI_BUSIF_TRANSFER &&
            playback->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XMONO &&
            playback->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XSTEREO &&
            playback->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_8CHAN &&
            playback->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            ado_debug( DB_LVL_DRIVER, "Stop BUSIF for SSI %d subchan %d",
                       playback->ssi_chan, playback->ssi_subchan );
            ssiu_busif_stop(playback->ssi_chan, playback->ssi_subchan);
            rcar->active_ssi_subchan_mask[playback->ssi_chan] &= ~SSI_SUBCHANNEL_MASK(playback->ssi_subchan);
        }

        if (playback->use_scu) {
            if (playback->use_scu & RCAR_USE_SCU_CMD) {
                /* stop CMD if any CMD block is used */
                ado_debug( DB_LVL_DRIVER, "Stop CMD %d",
                           playback->cmd_chan );
                scu_cmd_stop(playback->cmd_chan);
            }

            /* stop SRC if any SCU block is used */
            ado_debug( DB_LVL_DRIVER, "Stop SRC %d",
                       playback->src_chan );
            scu_src_stop(playback->src_chan);
        }

        if (rcar_generation_get() == RCAR_GENERATION_3) {
            /* stop dmac */
            if (playback->use_scu) {
                /* stop pp dmac */
                ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC PP" );
                audio_dmac_pp_stop(playback->dma_context.audiodma_pp_chn);
            }

            ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC" );
            audio_dmac_stop(playback->dma_context.audiodma_chn);
        }

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

        /* For start order on 3rd generation R-Car SoCs, refer to
         * "R-Car Series, 3rd Generation User's Manual: Hardware"
         * version 1.50, section 40.3.5 Transfer Flow - figure 40.4 and
         * notes 1, 2, as well as section 41.3.13 Procedure of SSIU Transfer -
         * figures 41.38 and 41.42
         * For start order on 2nd generation R-Car SoCs, refer to
         * "R-Car Series, 2nd Generation User's Manual: Hardware"
         * version 2.0, section 38.1.6 Transfer Flow - figure 38.4
         */

        if ( capture->use_scu ) {
            /* Start Peripheral-Peripheral DMAC */
            ado_debug( DB_LVL_DRIVER, "Start Audio DMAC PP" );
            audio_dmac_pp_start(capture->dma_context.audiodma_pp_chn);
        }

        /* Start Audio-DMAC */
        ado_debug( DB_LVL_DRIVER, "Start Audio DMAC" );
        audio_dmac_start(capture->dma_context.audiodma_chn);

        if ( capture->ssi_start_mode == SSI_SYNC_SSI0129_START ) {
            /* Multi Channel SSI */

            /* start the individual SSIs */
            ado_debug( DB_LVL_DRIVER, "Start SSI 0,1,2 (no CR.EN)" );
            ssi_start( SSI_CHANNEL_0, SSI_SYNC_SSI0129_START );
            ssi_start( SSI_CHANNEL_1, SSI_SYNC_SSI0129_START );
            ssi_start( SSI_CHANNEL_2, SSI_SYNC_SSI0129_START );
            rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK_012;
            if( capture->voices == 8 ) {
                ado_debug( DB_LVL_DRIVER, "Start SSI 9 (no CR.EN)" );
                ssi_start( SSI_CHANNEL_9, SSI_SYNC_SSI0129_START );
                rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK_9;
            }

            /* start BUSIF 0 */
            ssiu_busif_start(SSI_CHANNEL_0, 0);
            rcar->active_ssi_subchan_mask[capture->ssi_chan] |= SSI_SUBCHANNEL_MASK(0);

            /* start in a synchronized fashion the SSI0129*/
            ado_debug( DB_LVL_DRIVER, "Synchronized start of SSI 0,1,2(,9)" );
            ssiu_start( SSI_SYNC_SSI0129_START );
        } else if ( capture->ssi_start_mode == SSI_SYNC_SSI34_START ) {
            /* SSI 3,4 configured for synchronized start */

            /* start the SSI */
            ado_debug( DB_LVL_DRIVER, "Start SSI 3,4 (no CR.EN)" );
            ssi_start( capture->ssi_chan, SSI_SYNC_SSI34_START );

            rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK (capture->ssi_chan);

            /* start BUSIF 0 */
            ssiu_busif_start(capture->ssi_chan, 0);
            rcar->active_ssi_subchan_mask[capture->ssi_chan] |= SSI_SUBCHANNEL_MASK(0);

            /* start in a synchronized fashion the SSI34 */
            if ( ( rcar->active_ssi_chan_mask & SSI_CHANNEL_MASK_34 ) == SSI_CHANNEL_MASK_34 ) {
                ado_debug( DB_LVL_DRIVER, "Synchronized start of SSI 3,4" );
                ssiu_start( SSI_SYNC_SSI34_START );
            }
        } else {
            /* start busif 0 if not in TDM split mode or TDM ex-split mode, before starting the SSI channel */
            if( capture->ssi_transfer_mode == SSI_BUSIF_TRANSFER &&
                capture->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XMONO &&
                capture->ssi_op_mode != SSI_OP_MODE_TDMSPLIT_4XSTEREO &&
                capture->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_8CHAN &&
                capture->ssi_op_mode != SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
                /* start busif SSIx-0 */
                ado_debug( DB_LVL_DRIVER, "Start BUSIF for SSI %d subchan 0", capture->ssi_chan );
                ssiu_busif_start(capture->ssi_chan, 0);
                rcar->active_ssi_subchan_mask[capture->ssi_chan] |= SSI_SUBCHANNEL_MASK(0);
            }
            /* start the shared clock source SSI if this is the first channel that starts */
            if ( !rcar->active_ssi_chan_mask && capture->ssi_chan != rcar->sharedclk_ssi_chan ) {
                ado_debug ( DB_LVL_DRIVER, "Start SSI %d (CR.EN)", rcar->sharedclk_ssi_chan );
                ssi_start ( rcar->sharedclk_ssi_chan, SSI_INDEPENDENT_START );
            }
            /* start SSIx as independent SSI, if not already started */
            if ( ! ( rcar->active_ssi_chan_mask & SSI_CHANNEL_MASK (capture->ssi_chan) ) ) {
                ado_debug( DB_LVL_DRIVER, "Start SSI %d (CR.EN)", capture->ssi_chan );
                ssi_start( capture->ssi_chan, SSI_INDEPENDENT_START );
                rcar->active_ssi_chan_mask |= SSI_CHANNEL_MASK (capture->ssi_chan);
            }
        }

        /* start busif SSIx-y if in TDM split mode or TDM ex-split mode, after starting the SSI channel */
        if( capture->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
            capture->ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
            capture->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
            capture->ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
            ado_debug( DB_LVL_DRIVER, "Start BUSIF for SSI %d subchan %d",
                       capture->ssi_chan, capture->ssi_subchan );
            ssiu_busif_start(capture->ssi_chan, capture->ssi_subchan);
            rcar->active_ssi_subchan_mask[capture->ssi_chan] |= SSI_SUBCHANNEL_MASK(capture->ssi_subchan);
        }

        if ( capture->use_scu ) {
            /* start CMD if any CMD block is used */
            if ( capture->use_scu & RCAR_USE_SCU_CMD ) {
                /* start CMD */
                ado_debug( DB_LVL_DRIVER, "Start CMD %d",
                           capture->cmd_chan );
                scu_cmd_start(capture->cmd_chan);
            }

            /* start SRC if any SCU block is used */
            ado_debug( DB_LVL_DRIVER, "Start SRC %d", capture->src_chan );
            scu_src_start(capture->src_chan);
        }

        capture->state = RCAR_CHN_STARTED;

    } else {
        bool skipped_ssi_stop = false;

        ado_debug (DB_LVL_DRIVER, "ADO_PCM_TRIGGER_STOP cmd %d", cmd);

        capture->state = RCAR_CHN_STOPPED;

        /* For stop order on 3rd generation R-Car SoCs, refer to
         * "R-Car Series, 3rd Generation User's Manual: Hardware"
         * version 1.50, section 40.3.5 Transfer Flow - figure 40.4 and
         * notes 3, 4, as well as section 41.3.13 Procedure of SSIU Transfer -
         * figures 41.39 and 41.43
         * For stop order on 2nd generation R-Car SoCs, refer to
         * "R-Car Series, 2nd Generation User's Manual: Hardware"
         * version 2.0, section 38.1.6 Transfer Flow - figure 38.4
         */

        if (rcar_generation_get() == RCAR_GENERATION_2) {
            /* stop dmac */
            if (capture->use_scu) {
                /* stop pp dmac */
                ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC PP" );
                audio_dmac_pp_stop(capture->dma_context.audiodma_pp_chn);
            }

            ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC" );
            audio_dmac_stop(capture->dma_context.audiodma_chn);
        }

        /* stop busif SSIx-y */
        if ( capture->ssi_transfer_mode == SSI_BUSIF_TRANSFER ) {
            ado_debug( DB_LVL_DRIVER, "Stop BUSIF for SSI %d subchan %d",
                       capture->ssi_chan, capture->ssi_subchan );
            ssiu_busif_stop(capture->ssi_chan, capture->ssi_subchan);
            rcar->active_ssi_subchan_mask[capture->ssi_chan] &= ~SSI_SUBCHANNEL_MASK(capture->ssi_subchan);
        }

        if ( capture->ssi_start_mode == SSI_SYNC_SSI0129_START ) {
            /* Multichannel SSI */

            /* stop the individual SSIs */
            ado_debug( DB_LVL_DRIVER, "Stop SSI 0,1,2 (no CR.EN)" );
            if( capture->voices == 8 ) {
                ado_debug( DB_LVL_DRIVER, "Stop SSI 9 (no CR.EN)" );
                ssi_stop( SSI_CHANNEL_9 );
                rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK_9;
            }
            ssi_stop( SSI_CHANNEL_2 );
            ssi_stop( SSI_CHANNEL_1 );
            ssi_stop( SSI_CHANNEL_0 );
            rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK_012;

            ado_debug( DB_LVL_DRIVER, "Synchronized stop of SSI 0,1,2(,9)" );
            ssiu_stop( SSI_SYNC_SSI0129_START );
        } else if ( capture->ssi_start_mode == SSI_SYNC_SSI34_START ) {
            // TODO: need to handle split and exsplit modes here?
            /* SSI 3,4 configured for synchronized start */
            if ( rcar->active_ssi_chan_mask & SSI_CHANNEL_MASK_34 ) {
                /* stop the individual SSIs */
                ado_debug( DB_LVL_DRIVER, "Stop SSI 3,4 (no CR.EN)" );
                ssi_stop( SSI_CHANNEL_3 );
                ssi_stop( SSI_CHANNEL_4 );

                ado_debug( DB_LVL_DRIVER, "Synchronized stop of SSI 3,4" );
                ssiu_stop( SSI_SYNC_SSI34_START );

                rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK_34;
            } else {
                skipped_ssi_stop = true;
            }
        } else {
            /* stop SSIx */

            /* SSIx can be stopped if it doesn't have any active subchannels, and it
             * is not the shared clk source SSI or if it is the last SSI that is still active */
            if ( rcar->active_ssi_subchan_mask[capture->ssi_chan] ||
                 ( capture->ssi_chan == rcar->sharedclk_ssi_chan &&
                   SSI_CHANNEL_MASK(capture->ssi_chan) != rcar->active_ssi_chan_mask ) ) {
                skipped_ssi_stop = true;
            } else {
                ado_debug( DB_LVL_DRIVER, "Stop SSI %d (CR.EN)", capture->ssi_chan );
                ssi_stop(capture->ssi_chan);
                rcar->active_ssi_chan_mask &= ~SSI_CHANNEL_MASK(capture->ssi_chan);

                /* stop the shared clock source SSI if this is the last channel that was active */
                if ( capture->ssi_chan != rcar->sharedclk_ssi_chan &&
                     !rcar->active_ssi_chan_mask ) {
                    ado_debug( DB_LVL_DRIVER, "Stop SSI %d (CR.EN)", rcar->sharedclk_ssi_chan );
                    ssi_stop( rcar->sharedclk_ssi_chan );
                }
            }
        }

        if ( !skipped_ssi_stop ) {
            /* Wait for idle mode*/
            ado_debug( DB_LVL_DRIVER, "Waiting for IDST on SSI %d",
                       capture->ssi_chan );
            ssi_wait_status(capture->ssi_chan, SSISR_IDST_MASK);
        }

        if (capture->use_scu) {
            /* stop SRC if any SCU block is used */
            ado_debug( DB_LVL_DRIVER, "Stop SRC %d", capture->src_chan );
            scu_src_stop(capture->src_chan);

            if (capture->use_scu & RCAR_USE_SCU_CMD) {
                /* stop CMD if any CMD block is used */
                ado_debug( DB_LVL_DRIVER, "Stop CMD %d", capture->cmd_chan );
                scu_cmd_stop(capture->cmd_chan);
            }
        }

        if (rcar_generation_get() == RCAR_GENERATION_3) {
            /* stop dmac */
            if (capture->use_scu) {
                /* stop pp dmac */
                ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC PP" );
                audio_dmac_pp_stop(capture->dma_context.audiodma_pp_chn);
            }

            ado_debug( DB_LVL_DRIVER, "Stop Audio DMAC" );
            audio_dmac_stop(capture->dma_context.audiodma_chn);
        }
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

    audio_dmac_count_register_get(pc->dma_context.audiodma_chn, &bytes_left);

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
            if ( rcar->devices[i].audio_channels[chan].dma_context.audiodma_irq == irq ) {
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
    status = audio_dmac_cleanup ( audio_chan->dma_context.audiodma_chn );

    if ( status == EOK ) {
        /* Signal to io-audio (DMA transfer was completed) */
        dma_interrupt ( audio_chan->subchn );
    } else {
        ado_pcm_error ( audio_chan->subchn, ADO_PCM_STATUS_ERROR );
    }
}

static void rcar_release_resources(rcar_context_t * rcar)
{
    uint32_t released_ssi_chan_mask = 0;

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            rcar_audio_channel_t* audio_chan = &rcar->devices[i].audio_channels[chan];

            if ( audio_chan->ssi_chan == SSI_CHANNEL_NUM ) {
                continue;
            }

            if ( audio_chan->ssi_op_mode == SSI_OP_MODE_MULTICH) { //Multiple SSI
                rcar_release_ssi( SSI_CHANNEL_0, SSI_CHANNEL_2 );
                if ( audio_chan->voices == 6 ) {
                    released_ssi_chan_mask |= SSI_CHANNEL_MASK_012;
                } else {
                    rcar_release_ssi( SSI_CHANNEL_9, SSI_CHANNEL_9 );
                    released_ssi_chan_mask |= SSI_CHANNEL_MASK_0129;
                }
            } else {
                if ( ! ( SSI_CHANNEL_MASK(audio_chan->ssi_chan) & released_ssi_chan_mask ) ) {
                    rcar_release_ssi( audio_chan->ssi_chan, audio_chan->ssi_chan );
                    released_ssi_chan_mask |= SSI_CHANNEL_MASK(audio_chan->ssi_chan);
                }
            }
            audio_chan->ssi_chan = SSI_CHANNEL_NUM;

            if ( audio_chan->hdmi_chan != SSI_HDMI_CHANNEL_NUM ) {
                rcar_release_hdmi( audio_chan->hdmi_chan, audio_chan->hdmi_chan );
                audio_chan->hdmi_chan = SSI_HDMI_CHANNEL_NUM;
            }

            if ( audio_chan->src_chan != SCU_SRC_CHANNEL_NUM ) {
                rcar_release_src( audio_chan->src_chan );
                audio_chan->src_chan = SCU_SRC_CHANNEL_NUM;
            }

            if ( audio_chan->cmd_chan != SCU_CMD_CHANNEL_NUM ) {
                rcar_release_src( audio_chan->cmd_chan );
                audio_chan->cmd_chan = SCU_CMD_CHANNEL_NUM;
            }
        }
    }

    if ( rcar->sharedclk_ssi_chan != SSI_CHANNEL_NUM ) {
        if ( ! ( SSI_CHANNEL_MASK(rcar->sharedclk_ssi_chan) & released_ssi_chan_mask ) ) {
            rcar_release_ssi ( rcar->sharedclk_ssi_chan, rcar->sharedclk_ssi_chan );
            released_ssi_chan_mask |= rcar->sharedclk_ssi_chan;
        }
        rcar->sharedclk_ssi_chan = SSI_CHANNEL_NUM;
    }
}

static int rcar_reserve_chan_scu_resources
(
    rcar_context_t * rcar,
    rcar_audio_channel_t* audio_chan
)
{
    if (audio_chan->use_scu) {
        /* if using any SCU block for playback, reserve an SRC instance, as it is
         * required for DMA transfers into the SCU, even if the SRC function is not used
         */
        if( rcar_reserve_src( audio_chan->voices > 2 ? 1 : 0, 0,
                              (audio_chan->use_scu & RCAR_USE_SCU_CMD) ? 1 : 0,
                              &audio_chan->src_chan ) != EOK) {
            ado_error_fmt("failed reserving SRC");
            return EAGAIN;
        }

        /* if using any CMD block for playback, reserve a CMD instance */
        if( audio_chan->use_scu & RCAR_USE_SCU_CMD ) {
            if( rcar_reserve_cmd( &audio_chan->cmd_chan ) != EOK ) {
                ado_error_fmt("failed reserving CMD");
                return EAGAIN;
            }
        }
    }

    return EOK;
}

static int rcar_reserve_chan_resources
(
    rcar_context_t * rcar,
    rcar_audio_channel_t* audio_chan,
    uint32_t ssi_chan,
    uint32_t hdmi_chan
)
{
    /* seed the resources if not already done */
    rcar_create_resources();

    /* reserve the required resources */
    if (audio_chan->ssi_op_mode == SSI_OP_MODE_MULTICH) { //Multiple SSI
        if( rcar_reserve_ssi( SSI_CHANNEL_0, SSI_CHANNEL_2 ) != EOK ) {
            rcar_release_resources( rcar );
            ado_error_fmt("failed reserving SSI 0,1,2");
            return EAGAIN;
        }
        if( audio_chan->voices == 8 &&
            rcar_reserve_ssi( SSI_CHANNEL_9, SSI_CHANNEL_9 ) != EOK ) {
            ado_error_fmt("failed reserving SSI 9");
            rcar_release_resources( rcar );
            return EAGAIN;
        }
        audio_chan->ssi_chan = SSI_CHANNEL_0;
    } else {
        if( rcar_reserve_ssi( ssi_chan, ssi_chan ) != EOK ) {
            rcar_release_resources( rcar );
            ado_error_fmt("failed reserving SSI %d", ssi_chan);
            return EAGAIN;
        }
        audio_chan->ssi_chan = ssi_chan;
    }

    if( hdmi_chan != SSI_HDMI_CHANNEL_NUM ) {
        if( rcar_reserve_hdmi( hdmi_chan, hdmi_chan ) != EOK ) {
            rcar_release_resources( rcar );
            ado_error_fmt("failed reserving HDMI %d", hdmi_chan);
            return EAGAIN;
        }
        audio_chan->hdmi_chan = hdmi_chan;
    }

    return rcar_reserve_chan_scu_resources ( rcar, audio_chan );
}

static rcar_audio_channel_t* rcar_new_audio_chan
(
    rcar_context_t * rcar,
    uint32_t is_transmit
)
{
    rcar_audio_channel_t* audio_chan = NULL;
    uint32_t chan = is_transmit ? ADO_PCM_CHANNEL_PLAYBACK : ADO_PCM_CHANNEL_CAPTURE;

    // pick the next available channel
    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        if ( rcar->devices[i].audio_channels[chan].ssi_chan == SSI_CHANNEL_NUM ) {
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
    uint32_t ssi_chan,
    uint32_t hdmi_chan,
    uint32_t voice_map,
    ssi_op_mode_t ssi_op_mode,
    uint32_t use_scu
)
{
    uint32_t voices_exsplit_mode = 0;
    uint32_t max_ssi_subchan = 0;
    uint32_t voices = 0;
    uint32_t ssi_voices = rcar->ssi_voices;
    uint32_t sample_size = rcar->sample_size;
    uint32_t slot_size = rcar->slot_size;
    ssi_transfer_mode_t ssi_transfer_mode;

    ado_debug ( DB_LVL_DRIVER, "is_transmit=%d, ssi_chan=%d, voice_map=%d, ssi_op_mode=%d, use_scu=%d",
        is_transmit, ssi_chan, voice_map, ssi_op_mode, use_scu);

    voices_exsplit_mode = ( voice_map & 0xFFFFFFE0 ) != 0 ? 1 : 0;
    if ( !voices_exsplit_mode ) {
        voices = voice_map;
    } else {
        voices = 0;
        for ( uint32_t i = 0; i < SSI_SUB_CHANNEL_NUM*4; i+=4 ) {
            voices += ( voice_map >> i ) & 0xF;
        }
    }

    /* TODO: assign SSI012, SSI0129, SSI34 start mode */

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
    } else if ( ssi_op_mode == SSI_OP_MODE_MULTICH ) {
        if ( !voices ) {
            ado_error_fmt ( "voices need to be specified in multi channel mode" );
            return EINVAL;
        }
        if ( voices_exsplit_mode ) {
            ado_error_fmt( "voice map 0x%x incompatible with multi channel mode", voice_map );
            return EINVAL;
        }
        if ( voices && voices != 6 && voices != 8 ) {
            ado_error_fmt( "%d voices not supported in multi channel mode", voices );
            return EINVAL;
        }
        /* in multi channel mode each SSI is programmed in stereo mode */
        if ( !ssi_voices ) {
            ssi_voices = 2;
        } else {
            if ( ssi_voices != 2 ) {
                ado_error_fmt( "voices %d does not match previously determined ssi_voices %d",
                    2, ssi_voices );
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
    if ( ( ( ssi_op_mode == SSI_OP_MODE_TDM || ssi_op_mode == SSI_OP_MODE_TDMEXT ) &&
         ( ssi_voices != 6 && ssi_voices != 8 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ) && ( ssi_voices != 4 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
             ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) && ( ssi_voices != 8 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ||
             ssi_op_mode == SSI_OP_MODE_TDM_16CHAN ) && ( ssi_voices != 16 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_STEREO || ssi_op_mode == SSI_OP_MODE_MULTICH ) &&
         ( ssi_voices != 2 ) ) ||
         ( ( ssi_op_mode == SSI_OP_MODE_MONO ) && ( ssi_voices != 1 ) ) ) {
        ado_error_fmt( "op_mode %d and voices %d are inconsistent", ssi_op_mode, ssi_voices );
        return EINVAL;
    }

    /* check if the requested SSI channel supports the determined op mode */
    if ( ssi_op_mode == SSI_OP_MODE_TDMEXT ) {
        if ( !rcar_ssi_tdmext_supported( ssi_chan ) ) {
            ado_error_fmt("SSI %d does not support the TDM EXT mode", ssi_chan);
            return EINVAL;
        }
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
                ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
        if ( !rcar_ssi_tdmsplit_supported( ssi_chan ) ) {
            ado_error_fmt("SSI %d does not support the TDM SPLIT mode", ssi_chan);
            return EINVAL;
        }
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
                ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        if ( !rcar_ssi_tdmexsplit_supported( ssi_chan ) ) {
            ado_error_fmt("SSI %d does not support the TDM EXSPLIT mode", ssi_chan);
            return EINVAL;
        }
    }

    /* if HDMI output requested, check if that is consistent with the determined op mode */
    if ( hdmi_chan != SSI_HDMI_CHANNEL_NUM && ssi_op_mode != SSI_OP_MODE_MULTICH &&
         ssi_op_mode != SSI_OP_MODE_STEREO ) {
        ado_error_fmt("HDMI interface is not supported in op_mode %d", ssi_op_mode);
        return EINVAL;
    }

    /* check if the sample_size is supported for the determined op mode */
    if ( ssi_op_mode == SSI_OP_MODE_MULTICH ||
         ssi_op_mode == SSI_OP_MODE_TDMEXT ||
         ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
         ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
         ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) {
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
        } else if ( sample_size != 16 ) {
            ado_error_fmt("Sample size %d is not supported with op mode %d", sample_size, ssi_op_mode);
            return EINVAL;
        }
        if ( slot_size == 0 ) {
            slot_size = 16;
        } else if ( slot_size != 16 ) {
            ado_error_fmt("Slot size %d is not supported with op mode %d", slot_size, ssi_op_mode);
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

    /* validate that the sample_size is less equal the slot size */
    if ( sample_size > slot_size ) {
        ado_error_fmt("Sample size %d is greater than the slot size %d", sample_size, slot_size);
        return EINVAL;
    }

    /* check if the sample_size is supported for the intended SCU use */
    if ( ( sample_size > 24 ) && use_scu ) {
        ado_error_fmt("Sample size %d is not supported by SCU", sample_size);
        return EINVAL;
    }

    /* determine the SSI transfer mode */
    if ( use_scu || ssi_op_mode == SSI_OP_MODE_MULTICH ||
         ssi_op_mode == SSI_OP_MODE_TDMEXT ||
         ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ||
         ssi_op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ||
         ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
         ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN )
    {
        ssi_transfer_mode = SSI_BUSIF_TRANSFER;
    } else {
        ssi_transfer_mode = SSI_INDEPENDENT_TRANSFER;
    }

    rcar->ssi_voices = ssi_voices;
    rcar->sample_size = sample_size;
    rcar->slot_size = slot_size;

    if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        max_ssi_subchan = SSI_SUB_CHANNEL_7;
        rcar->tdm_exsplit_map[ssi_chan] = voice_map;
    } else if ( ssi_op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) {
        max_ssi_subchan = SSI_SUB_CHANNEL_3;
        rcar->tdm_exsplit_map[ssi_chan] = voice_map;
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
            audio_chan->ssi_transfer_mode = ssi_transfer_mode;
            audio_chan->ssi_start_mode = SSI_INDEPENDENT_START;
            audio_chan->use_scu = use_scu;
            audio_chan->voices = subchan_voices;
            if ( i == SSI_SUB_CHANNEL_0 ) {
                if ( rcar_reserve_chan_resources ( rcar, audio_chan, ssi_chan, hdmi_chan ) != EOK ) {
                    ado_error_fmt ( "Could not reserve resources ssi_chan=%d, hdmi_chan=%d, use_scu=%x",
                        ssi_chan, hdmi_chan, audio_chan->use_scu );
                    return EINVAL;
                }
            } else {
                audio_chan->ssi_chan = ssi_chan;
                audio_chan->hdmi_chan = hdmi_chan;
                if ( rcar_reserve_chan_scu_resources ( rcar, audio_chan ) != EOK ) {
                    ado_error_fmt ( "Could not reserve scu resources use_scu=%x", audio_chan->use_scu );
                    return EINVAL;
                }
            }
            audio_chan->ssi_subchan = i;
        }
    }

    return EOK;
}

static int rcar_parse_commandline (rcar_context_t * rcar, char *args)
{
    int      opt = 0;
    char     *value;
    int      numvalue = 0;
    uint32_t min_idx, max_idx;
    uint32_t sample_rate_max;

    /* local variables to hold ssi and hmdi channels specified in io-audio conf file, or
     * their default value from the variant.h file, or other channel specific options;
     * copied to rcar audio channel variables upon completion of parsing all the
     * audio channel specific options
     */
    uint32_t tx_ssi_chan = SSI_CHANNEL_NUM;
    uint32_t rx_ssi_chan = SSI_CHANNEL_NUM;
    uint32_t hdmi_chan = SSI_HDMI_CHANNEL_NUM;
    ssi_op_mode_t tx_ssi_op_mode = SSI_OP_MODE_NUM;
    ssi_op_mode_t rx_ssi_op_mode = SSI_OP_MODE_NUM;
    uint32_t tx_voice_map = 0;
    uint32_t rx_voice_map = 0;
    uint32_t tx_use_scu = 0;
    uint32_t rx_use_scu = 0;

    uint32_t ssi_chan_mask = 0;

    if ( DEFAULT_USE_SRC_PLAYBACK ) {  /* see variant.h for default */
        tx_use_scu |= RCAR_USE_SCU_SRC;
    }
    if ( DEFAULT_USE_DVC_PLAYBACK ) {  /* see variant.h for default */
        tx_use_scu |= RCAR_USE_SCU_DVC;
    }
    if ( DEFAULT_USE_SRC_CAPTURE ) {   /* see variant.h for default */
        rx_use_scu |= RCAR_USE_SCU_SRC;
    }
    if ( DEFAULT_USE_DVC_CAPTURE ) {   /* see variant.h for default */
        rx_use_scu |= RCAR_USE_SCU_DVC;
    }

    char     *opts[] = {
    #define RCAR_OPT_TX_SSI            0
        "tx_ssi",           // e.g. tx_ssi=0, tx_ssi=0129, enumerates the SSI indexes used for playback
    #define RCAR_OPT_RX_SSI            1
        "rx_ssi",           // e.g. rx_ssi=1, specifies the SSI index used for capture
    #define RCAR_OPT_TX_VOICES         2
        "tx_voices",        // e.g. tx_voices=8, tx_voices=2, tx_voices=4:0:2:2, specifies number of playback voices used
    #define RCAR_OPT_RX_VOICES         3
        "rx_voices",        // e.g. rx_voices=8, rx_voices=2, rx_voices=4:0:2:2, specifies number of capture voices used
    #define RCAR_OPT_TX_OP_MODE        4
        "tx_op_mode",       // "tx_op_mode" indicates a special TDM op mode used for playback, possible values are "tdm_ext",
                            // "tdm_split", tdm_exsplit, while other op modes are inferred from the use of multiple SSI
                            // or from the number of voices
    #define RCAR_OPT_RX_OP_MODE        5
        "rx_op_mode",       // "rx_op_mode" indicates a special TDM op mode used for capture, possible values are "tdm_ext",
                            // "tdm_split", tdm_exsplit, while other op modes are inferred from the use of multiple SSI
                            // or from the number of voices
    #define RCAR_OPT_TX_USE_SRC        6
        "tx_use_src",       // tx_use_src=1 indicates that SRC is used by playback channel, 0 indicates that SRC is not used
    #define RCAR_OPT_RX_USE_SRC        7
        "rx_use_src",       // rx_use_src=1 indicates that SRC is used by capture channel, 0 indicates that SRC is not used
    #define RCAR_OPT_TX_USE_DVC        8
        "tx_use_dvc",       // tx_use_dvc=1 indicates that DVC is used by playback channel, 0 indicates that DVC is not used
    #define RCAR_OPT_RX_USE_DVC        9
        "rx_use_dvc",       // rx_use_dvc=1 indicates that DVC is used by capture channel, 0 indicates that DVC is not used
    #define RCAR_OPT_CLK_MODE          10
        "clk_mode",         // clk_mode="master" indicates that one of the used SSIs indicated by tx_ssi and rx_ssi is
                            // configured as master, clk_mode="slave" indicates that all SSIs used by this driver
                            // instance are configured as slave
    #define RCAR_OPT_SYNC_START        11
        "sync_start",       // no params, indicates the use of synchronized start of SSI3,4
    #define RCAR_OPT_SAMPLE_SIZE       12
        "sample_size",
    #define RCAR_OPT_CLK_POL           13
        "clk_pol",
    #define RCAR_OPT_FSYNC_POL         14
        "fsync_pol",
    #define RCAR_OPT_BIT_DELAY         15
        "bit_delay",
    #define RCAR_OPT_SAMPLE_RATE       16
        "sample_rate",
    #define RCAR_OPT_SAMPLE_RATE_LIST  17
        "sample_rate_list",
    #define RCAR_OPT_SLOT_SIZE         18
        "slot_size",
    #define RCAR_OPT_HDMI_CHAN         19
        "hdmi_chan",
    #define RCAR_OPT_VER               20
        "ver",
    #define RCAR_OPT_MCLK1             21
        "mclk_1",
    #define RCAR_OPT_MCLK2             22
        "mclk_2",
    #define RCAR_OPT_MCLK_FS           23
        "mclk_fs",
    #define RCAR_OPT_MCLK_SOURCE1      24
        "mclk_clk_source1",
    #define RCAR_OPT_MCLK_SOURCE2      25
        "mclk_clk_source2",
    #define RCAR_OPT_SAMPLE_RATE_SSI   26
        "sample_rate_ssi",
    #define RCAR_OPT_DEBUG             27
        "debug",            // no params, refers to the use of register dumps at the start and end of
                            // playback or capture sessions
        NULL
    };

    ado_debug ( DB_LVL_DRIVER, "" );

    /* driver option related data members */
    rcar->ssi_masterslave_mode             = SSI_MS_MODE_NUM;
    rcar->sample_rate_min                  = SAMPLE_RATE_MIN;  /* see variant.h */
    rcar->sample_rate_max                  = SAMPLE_RATE_MAX;  /* see variant.h */
    rcar->ssi_sample_rate                  = SAMPLE_RATE_SSI;  /* see variant.h */
    rcar->sample_rates                     = 0;
    rcar->slot_size                        = 0;
    rcar->sample_size                      = 0;
    rcar->sync_start_mode                  = 0;
    rcar->ssi_config.clk_pol               = SSI_BIT_CLK_POL_RISING;
    rcar->ssi_config.ws_pol                = SSI_WS_POL_0;
    rcar->ssi_config.bit_delay             = SSI_BIT_DELAY_ONE;
    rcar->ssi_config.padding_pol           = SSI_PADDING_POL_LOW;
    rcar->ssi_config.serial_data_alignment = SSI_SER_DATA_ALIGN_DATA_FIRST;
    rcar->ssi_config.sys_word_length       = SSI_SYS_WORD_LEN_16BIT_STEREO;
    rcar->ssi_config.data_word_length      = SSI_DATA_WORD_LEN_16BIT;
    rcar->mclk_1                           = MCLK_1;           /* see variant.h */
    rcar->mclk_2                           = MCLK_2;           /* see variant.h */
    rcar->mclk_source_1                    = MCLK_SOURCE_1;    /* see variant.h */
    rcar->mclk_source_2                    = MCLK_SOURCE_2;    /* see variant.h */
    rcar->mclk_fs                          = MCLK_FS;          /* see variant.h */
    rcar->debug                            = 0;                /* by default, no register dumps */
    rcar->init_flags                       = 0;

    rcar->sharedclk_ssi_chan               = SSI_CHANNEL_NUM;
    rcar->ssi_voices                       = 0;
    rcar->ssi_pin_mode                     = SSI_INDEPENDENT_PINS;
    rcar->use_src_summary                  = 0;
    rcar->use_scu_summary                  = 0;
    rcar->use_dtcp_summary                 = 0;
    rcar->use_mlp_summary                  = 0;
    rcar->config_ssi_chan_mask             = 0;
    rcar->acquired_ssi_chan_mask           = 0;
    rcar->active_ssi_chan_mask             = 0;
    rcar->busif_ssi_chan_mask              = 0;

    for ( uint32_t i = 0; i < SSI_CHANNEL_NUM; i++ ) {
        rcar->acquired_ssi_subchan_mask[i] = 0;
        rcar->active_ssi_subchan_mask[i] = 0;
        rcar->tdm_exsplit_map[i] = 0;
    }

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].ssi_op_mode             = SSI_OP_MODE_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].ssi_op_mode              = SSI_OP_MODE_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].ssi_transfer_mode       = SSI_INDEPENDENT_TRANSFER;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].ssi_transfer_mode        = SSI_INDEPENDENT_TRANSFER;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].ssi_start_mode          = SSI_INDEPENDENT_START;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].ssi_start_mode           = SSI_INDEPENDENT_START;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].use_scu                 = 0;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].use_scu                  = 0;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].use_mlp                 = 0;  /* by default, disable mlp */
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].use_mlp                  = 0;  /* by default, disable mlp */
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].use_dtcp                = 0;  /* by default, disable dtcp */
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].use_dtcp                 = 0;  /* by default, disable dtcp */
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].ssi_chan                = SSI_CHANNEL_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].src_chan                = SCU_SRC_CHANNEL_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].cmd_chan                = SCU_CMD_CHANNEL_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].ssi_chan                 = SSI_CHANNEL_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].src_chan                 = SCU_SRC_CHANNEL_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].cmd_chan                 = SCU_CMD_CHANNEL_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].hdmi_chan               = SSI_HDMI_CHANNEL_NUM;
        rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].hdmi_chan                = SSI_HDMI_CHANNEL_NUM;
    }
    /* Attempt to detect R-Car version based on confstr */
    rcar_version_init();

    /* Sets options to each values */
    while ( args != NULL && args[0] != 0 ) {
        opt = getsubopt ( &args, opts, &value );

        /* Handle in a single place the case where an option that would need a value
           to be specified is missing the value */
        switch ( opt ) {
            case RCAR_OPT_TX_SSI:
            case RCAR_OPT_RX_SSI:
            case RCAR_OPT_TX_VOICES:
            case RCAR_OPT_RX_VOICES:
            case RCAR_OPT_TX_OP_MODE:
            case RCAR_OPT_RX_OP_MODE:
            case RCAR_OPT_TX_USE_SRC:
            case RCAR_OPT_RX_USE_SRC:
            case RCAR_OPT_TX_USE_DVC:
            case RCAR_OPT_RX_USE_DVC:
            case RCAR_OPT_CLK_MODE:
            case RCAR_OPT_SAMPLE_SIZE:
            case RCAR_OPT_CLK_POL:
            case RCAR_OPT_FSYNC_POL:
            case RCAR_OPT_BIT_DELAY:
            case RCAR_OPT_SAMPLE_RATE:
            case RCAR_OPT_SAMPLE_RATE_LIST:
            case RCAR_OPT_SLOT_SIZE:
            case RCAR_OPT_HDMI_CHAN:
            case RCAR_OPT_VER:
            case RCAR_OPT_MCLK1:
            case RCAR_OPT_MCLK2:
            case RCAR_OPT_MCLK_SOURCE1:
            case RCAR_OPT_MCLK_SOURCE2:
            case RCAR_OPT_MCLK_FS:
            case RCAR_OPT_SAMPLE_RATE_SSI:
                if ( value == NULL ) {
                    ado_error_fmt ( "Option %s requires a value", opts[opt] );
                    return EINVAL;
                }
                break;
            default:
                break;
        }
        switch ( opt ) {
            case RCAR_OPT_TX_SSI:
                if ( rcar_version_get() == RCAR_VERSION_UNKNOWN ) {
                    ado_error_fmt( "RCAR version needs to be specified before %s", opts[opt] );
                    return EINVAL;
                }
                if ( tx_ssi_chan != SSI_CHANNEL_NUM ) {
                    // if we already processed options for a playback channel, reserve resources for the
                    // current playback channel
                    if ( rcar_audio_chan_config ( rcar, 1, tx_ssi_chan, hdmi_chan, tx_voice_map, tx_ssi_op_mode, tx_use_scu ) != EOK ) {
                        return EINVAL;
                    }
                    tx_ssi_chan = SSI_CHANNEL_NUM;
                    hdmi_chan = SSI_HDMI_CHANNEL_NUM;
                }
                if ( !strcmp(value, "0129") ) {
                    if ( tx_ssi_op_mode != SSI_OP_MODE_NUM &&
                         tx_ssi_op_mode != SSI_OP_MODE_MULTICH ) {
                        ado_error_fmt( "Multi SSI config not supported in op mode %d", tx_ssi_op_mode );
                        return EINVAL;
                    }
                    if ( !rcar_ssi_supported ( SSI_CHANNEL_0 ) || !rcar_ssi_supported ( SSI_CHANNEL_1 ) ||
                         !rcar_ssi_supported ( SSI_CHANNEL_2 ) || !rcar_ssi_supported ( SSI_CHANNEL_9 ) ) {
                        ado_error_fmt( "Some of SSI 0,1,2,9 not supported" );
                        return EINVAL;
                    }
                    if ( ssi_chan_mask & SSI_CHANNEL_MASK_0129 ) {
                        ado_error_fmt( "Some of SSI 0,1,2,9 were already specified for another channel" );
                        return EINVAL;
                    }
                    tx_ssi_op_mode = SSI_OP_MODE_MULTICH;
                    tx_voice_map = 0x8;
                    // set playback ssi_chan as 0, the use of multiple channels is specified by the op mode
                    // and voices map
                    tx_ssi_chan = SSI_CHANNEL_0;
                    ssi_chan_mask |= SSI_CHANNEL_MASK_0129;
                    ado_debug( DB_LVL_DRIVER, "tx uses multiple SSI 0129" );
                } else if ( !strcmp(value, "012") ) {
                    if ( tx_ssi_op_mode != SSI_OP_MODE_NUM &&
                         tx_ssi_op_mode != SSI_OP_MODE_MULTICH ) {
                        ado_error_fmt( "Multi SSI config not supported in op mode %d", tx_ssi_op_mode );
                        return EINVAL;
                    }
                    if ( !rcar_ssi_supported ( SSI_CHANNEL_0 ) || !rcar_ssi_supported ( SSI_CHANNEL_1 ) ||
                         !rcar_ssi_supported ( SSI_CHANNEL_2 ) ) {
                        ado_error_fmt( "Some of SSI 0,1,2 not supported" );
                        return EINVAL;
                    }
                    if ( ssi_chan_mask & SSI_CHANNEL_MASK_012 ) {
                        ado_error_fmt( "Some of SSI 0,1,2 were already specified for another channel" );
                        return EINVAL;
                    }
                    tx_ssi_op_mode = SSI_OP_MODE_MULTICH;
                    tx_voice_map = 0x6;
                    // set playback ssi_chan as 0, the use of multiple channels is specified by the op mode
                    tx_ssi_chan = SSI_CHANNEL_0;
                    ssi_chan_mask |= SSI_CHANNEL_MASK_012;
                    ado_debug( DB_LVL_DRIVER, "tx uses multiple SSI 0,1,2" );
                } else {
                    numvalue = strtol( value, NULL, 0 );
                    if ( !rcar_ssi_supported( numvalue ) ) {
                        ado_error_fmt( "SSI %d not supported", numvalue );
                        return EINVAL;
                    }
                    tx_ssi_chan = numvalue;
                    if ( ssi_chan_mask & SSI_CHANNEL_MASK(tx_ssi_chan) ) {
                        ado_error_fmt ( "SSI %d was already specified for another channel", tx_ssi_chan );
                        return EINVAL;
                    }
                    ssi_chan_mask |= (1 << tx_ssi_chan);
                    ado_debug( DB_LVL_DRIVER, "tx uses SSI %d", numvalue );
                }
                break;
            case RCAR_OPT_RX_SSI:
                if (rcar_version_get() == RCAR_VERSION_UNKNOWN) {
                    ado_error_fmt( "RCAR version needs to be specified before %s", opts[opt] );
                    return EINVAL;
                }
                if ( rx_ssi_chan != SSI_CHANNEL_NUM ) {
                    // if we already processed options for a capture channel, reserve resources for the
                    // current capture channel
                    if ( rcar_audio_chan_config ( rcar, 0, rx_ssi_chan, SSI_HDMI_CHANNEL_NUM, rx_voice_map, rx_ssi_op_mode, rx_use_scu ) != EOK ) {
                        return EINVAL;
                    }
                    rx_ssi_chan = SSI_CHANNEL_NUM;
                }
                if ( !strcmp(value, "0129") ) {
                    if ( rx_ssi_op_mode != SSI_OP_MODE_NUM &&
                        rx_ssi_op_mode != SSI_OP_MODE_MULTICH ) {
                        ado_error_fmt( "Multi SSI config not supported in op mode %d", rx_ssi_op_mode );
                        return EINVAL;
                    }
                    if ( !rcar_ssi_supported ( SSI_CHANNEL_0 ) || !rcar_ssi_supported ( SSI_CHANNEL_1 ) ||
                        !rcar_ssi_supported ( SSI_CHANNEL_2 ) || !rcar_ssi_supported ( SSI_CHANNEL_9 ) ) {
                        ado_error_fmt( "Some of SSI 0,1,2,9 not supported" );
                        return EINVAL;
                    }
                    if ( ssi_chan_mask & SSI_CHANNEL_MASK_0129 ) {
                        ado_error_fmt( "Some of SSI 0,1,2,9 were already specified for another channel" );
                        return EINVAL;
                    }
                    rx_ssi_op_mode = SSI_OP_MODE_MULTICH;
                    rx_voice_map = 0x8;
                    // set capture ssi_chan as 0, the use of multiple channels is specified by the op mode
                    // and capture voices
                    rx_ssi_chan = SSI_CHANNEL_0;
                    ssi_chan_mask |= SSI_CHANNEL_MASK_0129;
                    ado_debug( DB_LVL_DRIVER, "rx uses multiple SSI 0129" );
                } else if ( !strcmp(value, "012") ) {
                    if ( rx_ssi_op_mode != SSI_OP_MODE_NUM &&
                        rx_ssi_op_mode != SSI_OP_MODE_MULTICH ) {
                        ado_error_fmt( "Multi SSI config not supported in op mode %d", rx_ssi_op_mode );
                        return EINVAL;
                    }
                    if ( !rcar_ssi_supported ( SSI_CHANNEL_0 ) || !rcar_ssi_supported ( SSI_CHANNEL_1 ) ||
                        !rcar_ssi_supported ( SSI_CHANNEL_2 ) ) {
                        ado_error_fmt( "Some of SSI 0,1,2 not supported" );
                        return EINVAL;
                    }
                    if ( ssi_chan_mask & SSI_CHANNEL_MASK_012 ) {
                        ado_error_fmt( "Some of SSI 0,1,2 were already specified for another channel" );
                        return EINVAL;
                    }
                    rx_ssi_op_mode = SSI_OP_MODE_MULTICH;
                    rx_voice_map = 0x6;
                    // set playback ssi_chan as 0, the use of multiple channels is specified by the op mode
                    rx_ssi_chan = SSI_CHANNEL_0;
                    ssi_chan_mask |= SSI_CHANNEL_MASK_012;
                    ado_debug( DB_LVL_DRIVER, "rx uses multiple SSI 0,1,2" );
                } else {
                    numvalue = strtol( value, NULL, 0 );
                    if ( !rcar_ssi_supported( numvalue ) ) {
                        ado_error_fmt( "SSI %d not supported", numvalue );
                        return EINVAL;
                    }
                    rx_ssi_chan = numvalue;
                    if ( ssi_chan_mask & SSI_CHANNEL_MASK(rx_ssi_chan) ) {
                        ado_error_fmt ( "SSI %d was already specified for another channel", rx_ssi_chan );
                        return EINVAL;
                    }
                    ssi_chan_mask |= (1 << numvalue);
                    ado_debug( DB_LVL_DRIVER, "rx uses SSI %d", numvalue );
                }
                break;
            case RCAR_OPT_TX_USE_SRC:
                if ( rcar_version_get() == RCAR_VERSION_UNKNOWN ) {
                    ado_error_fmt( "RCAR version needs to be specified before %s", opts[opt] );
                    return EINVAL;
                }
                if ( rcar_src_get_supported_range ( &min_idx, &max_idx ) == EOK ) {
                    numvalue = strtol( value, NULL, 0 );
                    if (numvalue) {
                        tx_use_scu |= RCAR_USE_SCU_SRC;
                    } else {
                        tx_use_scu &= ~RCAR_USE_SCU_SRC;
                    }
                } else {
                    ado_error_fmt( "SRC not supported" );
                    return EINVAL;
                }
                break;
            case RCAR_OPT_RX_USE_SRC:
                if (rcar_version_get() == RCAR_VERSION_UNKNOWN) {
                    ado_error_fmt( "RCAR version needs to be specified before %s", opts[opt] );
                    return EINVAL;
                }
                if ( rcar_src_get_supported_range(&min_idx, &max_idx) == EOK ) {
                    numvalue = strtol( value, NULL, 0 );
                    if (numvalue) {
                        rx_use_scu |= RCAR_USE_SCU_SRC;
                    } else {
                        rx_use_scu &= ~RCAR_USE_SCU_SRC;
                    }
                } else {
                    ado_error_fmt( "SRC not supported" );
                    return EINVAL;
                }
                break;
            case RCAR_OPT_TX_USE_DVC:
                if (rcar_version_get() == RCAR_VERSION_UNKNOWN) {
                    ado_error_fmt( "RCAR version needs to be specified before %s", opts[opt] );
                    return EINVAL;
                }
                if ( rcar_cmd_get_supported_range(&min_idx, &max_idx) == EOK ) {
                    numvalue = strtol( value, NULL, 0 );
                    if (numvalue) {
                        tx_use_scu |= RCAR_USE_SCU_DVC;
                    } else {
                        tx_use_scu &= ~RCAR_USE_SCU_DVC;
                    }
                } else {
                    ado_error_fmt( "DVC not supported" );
                    return EINVAL;
                }
                break;
            case RCAR_OPT_RX_USE_DVC:
                if (rcar_version_get() == RCAR_VERSION_UNKNOWN) {
                    ado_error_fmt( "RCAR version needs to be specified before %s", opts[opt] );
                    return EINVAL;
                }
                if ( rcar_cmd_get_supported_range(&min_idx, &max_idx) == EOK ) {
                    numvalue = strtol( value, NULL, 0 );
                    if (numvalue) {
                        rx_use_scu |= RCAR_USE_SCU_DVC;
                    } else {
                        rx_use_scu &= ~RCAR_USE_SCU_DVC;
                    }
                } else {
                    ado_error_fmt( "DVC not supported" );
                    return EINVAL;
                }
                break;
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
                    rcar->ssi_masterslave_mode = SSI_MS_MODE_MASTER_SLAVE;
                } else if ( strcmp( value, "slave") == 0 ) {
                    rcar->ssi_masterslave_mode = SSI_MS_MODE_ALL_SLAVE;
                } else {
                    ado_error_fmt( "Invalid clk_mode %s", value );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "clk_mode %s: %d", value, rcar->ssi_masterslave_mode );
                break;
            case RCAR_OPT_SYNC_START:
                rcar->sync_start_mode = 1;
                break;
            case RCAR_OPT_TX_OP_MODE:
                if ( tx_ssi_op_mode == SSI_OP_MODE_MULTICH ) {
                    ado_error_fmt( "Special TDM modes not supported for multi SSI config" );
                    return EINVAL;
                }
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
                if ( rx_ssi_op_mode == SSI_OP_MODE_MULTICH ) {
                    ado_error_fmt( "Special TDM modes not supported for multi SSI config" );
                    return EINVAL;
                }
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
            case RCAR_OPT_HDMI_CHAN:
                if (rcar_version_get() == RCAR_VERSION_UNKNOWN) {
                    ado_error_fmt( "RCAR version needs to be specified before %s", opts[opt] );
                    return EINVAL;
                }
                numvalue = strtol( value, NULL, 0 );
                if ( rcar_hdmi_supported( numvalue ) ) {
                    hdmi_chan = numvalue;
                } else {
                    ado_error_fmt( "Invalid hdmi_chan %d", numvalue );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "hdmi_chan %d", hdmi_chan );
                break;
            case RCAR_OPT_VER:
                numvalue = rcar_version_parse(value);
                if (numvalue == RCAR_VERSION_UNKNOWN) {
                    ado_debug( DB_LVL_DRIVER, "unknown version string=%s", value );
                    return EINVAL;
                }
                if (rcar_version_set(numvalue) != EOK) {
                    ado_debug( DB_LVL_DRIVER, "version string=%s attempts to override previously set version", value );
                    return EINVAL;
                }
                ado_debug( DB_LVL_DRIVER, "version string=%s", value );
                break;
            case RCAR_OPT_MCLK1:
                numvalue = strtol( value, NULL, 0 );
                rcar->mclk_1 = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk_1 %d", rcar->mclk_1 );
                break;
            case RCAR_OPT_MCLK2:
                numvalue = strtol( value, NULL, 0 );
                rcar->mclk_2 = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk_2 %d", rcar->mclk_2 );
                break;
            case RCAR_OPT_MCLK_FS:
                numvalue = strtol( value, NULL, 0 );
                rcar->mclk_fs = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk_fs %d", rcar->mclk_fs );
                break;
            case RCAR_OPT_MCLK_SOURCE1:
                numvalue = strtol( value, NULL, 0 );
                if (numvalue < AUDIO_CLK_NONE || numvalue > AUDIO_CLKC) {
                    ado_error_fmt("Invalid mclk source 1 value %d", numvalue);
                    return EINVAL;
                }
                rcar->mclk_source_1 = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk_source_1 %d", rcar->mclk_source_1 );
                break;
            case RCAR_OPT_MCLK_SOURCE2:
                numvalue = strtol( value, NULL, 0 );
                if (numvalue < AUDIO_CLK_NONE || numvalue > AUDIO_CLKC) {
                    ado_error_fmt("Invalid mclk source 2 value %d", numvalue);
                    return EINVAL;
                }
                rcar->mclk_source_2 = numvalue;
                ado_debug( DB_LVL_DRIVER, "mclk_source_2 %d", rcar->mclk_source_2 );
                break;
            case RCAR_OPT_SAMPLE_RATE_SSI:
                numvalue = strtol( value, NULL, 0 );
                rcar->ssi_sample_rate = numvalue;
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

    if ( tx_ssi_chan != SSI_CHANNEL_NUM ) {
        if ( rcar_audio_chan_config ( rcar, 1, tx_ssi_chan, hdmi_chan, tx_voice_map, tx_ssi_op_mode, tx_use_scu ) != EOK ) {
            return EINVAL;
        }
    }

    if ( rx_ssi_chan != SSI_CHANNEL_NUM ) {
        if ( rcar_audio_chan_config ( rcar, 0, rx_ssi_chan, SSI_HDMI_CHANNEL_NUM, rx_voice_map, rx_ssi_op_mode, rx_use_scu ) != EOK ) {
            return EINVAL;
        }
    }

    if ( rcar_version_get() == RCAR_VERSION_UNKNOWN ) {
        ado_error_fmt( "RCAR version is unknown" );
        return EINVAL;
    }

    /* if masterslave_mode not specified among options, use the default value */
    if ( rcar->ssi_masterslave_mode == SSI_MS_MODE_NUM ) {
        rcar->ssi_masterslave_mode = DEFAULT_SSI_MASTERSLAVE_MODE;
    }

    /* if RCAR_OPT_SAMPLE_RATE or RCAR_OPT_SAMPLE_RATE_LIST options not specified,
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

    /* when adding MLP support change this validation to include also MLP use
     * for either playback or capture, not just SSI use */
    if ( !ssi_chan_mask ) {
        ado_error_fmt( "No SSI specified for either transmit or receive" );
        return EINVAL;
    }

    rcar->config_ssi_chan_mask = ssi_chan_mask;

    /* the only reason to configure multiple SSI in the same driver instance is pin sharing
     * between the different SSIs; pin sharing can occur if the used SSIs are a subset of
     * SSI {0,1,2,9} or {0,1,2,3} or {3,4,9} or {7,8};
     * there is one case when although a single SSI is configured for the driver instance,
     * pin sharing must be used - it is the case of SSI8, which doesn't have its own clocks
     * and must always use SSI7 clocks */
    if ( ( count_set_bits ( ssi_chan_mask ) > 1 ) || ( ssi_chan_mask & SSI_CHANNEL_MASK_8 ) ) {
        ssi_channel_t sharedclk_ssi_chan = rcar_get_sharedclk_ssi ( ssi_chan_mask );
        if ( sharedclk_ssi_chan == SSI_CHANNEL_NUM ) {
            ado_error_fmt( "SSIs %x can not be configured in pin sharing mode", ssi_chan_mask );
            return EINVAL;
        }
        rcar->ssi_pin_mode = SSI_SHARED_PINS;
        if ( ( SSI_CHANNEL_MASK(sharedclk_ssi_chan) & ssi_chan_mask ) == 0 ) {
            if ( rcar_reserve_ssi( sharedclk_ssi_chan, sharedclk_ssi_chan ) != EOK ) {
                ado_error_fmt( "Could not reserve resource sharedclk_ssi_chan=%d", sharedclk_ssi_chan );
                return EINVAL;
            }
            rcar->sharedclk_ssi_chan = sharedclk_ssi_chan;
        } else {
            rcar->sharedclk_ssi_chan = sharedclk_ssi_chan;
        }
        ado_debug ( DB_LVL_DRIVER, "Shared clk SSI is SSI%d", rcar->sharedclk_ssi_chan );
    }

    /* Determine the global SCU and SRC use summary */
    uint32_t channel_count = 0;
    uint32_t scu_channel_count = 0;
    uint32_t src_channel_count = 0;
    uint32_t is_mono = 0;

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan ++ ) {
            rcar_audio_channel_t* audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->ssi_chan == SSI_CHANNEL_NUM ) {
                continue;
            }
            if ( audio_chan->ssi_op_mode == SSI_OP_MODE_MONO ) {
                is_mono = 1;
            }
            if ( audio_chan->ssi_transfer_mode == SSI_BUSIF_TRANSFER ) {
                rcar->busif_ssi_chan_mask |= SSI_CHANNEL_MASK(audio_chan->ssi_chan);
            }
            channel_count++;
            if ( audio_chan->use_scu ) {
                scu_channel_count++;
            }
            if ( audio_chan->use_scu & RCAR_USE_SCU_SRC ) {
                src_channel_count++;
            }
        }
    }

    if ( !src_channel_count ) {
        rcar->use_src_summary = RCAR_CHANNELS_NONE;
    } else if ( src_channel_count < channel_count ) {
        rcar->use_src_summary = RCAR_CHANNELS_SOME;
    } else {
        rcar->use_src_summary = RCAR_CHANNELS_ALL;
    }

    if ( !scu_channel_count ) {
       rcar->use_scu_summary = RCAR_CHANNELS_NONE;
    } else if ( scu_channel_count < channel_count ) {
        rcar->use_scu_summary = RCAR_CHANNELS_SOME;
    } else {
        rcar->use_scu_summary = RCAR_CHANNELS_ALL;
    }

    /* validate that the sample_rate_max or sample_rate_ssi, ssi_voices and slot_size combination
       can be supported by one of the mclk_1 and mclk_2 specified in the variant.h file */
    if ( rcar->use_src_summary == RCAR_CHANNELS_NONE ) {
        /* if SRC is used, the SSI sample rate is fixed at all times to SSI_SAMPLE_RATE or its config file/command line
         * override; if no SRC is used, set SSI sample rate to 0; it will be set to the pcm channel configured
         * sample rate in playback or capture acquire, and reset to 0 in playback or capture release
         */
        rcar->ssi_sample_rate = 0;
        sample_rate_max = rcar->sample_rate_max;
    } else {
        sample_rate_max = rcar->ssi_sample_rate;
    }

    if ( rcar->mclk_1 != 0 &&
         rcar->mclk_1 / sample_rate_max * sample_rate_max == rcar->mclk_1 &&
         sample_rate_max * rcar->ssi_voices * rcar->slot_size > rcar->mclk_1 ) {
        ado_error_fmt("Max sample rate %d, slot_size %d and voices %d not supported by MCLK rate %d",
                      sample_rate_max, rcar->slot_size, rcar->ssi_voices, rcar->mclk_1);
        return EINVAL;
    }

    if ( rcar->mclk_2 != 0 &&
         rcar->mclk_2 / sample_rate_max * sample_rate_max == rcar->mclk_2 &&
         sample_rate_max * rcar->ssi_voices * rcar->slot_size > rcar->mclk_2 ) {
        ado_error_fmt("Max sample rate %d, slot_size %d and voices %d not supported by MCLK rate %d",
                      sample_rate_max, rcar->slot_size, rcar->ssi_voices, rcar->mclk_2);
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
        default:
            ado_error_fmt( "Unsupported slot size %d", rcar->slot_size );
            return EINVAL;
    }

    switch( rcar->sample_size ) {
        case 16:
            rcar->ssi_config.data_word_length      = SSI_DATA_WORD_LEN_16BIT;
            break;
        case 24:
            rcar->ssi_config.data_word_length      = SSI_DATA_WORD_LEN_24BIT;
            break;
        case 32:
            rcar->ssi_config.data_word_length      = SSI_DATA_WORD_LEN_32BIT;
            break;
        default:
            ado_error_fmt( "Unsupported sample size %d", rcar->sample_size );
            return EINVAL;
    }

    return EOK;
}

static void rcar_config_dump ( HW_CONTEXT_T * rcar )
{
    ado_debug (DB_LVL_DRIVER, "rcar");

    ado_debug ( DB_LVL_DRIVER, "sample_rates 0x%x, sample_rate_min %d, sample_rate_max %d, ssi_sample_rate %d",
        rcar->sample_rates, rcar->sample_rate_min, rcar->sample_rate_max, rcar->ssi_sample_rate );
    ado_debug ( DB_LVL_DRIVER, "sample_size %d, slot_size %d, ssi_voices %d, masterslave_mode %d, sync_start_mode %d, ssi_pin_mode %d",
        rcar->sample_size, rcar->slot_size, rcar->ssi_voices, rcar->ssi_masterslave_mode, rcar->sync_start_mode, rcar->ssi_pin_mode );
    ado_debug ( DB_LVL_DRIVER, "sharedclk_ssi_chan %d, config_ssi_chan_mask 0x%x, busif_ssi_chan_mask 0x%x, acquired_ssi_chan_mask 0x%x, active_ssi_chan_mask 0x%x",
        rcar->sharedclk_ssi_chan, rcar->config_ssi_chan_mask, rcar->busif_ssi_chan_mask, rcar->acquired_ssi_chan_mask, rcar->active_ssi_chan_mask);
    ado_debug ( DB_LVL_DRIVER, "use_src_summary %d, use_scu_summary %d", rcar->use_src_summary, rcar->use_scu_summary );

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            rcar_audio_channel_t* audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->ssi_chan == SSI_CHANNEL_NUM ) continue;
            ado_debug ( DB_LVL_DRIVER, "Chan[%d][%d] ssi_chan %d, ssi_subchan %d, src_chan %d, cmd_chan %d, hdmi_chan %d, ssi_transfer_mode %d, ssi_op_mode %d",
                i, chan, audio_chan->ssi_chan, audio_chan->ssi_subchan, audio_chan->src_chan, audio_chan->cmd_chan,
                audio_chan->hdmi_chan, audio_chan->ssi_transfer_mode, audio_chan->ssi_op_mode );
        }
    }
}

static void rcar_register_dump ( HW_CONTEXT_T * rcar )
{
    uint32_t dumped_ssi_chan_mask = 0;

    ado_debug (DB_LVL_DRIVER, "rcar");

    ssiu_common_register_dump();

    if ( rcar->sharedclk_ssi_chan != SSI_CHANNEL_NUM ) {
        ssiu_ssi_register_dump ( rcar->sharedclk_ssi_chan );
        dumped_ssi_chan_mask |= SSI_CHANNEL_MASK(rcar->sharedclk_ssi_chan);
    }

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            rcar_audio_channel_t* audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->ssi_chan == SSI_CHANNEL_NUM ) {
                continue;
            }
            if ( ! ( SSI_CHANNEL_MASK (audio_chan->ssi_chan) & dumped_ssi_chan_mask ) ) {
                ssiu_ssi_register_dump( audio_chan->ssi_chan );
                dumped_ssi_chan_mask |= SSI_CHANNEL_MASK(audio_chan->ssi_chan);
            }
            if ( audio_chan->ssi_op_mode == SSI_OP_MODE_MULTICH ) {
                ssiu_ssi_register_dump( SSI_CHANNEL_0 );
                ssiu_ssi_register_dump( SSI_CHANNEL_1 );
                ssiu_ssi_register_dump( SSI_CHANNEL_2 );
                dumped_ssi_chan_mask |= SSI_CHANNEL_MASK_012;
                if ( audio_chan->voices == 8 ) {
                    ssiu_ssi_register_dump( SSI_CHANNEL_9 );
                    dumped_ssi_chan_mask |= SSI_CHANNEL_MASK_9;
                }
            } else {
                if ( ! ( SSI_CHANNEL_MASK (audio_chan->ssi_chan) & dumped_ssi_chan_mask ) ) {
                    ssiu_ssi_register_dump( audio_chan->ssi_chan );
                    dumped_ssi_chan_mask |= SSI_CHANNEL_MASK(audio_chan->ssi_chan);
                }
            }
            if( audio_chan->use_scu ) {
                scu_src_register_dump( audio_chan->src_chan );
                if ( audio_chan->use_scu & RCAR_USE_SCU_CMD ) {
                    scu_cmd_register_dump ( audio_chan->cmd_chan );
                    if( audio_chan->use_scu & RCAR_USE_SCU_DVC ) {
                        scu_dvc_register_dump( audio_chan->cmd_chan );
                    }
                }
            }
        }
    }

    adg_register_dump();

    rcar_mixer_register_dump();
}

static int rcar_init_cleanup( HW_CONTEXT_T * rcar )
{
    rcar_audio_channel_t* audio_chan = NULL;

    ado_debug (DB_LVL_DRIVER, "rcar");

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            if ( rcar->devices[i].audio_channels[chan].ssi_chan == SSI_CHANNEL_NUM ) {
                continue;
            }
            if ( rcar->devices[i].audio_channels[chan].ssi_op_mode == SSI_OP_MODE_MULTICH ) {
                audio_chan = &rcar->devices[i].audio_channels[chan];
                break;
            }
        }
    }

    /* First perform SSIU multi channel clean-up and related SCU cleanup, if applicable */
    if ( audio_chan ) {
        if ( audio_chan->voices == 6 ) {
            ssiu_multichannel_cleanup ( SSI_CHANNEL_MASK_012 );
            rcar->config_ssi_chan_mask &= ~SSI_CHANNEL_MASK_012;
        } else {
            ssiu_multichannel_cleanup ( SSI_CHANNEL_MASK_0129 );
            rcar->config_ssi_chan_mask &= ~SSI_CHANNEL_MASK_0129;
        }
        /* just in case, stop the SRC and CMD */
        if ( audio_chan->src_chan != SCU_SRC_CHANNEL_NUM ) {
            scu_src_stop(audio_chan->src_chan);
        }
        if ( audio_chan->cmd_chan != SCU_CMD_CHANNEL_NUM ) {
            scu_cmd_stop(audio_chan->cmd_chan);
        }
        if ( audio_chan->src_chan != SCU_SRC_CHANNEL_NUM ) {
            scu_src_cleanup(audio_chan->src_chan);
        }
        if( audio_chan->cmd_chan != SCU_CMD_CHANNEL_NUM ) {
            scu_dvc_cleanup(audio_chan->cmd_chan);
        }
    }

    /* Now perform SSIU clean-up and related SCU cleanup for any independent channels */
    if ( rcar->config_ssi_chan_mask ) {
        ssiu_indepchannel_common_cleanup ( rcar->config_ssi_chan_mask );

        for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
            for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
                if ( rcar->devices[i].audio_channels[chan].ssi_chan == SSI_CHANNEL_NUM ) {
                    continue;
                }
                audio_chan = &rcar->devices[i].audio_channels[chan];
                if ( rcar->config_ssi_chan_mask & SSI_CHANNEL_MASK(audio_chan->ssi_chan) ) {
                    ssiu_indepchannel_cleanup ( audio_chan->ssi_chan );
                    rcar->config_ssi_chan_mask &= ~SSI_CHANNEL_MASK(audio_chan->ssi_chan);

                    /* just in case, stop the SRC and CMD */
                    if ( audio_chan->src_chan != SCU_SRC_CHANNEL_NUM ) {
                        scu_src_stop(audio_chan->src_chan);
                    }
                    if ( audio_chan->cmd_chan != SCU_CMD_CHANNEL_NUM ) {
                        scu_cmd_stop(audio_chan->cmd_chan);
                    }
                    if ( audio_chan->src_chan != SCU_SRC_CHANNEL_NUM ) {
                        scu_src_cleanup(audio_chan->src_chan);
                    }
                    if( audio_chan->cmd_chan != SCU_CMD_CHANNEL_NUM ) {
                        scu_dvc_cleanup(audio_chan->cmd_chan);
                    }
                }
            }
        }
    }

    if ( rcar->config_ssi_chan_mask == SSI_CHANNEL_MASK(rcar->sharedclk_ssi_chan) ) {
        ssiu_indepchannel_cleanup ( rcar->sharedclk_ssi_chan );
        rcar->config_ssi_chan_mask &= ~SSI_CHANNEL_MASK(rcar->sharedclk_ssi_chan);
    }

    return EOK;
}

static int rcar_init( HW_CONTEXT_T * hw )
{
    int status = EOK;
    uint32_t setup_ssi_chan_mask = 0;
    ssi_op_mode_t ssi_op_mode = SSI_OP_MODE_NUM;
    rcar_context_t* rcar = (rcar_context_t*) hw;
    rcar_audio_channel_t* audio_chan = NULL;

    ado_debug (DB_LVL_DRIVER, "INIT");

    if ( rcar->debug ) {
        rcar_config_dump( rcar );
    }

    /* first let's set up SSIU for multichannel mode if applicable */
    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->ssi_chan == SSI_CHANNEL_NUM ) {
                continue;
            }
            if ( audio_chan->ssi_op_mode == SSI_OP_MODE_MULTICH ) {
                uint32_t ssi_chan_mask = audio_chan->voices == 6 ? SSI_CHANNEL_MASK_012 : SSI_CHANNEL_MASK_0129;
                uint32_t is_transmit = ( chan == ADO_PCM_CHANNEL_PLAYBACK ? 1 : 0 );
                status = ssiu_multichannel_setup ( ssi_chan_mask, is_transmit, rcar->ssi_masterslave_mode,
                    rcar->sample_size, audio_chan->hdmi_chan, &rcar->ssi_config );
                if ( status != EOK ) {
                    break;
                }
                setup_ssi_chan_mask |= ssi_chan_mask;
                break;
            }
        }
        if ( status != EOK || setup_ssi_chan_mask ) break;
    }

    if ( status != EOK ) {
        ado_error_fmt("failed setting up SSIU for multi channel config");
        rcar_init_cleanup( rcar );
        return status;
    }

    /* next let's set up SSIU for independent channel mode if applicable */
    if ( setup_ssi_chan_mask != rcar->config_ssi_chan_mask ) {
        uint32_t ssi_chan_mask = rcar->config_ssi_chan_mask & ~setup_ssi_chan_mask;
        uint32_t ssi_start_mode = SSI_INDEPENDENT_START;

        if ( ssi_chan_mask == SSI_CHANNEL_MASK_34 && rcar->sync_start_mode ) {
            ssi_start_mode = SSI_SYNC_SSI34_START;
        }

        status = ssiu_indepchannel_common_setup ( ssi_chan_mask, rcar->busif_ssi_chan_mask, rcar->ssi_masterslave_mode, ssi_start_mode );

        if ( status == EOK ) {
            for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
                for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
                    uint32_t uses_busif = 0;
                    uint32_t is_transmit = chan == ADO_PCM_CHANNEL_PLAYBACK ? 1 : 0;
                    audio_chan = &rcar->devices[i].audio_channels[chan];
                    if ( ( audio_chan->ssi_chan == SSI_CHANNEL_NUM ) ||
                         ( SSI_CHANNEL_MASK(audio_chan->ssi_chan) & setup_ssi_chan_mask ) ) {
                        continue;
                    }
                    audio_chan->ssi_start_mode = ssi_start_mode;
                    if ( audio_chan->ssi_transfer_mode == SSI_BUSIF_TRANSFER ) {
                        uses_busif = 1;
                    }
                    status = ssiu_indepchannel_setup ( audio_chan->ssi_chan, rcar->sharedclk_ssi_chan, is_transmit, uses_busif,
                        audio_chan->ssi_op_mode, rcar->ssi_masterslave_mode, audio_chan->ssi_start_mode, rcar->sample_size, rcar->ssi_voices,
                        audio_chan->hdmi_chan, &rcar->ssi_config, rcar->tdm_exsplit_map[audio_chan->ssi_chan] );
                    if ( status != EOK ) break;

                    setup_ssi_chan_mask |= SSI_CHANNEL_MASK(audio_chan->ssi_chan);
                    ssi_op_mode = audio_chan->ssi_op_mode;
                }
                if ( status != EOK ) break;
            }
        }
    }

    if ( status != EOK ) {
        ado_error_fmt("failed setting up SSIU for independent channel config");
        rcar_init_cleanup( rcar );
        return status;
    }

    /* Now set up the shared clock source SSI channel, if not done already */
    if ( (rcar->sharedclk_ssi_chan != SSI_CHANNEL_NUM) &&
         !(SSI_CHANNEL_MASK(rcar->sharedclk_ssi_chan) & setup_ssi_chan_mask) ) {
        status = ssiu_indepchannel_setup ( rcar->sharedclk_ssi_chan, rcar->sharedclk_ssi_chan, 0, 0,
                     ssi_op_mode, rcar->ssi_masterslave_mode, SSI_INDEPENDENT_START, rcar->sample_size,
                     rcar->ssi_voices, SSI_HDMI_CHANNEL_NUM, &rcar->ssi_config, 0 );
        if ( status != EOK ) {
            ado_error_fmt("failed setting up shared clock SSI %d", rcar->sharedclk_ssi_chan);
            rcar_init_cleanup( rcar );
            return status;
        }
    }

    /* Finally, set up the SCU */
    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan <= ADO_PCM_CHANNEL_CAPTURE; chan++ ) {
            uint32_t src_freq_in, src_freq_out;
            uint32_t in_sync, out_sync;
            audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->ssi_chan == SSI_CHANNEL_NUM || !audio_chan->use_scu ) {
                continue;
            }

            if( audio_chan->use_scu & RCAR_USE_SCU_DVC ) {
                /* Initialize the DVC volume variables to 0dB for all channels */
                for( uint32_t v = 0; v < sizeof(audio_chan->dvc_volume)/sizeof(audio_chan->dvc_volume[0]); v++ ) {
                    audio_chan->dvc_volume[v] = SCU_DVC_MAX_VOLUME; /* SCU_DVC_MAX_VOLUME corresponds to 0 dB */
                }

                /* Initialize the DVC mute to unmuted for all channels */
                audio_chan->dvc_mute = 0;
            }

            /* If using SRC set the freq_in, freq_out accordingly, otherwise zero
             * them to indicate that only a basic setup of the SRC is needed, to
             * be able to use the DVC functionality
             */
            if ( chan == ADO_PCM_CHANNEL_PLAYBACK ) {
                if( audio_chan->use_scu & RCAR_USE_SCU_SRC ) {
                    src_freq_in = rcar->sample_rate_max;
                    src_freq_out = rcar->ssi_sample_rate;
                } else {
                    src_freq_in = src_freq_out = 0;
                }
                in_sync = 1;
                out_sync = 0;
            } else {
                if( audio_chan->use_scu & RCAR_USE_SCU_SRC ) {
                    src_freq_in = rcar->ssi_sample_rate;
                    src_freq_out = rcar->sample_rate_max;
                } else {
                    src_freq_in = src_freq_out = 0;
                }
                in_sync = 0;
                out_sync = (audio_chan->use_scu & RCAR_USE_SCU_DVC) ? 0 : 1;
            }

            /* Setup syncronous SRC */
            status = scu_src_setup ( audio_chan->src_chan, in_sync, out_sync,
                rcar->sample_size, audio_chan->voices, src_freq_in, src_freq_out );

            if ( status != EOK ) {
                ado_error_fmt("failed setting up SRC");
                break;
            }

            if ( audio_chan->use_scu & RCAR_USE_SCU_CMD ) {
                /* use SRC -> CMD */
                status = scu_cmd_setup( audio_chan->cmd_chan, audio_chan->src_chan, 0 );

                /* setup DVC */
                if ( status == EOK && ( audio_chan->use_scu & RCAR_USE_SCU_DVC ) ) {
                     status = scu_dvc_setup ( audio_chan->cmd_chan, rcar->sample_size, audio_chan->voices,
                         audio_chan->dvc_volume, audio_chan->dvc_mute );
                }

                if ( status != EOK ) {
                    ado_error_fmt("failed setting up DVC");
                    break;
                }
            }

            if ( status != EOK ) break;
        }
    }

    if ( status != EOK ) {
        ado_error_fmt("failed setting up SCU");
        rcar_init_cleanup( rcar );
        return status;
    }

    return EOK;
}

static int rcar_set_clock_rate_codec_ssi( rcar_context_t * rcar, uint32_t sample_rate )
{
    int ret = EOK;
    uint32_t divisor = 0;
    uint32_t ssi_divisor = 0;
    uint32_t adg_divisor = 0;
    uint32_t ssi_mclk_src = AUDIO_CLKA;
    uint32_t ssi_mclk_rate = 0;
    uint32_t ssi_sample_rate;

    ado_debug( DB_LVL_DRIVER, "sample_rate=%d", sample_rate );

    if( rcar->ssi_sample_rate ) {
        ssi_sample_rate = rcar->ssi_sample_rate;
    } else {
        ssi_sample_rate = sample_rate;
    }
    ado_debug( DB_LVL_DRIVER, "ssi_sample_rate=%d", ssi_sample_rate );

    if ( rcar->mclk_1 != 0 && rcar->mclk_1 / ssi_sample_rate * ssi_sample_rate == rcar->mclk_1 ) {
        ssi_mclk_rate = rcar->mclk_1;
        ssi_mclk_src = rcar->mclk_source_1;
    } else if ( rcar->mclk_2 != 0 && rcar->mclk_2 / ssi_sample_rate * ssi_sample_rate == rcar->mclk_2 ) {
        ssi_mclk_rate = rcar->mclk_2;
        ssi_mclk_src = rcar->mclk_source_2;
    } else if ( rcar->mclk_fs != 0 ) {
        ssi_mclk_rate = ssi_sample_rate * rcar->mclk_fs;
        ssi_mclk_src = MCLK_SOURCE_FS;
    } else {
        ado_error_fmt("Could not find a valid mclk source for sample_rate %d", ssi_sample_rate);
        return EINVAL;
    }

    ado_debug( DB_LVL_DRIVER, "Selected SSI clock source %s with mclk rate %d",
               ( ssi_mclk_src == AUDIO_CLKA ? "AUDIO_CLKA" : "AUDIO_CLKB" ), ssi_mclk_rate );

    divisor = ssi_mclk_rate / rcar->slot_size / rcar->ssi_voices / ssi_sample_rate;

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
        default:
            ado_error_fmt("Unsupported divisor %d", divisor);
            return EINVAL;
    }

    ado_debug( DB_LVL_DRIVER, "Set SSI divisor to %d, ADG divisor to %d",
               ssi_divisor, adg_divisor );

    for ( uint32_t ssi_chan = 0; ssi_chan < SSI_CHANNEL_NUM; ssi_chan++ ) {
        if ( ( rcar->config_ssi_chan_mask & SSI_CHANNEL_MASK(ssi_chan) ) ||
             ( rcar->sharedclk_ssi_chan == ssi_chan ) ) {
            ret = ssi_set_divisor( ssi_chan, ssi_divisor );
            if (ret != EOK) {
                ado_error_fmt( "failed setting the SSI divisor for SSI %d", ssi_chan );
                return ret;
            }

            if ( ssi_chan != SSI_CHANNEL_8 ) {
                ret = adg_set_ssi_clksrc ( ssi_chan, ssi_mclk_src );
                if (ret != EOK) {
                    ado_error_fmt( "failed setting the ADG Clock Select for SSI %d", ssi_chan );
                    return ret;
                }
                ret = adg_set_ssi_clkdiv ( ssi_chan, adg_divisor );
                if (ret != EOK) {
                    ado_error_fmt( "failed setting the ADG divisor for SSI %d", ssi_chan );
                    return ret;
                }
            }
        }
    }

    /* set the mixer/codec clock rate if applicable */
    ret = rcar_mixer_set_clock_rate( rcar, ssi_sample_rate );
    if (ret != EOK) {
        ado_error_fmt( "failed setting the mixer clock rate" );
        return ret;
    }

#if _NTO_VERSION >= 700
    ret = VARIANT_SET_CLOCK_RATE( rcar, ssi_mclk_rate, 0, ssi_sample_rate );
    if( ret != EOK ) {
        ado_error_fmt ("Failed to set variant clock rate");
        return ret;
    }
#endif

    return EOK;
}

static int rcar_set_clock_rate_scu ( rcar_context_t * rcar, rcar_audio_channel_t* audio_chan, uint32_t sample_rate )
{
    int ret = EOK;

    ado_debug ( DB_LVL_DRIVER, "sample_rate=%d", sample_rate );

    if ( audio_chan->use_scu ) {
        uint32_t scu_clk_src_ssiws = ( rcar->sharedclk_ssi_chan != SSI_CHANNEL_NUM ?
                                       rcar->sharedclk_ssi_chan : audio_chan->ssi_chan );

        /* Re-setup SRC with new sample rate, if SRC function used; if SRC function
         * not used, the SRC has been already properly set-up at init
         */

        if ( audio_chan->chan == ADO_PCM_CHANNEL_PLAYBACK ) { /* playback */
            if ( audio_chan->use_scu & RCAR_USE_SCU_SRC ) {
                /* Set up the SRC */
                ret = scu_src_setup ( audio_chan->src_chan, 1, 0, rcar->sample_size, audio_chan->voices,
                    sample_rate, rcar->ssi_sample_rate );

                if ( ret != EOK) {
                    ado_error_fmt( "failed setting up the SRC" );
                    return ret;
                }
            }

            /* The SRC input timing source doesn't need to be set, as the input operates in synchronous mode */

            /* Set the SRC output timing source to be the master SSI channel's WS */
            ret = adg_set_src_clksrc_ssiws ( audio_chan->src_chan, scu_clk_src_ssiws, 1 );
            if ( ret != EOK) {
                ado_error_fmt ( "failed setting the SRC out clock source" );
                return ret;
            }

            if ( audio_chan->use_scu & RCAR_USE_SCU_CMD ) {
                /* Set the CMD output timing source to be the master SSI channel's WS */
                ret = adg_set_cmd_clksrc_ssiws ( audio_chan->cmd_chan, scu_clk_src_ssiws );
                if ( ret != EOK ) {
                    ado_error_fmt ( "failed setting the CMD clock source" );
                    return ret;
                }
            }
        } else { /* capture */
            uint32_t sync_out = ( audio_chan->use_scu & RCAR_USE_SCU_DVC ) ? 0 : 1;
            /* TODO: see if synco_cmd in CMD_ROUTE_SELECT can be used to always have sync_out=1
             * and not need an SRC and CMD output timing source in the scenario of capture with
             * both SRC and CMD in use */
            if ( audio_chan->use_scu & RCAR_USE_SCU_SRC ) {
                ret = scu_src_setup ( audio_chan->src_chan, 0, sync_out, rcar->sample_size, audio_chan->voices,
                    rcar->ssi_sample_rate, sample_rate );
                if ( ret != EOK) {
                    ado_error_fmt ( "failed setting up the SRC" );
                    return ret;
                }
            }

            /* Set the SRC input timing source to be the master SSI channel's WS */
            ret = adg_set_src_clksrc_ssiws ( audio_chan->src_chan, scu_clk_src_ssiws, 0 );
            if ( ret != EOK) {
                ado_error_fmt ( "failed setting the SRC in clock source" );
                return ret;
            }

            if ( audio_chan->use_scu & RCAR_USE_SCU_DVC ) {
                /* Re-setup DVC  */
                ret = scu_dvc_setup ( audio_chan->cmd_chan, rcar->sample_size, audio_chan->voices,
                     audio_chan->dvc_volume, audio_chan->dvc_mute );
                if ( ret != EOK) {
                    ado_error_fmt ( "failed setting up the DVC" );
                    return ret;
                }
            }

            if( ( audio_chan->use_scu & RCAR_USE_SCU_SRC ) &&
                ( audio_chan->use_scu & RCAR_USE_SCU_CMD ) ) {
                if ( !sync_out ) {
                    uint32_t scu_divisor = 0;
                    uint32_t scu_mclk_src = AUDIO_CLK_NONE;
                    uint32_t scu_mclk_rate = 0;

                    /* If both SRC and CMD are used, have to set the SRC and CMD output timing
                     * source based on the capture sample rate
                     */
                    if ( rcar->mclk_1 != 0 && rcar->mclk_1 / sample_rate * sample_rate == rcar->mclk_1 ) {
                        scu_mclk_rate = rcar->mclk_1;
                        scu_mclk_src = rcar->mclk_source_1;
                    } else if ( rcar->mclk_2 != 0 && rcar->mclk_2 / sample_rate * sample_rate == rcar->mclk_2 ) {
                        scu_mclk_rate = rcar->mclk_2;
                        scu_mclk_src = rcar->mclk_source_2;
                    } else {
                        ado_error_fmt ( "Could not find a valid mclk source for sample_rate %d", sample_rate );
                        return EINVAL;
                    }

                    ado_debug ( DB_LVL_DRIVER, "Selected capture SRC out clock source %s with mclk rate %d",
                                ( scu_mclk_src == AUDIO_CLKA ? "AUDIO_CLKA" : "AUDIO_CLKB" ), scu_mclk_rate );

#if _NTO_VERSION >= 700
                    ret = VARIANT_SET_CLOCK_RATE( rcar, 0, scu_mclk_rate, sample_rate );
                    if ( ret != EOK ) {
                        ado_error_fmt ( "Failed to set variant clock rate" );
                        return ret;
                    }
#endif

                    /* The SRC/CMD output clock is a word select clock, not a bit clock
                     * scu_divisor = scu_mclk_rate / sample_rate
                     */
                    scu_divisor = scu_mclk_rate / sample_rate;

                    ado_debug ( DB_LVL_DRIVER, "Set SCU divisor to %d", scu_divisor );

                    ret = adg_set_src_clksrc ( audio_chan->src_chan, scu_mclk_src, 1 );
                    if ( ret != EOK ) {
                        ado_error_fmt ( "failed setting the SRC out clock source" );
                        return ret;
                    }

                    ret = adg_set_src_clkdiv ( audio_chan->src_chan, scu_divisor, 1 );
                    if ( ret != EOK ) {
                        ado_error_fmt ( "failed setting the SRC out clock divisor" );
                        return ret;
                    }

                    /* Set the CMD output timing source to be based on the capture sample rate */
                    ret = adg_set_cmd_clksrc ( audio_chan->cmd_chan, scu_mclk_src );
                    if ( ret != EOK ) {
                        ado_error_fmt ( "failed setting the CMD clock source" );
                        return ret;
                    }

                    ret = adg_set_cmd_clkdiv ( audio_chan->cmd_chan, scu_divisor );
                    if ( ret != EOK ) {
                        ado_error_fmt ( "failed setting the CMD clock divisor" );
                        return ret;
                    }
                } else {
                    /* No need to set SRC or CMD output timing source, just configure
                     * the CMD to use synchronous SRC output data timing;
                     * for now this path is inactive (sync_out always 0 for both SRC and
                     * CMD in use), as synchronous SRC output data timing for the CMD
                     * doesn't seem to work at this time - TODO: clarify this with Renesas
                     */
                    scu_cmd_setup ( audio_chan->cmd_chan, audio_chan->src_chan, 1 );
                }
            } else if (audio_chan->use_scu & RCAR_USE_SCU_CMD) {
                /* Set the SRC output timing source to be the master SSI channel's WS */
                ret = adg_set_src_clksrc_ssiws ( audio_chan->src_chan, scu_clk_src_ssiws, 1 );
                if ( ret != EOK ) {
                    ado_error_fmt( "failed setting the SRC out clock source" );
                    return ret;
                }
                /* Set the CMD output timing source to be the master SSI channel's WS */
                ret = adg_set_cmd_clksrc_ssiws ( audio_chan->cmd_chan, scu_clk_src_ssiws );
                if ( ret != EOK ) {
                    ado_error_fmt( "failed setting the CMD clock source" );
                    return ret;
                }
            }
        }
    }

    return EOK;
}

int rcar_set_dvc_volume(rcar_context_t * rcar, rcar_audio_channel_t* audio_chan, uint32_t voice, uint32_t vol)
{
    int status;
    status = scu_dvc_set_vol ( audio_chan->cmd_chan, voice, vol );
    if ( status == EOK ) {
        audio_chan->dvc_volume[voice] = vol;
    }
    return status;
}

int rcar_get_dvc_volume ( rcar_context_t * rcar, rcar_audio_channel_t* audio_chan, uint32_t voice, uint32_t *vol )
{
    if ( vol == NULL ) {
        return EINVAL;
    }
    if ( voice >= sizeof(audio_chan->dvc_volume) / sizeof(audio_chan->dvc_volume[0]) ) {
        return EINVAL;
    }
    *vol = audio_chan->dvc_volume[voice];
    return EOK;
}

int rcar_set_dvc_mute ( rcar_context_t * rcar, rcar_audio_channel_t* audio_chan, uint32_t mute )
{
    int status;
    status = scu_dvc_set_mute ( audio_chan->cmd_chan, mute );
    if (status == EOK) {
        audio_chan->dvc_mute = mute;
    }
    return status;
}

int rcar_get_dvc_mute ( rcar_context_t * rcar, rcar_audio_channel_t* audio_chan, uint32_t *mute )
{
    return scu_dvc_get_mute ( audio_chan->cmd_chan, mute );
}

ado_dll_version_t ctrl_version;
#if _NTO_VERSION >= 710
void ctrl_version ( int *major, int *minor, const char **date )
{
    *major = ADO_MAJOR_VERSION;
    *minor = 1;
    *date = __DATE__;
}
#else
void ctrl_version ( int *major, int *minor, char *date )
{
    *major = ADO_MAJOR_VERSION;
#if _NTO_VERSION < 700
    *minor = 2;
#else
    *minor = 1;
#endif
    date = __DATE__;
    (void)(date); /* Unused */
}
#endif

static void ctrl_init_cleanup(rcar_context_t * rcar)
{
    ado_debug (DB_LVL_DRIVER, "rcar");

    rcar_init_cleanup (rcar);
    ado_mutex_destroy (&rcar->hw_lock);

    if (rcar->init_flags & RCAR_INIT_FLAG_SSIU) {
        ssiu_deinit();
        rcar->init_flags &= ~RCAR_INIT_FLAG_SSIU;
    }
    if (rcar->init_flags & RCAR_INIT_FLAG_SCU) {
        scu_deinit();
        rcar->init_flags &= ~RCAR_INIT_FLAG_SCU;
    }
    if (rcar->init_flags & RCAR_INIT_FLAG_ADG) {
        adg_deinit();
        rcar->init_flags &= ~RCAR_INIT_FLAG_ADG;
    }
    if (rcar->init_flags & RCAR_INIT_FLAG_DMAC) {
        audio_dmac_deinit(rcar);
        rcar->init_flags &= ~RCAR_INIT_FLAG_DMAC;
    }
#if _NTO_VERSION >= 700
    if (rcar->init_flags & RCAR_INIT_FLAG_VARIANT) {
        (void)VARIANT_UNINIT(rcar);
        rcar->init_flags &= ~RCAR_INIT_FLAG_VARIANT;
    }
#endif
    rcar_release_resources (rcar);
    ado_free (rcar);

    rcar = NULL;
}

/* */
/*  Initialize */
/* */

int ctrl_init (HW_CONTEXT_T ** hw_context, ado_card_t * card, char *args)
{
    rcar_context_t *rcar;
    unsigned long ssi_base;
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

#if _NTO_VERSION >= 700
    /* the hwinfo query works for all gen3 variants, if it fails, then
     * this is a gen2 variant, use the RCAR_GEN2_SSI_BASE as base and
     * as len the size of ssi_reg_t times the max number of SSIs
     */
    uint32_t ssi_len;
    if ( rcar_query_hwinfo(RCAR_HWI_AUDIO_SSI, &ssi_base, &ssi_len) != EOK ) {
        ssi_base = RCAR_GEN2_SSI_BASE;
    }
#else
    ssi_base = RCAR_SSI_BASE;
#endif

    ado_card_set_shortname ( card, "rcar" );
    ado_card_set_longname ( card, "rcar", ssi_base );

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

    /* Map SCU */
    if ( rcar->use_scu_summary != RCAR_CHANNELS_NONE ) {
        if ( (status = scu_init()) != EOK ) {
            ado_error_fmt ("rcar: SCU init failed");
            ctrl_init_cleanup(rcar);
            rcar = NULL;
            return -status;
        }
        rcar->init_flags |= RCAR_INIT_FLAG_SCU;
    }

    /* Map ADG */
    if ( (status = adg_init()) != EOK ) {
        ado_error_fmt ("rcar: ADG init failed");
        ctrl_init_cleanup(rcar);
        rcar = NULL;
        return -status;
    }
    rcar->init_flags |= RCAR_INIT_FLAG_ADG;

    if ( (status = rcar_init(rcar)) != EOK ) {
        ado_error_fmt ("rcar: RCAR hw init failed");
        ctrl_init_cleanup(rcar);
        return -status;
    }

#if _NTO_VERSION >= 700
    if ( (status = rcar_get_i2c_bus_num(&rcar->i2c_dev)) != EOK ) {
        ado_error_fmt ("rcar: Invalid i2c dev");
        return status;
    }

    if ( (status = VARIANT_INIT(rcar) != EOK ) ) {
        ado_error_fmt ("rcar: Unable to perform variant specific init");
        ctrl_init_cleanup(rcar);
        return -status;
    }

    rcar->init_flags |= RCAR_INIT_FLAG_VARIANT;
#endif

    status = rcar_set_clock_rate_codec_ssi ( rcar, rcar->sample_rate_max );

    if( status != EOK ) {
        ado_error_fmt("failed setting up the clock rate for codec and SSI");
        ctrl_init_cleanup( rcar );
        return -status;
    }

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        rcar_audio_channel_t* playback = NULL;
        rcar_audio_channel_t* capture = NULL;
        uint32_t codec_rates;

        if ( rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK].ssi_chan != SSI_CHANNEL_NUM ) {
            playback = &rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_PLAYBACK];
            playback->chmap = ado_pcm_get_default_chmap ( playback->voices );
        }
        if ( rcar->devices[i].audio_channels[ADO_PCM_CHANNEL_CAPTURE].ssi_chan != SSI_CHANNEL_NUM ) {
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
            if ( rcar->devices[i].audio_channels[chan].ssi_chan == SSI_CHANNEL_NUM ) {
                continue;
            }
            audio_chan = &rcar->devices[i].audio_channels[chan];
            status = rcar_set_clock_rate_scu ( rcar, audio_chan, rcar->sample_rate_max );
            if ( status != EOK ) {
                ado_error_fmt("failed setting up the clock rate for SCU");
                ctrl_init_cleanup( rcar );
                return -status;
            }

            ado_debug (DB_LVL_DRIVER, "Attaching DMA interrupt %d", audio_chan->dma_context.audiodma_irq);

            if ( ( status = ado_attach_interrupt ( card, audio_chan->dma_context.audiodma_irq,
                       rcar_interrupt, rcar ) ) != EOK ) {
                /* ado_attach_interrupt returns -errno, no need to swap the status sign before returning status */
                ado_error_fmt ( "rcar: Unable to attach DMA interrupt %d (%s)",
                                audio_chan->dma_context.audiodma_irq, strerror (errno) );
                ctrl_init_cleanup ( rcar );
                return status;
            }

            audio_chan->pcm_caps.chn_flags = SND_PCM_CHNINFO_BLOCK |
                SND_PCM_CHNINFO_INTERLEAVE | SND_PCM_CHNINFO_BLOCK_TRANSFER |
                SND_PCM_CHNINFO_MMAP | SND_PCM_CHNINFO_MMAP_VALID;

            if( rcar->sample_size == 16 ) {
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

            if ( ! ( audio_chan->use_scu & RCAR_USE_SCU_SRC ) ) {
                /* If more than one channel configured and there is a mix of channels using
                 * and not using hardware SRC, the sample rate of the channels not using SRC
                 * must be locked to the SRC_SAMPLE_RATE; update the capabilities to reflect that
                 */
                if ( rcar->use_src_summary == RCAR_CHANNELS_SOME ) {
                    audio_chan->pcm_caps.min_rate = rcar->ssi_sample_rate;
                    audio_chan->pcm_caps.max_rate = rcar->ssi_sample_rate;
                    audio_chan->pcm_caps.rates = ado_pcm_rate2flag(rcar->ssi_sample_rate);
                }

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
            }

            audio_chan->pcm_funcs.aquire = rcar_acquire;
            audio_chan->pcm_funcs.release = rcar_release;
            audio_chan->pcm_funcs.prepare = rcar_prepare;
            if ( chan == ADO_PCM_CHANNEL_PLAYBACK ) {
                audio_chan->pcm_funcs.trigger = rcar_playback_trigger;
            } else {
                audio_chan->pcm_funcs.trigger = rcar_capture_trigger;
            }
#if _NTO_VERSION < 700
            audio_chan->pcm_funcs.position3 = rcar_position;
            audio_chan->pcm_funcs.capabilities2 = rcar_capabilities;
#else
            audio_chan->pcm_funcs.position = rcar_position;
            audio_chan->pcm_funcs.capabilities = rcar_capabilities;
#endif
        }


        /* Create a PCM audio device */
#if _NTO_VERSION < 700
        if ( ado_pcm_create ( card, "R-Car SSI", 0, "rcar",
                 playback ? 1 : 0, playback ? &playback->pcm_caps : NULL, playback ? &playback->pcm_funcs : NULL,
                 capture ? 1 : 0, capture ? &capture->pcm_caps : NULL, capture ? &capture->pcm_funcs : NULL,
                 &rcar->pcm ) != EOK ) {
            /* ado_pcm_create returns -1 on error, assign status to errno if set, else EINVAL */
            status = errno ? errno : EINVAL;
            ado_error_fmt ( "rcar: Unable to create pcm devices (%s)", strerror (errno) );
            ctrl_init_cleanup ( rcar );
            return -status;
        }
#else
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
#endif
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

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/rcar_dll.c $ $Rev: 911285 $")
#endif
