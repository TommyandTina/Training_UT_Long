/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <aarch64/r-car.h>

#include "pwm_priv.h"
#include "pwm_log.h"

extern pwm_device_t *g_device;
extern unsigned g_verbose;

static inline void pwm_chan_write(pwm_channel_t *chan, uint32_t reg, uint32_t data)
{
    out32(chan->iobase + reg, data);
}

static inline uint32_t pwm_chan_read(pwm_channel_t *chan, uint32_t reg)
{
    return in32(chan->iobase + reg);
}

static void pwm_chan_update(pwm_channel_t* chan, uint32_t reg, uint32_t data, uint32_t mask)
{
    uint32_t value;

    value = pwm_chan_read(chan, reg);
    value &= ~mask;
    value |= data & mask;
    pwm_chan_write(chan, reg, value);
}

static pwm_channel_t *pwm_find_chan(int *chan)
{
    int idx = *chan;

    if (idx >= g_device->n_chan) {
        return NULL;
    }

    for (int i = 0; i < g_device->n_chan; i++) {
        if (g_device->chans[idx].active) {
            return &g_device->chans[idx];
        }
    }

    return NULL;
}

static void pwm_set_clock_control(pwm_channel_t *chan, pwm_config_t* cfg, int div)
{
    uint32_t value;

    value = pwm_chan_read(chan, RCAR_PWM_PWMCR);
    value &= ~(RCAR_PWMCR_CCMD | RCAR_PWMCR_CC0_MASK | RCAR_PWMCR_SS0);

    if (cfg->oneshot) {
        value |= RCAR_PWMCR_SS0;
    }

    if (div & 1) {
        value |= RCAR_PWMCR_CCMD;
    }

    div >>= 1;

    value |= div << RCAR_PWMCR_CC0_SHIFT;

    pwm_chan_write(chan, RCAR_PWM_PWMCR, value);
}

static int pwm_set_counter(pwm_channel_t *chan, pwm_config_t* cfg, int div)
{
    unsigned long long one_cycle, tmp;
    uint32_t cyc, ph;
    lldiv_t div_tmp, div_one_cycle;

    one_cycle = (unsigned long long)NSEC_PER_SEC * 100ULL * (1 << div);
    div_one_cycle = lldiv(one_cycle, g_device->refclk);

    tmp = cfg->period_ns * 100ULL;
    div_tmp = lldiv(tmp, div_one_cycle.quot);

    cyc = (div_tmp.quot << RCAR_PWMCNT_CYC0_SHIFT) & RCAR_PWMCNT_CYC0_MASK;

    tmp = cfg->duty_ns * 100ULL;
    div_tmp = lldiv(tmp, div_one_cycle.quot);

    ph = div_tmp.quot & RCAR_PWMCNT_PH0_MASK;

    if((cyc == 0) || (ph == 0)) {
        return -1;
    }

    pwm_chan_write(chan, RCAR_PWM_PWMCNT, cyc | ph);

    return EOK;
}

static int pwm_get_clock_div(pwm_channel_t *chan, pwm_config_t* cfg)
{
    unsigned int div;
    unsigned long long min, max;
    lldiv_t period_min, period_max;

    min = (unsigned long long)NSEC_PER_SEC * RCAR_PWM_MIN_CYCLE;
    period_min = lldiv(min, g_device->refclk);

    if (cfg->period_ns < period_min.quot) {
        return -1;
    }

    for (div = 0; div <= RCAR_PWM_MAX_DIVISION; div++) {
        max = (unsigned long long)NSEC_PER_SEC * RCAR_PWM_MAX_CYCLE * (1 << div);
        period_max = lldiv(max, g_device->refclk);
        if(cfg->period_ns <= period_max.quot) {
            break;
        }
    }

    return (div <= RCAR_PWM_MAX_DIVISION) ? div : -1;
}

static int pwm_chan_config(pwm_channel_t *chan, pwm_config_t *cfg)
{
    int div, ret;

    if ((div = pwm_get_clock_div(chan, cfg)) < 0) {
        return -1;
    }

    pwm_chan_update(chan, RCAR_PWM_PWMCR, RCAR_PWMCR_SYNC, RCAR_PWMCR_SYNC);

    if (!(ret = pwm_set_counter(chan, cfg, div))) {
        pwm_set_clock_control(chan, cfg, div);
    }

    pwm_chan_update(chan, RCAR_PWM_PWMCR, 0, RCAR_PWMCR_SYNC);

    return ret;
}

static void pwm_chan_disable(pwm_channel_t *chan)
{
    pwm_chan_update(chan, RCAR_PWM_PWMCR, 0, RCAR_PWMCR_EN0);
}

static int pwm_chan_enable(pwm_channel_t *chan)
{
    uint32_t value;

    value = pwm_chan_read(chan, RCAR_PWM_PWMCNT);

    if ((value & RCAR_PWMCNT_CYC0_MASK) == 0 || 
       (value & RCAR_PWMCNT_PH0_MASK) == 0) {
        return -1;
    }

    pwm_chan_update(chan, RCAR_PWM_PWMCR, RCAR_PWMCR_EN0, RCAR_PWMCR_EN0);

    return 0;
}

static int pwm_devctl_configure(resmgr_context_t *ctp, io_devctl_t *msg)
{
    pwm_config_t *cfg = (pwm_config_t*)_DEVCTL_DATA(msg->i);
    pwm_channel_t *chan;

    if (!(chan = pwm_find_chan((int*)&cfg->channel))) {
        pwm_slogf(_SLOG_ERROR, "[%d] Configure failed, channel is invalid", ctp->info.pid);
        return EINVAL;
    }

    if (chan->owner != ctp->info.pid) {
        pwm_slogf(_SLOG_ERROR, "[%d] Configure failed, permission denied", ctp->info.pid);
        return EACCES;
    }

    if (pwm_chan_config(chan, cfg) < 0) {
        pwm_slogf(_SLOG_ERROR, "[%d] Configure failed, bad parameter", ctp->info.pid);
        return EINVAL;
    }

    pwm_slogf(_SLOG_DEBUG1, "[%d] Configured chan %d, pwmcr=0x%08x, pwmcnt=0x%08x", 
                    chan->owner, 
                    chan->index,
                    pwm_chan_read(chan, RCAR_PWM_PWMCR), 
                    pwm_chan_read(chan, RCAR_PWM_PWMCNT));

    return EOK;
}

static int pwm_devctl_disable(resmgr_context_t *ctp, io_devctl_t *msg)
{
    pwm_channel_t *chan;

    if (!(chan = pwm_find_chan(_DEVCTL_DATA(msg->i)))) {
        pwm_slogf(_SLOG_ERROR, "[%d] Stop failed, channel is invalid", ctp->info.pid);
        return EINVAL;
    }

    if(chan->owner != ctp->info.pid) {
        pwm_slogf(_SLOG_ERROR, "[%d] Stop failed, permission denied", ctp->info.pid);
        return EACCES;
    }

    pwm_chan_disable(chan);

    return EOK;
}

static int pwm_devctl_enable(resmgr_context_t *ctp, io_devctl_t *msg)
{
    pwm_channel_t *chan;

    if(!(chan = pwm_find_chan(_DEVCTL_DATA(msg->i)))) {
        pwm_slogf(_SLOG_ERROR, "[%d] Start failed, channel is invalid", ctp->info.pid);
        return EINVAL;
    }

    if (chan->owner != ctp->info.pid) {
        pwm_slogf(_SLOG_ERROR, "[%d] Start failed, permission denied", ctp->info.pid);
        return EACCES;
    }

    if (pwm_chan_enable(chan) < 0) {
        pwm_slogf(_SLOG_ERROR, "[%d] Start failed, channel is not configured", chan->owner);
        return ECANCELED;
    }

    return EOK;
}

static int pwm_devctl_detach(resmgr_context_t *ctp, io_devctl_t *msg)
{
    pwm_channel_t *chan;

    if (!(chan = pwm_find_chan(_DEVCTL_DATA(msg->i)))) {
        pwm_slogf(_SLOG_ERROR, "[%d] Detach failed, channel is invalid", ctp->info.pid);
        return EINVAL;
    }

    pthread_mutex_lock(&chan->lock);

    if ((chan->owner) && (chan->owner != ctp->info.pid)) {
        pwm_slogf(_SLOG_ERROR, "[%d] Detach failed, permission denied", ctp->info.pid);
        pthread_mutex_unlock(&chan->lock);
        return EACCES;
    }

    if (chan->owner) {
        pwm_slogf(_SLOG_INFO, "[%d] Detached chan %d", ctp->info.pid, chan->index);
        chan->owner = 0;
    }

    pthread_mutex_unlock(&chan->lock);

    return EOK;
}

static int pwm_devctl_attach(resmgr_context_t *ctp, io_devctl_t *msg)
{
    pwm_channel_t *chan;

    if (!(chan = pwm_find_chan(_DEVCTL_DATA(msg->i)))) {
        pwm_slogf(_SLOG_ERROR, "[%d] Attach failed, channel is invalid", ctp->info.pid);
        return EINVAL;
    }

    pthread_mutex_lock(&chan->lock);

    if ((chan->owner) && (ctp->info.pid != chan->owner)) {
        pwm_slogf(_SLOG_ERROR, "[%d] Attach failed, channel is being used by another pocess", ctp->info.pid);
        pthread_mutex_unlock(&chan->lock);
        return EBUSY;
    }

    if (!chan->owner) {
        pwm_slogf(_SLOG_INFO, "[%d] Attached chan %d", ctp->info.pid, chan->index);
        chan->owner = ctp->info.pid;
    }

    pthread_mutex_unlock(&chan->lock);

    return EOK;
}

int pwm_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
    int status;

    if ((status = iofunc_devctl_default(ctp, msg, ocb)) != _RESMGR_DEFAULT) {
        return status;
    }

    if ((status = iofunc_attr_unlock(&g_device->resmgr.ioattr)) != EOK) {
        return status;
    }

    switch (msg->i.dcmd) {
        case DCMD_PWM_ATTACH:
            status = pwm_devctl_attach(ctp, msg);
            break;
        case DCMD_PWM_DETACH:
            status = pwm_devctl_detach(ctp, msg);
            break;
        case DCMD_PWM_ENABLE:
            status = pwm_devctl_enable(ctp, msg);
            break;
        case DCMD_PWM_DISABLE:
            status = pwm_devctl_disable(ctp, msg);
            break;
        case DCMD_PWM_CONFIGURE:
            status = pwm_devctl_configure(ctp, msg);
            break;
        default:
            pwm_slogf(_SLOG_ERROR, "[%d] Unknown devctl command", ctp->info.pid);
            status = ENOSYS;
            break;
    }

    iofunc_attr_lock(&g_device->resmgr.ioattr);
    return status;
}

int pwm_slogf(const int verbose, const char *fmt, ...)
{
    int status;
    va_list arg;

    if (g_verbose < verbose) {
        return -1;
    }

    va_start(arg, fmt);
    status = vslogf(_SLOG_SETCODE(_SLOGC_TEST, 0), verbose, fmt, arg);
    va_end(arg);

    if (-1 == status){
        va_start(arg, fmt);
        status = vfprintf(stderr, fmt, arg);
        va_end(arg);
    }
    
    return status;
}

