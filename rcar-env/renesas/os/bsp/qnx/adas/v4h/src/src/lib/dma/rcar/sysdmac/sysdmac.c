/*
 * Copyright (c) 2018, 2023, BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <atomic.h>
#include <sys/slog2.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <hw/dma.h>
#include <arm/inout.h>
#include <aarch64/r-car.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>
#include "sysdmac.h"
#include "slogf.h"

//#define DEBUG 1

#define RCAR_DMA_XFER_COUNT_MAX      16777215

/* Contents of the CHCR register */
#define RCAR_DMACHCR_CAE            0x80000000
#define RCAR_DMACHCR_CAIE           0x40000000
#define RCAR_DMACHCR_DPM            0x30000000
#define RCAR_DMACHCR_DPM_DISABLE    0x00000000
#define RCAR_DMACHCR_DPM_NORMAL     0x10000000
#define RCAR_DMACHCR_DPM_RPT        0x20000000
#define RCAR_DMACHCR_DPM_RPT_INF    0x30000000
#define RCAR_DMACHCR_RPT            0x0E000000
#define RCAR_DMACHCR_RPT_S          0x08000000
#define RCAR_DMACHCR_RPT_D          0x04000000
#define RCAR_DMACHCR_RPT_C          0x02000000
#define RCAR_DMACHCR_RPT_CH         0x01000000
#define RCAR_DMACHCR_DPB            0x00400000
#define RCAR_DMACHCR_TS2            0x00100000
#define RCAR_DMACHCR_DSE            0x00080000
#define RCAR_DMACHCR_DSIE           0x00040000
#define RCAR_DMACHCR_DM             0x0000C000
#define RCAR_DMACHCR_DM_FIXED       0x00000000
#define RCAR_DMACHCR_DM_INCR        0x00004000
#define RCAR_DMACHCR_DM_DECR        0x00008000
#define RCAR_DMACHCR_SM             0x00003000
#define RCAR_DMACHCR_SM_FIXED       0x00000000
#define RCAR_DMACHCR_SM_INCR        0x00001000
#define RCAR_DMACHCR_SM_DECR        0x00002000
#define RCAR_DMACHCR_RS             0x00000F00
#define RCAR_DMACHCR_RS_AUTO        0x00000400
#define RCAR_DMACHCR_RS_ONCHIP      0x00000800
#define RCAR_DMACHCR_TS             0x00000018
#define RCAR_DMACHCR_TS_BYTE        0x00000000
#define RCAR_DMACHCR_TS_WORD        0x00000008
#define RCAR_DMACHCR_TS_LONG        0x00000010
#define RCAR_DMACHCR_TS_8           0x00100018
#define RCAR_DMACHCR_TS_16          0x00000018
#define RCAR_DMACHCR_TS_32          0x00100000
#define RCAR_DMACHCR_TS_64          0x00100008
#define RCAR_DMACHCR_IE             0x00000004
#define RCAR_DMACHCR_TE             0x00000002
#define RCAR_DMACHCR_DE             0x00000001

/* Contents of the CHCRB register */
#define RCAR_DMACHCRB_DRST          0x00008000

/* Memory descriptors related definitions */
#define SYSDMAC_DESCRIPTORS_PER_GROUP    ( ((rcar_soc == PRODUCT_ID_RCAR_S4) || (rcar_soc == PRODUCT_ID_RCAR_V4H)) ? 4096 : 128 )


/*
 * DMAC register map and the corresponding variables used in this driver:
 *
 * Rcar Gen3: H3/M3/D3/E3/V3M/V3H
 *      DMAC base  ----------- 0x0 ---> pbase_grp, size_grp 0x100
 *                 | regs    |
 *                 |         |
 *                 ----------- 0x8000 ---> pbase, size 0x3000
 *                 | channels| chregs_size 0x80 (per channel)
 *                 |         |
 *                 ----------- 0xA000 --> desc_offset
 *                 |  desc   |
 *                 |         |
 *                 -----------
 *
 * Rcar Gen4: V3U/S4/V4H
 *      DMAC base  ----------- 0x0 ---> pbase, size 0xF200
 *                 | channels| chregs_size 0x1000 (per channel)
 *                 |         |
 *                 -----------
 *
 *
 *  DMAC COM base  ----------- 0x0 ---> pbase_grp, size_grp 0x1000
 *                 | regs    | size 0x200
 *                 |         |
 *                 ----------- 0x800 --> desc_offset (V3U only)
 *                 |  desc   |
 *                 |         |
 *                 -----------
 */


/* Number of SYSDMAC + AUDIODMAC + RT channel groups, each with its own memory address base */
#define RCAR_MAX_DMAC_MMAP_GROUPS         7   /* support groups SYSDMAC 0/1/2, AUDIODMAC 0/1 and RTDMAC 0/1 */

/* Size of registers that are common to a group of channel */
#define RCAR_SYSDMAC_GRP_REGSIZE 0x100

static uint32_t        rcar_soc = PRODUCT_ID_RCAR_UNKNOWN;
static uint8_t         dmac_channels[DMA_TYPE_NUM] = {0};
static uint8_t         dmac_channels_per_group[DMA_TYPE_NUM] = {0};
static sysdmac_ctrl_t  sysdmac[RCAR_MAX_DMAC_MMAP_GROUPS] = { 0 };
static const char*     dmac_type_names[] = {
    [DMA_SYS]   = "sys",
    [DMA_AUDIO] = "audio",
    [DMA_RT]    = "rt",
};

static int dma_alloc_buffer(void* const handle, dma_addr_t *addr, unsigned size, unsigned flags);
static void dma_free_buffer(void* const handle, dma_addr_t *addr);

static void rsrcdb_dma_to_rsrcdb_name(char* const buf, const int buf_size,
                                const dma_type_t type, const int is_channel)
{
    snprintf(buf, buf_size, "%sdma_%s", dmac_type_names[type], is_channel ? "channels" : "memory");
}

static int rsrcdb_create_resources(const dma_type_t type)
{
    rsrc_alloc_t rsrc_req;
    int dma_rsrc_ret = EOK;
    int mem_rsrc_ret = EOK;
#define NAME_SIZE  50
    char name[NAME_SIZE];

    /* Add the DMA Channels to the resources database */
    memset(&rsrc_req, 0, sizeof(rsrc_req));

    rsrcdb_dma_to_rsrcdb_name(name, NAME_SIZE, type, 1);

    dma_rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -2, name, 0);
    if (dma_rsrc_ret < 0) {
        dma_rsrc_ret = errno;
        rcar_dma_slogf(SLOG2_ERROR, "%s: Failed querying %s dma resources", __FUNCTION__, name);
        return dma_rsrc_ret;
    } else if (dma_rsrc_ret == 0) {
        rsrc_req.start = 0;
        rsrc_req.end = dmac_channels[type] - 1;
        rsrc_req.flags = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name = name;

        dma_rsrc_ret = rsrcdbmgr_create(&rsrc_req, 1);
        if ( dma_rsrc_ret != EOK ) {
            dma_rsrc_ret = errno;
            rcar_dma_slogf(SLOG2_ERROR, "%s: Failed creating resources: err %d", __FUNCTION__, dma_rsrc_ret );
            return dma_rsrc_ret;
        }
    } else {
        dma_rsrc_ret = EALREADY;
    }

    /* Add the DMA descriptors memory to the resources database */
    memset (&rsrc_req, 0, sizeof(rsrc_req));

    rsrcdb_dma_to_rsrcdb_name(name, NAME_SIZE, type, 0);

    mem_rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -2, name, 0);
    if (mem_rsrc_ret < 0) {
        mem_rsrc_ret = errno;
        rcar_dma_slogf(SLOG2_ERROR, "%s: Failed querying %s memory resources", __FUNCTION__, name);
        return mem_rsrc_ret;
    } else if (mem_rsrc_ret == 0) {
        int channel_groups;
        channel_groups = dmac_channels[type] / dmac_channels_per_group[type];
        channel_groups += ((dmac_channels[type] % dmac_channels_per_group[type]) ? 1 : 0);

        rsrc_req.start  = 0;
        rsrc_req.end    = channel_groups * 128 - 1;
        rsrc_req.flags  = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name   = name;

        mem_rsrc_ret = rsrcdbmgr_create(&rsrc_req, 1);
        if (mem_rsrc_ret != EOK) {
            mem_rsrc_ret = errno;
            rcar_dma_slogf(SLOG2_ERROR, "%s: Failed creating resources: err %d", __FUNCTION__, mem_rsrc_ret);
            return mem_rsrc_ret;
        }
    } else {
        mem_rsrc_ret = EALREADY;
    }

    if ((dma_rsrc_ret == EALREADY) && (mem_rsrc_ret == EALREADY)) {
        return EALREADY;
    }

    return EOK;
}

static int rsrcdb_attach_channel(const dma_channel_t* const chan, const unsigned* const channel,
                        const uint64_t priority, const dma_attach_flags flags)
{
#define NAME_SIZE  50
    char name[NAME_SIZE];
    rsrc_request_t  req = { 0 };

    rsrcdb_dma_to_rsrcdb_name(name, NAME_SIZE, chan->type, 1);

    req.length = 1;
    req.start  = 0;
    req.end    = chan->channels - 1;
    req.flags  = RSRCDBMGR_FLAG_NAME;
    req.name   = name;

    if ((chan->range_min != -1) && (chan->range_max != -1)) {
        req.start  = chan->range_min;
        req.end    = chan->range_max;
        req.flags |= RSRCDBMGR_FLAG_RANGE;
    }

    if (!(flags & DMA_ATTACH_ANY_CHANNEL)) {
        // A specific (valid) channel is requested
        if (channel == NULL) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: NULL is an Invalid Channel number", __FUNCTION__);
            return -1;
        }

        // Check if we have range limitations
        if ((req.flags & RSRCDBMGR_FLAG_RANGE) &&
             ((*channel < req.start) || (*channel > req.end))) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: channel %d is out of range %jd:%jd!", __FUNCTION__,
                    channel ? (int) *channel : -1, req.start, req.end);
            return -1;
        }

        req.start = req.end = *channel;
        req.flags |= RSRCDBMGR_FLAG_RANGE;
    }

    if ((flags & DMA_ATTACH_PRIORITY_STRICT) || (flags & DMA_ATTACH_PRIORITY_ATLEAST)) {
        // Channel priority request
        // Check if the Priority is in the range
        if ((req.flags & RSRCDBMGR_FLAG_RANGE) && (priority < req.start)) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: priority %jd is out of range %jd:%jd!", __FUNCTION__,
                    priority, req.start, req.end);
            return -1;
        }

        // Priority Strict or at least?
        req.start = priority;
        req.end = (flags & DMA_ATTACH_PRIORITY_STRICT) ?
                    priority : (uint64_t) chan->channels - 1;
        req.flags |= RSRCDBMGR_FLAG_RANGE;
    }

    // DMAs have reversed priority, i.e., channel 0 has the highest priority.
    if (!(flags & DMA_ATTACH_PRIORITY_HIGHEST)) {
        req.flags |= RSRCDBMGR_FLAG_TOPDOWN;
    }

    if (rsrcdbmgr_attach(&req, 1) == -1) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: %s DMA channel rsrcdbmgr_attach failed: %s", __FUNCTION__,
                    chan->name, strerror(errno));
        return -1;
    }

    return req.start;
}

static int rsrcdb_attach_mem(const dma_channel_t* const chan)
{
#define NAME_SIZE  50
    char name[NAME_SIZE];
    rsrc_request_t  req = { 0 };

    rsrcdb_dma_to_rsrcdb_name(name, NAME_SIZE, chan->type, 0);

    req.length = chan->desc_num;
    req.start  = chan->grp_idx * 128;
    req.end    = req.start + 128 - 1;
    req.flags  = RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = name;

    // If resource request fails, the library will try to allocate DMA descriptor from system memory later
    if (rsrcdbmgr_attach(&req, 1) == -1) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: %s DMA chan %d grp %d descriptor rsrcdbmgr_attach failed: %s",
            __FUNCTION__, name, chan->chan_idx, chan->grp_idx, strerror(errno));
        return -1;
    }

    return req.start;
}

static int rsrcdb_detach_resources(const dma_channel_t* const chan)
{
#define NAME_SIZE  50
    char name[NAME_SIZE];
    rsrc_request_t  req = { 0 };

    rsrcdb_dma_to_rsrcdb_name(name, NAME_SIZE, chan->type, 1);

    // release DMA resource
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = name;
    rsrcdbmgr_detach(&req, 1);

    if (chan->desc_num > 0) {
        rsrcdb_dma_to_rsrcdb_name(name, NAME_SIZE, chan->type, 0);
        req.length = chan->desc_num;
        req.start  = chan->desc_idx;
        req.end    = chan->chan_idx + chan->desc_num - 1;
        req.flags  = RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = name;
        rsrcdbmgr_detach(&req, 1);
    }

    return EOK;
}

static int
dma_init(const char *options)
{
    unsigned hwi_off, tag_idx;
    hwi_tag *tag;
    int type;

    rcar_dma_slog2_init();

    hwi_off = hwi_find_device("r-car", 0);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, 0);
        if (tag) {
            rcar_soc = tag->hwversion.version;
        } else {
            rcar_dma_slogf(SLOG2_ERROR, "%s: Unable to query product_id from HW info\n", __func__);
            return (-EIO);
        }
    } else {
        rcar_dma_slogf(SLOG2_ERROR, "%s: Unable to find product version from HW info\n", __func__);
        return (-EIO);
    }

    hwi_off = hwi_find_device("dma,options", 0);
    if (hwi_off != HWI_NULL_OFF) {
        tag_idx = 0;
        while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
            if (strcmp("sysdmac_chans",  __hwi_find_string(tag->regname.regname)) == 0) {
                dmac_channels[DMA_SYS] = tag->regname.offset;
            } else if (strcmp("sysdmac_chans_per_grp",  __hwi_find_string(tag->regname.regname)) == 0) {
                dmac_channels_per_group[DMA_SYS] = tag->regname.offset;
            } else if (strcmp("rtdmac_chans",  __hwi_find_string(tag->regname.regname)) == 0) {
                dmac_channels[DMA_RT] = tag->regname.offset;
            } else if (strcmp("rtdmac_chans_per_grp",  __hwi_find_string(tag->regname.regname)) == 0) {
                dmac_channels_per_group[DMA_RT] = tag->regname.offset;
            } else if (strcmp("audiodmac_chans",  __hwi_find_string(tag->regname.regname)) == 0) {
                dmac_channels[DMA_AUDIO] = tag->regname.offset;
            } else if (strcmp("audiodmac_chans_per_grp",  __hwi_find_string(tag->regname.regname)) == 0) {
                dmac_channels_per_group[DMA_AUDIO] = tag->regname.offset;
            }
        }
    }

    for (type = 0; type < DMA_TYPE_NUM; type++) {
        if (dmac_channels[type] > 0) {
            rsrcdb_create_resources(type);
        }

#ifdef DEBUG
        printf("DMA: %s: %s-dmac channels %d  channel groups %d", __FUNCTION__,
                        dmac_type_names[type], dmac_channels[type], dmac_channels_per_group[type]);
#endif
    }

    return EOK;
}

static int query_hwinfo(dma_channel_t* const chan)
{
    unsigned hwi_off, tag_idx;
    hwi_tag *tag;
    uint32_t chan_in_grp_idx = chan->chan_idx_in_grp;

    hwi_off = hwi_find_device(chan->name, chan->grp_idx);
    if (hwi_off == HWI_NULL_OFF) {
        return ENOTSUP;
    }
    tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
    if (tag) {
        chan->pbase = chan->pbase_grp = tag->location.base;
        chan->size = chan->size_grp = tag->location.len;

        tag_idx = 1;
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, &tag_idx);
        if (tag) {
            chan->pbase_grp = tag->location.base;
            chan->size_grp = tag->location.len;
        }

        tag_idx = 0;
        while ((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
            if (strcmp(RCAR_HWI_DMA_REG,  __hwi_find_string(tag->regname.regname)) == 0) {
                chan->pbase += tag->regname.offset;
            } else if (strcmp(RCAR_HWI_DMA_SIZE,  __hwi_find_string(tag->regname.regname)) == 0) {
                chan->regs_size = tag->regname.offset;
            } else if (strcmp(RCAR_HWI_DMA_DESC,  __hwi_find_string(tag->regname.regname)) == 0) {
                chan->desc_offset = tag->regname.offset;
            }
        }

        chan->irq = hwitag_find_ivec(hwi_off, &chan_in_grp_idx);
    }

    if (!chan->pbase || !chan->pbase_grp || !chan->size || !chan->size_grp || !chan->regs_size) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: Failed! %s-dmac grp %d", __func__, chan->name, chan->grp_idx);
        return EINVAL;
    } else {
        return EOK;
    }
}

static void
dma_fini(void)
{
}

static void
dma_query_channel(void* const handle, dma_channel_query_t *chinfo)
{
    const dma_channel_t* const chan = handle;

    chinfo->chan_idx = chan->chan_idx;
    chinfo->irq      = chan->irq;
}

static int
dma_driver_info(dma_driver_info_t *info)
{
    if (rcar_soc == PRODUCT_ID_RCAR_UNKNOWN) {
        if (dma_init(NULL) != EOK) {
            errno = EINVAL;
            return -1;
        }
    }

    info->dma_version_major = DMALIB_VERSION_MAJOR;
    info->dma_version_minor = DMALIB_VERSION_MINOR;
    info->dma_rev           = DMALIB_REVISION;
    info->lib_rev           = 0;
    info->description       = "RCAR SYS DMAC Controller with fixed-reversed channel priority";
    info->num_channels      = dmac_channels[DMA_SYS];
    info->max_priority      = dmac_channels[DMA_SYS] - 1;

    return 0;
}

static int
dma_channel_info(const unsigned channel, dma_channel_info_t* const info)
{
    if (rcar_soc == PRODUCT_ID_RCAR_UNKNOWN) {
        if (dma_init(NULL) != EOK) {
            errno = EINVAL;
            return -1;
        }
    }

    if (channel > dmac_channels[DMA_SYS]) {
        errno = ECHRNG;
        return -1;
    }

    info->max_xfer_size         = 0xffffffff;
    info->xfer_unit_sizes       = 0x1; // 64, 32, 16, 8, 4, 2, 1 byte unit transfers
    info->max_src_fragments     = SYSDMAC_DESCRIPTORS_PER_GROUP;
    info->max_dst_fragments     = SYSDMAC_DESCRIPTORS_PER_GROUP;
    info->max_src_segments      = SYSDMAC_DESCRIPTORS_PER_GROUP;
    info->max_dst_segments      = SYSDMAC_DESCRIPTORS_PER_GROUP;
    info->caps                  = DMA_CAP_SRC_INCREMENT | DMA_CAP_SRC_DECREMENT | DMA_CAP_SRC_SEGMENTED | DMA_CAP_SRC_NO_INCREMENT |
                                  DMA_CAP_DST_INCREMENT | DMA_CAP_DST_DECREMENT | DMA_CAP_DST_SEGMENTED | DMA_CAP_DST_NO_INCREMENT |
                                  DMA_CAP_DEVICE_TO_MEMORY | DMA_CAP_MEMORY_TO_DEVICE | DMA_CAP_EVENT_ON_COMPLETE | DMA_CAP_BUFFER_CONTROL;
    info->mem_lower_limit       = 0;
    info->mem_upper_limit       = 0xffffffff;
    info->mem_nocross_boundary  = 0;

    return 0;
}

static char *dma_opts[] = {
#define SYSDMAC_OPT_VER       0
    "ver",          // optional, R-car version, e.g. "h2", "m2", "e2", "v2", "h3", "m3", "w2h"
#define SYSDMAC_OPT_DMATYPE   1
    "dma",          // dmac type, "sys" or "audio", default "sys"
#define SYSDMAC_OPT_DESCNUM   2
    "desc",         // number of internal descriptors required
#define SYSDMAC_OPT_DEBUG     3
    "debug",        // debug, 1=on, 0=off, default off
#define SYSDMAC_OPT_CAIE      4
    "cae_intr",   // Channel Address Error interrupt 1=enabled, 0=disabled, default=disabled
#define SYSDMAC_OPT_RANGE_MIN 5
    "range_min",   // The start of the requested channel range that the a channel to assigned from.
#define SYSDMAC_OPT_RANGE_MAX 6
    "range_max",   // The start of the requested channel range that the a channel to assigned from.
#if _NTO_VERSION >= 704
#define SYSDMAC_OPT_TMEM      7
    "typed_mem",    // typed mem name for buffer
#endif
    NULL
};

static int
sysdmac_parse_options(dma_channel_t* const chan, char *options)
{
    char   *value;
    int    opt;
    int    type;

#if _NTO_VERSION >= 704
    chan->typed_mem_fd = NOFD;
#endif
    chan->caie = 1;

    while (options && (*options != '\0')) {
        if ((opt = getsubopt(&options, dma_opts, &value)) == -1) {
            rcar_dma_slogf(SLOG2_ERROR, "%s:%d - getsubopt()failed - %s", __func__, __LINE__, strerror(errno));
            return EINVAL;
        }
        switch (opt) {
            case SYSDMAC_OPT_VER:
                dma_init(value);
                break;
            case SYSDMAC_OPT_DMATYPE:
                strncpy(chan->name, value, RCAR_DMA_TYPE_NAME_MAX_SIZE-1);
                break;
            case SYSDMAC_OPT_DESCNUM:
                chan->desc_num = (int) strtoul(value, NULL, 0);
                break;
            case SYSDMAC_OPT_DEBUG:
                chan->debug = (uint32_t) strtoul(value, NULL, 0);
                break;
            case SYSDMAC_OPT_RANGE_MIN:
                chan->range_min = (int16_t) strtoul(value, NULL, 0);
                break;
            case SYSDMAC_OPT_RANGE_MAX:
                chan->range_max = (int16_t) strtoul(value, NULL, 0);
                break;
#if _NTO_VERSION >= 704
            case SYSDMAC_OPT_TMEM:
                chan->typed_mem_fd = posix_typed_mem_open(value, O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
                if (chan->typed_mem_fd == NOFD) {
                    rcar_dma_slogf(SLOG2_ERROR, "Unable to find typed mem %s: %s", value, strerror(errno));
                    return errno;
                }
                break;
#endif
            case SYSDMAC_OPT_CAIE:
                chan->caie = (uint32_t) strtoul(value, NULL, 0);
                break;
            default:
                rcar_dma_slogf(SLOG2_ERROR, "%s: Invalid option %s", __func__, dma_opts[opt]);
                return EINVAL;
        }
    }

    if (rcar_soc == PRODUCT_ID_RCAR_UNKNOWN) {
        if (dma_init(NULL) != EOK) {
            return EINVAL;
        }
    }

    if ((dmac_channels[DMA_SYS] == 0) && (dmac_channels[DMA_AUDIO] == 0) && (dmac_channels[DMA_RT] == 0)) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: Invalid channel number", __func__);
        return EINVAL;
    }

    // a number of descriptors larger than 128 is invalid
    if (chan->desc_num > 128) {
        chan->desc_num = 0;
    }

    // S4, V4H doesn't support built-in descriptor ???
    if ((rcar_soc == PRODUCT_ID_RCAR_S4) || (rcar_soc == PRODUCT_ID_RCAR_V4H)) {
        chan->desc_num = 0;
    }
    chan->type = DMA_TYPE_NUM;
    for (type = 0; type < DMA_TYPE_NUM; type++) {
        if (strcmp(chan->name, dmac_type_names[type]) == 0) {
            chan->type = type;
            chan->channels = dmac_channels[type];
            chan->channels_per_group = dmac_channels_per_group[type];
            chan->channel_groups = chan->channels / chan->channels_per_group;
            chan->channel_groups += (chan->channels % chan->channels_per_group) ? 1 : 0;
            break;
        }
    }

    if (chan->type == DMA_TYPE_NUM) {
        rcar_dma_slogf(SLOG2_ERROR, "Invalid DMAC type %s", chan->name);
        return EINVAL;
    }

    if ((chan->range_min == -1) ^ (chan->range_max == -1)) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: Invalid channel range %d:%d", __FUNCTION__,
            chan->range_min, chan->range_max);
        return EINVAL;
    }

    /* Default values of Buffer Control */
    chan->buffer_mbu_default = 0x80;
    chan->buffer_mbu_max     = 512;
    chan->buffer_ulb_default = 0x400;
    chan->buffer_ulb_max     = 1024;

#if _NTO_VERSION >= 704
    /* SYS-DMAC can only support 32-bit addresses when IPMMU is enabled, so
     * force the use of below4G type memory if an smmu object was provided
     * and no type memory was specified in the options.
     */
    if (chan->sop && (chan->typed_mem_fd == NOFD))
    {
        chan->typed_mem_fd = posix_typed_mem_open("sysram&below4G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
        if (chan->typed_mem_fd == NOFD) {
            rcar_dma_slogf(SLOG2_ERROR, "Failed to open typed mem sysram&below4G - %s", strerror(errno));
            return errno;
        }
    }
#endif

    return EOK;
}

static int sysdmac_map_registers(dma_channel_t* const chan)
{
    uint32_t mem_map_idx;
    uint32_t reg_offset = 0;
    int ret;

    chan->pbase = 0;
    chan->pbase_grp = 0;

    ret = query_hwinfo(chan);
    if (ret != EOK) {
        return ret;
    }

#ifdef DEBUG
    printf("DMA: type %d chan %d chan_idx_in_grp %d grp_idx %d",
        chan->type, chan->chan_idx, chan->chan_idx_in_grp, chan->grp_idx);
    printf("DMA: pbase 0x%lx pbase_grp 0x%lx regs_size 0x%x size 0x%x size_grp 0x%x desc_offset 0x%x irq %d",
        chan->pbase, chan->pbase_grp, chan->regs_size,
        chan->size, chan->size_grp, chan->desc_offset, chan->irq);
#endif

    // check whether the memory for the DMA channel group is already mapped in.
    for (mem_map_idx = 0; mem_map_idx < RCAR_MAX_DMAC_MMAP_GROUPS; mem_map_idx++) {
        if (sysdmac[mem_map_idx].paddr == chan->pbase) {
            atomic_add(&sysdmac[mem_map_idx].num, 1);
            chan->vbase = sysdmac[mem_map_idx].vaddr;
            chan->vbase_grp = sysdmac[mem_map_idx].vaddr_grp;
            break;
        }
    }

    if (chan->vbase == 0) {
        chan->vbase = mmap_device_io(chan->size, chan->pbase);
        if (chan->vbase == (uintptr_t)MAP_FAILED) {
            return errno;
        }

        if (chan->pbase_grp != chan->pbase) {
            chan->vbase_grp = mmap_device_io(chan->size_grp, chan->pbase_grp);
            if (chan->vbase_grp == (uintptr_t)MAP_FAILED) {
                return errno;
            }
        } else {
            chan->vbase_grp = chan->vbase;
        }
        for (mem_map_idx = 0; mem_map_idx < RCAR_MAX_DMAC_MMAP_GROUPS; mem_map_idx++) {
            if (sysdmac[mem_map_idx].num == 0) {
                sysdmac[mem_map_idx].paddr = chan->pbase;
                sysdmac[mem_map_idx].vaddr = chan->vbase;
                sysdmac[mem_map_idx].paddr_grp = chan->pbase_grp;
                sysdmac[mem_map_idx].vaddr_grp = chan->vbase_grp;
                sysdmac[mem_map_idx].num   = 1;
                break;
            }
        }
    }

    if (chan->desc_num > 0) {
        chan->desc.vaddr = (void *)chan->vbase_grp;
        chan->desc.paddr = chan->pbase_grp;
    }

    /* Calculate register offset for the requested DMA channel */
    reg_offset = chan->chan_idx_in_grp * chan->regs_size;
    chan->regs = chan->vbase + reg_offset;

#if _NTO_VERSION >= 704
    if (chan->sop) {
        /* Each channel is seen as a separate device by the IPMMU, so do a
         * smmu_device_add_mmio() on the specific channel and not the full controller
         */
        if (smmu_device_add_mmio( chan->sop, chan->pbase + reg_offset, chan->regs_size ) == -1) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: smmu_device_add_mmio() failed for 0x%"PRIxPTR":%d - %s\n", __func__, chan->pbase + reg_offset, chan->regs_size, strerror(errno));

            const unsigned prev_value = atomic_sub_value(&sysdmac[mem_map_idx].num, 1);
            if (prev_value == 1) {
                munmap_device_io(chan->vbase, chan->size);
                if (chan->pbase_grp != chan->pbase) {
                    munmap_device_io(chan->vbase_grp, chan->size_grp);
                }
                sysdmac[mem_map_idx].num = sysdmac[mem_map_idx].paddr = sysdmac[mem_map_idx].vaddr = sysdmac[mem_map_idx].paddr_grp = sysdmac[mem_map_idx].vaddr_grp = 0;
            }
            return errno;
        }
    }

#endif
    return EOK;
}

static void sysdmac_unmap_registers(dma_channel_t* const chan)
{
    uint32_t mem_map_idx;

    // unmap memory if not in use for other DMA channels in same group
    for (mem_map_idx = 0; mem_map_idx < RCAR_MAX_DMAC_MMAP_GROUPS; mem_map_idx++) {
        if (sysdmac[mem_map_idx].paddr == chan->pbase) {
            const unsigned prev_value = atomic_sub_value(&sysdmac[mem_map_idx].num, 1);
            if (prev_value == 1) {
                munmap_device_io(sysdmac[mem_map_idx].vaddr, chan->size);
                sysdmac[mem_map_idx].num = sysdmac[mem_map_idx].paddr = sysdmac[mem_map_idx].vaddr = 0;

                if (chan->pbase_grp != chan->pbase) {
                    munmap_device_io(sysdmac[mem_map_idx].vaddr_grp, chan->size_grp);
                }
                sysdmac[mem_map_idx].paddr_grp = sysdmac[mem_map_idx].vaddr_grp = 0;
            }
#if _NTO_VERSION >= 704
            if (chan->sop) {
                uint32_t reg_offset = (chan->chan_idx % chan->channels_per_group) * chan->regs_size;
                smmu_device_add_mmio( NULL, chan->pbase + reg_offset, chan->regs_size );
            }
#endif
            chan->vbase = 0;
            chan->vbase_grp = 0;
        }
    }
}

static void sysdmac_dump_registers(dma_channel_t* const chan)
{
    rcar_dma_slogf(SLOG2_INFO, "%s DMA chan %d reg dump: DMADPBASE=%x DMAFIXDPBASE=%x DMACHCRB=%x DMACHCR=%x DMARS=%x DMADPCR=%x",
        chan->name, chan->chan_idx,
        in32(chan->regs + RCAR_DMAC_DMADPBASE), in32(chan->regs + RCAR_DMAC_DMAFIXDPBASE),
        in32(chan->regs + RCAR_DMAC_DMACHCRB), in32(chan->regs + RCAR_DMAC_DMACHCR),
        in32(chan->regs + RCAR_DMAC_DMARS), in32(chan->regs + RCAR_DMAC_DMADPCR));
    rcar_dma_slogf(SLOG2_INFO, "%s DMA chan %d reg dump: DMASAR=%x DMAFIXSAR=%x DMADAR=%x DMAFIXDAR=%x DMATCR=%x DMATCRB=%x",
        chan->name, chan->chan_idx,
        in32(chan->regs + RCAR_DMAC_DMASAR), in32(chan->regs + RCAR_DMAC_DMAFIXSAR),
        in32(chan->regs + RCAR_DMAC_DMADAR), in32(chan->regs + RCAR_DMAC_DMAFIXDAR),
        in32(chan->regs + RCAR_DMAC_DMATCR), in32(chan->regs + RCAR_DMAC_DMATCRB));
}

static void *
#if _NTO_VERSION >= 704
dma_channel_attach_smmu(const char* const options, const struct sigevent* const event, unsigned* const channel,
                const int priority, const dma_attach_flags flags, struct smmu_object* const sop)
#else
dma_channel_attach(const char* const options, const struct sigevent* const event, unsigned* const channel,
                const int priority, const dma_attach_flags flags)
#endif
{
    dma_channel_t   *chan;
    char            *optstr = NULL;
    int             ret;
    int             chan_idx;

    if (options) {
        optstr = strdup(options);
    } else {
        optstr = strdup("dma=sys");
    }

    if (optstr == NULL) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: strdup failed", __FUNCTION__);
        return NULL;
    }

    chan = calloc(1, sizeof(*chan));
    if (chan == NULL) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: calloc failed", __FUNCTION__);
        goto fail0;
    }

#if _NTO_VERSION >= 704
    chan->sop = sop;
#endif

    chan->range_min = -1;
    chan->range_max = -1;
    if ((errno = sysdmac_parse_options(chan, optstr)) != EOK) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: parse options failed", __FUNCTION__);
        goto fail1;
    }

    // DMA type is available on this SoC?
    if (chan->channels == 0 || chan->channels_per_group == 0) {
        if (chan->debug) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: channels/chan per grp (%d/%d) are not available for %s-dmac",
                __FUNCTION__, chan->channels, chan->channels_per_group, dmac_type_names[chan->type]);
        }
        goto fail1;
    }

    /* Get a channel assigned from the resources database */
    chan_idx = rsrcdb_attach_channel(chan, channel, priority, flags);
    if (chan_idx < 0) {
        goto fail1;
    }

    if (channel) {
        *channel = chan_idx;
    }

    chan->chan_idx = chan_idx;
    chan->grp_idx = chan_idx / chan->channels_per_group;
    chan->chan_idx_in_grp = chan_idx % chan->channels_per_group;

    // Request descriptor memory if necessary
    if ((chan->desc_num > 0) && (chan->desc_num <= 128)) {
        ret = rsrcdb_attach_mem(chan);
        if (ret < 0) {
            chan->desc_num = 0;
        } else {
            chan->desc_idx = ret;
        }
    }

    // map in DMA registers for DMA channel group, including descriptor memory
    ret = sysdmac_map_registers(chan);
    if (ret != EOK) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: %s DMA chan %d grp %d register map failed: %s", __FUNCTION__,
            chan->name, chan->chan_idx, chan->grp_idx, strerror(ret));
        goto fail2;
    }

    // Disable channel
    out32(chan->regs + RCAR_DMAC_DMACHCR, 0);

    chan->iid = -1;

    if ((flags & (DMA_ATTACH_EVENT_ON_COMPLETE | DMA_ATTACH_EVENT_PER_SEGMENT)) && (event != NULL)) {
        chan->iid = InterruptAttachEvent(chan->irq, event, _NTO_INTR_FLAGS_TRK_MSK);

        if (chan->iid == -1) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: %s DMA chan %d grp %d interrupt attach failed: %s", __FUNCTION__,
                chan->name, chan->chan_idx, chan->grp_idx, strerror(errno));
            goto fail3;
        }
    }

    chan->aflags = flags;

    return chan;

fail3:
    sysdmac_unmap_registers(chan);
fail2:
    rsrcdb_detach_resources(chan);

fail1:
    free(chan);

fail0:
    if (optstr) {
        free(optstr);
    }
    return (NULL);
}

#if _NTO_VERSION >= 704
static void *
dma_channel_attach(const char* const options, const struct sigevent* const event, unsigned* const channel,
            const int priority, const dma_attach_flags flags)
{
    return (dma_channel_attach_smmu(options, event, channel, priority, flags, NULL));
}
#endif

static void
dma_channel_release(void* const handle)
{
    dma_channel_t* const chan = handle;

    // Disable the channel
    out32(chan->regs + RCAR_DMAC_DMACHCR, 0);

    // release descriptor memory
    if (chan->desc.len > 0) {
        dma_free_buffer(handle, &chan->desc);
    }

    // Detach interrupt
    if (chan->iid != -1) {
        InterruptDetach(chan->iid);
    }

    // release DMA resource
    rsrcdb_detach_resources(chan);

    // unmap registers
    sysdmac_unmap_registers(chan);

#if _NTO_VERSION >= 704
    if (chan->typed_mem_fd != NOFD) {
        close(chan->typed_mem_fd);
        chan->typed_mem_fd = NOFD;
    }
#endif

    free(handle);
}

static int
dma_setup_descriptor(void* const handle, const dma_transfer_t* const tinfo, uint32_t chcr)
{
    dma_channel_t* const chan = handle;
    sysdmac_desc_t  *desc;
    off64_t         dpbase;     // descriptor physical address
    int             segs, sgi, desc_idx_in_grp;
    uint32_t        fixsar = 0, fixdar = 0;

    // We only support source and destination have same amount of segments
    if ((tinfo->src_fragments <= 0) && (tinfo->dst_fragments <= 0)) {
        return -1;
    }

    segs = (tinfo->src_flags & DMA_ADDR_FLAG_SEGMENTED) ? tinfo->src_fragments : tinfo->dst_fragments;

    if (chan->desc_num == 0) {  // no internal descriptor memory
        if (segs > SYSDMAC_DESCRIPTORS_PER_GROUP) {
            rcar_dma_slogf(SLOG2_ERROR, "SYSDMAC: Unsupported segment value %d", segs);
            return (-1);
        }
        if (chan->desc.len == 0) {
            if (dma_alloc_buffer( handle,
                                  &chan->desc,
                                  segs * sizeof(sysdmac_desc_t),
                                  DMA_BUF_FLAG_NOCACHE) != 0) {
                rcar_dma_slogf(SLOG2_ERROR, "SYSDMAC: Unable to allocate DMA descriptor buffer");
                rcar_dma_slogf(SLOG2_ERROR, "%s: %s DMA chan %d grp %d buf alloc failed: %s", __FUNCTION__,
                    chan->name, chan->chan_idx, chan->grp_idx, strerror(errno));
                return (-1);
            }
        }
        dpbase = chan->desc.paddr;
        desc   = (sysdmac_desc_t *)chan->desc.vaddr;
    } else {
        if (segs > chan->desc_num) {
            rcar_dma_slogf(SLOG2_ERROR, "SYSDMAC: Segment number %d larger than descriptor number %d\n", segs, chan->desc_num);
            return (-1);
        }
        desc_idx_in_grp = chan->desc_idx % 128;
        dpbase = chan->desc.paddr + chan->desc_offset + desc_idx_in_grp * sizeof(sysdmac_desc_t);
        desc   = (sysdmac_desc_t *)(chan->desc.vaddr + chan->desc_offset + desc_idx_in_grp * sizeof(sysdmac_desc_t));
    }
    // use Built-in memory or external memory
    chan->desc_seg_num = segs;

    out32(chan->regs + RCAR_DMAC_DMADPBASE, dpbase | 1ULL); // use Built-in memory or external memory
    out32(chan->regs + RCAR_DMAC_DMAFIXDPBASE, dpbase >> 32);
    out32(chan->regs + RCAR_DMAC_DMACHCRB, RCAR_DMACHCRB_DRST);  // Reset descriptor
    in32(chan->regs +  RCAR_DMAC_DMACHCRB);
    out32(chan->regs + RCAR_DMAC_DMACHCRB, (segs - 1) << 24);

    if ( (rcar_soc == PRODUCT_ID_RCAR_S4) || (rcar_soc == PRODUCT_ID_RCAR_V4H) ) {
        out32(chan->regs + RCAR_DMAC_DMADPCR, (segs - 1) & 0xF00);
    }

    fixsar = tinfo->src_addrs[0].paddr >> 32;
    fixdar = tinfo->dst_addrs[0].paddr >> 32;
#if _NTO_VERSION >= 704
    if (chan->sop) {
        if (fixsar || fixdar) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: above4G not supported when smmu is enabled", __FUNCTION__);
            return (-1);
        }
    }
#endif
    out32(chan->regs + RCAR_DMAC_DMAFIXSAR, fixsar);
    out32(chan->regs + RCAR_DMAC_DMAFIXDAR, fixdar);

    for (sgi = 0; sgi < segs; sgi++, desc++) {
        if ((tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT) == 0) {
            if ((tinfo->src_addrs[sgi].paddr >> 32) != fixsar) {
                rcar_dma_slogf(SLOG2_ERROR, "%s: inconsistent MSB in source descriptors", __FUNCTION__);
                return (-1);
            }
        }
        desc->sar  = (tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT) ? tinfo->src_addrs[0].paddr : tinfo->src_addrs[sgi].paddr;
        if ((tinfo->dst_flags & DMA_ADDR_FLAG_NO_INCREMENT) == 0) {
            if ((tinfo->dst_addrs[sgi].paddr >> 32) != fixdar) {
                rcar_dma_slogf(SLOG2_ERROR, "%s: inconsistent MSB in destination descriptors", __FUNCTION__);
                return (-1);
            }
        }
        desc->dar  = (tinfo->dst_flags & DMA_ADDR_FLAG_NO_INCREMENT) ? tinfo->dst_addrs[0].paddr : tinfo->dst_addrs[sgi].paddr;
        /* Determine the number of transfers based on the buffer length used on the side that uses DMA memory buffer(s),
         * that side is the one that does not use the DMA_ADDR_FLAG_NO_INCREMENT flag */
        desc->tcr  = (tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT) ? tinfo->dst_addrs[sgi].len : tinfo->src_addrs[sgi].len;
        desc->tcr /= tinfo->xfer_unit_size;

        if (desc->tcr > RCAR_DMA_XFER_COUNT_MAX) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: Transfer count %d is not exceeded 16,777,215", __FUNCTION__, desc->tcr);
            return -1;
        }

        /* FIXME: Need delay sometime here so that descriptor setting is updated down to memory */
        if (rcar_soc == PRODUCT_ID_RCAR_S4) {
            nanospin_ns(1000);
        }
    }

    /* Enable Channel Address error interrupt */
    chcr |= RCAR_DMACHCR_RPT | RCAR_DMACHCR_DPB | (chan->caie ? RCAR_DMACHCR_CAIE : 0);

    if (chan->aflags & DMA_ATTACH_EVENT_PER_SEGMENT) {
        chcr |= RCAR_DMACHCR_DSIE;
    }
    if (chan->aflags & DMA_ATTACH_EVENT_ON_COMPLETE) {
        chcr |= RCAR_DMACHCR_IE;
    }

    out32(chan->regs + RCAR_DMAC_DMACHCR, chcr);

    if (chan->debug) {
        sysdmac_dump_registers(chan);
    }

    return 0;
}

static int
dma_setup_xfer(void* const handle, const dma_transfer_t* const tinfo)
{
    dma_channel_t* const chan = handle;
    uint32_t        chcr = 0, fixsar = 0, fixdar = 0;

    switch (tinfo->xfer_unit_size) {
        case 2:
            chcr |= RCAR_DMACHCR_TS_WORD;
            break;
        case 4:
            chcr |= RCAR_DMACHCR_TS_LONG;
            break;
        case 8:
            chcr |= RCAR_DMACHCR_TS_8;
            break;
        case 16:
            chcr |= RCAR_DMACHCR_TS_16;
            break;
        case 32:
            chcr |= RCAR_DMACHCR_TS_32;
            break;
        case 64:
            chcr |= RCAR_DMACHCR_TS_64;
            break;
        default:
            chcr |= RCAR_DMACHCR_TS_BYTE;
            break;
    }

    if (tinfo->src_flags & DMA_ADDR_FLAG_DECREMENT) {
        chcr |= RCAR_DMACHCR_SM_DECR;
    } else if (!(tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT)) {
        chcr |= RCAR_DMACHCR_SM_INCR;
    }

    if (tinfo->dst_flags & DMA_ADDR_FLAG_DECREMENT) {
        chcr |= RCAR_DMACHCR_DM_DECR;
    }
    if (!(tinfo->dst_flags & DMA_ADDR_FLAG_NO_INCREMENT)) {
        chcr |= RCAR_DMACHCR_DM_INCR;
    }

    // Hardware triggered DMA transfer
    if ((tinfo->src_flags | tinfo->dst_flags) & DMA_ADDR_FLAG_DEVICE) {
        out16(chan->regs + RCAR_DMAC_DMARS, tinfo->req_id);
        chcr |= RCAR_DMACHCR_RS_ONCHIP;
    } else {
        chcr |= RCAR_DMACHCR_RS_AUTO;
    }

    // xfer_complete needs this
    chan->mflags = tinfo->mode_flags;
    // dma_bytes_left needs xfer_unit_size
    chan->xfer_count = tinfo->xfer_bytes / tinfo->xfer_unit_size;
    if (chan->xfer_count > RCAR_DMA_XFER_COUNT_MAX) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: Transfer count %d is not exceeded 16,777,215", __FUNCTION__, chan->xfer_count);
        return -1;
    }
    chan->xfer_unit_size = tinfo->xfer_unit_size;

    // set buffer control
    if (tinfo->mode_flags & DMA_MODE_FLAG_BUFFER_CONTROL) {
        uint32_t bufcr = 0;
        if (tinfo->buffer_mbu > chan->buffer_mbu_max) {
            bufcr |= chan->buffer_mbu_max << 16;
        } else if (tinfo->buffer_mbu) {
            bufcr |= tinfo->buffer_mbu << 16;
        } else {
            bufcr |= chan->buffer_mbu_default << 16;
        }

        if (tinfo->buffer_ulb > chan->buffer_ulb_max) {
            bufcr |= chan->buffer_ulb_max;
        } else if (tinfo->buffer_ulb) {
            bufcr |= tinfo->buffer_ulb;
        } else {
            bufcr |= chan->buffer_ulb_default;
        }

        out32(chan->regs + RCAR_DMAC_DMABUFCR,  bufcr);
    }

    if ((tinfo->src_flags | tinfo->dst_flags) & DMA_ADDR_FLAG_SEGMENTED) {
        if (tinfo->mode_flags & DMA_MODE_FLAG_BURST) {
            chcr |= RCAR_DMACHCR_DPM_RPT_INF;
    } else if (tinfo->mode_flags & DMA_MODE_FLAG_REPEAT) {
            chcr |= RCAR_DMACHCR_DPM_RPT;
        } else {
            chcr |= RCAR_DMACHCR_DPM_NORMAL;
        }

        return dma_setup_descriptor(handle, tinfo, chcr);
    }

    // segmented mode not in use
    chan->desc_seg_num = 0;

    if (chan->aflags & DMA_ATTACH_EVENT_ON_COMPLETE) {
        chcr |= RCAR_DMACHCR_IE | (chan->caie ? RCAR_DMACHCR_CAIE : 0);
    }

    out32(chan->regs + RCAR_DMAC_DMACHCR, chcr);

    // Single DMA buffer
    fixsar = tinfo->src_addrs[0].paddr >> 32;
    fixdar = tinfo->dst_addrs[0].paddr >> 32;
#if _NTO_VERSION >= 704
    if (chan->sop) {
        if (fixsar || fixdar) {
            rcar_dma_slogf(SLOG2_ERROR, "%s: above4G not supported when smmu is enabled", __FUNCTION__);
            return (-1);
        }
    }
#endif
    out32(chan->regs + RCAR_DMAC_DMASAR, tinfo->src_addrs[0].paddr);
    out32(chan->regs + RCAR_DMAC_DMAFIXSAR, fixsar);
    out32(chan->regs + RCAR_DMAC_DMADAR, tinfo->dst_addrs[0].paddr);
    out32(chan->regs + RCAR_DMAC_DMAFIXDAR, fixdar);
    out32(chan->regs + RCAR_DMAC_DMATCR, chan->xfer_count);

    if (chan->debug) {
        sysdmac_dump_registers(chan);
    }

    return 0;
}

static int
dma_start(void* const handle)
{
    dma_channel_t* const chan = handle;
    uint32_t        chcr;

    chcr = in32(chan->regs + RCAR_DMAC_DMACHCR) & ~(RCAR_DMACHCR_TE|RCAR_DMACHCR_DSE|RCAR_DMACHCR_CAE);

    out32(chan->regs + RCAR_DMAC_DMACHCR, chcr | RCAR_DMACHCR_DE);
    if (chan->debug) {
        sysdmac_dump_registers(chan);
    }

    return 0;
}

static int
dma_alloc_buffer(void* const handle, dma_addr_t* const addr, const unsigned size, const unsigned flags)
{
    const dma_channel_t* const chan = handle;
    int     prot_flags = PROT_READ | PROT_WRITE;
    int     map_flags  = MAP_PHYS | MAP_ANON;

    if (flags & DMA_BUF_FLAG_NOCACHE) {
        prot_flags |= PROT_NOCACHE;
    }

    if (flags & DMA_BUF_FLAG_SHARED) {
        map_flags |= MAP_SHARED;
    } else {
        map_flags |= MAP_PRIVATE;
    }
#if _NTO_VERSION >= 704
    if (chan && (chan->typed_mem_fd != NOFD)) {
        map_flags = MAP_SHARED;
    }
    addr->vaddr = mmap(NULL, size, prot_flags, map_flags, chan->typed_mem_fd, 0);
#else
    (void) chan;
    addr->vaddr = mmap(NULL, size, prot_flags, map_flags, NOFD, 0);
#endif
    if (addr->vaddr == MAP_FAILED) {
        rcar_dma_slogf(SLOG2_ERROR, "%s failed: %s", __FUNCTION__, strerror(errno));
        return errno;
    }

    mem_offset64(addr->vaddr, NOFD, 1, &addr->paddr, 0);
    addr->len = size;

#if _NTO_VERSION >= 704
    /* If the driver allocs the buffer before attaching to the channel
     * then it will have to add the mapping to the smmu object itself
     */
    if (chan && chan->sop) {
        struct smmu_map_entry entry;

        entry.phys = (uintptr64_t)addr->paddr;
        entry.len  = addr->len;
        smmu_mapping_add (chan->sop, SMF_READ|SMF_WRITE, 0, 1, &entry, 0);
    }
#endif

    return 0;
}

static void
dma_free_buffer(void* const handle, dma_addr_t *addr)
{
    dma_channel_t* const chan = handle;

    if (addr->len) {
        munmap(addr->vaddr, addr->len);
#if _NTO_VERSION >= 704
        if (chan && chan->sop) {
            struct smmu_map_entry entry;

            entry.phys = (uintptr64_t)addr->paddr;
            entry.len  = addr->len;
            smmu_mapping_add (chan->sop, SMF_NONE, 0, 1, &entry, 0);
        }
#else
    (void) chan;
#endif
    }
    addr->vaddr = NULL;
    addr->len   = 0;
}

static int
dma_abort(void* const handle)
{
    dma_channel_t* const chan = handle;
    const int timeout = 1024; /* 1024 * 1 microseconds = ~1 miliseconds */
    int i = 0;

    /* Recover default setting */
    if (chan->mflags & DMA_MODE_FLAG_BUFFER_CONTROL) {
        out32(chan->regs + RCAR_DMAC_DMABUFCR,  chan->buffer_mbu_default << 16 | chan->buffer_ulb_default);
    }

    if ( (in32(chan->regs + RCAR_DMAC_DMACHCR) & RCAR_DMACHCR_CAE) ) {
        /* V3U/S4/V4H DMA channel clear has a Security vulnerability and this function is disabled? */
        // Clear the DMA channel before proceeding
        // Since you can only write a 1 to each bit in the clear register
        // There isn't worry about a race condition with another channel that may be a
        // part of this global register.
        if (IS_RCAR_GEN4(rcar_soc)) {
            out32(chan->regs + RCAR_DMAC_CHCLR, 1);
        } else {
            out32(chan->vbase_grp + RCAR_DMAC_DMACHCLR, 1 << chan->chan_idx_in_grp);
        }
    } else {
        out32(chan->regs + RCAR_DMAC_DMACHCR,
                in32(chan->regs + RCAR_DMAC_DMACHCR) & ~(RCAR_DMACHCR_DE | RCAR_DMACHCR_DSIE | RCAR_DMACHCR_IE | RCAR_DMACHCR_CAIE | RCAR_DMACHCR_DSE | RCAR_DMACHCR_TE));
    }

    /* Even though we set the RCAR_DMACHCR_DE bit to 0, it will read as 1 until
     * the current transfer is complete; wait until the current transfer is complete
     * by checking that the RCAR_DMACHCR_DE bit reads 0 */
    for (i = 0; i < timeout; i++) {
        if ( !(in32(chan->regs + RCAR_DMAC_DMACHCR) & RCAR_DMACHCR_DE) ) {
            break;
        }

        nanospin_ns(100000); /* 100 microseconds */
    }

    if (chan->iid != -1) {
        InterruptUnmask(chan->irq, chan->iid);
    }

    if (chan->debug) {
        sysdmac_dump_registers(chan);
    }

    if (i == timeout) {
        rcar_dma_slogf(SLOG2_ERROR, "%s: %s DMA chan %d: wait for DE clear timed out",
            __FUNCTION__, chan->name, chan->chan_idx);
        return ETIMEDOUT;
    } else if ((i > 0) && chan->debug) {
        rcar_dma_slogf(SLOG2_INFO, "%s: %s DMA chan %d: waited %d ms for DE clear",
            __FUNCTION__, chan->name, chan->chan_idx, i/10);
    }

    return 0;
}

static unsigned
dma_bytes_left(void* const handle)
{
    dma_channel_t* const chan = handle;

    /* assign xfer_cnt to current TCR value for current segment */
    uint32_t xfer_cnt = in32(chan->regs + RCAR_DMAC_DMATCR);

    /* if segmented transfer is used, add the TCR values of all following segments */
    if (chan->desc_seg_num) {
        uint32_t next_seg = (in32(chan->regs + RCAR_DMAC_DMACHCRB) & 0x00FF0000) >> 16;
        /* if next_seg is 0, this was the last segment, nothing to add */
        if (next_seg) {
            const sysdmac_desc_t *desc;
            if (chan->desc_num == 0) {
                desc   = chan->desc.vaddr;
            } else {
                const uint32_t desc_idx_in_grp = chan->desc_idx % 128;
                desc   = (chan->desc.vaddr + chan->desc_offset +
                                                desc_idx_in_grp * sizeof(sysdmac_desc_t));
            }
            while (next_seg < chan->desc_seg_num) {
                xfer_cnt += desc[next_seg++].tcr;
            }
        }
    }

    return xfer_cnt * chan->xfer_unit_size;
}

static int
dma_xfer_complete(void* const handle)
{
    uint32_t status = 0, ret = EOK;
    dma_channel_t* const chan = handle;

    status = in32(chan->regs + RCAR_DMAC_DMACHCR);

    /* Return the error status code as an indication of whether an error occurred or not */
    ret = (status & RCAR_DMACHCR_CAE) ? (status & RCAR_DMACHCR_CAE) : EOK;

    /* Abort the transfer if we are not in repeat mode or if we got an error */
    if (!(chan->mflags & (DMA_MODE_FLAG_REPEAT | DMA_MODE_FLAG_BURST)) || (ret != EOK)) {
        dma_abort(handle);
        return (ret);
    }

    /* Recover default setting */
    if (chan->mflags & DMA_MODE_FLAG_BUFFER_CONTROL) {
        out32(chan->regs + RCAR_DMAC_DMABUFCR,  chan->buffer_mbu_default << 16 | chan->buffer_ulb_default);
    }

    out32(chan->regs + RCAR_DMAC_DMACHCR, status & ~(RCAR_DMACHCR_CAE | RCAR_DMACHCR_DSE | RCAR_DMACHCR_TE));

    if (chan->iid != -1) {
        InterruptUnmask(chan->irq, chan->iid);
    }

    return (ret);
}

static int
dma_check_xfer_complete(void* const handle)
{
    dma_channel_t * const chan = handle;

    return  (in32(chan->regs + RCAR_DMAC_DMACHCR) & RCAR_DMACHCR_TE);
}

int
get_dmafuncs(dma_functions_t *functable, int tabsize)
{
    DMA_ADD_FUNC(functable, init, dma_init, tabsize);
    DMA_ADD_FUNC(functable, fini, dma_fini, tabsize);
    DMA_ADD_FUNC(functable, driver_info, dma_driver_info, tabsize);
    DMA_ADD_FUNC(functable, channel_info, dma_channel_info, tabsize);
    DMA_ADD_FUNC(functable, channel_attach, dma_channel_attach, tabsize);
#if _NTO_VERSION >= 704
    DMA_ADD_FUNC(functable, channel_attach_smmu, dma_channel_attach_smmu, tabsize);
#endif
    DMA_ADD_FUNC(functable, channel_release, dma_channel_release, tabsize);
    DMA_ADD_FUNC(functable, setup_xfer, dma_setup_xfer, tabsize);
    DMA_ADD_FUNC(functable, xfer_start, dma_start, tabsize);
    DMA_ADD_FUNC(functable, xfer_abort, dma_abort, tabsize);
    DMA_ADD_FUNC(functable, bytes_left, dma_bytes_left, tabsize);
    DMA_ADD_FUNC(functable, xfer_complete, dma_xfer_complete, tabsize);
    DMA_ADD_FUNC(functable, alloc_buffer, dma_alloc_buffer, tabsize);
    DMA_ADD_FUNC(functable, free_buffer, dma_free_buffer, tabsize);
    DMA_ADD_FUNC(functable, query_channel, dma_query_channel, tabsize);
    DMA_ADD_FUNC(functable, check_xfer_complete, dma_check_xfer_complete, tabsize);

    return 0;
}

