/*
 * Copyright (c) 2020, 2023, BlackBerry Limited. All rights reserved.
 * Copyright (c) 2021 Renesas Electronics Corporation.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
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
    return in32(cpg_dev.vbase + reg);
}

void cpg_write(const uint32_t reg, const uint32_t data)
{
    out32(cpg_dev.vbase + cpg_dev.wpr_off, ~data);
    out32(cpg_dev.vbase + reg, data);
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

const uint32_t cpg_query_soc_id(void)
{
    return cpg_dev.soc_id;
}

int cpg_init_common(const cpg_opts_t* const cpg_opts, const uint32_t product_id)
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

    cpg_dev.pbase     = cpg_opts->base;
    cpg_dev.size      = cpg_opts->size;
    cpg_dev.ext_clk   = cpg_opts->ext_clk;
    cpg_dev.soc_id    = RCAR_PRODUCT_ID(product_id);
    cpg_dev.soc_rev   = RCAR_PRODUCT_REV(product_id);

    if (IS_RCAR_GEN4(cpg_dev.soc_id)) {
        cpg_dev.wpr_off    = RCAR_GEN4_CPG_CPGWPR;
        cpg_dev.ecr_off    = RCAR_GEN4_CPG_PLLECR;
        cpg_dev.frqcrb_off = RCAR_GEN4_CPG_FRQCRB;
    } else if (IS_RCAR_GEN3(cpg_dev.soc_id)) {
        cpg_dev.wpr_off    = RCAR_GEN3_CPG_CPGWPR;
        cpg_dev.ecr_off    = RCAR_GEN3_CPG_PLLECR;
        cpg_dev.frqcrb_off = RCAR_GEN3_CPG_FRQCRB;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Unknown SoC, id 0x%x", __func__, cpg_dev.soc_id);
        return CPG_RET_INVALPARA;
    }

    //cpg_slogf(_SLOG_INFO, "%s: CPG: Base 0x%x:0x%x ext_clk %d soc_id 0x%x soc_rev 0x%x",
    //        __func__, cpg_dev.pbase, cpg_dev.size, cpg_dev.ext_clk, cpg_dev.soc_id, cpg_dev.soc_rev);

    return CPG_RET_OK;
}

cpg_support_funcs_t cpg_support_funcs = {
    .init = cpg_init,
    .deinit = cpg_deinit,
    .query_soc_id = cpg_query_soc_id,

    .mstp_config = mstp_config,
    .mstp_status = mstp_status,
    .mstp_reset  = mstp_reset,
    .mstp_reset_config = mstp_reset_config,
    .mstp_reset_state  = mstp_reset_state,

    .mstp_reset_gen4_a = mstp_reset_gen4_a,
    .mstp_reset_gen4_b = mstp_reset_gen4_b,
    .mstp_reset_gen4_c = mstp_reset_gen4_c,
    .mstp_assert_gen4_a = mstp_assert_gen4_a,
    .mstp_assert_gen4_b = mstp_assert_gen4_b,
    .mstp_assert_gen4_c = mstp_assert_gen4_c,
    .mstp_deassert_gen4 = mstp_deassert_gen4,

    .clk_config  = clk_config,
    .clk_rate    = clk_rate,
    .clk_divider = clk_divider,
    .clk_status  = clk_status,
#ifdef VARIANT_pm
    .clk_freq    = clk_freq,
    .clk_opp     = clk_opp,
#endif
};
