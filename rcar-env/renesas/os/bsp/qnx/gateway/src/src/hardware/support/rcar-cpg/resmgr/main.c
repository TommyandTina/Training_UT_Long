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

/* Connect functions */
static resmgr_connect_funcs_t connect_funcs;
static resmgr_io_funcs_t      io_funcs;
static iofunc_attr_t          attr;

static volatile uint8_t       driver_stop;
static char                   dllname[50] = { 0 };
int                           verbose = _SLOG_ERROR;

static pmic_model_t pmic_name_to_model(const char* const name)
{
    pmic_model_t model = PMIC_MODEL_UNKNOWN;

    if (name && (strcmp(name, "BD9571MWF") == 0)) {
        model = PMIC_MODEL_BOHM_BD9571MWF;
    }
    return model;
}

static void query_hwi_device(cpg_opts_t* const cpg_opts, avs_opts_t* const avs_opts)
{
    unsigned  hwi_off;
    unsigned  tag_idx;
    const hwi_tag *tag;
    const char* name;

    /* CPG */
    hwi_off = hwi_find_device(RCAR_HWI_CPG, 0);
    if(hwi_off != HWI_NULL_OFF){
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, NULL);
        if(tag){
            cpg_opts->base = (uint32_t) tag->location.base;
            cpg_opts->size = tag->location.len;
        }

        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, NULL);
        if(tag){
            cpg_opts->ext_clk = tag->inputclk.clk/tag->inputclk.div;
        }
    }

    /* AVS */
    hwi_off = hwi_find_device(RCAR_HWI_AVS, 0);
    if(hwi_off != HWI_NULL_OFF){
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, NULL);
        if(tag){
            avs_opts->base = (uint32_t) tag->location.base;
            avs_opts->size = tag->location.len;
        }
    }

    /* PMIC */
    hwi_off = hwi_find_device(RCAR_HWI_PMIC, 0);
    if (hwi_off == HWI_NULL_OFF) {
        return;
    }

    tag_idx = 0;
    while(1) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx);
        if (!tag){
            break;
        }
        name = __hwi_find_string(tag->regname.regname);
        if(strcmp(RCAR_HWI_PMIC_MODEL, name) == 0) {
            const char* const pmic_name = __hwi_find_string(tag->regname.offset);
            avs_opts->pmic.model = pmic_name_to_model(pmic_name);
        } else if(strcmp(RCAR_HWI_PMIC_I2C_BUS, name) == 0) {
            avs_opts->pmic.i2c_bus = (int) tag->regname.offset;
        } else if(strcmp(RCAR_HWI_PMIC_I2C_SLAVE_ADDR, name) == 0) {
            avs_opts->pmic.i2c_slave_addr = (int) tag->regname.offset;
        } else {
            cpg_slogf(_SLOG_ERROR, "%s: %s is not a valid Hwinfo default setting.",
                            __func__, name);
            break;
        }
    }
}

/* Handle the command line options */
static int parse_options(const int argc, char* const argv[], cpg_opts_t* const cpg_opts)
{
    int c;
    int ret = 0;

    while((c = getopt(argc, argv, "b:c:i:d:v")) != -1) {
        switch (c) {
            case 'b':
                cpg_opts->base = (uint32_t) strtoul(optarg, NULL, 0);
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
    int ret = -1, status = -1;
    int dcmd;

    status = iofunc_devctl_default(ctp, msg, ocb);
    if (status != _RESMGR_DEFAULT) {
        goto fail;
    }

    /* Unlockling attributes */
    if ((status = iofunc_attr_unlock(&attr))) {
        goto fail;
    }

    const cpg_support_funcs_t* const cpg_funcs = ctp->extra->data;
    dcmd = msg->i.dcmd;

    switch (dcmd) {
        case RCAR_CPG_DEVCTL_MODCTL:
            {
                cpg_mstpctl_t* const mstpctl = _DEVCTL_DATA(msg->i);
                ret = rcar_cpg_mstp_control(cpg_funcs, mstpctl, &status);
                if(ret == CPG_RET_OK) {
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
                if(ret == CPG_RET_OK) {
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
    return (_RESMGR_ERRNO(-1));
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
    avs_opts_t       avs_opts = { 0 };

    /* Make sure the driver isn't already running */
    if(cpg_single_instance(CPG_ATTACH_NAME)) {
        printf("%s: %s is already started\n", argv[0], CPG_ATTACH_NAME);
        return CPG_RET_ERROR;
    }

    if ( (ThreadCtl(_NTO_TCTL_IO, NULL)) == -1) {
        printf("%s: ThreadCtl failed\n", argv[0]);
        return CPG_RET_ERROR;
    }

    query_hwi_device(&cpg_opts, &avs_opts);

    /* Parse the command-line options */
    if(parse_options(argc, argv, &cpg_opts)) {
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
    status = cpg_funcs->init(&cpg_opts, &avs_opts);
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
    if(dpp == NULL) {
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

    /* Attach our device name */
    id= resmgr_attach(dpp, &resmgr_attr, CPG_DEV_NAME, _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &attr);
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
