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

#include <sys/neutrino.h>

#define ADO_DEVICE_MMAP(card, phys_addr, size) ado_device_mmap(card, phys_addr, size)
#define ADO_DEVICE_MUNMAP(card, phys_addr, size) ado_device_munmap(card, phys_addr, size)
#define ADO_PCM_BUF_MAP(card, config, phys_addr, size, flags) ado_pcm_buf_map(card, config, phys_addr, size, flags)
#define ADO_PCM_BUF_ALLOC(card, config, size, flags) ado_pcm_buf_alloc(card, config, size, flags)
#define ADO_PCM_BUF_FREE(card, config) ado_pcm_buf_free(card, config)
