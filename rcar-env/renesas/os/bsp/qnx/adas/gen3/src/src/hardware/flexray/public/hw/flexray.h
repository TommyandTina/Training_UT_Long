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
#ifndef _FLEXRAY_H
#define _FLEXRAY_H

#include <stdint.h>
#include <stdbool.h>
#include <devctl.h>
		
#define FR_MBF_NUM_STATIC			8
#define FR_MBF_NUM_FIFO				11
#define FR_NODE1					1
#define FR_NODE2					2

#define FR_TIMER0					(1<<0)		//!<	FlexRay Timer 0 is selected
#define FR_TIMER1					(1<<1)		//!<	FlexRay Timer 1 is selected
#define FR_TIMER2					(1<<2)		//!<	FlexRay Timer 2 is selected
#define FR_PULSE_EVENT_TIMER		(_PULSE_CODE_MINAVAIL + 1)
#define FR_PULSE_EVENT_STOPWATCH	(_PULSE_CODE_MINAVAIL + 2)
#define FR_V3U_NAME_TIMER			"flexray_rcarv3u"
#define RCAR_FR_UMODE				(1)			//!<	Use user RAM handles the data transfer between URAM and FlexRay module 

#define FR_MAX_DATA_LEN				(256)		//!<	Maximum length of data in bytes   
#define FR_MAX_BUF					(128) 		//!<	Number of messages in message RAM
#define FR_MAX_PAYLOAD				(254)		//!<	Maximum length of data section of one message in messages RAM 

/** 
 * @brief Flexible storage of data sections with different length. Some maximum values are:
*/
#define FR_MBF_NUM					(30)				//!<	30 message buffers
#define FR_MBF_56					(56)				//!<	56 message buffers
#define FR_MBF_128					(128)				//!<	128 message buffers
#define FR_MAX_DATA1				(FR_MBF_NUM*254)	//!<	The maximum length of data section of 30 message buffers
#define FR_MAX_DATA2				(FR_MBF_56*128)		//!<	The maximum length of data section of 128 message buffers
#define FR_MAX_DATA3				(FR_MBF_128*48)		//!<	The maximum length of data section of 48 message buffers
								
#define	FR_CMD_MASK_HEADER		(1<<0)			//!<	Header is selected for transfer.
#define	FR_CMD_MASK_DATA		(1<<1)			//!<	Data is selected for transfer.
#define	FR_CMD_MASK_REQUEST		(1<<2)			//!<	Transmit buffer released for transmission.

/** 
* @brief Received on Channel Indicator.
*/
typedef enum
{
    FR_RCI_B = 0,   		//!<	Receive frame on/from channel B.
	FR_RCI_A = 1,  			//!<	Receive frame on/from channel A. 
} fr_rx_channel;

/** 
* @brief Channel Filter Control.
*/
typedef enum
{
    FR_CHANNEL_A = 1,  			//!<	Transmit/Receive frame on/from channel A. 
    FR_CHANNEL_B = 2,   		//!<	Transmit/Receive frame on/from channel B.
	FR_CHANNEL_AB = 3 			//!<	Transmit/Receive frame on/from both channels(static segment only): A or B.
} fr_msg_channel;
/** 
* @brief Message buffer type.
*/
typedef enum
{
    FR_MSGBUF_TYPE_RX = 1,  	//!<	Message buffer is configured as Receive Buffer. 
    FR_MSGBUF_TYPE_TX = 2,   	//!<	Message buffer is configured as Transmit Buffer.
	FR_MSGBUF_TYPE_FIFO = 3 	//!<	Message buffer is configured as FIFO Receive Buffer.
} fr_msgbuf_type;
/** 
* @brief Transmission Mode.
*/
typedef enum
{
    FR_CONTINUOUS_MODE = 0,  	//!<	Continuous mode. 
    FR_SINGLE_MODE = 1   		//!<	Single-shot mode.
} fr_tx_mode;

#define FR_MSGCTL_NO_STATIC		(1<<0)			//!<	Receive fifo is not used in static segment.
#define FR_MSGCTL_NO_NULL_FRAME	(1<<1)			//!<	Fifo reject all null frames.
#define FR_MSGBUF_UNLOCK		(1<<3)			/**<	All buffers unlocked. Reconfiguration of message buffers enabled with static and dynamic buffers. \n
Reconfiguration of message buffers belonging to the fifo is locked.		
Exception: In nodes configured for sync frame transmission or for single slot mode operation message buffer 0 is always clocked.
if Sync Frame Payload Multiplex is set then the message buffer 0 and message buffer 1 are locked.
Sync Frame Payload Multiplex is not set then the message buffer 0 is locked.*/
#define FR_MSGBUF_LOCK_FIFO_STATIC		(1<<4)	//!<	Reconfiguration of static and fifo messages locked. Transmission of message buffers for static segment is disabled.							
#define FR_MSGBUF_LOCK					(1<<5)	//!<	Reconfiguration of all message buffers locked
#define FR_MSGBUF_LOCK_STATIC_TX		(1<<6)
#define FR_MSGBUF_SYNC_FRAME_MUL		(1<<7)  //!<	if set:	Both message buffers 0 and 1 are locked against reconfiguration.
												//!<	if not set:	Only message buffer 0 locked against reconfiguration.
/** 
 * @brief Cycle code determines the cycle set used for cycle counter filtering.
 *  Valid values are 2, 4, 8, 16, 32, 64
*/
#define FR_EVERY_CYCLE				0		//!<	Matching all cycles.	

#define FR_DRVR1_EIR_PEMC         	(1<<0)	//!<    Error mode has changed.
#define FR_DRVR1_EIR_CNA          	(1<<1)	//!<    CHI command not accepted.
#define FR_DRVR1_EIR_SFBM         	(1<<2)	//!<    Less than the required minimum of sync frames received.
#define FR_DRVR1_EIR_SFO          	(1<<3)	//!<    More sync frames received than configured.
#define FR_DRVR1_EIR_CCF          	(1<<4)	//!<    Clock correction failed.
#define FR_DRVR1_EIR_CCL          	(1<<5)	//!<    CHI Command Locked.
#define FR_DRVR1_EIR_AERR         	(1<<6)	//!<    Access error.
#define FR_DRVR1_EIR_RFO          	(1<<7)	//!<    Receive FIFO Overrun.
#define FR_DRVR1_EIR_EFA          	(1<<8)	//!<    Empty FIFO Access.
#define FR_DRVR1_EIR_IIBA         	(1<<9)	//!<    Illegal Input Buffer Access.
#define FR_DRVR1_EIR_IOBA         	(1<<10)	//!<    Illegal Output buffer Access.
#define FR_DRVR1_EIR_MHF          	(1<<11)	//!<    Message Handler failure.
#define FR_DRVR1_EIR_EDA          	(1<<12)	//!<    Error Detected on Channel A.
#define FR_DRVR1_EIR_LTVA         	(1<<13)	//!<    Latest Transmit Violation Channel A.
#define FR_DRVR1_EIR_TABA         	(1<<14)	//!<    Transmission Across Boundary Channel A.
#define FR_DRVR1_EIR_EDB          	(1<<15)	//!<    Error Detected on Channel B.
#define FR_DRVR1_EIR_LTVB         	(1<<16)	//!<    Latest transmit violation detected on channel B.
#define FR_DRVR1_EIR_TABB         	(1<<17)	//!<    Transmission across slot boundary detected on channel B.
//!<    Access Error Message RAM	
#define FR_DRVR1_MHDS_AMR			(1<<18)	//!<    Access Error Message RAM.
#define FR_DRVR1_MHDS_ATBF1			(1<<18)	//!<    Access Error Transient buffer RAM A.
#define FR_DRVR1_MHDS_ATBF2			(1<<20)	//!<    Access Error Transient buffer RAM B.
#define FR_DRVR1_MHDS_FMBD			(1<<21)	//!<    Faulty Message Buffer Detected.
#define FR_DRVR1_MHDS_MFMB			(1<<22)	//!<    Multiple Faulty Message Buffers detected.		
//!<    Access Error URAM			
#define FR_DRVR1_AES_EIDX			(1<<23)	//!<    Data structure pointer index number.
#define FR_DRVR1_AES_IAE			(1<<24)	//!<    Input transfer Access Error.
#define FR_DRVR1_AES_OAE			(1<<25)	//!<    Output transfer Access Error.
#define FR_DRVR1_AES_FAE			(1<<26)	//!<    FIFO transfer Access Error.
#define FR_DRVR1_AES_MAE			(1<<27)	//!<    Multiple Access Errors.
#define FR_DRVR1_MAX				(28)	

/**
@brief Contains information of received message header in message RAM
*/
typedef struct fr_receive_msg_buffer
{
	uint8_t			cmdMask	;						//!<    Buffer Command Mask.
	uint8_t			idx;  							//!<    Index of message buffer.
	fr_msgbuf_type	type;  							//!<    Type of received message buffer: FR_MSGBUF_TYPE_RX or FR_MSGBUF_TYPE_FIFO.
	uint8_t			cycleCode;						//!<	Cycle code.
    bool 			ppi; 							//!<	true: Payload Preamble Indicator is set; false:	Payload Preamble Indicator is not set.  
    uint8_t			nullFrame;						//!<	Null frame indicator.
	bool 			sync; 							//!<	true: Frame is a sync frame; false: Frame is not a sync frame.
	bool 			startup;						//!< 	true: Frame is a startup frame; false: Frame is not a startup frame. 
	fr_rx_channel	channel;						//!<	Received on Channel Indicator
    uint8_t			cycleCount;						//!<	Cycle count of frame is received.
    uint8_t			payloadLength;					//!<	Length of payload segment (2-byte words) stored from received frame.
    uint16_t		headerCrc;						//!<	Header crc.
	uint16_t		frameId;						//!<	The frame ID.
	uint8_t 		data[FR_MAX_DATA_LEN];			//!<	Data section of the message buffer.
} __attribute__ ((packed)) fr_msg_buffer_rx_t;

/** 
 @brief Header and data section of a message buffer in the Message RAM
 */
typedef struct
{
	uint8_t				cmdMask;				//!<    Buffer Command Mask.
	uint8_t				idx;  					//!<    Index of message buffer.
	uint16_t			frameId;				//!<	Frame ID of the selected message buffer. 													
	uint8_t				cycleCount;				//!<	Cycle code determines the cycle set used for cycle counter filtering.
	fr_msg_channel		channel;				//!<	Channel Filter Control.
	fr_msgbuf_type		type;  					//!<    Type of message buffer. 
	fr_tx_mode 			mode; 					//!< 	The transmission mode.
	bool 				ppi; 					//!<	true: Payload Preamble Indicator is set; false:	Payload Preamble Indicator is not set.	
	bool 				sync; 					//!<	true: Frame is a sync frame; false: Frame is not a sync frame.
	bool 				startup;				//!< 	true: Frame is a startup frame; false: Frame is not a startup frame. 	
	uint8_t				payloadLength;			//!<	Length of data section.
	uint8_t 			data[FR_MAX_DATA_LEN];	//!<	Data section of message RAM.
} __attribute__ ((packed)) fr_msg_ram_t;

typedef struct fr_stopwatch_value{		
	uint32_t				smtv;				//!<	Stop Watch Captured Macrotick Value.
	uint32_t 				sccv;				//!<	Stop Watch Captured Cycle Counter Value.
	uint32_t 				sscvb;				//!<	Stop Watch Captured Slot Counter Value Channel B.
	uint32_t 				sscva;				//!<	Stop Watch Captured Slot Counter Value Channel A.
} __attribute__ ((packed)) fr_stopwatch_value_t;

typedef struct fr_devctl_error {
        unsigned                drvr1;     		//!<	Contains errors from FR_DRVR1_EIR_PEMC to FR_DRVR1_AES_MAE
        unsigned                drvr2;     
        unsigned                drvr3;     
        unsigned                drvr4;     
} fr_devctl_error_t;

#define FR_DEVCTL_TX_MSG				__DIOT(_DCMD_MISC, 1,  fr_msg_ram_t)			//!<	Transmit message buffer
#define FR_DEVCTL_RX_MSG				__DIOTF(_DCMD_MISC,2,  fr_msg_buffer_rx_t)		//!<	Receive message buffer
#define FR_DEVCTL_URAM_TX				__DIOT(_DCMD_MISC, 3,  fr_msg_ram_t)
#define FR_DEVCTL_URAM_RX				__DIOTF(_DCMD_MISC,4,  fr_msg_buffer_rx_t)
#define FR_DEVCTL_START_TIMER		 	__DIOT(_DCMD_MISC, 5,  uint32_t)
#define FR_DEVCTL_STOP_TIMER		 	__DIOT(_DCMD_MISC, 6,  uint32_t)
#define FR_DEVCTL_GET_STOPWATCH_VALUE	__DIOTF(_DCMD_MISC,7,  fr_stopwatch_value_t)
#define FR_DEVCTL_ERROR   				__DIOTF(_DCMD_MISC,8,  fr_devctl_error_t)
#endif
