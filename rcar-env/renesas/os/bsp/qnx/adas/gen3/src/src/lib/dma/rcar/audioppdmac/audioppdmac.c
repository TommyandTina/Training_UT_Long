/*
 * $QNXLicenseC:
 * Copyright 2015, QNX Software Systems.
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <atomic.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <hw/dma.h>
#include <arm/inout.h>
#if _NTO_VERSION >= 700
#include <aarch64/r-car-gen3.h>
#include <arm/r-car-gen2.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>
#else
#include <arm/r-car.h>
#endif
#include "audioppdmac.h"


/* Register offsets */
#define AUDIOPPDMAC_SAR         0x00
#define AUDIOPPDMAC_DAR         0x04
#define AUDIOPPDMAC_CHCR        0x0C

/* Register contents for AUDIOPPDMAC_CHCR register */
#define AUDIOPPDMAC_CHCR_DE     (1 << 0)    /* DMA enable */

/* Number of AUDIO PP DMAC channel groups, each with its own memory address base */
#define RCAR_PP_DMAC_GROUPS_H2         1   /* H2/M2/E2 variants have 1 group */
#define RCAR_PP_DMAC_GROUPS_V2         0   /* V2 variants don't support this functionality */
#define RCAR_PP_DMAC_GROUPS_H3         2   /* H3/M3 variants have two groups */
#define RCAR_PP_DMAC_GROUPS_D3         1   /* D3 variants have one group */
#define RCAR_PP_DMAC_GROUPS_E3         1   /* E3 variants have one group */
#define RCAR_PP_DMAC_GROUPS_V3         0   /* V3 variants don't support this functionality */

#define RCAR_PP_DMAC_CHANNELS_H2       29  /* H2/M2/E2 variants have 29 channels */
#define RCAR_PP_DMAC_CHANNELS_V2       0   /* V2 variants don't support this functionality */
#define RCAR_PP_DMAC_CHANNELS_H3       58  /* H3/M3 variants have 58 channels */
#define RCAR_PP_DMAC_CHANNELS_D3       4   /* D3 variants have 4 channels */
#define RCAR_PP_DMAC_CHANNELS_E3       29  /* E3 variants have 29 channels */
#define RCAR_PP_DMAC_CHANNELS_V3       0   /* V3 variants don't support this functionality */

/* Max number of groups */
#define RCAR_MAX_PP_DMAC_GROUPS        RCAR_PP_DMAC_GROUPS_H3

static rcar_soc_t  rcar_soc = RCAR_UNKNOWN;
static audioppdmac_ctrl_t  audioppdmac[RCAR_MAX_PP_DMAC_GROUPS] = { {0, 0}, };
static uint32_t    dmac_channels;
static uint32_t    dmac_groups;

#if _NTO_VERSION >= 700
static void query_hwinfo(dma_channel_t *chan)
{
    unsigned hwi_off;
    hwi_tag *tag;

    hwi_off = hwi_find_device(RCAR_HWI_AUDIOPPDMAC, chan->grp_idx);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag){
            chan->pbase = tag->location.base;
            chan->size = tag->location.len;
        }
    }
}
#endif

static int
audioppdma_init(const char *options)
{
#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (options == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        options = cs_machine_str;
    }
#endif
    if (options) {
        if( strstr( options, "H2") || strstr( options, "h2") ) {
            rcar_soc = RCAR_H2;
        } else if ( strstr( options, "M2") || strstr( options, "m2") ) {
            rcar_soc = RCAR_M2;
        } else if ( strstr( options, "E2") || strstr( options, "e2") ) {
            rcar_soc = RCAR_E2;
        } else if ( strstr( options, "V2") || strstr( options, "v2") ) {
            rcar_soc = RCAR_V2;
        } else if ( strstr( options, "H3") || strstr( options, "h3") ) {
            rcar_soc = RCAR_H3;
        } else if ( strstr( options, "M3") || strstr( options, "m3") ) {
            rcar_soc = RCAR_M3;
        } else if ( strstr( options, "D3") || strstr( options, "d3") ) {
            rcar_soc = RCAR_D3;
        } else if ( strstr( options, "E3") || strstr( options, "e3") ) {
            rcar_soc = RCAR_E3;
        } else if ( strstr( options, "V3") || strstr( options, "v3") ) {
            rcar_soc = RCAR_V3;
        } else if ( strstr( options, "W2H") || strstr( options, "w2h") ) {
            rcar_soc = RCAR_W2H;
        }
    }

    switch (rcar_soc) {
        case RCAR_H2:
        case RCAR_M2:
        case RCAR_E2:
        case RCAR_W2H:
            dmac_channels = RCAR_PP_DMAC_CHANNELS_H2;
            dmac_groups   = RCAR_PP_DMAC_GROUPS_H2;
            break;
        case RCAR_V2:
            dmac_channels = RCAR_PP_DMAC_CHANNELS_V2;
            dmac_groups   = RCAR_PP_DMAC_GROUPS_V2;
            break;
        case RCAR_H3:
        case RCAR_M3:
            dmac_channels = RCAR_PP_DMAC_CHANNELS_H3;
            dmac_groups   = RCAR_PP_DMAC_GROUPS_H3;
            break;
        case RCAR_V3:
            dmac_channels = RCAR_PP_DMAC_CHANNELS_V3;
            dmac_groups   = RCAR_PP_DMAC_GROUPS_V3;
            break;
        case RCAR_D3:
            dmac_channels = RCAR_PP_DMAC_CHANNELS_D3;
            dmac_groups   = RCAR_PP_DMAC_GROUPS_D3;
            break;
        case RCAR_E3:
            dmac_channels = RCAR_PP_DMAC_CHANNELS_E3;
            dmac_groups   = RCAR_PP_DMAC_GROUPS_E3;
            break;
        default:
            fprintf(stderr, "%s: Unknown SoC %d (%s)\n", __FUNCTION__,
                                rcar_soc, options ? options : "Null");
            return EINVAL;
    }
    return EOK;
}

static void
audioppdma_fini()
{
}

static char *dmac_opts[] = {
#define AUDIOPPDMAC_OPT_VER    0
    "ver",
#define AUDIOPPDMAC_OPT_DEBUG  1
    "debug",
    NULL
};

static int
audioppdmac_parse_options(dma_channel_t *chan, char *options)
{
    char   *value;
    int     opt;

    while (options && *options != '\0') {
        if ((opt = getsubopt(&options, dmac_opts, &value)) == -1) {
            return EINVAL;
        }
        switch (opt) {
            case AUDIOPPDMAC_OPT_VER:
                audioppdma_init(value);
                break;
            case AUDIOPPDMAC_OPT_DEBUG:
                chan->debug = strtoul(value, 0, 0);
                break;
            default:
                return EINVAL;
        }
    }

    if (rcar_soc == RCAR_UNKNOWN) {
        if (audioppdma_init(NULL) != EOK) {
            return EINVAL;
        }
    }

    chan->channels       = dmac_channels;
    chan->channel_groups = dmac_groups;

    return EOK;
}

static void
audioppdma_query_channel(void *handle, dma_channel_query_t *chinfo)
{
    dma_channel_t   *chan = handle;

    chinfo->chan_idx = chan->chan_idx;
}

static int
audioppdma_driver_info(dma_driver_info_t *info)
{
    if (rcar_soc == RCAR_UNKNOWN) {
        if (audioppdma_init(NULL) != EOK) {
            errno = EINVAL;
            return -1;
        }
    }

    info->dma_version_major = DMALIB_VERSION_MAJOR;
    info->dma_version_minor = DMALIB_VERSION_MINOR;
    info->dma_rev           = DMALIB_REVISION;
    info->lib_rev           = 0;
    info->description       = "RCAR AUDIO PP DMAC Controller";
    info->num_channels      = dmac_channels;

    return 0;
}

static int
audioppdma_channel_info(unsigned channel, dma_channel_info_t *info)
{
    if (rcar_soc == RCAR_UNKNOWN) {
        if (audioppdma_init(NULL) != EOK) {
            errno = EINVAL;
            return -1;
        }
    }

    if (channel >= dmac_channels) {
        errno = ECHRNG;
        return -1;
    }

    info->max_xfer_size         = 0xffffffff;
    info->xfer_unit_sizes       = 0x4; /* 4 byte unit transfers, fixed */
    info->max_src_fragments     = 1;
    info->max_dst_fragments     = 1;
    info->max_src_segments      = 1;
    info->max_dst_segments      = 1;
    info->caps                  = DMA_CAP_DEVICE_TO_DEVICE;
    info->mem_lower_limit       = 0;
    info->mem_upper_limit       = 0xffffffff;
    info->mem_nocross_boundary  = 0;

    return 0;
}

static uintptr_t audioppdma_map_registers(dma_channel_t *chan)
{
    uint32_t mem_map_idx;
    uint32_t chan_per_grp = chan->channels / chan->channel_groups;
    uint32_t chan_in_grp_idx = chan->chan_idx % chan_per_grp;

    chan->pbase = 0;
#if _NTO_VERSION >= 700
    query_hwinfo(chan);
    if( chan->pbase == 0 ) {
        if( rcar_soc == RCAR_H3 || rcar_soc == RCAR_M3 || rcar_soc == RCAR_D3) {
            switch( chan->grp_idx ) {
                case 0: chan->pbase = RCAR_GEN3_AUDIODMACPP_BASE; break;
                case 1: chan->pbase = RCAR_GEN3_AUDIODMACPP_EXT_BASE; break;
            }
            chan->size = RCAR_GEN3_AUDIODMACPP_SIZE;
        } else if (rcar_soc == RCAR_E3) {
            chan->pbase = RCAR_GEN3_AUDIODMACPP_EXT_BASE;
            chan->size = RCAR_GEN3_AUDIODMACPP_SIZE;
        } else if( rcar_soc == RCAR_H2 || rcar_soc == RCAR_M2 ||
                   rcar_soc == RCAR_E2 || rcar_soc == RCAR_W2H) {
            switch( chan->grp_idx ) {
                case 0: chan->pbase = RCAR_GEN2_AUDIODMACPP_BASE; break;
            }
            chan->size = RCAR_GEN2_AUDIODMACPP_SIZE;
        }
    }
#else
    /* before 7.0 only 2nd generation rcar platforms are supported;
     * these platforms support only one audio dmac pp group
     */
    chan->pbase = RCAR_AUDIODMACPP_BASE;
    chan->size = RCAR_AUDIODMACPP_SIZE;
#endif

    if( chan->pbase == 0 ) {
        return (uintptr_t)MAP_FAILED;
    }

    /* check whether the memory for the DMA channel group is already mapped in. */
    for (mem_map_idx = 0; mem_map_idx < RCAR_MAX_PP_DMAC_GROUPS; mem_map_idx++) {
        if (audioppdmac[mem_map_idx].paddr == chan->pbase) {
            atomic_add(&audioppdmac[mem_map_idx].num, 1);
            chan->vbase = audioppdmac[mem_map_idx].vaddr;
            break;
        }
    }

    if (chan->vbase == 0) {
        chan->vbase = mmap_device_io(chan->size, chan->pbase);
        if (chan->vbase == (uintptr_t)MAP_FAILED) {
            return (uintptr_t)MAP_FAILED;
        }
        for (mem_map_idx = 0; mem_map_idx < RCAR_MAX_PP_DMAC_GROUPS; mem_map_idx++) {
            if (audioppdmac[mem_map_idx].num == 0) {
                audioppdmac[mem_map_idx].paddr = chan->pbase;
                audioppdmac[mem_map_idx].vaddr = chan->vbase;
                audioppdmac[mem_map_idx].num   = 1;
                break;
            }
        }
    }

    chan->regs = chan->vbase + 0x20 + chan_in_grp_idx * 0x10;

    return (chan->vbase);
}

static void audioppdmac_unmap_registers(dma_channel_t *chan)
{
    uint32_t mem_map_idx;

    /* unmap memory if not in use for other DMA channels in same group */
    for (mem_map_idx = 0; mem_map_idx < RCAR_MAX_PP_DMAC_GROUPS; mem_map_idx++) {
        if (audioppdmac[mem_map_idx].paddr == chan->pbase) {
            atomic_sub(&audioppdmac[mem_map_idx].num, 1);
            if (audioppdmac[mem_map_idx].num == 0) {
                munmap_device_io(audioppdmac[mem_map_idx].vaddr, chan->size);
                audioppdmac[mem_map_idx].num = audioppdmac[mem_map_idx].paddr = audioppdmac[mem_map_idx].vaddr = 0;
            }
            chan->vbase = 0;
        }
    }
}

static void audioppdmac_dump_registers(dma_channel_t *chan)
{
    fprintf(stdout, "AUDIOPP DMA chan %d reg dump: SAR=%x DAR=%x CHCR=%x\n", chan->chan_idx,
        in32(chan->regs + AUDIOPPDMAC_SAR), in32(chan->regs + AUDIOPPDMAC_DAR),
        in32(chan->regs + AUDIOPPDMAC_CHCR));
}

static int audioppdmac_create_resources(dma_channel_t *chan)
{
    static rsrc_alloc_t rsrc_req;
    int rsrc_ret = EOK;

    memset (&rsrc_req, 0, sizeof(rsrc_req));

    rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -1, "audiopp", RSRCDBMGR_DMA_CHANNEL);

    if( rsrc_ret < 0 ) {
        rsrc_ret = errno;
        fprintf (stderr, "%s: Failed querying audiopp resources\n", __FUNCTION__);
        return rsrc_ret;
    } else if( rsrc_ret > 0 ) {
        return EALREADY;
    }

    rsrc_req.start = 0;
    rsrc_req.end = chan->channels - 1;
    rsrc_req.flags = RSRCDBMGR_DMA_CHANNEL|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_NOREMOVE;
    rsrc_req.name = "audiopp";

    rsrc_ret = rsrcdbmgr_create( &rsrc_req, 1 );

    if( rsrc_ret != EOK ) {
        rsrc_ret = errno;
        fprintf(stderr, "%s: Failed creating resources: err %d\n", __FUNCTION__, rsrc_ret );
    }

    return rsrc_ret;
}

static void *
audioppdma_channel_attach(const char *options, const struct sigevent *event, unsigned *channel, int priority, dma_attach_flags flags)
{
    dma_channel_t   *chan;
    char   *optstr = NULL;
    rsrc_request_t  req = { 0 };
    uint32_t chans_per_group;

    chan = calloc(1, sizeof(*chan));
    if (chan == NULL) {
        fprintf(stderr, "%s: calloc failed\n", __FUNCTION__);
        return (NULL);
    }

    if (options) {
        optstr = strdup(options);
    } else {
        optstr = strdup("ver=h2");
    }

    if (optstr == NULL) {
        fprintf(stderr,  "%s: strdup failed\n", __FUNCTION__);
        free(chan);
        return NULL;
    }

    if (audioppdmac_parse_options(chan, optstr) != EOK) {
        fprintf(stderr, "%s: dma_parse_options failed\n", __FUNCTION__);
        free(chan);
        free(optstr);
        return NULL;
    }

    audioppdmac_create_resources(chan);

    req.length = 1;
    req.start  = 0;
    req.end    = chan->channels - 1;
    req.flags  = RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = "audiopp";

    if (rsrcdbmgr_attach(&req, 1) == -1) {
        fprintf(stderr, "%s: rsrcdbmgr_attach failed: %s\n", __FUNCTION__, strerror(errno));
        free(chan);
        free(optstr);
        return (NULL);
    }

    chans_per_group = chan->channels / chan->channel_groups;
    chan->chan_idx = req.start;
    chan->grp_idx = req.start / chans_per_group;

    if( audioppdma_map_registers( chan ) == (uintptr_t)MAP_FAILED ) {
        fprintf(stderr, "%s: register map failed\n", __FUNCTION__);
        req.end = req.start;
        rsrcdbmgr_detach(&req, 1);
        free(chan);
        free(optstr);
        return (NULL);
    }

    /* Disable the channel */
    out32(chan->regs + AUDIOPPDMAC_CHCR, 0);

    free(optstr);

    return chan;
}

static void
audioppdma_channel_release(void *handle)
{
    dma_channel_t *chan = handle;
    rsrc_request_t req = { 0 };

    /* Disable the channel */
    out32(chan->regs + AUDIOPPDMAC_CHCR, 0);

    /* release DMA resource */
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = "audiopp";
    rsrcdbmgr_detach(&req, 1);

    /* unmap registers */
    audioppdmac_unmap_registers(chan);

    free(handle);
}

static int
audioppdma_setup_xfer(void *handle, const dma_transfer_t *tinfo)
{
    dma_channel_t   *chan = handle;

    out32(chan->regs + AUDIOPPDMAC_SAR,  tinfo->src_addrs[0].paddr);
    out32(chan->regs + AUDIOPPDMAC_DAR,  tinfo->dst_addrs[0].paddr);
    out32(chan->regs + AUDIOPPDMAC_CHCR, tinfo->req_id);

    if (chan->debug) {
        audioppdmac_dump_registers (chan);
    }

    return 0;
}

static int
audioppdma_start(void *handle)
{
    dma_channel_t   *chan = handle;

    out32(chan->regs + AUDIOPPDMAC_CHCR, in32(chan->regs + AUDIOPPDMAC_CHCR) | AUDIOPPDMAC_CHCR_DE);

    if (chan->debug) {
        audioppdmac_dump_registers (chan);
    }

    return 0;
}

static int
audioppdma_abort(void *handle)
{
    dma_channel_t   *chan = handle;
    int timeout = 100; /* 100 * 100 microseconds = 10 miliseconds */
    int i = 0;

    out32(chan->regs + AUDIOPPDMAC_CHCR, in32(chan->regs + AUDIOPPDMAC_CHCR) & ~(AUDIOPPDMAC_CHCR_DE));

    /* Even though we set the AUDIOPPDMAC_CHCR_DE bit to 0, it will read as 1 until
     * the current transfer is complete; wait until the current transfer is complete
     * by checking that the AUDIOPPDMAC_CHCR_DE bit reads 0 */
    for (i = 0; i < timeout; i++) {
        if ( !(in32(chan->regs + AUDIOPPDMAC_CHCR) & AUDIOPPDMAC_CHCR_DE) ) {
            break;
        }

        nanospin_ns(100000); /* 100 microseconds */
    }

    if (chan->debug) {
        audioppdmac_dump_registers (chan);
    }

    if (i == timeout) {
        fprintf(stderr, "%s: AUDIOPP DMA chan %d: wait for DE clear timed out\n",
            __FUNCTION__, chan->chan_idx);
        return ETIMEDOUT;
    } else if (i > 0 && chan->debug) {
        fprintf(stdout, "%s: AUDIOPP DMA chan %d: waited %d ms for DE clear\n",
            __FUNCTION__, chan->chan_idx, i/10);
    }

    return 0;
}

int
get_audioppdmafuncs(dma_functions_t *functable, int tabsize)
{
    DMA_ADD_FUNC(functable, init, audioppdma_init, tabsize);
    DMA_ADD_FUNC(functable, fini, audioppdma_fini, tabsize);
    DMA_ADD_FUNC(functable, driver_info, audioppdma_driver_info, tabsize);
    DMA_ADD_FUNC(functable, channel_info, audioppdma_channel_info, tabsize);
    DMA_ADD_FUNC(functable, channel_attach, audioppdma_channel_attach, tabsize);
    DMA_ADD_FUNC(functable, channel_release, audioppdma_channel_release, tabsize);
    DMA_ADD_FUNC(functable, setup_xfer, audioppdma_setup_xfer, tabsize);
    DMA_ADD_FUNC(functable, xfer_start, audioppdma_start, tabsize);
    DMA_ADD_FUNC(functable, xfer_abort, audioppdma_abort, tabsize);
    DMA_ADD_FUNC(functable, xfer_complete, audioppdma_abort, tabsize);
    DMA_ADD_FUNC(functable, query_channel, audioppdma_query_channel, tabsize);

    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/dma/rcar/audioppdmac/audioppdmac.c $ $Rev: 882286 $")
#endif
