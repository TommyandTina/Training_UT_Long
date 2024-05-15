/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation.
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
#include <string.h>
#include <sys/iofunc.h>
#include <fcntl.h>
#include <hw/rcar_sysc.h>

static int sysc_fd = -1;

static int rcar_sysc_init(void)
{
    /* Open SYSC resource manager */
    sysc_fd = open(SYSC_DEV_NAME, O_RDWR);
    if (sysc_fd == -1) {
        return SYSC_RET_ERROR;
    }

    return SYSC_RET_OK;
}


static void rcar_sysc_deinit(void)
{
    /* Close sysc resource manager fd */
    if (sysc_fd != -1) {
        close(sysc_fd);
    }
}

static int rcar_sysc_domain_control(const int pdr_number, const int cmd, int* const is_off)
{
    sysc_devctl_t sysc = {0};
    int status = SYSC_RET_OK;

    if (sysc_fd == -1) {
        return SYSC_RET_ERROR;
    }

    sysc.pdr_number = pdr_number;

    if(devctl(sysc_fd, cmd, &sysc, sizeof(sysc), &status)) {
        return SYSC_RET_ERROR;
    }

    if (is_off) {
        *is_off = sysc.is_off;
    }

    return status;
}

static int rcar_sysc_domain_power_is_off(const int pdr_number)
{
    int ret;
    int is_off = 0;

    ret = rcar_sysc_domain_control(pdr_number, RCAR_SYSC_DEVCTL_IS_POWER_OFF, &is_off);

    if (ret == SYSC_RET_ERROR) {
        return -1;
    }

    return is_off;
}

static int rcar_sysc_domain_power_on(const int pdr_number)
{
    return rcar_sysc_domain_control(pdr_number, RCAR_SYSC_DEVCTL_POWER_ON, NULL);
}

static int rcar_sysc_domain_power_off(const int pdr_number)
{
    return rcar_sysc_domain_control(pdr_number, RCAR_SYSC_DEVCTL_POWER_OFF, NULL);
}

static int rcar_sysc_read_pdmode(void)
{
    int pdmode = -1;

    rcar_sysc_domain_control(0, RCAR_SYSC_DEVCTL_READ_PDMODE, &pdmode);

    return pdmode;
}

/*
 * Returns the hardware access function table.
 * The "funcs" table passed from the caller is populated with the pointers to the low level functions.
 * The caller can access the low level functions after completing this call.
 *
 */
int sysc_mgr_getfuncs(sysc_mgr_funcs_t *funcs, const int tabsize)
{
    SYSC_MGR_ADD_FUNC(sysc_mgr_funcs_t, funcs, init, rcar_sysc_init, tabsize);
    SYSC_MGR_ADD_FUNC(sysc_mgr_funcs_t, funcs, deinit, rcar_sysc_deinit, tabsize);
    SYSC_MGR_ADD_FUNC(sysc_mgr_funcs_t, funcs, power_is_off, rcar_sysc_domain_power_is_off, tabsize);
    SYSC_MGR_ADD_FUNC(sysc_mgr_funcs_t, funcs, power_off, rcar_sysc_domain_power_off, tabsize);
    SYSC_MGR_ADD_FUNC(sysc_mgr_funcs_t, funcs, power_on, rcar_sysc_domain_power_on, tabsize);
    SYSC_MGR_ADD_FUNC(sysc_mgr_funcs_t, funcs, read_pdmode, rcar_sysc_read_pdmode, tabsize);

    return SYSC_RET_OK;
}
