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

#include "proto.h"

sysc_dev_t sysc_dev = { 0 };

int sysc_init_common(const sysc_opts_t* const sysc_opts, const uint32_t product_id)
{
    sysc_dev.verbose = _SLOG_ERROR;

    if ( sysc_opts == NULL ) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid options: sysc_opts == NULL", __func__);
        return SYSC_RET_ERROR;
    }

    if ( (sysc_opts->base == 0) || (sysc_opts->size == 0) ) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid options: SYSC Base 0x%x:0x%x",
                 __func__, sysc_opts->base, sysc_opts->size);
        return SYSC_RET_ERROR;
    }

    if (sysc_opts->verbose > _SLOG_ERROR) {
        sysc_dev.verbose = sysc_opts->verbose;
    }

    sysc_dev.pbase   = sysc_opts->base;
    sysc_dev.size    = sysc_opts->size;
    sysc_dev.verbose = sysc_opts->verbose;
    sysc_dev.soc_id  = RCAR_GEN3_PRODUCT_ID(product_id);
    sysc_dev.soc_rev = RCAR_GEN3_PRODUCT_REV(product_id);

    if ( (sysc_dev.soc_id != PRODUCT_ID_RCAR_V3U) && !IS_RCAR_GEN3(sysc_dev.soc_id) ) {
        sysc_slogf(_SLOG_ERROR, "%s: Error: Unknown SoC, id 0x%x", __func__, sysc_dev.soc_id);
        return SYSC_RET_ERROR;
    }

    sysc_slogf(_SLOG_INFO, "%s: SYSC: Base 0x%x:0x%x soc_id 0x%x soc_rev 0x%x", __func__,
        sysc_dev.pbase, sysc_dev.size, sysc_dev.soc_id, sysc_dev.soc_rev);

    return SYSC_RET_OK;
}

sysc_support_funcs_t sysc_support_funcs = {
    .init         = rcar_sysc_init,
    .deinit       = rcar_sysc_deinit,
    .power_set    = rcar_sysc_power_set,
    .power_is_off = rcar_sysc_power_is_off,
    .read_pdmode  = rcar_sysc_pdmode_read,
};
