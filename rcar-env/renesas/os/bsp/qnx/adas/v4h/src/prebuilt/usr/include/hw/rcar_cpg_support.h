/*
 * Copyright (c) 2020, 2023, BlackBerry Limited. All rights reserved.
 * Copyright (c) 2022, Renesas Electronics Corporation.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

#ifndef _RCAR_CPG_SUPPORT_H_INCLUDED
#define _RCAR_CPG_SUPPORT_H_INCLUDED

#include <hw/rcar_cpg.h>

typedef struct cpg_opts_ {
    uint32_t     base;
    uint32_t     size;
    uint32_t     ext_clk;
    int          verbose;
} cpg_opts_t;

/*
 * Hardware Access Function Table
 */
typedef struct {
    int   (*init)(const cpg_opts_t* const cpg_opts);
    void  (*deinit)(void);
    const uint32_t (*query_soc_id)(void);

    int (*mstp_config)(const int mod_id, const int enable);
    int (*mstp_status)(const int mod_id, int *status);
    int (*mstp_reset)(const int mod_id);
    int (*mstp_reset_config)(const int mod_id, const int assert);
    int (*mstp_reset_state)(const int mod_id, int *status);
    int (*mstp_reset_gen4_a)(const int mod_id);
    int (*mstp_reset_gen4_b)(const int mod_id);
    int (*mstp_reset_gen4_c)(const int mod_id);
    int (*mstp_assert_gen4_a)(const int mod_id);
    int (*mstp_assert_gen4_b)(const int mod_id);
    int (*mstp_assert_gen4_c)(const int mod_id);
    int (*mstp_deassert_gen4)(const int mod_id);

    int (*clk_config)(const cpg_clk_id_t clk_id, const int idx, const int enable);
    int (*clk_rate)(const cpg_clk_id_t clk_id, const int idx, unsigned *rate, const int set);
    int (*clk_divider)(const cpg_clk_id_t clk_id, const int idx, unsigned *clk_div, const int set);
    int (*clk_status)(const cpg_clk_id_t clk_id, const int idx, int *status);
#ifdef VARIANT_pm
    int (*clk_freq)(const cpg_clk_id_t clk_id, const int idx, unsigned *freq_mhz, const int set);
    int (*clk_opp)(const cpg_clk_id_t clk_id, const int idx, uint32_t freq, uint32_t freq_rng, uint32_t* const opp);
#endif

} cpg_support_funcs_t;

extern cpg_support_funcs_t cpg_support_funcs;

#endif /* _RCAR_CPG_SUPPORT_H_INCLUDED */
