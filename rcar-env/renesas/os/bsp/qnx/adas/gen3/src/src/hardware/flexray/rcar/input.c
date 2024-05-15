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

#include <unistd.h>
#include <sys/mman.h>
#include <atomic.h>
#include "rcar-flexray.h"
#include "flexray-regs.h"

#define frwrhs1_off					0x00
#define frwrhs2_off					0x04
#define frwrhs3_off					0x08
#define frwrds_off					0x0C

#define frwrhs4_off					0x00
#define frdp_off					0x04

/** 
 * @brief Configure input data transfer.
 * @param dev: Pointer to fr_dev_t structure.
 * @param pmsg: A pointer to fr_msg_tx_t structure.
 * @return 0:success, -1, EBUSY:error
 */
int fr_config_input(fr_dev_t *dev,fr_msg_tx_t *pmsg)
{
	uint8_t	i = 0;
	uint8_t numberMessage = 0;
	uint8_t	itm = 0;	
	fr_regs_t *regs = &dev->regs;
	fr_input_table_t *pt = &dev->input_info;
	int fr_align = FR_ALIGN;
	int entry_len, header_len, data_len;
	uint8_t idx = 0;
	uint8_t index = 0;

	numberMessage = pmsg->numberMessage;
    //Disable the Input Transfer
    clear_bit(regs->data_xfer_ctrl.fritc, FR_ITC_ITE);
	entry_len 	= FR_INPUT_ENTRY_LEN;
	header_len 	= FR_HEADER_LEN;
	data_len 	= FR_DATA_LEN;
	/*-- input_pointer_table_size[bytes] = (((FRITC.ITM +1)*2)*4) 
	*    Equation 4-1: user_input_address = FRIBA.ITA + input_pointer_table_size */
	pt->entry_size 	= (FR_MAX_BUF + 1)*entry_len;
	pt->header_size = (FR_MAX_BUF)*header_len;
	pt->data_size 	= (FR_MAX_BUF)*data_len;
	pt->table_size  = pt->entry_size + pt->header_size + pt->data_size + entry_len;
	//Configure input structure pointer table base address FRIBA.ITA
	if(pt->ita_fd == -1){	
		pt->ita_fd = posix_typed_mem_open(RCARFR_POSIX_TYPED_MEM_PATH, O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
		if (pt->ita_fd == -1){
			fr_slogf(FR_SLOG_DEBUG0,"%s: Can't open posix typed memory for input pointer table", __func__);
			return -1;
		}
		pt->ita_vaddr = (uintptr_t)mmap64(NULL, pt->table_size,PROT_READ|PROT_WRITE|PROT_NOCACHE, MAP_SHARED , pt->ita_fd, 0);
		if (pt->ita_vaddr == (uintptr_t)MAP_FAILED){
			fr_slogf(FR_SLOG_DEBUG0,"%s: mmap memory for input pointer table failed", __func__);
			return -1;
		}
		//Get physical address
		if (mem_offset64((const void *)pt->ita_vaddr, NOFD, 1, &pt->ita_paddr, 0) == -1){
			fr_slogf(FR_SLOG_DEBUG0,"%s: Unable to get the physical memory of input pointer table", __func__);
			munmap((void*)pt->ita_vaddr, pt->table_size);
			return -1;
		}
	}
	/*	Configure input pointer table base address.
		The user can only write to this bit when FRITS.ITS is '0'..
	*/
    if(fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_ITS,FR_BIT_CLEAR)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Input Transfer Status is disable", __func__);
		return EBUSY;
	}
	
	pt->ita_vaddr = (pt->ita_vaddr + (fr_align -1)) & ~(fr_align-1);
	pt->ita_paddr = (pt->ita_paddr + (fr_align -1)) & ~(fr_align-1);

    write_bits(regs->data_xfer_ctrl.friba, FR_IBA_ITA_L, FR_IBA_ITA_H, pt->ita_paddr);
    /* Configure size or input pointer table FRITC.ITM=numberMessage-1. 
		Valid values are h'00 (1 queue entry) to h'7F (128 queue entries). 
	*/
    write_bits(regs->data_xfer_ctrl.fritc, FR_ITC_ITM_L, FR_ITC_ITM_H, FR_MAX_BUF-1);
	
	itm = FR_REG_BITS(regs->data_xfer_ctrl.fritc, FR_ITC_ITM_L, FR_ITC_ITM_H);
	itm &= FR_ITC_ITM_MAX;
	/*	Equation 4-1: user_input_address = FRIBA.ITA+input_pointer_table_size
		input_pointer_table_size_bytes = (((FRITC.ITM+1)*2)*4)
	*/
	pt->user_vaddr = (pt->ita_vaddr + (itm + 1)*FR_INPUT_ENTRY_LEN);
	pt->user_paddr = (pt->ita_paddr + (itm + 1)*FR_INPUT_ENTRY_LEN);	
	/*	Allocate memory for Input pointer table. FRWRHS4 [entry 0..numberMessage]
		Data pointer for entry 0...numberMessage
	*/ 			
	pt->uentry.frwrhs4 			= pt->user_vaddr;
	pt->uentry.dp 				= pt->user_vaddr + frdp_off;
	pt->iph_vaddr			= pt->user_vaddr + entry_len;
	pt->iph_paddr			= pt->user_paddr + entry_len;
	pt->ipd_vaddr			= pt->iph_vaddr  + (FR_MAX_BUF)*header_len;
	pt->ipd_paddr			= pt->iph_paddr  + (FR_MAX_BUF)*header_len;
	
	for(i=0;i<FR_MAX_BUF;i++) {
		pt->pentry[i].frwrhs4 	= (pt->ita_vaddr + i*entry_len) + frwrhs4_off;
		pt->pentry[i].dp 		= (pt->ita_vaddr + i*entry_len) + frdp_off;
	}
	for(i=0;i<numberMessage;i++) {
		idx = pmsg->nmsg[i].idx;
		for(index =0; index< FR_MAX_DATA_INDEX; index++){
			pt->pdata[idx].frwrds[index] = (pt->ipd_vaddr + idx*data_len + index*FR_RG_DATA_LEN);
		}
		pt->pheader[idx].frwrhs1 	= (pt->iph_vaddr + idx*header_len) + frwrhs1_off;
		pt->pheader[idx].frwrhs2 	= (pt->iph_vaddr + idx*header_len) + frwrhs2_off;
		pt->pheader[idx].frwrhs3 	= (pt->iph_vaddr + idx*header_len) + frwrhs3_off;
		pt->pheader[idx].frdata 	= (pt->iph_vaddr + idx*header_len) + frwrds_off;
		
		out32(pt->pentry[i].dp,(uint32_t)(pt->iph_paddr + idx*header_len));
		out32(pt->pheader[idx].frdata,(uint32_t)(pt->ipd_paddr + idx*data_len));	
	}
	/*	FRIBCR.IBSYS = 0 ?
	*/
    if(fr_check_state_of_bit(regs->in_buf.fribcr, FR_IBCR_IBSYS,FR_BIT_CLEAR)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Wait FRIBCR.IBSYS clear failed", __func__);
		return EBUSY;
	}
	// Enable the Transfer Handler. Set FRITC.ITE to '1'
	if(FR_REG_BIT(regs->data_xfer_ctrl.fritc, FR_ITC_ITE) == FR_BIT_CLEAR){
		set_bit(regs->data_xfer_ctrl.fritc, FR_ITC_ITE);
		//Wait until is FRITS.ITS=1. The FRITS.ITS set to '1' when FRITC.ITE is set to '1'.
		if(fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_ITS,FR_BIT_SET)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Wait FRITC.ITE is set to '1' failed", __func__);
			return EBUSY;
		}
	}
	return 0;
}

/** 
 * @brief Request a transfer of an input data structure.
 * @param dev: Pointer to fr_dev_t structure.
 * @param pmsg: Pointer to fr_msg_ram_t structure.
 * @return 0: success, EINVAL, ETIMEDOUT: error
 */
int fr_uram_set_select_tx(fr_dev_t *dev)
{
	uint8_t idx = 0;
	int status = 0;
	fr_regs_t *regs = &dev->regs;
	fr_input_table_t *pt = &dev->input_info;
	frmsg_t *frmsg;
	
	frmsg = frmsg_dequeue_element(dev->tx_msg_queue);
	if (frmsg){
		idx = frmsg->msg.tx_msg.idx;

		out32(pt->uentry.dp,(uint32_t)(pt->ipd_paddr + idx*FR_DATA_LEN));
		
		status = fr_set_header_uram(dev,&pt->pheader[idx],&frmsg->msg.tx_msg);
		if(status) {
			goto end_tx;
		}
		fr_set_data_to_uram(dev,&pt->pdata[idx],frmsg->msg.tx_msg.payloadLength,frmsg->msg.tx_msg.data);
		if(fr_store_to_uram(dev,&pt->uentry,&frmsg->msg.tx_msg)){
			status = ETIMEDOUT;
			goto end_tx;
		}
		if(fr_wait_inputbuffer_busy(dev)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Wait transfer between IBF Shadow and Message RAM timeout", __func__);
			status = ETIMEDOUT;
			goto end_tx;
		}
		/*	Section 53.2.12.7 (1)
			The user should not write to this register when FRITS.ITS is '0'.
			The user should not write to this register when FRITS.UIRP is '1'.
			The user should not write to this register when FRITS.IQH is '1'.
			The user should only write FRITC.ITM+1 to this register.
		*/
		if(fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_ITS,FR_BIT_SET)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Input Transfer is disable",__func__);
			status = ETIMEDOUT;
		}
		if(fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_UIRP,FR_BIT_CLEAR)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: User input transfer request pending",__func__);
			status = ETIMEDOUT;
		}
		if(fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_IQH,FR_BIT_CLEAR)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Input queue halted",__func__);
			status = ETIMEDOUT;		
		}
		write_bits(regs->data_xfer_ctrl.fruir, FR_UIR_UIDX_L,FR_UIR_UIDX_H, 
		(FR_REG_BITS(regs->data_xfer_ctrl.fritc,FR_ITC_ITM_L,FR_ITC_ITM_H) & FR_ITC_ITM_MAX) + 1);
		
end_tx:		
		/* Free queue */
		frmsg_queue_element(dev->tx_free_queue,frmsg);
		return status;	
	}
return status;	
}

/** 
 * @brief Request input transfer.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: Pointer to struct io_devctl_t.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @return 0:success, EIO,EINVAL :error
 */
int rcar_uram_select_tx(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	fr_regs_t *regs = &dev->regs;
	uint8_t *buf;
	fr_msg_ram_t *pmsg;
	
	if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) != FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Input Transfer is disable\n",__func__);
		return ETIMEDOUT;
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
		fr_uram_set_select_tx(dev);
		return fr_tx_replay_data(ctp->iov,msg,ctp->rcvid,&txmsg->msg.tx_msg);
	}else{
		return EAGAIN;
	}
	return EOK;
}
