/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
* Copyright 2021, Renesas Electronics Corporation.
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <atomic.h>
#include <signal.h>
#include <sys/procmgr.h>
#include <hw/pwm.h>
#include "pwm_private.h"

#define PWM_RESMGR_NAME		"pwm"
#define PWM_DEV_NAME		"/dev/" PWM_RESMGR_NAME			// device name

char* progname = NULL;
pwm_dev_t *dev;

static resmgr_connect_funcs_t    connect_funcs;
static resmgr_io_funcs_t         io_funcs;
static cpg_mgr_funcs_t           cpg_hwfuncs;

void fr_print_usemsg(void)
{

	fprintf(stdout,"-c  channel\n");
	fprintf(stdout,"          Select channel\n");
	fprintf(stdout,"          Example:  pwm-rcar -c 0\n");
}

/** 
 * @brief single_instance. Ensure that the driver is not already running
 * @param resmgr_name: Name of device
 * @return 0: success, -1: error
 */
 
static int single_instance(char* resmgr_name)
{
	name_attach_t* attach;

	// Create a local name
	if ((attach = name_attach(NULL, resmgr_name, 0)) == NULL){
		fprintf(stderr, "\nError: Is '%s' already started?  ", resmgr_name);
		perror("name_attach Failed");
		return(-1);
	}

	if (-1 == ChannelDestroy(attach->chid)){
		perror("ChannelDestroy failed:");
    }
	return 0;
}

void pwm_init(pwm_dev_t *dev)
{
	uint32_t value;
	uint32_t prr_val;

	/* Get Product ID */
	prr_val = in32(dev->prr);

	if(RCAR_GEN3_PRODUCT_ID(prr_val) != PRODUCT_ID_RCAR_V3U)
		value = CPG_MSTP_GEN3_PWM;
	else 
		value = (((6) & 0xFF) << 8) | ((28) & 0xFF); /* MSTPCR6, bit 28 */
	
	cpg_hwfuncs.mstp_enable(value);
}

int main(int argc, char **argv)
{
	int c = 0;
	uint8_t channel = 0;
	int status = EOK;
	char name[NAME_MAX];
	resmgr_attr_t rattr;
	int id = 0;
	
	while ((c = getopt(argc, argv, "hc:"
	)) != -1) {
		switch (c) {
			case 'c':
				channel = (uint8_t)strtol(optarg, NULL,10);
				break;
			case 'h':
				fr_print_usemsg();
				break;
		}
	}

	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)){
		fprintf(stderr,"%s: ThreadCtl failed: %s\n", progname, strerror(errno));
		status = EIO;
		exit(EXIT_FAILURE);
	}
	sprintf(name, "%s", PWM_RESMGR_NAME);
    if(single_instance(name)){
		exit(EXIT_FAILURE);
	}
	
	dev = (pwm_dev_t *) malloc(sizeof(pwm_dev_t ));
	if (dev == NULL)
	{
		fprintf(stderr,"%s: Couldn't malloc: %s\n",progname, strerror (errno));
		exit(EXIT_FAILURE);
	}
	/* register mapping for PWM channel 0 */
	if ((dev->pwm0 = mmap_device_io(PWM_SIZE, RCAR_GEN3_PWM0_BASE + channel * PWM_CHANNEL_OFFSET)) == (uintptr_t)MAP_FAILED) {
		fprintf(stderr,"%s: Couldn't mapping for ths 0: %s\n", __func__, strerror(errno));
		status = ENOMEM;
		goto fail1;
	}
	if ((cpg_mgr_getfuncs(&cpg_hwfuncs, sizeof(cpg_mgr_funcs_t))) != EOK) {
		fprintf(stderr,"%s: Unable to get cpg hw funcs\n", __func__);
		status = EIO;
		exit(EXIT_FAILURE);
	}
    else {
        cpg_hwfuncs.init(0);
    }
	if((dev->prr = mmap_device_io(4, RCAR_GEN3_PRODUCT_REGISTER)) == (uintptr_t)MAP_FAILED) {
		fprintf(stderr,"%s: Couldn't mapping for ths 0: %s\n", __func__, strerror(errno));
		status = ENOMEM;
		goto fail1;
	}
	pwm_init(dev);
	/* Initialize dispatch interface */
    if((dev->dpp = dispatch_create()) == NULL) {
        fprintf(stderr,"%s: Unable to allocate dispatch handle.\n",argv[0]);
        goto fail2;
    }
    /* Initialize resource manager attributes */
    memset(&rattr, 0, sizeof(rattr));
    rattr.nparts_max = 1;
    rattr.msg_max_size = MAX_MSG_SIZE;

    /* Initialize functions for handling messages */
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
                     _RESMGR_IO_NFUNCS, &io_funcs);
    io_funcs.devctl = pwm_devctl;

    /* Initialize attribute structure used by the device */
	iofunc_attr_init(&dev->att, S_IFCHR | 0666, 0, 0);
    /* Attach our device name */
    id = resmgr_attach(
            dev->dpp,			/* dispatch handle        */
            &rattr,				/* resource manager attrs */
			PWM_DEV_NAME,		/* device name            */
            _FTYPE_ANY,			/* open type              */
            0,					/* flags                  */
            &connect_funcs,		/* connect routines       */
            &io_funcs,			/* I/O routines           */
			(RESMGR_HANDLE_T *)dev); /* handle                 */
    if(id == -1){
        fprintf(stderr,"%s: Unable to attach name.\n", argv[0]);
        goto fail2;
    }
	dev->att.mount = &dev->mount;
	resmgr_devino(id,&dev->mount.dev,&dev->att.inode);
    /* Allocate a context structure */
    dev->ctp = dispatch_context_alloc(dev->dpp);

	// Run process in the background
	procmgr_daemon( 0, PROCMGR_DAEMON_NOCHDIR | PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL | PROCMGR_DAEMON_KEEPUMASK );

	/* Start the resource manager message loop */
    while(1) {
        if((dev->ctp = dispatch_block(dev->ctp)) == NULL) {
            fprintf(stderr, "block error\n");
			 goto fail2;
        }
        dispatch_handler(dev->ctp);
    }

fail2:	
	cpg_hwfuncs.deinit();

fail1:
	if(dev->prr != (uintptr_t)MAP_FAILED)
		munmap_device_io(dev->prr, 4);
		
	if(dev->pwm0 != (uintptr_t)MAP_FAILED)
		munmap_device_io(dev->pwm0, PWM_SIZE);
				
	if(dev != NULL)
	    free(dev);
		
	return status;
}
