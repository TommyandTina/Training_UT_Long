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

#include "rcar_mixer.h"
#include "variant.h"

/* Note: the following volume range definitions are all
 * identical across multiple mounted instances of the deva rcar driver,
 * thus it is safe to define as static variables; they should be const as
 * well, except for the fact that they get passed to snd_mixer APIs that
 * take non-const arguments */

/* TODO: this routine should also take an index parameter, for the case that the
 * deva driver supports multiple playback or capture channels */
static bool rcar_mixer_is_active ( HW_CONTEXT_T *rcar, int channel )
{
    uint32_t index = 0;
    if ( rcar->devices[index].audio_channels[channel].state != RCAR_CHN_IDLE ) {
        return ( rcar->devices[index].audio_channels[channel].state == RCAR_CHN_STARTED ? true : false );
    }

    return (false);
}

static const snd_pcm_chmap_t* rcar_mixer_get_chmap ( HW_CONTEXT_T *rcar, int channel, int index )
{
    if ( rcar->devices[index].audio_channels[channel].state != RCAR_CHN_IDLE ) {
        return ( rcar->devices[index].audio_channels[channel].chmap );
    }

    return (NULL);
}

static int32_t
build_codec_mixer ( ado_card_t * card, HW_CONTEXT_T * rcar, rcar_audio_device_t* dev )
{
    static uint32_t built_codec_mixer = 0;
    int32_t status;
    uint32_t codec_rate;

    if ( built_codec_mixer ) {
        return EALREADY;
    }

    memset ( &rcar->codec_mixer_callbacks, 0, sizeof (rcar->codec_mixer_callbacks) );

    ado_mixer_dll_codec_params_t params = {0};

    params.hw_context = rcar;

    if ( rcar->sample_rate_min == rcar->sample_rate_max ) {
        codec_rate = rcar->sample_rate_max;
    } else {
        //codec_rate = 48000;
        codec_rate = rcar->sample_rate_max;
    }
    params.tx_sample_rate = codec_rate;
    params.rx_sample_rate = codec_rate;
    params.tx_sample_size = rcar->sample_size;
    params.rx_sample_size = rcar->sample_size;
    params.tx_voices = rcar->ssi_voices;
    params.rx_voices = rcar->ssi_voices;

    /* The codec clock mode will be the opposite of the Audio Controllers clock mode */
    if ( rcar->ssi_masterslave_mode == SSI_MS_MODE_MASTER ) {
        params.clk_master = false;
    } else {
        params.clk_master = true;
    }

    /* set i2c_dev, i2c_addr, mclk to invalid values, these should be passed as config file codec params */
    params.i2c_dev = -1;
    params.i2c_addr = -1;
    params.mclk = 0;

    /* mixer -> ctrl callbacks */
    params.is_active = rcar_mixer_is_active;
    params.get_chmap = rcar_mixer_get_chmap;

    /* If NULL is passed in as the DLL name, this will cause the ado_mixer_dll() call
     * to look in the audio configuration file for the dll name (mixer_dll=xxxx)
     */
    if ( ( status = ado_mixer_dll ( card, NULL, I2S_CODEC_MINOR_VERSION, &params,
               &rcar->codec_mixer_callbacks, &rcar->codec_mixer ) ) ) {
        /* ado_mixer_dll returns -1 on failure, return instead ENOENT */
        status = errno;
        ado_error_fmt ( "ado_mixer_dll failed (%d)", status);
    } else if ( rcar->codec_mixer_callbacks.codec_set_default_group == NULL ) {
        ado_error_fmt ( "Mixer DLL missing codec_set_default_group callback" );
        status = ENOTSUP;
        ado_mixer_destroy ( rcar->codec_mixer );
    }

    if ( status == EOK ) {
        built_codec_mixer = 1;
    }

    return status;
}

int rcar_mixer_set_clock_rate( HW_CONTEXT_T * rcar, uint32_t sample_rate )
{
    int status = EOK;
   
    if ( rcar->codec_mixer && rcar->codec_mixer_callbacks.codec_set_rate ) {
        rcar->codec_mixer_callbacks.codec_set_rate ( rcar->codec_mixer, sample_rate, ADO_PCM_CHANNEL_PLAYBACK );
    }

    if ( status != EOK ) {
        ado_error_fmt ("Failed to set codec clock rate");
        return status;
    }

    return EOK;
}

int rcar_mixer_supported_rates( HW_CONTEXT_T* rcar, uint32_t* rates )
{
    if (!rates) {
        return EINVAL;
    }
    if ( rcar->codec_mixer ) {
        if ( rcar->codec_mixer_callbacks.codec_capabilities &&
             rcar->codec_mixer_callbacks.codec_set_rate ) {
            ado_mixer_dll_codec_capabilities_t cap = {0};
            rcar->codec_mixer_callbacks.codec_capabilities ( rcar->codec_mixer, &cap, ADO_PCM_CHANNEL_PLAYBACK );
            /* Ignore non-standard rates, as they are not supported by the RCar SoCs, still
             * need to free the memory for ratelist if allocated by the codec_capabilities callback
             */
            if (cap.ratelist.list && cap.ratelist.list_free) {
                cap.ratelist.list_free(cap.ratelist.list);
            }
            *rates = cap.rates;
        } else if ( !rcar->codec_mixer_callbacks.codec_set_rate ) {
            if ( rcar->sample_rate_min == rcar->sample_rate_max ) {
                *rates = ado_pcm_rate2flag (rcar->sample_rate_max);
            } else {
                uint32_t rate_flag = ado_pcm_rate2flag(rcar->sample_rate_min);
                int32_t codec_rate;
                *rates = 0;
                while (true) {
                    codec_rate = ado_pcm_flag2rate(rate_flag);
                    if (!codec_rate) break;
                    if ((codec_rate >= rcar->sample_rate_min) && (codec_rate <= rcar->sample_rate_max) ) {
                        *rates |= rate_flag;
                    }
                    rate_flag <<= 1;
                }
            }
        } else {
            *rates = SND_PCM_RATE_ALL;
        }
    } else {
        *rates = SND_PCM_RATE_ALL;
    }
    return EOK;
}

/* if the driver variant supports a hardware codec mixer, build and use
 * the corresponding mixer elements/group; if dvc enabled for either
 * playback or capture setup the SCU mixer DVC volume/mute mixer elements
 * and group
 */
int rcar_mixer_init ( ado_card_t* card, HW_CONTEXT_T* rcar, rcar_audio_device_t* dev )
{
    int status = EOK;
    if ( !card || !rcar || !dev ) {
        return EINVAL;
    }

    /* Attempt to build the  codec mixer as separate mixers, 
    * as they use different mixer contexts and their
     * elements/groups can't be merged under a common mixer; if none of the scu and codec
     * mixers get created, create a simple mixer
     */

    status = build_codec_mixer ( card, rcar, dev );
    if ( status == ENOENT ) {
        ado_debug ( DB_LVL_DRIVER, "No codec mixer available for card %d", ado_get_cardno(card) );
    } else if ( status != EOK && status != EALREADY ) {
        ado_error_fmt ( "Failed building a codec mixer (%d)", status );
        return status;
    }

    if ( rcar->codec_mixer && &rcar->devices[0] == dev ) {
        /* associate the codec mixer with the first device, in case there is more than one device */
        dev->mixer = rcar->codec_mixer;
    } else {
        if ( ado_mixer_create ( card, "mixer", &dev->mixer, rcar ) != EOK ) {
            /* ado_mixer_create returns -1 and sets errno on failure */
            status = errno;
            ado_error_fmt ( "Failed building a simple mixer (%d)", status );
            return status;
        }
    }

    return EOK;
}

void
rcar_mixer_set_default_group ( HW_CONTEXT_T* rcar, ado_pcm_t *pcm, int channel, int index )
{

    if ( index >= RCAR_MAX_DEVICES || rcar->devices[index].pcm != pcm ) {
        return;
    }

    /* Check if a codec mixer got created; there is only one codec mixer instance per rcar driver instance,
     * but it might support multiple transmit/receive paths - pass the audio_device index to the codec's
     * codec_set_default_group callback
     */
    if ( rcar->codec_mixer ) {
        /* if a mixer dll is used, use its default group handling */
        if ( rcar->codec_mixer_callbacks.codec_set_default_group ) {
            rcar->codec_mixer_callbacks.codec_set_default_group ( pcm, rcar->codec_mixer, channel, index );
        }
    }
}

