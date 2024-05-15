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

#include <atomic.h>
#include "rcar-flexray.h"
#include "flexray-regs.h"


int fr_delay(useconds_t useconds)
{
	return usleep(useconds);
}

/** 
 * @brief Wait Input Buffer Busy Shadow is cleared.
 * @param dev: Pointer to struct fr_dev_t.
 * @return 0: success, EBUSY: error
 */
int fr_wait_inputbuffer_busy(fr_dev_t *dev)
{
	int i;
	int  retry = FR_WAIT_BUF_BUSY;
	fr_regs_t *regs = &dev->regs;
	
	for (i = 0; i < retry; i++) {
		if (FR_REG_BIT(regs->in_buf.fribcr, FR_IBCR_IBSYS) == FR_BIT_CLEAR){
			return 0;
		}
	}
	return EBUSY;
}

/** 
 * @brief Waits the state of operation of the Commmunication Controller Protocol Operation Control(POC)
 * @param regs: Pointer to FlexRay module registers.
 * @param pocState: The state of operation of the Protocol Operation Control.
 * @return 0: 0: success, EBUSY: error.
 */
int fr_wait_pocs_state(fr_regs_t *regs, fr_poc_state pocState)
{
	int retry = FR_WAIT_CYCLE,i = 0;
	uint8_t pocs = 0;

	for (i = 0; i < retry; i++) {
		pocs = FR_REG_BITS(regs->cc_stat.frccsv, FR_CCSV_POCS_L, FR_CCSV_POCS_H) & FR_CCSV_POCS_MASK;

		if ((pocs == pocState))
			return 0;
			
		fr_delay(1);
	}

	return EBUSY;
}
/** 
 * @brief Checking if a bit is set or not 
 * @param regs: The address of FlexRay register.
 * @param position: The position of bit in register.
 * @param status: The status of bit in register.
 * @return 0: success, EBUSY: error.
 */
int fr_check_state_of_bit(uintptr_t regs, uint32_t position, uint32_t status)
{
	int i;
	int  retry = FR_WAIT_CYCLE;
	for (i = 0; i < retry; i++) {
		if(FR_REG_BIT(regs, position) == status){
			return 0;
		}
		fr_delay(1);
	}
	
	return EBUSY;
}

/** 
 * @brief Wait Output Buffer Busy Shadow is cleared.
 * @param dev: Pointer to struct fr_dev_t.
 * @return 0: success, EBUSY: error
 */
int fr_wait_outputbuffer_busy(fr_dev_t *dev)
{
	int i;
	int  retry = FR_WAIT_BUF_BUSY;
	fr_regs_t *regs = &dev->regs;
	
	for (i = 0; i < retry; i++) {
		if (FR_REG_BIT(regs->out_buf.frobcr, FR_OBCR_OBSYS) == FR_BIT_CLEAR){
			return 0;
		}
	}
	return EBUSY;	
	
}
