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
#include <unistd.h>
#include <errno.h>
#include <devctl.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/mman.h>
#include <sys/slog.h>
#include <hw/pwm.h>
#include "pwm_private.h"

static pthread_mutex_t g_csIOCTL= PTHREAD_MUTEX_INITIALIZER;

static void rcar_pwm_update(pwm_dev_t* dev, uint32_t mask, uint32_t data, unsigned int offset)
{
	uint32_t value;

	value = pwm_read_reg(dev->pwm0, offset);
	value &= ~mask;
	value |= data & mask;
	pwm_write_reg(value,dev->pwm0, offset);
}

static int rcar_pwm_get_clock_division(int period_ns)
{
	unsigned long long min, max; /* min, max cycle / nanoseconds */
	unsigned int div;
	lldiv_t period_min, period_max;

	min = (unsigned long long)NSEC_PER_SEC * RCAR_PWM_MIN_CYCLE;
	period_min = lldiv(min, DEFALUT_CLKRATE);

	if (period_ns < period_min.quot)
	{
		return ERANGE;
	}

	for (div = 0; div <= RCAR_PWM_MAX_DIVISION; div++)
	{
		max = (unsigned long long)NSEC_PER_SEC * RCAR_PWM_MAX_CYCLE * (1 << div);
		period_max = lldiv(max, DEFALUT_CLKRATE);
		if (period_ns <= period_max.quot)
			break;
	}
    pwm_slogf(VERBOSE_QUIET, "div=%d\n",(unsigned int)div);

	return (div <= RCAR_PWM_MAX_DIVISION) ? div : ERANGE;
}

static int rcar_pwm_set_counter(pwm_dev_t* dev, int div, int duty_ns, int period_ns)
{
	unsigned long long one_cycle, tmp;	/* 0.01 nanoseconds */
	unsigned long clk_rate = DEFALUT_CLKRATE;
	uint32_t cyc, ph;
	lldiv_t div_tmp, div_one_cycle;

	one_cycle = (unsigned long long)NSEC_PER_SEC * 100ULL * (1 << div);
	div_one_cycle = lldiv(one_cycle, clk_rate);

	tmp = period_ns * 100ULL;
	div_tmp = lldiv(tmp, div_one_cycle.quot);

    pwm_slogf(VERBOSE_QUIET,"cyc=%d\n",(unsigned int)div_tmp.quot);

	cyc = (div_tmp.quot << RCAR_PWMCNT_CYC0_SHIFT) & RCAR_PWMCNT_CYC0_MASK;

	tmp = duty_ns * 100ULL;
	div_tmp = lldiv(tmp, div_one_cycle.quot);
    pwm_slogf(VERBOSE_QUIET, "ph=%d\n",(unsigned int)div_tmp.quot);
	ph = div_tmp.quot & RCAR_PWMCNT_PH0_MASK;

	/* Avoid prohibited setting */
	if (cyc == 0 || ph == 0)
	{
		pwm_slogf(VERBOSE_QUIET, "rcar_pwm_set_counter EINVAL");
		return EINVAL;
	}

	pwm_write_reg(cyc | ph,dev->pwm0, RCAR_GEN3_PWM_PWMCNT);
	
	return EOK;
}

static void rcar_pwm_set_clock_control(pwm_dev_t* dev, unsigned int div)
{
	uint32_t value;

	value = pwm_read_reg(dev->pwm0, RCAR_GEN3_PWM_PWMCR);
	value &= ~(RCAR_PWMCR_CCMD | RCAR_PWMCR_CC0_MASK);

	if (div & 1)
		value |= RCAR_PWMCR_CCMD;

	div >>= 1;

	value |= div << RCAR_PWMCR_CC0_SHIFT;

	pwm_write_reg(value,dev->pwm0, RCAR_GEN3_PWM_PWMCR);
}

static int rcar_pwm_config(pwm_dev_t* dev, int duty_ns, int period_ns)
{
	int div, ret;

	div = rcar_pwm_get_clock_division(period_ns);
	if (div < 0)
		return div;

	rcar_pwm_update(dev, RCAR_PWMCR_SYNC, RCAR_PWMCR_SYNC, RCAR_GEN3_PWM_PWMCR);

	ret = rcar_pwm_set_counter(dev, div, duty_ns, period_ns);
	if (!ret)
		rcar_pwm_set_clock_control(dev, div);

	/* The SYNC should be set to 0 even if rcar_pwm_set_counter failed */
	rcar_pwm_update(dev, RCAR_PWMCR_SYNC, 0, RCAR_GEN3_PWM_PWMCR);

	return ret;
}

static int rcar_pwm_enable(pwm_dev_t* dev)
{
	uint32_t value;

	/* Don't enable the PWM device if CYC0 or PH0 is 0 */
	value = pwm_read_reg(dev->pwm0, RCAR_GEN3_PWM_PWMCNT);
	if ((value & RCAR_PWMCNT_CYC0_MASK) == 0 || (value & RCAR_PWMCNT_PH0_MASK) == 0)
		return EINVAL;

	rcar_pwm_update((void*)dev, RCAR_PWMCR_EN0, RCAR_PWMCR_EN0, RCAR_GEN3_PWM_PWMCR);

	return 0;
}

static void rcar_pwm_disable(pwm_dev_t* dev)
{
	rcar_pwm_update(dev, RCAR_PWMCR_EN0, 0, RCAR_GEN3_PWM_PWMCR);
}

int pwm_devctl (resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	int	nbytes, status;
	pwm_config_t *config;
	
	pwm_dev_t *dev = (pwm_dev_t *)ocb->attr;
	
	if((status = iofunc_devctl_default(ctp, msg, ocb)) != _RESMGR_DEFAULT){
		return(status);
	}

	switch(msg -> i.dcmd){
	case DCMD_PWM_SETPERIOD:
		// set period and duty cycle
		pthread_mutex_lock(&g_csIOCTL);
		/* Get the data from the message. */
		config = _DEVCTL_DATA (msg->i);
		status = rcar_pwm_config(dev, config->duty_ns, config->period_ns);
		pthread_mutex_unlock(&g_csIOCTL);
		if(status != EOK){
			return (status);
		}
		nbytes = 0;
		break;
	case DCMD_PWM_ENABLEOUTPUT:
		status = rcar_pwm_enable(dev);
		if(status != EOK){
			return (status);
		}
		nbytes = 0;
		break;
	case DCMD_PWM_DISABLEOUTPUT:
		rcar_pwm_disable(dev);
		nbytes = 0;
		break;
	default:
		return (ENOSYS);
	}

	/* Clear the return message. */
	memset(&msg->o, 0, sizeof(msg->o));

	/* Indicate the number of bytes and return the message */
	msg->o.nbytes = nbytes;
	return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o) + nbytes));
}
