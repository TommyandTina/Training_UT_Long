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

#ifndef _HW_TMU_DRIVER_H_INCLUDED
#define _HW_TMU_DRIVER_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

/*
 * TMU clock divisor
 */
typedef enum {
	TMU_CLOCK_DIV_EXT   = 1,	// Count on external clock (TCLK)
	TMU_CLOCK_DIV_4     = 4,	// Count on Clkp/4
	TMU_CLOCK_DIV_16    = 16,	// Count on Clkp/16
	TMU_CLOCK_DIV_64    = 64,	// Count on Clkp/64
	TMU_CLOCK_DIV_256   = 256,	// Count on Clkp/256
	TMU_CLOCK_DIV_1024  = 1024,	// Count on Clkp/1024
	TMU_CLOCK_DIV_INVAL = -1

} tmu_clock_div_t;

/*
 * TMU Operational Modes
 */
typedef enum {
	TMU_MODE_INVAL      = 0,
	TMU_MODE_NORMAL     = 1,   // Normal counter mode
	TMU_MODE_INCAP      = 2,   // Input capture mode (channel 5, 8, 11, 14) 
}  tmu_mode_t;

typedef enum {
    TMU_CLOCK_EDGE_INVAL = 0,
    TMU_CLOCK_EDGE_RISE  = 1, // Count on rising edge
    TMU_CLOCK_EDGE_FALL	 = 2, // Count on falling edge
    TMU_CLOCK_EDGE_BOTH	 = 3, // Count on rising and falling edge    
} tmu_clock_edge_t;

/* flags to channel_attach */
typedef enum {
	TMU_ATTACH_ANY_CHANNEL =	    0x00000001, /* Attach to any free channel */
	TMU_ATTACH_PRIORITY_STRICT =    0x00000002, /* "priority" parameter exactly specifies required priority */
	TMU_ATTACH_UNDR_EVENT =	        0x00000004, /* Want an event on underflow completion */
	TMU_ATTACH_UNDR_ISR =           0x00000008, /* Want an handler on underflow completion */
	TMU_ATTACH_INCAP_EVENT =        0x00000010, /* Want an event on input capture completion */
	TMU_ATTACH_INCAP_ISR =          0x00000020, /* Want an handler on input capture completion */
} tmu_attach_flags;


typedef struct tmu_clock {
    int parent;                // parent clock
    int div;                   // clock div
    int req;                   // (parent clock) / (clock div)
    tmu_clock_edge_t clk_edge;
} tmu_clock_t;

struct tmu_factory {
	int  (*init)(char *options);
	void (*fini)(void);    
	void*(*channel_attach)(const struct sigevent *unevent, const struct sigevent * (* unhandler)(void *, int), 
               const struct sigevent *inevent, const struct sigevent * (* inhandler)(void *, int), 
               const void * data, int priority, tmu_attach_flags flags);
	void (*channel_release)(void *handle);
    int  (*start_counter)(void *handle);
    int  (*stop_counter)(void *handle);
    int  (*start)(void *handle, uint32_t * count);
    int  (*stop)(void *handle, uint32_t * count);
    int  (*set_mode)(void *handle, const tmu_mode_t mode);
    int  (*get_mode)(void *handle, tmu_mode_t * const mode);
    int  (*set_constant)(void *handle, const uint32_t tcor);
    int  (*get_constant)(void *handle, uint32_t * const tcor);
    int  (*set_count)(void *handle, const uint32_t count);
    int  (*get_count)(void *handle, uint32_t * const count);
    int  (*get_incapture)(void *handle, uint32_t * const tcpr);
    int  (*set_clock)(void *handle, tmu_clock_t * const clk);
    int  (*get_clock)(void *handle, tmu_clock_t * const clk);
    int  (*get_underflow_count)(void *handle, uint32_t * const underflow_count);
    int  (*get_tick)(void *handle, uint64_t * const tick);
    int  (*change_tick_to_sec)(void *handle, uint64_t tick, double * const sec);
    void (*underflow_isr)(void * handle);
    void (*incapture_isr)(void * handle);
} tmu_factory_t;

#define _SLOGC_TMU				0

int tmu_slog_verbose;	// -v for verbose operation

static void tmu_slogf(int severity, int verbose_level, const char *fmt, ...)
{
    int      status = 0;
    va_list  arg;

    if( verbose_level <= tmu_slog_verbose)
    {
        va_start(arg, fmt);
        status = vslogf(_SLOG_SETCODE(_SLOGC_TEST,_SLOGC_TMU), severity, fmt, arg);
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

#endif /* _HW_TMU_DRIVER_H_INCLUDED */
