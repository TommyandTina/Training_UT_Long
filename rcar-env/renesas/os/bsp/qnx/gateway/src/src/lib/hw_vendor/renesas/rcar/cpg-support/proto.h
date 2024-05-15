/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation.
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

#define IS_RCAR_GEN3(_id)      ((_id == PRODUCT_ID_RCAR_H3)  || \
                                (_id == PRODUCT_ID_RCAR_M3W) || (_id == PRODUCT_ID_RCAR_M3N) || \
                                (_id == PRODUCT_ID_RCAR_V3M) || (_id == PRODUCT_ID_RCAR_V3H) || \
                                (_id == PRODUCT_ID_RCAR_D3)  || (_id == PRODUCT_ID_RCAR_E3))

#define IS_RCAR_GEN4(_id)      ((_id == PRODUCT_ID_RCAR_V3U) || (_id == PRODUCT_ID_RCAR_S4))

#define CPG_RCAR_PRODUCT_REG   RCAR_PRODUCT_REGISTER

typedef struct dev_mem_ {
    uint32_t         pbase;
    uintptr_t        vbase;
    uint32_t         size;
} dev_mem_t;

typedef struct pmic_dev_ {
    pmic_model_t  model;
    int           i2c_bus;
    int           i2c_slave_addr;
    int           i2c_fd;
} pmic_dev_t;

typedef struct cpg_dev_ {
    dev_mem_t        cpg;
    dev_mem_t        avs;        // Optional: needed for cpu_freq_set only
    int              verbose;
    uint32_t         soc_id;
    uint32_t         soc_rev;
    uint32_t         ext_clk;
    pmic_dev_t       pmic;       // Optional: needed for cpu_freq_set only
    uint32_t         wpr_off;
    uint32_t         ecr_off;
    uint32_t         frqcrb_off;
	int              inited;
} cpg_dev_t;

extern cpg_dev_t cpg_dev;

#if defined VARIANT_a
/* Static library is used for Startup/IPL only
 * You must define your own nanospin and cpg_slogf functions */
extern int iic_write_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);
extern void kprintf(const char *fmt, ... );
extern void usec_delay(uint32_t usec);

#define cpg_slogf(vlevel, fmt, args...)        \
            if( vlevel <= cpg_dev.verbose ) {  \
                kprintf(fmt"\n", ##args);      \
            }
#else
int cpg_slogf(const int vlevel, const char * const fmt, ...);
#endif

int   cpg_init(const cpg_opts_t* const cpg_opts, const avs_opts_t* const avs_opts);
void  cpg_deinit(void);

int mstp_config(const int mod_id, const int enable);
int mstp_status(const int mod_id, int *status);
int mstp_reset(const int mod_id);
int mstp_reset_config(const int mod_id, const int assert);
int mstp_reset_state(const int mod_id, int *status);

int clk_config(const cpg_clk_id_t clk_id, const int idx, const int enable);
int clk_rate(const cpg_clk_id_t clk_id, const int idx, unsigned *rate, const int set);
int clk_divider(const cpg_clk_id_t clk_id, const int idx, unsigned *clk_div, const int set);
int clk_status(const cpg_clk_id_t clk_id, const int idx, int *status);

uint32_t cpg_read(uint32_t reg);
void cpg_write(uint32_t reg, uint32_t data);
int cpg_waitfor(const uint32_t reg, const uint32_t mask, const uint32_t val);
int cpg_init_common(const cpg_opts_t* const cpg_opts, const avs_opts_t* const avs_opts, const uint32_t product_id);
int avs_cpu_freq_set(const uint32_t cpufreq);

int nanospin_ns( unsigned long nsec );

#endif /* _PROTO_H_INCLUDED */
