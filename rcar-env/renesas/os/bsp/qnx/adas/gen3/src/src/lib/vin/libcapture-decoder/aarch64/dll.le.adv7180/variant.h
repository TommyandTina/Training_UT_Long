/*
* $QNXLicenseC:
* Copyright 2017, QNX Software Systems.
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

#ifndef __VARIANT_H__
#define __VARIANT_H__

struct _video_info;

int adv7180_init(struct _video_info* video, int channel);
int adv7180_fini(struct _video_info* video, int channel);
int adv7180_update(struct _video_info* video, int channel);
int adv7180_power(struct _video_info *video, int channel, int enable);


#define DECODER_INFO        "ADV7180"
#define DECODER_INIT        adv7180_init
#define DECODER_FINI        adv7180_fini
#define DECODER_UPDATE      adv7180_update
#define DECODER_POWER       adv7180_power

#endif // __VARIANT_H__
