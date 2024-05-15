/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
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

#include "rcar_mixer.h"
#include "scu.h"
#include "variant.h"

static int32_t pcm_devices[1] = { 0 };

/* Note: the following volume range definitions are all
 * identical across multiple mounted instances of the deva rcar driver,
 * thus it is safe to define as static variables; they should be const as
 * well, except for the fact that they get passed to snd_mixer APIs that
 * take non-const arguments */

/* Digital output volume range - the SCU supports max volume of +18 dB,
   but for near to full scale PCM data, any digital volume in excess of 0dB
   will result in distortion, limiting the range to digital volume up to 0 dB */
#if _NTO_VERSION < 700
static struct snd_mixer_element_volume1_range   scu_dvc_range[8] = {
    {0, 480, -12000, 0},    // min, max, min_dB, max_dB (SPEAKER)
    {0, 480, -12000, 0},    // min, max, min_dB, max_dB (SPEAKER)
    {0, 480, -12000, 0},    // min, max, min_dB, max_dB (SPEAKER)
    {0, 480, -12000, 0},    // min, max, min_dB, max_dB (SPEAKER)
    {0, 480, -12000, 0},    // min, max, min_dB, max_dB (SPEAKER)
    {0, 480, -12000, 0},    // min, max, min_dB, max_dB (SPEAKER)
    {0, 480, -12000, 0},    // min, max, min_dB, max_dB (SPEAKER)
    {0, 480, -12000, 0}     // min, max, min_dB, max_dB (SPEAKER)
};
#else
static struct snd_mixer_element_volume1_range   scu_dvc_range[8] = {
    {0, 480, -12000, 0, 100, {0}},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {0, 480, -12000, 0, 100, {0}},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {0, 480, -12000, 0, 100, {0}},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {0, 480, -12000, 0, 100, {0}},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {0, 480, -12000, 0, 100, {0}},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {0, 480, -12000, 0, 100, {0}},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {0, 480, -12000, 0, 100, {0}},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {0, 480, -12000, 0, 100, {0}}     // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
};
#endif

/*
 * scu_vol_control - volume control using the SCU DVC block
 */
static int32_t
scu_vol_control
(
    MIXER_CONTEXT_T * rcar,
    ado_mixer_delement_t * element,
    uint8_t set,
    uint32_t * vol,
    void *instance_data
)
{
    int32_t altered = 0;
    rcar_audio_channel_t* audio_chan = instance_data;
    uint32_t data[8];
    uint32_t i;

    if ( !audio_chan ) {
        ado_error_fmt ( "instance_data is NULL ");
        return 0;
    }

    for (i = 0; i < audio_chan->voices; i++) {
        rcar_get_dvc_volume(rcar, audio_chan, i, &data[i]);
    }

    if (set) {
        for (i = 0; i < audio_chan->voices; i++) {
            altered = altered || ( vol[i] != data[i] );
            data[i] = vol[i] < SCU_DVC_MAX_VOLUME ? vol[i] : SCU_DVC_MAX_VOLUME;
            ado_debug( DB_LVL_DRIVER, "rcar : setting DVC volume[%d]=0x%x", i, data[i] );
        }

        ado_mutex_lock (&rcar->hw_lock);

        for (i = 0; i < audio_chan->voices; i++) {
            rcar_set_dvc_volume(rcar, audio_chan, i, data[i]);
        }

        ado_mutex_unlock (&rcar->hw_lock);
    } else {
        for (i = 0; i < audio_chan->voices; i++) {
            vol[i] = data[i];
        }
    }
    return altered;
}

/*
 * scu_mute_control - mute control using the SCU DVC block
 */
static int32_t
scu_mute_control
(
    MIXER_CONTEXT_T * rcar,
    ado_mixer_delement_t * element,
    uint8_t set,
    uint32_t * mute,
    void *instance_data
)
{
    int32_t altered = 0;
    rcar_audio_channel_t* audio_chan = instance_data;
    uint32_t data;

    if ( !audio_chan ) {
        ado_error_fmt ( "instance_data is NULL ");
        return 0;
    }

    rcar_get_dvc_mute(rcar, audio_chan, &data);

    if (set) {
        altered = ( (*mute & 0xFF) != data );
        data = *mute & 0xFF;

        ado_debug( DB_LVL_DRIVER, "rcar : setting DVC mute to 0x%x", data );

        ado_mutex_lock (&rcar->hw_lock);

        rcar_set_dvc_mute(rcar, audio_chan, data);

        ado_mutex_unlock (&rcar->hw_lock);
    } else {
        *mute = data;
    }
    return altered;
}

#if _NTO_VERSION < 710
/* use for mono or stereo configurations */
static snd_mixer_voice_t default_voices[2] = {
    {SND_MIXER_VOICE_LEFT,  0, {0}}, // left channel
    {SND_MIXER_VOICE_RIGHT, 0, {0}}, // right channel
};

/* use for quad channel configuration */
static snd_mixer_voice_t quad_voices[4] = {
    {SND_MIXER_VOICE_LEFT, 0, {0}},
    {SND_MIXER_VOICE_RIGHT, 0, {0}},
    {SND_MIXER_VOICE_REAR_LEFT, 0, {0}},
    {SND_MIXER_VOICE_REAR_RIGHT, 0, {0}}
};

/* use for 5.1 channel configuration */
static snd_mixer_voice_t five_one_voices[6] = {
    {SND_MIXER_VOICE_LEFT, 0, {0}},
    {SND_MIXER_VOICE_RIGHT, 0, {0}},
    {SND_MIXER_VOICE_CENTER, 0, {0}},
    {SND_MIXER_VOICE_REAR_LEFT, 0, {0}},
    {SND_MIXER_VOICE_REAR_RIGHT, 0, {0}},
    {SND_MIXER_VOICE_WOOFER, 0, {0}}
};

/* use for 7.1 channel configuration */
static snd_mixer_voice_t seven_one_voices[8] = {
    {SND_MIXER_VOICE_LEFT, 0, {0}},
    {SND_MIXER_VOICE_RIGHT, 0, {0}},
    {SND_MIXER_VOICE_CENTER, 0, {0}},
    {SND_MIXER_VOICE_REAR_LEFT, 0, {0}},
    {SND_MIXER_VOICE_REAR_RIGHT, 0, {0}},
    {SND_MIXER_VOICE_WOOFER, 0, {0}},
    {SND_MIXER_VOICE_SURR_LEFT, 0, {0}},
    {SND_MIXER_VOICE_SURR_RIGHT, 0, {0}}
};
#endif


/*
 *  build_scu_mixer
 */
static int32_t
build_scu_mixer( ado_card_t * card, HW_CONTEXT_T * rcar, rcar_audio_device_t* dev )
{
    int32_t error = 0;
    int32_t status;
    ado_mixer_delement_t *pre_elem = NULL, *elem = NULL, *vol_elem = NULL, *mute_elem = NULL;
#if _NTO_VERSION < 710
    snd_mixer_voice_t* voices = NULL;
    intptr_t chn_mask = 0;
#endif
    ado_debug (DB_LVL_MIXER, "Creating mixer");

    rcar_audio_channel_t* playback = NULL;
    rcar_audio_channel_t* capture = NULL;

    if ( dev->audio_channels[ADO_PCM_CHANNEL_PLAYBACK].ssi_chan != SSI_CHANNEL_NUM ) {
        playback = &dev->audio_channels[ADO_PCM_CHANNEL_PLAYBACK];
    }

    if ( dev->audio_channels[ADO_PCM_CHANNEL_CAPTURE].ssi_chan != SSI_CHANNEL_NUM ) {
        capture = &dev->audio_channels[ADO_PCM_CHANNEL_CAPTURE];
    }

    if ( ( playback && ( playback->use_scu & RCAR_USE_SCU_DVC ) ) ||
         ( capture && ( capture->use_scu & RCAR_USE_SCU_DVC ) ) ) {
        if (ado_mixer_create( card, "mixer", &dev->scu_mixer, rcar ) != EOK) {
            /* ado_mixer_create returns -1 and sets errno on failure */
            status = errno;
            ado_error_fmt ("Failed to create scu mixer (%d)", status);
            return status;
        }

        ado_debug (DB_LVL_MIXER, "Building mixer");

        if ( playback && (playback->use_scu & RCAR_USE_SCU_DVC) ) {
#if _NTO_VERSION < 710
            switch (playback->voices) {
	    case 1:
                chn_mask = SND_MIXER_CHN_MASK_MONO;
	        voices = default_voices;
	        break;
	    case 2:
	    default:
	        chn_mask = SND_MIXER_CHN_MASK_STEREO;
	        voices = default_voices;
	        break;
	    case 4:
	        chn_mask = SND_MIXER_CHN_MASK_4;
	        voices = quad_voices;
	        break;
	    case 6:
	        chn_mask = SND_MIXER_CHN_MASK_5_1;
	        voices = five_one_voices;
	        break;
	    case 8:
	        chn_mask = SND_MIXER_CHN_MASK_7_1;
	        voices = seven_one_voices;
	        break;
            }
#endif
            if (!error && (elem = ado_mixer_element_pcm1 (dev->scu_mixer, "DVC Playback",
                                                          SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0])) == NULL) {
                error++;
            }
            pre_elem = elem;

            /* #################### */
            /* the DIGITAL CONTROLS */
            /* #################### */

            /* change volume by SCU-DVC */
            if (!error && (vol_elem = ado_mixer_element_volume1 (dev->scu_mixer, "DVC Output Volume",
                                                                 playback->voices, scu_dvc_range,
                                                                 scu_vol_control,
                                                                 playback, NULL)) == NULL) {
                error++;
            }
            if (!error && ado_mixer_element_route_add (dev->scu_mixer, pre_elem, vol_elem) != 0) {
                error++;
            }
            pre_elem = vol_elem;
            /* change mute by SCU DVC */
            if (!error && (mute_elem = ado_mixer_element_sw1 (dev->scu_mixer, "DVC Output Mute",
                                                              playback->voices, scu_mute_control,
                                                              playback, NULL)) == NULL) {
                error++;
            }
            if (!error && ado_mixer_element_route_add(dev->scu_mixer, pre_elem, mute_elem) != 0) {
                error++;
            }
            pre_elem = mute_elem;
#if _NTO_VERSION < 710
            if (!error && (elem = ado_mixer_element_io (dev->scu_mixer, "DVC Output", SND_MIXER_ETYPE_OUTPUT,
                                                        0, playback->voices, voices)) == NULL) {
                error++;
            }
#else
            if (!error && (elem = ado_mixer_element_io (dev->scu_mixer, "DVC Output", SND_MIXER_ETYPE_OUTPUT,
                                                        0, playback->chmap)) == NULL) {
                error++;
            }
#endif
            if (!error && ado_mixer_element_route_add (dev->scu_mixer, pre_elem, elem) != 0) {
                error++;
            }
#if _NTO_VERSION < 710
            if (!error && (ado_mixer_playback_group_create (dev->scu_mixer, "DVC OUT",
                                                            chn_mask, vol_elem, mute_elem)) == NULL) {
                error++;
            }
#else
            if (!error && (ado_mixer_playback_group_create (dev->scu_mixer, "DVC OUT",
                                                            playback->chmap, vol_elem, mute_elem)) == NULL) {
                error++;
            }
#endif
        }

        if ( capture && (capture->use_scu & RCAR_USE_SCU_DVC) ) {
#if _NTO_VERSION < 710
            switch (capture->voices) {
	    case 1:
                chn_mask = SND_MIXER_CHN_MASK_MONO;
	        voices = default_voices;
	        break;
	    case 2:
	    default:
	        chn_mask = SND_MIXER_CHN_MASK_STEREO;
	        voices = default_voices;
	        break;
	    case 4:
	        chn_mask = SND_MIXER_CHN_MASK_4;
	        voices = quad_voices;
	        break;
	    case 6:
	        chn_mask = SND_MIXER_CHN_MASK_5_1;
	        voices = five_one_voices;
	        break;
	    case 8:
	        chn_mask = SND_MIXER_CHN_MASK_7_1;
	        voices = seven_one_voices;
	        break;
            }
            if (!error && (elem = ado_mixer_element_io (dev->scu_mixer, "DVC Input", SND_MIXER_ETYPE_INPUT,
                                                        0, capture->voices, voices)) == NULL ) {
                error++;
            }
#else
            if (!error && (elem = ado_mixer_element_io (dev->scu_mixer, "DVC Input", SND_MIXER_ETYPE_INPUT,
                                                        0, capture->chmap)) == NULL ) {
                error++;
            }
#endif
            pre_elem = elem;

            /* #################### */
            /* the DIGITAL CONTROLS */
            /* #################### */

            /* change volume by SCU-DVC */
            if (!error && (vol_elem = ado_mixer_element_volume1 (dev->scu_mixer, "DVC Input Volume",
                                                                 capture->voices, scu_dvc_range,
                                                                 scu_vol_control, capture, NULL)) == NULL ) {
                error++;
            }
            if ( !error && ado_mixer_element_route_add (dev->scu_mixer, pre_elem, vol_elem) != 0 ) {
                error++;
            }
            pre_elem = vol_elem;
            /* change mute by SCU DVC */
            if (!error && (mute_elem = ado_mixer_element_sw1 (dev->scu_mixer, "DVC Input Mute",
                                                              capture->voices, scu_mute_control,
                                                              capture, NULL)) == NULL) {
                error++;
            }
            if (!error && ado_mixer_element_route_add(dev->scu_mixer, pre_elem, mute_elem) != 0) {
                error++;
            }
            pre_elem = mute_elem;
            if ( !error && (elem = ado_mixer_element_pcm1 (dev->scu_mixer, "DVC Capture",
                                                           SND_MIXER_ETYPE_CAPTURE1, 1, &pcm_devices[0])) == NULL ) {
                error++;
            }
            if ( !error && ado_mixer_element_route_add (dev->scu_mixer, pre_elem, elem) != 0 ) {
                error++;
            }
#if _NTO_VERSION < 710
            if ( !error && (ado_mixer_capture_group_create (dev->scu_mixer, "DVC IN",
                                                            chn_mask, vol_elem, mute_elem, NULL, NULL)) == NULL ) {
                error++;
            }
#else
            if ( !error && (ado_mixer_capture_group_create (dev->scu_mixer, "DVC IN",
                                                            capture->chmap, vol_elem, mute_elem, NULL, NULL)) == NULL ) {
                error++;
            }
#endif
        }
    }

    if ( !error ) {
        return EOK;
    }

    return ENOMEM;
}

#if _NTO_VERSION >= 700
/* TODO: this routine should also take an index parameter, for the case that the
 * deva driver supports multiple playback or capture channels */
static bool rcar_mixer_is_active ( HW_CONTEXT_T *rcar, int channel )
{
    uint32_t index = 0;
    if ( rcar->devices[index].audio_channels[channel].ssi_chan != SSI_CHANNEL_NUM ) {
        return ( rcar->devices[index].audio_channels[channel].state == RCAR_CHN_STARTED ? true : false );
    }

    return (false);
}

static const snd_pcm_chmap_t* rcar_mixer_get_chmap ( HW_CONTEXT_T *rcar, int channel, int index )
{
    if ( rcar->devices[index].audio_channels[channel].ssi_chan != SSI_CHANNEL_NUM ) {
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

    if ( rcar->ssi_sample_rate ) {
        codec_rate = rcar->ssi_sample_rate;
    } else if ( rcar->sample_rate_min == rcar->sample_rate_max ) {
        codec_rate = rcar->sample_rate_max;
    } else {
        codec_rate = 48000;
    }
    params.tx_sample_rate = codec_rate;
    params.rx_sample_rate = codec_rate;
    params.tx_sample_size = rcar->sample_size;
    params.rx_sample_size = rcar->sample_size;
    params.tx_voices = rcar->ssi_voices;
    params.rx_voices = rcar->ssi_voices;

    /* The codec clock mode will be the opposite of the Audio Controllers clock mode */
    if ( rcar->ssi_masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
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
        ado_error_fmt ( "ado_mixer_dll failed (%d)", status );
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
#else
static int32_t
build_codec_mixer( ado_card_t * card, HW_CONTEXT_T * rcar )
{
    int32_t status;
#ifdef USE_CODEC_MIXER
    if ( USE_CODEC_MIXER ) {
        status = CODEC_MIXER_INIT( card, rcar );
        if ( status != EOK ) {
            ado_error_fmt ("Failed to init variant codec mixer");
            return status;
        }
    } else {
        status = ENOENT;
    }
#else
    status = ENOENT;
#endif
    return status;
}
#endif

int rcar_mixer_set_clock_rate( HW_CONTEXT_T * rcar, uint32_t sample_rate )
{
    int status = EOK;

#if _NTO_VERSION >= 700
    if ( rcar->codec_mixer && rcar->codec_mixer_callbacks.codec_set_rate ) {
        rcar->codec_mixer_callbacks.codec_set_rate ( rcar->codec_mixer, sample_rate, ADO_PCM_CHANNEL_PLAYBACK );
    }
#elif defined(CODEC_SET_CLOCK_RATE)
    status = CODEC_SET_CLOCK_RATE ( sample_rate );
#endif

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
#if _NTO_VERSION >= 700
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
            uint32_t codec_rate;
            if ( rcar->ssi_sample_rate ) {
                codec_rate = rcar->ssi_sample_rate;
            } else if ( rcar->sample_rate_min == rcar->sample_rate_max ) {
                codec_rate = rcar->sample_rate_max;
            } else {
                codec_rate = 48000;
            }
            *rates = ado_pcm_rate2flag (codec_rate);
        } else {
            *rates = SND_PCM_RATE_ALL;
        }
    } else {
        *rates = SND_PCM_RATE_ALL;
    }
#else
    *rates = SND_PCM_RATE_ALL;
#endif
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

    /* Attempt to build the scu mixer (if dvc enabled for either playback or capture) and
     * codec mixer as separate mixers, as they use different mixer contexts and their
     * elements/groups can't be merged under a common mixer; if none of the scu and codec
     * mixers get created, create a simple mixer
     */
    status = build_scu_mixer ( card, rcar, dev );
    if ( status != EOK ) {
        ado_error_fmt ( "Failed building an scu mixer (%d)", status );
        return status;
    }
    status = build_codec_mixer ( card, rcar, dev );
    if ( status == ENOENT ) {
        ado_debug ( DB_LVL_DRIVER, "No codec mixer available for card %d", ado_get_cardno(card) );
    } else if ( status != EOK && status != EALREADY ) {
        ado_error_fmt ( "Failed building a codec mixer (%d)", status );
        return status;
    }

    if ( dev->scu_mixer ) {
        dev->mixer = dev->scu_mixer;
    } else if ( rcar->codec_mixer && &rcar->devices[0] == dev ) {
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

void rcar_mixer_register_dump()
{
#if _NTO_VERSION >= 700
    // TODO: Ideally if using the new mixer dll approach, there would be a register_dump callback
    // to invoke here
    VARIANT_REGISTER_DUMP();
#else
    CODEC_REGISTER_DUMP();
#endif
}

void
rcar_mixer_set_default_group ( HW_CONTEXT_T* rcar, ado_pcm_t *pcm, int channel, int index )
{
    rcar_audio_channel_t * audio_chan;
    rcar_audio_device_t * audio_dev;
    ado_mixer_delement_t * delement;
    ado_mixer_dgroup_t * dgroup;

    if ( index >= RCAR_MAX_DEVICES || rcar->devices[index].pcm != pcm ) {
        return;
    }

    audio_chan = &rcar->devices[index].audio_channels[channel];
    audio_dev = &rcar->devices[index];

    if ( audio_chan->ssi_chan == SSI_CHANNEL_NUM ) {
        return;
    }

    /* if the scu mixer got created and has a mixer group for the direction indicated by channel,
     * designate it as the default group for that direction
     */
    if ( audio_dev->scu_mixer && ( audio_chan->use_scu & RCAR_USE_SCU_DVC ) ) {
        switch ( channel )
        {
            case ADO_PCM_CHANNEL_PLAYBACK:
            {
                /* in the context of the SCU mixer of audio_dev there can be a single "DVC Playback"/"DVC OUT"
                 * instance, use index 0 in the ado_mixer_find_* APIs
                 */
                delement = ado_mixer_find_element ( audio_dev->scu_mixer, SND_MIXER_ETYPE_PLAYBACK1, "DVC Playback", 0 );
                dgroup = ado_mixer_find_group ( audio_dev->scu_mixer, "DVC OUT", 0 );
                if ( delement && dgroup ) {
                    ado_pcm_chn_mixer ( pcm, ADO_PCM_CHANNEL_PLAYBACK, audio_dev->scu_mixer, delement, dgroup );
                    return;
                }

                break;
            }
            case ADO_PCM_CHANNEL_CAPTURE:
            {
                /* in the context of the SCU mixer of audio_dev there can be a single "DVC Capture"/"DVC IN"
                 * instance, use index 0 in the ado_mixer_find_* APIs
                 */
                delement = ado_mixer_find_element ( audio_dev->scu_mixer, SND_MIXER_ETYPE_CAPTURE1, "DVC Capture", 0 );
                dgroup = ado_mixer_find_group ( audio_dev->scu_mixer, "DVC IN", 0 );
                if ( delement && dgroup ) {
                    ado_pcm_chn_mixer ( pcm, ADO_PCM_CHANNEL_CAPTURE, audio_dev->scu_mixer, delement, dgroup );
                    return;
                }

                break;
            }

            default:
                break;
        }
    }

    /* next, check if a codec mixer got created; there is only one codec mixer instance per rcar driver instance,
     * but it might support multiple transmit/receive paths - pass the audio_device index to the codec's
     * codec_set_default_group callback
     */
    if ( rcar->codec_mixer ) {
#if _NTO_VERSION >= 700
        /* if a mixer dll is used, use its default group handling */
        if ( rcar->codec_mixer_callbacks.codec_set_default_group ) {
            rcar->codec_mixer_callbacks.codec_set_default_group ( pcm, rcar->codec_mixer, channel, index );
        }
#elif defined( USE_CODEC_MIXER )
        /* if a variant codec mixer is available, use its default group handling */
        if ( USE_CODEC_MIXER ) {
            CODEC_SET_DEFAULT_GROUP( pcm, rcar->codec_mixer, channel, index );
        }
#endif
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/rcar_mixer.c $ $Rev: 911285 $")
#endif
