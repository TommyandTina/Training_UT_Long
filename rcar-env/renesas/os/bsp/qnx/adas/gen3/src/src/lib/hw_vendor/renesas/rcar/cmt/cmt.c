/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <atomic.h>
#include <hw/inout.h>
#include <sys/syspage.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <sys/mman.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>
#include <aarch64/r-car-gen3.h>
#include "proto.h"

/* Contents of the CMT registers */

/* CMSTR */
#define CMT_CMSTR_STR					(1 << 0)	//1: CMCNT0 start counting; 0: CMCNT0 halts
#define CMT_CMSTR_STRRS 				(1 << 8)	//1: RCLK-synchronous counter start/stop mode; 0: Normal operation

/* CMCSR */
#define CMT_CMCSR_CKS_MSK               7
#define CMT_CMCSR_DBGIVD				(1 << 3)	//Debug Mode Operation Select
#define CMT_CMCSR_CMR   				(2 << 4)	//Enables an internal interrupt request
#define CMT_CMCSR_CMM					(1 << 8)	//1: Free-running operation, 0: One-shot operation
#define CMT_CMCSR_CMS					(1 << 9)	//CMCSRH0[9].CMSH =0 -> 1: Operates as a 16-bit counter, 0: Operates as a 32-bit counter
#define CMT_CMCSR_WRFLG					(1 << 13)	//Write CMCNT register Flag
#define CMT_CMCSR_OVF					(1 << 14)	//1: CMCNTn has overflowed, 0: CMCNTn has not overflowed
#define CMT_CMCSR_CMF					(1 << 15)	//1: CMCNTn and CMCORn values have matched, 0: CMCNTn and CMCORn values have not matched

#define CMT_INPUT_CKS_MSK               0xF

/* CMCSRH */
#define CMT_CMCSRH_CMSH					(1 << 9)	//CMCSRHn.CMSH[9] =1, CMCSRn.CMS[9]:x =>  Operates as a 48-bit counter
#define CMT_CMCSRH_CKSH					(1 << 0)	

/* CMCSRM */
#define CMT_CMCSRM_CMPSTART				(1 << 0)	// 0: CMCNTMn Halts; 1: CMCNTMn Starts counting
#define CMT_CMCSRM_CMPCLR				(1 << 1)	// 0: No operation; 1: Clears counter


/* CMT0 type has maximum 2 channels and 1 group */
#define RCAR_CMT0_MAX_CHANNELS          2
#define RCAR_CMT0_CHAN_PER_GROUP        2

/* CMT1 type has maximum 24 channels and 3 groups */
#define RCAR_CMT1_MAX_CHANNELS          24
#define RCAR_CMT1_CHAN_PER_GROUP        8

#define RCAR_CMT1_48BIT_MAX_CHANNELS    15
#define RCAR_CMT1_48BIT_CHAN_PER_GROUP  5

/* CMT has 4 Group CMT0, CMT1, CMT2, CMT3 */
#define RCAR_CMT_MAX_MMAP_GROUPS        4   /* support groups CMT0/1/2/3*/

#define RCAR_GEN3_CPG_RCLK_3289         32890
#define RCAR_GEN3_CPG_RCLK_3255         32550
#define RCAR_GEN3_CPG_RCLK_32768        32768
#define RCAR_GEN3_CPG_RCLK_32800        32800
#define RCAR_GEN3_CPG_RCLK_31250        31250
#define RCAR_GEN3_CPG_CPEX              (16666666 / 2)
#define RCAR_GEN3_CPG_PSEU              32000
#define RCAR_D3E3_CPG_RCLK              31250
#define RCAR_D3E3_CPG_CPEX              (12000000 / 2)
#define RCAR_V3M_CPG_RCLK               32768
#define RCAR_V3M_CPG_CPEX               (10000000 / 2)
#define RCAR_V3HU_CPG_RCLK              32800
#define RCAR_V3U_CPG_CPEX               (16666666 / 2)
#define RCAR_V3U_CPG_PSEU               32000

/* IRQ number for H3/M3/M3N/E3/D3 */
static uint16_t GEN3_CMT_OVF_IRQS[] =
{
    /* CMT0 (Type 0) */
    RCAR_GEN3_INTCSYS_CMT00,
    RCAR_GEN3_INTCSYS_CMT01,
    /* CMT1, CMT2, CMT3 (Type 1) */
    RCAR_GEN3_INTCSYS_CMT10,
    RCAR_GEN3_INTCSYS_CMT11,
    RCAR_GEN3_INTCSYS_CMT12,
    RCAR_GEN3_INTCSYS_CMT13,
    RCAR_GEN3_INTCSYS_CMT14,
    RCAR_GEN3_INTCSYS_CMT15,
    RCAR_GEN3_INTCSYS_CMT16,
    RCAR_GEN3_INTCSYS_CMT17,
    RCAR_GEN3_INTCSYS_CMT20,
    RCAR_GEN3_INTCSYS_CMT21,
    RCAR_GEN3_INTCSYS_CMT22,
    RCAR_GEN3_INTCSYS_CMT23,
    RCAR_GEN3_INTCSYS_CMT24,
    RCAR_GEN3_INTCSYS_CMT25,
    RCAR_GEN3_INTCSYS_CMT26,
    RCAR_GEN3_INTCSYS_CMT27,
    RCAR_GEN3_INTCSYS_CMT30,
    RCAR_GEN3_INTCSYS_CMT31,
    RCAR_GEN3_INTCSYS_CMT32,
    RCAR_GEN3_INTCSYS_CMT33,
    RCAR_GEN3_INTCSYS_CMT34,
    RCAR_GEN3_INTCSYS_CMT35,
    RCAR_GEN3_INTCSYS_CMT36,
    RCAR_GEN3_INTCSYS_CMT37,
};

/* IRQ number for V3M/V3H */
static uint16_t V3MH_CMT_OVF_IRQS[] =
{
    /* CMT0 (Type 0) */
    RCAR_GEN3_INTCSYS_CMT00,
    RCAR_GEN3_INTCSYS_CMT01,
    /* CMT1, CMT2, CMT3 (Type 1) */
    RCAR_GEN3_INTCSYS_CMT10,
    RCAR_GEN3_INTCSYS_CMT11,
    RCAR_GEN3_INTCSYS_CMT12,
    RCAR_GEN3_INTCSYS_CMT13,
    RCAR_GEN3_INTCSYS_CMT14,
    RCAR_GEN3_INTCSYS_CMT15,
    RCAR_GEN3_INTCSYS_CMT16,
    RCAR_GEN3_INTCSYS_CMT17,
    RCAR_GEN3_INTCSYS_V3_CMT20,
    RCAR_GEN3_INTCSYS_V3_CMT21,
    RCAR_GEN3_INTCSYS_V3_CMT22,
    RCAR_GEN3_INTCSYS_V3_CMT23,
    RCAR_GEN3_INTCSYS_V3_CMT24,
    RCAR_GEN3_INTCSYS_V3_CMT25,
    RCAR_GEN3_INTCSYS_V3_CMT26,
    RCAR_GEN3_INTCSYS_V3_CMT27,
    RCAR_GEN3_INTCSYS_V3_CMT30,
    RCAR_GEN3_INTCSYS_V3_CMT31,
    RCAR_GEN3_INTCSYS_V3_CMT32,
    RCAR_GEN3_INTCSYS_V3_CMT33,
    RCAR_GEN3_INTCSYS_V3_CMT34,
    RCAR_GEN3_INTCSYS_V3_CMT35,
    RCAR_GEN3_INTCSYS_V3_CMT36,
    RCAR_GEN3_INTCSYS_V3_CMT37,
};

/* IRQ number for V3U */
static uint16_t V3U_CMT_OVF_IRQS[] =
{
    /* CMT0 (Type 0) */
    RCAR_V3U_INTCSYS_CMT00,
    RCAR_V3U_INTCSYS_CMT01,
    /* CMT1, CMT2, CMT3 (Type 1) */
    RCAR_V3U_INTCSYS_CMT10,
    RCAR_V3U_INTCSYS_CMT11,
    RCAR_V3U_INTCSYS_CMT12,
    RCAR_V3U_INTCSYS_CMT13,
    RCAR_V3U_INTCSYS_CMT14,
    RCAR_V3U_INTCSYS_CMT15,
    RCAR_V3U_INTCSYS_CMT16,
    RCAR_V3U_INTCSYS_CMT17,
    RCAR_V3U_INTCSYS_CMT20,
    RCAR_V3U_INTCSYS_CMT21,
    RCAR_V3U_INTCSYS_CMT22,
    RCAR_V3U_INTCSYS_CMT23,
    RCAR_V3U_INTCSYS_CMT24,
    RCAR_V3U_INTCSYS_CMT25,
    RCAR_V3U_INTCSYS_CMT26,
    RCAR_V3U_INTCSYS_CMT27,
    RCAR_V3U_INTCSYS_CMT30,
    RCAR_V3U_INTCSYS_CMT31,
    RCAR_V3U_INTCSYS_CMT32,
    RCAR_V3U_INTCSYS_CMT33,
    RCAR_V3U_INTCSYS_CMT34,
    RCAR_V3U_INTCSYS_CMT35,
    RCAR_V3U_INTCSYS_CMT36,
    RCAR_V3U_INTCSYS_CMT37,
};

static int cmt_channel_map[] = {/* 16/32 bit: */ 0, 1, 7, 8, 9, 15, 16, 17, 23, 24, 25, /* 16/32/48 bit: */ 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 18, 19, 20, 21, 22 };
static cmt_ctrl_t  cmt_mem_mmap[RCAR_CMT_MAX_MMAP_GROUPS] = { {0, 0}, };
static int cmt_clk_rclk = RCAR_GEN3_CPG_RCLK_3289;
static int cmt_clk_cpex = RCAR_GEN3_CPG_CPEX;
static int cmt_clk_pseu = RCAR_GEN3_CPG_PSEU;
uint16_t *cmt_irq_map = &GEN3_CMT_OVF_IRQS[0];

static int
rcar_cmt_init(char* options)
{
    const char  *option = NULL;
    int         opt;
    char        *value;
    static char	*opts[] = { "verbose", "soc", NULL };

    cmt_slog_verbose = 0;

    while( *options != '\0' ) {
        if( ( opt = getsubopt( &options, opts, &value ) ) == -1 ) {
            continue;
        }

        switch( opt ) {
            case 0:			// verbose
                cmt_slog_verbose = strtoul(value, NULL, 0);
                break;
            case 1:			// soc
                option = value;
                break;
            default:
               break;
        }
    }

#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (option == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        option = cs_machine_str;
    }
#endif

    if (option) {
        if ( strstr( option, "H3")  || strstr( option, "h3")  || 
             strstr( option, "M3N") || strstr( option, "m3n") ||
             strstr( option, "M3")  || strstr( option, "m3") ) {
            uint32_t  rcar_moder_reg = 0;
            uintptr_t vbase = (uintptr_t)mmap_device_io(4, RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR);
            if (vbase != (uintptr_t)MAP_FAILED) {
                rcar_moder_reg = in32(vbase);
                munmap_device_io(vbase, 4);
            }
            if(!(rcar_moder_reg & BIT(13)) && !(rcar_moder_reg & BIT(14))) {
                cmt_clk_rclk = SYSPAGE_ENTRY(qtime)->cycles_per_sec / (16 * 16);
            } else if(!(rcar_moder_reg & BIT(13)) && (rcar_moder_reg & BIT(14))) {
                cmt_clk_rclk = SYSPAGE_ENTRY(qtime)->cycles_per_sec / (19 * 16);
            } else if((rcar_moder_reg & BIT(13)) && !(rcar_moder_reg & BIT(14))) {
                cmt_clk_rclk = SYSPAGE_ENTRY(qtime)->cycles_per_sec / (24 * 16);
            } else {
                cmt_clk_rclk = SYSPAGE_ENTRY(qtime)->cycles_per_sec / (32 * 16);
            }
            cmt_clk_cpex = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
        } else if ( strstr( option, "D3") || strstr( option, "d3") ||
                    strstr( option, "E3") || strstr( option, "e3") ) {
            uint32_t rcar_rckcr_reg = 0;
            uintptr_t vbase = (uintptr_t)mmap_device_io(4, RCAR_GEN3_CPG_BASE + RCAR_GEN3_CPG_RCKCR);
            if (vbase != (uintptr_t)MAP_FAILED) {
                rcar_rckcr_reg = in32(vbase);
                munmap_device_io(vbase, 4);
            }

            if(rcar_rckcr_reg & BIT(15))
                cmt_clk_rclk = RCAR_GEN3_CPG_RCLK_32800;
            else
                cmt_clk_rclk = RCAR_GEN3_CPG_RCLK_31250;
            cmt_clk_cpex = SYSPAGE_ENTRY(qtime)->cycles_per_sec / 2;
        } else if ( strstr( option, "V3M") || strstr( option, "v3m") ) {
            cmt_clk_rclk = RCAR_V3M_CPG_RCLK;
            cmt_clk_cpex = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
            cmt_irq_map = &V3MH_CMT_OVF_IRQS[0];
        } else if ( strstr( option, "V3H") || strstr( option, "v3h") ) {
            uint32_t  rcar_moder_reg = 0;
            uintptr_t vbase = (uintptr_t)mmap_device_io(4, RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR);
            if (vbase != (uintptr_t)MAP_FAILED) {
                rcar_moder_reg = in32(vbase);
                munmap_device_io(vbase, 4);
            }
            if(rcar_moder_reg & BIT(29))
                cmt_clk_rclk = RCAR_GEN3_CPG_RCLK_32800;
            else
                cmt_clk_rclk = RCAR_GEN3_CPG_RCLK_32768;
            cmt_clk_cpex = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
            cmt_irq_map = &V3MH_CMT_OVF_IRQS[0];
        } else if ( strstr( option, "V3U") || strstr( option, "v3u") ) {
            uint32_t  rcar_moder_reg = 0;
            uintptr_t vbase = (uintptr_t)mmap_device_io(4, RCAR_V3U_RST_DM0_BASE + RCAR_V3U_RST_MODEMR0);
            if (vbase != (uintptr_t)MAP_FAILED) {
                rcar_moder_reg = in32(vbase);
                munmap_device_io(vbase, 4);
            }
            if(rcar_moder_reg & BIT(29))
                cmt_clk_rclk = RCAR_GEN3_CPG_RCLK_32800;
            else
                cmt_clk_rclk = RCAR_GEN3_CPG_RCLK_32768;
            cmt_clk_cpex = SYSPAGE_ENTRY(qtime)->cycles_per_sec / 2;
            cmt_irq_map = &V3U_CMT_OVF_IRQS[0];
        } else {
            CMT_SLOG_ERROR( "%s : Unknown SoC", __FUNCTION__);
            return (-1);
        }
    }

    return EOK;
}

static void
rcar_cmt_fini()
{
}

static int
rcar_cmt_query_channel(void *handle, cmt_channel_query_t *chinfo)
{
    cmt_chan_t   *chan = handle;

    if (!handle) {
        errno = ECHRNG;
        return -1;
    }

    chinfo->cmt_idx         = chan->grp_idx;
    chinfo->chan_idx        = chan->chan_idx_in_grp;
    chinfo->counter_size    = chan->counter_size;
    chinfo->clock_div       = chan->clock_div;
    chinfo->clock_src       = chan->clock_src;
    chinfo->irq             = chan->irq;
    chinfo->frequency       = chan->frequency;

    return 0;
}

static int
rcar_cmt_channel_info(int cmt_idx, int chan_idx, cmt_channel_info_t *chinfo)
{
    if ( (cmt_idx > RCAR_CMT_MAX_MMAP_GROUPS) ||
         (cmt_idx == 0 && chan_idx > RCAR_CMT0_MAX_CHANNELS) ||
         (cmt_idx != 0 && chan_idx > RCAR_CMT1_CHAN_PER_GROUP) ) {
        return -1;
    }

    if (cmt_idx != 0 && chan_idx <= 4) {
        chinfo->clock_src        = CMT_CLOCK_SRC_RCLK | CMT_CLOCK_SRC_CPEX | CMT_CLOCK_SRC_PSEUDO;
        chinfo->counter_size_max = 48;
        chinfo->frequency_max    = cmt_clk_cpex;
    } else {
        chinfo->clock_src        = CMT_CLOCK_SRC_RCLK;
        chinfo->counter_size_max = 32;
        chinfo->frequency_max    = cmt_clk_rclk;
    }

    return 0;
}

static void rcar_cmt_query_hwinfo(cmt_chan_t *chan)
{
    unsigned hwi_off, tag_idx;
    hwi_tag *tag;
    uint32_t chan_idx_in_grp = chan->chan_idx_in_grp;

    hwi_off = hwi_find_device(RCAR_HWI_CMT, chan->grp_idx);

    if(hwi_off != HWI_NULL_OFF){
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag){
            chan->pbase = tag->location.base;
            chan->size = tag->location.len;

            tag_idx = 0;
            while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                if(strcmp(RCAR_HWI_CMT_REG,  __hwi_find_string(tag->regname.regname)) == 0) {
                    chan->regs = tag->regname.offset;
                }
            }

            chan->irq = hwitag_find_ivec(hwi_off, &chan_idx_in_grp);

            tag_idx = 0;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, &tag_idx);
            if(tag) chan->clk_rclk_max = tag->inputclk.clk;
            tag_idx = 1;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, &tag_idx);
            if(tag) chan->clk_cpex_max = tag->inputclk.clk;
            tag_idx = 2;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, &tag_idx);
            if(tag) chan->clk_pseu_max = tag->inputclk.clk;
        }
    }
}

static uintptr_t rcar_cmt_map_registers(cmt_chan_t *chan)
{
    uint32_t mem_map_idx;

    chan->pbase = chan->irq = chan->regs = 0;

    rcar_cmt_query_hwinfo(chan);

    if( chan->pbase == 0 ) {
        switch( chan->grp_idx ) {
            case 0: 
                chan->pbase = RCAR_GEN3_CMT0_BASE; 
                chan->regs = RCAR_GEN3_CMT0_REG_OFFSET;
                break;
            case 1: 
                chan->pbase = RCAR_GEN3_CMT1_BASE;
                chan->regs = RCAR_GEN3_CMT1_REG_OFFSET;
                break;
            case 2:
                chan->pbase = RCAR_GEN3_CMT2_BASE;
                chan->regs = RCAR_GEN3_CMT1_REG_OFFSET;
                break;
            case 3:
                chan->pbase = RCAR_GEN3_CMT3_BASE;
                chan->regs = RCAR_GEN3_CMT1_REG_OFFSET;
                break;
        }

        chan->size = RCAR_GEN3_CMT_SIZE;
    }

    if( chan->pbase == 0) {
        return (uintptr_t)MAP_FAILED;
    }

    // check whether the memory for the CMT channel group is already mapped in.
    for (mem_map_idx = 0; mem_map_idx < RCAR_CMT_MAX_MMAP_GROUPS; mem_map_idx++) {
        if (cmt_mem_mmap[mem_map_idx].paddr == chan->pbase) {
            atomic_add(&cmt_mem_mmap[mem_map_idx].num, 1);
            chan->vbase = cmt_mem_mmap[mem_map_idx].vaddr;
            break;
        }
    }

    if (chan->vbase == 0) {
        chan->vbase = mmap_device_io(chan->size, chan->pbase);
        if (chan->vbase == (uintptr_t)MAP_FAILED)
            return (uintptr_t)MAP_FAILED;

        for (mem_map_idx = 0; mem_map_idx < RCAR_CMT_MAX_MMAP_GROUPS; mem_map_idx++) {
            if (cmt_mem_mmap[mem_map_idx].num == 0) {
                cmt_mem_mmap[mem_map_idx].paddr = chan->pbase;
                cmt_mem_mmap[mem_map_idx].vaddr = chan->vbase;
                cmt_mem_mmap[mem_map_idx].num   = 1;
                break;
            }
        }
    }

    chan->regs += chan->vbase + chan->chan_idx_in_grp * RCAR_GEN3_CMT_REG_SIZE;

    
    if (chan->clk_rclk_max == 0) {
        chan->clk_rclk_max = cmt_clk_rclk;
    }
    if (chan->clk_cpex_max == 0) {
        chan->clk_cpex_max = cmt_clk_cpex;
    }
    if (chan->clk_pseu_max == 0) {
        chan->clk_pseu_max = cmt_clk_pseu;
    }

    return (chan->vbase);
}

static void rcar_cmt_unmap_registers(cmt_chan_t *chan)
{
    uint32_t mem_map_idx;

    // unmap memory if not in use for other CMT channels in same group
    for (mem_map_idx = 0; mem_map_idx < RCAR_CMT_MAX_MMAP_GROUPS; mem_map_idx++) {
        if (cmt_mem_mmap[mem_map_idx].paddr == chan->pbase) {
            atomic_sub(&cmt_mem_mmap[mem_map_idx].num, 1);
            if (cmt_mem_mmap[mem_map_idx].num == 0) {
                munmap_device_io(cmt_mem_mmap[mem_map_idx].vaddr, chan->size);
                cmt_mem_mmap[mem_map_idx].num = cmt_mem_mmap[mem_map_idx].paddr = cmt_mem_mmap[mem_map_idx].vaddr = 0;
            }
            chan->vbase = 0;
        }
    }
}

static int rcar_cmt_create_resources(cmt_chan_t *chan)
{
    rsrc_alloc_t rsrc_req;
    int rsrc_ret = EOK;

    memset(&rsrc_req, 0, sizeof(rsrc_req));

    rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -1, chan->name, RSRCDBMGR_IO_PORT);

    if (rsrc_ret < 0) {
        rsrc_ret = errno;
        CMT_SLOG_ERROR("%s: Failed querying %s cmt resources", __FUNCTION__, chan->name);
        return rsrc_ret;
    } else if (rsrc_ret == 0) {
        rsrc_req.start = 0;
        rsrc_req.end = chan->channels - 1;
        rsrc_req.flags = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name = chan->name;

        rsrc_ret = rsrcdbmgr_create (&rsrc_req, 1);

        if( rsrc_ret != EOK ) {
            rsrc_ret = errno;
            CMT_SLOG_ERROR("%s: Failed creating cmt resources: err %d", __FUNCTION__, rsrc_ret );
            return rsrc_ret;
        }
    } else {
        return EALREADY;
    }

    return EOK;
}

static void *
rcar_cmt_channel_attach(const struct sigevent *event, const struct sigevent * (* handler)(void *, int), 
        const void * data, int priority, int timer_bitsz, cmt_attach_flags flags)
{
    cmt_chan_t   *chan;
    rsrc_request_t  req = { 0 };

    chan = calloc(1, sizeof(*chan));
    if (chan == NULL) {
        CMT_SLOG_ERROR("%s: calloc failed", __FUNCTION__);
        goto fail;
    }

    strcpy(chan->name, "cmt");

    chan->channels = RCAR_CMT0_MAX_CHANNELS + RCAR_CMT1_MAX_CHANNELS;

    rcar_cmt_create_resources(chan);

    req.length = 1;
    req.start  = 0;
    req.end    = chan->channels - 1;
    req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;

    // The lower CMT channel numbers have highest priority
    if (flags & CMT_ATTACH_PRIORITY_STRICT) {
        req.start = req.end = priority;
    } else {
        req.flags |= RSRCDBMGR_FLAG_TOPDOWN;
        if ((timer_bitsz == 48) || (flags & (CMT_ATTACH_CLOCK_CPEX | CMT_ATTACH_CLOCK_PSEUDO))) {
            req.start  = chan->channels - RCAR_CMT1_48BIT_MAX_CHANNELS;
        }
    }

    if (rsrcdbmgr_attach(&req, 1) == -1) {
        CMT_SLOG_ERROR("%s: CMT channel rsrcdbmgr_attach failed: %s", __FUNCTION__, strerror(errno));
        goto fail1;
    }

    if (flags & CMT_ATTACH_PRIORITY_STRICT)
        chan->chan_idx = req.start;
    else
        chan->chan_idx = cmt_channel_map[req.start];

    if (chan->chan_idx < 2) {
        chan->channels_per_group = RCAR_CMT0_CHAN_PER_GROUP;
        chan->grp_idx = 0;
        chan->chan_idx_in_grp = chan->chan_idx;
    } else {
        chan->channels_per_group = RCAR_CMT1_CHAN_PER_GROUP;
        chan->chan_idx -= RCAR_CMT0_MAX_CHANNELS;
        chan->grp_idx = 1 + (chan->chan_idx / chan->channels_per_group);
        chan->chan_idx_in_grp = chan->chan_idx % chan->channels_per_group;
    }

    // map in CMT registers for CMT channel group, including descriptor memory
    if (rcar_cmt_map_registers(chan) == (uintptr_t)MAP_FAILED) {
        goto fail2;
    }

    CMT_SLOG_INFO( 1, "CMT channel %d (%08x) mapped to %08x", chan->chan_idx, chan->pbase,
            (uint32_t)chan->vbase);

    if (chan->irq == 0) {
        if (flags & CMT_ATTACH_PRIORITY_STRICT)
            chan->irq = cmt_irq_map[req.start];
        else
            chan->irq = cmt_irq_map[cmt_channel_map[req.start]];        
    }

    chan->iid = -1;

    if (flags & CMT_ATTACH_EVENT_ON_OVERFLOW && event != NULL) {
        chan->iid = InterruptAttachEvent(chan->irq, event, _NTO_INTR_FLAGS_TRK_MSK);
    }
    else if (flags & CMT_ATTACH_ISR_ON_OVERFLOW && handler != NULL) {
        chan->iid = InterruptAttach(chan->irq, handler, data, 0, _NTO_INTR_FLAGS_TRK_MSK);
    }

    if (chan->iid == -1) {
        goto fail3;
    }

    chan->counter_support_48bit = 0;
    if ((chan->grp_idx) && (chan->chan_idx_in_grp <= 4))
        chan->counter_support_48bit = 1;

    chan->aflags = flags;

    // Start clock supplying to the CMT channel
    out32(chan->vbase + RCAR_GEN3_CMT_CMCLKE,
            in32(chan->vbase + RCAR_GEN3_CMT_CMCLKE) | (1 << ( (chan->grp_idx == 0 ? 5 : 0) + chan->chan_idx_in_grp)));

    return chan;

fail3:
    rcar_cmt_unmap_registers(chan);
fail2:
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;
    rsrcdbmgr_detach(&req, 1);

fail1:
    free(chan);
fail:
    return (NULL);
}

static int 
rcar_cmt_wait_for_access(void *handle)
{
    cmt_chan_t *chan = handle;
    int  timeout = 100; // Wait for 2 cycles (61us) of the RCLK clock (32.8 kHz), using max 100us

    for (;;) {
        if ((in32(chan->regs + RCAR_GEN3_CMT_CMCSR) & CMT_CMCSR_WRFLG) == 0)
            return 0;

        if (--timeout < 0)
            return -1;

        nanospin_ns(1000);
    }
}

static uint64_t 
rcar_cmt_read_cmcnt_reg(void *handle)
{
    cmt_chan_t *chan = handle;
    uint64_t  cmcnt = 0;

    if (chan->counter_support_48bit)
        cmcnt = ((uint64_t)in32(chan->regs + RCAR_GEN3_CMT_CMCNTH)) << 32;

    cmcnt += in32(chan->regs + RCAR_GEN3_CMT_CMCNT);

    return cmcnt;
}

static int 
rcar_cmt_set_cmcnt_reg(void *handle, uint64_t value)
{
    cmt_chan_t *chan = handle;
    int  i, timeout = 100; // Wait for 2 cycles (61us) of the RCLK clock (32.8 kHz), using max 100us

    for (i = 0 ; i < timeout; i++) {
        if ((in32(chan->regs + RCAR_GEN3_CMT_CMCSR) & CMT_CMCSR_WRFLG) == 0) {
            value &= chan->counter_mask;

            if (chan->counter_support_48bit)
                out32(chan->regs + RCAR_GEN3_CMT_CMCNTH, value >> 32);

            out32(chan->regs + RCAR_GEN3_CMT_CMCNT, value);

            return EOK;
        }

        nanospin_ns(1000);
    }

    return EBUSY;
}

static int 
rcar_cmt_counter_set_constant(void *handle, uint64_t value)
{
    cmt_chan_t *chan = handle;
    uint64_t cm_const;

    if (value == 0) {
        CMT_SLOG_ERROR("%s: Invalid constant %ld", __func__, value);
        return EINVAL;
    }

    cm_const = value & chan->counter_mask;

    out32(chan->regs + RCAR_GEN3_CMT_CMCOR, cm_const);

    if (chan->counter_support_48bit)
        out32(chan->regs + RCAR_GEN3_CMT_CMCORH, (cm_const >> 32) & 0xFFFF);

    /* Wait for 2 cycles of the RCLK clock (32.8 kHz) */
    nanospin_ns(61000);

    return EOK;
}

static uint64_t 
rcar_cmt_counter_get_constant(void *handle)
{
    cmt_chan_t *chan = handle;
    uint64_t cm_const;

    cm_const = in32(chan->regs + RCAR_GEN3_CMT_CMCOR);

    if (chan->counter_support_48bit)
        cm_const |= (uint64_t)in32(chan->regs + RCAR_GEN3_CMT_CMCORH) << 32;

    return cm_const;
}

static int
rcar_cmt_counter_set_count(void *handle, uint64_t value)
{
    if (!rcar_cmt_set_cmcnt_reg(handle, value)) {
        if (!rcar_cmt_wait_for_access(handle)) {
            return EOK;
        }
    }

    return EBUSY;
}

static uint64_t
rcar_cmt_counter_get_count(void *handle, int stop)
{
    uint64_t cnt, cnt1;
    
    cnt = rcar_cmt_read_cmcnt_reg(handle);

    if (stop) { // 'stop' is used in case we stopped the counter and wait for the couter update is completed
        while ((cnt1 = rcar_cmt_read_cmcnt_reg(handle)) != cnt) { 
            cnt = cnt1;
        }
    }

    return cnt;
}

static int
rcar_cmt_counter_setup(void *handle, uint64_t cmconst, int cntr_size, cmt_cm_mode_flags mode, cmt_clk_div_flags cks_div)
{
    cmt_chan_t *chan = handle;
    uint32_t   csr = 0;
    uint32_t   csrh = 0;

    if (cntr_size != 16 && cntr_size != 32 && cntr_size != 48) {
        CMT_SLOG_ERROR("%s: Invalid counter size %d", __func__, cntr_size);
        return EINVAL;
    }

    chan->clock_div = cks_div & CMT_INPUT_CKS_MSK;
    if (chan->counter_support_48bit == 0) {
        if (chan->clock_div & ~CMT_CMCSR_CKS_MSK) {
            CMT_SLOG_ERROR("%s: Invalid clock div 0x04%x", __func__, chan->clock_div);
            return EINVAL;
        }
    }

    chan->counter_size = cntr_size;

    if ( chan->counter_size == 48) {
        if (chan->counter_support_48bit == 0) {
            chan->counter_size = 32; // Lower down 32-bit counter
        }
    }

    if (chan->clock_div == 0)
        chan->clock_div = CMT_CKS_RCLK_DIV_1;

    csr |= (chan->clock_div & CMT_CMCSR_CKS_MSK);
    csrh = (chan->clock_div >> 3) & CMT_CMCSRH_CKSH;

    switch (chan->counter_size) {
        case 48: 
            csrh |= CMT_CMCSRH_CMSH;
            chan->counter_mask = BITMASK(0, 47);
            break;
        case 16:
            csr |= CMT_CMCSR_CMS;
            chan->counter_mask = BITMASK(0, 15);
            break;
        case 32:
        default: 
            chan->counter_mask = BITMASK(0, 31);
            break;
    }

    if (mode == CMT_MODE_FREE_RUNING)
        csr |= CMT_CMCSR_CMM;

    if (chan->aflags & (CMT_ATTACH_EVENT_ON_OVERFLOW | CMT_ATTACH_ISR_ON_OVERFLOW))
        csr |= CMT_CMCSR_CMR;

    // Stop counter
    out32(chan->regs + RCAR_GEN3_CMT_CMSTR, 0);

    /* Wait for 2 cycles of the RCLK clock (32.8 kHz) */
    nanospin_ns(61000);

    if (rcar_cmt_set_cmcnt_reg(handle, 0)) {
        return EBUSY;
    }

    out32(chan->regs + RCAR_GEN3_CMT_CMCSR, csr);
    if (chan->counter_support_48bit)
        out32(chan->regs + RCAR_GEN3_CMT_CMCSRH, csrh);

    cmconst &= chan->counter_mask;
    if(!cmconst)
        cmconst = chan->counter_mask;

    out32(chan->regs + RCAR_GEN3_CMT_CMCOR, cmconst);

    if (chan->counter_support_48bit)
        out32(chan->regs + RCAR_GEN3_CMT_CMCORH, (cmconst >> 32) & 0xFFFF);

    /* Wait for 2 cycles of the RCLK clock (32.8 kHz) */
    nanospin_ns(61000);

    /* Calculates counting clock */
    switch ((chan->clock_div >> 2) & 3) {
        case 1:   
            chan->frequency = chan->clk_rclk_max;
            chan->clock_src = CMT_CLOCK_SRC_RCLK;
            break;
        case 2:
            chan->frequency = chan->clk_cpex_max;
            chan->clock_src = CMT_CLOCK_SRC_CPEX;
            break;
        case 3:   
            chan->frequency = chan->clk_pseu_max;
            chan->clock_src = CMT_CLOCK_SRC_PSEUDO;
            break;
    }

    switch ((chan->clock_div) & 3) {
        case 0:   
            chan->frequency /= 8;
            break;
        case 1:   
            chan->frequency /= 32;
            break;
        case 2:
            chan->frequency /= 128;
            break;
        default:
            break;
    }

    if (rcar_cmt_wait_for_access(handle)) {
        return EBUSY;
    }

    return 0;
}

static int
rcar_cmt_counter_start(void *handle)
{
    cmt_chan_t   *chan = handle;

    out32(chan->regs + RCAR_GEN3_CMT_CMSTR, CMT_CMSTR_STR);

    return 0;
}

static int
rcar_cmt_counter_stop(void *handle)
{
    cmt_chan_t *chan = handle;

    // Stop counter
    out32(chan->regs + RCAR_GEN3_CMT_CMSTR, 0);

    return 0;
}

static int
rcar_cmt_counter_overflow(void *handle)
{
    cmt_chan_t *chan = handle;
    uint32_t       cmcsr;

    chan->overflow_cnt++;

    cmcsr = in32(chan->regs + RCAR_GEN3_CMT_CMCSR);

    // Clear interrupt flags
    out32(chan->regs + RCAR_GEN3_CMT_CMCSR, cmcsr & ~(CMT_CMCSR_CMF | CMT_CMCSR_OVF));

    if ((chan->aflags & CMT_ATTACH_EVENT_ON_OVERFLOW) && (chan->iid != -1)) {
        InterruptUnmask(chan->irq, chan->iid);
    }

    return 0;
}

static uint64_t
rcar_cmt_counter_get_cycle(void *handle, int stop)
{
    cmt_chan_t *chan = handle;
    uint64_t cm_const = rcar_cmt_counter_get_constant(handle);
    uint64_t count = rcar_cmt_counter_get_count(handle, stop);
    
    return (count + cm_const * chan->overflow_cnt);
}

static uint64_t
rcar_cmt_counter_change_cycle_to_ns(void *handle, uint64_t cnt, cmt_clk_div_flags clk_div)
{
    cmt_chan_t *chan = handle;
    uint64_t freq = chan->frequency;

    if (clk_div) {
        /* Calculates counting clock */
        switch ((clk_div >> 2) & 3) {
            case 1:   
                freq = chan->clk_rclk_max;
                break;
            case 2:
                freq = chan->clk_cpex_max;
                break;
            case 3:   
                freq = chan->clk_pseu_max;
                break;
        }

        switch ((clk_div) & 3) {
            case 0:   
                freq /= 8;
                break;
            case 1:   
                freq /= 32;
                break;
            case 2:
                freq /= 128;
                break;
            default:
                break;
        }
    }

    if (!freq)
        return 0;

    return ((cnt * 1000000000) / freq); // uint in ns
}

static uint64_t
rcar_cmt_counter_change_ns_to_cycle(void *handle, uint64_t ns, cmt_clk_div_flags clk_div)
{
    cmt_chan_t *chan = handle;
    uint64_t count = chan->frequency;
    
    if (clk_div) {
        /* Calculates counting clock */
        switch ((clk_div >> 2) & 3) {
            case 1:   
                count = chan->clk_rclk_max;
                break;
            case 2:
                count = chan->clk_cpex_max;
                break;
            case 3:   
                count = chan->clk_pseu_max;
                break;
        }

        switch ((clk_div) & 3) {
            case 0:   
                count /= 8;
                break;
            case 1:   
                count /= 32;
                break;
            case 2:
                count /= 128;
                break;
            default:
                break;
        }
    }

    return ((count * ns) / 1000000000);
}

static void
rcar_cmt_channel_release(void *handle)
{
    cmt_chan_t   *chan = handle;
    rsrc_request_t  req = { 0 };

    // Stop counting
    out32(chan->vbase + RCAR_GEN3_CMT_CMSTR, 0);

    rcar_cmt_wait_for_access(handle);

    /* Wait for 3 cycles of the RCLK clock (32.8 kHz) */
    nanospin_ns(100000);
    
    // Stop clock supplying
    out32(chan->vbase + RCAR_GEN3_CMT_CMCLKE, 
            in32(chan->vbase + RCAR_GEN3_CMT_CMCLKE) & ~(1 << ((chan->grp_idx == 0 ? 5 : 0) + chan->chan_idx_in_grp)));

    // Detach interrupt
    if (chan->iid != -1) {
        InterruptDetach(chan->iid);
    }

    // release CMT resource
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;
    rsrcdbmgr_detach(&req, 1);

    // unmap registers
    rcar_cmt_unmap_registers(chan);

    free(handle);
}

struct cmt_factory cmt_support = {
    .init               = rcar_cmt_init,
    .fini               = rcar_cmt_fini,
    .query_channel      = rcar_cmt_query_channel,
    .channel_info       = rcar_cmt_channel_info,
    .channel_attach     = rcar_cmt_channel_attach,
    .channel_release    = rcar_cmt_channel_release,
    .setup_counter      = rcar_cmt_counter_setup,
    .set_const          = rcar_cmt_counter_set_constant,
    .get_const          = rcar_cmt_counter_get_constant,
    .start_count        = rcar_cmt_counter_start,
    .stop_count         = rcar_cmt_counter_stop,
    .overflow           = rcar_cmt_counter_overflow,
    .get_count          = rcar_cmt_counter_get_count,
    .set_count          = rcar_cmt_counter_set_count,
    .get_cycle          = rcar_cmt_counter_get_cycle,
    .change_cycle_to_ns = rcar_cmt_counter_change_cycle_to_ns,
    .change_ns_to_cycle = rcar_cmt_counter_change_ns_to_cycle,
};

 
