/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */
 
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <atomic.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <sys/resmgr.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/procmgr.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>
#include <hw/gpio.h>
#include <aarch64/r-car.h>
#include "proto.h"

// resmgr name
#define RESMGR_NAME			"gpio"
#define DEV_NAME			"/dev/" RESMGR_NAME		// do not modify

// our connect functions
resmgr_connect_funcs_t	connect_funcs;
resmgr_io_funcs_t		io_funcs;

//	our dispatch, resource manager and iofunc variables

gpio_drv_info_t 		*gpio_dev = NULL;
char*   progname = NULL;
static volatile unsigned done = 0;
extern int verbose;
/** 
 * @brief usage - Print Usage options and exit. 
 */
static void usage(char* progname) 
{
    printf( "Usage: %s [options]\n"
            "\t-g        GPIO group number\n"
            "\t-p        GPIO base address:GPIO size\n"
            "\t-f        FPC base address:PFC size\n"
            "\t-i        GPIO IRQ numbder\n"
            "\t-I        GPIO group number to query GPIO base, IRQ, ... from HWINFO\n"
            "\t-v        verbosity\n"
            "\t-h        prints this info\n"
            "\n",
            progname);
}

/** 
 * @brief signal_handler - Handler termination condition.
 */
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

static void query_hwinfo(gpio_drv_info_t *dev, int unit)
{
    unsigned hwi_off, tag_idx;
    hwi_tag *tag;

    hwi_off = hwi_find_device(RCAR_HWI_GPIO, unit);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            dev->phybase = tag->location.base;
            dev->size = tag->location.len;

            tag_idx = 1;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, &tag_idx);
            if (tag) {
                dev->pfc_phybase = tag->location.base;
                dev->pfc_size = tag->location.len;
            }

            dev->irq = hwitag_find_ivec(hwi_off, 0);

            tag_idx = 0;
            if ((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                if (strcmp(RCAR_HWI_GPIO_GROUP_NUM,  __hwi_find_string(tag->regname.regname)) == 0) {
                    dev->groups = tag->regname.offset;
                }
            }
        }
    }
}

/** 
 * @brief options - Parse command line when start the GPIO driver.
 * @param opts: Store options for processing.
 * @param argc:
 * @param argv: 
 * @return EOK - No error
 *         -1  - Error
 */
static int options(gpio_drv_info_t *dev, int argc, char* argv[]) 
{
    char *cp;
    int	c;
    hwi_tag *tag;
    unsigned hwi_off;

    dev->group = -1;
    dev->verbose = 0;
    dev->phybase = 0;
    dev->size = 0;
    dev->pfc_phybase = 0;
    dev->pfc_size = 0;
    dev->irq = 0;
    dev->gen3_soc_type = 1;

    while ((c = getopt(argc, argv, "hg:vp:f:i:I:")) != -1) {
        switch (c) 
        {
            case 'p':
                dev->phybase = strtoul(optarg, NULL, 0);
                if ((cp = strchr(optarg, ':'))) {
                    optarg = cp+1;
                    dev->size = strtoul(optarg, NULL, 0);
                }
                break;
            case 'f':
                dev->pfc_phybase = strtoul(optarg, NULL, 0);
                if ((cp = strchr(optarg, ':'))) {
                    optarg = cp+1;
                    dev->pfc_size = strtoul(optarg, NULL, 0);
                }
                break;
            case 'i':
                dev->irq  = strtoul(optarg, NULL, 0);
                break;
            case 'g':
                dev->group = atoi(optarg);
                break;
            case 'I':
                dev->group = atoi(optarg);
                query_hwinfo(dev, dev->group);
                break;
            case 'v':
                verbose++;
                break;
            case 'h':
                usage(argv[0]);
                return 1;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    if (dev->group == -1) {
        printf ("Invalid group %d\n", dev->group);
        return -1;
    }

    hwi_off = hwi_find_device("r-car", 0);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, 0);
        if (tag) {
             dev->product_id = tag->hwversion.version;
             dev->version_id = tag->hwversion.hclass;
        } else {
            printf("Unable to query product_id from HW info\n");
            return (-1);
        }
    } else {
        printf("Unable to find product version from HW info\n");
        return (-1);
    }

    if (dev->product_id == 0) {
        printf ("Invalid product id (%x)\n", dev->product_id);
        return -1;
    }

    dev->gen3_soc_type = !IS_RCAR_GEN4(dev->product_id);

    return EOK;
}


/** 
 * @brief gpio_init - initialize GPIO.
 * @param dev: Store information related to gpio for processing.
 * @return :
 *      EOK - No error
 *      Other - Error
 */
static int gpio_init(gpio_drv_info_t *dev) 
{
    gpio_slogf(1, "GPIO base = 0x%08x, PFC base =0x%08x, irq = %d", dev->phybase, dev->pfc_phybase, dev->irq);

    if (!dev->gen3_soc_type) { // for Gen4 : V3U, S4, V4H
        if (!dev->phybase  || ((dev->pfc_reg = mmap_device_io(dev->size, dev->phybase)) == MAP_DEVICE_FAILED)) {
            fprintf(stderr, "mmap_device_io with gpio_regbase(0x%08x)", dev->phybase);
            return -ENOMEM;
        } 
        dev->gpio_reg = (dev->pfc_reg + RCAR_GEN4_GPIO_IOINTSEL);
     } else { //for other Gen3
        if (!dev->phybase || ((dev->gpio_reg = mmap_device_io(dev->size, dev->phybase)) ==  MAP_DEVICE_FAILED)) {
            fprintf(stderr, "mmap_device_io with gpio_regbase(0x%08x)", dev->phybase);
            return -ENOMEM;
        }
        if (!dev->pfc_phybase || ((dev->pfc_reg = mmap_device_io(dev->pfc_size, dev->pfc_phybase)) == MAP_DEVICE_FAILED)) {
            munmap_device_io(dev->gpio_reg, dev->size);
            fprintf(stderr, "mmap_device_io with regbase(0x%08x)", dev->pfc_phybase);
            return -ENOMEM;
        }
     }

    return EOK;
}


/** 
 * @brief gpio_fini - GPIO finish.
 * @param dev: Store information related to gpio for processing.
 */
static void gpio_fini (gpio_drv_info_t *dev) {
    
    if (!dev->gen3_soc_type){ //  For Gen4 : V3U, S4, V4H
        munmap_device_io (dev->pfc_reg, dev->size);
    } else { // For other Gen3
        munmap_device_io (dev->pfc_reg, dev->pfc_size);
        munmap_device_io (dev->gpio_reg, dev->size);
     }
}


/** 
 * @brief single_instance - Ensure that the driver is not already running
 * request a minor number of 0 if we get it, we're the first instantiation.
 */
static void single_instance(char* resmgr_name) 
{
    name_attach_t* attach;

    // Create a local name
    if ((attach = name_attach(NULL, resmgr_name, 0)) == NULL) {
        fprintf(stderr, "\nError: Is '%s' already started?  ", resmgr_name);
        perror("name_attach Failed");
        exit(EXIT_FAILURE);
    }

    if (-1 == ChannelDestroy(attach->chid)) {
        perror("ChannelDestroy failed:");
    }
}

/** 
 * @brief main - Main routine for driver and resource manager.
 * @return EXIT_SUCCESS - No error
 */
int main(int argc, char *argv[]) 
{
    dispatch_t*			dpp;
    resmgr_attr_t		rattr;
    dispatch_context_t*	ctp;
    iofunc_attr_t       ioattr;
    int		pathID;
    int		ret;
    char 	*name;

    if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
        return 0;
    }

    if (argc == 0) {
        return 0;
    }

    progname = argv[0];

    gpio_dev = malloc(sizeof(gpio_drv_info_t));
    if (!gpio_dev) {
        fprintf(stderr, "%s: malloc failed\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // parse the command-line options
    if ((ret = options(gpio_dev, argc, argv))) {
        exit((ret < 0) ? EXIT_FAILURE : 0);
    }

    if ((name = malloc(sizeof(DEV_NAME) + 1)) == NULL) {
        fprintf(stderr, "%s: malloc failed\n", argv[0]);
        free(gpio_dev);
        exit(EXIT_FAILURE);
    }

    sprintf(name, "%s%d", RESMGR_NAME, gpio_dev->group);
    single_instance(name);

    //	allocate and initialize a dispatch structure for use by our main loop
    dpp = dispatch_create();
    if (dpp == NULL) {
        fprintf(stderr, "%s:  couldn't dispatch_create: %s\n",
                argv[0], strerror (errno));
        free(gpio_dev);
        exit(EXIT_FAILURE);
    }

    // Run process in the background
    procmgr_daemon( 0, PROCMGR_DAEMON_NOCHDIR | PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL | PROCMGR_DAEMON_KEEPUMASK );

    //
    // Set up the resource manager attributes structure, we'll
    // use this as a way of passing information to resmgr_attach().
    // For now, we just use defaults.
    //
    memset(&rattr, 0, sizeof (rattr));
    rattr.nparts_max = MAX_IOV_SIZE;	// max ctp->iov[] size
    rattr.msg_max_size = MAX_MSG_SIZE;	// max size of a message

    //
    // Intialize the connect functions and I/O functions tables to
    // their defaults and then override the defaults with the
    // functions that we are providing.
    //
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
                     _RESMGR_IO_NFUNCS, &io_funcs);

    // Use our own: read, write, and devctl
    io_funcs.devctl = io_devctl;

    //	Set the mode to 0666 (R/W)
    //	Note leading 0 for octal -------v
    iofunc_attr_init(&ioattr, S_IFCHR | 0666, NULL, NULL);

    //
    //	Call resmgr_attach to register our prefix with the
    //	process manager, and also to let it know about our connect
    //	and I/O functions.
    //
    //	On error, returns -1 and errno is set.
    //
    sprintf(name, "%s%d", DEV_NAME, gpio_dev->group);
    pathID = resmgr_attach(dpp, &rattr, name, _FTYPE_ANY, 0,
                         &connect_funcs, &io_funcs, &ioattr);
    free(name);
    if (pathID == -1) {
        fprintf(stderr, "%s:  couldn't attach pathname: %s\n",
                progname, strerror(errno));
        dispatch_destroy(dpp);
        free(gpio_dev);
        exit(EXIT_FAILURE);
    }

    // Initialize device
    if (gpio_init(gpio_dev)) {
        resmgr_detach(dpp, pathID, _RESMGR_DETACH_ALL);
        dispatch_destroy(dpp);
        free(gpio_dev);
        exit(EXIT_FAILURE);
    }

    /* Create signal handler to handle terminating signal */
    /* Install signal handlers */
    signal( SIGINT, signal_handler);
    signal( SIGQUIT, signal_handler);
    signal( SIGTERM, signal_handler);
    signal( SIGKILL, signal_handler);
    signal( SIGSEGV, signal_handler);

    ctp = dispatch_context_alloc (dpp);

    //main loop to process calls into driver or manager
    while (!done) {
        if ((ctp = dispatch_block(ctp)))
            dispatch_handler(ctp);
        else {
            //fprintf (stderr, "%s:  dispatch_block failed: %s\n", 
            //        progname, strerror(errno));
            break;
        }

    }

    if (ctp) 
        dispatch_context_free(ctp);
    resmgr_detach(dpp, pathID, _RESMGR_DETACH_ALL);
    dispatch_destroy(dpp);
    gpio_fini(gpio_dev);
    free(gpio_dev);

    return EXIT_SUCCESS;
}
