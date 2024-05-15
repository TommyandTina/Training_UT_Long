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
#include <sys/procmgr.h>
#include <sys/siginfo.h>
#include <atomic.h>
#include "rcar-flexray.h"
#include "flexray-regs.h"

int fr_intr_rx(fr_dev_t *dev,frmsg_t *rx_msg)
{	
	int i =0;
	int status = -1;
	fr_msg_buffer_rx_t *pmsg = (fr_msg_buffer_rx_t*)&rx_msg->msg.rx_msg;

	for(i=0; i<dev->msg_cfg.numberMessage;i++) {
		if((dev->msg_cfg.nmsg[i].type == FR_MSGBUF_TYPE_RX) && ( (!dev->msg_cfg.nmsg[i].sync) || (!dev->msg_cfg.nmsg[i].startup))){
			if(0==fr_check_data(dev,dev->msg_cfg.nmsg[i].idx)){
				pmsg->cmdMask = dev->msg_cfg.nmsg[i].cmdMask;
				pmsg->type = dev->msg_cfg.nmsg[i].type;
				pmsg->idx = dev->msg_cfg.nmsg[i].idx;
				status = fr_rx_msg_ram(dev,pmsg);
				break;	
			}
		}
	}
	return status;
}

int fr_intr_fifo_rx(fr_dev_t *dev,frmsg_t *rx_msg)
{	
	uint32_t	frfsr = 0;
	uint8_t		rffl = 0;
	static uint8_t nfifo = 0;
	int status = -1;
	fr_regs_t *regs = &dev->regs;
	fr_msg_buffer_rx_t *pmsg = (fr_msg_buffer_rx_t*)&rx_msg->msg.rx_msg;
	
	if(nfifo==0){
		nfifo = FR_REG_BITS(regs->msg_buf_ctrl.frmrc, FR_MRC_FFB_L,FR_MRC_FFB_H);
	}
	frfsr = in32(regs->msg_buf_stat.frfsr);
	if(frfsr & (1<<FR_FSR_RFNE)){
		fr_get_fifo_fill_level(dev,&rffl);
		if(rffl>0){
			pmsg->cmdMask = FR_CMD_MASK_HEADER|FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
			pmsg->type = FR_MSGBUF_TYPE_FIFO;	
			status = fr_rx_fifo_msg_ram(dev,pmsg,nfifo);
			nfifo = nfifo + 1;
		}			

	}
	if(dev->msg_lcb==nfifo){
		nfifo = 0;
	}
	return status;
}

int fr_intr_rx_uram(fr_dev_t *dev,frmsg_t *rx_msg)
{	
	int i = 0;
	int status = -1;
	fr_msg_buffer_rx_t *pmsg = (fr_msg_buffer_rx_t*)&rx_msg->msg.rx_msg;
	for(i=0; i<dev->msg_cfg.numberMessage;i++) {
		if((dev->msg_cfg.nmsg[i].type == FR_MSGBUF_TYPE_RX) && ( (!dev->msg_cfg.nmsg[i].sync) || (!dev->msg_cfg.nmsg[i].startup))){
			if(0==fr_uram_check_newdata(dev,dev->msg_cfg.nmsg[i].idx)){
				pmsg->cmdMask = dev->msg_cfg.nmsg[i].cmdMask;
				pmsg->type = dev->msg_cfg.nmsg[i].type;
				pmsg->idx = dev->msg_cfg.nmsg[i].idx;
				fr_rx_uram(dev,pmsg);
				status = 0;
				break;	
			}
		}
	}
	return status;
}

int fr_intr_fifo_rx_uram(fr_dev_t *dev,frmsg_t *rx_msg)
{	
	uint8_t fifo_ffl;
	int status = -1;	
	fr_regs_t *regs = &dev->regs;
	fr_msg_buffer_rx_t *pmsg = (fr_msg_buffer_rx_t*)&rx_msg->msg.rx_msg;
	
	fr_uram_get_fifo_fill_level(dev,&fifo_ffl);	
	if(fifo_ffl>0){
		if(in32(regs->data_xfer_stat.frots) & (1<<FR_OTS_FDA)){
			pmsg->type = FR_MSGBUF_TYPE_FIFO;	
			fr_fifo_rx_uram(dev,pmsg);
			status = 0;	
		}
	}
	return status;
}

void fr_set_error(fr_dev_t *dev, uint32_t freir,uint32_t frmhds,uint32_t fraes)
{
	switch(freir){
	case (1<<FR_EIR_PEMC): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_PEMC);
		break;	
	case (1<<FR_EIR_CNA): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_CNA);
		break;
	case (1<<FR_EIR_SFBM): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_SFBM);
		break;
	case (1<<FR_EIR_SFO): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_SFO);
		break;
	case (1<<FR_EIR_CCF): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_CCF);
		break;
	case (1<<FR_EIR_CCL): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_CCL);
		break;
	case (1<<FR_EIR_AERR): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_AERR);
		break;	
	case (1<<FR_EIR_EFA): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_EFA);
		break;
	case (1<<FR_EIR_IIBA): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_IIBA);
		break;
	case (1<<FR_EIR_IOBA): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_IOBA);
		break;
	case (1<<FR_EIR_MHF): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_MHF);
		break;
	case (1<<FR_EIR_EDA): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_EDA);
		break;
	case (1<<FR_EIR_LTVA): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_LTVA);
		break;
	case (1<<FR_EIR_TABA): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_TABA);
		break;
	case (1<<FR_EIR_EDB): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_EDB);
		break;	
	case (1<<FR_EIR_LTVB): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_LTVB);
		break;	
	case (1<<FR_EIR_TABB): 
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_EIR_TABB);
		break;	
	}
	if(frmhds & (1<<FR_MHDS_AMR)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_MHDS_AMR);
	}
	if(frmhds & (1<<FR_MHDS_ATBF1)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_MHDS_ATBF1);
	}
	if(frmhds & (1<<FR_MHDS_ATBF2)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_MHDS_ATBF2);
	}	
	if(frmhds & (1<<FR_MHDS_FMBD)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_MHDS_FMBD);
	}
	if(frmhds & (1<<FR_MHDS_MFMB)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_MHDS_FMBD);
	}	
	if(fraes & FR_AES_EIDX_MASK){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_AES_EIDX);
	}
	if(fraes & (1<<FR_AES_IAE)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_AES_IAE);
	}
	if(fraes & (1<<FR_AES_FAE)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_AES_OAE);
	}
	if(fraes & (1<<FR_AES_MAE)){
		atomic_set(&dev->fr_err.drvr1,FR_DRVR1_AES_MAE);
	}	
}

const struct sigevent * fr_isr(void *arg, int id)
{
	fr_dev_t *dev = (fr_dev_t *)arg;
    fr_regs_t *regs = &dev->regs;
	struct sigevent *event = NULL;
	frmsg_t *fr_msg = NULL;
	uint32_t fros;
	uint32_t frsir = 0;
	uint32_t frsir_mask = FR_FRSIES_ENABLE;
	uint32_t freir = 0;
	uint32_t frots = 0;
	uint32_t fraes = 0;
	uint32_t frmhds = 0;
	
	unsigned cycle_counter = 0;
	unsigned macrotick = 0;
	
	frsir_mask |= (1<<FR_SIR_SWE)|(1<<FR_SIES_TI0E)|(1<<FR_SIES_TI1E);

	// Receive Interrupt
	fros  = in32(regs->ope.fros);
	frsir = in32(regs->intr.frsir);
	freir = in32(regs->intr.freir);
	frots = in32(regs->data_xfer_stat.frots);
	fraes = in32(regs->data_xfer_stat.fraes);
	frmhds = in32(regs->msg_buf_stat.frmhds);
	
	if(fros & (FR_OS_TIS_MASK<<FR_OS_T0IS)){
		out32(regs->ope.fros, fros & (FR_OS_TIS_MASK<<FR_OS_T0IS));
	}
	if(freir){
		out32(regs->intr.freir, freir);
	}
	if(fraes){
		out32(regs->data_xfer_stat.fraes,fraes);
	}
	if(frmhds & FR_MHDS_ERR){
		out32(regs->msg_buf_stat.frmhds,FR_MHDS_ERR);
	}
	if(frsir){
		out32(regs->intr.frsir, frsir);
	}
	if(frots & (1<<FR_OTS_OTIS)){
		out32(regs->data_xfer_stat.frots,(1<<FR_OTS_OTIS));	
	}
	if(frots & (1<<FR_OTS_FIS)){
		out32(regs->data_xfer_stat.frots,(1<<FR_OTS_FIS));
	}
	
	if((freir & FR_EIR_ENABLE) || (fraes & (1<<FR_AES_IAE)) || (fraes & (1<<FR_AES_OAE))
		|| (fraes & (1<<FR_AES_FAE)) || (fraes & (1<<FR_AES_MAE))){	
		fr_set_error(dev,freir,frmhds,fraes);
	}else{
		atomic_clr(&dev->fr_err.drvr1,0xFFFFFFFF);
	}

	if(frsir & (1<<FR_SIR_TI0)){
		cycle_counter = in32(regs->cc_stat.frmtccv) & FRMTCCV_CCV_MASK;
		macrotick = in32(regs->cc_stat.frmtccv) & FRMTCCV_MTV_MASK;
		atomic_set(&dev->cycle_counter,cycle_counter);
		atomic_set(&dev->macrotick,macrotick);
	}	
	if(frsir & (1<<FR_SIR_TI1)){
		cycle_counter = in32(regs->cc_stat.frmtccv) & FRMTCCV_CCV_MASK;
		macrotick = in32(regs->cc_stat.frmtccv) & FRMTCCV_MTV_MASK;
		atomic_set(&dev->cycle_counter,cycle_counter);
		atomic_set(&dev->macrotick,macrotick);
	}
	
	if(frsir){
		if(frsir & frsir_mask){
			atomic_set(&dev->frsir,(frsir & frsir_mask));
		}
		if(frsir & (1<<FR_SIR_SWE)){
			atomic_clr(&dev->smtv,0xFFFFFFFF);
			atomic_clr(&dev->sccv,0xFFFFFFFF);
			atomic_clr(&dev->sscva,0xFFFFFFFF);
			atomic_clr(&dev->sscvb,0xFFFFFFFF);
			atomic_set(&dev->smtv,FR_REG_BITS(regs->timer.frstpw1,FR_STPW1_SMTV_L,FR_STPW1_SMTV_H));
			atomic_set(&dev->sccv,FR_REG_BITS(regs->timer.frstpw1,FR_STPW1_SCCV_L,FR_STPW1_SCCV_H));
			atomic_set(&dev->sscva,FR_REG_BITS(regs->timer.frstpw2,FR_STPW2_SSCVA_L,FR_STPW2_SSCVA_H));
			atomic_set(&dev->sscvb,FR_REG_BITS(regs->timer.frstpw2,FR_STPW2_SSCVB_L,FR_STPW2_SSCVB_H));	
		}			
	}
	if(dev->mode){		
		if(frsir & (1<<FR_SIR_RXI)){
			fr_msg = frmsg_dequeue_element(dev->rx_free_queue);
			if (!fr_msg){
				fr_msg = frmsg_dequeue_element(dev->rx_msg_queue);
				if (!fr_msg){
					return event;
				}
			}					
			if(0==fr_intr_rx(dev,fr_msg)){
				// Add populated element to the receive queue
				frmsg_queue_element(dev->rx_msg_queue, fr_msg);
				if (dev->rx_wait_client_queue->cnt){
					event = &dev->event;
				}
			}
						
		}
		if(frsir & (1<<FR_SIR_RFNE)){	
			fr_msg = frmsg_dequeue_element(dev->rx_free_queue);
			if (!fr_msg){
				fr_msg = frmsg_dequeue_element(dev->rx_msg_queue);
				if (!fr_msg){
					return event;
				}
			}					
			if(0==fr_intr_fifo_rx(dev,fr_msg)){	
				/* Add populated element to the receive queue */
				frmsg_queue_element(dev->rx_msg_queue, fr_msg);

				if (dev->rx_wait_client_queue->cnt){
					event = &dev->event;
				}
			}
		}
		
		if(frsir & (1<<FR_SIR_TIBC)){
			// Transmit next message if there is existing messages
			fr_msg = frmsg_dequeue_element(dev->tx_msg_queue);
			if (fr_msg){
				frmsg_queue_element(dev->tx_msg_queue,fr_msg);
				// Transmit next message
				fr_transmit(dev);
			}
			// Block clients if need
			if (dev->tx_wait_client_queue->cnt){
				event = &dev->event;
			}
		}
	}else{	
		if(frots & (1<<FR_OTS_OTIS)){		
			fr_msg = frmsg_dequeue_element(dev->rx_free_queue);
			if (!fr_msg){
				fr_msg = frmsg_dequeue_element(dev->rx_msg_queue);
				if (!fr_msg){
					return event;
				}
			}
			if(0==fr_intr_rx_uram(dev,fr_msg)){
				/* Add populated element to the receive queue */
				frmsg_queue_element(dev->rx_msg_queue, fr_msg);
				if (dev->rx_wait_client_queue->cnt){
					event = &dev->event;
				}
			}
		}			
		if(frots & (1<<FR_OTS_FIS)){
			fr_msg = frmsg_dequeue_element(dev->rx_free_queue);
			if (!fr_msg){
				fr_msg = frmsg_dequeue_element(dev->rx_msg_queue);
				if (!fr_msg){
					return event;
				}
			}
			if(0==fr_intr_fifo_rx_uram(dev,fr_msg)){
				// Add populated element to the receive queue
				frmsg_queue_element(dev->rx_msg_queue, fr_msg);
				if (dev->rx_wait_client_queue->cnt){
					event = &dev->event;
				}
			}
		}	
		if(frsir & (1<<FR_SIR_TIBC)){
			fr_msg = frmsg_dequeue_element(dev->tx_msg_queue);
			if (fr_msg){
				frmsg_queue_element(dev->tx_msg_queue,fr_msg);
				fr_uram_transmit(dev);
			}
			if (dev->tx_wait_client_queue->cnt){
				event = &dev->event;
			}
		}			
	}
	if( (frsir & (1<<FR_SIR_TI1)) || (frsir & (1<<FR_SIR_TI0)) || (frsir & (1<<FR_SIR_SWE))){	
		event = &dev->event;
	}
	return event;
}
