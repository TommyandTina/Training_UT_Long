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


#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <atomic.h>
#include "rcar-flexray.h"
#include "flexray-regs.h"


/** 
 * @brief Get number of FIFO buffers filled with new data.
 * @param dev: Pointer to struct fr_dev_t.
 * @param rffl: Number of FIFO buffers filled.
 * @return none
 */
void fr_get_fifo_fill_level(fr_dev_t *dev, uint8_t *rffl)
{
	fr_regs_t *regs = &dev->regs;
	*rffl = (uint8_t)(FR_REG_BITS(regs->msg_buf_stat.frfsr,FR_FSR_RFFL_L,FR_FSR_RFFL_H));
	return ;
}

/** 
 * @brief Check new data available in message ram.
 * @param dev: Pointer to struct fr_dev_t.
 * @param idx: The index of message buffer.
 * @return 0: success, -1: error
 */
int fr_check_data(fr_dev_t *dev,uint8_t idx)
{
	fr_regs_t *regs = &dev->regs;
	uint8_t 	i = 0;
	uint32_t 	mask = 0;
	uint32_t 	frndat = 0;
	
	i = idx/32;
	if(i>3){
		i = 3;
	}
	mask = 1 << (idx % 32);
	frndat = in32(regs->msg_buf_stat.frndat[i]);
	if(frndat & mask){
		return 0;
	}
	return -1;
}

void fr_read_header(fr_regs_t *regs, fr_msg_buffer_rx_t *msg)
{
	uint32_t header1 = in32(regs->out_buf.frrdhs1);
	uint32_t header2 = in32(regs->out_buf.frrdhs2);
	uint32_t header3 = in32(regs->out_buf.frrdhs3);
	
	msg->frameId 		= header1 & FR_RDHS1_FID_MASK; 
	msg->cycleCode 		= (header1 >> FR_RDHS1_CYC_L) & FR_RDHS1_CYC_MASK;
	msg->channel 		= (header3 >> FR_RDHS3_RCI) & FR_RDHS3_MASK_ONE;
	msg->headerCrc 		= (header2 >> FR_RDHS2_CRC_L) & FR_RDHS2_CRC_MASK;
	msg->payloadLength 	= ((header2 >> FR_RDHS2_PLR_L) & FR_RDHS_PLC_MASK)*2;
	msg->cycleCount 	= (header3 >> FR_RDHS3_RCC_L) & FR_RDHS3_RCC_MASK;
	msg->startup 		= (header3 >> FR_RDHS3_SFI) & FR_RDHS3_MASK_ONE;
	msg->sync 			= (header3 >> FR_RDHS3_SYN) & FR_RDHS3_MASK_ONE;
	msg->nullFrame 		= (header3 >> FR_RDHS3_NFI) & FR_RDHS3_MASK_ONE;
	msg->ppi 			= (header3 >> FR_RDHS3_PPI) & FR_RDHS3_MASK_ONE;
}

void fr_read_data(fr_dev_t *dev, uint8_t *payload, uint16_t data_len)
{
	fr_regs_t *regs = &dev->regs;
	uint16_t length = (data_len + 3) / 4;
	uint8_t  index = 0;
	uint32_t data = 0;

	if(data_len > FR_MAX_PAYLOAD){
		data_len = FR_MAX_PAYLOAD;
	}
	for(index = 0; index < length; index++){
		data= in32(regs->out_buf.frrdds[index]);
		*((uint32_t*)(payload + index*4)) = data;
	}			
}

/** 
 * @brief Gets number of message buffer currently accessible by the Host.
 * @param dev: Pointer to struct fr_dev_t.
 * @return Index of message buffer 
 */
uint8_t fr_get_output_buffer(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	uint8_t frobcr_obrh = 0;
	frobcr_obrh = FR_REG_BITS(regs->out_buf.frobcr,FR_OBCR_OBRH_L,FR_OBCR_OBRH_H);
	frobcr_obrh = frobcr_obrh & (FR_MAX_BUF -1);
	return frobcr_obrh;
}


/** 
 * @brief Set output buffer command mask.
 * @param dev: Pointer to struct fr_dev_t.
 * @param msg: Pointer to struct fr_msg_buffer_rx_t.
 * @return none
 */
void fr_set_buffer_command_mask(fr_dev_t *dev, fr_msg_buffer_rx_t *msg)
{
	fr_regs_t *regs = &dev->regs;
	uint32_t frobcm = 0;

	//whether header section is received from message buffer or not 
	if(msg->cmdMask & FR_CMD_MASK_HEADER){
		frobcm |= (1<<FR_OBCM_RHSS);
	} else {
		frobcm &= ~(1<<FR_OBCM_RHSS);
	}
	//whether data section is received from message buffer or not
	if(msg->cmdMask & FR_CMD_MASK_DATA){
		frobcm |= (1<<FR_OBCM_RDSS);
	} else {
		frobcm &= ~(1<<FR_OBCM_RDSS);
	}
	out32(regs->out_buf.frobcm,frobcm);	
}

/** 
 * @brief Set output buffer command request.
 * @param dev: Pointer to struct fr_dev_t.
 * @param msg: Pointer to struct fr_msg_buffer_rx_t.
 * @param idx: The index of message buffer.
 * @return 0: success, ETIMEDOUT: error
 */
int fr_set_buffer_command_request(fr_dev_t *dev, fr_msg_buffer_rx_t *msg, uint8_t idx)
{
	fr_regs_t *regs = &dev->regs;
	uint32_t frobcr = 0;
	
	//Sets buffer number is received in message ram
	frobcr = (idx<<FR_OBCR_OBRS_L) & FR_OBCR_OBRS_MASK;
	out32(regs->out_buf.frobcr,frobcr);
	//Requests to receive message buffer in message ram
	if(msg->cmdMask & FR_CMD_MASK_REQUEST){
		frobcr |= (1<<FR_OBCR_REQ);
	} else {
		frobcr &= ~(1<<FR_OBCR_REQ);
	}
	out32(regs->out_buf.frobcr,frobcr);
	// Wait FROBCR.OBSYS is cleared
	if(fr_wait_outputbuffer_busy(dev)){
		return ETIMEDOUT;
	}
	frobcr = 0;
	
	frobcr |= (1<<FR_OBCR_VIEW);
	
	out32(regs->out_buf.frobcr,frobcr);
	
	return 0;	
}

/** 
 * @brief Read message buffer in message RAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @param pmsg: Pointer to struct fr_msg_buffer_rx_t.
 * @param idx: The index of message buffer.
 * @return 0: success, ETIMEDOUT: error
 */
int fr_rx_msg_ram(fr_dev_t *dev, fr_msg_buffer_rx_t *pmsg)
{
	fr_regs_t *regs = &dev->regs;
	uint8_t frobcr_obrh = 0;
	uint8_t payloadLength = 0;
	uint32_t timeout = 0;
	
	if(fr_wait_outputbuffer_busy(dev)){
		return ETIMEDOUT;
	}			
	fr_set_buffer_command_mask(dev,pmsg);
	if(fr_set_buffer_command_request(dev,pmsg,pmsg->idx)){
		return ETIMEDOUT;
	}
	while(1){
		frobcr_obrh = fr_get_output_buffer(dev);
		if(frobcr_obrh == pmsg->idx){
			break;
		}
		if(timeout>FR_WAIT_CYCLE){
			return ETIMEDOUT;
		}
		timeout++;
	}
	fr_read_header(regs,pmsg);
	payloadLength = pmsg->payloadLength;
	fr_read_data(dev,pmsg->data,payloadLength);

	return 0;
}

/** 
 * @brief Read fifo message buffer in message RAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @param msg: Pointer to struct fr_msg_buffer_rx_t.
 * @param idx: The index of fifo message buffer.
 * @return 0: success, ETIMEDOUT: error
 */
int fr_rx_fifo_msg_ram(fr_dev_t *dev, fr_msg_buffer_rx_t *msg,uint8_t idx)
{
	fr_regs_t *regs = &dev->regs;
	uint8_t frobcr_obrh = 0;
	uint8_t payloadLength = 0;
	uint32_t timeout = 0;
	
	if(fr_wait_outputbuffer_busy(dev)){
		return ETIMEDOUT;
	}			
	fr_set_buffer_command_mask(dev,msg);
	if(fr_set_buffer_command_request(dev,msg,idx)){
		return ETIMEDOUT;
	}
	while(1){
		frobcr_obrh = fr_get_output_buffer(dev);
		if(frobcr_obrh == idx){
			break;
		}
		if(timeout>FR_WAIT_CYCLE){
			return ETIMEDOUT;
		}
		timeout++;
	}
	fr_read_header(regs,msg);
	payloadLength = msg->payloadLength;
	fr_read_data(dev,msg->data,payloadLength);

	return 0;
}

/** 
 * @brief Get message buffers in message RAM.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: Pointer to struct io_devctl_t.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @return 0:success, !=0 :error
 */
int rcar_rx_msg_ram(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	fr_regs_t *regs = &dev->regs;
	fr_poc_state pocState = 0;
	uint8_t *buf;
	fr_msg_buffer_rx_t *pmsg = NULL;
	
    if (msg->i.nbytes <= 0) {
        _IO_SET_READ_NBYTES(ctp, 0);
        return _RESMGR_NPARTS(0);
    }
	/* 	53.2.13.2 FROTS
	* 	(11) FROTS.OTS
		While this bit is '1', there should be no read or write access to the address area ERAY+h'0600 to ERAY+h'07FF.
	*/
	if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) == FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: user ram transfer is operating", __func__);
		return EINVAL;
	}
	fr_get_pocs_state(regs,&pocState);
	if(pocState != FR_STATE_NORMAL_ACTIVE){
		fr_slogf(FR_SLOG_DEBUG1,"%s: FlexRay is not in STATE_NORMAL_ACTIVE", __func__);
		return EIO;
	}
	buf = (uint8_t *)msg;
	buf += sizeof(io_devctl_t);
	pmsg = (fr_msg_buffer_rx_t*)buf;
	if(pmsg->type == FR_MSGBUF_TYPE_TX){
		return ENOSYS;
	}
	return  fr_rx_replay_data(dev,ctp,msg,ocb->ioflag);	
}
