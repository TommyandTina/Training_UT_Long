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

#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/procmgr.h>
#include <sys/mman.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <stdarg.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>

#include <hw/rcar_sysc_support.h>

static int rcar_sysc_attr_do_nothing(IOFUNC_ATTR_T *attr);

/* Connect functions */
static resmgr_connect_funcs_t    connect_funcs;
static resmgr_io_funcs_t         io_funcs;
static iofunc_attr_t             attr;
static iofunc_funcs_t            ocb_funcs = { _IOFUNC_NFUNCS,
                                               .ocb_calloc   = NULL, /* use default */
                                               .ocb_free     = NULL, /* use default */
                                               .attr_lock    = rcar_sysc_attr_do_nothing,
                                               .attr_unlock  = rcar_sysc_attr_do_nothing,
                                               .attr_trylock = rcar_sysc_attr_do_nothing };
static iofunc_mount_t            iofunc_mount = { 0, 0, 0, 0, &ocb_funcs };

static volatile uint8_t          driver_stop = 0;

static char                      dllname[50];
static void                      *sysc_handle;
static const sysc_support_funcs_t *sysc_funcs;

/* -v for verbose operation */
static int verbose = _SLOG_ERROR;

int sysc_slogf(const int level, const char* const fmt, ...);

/* sysc_slogf is also used by the sysc_support dll once linked. Do not make it static */
int sysc_slogf(const int level, const char* const fmt, ...)
{
    int         status = 0;
    va_list     arg;

    if( level <= verbose)
    {
        va_start(arg, fmt);
        status = vslogf(_SLOG_SETCODE(_SLOGC_TEST, 0), level, fmt, arg);
        va_end(arg);

        /* If slogger not running, send output to stderr */
        if( -1 == status)
        {
            va_start(arg, fmt);
            status = vfprintf(stderr, fmt, arg);
            va_end(arg);
        }
    }
    return status;
}

static void query_hwi_device(sysc_opts_t* const sysc_opts)
{
    unsigned  hwi_off;
    const hwi_tag *tag;

    hwi_off = hwi_find_device(RCAR_HWI_SYSC, 0);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, NULL);
        if (tag) {
            sysc_opts->base = (uint32_t) tag->location.base;
            sysc_opts->size  = tag->location.len;
        }
    }
}

/* Handle the command line options */
static int parse_options(const int argc, char* const argv[], sysc_opts_t* const sysc_opts)
{
    int c;
    int ret = SYSC_RET_OK;

    while ((c = getopt(argc, argv, "b:d:v")) != -1) {
        switch (c) {
            case 'b':
                sysc_opts->base = (uint32_t) strtoul(optarg, NULL, 0);
                const char* const cp = strchr (optarg, ':');
                if (cp) {
                    sysc_opts->size = (uint32_t) strtoul (cp + 1, NULL, 0);
                }
                break;
            case 'd':
                snprintf(dllname, sizeof(dllname), "sysc-%s.so", optarg);
                break;
            case 'v':
                verbose++;
                break;
            default:
                ret = SYSC_RET_ERROR;
                break;
        }
    }

    if (dllname[0] == 0) {
        sysc_slogf(_SLOG_ERROR, "%s: Support file %s not found", __func__, dllname);
        return SYSC_RET_ERROR;
    }

    return ret;
}


/* Ensure that the driver is not already running */
static int sysc_single_instance(const char* const attach_name)
{
    if (name_attach(NULL, attach_name, 0) == NULL) {
        sysc_slogf(_SLOG_ERROR, "%s: name_attach failed", __func__);
        return SYSC_RET_ERROR;
    }

    return SYSC_RET_OK;
}

static int rcar_sysc_init(sysc_opts_t* const sysc_opts)
{
    int status;

    sysc_handle = dlopen(dllname, RTLD_NOW|RTLD_GROUP|RTLD_WORLD);
    if (sysc_handle == NULL) {
        sysc_slogf(_SLOG_ERROR, "%s: unable to load support file %s", __func__, dllname);
        return SYSC_RET_ERROR;
    }

    sysc_funcs = dlsym(sysc_handle, "sysc_support_funcs");
    if (sysc_funcs == NULL) {
        sysc_slogf(_SLOG_ERROR, "%s: dlsym failed, dllname %s, error: %s",
            __func__, dllname, dlerror());
        return SYSC_RET_ERROR;
    }

    sysc_opts->verbose = verbose;
    status = sysc_funcs->init(sysc_opts);
    if (status != SYSC_RET_OK) {
        sysc_slogf(_SLOG_ERROR, "%s: sysc support init failed, status %d", __func__, status);
        return status;
    }

    return SYSC_RET_OK;
}

static void rcar_sysc_deinit(void)
{
    if (sysc_funcs) {
        sysc_funcs->deinit();
    }

    if (sysc_handle) {
        dlclose(sysc_handle);
    }
}

static int rcar_sysc_io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T* const ocb)
{
    int status = SYSC_RET_ERROR;
    unsigned long nbytes = 0;
    int dcmd;
    sysc_devctl_t* sysc;

    status = iofunc_devctl_default(ctp, msg, ocb);
    if (status != _RESMGR_DEFAULT) {
        sysc_slogf(_SLOG_ERROR, "%s: iofunc_devctl_default failed", __func__);
        status = SYSC_RET_ERROR;
        goto fail;
    }

    dcmd = msg->i.dcmd;
    nbytes = sizeof(sysc_devctl_t);
    /* Get the data from the message */
    sysc = _DEVCTL_DATA(msg->i);

    switch (dcmd) {
        case RCAR_SYSC_DEVCTL_POWER_ON:
            status = sysc_funcs->power_set(sysc->pdr_number, 0);
            break;
        case RCAR_SYSC_DEVCTL_POWER_OFF:
            status = sysc_funcs->power_set(sysc->pdr_number, 1);
            break;
        case RCAR_SYSC_DEVCTL_IS_POWER_OFF:
            status = sysc_funcs->power_is_off(sysc->pdr_number);
            break;
        case RCAR_SYSC_DEVCTL_READ_PDMODE:
            status = sysc_funcs->read_pdmode();
            break;
        default:
            status = SYSC_RET_ERROR;
            sysc_slogf(_SLOG_ERROR, "%s : Unknown command", __func__);
            goto fail;
    }

    SETIOV(&ctp->iov[0], &msg->o, sizeof(msg->o) + nbytes);
    memset(&msg->o, 0, sizeof(msg->o));
    sysc = _DEVCTL_DATA(msg->o);
    sysc->is_off = status;
    msg->o.ret_val = status;
    msg->o.nbytes = (unsigned)nbytes;
    return(_RESMGR_NPARTS(1));

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

static int rcar_sysc_attr_do_nothing(IOFUNC_ATTR_T *attr)
{
    (void)attr;
    return EOK;
}

int main(const int argc, char* const argv[]) {
    resmgr_attr_t    resmgr_attr;
    resmgr_context_t *ctp;
    dispatch_t       *dpp;
    int              id;
    int              status = SYSC_RET_OK;
    sysc_opts_t      sysc_opts = {0};

    /* Make sure the driver isn't already running */
    if ((sysc_single_instance(SYSC_ATTACH_NAME))) {
        printf("%s : %s is already started\n", argv[0], SYSC_ATTACH_NAME);
        return SYSC_RET_ERROR;
    }

    if ((ThreadCtl_r(_NTO_TCTL_IO, NULL))) {
        printf("%s : ThreadCtl failed\n", argv[0]);
        return SYSC_RET_ERROR;;
    }

    query_hwi_device(&sysc_opts);

    /* Parse the command-line options */
    if ((parse_options(argc, argv, &sysc_opts))) {
        printf("%s : Invalid option\n", argv[0]);
        return SYSC_RET_ERROR;
    }

    if ((status = rcar_sysc_init(&sysc_opts))) {
        printf("%s : rcar_sysc_init failed, status %d\n", argv[0], status);
        return SYSC_RET_ERROR;
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
        printf("%s : Unable to allocate dispatch handle\n", argv[0]);
        status = -1;
        goto fail;
    }

    /* Initialize resource manager attributes */
    memset(&resmgr_attr, 0, sizeof resmgr_attr);
    resmgr_attr.nparts_max = 1;
    resmgr_attr.msg_max_size = 2048;

    /* Initialize functions for handling messages */
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
            _RESMGR_IO_NFUNCS, &io_funcs);

    io_funcs.devctl = rcar_sysc_io_devctl;

    /* Initialize attribute structure used by the device */
    iofunc_attr_init(&attr, S_IFNAM | 0666, NULL, NULL);

    attr.mount = &iofunc_mount;

    /* Attach our device name */
    id= resmgr_attach(dpp, &resmgr_attr, SYSC_DEV_NAME, _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &attr);
    if (id == -1) {
        printf("%s : Couldn't attach pathname: %s\n", argv[0], SYSC_DEV_NAME);
        status = SYSC_RET_ERROR;
        goto fail;
    }

    /* Allocate a context structure */
    ctp = resmgr_context_alloc( dpp );

    procmgr_daemon(EXIT_SUCCESS, PROCMGR_DAEMON_NOCHDIR
                                | PROCMGR_DAEMON_NODEVNULL
                                | PROCMGR_DAEMON_NOCLOSE);

    sysc_slogf(_SLOG_INFO, "%s : R-Car SYSC resource manager started: %s\n", __FUNCTION__,
                        SYSC_DEV_NAME);

    /* Start the resource manager message loop */
    while (driver_stop == 0) {
        ctp = resmgr_block(ctp);
        if (ctp == NULL) {
           sysc_slogf(_SLOG_ERROR, "%s : block error\n", argv[0]);
           status = SYSC_RET_ERROR;
           break;
        }
        resmgr_handler(ctp);
    }

    resmgr_detach(dpp, id, _RESMGR_DETACH_ALL);
fail:
    rcar_sysc_deinit();

    return status;
}
