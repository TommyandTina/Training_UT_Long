/*
 * Copyright (C) 2019-2020 Renesas Electronics Europe Ltd. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

.extern __stack
.extern __irq_stack
.extern __supervisor_stack
.extern __abort_stack
.extern __fiq_stack
.extern __undef_stack
.extern _vector_table
.extern SystemInit

.global _reset

.section .boot

#define MODE_MASK	0x1F		/* Bit mask for mode bits in CPSR */
#define ARM_MODE_USR	0x10
#define ARM_MODE_FIQ	0x11
#define ARM_MODE_IRQ	0x12
#define ARM_MODE_SVC	0x13
#define ARM_MODE_MON	0x16
#define ARM_MODE_ABT	0x17
#define ARM_MODE_UND	0x1B
#define ARM_MODE_SYS	0x1F

_reset:
	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_SYS	/* Set mode bits */
	msr	cpsr_c, r0		/* Change the mode */
	ldr	sp, =__stack
	mov 	r14, #0

	bl 	SystemInit		/* Initialize caches, MPU, etc */

	bl	_start

.Ldone:	b	.Ldone			/* Should never get here */


/* Called by newlib crt0 _start after it has set up the stacks */
software_init_hook:
	/* Initialise stack pointers, registers are banked so we have to read PSR each time */

	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_IRQ	/* Set mode bits */
	msr	cpsr_c, r0		/* Change the mode */
	ldr	sp, =__irq_stack
	mov 	r14, #0

	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_SVC	/* Set mode bits */
	msr	cpsr_c, r0		/* Change the mode */
	ldr	sp, =__supervisor_stack
	mov 	r14, #0

	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_ABT	/* Set mode bits */
	msr	cpsr_c, r0		/* Change the mode */
	ldr	sp, =__abort_stack
	mov 	r14, #0

	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_FIQ	/* Set mode bits */
	msr	cpsr_c, r0		/* Change the mode */
	ldr	sp, =__fiq_stack
	mov 	r14, #0

	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_UND	/* Set mode bits */
	msr	cpsr_c, r0		/* Change the mode */
	ldr	sp, =__undef_stack
	mov 	r14, #0

	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_SYS	/* Set mode bits */
	msr	cpsr_c, r0		/* Change the mode */
	ldr	sp, =__stack
	mov 	r14, #0

	mov		pc, lr

.end
