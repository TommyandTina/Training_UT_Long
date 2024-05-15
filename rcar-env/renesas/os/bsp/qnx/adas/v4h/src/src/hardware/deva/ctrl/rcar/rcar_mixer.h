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

#ifndef _R_Car_MIXER_H
#define _R_Car_MIXER_H

#define MIXER_CONTEXT_T struct rcar_context

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include "rcar.h"

int rcar_mixer_init( ado_card_t* card, HW_CONTEXT_T* rcar, rcar_audio_device_t* dev );
void rcar_mixer_set_default_group( HW_CONTEXT_T* rcar, ado_pcm_t *pcm, int channel, int index );
int rcar_mixer_set_clock_rate( HW_CONTEXT_T * rcar, uint32_t sample_rate );
int rcar_mixer_supported_rates( HW_CONTEXT_T* rcar, uint32_t* rates );

#endif /* _R_Car_MIXER_H */