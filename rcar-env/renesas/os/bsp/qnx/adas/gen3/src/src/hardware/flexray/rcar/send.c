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

#include <sys/mman.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <atomic.h>
#include "rcar-flexray.h"
#include "flexray-regs.h"

/** 
 * @brief Set header for URAM
 * @param dev: Pointer to fr_dev_t structure.
 * @param header: Pointer to fr_input_header_t structure.
 * @param msg: Pointer to fr_msg_ram_t structure.
 * @return 0:success, EINVAL, ETIMEDOUT: error
 */
int fr_set_header_uram(fr_dev_t *dev,fr_input_header_t *ph,fr_msg_ram_t *msg)
{
	uint32_t frwrhs1 = 0;
	uint32_t frwrhs2 = 0;
	uint32_t frwrhs3 = 0;
	uint16_t  dataPointer = 0;
	
	//Enables the receive / transmit interrupt for the corresponding message buffer
	if( (msg->type==FR_MSGBUF_TYPE_RX)||(msg->type==FR_MSGBUF_TYPE_TX) ){
		frwrhs1 |= (1<<FR_WRHS1_MBI); 
	}else{
		frwrhs1 &= ~(1<<FR_WRHS1_MBI);
	}
	//Configure transmission mode
	if (msg->mode==FR_SINGLE_MODE){
		frwrhs1 |= (1<<FR_WRHS1_TXM);
	} else {
		frwrhs1 &= ~(1<<FR_WRHS1_TXM);
	}

	// Configure direction
	if (msg->type==FR_MSGBUF_TYPE_TX){
		// Configure payload preamble indicator transmit
		if (msg->ppi){
			frwrhs1 |= (1<<FR_WRHS1_PPIT);
		} else {
			frwrhs1 &= ~(1<<FR_WRHS1_PPIT);
		}
		frwrhs1 |= (1<<FR_WRHS1_CFG);	
	}else{
		frwrhs1 &= ~(1<<FR_WRHS1_CFG);
	}			
	// Configure channel control/filter
	if(msg->channel==FR_CHANNEL_AB) { 	
		frwrhs1 |= (FR_CHANNEL_AB<<FR_WRHS1_CH_L);
	}else if(msg->channel==FR_CHANNEL_A){
		frwrhs1 |= (FR_CHANNEL_A<<FR_WRHS1_CH_L);
	}else if(msg->channel==FR_CHANNEL_B){
		frwrhs1 |= (FR_CHANNEL_B<<FR_WRHS1_CH_L);
	}else{
		frwrhs1 &= ~(FR_CHANNEL_AB<<FR_WRHS1_CH_L);
	}		
	// Configure cycle count filter
	frwrhs1 |= (msg->cycleCount<<FR_WRHS1_CYC_L);
	// Configure frame filter
	if ((msg->frameId <= dev->gNumberOfStaticSlots) && (msg->payloadLength > 2*dev->gPayloadLengthStatic)) {
		return EINVAL;
	}
	if (msg->frameId > FR_FID_MAX){
		msg->frameId = FR_FID_MAX;
	} 
	frwrhs1 |= (msg->frameId<<FR_WRHS1_FID_L);
	
	// Configure payload length of the message
	if (msg->payloadLength > FR_MAX_PAYLOAD){
		msg->payloadLength = FR_MAX_PAYLOAD;
	} 
	frwrhs2 |= (((msg->payloadLength + 1)/2)<<FR_WRHS2_PLC_L);

	if(msg->type==FR_MSGBUF_TYPE_TX){
		// Configure msg crc
		frwrhs2 |= (get_header_crc(msg)<<FR_WRHS2_CRC_L); 	
	}
	// Configure data pointer 
	dataPointer = (FR_MBF_MAX_MEM -1 ) - ((msg->payloadLength + 3)/4)*(msg->idx);
	if (dataPointer < (msg->idx)*4 ){
		return EINVAL;
	}		
	// Wait if transfer between IBF Shadow and Message RAM in progress
	if(fr_wait_inputbuffer_busy(dev)){
		return ETIMEDOUT;
	}

	out32(ph->frwrhs1,frwrhs1);
	out32(ph->frwrhs2,frwrhs2);
	frwrhs3 = dataPointer & FR_WRHS3_DP_MASK;
	out32(ph->frwrhs3,frwrhs3);
    return 0;
}

/** 
 * @brief Store message URAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @param msg: Pointer to struct fr_msg_ram_t.
 * @return 0:success, ETIMEDOUT :error
 */
int fr_store_to_uram(fr_dev_t *dev,fr_input_entry_t *pe,fr_msg_ram_t *msg)
{
	fr_regs_t *regs = &dev->regs;
	uint32_t frwrhs4 = 0;
	
	// Update header section
	if(msg->cmdMask & FR_CMD_MASK_HEADER){
		frwrhs4 |= (1<<FR_WRHS4_LHS);		
	} else {
		frwrhs4 &= ~(1<<FR_WRHS4_LHS);
	}
	if(msg->payloadLength > FR_MAX_PAYLOAD){
		msg->payloadLength = FR_MAX_PAYLOAD;
	} 
	frwrhs4 |= ((msg->payloadLength/4)<<FR_WRHS4_DSL_L);

	if(msg->cmdMask & FR_CMD_MASK_DATA){
		frwrhs4 |= (1<<FR_WRHS4_LDS);
	} else {
		frwrhs4 &= ~(1<<FR_WRHS4_LDS);
	}
	//if(msg->type==FR_MSGBUF_TYPE_TX){
		if(msg->cmdMask & FR_CMD_MASK_REQUEST){
			frwrhs4 |= (1<<FR_WRHS4_STR);
		} else {
			frwrhs4 &= ~(1<<FR_WRHS4_STR);
		}
	//}
	frwrhs4 |= (msg->idx<<FR_WRHS4_IMBNR_L);
	if(FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) != FR_BIT_SET){
		return ETIMEDOUT;
	}
	if(fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_IQFP,FR_BIT_CLEAR)){
		return ETIMEDOUT;
	}
	// Write command mask
	out32(pe->frwrhs4,frwrhs4);
	// Demand data transfer to target message buffer: write FRIQC.IMBNR
	if(FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) != FR_BIT_SET){
		return ETIMEDOUT;
	}
	if(fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_IQFP,FR_BIT_CLEAR)){
		return ETIMEDOUT;
	}	
	write_bits(regs->data_xfer_ctrl.friqc, FR_IQC_IMBNR_L, FR_IQC_IMBNR_H, msg->idx);
	
	return 0;
}
/** 
 * @brief Store messages to message RAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @return 0:success, ETIMEDOUT :error
 */
int fr_transmit(fr_dev_t *dev)
{
	frmsg_t *frmsg;
	int status = 0;
	frmsg = frmsg_dequeue_element(dev->tx_msg_queue);

	if (frmsg){
		/* Start transmission */
		status = fr_set_header_message_buffer(dev,&frmsg->msg.tx_msg);
		if(status) {
			goto end_tx;
		}
		fr_set_data_buffer(dev,frmsg->msg.tx_msg.payloadLength,frmsg->msg.tx_msg.data);
		fr_store_to_message_ram(dev,&frmsg->msg.tx_msg);
		if(fr_wait_inputbuffer_busy(dev)){
			status = ETIMEDOUT;
			goto end_tx;
		}
end_tx:		
		/* Free queue */
		frmsg_queue_element(dev->tx_free_queue,frmsg);
		return status;
	}
return status;	
}
/** 
 * @brief Store messages to message RAM.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: Pointer to struct io_devctl_t.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @return 0:success, !=0 :error
 */
int rcar_transmit_msgs(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
    fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	fr_regs_t *regs = &dev->regs;
	uint8_t *buf;
	fr_msg_ram_t *pmsg;
	fr_poc_state pocState = 0;
		
	fr_get_pocs_state(regs,&pocState);
	if(pocState != FR_STATE_NORMAL_ACTIVE){
		fr_slogf(FR_SLOG_DEBUG1,"%s: FlexRay is not in STATE_NORMAL_ACTIVE", __func__);
		return EIO;
	}
	if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) == FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG2,"%s: user ram transfer is operating", __func__);
		return EINVAL;
	}

	buf = (uint8_t *)msg;
	buf += sizeof(io_devctl_t);
	pmsg = (fr_msg_ram_t*)buf;
	if( (pmsg->type == FR_MSGBUF_TYPE_RX)||(pmsg->type == FR_MSGBUF_TYPE_FIFO)){
		return ENOSYS;
	}
	frmsg_t *txmsg = frmsg_dequeue_element(dev->tx_free_queue);
	if(txmsg){
		memcpy(&txmsg->msg.tx_msg,pmsg,sizeof(fr_msg_ram_t));
		frmsg_queue_element(dev->tx_msg_queue,txmsg);
		fr_transmit(dev);
		return fr_tx_replay_data(ctp->iov,msg,ctp->rcvid,&txmsg->msg.tx_msg);
	}else{
		return EAGAIN;
	}
}
/** 
 * @brief Write data to uram.
 * @param dev: Pointer to struct fr_dev_t.
 * @param pdata: Pointer to struct fr_uram_data_t.
 * @param payloadLength: Length of data.
 * @param payload: Pointer to data.
 * @return 0:success, !=0 :error
 */
int fr_set_data_to_uram(fr_dev_t *dev,fr_uram_data_t *pdata,uint8_t payloadLength, uint8_t *payload)
{
	uint32_t	data; 
	int i = 0;
	int status = 0;
	
	status = fr_wait_inputbuffer_busy(dev);
	if(status){
		return status;
	}
	for(i=0; i<payloadLength; i+=4){
		data = 0x00000000;
		if((i+0)<payloadLength) data |= (uint32_t)payload[i+0] << 0;
		if((i+1)<payloadLength) data |= (uint32_t)payload[i+1] << 8;
		if((i+2)<payloadLength) data |= (uint32_t)payload[i+2] << 16;
		if((i+3)<payloadLength) data |= (uint32_t)payload[i+3] << 24;
		out32(pdata->frwrds[i>>2],data);
	}
	return 0;
}
/** 
 * @brief Store messages to URAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @return 0:success, ETIMEDOUT :error
 */
int fr_uram_transmit(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	uint32_t ipadx = 0, idx = 0;
	fr_input_table_t *pt = &dev->input_info;
	int status = 0;
	frmsg_t *frmsg = NULL;

	frmsg = frmsg_dequeue_element(dev->tx_msg_queue);
	if (frmsg){
		/* Start transmission */
		ipadx = FR_REG_BITS(regs->data_xfer_stat.frits,FR_ITS_IPIDX_L,FR_ITS_IPIDX_H);
		idx = frmsg->msg.tx_msg.idx;		
		status = fr_set_header_uram(dev,&pt->pheader[idx],&frmsg->msg.tx_msg);
		if(status) {
			goto end_tx;
		}
		fr_set_data_to_uram(dev,&pt->pdata[idx],frmsg->msg.tx_msg.payloadLength,frmsg->msg.tx_msg.data);
		out32(pt->pentry[ipadx].dp,(uint32_t)(pt->ipd_paddr + idx*FR_DATA_LEN));
		if(fr_store_to_uram(dev,&pt->pentry[ipadx],&frmsg->msg.tx_msg)){
			status = ETIMEDOUT;
			goto end_tx;
		}
		if(fr_wait_inputbuffer_busy(dev)){
			status = ETIMEDOUT;
			goto end_tx;
		}
end_tx:		
		/* Free queue */
		frmsg_queue_element(dev->tx_free_queue,frmsg);
		return status;
	}
	return status;	
}
/** 
 * @brief Store messages to URAM.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: Pointer to struct io_devctl_t.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @return 0:success, !=0 :error
 */
int rcar_fr_transmit_uram(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
    fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	fr_regs_t *regs = &dev->regs;
    int status = 0;
	uint8_t *buf;
	fr_msg_ram_t *pmsg;
	fr_poc_state pocState = 0;

	if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) != FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Input Transfer Status is disable\n",__func__);
		return ETIMEDOUT;
	}
	
	fr_get_pocs_state(regs,&pocState);
	if(pocState != FR_STATE_NORMAL_ACTIVE){
		fr_slogf(FR_SLOG_DEBUG1,"%s: FlexRay is not in STATE_NORMAL_ACTIVE", __func__);
		return EIO;
	}
	buf = (uint8_t *)msg;
	buf += sizeof(io_devctl_t);
	pmsg = (fr_msg_ram_t*)buf;

	if( (pmsg->type == FR_MSGBUF_TYPE_RX)||(pmsg->type == FR_MSGBUF_TYPE_FIFO)){
		return ENOSYS;
	}
	frmsg_t *txmsg = frmsg_dequeue_element(dev->tx_free_queue);
	if(txmsg){
		memcpy(&txmsg->msg.tx_msg,pmsg,sizeof(fr_msg_ram_t));
		frmsg_queue_element(dev->tx_msg_queue,txmsg);
		status = fr_uram_transmit(dev);
		if(status){
			return status;
		}else{
			return fr_tx_replay_data(ctp->iov,msg,ctp->rcvid,&txmsg->msg.tx_msg);
		}
	}else{
		return EAGAIN;
	}	
	
	return (EOK);
}