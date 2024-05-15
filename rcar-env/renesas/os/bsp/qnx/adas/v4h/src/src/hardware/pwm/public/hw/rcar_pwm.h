/*
* $QNXLicenseC:
* Copyright 2022, QNX Software Systems.
* Copyright 2022, Renesas Electronics Corporation.
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

#ifndef _RCAR_PWM_H_INCLUDED
#define _RCAR_PWM_H_INCLUDED

#include <stdbool.h>
#include <devctl.h>

#define PWM_NAME             "pwm"
#define PWM_DEVICE           "/dev/"PWM_NAME

#define PWM_CMD_ATTACH       (1)
#define PWM_CMD_DETACH       (2)
#define PWM_CMD_CONFIGURE    (3)
#define PWM_CMD_ENABLE       (4)
#define PWM_CMD_DISABLE      (5)

typedef struct {
    bool oneshot;         /* If this is set, PWM timer will operate in single cycle mode */
    int channel;          /* Set PWM channel that configuration is applied for */
    uint64_t period_ns;   /* Set duration (in nanosecond) for one PWM cycle */
    uint64_t duty_ns;     /* Set duration (in nanosecond) for high level in one PWM cycle */
} pwm_config_t;

/*
 * Request a PWM resource
 * Argument
 *   PWM channel
 * Error
 *   EINVAL : Invalid PWM channel (not existed or unusable by driver configuration)
 *   EBUSY  : The channel is owned by another application
 */
#define DCMD_PWM_ATTACH    __DIOT (_DCMD_MISC, PWM_CMD_ATTACH, int)

/*
 * Release a PWM resource
 * Argument
 *   PWM channel
 * Error
 *   EINVAL : Invalid PWM channel (not existed or unusable by driver configuration)
 *   EACCES : Application do not have permission to release the channel (not owner)
 */
#define DCMD_PWM_DETACH    __DIOT (_DCMD_MISC, PWM_CMD_DETACH, int)

/*
 * Configure PWM waveform
 * Argument
 *   Struct pwm_config_t
 * Error
 *   EINVAL : Invalid PWM channel / Bad PWM waveform configuration
 *   EACCES : Application do not have permission to configure the channel (not owner)
 */
#define DCMD_PWM_CONFIGURE    __DIOT (_DCMD_MISC, PWM_CMD_CONFIGURE, pwm_config_t)

/*
 * Enable output waveform from PWM pin
 * Argument
 *   PWM channel
 * Error
 *   EINVAL    : Invalid PWM channel
 *   EACCES    : Application do not have permission to control the channel (not owner)
 *   ECANCELED : PWM waveform is not configured correctly
 */
#define DCMD_PWM_ENABLE    __DIOT (_DCMD_MISC, PWM_CMD_ENABLE, int)

/*
 * Disable output waveform from PWM pin
 * Argument
 *   PWM channel
 * Error
 *   EINVAL : Invalid PWM channel
 *   EACCES : Application do not have permission to control the channel (not owner)
 */
#define DCMD_PWM_DISABLE    __DIOT (_DCMD_MISC, PWM_CMD_DISABLE, int)

#endif /* _RCAR_PWM_H_INCLUDED */
