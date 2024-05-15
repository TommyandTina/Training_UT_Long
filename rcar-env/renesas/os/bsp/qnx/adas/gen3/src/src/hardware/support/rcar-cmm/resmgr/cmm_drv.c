/*
 * Renesas Proprietary Information.
 * The information contained herein is confidential property of
 * Renesas Electronics Corporation
 *
 * Copyright (C) Renesas Electronics Corporation 2018 All rights reserved.
 */

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <sys/mman.h>
#include <wfdqnx/wfdcfg.h>
#include <hw/rcar_cmm.h>
#include "cmm_private.h"

char* progname = NULL;
cmm_dev_t *dev;

sem_t g_sem;

mqd_t g_send_que_lut = 0;
mqd_t g_send_que_clu = 0;
mqd_t g_send_que_hgo = 0;
mqd_t g_recv_que_lut = 0;
mqd_t g_recv_que_clu = 0;
mqd_t g_recv_que_hgo = 0;

static uint32_t hdsr, hder;
static bool force_quit = false;
static resmgr_connect_funcs_t	connect_funcs;
static resmgr_io_funcs_t		io_funcs;
static unsigned int g_channel;

/* Originally, g_resolution is used to lookup active video timing and calculate 
 * set value for HDSR and HDER register. Since HDSR and HDER is already set in 
 * DU register when starting screen, we can read HDSR and HDER value from DU */
#define USE_DU_SETTING

#if !defined USE_DU_SETTING

static unsigned int g_resolution;

struct mode {
	const struct wfdcfg_timing timing;
	const struct wfdcfg_keyval *ext;
};

static const struct mode salvatorex_hdmi_modes[] = {
	{
		.timing = {  // 1280x720 @ 60Hz
			.pixel_clock_kHz = 74250,
			.hpixels = 1280, .hfp = 110, .hsw = 40, .hbp = 220, // 1650 total
			.vlines  = 720,  .vfp = 5,   .vsw = 5,  .vbp = 20,  // 750 total
			.flags = 0 ,
		},
	},
	{
		.timing = { // 1920x1080 @ 60Hz
			.pixel_clock_kHz = 148500,
			.hpixels = 1920, .hfp = 88, .hsw = 44, .hbp = 148, // 2200 total
			.vlines  = 1080, .vfp =  4, .vsw =  5, .vbp =  36, // 1125 total
			.flags = WFDCFG_PREFERRED, // Mark this native resolution
		},
	},
	{
		// marks end of list
		.timing = {.pixel_clock_kHz = 0},
	},
};

static const struct mode salvatorex_lvds_modes[] = {
	{
		.timing = {  // 1280x800 @ 60 Hz
			.pixel_clock_kHz = 71107,
			.hpixels = 1280, .hbp = 48, .hsw = 32, .hfp = 80,  // 1440 total
			.vlines  =  800, .vbp =  2, .vsw =  6, .vfp = 15,  //  823 total
			.flags = WFDCFG_PREFERRED, // Mark this native resolution
		},
	},
	{
			// marks end of list
			.timing = {.pixel_clock_kHz = 0},
	},
};

static const struct mode salvatorex_vga_modes[] = {
	{
		.timing = { // 1024x768 @ 60Hz
			.pixel_clock_kHz = 64996,
			.hpixels = 1024, .hfp = 24, .hsw = 136, .hbp = 160, // 1344 total
			.vlines  =  768, .vfp =  3, .vsw =   6, .vbp =  29, //  806 total
			.flags = 0,
		},
	},
	{
		.timing = {  // 1280x720 @ 60 Hz
			.pixel_clock_kHz = 74250,
			.hpixels = 1280, .hfp=110, .hsw= 40, .hbp=220,  // 1650 total
			.vlines  =  720, .vfp=  5, .vsw=  5, .vbp= 20,  //  750 total
			.flags = 0 ,
		},
	},
	/* Common resolution for VGA monitors */
	{
		.timing = {  // 1366x768 @ 60 (59.789) Hz, standard timing
			.pixel_clock_kHz = 85500,
			.hpixels = 1366, .hfp= 70, .hsw=143, .hbp=213,  // 1792 total
			.vlines  =  768, .vfp=  3, .vsw=  3, .vbp= 24,  //  798 total
			.flags = 0 ,
		},
	},
	{
		// marks end of list
		.timing = {.pixel_clock_kHz = 0},
		.ext = NULL,
	},
};
#endif

/*
 * Initialize DUn module
 */
void du_cmm_init(cmm_dev_t *dev)
{
	uint32_t reg;

	if(g_channel == 1 || g_channel == 3)
	{
		// Set DEF7Rm register to activate  CMM channel 1,3.
		ducmm_write_reg(DEF7R_CODE | DEF7R_CMME1, (void*)dev->du_control, DEF7Rm);
	}
	else
	{
		// Set DEF7Rm register to activate  CMM channel 0,2.
		ducmm_write_reg(DEF7R_CODE | DEF7R_CMME0, (void*)dev->du_control, DEF7Rm);
	}

#if defined USE_DU_SETTING
	/* Here we read out HDSR and HDER value from DU register. DU driver set
	 * value for these registers as below:
	 *   HDSR = hsw + hbp - 19
	 *   HDER = hsw + hbp - 19 + hpixels
	 * Following hardware manual, when using the CMM function, 25 should be 
	 * subtracted from both HDSR and HDER
	 */
	hdsr = ducmm_read_reg((void*)dev->du, HDSR);
	hder = ducmm_read_reg((void*)dev->du, HDER);

	ducmm_write_reg(hdsr - 25, (void*)dev->du, HDSR);
	ducmm_write_reg(hder - 25, (void*)dev->du, HDER);
#else
	// HDSR,HDER
	if(g_channel==0)
	{
		// LVDS
		ducmm_write_reg(salvatorex_lvds_modes[g_resolution].timing.hsw+salvatorex_lvds_modes[g_resolution].timing.hbp-19-25,
				(void*)dev->du,	HDSR);
		ducmm_write_reg(salvatorex_lvds_modes[g_resolution].timing.hsw+salvatorex_lvds_modes[g_resolution].timing.hbp-19-25+salvatorex_lvds_modes[0].timing.hpixels,
				(void*)dev->du, HDER);
	}
	else if(g_channel==1 || g_channel==2)
	{
		// HDMI
		ducmm_write_reg(salvatorex_hdmi_modes[g_resolution].timing.hsw+salvatorex_hdmi_modes[g_resolution].timing.hbp-19-25,
				(void*)dev->du, HDSR);
		ducmm_write_reg(salvatorex_hdmi_modes[g_resolution].timing.hsw+salvatorex_hdmi_modes[g_resolution].timing.hbp-19-25+salvatorex_hdmi_modes[1].timing.hpixels,
				(void*)dev->du, HDER);
	}
	else if(g_channel==3)
	{
		// VGA
		ducmm_write_reg(salvatorex_vga_modes[g_resolution].timing.hsw+salvatorex_vga_modes[g_resolution].timing.hbp-19-25,
				(void*)dev->du, HDSR);
		ducmm_write_reg(salvatorex_vga_modes[g_resolution].timing.hsw+salvatorex_vga_modes[g_resolution].timing.hbp-19-25+salvatorex_vga_modes[g_resolution].timing.hpixels,
				(void*)dev->du, HDER);
	}
#endif

	// Display operation stop
	reg = ducmm_read_reg((void*)dev->du_control, DSYSR0);
	reg &= ~(DEN);
	reg |= (DRES);
	ducmm_write_reg(reg, (void*)dev->du_control, DSYSR0);
	//
	// CMM initial setting (This sample application operates in single buffer mode)
	//
	unsigned int cmm_diable;
	switch(g_channel)
	{
		case 0:
			cmm_diable = CMM0_DISABLE;
			break;
		case 1:
			cmm_diable = CMM1_DISABLE;
			break;
		case 2:
			cmm_diable = CMM2_DISABLE;
			break;
		case 3:
			cmm_diable = CMM3_DISABLE;
			break;
		default:
			cmm_diable = CMM1_DISABLE;
			break;
	}

	// Release from the module stand-by state
	// RMSTPCR7
	reg = ducmm_read_reg((void*)dev->cpg, 0);
	reg &= ~cmm_diable;
	ducmm_write_reg(reg, (void*)dev->cpg, 0);
	// SMSTPCR7
	reg = ducmm_read_reg((void*)dev->cpg, 0x20);
	reg &= ~cmm_diable;
	ducmm_write_reg(reg, (void*)dev->cpg, 0x20);

	// Display synchronous operation starts(DRES=0, DEN=1)
	reg = ducmm_read_reg((void*)dev->du_control, DSYSR0);
	reg &= ~(DRES);
	reg |= (DEN);
	ducmm_write_reg(reg, (void*)dev->du_control, DSYSR0);
	reg = ducmm_read_reg((void*)dev->cmm, CM2_CTL0);

	// CLU, LUT and HGO are double buffer
	reg |= DBUF | CLUDB;
	// Histogram data is measured before LUT processing.
	reg |= HISTS;
	/* Experiments show that, set correct vsync polarity 
	 * help CM2_CTL1.BFS toggled as expected
	 */
	if(ducmm_read_reg((void*)dev->du, DSMR) & (1 << 18)) {
		reg |= (1 << 4);
	}
	ducmm_write_reg(reg, (void*)dev->cmm, CM2_CTL0);
	// resets all HGO read-only registers
	ducmm_write_reg(HGO_RESET, (void*)dev->cmm, CM2_HGO_REGRST);

	// Enable VBK interrupt
	/* VBK interrupt is already enabled when screen start */
#if 0
	ducmm_write_reg(VBCL, (void*)dev->du, DSRCR);
	reg = ducmm_read_reg((void*)dev->du, DIER);
	reg |= VBK_ENABLE;
	ducmm_write_reg(reg, (void*)dev->du, DIER);
#endif

	dev->active = 0;
}

const struct sigevent* intr_vsync_handler (void *arg, int id)
{
	uint32_t reg;

	reg = ducmm_read_reg((void*)dev->du, DSSR);
	if (!(reg & (1<<11))) {
		/* VBK flag is 0 */
		return NULL;
	}
	// Clear the VBK flag
	/* Drop fps is observed on D3 when clear VBK flag here. 
	 * Since CMM's vsync irq handler is called before DU's 
	 * vsync irq handler, clear VBK flag here might be cause 
	 * missing vsync in DU driver
	 */
	// ducmm_write_reg(VBCL, (void*)dev->du, DSRCR);

	return dev->g_event;
}

void signal_handler(int signum)
{
	switch(signum) {
		case SIGINT:
		case SIGQUIT:
		case SIGTERM:
		case SIGKILL:
		case SIGSEGV:
			force_quit = true;
			break;
		default:
			break;
	}
}

void *start_thread_pool(void *arg)
{
	/* Start the threads. This function doesn't return. */
	thread_pool_start((thread_pool_t*)arg);

	return NULL;
}

int main(int argc, char **argv)
{
	thread_pool_attr_t	pool_attr;
	resmgr_attr_t resmgr_attr;
	thread_pool_t	*tpp;
	struct mq_attr attr;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 100;
	attr.mq_msgsize = 2000;
	int pshared = 0;
	int value = 0;
	int du_irq;
	uint64_t du_addr, cmm_addr;

	g_channel = 1;
#if !defined USE_DU_SETTING
	g_resolution = 1;
#endif
	while( *++argv != NULL ){
		if( **argv == '-' )
		{
			switch( *(*argv+1) )
			{
				case 'c':	/* DU channel */
					g_channel = atoi((*argv+3));
				break;
#if !defined USE_DU_SETTING
				case 'r':	/* display resolution */
					g_resolution = atoi((*argv+3));
				break;
#endif
			}
		}
	}
	switch(g_channel)
	{
	case 1:
		du_addr = DU1_CR_BASEADDRESS;
		du_irq = DU1_INTID;
		cmm_addr = CMM1_BASEADDRESS;
		break;
	case 2:
		du_addr = DU2_CR_BASEADDRESS;
		du_irq = DU2_INTID;
		cmm_addr = CMM2_BASEADDRESS;
		break;
	case 3:
		du_addr = DU3_CR_BASEADDRESS;
		du_irq = DU3_INTID;
		cmm_addr = CMM3_BASEADDRESS;
		break;
	default:
		du_addr = DU0_CR_BASEADDRESS;
		du_irq = DU0_INTID;
		cmm_addr = CMM0_BASEADDRESS;
		break;
	}

	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0))
	{
		cmm_slogf(VERBOSE_QUIET,"%s: ThreadCtl failed: %s\n", progname, strerror(errno));
		return EXIT_FAILURE;
	}
	
	// initialize semaphore
	if(sem_init(&g_sem, pshared, value) != 0)
	{
		return EXIT_FAILURE;
	}

	dev=(cmm_dev_t *) malloc(sizeof(cmm_dev_t ));
	if (dev == NULL)
	{
		cmm_slogf(VERBOSE_QUIET,"%s:  couldn't malloc: %s\n",progname, strerror (errno));
		return EXIT_FAILURE;
	}
	
	// allocate and initialize a dispatch structure for use by our main loop
	dev->dpp = dispatch_create();
	if (dev->dpp == NULL)
	{
		cmm_slogf(VERBOSE_QUIET,"%s:  couldn't dispatch_create: %s\n",progname, strerror (errno));
		free(dev);
		return EXIT_FAILURE;
	}

	/*
	 * Set up the resource manager attributes structure, we'll
	 * use this as a way of passing information to resmgr_attach().
	 * For now, we just use defaults.
	 */
	
	memset(&resmgr_attr, 0, sizeof resmgr_attr);
	resmgr_attr.msg_max_size = MAX_MSG_SIZE;

	/*
	 * Initialize the connect functions and I/O functions tables to
	 * their defaults and then override the defaults with the
	 * functions that we are providing.
	 */
	
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);

	// use our own devctl
	io_funcs.devctl = cmm_devctl;
	
	iofunc_attr_init(&dev->att, S_IFCHR | 0666, 0, 0);

	/* attach our device name */
	dev->id = resmgr_attach(
			dev->dpp,		/* dispatch handle			*/
			&resmgr_attr,	/* resource manager attrs	*/
			CMM_DEV_NAME,		/* device name				*/
			_FTYPE_ANY,		/* open type				*/
			0,				/* flags					*/
			&connect_funcs,	/* connect routines			*/
			&io_funcs,		/* I/O routines				*/
			&dev->att);		/* handle					*/
	if(dev->id == -1) {
		cmm_slogf(VERBOSE_QUIET,"%s:  resmgr_attach failure %s\n", progname, strerror(errno));
		free(dev);
		return EXIT_FAILURE;
	}
	
	/* register mapping for CPG */
	if ((dev->cpg = mmap_device_io(0x200, RMSTPCR7)) == -1) {
		cmm_slogf(VERBOSE_QUIET,"%s:  couldn't mapping for ths 0: %s\n", __FUNCTION__, strerror(errno));
		free(dev);
		return EXIT_FAILURE;
	}

	if(g_channel == 0 || g_channel == 1)
	{
		/* register mapping for DU0 */
		if ((dev->du_control = mmap_device_io(DU_REGSIZE, DU0_CR_BASEADDRESS)) == -1) {
			cmm_slogf(VERBOSE_QUIET,"%s:  couldn't mapping for ths 0: %s\n", __FUNCTION__, strerror(errno));
			free(dev);
			return EXIT_FAILURE;
		}
	}
	else
	{
		/* register mapping for DU2 */
		if ((dev->du_control = mmap_device_io(DU_REGSIZE, DU2_CR_BASEADDRESS)) == -1) {
			cmm_slogf(VERBOSE_QUIET,"%s:  couldn't mapping for ths 0: %s\n", __FUNCTION__, strerror(errno));
			free(dev);
			return EXIT_FAILURE;
		}
	}
	
	if(g_channel == 1 || g_channel == 3)
	{
		/* register mapping for DU1 or DU3 */
		if ((dev->du = mmap_device_io(DU_REGSIZE, du_addr)) == -1) {
			cmm_slogf(VERBOSE_QUIET,"%s:  couldn't mapping for ths 0: %s\n", __FUNCTION__, strerror(errno));
			free(dev);
			return EXIT_FAILURE;
		}
	}
	else
	{
		// DU0 or DU2
		dev->du = dev->du_control;
	}

	/* register mapping for CMMn */
	if ((dev->cmm = mmap_device_io(CMM_REGSIZE, cmm_addr)) == -1) {
		cmm_slogf(VERBOSE_QUIET,"%s:  couldn't mapping for ths 0: %s\n", __FUNCTION__, strerror(errno));
		free(dev);
		return EXIT_FAILURE;
	}

	du_cmm_init(dev);

	dev->g_event=(struct sigevent *) malloc(sizeof(struct sigevent ));
	if (dev->g_event == NULL)
	{
		cmm_slogf(VERBOSE_QUIET,"%s:  couldn't malloc: %s\n",progname, strerror (errno));
		free(dev);
		return EXIT_FAILURE;
	}

	// Attach an interrupt handler to an DU0 interrupt
	if( -1 == InterruptAttach( du_irq, &intr_vsync_handler, NULL, 0, _NTO_INTR_FLAGS_PROCESS/*_NTO_INTR_FLAGS_END*/))
	{
		cmm_slogf(VERBOSE_QUIET,"%s:  InterruptAttach failure %s\n", progname, strerror(errno));
		free(dev->g_event);
		free(dev);
		return EXIT_FAILURE;
	}

	dev->chid = ChannelCreate(0);
	dev->irqcoid = ConnectAttach(0, 0, dev->chid, _NTO_SIDE_CHANNEL, 0);
	SIGEV_PULSE_INIT(dev->g_event, dev->irqcoid, SIGEV_PULSE_PRIO_INHERIT, _PULSE_CODE_MINAVAIL , 0);

	/* initialize thread pool attributes */
	memset(&pool_attr, 0, sizeof pool_attr);
	pool_attr.handle = dev->dpp;
	pool_attr.context_alloc = (void *)dispatch_context_alloc;
	pool_attr.block_func = (void *)dispatch_block;
	pool_attr.unblock_func = (void *)dispatch_unblock;
	pool_attr.handler_func = (void *)dispatch_handler;
	pool_attr.context_free = (void *)dispatch_context_free;
	pool_attr.lo_water = 2;
	pool_attr.hi_water = 4;
	pool_attr.increment = 1;
	pool_attr.maximum = 50;
	
	/* allocate a thread pool handle */
	if((tpp = thread_pool_create(&pool_attr, POOL_FLAG_EXIT_SELF)) == NULL) {
		free(dev->g_event);
		free(dev);
		return EXIT_FAILURE;
	}

	// create message queue for lut
	g_send_que_lut = mq_open("/mq_du_cmm_lut", O_CREAT|O_WRONLY,S_IRUSR | S_IWUSR ,&attr);
	mq_getattr(g_send_que_lut, &attr);
	// create message queue for clut
	g_send_que_clu = mq_open("/mq_du_cmm_clu", O_CREAT|O_WRONLY,S_IRUSR | S_IWUSR ,&attr);
	mq_getattr(g_send_que_clu, &attr);
	// create message queue for hgo
	g_send_que_hgo = mq_open("/mq_du_cmm_hgo", O_CREAT|O_WRONLY,S_IRUSR | S_IWUSR ,&attr);
	mq_getattr(g_send_que_hgo, &attr);

	/* Start the thread that will handle interrupt events. */
	pthread_t sync_tid, pool_tid = 0;
	pthread_create (&sync_tid, NULL, vsync_handler, NULL);

	/* thread_pool_start() will not return to main thread, But we need to 
	 * restore HDSR and HDER when driver terminated, so we need to start 
	 * thread pool in separate thread 
	 */
	pthread_create (&pool_tid, NULL, start_thread_pool, tpp);

	signal(SIGINT,  signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGKILL, signal_handler);
	signal(SIGSEGV, signal_handler);

	while(!force_quit) {
		sleep(1);
	}

	/* Restore HDSR, HDER */
	ducmm_write_reg(hdsr, (void*)dev->du, HDSR);
	ducmm_write_reg(hder, (void*)dev->du, HDER);

	munmap_device_io(dev->cpg, 0x200);
	munmap_device_io(dev->du_control, DU_REGSIZE);
	munmap_device_io(dev->du, DU_REGSIZE);
	munmap_device_io(dev->cmm, CMM_REGSIZE);

	free(dev->g_event);
	free(dev);

	return EXIT_SUCCESS;
}


 
