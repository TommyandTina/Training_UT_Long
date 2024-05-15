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
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/slog.h>
#include <hw/rcar_cpg_support.h>

#include "proto.h"

cpg_dev_t cpg_dev = { 0 };

uint32_t cpg_read(const uint32_t reg)
{
    return in32(cpg_dev.cpg.vbase + reg);
}

void cpg_write(const uint32_t reg, const uint32_t data)
{
    out32(cpg_dev.cpg.vbase + cpg_dev.wpr_off, ~data);
    out32(cpg_dev.cpg.vbase + reg, data);
}

int cpg_waitfor(const uint32_t reg, const uint32_t mask, const uint32_t val)
{
    int i = 1000;
    while(--i) {
        if ( (cpg_read(reg) & mask) == val) {
            return CPG_RET_OK;
        }
        nanospin_ns(100);
    }
    return CPG_RET_TIMEOUT;
}

int cpg_init_common(const cpg_opts_t* const cpg_opts, const avs_opts_t* const avs_opts,
                const uint32_t product_id)
{
    cpg_dev.verbose = _SLOG_ERROR;

    if (!cpg_opts) {
        cpg_slogf(_SLOG_INFO, "%s: Invalid cpg_opts (NULL)", __func__);
        return CPG_RET_INVALPARA;
    }

    if ( (cpg_opts->base == 0) || (cpg_opts->size == 0) || (cpg_opts->ext_clk == 0) ) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid options: CPG Base 0x%x:0x%x ext_clk %d",
                 __func__, cpg_opts->base, cpg_opts->base, cpg_opts->ext_clk);
        return CPG_RET_INVALPARA;
    }

    if (cpg_opts->verbose > _SLOG_ERROR) {
        cpg_dev.verbose = cpg_opts->verbose;
    }

    cpg_dev.cpg.pbase = cpg_opts->base;
    cpg_dev.cpg.size  = cpg_opts->size;
    cpg_dev.ext_clk   = cpg_opts->ext_clk;
    cpg_dev.soc_id    = RCAR_GEN3_PRODUCT_ID(product_id);
    cpg_dev.soc_rev   = RCAR_GEN3_PRODUCT_REV(product_id);

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3U) {
        cpg_dev.wpr_off    = RCAR_V3U_CPG_CPGWPR;
        cpg_dev.ecr_off    = RCAR_V3U_CPG_PLLECR;
        cpg_dev.frqcrb_off = RCAR_V3U_CPG_FRQCRB;
    } else if (IS_RCAR_GEN3(cpg_dev.soc_id)) {
        cpg_dev.wpr_off    = RCAR_GEN3_CPG_CPGWPR;
        cpg_dev.ecr_off    = RCAR_GEN3_CPG_PLLECR;
        cpg_dev.frqcrb_off = RCAR_GEN3_CPG_FRQCRB;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Unknown SoC, id 0x%x", __func__, cpg_dev.soc_id);
        return CPG_RET_INVALPARA;
    }

    cpg_slogf(_SLOG_INFO, "%s: CPG: Base 0x%x:0x%x ext_clk %d soc_id 0x%x soc_rev 0x%x",
            __func__, cpg_dev.cpg.pbase, cpg_dev.cpg.size, cpg_dev.ext_clk, cpg_dev.soc_id, cpg_dev.soc_rev);

    /* AVS */
    cpg_dev.pmic.model = PMIC_MODEL_UNKNOWN;
    cpg_dev.pmic.i2c_bus = -1;
    cpg_dev.pmic.i2c_slave_addr = -1;
    cpg_dev.pmic.i2c_fd = -1;

    if (avs_opts) {
        /* AVS/PMIC params check is done in avs.c */
        cpg_dev.avs.pbase = avs_opts->base;
        cpg_dev.avs.size  = avs_opts->size;

        cpg_dev.pmic.model          = avs_opts->pmic.model;
        cpg_dev.pmic.i2c_bus        = avs_opts->pmic.i2c_bus;
        cpg_dev.pmic.i2c_slave_addr = avs_opts->pmic.i2c_slave_addr;

        cpg_slogf(_SLOG_INFO, "%s: AVS: Base 0x%x:0x%x", __func__, cpg_dev.avs.pbase, cpg_dev.avs.size);
        cpg_slogf(_SLOG_INFO, "%s: PMIC: model=%d i2c=0x%x@i2c%d", __func__,
                cpg_dev.pmic.model, cpg_dev.pmic.i2c_slave_addr, cpg_dev.pmic.i2c_bus);
    }

    return CPG_RET_OK;
}

cpg_support_funcs_t cpg_support_funcs = {
    .init = cpg_init,
    .deinit = cpg_deinit,

    .mstp_config = mstp_config,
    .mstp_status = mstp_status,
    .mstp_reset  = mstp_reset,
    .mstp_reset_config = mstp_reset_config,
    .mstp_reset_state  = mstp_reset_state,

    .clk_config  = clk_config,
    .clk_rate    = clk_rate,
    .clk_divider = clk_divider,
    .clk_status  = clk_status,
};
