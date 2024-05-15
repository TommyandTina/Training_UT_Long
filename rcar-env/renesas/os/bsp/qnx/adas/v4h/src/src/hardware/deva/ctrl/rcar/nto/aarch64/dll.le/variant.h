/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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

#define DEFAULT_SSI_MASTERSLAVE_MODE    SSI_MS_MODE_MASTER
#define DEFAULT_SSI_OP_MODE             SSI_OP_MODE_STEREO
#define DEFAULT_SLOT_SIZE               32              /* 32-bit */
#define DEFAULT_SAMPLE_SIZE             16              /* 16-bit */
#define DEFAULT_VOICES                  2
#define DEFAULT_SSI_CHANNEL_PLAYBACK    SSI_CHANNEL_NUM /* Do not specify a default value, to allow for a capture only use case */
#define DEFAULT_SSI_CHANNEL_CAPTURE     SSI_CHANNEL_NUM /* Do not specify a default value, to allow for a playback only use case */
#define SAMPLE_RATE_SSI                 48000           /* Acording to Linux fs=48.0kHz */
#define SAMPLE_RATE_MIN                 8000
#define SAMPLE_RATE_MAX                 192000
#define MCLK                            12288000        /* Acording to Linux audio_clkin 12288000Hz*/
#define MCLK_FS                         0
