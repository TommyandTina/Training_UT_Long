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
 

#ifndef __RCAR_CMT_PROTO_H__
#define __RCAR_CMT_PROTO_H__

#include <inttypes.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <hw/rcar_cmt.h>

typedef struct _cmt_chan_s
{
    paddr_t         pbase;
    uintptr_t       vbase;
    uint32_t        size;
    uintptr_t       regs;
    uint32_t        channels;
    uint32_t        channels_per_group; // 2 for CMT0, 8 for CMT1/2/3
    uint64_t        counter_mask;
    uint32_t        chan_idx;         // 0, 1 for CMT0; 0, 1, 2,...23 for CMT1/2/3
    uint32_t        grp_idx;          // 0 for CMT0, 1 for CMT1, 2 for CMT2, 3 for CMT3
    uint32_t        chan_idx_in_grp;  // 0, 1 for CMT0; 0, 1, 2, ..., 7 for CMT1/2/3
    uint32_t        counter_size;     // 16, 32, 48
    int             counter_support_48bit; 
    cmt_clk_div_flags clock_div;    
    cmt_clk_src_flags clock_src;
    uint32_t        frequency;
    int             irq;
    int             iid;
    char            name[16];          // "cmt"
    uint64_t		overflow_cnt;	   // InterruptWait() timeout count
    cmt_attach_flags aflags;           // attach flag of client
    int             clk_rclk_max;
    int             clk_cpex_max;
    int             clk_pseu_max;
} cmt_chan_t;

typedef struct
{
    paddr_t         paddr;
    uintptr_t       vaddr;
    unsigned        num;    // attach count
} cmt_ctrl_t;


#define CMT_SLOG_INFO(verbose_level, format, args... )    do { cmt_slogf( _SLOG_INFO,    verbose_level, format, ##args ); } while(0)
#define CMT_SLOG_WARNING(verbose_level, format, args... ) do { cmt_slogf( _SLOG_WARNING, verbose_level, format, ##args ); } while(0)
#define CMT_SLOG_ERROR(format, args... )                  do { cmt_slogf( _SLOG_ERROR,   0,             format, ##args ); } while(0)


#endif /* #ifndef __RCAR_CMT_PROTO_H__ */
