/*
 * $QNXLicenseC:
 * Copyright 2016 QNX Software Systems.
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

extern int variant_init ( HW_CONTEXT_T * rcar );
extern int variant_uninit ( HW_CONTEXT_T *rcar );
extern int variant_set_clock_rate( HW_CONTEXT_T *rcar, uint32_t mclk1_rate, uint32_t mclk2_rate );
extern void cs2000_register_dump(void);

#define DEFAULT_SSI_MASTERSLAVE_MODE    SSI_MS_MODE_MASTER_SLAVE
#define DEFAULT_SSI_OP_MODE             SSI_OP_MODE_STEREO
#define DEFAULT_USE_SRC_PLAYBACK        1               /* enable src for playback */
#define DEFAULT_USE_SRC_CAPTURE         1               /* enable src for capture */
#define DEFAULT_USE_DVC_PLAYBACK        1               /* enable dvc for playback */
#define DEFAULT_USE_DVC_CAPTURE         0               /* disable dvc for capture */
#define DEFAULT_SLOT_SIZE               32              /* 32-bit */
#define DEFAULT_SAMPLE_SIZE             16              /* 16-bit */
#define DEFAULT_VOICES                  2
#define DEFAULT_SSI_CHANNEL_PLAYBACK    SSI_CHANNEL_NUM /* do not specify a default value, to allow for a capture only use case */
#define DEFAULT_SSI_CHANNEL_CAPTURE     SSI_CHANNEL_NUM /* do not specify a default value, to allow for a playback only use case */
#define SAMPLE_RATE_SSI                 48000           /* fixed SSI sample rate, when SRC is enabled */
#define SAMPLE_RATE_MIN                 8000
#define SAMPLE_RATE_MAX                 192000
#define MCLK_1                          22579200
#define MCLK_SOURCE_1                   AUDIO_CLKA
#define MCLK_2                          24576000
#define MCLK_SOURCE_2                   AUDIO_CLKB
#define MCLK_FS                         0
#define MCLK_SOURCE_FS                  AUDIO_CLK_NONE

#define VARIANT_SET_CLOCK_RATE(rcar, mclk1_rate, mclk2_rate, ssi_sample_rate) variant_set_clock_rate(rcar, mclk1_rate, mclk2_rate)
#define VARIANT_REGISTER_DUMP()                        cs2000_register_dump()
#define VARIANT_INIT(rcar)                             variant_init(rcar)
#define VARIANT_UNINIT(rcar)                           variant_uninit(rcar)

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/nto/aarch64/dll.le.cs2000/variant.h $ $Rev: 911285 $")
#endif
