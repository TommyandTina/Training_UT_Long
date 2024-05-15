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

struct wm8731_context;
#define MIXER_CONTEXT_T struct wm8731_context

#include <audio_driver.h>
#include <mixer/i2s_codec_dll.h>
#include "wm8731.h"


static int32_t pcm_devices[1] = { 0 };

/* Output volume range of the WM8731 DAC */
static struct snd_mixer_element_volume1_range wm8731_output_range[2] = {
    {WM8731_MIN_DIGITAL_VOL, WM8731_MAX_DIGITAL_VOL, -7300, 600, 100},    // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
    {WM8731_MIN_DIGITAL_VOL, WM8731_MAX_DIGITAL_VOL, -7300, 600, 100}     // min, max, min_dB, max_dB, dB_scale_factor (SPEAKER)
};

static int32_t
wm8731_master_vol_control(MIXER_CONTEXT_T * wm8731, ado_mixer_delement_t * element, uint8_t set,
                          uint32_t * vol, void *instance_data)
{
    uint32_t data[NUM_CHAN];
    int32_t altered = 0;
    int chan;

    /* get output volume from hardware */
    wm8731_output_vol_get(wm8731, data);

    if (set) {
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            if ( data[chan] != vol[chan] ) {
            	data[chan] = vol[chan];
                ado_debug( DB_LVL_DRIVER, "wm8731 : changing channel %u output volume to %x", chan, data[chan] );
                altered = 1;
                /* set output volume to CODEC */
                wm8731_output_vol_set(wm8731, chan, data[chan]);
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
wm8731_master_mute_control(MIXER_CONTEXT_T * wm8731, ado_mixer_delement_t * element, uint8_t set,
                           uint32_t * mute, void *instance_data)
{
    uint32_t data;
    int32_t altered = 0;

    wm8731_output_mute_get(wm8731, &data);

    /* implement joint-mute, as the mute bit for L,R channels is common */
    if (set) {
        altered = data != *mute;
        if (altered) {
            wm8731_output_mute_set(wm8731, *mute);
        }
    } else { /* read mute */
        *mute = data;
    }

    return altered;
}

static int32_t
wm8731_line_input_vol_control(MIXER_CONTEXT_T * wm8731, ado_mixer_delement_t * element, uint8_t set,
                              uint32_t * nvol, void *instance_data)
{
    uint32_t data[NUM_CHAN];
    int32_t altered = 0;
    int chan;

    /* get output volume from hardware */
    wm8731_line_input_vol_get(wm8731, data);

    if (set) {
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            if ( data[chan] != nvol[chan] ) {
            	data[chan] = nvol[chan];
                ado_debug( DB_LVL_DRIVER, "wm8731 : changing channel %u line input volume to %x", chan, data[chan] );
                altered = 1;
                
                /* set line input volume to CODEC */
                wm8731_line_input_vol_set(wm8731, chan, data[chan]);
            }
        }

    } else { /* read volume */
        for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            nvol[chan] = data[chan];
        }
    }

    return altered;
}

static int32_t
wm8731_line_input_mute_control(MIXER_CONTEXT_T * wm8731, ado_mixer_delement_t * element, uint8_t set,
                               uint32_t * mute, void *instance_data)
{
    uint32_t data[NUM_CHAN];
    uint32_t altered = 0;
    int chan;

    wm8731_line_input_mute_get(wm8731, data);

    if (set) {
    	for ( chan = LEFT_CHAN; chan < NUM_CHAN; chan++ ) {
            if ( data[chan] != mute[NUM_CHAN] ) {
            	data[chan] = mute[chan];
                ado_debug( DB_LVL_DRIVER, "wm8731 : changing channel %u line input mute to %x", chan, data[chan] );
                altered = 1;
                
                wm8731_line_input_mute_set(wm8731, chan, data[chan]);
            }
        }
    } else { /* read mute */
        mute[LEFT_CHAN] = data[LEFT_CHAN];
        mute[RIGHT_CHAN] = data[RIGHT_CHAN];
    }

    return altered;
}

static int32_t
wm8731_mic_input_mute_control(MIXER_CONTEXT_T * wm8731, ado_mixer_delement_t * element, uint8_t set,
                              uint32_t * mute, void *instance_data)
{
    uint32_t data;
    uint32_t altered = 0;

    wm8731_mic_input_mute_get(wm8731, &data);
    /* implement joint-mute, as the Line Input Mute to ADC bit for L,R channels is common */
    if (set) {
        altered = data != *mute;
        if (altered) {
            wm8731_mic_input_mute_set(wm8731, *mute);
        }
    } else { /* read mute */
        *mute = data;
    }

    return altered;
}

/*
 *  build_wm8731_mixer
 */
static int32_t
build_wm8731_mixer(MIXER_CONTEXT_T *wm8731)
{
    int error = 0;
    ado_mixer_delement_t *pre_elem, *vol_elem, *mute_elem, *elem = NULL;
    
    const snd_pcm_chmap_t *chmap = ado_pcm_get_default_chmap(2);

    ado_debug (DB_LVL_DRIVER, "WM8731");

    /* ################# */
    /* the OUTPUT GROUPS */
    /* ################# */
    if (!error && (elem = ado_mixer_element_pcm1 (wm8731->mixer, SND_MIXER_ELEMENT_PLAYBACK,
                SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0])) == NULL)
        error++;

    pre_elem = elem;

    if (!error && (vol_elem = ado_mixer_element_volume1 (wm8731->mixer, "Digital Volume",
                2, wm8731_output_range, wm8731_master_vol_control, NULL, NULL)) == NULL)
        error++;

    /* route pcm to volume */
    if (!error && ado_mixer_element_route_add(wm8731->mixer, pre_elem, vol_elem) != 0)
        error++;

    pre_elem = vol_elem;

    if (!error && (mute_elem = ado_mixer_element_sw2 (wm8731->mixer, "Digital Mute",
                wm8731_master_mute_control, NULL, NULL)) == NULL)
        error++;

    /* route volume to mute */
    if (!error && ado_mixer_element_route_add(wm8731->mixer, pre_elem, mute_elem) != 0)
        error++;

    pre_elem = mute_elem;

    if (!error && (elem = ado_mixer_element_io (wm8731->mixer, "DAC Output",
                SND_MIXER_ETYPE_OUTPUT, 0, chmap)) == NULL)
        error++;

    if (!error && ado_mixer_element_route_add (wm8731->mixer, pre_elem, elem) != 0)
        error++;

    pre_elem = elem;

    if (!error && ado_mixer_playback_group_create(wm8731->mixer, SND_MIXER_MASTER_OUT,
                chmap, vol_elem, mute_elem) == NULL)
        error++;

  
    /* #################### */
    /* the CAPTURE GROUPS   */
    /* #################### */
    if (!error && (elem = ado_mixer_element_pcm1 (wm8731->mixer, SND_MIXER_ELEMENT_CAPTURE,
                SND_MIXER_ETYPE_CAPTURE1, 1, &pcm_devices[0])) == NULL)
        error++;

    pre_elem = elem;


    if (!error && (vol_elem = ado_mixer_element_volume1 (wm8731->mixer, "Line Input Volume",
                2, wm8731_output_range, wm8731_line_input_vol_control, NULL, NULL)) == NULL)
        error++;

    /* route pcm to volume */
    if (!error && ado_mixer_element_route_add(wm8731->mixer, pre_elem, vol_elem) != 0)
        error++;

    pre_elem = vol_elem;

    if (!error && (mute_elem = ado_mixer_element_sw1 (wm8731->mixer, "Line Input Mute",
                2, wm8731_line_input_mute_control, NULL, NULL)) == NULL)
        error++;

    /* route volume to mute */
    if (!error && ado_mixer_element_route_add(wm8731->mixer, pre_elem, mute_elem) != 0)
        error++;

    pre_elem = mute_elem;

        if (!error && (elem = ado_mixer_element_io (wm8731->mixer, "ADC INPUT",
                SND_MIXER_ETYPE_INPUT, 0, chmap)) == NULL)
        error++;

    if (!error && ado_mixer_element_route_add (wm8731->mixer, pre_elem, elem) != 0)
        error++;

    if (!error && (ado_mixer_capture_group_create (wm8731->mixer, SND_MIXER_LINE_IN,
                chmap, vol_elem, mute_elem, NULL, NULL)) == NULL ) 
        error++;

    if (!error && (mute_elem = ado_mixer_element_sw2 (wm8731->mixer, "Mic Input Mute",
                wm8731_mic_input_mute_control, NULL, NULL)) == NULL)
        error++;

    /* route volume to mute */
    if (!error && ado_mixer_element_route_add(wm8731->mixer, pre_elem, mute_elem) != 0)
        error++;

    pre_elem = mute_elem;

    if (!error && (elem = ado_mixer_element_io(wm8731->mixer, "MIC Input", 
    	        SND_MIXER_ETYPE_INPUT, 0, chmap)) == NULL) 
        error++;

     if (!error && ado_mixer_element_route_add(wm8731->mixer, pre_elem, elem) != 0)
        error++;

    if (!error && (ado_mixer_capture_group_create (wm8731->mixer, SND_MIXER_MIC_IN,
                chmap, vol_elem, mute_elem, NULL, NULL)) == NULL ) 
        error++;

    if (!error) {
        return EOK;
    }

    return ENOMEM;
}

static void
wm8731_mixer_set_default_group( ado_pcm_t *pcm, ado_mixer_t *mixer, int channel, int index )
{
    switch (channel)
    {
        case ADO_PCM_CHANNEL_PLAYBACK:
            ado_pcm_chn_mixer (pcm, ADO_PCM_CHANNEL_PLAYBACK, mixer,
                ado_mixer_find_element (mixer, SND_MIXER_ETYPE_PLAYBACK1, SND_MIXER_ELEMENT_PLAYBACK, index), 
                ado_mixer_find_group (mixer, SND_MIXER_MASTER_OUT, index));
            break;
        case ADO_PCM_CHANNEL_CAPTURE:
            ado_pcm_chn_mixer(pcm, ADO_PCM_CHANNEL_CAPTURE, mixer,
                ado_mixer_find_element(mixer, SND_MIXER_ETYPE_CAPTURE1, SND_MIXER_ELEMENT_CAPTURE, index),
                ado_mixer_find_group(mixer, SND_MIXER_MIC_IN, index));
        default:
            break;
    }
}

void wm8731_mixer_register_dump( MIXER_CONTEXT_T *wm8731 )
{
    wm8731_register_dump( wm8731 );
}

static ado_mixer_reset_t wm8731_mixer_reset;
static int32_t wm8731_mixer_reset (MIXER_CONTEXT_T *wm8731)
{
    int32_t status;

    ado_debug (DB_LVL_MIXER, "MIXER RESET");

    status = wm8731_init ( wm8731 );

    return status;
}

static ado_mixer_destroy_t wm8731_mixer_destroy;
static int32_t wm8731_mixer_destroy (MIXER_CONTEXT_T *wm8731)
{
    ado_debug ( DB_LVL_MIXER, "MIXER DESTROY" );

    wm8731_deinit( wm8731 );

    return EOK;
}

static void wm8731_mixer_set_rate (ado_mixer_t *mixer, uint32_t sample_rate, int channel)
{
    wm8731_context_t *wm8731 = ado_mixer_get_context(mixer);

    ado_debug ( DB_LVL_DRIVER, "Setting WM8731 sample rate to %d", sample_rate );

    wm8731_rate_setting ( wm8731, sample_rate );
}

static void wm8731_mixer_capabilities (ado_mixer_t *mixer, ado_mixer_dll_codec_capabilities_t *cap, int channel)
{
    wm8731_context_t *wm8731 = ado_mixer_get_context(mixer);

    wm8731_capabilities ( wm8731, cap, channel );
}

/* Option names are prefixed with codec name to prevent name collisions
 * with other options handled at higher levels in io-audio;
 * TODO: add option for tdm when needed
 */
static const char* opts[] = {
#define I2C_DEV              0
    "wm8731_i2c_dev",
#define I2C_ADDR             1
    "wm8731_i2c_addr",
#define DEBUG                2
    "wm8731_debug",
    NULL
};

#define N_OPTS ((sizeof(opts)/sizeof(opts[0])) - 1U)

static int
parse_config_opts ( MIXER_CONTEXT_T *wm8731 )
{
    const ado_dict_t *dict = NULL;
    const char* opt_str[N_OPTS] = {NULL};
    long value;
    ado_card_t *card = ado_mixer_get_card ( wm8731->mixer );

    dict = ado_get_card_dict (card);

    ado_config_load_key_values(dict, opts, opt_str, N_OPTS, 0, -1);

    if ( opt_str[I2C_DEV] != NULL ) {
        if ( ( value = strtol ( opt_str[I2C_DEV], NULL, 0 ) ) >= 0 ) {
            wm8731->params.i2c_dev = value;
        } else {
            ado_error_fmt( "Incorrect value for option %s: %s", opts[I2C_DEV], opt_str[I2C_DEV] );
            return EINVAL;
        }
    }

    if ( opt_str[I2C_ADDR] != NULL ) {
        if ( ( value = strtol ( opt_str[I2C_ADDR], NULL, 0 ) ) != 0 ) {
            wm8731->params.i2c_addr = value;
        } else {
            ado_error_fmt( "Incorrect value for option %s: %s", opts[I2C_ADDR], opt_str[I2C_ADDR] );
            return EINVAL;
        }
    }

    if (opt_str[DEBUG] != NULL) {
        if ( !strncasecmp ( opt_str[DEBUG], "enable", strlen("enable") ) ) {
            wm8731->debug = true;
        }
    }

    /* do a basic validation of the options:
     * i2c_dev and i2c_addr must be >= 0 (should have been initialized to -1 in the deva ctrl driver) */
    if ( wm8731->params.i2c_dev < 0 || wm8731->params.i2c_addr < 0 ) {
        ado_error_fmt ( "Missing or incorrect I2C params: i2c_dev %d, i2c_addr %d",
                        wm8731->params.i2c_dev, wm8731->params.i2c_addr );
        return EINVAL;
    }

    return EOK;
}

ado_mixer_dll_init_t mixer_dll_init;
int mixer_dll_init (MIXER_CONTEXT_T ** context, ado_mixer_t * mixer, void *params, void *raw_callbacks, int version)
{
    wm8731_context_t *wm8731;
    ado_mixer_dll_codec_callbacks_t *callbacks = raw_callbacks;

    ado_debug(DB_LVL_MIXER, "Initializing WM8731 Codec");

    if ( (wm8731 = (wm8731_context_t *) ado_calloc(1, sizeof (wm8731_context_t))) == NULL ) {
        ado_error_fmt( "WM8731: Failed to allocate device structure - %s", strerror(errno) );
        return (-1);
    }
    *context = wm8731;
    ado_mixer_set_name ( mixer, "WM8731" );

    wm8731->mixer = mixer;
    memcpy ( &wm8731->params, params, sizeof (wm8731->params) );

    if ( wm8731->params.is_active == NULL ) {
        ado_error_fmt ( "WM8731: is_active routine missing" );
        ado_free( wm8731 );
        return (-1);
    }

    if ( parse_config_opts(wm8731) != EOK ) {
        ado_error_fmt("Failed parsing WM8731 options");
        ado_free(wm8731);
        return -1;
    }

    if (callbacks) {
        callbacks->codec_set_default_group = wm8731_mixer_set_default_group;
        callbacks->codec_set_rate = wm8731_mixer_set_rate;
        callbacks->codec_capabilities = wm8731_mixer_capabilities;
    }

    if (build_wm8731_mixer(wm8731) != EOK ) {
        ado_error_fmt("Failed building WM8731 mixer");
        ado_free(wm8731);
        return -1;
    }

    if (wm8731_mixer_reset(wm8731) != EOK ) {
        ado_error_fmt("Failed to reset WM8731");
        ado_free(wm8731);
        return -1;
    }

    ado_mixer_set_reset_func(wm8731->mixer, wm8731_mixer_reset);
    ado_mixer_set_destroy_func(wm8731->mixer, wm8731_mixer_destroy);

    return 0;
}

ado_dll_version_t version;
void
version (int *major, int *minor, const char **date)
{
	*major = ADO_MAJOR_VERSION;
	*minor = I2S_CODEC_MINOR_VERSION;
	*date = __DATE__;
}

