/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
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
#include <aarch64/r-car-gen3.h>
#include "rcar_startup.h"

#define NUM_CLUSTERS_V3U                4
#define NUM_CPUS_PER_CLUSTER_V3U        2

#define APMU_A3PWRCTRL_A3WUP_REQ        1
#define APMU_A3FSMSTSR_STATE_ON         0x04
#define APMU_FSMSTSRCL_STATE_ON         0x12

#define APMU_L3CTRLCL_L3STS_MSK         (7 << 16)
 #define APMU_L3CTRLCL_L3STS_ALL_ON     (4 << 16)
 #define APMU_L3CTRLCL_L3STS_34_ON      (3 << 16)
 #define APMU_L3CTRLCL_L3STS_24_ON      (2 << 16)
 #define APMU_L3CTRLCL_L3STS_14_ON      (1 << 16)
 #define APMU_L3CTRLCL_L3STS_OFF        (0 << 16)
#define APMU_L3CTRLCL_L3DENY            (1 << 9)
#define APMU_L3CTRLCL_L3ACCEPT          (1 << 8)
#define APMU_L3CTRLCL_L3REQ             (1 << 4)
#define APMU_L3CTRLCL_L3CTRL_MSK        (7 << 0)
 #define APMU_L3CTRLCL_L3CTRL_ALL_ON    (4 << 0)
 #define APMU_L3CTRLCL_L3CTRL_34_ON     (3 << 0)
 #define APMU_L3CTRLCL_L3CTRL_24_ON     (2 << 0)
 #define APMU_L3CTRLCL_L3CTRL_14_ON     (1 << 0)
 #define APMU_L3CTRLCL_L3CTRL_OFF       (0 << 0)

#define APMU_PWRCTRLCL_WUP_REQ          1

#define APMU_FSMSTSRCL_STATE_MSK        0xFF
 #define APMU_FSMSTSRCL_STATE_ON        0x12
 #define APMU_FSMSTSRCL_STATE_OFF       0x00
 #define APMU_FSMSTSRCL_STATE_FUNC_RET  0x21
 #define APMU_FSMSTSRCL_STATE_MEM_RET   0x31
#define APMU_A3PWRCTRL_A3_WUP_REQ       (1 << 0)
#define APMU_A3PWRCTRL_A3_PDN _EN       (1 << 4)
#define APMU_A3FSMSTSR_STATE_MSK        0xFF
 #define APMU_A3FSMSTSR_STATE_ON        0x04
 #define APMU_A3FSMSTSR_STATE_OFF       0x00

#define APMU_PWRCTRLC_WUP_REQ           1

#define APMU_FSMSTSRC_STATE_MSK         0xFF
 #define APMU_FSMSTSRC_STATE_ON         0x0D
 #define APMU_FSMSTSRC_STATE_OFF        0x00
 #define APMU_FSMSTSRC_STATE_OFF_EMU    0x1C

#define APMU_CLUSTER_OFFSET             0x200
#define APMU_CLUSTER_SIZE               0x40
#define APMU_CORE_IN_CLUSTER_SIZE       0x40


typedef struct cluster_s {
	uint32_t       id;       // CPU part number
	uint8_t        idx;      // CPUID in the cluster
	uint8_t        num_cpu;  // Number of available cores in the cluster
	uint32_t       cpu_freq; // The frequency of the cpus in the cluster
} cluster;

static cluster clusters[NUM_CLUSTERS_V3U];

/*
 * max_cpus: Maximum number of cpus in both clusters.
 * max_cpus_cluster0: Maximum number of cpus in the boot cluster (cluster 0)
 */
uint8_t max_cpus_cluster0 = ~0;

/*
 * psci calls are supported if the board boots first into the Arm Trusted firmware, which sets up the
 * PSCI lib. When booting without ATF, no support for PSCI is available.
 */
uint32_t psci_support = 1;

/*
 * Secondary cpus entry point is _start. the cpu then waits for the secondary_core_start to be set
 * before jumping to that adress to continue the execution.
 */
uintptr_t secondary_core_start = 0;

/*
 * R-CAR Gen3 CPUs
 * V3U:  8 A76 cores
 */
static unsigned num_cpus_in_cluster(uint32_t cluster_idx, uint32_t cluster_id)
{
	uint32_t prr   = in32(RCAR_GEN3_PRODUCT_REGISTER);
	uint32_t cpu_mask;
	uint8_t  num_cpu = 0;

	switch (cluster_id) {
		case MIDR_CPU_A76_ID:
			cpu_mask = RCAR_GEN3_PRODUCT_CA76EN(prr, cluster_idx);
			break;
		default:
			/* Unknown cluster. Disable all cpus */
			cpu_mask = 0x3;
			break;
	}

	for (int i = 0 ; i < NUM_CPUS_PER_CLUSTER_V3U; i++) {
		/* cpu_mask bits: 0 cpu available, 1 cpu not available */
		if ((cpu_mask & (1 << i)) == 0)
			num_cpu++;
	}

	return num_cpu;
}

unsigned board_smp_num_cpu(void)
{
	unsigned int i;
	unsigned num_cpu = 0;

	for(i = 0; i < NUM_CLUSTERS_V3U; i++) {
		clusters[i].idx      = i;
		clusters[i].id       = MIDR_CPU_ID(aarch64_midr());
		clusters[i].num_cpu  = min(num_cpus_in_cluster(clusters[i].idx, clusters[i].id), max_cpus_cluster0);
		clusters[i].cpu_freq = cpu_freq;

		num_cpu += clusters[i].num_cpu;
	}
	//kprintf("board_smp_num_cpu: %d cores, max allowed %d\n", num_cpu, max_cpus);

	return num_cpu;
}

static void power_on(uint32_t ctl_reg, uint32_t ctl_msk, uint32_t ctl_on, uint32_t sts_reg, uint32_t sts_msk, uint32_t sts_on)
{
	uint32_t tmp;

	tmp  =  in32(ctl_reg);
	tmp &= ~ctl_msk;
	tmp |=  ctl_on;
	out32(RCAR_V3U_APMU_DM0_BASE + RCAR_V3U_APMU_WPR, ~tmp);
	out32(ctl_reg, tmp);

	while ((in32(sts_reg) & sts_msk) != sts_on) { ; }
}

static void power_scu_on(uint32_t cluster_idx)
{
	uintptr_t cpg_apmu_clusterx;

	cpg_apmu_clusterx  = RCAR_V3U_APMU_DM0_BASE + RCAR_V3U_APMU_A76_CLUSTER0_GP;
	cpg_apmu_clusterx += APMU_CLUSTER_SIZE * cluster_idx;

	// Need power ON A3E1 domain for Cluster 2,3 ?
	if (!(cluster_idx % 2) && (cluster_idx > 1)) {
		power_on(RCAR_V3U_APMU_DM0_BASE + RCAR_V3U_APMU_ADMIN_GP + RCAR_V3U_APMU_A3PWRCTRL1,
				 APMU_A3PWRCTRL_A3_WUP_REQ,
				 APMU_A3PWRCTRL_A3_WUP_REQ,
				 RCAR_V3U_APMU_DM0_BASE + RCAR_V3U_APMU_ADMIN_GP + RCAR_V3U_APMU_A3FSMSTSR1,
				 APMU_A3FSMSTSR_STATE_MSK,
				 APMU_A3FSMSTSR_STATE_ON);
	}

	// Wakeup Cluster 1,2,3
	power_on(cpg_apmu_clusterx + RCAR_V3U_APMU_PWRCTRLCL,
			 APMU_PWRCTRLCL_WUP_REQ,
			 APMU_PWRCTRLCL_WUP_REQ,
			 cpg_apmu_clusterx  + RCAR_V3U_APMU_FSMSTSRCL,
			 APMU_FSMSTSRCL_STATE_MSK,
			 APMU_FSMSTSRCL_STATE_ON);

	// Power ClusterX L3 cache
	power_on(cpg_apmu_clusterx + RCAR_V3U_APMU_L3CTRLCL,
			 APMU_L3CTRLCL_L3CTRL_MSK,
			 APMU_L3CTRLCL_L3CTRL_ALL_ON,
			 cpg_apmu_clusterx  + RCAR_V3U_APMU_L3CTRLCL,
			 APMU_L3CTRLCL_L3STS_MSK,
			 APMU_L3CTRLCL_L3STS_ALL_ON);
}

static void power_cpu_on(uint8_t cpu_no, uint32_t cluster_idx)
{
	uintptr_t cpg_apmu_corex;

	cpg_apmu_corex  = RCAR_V3U_APMU_DM0_BASE + RCAR_V3U_APMU_A76_CORE0_GP;
	cpg_apmu_corex += APMU_CLUSTER_OFFSET * cluster_idx;
	cpg_apmu_corex += APMU_CORE_IN_CLUSTER_SIZE * (cpu_no % 2);

	if((cpu_no % 2) && (cluster_idx > 0))
		power_scu_on(cluster_idx); // Power on cluster 1,2,...

	// Wakeup CPU 1,2,..7
	power_on(cpg_apmu_corex + RCAR_V3U_APMU_PWRCTRLC,
			 APMU_PWRCTRLC_WUP_REQ,
			 APMU_PWRCTRLC_WUP_REQ,
			 cpg_apmu_corex + RCAR_V3U_APMU_FSMSTSRC,
			 APMU_FSMSTSRC_STATE_MSK,
			 APMU_FSMSTSRC_STATE_ON);
}

void board_smp_init(struct smp_entry *smp, unsigned num_cpus)
{
	smp->send_ipi = (void *)&sendipi_gic_v3_sr;
}

int board_smp_start(unsigned cpu, void (*start)(void))
{
	int32_t rval;
	uint8_t cluster = 0;
	unsigned int i;

	if (debug_flag) {
		kprintf("board_smp_start: cpu%d @ %v\n", cpu, start);
	}

	board_mmu_disable();

	for(i = 0; i < NUM_CLUSTERS_V3U; i++) {
		if(cpu >= clusters[i].num_cpu) {
			cpu = cpu - clusters[i].num_cpu;
			cluster++;
		} else {
			break;
		}
	}

	// Set the frequency of the target cpu
	cpu_freq = clusters[cluster].cpu_freq;

	if (psci_support) {
		// Set the cluster idx
		cpu <<= 8;
		cpu |= (clusters[cluster].idx << 16);

		rval = psci_cpu_on(cpu, (uint64_t)start, 0);

		if (rval != PSCI_SUCCESS) {
			// Secondary cpu start failed.
			crash("psci_cpu_on returned %x, cpu: %x\n", rval, cpu);
		}
	} else {
		// Turn on secondary cpu. Entry point is _start. the cpu waits for the secondary_core_start
		// to be set.
		power_cpu_on(cpu, clusters[cluster].idx);
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
