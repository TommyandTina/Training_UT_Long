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
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <devctl.h>
#include <unistd.h>
#include <atomic.h>
#include <sys/procmgr.h>
#include "rcar-flexray.h"

FRCLIENTWAITQ *client_queue_create(int waitq_size)
{
	FRCLIENTWAITQ *wl = NULL;
	FRCLIENTWAIT * elems = NULL;
	FRCLIENTWAIT * head = NULL;
	int i = 0 ;

	wl = (FRCLIENTWAITQ*)malloc(sizeof(FRCLIENTWAITQ));
	if(NULL==wl){
		return NULL;
	}
	memset(wl,0,sizeof(FRCLIENTWAITQ));
	if (waitq_size){
		wl->elems = malloc(sizeof(FRCLIENTWAIT)*waitq_size); 
		if(NULL==wl->elems){
			free(wl); 
			return NULL;
		}
		wl->cnt = waitq_size;
		elems = wl->elems;
		head = elems;
		wl->head = head;
		elems++;
		for(i=0;i< waitq_size -1; i++){
			head->next = elems;
			elems++;
			head++;
		}
		head->next = NULL;
		wl->tail = head;
	}else{
		wl->head = head;
		wl->tail = elems;
		wl->cnt = 0;
	}
	return wl;
}

FRCLIENTWAIT *client_dequeue(FRCLIENTWAITQ *wl)
{
	FRCLIENTWAIT  *wq = NULL;

	InterruptLock(&wl->lock);

	if (wl->cnt){
		wq = wl->head;
		wl->head = wq->next;
		if(wl->cnt==1) {
			wl->tail = NULL;
		}
		wl->cnt--;
	}else{
		wq = NULL;
	}

	InterruptUnlock(&wl->lock);
	return wq;
}

void client_queue(FRCLIENTWAITQ *wl,FRCLIENTWAIT *wq)
{
	wq->next = NULL;
	InterruptLock(&wl->lock);
	if(wl->cnt) {
		wl->tail->next = wq;
		wl->tail = wq;
	} else {
		wl->tail = wq;
		wl->head = wq;
	}
	wl->cnt++;
	InterruptUnlock(&wl->lock);
}

frmsg_list_t *msg_list_create(unsigned nmsg)
{
	int  i = 0;
	frmsg_list_t *ml = NULL;
	frmsg_t *msgs = NULL;
	frmsg_t *prev = NULL;
	frmsg_t *head = NULL;
	ml = (frmsg_list_t*)malloc(sizeof(frmsg_list_t));
	if (NULL==ml){
		return NULL;
	}
	memset(ml,0,sizeof(frmsg_list_t));
	if(nmsg){
		ml->msgs = (frmsg_t*)malloc(sizeof(frmsg_t)*nmsg);
		if(NULL==ml->msgs){
			free(ml);
			return NULL;
		}
		ml->nmsg = nmsg;
		ml->msgcnt = nmsg;
		msgs = ml->msgs;
		head = msgs;
		ml->head = head;
		msgs++;
		for(i=0;i< nmsg -1; i++){
			head->prev = prev;
			head->next = msgs;
			prev = head;
			msgs++;
			head++;
		}
		head->prev = prev;
		head->next = NULL;
		ml->tail = head;
	}else{
		ml->msgcnt = 0;
		ml->msgs = NULL;
		ml->head = NULL;
		ml->tail = NULL;
	}

	return ml;
}

void msg_list_destroy(frmsg_list_t *ml)
{
	if(ml){
		free(ml->msgs);
		free(ml);
	}
}

void client_queue_destroy(FRCLIENTWAITQ *wl)
{
	if(wl){
		free(wl->elems);
		free(wl);
	}
}


frmsg_t *frmsg_dequeue_element(frmsg_list_t *ml)
{
	frmsg_t *fr_msg = NULL; 
	InterruptLock(&ml->lock);
	if(ml->msgcnt){
		fr_msg = ml->head;
		ml->head = ml->head->next;
		if(ml->msgcnt==1){
			ml->tail = NULL;
		}
		else{
			ml->head->prev = NULL;
		}
		ml->msgcnt--;
	}
	InterruptUnlock(&ml->lock);
	return fr_msg;
}

void frmsg_queue_element(frmsg_list_t *ml,frmsg_t *queue_elem)
{
	queue_elem->next = NULL;
	InterruptLock(&ml->lock);
	if(ml->msgcnt){
		frmsg_t *tail = ml->tail;
		queue_elem->prev = tail;
		tail->next = queue_elem;
		ml->tail = queue_elem;
	}else{
		queue_elem->prev = NULL;
		ml->tail = queue_elem;
		ml->head = queue_elem;
	}
	ml->msgcnt++;

	InterruptUnlock(&ml->lock);
}

int fr_rx_replay_data(fr_dev_t *dev,resmgr_context_t *ctp,io_devctl_t *msg,int ioflag)
{
	int nparts = 1;
	int len = 0;
	frmsg_t *msg_queue = NULL;
	
	do{
		msg_queue = frmsg_dequeue_element(dev->rx_msg_queue);
		if(msg_queue){
			frmsg_queue_element(dev->rx_free_queue, msg_queue);
			len = sizeof(fr_msg_buffer_rx_t);
			memset(&msg->o, 0, sizeof(msg->o));
			msg->o.nbytes = 0;
			msg->o.ret_val = EOK;
			SETIOV(ctp->iov, &msg->o, sizeof(msg->o));
			SETIOV(ctp->iov + 1, &msg_queue->msg.rx_msg, len);
			nparts++;
			return MsgReplyv(ctp->rcvid, EOK, ctp->iov, nparts);
		}
		
	}while(NULL != msg_queue);

	if(ioflag & O_NONBLOCK){
		return EAGAIN;
	}else{
		FRCLIENTWAIT *wq = client_dequeue(dev->rx_wait_free_queue);
		if(wq){
		  wq->rcvid= ctp->rcvid;
		  client_queue(dev->rx_wait_client_queue,wq);
		  return _RESMGR_NOREPLY;
		}
	}
	return _RESMGR_NOREPLY;
}

int fr_tx_replay_data(iov_t iov[],io_devctl_t *msg,int rcvid,fr_msg_ram_t *pmsg)
{
	int nparts = 1;
	int len = 0;

	len = sizeof(fr_msg_ram_t);
	memset(&msg->o, 0, sizeof(msg->o));
	msg->o.nbytes = 0;
	msg->o.ret_val = EOK;
	SETIOV(iov, &msg->o, sizeof(msg->o));
	SETIOV(iov + 1, pmsg, len);
	nparts++;
	return MsgReplyv(rcvid, EOK, iov, nparts);
}

int  io_unblock_rx(fr_dev_t *dev,int rcvid)
{
	FRCLIENTWAIT *i = NULL;
	FRCLIENTWAIT *tmp = NULL;
	FRCLIENTWAITQ *wcq = NULL;
	wcq = (FRCLIENTWAITQ *)dev->rx_wait_client_queue;
	
	if(NULL==wcq->head){
		return EINTR;
	}
	if(wcq->head->rcvid != rcvid){
		for(i = wcq->head ; i; i = wcq->head->next){
			tmp = i->next;
			if (!tmp){
				break;
			}
			if(tmp->rcvid == rcvid){
				if( wcq->head == tmp){
					goto end_for_rx;
				}
				tmp = wcq->tail;
				i->next = tmp->next;
				if(tmp){
					wcq->tail = i;
				}
				goto end_rx;
			}
		}
		return EINTR;
	}
	tmp = wcq->head;
end_for_rx:
	wcq->head = tmp->next;
	if (!tmp->next){
		wcq->tail = NULL;
	}
end_rx:
	--wcq->cnt;
	client_queue(dev->rx_wait_free_queue, tmp);
	return EINTR;
}

int  io_unblock_tx(fr_dev_t *dev,int rcvid)
{
	FRCLIENTWAIT *i = NULL;
	FRCLIENTWAIT *tmp = NULL;
	FRCLIENTWAITQ *wcq = NULL;
	
	wcq = (FRCLIENTWAITQ *)dev->tx_wait_client_queue;
	if(NULL==wcq->head){
		return EINTR;
	}
	if(wcq->head->rcvid != rcvid){
		for(i = wcq->head ; i; i = wcq->head->next){
			tmp = i->next;
			if (!tmp){
				break;
			}
			if(tmp->rcvid == rcvid){
				if( wcq->head == tmp){
					goto end_for_tx;
				}
				tmp = wcq->tail;
				i->next = tmp->next;
				if(tmp){
					wcq->tail = i;
				}
				goto end_tx;
			}
		}
		return EINTR;
	}
	tmp = wcq->head;
end_for_tx:
	wcq->head = tmp->next;
	if (!tmp->next){
		wcq->tail = NULL;
	}
end_tx:
	--wcq->cnt;
	client_queue(dev->tx_wait_free_queue, tmp);

	return EINTR;	
}

int  io_unblock(resmgr_context_t *ctp, io_pulse_t *msg, RESMGR_OCB_T *ocb)
{
	int status = 0;
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	
	status = io_unblock_rx(dev,ctp->rcvid);
	status |= io_unblock_tx(dev,ctp->rcvid);
	
	return status;
}

int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
	int status = 0; 
	int nbytes = 0;
	frmsg_t *txmsg = NULL;
	int	nonblock = 0;
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	
    if ((status = iofunc_write_verify(ctp, msg, ocb, &nonblock)) != EOK){
        return status;	
	}
	
	if((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE){
		return ENOSYS;
	}
	
	nbytes = msg->i.nbytes;
    if (nbytes <= 0) {
        _IO_SET_WRITE_NBYTES(ctp, 0);
        return _RESMGR_NPARTS(0);
    }

	if(nbytes > sizeof(fr_msg_ram_t)){
		return EINVAL;
	}
	
	txmsg = frmsg_dequeue_element(dev->tx_free_queue);
	if(txmsg){
		if(txmsg->msg.tx_msg.type==FR_MSGBUF_TYPE_TX){
			status = resmgr_msgread(ctp, &txmsg->msg.tx_msg, sizeof(fr_msg_ram_t), sizeof(msg->i));
			frmsg_queue_element(dev->tx_msg_queue, txmsg);
			if(dev->mode){
				return fr_transmit(dev);
			}else{
				return fr_uram_transmit(dev);		
			}
			dev->attr.flags |= (IOFUNC_ATTR_CTIME | IOFUNC_ATTR_MTIME);
			ctp->status = status;
		}else{
			return ENOTSUP;
		}
	}else{
		return EAGAIN;
	}

	return status;
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
	int status; 
	ssize_t write_size = 0;
	int nbytes = 0;
	int	nonblock = 0;
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	
	if((status = iofunc_read_verify(ctp, msg, ocb, &nonblock)) != EOK) {
		return status;
	}
	nbytes = msg->i.nbytes;
	if (nbytes==0){
		ctp->status = 0;
		return nbytes;
	}
	if (nbytes < sizeof(fr_msg_buffer_rx_t)){
		return EINVAL;
	}
	frmsg_t *rx_msg = frmsg_dequeue_element(dev->rx_msg_queue);
	if(rx_msg){
		if(rx_msg->msg.rx_msg.type==FR_MSGBUF_TYPE_TX){
			return ENOTSUP;
		}
		write_size = resmgr_msgwrite(ctp,(const void*)&rx_msg->msg.rx_msg,sizeof(fr_msg_buffer_rx_t),0);
		frmsg_queue_element(dev->rx_free_queue,rx_msg);
		dev->attr.flags |= IOFUNC_ATTR_ATIME;
		return MsgReply(ctp->rcvid, write_size, NULL,0);	
	}else{
		FRCLIENTWAIT  *wq = client_dequeue(dev->rx_wait_free_queue);
		if(!wq) {
			return EAGAIN;
		}
		wq->rcvid = ctp->rcvid;
		client_queue(dev->rx_wait_client_queue,wq);
	}
	return(_RESMGR_NOREPLY);
}

int event_pulse_handler(message_context_t * ctp,int code,unsigned flags,void *handle)
{
	int cnt = 0;
	fr_dev_t *dev = (fr_dev_t *)handle;
	FRCLIENTWAIT *wq = NULL;
	if(dev->frsir & (1<<FR_SIR_SWE)){
		atomic_clr(&dev->frsir,(1<<FR_SIR_SWE));		
		if(dev->intr_timer){
			if(dev->timer_start){
				if(dev->timer_coid == -1){
					dev->timer_coid = name_open( FR_V3U_NAME_TIMER,0);
				}
				if(dev->timer_coid != -1) {
					MsgSendPulse(dev->timer_coid, dev->priority, FR_PULSE_EVENT_STOPWATCH,0);
				}
			}
		}
	}				
	if((dev->frsir & (1<<FR_SIR_TI0)) || (dev->frsir & (1<<FR_SIR_TI1))){
		if(dev->frsir & (1<<FR_SIR_TI0)){
			atomic_clr(&dev->frsir,(1<<FR_SIR_TI0));
		}
		if(dev->frsir & (1<<FR_SIR_TI1)){
			atomic_clr(&dev->frsir,(1<<FR_SIR_TI1));
		}	
		if(dev->intr_timer){
			if(dev->timer_start){
				if(dev->timer_coid == -1){
					dev->timer_coid = name_open( FR_V3U_NAME_TIMER,0);
				}
				if(dev->timer_coid != -1) {
					MsgSendPulse(dev->timer_coid, dev->priority, FR_PULSE_EVENT_TIMER,0);
				}
			}
		}
	}
	if(dev->rx_msg_queue->msgcnt) {
		cnt = 0;						
		do {
			wq = client_dequeue(dev->rx_wait_client_queue);
			if(!wq) {
				break;
			}
			cnt++;
			resmgr_msg_again(ctp, wq->rcvid);	
			client_queue(dev->rx_wait_free_queue,wq);
		} while(dev->rx_msg_queue->msgcnt > cnt);
	}
	return 0;
}

int fr_resmgr_init_device(fr_dev_t *dev)
{ 
	ThreadCtl( _NTO_TCTL_IO, 0 );

	dev->tx_free_queue = msg_list_create(dev->msgq_size);
	if(NULL==dev->tx_free_queue){
		fprintf(stderr,"Unable to allocate Tx message queue elements.\n");
		return -1;
	}
	dev->rx_free_queue = msg_list_create(dev->msgq_size);
	if(NULL==dev->rx_free_queue){
		free(dev->tx_free_queue);
		fprintf(stderr,"Unable to allocate Rx message queue elements.\n");
		return -1;
	}	
	dev->tx_msg_queue = msg_list_create(0);
	if(NULL==dev->tx_msg_queue){
		free(dev->tx_free_queue);
		free(dev->rx_free_queue);
		return -1;
	}
	dev->rx_msg_queue = msg_list_create(0);
	if(NULL==dev->rx_msg_queue){
		free(dev->tx_free_queue);
		free(dev->rx_free_queue);
		free(dev->tx_msg_queue);
		return -1;
	}
	dev->tx_wait_free_queue = client_queue_create(dev->waitq_size);
	if(NULL==dev->tx_wait_free_queue){
		fprintf(stderr,"Unable to allocate wait free queue elements.\n");
		free(dev->tx_free_queue);
		free(dev->rx_free_queue);
		free(dev->tx_msg_queue);
		free(dev->rx_msg_queue);
		return -1;
	}
	dev->rx_wait_free_queue = client_queue_create(dev->waitq_size);
	if(NULL==dev->rx_wait_free_queue){
		fprintf(stderr,"Unable to allocate wait free queue elements.\n");
		free(dev->tx_free_queue);
		free(dev->rx_free_queue);
		free(dev->tx_msg_queue);
		free(dev->rx_msg_queue);
		free(dev->tx_wait_free_queue);
		return -1;
	}
	dev->tx_wait_client_queue = client_queue_create(0);
	if(NULL==dev->tx_wait_client_queue){
		fprintf(stderr,"Unable to allocate wait client queue elements.\n");
		free(dev->tx_free_queue);
		free(dev->rx_free_queue);
		free(dev->tx_msg_queue);
		free(dev->rx_msg_queue);
		free(dev->tx_wait_free_queue);
		free(dev->rx_wait_free_queue);
		return -1;
	}
	dev->rx_wait_client_queue = client_queue_create(0);
	if(NULL==dev->rx_wait_client_queue){
		fprintf(stderr,"Unable to allocate wait client queue elements.\n");
		free(dev->tx_free_queue);
		free(dev->rx_free_queue);
		free(dev->tx_msg_queue);
		free(dev->rx_msg_queue);
		free(dev->tx_wait_free_queue);
		free(dev->rx_wait_free_queue);
		free(dev->tx_wait_client_queue);
		return -1;
	}
	memset(&dev->tx_free_queue->lock, 0,sizeof(intrspin_t));
	memset(&dev->rx_free_queue->lock, 0,sizeof(intrspin_t));
	memset(&dev->tx_msg_queue->lock, 0,sizeof(intrspin_t));
	memset(&dev->rx_msg_queue->lock, 0,sizeof(intrspin_t));
	memset(&dev->tx_wait_free_queue->lock, 0,sizeof(intrspin_t));
	memset(&dev->tx_wait_client_queue->lock, 0,sizeof(intrspin_t));
	memset(&dev->rx_wait_free_queue->lock, 0,sizeof(intrspin_t));
	memset(&dev->rx_wait_client_queue->lock, 0,sizeof(intrspin_t));
	
	return 0;
}
