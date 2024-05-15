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


#ifndef _HW_CMT_DRIVER_H_INCLUDED
#define _HW_CMT_DRIVER_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

/* flags to channel_attach */
typedef enum {
	CMT_ATTACH_ANY_CHANNEL =        0x00000001,	/* Attach to any free CMT channel */
	CMT_ATTACH_PRIORITY_STRICT =    0x00000002,	/* "priority" parameter exactly specifies required priority */
    CMT_ATTACH_ANY_CLOCK_SRC =      0x00000004,	/* Attach to a channel that counts on any clock source */
    CMT_ATTACH_CLOCK_RCLK =         0x00000008,	/* Attach to a channel that counts on RCLK clock */
    CMT_ATTACH_CLOCK_CPEX =         0x00000010,	/* Attach to a channel that counts on CPEX clock */
    CMT_ATTACH_CLOCK_PSEUDO =       0x00000020,	/* Attach to a channel that counts on Psuedo clock */
	CMT_ATTACH_EVENT_ON_OVERFLOW =  0x00000040,	/* Want an event on overflow */
    CMT_ATTACH_ISR_ON_OVERFLOW =    0x00000080,	/* Want an interrupt handler on overflow */
} cmt_attach_flags;


/* flags to channel_attach */
typedef enum {
	CMT_MODE_ONE_SHOT =	 0x00000001,	/* One-shot operation*/
	CMT_MODE_FREE_RUNING = 0x00000002,	/* Free-running operation */
} cmt_cm_mode_flags;

typedef enum {
    CMT_CLOCK_SRC_RCLK =    0x00000001,	/* Count on RCLK */
    CMT_CLOCK_SRC_CPEX =    0x00000002,	/* Count on CPEX */
    CMT_CLOCK_SRC_PSEUDO =  0x00000004,	/* Count on Pseudo */
} cmt_clk_src_flags;


typedef enum {
    CMT_CKS_RCLK_DIV_1 =		0x00000007,	/* Count on RCLK/1 */
	CMT_CKS_RCLK_DIV_8 =		0x00000004,	/* Count on RCLK/8 */
	CMT_CKS_RCLK_DIV_32 =		0x00000005,	/* Count on RCLK/32 */
	CMT_CKS_RCLK_DIV_128 =		0x00000006,	/* Count on RCLK/128 */
    CMT_CKS_CPEX_DIV_1 =		0x0000000B,	/* Count on CPEXφ/1 */
	CMT_CKS_CPEX_DIV_8 =		0x00000008,	/* Count on CPEXφ/8 */
	CMT_CKS_CPEX_DIV_32 =		0x00000009,	/* Count on CPEXφ/32 */
	CMT_CKS_CPEX_DIV_128 =		0x0000000A,	/* Count on CPEXφ/128 */
    CMT_CKS_PSEU_DIV_1 =		0x0000000F,	/* Count on Pseudo 32kHz/1 */
	CMT_CKS_PSEU_DIV_8 =		0x0000000C,	/* Count on Pseudo 32kHz/8 */
	CMT_CKS_PSEU_DIV_32 =		0x0000000D,	/* Count on Pseudo 32kHz/32 */
	CMT_CKS_PSEU_DIV_128 =		0x0000000E,	/* Count on Pseudo 32kHz/128 */
} cmt_clk_div_flags;

typedef struct cmt_channel_query {
    _Uint32t    cmt_idx;         /* 0 : CMT0; 1 : CMT1; 2 : CMT2; 3 : CMT3 */
	_Uint32t	chan_idx;        /* Channel index in 1 CMT group (CMT0 : 0-1; CMT1/CMT2/CMT3 : 0-7) */
	_Uint32t	irq;
    _Uint32t    counter_size;    /* counter size that is selected */
    _Uint32t    clock_src;       /* One of RCLK, CPEX, Pseudo that is selected (one of cmt_clk_src_flags) */
    _Uint32t    clock_div;       /* clock div (one of cmt_clk_div_flags */
    _Uint32t    frequency;       /* clock value */
	_Uint32t	reserved[16];
} cmt_channel_query_t;

typedef struct cmt_channel_info {
    _Uint32t    counter_size_max;    /* Max counter size 48 bits or 32 bits */
    _Uint32t    clock_src;           /* One of or many of RCLK, CPEX, Pseudo */
    _Uint32t    frequency_max;       /* max clock value */
	_Uint32t	reserved[16];
} cmt_channel_info_t;

struct cmt_factory {
	int		 (*init)(char *options);
	void	 (*fini)(void);
    int 	 (*query_channel)(void *handle, cmt_channel_query_t *chinfo);
    int 	 (*channel_info)(int cmt_idx, int chan_idx, cmt_channel_info_t *chinfo);    
    void *	 (*channel_attach)(const struct sigevent *event, const struct sigevent * (* unhandler)(void *, int), 
                const void * data, int timer_bitsz, int priority, cmt_attach_flags flags);
	void	 (*channel_release)(void *handle);
	int		 (*setup_counter)(void *handle, uint64_t cmconst, int cntr_size, 
                cmt_cm_mode_flags mode, cmt_clk_div_flags cks_div);
    int		 (*set_const)(void *handle, uint64_t value);
    uint64_t (*get_const)(void *handle);
	int      (*set_count)(void *handle, uint64_t value);
    uint64_t (*get_count)(void *handle, int stop);
	int		 (*start_count)(void *handle);
	int		 (*stop_count)(void *handle);
    int		 (*overflow)(void *handle);
    uint64_t (*get_cycle)(void *handle, int stop);
    uint64_t (*change_cycle_to_ns)(void *handle, uint64_t tick, cmt_clk_div_flags clk_div);
    uint64_t (*change_ns_to_cycle)(void *handle, uint64_t ns, cmt_clk_div_flags clk_div);
} cmt_factory_t;

#define BITMASK(x,y)        (((2ULL << (y-x)) - 1) << x)

#define _SLOGC_CMT				0

int cmt_slog_verbose;	// -v for verbose operation

static void cmt_slogf(int severity, int verbose_level, const char *fmt, ...)
{
    int      status = 0;
    va_list  arg;

    if( verbose_level <= cmt_slog_verbose)
    {
        va_start(arg, fmt);
        status = vslogf(_SLOG_SETCODE(_SLOGC_TEST, _SLOGC_CMT), severity, fmt, arg);
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

#endif /* _HW_CMT_H_INCLUDED */

