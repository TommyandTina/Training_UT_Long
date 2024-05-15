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

#ifndef _PWM_PRIVATE_H_INCLUDED
#define _PWM_PRIVATE_H_INCLUDED

#include <sys/resmgr.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <hw/inout.h>
#include <aarch64/r-car-gen3.h>
#include <hw/rcar_cpg.h>

#define PWM_SIZE				0x0C
#define PWM_CHANNEL_OFFSET		0x1000
#define DEFALUT_CLKRATE			66.66 * 1000000UL	// 66.66MHz

#define NSEC_PER_SEC 1000000000L
#define RCAR_PWM_MAX_DIVISION	24
#define RCAR_PWM_MIN_CYCLE		2

#define MAX_MSG_SIZE			IOBUF_SIZE			// max size of a message, should be max(IOBUF, devctl_t)
#define IOBUF_SIZE				1024				// size of io_buffer used to for msg read/write with client
#define VERBOSE_QUIET			0

#define  RCAR_PWMCNT_CYC0_MASK	0x03ff0000
#define  RCAR_PWMCNT_CYC0_SHIFT	16
#define  RCAR_PWMCNT_PH0_MASK	0x000003ff
#define  RCAR_PWMCNT_PH0_SHIFT	0

#define RCAR_PWM_MAX_DIVISION	24
#define RCAR_PWM_MIN_CYCLE		2
#define RCAR_PWM_MAX_CYCLE		1023

#define  RCAR_PWMCR_CC0_MASK	0x000f0000
#define  RCAR_PWMCR_CC0_SHIFT	16
#define  RCAR_PWMCR_CCMD		BIT(15)
#define  RCAR_PWMCR_SYNC		BIT(11)
#define  RCAR_PWMCR_SS0			BIT(4)
#define  RCAR_PWMCR_EN0			BIT(0)

typedef struct pwm_dev {
    iofunc_attr_t		att;
    dispatch_t			*dpp;
    dispatch_context_t	*ctp;
	iofunc_mount_t      mount;
    uintptr_t			pwm0;
    uintptr_t			prr;	
} pwm_dev_t;

//extern pwm_dev_t *dev;

#define PWM_DISABLE_CPG			BIT(23)			// 1:Disable PWM

int pwm_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
int pwm_slogf(const int Verbose, const char *fmt, ...);

/******************************************************************************
Function:		pwm_write_reg
Description:	Write to register
Returns:		void
******************************************************************************/
static inline void pwm_write_reg(unsigned int data, uintptr_t base, unsigned int offset)
{
	out32(base + offset, (uint32_t)data);
}

/******************************************************************************
Function:		pwm_read_reg
Description:	Read from register
Returns:		void
******************************************************************************/
static inline unsigned int pwm_read_reg(uintptr_t base, unsigned int offset)
{
	return in32(base + offset);
}

#endif /* _PWM_PRIVATE_H_INCLUDED */
