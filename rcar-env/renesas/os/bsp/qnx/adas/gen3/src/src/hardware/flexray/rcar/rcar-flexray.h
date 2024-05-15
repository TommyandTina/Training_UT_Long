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
#ifndef __FLEXRAY_RCAR_H__
#define __FLEXRAY_RCAR_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <hw/inout.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/siginfo.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <hw/sysinfo.h>
#include <drvr/hwinfo.h>
#include <aarch64/r-car-gen3.h>
#include <hw/flexray.h>
#include "flexray-regs.h"
#define RCARFR_POSIX_TYPED_MEM_PATH 	"/memory/below4G"

#define FR_REG_BIT(reg,bit)				((in32(reg) & ((uint32_t)(0x01 << bit))) >> bit)
#define FR_REG_BITS(reg,bit_l,bit_h)	((in32(reg) & ((uint32_t)(((0x01 << (bit_h - bit_l + 1)) - 1) << bit_l))) >> bit_l)
	
#define FR_MAX_DATA_INDEX				(64)
#define FR_MAX_FIFO						(0x1F + 1)				//!<	FIFO Table Max in URAM
#define FR_MBF_MAX_MEM					(2048)					//!<	The length(4 bytes) of message RAM
#define FR_FID_MAX						(0x7FF)	
#define FR_HEADER_LEN					(4*4) 					//!<	The length of a header section of input data structure 
#define FR_INPUT_ENTRY_LEN				(2*4) 					//!<	The length of an input pointer entry
#define FR_DATA_LEN						(4*FR_MAX_DATA_INDEX) 	//!<	The length of a data section
#define FR_OUTPUT_ENTRY_LEN				(4) 					//!<	The length of an output pointer entry
#define FR_OUTPUT_HEADER_LEN			(4*5) 					//!<	The length of a header section of output data structure
#define FR_ALIGN						4			
#define FR_RG_DATA_LEN					4
	
	
#define	FR_IOBUF_SIZE					(sizeof(fr_msg_ram_rx_t) + sizeof(io_devctl_t) + 4)
	
#define FR_SLOG_DEBUG0					(0)
#define FR_SLOG_DEBUG1					(1)
#define FR_SLOG_DEBUG2					(2)
#define FR_SLOG_DEBUG3					(3)
#define FR_BIT_SET						(1)
#define FR_BIT_CLEAR					(0)
#define FR_WAIT_CYCLE  					(100)
#define FR_WAIT_BUF_BUSY				(100)
#define	FR_MSGBUF_CH_NONE				0	//!<	No transmission; Receive buffer: ignore frame.			
#define FR_PRIORITY						10
#define FR_STACKSIZE					8192
#define FR_CLIENTS_MAX      			128

/** 
* @brief Channel used by the node to send a wakeup pattern.
*/
typedef enum
{
    FR_WAKEUP_CH_A = 0,  //!<	Send wakeup pattern on channel A. 
    FR_WAKEUP_CH_B = 1   //!<	Send wakeup pattern on channel B. 
} fr_wakeup_channel;

// The values are used to reference POC's states
typedef enum {
    FR_STATE_DEFAULT_CONFIG                     = 0x00,
    FR_STATE_READY                              = 0x01,
    FR_STATE_NORMAL_ACTIVE                      = 0x02,
    FR_STATE_NORMAL_PASSIVE                     = 0x03,
    FR_STATE_HALT                               = 0x04,
    FR_STATE_CONFIG                             = 0x0F,
    FR_STATE_WAKEUP_STANDBY                     = 0x10,
    FR_STATE_WAKEUP_LISTEN                      = 0x11,
    FR_STATE_WAKEUP_SEND                        = 0x12,
    FR_STATE_WAKEUP_DETECT                      = 0x13,
    FR_STATE_STARTUP_PREPARE                    = 0x20,
    FR_STATE_COLDSTART_LISTEN                   = 0x21,
    FR_STATE_COLDSTART_COLLISION_RESOLUTION     = 0x22,
    FR_STATE_COLDSTART_CONSISTENCY_CHECK        = 0x23,
    FR_STATE_COLDSTART_GAP                      = 0x24,
    FR_STATE_COLDSTART_JOIN                     = 0x25,
    FR_STATE_INTEGRATION_COLDSTART_CHECK        = 0x26,
    FR_STATE_INTEGRATION_LISTEN                 = 0x27,
    FR_STATE_INTEGRATION_CONSISTENCY_CHECK      = 0x28,
    FR_STATE_INITIALIZE_SCHEDULE                = 0x29,
    FR_STATE_ABORT_STARTUP                      = 0x2A,
    FR_STATE_STARTUP_SUCCESS                    = 0x2B,
    FR_STATE_UNSPEC                    			= 0x2C
} fr_poc_state;

/** \brief Baud rate on the FlexRay bus.
 */
typedef enum
{
    FR_CMD_NOT_ACCEPTED				= 0,	//!<	Command not accepted..
    FR_CMD_CONFIG					= 1,	//!<	Go to POC state CONFIG. 
    FR_CMD_READY					= 2,	//!<	RGo to POC state READY.
    FR_CMD_WAKEUP					= 3,	//!<	Go to POC state WAKEUP 
    FR_CMD_RUN						= 4,	//!<	Go to POC state STARTUP. 
    FR_CMD_ALL_SLOTS				= 5,	//!<	Leave SINGLE slot mode. 
    FR_CMD_HALT						= 6,	//!<	Set halt request. 
    FR_CMD_FREEZE					= 7,	//!<	Set the freeze status indicator and go to POC state HALT immediately. Can be called from any state 
    FR_CMD_SEND_MTS					= 8,	//!<	Send single MTS symbol. 
    FR_CMD_ALLOW_COLDSTART			= 9,	//!<	Resets flags Cold Start Inhibit. 
    FR_CMD_RESET_STATUS_INDICATORS	= 10,	//!<	Resets flags Coldstart Noise,Coldstart Abort and Wakeup.
    FR_CMD_CLEAR_RAMS				= 12	//!<	All internal RAM blocks are initialized to zero.
} fr_poc_cmd;

/** \brief Controller host interface command.
 */
typedef enum
{
    FR_BAUD_RATE_10  = 0, //!<	Baud rate is 10MBits/s.
    FR_BAUD_RATE_5   = 1, //!<	Baudrate is 5MBits/s.
    FR_BAUD_RATE_2_5 = 2  //!<	Baudrate is 2.5MBits/s. 
} fr_baud_rate;

/** \brief Definition timer type
*/
typedef enum 
{
    FR_TIMER_ABSOLUTE = 0,			//!<	Absolute timer
    FR_TIMER_RELATIVE             	//!<	Relative timer
} fr_timer_type;

/** \brief Definition of timer mode
*/
typedef enum 
{
    FR_TIMER_SINGLE_SHOT = 0,      	//!<	Timer Single-shot mode
    FR_TIMER_CONTINUOUS             //!<	Timer Continuous mode
} fr_timer_mode;

/** \brief Definition stop watch trigger type
*/
typedef enum 
{
    FR_STOWATCH_SOFTWARE = 0,		//!<	Software stop watch trigger
    FR_STOWATCH_TRIGGER             //!<	Stop watch trigger
} fr_stopwatch_trigger_type;

/** \brief Definition stop watch trigger edge type
*/
typedef enum 
{
    FR_STOWATCH_EDGE_FALLING = 0,		//!<	Stop watch trigger edge rising
    FR_STOWATCH_EDGE_RISING             //!<	Stop watch trigger edge falling
} fr_stopwatch_trigger_edge;

/** \brief Definition of stop watch mode
*/
typedef enum 
{
    FR_STOWATCH_SINGLE_SHOT = 0,      //!<	Stop watch Single-shot mode
    FR_STOWATCH_CONTINUOUS             //!<	Stop watch Continuous mode
} fr_stopwatch_mode;

typedef struct 
{
    uint8_t id;             		//!<	Timer ID     
    fr_timer_type type;          	//!<	Timer type
    fr_timer_mode mode;  			//!<	Ttimer mode
    uint32_t macrotickOffset;   	//!<	Timer Macrotick Offset
	uint8_t cycleCode;          	//!<	Timer 0 Cycle Code
} fr_timer;

typedef struct fr_stopwatch_
{
    fr_stopwatch_trigger_type type;      //!<	Stop watch trigger type
    fr_stopwatch_mode mode;  			//!<	Stop watch  mode
    fr_stopwatch_trigger_edge edge;   	//!<	Stop watch trigger edge
} fr_stopwatch_t;

typedef struct fr_op_regs_s {
    uintptr_t froc;
    uintptr_t fros;
} fr_op_regs_t;

typedef struct fr_spec_regs_s {
    uintptr_t frlck;
} fr_spec_regs_t;

typedef struct fr_intr_regs_s {
    uintptr_t freir;
    uintptr_t frsir;
    uintptr_t freils;
    uintptr_t frsils;
    uintptr_t freies;
    uintptr_t freier;
    uintptr_t frsies;
    uintptr_t frsier;
    uintptr_t frile;
} fr_intr_regs_t;

typedef struct fr_timer_regs_s {
    uintptr_t frt0c;
    uintptr_t frt1c;
    uintptr_t frt2c;
    uintptr_t frstpw1;
    uintptr_t frstpw2;
} fr_timer_regs_t;

typedef struct fr_cc_ctrl_regs_s {
    uintptr_t frsucc1;	//0x80
    uintptr_t frsucc2;	//0x84
    uintptr_t frsucc3;	//0x88
    uintptr_t frnemc;	//0x8C
    uintptr_t frprtc1;	//0x90
    uintptr_t frprtc2;	//0x94
    uintptr_t frmhdc;	//0x98
	uintptr_t reserved;	//0x9C
    uintptr_t frgtuc1;  //0xA0
    uintptr_t frgtuc2;	//0xA4
    uintptr_t frgtuc3;	//0xA8
    uintptr_t frgtuc4;	//0xAC
    uintptr_t frgtuc5;	//0xB0
    uintptr_t frgtuc6;	//0xB4
    uintptr_t frgtuc7;	//0xB8
    uintptr_t frgtuc8;	//0xBC
    uintptr_t frgtuc9;	//0xC0
    uintptr_t frgtuc10;	//0xC4
    uintptr_t frgtuc11;	//0xC8
} fr_cc_ctrl_regs_t;

typedef struct fr_cc_stat_regs_s {
    uintptr_t frccsv;		//0x100
    uintptr_t frccev;		//0x104
	uintptr_t reserved[2];	//0x108,0x10C
    uintptr_t frscv;		//0x110
    uintptr_t frmtccv;		//0x114
    uintptr_t frrcv;		//0x118
    uintptr_t frocv;		//0x11C
    uintptr_t frsfs;		//0x120
    uintptr_t frswnit;		//0x124
    uintptr_t fracs;		//0x128
	uintptr_t reserved1;	//0x12C
    uintptr_t fresid1;		//0x130
    uintptr_t fresid2;
    uintptr_t fresid3;
    uintptr_t fresid4;
    uintptr_t fresid5;
    uintptr_t fresid6;
    uintptr_t fresid7;
    uintptr_t fresid8;
    uintptr_t fresid9;
    uintptr_t fresid10;
    uintptr_t fresid11;
    uintptr_t fresid12;
    uintptr_t fresid13;
    uintptr_t fresid14;
    uintptr_t fresid15;
	uintptr_t reserved2; //0x16C
    uintptr_t frosid1;	 //0x170
    uintptr_t frosid2;
    uintptr_t frosid3;
    uintptr_t frosid4;
    uintptr_t frosid5;
    uintptr_t frosid6;
    uintptr_t frosid7;
    uintptr_t frosid8;
    uintptr_t frosid9;
    uintptr_t frosid10;
    uintptr_t frosid11;
    uintptr_t frosid12;
    uintptr_t frosid13;
    uintptr_t frosid14;
    uintptr_t frosid15;		//0x1A8
	uintptr_t reserved3; 	//0x1AC
    uintptr_t frnmv1;		//0x1B0
    uintptr_t frnmv2;		//0x1B4
    uintptr_t frnmv3;		//0x1B8
} fr_cc_stat_regs_t;

typedef struct fr_buf_ctrl_regs_s {
    uintptr_t frmrc;
    uintptr_t frfrf;
    uintptr_t frfrfm;
    uintptr_t frfcl;
} fr_msg_buf_ctrl_regs_t;

typedef struct fr_buf_stat_regs_s {
    uintptr_t frmhds;
    uintptr_t frldts;
    uintptr_t frfsr;
    uintptr_t frmhdf;
    uintptr_t frtxrq[4];
    uintptr_t frndat[4];
    uintptr_t frmbsc[4];
} fr_msg_buf_stat_regs_t;

typedef struct fr_in_buf_regs_s {
    uintptr_t frwrds[FR_MAX_DATA_INDEX];
    uintptr_t frwrhs1;
    uintptr_t frwrhs2;
    uintptr_t frwrhs3;
    uintptr_t fribcm;
    uintptr_t fribcr;
} fr_in_buf_regs_t;

typedef struct fr_out_buf_regs_s {
    uintptr_t frrdds[FR_MAX_DATA_INDEX];
    uintptr_t frrdhs1;
    uintptr_t frrdhs2;
    uintptr_t frrdhs3;
    uintptr_t frmbs;
    uintptr_t frobcm;
    uintptr_t frobcr;
} fr_out_buf_regs_t;

typedef struct fr_data_xfer_ctrl_regs_s {
    uintptr_t fritc;
    uintptr_t frotc;
    uintptr_t friba;
    uintptr_t frfba;
    uintptr_t froba;
    uintptr_t friqc;
    uintptr_t fruir;
    uintptr_t fruor;
} fr_data_xfer_ctrl_regs_t;

typedef struct fr_data_xfer_stat_regs {
    uintptr_t frits;
    uintptr_t frots;
    uintptr_t fraes;
    uintptr_t fraea;
    uintptr_t frda[4];
} fr_data_xfer_stat_regs_t;

typedef struct fr_regs_s {
    fr_op_regs_t                ope;
    fr_spec_regs_t              spec;
    fr_intr_regs_t              intr;
    fr_timer_regs_t             timer;
    fr_cc_ctrl_regs_t           cc_ctrl;
    fr_cc_stat_regs_t           cc_stat;
    fr_msg_buf_ctrl_regs_t      msg_buf_ctrl;
    fr_msg_buf_stat_regs_t      msg_buf_stat;
    fr_in_buf_regs_t            in_buf;
    fr_out_buf_regs_t           out_buf;
    fr_data_xfer_ctrl_regs_t    data_xfer_ctrl;
    fr_data_xfer_stat_regs_t    data_xfer_stat;
} fr_regs_t;

typedef struct fr_cluster_param {
	uint8_t			gColdStartAttempts;
	uint8_t			gdActionPointOffset;
	uint8_t			gdCASRxLowMax;
	uint8_t			gdDynamicSlotIdlePhase;
	uint8_t			gdMinislot;
	uint8_t			gdMinislotActionPointOffset;
	uint16_t		gdStaticSlot;
	uint8_t			gdTSSTransmitter;
	uint8_t			gdWakeupSymbolRxIdle;
	uint8_t			gdWakeupSymbolRxLow;
	uint16_t		gdWakeupSymbolRxWindow;
	uint8_t			gdWakeupSymbolTxIdle;
	uint8_t			gdWakeupSymbolTxLow;
	uint8_t			gListenNoise;
	uint16_t 		gMacroPerCycle;
	uint8_t			gMaxWithoutClockCorrectionFatal;
	uint8_t			gMaxWithoutClockCorrectionPassive;
	uint16_t		gNumberOfMinislots;
	uint16_t		gNumberOfStaticSlots;
	uint16_t		gOffsetCorrectionStart;
	uint8_t			gPayloadLengthStatic;
	uint8_t			gSyncNodeMax;
	// Protocol related 
	uint16_t		gdNIT;
	uint8_t			gdSampleClockPeriod;
	uint8_t			gNetworkManagementVectorLength;
} fr_cluster_param_t;

typedef struct fr_node_param {
	// Protocol relevant
	bool				pAllowHaltDueToClock;
	uint8_t				pAllowPassiveToActive;
	uint8_t				pChannels;
	uint16_t			pdAcceptedStartupRange;
	uint8_t				pClusterDriftDamping;
	uint8_t				pDecodingCorrection;
	uint8_t				pDelayCompensationA;
	uint8_t				pDelayCompensationB;
	uint32_t			pdListenTimeOut;
	uint16_t			pdMaxDrift;
	uint8_t				pExternOffsetCorrection;
	uint8_t				pExternRateCorrection;
	bool				pKeySlotUsedForStartup;
	bool				pKeySlotUsedForSync;
	uint16_t			pLatestTx;
	uint8_t				pMacroInitialOffsetA;
	uint8_t				pMacroInitialOffsetB;
	uint16_t			pMicroInitialOffsetA;
	uint16_t			pMicroInitialOffsetB;
	uint32_t			pMicroPerCycle;
	uint16_t			pOffsetCorrectionOut;
	uint16_t			pRateCorrectionOut;
	bool				pSingleSlotEnabled;
	fr_wakeup_channel	pWakeupChannel;
	uint8_t				pWakeupPattern;
	//Protocol related 
	uint8_t				pSamplesPerMicrotick;
} fr_node_param_t;

/** 
 * @brief Data register of input table.
 */
typedef struct _fr_input_data {
	uintptr_t            frwrds[FR_MAX_DATA_INDEX];
} fr_uram_data_t;

/** 
 * @brief Input data structure.
 */

typedef struct _fr_input_header {
    uintptr_t            frwrhs1;
    uintptr_t            frwrhs2;
    uintptr_t            frwrhs3;
    uintptr_t     		 frdata;    
} fr_input_header_t;
/** 
 * @brief Input entry structure.
 */
typedef struct _fr_input_entry {
    uintptr_t			frwrhs4;
    uintptr_t			dp;
} fr_input_entry_t;

/** 
 * @brief Input table structure.
 */
typedef struct _fr_input_table {
	int 				ita_fd;
	uintptr_t			ita_vaddr;			//!<	Vitual address of pointer table.
	off64_t				ita_paddr;			//!<	physical adrress of pointer table .
	uintptr_t			user_vaddr;			//!<	Vitual address of user input.
	off64_t				user_paddr;			//!<	physical address of user input.
	uintptr_t			iph_vaddr;			//!<	Vitual address of header.
	off64_t				iph_paddr;			//!<	physical address of header.
	uintptr_t			ipd_vaddr;			//!<	Vitual address of data.
	off64_t				ipd_paddr;			//!<	physical address of data.
	uint16_t			table_size;			//!<	Length of pointer table in bytes.
	uint16_t			entry_size;			//!<	Length of entry in bytes.
	uint16_t			header_size;		//!<	Length of header structure.
	uint16_t			data_size;			//!<	Length of data section .	
	fr_input_entry_t	pentry[FR_MAX_BUF];
	fr_input_header_t	pheader[FR_MAX_BUF];
	fr_uram_data_t		pdata[FR_MAX_BUF];
	fr_input_entry_t	uentry;
} fr_input_table_t;


/** 
 * @brief Data register of output table.
 */
typedef struct _fr_output_data {
	uintptr_t            frrdds[FR_MAX_DATA_INDEX];
} fr_output_data_t;

/** 
 * @brief Output data structure.
 */
typedef struct _fr_output_header {
    uintptr_t            frrdhs1;
    uintptr_t            frrdhs2;
    uintptr_t            frrdhs3;
    uintptr_t            frmbs;
    uintptr_t            frdata;  
} fr_output_header_t;

typedef struct _fr_output_buffer {
    uintptr_t            frmb;  
} fr_output_buffer_t;

/** 
 * @brief Output table structure
*/
typedef struct _fr_output_table {
	int 				ota_fd;
	int 				fifo_fd;
	uintptr_t			ota_vaddr;			//!<	Virtual Output Table Address 
	off64_t				ota_paddr;			//!<	physical memory of Output Table Address
	uintptr_t			oph_vaddr;			//!<	Vitual address of header.
	off64_t				oph_paddr;			//!<	physical address of header.
	uintptr_t			opd_vaddr;			//!<	Vitual address of data.
	off64_t				opd_paddr;			//!<	physical address of data.
	uintptr_t			ff_vaddr;			//!<	Virtual fifo address 
	off64_t				ff_paddr;			//!<	physical memory of fifo address
	uintptr_t			fph_vaddr;			//!<	Vitual address of fifo header.
	off64_t				fph_paddr;			//!<	physical address of fifo header.
	uintptr_t			fpd_vaddr;			//!<	Vitual address of data.
	off64_t				fpd_paddr;			//!<	physical address of data.
	uint16_t			ota_size;			//!<	Length of pointer table in bytes.
	uint32_t			header_size;		//!<	Length of output table pointer in bytes
	uint16_t			data_size;			//!<	Length of data in bytes.
	uint32_t			ff_size;			//!<	Length of fifo in bytes
	uint32_t			ff_header_size;		//!<	Length of fifo header in bytes
	uint16_t			ff_data_size;		//!<	Length of fifo data in bytes.

	fr_output_buffer_t	pentry[FR_MAX_BUF];
	fr_output_header_t	pheader[FR_MAX_BUF];	
	fr_output_data_t 	pdata[FR_MAX_BUF];	
	fr_output_buffer_t	pf_entry[FR_MAX_FIFO];
	fr_output_header_t	pf_header[FR_MAX_FIFO];
	fr_output_data_t 	pf_data[FR_MAX_FIFO];
} fr_output_table_t;

typedef struct {
	uint8_t		staticBuffers;				/**<	Number of static message buffer. \n
											0:		No static message buffers configured. \n
											1...127: Message buffers(1-127) assigned to the static message buffer. \n
											128:	No dynamic and fifo message buffers configured. \n 
													All message buffers are static message buffers. \n
											>128:	No static message buffers configured.*/
    uint8_t		dynamicBuffers;				/**<    Number of dynamic message buffer. \n
											0:		No static message buffers configured. \n
											1...127:	Message buffers(1-127) assigned to the dynamic message buffer. \n
											>=128:	No dynamic message buffers configured. \n */
	uint8_t		fifoBuffers;				/**<	Number of fifo message buffer. \n
											0:		No fifo message buffers configured. \n
											1-127:	Message buffers(1-127) assigned to the fifo message buffer. \n
											>=128:	No fifo message buffers configured.*/
	uint16_t	frameIdRejected;			//!<	Determines the frame ID of the incoming frames to be rejected by the fifo. frameIdRejected = 0...2047.
	uint16_t	frameIdFilter;				//!<	Frame id  used to mask the frame ID rejected by the fifo. If frameIdRejected=0, frameIdFilter=0 then no frame ID is rejected.
	uint8_t		channelFilter;				/**<	fifo receive channel filter: \n
											3:	No reception. \n
											2:	Receive only on channel A. \n
											1:	Receive only on channel B. \n 
											0:	Receive on both channels */
	uint8_t		cycleCount;					//!<	Cycle counter filter determines the cycle set to which frame ID and channel rejection filter are applied.
											//!<	In cycles not belonging to the cycle set specified by cycleCount, all frames are rejected.
											//!<	0:		Matching all cycles.	
	uint8_t		type;						//!<	Type of receive fifo, reconfig and Sync Frame Payload Multiplex of message buffer control 											
	uint8_t		fifoThreshold;				//!<	Receive fifo critical level
} fr_msg_buffer_control_t;

typedef struct fr_msgram_transfers_{		
	uint8_t 				numberMessage;		//!<	Number of message buffers.
	fr_msg_ram_t			nmsg[FR_MAX_BUF];	//!<	Header and data sections of message buffers.
} __attribute__ ((packed)) fr_msg_tx_t;


/** 
 * @brief Flexray Tx,Rx Message Linked List element
*/
typedef struct _frmsg {
	union {
		fr_msg_ram_t  		tx_msg;  	//!<	Flexray Tx message RAM
		fr_msg_buffer_rx_t	rx_msg;		//!<	Flexray Rx message RAM
	}msg;
    struct _frmsg *next;  			//!<	Pointer to next message
    struct _frmsg *prev;  			//!<	Pointer to previous message
} frmsg_t;

/**
 * @brief Tx,Rx Message List control
*/
typedef struct _frmsg_list {
    size_t                len;        		//!<	Length of each msg in bytes 
    unsigned              nmsg;       		//!<	Total number of messages in list 
    volatile unsigned     msgcnt;     		//!<	Number of messages in use
    frmsg_t               *msgs;      		//!<	Array of messages defining the message list 
    frmsg_t               *head;      		//!<	Head of list to add new messages
    frmsg_t               *tail;      		//!<	Tail of list to consume messages
    intrspin_t            lock;       		//!<	List locking mechanism 
} frmsg_list_t;

/** 
 * @brief Blocked Client Wait Structure
*/
typedef struct fr_client_wait_entry
{
    int                           rcvid;    //!<	Blocked client's rcvid
    struct fr_client_wait_entry   *next;    //!<	Pointer to next blocked client
} FRCLIENTWAIT;
/** 
 * @brief Blocked Client Wait Queue
*/
typedef struct fr_client_waitq_entry
{
    FRCLIENTWAIT     *elems;
    FRCLIENTWAIT     *head;    				//!<	Head of client wait queue 
    FRCLIENTWAIT     *tail;    				//!<	Tail of client wait queue
    int             cnt;       				//!<	Number of clients waiting
    intrspin_t      lock;      				//!<	List locking mechanism
} FRCLIENTWAITQ;

/** 
 * @brief Generic Flexray device structure
*/
typedef struct fr_dev_s {
    iofunc_attr_t		attr;
	iofunc_mount_t      mount;
    dispatch_t			*dpp;
    dispatch_context_t	*ctp;
    fr_regs_t			regs;
	uintptr_t			fr_reg_base;
    fr_input_table_t	input_info;
    fr_output_table_t	output_info;
    fr_msg_buffer_control_t	msg_ctl;
	int					priority;
	uint8_t				baud_rate;			
	char 				system_parameter[PATH_MAX];
	fr_node_param_t		node_param;
	fr_cluster_param_t	cluster_param;
	uint16_t gNumberOfStaticSlots;
	uint8_t gPayloadLengthStatic;
	int					iid;
	volatile unsigned	frsir;
	volatile unsigned	smtv;				//!<	Stop Watch Captured Macrotick Value.
	volatile unsigned 	sccv;				//!<	Stop Watch Captured Cycle Counter Value.
	volatile unsigned 	sscvb;				//!<	Stop Watch Captured Slot Counter Value Channel B.
	volatile unsigned 	sscva;				//!<	Stop Watch Captured Slot Counter Value Channel A.	
	volatile unsigned	cycle_counter;
	volatile unsigned	macrotick;
	uint8_t 			node;
	uint8_t 			mode;
	fr_devctl_error_t   fr_err;
	bool				intr_timer;
	bool				timer_start;
	int					timer_coid;
	paddr_t             phybase;
    uint32_t            len;
    uint32_t            irq;
	uint32_t            msgq_size;      		//!<	Number of messages
    uint32_t            waitq_size;     		//!<	Length of CAN message data
	frmsg_list_t        *tx_free_queue;     	//!<	Free tx element queue
    frmsg_list_t        *tx_msg_queue;      	//!<	Active tx queue
	frmsg_list_t        *rx_free_queue;     	//!<	Free rx element queue
    frmsg_list_t        *rx_msg_queue;      	//!<	Active rx queue
    struct sigevent     event;          		//!<	Device event
    FRCLIENTWAITQ       *tx_wait_free_queue; 	//!<	Client wait queue free elems
    FRCLIENTWAITQ       *tx_wait_client_queue; 	//!<	Blocked Clients queue
	FRCLIENTWAITQ       *rx_wait_free_queue; 	//!<	Client wait queue free elems
    FRCLIENTWAITQ       *rx_wait_client_queue; 	//!<	Blocked Clients queue
	fr_msg_tx_t 		msg_cfg;
	uint8_t				msg_lcb;
} fr_dev_t;


int fr_rx_replay_data(fr_dev_t *dev,resmgr_context_t *ctp,io_devctl_t *msg,int ioflag);
int fr_tx_replay_data(iov_t iov[],io_devctl_t *msg,int rcvid,fr_msg_ram_t *pmsg);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_read(resmgr_context_t* ctp, io_read_t* msg, RESMGR_OCB_T *ocb);
int event_pulse_handler(message_context_t * ctp,int code,unsigned flags,void *handle);
int fr_resmgr_init_device(fr_dev_t *dev);
frmsg_list_t *msg_list_create(unsigned nmsg);
void msg_list_destroy(frmsg_list_t *ml);
void frmsg_queue_element(frmsg_list_t *ml,frmsg_t *queue_elem);
frmsg_t *frmsg_dequeue_element(frmsg_list_t *ml);
FRCLIENTWAITQ *client_queue_create(int waitq_size);
FRCLIENTWAIT *client_dequeue(FRCLIENTWAITQ *wl);
void client_queue(FRCLIENTWAITQ *wl,FRCLIENTWAIT *wq);
void client_queue_destroy(FRCLIENTWAITQ *wl);
const struct sigevent * fr_isr(void *arg, int id);
int fr_slogf(const int minVerbose, const char *fmt, ...);
void fr_get_pocs_state(fr_regs_t *regs,fr_poc_state *pocState);
int fr_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
int io_unblock(resmgr_context_t *ctp, io_pulse_t *msg, RESMGR_OCB_T *ocb);
int rcar_flexray_init(fr_dev_t *dev);
int rcar_fr_deinit(fr_dev_t *dev);
int fr_check_msg_buf(uint8_t nmsg, uint16_t dataLength);
uint16_t get_header_crc(fr_msg_ram_t *header);
int fr_transmit(fr_dev_t *dev);
int rcar_transmit_msgs(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
int fr_startup(fr_dev_t *dev);
void fr_set_data_buffer(fr_dev_t *dev,uint8_t payloadLength, uint8_t *payload);
void fr_store_to_message_ram(fr_dev_t *dev,fr_msg_ram_t *msg);
int fr_set_header_message_buffer(fr_dev_t *dev,fr_msg_ram_t *msg);
int fr_config_msg_ram(fr_dev_t *dev,fr_msg_buffer_control_t *cfg);
void fr_get_fifo_fill_level(fr_dev_t *dev, uint8_t *rffl);
int fr_rx_msg_ram(fr_dev_t *dev, fr_msg_buffer_rx_t *msg);
int fr_rx_fifo_msg_ram(fr_dev_t *dev, fr_msg_buffer_rx_t *msg,uint8_t idx);
int rcar_rx_msg_ram(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
int fr_check_data(fr_dev_t *dev,uint8_t idx);
int rcar_stop_timer(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
int rcar_start_timer(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
int rcar_get_stop_watch_value(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
int rcar_get_error(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
//URAM data transfer
int fr_config_input(fr_dev_t *dev,fr_msg_tx_t *pmsg);
int fr_set_output_fifo_size(fr_dev_t *dev);
void fr_allocate_output_memory(fr_dev_t *dev,fr_msg_tx_t *pmsg);
int fr_uram_output_enable(fr_dev_t *dev);
void fr_config_fifo(fr_dev_t *dev,fr_msg_tx_t *pmsg);
int fr_uram_check_newdata(fr_dev_t *dev,uint8_t idx);
int fr_store_to_uram(fr_dev_t *dev,fr_input_entry_t *pe,fr_msg_ram_t *msg);
int fr_set_header_uram(fr_dev_t *dev,fr_input_header_t *ph,fr_msg_ram_t *msg);
int fr_set_data_to_uram(fr_dev_t *dev,fr_uram_data_t *pdata,uint8_t payloadLength, uint8_t *payload);
void fr_enable_irq_output_uram(fr_dev_t *dev);
int fr_uram_transmit(fr_dev_t *dev);
int rcar_fr_transmit_uram(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
int rcar_uram_select_tx(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
//Output data transfer 
int fr_config_output(fr_dev_t *dev);
void fr_select_output_mode(fr_dev_t *dev, bool	dataMode);
int rcar_rx_uram(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
void fr_fifo_rx_uram(fr_dev_t *dev, fr_msg_buffer_rx_t *nmsg);
void fr_rx_uram(fr_dev_t *dev,fr_msg_buffer_rx_t *nmsg);
void fr_rx_uram_set_header(fr_dev_t *dev,fr_output_header_t *ph,fr_msg_ram_t *msg);
void fr_uram_read_header(fr_dev_t *dev,fr_output_header_t *ph, fr_msg_buffer_rx_t *nmsg);
void fr_uram_read_data(fr_output_data_t *pd, uint8_t *payload, uint16_t data_len);
void fr_output_clear_data_available(fr_dev_t *dev,uint8_t index);
void fr_uram_get_fifo_fill_level(fr_dev_t *dev, uint8_t *fifo_ffl);
int rcar_uram_select_rx(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);

int fr_delay(useconds_t useconds);
int fr_wait_pocs_state(fr_regs_t *regs, fr_poc_state pocState);
int fr_wait_inputbuffer_busy(fr_dev_t *dev);
int fr_check_state_of_bit(uintptr_t regs, uint32_t position, uint32_t status);
int fr_wait_outputbuffer_busy(fr_dev_t *dev);
int fr_wait_transfer_inputbuffer_complete(fr_dev_t *dev,fr_msg_ram_t *msg);
int fr_get_conf(fr_dev_t *dev);
int fr_set_node_param(fr_regs_t *regs, fr_node_param_t *p);
int fr_set_cluster_param(fr_regs_t *regs, fr_cluster_param_t *p);

static inline void set_bit(uintptr_t reg, uint8_t bit)
{
    out32(reg, (in32(reg) | (uint32_t)(0x01 << bit)));
}

static inline void clear_bit(uintptr_t reg, uint8_t bit)
{
    out32(reg, (in32(reg) & ~((uint32_t)(0x01 << bit))));
}

static inline void write_bits(uintptr_t reg, uint8_t bit_l, uint8_t bit_h, uint32_t val)
{
    uint32_t mask = (((0x01 << (bit_h - bit_l + 1)) - 1) << bit_l);
    out32((reg), (in32((reg)) & ~((uint32_t)mask)) | ((val<<bit_l) & ((uint32_t)mask)));
}

static inline int fr_read_succ1(uintptr_t reg, int retry)
{
	uint32_t succ1;

	for (int i = 0; i < retry; i++) {
		succ1 = in32(reg);

		if (!(succ1 & (1<<FR_SUCC1_PBSY)))
			return succ1;

		fr_delay(1);
	}

	return -1;
}
#else
	
#endif
