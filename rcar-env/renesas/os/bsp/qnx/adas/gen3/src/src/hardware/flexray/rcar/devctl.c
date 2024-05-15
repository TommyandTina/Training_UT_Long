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
#include <string.h>
#include <atomic.h>
#include "rcar-flexray.h"

/** 
 * @brief This routine gets called whenever the client does "devctl" call.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: Pointer to struct io_devctl_t.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @@return 0:success, !=0:error
 */
int fr_devctl(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	int status = 0;

	fr_dev_t *dev = (fr_dev_t *)ocb->attr;

	if(ctp->size < sizeof(msg->i) ){
		return (EBADMSG);
	}

	if ((status = iofunc_devctl_default(ctp, msg, ocb)) != _RESMGR_DEFAULT){
		return status;
	}
	switch (msg->i.dcmd){			
		case FR_DEVCTL_TX_MSG:
			if(dev->mode){
				return rcar_transmit_msgs(ctp,msg,ocb);
			}else{
				return rcar_fr_transmit_uram(ctp,msg,ocb);
			}
			break;
		case FR_DEVCTL_RX_MSG:
			if(dev->mode){
				return rcar_rx_msg_ram(ctp,msg,ocb);	
			}else{
				return rcar_rx_uram(ctp,msg,ocb);
			}
			break;
		case FR_DEVCTL_URAM_TX:
			if(dev->mode){
				return (ENOSYS);
			}else{
				return rcar_uram_select_tx(ctp,msg,ocb);		
			}		
		case FR_DEVCTL_URAM_RX:
			return rcar_uram_select_rx(ctp,msg,ocb);
		case FR_DEVCTL_ERROR:
			return rcar_get_error(ctp,msg,ocb);		
		case FR_DEVCTL_START_TIMER:
			return rcar_start_timer(ctp,msg,ocb);		
		case FR_DEVCTL_STOP_TIMER:
			return rcar_stop_timer(ctp,msg,ocb);				
		case FR_DEVCTL_GET_STOPWATCH_VALUE:
			return rcar_get_stop_watch_value(ctp,msg,ocb);
		default:
			return (ENOSYS);
	}
	if(status) {
		return EIO;
	}else{
		return EOK;
	}
}
