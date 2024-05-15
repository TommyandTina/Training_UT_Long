/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

void cpg_deinit(void)
{
}

int cpg_init(const cpg_opts_t* const cpg_opts)
{
    int status;
    const uint32_t product_id = in32(RCAR_PRODUCT_REGISTER);

    status = cpg_init_common(cpg_opts, product_id);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: cpg_init_common failed, status %d", __func__, status);
        return status;
    }

    cpg_dev.vbase = cpg_dev.pbase;
    cpg_dev.inited = 1;

    return CPG_RET_OK;
}
