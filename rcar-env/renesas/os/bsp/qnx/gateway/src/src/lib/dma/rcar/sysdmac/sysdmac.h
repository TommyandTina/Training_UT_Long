/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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


#ifndef __RCAR_SYSDMAC_H__
#define __RCAR_SYSDMAC_H__

#include <inttypes.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <fcntl.h>

typedef enum
{
    DMA_SYS = 0,
    DMA_AUDIO,
    DMA_RT,
    DMA_TYPE_NUM
} dma_type_t;

typedef struct
{
    paddr_t         paddr;
    uintptr_t       vaddr;
    paddr_t         paddr_grp;
    uintptr_t       vaddr_grp;
    unsigned        num;    // attach count
} sysdmac_ctrl_t;

typedef struct
{
    uint32_t        sar;
    uint32_t        dar;
    uint32_t        tcr;
    uint32_t        chcr;
} sysdmac_desc_t;

#define RCAR_DMA_NAME_MAX_SIZE      16

typedef struct
{
    uint32_t        chan_idx;
    uint32_t        grp_idx;
    uint32_t        chan_idx_in_grp;
    paddr_t         pbase;
    paddr_t         vbase;
    uint32_t        size;
    paddr_t         pbase_grp;
    paddr_t         vbase_grp;
    uint32_t        size_grp;          // Size of the DMA  group registers common to a group of channels
    uintptr_t       regs;
    uint32_t        regs_size;          // Size of the DMA channel registers
    uint32_t        channels;
    uint32_t        channel_groups;
    uint32_t        channels_per_group;
    dma_type_t      dma_type;

    uint32_t        xfer_count;
    uint32_t        xfer_unit_size;
    int             irq;
    int             iid;

    char            name[RCAR_DMA_NAME_MAX_SIZE];   // "sys" or "audio" or "rt"

    int16_t         range_min;  // Channel range start
    int16_t         range_max;  // Channel range end

    dma_mode_flags  mflags;     // mode flag of current transfer
    dma_attach_flags aflags;    // attach flag of client

    // built in descriptor memory
    int             desc_num;   // number of descriptor sets, zero if built in descriptor memory not used
    int             desc_idx;   // descriptor index
    uint32_t        desc_offset;

    // external descriptor memory
    dma_addr_t      desc;

    // built in or external memory descriptor segment number currently set up for transfer
    uint32_t        desc_seg_num;

    // buffer control
    int             buffer_mbu_max;
    int             buffer_mbu_default;
    int             buffer_ulb_max;
    int             buffer_ulb_default;

    uint32_t        debug;
#if _NTO_VERSION >= 704
    unsigned        typed_mem_fd;
#endif
    uint32_t        caie;
#if _NTO_VERSION >= 704
    struct smmu_object *sop;
#endif
} dma_channel_t;


#endif /* #ifndef __RCAR_SYSDMAC_H__ */

