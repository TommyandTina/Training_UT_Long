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
 
#ifndef RCAR_TMU_PROTO_H_
#define RCAR_TMU_PROTO_H_

#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <hw/inout.h>
#include <hw/rcar_tmu.h>

struct tmu_dev_s;
struct tmu_ocb_s;


#define RCAR_TMU_CHANNEL_MIN 0
#define RCAR_TMU_CHANNEL_MAX 15

#define RCAR_GEN3_CPG_133    133333333
#define RCAR_GEN3_CPG_125    125000000
#define RCAR_GEN3_CPG_120    120000000
#define RCAR_GEN3_CPG_CP      16666666
#define RCAR_V3M_CPG_CP       10000000
#define RCAR_D3_CPG_CP        24000000
#define RCAR_E3_CPG_CP        24000000

/*******************************************************************************
 * Timer Start Register (TSTRT) Bits
 ******************************************************************************/
#define TMU_TSTRT_STR0		(1 << 0)	// TMU0, TMU3, TMU6, TMU9,  TMU12
#define TMU_TSTRT_STR1		(1 << 1)	// TMU1, TMU4, TMU7, TMU10, TMU13
#define TMU_TSTRT_STR2		(1 << 2)	// TMU2, TMU5, TMU8, TMU11, TMU14

/*******************************************************************************
 * Timer Control Register (TCR) Bits
 ******************************************************************************/
#define TMU_TCR_ICPF		(1 << 9)
#define TMU_TCR_UNF			(1 << 8)
#define TMU_TCR_ICP			(2 << 6)	// Input Capture enabled, no interrupt
#define TMU_TCR_ICP_INT		(3 << 6)	// Input Capture enabled, with interrupt
// #define TMU_TCR_ICP_INT_	(0x01 << 6)	// Input Capture enabled, with interrupt (01: setting prohibited)
#define TMU_TCR_UNIE		(1 << 5)
#define TMU_TCR_CKEG_RISE	(0 << 3)	// Count on rising edge
#define TMU_TCR_CKEG_FALL	(1 << 3)	// Count on falling edge
#define TMU_TCR_CKEG_BOTH	(3 << 3)	// Count on rising and falling edge
#define TMU_TCR_CKEG_MSK	(3 << 3)	// Count on rising and falling edge
#define TMU_TCR_TPSC_4		(0 << 0)	// Timer Prescalar = Clkp/4
#define TMU_TCR_TPSC_16		(1 << 0)	// Timer Prescalar = Clkp/16
#define TMU_TCR_TPSC_64		(2 << 0)	// Timer Prescalar = Clkp/64
#define TMU_TCR_TPSC_256	(3 << 0)	// Timer Prescalar = Clkp/256
#define TMU_TCR_TPSC_1024	(4 << 0)	// Timer Prescalar = Clkp/1024
#define TMU_TCR_TPSC_EXT	(7 << 0)	// Count on external clock (TCLK; channel 3 to 14)
#define TMU_TCR_TPSC_MSK	(7 << 0)	// Timer Prescalar = Clkp/4

typedef enum tmu_condflag_e
{
	TMU_COND_DATA_INVALID=0,
	TMU_COND_DATA_VALID
}tmu_condflag_t;

/*
 * Flag used to indicate which edge we want to look for with input
 * capture mode
 */
typedef enum
{
	TMU_INTR_EDGE_NONE = 0x00,
	TMU_INTR_EDGE_RISING = 0x01,
	TMU_INTR_EDGE_FALLING = 0x02,
	TMU_INTR_EDGE_BOTH = 0x03

}tmu_edge_intr_t;

/*
 * Flags used to indicate which mode we want to operate in
 */
typedef enum
{
	TMU_ISR_FLAG_UNDERFLOW = TMU_TCR_UNF,	// Interrupted because of counter underflow
	TMU_ISR_FLAG_IN_CAP = TMU_TCR_ICPF		// Interrupted because of input capture
}tmu_isrflags_t;


typedef struct tmu_regs_s {
	int		        tstrt;
	int		        tcor;
	int		        tcnt;
	int		        tcr;
	int		        tcpr;    
}tmu_regs_t;

typedef struct tmu_chan_s {
    uintptr_t		paddr;		    // Physical address of the TMU device on the target
    uintptr_t		vbase;		    // mmap'ed pointer to the TMU device on the target
    uint32_t		offset;		    // Offset of this channel within a TMU "block"
    tmu_regs_t      regs;
    uint8_t         channels;       // Max channel number 
	uint8_t			chan_idx;
	tmu_mode_t		mode;		    // Operational Mode (normal or input capture)
    tmu_clock_edge_t	clock_edge;		// clock edge
	tmu_clock_div_t	clock_div;	    // TMU pclk divisor
	int      		isr_prio;	    // Priority of ISR thread
	int		        tstrt_mask;	    // Bit mask for this counter in TSTRT
    tmu_edge_intr_t edge_intr;	    // Current edge interrupt generation
    int			    undr_irq;	    // Interrupt for underflow
    int			    undr_iid;	    // Interrupt for underflow
    int			    incap_irq;	    // Interrupt for input capture
    int			    incap_iid;	    // Interrupt for input capture
    uint32_t		flags;
    uint8_t			stop;			// Flag used to shutdown interrupt thread
	uint32_t		clkp;		    // Peripheral Clock frequency
	uint32_t 		constant;	    // Initial counter constant (TCOR)
	uint64_t		underflow_cnt;	// InterruptWait() timeout count
    char            name[16];       // "tmu"
}tmu_chan_t;

#define TMU_SLOG_INFO(verbose_level, format, args... )    do { tmu_slogf( _SLOG_INFO,    verbose_level, format, ##args ); } while(0)
#define TMU_SLOG_WARNING(verbose_level, format, args... ) do { tmu_slogf( _SLOG_WARNING, verbose_level, format, ##args ); } while(0)
#define TMU_SLOG_ERROR( format, args... )                 do { tmu_slogf( _SLOG_ERROR,   0,             format, ##args ); } while(0)

static void rcar_tmu_disable_interrupt(tmu_chan_t * const chan);
static int rcar_tmu_start_counter(void * handle);
static int rcar_tmu_stop_counter(void * handle);
static int rcar_tmu_set_constant(void * handle, const uint32_t tmucor);
static int rcar_tmu_set_mode(void * handle, tmu_mode_t mode);
static int rcar_tmu_set_count(void * handle, const uint32_t count);
static int rcar_tmu_set_clockdiv(tmu_chan_t * const chan, tmu_clock_div_t clockdiv);
static int rcar_tmu_set_clock_edge(void * hdl, const tmu_clock_edge_t clock_edge);

#endif /* RCAR_TMU_PROTO_H_ */
