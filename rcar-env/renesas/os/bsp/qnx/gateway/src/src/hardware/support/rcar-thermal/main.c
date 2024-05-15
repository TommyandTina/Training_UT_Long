/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX baseelopment Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <stdio.h>
#include <atomic.h>
#include "proto.h"


/* our connect functions */
resmgr_connect_funcs_t connect_funcs;
resmgr_io_funcs_t io_funcs;

/* our dispatch, resource manager and iofunc variables */
resmgr_attr_t rattr;
rcar_tsc_dev_t *dev;

extern int verbose;
static volatile unsigned done = 0;

/*
 * Get current temperature:
 */
int thermal_get_temperature(thermal_packet_t *result)
{
    int ret = EOK, i;
    float temp;

    thermal_slogf(VERBOSE_LEVEL2, "%s", __FUNCTION__);

    for(i = dev->chan_min; i <= dev->chan_max ; i++) {
        if((ret = rcar_tsc_get_temp(dev->tsc[i], &temp)) != EOK)
            return ret;

        result->tsc_val[i] = temp;
    }

    result->tsc_min = dev->chan_min;
    result->tsc_max = dev->chan_max;

    return ret;
}

/*
 * Get current voltage:
 */
int thermal_get_voltage(thermal_packet_t *result)
{
    int ret = EOK, mvolt, i;

    thermal_slogf(VERBOSE_LEVEL2, "%s", __FUNCTION__);

    for(i = dev->chan_min; i <= dev->chan_max ; i++) {
        if((ret = rcar_tsc_get_mvolt(dev->tsc[i], &mvolt)) != EOK)
            return ret;

        result->tsc_val[i] = mvolt;
    }

    result->tsc_min = dev->chan_min;
    result->tsc_max = dev->chan_max;

    return ret;
}


/*
 * Single_instance
 * Ensure that the driver is not already running
 * Request a minor number of 0
 * If we get it, we're the first instantiation.
 */
int single_instance(char *resmgr_name)
{
    name_attach_t* attach;

    /* Create a local name */
    if ((attach = name_attach(NULL, resmgr_name, 0)) == NULL) {
        return -1;
    }

    return 0;
}

int option_parse(rcar_tsc_dev_t *dev, int argc, char *argv[])
{
    int c;
    char *end;
    int status = EOK;

    dev->upper_thrd = 60;
    dev->lower_thrd = 10;

    while ((c = getopt(argc, argv, "u:v")) != -1)
    {
        switch (c)
        {
            case 'u':
                dev->upper_thrd= strtoul(optarg, &end, 0);
                if(*end == ':') {
                    dev->lower_thrd = strtoul(end  + 1, NULL, 0);
                }
                break;
            case 'v':
                verbose++;
                break;
            default:
                status = -1;
                break;
        }
    }

    return status;
}

static void signal_handler(int signo)
{
    switch(signo) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGKILL:
        case SIGSEGV:
            atomic_set(&done, 1);
            break;
        default:
            break;
    }
}
       
int main(int argc, char *argv[])
{
    if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
        fprintf(stderr,"%s : ThreadCtl failed\n", argv[0]);
        exit(EXIT_FAILURE);
    }    

    /* Make sure the driver isn't already running */
    if(EOK != single_instance(RESMGR_NAME)) {
        fprintf(stderr,"%s: Path name is already existing\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    dev = (rcar_tsc_dev_t *)calloc(1, sizeof(rcar_tsc_dev_t));
    if(!dev){
        fprintf(stderr,"%s: Unable to allocate TMU device\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Parse the command-line options */
    if(option_parse(dev, argc, argv)) {
        fprintf(stderr, "%s: Invalid option\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*Allocate and initialize a dispatch structure for use by our resource manager loop*/
    dev->dpp = dispatch_create();
    if (NULL == dev->dpp) {
        fprintf(stderr,"%s: Failed to create dispatch\n", argv[0]);
        free(dev);
        exit(EXIT_FAILURE);
    }

    /*
     * Set up the resource manager attributes structure, we'll
     * use this as a way of passing information to resmgr_attach().
     * For now, we just use defaults.
     */
    memset(&rattr, 0, sizeof rattr);
    rattr.nparts_max = 1;
    rattr.msg_max_size = 2048;

    /*
     * Intialize the connect functions and I/O functions tables to
     * their defaults and then override the defaults with the
     * functions that we are providing.
     */
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
            _RESMGR_IO_NFUNCS, &io_funcs);

    // use our own devctl
    io_funcs.devctl = io_devctl;

    /*
     * Set the mode to 0666 (R/W)
     * Note leading 0 for octal -------v
     */
    iofunc_attr_init(&dev->hdr, S_IFCHR | 0666, NULL, NULL);

    /*
     * Call resmgr_attach to register our prefix with the
     * process manager, and also to let it know about our connect and I/O functions.
     * On error, returns -1 and errno is set.
     */
    dev->resmgr_id= resmgr_attach(dev->dpp, &rattr, DEV_NAME, _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &dev->hdr);
    if (dev->resmgr_id == -1) {
        fprintf(stderr,"%s : Couldn't attach pathname: %s\n", argv[0], DEV_NAME);
        resmgr_detach(dev->dpp, dev->resmgr_id, _RESMGR_DETACH_ALL);
        free(dev);
        exit(EXIT_FAILURE);
    }

    /* create a connection to the channel that our resource manager is receiving on */
    dev->coid = message_connect (dev->dpp, MSG_FLAG_SIDE_CHANNEL);
    if (dev->coid == -1) {
        fprintf(stderr,"%s: unable to connect to side channel\n", argv[0]);
        resmgr_detach(dev->dpp, dev->resmgr_id, _RESMGR_DETACH_ALL);
        free(dev);
        exit(EXIT_FAILURE);
    }

    /* Parse device information based on product ID */
    if(EOK != rcar_thermal_init(dev)) {
        fprintf(stderr,"%s: Parse device info failed\n", argv[0]);
        resmgr_detach(dev->dpp, dev->resmgr_id, _RESMGR_DETACH_ALL);
        free(dev);
        exit(EXIT_FAILURE);
    }

    /* Create signal handler to handle terminating signal */
    /* Install signal handlers */
    signal( SIGINT, signal_handler);
    signal( SIGQUIT, signal_handler);
    signal( SIGTERM, signal_handler);
    signal( SIGKILL, signal_handler);
    signal( SIGSEGV, signal_handler);

    /* Allocate a context structure */
    dev->ctp = dispatch_context_alloc( dev->dpp );

    /*  Start the resource manager loop */
    while (!done) {
        if ((dev->ctp = dispatch_block( dev->ctp )))
            dispatch_handler(dev->ctp);
        else
            break;
    }

    rcar_thermal_deinit(dev);

    if (dev->ctp) 
        dispatch_context_free(dev->ctp);
    resmgr_detach(dev->dpp, dev->resmgr_id, _RESMGR_DETACH_ALL);
    ConnectDetach(dev->coid);
    dispatch_destroy(dev->dpp);
    free(dev);

    return 0;
}

