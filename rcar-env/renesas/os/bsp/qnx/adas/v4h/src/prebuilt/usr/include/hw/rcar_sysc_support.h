/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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


#ifndef _HW_SYSC_SUPPORT_H_INCLUDED
#define _HW_SYSC_SUPPORT_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include <hw/rcar_sysc.h>

typedef struct sysc_opts_ {
    uint32_t     base;
    uint32_t     size;
    uint32_t     apmu_base;
    int          verbose;
} sysc_opts_t;

/*
 * Hardware Access Function Table
 */
typedef struct {
    int  (*init)(const sysc_opts_t* const sysc_opts);
    void (*deinit)(void);
    int  (*power_set)(int pdr_number, int status);
    int  (*power_is_off)(int pdr_number);
    int  (*read_pdmode)(void);
#define APMU_POWER_CPU      1
#define APMU_POWER_CLUSTER  2
#define APMU_POWER_L3CACHE  3
#define APMU_POWER_A3DOMAIN 4
    int  (*apmu_power_set)(int pdr_number, uint32_t flags, int status);
} sysc_support_funcs_t;

extern sysc_support_funcs_t sysc_support_funcs;

#endif /* _HW_SYSC_SUPPORT_H_INCLUDED */
