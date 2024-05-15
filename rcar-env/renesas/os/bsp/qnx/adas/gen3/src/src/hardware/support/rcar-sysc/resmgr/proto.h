/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
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


#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdint.h>
#include <sys/slog.h>

int variant_sysc_pbase_get(uint32_t *pbase, uint32_t* size);
int variant_sysc_domain_power_is_off(uintptr_t sysc_vbase, int pdr_number);
int variant_sysc_domain_power_off(uintptr_t sysc_vbase, int pdr_number);
int variant_sysc_domain_power_on(uintptr_t sysc_vbase, int pdr_number);

int sysc_slogf(int level, const char *fmt, ...);

#endif /* #ifndef __PROTO_H__ */

