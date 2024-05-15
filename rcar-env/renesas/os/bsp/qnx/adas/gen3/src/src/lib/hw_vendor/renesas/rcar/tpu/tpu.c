/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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


#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <atomic.h>
#include <hw/inout.h>
#include <sys/iofunc.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include "proto.h"


struct tpu_info_s {
    uint32_t	offset;
    int		    tstr_mask;
};

static struct tpu_info_s tpu_info[] = {
    /* Channel 0 */  { RCAR_GEN3_TPU_0_OFFSET, TPU_TSTR_CST0 },
    /* Channel 1 */  { RCAR_GEN3_TPU_1_OFFSET, TPU_TSTR_CST1 },
    /* Channel 2 */  { RCAR_GEN3_TPU_2_OFFSET, TPU_TSTR_CST2 },
    /* Channel 3 */  { RCAR_GEN3_TPU_3_OFFSET, TPU_TSTR_CST3 },
};

static tpu_ctrl_t  tpu_mmap = { 0 };

int tpu_slog_verbose = 0; // -v for verbose operation

static int
tpu_init(char* options)
{
    int         opt;
    char        *value;
    static char	*opts[] = {
                            "verbose",
                            NULL
                          };

    while( *options != '\0' ) {
		if( ( opt = getsubopt( &options, opts, &value ) ) == -1 ) {
            continue;
		}

		switch( opt ) {
			case 0:			// priority
                tpu_slog_verbose = strtoul(value, NULL, 0);
                break;
            default:
               break;
        }
    }
    
    return EOK;
}

static void
tpu_fini()
{
}

static uintptr_t tpu_map_registers(tpu_chan_t *chan)
{
    if( chan->pbase == 0) {
        return (uintptr_t)MAP_FAILED;
    }

    chan->vbase = 0;

    // check whether the memory for the CMT channel group is already mapped in.
    if (tpu_mmap.paddr == chan->pbase) {
        atomic_add(&tpu_mmap.num, 1);
        chan->vbase = tpu_mmap.vaddr;
    }

    if (chan->vbase == 0) {
        chan->vbase = mmap_device_io(chan->size, chan->pbase);
        if (chan->vbase == (uintptr_t)MAP_FAILED)
            return (uintptr_t)MAP_FAILED;

        if (tpu_mmap.num == 0) {
            tpu_mmap.paddr = chan->pbase;
            tpu_mmap.vaddr = chan->vbase;
            tpu_mmap.num   = 1;
        }
    }

    chan->regs += chan->vbase;

    return (chan->vbase);
}

static void tpu_unmap_registers(tpu_chan_t *chan)
{
    // unmap memory if not in use for other TPU channels in same group
    if (tpu_mmap.paddr == chan->pbase) {
        atomic_sub(&tpu_mmap.num, 1);
        if (tpu_mmap.num == 0) {
            munmap_device_io(tpu_mmap.vaddr, chan->size);
            tpu_mmap.num = tpu_mmap.paddr = tpu_mmap.vaddr = 0;
        }
        chan->vbase = 0;
    }
}

static int tpu_create_resources(tpu_chan_t *chan)
{
    rsrc_alloc_t rsrc_req;
    int rsrc_ret = EOK;

    memset(&rsrc_req, 0, sizeof(rsrc_req));

    rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -1, chan->name, RSRCDBMGR_IO_PORT);

    if (rsrc_ret < 0) {
        rsrc_ret = errno;
        TPU_SLOG_ERROR("%s: Failed querying %s cmt resources", __FUNCTION__, chan->name);
        return rsrc_ret;
    } else if (rsrc_ret == 0) {
        rsrc_req.start = 0;
        rsrc_req.end = chan->channels - 1;
        rsrc_req.flags = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name = chan->name;

        rsrc_ret = rsrcdbmgr_create (&rsrc_req, 1);

        if( rsrc_ret != EOK ) {
            rsrc_ret = errno;
            TPU_SLOG_ERROR("%s: Failed creating cmt resources: err %d", __FUNCTION__, rsrc_ret );
            return rsrc_ret;
        }
    } else {
        return EALREADY;
    }

    return EOK;
}

static void * tpu_channel_attach(int priority, tpu_attach_flags flags)
{
    tpu_chan_t   *chan;
    rsrc_request_t  req = { 0 };

    chan = calloc(1, sizeof(*chan));
    if (chan == NULL) {
        TPU_SLOG_ERROR("%s: calloc failed", __FUNCTION__);
        goto fail;
    }

    strcpy(chan->name, "tpu");

    chan->channels = RCAR_TPU_CHANNELS;

    tpu_create_resources(chan);

    req.length = 1;
    req.start  = 0;
    req.end    = chan->channels - 1;
    req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;

    // The lower CMT channel numbers have highest priority
    if (flags & TPU_ATTACH_MOTOR_CONTROL_CHANNEL) {
        req.start = req.end = 0;
    }
    else if (flags & TPU_ATTACH_PRIORITY_STRICT) {
        req.start = req.end = priority;
    } else {
        req.flags |= RSRCDBMGR_FLAG_TOPDOWN;
    }

    if (rsrcdbmgr_attach(&req, 1) == -1) {
        TPU_SLOG_ERROR("%s: TPU channel rsrcdbmgr_attach failed: %s", __FUNCTION__, strerror(errno));
        goto fail1;
    }

    chan->chan_idx   = req.start;
    chan->pbase      = RCAR_GEN3_TPU_BASE;
    chan->size       = RCAR_GEN3_TPU_SIZE;
    chan->tstr_mask  = tpu_info[chan->chan_idx].tstr_mask;
    chan->regs       = tpu_info[chan->chan_idx].offset;
    chan->pclock     = RCAR_V3U_CLK_S1D8;
    chan->prescaler  = TPU_CLOCK_DIV_1;
    chan->clock_edge = TPU_CLOCK_EDGE_RISE;
    chan->mode       = (flags & TPU_ATTACH_MOTOR_CONTROL_CHANNEL) ? TPU_MODE_MOTOR_CONTROL : TPU_MODE_NORMAL;
    chan->aflags     = flags;
    if ((chan->aflags & TPU_ATTACH_INTERRUPT_ENABLE) && (chan->aflags & TPU_ATTACH_MOTOR_CONTROL_CHANNEL)) {
        chan->intr_msk = TPU_TSR_TIMER_MSK    // For Timer
                       | TPU_TSR_MOTOR_MSK;   // For Motor control
    }
    else if (chan->aflags & TPU_ATTACH_INTERRUPT_ENABLE) {
        chan->intr_msk = TPU_TSR_TIMER_MSK;   // For Timer
    } 
    else {
        chan->intr_msk = 0;
    }

    // map in CMT registers for CMT channel group, including descriptor memory
    if (tpu_map_registers(chan) == (uintptr_t)MAP_FAILED) {
        goto fail2;
    }

    tpu_timer_stop_counter(chan);
    tpu_disable_interrupt(chan);
    tpu_timer_set_clock(chan, chan->prescaler, chan->clock_edge);
    tpu_timer_set_mode(chan, chan->mode);
    tpu_timer_set_pin_state(chan, TPU_PIN_OUTPUT_1, 0);

    return chan;

fail2:
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;
    rsrcdbmgr_detach(&req, 1);

fail1:
    free(chan);
fail:
    return (NULL);
}


void tpu_deinit(void * hdl)
{
    tpu_chan_t *chan = hdl;
    rsrc_request_t  req = { 0 };

    if(NULL != chan) {
        tpu_timer_stop_counter(chan);
        tpu_disable_interrupt(chan);

        // release TPU resource
        req.length = 1;
        req.start  = req.end = chan->chan_idx;
        req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = chan->name;
        rsrcdbmgr_detach(&req, 1);

        // unmap registers
        tpu_unmap_registers(chan);

        free(hdl);
    }
}


static int tpu_timer_is_running(tpu_chan_t *chan) 
{
	return in16(chan->vbase + RCAR_GEN3_TPU_TSTR) & chan->tstr_mask;
}


static int tpu_disable_interrupt(tpu_chan_t *chan) 
{
    /* Clear status */
    out16(chan->regs + RCAR_GEN3_TPU_TSR, 0);
    /* Disable interrupts */
    out16(chan->regs + RCAR_GEN3_TPU_TIER, 0);		    

    return EOK;
}


static int tpu_enable_interrupt(tpu_chan_t *chan) 
{
    /* Clear status */
    out16(chan->regs + RCAR_GEN3_TPU_TSR, ~chan->intr_msk);
    /* Enable interrupts */
    out16(chan->regs + RCAR_GEN3_TPU_TIER, chan->intr_msk);

	return(EOK);
}


static int tpu_timer_start_counter(void * hdl)
{
    tpu_chan_t *chan = hdl;

	if(NULL != chan) {
		/* Start counter */
        out16(chan->vbase + RCAR_GEN3_TPU_TSTR, in16(chan->vbase + RCAR_GEN3_TPU_TSTR) | chan->tstr_mask);

        return EOK;
	}

    return EFAULT;
}


static int tpu_timer_stop_counter(void * hdl)
{
    tpu_chan_t *chan = hdl;
	int is_running = 0;

	if(NULL != chan) {
		is_running = tpu_timer_is_running(chan);

        /* Stop counter */
        out16(chan->vbase + RCAR_GEN3_TPU_TSTR, in16(chan->vbase + RCAR_GEN3_TPU_TSTR) & ~chan->tstr_mask);
	}

	return is_running;
}


static int tpu_timer_set_clock(void * hdl, tpu_prescalers_t prescaler, tpu_clock_edge_t clock_edge)
{
    tpu_chan_t *chan  = hdl;
    uint16_t tcr = 0;
    int ret = EOK, error = 0;

    if(NULL != chan) {
        switch(prescaler) {
            case TPU_CLOCK_DIV_1:
                tcr |= TPU_TCR_TPSC_1;
                break;
            case TPU_CLOCK_DIV_4:
                tcr |= TPU_TCR_TPSC_4;
                break;
            case TPU_CLOCK_DIV_16:
                tcr |= TPU_TCR_TPSC_16;
                break;
            case TPU_CLOCK_DIV_64:
                tcr |= TPU_TCR_TPSC_64;
                break;
            default:
                error++;
                break;
        }
        switch(clock_edge) {
            case TPU_CLOCK_EDGE_BOTH:
                if (prescaler == TPU_CLOCK_DIV_1) 
                    error++;
                tcr |= TPU_TCR_CKEG_BOTH;
                break;
            case TPU_CLOCK_EDGE_RISE:
                tcr |= TPU_TCR_CKEG_RISE;
                break;
            case TPU_CLOCK_EDGE_FALL:
                if (prescaler == TPU_CLOCK_DIV_1)
                    error++;
                tcr |= TPU_TCR_CKEG_FALL;
                break;
            default:
                error++;
                break;
        }

        if(error == 0) {
            /* Stop channel when changing registers */
            int is_running = tpu_timer_stop_counter(chan);       

            out16(chan->regs + RCAR_GEN3_TPU_TCR, 
                    (in16(chan->regs + RCAR_GEN3_TPU_TCR) & ~(TPU_TCR_TPSC_MSK | TPU_TCR_CKEG_MSK)) | tcr);          

            TPU_SLOG_INFO(2, "%s: TCR=0x%04x (0x%x)", __func__, in16(chan->regs + RCAR_GEN3_TPU_TCR), tcr);

            if(0 != is_running) tpu_timer_start_counter(chan);
        }
        else {
            TPU_SLOG_ERROR("%s: Invalid argument", __func__);
            ret = EINVAL;
        }
        
        return ret;
    }
    
    return EFAULT;
}


static int tpu_timer_set_clear_src(void * hdl, tpu_clear_src_t cclr)
{
    tpu_chan_t * chan = hdl;
	uint16_t tcr = 0;
    int ret = EOK;

	if(NULL != chan) {
        switch(cclr) {
            case TPU_TCNT_CLR_DIS:
                break;
            case TPU_TCNT_CLR_ON_MATCH_A:
                tcr = TPU_TCR_CCLR_TGRA;
                break;
            case TPU_TCNT_CLR_ON_MATCH_B:
                tcr = TPU_TCR_CCLR_TGRB;
                break;
            case TPU_TCNT_CLR_ON_MATCH_C:
                tcr = TPU_TCR_CCLR_TGRC;
                break;
            case TPU_TCNT_CLR_ON_MATCH_D:
                tcr = TPU_TCR_CCLR_TGRD;
                break;
            default:
                TPU_SLOG_ERROR("%s: Invalid mode", __func__);
                ret = EINVAL;
                break;
        }

        if (ret == EOK ) {
            int is_running = tpu_timer_stop_counter(chan);

            out16(chan->regs + RCAR_GEN3_TPU_TCR, 
                    (in16(chan->regs + RCAR_GEN3_TPU_TCR) & ~TPU_TCR_CCLR_MASK) | tcr);

            TPU_SLOG_INFO(2, "%s: TCR=0x%04x (0x%x)", __func__, in16(chan->regs + RCAR_GEN3_TPU_TCR), tcr);

            if (is_running) tpu_timer_start_counter(chan);
        }

        return ret;
    }

    return EFAULT;
}


static int tpu_timer_set_mode(void * hdl, tpu_mode_t mode) 
{
    tpu_chan_t * chan = hdl;
	uint16_t tmdr = 0;
    int ret = EOK;

	if(NULL != chan) {
        if(mode & TPU_MODE_PWM) {
            /* PWM */
            tmdr = TPU_TMDR_MD_PWM;
        }
        if(mode & TPU_MODE_TGRA_BUF_OP) {
            /* TGRA and TGRC are used for buffered operation */
            tmdr |= TPU_TMDR_BFA;
        }
        if(mode & TPU_MODE_TGRB_BUF_OP) {
            /* TGRB and TGRD are used for buffered operation */
            tmdr |= TPU_TMDR_BFB;
            
        }
        if(mode & TPU_MODE_BUF_WR_ON_CLEAR) {
            /* TGRA and TGRB are rewritten on clearing of the counter */
            tmdr |= TPU_TMDR_BFWT;
        }

        if (ret == EOK ) {
            int is_running = tpu_timer_stop_counter(chan);

            out16(chan->regs + RCAR_GEN3_TPU_TMDR, tmdr);

            chan->mode = mode;

            TPU_SLOG_INFO(2, "%s: TMDR=0x%04x (0x%x)", __func__, in16(chan->regs + RCAR_GEN3_TPU_TMDR), tmdr);
            
            if (is_running) tpu_timer_start_counter(chan);
        }

        return ret;
    }

	return EFAULT;
}


static int tpu_timer_set_pin_state(void * hdl, tpu_pin_state_t state, int initial) 
{
    tpu_chan_t *chan = hdl;
    int ret = EOK;
    uint16_t tior = 0;
    
    if(NULL != chan) {
        if (chan->mode == TPU_MODE_PWM) {
            switch(state) {
                case TPU_PIN_OUTPUT_0:
                    tior |= TPU_TIOR_IOA2 | TPU_TIOR_IOA0;
                    break;
                case TPU_PIN_OUTPUT_1:
                    tior |= TPU_TIOR_IOA1;
                    break;
                default:
                    ret = EINVAL;
                    break;
            }
        }
        else {
            if (initial) {
                tior |= TPU_TIOR_IOA2;
            }
            switch(state) {
                case TPU_PIN_OUTPUT_0:
                    tior |= TPU_TIOR_IOA0;
                    break;
                case TPU_PIN_OUTPUT_1:
                    tior |= TPU_TIOR_IOA1;
                    break;
                case TPU_PIN_OUTPUT_TOGGLE:
                    tior |= TPU_TIOR_IOA0 | TPU_TIOR_IOA1;
                    break;
                default:
                    ret = EINVAL;
                    break;
            }
        }

        if (ret == EOK) {
            int is_running = tpu_timer_stop_counter(chan);

            out16(chan->regs + RCAR_GEN3_TPU_TIOR, tior);

            TPU_SLOG_INFO(2, "%s: TIOR=0x%04x (0x%x)", __func__, in16(chan->regs + RCAR_GEN3_TPU_TIOR), tior);

            if (is_running) tpu_timer_start_counter(chan);            
        }
        else {
            TPU_SLOG_ERROR("%s: Invalid argument", __func__);
            ret = EINVAL;
        }
    
        return ret;
    }

	return EFAULT;      
}


static int tpu_timer_set_general_reg(void *hdl, tpu_general_reg_t reg, uint16_t val) 
{
    tpu_chan_t *chan = hdl;
    int ret = EOK;

	if(NULL != chan) {
        switch (reg) {
            case TPU_REG_GENERAL_A:
                out16(chan->regs + RCAR_GEN3_TPU_TGRA, val);
                break;
            case TPU_REG_GENERAL_B:
                out16(chan->regs + RCAR_GEN3_TPU_TGRB, val);
                break;
            case TPU_REG_GENERAL_C:
                out16(chan->regs + RCAR_GEN3_TPU_TGRC, val);
                break;
            case TPU_REG_GENERAL_D:
                out16(chan->regs + RCAR_GEN3_TPU_TGRD, val);
                break;
            default:
                TPU_SLOG_ERROR("%s: Invalid argument", __func__);
                ret = EINVAL;
        }
    
        return ret;
    }

	return EFAULT;
}


static int tpu_timer_get_general_reg(void *hdl, tpu_general_reg_t reg, uint16_t * const val) 
{
    tpu_chan_t *chan = hdl;
    int ret = EOK;

	if(NULL != chan) {
        switch (reg) {
        case TPU_REG_GENERAL_A:
            *val = in16(chan->regs + RCAR_GEN3_TPU_TGRA);
            break;
        case TPU_REG_GENERAL_B:
            *val = in16(chan->regs + RCAR_GEN3_TPU_TGRB);
            break;
        case TPU_REG_GENERAL_C:
            *val = in16(chan->regs + RCAR_GEN3_TPU_TGRC);
            break;
        case TPU_REG_GENERAL_D:
            *val = in16(chan->regs + RCAR_GEN3_TPU_TGRD);
            break;
        default:
            TPU_SLOG_ERROR("%s: Invalid argument", __func__);
            ret = EINVAL;
        }
    
        return ret;
    }

	return EFAULT;
}


static int tpu_timer_set_count(void * hdl, uint16_t count)
{
    tpu_chan_t *chan = hdl;

	if(NULL != chan) {
        out16(chan->regs + RCAR_GEN3_TPU_TCNT, count);

        TPU_SLOG_INFO(2, "%s: TCNT=%04x (%04x)", __func__, in16(chan->regs + RCAR_GEN3_TPU_TCNT), count);
        
        return EOK;
    }

	return EFAULT;
}


static int tpu_timer_get_count(void *hdl, uint16_t * const count)
{
    tpu_chan_t *chan = hdl;

	if(NULL != chan) {
		*count = in16(chan->regs + RCAR_GEN3_TPU_TCNT);
	
        return EOK;
    }

	return EFAULT;
}


#define    NSEC_PER_SEC  (1000*1000*1000)

static int tpu_config_waveform_config(void * hdl, int duty_ns, int period_ns) 
{
    tpu_chan_t *chan = hdl;
    int prescaler, prescalers[TPU_CLK_PRESCALERS_NUM] = { TPU_CLOCK_DIV_1, TPU_CLOCK_DIV_4, TPU_CLOCK_DIV_16, TPU_CLOCK_DIV_64 };
    uint32_t period;
	uint32_t duty;
	int ret = EOK;
    int error = 0;

    if(NULL != chan) {
        /* Find the highest acceptable clock prescaler */
        for (prescaler = 0; prescaler < TPU_CLK_PRESCALERS_NUM; ++prescaler) {
            period = chan->pclock / prescalers[prescaler] / (NSEC_PER_SEC / period_ns);
            if (period <= 0xffff)
                break;
        }

        if (prescaler == TPU_CLK_PRESCALERS_NUM || period == 0) {
            TPU_SLOG_ERROR("%s: clock rate mismatch", __func__);
            ret = ENOTSUP;
        }

        if (duty_ns) {
            duty = chan->pclock / prescalers[prescaler] / (NSEC_PER_SEC / duty_ns);
            if (duty > period) {
                TPU_SLOG_ERROR("%s: Unable to find an valid duty(%d) and period(%d) waveform", __func__, duty, period);
                ret = EINVAL;
            }
        } else {
            duty = 0;
        }

        if (ret == EOK) {
            if ((chan->prescaler == prescalers[prescaler]) && (chan->period == period)) {
                /* Just change duty cycle only */
                error = tpu_timer_set_general_reg(hdl, TPU_REG_GENERAL_A, duty);
            }
            else {
                /* 
                 * Configure a new waveform 
                 */
                int is_running = tpu_timer_stop_counter(chan);

                 /* Count on rising edge and Set prescaler */
                if ((!error) && tpu_timer_set_clock(hdl, chan->prescaler, TPU_CLOCK_EDGE_RISE))
                    error++;

                /* Clear TCNT on TGRB match */
                if ((!error) && tpu_timer_set_clear_src(hdl, TPU_TCNT_CLR_ON_MATCH_B))
                    error++;

                /* Set PWM mode */
                if ((!error) && tpu_timer_set_mode(hdl, TPU_MODE_PWM))
                    error++;

                 /* Output 1 until TGRA, output 0 until TGRB */
                if ((!error) && tpu_timer_set_pin_state(hdl, TPU_PIN_OUTPUT_0, TPU_PIN_INITIAL_1))
                    error++;

                if ((!error) && tpu_timer_set_general_reg(hdl, TPU_REG_GENERAL_A, chan->duty))
                    error++;
                if ((!error) && tpu_timer_set_general_reg(hdl, TPU_REG_GENERAL_B, chan->period))
                    error++;

                if (is_running && !error) tpu_timer_start_counter(chan);
            }

            if (!error) {
                chan->prescaler = prescalers[prescaler];
                chan->duty = duty;
                chan->period = period;

                TPU_SLOG_INFO(2, "%s: clock rate %u, prescaler %u, period %u, duty %u\n", __func__,
                        chan->pclock, chan->prescaler, chan->period, chan->duty);
            }
            else {
                TPU_SLOG_ERROR("%s: Cannot configure waveform with duty %dns and period %dns correctly", __func__, duty_ns, period_ns);
                ret = EIO;
            }
        }

        return ret;
    }

	return EFAULT;
}


static int tpu_start(void * hdl, uint16_t * count) 
{
    tpu_chan_t * chan = hdl;    

	if(NULL != chan) {
        if (chan->mode == TPU_MODE_PWM) {
            uint16_t tior = in16(chan->regs + RCAR_GEN3_TPU_TIOR);
            if ((tior != TPU_TIOR_IOA1) && (tior != (TPU_TIOR_IOA2 | TPU_TIOR_IOA0))) {
                TPU_SLOG_ERROR("%s: Pin setting (%x) is not supported in PWM mode", __func__, tior);
                return ENOSR;
            }
        }
		tpu_timer_stop_counter(chan);

		/* Reset overflow count */
		chan->overflow_cnt = 0;

        if (count) {
            *count = in16(chan->regs + RCAR_GEN3_TPU_TCNT);
        }

        /* Enable interrupt */
        tpu_enable_interrupt(chan);

		TPU_SLOG_INFO(2, "%s: TCR=%04x, TMDR=%04x, TIOR=%04x, TIER=%04x, TCNT=%04x", __func__, 
                        in16(chan->regs + RCAR_GEN3_TPU_TCR),
                        in16(chan->regs + RCAR_GEN3_TPU_TMDR),
                        in16(chan->regs + RCAR_GEN3_TPU_TIOR),
                        in16(chan->regs + RCAR_GEN3_TPU_TIER),
                        in16(chan->regs + RCAR_GEN3_TPU_TCNT));

		/* Start counter */
		tpu_timer_start_counter(chan);
	
        return EOK;
    }

	return EFAULT;
}


static int tpu_stop(void * hdl, uint16_t * count) 
{
    tpu_chan_t * chan = hdl;

	if(NULL != chan) {
		/* Stop counter */
		tpu_timer_stop_counter(chan);

        if (count) {
            *count = in16(chan->regs + RCAR_GEN3_TPU_TCNT);
        }

		/* Disable interrupt */
		return tpu_disable_interrupt(chan);
    }

	return EFAULT;
}


static int tpu_timer_intr_isr(void *hdl, uint16_t * const tsr)
{
    tpu_chan_t *chan = hdl;
    uint16_t val;
    
    if(NULL != chan) {
        val  = in16(chan->regs + RCAR_GEN3_TPU_TSR);
        val &= in16(chan->regs + RCAR_GEN3_TPU_TIER);

        // Clear interrupt flags
        out16(chan->regs + RCAR_GEN3_TPU_TSR, ~val);

        if (val & TPU_TSR_TCFV) {
            chan->overflow_cnt++;
        }

        if (tsr) *tsr = val;

        return EOK;
    }

	return EFAULT;
}


int get_tpufuncs(tpu_functions_t *functable, int tabsize)
{
    TPU_ADD_FUNC(functable, init, tpu_init, tabsize);
    TPU_ADD_FUNC(functable, fini, tpu_fini, tabsize);
    TPU_ADD_FUNC(functable, channel_attach, tpu_channel_attach, tabsize);
    TPU_ADD_FUNC(functable, channel_release, tpu_deinit, tabsize);
    TPU_ADD_FUNC(functable, start_timer, tpu_timer_start_counter, tabsize);
    TPU_ADD_FUNC(functable, stop_timer, tpu_timer_stop_counter, tabsize);
    TPU_ADD_FUNC(functable, start, tpu_start, tabsize);
    TPU_ADD_FUNC(functable, stop, tpu_stop, tabsize);
    TPU_ADD_FUNC(functable, set_mode, tpu_timer_set_mode, tabsize);
    TPU_ADD_FUNC(functable, set_clear_src, tpu_timer_set_clear_src, tabsize);
    TPU_ADD_FUNC(functable, set_clock, tpu_timer_set_clock, tabsize);    
    TPU_ADD_FUNC(functable, set_pin_state, tpu_timer_set_pin_state, tabsize);
    TPU_ADD_FUNC(functable, set_general_reg, tpu_timer_set_general_reg, tabsize);
    TPU_ADD_FUNC(functable, get_general_reg, tpu_timer_get_general_reg, tabsize);
    TPU_ADD_FUNC(functable, set_count, tpu_timer_set_count, tabsize);
    TPU_ADD_FUNC(functable, get_count, tpu_timer_get_count, tabsize);
    TPU_ADD_FUNC(functable, config_waveform, tpu_config_waveform_config, tabsize);
    TPU_ADD_FUNC(functable, intr_isr, tpu_timer_intr_isr, tabsize);

    return 0;
}
