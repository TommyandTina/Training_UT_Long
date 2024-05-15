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

#ifndef _DA7212_H
#define _DA7212_H

#include <stdint.h>
#include <mixer/i2s_codec_dll.h>

#define DA7212_MAX_HP_VOL          63
#define DA7212_MIN_HP_VOL          0

#define DA7212_MAX_MIC_VOL         7
#define DA7212_MIN_MIC_VOL         0

#define DA7212_MAX_MIXIN_VOL       15
#define DA7212_MIN_MIXIN_VOL       0

typedef struct da7212_context
{
    ado_mixer_t                  *mixer;
    ado_mixer_dll_codec_params_t params;
    bool                         debug;
} da7212_context_t;

enum {
    LEFT_CHAN,
    RIGHT_CHAN,
    NUM_CHAN
};

int da7212_init(da7212_context_t* da7212);
int da7212_deinit(da7212_context_t* da7212);
int da7212_hp_output_vol_get(da7212_context_t* da7212, uint32_t *vol);
int da7212_hp_output_vol_set(da7212_context_t* da7212, int channel, uint32_t vol);
int da7212_hp_output_mute_get(da7212_context_t* da7212, uint32_t* mute);
int da7212_hp_output_mute_set(da7212_context_t* da7212, int channel, uint32_t mute);
int da7212_mixin_input_vol_get(da7212_context_t* da7212, uint32_t *vol);
int da7212_mixin_input_vol_set(da7212_context_t* da7212, int channel, uint32_t vol);
int da7212_mixin_input_mute_get(da7212_context_t* da7212, uint32_t *mute);
int da7212_mixin_input_mute_set(da7212_context_t* da7212, int channel, uint32_t mute);
int da7212_mic_1_input_mute_get(da7212_context_t* da7212, uint32_t* mute);
int da7212_mic_1_input_mute_set(da7212_context_t* da7212, uint32_t mute);
int da7212_mic_2_input_mute_get(da7212_context_t* da7212, uint32_t* mute);
int da7212_mic_2_input_mute_set(da7212_context_t* da7212, uint32_t mute);
int da7212_mic_1_input_vol_get(da7212_context_t* da7212, uint32_t* mute);
int da7212_mic_1_input_vol_set(da7212_context_t* da7212, uint32_t mute);
int da7212_mic_2_input_vol_get(da7212_context_t* da7212, uint32_t* mute);
int da7212_mic_2_input_vol_set(da7212_context_t* da7212, uint32_t mute);
int da7212_rate_setting(da7212_context_t* da7212, uint32_t sample_rate, int channel);
int da7212_dig_routing_dai_get(da7212_context_t* da7212, uint32_t* val, int mode);
int da7212_dig_routing_dai_set(da7212_context_t* da7212, uint32_t mode);
int da7212_dig_routing_dac_get(da7212_context_t* da7212, uint32_t* val, int mode);
int da7212_dig_routing_dac_set(da7212_context_t* da7212, uint32_t mode);
int da7212_capabilities(da7212_context_t* da7212, ado_mixer_dll_codec_capabilities_t *cap, int channel);
void da7212_register_dump(da7212_context_t* da7212);

#endif /* _DA7212_H */

