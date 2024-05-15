/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/procmgr.h>
#include <aarch64/r-car.h>

#ifndef THREAD_POOL_PARAM_T
#define THREAD_POOL_PARAM_T dispatch_context_t
#endif

#include "pwm_priv.h"
#include "pwm_log.h"

pwm_device_t *g_device = NULL;
unsigned g_verbose = _SLOG_INFO;
bool setup_pin = false;
char* chan_list = NULL;

void pwm_cleanup_data()
{
    if(g_device->chans) {
        free(g_device->chans);
    }

    if(g_device->iobase != MAP_DEVICE_FAILED) {
        munmap_device_io(g_device->iobase, g_device->n_chan * RCAR_PWM_SIZE);
    }

    pwm_slogf(_SLOG_INFO, "Driver terminated");

    free(g_device); g_device = NULL;
}

int pwm_hw_init(void)
{
    int bit;

    if (setup_pin) {
        pwm_pfc_config();
    }

    if (cpg_mgr_getfuncs(&g_device->clk, sizeof(cpg_mgr_funcs_t)) != EOK) {
        fprintf(stderr, "Unable to access CPG resource\n");
        return -1;
    }

    g_device->clk.init(0);

    if (IS_RCAR_GEN3(g_device->product)) {
        bit = CPG_MSTP_GEN3_PWM;
    } else {
        bit = CPG_MSTP_GEN4_PWM;
    }

    if (!g_device->clk.mstp_is_enabled(bit)) {
        g_device->clk.mstp_enable(bit);
        g_device->clk.mstp_reset(bit);
    }

    g_device->clk.deinit();

    return 0;
}

int pwm_hw_deinit(void)
{
    int bit;

    if (!g_device->clk.init) {
        return 0;
    }

    if (IS_RCAR_GEN3(g_device->product)) {
        bit = CPG_MSTP_GEN3_PWM;
    } else {
        bit = CPG_MSTP_GEN4_PWM;
    }

    if (g_device->clk.mstp_is_enabled(bit)) {
        g_device->clk.mstp_disable(bit);
    }

    g_device->clk.deinit();

    return 0;
}

void *pwm_start_tpool(void *arg)
{
    pwm_resmgr_t *resmgr = &g_device->resmgr;

    resmgr->tpattr.handle = resmgr->dpp;
    resmgr->tpattr.context_alloc = dispatch_context_alloc;
    resmgr->tpattr.block_func = dispatch_block;
    resmgr->tpattr.unblock_func = dispatch_unblock;
    resmgr->tpattr.handler_func = dispatch_handler;
    resmgr->tpattr.context_free = dispatch_context_free;
    resmgr->tpattr.lo_water = 2;
    resmgr->tpattr.hi_water = 4;
    resmgr->tpattr.increment = 1;
    resmgr->tpattr.maximum = 50;

    if (!(resmgr->tpp = thread_pool_create(&resmgr->tpattr, POOL_FLAG_EXIT_SELF))) {
        fprintf(stderr, "Unable to create thread pool (%s)\n", strerror(errno));
        return NULL;
    }

    if (thread_pool_start(resmgr->tpp) < 0) {
        fprintf(stderr, "Unable to start thread pool (%s)\n", strerror(errno));
        return NULL;
    }

    return NULL;
}

int pwm_start_resmgr(void)
{
    pwm_resmgr_t *resmgr = &g_device->resmgr;

    memset(resmgr, 0, sizeof(pwm_resmgr_t));
 
    if (!(resmgr->dpp = dispatch_create())) {
        fprintf(stderr, "Unable to create resource dispatcher (%s)\n", strerror(errno));
        return -1;
    }

    resmgr->rattr.nparts_max = 2;
    resmgr->rattr.msg_max_size = 2048;

    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &resmgr->connect_funcs, _RESMGR_IO_NFUNCS, &resmgr->io_funcs);
    iofunc_attr_init((iofunc_attr_t*)&resmgr->ioattr, S_IFNAM | 0666, 0, 0);

    resmgr->io_funcs.devctl = pwm_devctl;

    resmgr->linkid = resmgr_attach(
                           resmgr->dpp,
                           &resmgr->rattr,
                           PWM_DEVICE,
                           _FTYPE_ANY,
                           0,
                           &resmgr->connect_funcs,
                           &resmgr->io_funcs,
                           (iofunc_attr_t*)&resmgr->ioattr);

    if (resmgr->linkid < 0) {
        fprintf(stderr, "Unable to attach resource manager (%s)\n", strerror(errno));
        return -1;
    }

    return 0;
}

void pwm_signal_handler(int signum)
{
    switch (signum) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGKILL:
        case SIGSEGV:
            g_device->force_quit = true;
            break;
        default:
            break;
    }
}

int pwm_select_channel(void)
{
    int idx;
    char *token;

    if (!chan_list) {
        for(int idx = 0; idx < g_device->n_chan; idx++) {
            g_device->chans[idx].active = true;
        }
        return 0;
    }

    while ((token = strtok_r(chan_list, " ,", &chan_list))) {
        if ((idx = (char)atoi(token)) >= g_device->n_chan) {
            fprintf(stderr, "Channel %d is not available\n", idx);
            return -1;
        }
        g_device->chans[idx].active = true;
    }

    return 0;
}

int pwm_parse_option(int argc, char *argv[])
{
    int opt, index;

    static struct option options[] = {
        { "channel",    required_argument, 0, 'c' },
        { "verbose",    required_argument, 0, 'v' },
        { "setup-pin",  no_argument,       0, 's' },
        { NULL,         0,                 0,  0  }
    };

    while ((opt = getopt_long(argc, argv,"c:v:s", options, &index )) != -1) {
        switch (opt) {
            case 'c':
                chan_list = strdup(optarg);
                break;
            case 'v':
                g_verbose = atoi(optarg);
                break;
            case 's':
                setup_pin = true;
                break;
            default:
                return -1;
        }
    }

    return 0;
}

int pwm_check_existance(void)
{
    if (!access(PWM_DEVICE, F_OK)) {
        fprintf(stderr, "Unable to create device node (%s)\n", PWM_DEVICE);
        return -1;
    }

    return 0;
}

int pwm_check_product(void)
{
    uintptr_t prr;

    if ((prr = mmap_device_io(4, RCAR_PRODUCT_REGISTER)) == MAP_DEVICE_FAILED) {
        fprintf(stderr, "Unable to mmap device memory (%s)\n", strerror(errno));
        return -1;
    }

    g_device->product = RCAR_PRODUCT_ID(in32(prr));
    g_device->refclk = 66666667;

    switch (g_device->product) {
        case PRODUCT_ID_RCAR_H3:
        case PRODUCT_ID_RCAR_M3W:
        case PRODUCT_ID_RCAR_M3N:
        case PRODUCT_ID_RCAR_V3M:
        case PRODUCT_ID_RCAR_V3H:
        case PRODUCT_ID_RCAR_E3:
        case PRODUCT_ID_RCAR_D3:
            g_device->n_chan = 7;
            break;
        case PRODUCT_ID_RCAR_V3U:
            g_device->n_chan = 5;
            break;
        case PRODUCT_ID_RCAR_V4H:
            g_device->n_chan = 10;
            break;
        default:
            fprintf(stderr, "Unsupported product (0x%02x)\n", g_device->product);
            return -1;
    }

    pwm_slogf(_SLOG_DEBUG1, "Detected chip 0x%02x, n_chan %d, refclk %d", g_device->product, g_device->n_chan, g_device->refclk);

    if (!(g_device->chans = calloc(g_device->n_chan, sizeof(pwm_channel_t)))) {
        fprintf(stderr, "Unable to allocate memory (%s)\n", strerror(errno));
        return -1;
    }

    munmap_device_io(prr, 4);

    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    if (ThreadCtl(_NTO_TCTL_IO, 0) < 0) {
        fprintf(stderr, "Unable to request I/O privilege (%s)\n", strerror(errno));
        abort();
    }

    if (pwm_parse_option(argc, argv) < 0) {
        abort();
    }

    if (!(g_device = calloc(1, sizeof(pwm_device_t)))) {
        fprintf(stderr, "Unable to allocate memory (%s)\n", strerror(errno));
        abort();
    }

    if ((pwm_check_product() < 0) || 
       (pwm_check_existance() < 0) ||
       (pwm_select_channel() < 0)) {
        goto cleanup;
    }

    if ((g_device->iobase = (uintptr_t)mmap_device_io(g_device->n_chan * RCAR_PWM_SIZE, RCAR_PWM0_BASE)) == MAP_DEVICE_FAILED) {
        fprintf(stderr, "Unable to map device memory (%s)\n", strerror(errno));
        goto cleanup;
    }

    for (int idx = 0; idx < g_device->n_chan; idx++) {
        if(g_device->chans[idx].active) {
            g_device->chans[idx].index = idx;
            g_device->chans[idx].iobase = g_device->iobase + idx * RCAR_PWM_SIZE;
            pthread_mutex_init(&g_device->chans[idx].lock, NULL);
        }
    }

    signal(SIGINT,  pwm_signal_handler);
    signal(SIGQUIT, pwm_signal_handler);
    signal(SIGTERM, pwm_signal_handler);
    signal(SIGKILL, pwm_signal_handler);
    signal(SIGSEGV, pwm_signal_handler);

    if ((pwm_hw_init() < 0) || (pwm_start_resmgr() < 0)) {
        goto cleanup;
    }

    procmgr_daemon(EXIT_SUCCESS, PROCMGR_DAEMON_NOCHDIR | 
                                 PROCMGR_DAEMON_NODEVNULL |
                                 PROCMGR_DAEMON_NOCLOSE);

    if (pthread_create(&tid, NULL, pwm_start_tpool, NULL) < 0) {
        fprintf(stderr, "Start thread pool failed (%s)\n", strerror(errno));
        goto cleanup;
    } else {
        pwm_slogf(_SLOG_INFO, "Starting resource manager");
    }

    while (!g_device->force_quit) {
        sleep(1);
    }

cleanup:
    pwm_hw_deinit();
    pwm_cleanup_data();

    return 0;
}
