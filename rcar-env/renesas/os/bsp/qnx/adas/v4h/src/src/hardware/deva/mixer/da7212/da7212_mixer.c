/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

struct da7212_context;
#define MIXER_CONTEXT_T struct da7212_context

#include <audio_driver.h>
#include <mixer/i2s_codec_dll.h>
#include "da7212.h"

static int32_t pcm_devices[1] = { 0 };

/* Output volume range of the DA7212 DAC */
static struct snd_mixer_element_volume1_range da7212_hp_output_range[2] = {
    {DA7212_MIN_HP_VOL, DA7212_MAX_HP_VOL, -5700, 600, 100},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {DA7212_MIN_HP_VOL, DA7212_MAX_HP_VOL, -5700, 600, 100}     // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
};

/* Mic input volume range of the DA7212 ADC */
static struct snd_mixer_element_volume1_range da7212_mic_input_range[2] = {
    {DA7212_MIN_MIC_VOL, DA7212_MAX_MIC_VOL, -600, 3600, 100},    // min, max, min_dB, max_dB, dB_scale_factor (MIC)
    {DA7212_MIN_MIC_VOL, DA7212_MAX_MIC_VOL, -600, 3600, 100}     // min, max, min_dB, max_dB, dB_scale_factor (MIC)
};

/* Minxin input volume range of the DA7212 ADC */
static struct snd_mixer_element_volume1_range da7212_mixin_input_range[2] = {
    {DA7212_MIN_MIXIN_VOL, DA7212_MAX_MIXIN_VOL, -450, 1800, 100},    // min, max, min_dB, max_dB, dB_scale_factor (MINXIN)
    {DA7212_MIN_MIXIN_VOL, DA7212_MAX_MIXIN_VOL, -450, 1800, 100}     // min, max, min_dB, max_dB, dB_scale_factor (MINXIN)
};

static int32_t
da7212_hp_output_vol_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                             uint32_t * vol, void *instance_data )
{
    uint32_t data[NUM_CHAN];
    int32_t altered = 0;
    int chan;

    /* get output volume from hardware */
    da7212_hp_output_vol_get ( da7212, data );

    if ( set ) {
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            if ( data[chan] != vol[chan] ) {
                data[chan] = vol[chan];
                ado_debug ( DB_LVL_DRIVER, "da7212 : changing channel %u output volume to %x", chan, data[chan] );
                altered = 1;
                /* set output volume to CODEC */
                da7212_hp_output_vol_set ( da7212, chan, data[chan] );
            }
        }
    } else { /* read volume */
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            vol[chan] = data[chan];
        }
    }

    return altered;
}

static int32_t
da7212_hp_output_mute_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                              uint32_t * mute, void *instance_data )
{
    uint32_t data[NUM_CHAN];
    uint32_t altered = 0;
    int chan;

    da7212_hp_output_mute_get ( da7212, data );

    if ( set ) {
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            if ( data[chan] != mute[chan] ) {
                data[chan] = mute[chan];
                ado_debug ( DB_LVL_DRIVER, "da7212 : changing channel %u output mute to %x", chan, data[chan] );
                altered = 1;

                da7212_hp_output_mute_set ( da7212, chan, data[chan] );
            }
        }
    } else { /* read mute */
        mute[LEFT_CHAN] = data[LEFT_CHAN];
        mute[RIGHT_CHAN] = data[RIGHT_CHAN];
    }

    return altered;
}

static int32_t
da7212_mixin_input_vol_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                               uint32_t * vol, void *instance_data )
{
    uint32_t data[NUM_CHAN];
    int32_t altered = 0;
    int chan;

    /* get mixin input volume from hardware */
    da7212_mixin_input_vol_get ( da7212, data );

    if ( set ) {
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            if ( data[chan] != vol[chan] ) {
                data[chan] = vol[chan];
                ado_debug ( DB_LVL_DRIVER, "da7212 : changing channel %u mixin input volume to %x", chan, data[chan] );
                altered = 1;

                /* set mixin input volume to CODEC */
                da7212_mixin_input_vol_set ( da7212, chan, data[chan] );
            }
        }

    } else { /* read volume */
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            vol[chan] = data[chan];
        }
    }

    return altered;
}

static int32_t
da7212_mixin_input_mute_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                                uint32_t * mute, void *instance_data )
{
    uint32_t data[NUM_CHAN];
    uint32_t altered = 0;
    int chan;

    da7212_mixin_input_mute_get ( da7212, data );

    if ( set ) {
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            if ( data[chan] != mute[chan] ) {
                data[chan] = mute[chan];
                ado_debug ( DB_LVL_DRIVER, "da7212 : changing channel %u minxin input mute to %x", chan, data[chan] );
                altered = 1;

                da7212_mixin_input_mute_set ( da7212, chan, data[chan] );
            }
        }
    } else { /* read mute */
        mute[LEFT_CHAN] = data[LEFT_CHAN];
        mute[RIGHT_CHAN] = data[RIGHT_CHAN];
    }

    return altered;
}

static int32_t
da7212_mic_1_input_mute_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                                uint32_t * mute, void *instance_data )
{
    uint32_t data;
    uint32_t altered = 0;

    da7212_mic_1_input_mute_get ( da7212, &data );
    if ( set ) {
        altered = data != *mute;
        if ( altered ) {
            da7212_mic_1_input_mute_set ( da7212, *mute );
        }
    } else { /* read mute */
        *mute = data;
    }

    return altered;
}

static int32_t
da7212_mic_1_input_vol_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                               uint32_t * vol, void *instance_data )
{
    uint32_t data;
    uint32_t altered = 0;

    da7212_mic_1_input_vol_get ( da7212, &data );
    if ( set ) {
        altered = data != *vol;
        if ( altered ) {
            da7212_mic_1_input_vol_set ( da7212, *vol );
        }
    } else {
        *vol = data;
    }

    return altered;
}

static int32_t
da7212_mic_2_input_mute_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                                uint32_t * mute, void *instance_data )
{
    uint32_t data;
    uint32_t altered = 0;

    da7212_mic_2_input_mute_get ( da7212, &data );
    if ( set ) {
        altered = data != *mute;
        if ( altered ) {
            da7212_mic_2_input_mute_set( da7212, *mute );
        }
    } else { /* read mute */
        *mute = data;
    }

    return altered;
}

static int32_t
da7212_mic_2_input_vol_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                               uint32_t * vol, void *instance_data )
{
    uint32_t data;
    uint32_t altered = 0;

    da7212_mic_2_input_vol_get ( da7212, &data );
    if ( set ) {
        altered = data != *vol;
        if ( altered ) {
            da7212_mic_2_input_vol_set ( da7212, *vol );
        }
    } else {
        *vol = data;
    }

    return altered;
}

static int32_t
da7212_playack_capture_control ( MIXER_CONTEXT_T * da7212, ado_mixer_delement_t * element, uint8_t set,
                               uint32_t * cap, void *instance_data )
{
    uint32_t data;

    if ( set ) {
        da7212_dig_routing_dai_get ( da7212, &data, *cap );
        if ( data ) {
            da7212_dig_routing_dai_set ( da7212, *cap );
        }

        da7212_dig_routing_dac_get ( da7212, &data, *cap );
        if ( data ) {
            da7212_dig_routing_dac_set ( da7212, *cap );
        }

        if (*cap)
            printf("=== Capture mode ===\n");
        else
            printf("=== Playback mode ===\n");
    }

    return 0;
}

/*
 *  build_da7212_mixer
 */
static int32_t
build_da7212_mixer ( MIXER_CONTEXT_T *da7212 )
{
    int error = 0;
    ado_mixer_delement_t *pre_elem, *vol_elem, *mute_elem, *cap_elem = NULL, *elem = NULL;

    const snd_pcm_chmap_t *chmap = ado_pcm_get_default_chmap ( 2 );


    ado_debug ( DB_LVL_DRIVER, "DA7212" );

    /* ################# */
    /* the OUTPUT GROUPS */
    /* ################# */
    if ( !error && (elem = ado_mixer_element_pcm1 ( da7212->mixer, SND_MIXER_ELEMENT_PLAYBACK,
                SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0] )) == NULL)
        error++;

    pre_elem = elem;

    if ( !error && (vol_elem = ado_mixer_element_volume1 ( da7212->mixer, "Headphone Volume",
                2, da7212_hp_output_range, da7212_hp_output_vol_control, NULL, NULL )) == NULL )
        error++;

    /* route pcm to volume */
    if ( !error && ado_mixer_element_route_add(da7212->mixer, pre_elem, vol_elem) != 0 )
        error++;

    pre_elem = vol_elem;

    if ( !error && (mute_elem = ado_mixer_element_sw2 ( da7212->mixer, "Headphone Mute",
                da7212_hp_output_mute_control, NULL, NULL )) == NULL )
        error++;

    /* route volume to mute */
    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, mute_elem ) != 0 )
        error++;

    pre_elem = mute_elem;

    if ( !error && (elem = ado_mixer_element_io ( da7212->mixer, "DAC Output",
                SND_MIXER_ETYPE_OUTPUT, 0, chmap )) == NULL )
        error++;

    if ( !error && ado_mixer_element_route_add (da7212->mixer, pre_elem, elem) != 0 )
        error++;

    pre_elem = elem;

    if ( !error && ado_mixer_playback_group_create ( da7212->mixer, SND_MIXER_HEADPHONE_OUT,
                chmap, vol_elem, mute_elem ) == NULL )
        error++;

    /* #################### */
    /* the CAPTURE GROUPS   */
    /* #################### */
    if ( !error && (elem = ado_mixer_element_pcm1 ( da7212->mixer, SND_MIXER_ELEMENT_CAPTURE,
                SND_MIXER_ETYPE_CAPTURE1, 1, &pcm_devices[0] )) == NULL )
        error++;

    pre_elem = elem;

    if ( !error && (vol_elem = ado_mixer_element_volume1 ( da7212->mixer, "Mixin Volume",
                2, da7212_mixin_input_range, da7212_mixin_input_vol_control, NULL, NULL )) == NULL )
        error++;

    /* route pcm to volume */
    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, vol_elem ) != 0 )
        error++;

    pre_elem = vol_elem;

    if ( !error && (mute_elem = ado_mixer_element_sw2 ( da7212->mixer, "Mixin Mute",
                da7212_mixin_input_mute_control, NULL, NULL )) == NULL )
        error++;

    /* route volume to mute */
    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, mute_elem ) != 0 )
        error++;

    pre_elem = mute_elem;

    if ( !error && (elem = ado_mixer_element_io ( da7212->mixer, "ADC INPUT",
                SND_MIXER_ETYPE_INPUT, 0, chmap )) == NULL )
    error++;

    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, elem ) != 0 )
        error++;

    if ( !error && (ado_mixer_capture_group_create ( da7212->mixer, "Mixin",
                chmap, vol_elem, mute_elem, NULL, NULL )) == NULL )
        error++;

    /* Mic 1*/
    if ( !error && (elem = ado_mixer_element_pcm1 ( da7212->mixer, SND_MIXER_ELEMENT_CAPTURE,
                SND_MIXER_ETYPE_CAPTURE1, 1, &pcm_devices[0] )) == NULL )
        error++;

    pre_elem = elem;

    if ( !error && (vol_elem = ado_mixer_element_volume1 ( da7212->mixer, "Mixin 1 Volume",
                1, da7212_mic_input_range, da7212_mic_1_input_vol_control, NULL, NULL )) == NULL )
        error++;

    /* route pcm to volume */
    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, vol_elem ) != 0 )
        error++;

    pre_elem = vol_elem;

    if ( !error && (mute_elem = ado_mixer_element_sw2 ( da7212->mixer, "Mic 1 Mute",
                da7212_mic_1_input_mute_control, NULL, NULL )) == NULL )
        error++;

    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, mute_elem ) != 0 )
        error++;

    pre_elem = mute_elem;

    if ( !error && (elem = ado_mixer_element_io ( da7212->mixer, "MIC Input",
                SND_MIXER_ETYPE_INPUT, 0, chmap )) == NULL )
        error++;

    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, elem ) != 0 )
        error++;

    if ( !error && (ado_mixer_capture_group_create ( da7212->mixer, SND_MIXER_MIC_IN,
                chmap, vol_elem, mute_elem, NULL, NULL )) == NULL )
        error++;

    /* Mic 2*/
    if ( !error && (elem = ado_mixer_element_pcm1 ( da7212->mixer, SND_MIXER_ELEMENT_CAPTURE,
                SND_MIXER_ETYPE_CAPTURE1, 1, &pcm_devices[0] )) == NULL )
        error++;

    pre_elem = elem;

    if ( !error && (vol_elem = ado_mixer_element_volume1 ( da7212->mixer, "Mixin 2 Volume",
                1, da7212_mic_input_range, da7212_mic_2_input_vol_control, NULL, NULL )) == NULL )
        error++;

    /* route pcm to volume */
    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, vol_elem ) != 0 )
        error++;

    pre_elem = vol_elem;

    if ( !error && (mute_elem = ado_mixer_element_sw2 ( da7212->mixer, "Mic 2 Mute",
                da7212_mic_2_input_mute_control, NULL, NULL )) == NULL )
        error++;

    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, mute_elem ) != 0 )
        error++;

    pre_elem = mute_elem;

    if ( !error && (elem = ado_mixer_element_io ( da7212->mixer, "MIC Input",
                SND_MIXER_ETYPE_INPUT, 0, chmap )) == NULL )
        error++;

    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, elem ) != 0 )
        error++;

    if ( !error && (ado_mixer_capture_group_create ( da7212->mixer, "Mic 2",
                chmap, vol_elem, mute_elem, NULL, NULL )) == NULL )
        error++;

    /* Playback - Capture rounting */
    if ( !error && (elem = ado_mixer_element_pcm1 ( da7212->mixer, SND_MIXER_ELEMENT_CAPTURE,
                SND_MIXER_ETYPE_CAPTURE1, 1, &pcm_devices[0] )) == NULL )
        error++;

    pre_elem = elem;

    if ( !error && (cap_elem = ado_mixer_element_sw1 ( da7212->mixer, "Playback - Capture",
                1, da7212_playack_capture_control, NULL, NULL )) == NULL )
        error++;

    if ( !error && ado_mixer_element_route_add ( da7212->mixer, pre_elem, cap_elem ) != 0 )
        error++;

    pre_elem = cap_elem;

    if ( !error && (elem = ado_mixer_element_io(da7212->mixer, "MIC Input",
                SND_MIXER_ETYPE_INPUT, 0, chmap )) == NULL )
        error++;

    if ( !error && (ado_mixer_capture_group_create ( da7212->mixer, "Playback - Capture",
                chmap, NULL, NULL, cap_elem, NULL )) == NULL )
        error++;

    if ( !error ) {
        return EOK;
    }

    return ENOMEM;
}

static void
da7212_mixer_set_default_group ( ado_pcm_t *pcm, ado_mixer_t *mixer, int channel, int index )
{
    switch ( channel )
    {
        case ADO_PCM_CHANNEL_PLAYBACK:
            ado_pcm_chn_mixer ( pcm, ADO_PCM_CHANNEL_PLAYBACK, mixer,
                ado_mixer_find_element ( mixer, SND_MIXER_ETYPE_PLAYBACK1, SND_MIXER_ELEMENT_PLAYBACK, index ),
                ado_mixer_find_group ( mixer, SND_MIXER_MASTER_OUT, index ) );
            break;
        case ADO_PCM_CHANNEL_CAPTURE:
            ado_pcm_chn_mixer( pcm, ADO_PCM_CHANNEL_CAPTURE, mixer,
                ado_mixer_find_element ( mixer, SND_MIXER_ETYPE_CAPTURE1, SND_MIXER_ELEMENT_CAPTURE, index ),
                ado_mixer_find_group ( mixer, SND_MIXER_MIC_IN, index ) );
        default:
            break;
    }
}

void da7212_mixer_register_dump ( MIXER_CONTEXT_T *da7212 )
{
    da7212_register_dump ( da7212 );
}

static ado_mixer_reset_t da7212_mixer_reset;
static int32_t da7212_mixer_reset ( MIXER_CONTEXT_T *da7212 )
{
    int32_t status;

    ado_debug ( DB_LVL_MIXER, "MIXER RESET" );
    status = da7212_init ( da7212 );

    return status;
}

static ado_mixer_destroy_t da7212_mixer_destroy;
static int32_t da7212_mixer_destroy ( MIXER_CONTEXT_T *da7212 )
{
    ado_debug ( DB_LVL_MIXER, "MIXER DESTROY" );
    da7212_deinit ( da7212 );

    return EOK;
}

static void da7212_mixer_set_rate ( ado_mixer_t *mixer, uint32_t sample_rate, int channel )
{
    da7212_context_t *da7212 = ado_mixer_get_context ( mixer );

    ado_debug ( DB_LVL_DRIVER, "Setting DA7212 sample rate to %d", sample_rate );

    da7212_rate_setting ( da7212, sample_rate, channel );
}

static void da7212_mixer_capabilities ( ado_mixer_t *mixer, ado_mixer_dll_codec_capabilities_t *cap, int channel )
{
    da7212_context_t *da7212 = ado_mixer_get_context ( mixer );
    da7212_capabilities ( da7212, cap, channel );
}

/* Option names are prefixed with codec name to prevent name collisions
 * with other options handled at higher levels in io-audio;
 * TODO: add option for tdm when needed
 */
static const char* opts[] = {
#define I2C_DEV              0
    "da7212_i2c_dev",
#define I2C_ADDR             1
    "da7212_i2c_addr",
#define MCLK                 2
    "da7212_mclk",
#define DEBUG                3
    "da7212_debug",
    NULL
};

#define N_OPTS ((sizeof(opts)/sizeof(opts[0])) - 1U)

static int
parse_config_opts ( MIXER_CONTEXT_T *da7212 )
{
    const ado_dict_t *dict = NULL;
    const char* opt_str[N_OPTS] = {NULL};
    long value;
    ado_card_t *card = ado_mixer_get_card ( da7212->mixer );

    dict = ado_get_card_dict ( card );

    ado_config_load_key_values ( dict, opts, opt_str, N_OPTS, 0, -1 );

    if ( opt_str[I2C_DEV] != NULL ) {
        if ( ( value = strtol ( opt_str[I2C_DEV], NULL, 0 ) ) >= 0 ) {
            da7212->params.i2c_dev = value;
        } else {
            ado_error_fmt ( "Incorrect value for option %s: %s", opts[I2C_DEV], opt_str[I2C_DEV] );
            return EINVAL;
        }
    }

    if ( opt_str[I2C_ADDR] != NULL ) {
        if ( ( value = strtol ( opt_str[I2C_ADDR], NULL, 0 ) ) != 0 ) {
            da7212->params.i2c_addr = value;
        } else {
            ado_error_fmt ( "Incorrect value for option %s: %s", opts[I2C_ADDR], opt_str[I2C_ADDR] );
            return EINVAL;
        }
    }

    if ( opt_str[MCLK] != NULL ) {
        if ( ( value = strtol ( opt_str[MCLK], NULL, 0 ) ) != 0 ) {
            da7212->params.mclk = value;
        } else {
            da7212->params.mclk = 12288000;
        }
    }

    if ( opt_str[DEBUG] != NULL ) {
        if ( !strncasecmp ( opt_str[DEBUG], "enable", strlen("enable") ) ) {
            da7212->debug = true;
        }
    }

    /* do a basic validation of the options:
     * i2c_dev and i2c_addr must be >= 0 (should have been initialized to -1 in the deva ctrl driver) */
    if ( da7212->params.i2c_dev < 0 || da7212->params.i2c_addr < 0 ) {
        ado_error_fmt ( "Missing or incorrect I2C params: i2c_dev %d, i2c_addr %d",
                        da7212->params.i2c_dev, da7212->params.i2c_addr );
        return EINVAL;
    }

    return EOK;
}

ado_mixer_dll_init_t mixer_dll_init;
int mixer_dll_init ( MIXER_CONTEXT_T ** context, ado_mixer_t * mixer, void *params, void *raw_callbacks, int version )
{
    da7212_context_t *da7212;
    ado_mixer_dll_codec_callbacks_t *callbacks = raw_callbacks;

    ado_debug(DB_LVL_MIXER, "Initializing DA7212 Codec");

    if ( (da7212 = (da7212_context_t *) ado_calloc(1, sizeof (da7212_context_t))) == NULL ) {
        ado_error_fmt( "DA7212: Failed to allocate device structure - %s", strerror(errno) );
        return (-1);
    }
    *context = da7212;
    ado_mixer_set_name ( mixer, "DA7212" );

    da7212->mixer = mixer;

    memcpy ( &da7212->params, params, sizeof (da7212->params) );

    if ( da7212->params.is_active == NULL ) {
        ado_error_fmt ( "DA7212: is_active routine missing" );
        ado_free ( da7212 );
        return (-1);
    }

    if ( parse_config_opts ( da7212 ) != EOK ) {
        ado_error_fmt ( "Failed parsing DA7212 options" );
        ado_free ( da7212 );
        return -1;
    }

    if ( callbacks ) {
        callbacks->codec_set_default_group = da7212_mixer_set_default_group;
        callbacks->codec_set_rate = da7212_mixer_set_rate;
        callbacks->codec_capabilities = da7212_mixer_capabilities;
    }

    if ( build_da7212_mixer ( da7212 ) != EOK ) {
        ado_error_fmt ( "Failed building DA7212 mixer" );
        ado_free ( da7212 );
        return -1;
    }

    if (da7212_mixer_reset ( da7212 ) != EOK) {
        ado_error_fmt ( "Failed to reset DA7212" );
        ado_free ( da7212 );
        return -1;
    }

    ado_mixer_set_reset_func ( da7212->mixer, da7212_mixer_reset );
    ado_mixer_set_destroy_func ( da7212->mixer, da7212_mixer_destroy );

    return 0;
}

ado_dll_version_t version;
void
version ( int *major, int *minor, const char **date )
{
    *major = ADO_MAJOR_VERSION;
    *minor = I2S_CODEC_MINOR_VERSION;
    *date = __DATE__;
}

