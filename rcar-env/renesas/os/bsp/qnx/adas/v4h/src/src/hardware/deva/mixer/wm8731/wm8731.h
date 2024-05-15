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

#ifndef _WM8731_H
#define _WM8731_H

#include <stdint.h>
#include <mixer/i2s_codec_dll.h>

#define WM8731_MAX_DIGITAL_VOL          127
#define WM8731_MIN_DIGITAL_VOL          48

typedef struct wm8731_context
{
    ado_mixer_t                  *mixer;
    ado_mixer_dll_codec_params_t params;
    bool                         debug;
} wm8731_context_t;

enum {
    LEFT_CHAN,
    RIGHT_CHAN,
    NUM_CHAN
};

int wm8731_init(wm8731_context_t* wmb731);
int wm8731_deinit(wm8731_context_t* wmb731);
int wm8731_output_vol_get(wm8731_context_t* wm8731, uint32_t *vol);
int wm8731_output_vol_set(wm8731_context_t* wm8731, int channel, uint32_t vol);
int wm8731_output_mute_get(wm8731_context_t* wm8731, uint32_t* mute);
int wm8731_output_mute_set(wm8731_context_t* wm8731, uint32_t mute);
int wm8731_line_input_vol_get(wm8731_context_t* wm8731, uint32_t *vol);
int wm8731_line_input_vol_set(wm8731_context_t* wm8731, int channel, uint32_t vol);
int wm8731_line_input_mute_get(wm8731_context_t* wm8731, uint32_t *mute);
int wm8731_line_input_mute_set(wm8731_context_t* wm8731, int channel, uint32_t mute);
int wm8731_mic_input_mute_get(wm8731_context_t* wm8731, uint32_t* mute);
int wm8731_mic_input_mute_set(wm8731_context_t* wm8731, uint32_t mute);
int wm8731_mic_boost_get(wm8731_context_t* wm8731, uint32_t* val);
int wm8731_mic_boost_set(wm8731_context_t* wm8731, uint32_t set);
int wm8731_bypass_get(wm8731_context_t* wm8731, uint32_t* val);
int wm8731_bypass_set(wm8731_context_t* wm8731, uint32_t set);
int wm8731_sidetone_get(wm8731_context_t* wm8731, uint32_t* val);
int wm8731_sidetone_set(wm8731_context_t* wm8731, uint32_t set);
int wm8731_dacsel_get(wm8731_context_t* wm8731, uint32_t* val);
int wm8731_dacsel_set(wm8731_context_t* wm8731, uint32_t set);
int wm8731_adc8k_get(wm8731_context_t* wm8731, uint32_t* val);
int wm8731_adc8k_set(wm8731_context_t* wm8731, uint32_t adc8k);

int wm8731_rate_setting(wm8731_context_t* wm8731, uint32_t sample_rate);
int wm8731_capabilities(wm8731_context_t* wm8731, ado_mixer_dll_codec_capabilities_t *cap, int channel);
void wm8731_register_dump(wm8731_context_t* wm8731);

#endif /* _WM8731_H */

