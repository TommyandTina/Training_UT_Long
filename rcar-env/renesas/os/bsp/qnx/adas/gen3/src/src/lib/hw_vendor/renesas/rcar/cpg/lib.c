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
#include <hw/rcar_cpg.h>

static int cpg_fd = -1;

/* -v for verbose operation */
static int cpg_verbose_level = _SLOG_CRITICAL;

static void cpg_debug(const int level, const char* const fmt, ...)
{
    va_list arg;

    if( level <= cpg_verbose_level)
    {
        va_start( arg, fmt );
        vfprintf( stderr, fmt, arg );
        va_end( arg );
    }
}

static int rcar_cpg_init(const int verbose)
{
    if (verbose > cpg_verbose_level) {
        cpg_verbose_level = verbose;
    }

    /* Open CPG resource manager */
    cpg_fd = open(CPG_DEV_NAME, O_RDWR);
    if (cpg_fd == -1) {
        cpg_debug(_SLOG_ERROR, "%s: Unable to open(%s)\n", __func__, CPG_DEV_NAME);
        return CPG_RET_ERROR;
    }

    return CPG_RET_OK;
}

static int rcar_cpg_deinit(void)
{
    /* Close cpg resource manager */
    close(cpg_fd);
    return CPG_RET_OK;
}

static int rcar_cpg_mstpctl(const cpg_mstp_cmd_t cmd, const int mod_id, int *is_enabled)
{
    cpg_mstpctl_t cpg = { 0 };
    int status = CPG_RET_OK;

    if (cpg_fd == -1) {
        cpg_debug(_SLOG_ERROR, "%s: No %s opened\n", __func__, CPG_DEV_NAME);
        return CPG_RET_ERROR;
    }

    cpg.mod_id = mod_id;
    cpg.cmd = cmd;

    if (is_enabled) {
        cpg.is_enabled = *is_enabled;
    }

    if(devctl(cpg_fd, RCAR_CPG_DEVCTL_MODCTL, &cpg, sizeof(cpg), &status)) {
        cpg_debug(_SLOG_ERROR, "%s: devctl failed for module id %d  (cmd %d)\n",
            __func__, mod_id, cmd);
        return CPG_RET_ERROR;
    }

    if (is_enabled) {
        *is_enabled = cpg.is_enabled;
    }

    return status;
}


/*
 * MSTP clock
 */
static int rcar_cpg_mstp_is_enabled(const int mod_id)
{
    int ret;
    int is_enabled;

    ret = rcar_cpg_mstpctl(CMD_MSTP_CHECK, mod_id, &is_enabled);

    if (ret != CPG_RET_OK) {
        return ret;
    }

    return is_enabled ? 1 : 0;
}

static int rcar_cpg_mstp_enable(const int mod_id)
{
    return rcar_cpg_mstpctl(CMD_MSTP_ENABLE, mod_id, NULL);
}

static int rcar_cpg_mstp_disable(const int mod_id)
{
    return rcar_cpg_mstpctl(CMD_MSTP_DISABLE, mod_id, NULL);
}

static int rcar_cpg_mstp_reset(const int mod_id)
{
    return rcar_cpg_mstpctl(CMD_MSTP_RESET, mod_id, NULL);
}

static int rcar_cpg_mstp_reset_assert(const int mod_id)
{
    return rcar_cpg_mstpctl(CMD_MSTP_ASSERT, mod_id, NULL);
}

static int rcar_cpg_mstp_reset_deassert(const int mod_id)
{
    return rcar_cpg_mstpctl(CMD_MSTP_DEASSERT, mod_id, NULL);
}

static int rcar_cpg_mstp_reset_check(const int mod_id)
{
    int ret;
    int reset_status;

    ret = rcar_cpg_mstpctl(CMD_MSTP_RESET_CHECK, mod_id, &reset_status);

    if (ret != CPG_RET_OK) {
        return ret;
    }

    return (reset_status ? 1 : 0);
}
static int rcar_cpg_clkctl(const cpg_clk_cmd_t cmd, const cpg_clk_id_t clk_id,
                const int idx, uint32_t *val)
{
    cpg_clkctl_t cpg = { 0 };
    int status = CPG_RET_OK;

    if (cpg_fd == -1) {
        cpg_debug(_SLOG_ERROR, "%s: No %s opened\n", __func__, CPG_DEV_NAME);
        return CPG_RET_ERROR;
    }

    cpg.clk_id = clk_id;
    cpg.clk_idx = idx;
    cpg.cmd = cmd;

    if (val) {
        cpg.clk_val = *val;
    }

    if(devctl(cpg_fd, RCAR_CPG_DEVCTL_CLKCTL, &cpg, sizeof(cpg), &status)) {
        cpg_debug(_SLOG_ERROR, "%s: devctl failed for clock id %d failed (cmd %d)\n",
                __func__, clk_id, cmd);
        return CPG_RET_ERROR;
    }

    if (val) {
        *val = cpg.clk_val;
    }

    return status;
}

static int rcar_cpg_clock_enable(const cpg_clk_id_t clk_id, const int idx)
{
    return rcar_cpg_clkctl(CMD_CLK_ENABLE, clk_id, idx, NULL);
}


static int rcar_cpg_clock_disable(const cpg_clk_id_t clk_id, const int idx)
{
    return rcar_cpg_clkctl(CMD_CLK_DISABLE, clk_id, idx, NULL);
}

static int rcar_cpg_clock_is_enabled(const cpg_clk_id_t clk_id, const int idx)
{
    unsigned status;
    unsigned ret;

    if ((ret = rcar_cpg_clkctl(CMD_CLK_CHECK, clk_id, idx, &status)) == CPG_RET_OK) {
        if (status == 1) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return ret;
    }
}

static int rcar_cpg_clock_rate_get(const cpg_clk_id_t clk_id, const int idx, uint32_t* const rate)
{
    int ret;

    if (rate == NULL) {
        cpg_debug(_SLOG_ERROR, "%s: rate == NULL\n", __func__);
        return CPG_RET_INVALPARA;
    }

    ret = rcar_cpg_clkctl(CMD_CLK_RATE_GET, clk_id, idx, rate);

    return ret;
}

static int rcar_cpg_clock_rate_set(const cpg_clk_id_t clk_id, const int idx, uint32_t* const rate)
{
    if (rate == NULL) {
        cpg_debug(_SLOG_ERROR, "%s: rate == NULL\n", __func__);
        return CPG_RET_INVALPARA;
    }

    return rcar_cpg_clkctl(CMD_CLK_RATE_SET, clk_id, idx, rate);
}

static int rcar_cpg_clock_divider_get(const cpg_clk_id_t clk_id, const int idx, uint32_t* const div)
{
    int ret;

    if (div == NULL) {
        cpg_debug(_SLOG_ERROR, "%s: div == NULL\n", __func__);
        return CPG_RET_INVALPARA;
    }

    ret = rcar_cpg_clkctl(CMD_CLK_DIV_GET, clk_id, idx, div);

    return ret;
}

static int rcar_cpg_clock_divider_set(const cpg_clk_id_t clk_id, const int idx, uint32_t* const div)
{
    if (div == NULL) {
        cpg_debug(_SLOG_ERROR, "%s: div == NULL\n", __func__);
        return CPG_RET_INVALPARA;
    }

    return rcar_cpg_clkctl(CMD_CLK_DIV_SET, clk_id, idx, div);
}

/*
 * Returns the hardware access function table.
 * The "funcs" table passed from the caller is populated with the pointers to the low level functions.
 * The caller can access the low level functions after completing this call.
 *
 */

#define CPG_MGR_ADD_FUNC(tabletype, table, entry, func, limit) \
    do { \
        if (offsetof(tabletype, entry) + sizeof((table)->entry) <= (size_t)(limit)) \
            (table)->entry = (func); \
    } while (0)

int cpg_mgr_getfuncs(cpg_mgr_funcs_t *funcs, const int tabsize)
{
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, init, rcar_cpg_init, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, deinit, rcar_cpg_deinit, tabsize);

    /* MSTP */
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, mstp_is_enabled, rcar_cpg_mstp_is_enabled, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, mstp_enable, rcar_cpg_mstp_enable, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, mstp_disable, rcar_cpg_mstp_disable, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, mstp_reset, rcar_cpg_mstp_reset, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, mstp_assert, rcar_cpg_mstp_reset_assert, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, mstp_deassert, rcar_cpg_mstp_reset_deassert, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, mstp_reset_state, rcar_cpg_mstp_reset_check, tabsize);

    /* Module clock setting */
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, clk_is_enabled, rcar_cpg_clock_is_enabled, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, clk_enable, rcar_cpg_clock_enable, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, clk_disable, rcar_cpg_clock_disable, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, clk_rate_get, rcar_cpg_clock_rate_get, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, clk_rate_set, rcar_cpg_clock_rate_set, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, clk_divider_get, rcar_cpg_clock_divider_get, tabsize);
    CPG_MGR_ADD_FUNC(cpg_mgr_funcs_t, funcs, clk_divider_set, rcar_cpg_clock_divider_set, tabsize);

    return CPG_RET_OK;
}
