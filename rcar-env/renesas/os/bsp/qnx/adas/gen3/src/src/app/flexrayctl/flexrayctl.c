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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/dispatch.h>
#include <hw/config.h>
#include <hw/flexray.h>

#define FR_FIFO_TEST   				1

char devname[NAME_MAX];
fr_msg_buffer_rx_t uram_rx_user_msg;
fr_msg_ram_t tx_user_msg;
fr_stopwatch_value_t fr_stop_watch;
int oflag = 0;
int node = FR_NODE1;
int buffer_index = -1;
uint8_t fr_fifo_test = 0;
bool is_fr_timer_stop = false;
bool is_tx = false, is_rx = false;
bool is_timer_tx = false, is_timer_rx = false;
char path[_POSIX_PATH_MAX];
name_attach_t *fr_name = NULL;
int c;
int errflag = 0;
char *options, *value;
extern char *optarg;
	
char *r_opts[] = {
#define FR_NODE    		0
						"node",
#define FR_INDEX   		1
						"index",
#define FR_FIFO   	    2
						"fifo",
#define FR_TIMER   		3
						"timer",						
						NULL
};

char *timer_opts[] = {
#define FR_NODE    	0
					"node",
#define FR_RX   	1
					"rx",
#define FR_TX    	2
					"tx",
#define TIMER_FIFO  3
					"fifo",					
					NULL
};

char * msg_buf_err_drvr1[FR_DRVR1_MAX] = {
"Error mode has changed",
"CHI command not accepted",
"Less than the required minimum of sync frames received",
"More sync frames received than configured",
"Clock correction failed",
"CHI Command Locked",
"Access error",
"Receive FIFO Overrun",
"Empty FIFO Access",
"Illegal Input Buffer Access",
"Illegal Output buffer Access",
"Message Handler failure",
"Error Detected on Channel A",
"Latest Transmit Violation Channel A",
"Transmission Across Boundary Channel A",
"Error Detected on Channel B",
"Latest transmit violation detected on channel B",
"Transmission across slot boundary detected on channel B",
"Access Error Message RAM",
"Access Error Transient buffer RAM A",
"Access Error Transient buffer RAM B",
"Faulty Message Buffer Detected",
"Multiple Faulty Message Buffers detected",
"Error InDeX. Data structure pointer index number",
"Input transfer Access Error",
"Output transfer Access Error",
"FIFO transfer Access Error",
"Multiple Access Errors",
};

void fr_print_stopwatch_value(fr_stopwatch_value_t *value)
{
	fprintf(stdout,"Stop Watch Captured Macrotick Value=%u\n",value->smtv);
	fprintf(stdout,"Stop Watch Captured Cycle Counter Value=%u\n",value->sccv);	
	fprintf(stdout,"Stop Watch Captured Slot Counter Value Channel B=%u\n",value->sscvb);
	fprintf(stdout,"Stop Watch Captured Slot Counter Value Channel A=%u\n",value->sscva);	
}

int devctl_get_stopwatch_value(fr_stopwatch_value_t *stop_watch)
{
	int fd = -1;
	int status = 0;

	fd = open(devname, oflag);

	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed\n",__func__,devname);
		return -1;
	}
	status = devctl(fd, FR_DEVCTL_GET_STOPWATCH_VALUE,stop_watch, sizeof(fr_stopwatch_value_t), NULL);

	if(status){
		fprintf(stderr,"%s:Failed: error %s\n",__func__, strerror(status));
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
	
}
void fr_print_error_value(fr_devctl_error_t* perr)
{
	uint8_t i =0 , nerr = 0;
	for(i=0; i<FR_DRVR1_MAX;i++){
		if((1<<i) & perr->drvr1){
			fprintf(stdout,"%s \n",msg_buf_err_drvr1[i]);
			nerr++;
		}
	}
	if(nerr==0){
		fprintf(stdout,"No error\n");
	}
}

int fr_get_error(void)
{		
	int fd = -1;
	int status = 0;
	fr_devctl_error_t derror;

	fd = open(devname, oflag);

	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed\n",__func__,devname);
		return -1;
	}
	status = devctl(fd, FR_DEVCTL_ERROR,&derror, sizeof(fr_devctl_error_t), NULL);

	if(status){
		fprintf(stderr,"%s:Failed: error %s\n",__func__, strerror(status));
		close(fd);
		return -1;
	}
	fr_print_error_value(&derror);
	close(fd);
	return 0;
}

/**
 * @brief Print one message buffer to terminal
 * @param pmsg: pointer to fr_msg_buffer_rx_t structure 
 * @return none
 */
void print_rx_msg_buffer(fr_msg_buffer_rx_t *pmsg)
{		
	int i = 0;
	fprintf(stdout,"Frame ID                   = 0x%x \n",pmsg->frameId);
	fprintf(stdout,"Header CRC                 = 0x%x \n",pmsg->headerCrc);
	fprintf(stdout,"payloadLength              = %u \n",pmsg->payloadLength);
	fprintf(stdout,"Receive Cycle Count        = %u \n",pmsg->cycleCount);
	if(FR_RCI_A==pmsg->channel){
		fprintf(stdout,"Received on channel    : Receive on channel A \n");
	}else {
		fprintf(stdout,"Received on channel    : Receive on channel B \n");
	}		
	fprintf(stdout,"Startup Frame              = %u \n",pmsg->startup);
	fprintf(stdout,"Sync Frame                 = %u \n",pmsg->sync);
	fprintf(stdout,"Null Frame                 = %u \n",pmsg->nullFrame);
	fprintf(stdout,"Payload Preamble Indicator = %u \n",pmsg->ppi);
	fprintf(stdout,"Cycle Code                 = %u \n",pmsg->cycleCode);
	
	fprintf(stdout,"----------Data received----------\n");
	for(i =0 ;i <pmsg->payloadLength;i++) {
		fprintf(stdout,"%d ",pmsg->data[i]);
	}
	fprintf(stdout,"\n");
}

void print_usemsg(void)
{			
	fprintf(stdout,"-b        Use blocking mode.\n");
	fprintf(stdout,"-e        Print error information.\n");	
	fprintf(stdout,"-h        Print the usage of utility.\n");
	fprintf(stdout,"-n        Use nonblocking mode.\n");
	fprintf(stdout,"-r node,fifo,index \n");
	fprintf(stdout,"          Receive Flexray messages.\n");
	fprintf(stdout,"          Options specific to receive message buffers from node. These options include:\n");
	fprintf(stdout,"                      node=1   Receive message buffers from node 1\n");
	fprintf(stdout,"                      node=2   Receive message buffers from node 2\n");	
	fprintf(stdout,"                      fifo=0   Receive 14 message buffers\n");
	fprintf(stdout,"                      fifo=1   Receive fifo message buffers\n");
	fprintf(stdout,"                      index:   The message buffer index. This option is only availabe for fifo=0\n");
	fprintf(stdout,"                               node=1   index=[15...29]\n");
	fprintf(stdout,"                               node=2   index=[2...14]\n");	
	fprintf(stdout,"          For example:\n"); 
	fprintf(stdout,"                        Receive 14 message buffers from node 1:   flexrayctl -r node=1,fifo=0\n");
	fprintf(stdout,"                        Receive 14 message buffers from node 2:   flexrayctl -r node=2,fifo=0\n"); 
	fprintf(stdout,"                        Receive fifo message buffers from node 1: flexrayctl -r node=1,fifo=1\n");
	fprintf(stdout,"                        Receive fifo message buffers from node 2: flexrayctl -r node=2,fifo=1\n");	
	fprintf(stdout,"                        Receive message buffer 15 from node 1:    flexrayctl -r node=1,index=15\n");
	fprintf(stdout,"                        Receive message buffer 2  from node 2:    flexrayctl -r node=2,index=2\n");  	
	fprintf(stdout,"-t node,tx,rx\n");
	fprintf(stdout,"          Test timer and stopwatch.\n");
	fprintf(stdout,"          Options specific to test timer and stopwatch. These options include:\n");	
	fprintf(stdout,"                      node=1   Send message buffers to 1\n");
	fprintf(stdout,"                      node=2   Send message buffers to 2\n");
	fprintf(stdout,"                      rx:      Receive message buffers\n");
	fprintf(stdout,"                      tx:      Send message buffers\n");		
	fprintf(stdout,"          For example:  \n"); 
	fprintf(stdout,"                        Send 14 message buffers to node 1 and receive 14 message buffers from node 2: flexrayctl -t node=1,tx\n"); 
	fprintf(stdout,"                        Receive 14 message buffers from node 1 and send 14 message buffers to node 2: flexrayctl -t node=2,rx\n");
	fprintf(stdout,"-w node,fifo,index\n");
	fprintf(stdout,"          Send message buffers. Options specific to send message buffer. These options include:\n");	
	fprintf(stdout,"                      node=1   Send message buffers to 1\n");
	fprintf(stdout,"                      node=2   Send message buffers to 2\n");	
	fprintf(stdout,"                      fifo=0   Send 14 message buffers\n");
	fprintf(stdout,"                      fifo=2   Send 14 message buffers\n");	
	fprintf(stdout,"                      index:   The message buffer index. This option is only availabe for fifo=0\n");
	fprintf(stdout,"                               node=1   index=[2...14]\n");
	fprintf(stdout,"                               node=2   index=[15...29]\n");	
	fprintf(stdout,"          For example:  Send 14 message buffers to node 1: flexrayctl -w node=1,fifo=0\n"); 
	fprintf(stdout,"                        Send 14 message buffers to node 2: flexrayctl -w node=2,fifo=0\n"); 
	fprintf(stdout,"                        Send 14 message buffers to node 1: flexrayctl -w node=1,fifo=1\n");
	fprintf(stdout,"                        Send 14 message buffers to node 1: flexrayctl -w node=2,fifo=1\n");
	fprintf(stdout,"                        Send message buffer 2 to node 1:   flexrayctl -w node=1,index=2\n");
	fprintf(stdout,"                        Send message buffer 15 to node 2:  flexrayctl -w node=2,index=15\n"); 
	fprintf(stdout,"\n\n\n\n");
}

/**
 * @brief Receive message buffer from message ram
 * @param devname: device name to be used
 * @param pmsg: An pointer to fr_msg_ram_t structure
 * @param dcmd: command code
 * @return 0: success, !=0: error 
 */
int devctl_rx_msg(char* devname,fr_msg_ram_t *pmsg,int32_t dcmd)
{
	int fd = -1;
	int retry = 3;
	int status = EOK;
	fr_msg_buffer_rx_t msg;
	
	fd = open(devname, oflag);
	
	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed, error: %s\n",__func__,devname,strerror(errno));
		return -1;
	}
loop:
	if((pmsg->type == FR_MSGBUF_TYPE_RX)||(pmsg->type == FR_MSGBUF_TYPE_FIFO)){				
		msg.cmdMask = FR_CMD_MASK_HEADER|FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
		msg.idx = pmsg->idx;
		msg.type = pmsg->type;
		status = devctl(fd, dcmd, &msg, sizeof(fr_msg_buffer_rx_t),NULL);	
		if(status){
			close(fd);
			return status;
		}
	}
	if(retry==0){
		goto end;
	}
	if((msg.frameId==0)||(msg.frameId>2047)) {
		retry--;
		goto loop;
	}
end:	
	close(fd);
	print_rx_msg_buffer(&msg);
	return 0;
}

/**
 * @brief Receive message buffers from message ram
 * @param devname: device name to be used
 * @param cfg_msg: An array to fr_msg_ram_t structure
 * @param dcmd: command code
 * @param numberMessage: The number of message buffer
 * @return 0: success, !=0: error 
 */
int devctl_rx_msgs(char* devname,fr_msg_ram_t cfg_msg[],int32_t dcmd, uint8_t numberMessage)
{
	int fd = -1;
	int status = EOK;
	int i = 0;
	fr_msg_buffer_rx_t msg;
	
	fd = open(devname, oflag);
	
	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed, error: %s\n",__func__,devname,strerror(errno));
		return -1;
	}
	for(i=2;i<numberMessage;i++){
		if((cfg_msg[i].type != FR_MSGBUF_TYPE_TX)){				
			msg.cmdMask = FR_CMD_MASK_HEADER|FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
			msg.idx = cfg_msg[i].idx;
			msg.type = cfg_msg[i].type;
			msg.frameId = cfg_msg[i].frameId;
			status = devctl(fd, dcmd, &msg, sizeof(fr_msg_buffer_rx_t),NULL);	
			if(status){
				close(fd);
				return status;
			}
			print_rx_msg_buffer(&msg);
		}
	}
	close(fd);
	return 0;
}

/**
 * @brief Receive one message buffer from message RAM
 * @param devname: device name to be used
 * @param dcmd: command code
 * @param node: node is received
 * @param idx: The index of message 
 * @return 0: success, !=0: error 
 */
int devctl_uram_rx_msg(char* devname,int32_t dcmd, uint8_t node, uint8_t idx)
{
	int fd = -1;
	int status = EOK;
	fr_msg_buffer_rx_t msg;
	
	fd = open(devname, oflag);

	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed\n",__func__,devname);
		return -1;
	}
	if(node==FR_NODE1) {	
		msg.cmdMask = FR_CMD_MASK_HEADER|FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
		msg.idx = fr_cfg11[idx].idx;
		msg.type = fr_cfg11[idx].type;
		msg.frameId = fr_cfg11[idx].frameId;
	}	
	if(node==FR_NODE2) {
		msg.cmdMask = FR_CMD_MASK_HEADER|FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
		msg.idx = fr_cfg12[idx].idx;
		msg.type = fr_cfg12[idx].type;
		msg.frameId = fr_cfg12[idx].frameId;
	}	
	status = devctl(fd, dcmd, &msg, sizeof(fr_msg_buffer_rx_t),NULL);	
	if(status){
		close(fd);
		return status;
	}
	print_rx_msg_buffer(&msg);
	close(fd);
	return 0;
}
/**
 * @brief Send a message buffer to message ram
 * @param devname: device name to be used
 * @param pmsg: A pointer to fr_msg_ram_t structure
 * @param dcmd: command code
 * @return 0: success, !=0: error 
 */
int devctl_tx_msg_buf(char* devname,fr_msg_ram_t* pmsg, int32_t dcmd)
{
	int fd = -1;
	int status = EOK;
	int i = 0;
	fr_msg_ram_t msg;
	
	fd = open(devname, oflag);
	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed, error: %s\n",__func__,devname,strerror(errno));
		return fd;
	}
		
	if(pmsg->type == FR_MSGBUF_TYPE_TX){
		msg.cmdMask 			= FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
		msg.idx 				= pmsg->idx;
		msg.frameId 			= pmsg->frameId;
		msg.cycleCount 			= pmsg->cycleCount;
		msg.channel 			= pmsg->channel;
		msg.type 				= pmsg->type;	
		msg.mode 				= pmsg->mode;
		msg.ppi 				= pmsg->ppi;
		msg.sync 				= pmsg->sync;
		msg.startup 			= pmsg->startup;
		msg.payloadLength		= pmsg->payloadLength;			
		for(i =0;i <pmsg->payloadLength;i++) {
			msg.data[i] = pmsg->idx;
		}
		status = devctl(fd, dcmd, &msg, sizeof(fr_msg_ram_t),NULL);	
		if(status){
			close(fd);
			return status;
		}			
	}
	close(fd);
	return 0;
}

/**
 * @brief Send message buffers to message ram
 * @param devname: device name to be used
 * @param cfg_msg: An array to fr_msg_ram_t structure
 * @param dcmd: command code
 * @param numberMessage: The number of message buffer
 * @return 0: success, !=0: error 
 */
int devctl_tx_msg_bufs(char* devname,fr_msg_ram_t cfg_msg[], int32_t dcmd,uint8_t numberMessage)
{
	int fd = -1;
	int status = EOK;
	int i = 0, j = 0;
	fr_msg_ram_t msg;
	
	fd = open(devname, oflag);
	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed, error: %s\n",__func__,devname,strerror(errno));
		return fd;
	}
	for(i=2;i<numberMessage;i++){		
		if(cfg_msg[i].type == FR_MSGBUF_TYPE_TX){
			msg.cmdMask 			= FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
			msg.idx 				= cfg_msg[i].idx;
			msg.frameId 			= cfg_msg[i].frameId;
			msg.cycleCount 			= cfg_msg[i].cycleCount;
			msg.channel 			= cfg_msg[i].channel;
			msg.type 				= cfg_msg[i].type;	
			msg.mode 				= cfg_msg[i].mode;
			msg.ppi 				= cfg_msg[i].ppi;
			msg.sync 				= cfg_msg[i].sync;
			msg.startup 			= cfg_msg[i].startup;
			msg.payloadLength		= cfg_msg[i].payloadLength;			
			for(j =0;j <cfg_msg[i].payloadLength;j++) {
				msg.data[j] = cfg_msg[i].idx;
			}
			status = devctl(fd, dcmd, &msg, sizeof(fr_msg_ram_t),NULL);	
			if(status){
				close(fd);
				return status;
			}			
		}
	}
	
	close(fd);
	return 0;
}

/**
 * @brief Send one message buffer to message ram.
 * @param devname: device name to be used.
 * @param node: node in cluster.
 * @param idx: message buffer index.
 * @return 0: success, -1: error 
 */
int devctl_uram_tx_msg(char* devname,uint8_t node, int8_t idx)
{
	int fd = -1;
	int status = EOK;
	fr_msg_ram_t msg;
	
	fd = open(devname, oflag);
	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed\n",__func__,devname);
		return -1;
	}
	if(node==FR_NODE2){			
		msg.cmdMask 			= FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
		msg.idx 				= fr_cfg12[idx].idx;
		msg.frameId 			= fr_cfg12[idx].frameId;
		msg.cycleCount 			= fr_cfg12[idx].cycleCount;
		msg.channel 			= fr_cfg12[idx].channel;
		msg.type 				= fr_cfg12[idx].type;	
		msg.mode 				= fr_cfg12[idx].mode;
		msg.ppi 				= fr_cfg12[idx].ppi;
		msg.sync 				= fr_cfg12[idx].sync;
		msg.startup 			= fr_cfg12[idx].startup;
		msg.payloadLength		= fr_cfg12[idx].payloadLength;	
		for(int i =0;i <msg.payloadLength;i++) {
			msg.data[i] = i + 1;
		}
	}else{									
		msg.cmdMask 			= FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
		msg.idx 				= fr_cfg11[idx].idx;
		msg.frameId 			= fr_cfg11[idx].frameId;
		msg.cycleCount 		= fr_cfg11[idx].cycleCount;
		msg.channel 			= fr_cfg11[idx].channel;
		msg.type 				= fr_cfg11[idx].type;	
		msg.mode 				= fr_cfg11[idx].mode;
		msg.ppi 				= fr_cfg11[idx].ppi;
		msg.sync 				= fr_cfg11[idx].sync;
		msg.startup 			= fr_cfg11[idx].startup;
		msg.payloadLength		= fr_cfg11[idx].payloadLength;	
		for(int i =0;i <msg.payloadLength;i++) {
			msg.data[i] = idx;
		}
	}	
 
	status = devctl(fd, FR_DEVCTL_URAM_TX, &msg, sizeof(fr_msg_ram_t),NULL);	
	if(status){
		close(fd);
		return status;
	}
	close(fd);
	return 0;
}


/**
 * @brief Start flexray timer 
 * @param devname: device name to be used
 * @param fr_timer_id: flexray timer
 * @param dcmd: command code
 * @return 0: success, -1: error 
 */
int fr_timer_start(char* devname,uint32_t fr_timer_id,int32_t dcmd)
{
	int fd = -1;
	int status = EOK;
	uint32_t timer_id = fr_timer_id;
	fd = open(devname, oflag);
	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed\n",__func__,devname);
		return -1;
	}
	 
	status = devctl(fd, dcmd, &timer_id, sizeof(uint32_t),NULL);	

	if(status){
		fprintf(stderr,"%s:Failed: errno: %s\n",__func__,strerror(status));
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

/**
 * @brief Stop flexray timer 
 * @param devname: device name to be used
 * @param fr_timer_id: flexray timer
 * @param dcmd: command code
 * @return 0: success, -1: error 
 */
int fr_timer_stop(char* devname,uint32_t fr_timer_id,int32_t dcmd)
{
	int fd = -1;
	int status = EOK;
	uint32_t timer_id = fr_timer_id;
	fd = open(devname, oflag);
	if(fd == -1){
		fprintf(stderr, "%s: open device name %s failed\n",__func__,devname);
		return -1;
	}
	 
	status = devctl(fd, dcmd, &timer_id, sizeof(uint32_t),NULL);	

	if(status){
		fprintf(stderr,"%s:Failed: errno: %s\n",__func__,strerror(status));
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int fr_timer(void)
{ 
	struct _pulse	pulse;	
	int	            rcvid;
	uint8_t			index = 2;
	uint8_t			i = 15;
	uint8_t 		numberMessage = 0;
	fr_msg_ram_t* 	pmsgt = NULL;
	fr_msg_ram_t* 	pmsgr = NULL;
	while (1) {
		rcvid = MsgReceivePulse(fr_name->chid, &pulse, sizeof(pulse), NULL);
		if(rcvid== -1){
			continue;
		}
		switch (pulse.code) {
			case FR_PULSE_EVENT_TIMER:
			{
				numberMessage = 14;				
				if(index<=numberMessage){
					if(fr_fifo_test==0){
						if(node==FR_NODE1){
							pmsgt = &fr_cfg11[index];
							pmsgr = &fr_cfg11[i];
						}else{
							pmsgt = &fr_cfg12[i];
							pmsgr = &fr_cfg12[index];						
						}
					}else{
						if(node==FR_NODE1){
							pmsgt = &fr_cfg21[index];
							pmsgr = &fr_cfg21[i];
						}else{
							pmsgt = &fr_cfg22[i];
							pmsgr = &fr_cfg22[index];						
						}
					}						
					if(is_timer_tx){
						if(devctl_tx_msg_buf(devname,pmsgt,FR_DEVCTL_TX_MSG)){
							break;
						}
						if(devctl_rx_msg(devname,pmsgr,FR_DEVCTL_RX_MSG)){
							break;
						}						
					}else{
						if(devctl_rx_msg(devname,pmsgr,FR_DEVCTL_RX_MSG)){
							break;
						}
						if(devctl_tx_msg_buf(devname,pmsgt,FR_DEVCTL_TX_MSG)){
							break;
						}						
					}
					index++;
					i++;					
				}
				if(index>numberMessage){
					fr_timer_stop(devname,FR_TIMER0,FR_DEVCTL_STOP_TIMER);
					is_fr_timer_stop = true;
					return 0;
				}					
			}
			break;
			case FR_PULSE_EVENT_STOPWATCH:
				devctl_get_stopwatch_value(&fr_stop_watch);
				fr_print_stopwatch_value(&fr_stop_watch);
				break;				
		}
	}
	return 0;
}

int fr_timer_transmit(void)
{
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)){
		fprintf(stderr,"%s: ThreadCtl failed\n",__func__);
		exit( EXIT_FAILURE );

    }

	snprintf(path, sizeof(path),FR_V3U_NAME_TIMER);
	if ((fr_name = name_attach(NULL, path, 0)) == NULL) {
		printf( "%s: name_attach() failed (%s)", __func__, strerror(errno));
		exit( EXIT_FAILURE );

	}	
	
	if(fr_timer_start(devname,FR_TIMER0,FR_DEVCTL_START_TIMER)){
		fprintf(stdout,"Startup timer failed. This test mode does not support\n");
		exit( EXIT_FAILURE );

	}
	return fr_timer();
}
int start_app(void)
{
	int status = 0;
	if(is_tx){
		if(buffer_index==-1){
			fprintf(stdout,"Send data to node %d ",node);
			if(FR_NODE1==node){
				if(fr_fifo_test==0){
					if(devctl_tx_msg_bufs(devname,fr_cfg11,FR_DEVCTL_TX_MSG,FR_MBF_NUM)){
						fprintf(stdout,"failed\n");
					}else{
						fprintf(stdout,"is successful\n");
					}					
				}else{
					if(devctl_tx_msg_bufs(devname,fr_cfg21,FR_DEVCTL_TX_MSG,FR_MBF_NUM)){
						fprintf(stdout,"failed\n");
					}else{
						fprintf(stdout,"is successful\n");
					}
				}
			}else{
				if(fr_fifo_test==0){
					if(devctl_tx_msg_bufs(devname,fr_cfg12,FR_DEVCTL_TX_MSG,FR_MBF_NUM)){
						fprintf(stdout,"failed\n");
					}else{
						fprintf(stdout,"is successful\n");
					}
				}else{
					if(devctl_tx_msg_bufs(devname,fr_cfg22,FR_DEVCTL_TX_MSG,FR_MBF_NUM)){
						fprintf(stdout,"failed\n");
					}else{
						fprintf(stdout,"is successful\n");
					}
				}
			}			
		}else{
			if(fr_fifo_test==FR_FIFO_TEST){
				fprintf(stdout,"This option is only availabe for fifo=0\n");
				return 1;
			}
			fprintf(stdout,"Send message buffer index=%d to node %d ",buffer_index,node);
			status = devctl_uram_tx_msg(devname,node,buffer_index);
			if(status){
				fprintf(stderr,"failed: error: %s\n",strerror(status));
			}else{
				fprintf(stdout,"is successful\n");
			}
		}
	}
	if(is_rx){		
		if(buffer_index==-1){
			fprintf(stdout,"Receive data from message buffers from node %d\n",node);
			if(FR_NODE1==node){
				if(fr_fifo_test==0){
					devctl_rx_msgs(devname,fr_cfg11,FR_DEVCTL_RX_MSG,FR_MBF_NUM);						
				}else{
					devctl_rx_msgs(devname,fr_cfg21,FR_DEVCTL_RX_MSG,FR_MBF_NUM);
				}
			}else{
				if(fr_fifo_test==0){
					devctl_rx_msgs(devname,fr_cfg12,FR_DEVCTL_RX_MSG,FR_MBF_NUM);
				}else{
					devctl_rx_msgs(devname,fr_cfg22,FR_DEVCTL_RX_MSG,FR_MBF_NUM);
				}
			}					
		}else{
			if(fr_fifo_test==FR_FIFO_TEST){
				fprintf(stdout,"This option is only availabe for fifo=0\n");
				return 1;
			}
			fprintf(stdout,"Receive message buffer index=%d from node %d \n",buffer_index,node);			
			devctl_uram_rx_msg(devname,FR_DEVCTL_URAM_RX,node,buffer_index);			
		}
	}
	
	if((is_timer_tx) ||(is_timer_rx)){
			fr_timer_transmit();
	}
	return 0;
}


int parse_r_option(void)
{
	if ((optarg = strdup(optarg)) == NULL)  {
		errflag++;
		return 0;
	}
	options = optarg;
	while (*options != '\0') {
		switch(getsubopt(&options,r_opts,&value)){
			case FR_NODE : 
				if(value == NULL){
					fprintf(stdout,"Please enter flexray node \n");
					errflag++;
				}else{
					node = atoi(value);
					is_rx = true;
				}
				break;
			case FR_INDEX : 
				if(value == NULL){
					fprintf(stdout,"Please enter buffer index \n");
					errflag++;
				}else{
					buffer_index = atoi(value);
					if(node==FR_NODE1){
						if((buffer_index>(FR_MBF_NUM-1)) || (buffer_index<(FR_MBF_NUM/2))){
							fprintf(stdout,"Please enter number >15 and number < 30\n");
							errflag++;
						}
					}else{
						if((buffer_index>(FR_MBF_NUM/2)) || (buffer_index<1)){
							fprintf(stdout,"Please enter number >1 and number < 15\n");
							errflag++;
						}
					}
				}							
				break;
			case FR_FIFO : 
				if(value == NULL){
					fr_fifo_test = 0;
				}else{
					fr_fifo_test = atoi(value);
				}
				break;						
		}
	}
	return 0;
}

int parse_t_option(void)
{
	if ((optarg = strdup(optarg)) == NULL){
		errflag++;
		return 0;
	}
	options = optarg;
	while (*options != '\0') {
		switch(getsubopt(&options,timer_opts,&value)){
			case FR_NODE : 
				if(value == NULL){
					fprintf(stdout,"Please enter flexray node \n");
					errflag++;
				}else{
					node = atoi(value);
				}
				break;
			case FR_RX : 
				is_timer_rx = true;						
				break;
			case FR_TX :
				is_timer_tx = true;
				break;
			case TIMER_FIFO : 
				if(value == NULL){
					fr_fifo_test = 0;
				}else{
					fr_fifo_test = atoi(value);
				}
				break;				
		}
	}
	return 0;	
}

int parse_w_option(void)
{
	if ((optarg = strdup(optarg)) == NULL)  {
		errflag++;
		return 0;
	}
	options = optarg;
	while (*options != '\0') {
		switch(getsubopt(&options,r_opts,&value)){
			case FR_NODE : 
				if(value == NULL){
					fprintf(stdout,"Please enter flexray node \n");
					errflag++;
				}else{
					node = atoi(value);
					is_tx = true;
				}
				break;
			case FR_INDEX : 
				if(value == NULL){
					fprintf(stdout,"Please enter buffer index \n");
					errflag++;
				}else{
					buffer_index = atoi(value);
					if(node==FR_NODE1){
						if((buffer_index>(FR_MBF_NUM/2)) || (buffer_index<2)){
							fprintf(stdout,"Please enter number >1 and number < 15\n");
							errflag++;
						}
					}else{
						if((buffer_index<(FR_MBF_NUM/2)) || (buffer_index>(FR_MBF_NUM-1))){
							fprintf(stdout,"Please enter number >15 and number < 30\n");
							errflag++;
						}
					}
				}							
				break;
			case FR_FIFO : 
				if(value == NULL){
					fr_fifo_test = 0;
				}else{
					fr_fifo_test = atoi(value);
				}
				break;						
		}
	}
	return 0;	
}

int main(int argc, char **argv)
{
	buffer_index = -1;
	optind = 1;
	
	oflag = O_RDWR;
	strcpy(devname,"/dev/flexray");
	
	fr_fifo_test = 0;
	while ((c = getopt(argc, argv, "hbenr:w:t:"
	)) != -1) {
		switch (c) {		
		case 'b':
			oflag = O_RDWR;
			break;				
		case 'e':
			fr_get_error();
			break;
		case 'h':
			print_usemsg();
			break;						
		case 'n':
			oflag = O_RDWR | O_NONBLOCK;
             break;	 
		case 'r':
			parse_r_option();
			break;			
		case 't':
			parse_t_option();
			break;
		case 'w':
			parse_w_option();
			break;				
		}			
	}
	if(errflag){
		print_usemsg();
		return 0;
	}
	
	start_app();
	if((is_timer_tx) ||(is_timer_rx)){
		if(!is_fr_timer_stop){
			fprintf(stdout,"fr_timer_stop\n");
			fr_timer_stop(devname,FR_TIMER0,FR_DEVCTL_STOP_TIMER);
		}
		if(NULL != fr_name ){
			name_detach( fr_name, 0 );
		}
	}
		
	return 1;
}
