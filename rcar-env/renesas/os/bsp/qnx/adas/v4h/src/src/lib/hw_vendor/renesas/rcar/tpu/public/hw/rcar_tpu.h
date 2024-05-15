/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#ifndef _HW_TPU_DRIVER_H_INCLUDED
#define _HW_TPU_DRIVER_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

/* flags to channel_attach */
typedef enum {
	TPU_ATTACH_ANY_CHANNEL =	        0x00000001,	/* Attach to any free channel */
	TPU_ATTACH_MOTOR_CONTROL_CHANNEL =	0x00000002,	/* Attach to stepping-motor control channel */
	TPU_ATTACH_PRIORITY_STRICT =	    0x00000004,	/* "priority" parameter exactly specifies required priority */
	TPU_ATTACH_INTERRUPT_ENABLE =	    0x00000008,	/* Enable Interrupts */
} tpu_attach_flags;

/*
 * TPU clock divisor
 */
typedef enum {
	TPU_CLOCK_DIV_1  	 = 1,	// Count on Internal Clock/1
	TPU_CLOCK_DIV_4      = 4,	// Count on Internal Clock/4
	TPU_CLOCK_DIV_16     = 16,	// Count on Internal Clock/16
	TPU_CLOCK_DIV_64     = 64,	// Count on Internal Clock/64
	TPU_CLOCK_DIV_INVAL  = -1
} tpu_prescalers_t;

/*
 * TPU clock edge
 */
typedef enum {
    TPU_CLOCK_EDGE_RISE = 0,    // Count on rising edge
    TPU_CLOCK_EDGE_FALL,        // Count on falling edge
    TPU_CLOCK_EDGE_BOTH,        // Count on rising and falling edge    
    TPU_CLOCK_EDGE_INVAL
} tpu_clock_edge_t;


/*
 * TPU operational modes
 */
typedef enum {
    TPU_TCNT_CLR_DIS = 0,      // TCNT cleared when overflow 
    TPU_TCNT_CLR_ON_MATCH_A,   // TCNT cleared in response to TGRA compare match
    TPU_TCNT_CLR_ON_MATCH_B,   // TCNT cleared in response to TGRB compare match
    TPU_TCNT_CLR_ON_MATCH_C,   // TCNT cleared in response to TGRC compare match
    TPU_TCNT_CLR_ON_MATCH_D,   // TCNT cleared in response to TGRD compare match
} tpu_clear_src_t;


/*
 * TPU operational modes
 */
typedef enum {
	TPU_MODE_NORMAL            = 0x00000001,  // Normal counter, free-running/periodic mode
	TPU_MODE_PWM               = 0x00000002,  // PWM mode
	TPU_MODE_TGRA_NORMAL_OP    = 0x00000010,  // TGRA normal operation
    TPU_MODE_TGRA_BUF_OP       = 0x00000020,  // TGRA and TGRC are used for buffered operation
	TPU_MODE_TGRB_NORMAL_OP    = 0x00000100,  // TGRB normal operation
	TPU_MODE_TGRB_BUF_OP       = 0x00000200,  // TGRB and TGRD are used for buffered operation
    TPU_MODE_BUF_WR_ON_MATCH   = 0x00001000,  // TGRA and TGRB are rewritten on a compare match with the counters
    TPU_MODE_BUF_WR_ON_CLEAR   = 0x00002000,  // TGRA and TGRB are rewritten on clearing of the counter
	TPU_MODE_MOTOR_CONTROL     = 0x00010000,  // stepping-Motor control mode
} tpu_mode_t;

typedef enum {
    TPU_PIN_INITIAL_0 = 0,  // Initial output 0
    TPU_PIN_INITIAL_1,      // Initial output 1
	TPU_PIN_OUTPUT_0,       // Output 0 on TGRA macth
	TPU_PIN_OUTPUT_1,       // Output 1 on TGRA macth
    TPU_PIN_OUTPUT_TOGGLE,  // Toggle on TGRA macth
} tpu_pin_state_t;


typedef enum {
	TPU_REG_GENERAL_A = 0,      // TGRA register
	TPU_REG_GENERAL_B,          // TGRB register
    TPU_REG_GENERAL_C,          // TGRC register
    TPU_REG_GENERAL_D,          // TGRD register
} tpu_general_reg_t;

typedef enum {
    TPU_INTR_TIMER_MATCH_A	   = 0x00000001,	// TCNT is matching TGRA
    TPU_INTR_TIMER_MATCH_B	   = 0x00000002,	// TCNT is matching TGRB 
    TPU_INTR_TIMER_MATCH_C	   = 0x00000004,	// TCNT is matching TGRC 
    TPU_INTR_TIMER_MATCH_D	   = 0x00000008,	// TCNT is matching TGRD
    TPU_INTR_TIMER_OVERF       = 0x00000010,	// TCNT is overflow 
    TPU_INTR_MOTOR_STOP_STATE  = 0x00000100,    // Motor Control Stop Transition Detection
    TPU_INTR_MOTOR_ACCE_STATE  = 0x00000200,    // Motor Control Acceleration Transition Detection
    TPU_INTR_MOTOR_NORM_STATE  = 0x00000400,    // Motor Control Normal Transition Detection
    TPU_INTR_MOTOR_DECE_STATE  = 0x00000800,    // Motor Control Deceleration Transition Detection
    TPU_INTR_MOTOR_TRAN_REQ    = 0x00001000,    // Motor Control Data Transfer Request Detection
    TPU_INTR_MOTOR_DMA_OVERF   = 0x00002000,    // Motor Control Data Transfer Overflow Detection
} tpu_intr_t;

typedef struct _tpu_functions {
	int  (*init)(char *options);
	void (*fini)(void);    
	//void *(*channel_attach)(const struct sigevent *event, const struct sigevent * (* handler)(void *, int), 
    //               void * data, int priority, tpu_attach_flags flags);
	void *(*channel_attach)(int priority, tpu_attach_flags flags);
	void (*channel_release)(void *handle);
    int  (*start_timer)(void *handle);
    int  (*stop_timer)(void *handle);
    int  (*start)(void *handle, uint16_t * count);
    int  (*stop)(void *handle, uint16_t * count);
    int  (*set_mode)(void *handle, tpu_mode_t mode);
    int  (*set_clear_src)(void * hdl, tpu_clear_src_t cclr);
    int  (*set_clock)(void *handle, tpu_prescalers_t clock_div, tpu_clock_edge_t clock_edge);
    int  (*set_general_reg)(void *handle, tpu_general_reg_t reg, uint16_t val);
    int  (*get_general_reg)(void *handle, tpu_general_reg_t reg, uint16_t * const val);
    int  (*set_pin_state)(void * handle, tpu_pin_state_t level, int initial);
    int  (*set_count)(void *handle, uint16_t count);
    int  (*get_count)(void *handle, uint16_t * const count);
    int  (*config_waveform)(void * hdl, int duty_ns, int period_ns);
    int  (*intr_isr)(void * handle, uint16_t * const tsr);
} tpu_functions_t;

/* Macro used by H/W driver when populating dma_functions table */
#define TPU_ADD_FUNC(table, entry, func, limit) \
	if (((size_t)&(((tpu_functions_t *)0)->entry)) + \
		sizeof (void (*)()) <= (limit)) \
		(table)->entry = (func);

typedef int (*get_tpufuncs_t)(tpu_functions_t *funcs, int tabsize);

int get_tpufuncs(tpu_functions_t *funcs, int tabsize);


#define _SLOGC_TPU				102

extern int tpu_slog_verbose; // -v for verbose operation

static void tpu_slogf(int severity, int verbose_level, const char *fmt, ...)
{
    int      status = 0;
    va_list  arg;

    if( verbose_level <= tpu_slog_verbose)
    {
        va_start(arg, fmt);
        status = vslogf(_SLOG_SETCODE(_SLOGC_TEST, _SLOGC_TPU), severity, fmt, arg);
        va_end(arg);

        /* If slogger not running, send output to stderr */
        if( -1 == status)
        {
            va_start(arg, fmt);
            status = vfprintf(stderr, fmt, arg);
            va_end(arg);
        }
    }
}

#endif /* _HW_TPU_DRIVER_H_INCLUDED */
