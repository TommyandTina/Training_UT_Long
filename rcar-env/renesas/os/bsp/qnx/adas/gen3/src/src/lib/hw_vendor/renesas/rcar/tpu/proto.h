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


#ifndef _TPU_PROTO_H_INCLUDED
#define _TPU_PROTO_H_INCLUDED

#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <aarch64/r-car-gen3.h>
#include <hw/rcar_tpu.h>

#define RCAR_GEN3_TPU_BASE				0xE6E80000
#define RCAR_GEN3_TPU_SIZE 				0x200

#define RCAR_TPU_CHANNELS               4

#define RCAR_GEN3_TPU_0_OFFSET			0x000
#define RCAR_GEN3_TPU_1_OFFSET			0x040
#define RCAR_GEN3_TPU_2_OFFSET			0x080
#define RCAR_GEN3_TPU_3_OFFSET			0x0C0

/* Timer start register */
#define RCAR_GEN3_TPU_TSTR              0x00
 #define TPU_TSTR_CST0					(1 << 0)	// TPU0
 #define TPU_TSTR_CST1					(1 << 1)	// TPU1
 #define TPU_TSTR_CST2					(1 << 2)	// TPU2
 #define TPU_TSTR_CST3					(1 << 3)	// TPU3
 #define TPU_TSTR_TMST					(1 << 4)	// Motor stepping

/* Timer control register */
#define RCAR_GEN3_TPU_TCR				0x10
 #define TPU_TCR_TPSC_1					(0 << 0)	// Timer Prescaler = Internal Clock / 1
 #define TPU_TCR_TPSC_4					(1 << 0)	// Timer Prescaler = Internal Clock / 4
 #define TPU_TCR_TPSC_16				(2 << 0)	// Timer Prescaler = Internal Clock / 16
 #define TPU_TCR_TPSC_64				(3 << 0)	// Timer Prescaler = Internal Clock / 64
 #define TPU_TCR_TPSC_MSK				(7 << 0)	// Timer Prescaler = Internal Clock / 1

 #define TPU_CLK_PRESCALERS_NUM         4

 #define TPU_TCR_CKEG_RISE				(0 << 3)	// Count rising edges					
 #define TPU_TCR_CKEG_FALL    			(1 << 3)	// Count falling edges					
 #define TPU_TCR_CKEG_BOTH				(2 << 3)	// Count both edges					
 #define TPU_TCR_CKEG_MSK				(3 << 3)
 
 #define TPU_TCR_CCLR_NONE				(0 << 5)    // TCNT clearing disabled
 #define TPU_TCR_CCLR_TGRA				(1 << 5)	// TCNT cleared in response to TGRA compare match
 #define TPU_TCR_CCLR_TGRB				(2 << 5)	// TCNT cleared in response to TGRB compare match
 #define TPU_TCR_CCLR_TGRC				(5 << 5)	// TCNT cleared in response to TGRC compare match
 #define TPU_TCR_CCLR_TGRD				(6 << 5)	// TCNT cleared in response to TGRD compare match
 #define TPU_TCR_CCLR_MASK				(7 << 5)	

/* Timer mode register */
#define RCAR_GEN3_TPU_TMDR				0x14
 #define TPU_TMDR_BFWT					(1 << 6)	// Buffer Write Timing
 #define TPU_TMDR_BFB					(1 << 5)	// Buffer Operation B
 #define TPU_TMDR_BFA					(1 << 4)	// Buffer Operation A
 #define TPU_TMDR_MD_NORMAL				(0 << 0)	// Normal operation
 #define TPU_TMDR_MD_PWM				(2 << 0)	// PWM mode

/* Timer I/O control register */
#define RCAR_GEN3_TPU_TIOR				0x18
 #define TPU_TIOR_IOA0					(1 << 0)	// I/O Control bit 0
 #define TPU_TIOR_IOA1				    (1 << 1)	// I/O Control bit 1
 #define TPU_TIOR_IOA2				    (1 << 2)	// I/O Control bit 2

/* Timer interrupt status/enable register */
#define RCAR_GEN3_TPU_TIER				0x1C
#define RCAR_GEN3_TPU_TSR				0x20
 #define TPU_TSR_MOTOR_MSK			    (0x3F << 8)	// Interrupt for Motor control
 #define TPU_TSR_TMDOFS					(1 << 13)   // Motor Control Data Transfer Overflow Detection
 #define TPU_TSR_TMDRFS					(1 << 12)   // Motor Control Data Transfer Request Detection
 #define TPU_TSR_TMCFR					(1 << 11)   // Motor Control Deceleration Transition Detection
 #define TPU_TSR_TMCFT					(1 << 10)   // Motor Control Normal Transition Detection
 #define TPU_TSR_TMCFA					(1 << 9)    // Motor Control Acceleration Transition Detection
 #define TPU_TSR_TMCFS					(1 << 8)    // Motor Control Stop Transition Detection
 #define TPU_TSR_TIMER_MSK			    0x1F	    // Interrupt for Timer 
 #define TPU_TSR_TCFV					(1 << 4)	// Overflow 
 #define TPU_TSR_TGFD					(1 << 3)	// Compare Flag D 
 #define TPU_TSR_TGFC					(1 << 2)	// Compare Flag C 
 #define TPU_TSR_TGFB					(1 << 1)	// Compare Flag B 
 #define TPU_TSR_TGFA					(1 << 0)	// Output Compare Flag A 

#define RCAR_GEN3_TPU_TCNT				0x24		// Timer counter
#define RCAR_GEN3_TPU_TGRA				0x28		// Timer general register A 
#define RCAR_GEN3_TPU_TGRB				0x2C		// Timer general register B
#define RCAR_GEN3_TPU_TGRC				0x30		// Timer general register C
#define RCAR_GEN3_TPU_TGRD				0x44		// Timer general register D

/* Motor control */
#define RCAR_GEN3_TPU_TMIR              0x100
#define RCAR_GEN3_TPU_TMRR              0x104
#define RCAR_GEN3_TPU_TMSR              0x108
#define RCAR_GEN3_TPU_TMMPR0            0x10C
#define RCAR_GEN3_TPU_TMMPR1            0x110
#define RCAR_GEN3_TPU_TMSPR0            0x114
#define RCAR_GEN3_TPU_TMSPR1            0x118
#define RCAR_GEN3_TPU_TMOPR             0x11C
#define RCAR_GEN3_TPU_TMASR             0x120
#define RCAR_GEN3_TPU_TMTSR             0x130
#define RCAR_GEN3_TPU_TMRSR             0x134
#define RCAR_GEN3_TPU_TMSCR             0x140
#define RCAR_GEN3_TPU_TMTCR             0x144

extern int tpu_slog_verbose;

typedef struct
{
    paddr_t         paddr;
    uintptr_t       vaddr;
    unsigned        num;    // attach count
} tpu_ctrl_t;

typedef struct tpu_chan_s {
    uint32_t		pbase;		    // Physical address of the TPU module
    uintptr_t		vbase;		    // mmap'ed pointer to the TPU module
    int             size;
    uintptr_t       regs;           // mmap'ed pointer to the General Timer channel registers
    uintptr_t       motor_regs;     // mmap'ed pointer to the Motor Control registers
	int		        tstr_mask;	    // Bit mask for this counter in TSTRT
	int		        chan_idx;       // Channel index: 0, 1, 2, 3
    int             channels;
    int             pclock;         // Parent clock
	uint16_t 		prescaler;      // Timer prescaler		
	uint16_t 		clock_edge;		
    uint64_t        overflow_cnt;
    uint16_t        period;
    uint16_t        duty;
    uint16_t        intr_msk;       // Interrupt mask bits
    
	tpu_mode_t		mode;		    // Operational Mode 
    uint32_t		aflags;         // attach flag of client

    char            name[16];       // "tpu"
} tpu_chan_t;


#define TPU_SLOG_INFO(verbose_level, format, args... )    do { tpu_slogf( _SLOG_INFO,    verbose_level, format, ##args ); } while(0)
#define TPU_SLOG_WARNING(verbose_level, format, args... ) do { tpu_slogf( _SLOG_WARNING, verbose_level, format, ##args ); } while(0)
#define TPU_SLOG_ERROR(format, args... )                  do { tpu_slogf( _SLOG_ERROR,   0,             format, ##args ); } while(0)

static int tpu_disable_interrupt(tpu_chan_t *chan);
static int tpu_timer_start_counter(void *hdl);
static int tpu_timer_stop_counter(void *hdl);
static int tpu_timer_set_clock(void * hdl, tpu_prescalers_t clock_div, tpu_clock_edge_t clock_edge);
static int tpu_timer_set_mode(void * hdl, tpu_mode_t mode);
static int tpu_timer_set_pin_state(void * hdl, tpu_pin_state_t state, int initial);
static int tpu_timer_set_general_reg(void *hdl, tpu_general_reg_t reg, uint16_t val);
static int tpu_timer_get_general_reg(void *hdl, tpu_general_reg_t reg, uint16_t * const val);
static int tpu_timer_set_count(void * hdl, uint16_t count);
static int tpu_timer_get_count(void * hdl, uint16_t * const count);

#endif /* _TPU_PROTO_H_INCLUDED */
