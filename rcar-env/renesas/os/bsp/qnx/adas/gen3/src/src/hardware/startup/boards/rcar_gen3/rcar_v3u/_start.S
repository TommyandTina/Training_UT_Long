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
 * _start.S
 *
 * This code runs as the first instructions after control has transferred
 * from any bootup monitor.  This module defines a minimal bootup stack,
 * and sets SP to this stack, and then starts running C code in _main().
 *
 * For Renesas R-Car Gen3, only the master boot processor cpu0 is started
 * from cold reset. Master boot processor (CA57/CA53/CR7) can be selected
 * with Mode pin MD[6:7].
 */

#include <aarch64/r-car-gen3.h>
#include <aarch64/gic_v3.h>

	.text
	.align 2

/*
 * EL3 needs to perform some initial GIC configuration
 */
	.extern cstart
	.extern secondary_core_start
	.extern psci_support
	.global _start

_start:
	/* NOTE:
	 *  Do NOT modify registers X0-X3 before jumping to the cstart label.
	 *  These registers may contain information provided by the IPL and
	 *  cstart will save them in the boot_regs variable for later perusal
	 *  by other portions of startup.
	 */

	/*
	 * Set up EL3 state if necessary
	 */
	mrs     x9, CurrentEL
	cmp     x9, #0xc                // EL3
	bne     0f
	bl      setup_el3

0:  b       cstart

/*
 * Core is in EL3, this happens when no secure code was running, e.g. no ARM trusted firmware
 * before jumping to the startup/ipl.
 */
setup_el3:
	/*
	 * Disable interrupts
	 */
	msr     DAIFset, #0x0F

	/*
	 * Wakeup GIC Re-Distributor
	 */
	mov     x4, #RCAR_V3U_GICR_LPI_BASE
	mov     x5, #RCAR_V3U_GICR_SGI_BASE

	mrs     x6, mpidr_el1
	lsr     x7, x6, #32
	bfi     x6, x7, #24, #8                 /* Arrange w6 as aff3:aff2:aff1:aff0 */

1:
	ldr     w7, [x4, #ARM_GICR_TYPER+4]     /* GICR_TYPE[63:32] is Affinity Value */
	cmp     w6, w7
	beq     2f
	add     x4, x4, #ARM_GICR_SIZE_PER_CORE
	add     x5, x5, #ARM_GICR_SIZE_PER_CORE
	b       1b

	/* Have to wake up the CPU interface */
2:
	ldr     w6, [x4, #ARM_GICR_WAKER]
	bic     w6, w6, #GICR_WAKER_PROCESSORSLEEP
	str     w6, [x4, #ARM_GICR_WAKER]
	dsb     sy

	/* Wait for ChildrenAsleep bit to clear */
3:
	yield
	ldr     w6, [x4, #ARM_GICR_WAKER]
	tst     w6, #GICR_WAKER_CHILDRENASLEEP
	bne     3b

	/* SGIs and PPIs are in Group1NS */
	mov     w6, #0xffffffff
	str     w6, [x5, #ARM_GICR_IGROUPR0]

	/* ICC_SRE_EL3: SRE & Disable IRQ/FIQ Bypass, Allow access to  ICC_SRE_EL1 and ICC_SRE_EL2 */
	mrs     x4, S3_6_C12_C12_5
	orr     x4, x4, #0xf
	msr     S3_6_C12_C12_5, x4
	isb

	/*
	 * SCTLR_EL1
	 */
	mrs     x4, SCTLR_EL1
	bic     x4, x4, #(1 << 3)   /* SA:  disable stack alignment check       */
	bic     x4, x4, #(1 << 4)   /* SA0: disable EL0 stack alignment check   */
	bic     x4, x4, #(1 << 5)   /* CP15BEN: disable CP15 barrier operations */
	bic     x4, x4, #(1 << 16)  /* nTWI: trap EL0 WFI that would suspend    */
	bic     x4, x4, #(1 << 18)  /* nTWE: trap EL0 WFE that would suspend    */
	msr     SCTLR_EL1, x4

	/*
	 * Disable MMU and Data cache, Enable I cache
	 */
	mrs     x4, SCTLR_EL3
	bic     x4, x4, #(1 << 0)    /* Clear bit M(0) - EL3 stage 1 address translation disabled. */
	orr     x4, x4, #(1 << 1)    /* Set bit A(1)   - Alignment fault checking enabled. */
	bic     x4, x4, #(1 << 2)    /* Disable D Cache */
	orr     x4, x4, #(1 << 12)   /* Enable I Cache */
	msr     SCTLR_EL3, x4
	dsb     sy
	isb
	ic      ialluis
	isb     sy
	mov     x4, x4
	mov     x4, x4
	mov     x4, x4

	/*
	 * Disable traps to EL3 for accesses to the CPACR_EL1 reg
	 * and regs associated with floating-point and SIMD execution
	 */
	msr     cptr_el3, xzr

	/*
	 * Disable EL1/EL0 traps for FP and SIMD reg accesses
	 */
	mrs     x4, cpacr_el1
	orr     x4, x4, #(1 << 20)
	orr     x4, x4, #(1 << 21)
	msr     cpacr_el1, x4
	isb

	/*
	 *  Check the core id
	 */
	mrs     x4, mpidr_el1
	and     x4, x4, #0xffff00
	cbnz    x4, secondary_core

	/*
	 * Set default generic timer frequency, if not set already
	 */
	mrs     x4, cntfrq_el0
	cbnz    x4, 1f
	ldr     w4, cntfrq_default
	msr     cntfrq_el0, x4

	/*
	 *  Master core
	 *  Set the entry point of the secondary cores to _start
	 */
1:  mov     x4, #RCAR_V3U_APMU_DM0_BASE
	add     x4, x4, #RCAR_V3U_APMU_RVBARLC
	adr     x5, _start
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE1_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE2_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE3_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE4_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE5_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE6_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE7_GP]

	mov     x4, #RCAR_V3U_APMU_DM0_BASE
	add     x4, x4, #RCAR_V3U_APMU_RVBARHC
	ror     x5, x5, #32
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE1_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE2_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE3_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE4_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE5_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE6_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE7_GP]

	mov     x4, #RCAR_V3U_APMU_DM0_BASE
	add     x4, x4, #RCAR_V3U_APMU_RVBARPLC
	adr     x5, _start
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE1_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE2_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE3_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE4_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE5_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE6_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE7_GP]

	mov     x4, #RCAR_V3U_APMU_DM0_BASE
	add     x4, x4, #RCAR_V3U_APMU_RVBARPHC
	ror     x5, x5, #32
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE1_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE2_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE3_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE4_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE5_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE6_GP]
	str     w5, [x4, #RCAR_V3U_APMU_A76_CORE7_GP]

	/*
	 *  WDT reset enable
	 */
	 mov     x4, #RCAR_V3U_RST_DM0_BASE
	 mov     w5, #0xA55A0000          /* Code value (H’A55A) and RWDT & SWDT Reset Enable */
	 orr     w5, w5, #(1 << 15)       /* ICUMXBAR and ICUMXCPCR are initialized by PRESET#, WDT reset, and Soft Power On Reset (SRESCSR.PRES=1) */
	 str     w5, [x4, #RCAR_V3U_RST_WDTRSTCR]

	/*
	 *  PSCI not supported
	 */
	ldr     x4, =psci_support
	mov     w5, #0
	str     w5, [x4]

	ret

secondary_core:
	ldr     x4, =secondary_core_start
0:  wfe
	ldr     x5, [x4]
	cbz     x5, 0b
	br      x5


cntfrq_default: .word   100000000   // bogus default cntfrq_el0 value

