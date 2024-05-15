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

#include <audio_driver.h>
#include <mixer/i2s_codec_dll.h>

static int32_t pcm_devices[1] = { 0 };

/*
 *  build_codec_mixer
 */
static int32_t
build_codec_mixer(ado_mixer_t * mixer)
{
    int     error = 0;
    ado_mixer_delement_t *pre_elem, *elem = NULL;
    const snd_pcm_chmap_t *chmap = ado_pcm_get_default_chmap(2);

    /* ################# */
    /* the OUTPUT GROUPS */
    /* ################# */
    if (!error && (elem = ado_mixer_element_pcm1 (mixer, SND_MIXER_ELEMENT_PLAYBACK,
                SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0])) == NULL)
        error++;

    pre_elem = elem;

    if (!error && (elem = ado_mixer_element_io (mixer, "DAC Output",
                SND_MIXER_ETYPE_OUTPUT, 0, chmap)) == NULL)
        error++;

    if (!error && ado_mixer_element_route_add (mixer, pre_elem, elem) != 0)
        error++;

    if (!error && ado_mixer_playback_group_create(mixer, SND_MIXER_MASTER_OUT,
                chmap, NULL, NULL) == NULL)
        error++;

    if (!error) {
        return EOK;
    }

    return ENOMEM;
}

static void
codec_mixer_set_default_group( ado_pcm_t *pcm, ado_mixer_t *mixer, int channel, int index )
{
    switch (channel)
    {
        case ADO_PCM_CHANNEL_PLAYBACK:
            ado_pcm_chn_mixer (pcm, ADO_PCM_CHANNEL_PLAYBACK, mixer,
                ado_mixer_find_element (mixer, SND_MIXER_ETYPE_PLAYBACK1,
                    SND_MIXER_ELEMENT_PLAYBACK, index), ado_mixer_find_group (mixer,
                    SND_MIXER_MASTER_OUT, index));
            break;
        case ADO_PCM_CHANNEL_CAPTURE: /* no input group */
        default:
            break;
    }
}

ado_mixer_dll_init_t mixer_dll_init;
int mixer_dll_init (MIXER_CONTEXT_T ** context, ado_mixer_t * mixer, void *params, void *raw_callbacks, int version)
{
    ado_mixer_dll_codec_callbacks_t *callbacks = raw_callbacks;

    ado_mixer_set_name ( mixer, "Sample Codec" );

    if (callbacks) {
        callbacks->codec_set_default_group = codec_mixer_set_default_group;
    }

    if ( build_codec_mixer(mixer) != EOK ) {
        ado_error_fmt("Failed building Codec mixer");
        return -1;
    }

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

