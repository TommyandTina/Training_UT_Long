/*
 * Renesas Proprietary Information.
 * The information contained herein is confidential property of
 * Renesas Electronics Corporation
 *
 * Copyright (C) Renesas Electronics Corporation 2018 All rights reserved.
 */

/*
 * cmm_devctl.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <devctl.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/mman.h>
#include <hw/rcar_cmm.h>
#include "cmm_private.h"


static pthread_mutex_t g_csIOCTL= PTHREAD_MUTEX_INITIALIZER;

int cmm_get_hisogram(cmm_data_addr_t *data_hgo)
{
	cmm_hgo_virt_t hgo_virt;
	uint32_t i, j;

	const uint32_t histo_offset[3] = {
		CMM_HGO_R_HISTO(0),
		CMM_HGO_G_HISTO(0),
		CMM_HGO_B_HISTO(0),
	};

	hgo_virt.hgo_length = data_hgo->length;
	hgo_virt.hgo = mmap_device_memory( NULL,data_hgo->length,PROT_WRITE|PROT_NOCACHE,0,(_Uint64t)data_hgo->data);

	if(ducmm_read_reg((void*)dev->cmm, CM2_HGO_MODE) & 1 << 7)
	{
		// MAXRGB mode
		uint32_t* dst = hgo_virt.hgo;
		dst = dst + CMM_HGO_NUM;
		for(i=0; i < CMM_HGO_NUM; i++)
		{
			dst[i] = ducmm_read_reg((void*)dev->cmm, histo_offset[1] + i * 4);
		}
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			uint32_t *hgo_buf = hgo_virt.hgo + CMM_HGO_NUM * 4 * i;
			for (j = 0; j < CMM_HGO_NUM; j++)
				hgo_buf[j] = ducmm_read_reg((void*)dev->cmm, histo_offset[i] + j * 4);
		}
	}

	// resets all HGO read-only registers
	ducmm_write_reg(HGO_RESET, (void*)dev->cmm, CM2_HGO_REGRST);

	munmap_device_memory(hgo_virt.hgo, data_hgo->length);

	sem_post(&g_sem);

	return 0;
}

int cmm_set_hisogramInfo(cmm_hgo_info_t *data)
{
	uint32_t reg;

	// set Histogram selector
	reg = ducmm_read_reg((void*)dev->cmm, CM2_CTL0);
	reg &= ~CMM_CTL0_HISTS;
	reg |= data->hists << 20;
	ducmm_write_reg(reg, (void*)dev->cmm, CM2_CTL0);

	// set HGO mode
	reg = data->vratio;
	reg |=  data->hratio << 2;
	reg |=  data->ofsb_b << 4;
	reg |=  data->ofsb_g << 5;
	reg |=  data->ofsb_r << 6;
	reg |=  data->maxrgb << 7;
	ducmm_write_reg(reg, (void*)dev->cmm, CM2_HGO_MODE);

	// set HGO offset
	ducmm_write_reg(data->hoffset << 16 | data->voffset, (void*)dev->cmm, CM2_HGO_OFFSET);

	// setHGO size
	ducmm_write_reg(data->hsize << 16 | data->vsize, (void*)dev->cmm, CM2_HGO_SIZE);

	return 0;
}

int cmm_set_lut(cmm_data_addr_t *data_lut)
{
	cmm_lut_virt_t lut_virt;
	uint32_t offset_lut;

	lut_virt.lut_length = data_lut->length;
	lut_virt.lut = mmap_device_memory( NULL,data_lut->length,PROT_READ|PROT_NOCACHE,0,(_Uint64t)data_lut->data);

	// Set LUT
	uint32_t*  plut = (uint32_t*)(lut_virt.lut);
	if(ducmm_read_reg((void*)dev->cmm, CM2_CTL1) & BFS)
	{
		// use side A table
		offset_lut = CMM_LUT_TBLA(0);
	}
	else
	{
		// use side B table
		offset_lut = CMM_LUT_TBLB(0);
	}

	for(int i=0; i < CMM_LUT_NUM; i++)
	{
		// write lookup data
		ducmm_write_reg(*(plut+ i), (void*)dev->cmm, offset_lut + 4 * i);
	}

	munmap_device_memory(lut_virt.lut, data_lut->length);

	sem_post(&g_sem);
	return 0;
}

int cmm_set_clu(cmm_data_addr_t *data_lut)
{
	uint32_t offset_clu;
	cmm_lut_virt_t lut_virt;

	//
	// CMM setting (double buffer mode)
	//
	lut_virt.lut_length = data_lut->length;
	lut_virt.lut = mmap_device_memory( NULL,data_lut->length,PROT_READ|PROT_NOCACHE,0,(_Uint64t)data_lut->data);

	if(ducmm_read_reg((void*)dev->cmm, CM2_CTL1) & BFS)
	{
		// use side A table
		offset_clu = CM2_CLU_ADDRESS;
	}
	else
	{
		// use side B table
		offset_clu = CM2_CLU_ADDR2;
	}

	uint32_t*  pclu = (uint32_t*)(lut_virt.lut);
	for (int i = 0; i < CMM_CLU_NUM; i++)
	{
		ducmm_write_reg(*(pclu+ i), (void*)dev->cmm, offset_clu + 4);
	}

	munmap_device_memory(lut_virt.lut, data_lut->length);
	sem_post(&g_sem);

	return 0;
}

/* create default CLU table data */
static inline uint32_t index_to_clu_data(int index)
{
	int r, g, b;

	r = index % 17;
	index /= 17;
	g = index % 17;
	index /= 17;
	b = index % 17;

	r = (r << 20);
	if (r > (255 << 16))
		r = (255 << 16);
	g = (g << 12);
	if (g > (255 << 8))
		g = (255 << 8);
	b = (b << 4);
	if (b > (255 << 0))
		b = (255 << 0);

	return r | g | b;
}

int32_t cmm_set_config(unsigned char* config)
{
	int32_t err = 0;
	uint32_t cm2_ctl0 = 0;

	switch (*config) {
	case CSC_CONVERT_NONE:
		break;
	case CSC_CONVERT_BT601_YCbCr240:
		cm2_ctl0 |= (CMM_CTL0_TM_BT601_YC240 | CMM_CTL0_YC);
		break;
	case CSC_CONVERT_BT601_YCbCr255:
		cm2_ctl0 |= (CMM_CTL0_TM_BT601_YC255 | CMM_CTL0_YC);
		break;
	case CSC_CONVERT_BT709_RGB255:
		cm2_ctl0 |= (CMM_CTL0_TM_BT709_RG255 | CMM_CTL0_YC);
		break;
	case CSC_CONVERT_BT709_RGB235:
		cm2_ctl0 |= (CMM_CTL0_TM_BT709_RG235 | CMM_CTL0_YC);
		break;

	default:
		err = -EINVAL;
	}

	if(!err)
		ducmm_write_reg(cm2_ctl0 | DBUF | CLUDB, (void*)dev->cmm, CM2_CTL0);

	return err;
}

void cmm_start_stop(unsigned char* on)
{
	uint32_t table_data;

	if(*on==1)
	{
		// initialize letter box
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB_TH);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB0_H);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB0_V);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB1_H);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB1_V);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB2_H);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB2_V);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB3_H);
		ducmm_write_reg(0, (void*)dev->cmm, CMM_HGO_LB3_V);

		// initialize color table
		for (int i = 0; i < CMM_LUT_NUM; i++) {
			table_data = ((i << 16) | (i << 8) | (i << 0));
			ducmm_write_reg(table_data, (void*)dev->cmm, CMM_LUT_TBLA(i));
			ducmm_write_reg(table_data, (void*)dev->cmm, CMM_LUT_TBLB(i));
		}

		ducmm_write_reg(AAI | MVS, (void*)dev->cmm, CM2_CLU_CTRL);

		ducmm_write_reg(0, (void*)dev->cmm, CMM_CLU_ADDR);
		ducmm_write_reg(0, (void*)dev->cmm, CM2_CLU_ADDR2);

		for (int i = 0; i < CMM_CLU_NUM; i++) {
			table_data = index_to_clu_data(i);
			ducmm_write_reg(table_data, (void*)dev->cmm, CMM_CLU_DATA);
			ducmm_write_reg(table_data, (void*)dev->cmm, CM2_CLU_DATA2);
		}

		ducmm_write_reg(AAI | MVS | CLU_EN, (void*)dev->cmm, CM2_CLU_CTRL);
		ducmm_write_reg(LUT_EN, (void*)dev->cmm, CM2_LUT_CTRL);

		dev->active = 1;
	}
	else
	{
		ducmm_write_reg(0x00000000, (void*)dev->cmm, CM2_LUT_CTRL);
		ducmm_write_reg(0x00000000, (void*)dev->cmm, CM2_CLU_CTRL);

		dev->active = 0;
	}
}

void* vsync_handler (void *arg)
{
	cmm_data_addr_t data_lut;
	cmm_data_addr_t data_hgo;
	struct _pulse   msg;

	struct mq_attr attr;
	attr.mq_curmsgs = 0;
	attr.mq_flags = O_NONBLOCK;
	attr.mq_maxmsg = 100;
	attr.mq_msgsize = sizeof(cmm_data_addr_t)+10;

	g_recv_que_lut = mq_open("/mq_du_cmm_lut", O_CREAT|O_RDONLY|O_NONBLOCK,S_IRUSR | S_IWUSR ,&attr);
	mq_getattr(g_recv_que_lut, &attr);
	g_recv_que_clu = mq_open("/mq_du_cmm_clu", O_CREAT|O_RDONLY|O_NONBLOCK,S_IRUSR | S_IWUSR ,&attr);
	mq_getattr(g_recv_que_clu, &attr);
	g_recv_que_hgo = mq_open("/mq_du_cmm_hgo", O_CREAT|O_RDONLY|O_NONBLOCK,S_IRUSR | S_IWUSR ,&attr);
	mq_getattr(g_recv_que_hgo, &attr);

	/* Obtain I/O privileges */
	ThreadCtl( _NTO_TCTL_IO, 0 );

	while (1) {
		// wait for vsync interrupt
		if(MsgReceivePulse(dev->chid, &msg, sizeof(msg), NULL) == -1)
		{
			break;
		}
		if (!dev->active)
			// cmm is not activated
			continue;

		if(-1 != mq_receive(g_recv_que_lut, (char*)&data_lut, attr.mq_msgsize, NULL))
		{
			cmm_set_lut(&data_lut);
		}
		if(-1 != mq_receive(g_recv_que_clu, (char*)&data_lut, attr.mq_msgsize, NULL))
		{
			cmm_set_clu(&data_lut);
		}
		if(-1 != mq_receive(g_recv_que_hgo, (char*)&data_hgo, attr.mq_msgsize, NULL))
		{
			cmm_get_hisogram(&data_hgo);
		}
	}
	return NULL;
}

int cmm_devctl (resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	int	nbytes, status;
	cmm_data_addr_t *data_lut;
	cmm_data_addr_t *data_hgo;
	cmm_hgo_info_t *data_hgoinfo;
	unsigned char *cmm_on, *cmm_config;

	/* Obtain I/O privileges */
	ThreadCtl( _NTO_TCTL_IO, 0 );

	if ((status = iofunc_devctl_default(ctp, msg, ocb)) !=
		_RESMGR_DEFAULT) {
		return(status);
	}

	switch (msg -> i.dcmd) {
	case DCMD_CMM_SET_LUT:
		// set LUT
		pthread_mutex_lock(&g_csIOCTL);
		/* Get the data from the message. */
		data_lut = _DEVCTL_DATA (msg->i);
		pthread_mutex_unlock(&g_csIOCTL);

		if(mq_send(g_send_que_lut, (const char*)data_lut, sizeof(cmm_data_addr_t), 0) == -1)
		{
			cmm_slogf(0,"mq_send error errno=%d\n",errno);
		}
		nbytes = 0;
		break;
	case DCMD_CMM_SET_CLU:
		// set CLU
		pthread_mutex_lock(&g_csIOCTL);
		/* Get the data from the message. */
		data_lut = _DEVCTL_DATA (msg->i);
		pthread_mutex_unlock(&g_csIOCTL);

		if(mq_send(g_send_que_clu, (const char*)data_lut, sizeof(cmm_data_addr_t), 0) == -1)
		{
			cmm_slogf(0,"mq_send errorno=%d\n",errno);
		}
		nbytes = 0;
		break;
	case DCMD_CMM_SET_HGOINFO:
		// set HGO Info
		pthread_mutex_lock(&g_csIOCTL);
		data_hgoinfo = _DEVCTL_DATA (msg->i);
		cmm_set_hisogramInfo(data_hgoinfo);
		pthread_mutex_unlock(&g_csIOCTL);
		nbytes = 0;
		break;
	case DCMD_CMM_GET_HGO:
		// get HGO
		pthread_mutex_lock(&g_csIOCTL);
		data_hgo = _DEVCTL_DATA (msg->i);
		pthread_mutex_unlock(&g_csIOCTL);

		if(mq_send(g_send_que_hgo, (const char*)data_hgo, sizeof(cmm_data_addr_t), 0) == -1)
		{
			cmm_slogf(0,"mq_send errorno=%d\n",errno);
		}
		nbytes = 0;
		break;
	case DCMD_CMM_WAIT_CMD:
		// Wait for completion of lut/clu setting or histogram acquisition
		sem_wait(&g_sem);
		nbytes = 0;
		break;
	case DCMD_CMM_START_HGO:
		// start HGO
		pthread_mutex_lock(&g_csIOCTL);
		// resets all HGO read-only registers
		ducmm_write_reg(HGO_RESET, (void*)dev->cmm, CM2_HGO_REGRST);
		pthread_mutex_unlock(&g_csIOCTL);
		nbytes = 0;
		break;
	case DCMD_CMM_START_STOP:
		// start or stop cmm
		pthread_mutex_lock(&g_csIOCTL);
		cmm_on = _DEVCTL_DATA (msg->i);
		cmm_start_stop(cmm_on);
		pthread_mutex_unlock(&g_csIOCTL);
		nbytes = 0;
		break;
	case DCMD_CMM_CONFIG:
		// set cmm configuration(CSC)
		pthread_mutex_lock(&g_csIOCTL);
		cmm_config = _DEVCTL_DATA (msg->i);
		cmm_set_config(cmm_config);
		pthread_mutex_unlock(&g_csIOCTL);
		nbytes = 0;
		break;

	default:
		return (ENOSYS);
	}


	/* Clear the return message. */
	memset(&msg->o, 0, sizeof(msg->o));

	/* Indicate the number of bytes and return the message */
	msg->o.nbytes = nbytes;
	return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o) + nbytes));
}
