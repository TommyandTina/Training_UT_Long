/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2017, Renesas Electronics Corporation
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

#ifndef __VARIANT_H__
#define __VARIANT_H__


struct _video_info;

int max96712_init(struct _video_info *video, int device, int board_id);
int max96712_fini(struct _video_info *video, int channel);
int max96712_update(struct _video_info *video, int channel);
int max96712_disable(struct _video_info *video, int channel);
int max96712_check_input_source(struct _video_info *video, int channel);
int max96712_enable(struct _video_info *video, int channel, int enable);

#ifdef MAX96712_COLOR_PATTERN_TEST
#define DECODER_INFO        "MAX96712_PATTERN"
#else
#define DECODER_INFO        "MAX96712"
#endif

#define DECODER_INIT        max96712_init
#define DECODER_FINI        max96712_fini
#define DECODER_UPDATE      max96712_update
#define DECODER_ENABLE      max96712_enable
#define DECODER_DISABLE     max96712_disable
#define DECODER_CHECK       max96712_check_input_source

#endif // __VARIANT_H__
