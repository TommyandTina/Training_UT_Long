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

#include <stdio.h>
#include <string.h>
#include <atomic.h>
#include <signal.h>
#include <sys/procmgr.h>
#include <sys/resmgr.h>
#include <sys/procmgr.h>
#include "rcar-flexray.h"
//
#define RESMGR_NAME			"flexray"
#define DEV_NAME			"/dev/" RESMGR_NAME		// do not modify
char *options, *value;
extern char *optarg;
char *r_opts[] = {
#define FR_STATIC   			0
								"static",
#define FR_DYNAMIC  			1
								"dynamic",
#define FR_FIFO   				2
								"fifo",
#define FR_FRAME_ID_REJECTED   	3
								"reject",
#define FR_FRAME_ID_FILTER  	4
								"filter",
#define FR_CHANNEL_FILTER  		5
								"channel",
#define FR_CYCLE_COUNT  		6
								"cycle",
#define FR_SECURE_BUFFER  		7
								"secure",
#define FR_FIFO_LEVEL  			8
								"level",														
               NULL
};
resmgr_connect_funcs_t   connect_funcs;
resmgr_io_funcs_t        io_funcs;
static volatile unsigned running = 0;
fr_dev_t *pdev;
extern int	verbose;

void fr_print_usemsg(void)
{
#if 0	
	fprintf(stdout,"-b  baudrate\n");
	
	fprintf(stdout,"          Set baud rate for FlexRay\n");
	fprintf(stdout,"          baudrate 10 Set baudrate to 10 MBit/s \n");
	fprintf(stdout,"          baudrate 5  Set baudrate to 5 MBit/s \n");
	fprintf(stdout,"          baudrate 2  Set baudrate to 2.5 MBit/s \n");
	fprintf(stdout,"          Example:  flexray-rcar -b 10\n");
#endif	
	fprintf(stdout,"-c  static,dynamic,fifo,reject,filter,channel,cycle,level\n");
	fprintf(stdout,"          Configure message RAM.\n");
	fprintf(stdout,"          Options specific to configure message RAM. These options include:\n");
	fprintf(stdout,"                      static: The number of staic message buffer.\n");
	fprintf(stdout,"                      dynamic: The number of dynamic message buffer.\n");
	fprintf(stdout,"                      fifo: The number of fifo message buffer.\n");
	fprintf(stdout,"                      reject: The frame ID of the incoming frames to be rejected by the fifo.\n");
	fprintf(stdout,"                      filter: Frame ID used to mask the frame ID rejected by the fifo.\n");
	fprintf(stdout,"                      channel: The fifo receive channel filter\n");
	fprintf(stdout,"                                  channel=3:	No reception.\n");
	fprintf(stdout,"                                  channel=2:	Receive only on channel A.\n");
	fprintf(stdout,"                                  channel=1:	Receive only on channel B.\n"); 
	fprintf(stdout,"                                  channel=0:	Receive on both channels.\n");
	fprintf(stdout,"                      cycle: Cycle counter filter determines the cycle set to which frame ID and channel rejection filter are applied\n");
	fprintf(stdout,"                             In cycles not belonging to the cycle set specified by cycleCount, all frames are rejected.\n"); 
	fprintf(stdout,"                             cycle=0:	    Matching all cycles.\n"); 	
	fprintf(stdout,"                      secure: Type of receive fifo, reconfig and Sync Frame Payload Multiplex of message buffer control\n");
	fprintf(stdout,"                      level: Receive fifo critical level\n");
	fprintf(stdout,"                      For example:\n");		
	fprintf(stdout,"                      - Startup FlexRay with static message=8, dynamic message=22, fifo message=0\n");
	fprintf(stdout,"                      flexray-rcar -n 1 -c static=8,dynamic=22,fifo=0\n");		
	fprintf(stdout,"                      flexray-rcar -n 2 -c static=8,dynamic=22,fifo=0\n");
	fprintf(stdout,"                      - Startup FlexRay with static message=8, dynamic message=11, fifo message=11\n");
	fprintf(stdout,"                      flexray-rcar -n 1 -c static=8,dynamic=11,fifo=11\n");		
	fprintf(stdout,"                      flexray-rcar -n 2 -c static=8,dynamic=11,fifo=11\n");	
	fprintf(stdout,"-f  filename \n");
	fprintf(stdout,"          Specify filename as the absolute pathname to get the configuration of system parameters\n");
	fprintf(stdout,"          Example:  flexray-rcar -f /tmp/flexray-node.conf\n");
	fprintf(stdout,"-h        Print the usage of driver\n"); 
	fprintf(stdout,"-m  node  \n");
	fprintf(stdout,"          Select FlexRay mode.\n");
	fprintf(stdout,"          mode 0   Start driver use user RAM to handles the data transfer between URAM and FlexRay module(default mode 0).\n");
	fprintf(stdout,"          mode 1   Start driver do not use user RAM to handles the data transfer between URAM and FlexRay module.\n");	
	fprintf(stdout,"          Example:  Start driver use user RAM: flexray-rcar\n");
	fprintf(stdout,"          Example:  Start driver don't use user RAM: flexray-rcar -m 1\n");
	fprintf(stdout,"-n  node  \n");
	fprintf(stdout,"          Select FlexRay node.\n");
	fprintf(stdout,"          node 1   Start driver on node 1\n");
	fprintf(stdout,"          node 2   Start driver on node 2\n");		
	fprintf(stdout,"          Example:  Start driver on node 1: flexray-rcar -n 1\n");
	fprintf(stdout,"          Example:  Start driver on node 2: flexray-rcar -n 2\n");  	
	fprintf(stdout,"-p  priority \n");
	fprintf(stdout,"          The priority of the processing thread. The default is 10.\n");
	fprintf(stdout,"          Example:  flexray-rcar -p 10\n");	
	fprintf(stdout,"-v        Increase the verbosity\n");
	fprintf(stdout,"          Example:  flexray-rcar -vvv\n");
}

int parse_c(fr_dev_t *dev)
{	
	if ((optarg = strdup(optarg)) == NULL){
		return -1;
	}
	options = optarg;
	while (*options != '\0'){
		switch(getsubopt(&options,r_opts,&value)){
			case FR_STATIC : 
				if(value == NULL){
					dev->msg_ctl.staticBuffers = 8;
				}else{
					dev->msg_ctl.staticBuffers = atoi(value);
				}
				break;
			case FR_DYNAMIC: 
				if(value == NULL){
					dev->msg_ctl.dynamicBuffers = 22;
				}else{
					dev->msg_ctl.dynamicBuffers = atoi(value);
				}
				break;	
			case FR_FIFO: 
				if(value == NULL){
					dev->msg_ctl.fifoBuffers = 0;
				}else{
					dev->msg_ctl.fifoBuffers = atoi(value);
				}
				break;
			case FR_FRAME_ID_REJECTED: 
				if(value == NULL){
					dev->msg_ctl.frameIdRejected = 0;
				}else{
					dev->msg_ctl.frameIdRejected = atoi(value);
				}
				break;
			case FR_FRAME_ID_FILTER: 
				if(value == NULL){
					dev->msg_ctl.frameIdFilter = 0;
				}else{
					dev->msg_ctl.frameIdFilter = atoi(value);
				}
				break;
			case FR_CHANNEL_FILTER: 
				if(value == NULL){
					dev->msg_ctl.channelFilter = 0;
				}else{
					dev->msg_ctl.channelFilter = atoi(value);
				}
				break;
			case FR_CYCLE_COUNT: 
				if(value == NULL){
					dev->msg_ctl.cycleCount = 0;
				}else{
					dev->msg_ctl.cycleCount = atoi(value);
				}
				break;
			case FR_SECURE_BUFFER: 
				if(value == NULL){
					dev->msg_ctl.type = 0;
				}else{
					dev->msg_ctl.type = atoi(value);
				}
				break;
			case FR_FIFO_LEVEL: 
				if(value == NULL){
					dev->msg_ctl.fifoThreshold = 1;
				}else{
					dev->msg_ctl.fifoThreshold = atoi(value);
				}
				break;					
		}
	}
	return 0;
}

int fr_option(fr_dev_t *dev, int argc, char *agrv[])
{
	int c;
	int status = 0;
	do {
		c = getopt(argc, agrv, "hc:f:m:n:p:v");
		switch (c){
#if 0			
		case 'b':  
			if(optarg==NULL) {
				dev->baud_rate = 10;
			}else{
				dev->baud_rate = (uint8_t)strtol(optarg, NULL,10);
				if( (dev->baud_rate != 10) && (dev->baud_rate != 5)&& (dev->baud_rate != 2)){
					fprintf(stderr,"Invalid baud rate, it will be set 10 Mbit/s.\n");
				}
			}
			break;
#endif			
		case 'c':
			parse_c(dev);
			break;
		case 'f':
			memset(dev->system_parameter, 0, PATH_MAX);
			strncpy(dev->system_parameter,optarg,PATH_MAX-1);
			break;			
		case 'h':
			fr_print_usemsg();	
		case 'm':
			if(optarg==NULL) {
				dev->mode = 0;
			}else{
				dev->mode = (uint8_t)strtol(optarg, NULL,10);
			}
			break;				
		case 'n':
			if(optarg==NULL) {
				dev->node = FR_NODE1;
			}else{
				dev->node = (uint8_t)strtol(optarg, NULL,10);
			}
			break;	
		case 'p':
			if(optarg==NULL) {
				dev->priority = FR_PRIORITY;
			}else{
				dev->priority = (uint8_t)strtol(optarg, NULL,10);
			}
			break;			
		case 'v':
			verbose++;
			break;
		default:
			break;
		}
	} while ((-1 != c) && (0 == status));

	return 0;
}

static void sig_handler(int signo)
{
	atomic_set(&running, 1);
	dispatch_unblock(pdev->ctp);
}


/** 
 * @brief single_instance. Ensure that the driver is not already running
 * @param resmgr_name: Name of device.
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
 
int main(int argc, char *argv[])
{
	int                     id;
	resmgr_attr_t           rattr;
	struct sigaction        act;
	char name[NAME_MAX];
	int coid = 0;
	short code = 0;
	// Init resource manager 
	ThreadCtl( _NTO_TCTL_IO, 0 );
	
	sprintf(name, "%s", RESMGR_NAME);
    if(single_instance(name)){
		exit(EXIT_FAILURE);
	}
	
	pdev = (fr_dev_t *) calloc(1, sizeof(fr_dev_t));
	if (pdev == NULL){
		fprintf(stderr,"Unable to allocate memory for the device\n");
		return -1;
	}
	pdev->baud_rate 			= 10;
	pdev->priority 				= FR_PRIORITY;
	pdev->iid 					= -1;
	pdev->input_info.ita_fd 	= -1;
	pdev->output_info.ota_fd 	= -1;
	pdev->output_info.fifo_fd 	= -1;
	pdev->input_info.ita_vaddr 	= 0;
	pdev->input_info.iph_vaddr 	= 0;
	pdev->input_info.ipd_vaddr 	= 0;
	pdev->output_info.ota_vaddr = 0;
	pdev->output_info.oph_vaddr = 0;
	pdev->output_info.opd_vaddr = 0;
	pdev->output_info.ff_vaddr 	= 0;
	pdev->output_info.fph_vaddr = 0;
	pdev->output_info.fpd_vaddr = 0;
	pdev->fr_reg_base 			= 0;
	pdev->mode 					= 0;
	pdev->intr_timer			= true;
	pdev->timer_start           = false;
	pdev->timer_coid 			= -1;

	pdev->msg_ctl.staticBuffers = 8;
	pdev->msg_ctl.dynamicBuffers= 22;
	pdev->msg_ctl.fifoBuffers	= 0;
	pdev->msg_ctl.frameIdRejected = 0;
	pdev->msg_ctl.frameIdFilter = 0;
	pdev->msg_ctl.channelFilter = 0 ;
	pdev->msg_ctl.cycleCount	= 0;
	pdev->msg_ctl.type = FR_MSGCTL_NO_NULL_FRAME|FR_MSGBUF_UNLOCK;
	pdev->msg_ctl.fifoThreshold = 1;
	pdev->msg_lcb 				= 0;
	pdev->phybase				= 0;
	pdev->len					= 0;
	pdev->irq					= 0;
	pdev->msgq_size = FR_MAX_BUF;		// max number of queued flexray message buffers
	pdev->waitq_size = 16;				// max number of clients blocked wait for receive messages
	// Parsing options 
	fr_option(pdev, argc, argv);
	// Initializing FlexRay device
	fr_get_conf(pdev);
	if(fr_resmgr_init_device(pdev)){
		goto fr_exit;
	}
	if (rcar_flexray_init(pdev)) {
		fprintf(stderr,"%s: Unable to initialize the device.\n", __func__);
		goto fr_exit;
	}
	/* Initialize dispatch interface */
	pdev->dpp = dispatch_create();
	if(pdev->dpp == NULL){
		fprintf(stderr,"%s: Unable to allocate dispatch handle.\n",__func__);
		goto fr_exit;
	}
		
    // Register exit handler
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTERM);
    act.sa_handler = sig_handler;
    act.sa_flags = 0;
    sigaction(SIGTERM, &act, NULL);	
	
	/* Initialize resource manager attributes */
	memset(&rattr, 0, sizeof rattr);
	rattr.nparts_max = 1;
	rattr.msg_max_size =  sizeof(io_devctl_t) + sizeof(fr_msg_buffer_rx_t);
	/* Initialize connect functions and IO functions */
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,_RESMGR_IO_NFUNCS, &io_funcs);
	io_funcs.read = io_read;
	io_funcs.write = io_write;
	io_funcs.devctl = fr_devctl;
	io_funcs.unblock = io_unblock;	
	/* Initialize attribute structure */
	iofunc_attr_init_sized(&pdev->attr, S_IFNAM | 0666, NULL,NULL,sizeof(iofunc_attr_t));
	code = (short)pulse_attach(pdev->dpp, MSG_FLAG_ALLOC_PULSE, 0, event_pulse_handler, pdev);
	pdev->event.sigev_code = code;
	if (code == -1){
		fprintf(stderr,"%s: Unable to attach pulse event.\n",__func__);
		exit(EXIT_FAILURE);
	}
	coid = message_connect(pdev->dpp, MSG_FLAG_SIDE_CHANNEL);
	if(coid == -1){
		fprintf(stderr,"%s: Unable to attach channel and connection.\n",__func__);
		exit(EXIT_FAILURE);
	}
	pdev->event.sigev_notify = SIGEV_PULSE;
	pdev->event.sigev_coid = coid;
	pdev->event.sigev_value.sival_ptr = NULL;
	/* Attach our device name */
	sprintf(name, "%s", DEV_NAME);
	id = resmgr_attach(
			pdev->dpp,				/* dispatch handle        */
			&rattr,            		/* resource manager attrs */
			name,         			/* device name            */
			_FTYPE_ANY,             /* open type              */
			0,                      /* flags                  */
			&connect_funcs,         /* connect routines       */
			&io_funcs,              /* I/O routines           */
			(RESMGR_HANDLE_T *)pdev); /* handle                 */

	if(id == -1){
		fprintf(stderr,"%s: Unable to attach name under %s.\n",__func__,name);
		goto fr_exit;
	}
	pdev->attr.mount = &pdev->mount;
	resmgr_devino(id,&pdev->mount.dev,&pdev->attr.inode);
	/* Allocate a context structure */
	pdev->ctp = dispatch_context_alloc(pdev->dpp);
	if(NULL==pdev->ctp) {
		fprintf(stderr,"%s: resmgr_context_alloc() failed\n",__func__);
		goto fr_exit;
	}
	procmgr_daemon( 0,PROCMGR_DAEMON_NOCHDIR|PROCMGR_DAEMON_NOCLOSE|PROCMGR_DAEMON_NODEVNULL|PROCMGR_DAEMON_KEEPUMASK);
	/* Start the resource manager message loop */
	while(!running){
		pdev->ctp = dispatch_block (pdev->ctp);
		if(pdev->ctp != NULL) {
			dispatch_handler (pdev->ctp);
		}else if (errno != EFAULT) {
			atomic_set(&running, 1);
		}
	}
fr_exit:
	// Release the resources before exit
	msg_list_destroy(pdev->rx_free_queue);
	msg_list_destroy(pdev->tx_free_queue);
	client_queue_destroy(pdev->tx_wait_free_queue);
	client_queue_destroy(pdev->rx_wait_free_queue);
	rcar_fr_deinit(pdev);
	free((void*)pdev);
	return 0;
}
