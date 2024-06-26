#
# Copyright 2016, QNX Software Systems.
# Copyright 2021, Renesas Electronics Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License"). You
# may not reproduce, modify or distribute this software except in
# compliance with the License. You may obtain a copy of the License
# at: http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTIES OF ANY KIND, either express or implied.
#
# This file may contain contributions from others, either as
# contributors under the License or as licensors under other terms.
# Please review this entire file for other proprietary rights or license
# notices, as well as the QNX Development Suite License Guide at
# http://licensing.qnx.com/license-guide/ for other information.
#

/*
 * polled serial I/O
 */

#include "callout.ah"
#include <arm/scif.h>

/*
 * Patch interrupt callouts to access rw data.
 * The first call will also map the uart.
 *
 * Patcher routine takes the following arguments:
 *	x0 - syspage paddr
 *	x1 - vaddr of callout
 *	x2 - offset from start of syspage to start of callout routine
 *	x3 - offset from start of syspage to rw storage
 *	x4 - patch data
 *	x5 - callout_rtn
 */

patch_debug:
	sub		sp, sp, #16
	stp		x19, x30, [sp]

	add		x19, x0, x2				// x19 = address of callout routine

	/*
	 * Map UART using patch_data parameter
	 */
	mov		x0, #0x1000
	ldr		x1, [x4]
	bl		callout_io_map

	/*
	 * Patch callout with mapped virtual address in x0
	 */
	CALLOUT_PATCH	x19, w6, w7

	ldp		x19, x30, [sp]
	add		sp, sp, #16
	ret

/*
 * -----------------------------------------------------------------------
 * void	display_char_scif(struct sypage_entry *, char)
 *
 * x0: syspage pointer
 * x1: character
 * -----------------------------------------------------------------------
 */
CALLOUT_START(display_char_scif, 0, patch_debug)
	mov		x7, #0xabcd				// UART base address (patched)
	movk	x7, #0xabcd, lsl #16
	movk	x7, #0xabcd, lsl #32
	movk	x7, #0xabcd, lsl #48

0:	ldr		w2, [x7, #SCIF_SCFSR_OFF]
	tst		w2, #SCIF_SCSSR_TDFE
	b.eq	0b

	and		w0, w1, #0xff
	strb	w0, [x7, #SCIF_SCFTDR_OFF]

1:	ldr		w2, [x7, #SCIF_SCFSR_OFF]
	tst		w2, #SCIF_SCSSR_TEND
	b.eq	1b

	mov		w2, #0
	strh	w2, [x7, #SCIF_SCFSR_OFF]

	ret
CALLOUT_END(display_char_scif)


/*
 * -----------------------------------------------------------------------
 * char	poll_key_scif(struct sypage_entry *)
 *
 * x0: syspage pointer
 * -----------------------------------------------------------------------
 */
CALLOUT_START(poll_key_scif, 0, patch_debug)
	mov		x7, #0xabcd				// UART base address (patched)
	movk	x7, #0xabcd, lsl #16
	movk	x7, #0xabcd, lsl #32
	movk	x7, #0xabcd, lsl #48

	ldr		w2, [x7, #SCIF_SCFDR_OFF]
	tst		w2, #0x1f
	b.eq	0f
	ldrb	w0, [x7, #SCIF_SCFRDR_OFF]
	ret
0:	mov		w0, #-1
	ret
CALLOUT_END(poll_key_scif)


/*
 * -----------------------------------------------------------------------
 * int break_detect(struct syspage_entry *)
 *
 * x0: syspage pointer
 *
 * Return 1 if break detected, 0 otherwise
 * -----------------------------------------------------------------------
 */
CALLOUT_START(break_detect_scif, 0, patch_debug)
	mov		x7, #0xabcd				// UART base address (patched)
	movk	x7, #0xabcd, lsl #16
	movk	x7, #0xabcd, lsl #32
	movk	x7, #0xabcd, lsl #48

	ldr		w2, [x7, #SCIF_SCFSR_OFF]
	tst		w2, #SCIF_SCSSR_BRK
	b.eq	0f
	bic		w2, w2, #SCIF_SCSSR_BRK
	str		w2, [x7, #SCIF_SCFSR_OFF]
	mov		w0, #1
	ret
0:
	mov		w0, #0
	ret
CALLOUT_END(break_detect_scif)

