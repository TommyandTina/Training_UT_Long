/*
 * Copyright (c) 2020, 2023, BlackBerry Limited. All rights reserved.
 * Copyright (c) 2021, Renesas Electronics Corporation.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

#include "proto.h"

int rcar_cpg_clk_control(const cpg_support_funcs_t* const cpg_funcs, cpg_clkctl_t* const clkctl, int *status)
{
    int ret = CPG_RET_OK;

    switch (clkctl->cmd) {
        case CMD_CLK_ENABLE:
            *status = cpg_funcs->clk_config(clkctl->clk_id, clkctl->clk_idx, 1);
            break;
        case CMD_CLK_DISABLE:
            *status = cpg_funcs->clk_config(clkctl->clk_id, clkctl->clk_idx, 0);
            break;
        case CMD_CLK_RATE_GET:
            clkctl->clk_val = 0;
            *status = cpg_funcs->clk_rate(clkctl->clk_id, clkctl->clk_idx, &clkctl->clk_val, 0);
            break;
        case CMD_CLK_RATE_SET:
            *status = cpg_funcs->clk_rate(clkctl->clk_id, clkctl->clk_idx, &clkctl->clk_val, 1);
            break;
        case CMD_CLK_DIV_GET:
            clkctl->clk_val = 0;
            *status = cpg_funcs->clk_divider(clkctl->clk_id, clkctl->clk_idx, &clkctl->clk_val, 0);
            break;
        case CMD_CLK_DIV_SET:
            *status = cpg_funcs->clk_divider(clkctl->clk_id, clkctl->clk_idx, &clkctl->clk_val, 1);
            break;
        case CMD_CLK_CHECK:
            *status = cpg_funcs->clk_status(clkctl->clk_id, clkctl->clk_idx,(int*) &clkctl->clk_val);
            break;
#ifdef VARIANT_pm
        case CMD_CLK_FREQ_GET:
            ret = cpg_funcs->clk_freq(clkctl->clk_id, clkctl->clk_idx, &clkctl->clk_val, 0);
            break;
        case CMD_CLK_FREQ_SET_OPP:
            ret = cpg_funcs->clk_freq(clkctl->clk_id, clkctl->clk_idx, &clkctl->clk_val, 1);
            break;
        case CMD_CLK_FREQ_GET_OPP:
            ret = cpg_funcs->clk_opp(clkctl->clk_id, clkctl->clk_idx, clkctl->clk_val, clkctl->clk_freq_rng, &clkctl->clk_opp);
            break;
#endif // VARIANT_pm
        default:
            cpg_slogf(_SLOG_ERROR, "%s: Unknown command %d", __FUNCTION__, clkctl->cmd);
            ret = CPG_RET_ERROR;
    }

    return ret;
}
