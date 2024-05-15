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
#include <sys/mman.h>
#include <time.h>
#include "rcar-flexray.h"
#include "flexray-regs.h"

#define frrdhs1_off		0x00
#define frrdhs2_off		0x04
#define frrdhs3_off		0x08
#define frmbs_off		0x0C
#define frrdds_off		0x10

/** 
 * @brief Check new data available in message ram.
 * @param dev: Pointer to struct fr_dev_t.
 * @param idx: The index of message buffer.
 * @return 0: success, -1: error
 */
int fr_uram_check_newdata(fr_dev_t *dev,uint8_t idx)
{
	fr_regs_t *regs = &dev->regs;
	uint8_t 	i = 0;
	uint32_t 	mask = 0;
	uint32_t 	frda = 0;
	
	i = idx/32;
	if(i>3){
		i = 3;
	}
	mask = 1 << (idx % 32);
	frda = in32(regs->data_xfer_stat.frda[i]);
	if(frda & mask){
		return 0;
	}
	return -1;
}

/** 
 * @brief Get the number of unprocessed output FIFO structures.
 * @param dev: Pointer to struct fr_dev_t.
 * @param fifo_ffl: Pointer to uint8_t that contains the number of available output FIFO structures in the URAM. 
 * @return none
 */
void fr_uram_get_fifo_fill_level(fr_dev_t *dev, uint8_t *fifo_ffl)
{
	fr_regs_t *regs = &dev->regs;
	*fifo_ffl = (uint8_t)(FR_REG_BITS(regs->data_xfer_stat.frots,FR_OTS_FFL_L,FR_OTS_FFL_H));
	return ;
}

/** 
 * @brief Get index of output FIFO structures.
 * @param dev: Pointer to struct fr_dev_t.
 * @param fgidex: Pointer to uint8_t that contains the index where the current output data structure pointer in the FIFO pointer table is available for
 * reading.  
 * @return none
 */
void fr_uram_get_fifo_idx(fr_dev_t *dev, uint8_t *fgidex)
{
	fr_regs_t *regs = &dev->regs;
	*fgidex = (uint8_t)(FR_REG_BITS(regs->data_xfer_stat.frots,FR_OTS_FGIDX_L,FR_OTS_FGIDX_H));
	return ;
}
/** 
 * @brief Get header of output structures.
 * @param dev: Pointer to struct fr_dev_t.
 * @param ph: Pointer to struct fr_output_header_t.
 * @param msg: Pointer to struct fr_msg_buffer_rx_t. 
 * @return none
 */
void fr_uram_read_header(fr_dev_t *dev,fr_output_header_t *ph, fr_msg_buffer_rx_t *msg)
{	
	uint32_t header1 = 0;
	uint32_t header2 = 0;
	uint32_t header3 = 0;
	
	header1 = in32(ph->frrdhs1);
	header2 = in32(ph->frrdhs2);
	header3 = in32(ph->frrdhs3);	

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
/** 
 * @brief Get data of output structures.
 * @param pd: Pointer to struct fr_output_data_t.
 * @param payload: Pointer to uint8_t.
 * @param msg: Pointer to struct fr_msg_buffer_rx_t. 
 * @return none
 */
void fr_uram_read_data(fr_output_data_t *pd, uint8_t *payload, uint16_t data_len)
{
	uint16_t length = (data_len + 3) / 4;
	uint8_t  index = 0;
	uint32_t data = 0;

	if(data_len > FR_MAX_PAYLOAD){
		data_len = FR_MAX_PAYLOAD;
	}
	for(index = 0; index < length; index++){
		data= in32(pd->frrdds[index]);
		*((uint32_t*)(payload + index*4)) = data;
	}
}

void fr_fifo_rx_uram(fr_dev_t *dev, fr_msg_buffer_rx_t *msgBuffer)
{
	uint8_t 	fifo_fgidx = 0;
	uint8_t 	payloadLength = 0;
	fr_regs_t *regs = &dev->regs;
	fr_output_table_t *pt = &dev->output_info;

	fr_uram_get_fifo_idx(dev,&fifo_fgidx);
	fr_uram_read_header(dev,&pt->pf_header[fifo_fgidx],msgBuffer);
	payloadLength = msgBuffer->payloadLength;
	fr_uram_read_data(&pt->pf_data[fifo_fgidx],msgBuffer->data,payloadLength);
	out32(regs->data_xfer_stat.frots,(1<<FR_OTS_FDA));
}
/** 
 * @brief Receive message buffer by using URAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @param pmsg: Pointer to struct fr_msg_buffer_rx_t.
 * @return none
 */
void fr_rx_uram(fr_dev_t *dev, fr_msg_buffer_rx_t *pmsg)
{
	uint8_t 	payloadLength = 0;
	fr_regs_t *regs = &dev->regs;
	fr_output_table_t *pt = &dev->output_info;
	uint8_t i = pmsg->idx/32;
	uint32_t mask = 1 << (pmsg->idx % 32);
	
	if(i>3){
		i = 3;
	}
	fr_uram_read_header(dev,&pt->pheader[pmsg->idx],pmsg);
	payloadLength = pmsg->payloadLength;
	fr_uram_read_data(&pt->pdata[pmsg->idx],pmsg->data,payloadLength);
	out32(regs->data_xfer_stat.frda[i],mask);
}

/** 
 * @brief Clear data available for all dedicated receive buffers.
 * @param dev: Pointer to fr_dev_t structure.
 * @param index: index of messsage buffer.
 * @return none
 */
void fr_output_clear_data_available(fr_dev_t *dev,uint8_t index)
{
	uint8_t		i = 0;
	fr_regs_t *regs = &dev->regs;
	
	uint32_t mask = 1 << (index % 32);
	
	i = index/32;
	if(i>3){
		i = 3;
	}
	/* Clear DA 
		FRDA[i].DAb=1; This bit is cleared when writing a '1' to it.
		The user should not write a '1' to bits that are '0'.
	*/
	out32(regs->data_xfer_stat.frda[i], mask);
}

/** 
 * @brief Enable interrupt ouput transfer.
 * @param dev: Pointer to fr_dev_t structure.
 * @return none
 */
void fr_enable_irq_output_uram(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;		
	//Enable output transfer interrupt
	if(FR_REG_BIT(regs->data_xfer_ctrl.frotc, FR_OTC_OIE) == FR_BIT_CLEAR) {
		set_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OIE);
	}
	//Enable fifo transfer interrupt
	if(FR_REG_BIT(regs->data_xfer_ctrl.frotc, FR_OTC_FIE) == FR_BIT_CLEAR) {
		set_bit(regs->data_xfer_ctrl.frotc, FR_OTC_FIE);
	} 
}

/** 
 * @brief Set output and fifo pointer table size.
 * @param dev: Pointer to fr_dev_t structure.
 * @return 0:success, -1 :error
 */
int fr_set_output_fifo_size(fr_dev_t *dev)
{
	fr_output_table_t *pt = &dev->output_info;
	if(pt->ota_fd == -1){
	
		pt->ota_size 		= FR_MAX_BUF*FR_OUTPUT_ENTRY_LEN;
		pt->header_size 	= FR_MAX_BUF*FR_OUTPUT_HEADER_LEN;
		pt->data_size 		= FR_MAX_BUF*FR_DATA_LEN;
		pt->ota_size 		+= 	pt->header_size  + pt->data_size;
			
		pt->ota_fd = posix_typed_mem_open(RCARFR_POSIX_TYPED_MEM_PATH, O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
		if(pt->ota_fd == -1){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Can't open posix typed memory for output pointer table", __func__);
			return -1;
		}
		pt->ota_vaddr =(uintptr_t) mmap64(NULL, pt->ota_size,PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, pt->ota_fd, 0);
		if(pt->ota_vaddr == (uintptr_t)MAP_FAILED){
			fr_slogf(FR_SLOG_DEBUG1,"%s: mmap memory for output pointer table failed", __func__);
			return -1;
		}
		//Get physical address
		if(mem_offset((void*)pt->ota_vaddr, NOFD, 1, &pt->ota_paddr, 0) == -1){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Unable to get the physical memory for output table",__func__);
			munmap((void*)pt->ota_vaddr, pt->ota_size);
			return -1;
		}
	}
	if(pt->fifo_fd == -1){
		pt->ff_size 		= FR_MAX_FIFO*FR_OUTPUT_ENTRY_LEN;
		pt->ff_header_size 	= FR_MAX_FIFO*FR_OUTPUT_HEADER_LEN;
		pt->ff_data_size 	= FR_MAX_FIFO*FR_DATA_LEN;
		pt->ff_size 		+=  pt->ff_header_size + pt->ff_data_size; 	
		
		pt->fifo_fd = posix_typed_mem_open(RCARFR_POSIX_TYPED_MEM_PATH, O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
		if(pt->fifo_fd == -1){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Can't open posix typed memory for fifo pointer table", __func__);
			return -1;
		}
		pt->ff_vaddr =(uintptr_t) mmap64(NULL, pt->ff_size,PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, pt->fifo_fd, 0);
		if(pt->ff_vaddr == (uintptr_t)MAP_FAILED){
			fr_slogf(FR_SLOG_DEBUG1,"%s: mmap memory for fifo pointer table failed: error: %s", __func__,strerror(errno));
			return -1;
		}
		//Get physical address
		if(mem_offset((void*)pt->ff_vaddr, NOFD, 1, &pt->ff_paddr, 0) == -1){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Unable to get the physical memory for fifo table",__func__);
			munmap((void*)pt->ff_vaddr, pt->ff_size);
			return -1;
		}
	}		

	return 0;
}

/** 
 * @brief Configure output pointer table.
 * @param dev: Pointer to fr_dev_t structure.
 * @return 0:success, EBUSY:error
 */
int fr_config_output(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	fr_output_table_t *pt = &dev->output_info;
	int fr_align = FR_ALIGN;
	
	// Disable the Output Transfer
    clear_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OTE);
	if(fr_check_state_of_bit(regs->data_xfer_stat.frots, FR_OTS_OTS,FR_BIT_CLEAR)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Output Transfer Status is disable failed\n",__func__);
		return EBUSY;
	}
		
	// Align 4 byte 
	pt->ota_vaddr = (pt->ota_vaddr + (fr_align -1)) & ~(fr_align-1);
	pt->ota_paddr = (pt->ota_paddr + (fr_align -1)) & ~(fr_align-1);
	/*	Configure output pointer table address.
		The user can only write to this bit when FROTS.OTS is '0'..
	*/
	if(fr_check_state_of_bit(regs->data_xfer_stat.frots, FR_OTS_OTS,FR_BIT_CLEAR)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Output Transfer is enable\n",__func__);
		return EBUSY;
	}
    // Configure output structure pointer table base address
    out32(regs->data_xfer_ctrl.froba,pt->ota_paddr);
	return 0;
}
/** 
 * @brief Allocate memory for all required output data structure.
 * @param dev: Pointer to fr_dev_t structure.
 * @param pmsg: A pointer to fr_msg_tx_t structure.
 * @return none
 */
void fr_allocate_output_memory(fr_dev_t *dev,fr_msg_tx_t *pmsg)
{
	uint8_t	i = 0;
	uint8_t numberMessage = 0;
	fr_output_table_t *pt = &dev->output_info;
	int entry_len,header_len, data_len;
	uint8_t idx = 0;
	int index = 0;

	entry_len 	= FR_OUTPUT_ENTRY_LEN;
	header_len 	= FR_OUTPUT_HEADER_LEN;
	data_len 	= FR_DATA_LEN;
	numberMessage = pmsg->numberMessage;
	//Allocate memory for all required output data structure
	pt->oph_vaddr = pt->ota_vaddr + FR_MAX_BUF*entry_len;
	pt->oph_paddr = pt->ota_paddr + FR_MAX_BUF*entry_len;
	pt->opd_vaddr = pt->oph_vaddr + FR_MAX_BUF*header_len;
	pt->opd_paddr = pt->oph_paddr + FR_MAX_BUF*header_len;
	for(i=0;i<numberMessage;i++) {
		idx = pmsg->nmsg[i].idx;
		if(pmsg->nmsg[i].type==FR_MSGBUF_TYPE_RX){
			pt->pentry[idx].frmb 	= (pt->ota_vaddr + idx*entry_len);
			out32(pt->pentry[idx].frmb,(uint32_t)(pt->oph_paddr + idx*header_len));
			pt->pheader[idx].frrdhs1 	= (pt->oph_vaddr + idx*header_len) + frrdhs1_off;
			pt->pheader[idx].frrdhs2 	= (pt->oph_vaddr + idx*header_len) + frrdhs2_off;
			pt->pheader[idx].frrdhs3 	= (pt->oph_vaddr + idx*header_len) + frrdhs3_off;
			pt->pheader[idx].frmbs 		= (pt->oph_vaddr + idx*header_len) + frmbs_off;
			pt->pheader[idx].frdata 	= (pt->oph_vaddr + idx*header_len) + frrdds_off;
			fr_rx_uram_set_header(dev,&pt->pheader[idx],&pmsg->nmsg[i]);
			out32(pt->pheader[idx].frdata,(uint32_t)(pt->opd_paddr + idx*data_len));				
			for(index =0; index< FR_MAX_DATA_INDEX; index++){
				pt->pdata[idx].frrdds[index] = (pt->opd_vaddr + idx*data_len + index*4);					
			}
		}
	}
	
	//Clear DA for all dedicated receive buffers
	for(i = 0; i<numberMessage;i++){
		if(pmsg->nmsg[i].type==FR_MSGBUF_TYPE_RX){
			fr_output_clear_data_available(dev,pmsg->nmsg[i].idx);
		}
	}
	return ;
}

/** 
 * @brief Enable URAM output transfer.
 * @param dev: Pointer to fr_dev_t structure.
 * @return 0:success, EBUSY:error
 */
int fr_uram_output_enable(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;	
	// Set FROTC.OTE to '1'
	if(FR_REG_BIT(regs->data_xfer_ctrl.frotc, FR_OTC_OTE) == FR_BIT_CLEAR){
		set_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OTE);
		//Wait until is FROTS.OTS=1. The FROTS.OTS set to '1' when FROTC.OTE is set to '1'.
		if(fr_check_state_of_bit(regs->data_xfer_stat.frots, FR_OTS_OTS,FR_BIT_SET)){
			return EBUSY;
		}
	}
	return 0;
}


/** 
 * @brief Configure fifo pointer table.
 * @param dev: Pointer to fr_dev_t structure.
 * @return none
 */
void fr_config_fifo(fr_dev_t *dev,fr_msg_tx_t *pmsg)
{
	fr_regs_t *regs = &dev->regs;
	fr_output_table_t *pt = &dev->output_info;
	int entry_len,header_len, data_len;
	int index = 0;
	uint8_t i = 0;
	int fr_align = FR_ALIGN;
	
	entry_len 	= FR_OUTPUT_ENTRY_LEN;
	header_len 	= FR_OUTPUT_HEADER_LEN;
	data_len 	= FR_DATA_LEN;
	// Configure output structure pointer fifo base address
	// Align 4 byte 
	pt->ff_vaddr = (pt->ff_vaddr + (fr_align -1)) & ~(fr_align-1);
	pt->ff_paddr = (pt->ff_paddr + (fr_align -1)) & ~(fr_align-1);
		
	pt->fph_vaddr = pt->ff_vaddr + FR_MAX_FIFO*entry_len;
	pt->fph_paddr = pt->ff_paddr + FR_MAX_FIFO*entry_len;
	
	pt->fpd_vaddr = pt->fph_vaddr + FR_MAX_FIFO*header_len;
	pt->fpd_paddr = pt->fph_paddr + FR_MAX_FIFO*header_len;
	
	write_bits(regs->data_xfer_ctrl.frfba, FR_FBA_FTA_L, FR_FBA_FTA_H,pt->ff_paddr);
	write_bits(regs->data_xfer_ctrl.frotc, FR_OTC_FTM_L, FR_OTC_FTM_H,FR_MAX_FIFO -1);

	for(i=0;i<FR_MAX_FIFO;i++) {

		pt->pf_entry[i].frmb 	= (pt->ff_vaddr + i*entry_len);
	
		pt->pf_header[i].frrdhs1 	= (pt->fph_vaddr + i*header_len) + frrdhs1_off;
		pt->pf_header[i].frrdhs2 	= (pt->fph_vaddr + i*header_len) + frrdhs2_off;
		pt->pf_header[i].frrdhs3 	= (pt->fph_vaddr + i*header_len) + frrdhs3_off;
		pt->pf_header[i].frmbs 		= (pt->fph_vaddr + i*header_len) + frmbs_off;
		pt->pf_header[i].frdata 	= (pt->fph_vaddr + i*header_len) + frrdds_off;
		out32(pt->pf_entry[i].frmb,(uint32_t)(pt->fph_paddr + i*header_len));
		
		fr_rx_uram_set_header(dev,&pt->pf_header[i],&pmsg->nmsg[i]);
		
		for(index =0; index< FR_MAX_DATA_INDEX; index++){
			pt->pf_data[i].frrdds[index] = (pt->fpd_vaddr + i*data_len + index*4);
		}
		out32(pt->pf_header[i].frdata,(uint32_t)(pt->fpd_paddr + i*data_len));	
	}
}

/** 
 * @brief Deactive an request of the output transfer.
 * @param dev: Pointer to fr_dev_t structure.
 * @return 0:success, EBUSY:error
 */
int fr_deactive_output(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
    // Check if the output transfer is already halted
    if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) == FR_BIT_CLEAR){
        return 0;
	}

    // Disable the output transfer handler
    clear_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OTE);
	return fr_check_state_of_bit(regs->data_xfer_stat.frots, FR_OTS_OTS,FR_BIT_CLEAR);
}

/** 
 * @brief Select output mode.
 * @param dev: Pointer to fr_dev_t structure.
 * @param dataMode: true: Select new data and status changed mode.
 * 					false: Select new data only mode.
 * @return none
 */
void fr_select_output_mode(fr_dev_t *dev, bool	dataMode)
{
	fr_regs_t *regs = &dev->regs;
	//Select output transfer condition
	if(dataMode){
		set_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OTCS);
	} else {
		clear_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OTCS);
	}
}

/** 
 * @brief Set header for URAM receive message buffers
 * @param dev: Pointer to fr_dev_t structure.
 * @param header: Pointer to fr_output_header_t structure.
 * @param msg: Pointer to fr_msg_ram_t structure.
 * @return none
 */
void fr_rx_uram_set_header(fr_dev_t *dev,fr_output_header_t *ph,fr_msg_ram_t *msg)
{
	uint32_t frrdhs2 = 0;
	
	frrdhs2 |= (((msg->payloadLength + 1)/2)<<FR_WRHS2_PLC_L);
	out32(ph->frrdhs2,frrdhs2);
}

int fr_uram_select_rx(fr_dev_t *dev,fr_msg_buffer_rx_t *msg)
{
	fr_regs_t *regs = &dev->regs;
	
	//Data section transfer required?
	if(msg->cmdMask & FR_CMD_MASK_DATA){
		//Set FRUOR.URDS to 1
		set_bit(regs->data_xfer_ctrl.fruor, FR_UOR_URDS);
	} else {
		//Set FRUOR.URDS to 0
		clear_bit(regs->data_xfer_ctrl.fruor, FR_UOR_URDS);
	}
	//Write FRUOR.UMBNR
	write_bits(regs->data_xfer_ctrl.fruor, FR_UOR_UMBNR_L, FR_UOR_UMBNR_H, msg->idx);
	
	return 0;
}
/** 
 * @brief Receive an message buffer is slected from uram.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: Pointer to struct io_devctl_t.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @return 0:success, !=0 :error
 */
int rcar_uram_select_rx(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	fr_regs_t *regs = &dev->regs;
	uint8_t *buf;
	fr_msg_buffer_rx_t *pmsg;
	
	if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) != FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Output transfer is disable", __func__);
		return ENOTSUP;
	}

	buf = (uint8_t *)msg;
	buf += sizeof(io_devctl_t);
	pmsg = (fr_msg_buffer_rx_t*)buf;	
	if(pmsg->type == FR_MSGBUF_TYPE_TX){
		return ENOSYS;
	}

	if (fr_uram_select_rx(dev, pmsg)){
		return EBUSY;
	}
	if(fr_check_state_of_bit(regs->data_xfer_stat.frots, FR_OTS_UORP,FR_BIT_SET)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: wait User output transfer request pending timeout. frots=0x%08x\n",__func__,in32(regs->data_xfer_stat.frots));
		return EBUSY;
	}	
	return  fr_rx_replay_data(dev,ctp,msg,ocb->ioflag);	
}

/** 
 * @brief devctl:  Receive many message buffers by using URAM.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: A pointer to the buffer used to receive the message from the client.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @return 0:success, !=0 :error
 */
int rcar_rx_uram(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	fr_regs_t *regs = &dev->regs;
	fr_poc_state pocState = 0;
	uint8_t *buf;
	fr_msg_buffer_rx_t *pmsg;
	
    if (msg->i.nbytes <= 0) {
        _IO_SET_READ_NBYTES(ctp, 0);
        return _RESMGR_NPARTS(0);
    }
	if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) != FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Output transfer is disable", __func__);
		return EINVAL;
	}
	fr_get_pocs_state(regs,&pocState);
	if(pocState != FR_STATE_NORMAL_ACTIVE){
		fr_slogf(FR_SLOG_DEBUG1,"%s: FlexRay is not in STATE_NORMAL_ACTIVE. pocState=%d", __func__,pocState);
		return EINVAL;
	}	

	buf = (uint8_t *)msg;
	buf += sizeof(io_devctl_t);
	pmsg = (fr_msg_buffer_rx_t*)buf;
	if(pmsg->type == FR_MSGBUF_TYPE_TX){
		return ENOSYS;
	}	
	return  fr_rx_replay_data(dev,ctp,msg,ocb->ioflag);	
}
