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
#include <aarch64/r-car.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>

#include "proto.h"

void cpg_deinit()
{
    if (cpg_dev.cpg.vbase) {
        munmap_device_io(cpg_dev.cpg.vbase, cpg_dev.cpg.size);
    }

    if (cpg_dev.avs.vbase) {
        munmap_device_io(cpg_dev.avs.vbase, cpg_dev.avs.size);
    }

    if (cpg_dev.pmic.i2c_fd > 0) {
        close(cpg_dev.pmic.i2c_fd);
    }
}

int cpg_init(const cpg_opts_t* const cpg_opts, const avs_opts_t* const avs_opts)
{
    int status;
    uint32_t product_id;
    hwi_tag *tag;
    unsigned hwi_off;

    /* Find product id */
    hwi_off = hwi_find_device("r-car", 0);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, 0);
        if(tag) {
            product_id = tag->hwversion.version << 8 | tag->hwversion.hclass;
        } else {
            cpg_slogf(_SLOG_ERROR, "Unable to query product_id from HW info");
            return CPG_RET_ERROR;
        }
    } else {
        cpg_slogf(_SLOG_ERROR, "Unable to find product version from HW info");
        return CPG_RET_ERROR;
    }

    status = cpg_init_common(cpg_opts, avs_opts, product_id);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: cpg_init_common failed, status %d", __func__, status);
        return status;
    }

    /* CPG registers mapping */
    cpg_dev.cpg.vbase = (uintptr_t)mmap_device_io(cpg_dev.cpg.size, cpg_dev.cpg.pbase);
    if (cpg_dev.cpg.vbase == (uintptr_t)MAP_FAILED) {
        cpg_slogf(_SLOG_ERROR, "%s: Unable to map CPG registers 0x%x", __func__, cpg_dev.cpg.pbase);
        return CPG_RET_ERROR;
    }

    /* Optional: AVS registers mapping */
    if ( (cpg_dev.avs.pbase != 0) && (cpg_dev.avs.size != 0) ) {
        cpg_dev.avs.vbase = (uintptr_t)mmap_device_io(cpg_dev.avs.size, cpg_dev.avs.pbase);
        if (cpg_dev.avs.vbase == (uintptr_t)MAP_FAILED) {
            cpg_slogf(_SLOG_ERROR, "%s: Unable to map AVS registers 0x%x", __func__, cpg_dev.avs.pbase);
            return CPG_RET_ERROR;
        }
    }

    cpg_dev.inited = 1;

    return CPG_RET_OK;
}

