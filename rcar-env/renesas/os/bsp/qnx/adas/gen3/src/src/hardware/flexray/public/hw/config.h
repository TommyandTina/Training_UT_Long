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
#include <hw/flexray.h>
fr_msg_ram_t fr_cfg11[FR_MBF_NUM] = {
	//-----------------------11bit  7bit         	 2bit         1bit 			   1bit           1bit  1bit 1bit 	7bit Payload Length     
	//-----------------------Frame  Cycle        	 CH           CFG              TXM            PPIT  sync start 				 
	//-----------------------ID     Code                                                  			    			            
	{  FR_CMD_MASK_HEADER,0,0x001, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,true,true,  FR_MAX_PAYLOAD,},  // MB0
	{  FR_CMD_MASK_HEADER,1,0x002, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB1
	//-----------------------------	------------------------------------------------ ------  -------------------------------------
	{  FR_CMD_MASK_HEADER,2,0x003, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB2 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,3,0x004, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB3 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,4,0x005, 	FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB4 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,5,0x006, 	FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB5 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,6,0x007, 	FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB6 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,7,0x008, 	FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB7 : for TX/RX Test
	//----------------------------------------------------------------------------- ------  ----------------------------------------
	{  FR_CMD_MASK_HEADER,8, 0x011, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB8
	{  FR_CMD_MASK_HEADER,9, 0x012, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB9
	{  FR_CMD_MASK_HEADER,10,0x013, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB10 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,11,0x014, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB11 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,12,0x015, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB12 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,13,0x016, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB13 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,14,0x017, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB14 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,15,0x018, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB15 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,16,0x019, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB16
	{  FR_CMD_MASK_HEADER,17,0x01A, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB17
	{  FR_CMD_MASK_HEADER,18,0x01B, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB18 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,19,0x01C, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB19 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,20,0x01D, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB20 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,21,0x01E, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB21 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,22,0x01F, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB22 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,23,0x020, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB23 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,24,0x021, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB24 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,25,0x022, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB25 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,26,0x023, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB26 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,27,0x024, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB27 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,28,0x025, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB28 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,29,0x026, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB29 : for TX/RX Test
};	

fr_msg_ram_t fr_cfg12[FR_MBF_NUM] = {
	//-----------------------11bit  7bit         	 2bit         1bit 			   1bit           1bit  1bit 1bit 	7bit Payload Length     
	//-----------------------Frame  Cycle        	 CH           CFG              TXM            PPIT  sync start 				 
	//-----------------------ID     Code                                                  			    			            
	{  FR_CMD_MASK_HEADER,0,0x002, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,true,true,	FR_MAX_PAYLOAD,},  // MB0
	{  FR_CMD_MASK_HEADER,1,0x001, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB1
	//-----------------------------	----------------------------------------------- ------  ------------------------------------
	{  FR_CMD_MASK_HEADER,2,0x003, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB2 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,3,0x004, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB3 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,4,0x005, 	FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB4 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,5,0x006, 	FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB5 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,6,0x007, 	FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB6 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,7,0x008, 	FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB7 : for TX/RX Test
	//---------------------------------------------------------- ------------------ ------  --------------------------------------
	{  FR_CMD_MASK_HEADER,8, 0x011, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB8
	{  FR_CMD_MASK_HEADER,9, 0x012, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB9
	{  FR_CMD_MASK_HEADER,10,0x013, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB10 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,11,0x014, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB11 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,12,0x015, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB12 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,13,0x016, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB13 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,14,0x017, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB14 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,15,0x018, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB15 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,16,0x019, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB16
	{  FR_CMD_MASK_HEADER,17,0x01A, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB17
	{  FR_CMD_MASK_HEADER,18,0x01B, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB18 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,19,0x01C, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB19 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,20,0x01D, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB20 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,21,0x01E, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB21 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,22,0x01F, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB22 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,23,0x020, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB23 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,24,0x021, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB24 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,25,0x022, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB25 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,26,0x023, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB26 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,27,0x024, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB27 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,28,0x025, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB28 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,29,0x026, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB29 : for TX/RX Test
};

fr_msg_ram_t fr_cfg21[FR_MBF_NUM] = {
	//-----------------------11bit  7bit         	 2bit         1bit 			   1bit           1bit  1bit 1bit  7bit Payload Length   
	//-----------------------Frame  Cycle        	 CH           CFG              TXM            PPIT  sync start 				 
	//-----------------------ID     Code                                                  			    			            
	{  FR_CMD_MASK_HEADER,0,0x001, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,true,true,	FR_MAX_PAYLOAD,},  // MB0
	{  FR_CMD_MASK_HEADER,1,0x002, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB1
	//---------------------------------------------------------------------------- ------  ------------------------ ---------------------------------------
	{  FR_CMD_MASK_HEADER,2,0x003, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB2 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,3,0x004, 	FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB3 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,4,0x005, 	FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB4 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,5,0x006, 	FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB5 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,6,0x007, 	FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB6 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,7,0x008, 	FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB7 : for TX/RX Test
	//---------------------------------------------------------- ------------------ ------  --------------------------------------------------------------------
	{  FR_CMD_MASK_HEADER,8, 0x011, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB8
	{  FR_CMD_MASK_HEADER,9, 0x012, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB9
	{  FR_CMD_MASK_HEADER,10,0x013, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB10 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,11,0x014, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB11 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,12,0x015, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB12 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,13,0x016, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB13 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,14,0x017, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB14 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,15,0x018, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB15 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,16,0x019, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB16
	{  FR_CMD_MASK_HEADER,17,0x01A, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB17
	{  FR_CMD_MASK_HEADER,18,0x01B, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB18 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,19,0x01C, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB19 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,20,0x01D, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB20 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,21,0x01E, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB21 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,22,0x01F, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB22 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,23,0x020, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB23 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,24,0x021, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB24 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,25,0x022, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB25 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,26,0x023, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB26 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,27,0x024, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB27 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,28,0x025, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,	FR_MAX_PAYLOAD,},  // MB28 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,29,0x026, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,  FR_MAX_PAYLOAD,},  // MB29 : for TX/RX Test	
};			

fr_msg_ram_t fr_cfg22[FR_MBF_NUM] = {
	//-----------------------11bit  7bit         	 2bit         1bit 			   1bit           1bit  1bit 1bit 		   7bit Payload Length     
	//-----------------------Frame  Cycle        	 CH           CFG              TXM            PPIT  sync start 		 
	//-----------------------ID     Code                                                  			    			            
	{  FR_CMD_MASK_HEADER,0,0x002,  FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,true,true, 	FR_MAX_PAYLOAD,}, // MB0
	{  FR_CMD_MASK_HEADER,1,0x001,  FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,}, // MB1
	//---------------------------------------------------------------------------- ------  -----------------------------------------------------------
	{  FR_CMD_MASK_HEADER,2, 0x003, FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,}, // MB2 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,3, 0x004, FR_EVERY_CYCLE,FR_CHANNEL_AB,FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,}, // MB3 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,4, 0x005, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,}, // MB4 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,5, 0x006, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,}, // MB5 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,6, 0x007, FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,}, // MB6 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,7, 0x008, FR_EVERY_CYCLE,FR_CHANNEL_B, FR_MSGBUF_TYPE_RX,FR_SINGLE_MODE,false,false,false,FR_MAX_PAYLOAD,}, // MB7 : for TX/RX Test	
	//---------------------------------------------------------------------------- ------  ------------------------ ------------------------------------------
	{  FR_CMD_MASK_HEADER,8, 0x01C, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB8
	{  FR_CMD_MASK_HEADER,9, 0x01D, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB9
	{  FR_CMD_MASK_HEADER,10,0x01E, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB10 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,11,0x01F, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB11 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,12,0x020, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB12 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,13,0x021, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB13 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,14,0x022, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_FIFO,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB14 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,15,0x023, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB15 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,16,0x024, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB16
	{  FR_CMD_MASK_HEADER,17,0x025, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB17
	{  FR_CMD_MASK_HEADER,18,0x026, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_CONTINUOUS_MODE,false,false,false,FR_MAX_PAYLOAD,},  // MB18 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,19,0x011, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB19 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,20,0x012, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB20 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,21,0x013, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB21 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,22,0x014, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB22 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,23,0x015, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB23 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,24,0x016, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB24 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,25,0x017, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB25 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,26,0x018, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB26 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,27,0x019, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB27 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,28,0x01A, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB28 : for TX/RX Test
	{  FR_CMD_MASK_HEADER,29,0x01B, FR_EVERY_CYCLE,FR_CHANNEL_A, FR_MSGBUF_TYPE_TX,FR_SINGLE_MODE,false,false,false,      FR_MAX_PAYLOAD,},  // MB29 : for TX/RX Test	
};