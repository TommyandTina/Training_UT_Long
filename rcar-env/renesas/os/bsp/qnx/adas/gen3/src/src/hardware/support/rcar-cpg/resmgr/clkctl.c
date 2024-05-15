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
            *status = cpg_funcs->clk_status(clkctl->clk_id, clkctl->clk_idx, (int*)&clkctl->clk_val);
            break;
        default:
            cpg_slogf(_SLOG_ERROR, "[CPG clock]%s: Unknown command %d", __FUNCTION__, clkctl->cmd);
            ret = CPG_RET_ERROR;
    }

    cpg_slogf(_SLOG_INFO, "[CPG clock]%s : cmd 0x%x clk_id %d ret %d", __func__, clkctl->cmd, clkctl->clk_id, ret);

    return ret;
}
