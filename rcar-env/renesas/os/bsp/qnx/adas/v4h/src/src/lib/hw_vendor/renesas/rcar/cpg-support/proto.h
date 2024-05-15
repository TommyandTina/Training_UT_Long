/*
 * Copyright (c) 2020, 2023, BlackBerry Limited. All rights reserved.
 * Copyright (c) 2022, Renesas Electronics Corporation.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */


#ifndef _PROTO_H_INCLUDED
#define _PROTO_H_INCLUDED

#include <unistd.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <aarch64/r-car.h>
#include <hw/rcar_cpg_support.h>
#include <hw/inout.h>

#define ARRAY_SIZE(x)           ( (int)(sizeof(x) / sizeof((x)[0])) )
#ifdef BIT
#undef BIT
#define BIT(x)                  ( (uint32_t)(1 << (x)) )
#endif
#define CPG_REG_OFF_UNKNOWN     ~0U

typedef struct cpg_dev_ {
    uint32_t         pbase;
    uintptr_t        vbase;
    uint32_t         size;
    int              verbose;
    uint32_t         soc_id;
    uint32_t         soc_rev;
    uint32_t         ext_clk;
    uint32_t         wpr_off;
    uint32_t         ecr_off;
    uint32_t         frqcrb_off;
	int              inited;
} cpg_dev_t;

extern cpg_dev_t cpg_dev;

#if defined VARIANT_a
/* Static library is used for Startup/IPL only
 * You must define your own nanospin and cpg_slogf functions */
extern void kprintf(const char *fmt, ... );
extern void usec_delay(uint32_t usec);

#define cpg_slogf(vlevel, fmt, args...)        \
            if ( vlevel <= cpg_dev.verbose ) {  \
                kprintf(fmt"\n", ##args);      \
            }
#else
int cpg_slogf(const int vlevel, const char * const fmt, ...);
#endif

int   cpg_init(const cpg_opts_t* const cpg_opts);
void  cpg_deinit(void);

int mstp_config(const int mod_id, const int enable);
int mstp_status(const int mod_id, int *status);
int mstp_reset(const int mod_id);
int mstp_reset_config(const int mod_id, const int assert);
int mstp_reset_state(const int mod_id, int *status);
int mstp_reset_gen4_a(const int mod_id);
int mstp_reset_gen4_b(const int mod_id);
int mstp_reset_gen4_c(const int mod_id);
int mstp_assert_gen4_a(const int mod_id);
int mstp_assert_gen4_b(const int mod_id);
int mstp_assert_gen4_c(const int mod_id);
int mstp_deassert_gen4(const int mod_id);

int clk_config(const cpg_clk_id_t clk_id, const int idx, const int enable);
int clk_rate(const cpg_clk_id_t clk_id, const int idx, unsigned *rate, const int set);
int clk_divider(const cpg_clk_id_t clk_id, const int idx, unsigned *clk_div, const int set);
int clk_status(const cpg_clk_id_t clk_id, const int idx, int *status);
#ifdef VARIANT_pm
int clk_freq(const cpg_clk_id_t clk_id, const int idx, unsigned *freq_mhz, const int set);
int clk_opp(const cpg_clk_id_t clk_id, const int idx, uint32_t freq, uint32_t freq_rng, uint32_t* const opp);
#endif

uint32_t cpg_read(uint32_t reg);
void cpg_write(uint32_t reg, uint32_t data);
int cpg_waitfor(const uint32_t reg, const uint32_t mask, const uint32_t val);
int cpg_init_common(const cpg_opts_t* const cpg_opts, const uint32_t product_id);

int nanospin_ns( unsigned long nsec );

#endif /* _PROTO_H_INCLUDED */
