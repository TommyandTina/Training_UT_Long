/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

#include "startup.h"
#include "aarch64/psci.h"
#include <hw/inout.h>
#include "rcar_startup.h"
#include "rcar_gen4.h"

typedef struct rcar_smp_s {
    int       midr_id;
    int       cluster_id;
    int       core_id;
    uint32_t  cpu_freq; // The frequency of the cpus in the cluster
} rcar_smp_t;

/*
 * Secondary cpus entry point is _start. the cpu then waits for the secondary_core_start to be set
 * before jumping to that adress to continue the execution.
 */
uintptr_t secondary_core_start = 0;

/*
 * psci calls are supported if the board boots first into the Arm Trusted firmware, which sets up the
 * PSCI lib. When booting without ATF, no support for PSCI is available.
 */
uint32_t psci_support = 1;

static rcar_smp_t rcar_cpu_id[RCAR_GEN4_CORES_NUM_MAX];
uint8_t max_cpus_cluster0 = ~0;

static void rcar_cpu_power_on(int cpu, uintptr_t start)
{
    uint32_t flags = APMU_POWER_CPU;
    uintptr_t cpg_apmu_corex;

    /* Apply limitation for V4H 1.0 APMU access protection error:
     * Always allow any access to Group0-3 of CR52, CA76 cluster0-1 respectively.
     * (Set any bit of D[n]ACCENR(n=0-3)/RTGRPx, CL0GRPx, CL1GRPx(x=0-3) to 1)
     */
    if ((RCAR_PRODUCT_ID(rcar_product_id) == PRODUCT_ID_RCAR_V4H) &&
        (RCAR_PRODUCT_REV(rcar_product_id) == PRODUCT_REV_1_0)) {
        out32(RCAR_GEN4_APMU_DM0_BASE + RCAR_GEN4_APMU_D0ACCENR, 0xFF);
    }
    cpg_apmu_corex  = RCAR_GEN4_APMU_DM0_BASE + RCAR_GEN4_APMU_AP_CORE0_GP;
    cpg_apmu_corex += RCAR_GEN4_APMU_CLUSTER_OFFSET * rcar_cpu_id[cpu].cluster_id;
    cpg_apmu_corex += RCAR_GEN4_APMU_CORE_IN_CLUSTER_SIZE * (cpu % 2);

    // Set the entry point of the secondary cores to _start
	out32(cpg_apmu_corex + RCAR_GEN4_APMU_RVBARLC, start);
    out32(cpg_apmu_corex + RCAR_GEN4_APMU_RVBARHC, start >> 32);

    /* Power ON */
    sysc_support_funcs.apmu_power_set(cpu, APMU_POWER_CPU, flags);
}

unsigned board_smp_num_cpu(void)
{
    uint32_t cpu_mask;
    int num_cpu = 0;
    int cluster, core;

    for(cluster = 0; cluster < RCAR_GEN4_CLUSTERS_NUM_MAX; cluster++) {
        cpu_mask = RCAR_GEN4_PRODUCT_CAEN(rcar_product_id, cluster);
        for(core = 0; core < RCAR_GEN4_MAX_CORE_PER_CLUSTER; core++) {
            if((cpu_mask & (1 << core)) == 0) {
                rcar_cpu_id[num_cpu].midr_id = MIDR_CPU_ID(aarch64_midr());
                rcar_cpu_id[num_cpu].cluster_id = cluster;
                rcar_cpu_id[num_cpu].core_id = core;
                rcar_cpu_id[num_cpu].cpu_freq = cpu_freq_get(rcar_cpu_id[num_cpu].midr_id);
                num_cpu++;
            }
        }
    }

    return num_cpu;
}

void board_smp_init(struct smp_entry *smp, unsigned num_cpus)
{
    (void) num_cpus;
    if(((aa64_sr_rd64(id_aa64pfr0_el1) & (0xful << 24)) == 0) ||
       (inter_core_intr_errata() == true))  {
        smp->send_ipi = (void *)&sendipi_gic_v3_mm;
    } else {
        smp->send_ipi = (void *)&sendipi_gic_v3_sr;
    }
}

int board_smp_start(unsigned cpu, void (*start)(void))
{
    int rval;

    if (debug_flag) {
        kprintf("board_smp_start: cpu%d @ %v\n", cpu, start);
    }

    board_mmu_disable();

    // Set the frequency of the target cpu
    cpu_freq = rcar_cpu_id[cpu].cpu_freq;

    if(psci_support) {
        cpu = rcar_cpu_id[cpu].core_id << 8 | rcar_cpu_id[cpu].cluster_id << 16;
        rval = psci_cpu_on(cpu, (uint64_t)start, 0);
        if(rval != PSCI_SUCCESS) {
            crash("psci_cpu_on returned %x, cpu: %x\n", rval, cpu);
        }
    } else {
        rcar_cpu_power_on(cpu, (uintptr_t)start);
        secondary_core_start = (uintptr_t)start;

        // Get the secondary cpu out of the wfe
        __asm__ __volatile__(
            "dsb sy\n"
            "sev\n"
            : : : "memory"
        );
    }

    return 1;
}

unsigned board_smp_adjust_num(unsigned cpu)
{
    return cpu;
}
