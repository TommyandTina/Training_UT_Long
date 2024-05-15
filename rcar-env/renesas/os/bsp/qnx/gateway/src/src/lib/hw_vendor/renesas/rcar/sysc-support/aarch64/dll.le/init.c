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
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>

#include "proto.h"

void rcar_sysc_deinit()
{
    if (sysc_dev.vbase) {
        munmap_device_io(sysc_dev.vbase, sysc_dev.size);
    }
}

int rcar_sysc_init(const sysc_opts_t* const sysc_opts)
{
    int status;
    uint32_t product_id;
    uintptr_t prr_base;

    /* Find product id */
    prr_base = (uintptr_t)mmap_device_io(4, SYSC_RCAR_PRODUCT_REG);
    if (prr_base == (uintptr_t)MAP_FAILED) {
        sysc_slogf(_SLOG_ERROR, "%s: Unable to map PRR register 0x%x", __func__,
            SYSC_RCAR_PRODUCT_REG);
        return SYSC_RET_ERROR;
    }

    product_id = in32(prr_base);
    munmap_device_io(prr_base, 4);

    status = sysc_init_common(sysc_opts, product_id);
    if (status != SYSC_RET_OK) {
        sysc_slogf(_SLOG_ERROR, "%s: sysc_init_common failed, status %d", __func__, status);
        return status;
    }

    /* SYSC registers mapping */
    sysc_dev.vbase = (uintptr_t)mmap_device_io(sysc_dev.size, sysc_dev.pbase);
    if (sysc_dev.vbase == (uintptr_t)MAP_FAILED) {
        sysc_slogf(_SLOG_ERROR, "%s: Unable to map SYSC registers 0x%x", __func__, sysc_dev.pbase);
        return SYSC_RET_ERROR;
    }

    sysc_dev.inited = 1;

    return SYSC_RET_OK;
}

