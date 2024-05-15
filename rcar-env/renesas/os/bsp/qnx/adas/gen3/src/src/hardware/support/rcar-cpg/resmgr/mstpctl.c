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

int rcar_cpg_mstp_control(const cpg_support_funcs_t* const cpg_funcs, cpg_mstpctl_t* const mstpctl, int *status)
{
    int ret = CPG_RET_OK;
    int stat_val;

    switch (mstpctl->cmd) {
        case CMD_MSTP_ENABLE:
            *status = cpg_funcs->mstp_config(mstpctl->mod_id, 1);
            break;
        case CMD_MSTP_DISABLE:
            *status = cpg_funcs->mstp_config(mstpctl->mod_id, 0);
            break;
        case CMD_MSTP_RESET:
            *status = cpg_funcs->mstp_reset(mstpctl->mod_id);
            break;
        case CMD_MSTP_ASSERT:
            *status = cpg_funcs->mstp_reset_config(mstpctl->mod_id, 1);
            break;
        case CMD_MSTP_DEASSERT:
            *status = cpg_funcs->mstp_reset_config(mstpctl->mod_id, 0);
            break;
        case CMD_MSTP_CHECK:
            *status = cpg_funcs->mstp_status(mstpctl->mod_id, &stat_val);
            mstpctl->is_enabled = stat_val;
            break;
        case CMD_MSTP_RESET_CHECK:
            *status = cpg_funcs->mstp_reset_state(mstpctl->mod_id, &stat_val);
            mstpctl->is_enabled = stat_val;
            break;
        default:
            cpg_slogf(_SLOG_ERROR, "[CPG MSTP]%s : Unknown command %d", __FUNCTION__, mstpctl->cmd);
            ret = CPG_RET_ERROR;
    }

    cpg_slogf(_SLOG_INFO, "[CPG MSTP]%s : cmd 0x%x mod_id 0x%x ret %d", __func__, mstpctl->cmd, mstpctl->mod_id, ret);

    return ret;
}

