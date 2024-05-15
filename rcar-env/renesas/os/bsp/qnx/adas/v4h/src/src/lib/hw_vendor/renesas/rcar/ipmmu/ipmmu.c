/*
 * Copyright (c) 2023, BlackBerry Limited. All rights reserved.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

/**
 * @file
 * library to interface with smmuman
 */

#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/iomsg.h>
#include <sys/iomgr.h>
#include <sys/neutrino.h>
#include <signal.h>
#include <pthread.h>
#include <smmu.h>

#include <hw/rcar_ipmmu-api.h>
#include "iova.h"

#define MAX_INSTANCE          64
#define NUM_ELTS(__ary)       ((unsigned)(sizeof(__ary)/sizeof(*(__ary))))

#define IOVA_START_ADDRESS    (1024*1024*1024U)
#define IOVA_SIZE             (3*1024*1024*1024UL)
#define IOVA_ALIGNMENT        (4096U)

struct instance {
    int                 handle;
    int                 index;
    struct smmu_object  *sop;
    struct iova_context *iova_ctx;
};

static uint32_t instance_num;
static struct instance *instances[MAX_INSTANCE];

struct name_reg_map {
    const char  *const name;
    const int       channel;
    const uint32_t  reg;
    const uint32_t  size;
};

static const struct name_reg_map name_map[] = {
    { .name = "sdhci",           .channel = 0,  .reg = 0xee140000, .size = 0x10000 },  /* MMC */
    { .name = "fcpvd",           .channel = 0,  .reg = 0xfea20000, .size = 0x8000 },   /* VSPD */
    { .name = "fcpvd",           .channel = 1,  .reg = 0xfea28000, .size = 0x8000 },   /* VSPD */
    { .name = "ethernet-avb",    .channel = 0,  .reg = 0xe6800000, .size = 0x10000 },  /* ETH */
    { .name = "ethernet-avb",    .channel = 1,  .reg = 0xe6810000, .size = 0x10000 },  /* ETH */
    { .name = "ethernet-avb",    .channel = 2,  .reg = 0xe6820000, .size = 0x10000 },  /* ETH */
    { .name = "pcle",            .channel = 0,  .reg = 0xe65d0000, .size = 0x8000 },   /* PCLE */
    { .name = "pcle",            .channel = 1,  .reg = 0xe65d8000, .size = 0x8000 },   /* PCLE */
    { .name = "ims",             .channel = 0,  .reg = 0xfe860000, .size = 0x10000 },  /* IMR */
    { .name = "ims",             .channel = 1,  .reg = 0xfe870000, .size = 0x10000 },  /* IMR */
    { .name = "imr",             .channel = 0,  .reg = 0xfe880000, .size = 0x10000 },  /* IMR */
    { .name = "imr",             .channel = 1,  .reg = 0xfe890000, .size = 0x10000 },  /* IMR */
    { .name = "imr",             .channel = 2,  .reg = 0xfe8a0000, .size = 0x10000 },  /* IMR */
    { .name = "imp_top",         .channel = 0,  .reg = 0xff900000, .size = 0x6000 },   /* IMP */
    { .name = "imp",             .channel = 0,  .reg = 0xffa00000, .size = 0x20000 },  /* IMP */
    { .name = "imp",             .channel = 1,  .reg = 0xffa20000, .size = 0x20000 },  /* IMP */
    { .name = "imp",             .channel = 2,  .reg = 0xffb00000, .size = 0x20000 },  /* IMP */
    { .name = "imp",             .channel = 3,  .reg = 0xffb20000, .size = 0x20000 },  /* IMP */
    { .name = "imp_ocv",         .channel = 0,  .reg = 0xffa40000, .size = 0x10000 },  /* IMP */
    { .name = "imp_ocv",         .channel = 1,  .reg = 0xffa50000, .size = 0x10000 },  /* IMP */
    { .name = "imp_ocv",         .channel = 2,  .reg = 0xffa60000, .size = 0x10000 },  /* IMP */
    { .name = "imp_ocv",         .channel = 3,  .reg = 0xffa70000, .size = 0x10000 },  /* IMP */
    { .name = "imp_dma",         .channel = 0,  .reg = 0xffa80000, .size = 0x4000 },   /* IMP */
    { .name = "imp_dma",         .channel = 1,  .reg = 0xffb80000, .size = 0x4000 },   /* IMP */
    { .name = "imp_psc",         .channel = 0,  .reg = 0xffa84000, .size = 0x4000 },   /* IMP */
    { .name = "imp_dta",         .channel = 0,  .reg = 0xffa88000, .size = 0x4000 },   /* IMP */
    { .name = "imp_dma_slim",    .channel = 0,  .reg = 0xffa90000, .size = 0x4000 },   /* IMP */
    { .name = "imp_dma_slim",    .channel = 1,  .reg = 0xffa94000, .size = 0x4000 },   /* IMP */
    { .name = "imp_cnn",         .channel = 0,  .reg = 0xffaa0000, .size = 0x10000 },  /* IMP */
    { .name = "imp_reg_spmi",    .channel = 0,  .reg = 0xffa8c000, .size = 0x4000 },   /* IMP */
    { .name = "imp_reg_spmc",    .channel = 0,  .reg = 0xffab0000, .size = 0x4000 },   /* IMP */
    { .name = "imp_spmi",        .channel = 0,  .reg = 0xed300000, .size = 0x100000 }, /* IMP */
    { .name = "imp_spmc",        .channel = 0,  .reg = 0xed400000, .size = 0x200000 }, /* IMP */
    { .name = "imp_spmc",        .channel = 1,  .reg = 0xed600000, .size = 0x80000 },  /* IMP */
    { .name = "vdsp00",          .channel = 0,  .reg = 0xf1400000, .size = 0x1a0000 }, /* VDSP/DSPD/DSPP */
    { .name = "vdsp01",          .channel = 1,  .reg = 0xf1600000, .size = 0x1a0000 }, /* VDSP/DSPD/DSPP */
    { .name = "vdsp02",          .channel = 2,  .reg = 0xf1800000, .size = 0x1a0000 }, /* VDSP/DSPD/DSPP */
    { .name = "vdsp03",          .channel = 3,  .reg = 0xf1a00000, .size = 0x1a0000 }, /* VDSP/DSPD/DSPP */
    { .name = "dof",             .channel = 0,  .reg = 0xe7a10000, .size = 0x10000 },  /* VISIONIP */
    { .name = "smd_ps",          .channel = 0,  .reg = 0xe7a20000, .size = 0x10000 },  /* VISIONIP */
    { .name = "smd_post",        .channel = 0,  .reg = 0xe7a40000, .size = 0x10000 },  /* VISIONIP */
    { .name = "dof_fcprm",       .channel = 0,  .reg = 0xe7ab1000, .size = 0x1000 },   /* VISIONIP */
    { .name = "vip_wrap",        .channel = 0,  .reg = 0xe7b3b000, .size = 0x1000 },   /* VISIONIP */
    { .name = "gsx",             .channel = 0,  .reg = 0xfd000000, .size = 0x800000 }, /* GFX */
    { .name = "fcp_cs",          .channel = 0,  .reg = 0xfe90f000, .size = 0x1000 },   /* FCP-CS */
    { .name = "ivcp1e",          .channel = 0,  .reg = 0xfea00000, .size = 0x10000 },  /* ENCODER */
    { .name = "cisp",            .channel = 0,  .reg = 0xfec00000, .size = 0x100000 }, /* ISP */
    { .name = "cisp",            .channel = 1,  .reg = 0xfee00000, .size = 0x100000 }, /* ISP */
    { .name = "vspx",            .channel = 0,  .reg = 0xfedd0000, .size = 0x8000 },   /* FCPVX */
    { .name = "vspx",            .channel = 1,  .reg = 0xfedd8000, .size = 0x8000 },   /* FCPVX */
    { .name = "fcpvx",           .channel = 0,  .reg = 0xfedb0000, .size = 0x8000 },   /* FCPVX */
    { .name = "fcpvx",           .channel = 1,  .reg = 0xfedb8000, .size = 0x8000 },   /* FCPVX */
    { .name = "rtdmac0",         .channel = 0,  .reg = 0xffc10000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 1,  .reg = 0xffc11000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 2,  .reg = 0xffc12000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 3,  .reg = 0xffc13000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 4,  .reg = 0xffc14000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 5,  .reg = 0xffc15000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 6,  .reg = 0xffc16000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 7,  .reg = 0xffc17000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 8,  .reg = 0xffc18000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 9,  .reg = 0xffc19000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 10, .reg = 0xffc1a000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 11, .reg = 0xffc1b000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 12, .reg = 0xffc1c000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 13, .reg = 0xffc1d000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 14, .reg = 0xffc1e000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac0",         .channel = 15, .reg = 0xffc1f000, .size = 0x1000 },   /* RT-DMAC0 */
    { .name = "rtdmac1",         .channel = 0,  .reg = 0xffc20000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 1,  .reg = 0xffc21000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 2,  .reg = 0xffc22000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 3,  .reg = 0xffc23000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 4,  .reg = 0xffc24000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 5,  .reg = 0xffc25000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 6,  .reg = 0xffc26000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 7,  .reg = 0xffc27000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 8,  .reg = 0xffc28000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 9,  .reg = 0xffc29000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 10, .reg = 0xffc2a000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 11, .reg = 0xffc2b000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 12, .reg = 0xffc2c000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 13, .reg = 0xffc2d000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 14, .reg = 0xffc2e000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac1",         .channel = 15, .reg = 0xffc2f000, .size = 0x1000 },   /* RT-DMAC1 */
    { .name = "rtdmac2",         .channel = 0,  .reg = 0xffd70000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 1,  .reg = 0xffd71000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 2,  .reg = 0xffd72000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 3,  .reg = 0xffd73000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 4,  .reg = 0xffd74000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 5,  .reg = 0xffd75000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 6,  .reg = 0xffd76000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 7,  .reg = 0xffd77000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 8,  .reg = 0xffd78000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 9,  .reg = 0xffd79000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 10, .reg = 0xffd7a000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 11, .reg = 0xffd7b000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 12, .reg = 0xffd7c000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 13, .reg = 0xffd7d000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 14, .reg = 0xffd7e000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac2",         .channel = 15, .reg = 0xffd7f000, .size = 0x1000 },   /* RT-DMAC2 */
    { .name = "rtdmac3",         .channel = 0,  .reg = 0xffd80000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 1,  .reg = 0xffd81000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 2,  .reg = 0xffd82000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 3,  .reg = 0xffd83000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 4,  .reg = 0xffd84000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 5,  .reg = 0xffd85000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 6,  .reg = 0xffd86000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 7,  .reg = 0xffd87000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 8,  .reg = 0xffd88000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 9,  .reg = 0xffd89000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 10, .reg = 0xffd8a000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 11, .reg = 0xffd8b000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 12, .reg = 0xffd8c000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 13, .reg = 0xffd8d000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 14, .reg = 0xffd8e000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "rtdmac3",         .channel = 15, .reg = 0xffd8f000, .size = 0x1000 },   /* RT-DMAC3 */
    { .name = "vin",             .channel = 0,  .reg = 0xe6ef0000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 1,  .reg = 0xe6ef1000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 2,  .reg = 0xe6ef2000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 3,  .reg = 0xe6ef3000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 4,  .reg = 0xe6ef4000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 5,  .reg = 0xe6ef5000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 6,  .reg = 0xe6ef6000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 7,  .reg = 0xe6ef7000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 8,  .reg = 0xe6ef8000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 9,  .reg = 0xe6ef9000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 10, .reg = 0xe6efa000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 11, .reg = 0xe6efb000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 12, .reg = 0xe6efc000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 13, .reg = 0xe6efd000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 14, .reg = 0xe6efe000, .size = 0x1000 },   /* VIN */
    { .name = "vin",             .channel = 15, .reg = 0xe6eff000, .size = 0x1000 },   /* VIN */
    { .name = "dsi",             .channel = 0,  .reg = 0xfed80000, .size = 0x10000 },  /* DSI */
    { .name = "dsi",             .channel = 1,  .reg = 0xfed90000, .size = 0x10000 },  /* DSI */
    { .name = "hsm",             .channel = 0,  .reg = 0xff000000, .size = 0x80000 },  /* PAP */
    { .name = "dcm",             .channel = 0,  .reg = 0xff080000, .size = 0x64000 },  /* PAP */
    { .name = "cbf",             .channel = 0,  .reg = 0xff0e8000, .size = 0x3000 },   /* PAP */
    { .name = "sbf",             .channel = 0,  .reg = 0xff0ec000, .size = 0x4000 },   /* PAP */
    { .name = "osm",             .channel = 0,  .reg = 0xff0f0000, .size = 0x10000 },  /* PAP */
    { .name = "ism",             .channel = 0,  .reg = 0xff100000, .size = 0x20000 },  /* PAP */
    { .name = "ism",             .channel = 1,  .reg = 0xff120000, .size = 0x20000 },  /* PAP */
    { .name = "sdmac",           .channel = 0,  .reg = 0xff180000, .size = 0x10000 },  /* PAP */
    { .name = "fbs",             .channel = 0,  .reg = 0xff200000, .size = 0x80000 },  /* PAP */
    { .name = "apb",             .channel = 0,  .reg = 0xff290000, .size = 0x4000 },   /* PAP */
    { .name = "dul",             .channel = 0,  .reg = 0xff298000, .size = 0x1000 },   /* PAP */
    { .name = "icumx_cmd",       .channel = 0,  .reg = 0xe6610000, .size = 0x8000 }    /* ICUMX */
};


/**
 * get the free slot
 * @return index of array if there is a free slot, else -1
 */
static int
get_free_instance(void) {
    int i;
    for (i = 0; i < MAX_INSTANCE; i++) {
        if (instances[i] == NULL) {
            return i;
        }
    }

    return -1;
}


/**
 * cleanup smmu context
 */
static void
deinit_smmu(void) {
    if (instance_num == 0) {
        smmu_fini();
    }
}


/**
 * get the index of name_map for name, channel
 * @param  name     ipmmu domain name
 * @param  channel  channel num
 * @return index of name_map if matching, else -1
 */
static int
find_reg_index(const char *const name, const int channel) {

    unsigned int i;
    const unsigned long name_len = strlen(name);

    for (i = 0; i < NUM_ELTS(name_map); i++) {
        if ((strncmp(name, name_map[i].name, name_len) == 0) && (channel == name_map[i].channel)) {
            return i;
        }
    }

    return -1;
}


/**
 * check physical address is aligned with 4K
 * @param  paddr   physical address
 * @return true if 4k aligned, else false
 */
static inline bool
aligned_with_4K(const uint64_t paddr) {
    return ((paddr & 0xfffUL) == 0UL);
}


/**
 * connect to smmu manager and make a session with group name and chnnel_id
 * @param   group_name   the name of group
 * @param   channel_id   channel id
 * @return fd if succeed, else -errno
 */
int
ipmmu_open(const char *const group_name, const int channel_id) {
    const int inst_index = get_free_instance();
    if (inst_index < 0) {
        return -ENOENT;
    }

    const int map_index = find_reg_index(group_name, channel_id);
    if (map_index < 0) {
        return -ENOENT;
    }

    struct instance *inst = calloc(sizeof(*inst), 1);
    if (inst == NULL) {
        return -ENOMEM;
    }

    if ((instance_num == 0) && (smmu_init(0) == -1)) {
        free(inst);
        return -errno;
    }

    struct smmu_object *const sop = smmu_obj_create(SOCF_NONE);
    if (sop == NULL) {
        deinit_smmu();
        free(inst);
        return -errno;
    }

    const struct name_reg_map *const ent = &name_map[map_index];
    if (smmu_device_add_mmio(sop, ent->reg, ent->size) < 0) {
        smmu_obj_destroy(sop);
        deinit_smmu();
        free(inst);
        return -errno;
    }

    struct iova_context *const iova_ctx = alloc_iova_context(IOVA_START_ADDRESS, IOVA_SIZE, IOVA_ALIGNMENT);
    if (iova_ctx == NULL) {
        smmu_device_add_mmio(NULL, ent->reg, ent->size);
        smmu_obj_destroy(sop);
        deinit_smmu();
        free(inst);
        return -ENOMEM;
    }

    inst->handle = inst_index;
    inst->index = map_index;
    inst->sop = sop;
    inst->iova_ctx = iova_ctx;

    instances[inst_index] = inst;
    instance_num++;

    return inst->handle;
}


/**
 * close connection
 * @param   handle  fd
 */
void
ipmmu_close(const int handle) {
    if ((handle >= MAX_INSTANCE) || (handle < 0)) {
        return;
    }

    struct instance *const inst = instances[handle];

    free_iova_context(inst->iova_ctx);

    const struct name_reg_map *const ent = &name_map[inst->index];
    smmu_device_add_mmio(NULL, ent->reg, ent->size);
    smmu_obj_destroy(inst->sop);
    free(inst);
    instances[handle] = NULL;
    instance_num--;

    deinit_smmu();
}


/**
 * Do mapping and get the io virtual address
 * @param   handle   fd
 * @param   pa       physical address
 * @param   len      length of address space
 * @param   pva      the pointer for getting the virtual address
 * @return 0 if succeed, else -errno
 */
int
ipmmu_map(const int handle, const uint64_t pa, const size_t len, uint32_t *pva) {
    if ((handle >= MAX_INSTANCE) || (handle < 0)) {
        return -EINVAL;
    }
    if (!aligned_with_4K(pa) || (len <= 0)) {
        return -EINVAL;
    }

    const struct instance *const inst = instances[handle];
    if (inst == NULL) {
        return -EINVAL;
    }

    uint32_t addr;
    if (alloc_iova(inst->iova_ctx, len, &addr) != 0) {
        return -ENOMEM;
    }

    struct smmu_map_entry entry;
    entry.phys = addr;
    entry.len = len;
    if (smmu_mapping_add(inst->sop, SMF_READ|SMF_WRITE|SMF_TARGET, 0, 1, &entry, pa) < 0) {
        return -errno;
    }

    *pva = addr;
    return 0;
}


/**
 * Do unmapping
 * @param   handle   fd
 * @param   pa       physical address
 * @param   va       virtual address
 * @param   len      length of address space
 * @return 0 if succeed, else -errno
 */
int
ipmmu_unmap(const int handle, const uint64_t pa, const uint32_t va, const size_t len) {
    if ((handle >= MAX_INSTANCE) || (handle < 0)) {
        return -EINVAL;
    }
    if (!aligned_with_4K(pa) || (len <= 0)) {
        return -EINVAL;
    }

    struct instance *const inst = instances[handle];
    if (inst == NULL) {
        return -EINVAL;
    }

    struct smmu_map_entry entry;
    entry.phys = va;
    entry.len = len;
    if (smmu_mapping_add(inst->sop, SMF_TARGET, 0, 1, &entry, pa) < 0) {
        return -errno;
    }

    free_iova(inst->iova_ctx, va, len);
    return 0;
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.x_dev/trunk/lib/hw_vendor/renesas/rcar-gen4/ipmmu/ipmmu.c $ $Rev: 982648 $")
#endif
