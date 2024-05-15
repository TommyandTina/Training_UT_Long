/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#include <aarch64/r-car.h>
#include <aarch64/gic_v3.h>

	.text
	.align	2

	.globl	_start
	/*
	 * Could be EL3/EL2/EL1, Initial State:
	 * Little Endian, MMU Disabled, i/dCache Disabled
	 */
_start:
    mrs		x1, CurrentEL

	cmp		x1, #0xc
	b.eq	3f
	cmp		x1, #0x8
	b.eq	2f
	cmp		x1, #0x4
	b.eq	1f

3:  mrs     x1, SCR_EL3
    mov     x2, #((1<<10)|(1<<8)|(1<<0))
    orr     x1, x1, x2              /* SCR_EL3.RW, HCE, NS set */
    bic     x1, x1, #0x0E           /* IRQ & FIQ & EA set to 0 */
    msr     SCR_EL3, x1             /* Set EL2/EL1/EL0 to AArch64 */
    msr     CPTR_EL3, xzr           /* Enable FP/SIMD */

	mov		x0, #(RCAR_GEN4_EXT_CLK_20MHZ & 0xFFFF)
	movk	x0, #(RCAR_GEN4_EXT_CLK_20MHZ >> 16), lsl #16
	msr		cntfrq_el0, x0	/* Initialize CNTFRQ */
    b		0f

2:	mrs		x0, sctlr_el2
	orr		x0, x0, #(1 << 12)	/* Enable I-Cache */
	msr		sctlr_el2, x0
    mov		x0, #0x33ff
	msr		cptr_el2, x0	    /* Enable FP/SIMD */
	b		0f

1:	mrs		x0, sctlr_el1
	orr		x0, x0, #(1 << 12)	/* Enable I-Cache */
	msr		sctlr_el1, x0
	mov		x0, #(3 << 20)
	msr		cpacr_el1, x0	    /* Enable FP/SIMD */

0:
    bl      _gic3_init

    dsb     sy
    isb
    ic      ialluis
    isb     sy
    mov     x0, x0
    mov     x0, x0
    mov     x0, x0

	adr		x0, _stack
	mov		sp, x0

	bl		main

oops:
	b		oops

_gic3_init:
    mov	x29, lr			/* Save LR */

    msr     DAIFset, #0x0F          /* Disable interrupts etc. */

    /* 
     * Wakeup GIC Re-Distributor
     */
    ldr	    x4, =RCAR_GEN4_GICR_LPI_BASE
    mov     x5, #2
    str     w5, [x4, #0x0024]
    ldr     w5, [x4, #ARM_GICR_WAKER]
    bic     w5, w5, #GICR_WAKER_PROCESSORSLEEP
    str     w5, [x4, #ARM_GICR_WAKER]
	/* Wait till the GICR_WAKER.ChildrenAsleep changes to 0 */    
0:  ldr     w5, [x4, #ARM_GICR_WAKER]
    and     w5, w5, #GICR_WAKER_CHILDRENASLEEP
    cbnz    w5, 0b

    /* 
     * SGIs and PPIs are in Group1NS 
     */
    mov     x4, #RCAR_GEN4_GICR_SGI_BASE
    mov     w5, #0xffffffff
    str     w5, [x4, #ARM_GICR_IGROUPR0]

	/*
	 * Initialize Distributor
	 */
    ldr	    x0, =RCAR_GEN4_GICD_BASE
	mov	    w9, #0x37		            /* EnableGrp0 | EnableGrp1NS */
                                        /* EnableGrp1S | ARE_S | ARE_NS */
	str	    w9, [x0, ARM_GICD_CTLR]	    /* Secure GICD_CTLR */
	ldr	    w9, [x0, ARM_GICD_TYPER]
	and	    w10, w9, #0x1f	        /* ITLinesNumber */
	cbz	    w10, 0f			        /* No SPIs */
	add	    x11, x0, (ARM_GICD_IGROUPRn + 4)
	add	    x12, x0, (ARM_GICD_IGRPMODRn + 4)
	mov	    w9, #~0
1:	str	    w9, [x11], #0x4
	str	    wzr, [x12], #0x4	    /* Config SPIs as Group1NS */
	sub	    w10, w10, #0x1
	cbnz	w10, 1b
0:

    mov	lr, x29			/* Restore LR */

    ret

	.align	4
	.space	1024
_stack:
