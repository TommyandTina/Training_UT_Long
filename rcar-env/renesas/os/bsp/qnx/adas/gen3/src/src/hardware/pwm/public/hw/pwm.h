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

#ifndef _PWM_H_INCLUDED
#define _PWM_H_INCLUDED

#include <stdint.h>
#include <devctl.h>

// dvctl function
#define PWM_SET_CONFIG			1	// set duty and period
#define PWM_ENABLE				2	// enable pwm
#define PWM_DISABLE				3	// disable pwm

typedef struct {
	uint32_t period_ns;
	uint32_t duty_ns;
} pwm_config_t;

/*
	Define the actual devctl commnads
	there are four flavors of commands
	__DIOF		Data IO From the driver
	__DIOT		Data IO To the driver
	__DIOTF		Data IO To and From the driver
	__DION		a command with No associated data
*/

#define PWM_SETCONFIG_CMD_CODE	    1
#define PWM_ENABLEOUTPUT	        2
#define PWM_DISABLEOUTPUT		    3
#define DCMD_PWM_SETPERIOD		__DIOT(_DCMD_MISC, PWM_SETCONFIG_CMD_CODE, pwm_config_t)
#define DCMD_PWM_ENABLEOUTPUT	__DION(_DCMD_MISC, PWM_ENABLEOUTPUT)
#define DCMD_PWM_DISABLEOUTPUT	__DION(_DCMD_MISC, PWM_DISABLEOUTPUT)

#endif /* _PWM_H_INCLUDED */
