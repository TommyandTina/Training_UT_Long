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
#include <aarch64/gic_v2.h>

/*******************************************************************************
 * L2 Control register specific definitions.
 ******************************************************************************/
#define L2CTLR_EL1          S3_1_C11_C0_2   /* Instruction def. */

#define CPUECTLR_EL1            S3_1_C15_C2_1
#define CPUECTLR_SMP_BIT        (1 << 6)

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
    msr        DAIFset, #0x0F

    /*
     * Set the GIC Priority Mask to enable forwarding of interrupts to cpu interface.
     * Configure all interrupts as insecure Group1 interrupts.
     */
    mov     x4, #RCAR_GEN3_GIC_CPU_BASE
    mov     w5, #0xff
    str     w5, [x4, #ARM_GICC_PMR]

    mov     x4, #RCAR_GEN3_GIC_DIST_BASE
    /* determine how many SPI IGROUPRn regs we have */
    ldr     w6, [x4, #ARM_GICD_TYPER]
    and     x6, x6, #ARM_GICD_TYPER_ITLN  /* mask off # of SPI intr regs */
    /* set all IGROUPRn bits starting with last SPI reg and down to and including SGI+PPI reg */
    add     x4, x4, #ARM_GICD_IGROUPRn
    mov     w5, #0xffffffff
1:  str     w5, [x4, x6, LSL #2]
    subs    x6, x6, #1
    bge     1b

    /*
     * SCTLR_EL1
     */
    mrs     x4, SCTLR_EL1
    bic     x4, x4, #(1 << 3)	/* SA:  disable stack alignment check       */
    bic     x4, x4, #(1 << 4)	/* SA0: disable EL0 stack alignment check   */
    bic     x4, x4, #(1 << 5)	/* CP15BEN: disable CP15 barrier operations */
    bic     x4, x4, #(1 << 16)	/* nTWI: trap EL0 WFI that would suspend    */
    bic     x4, x4, #(1 << 18)	/* nTWE: trap EL0 WFE that would suspend    */
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
     * Enable data coherency with other cores in the cluster
     *
     */
    mrs     x4, CPUECTLR_EL1
    orr     x4, x4, #CPUECTLR_SMP_BIT
    msr     CPUECTLR_EL1, x4
    isb

    /*
     * Disable EL1/EL0 traps for FP and SIMD reg accesses
     */
    mrs     x4, cpacr_el1
    orr     x4, x4, #(1 << 20)
    orr     x4, x4, #(1 << 21)
    msr     cpacr_el1, x4

    /*
     * Set default generic timer frequency, if not set already
     */
    mrs     x4, cntfrq_el0
    cbnz    x4, 1f
    ldr     w4, cntfrq_default
    msr     cntfrq_el0, x4

    /*
     *  Check the core id
     */
1:  mrs     x4, MPIDR_EL1
    and     x4, x4, #0xffff
    cbnz    x4, secondary_core

    /*
     *  Master core
     *  Set the entry point of the secondary cores to _start
     */
    mov     x4, #RCAR_GEN3_RESET_BASE
    adr     x5, _start
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARL(0)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARL(1)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARL(2)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARL(3)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARL(0)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARL(1)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARL(2)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARL(3)]
    ror     x5, x5, #32
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARH(0)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARH(1)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARH(2)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA53CPUxBARH(3)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARH(0)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARH(1)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARH(2)]
    str     w5, [x4, #RCAR_GEN3_RESET_CA57CPUxBARH(3)]

    /* --------------------------------------------------------------------
     * Cortex-A57 specific settings
     * --------------------------------------------------------------------
     */

    /*
     * Setup L2 Cache config for H3 and M3
     * See section 5.4.1 in v1.50 of the R-Car Gen 3 Ref Manual
     */
    mrs     x5, L2CTLR_EL1
    mov     x6, #0x1e7          /* L2 RAM bits to modify      */
    bic     x5, x5, x6

    /* Check what product we booted with: ((RCAR_GEN3_PRODUCT_REGISTER & 0x7F00) >> 8) */
    ldr     x4, =RCAR_GEN3_PRODUCT_REGISTER
    ldr     w4, [x4]
    ubfx    w4, w4, #8, #7
    cmp     w4, #PRODUCT_ID_RCAR_H3
    beq     h3_l2
    cmp     w4, #PRODUCT_ID_RCAR_M3W
    beq     m3_l2
    cmp     w4, #PRODUCT_ID_RCAR_M3N
    bne     2f /* not H3/M3W/M3N, nothing to do */

m3_l2:
    orr     x5, x5, #0x2 << 6   /* M3 Tag Setting  (3 cycles)  */
                                /* M3 Data Setup is 0         */
    orr     x5, x5, #0x3        /* M3 Data Setting (4 cycles) */
    b       1f

h3_l2:
    orr     x5, x5, #0x2 << 6   /* H3 Tag Setting  (3 cycles)  */
    orr     x5, x5, #0x1 << 5   /* H3 Data Setup is 1         */
    orr     x5, x5, #0x3        /* H3 Data Setting (4 cycles) */
1:
    msr     L2CTLR_EL1, x5
2:

    /*
     *  PSCI not supported
     */
    ldr	    x4, =psci_support
    mov     w5, #0
    str     w5, [x4]

    ret


secondary_core:
    ldr	    x4, =secondary_core_start
0:  wfe
    ldr     x5, [x4]
    cbz     x5, 0b
    br      x5


cntfrq_default: .word   100000000   // bogus default cntfrq_el0 value
