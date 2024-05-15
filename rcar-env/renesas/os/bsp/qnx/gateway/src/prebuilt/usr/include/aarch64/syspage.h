/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
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

/*
 *  aarch64/syspage.h
 */

#ifndef __AARCH64_SYSPAGE_H_INCLUDED
#define __AARCH64_SYSPAGE_H_INCLUDED

#ifndef __SYSPAGE_H_INCLUDED
#error aarch64/syspage.h should not be included directly.
#endif

/*
 * CPU capability/state flags.
 *
 * This contains definitions of the legacy ARMv7 flags so that AArch32
 * programs can continue to check for feature flags as expected.
 */
#define	AARCH32_CPU_FLAG_V6			0x0002u		/* ARMv6 ISA implemented */
#define	AARCH32_CPU_FLAG_SMP		0x0008u		/* multiprocessor system */
#define AARCH32_CPU_FLAG_V7_MP		0x0010u		/* ARMv7 multiprocessor extenstions */
#define AARCH32_CPU_FLAG_V7			0x0020u		/* ARMv7 ISA implemented */
#define AARCH32_CPU_FLAG_NEON		0x0040u		/* Neon Media Engine */
#define AARCH32_CPU_FLAG_CC_INCR_32	0x0200u		/* CLOCKCYCLES_INCR_BIT == 32 */
#define AARCH32_CPU_FLAG_IDIV		0x0400u		/* h/w divide: UDIV, SDIV supported */
#define	AARCH32_CPU_FLAG_VFP_D32	0x0800u		/* VFP has 32 double registers */

#define	AARCH64_CPU_FLAG_SMP		(AARCH32_CPU_FLAG_SMP)
#define	AARCH64_CPU_FLAG_SIMD		(AARCH32_CPU_FLAG_NEON)
#define	AARCH64_CPU_FLAG_CC_INCR_32	(AARCH32_CPU_FLAG_CC_INCR_32)
#define AARCH64_CPU_FLAG_S32G_TLB_ERRATA	0x80u	/* S32G va > 2TB need to use tlbi vmalle1is */
#define AARCH64_CPU_FLAG_IMX8QM_TLB_ERRATA	0x100u	/* applicable only if Board Rev < 2.0(B0) */
#define AARCH64_CPU_SPECTRE_V2_FIX  0x1000u
#define AARCH64_CPU_SPECTRE_V2_MMU  0x2000u
#define AARCH64_CPU_ACTLR		0x4000u
#define AARCH64_CPU_FLAG_LSE	0x8000u
#define AARCH64_CPU_FLAG_VHE	0x00010000u
#define AARCH64_CPU_PAUTH		0x20000u
#define AARCH64_CPU_FLAG_KRYO_TLB	0x40000u
#define AARCH64_CPU_SSBS		0x80000u

#define AARCH64_CPU_FLAG_AES	0x100000u
#define AARCH64_CPU_FLAG_PMULL	0x200000u
#define AARCH64_CPU_FLAG_SHA1	0x400000u
#define AARCH64_CPU_FLAG_SHA256	0x800000u
#define AARCH64_CPU_FLAG_SHA512	0x1000000u
#define AARCH64_CPU_FLAG_SHA3	0x2000000u
#define AARCH64_CPU_FLAG_RNDR	0x4000000u

/*
 * A cpu number indexed array of a value needed to send an IPI to that CPU.
 * It can be simply the CPU number according to the GIC (which can be
 * different from the CPU number according to procnto), or even a full
 * affinity value identifying the CPU, as needed by the GICv3 system
 * register interface.
 */
struct aarch64_gic_map_entry {
	_Uint32t	gic_cpu[1];			/* variable sized array */
};

/*
 * A cpu number indexed array of the index into the redistributor array
 * for the same CPU.  This is needed for the GICv3 PPI callouts as well
 * as for qvm to look at the right redistributor regardless of what
 * procnto think the order of the CPUs is.
 */
struct aarch64_gicr_map_entry {
	_Uint32t	gicr_idx[1];			/* variable sized array */
};

struct aarch64_syspage_entry {
	_Uint64t			L0_vaddr;
	syspage_entry_info	gic_map;
	syspage_entry_info	gicr_map;
	_Uint64t			rsvd[6];
};

struct aarch64_kernel_entry {
	_Uint32t	code[2];
};

struct aarch64_cpupage_entry {
	_Uint32t	gic_cpu;
};

#endif /* __AARCH64_SYSPAGE_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/mainline/services/system/public/aarch64/syspage.h $ $Rev: 941594 $")
#endif
