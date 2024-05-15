/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#ifndef __BOARD_H__
#define __BOARD_H__

#include "common.h"
#include <dlfcn.h>
#include <pthread.h>
#include "variant.h"

typedef struct  _capture_context {
	int enable;
	int is_running;
	int source_idx;
	int active_dev;
	int n_devices;
    pthread_mutex_t *mutex;
    void *soc_api;
    void *dec_api;
	void *dec_context;
	void *soc_context;
} rcar_context_t;

typedef struct  {
	void* hdl;
	capture_context_t (*capture_create_context)(uint32_t flags);
	void (*capture_destroy_context)( capture_context_t context );
	int (*capture_is_property)(capture_context_t context, uint32_t prop);
	int (*capture_update)(capture_context_t context, uint32_t flags);
	int (*capture_set_property_i)(capture_context_t context, uint32_t prop, int32_t value);
	int (*capture_set_property_p)(capture_context_t context, uint32_t prop, void *value);
	int (*capture_get_property_i)(capture_context_t context, uint32_t prop, int32_t *value);
	int (*capture_get_property_p)(capture_context_t context, uint32_t prop, void **value);
	int (*capture_get_frame)(capture_context_t context, uint64_t timeout, uint32_t flags);
	int (*capture_release_frame)(capture_context_t context, uint32_t idx);
	int (*capture_create_buffers)(capture_context_t context, uint32_t property);
	int (*capture_put_buffer)(capture_context_t ctx, uint32_t idx, uint32_t flags);
	int (*capture_init)(capture_context_t ctx, int device, int board_id, void *info_addr);
} context_api_t;

#endif // __BOARD_H__
