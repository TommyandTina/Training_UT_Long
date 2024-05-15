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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <devctl.h>
#include <dlfcn.h>
#include <sys/procmgr.h>
#include <sys/resmgr.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>
#include <aarch64/r-car.h>
#include "proto.h"

static int rcar_cpg_attr_do_nothing(IOFUNC_ATTR_T *attr);

/* Connect functions */
static resmgr_connect_funcs_t connect_funcs;
static resmgr_io_funcs_t      io_funcs;
static iofunc_attr_t          attr;
static iofunc_funcs_t         ocb_funcs = { _IOFUNC_NFUNCS,
                                            .ocb_calloc   = NULL, /* use default */
                                            .ocb_free     = NULL, /* use default */
                                            .attr_lock    = rcar_cpg_attr_do_nothing,
                                            .attr_unlock  = rcar_cpg_attr_do_nothing,
                                            .attr_trylock = rcar_cpg_attr_do_nothing };
static iofunc_mount_t         iofunc_mount = { 0, 0, 0, 0, &ocb_funcs };

static volatile uint8_t       driver_stop;
static char                   dllname[50] = { 0 };
int                           verbose = _SLOG_ERROR;

static void query_hwi_device(cpg_opts_t* const cpg_opts)
{
    unsigned  hwi_off;
    const hwi_tag *tag;

    /* CPG */
    hwi_off = hwi_find_device(RCAR_HWI_CPG, 0);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, NULL);
        if (tag) {
            cpg_opts->base = (uint32_t) tag->location.base;
            cpg_opts->size = tag->location.len;
        }

        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, NULL);
        if (tag) {
            cpg_opts->ext_clk = tag->inputclk.clk/tag->inputclk.div;
        }
    } else {
        cpg_slogf(_SLOG_WARNING,"%s: RCAR_HWI_CPG not found", __func__);
    }
    cpg_slogf(_SLOG_INFO,"%s: ext_clk = %u",__func__,cpg_opts->ext_clk);
}

/* Handle the command line options */
static int parse_options(const int argc, char* const argv[], cpg_opts_t* const cpg_opts)
{
    int c;
    int ret = 0;

    while ((c = getopt(argc, argv, "b:s:c:i:d:v")) != -1) {
        switch (c) {
            case 'b':
                cpg_opts->base = (uint32_t) strtoul(optarg, NULL, 0);
                break;
            case 's':
                cpg_opts->size = (uint32_t) strtoul(optarg, NULL, 0);
                break;
            case 'c':
                cpg_opts->ext_clk = (uint32_t) strtoul(optarg, NULL, 0);
                break;
            case 'd':
                snprintf(dllname, sizeof(dllname), "cpg-%s.so", optarg);
                break;
            case 'v':
                verbose++;
                break;
            default:
                ret = (-1);
                break;
        }
    }

    if (dllname[0] == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Support file %s not found", __func__, dllname);
        return (-1);
    }

    return ret;
}

/* Ensure that the driver is not already running */
static int cpg_single_instance(const char* const attach_name)
{
    if (name_attach(NULL, attach_name, 0) == NULL) {
        return -1;
    }
    return 0;
}

static int rcar_cpg_io_devctl(resmgr_context_t *ctp, io_devctl_t* const msg, RESMGR_OCB_T* const ocb)
{
    unsigned nbytes = 0;
    int ret = CPG_RET_ERROR, status = 0;
    int dcmd;

    if ((iofunc_devctl_default(ctp, msg, ocb)) != _RESMGR_DEFAULT) {
        cpg_slogf(_SLOG_ERROR, "%s: iofunc_devctl_default failed", __func__);
        goto fail;
    }

    const cpg_support_funcs_t* const cpg_funcs = ctp->extra->data;
    dcmd = msg->i.dcmd;

    switch (dcmd) {
        case RCAR_CPG_DEVCTL_MODCTL:
            {
                cpg_mstpctl_t* const mstpctl = _DEVCTL_DATA(msg->i);
                ret = rcar_cpg_mstp_control(cpg_funcs, mstpctl, &status);
                if (ret == CPG_RET_OK) {
                    nbytes = sizeof(cpg_mstpctl_t);
                    SETIOV(&ctp->iov[0], &msg->o, sizeof(msg->o) + nbytes);
                    memset(&msg->o, 0, sizeof(msg->o));
                    msg->o.ret_val = status;
                    msg->o.nbytes = nbytes;
                    return(_RESMGR_NPARTS(1));
                }
            }
            break;
        case RCAR_CPG_DEVCTL_CLKCTL:
            {
                cpg_clkctl_t* const clkctl = _DEVCTL_DATA(msg->i);
                ret = rcar_cpg_clk_control(cpg_funcs, clkctl, &status);
                if (ret == CPG_RET_OK) {
                    nbytes = sizeof(cpg_clkctl_t);
                    SETIOV(&ctp->iov[0], &msg->o, sizeof(msg->o) + nbytes);
                    memset(&msg->o, 0, sizeof(msg->o));
                    msg->o.ret_val = status;
                    msg->o.nbytes = nbytes;
                    return(_RESMGR_NPARTS(1));
                }
            }
            break;
        default:
            cpg_slogf(_SLOG_ERROR, "%s: Unknown command", __func__);
    }
fail:
    return (_RESMGR_ERRNO(EIO));
}

static void rcar_signal_handler(const int signo)
{
    switch(signo) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGKILL:
        case SIGSEGV:
            driver_stop = 1;
            break;
        default:
            break;
    }
}

static int rcar_cpg_attr_do_nothing(IOFUNC_ATTR_T *attr)
{
    (void)attr;
    return EOK;
}

int main(const int argc, char* const argv[]) {
    resmgr_attr_t    resmgr_attr;
    resmgr_context_t *ctp;
    dispatch_t       *dpp;
    struct _extended_context extended_data;
    int              id;
    int              status = CPG_RET_OK;
    void*            cpg_support_handle = NULL;
    cpg_support_funcs_t* cpg_funcs;
    cpg_opts_t       cpg_opts = { 0 };

    /* Make sure the driver isn't already running */
    if (cpg_single_instance(CPG_ATTACH_NAME)) {
        printf("%s: %s is already started\n", argv[0], CPG_ATTACH_NAME);
        return CPG_RET_ERROR;
    }

    if ((ThreadCtl(_NTO_TCTL_IO, NULL)) == -1) {
        printf("%s: ThreadCtl failed\n", argv[0]);
        return CPG_RET_ERROR;
    }

    query_hwi_device(&cpg_opts);

    /* Parse the command-line options */
    if (parse_options(argc, argv, &cpg_opts)) {
        printf("%s: Invalid options\n", argv[0]);
        return CPG_RET_ERROR;
    }

    cpg_support_handle = dlopen(dllname, RTLD_NOW|RTLD_GROUP|RTLD_WORLD);
    if (cpg_support_handle == NULL) {
        printf("%s: unable to load support file %s, errno %d\n", argv[0], dllname, errno);
        return CPG_RET_ERROR;
    }

    cpg_funcs = dlsym(cpg_support_handle, "cpg_support_funcs");
    if (cpg_funcs == NULL) {
        printf("%s: dlsym failed, dllname %s, error: %s\n", argv[0], dllname, dlerror());
        status = CPG_RET_ERROR;
        goto fail;
    }

    cpg_opts.verbose = verbose;
    status = cpg_funcs->init(&cpg_opts);
    if (status) {
        printf("%s: cpg support init failed, status %d\n", argv[0], status);
        status = CPG_RET_ERROR;
        goto fail;
    }

    /* Install signal handlers */
    signal( SIGINT, rcar_signal_handler);
    signal( SIGQUIT, rcar_signal_handler);
    signal( SIGTERM, rcar_signal_handler);
    signal( SIGKILL, rcar_signal_handler);
    signal( SIGSEGV, rcar_signal_handler);

    /* Initialize dispatch interface */
    dpp = dispatch_create();
    if (dpp == NULL) {
        printf("%s: Unable to allocate dispatch handle\n", argv[0]);
        status = CPG_RET_ERROR;
        goto fail;
    }

    /* Initialize resource manager attributes */
    memset(&resmgr_attr, 0, sizeof resmgr_attr);
    resmgr_attr.nparts_max = 1;
    resmgr_attr.msg_max_size = 2048;

    /* Initialize functions for handling messages */
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
            _RESMGR_IO_NFUNCS, &io_funcs);

    io_funcs.devctl = rcar_cpg_io_devctl;

    /* Initialize attribute structure used by the device */
    iofunc_attr_init(&attr, S_IFNAM | 0666, NULL, NULL);

    attr.mount = &iofunc_mount;

    /* Attach our device name */
    id = resmgr_attach(dpp, &resmgr_attr, CPG_DEV_NAME, _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &attr);
    if (id == -1) {
        printf("%s: Couldn't attach pathname: %s\n", argv[0], CPG_DEV_NAME);
        status = CPG_RET_ERROR;
        goto fail;
    }

    /* Allocate a context structure */
    ctp = resmgr_context_alloc( dpp );

    extended_data.data = (void*) cpg_funcs;
    extended_data.length = sizeof(struct _extended_context);
    ctp->extra = &extended_data;

    procmgr_daemon(EXIT_SUCCESS, PROCMGR_DAEMON_NOCHDIR
                                | PROCMGR_DAEMON_NODEVNULL
                                | PROCMGR_DAEMON_NOCLOSE);

    cpg_slogf(_SLOG_INFO, "%s: R-Car CPG resource manager started: %s", __FUNCTION__,
                        CPG_DEV_NAME);

    /* Start the resource manager message loop */
    while (driver_stop == 0) {
        ctp = resmgr_block(ctp);
        if (ctp == NULL) {
           cpg_slogf(_SLOG_ERROR, "%s: block error", argv[0]);
           status = CPG_RET_ERROR;
           break;
        }
        resmgr_handler(ctp);
    }

    resmgr_detach(dpp, id, _RESMGR_DETACH_ALL);
fail:
    if (cpg_funcs) {
        cpg_funcs->deinit();
    }

    if (cpg_support_handle) {
        dlclose(cpg_support_handle);
    }

    return status;
}
