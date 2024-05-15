/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "proto.h"

int __attribute__ ((weak)) nanospin_ns(const unsigned long __nsec)
{
    /* Cannot fully achieve nanospin since each TMU0 cycle is a minimum of 240 nsec */
    usec_delay((unsigned)(__nsec/1000) + 1);
    return 0;
}

void rcar_sysc_deinit(void)
{
}

int rcar_sysc_init(const sysc_opts_t* const sysc_opts)
{
    int status;
    const uint32_t product_id = in32(SYSC_RCAR_PRODUCT_REG);

    status = sysc_init_common(sysc_opts, product_id);
    if (status != SYSC_RET_OK) {
        sysc_slogf(_SLOG_ERROR, "%s: sysc_init_common failed, status %d", __func__, status);
        return status;
    }

    sysc_dev.vbase = sysc_opts->base;
    sysc_dev.apmu_vbase = sysc_dev.apmu_pbase;

    sysc_dev.inited = 1;

    return SYSC_RET_OK;
}

