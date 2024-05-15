/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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

#include <stdio.h>
#include <stdarg.h>
#include <sys/slog2.h>
#include <sys/slogcodes.h>
#include "slogf.h"

static slog2_buffer_t buffer_handle;

int rcar_dma_slog2_init(void)
{
    static int init = 0;
    if (!init) {
        slog2_buffer_set_config_t buffer_config;
        const char * const buffer_set_name = "rcar_dma_lib";
        const char * const buffer_name = "rcar_dma_lib_buffer";

        buffer_config.buffer_set_name = (char *)buffer_set_name;
        buffer_config.num_buffers = 1;
        buffer_config.verbosity_level = SLOG2_INFO;
        buffer_config.buffer_config[0].buffer_name = (char *)buffer_name;
        buffer_config.buffer_config[0].num_pages = 8;

        if ( slog2_register (&buffer_config, &buffer_handle, 0) == -1 ) {
            fprintf (stderr, "In %s, unable to register slog2 buffer!\n", __FUNCTION__);
            return -1;
        }
        init = 1;
    }

    return 0;
}

int
rcar_dma_slogf(const unsigned char severity, const char * const fmt, ...)
{
    int status;
    va_list arg;

    va_start (arg, fmt);
    status = vslog2f (buffer_handle, 0, severity, fmt, arg);
    va_end (arg);
    return status;
}

