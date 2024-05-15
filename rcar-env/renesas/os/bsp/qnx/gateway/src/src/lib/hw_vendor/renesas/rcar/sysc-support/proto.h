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

#include <hw/inout.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <aarch64/r-car.h>
#include <hw/rcar_sysc_support.h>

#define ARRAY_SIZE(x)          ( (int)(sizeof(x) / sizeof((x)[0])) )
#ifdef BIT
#undef BIT
#define BIT(x)                 ( 1U << (x) )
#endif
#define SYSC_REG_OFF_UNKNOWN   ~0U

#define IS_RCAR_GEN3(_id)      ((_id == PRODUCT_ID_RCAR_H3)  || \
                                (_id == PRODUCT_ID_RCAR_M3W) || (_id == PRODUCT_ID_RCAR_M3N) || \
                                (_id == PRODUCT_ID_RCAR_V3M) || (_id == PRODUCT_ID_RCAR_V3H) || \
                                (_id == PRODUCT_ID_RCAR_D3)  || (_id == PRODUCT_ID_RCAR_E3))

#define IS_RCAR_GEN4(_id)      ((_id == PRODUCT_ID_RCAR_V3U) || (_id == PRODUCT_ID_RCAR_S4))

#define SYSC_RCAR_PRODUCT_REG   RCAR_PRODUCT_REGISTER

#define SYSC_PDMODE_MASK        3
#define SYSC_PDMODE_1           1
#define SYSC_PDMODE_3           3

typedef struct sysc_dev_ {
    uint32_t         pbase;
    uintptr_t        vbase;
    uint32_t         apmu_pbase;
    uintptr_t        apmu_vbase;
    uint32_t         size;
    int              verbose;
    uint32_t         soc_id;
    uint32_t         soc_rev;
	int              inited;
} sysc_dev_t;

extern sysc_dev_t sysc_dev;

#if defined VARIANT_a
/* Static library is used for Startup/IPL only
 * You must define your own nanospin and sysc_slogf functions */
extern void kprintf(const char *fmt, ... );
extern void usec_delay(uint32_t usec);

#define sysc_slogf(vlevel, fmt, args...)        \
            if( vlevel <= sysc_dev.verbose ) {  \
                kprintf(fmt"\n", ##args);       \
            }
#else
int sysc_slogf(const int vlevel, const char * const fmt, ...);
#endif

int   rcar_sysc_init(const sysc_opts_t* const sysc_opts);
void  rcar_sysc_deinit(void);
int   rcar_sysc_power_is_off(int pdr_number);
int   rcar_sysc_power_set(int pdr_number, int status);
int   rcar_sysc_pdmode_read(void);
int   rcar_apmu_power_set(int cpu, uint32_t flag, int status);

int   sysc_init_common(const sysc_opts_t* const sysc_opts, const uint32_t product_id);

int nanospin_ns( unsigned long nsec );

#endif /* _PROTO_H_INCLUDED */

