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
#define RCAR_SYSDMAC_SIZE 0x3000
#define RCAR_SYSDMAC_DESC (0xA000 - RCAR_SYSDMAC_REGS)
#endif
#include "sysdmac.h"

#define RCAR_DMA_XFER_COUNT_MAX      16777215

/* SYSDMAC registers, offset regs */
#define SYSDMAC_DMASAR              0x00
#define SYSDMAC_DMADAR              0x04
#define SYSDMAC_DMATCR              0x08
#define SYSDMAC_DMACHCR             0x0C
#define SYSDMAC_DMAFIXSAR           0x10
#define SYSDMAC_DMAFIXDAR           0x14
#define SYSDMAC_DMATCRB             0x18
#define SYSDMAC_DMACHCRB            0x1C
#define SYSDMAC_DMASART             0x20
#define SYSDMAC_DMADART             0x24
#define SYSDMAC_DMATSR              0x28
#define SYSDMAC_DMACHCRT            0x2C
#define SYSDMAC_DMATSRB             0x38
#define SYSDMAC_DMARS               0x40
#define SYSDMAC_DMABUFCR            0x48
#define SYSDMAC_DMADPBASE           0x50
#define SYSDMAC_DMADPCR             0x54
#define SYSDMAC_DMAFIXDPBASE        0x60

/* Contents of the CHCR register */
#define SYSDMAC_CHCR_CAE            0x80000000
#define SYSDMAC_CHCR_CAIE           0x40000000
#define SYSDMAC_CHCR_DPM            0x30000000
#define SYSDMAC_CHCR_DPM_DISABLE    0x00000000
#define SYSDMAC_CHCR_DPM_NORMAL     0x10000000
#define SYSDMAC_CHCR_DPM_RPT        0x20000000
#define SYSDMAC_CHCR_DPM_RPT_INF    0x30000000
#define SYSDMAC_CHCR_RPT            0x0E000000
#define SYSDMAC_CHCR_RPT_S          0x08000000
#define SYSDMAC_CHCR_RPT_D          0x04000000
#define SYSDMAC_CHCR_RPT_C          0x02000000
#define SYSDMAC_CHCR_DPB            0x00400000
#define SYSDMAC_CHCR_TS2            0x00100000
#define SYSDMAC_CHCR_DSE            0x00080000
#define SYSDMAC_CHCR_DSIE           0x00040000
#define SYSDMAC_CHCR_DM             0x0000C000
#define SYSDMAC_CHCR_DM_FIXED       0x00000000
#define SYSDMAC_CHCR_DM_INCR        0x00004000
#define SYSDMAC_CHCR_DM_DECR        0x00008000
#define SYSDMAC_CHCR_SM             0x00003000
#define SYSDMAC_CHCR_SM_FIXED       0x00000000
#define SYSDMAC_CHCR_SM_INCR        0x00001000
#define SYSDMAC_CHCR_SM_DECR        0x00002000
#define SYSDMAC_CHCR_RS             0x00000F00
#define SYSDMAC_CHCR_RS_AUTO        0x00000400
#define SYSDMAC_CHCR_RS_ONCHIP      0x00000800
#define SYSDMAC_CHCR_TS             0x00000018
#define SYSDMAC_CHCR_TS_BYTE        0x00000000
#define SYSDMAC_CHCR_TS_WORD        0x00000008
#define SYSDMAC_CHCR_TS_LONG        0x00000010
#define SYSDMAC_CHCR_TS_8           0x00100018
#define SYSDMAC_CHCR_TS_16          0x00000018
#define SYSDMAC_CHCR_TS_32          0x00100000
#define SYSDMAC_CHCR_TS_64          0x00100008
#define SYSDMAC_CHCR_IE             0x00000004
#define SYSDMAC_CHCR_TE             0x00000002
#define SYSDMAC_CHCR_DE             0x00000001

/* Contents of the CHCRB register */
#define SYSDMAC_CHCRB_DRST          0x00008000

/* Memory descriptors related definitions */
#define SYSDMAC_DESCRIPTORS_PER_GROUP     128

/* Channel and channel group number related definitions */
typedef struct _rcar_dma_chan_info {
    int    soc;
    int    sys_chans;
    int    sys_chans_per_grp;
    int    audio_chans;
    int    audio_chans_per_grp;
    int    rt_chans;
    int    rt_chans_per_grp;
} rcar_dma_chan_info_t;

rcar_dma_chan_info_t dma_chan_info[] = {
    /*                       SYS-CHAN   SYS-GROUP  AUDIO-CHAN   AUDIO-GROUP      RT-CHANS        RT_GROUPS  */ 
    { PRODUCT_ID_RCAR_H3,        48,        16,        32,            16,            16,             8       },
    { PRODUCT_ID_RCAR_M3W,       48,        16,        32,            16,            16,            16       },
    { PRODUCT_ID_RCAR_M3N,       48,        16,        32,            16,            16,             8       },
    { PRODUCT_ID_RCAR_E3,        48,        16,        16,            16,            16,             8       },
    { PRODUCT_ID_RCAR_D3,        24,         8,        16,            16,             8,             4       },
    { PRODUCT_ID_RCAR_V3M,       16,         8,         0,             0,            32,            16       },
    { PRODUCT_ID_RCAR_V3H,       32,        16,         0,             0,            32,            16       },
    { PRODUCT_ID_RCAR_V3U,       24,        16,         0,             0,            64,            16       },
    { PRODUCT_ID_RCAR_UNKNOWN,    0,         0,         0,             0,             0,             0       },
};

/* Number of SYSDMAC + AUDIODMAC + RT channel groups, each with its own memory address base */
#define RCAR_MAX_DMAC_MMAP_GROUPS         7   /* support groups SYSDMAC 0/1/2, AUDIODMAC 0/1 and RTDMAC 0/1 */

/* Size of registers that are common to a group of channel */
#define RCAR_SYSDMAC_GRP_REGSIZE 0x100

static rcar_soc_t  rcar_soc = PRODUCT_ID_RCAR_UNKNOWN;
static uint32_t    sys_channels;
static uint32_t    audio_channels;
static uint32_t    rt_channels;
static uint32_t    sys_channel_groups;
static uint32_t    audio_channel_groups;
static uint32_t    rt_channel_groups;
static uint32_t    sys_channels_per_group;
static uint32_t    audio_channels_per_group;
static uint32_t    rt_channels_per_group;

/* SYSDMAC0 IRQs: H3/M3W/M3N/E3=16, D3=8, V3M/V3H=0 */
static const uint16_t SYSDMAC0_TE_IRQS[] =
{
    200 + 32,   /* SYSDMAC0 - CH0 */
    201 + 32,   /* SYSDMAC0 - CH1 */
    202 + 32,   /* SYSDMAC0 - CH2 */
    203 + 32,   /* SYSDMAC0 - CH3 */
    204 + 32,   /* SYSDMAC0 - CH4 */
    205 + 32,   /* SYSDMAC0 - CH5 */
    206 + 32,   /* SYSDMAC0 - CH6 */
    207 + 32,   /* SYSDMAC0 - CH7 */
    208 + 32,   /* SYSDMAC0 - CH8 */
    209 + 32,   /* SYSDMAC0 - CH9 */
    210 + 32,   /* SYSDMAC0 - CH10 */
    211 + 32,   /* SYSDMAC0 - CH11 */
    212 + 32,   /* SYSDMAC0 - CH12 */
    213 + 32,   /* SYSDMAC0 - CH13 */
    214 + 32,   /* SYSDMAC0 - CH14 */
    215 + 32,   /* SYSDMAC0 - CH15 - applicable only to H3/M3 */
};

/* SYSDMAC1 IRQs: H3/M3W/M3N/E3=16, D3=8 */
static const uint16_t SYSDMAC1_TE_IRQS[] =
{
    216 + 32,   /* SYSDMAC1 - CH0 */
    217 + 32,   /* SYSDMAC1 - CH1 */
    218 + 32,   /* SYSDMAC1 - CH2 */
    219 + 32,   /* SYSDMAC1 - CH3 */
    308 + 32,   /* SYSDMAC1 - CH4 */
    309 + 32,   /* SYSDMAC1 - CH5 */
    310 + 32,   /* SYSDMAC1 - CH6 */
    311 + 32,   /* SYSDMAC1 - CH7 */
    312 + 32,   /* SYSDMAC1 - CH8 */
    313 + 32,   /* SYSDMAC1 - CH9 */
    314 + 32,   /* SYSDMAC1 - CH10 */
    315 + 32,   /* SYSDMAC1 - CH11 */
    316 + 32,   /* SYSDMAC1 - CH12 */
    317 + 32,   /* SYSDMAC1 - CH13 */
    318 + 32,   /* SYSDMAC1 - CH14 */
    319 + 32,   /* SYSDMAC1 - CH15 - only applicable to H3/M3 */
};

/* SYSDMAC1 IRQs: V3M=8, V3H=16 */
static const uint16_t SYSDMAC1_TE_IRQS_V3MH[] =
{
    216 + 32,   /* SYSDMAC1 - CH0 */
    217 + 32,   /* SYSDMAC1 - CH1 */
    218 + 32,   /* SYSDMAC1 - CH2 */
    219 + 32,   /* SYSDMAC1 - CH3 */
    308 + 32,   /* SYSDMAC1 - CH4 */
    309 + 32,   /* SYSDMAC1 - CH5 */
    310 + 32,   /* SYSDMAC1 - CH6 */
    311 + 32,   /* SYSDMAC1 - CH7 */
    353 + 32,   /* SYSDMAC1 - CH8 */
    354 + 32,   /* SYSDMAC1 - CH9 */
    355 + 32,   /* SYSDMAC1 - CH10 */
    356 + 32,   /* SYSDMAC1 - CH11 */
    357 + 32,   /* SYSDMAC1 - CH12 */
    358 + 32,   /* SYSDMAC1 - CH13 */
    359 + 32,   /* SYSDMAC1 - CH14 */
    360 + 32,   /* SYSDMAC1 - CH15 */
};

/* SYSDMAC2 IRQs: H3/M3W/M3N/E3=16, D3=8 */
static const uint16_t SYSDMAC2_TE_IRQS[] =
{
    /* all entries are only applicable to H3/M3 */
    417 + 32,   /* SYSDMAC2 - CH0 */
    418 + 32,   /* SYSDMAC2 - CH1 */
    419 + 32,   /* SYSDMAC2 - CH2 */
    420 + 32,   /* SYSDMAC2 - CH3 */
    421 + 32,   /* SYSDMAC2 - CH4 */
    422 + 32,   /* SYSDMAC2 - CH5 */
    423 + 32,   /* SYSDMAC2 - CH6 */
    424 + 32,   /* SYSDMAC2 - CH7 */
    425 + 32,   /* SYSDMAC2 - CH8 */
    426 + 32,   /* SYSDMAC2 - CH9 */
    427 + 32,   /* SYSDMAC2 - CH10 */
    428 + 32,   /* SYSDMAC2 - CH11 */
    429 + 32,   /* SYSDMAC2 - CH12 */
    430 + 32,   /* SYSDMAC2 - CH13 */
    431 + 32,   /* SYSDMAC2 - CH14 */
    397 + 32,   /* SYSDMAC2 - CH15 */
};

/* SYSDMAC2 IRQs: V3M=8, V3H=16 */
static const uint16_t SYSDMAC2_TE_IRQS_V3MH[] =
{   312 + 32,   /* SYSDMAC2 - CH0 */
    313 + 32,   /* SYSDMAC2 - CH1 */
    314 + 32,   /* SYSDMAC2 - CH2 */
    315 + 32,   /* SYSDMAC2 - CH3 */
    316 + 32,   /* SYSDMAC2 - CH4 */
    317 + 32,   /* SYSDMAC2 - CH5 */
    318 + 32,   /* SYSDMAC2 - CH6 */
    319 + 32,   /* SYSDMAC2 - CH7 */
    361 + 32,   /* SYSDMAC2 - CH8 */
    362 + 32,   /* SYSDMAC2 - CH9 */
    363 + 32,   /* SYSDMAC2 - CH10 */
    364 + 32,   /* SYSDMAC2 - CH11 */
    365 + 32,   /* SYSDMAC2 - CH12 */
    366 + 32,   /* SYSDMAC2 - CH13 */
    367 + 32,   /* SYSDMAC2 - CH14 */
    368 + 32,   /* SYSDMAC2 - CH15 */
};

/* V3U : SYSDMAC0 IRQs=16 & SYSDMAC1 IRQs=8 */
static const uint16_t SYSDMAC_TE_IRQS_V3U[] = 
{
    32 + 32,    /* SYSDMAC0 - CH0 */
    33 + 32,    /* SYSDMAC0 - CH1 */
    34 + 32,    /* SYSDMAC0 - CH2 */
    35 + 32,    /* SYSDMAC0 - CH3 */
    36 + 32,    /* SYSDMAC0 - CH4 */
    37 + 32,    /* SYSDMAC0 - CH5 */
    38 + 32,    /* SYSDMAC0 - CH6 */
    39 + 32,    /* SYSDMAC0 - CH7 */
    40 + 32,    /* SYSDMAC0 - CH8 */
    41 + 32,    /* SYSDMAC0 - CH9 */
    42 + 32,    /* SYSDMAC0 - CH10 */
    43 + 32,    /* SYSDMAC0 - CH11 */
    44 + 32,    /* SYSDMAC0 - CH12 */
    45 + 32,    /* SYSDMAC0 - CH13 */
    46 + 32,    /* SYSDMAC0 - CH14 */
    47 + 32,    /* SYSDMAC0 - CH15 */
    48 + 32,    /* SYSDMAC1 - CH0 */
    49 + 32,    /* SYSDMAC1 - CH1 */
    50 + 32,    /* SYSDMAC1 - CH2 */
    51 + 32,    /* SYSDMAC1 - CH3 */
    52 + 32,    /* SYSDMAC1 - CH4 */
    53 + 32,    /* SYSDMAC1 - CH5 */
    54 + 32,    /* SYSDMAC1 - CH6 */
    55 + 32,    /* SYSDMAC1 - CH7 */
};

static const uint16_t AUDIODMAC_TE_IRQS[] =
{
    320 + 32,   /* AUDIODMAC CH0 */
    321 + 32,   /* AUDIODMAC CH1 */
    322 + 32,   /* AUDIODMAC CH2 */
    323 + 32,   /* AUDIODMAC CH3 */
    324 + 32,   /* AUDIODMAC CH4 */
    325 + 32,   /* AUDIODMAC CH5 */
    326 + 32,   /* AUDIODMAC CH6 */
    327 + 32,   /* AUDIODMAC CH7 */
    328 + 32,   /* AUDIODMAC CH8 */
    329 + 32,   /* AUDIODMAC CH9 */
    330 + 32,   /* AUDIODMAC CH10 */
    331 + 32,   /* AUDIODMAC CH11 */
    332 + 32,   /* AUDIODMAC CH12 */
    333 + 32,   /* AUDIODMAC CH13 */
    334 + 32,   /* AUDIODMAC CH14 */
    335 + 32,   /* AUDIODMAC CH15 */
    336 + 32,   /* AUDIODMAC CH16 */
    337 + 32,   /* AUDIODMAC CH17 */
    338 + 32,   /* AUDIODMAC CH18 */
    339 + 32,   /* AUDIODMAC CH19 */
    340 + 32,   /* AUDIODMAC CH20 */
    341 + 32,   /* AUDIODMAC CH11 */
    342 + 32,   /* AUDIODMAC CH22 */
    343 + 32,   /* AUDIODMAC CH23 */
    344 + 32,   /* AUDIODMAC CH24 */
    345 + 32,   /* AUDIODMAC CH25 */
    346 + 32,   /* AUDIODMAC CH26 - only applicable to H3/M3*/
    347 + 32,   /* AUDIODMAC CH27 - only applicable to H3/M3*/
    348 + 32,   /* AUDIODMAC CH28 - only applicable to H3/M3*/
    349 + 32,   /* AUDIODMAC CH29 - only applicable to H3/M3*/
    382 + 32,   /* AUDIODMAC CH30 - only applicable to H3/M3 */
    383 + 32,   /* AUDIODMAC CH31 - only applicable to H3/M3 */
};

// For RT-DMAC

static const uint16_t RTDMAC0_TE_IRQS[] =
{
    /* all entries are only applicable to H3/M3/M3N/E3/D3 */
    449 + 32,   /* RTDMAC0 - CH0 */
    450 + 32,   /* RTDMAC0 - CH1 */
    451 + 32,   /* RTDMAC0 - CH2 */
    452 + 32,   /* RTDMAC0 - CH3 */
    453 + 32,   /* RTDMAC0 - CH4 */
    454 + 32,   /* RTDMAC0 - CH5 */
    455 + 32,   /* RTDMAC0 - CH6 */
    456 + 32,   /* RTDMAC0 - CH7 */
    /* all entries are only applicable to M3W */
    457 + 32,   /* RTDMAC1 - CH0 */
    458 + 32,   /* RTDMAC1 - CH1 */
    459 + 32,   /* RTDMAC1 - CH2 */
    460 + 32,   /* RTDMAC1 - CH3 */
    461 + 32,   /* RTDMAC1 - CH4 */
    462 + 32,   /* RTDMAC1 - CH5 */
    463 + 32,   /* RTDMAC1 - CH6 */
    464 + 32,   /* RTDMAC1 - CH7 */    
};

static const uint16_t RTDMAC1_TE_IRQS[] =
{
    /* all entries are only applicable to H3/M3N/E3/D3 */
    457 + 32,   /* RTDMAC1 - CH0 */
    458 + 32,   /* RTDMAC1 - CH1 */
    459 + 32,   /* RTDMAC1 - CH2 */
    460 + 32,   /* RTDMAC1 - CH3 */
    461 + 32,   /* RTDMAC1 - CH4 */
    462 + 32,   /* RTDMAC1 - CH5 */
    463 + 32,   /* RTDMAC1 - CH6 */
    464 + 32,   /* RTDMAC1 - CH7 */
};

/* V3MH : RTDMAC0 IRQs=16 & RTDMAC1 IRQs=16 */
static const uint16_t RTDMAC_TE_IRQS_V3MH[] =
{
    /* all entries are only applicable to V3M/V3H */
    200 + 32,   /* RTDMAC0 - CH0 */
    201 + 32,   /* RTDMAC0 - CH1 */
    202 + 32,   /* RTDMAC0 - CH2 */
    203 + 32,   /* RTDMAC0 - CH3 */
    204 + 32,   /* RTDMAC0 - CH4 */
    205 + 32,   /* RTDMAC0 - CH5 */
    206 + 32,   /* RTDMAC0 - CH6 */
    207 + 32,   /* RTDMAC0 - CH7 */
    208 + 32,   /* RTDMAC0 - CH8 */
    209 + 32,   /* RTDMAC0 - CH9 */
    210 + 32,   /* RTDMAC0 - CH10 */
    211 + 32,   /* RTDMAC0 - CH11 */
    212 + 32,   /* RTDMAC0 - CH12 */
    213 + 32,   /* RTDMAC0 - CH13 */
    214 + 32,   /* RTDMAC0 - CH14 */
    215 + 32,   /* RTDMAC0 - CH15  */
    321 + 32,   /* RTDMAC1 - CH0 */
    322 + 32,   /* RTDMAC1 - CH1 */
    323 + 32,   /* RTDMAC1 - CH2 */
    324 + 32,   /* RTDMAC1 - CH3 */
    325 + 32,   /* RTDMAC1 - CH4 */
    326 + 32,   /* RTDMAC1 - CH5 */
    327 + 32,   /* RTDMAC1 - CH6 */
    328 + 32,   /* RTDMAC1 - CH7 */
    329 + 32,   /* RTDMAC1 - CH8 */
    330 + 32,   /* RTDMAC1 - CH9 */
    331 + 32,   /* RTDMAC1 - CH10 */
    332 + 32,   /* RTDMAC1 - CH11 */
    333 + 32,   /* RTDMAC1 - CH12 */
    334 + 32,   /* RTDMAC1 - CH13 */
    335 + 32,   /* RTDMAC1 - CH14 */
    336 + 32,   /* RTDMAC1 - CH15 */
};

/* V3U : RTDMAC0 IRQs=16 & RTDMAC1 IRQs=16 & RTDMAC2 IRQs=16 & RTDMAC3 IRQs=16 */
static const uint16_t RTDMAC_TE_IRQS_V3U[] =
{
     64 + 32,	/* RTDMAC0 - CH0 */
     65 + 32,	/* RTDMAC0 - CH1 */
     66 + 32,	/* RTDMAC0 - CH2 */
     67 + 32,	/* RTDMAC0 - CH3 */
     68 + 32,	/* RTDMAC0 - CH4 */
     69 + 32,	/* RTDMAC0 - CH5 */
     70 + 32,	/* RTDMAC0 - CH6 */
     71 + 32,	/* RTDMAC0 - CH7 */
     72 + 32,	/* RTDMAC0 - CH8 */
     73 + 32,	/* RTDMAC0 - CH9 */
     74 + 32,	/* RTDMAC0 - CH10 */
     75 + 32,	/* RTDMAC0 - CH11 */
     76 + 32,	/* RTDMAC0 - CH12 */
     77 + 32,	/* RTDMAC0 - CH13 */
     78 + 32,	/* RTDMAC0 - CH14 */
     79 + 32,	/* RTDMAC0 - CH15 */
     80 + 32,	/* RTDMAC1 - CH0 */
     81 + 32,	/* RTDMAC1 - CH1 */
     82 + 32,	/* RTDMAC1 - CH2 */
     83 + 32,	/* RTDMAC1 - CH3 */
     84 + 32,	/* RTDMAC1 - CH4 */
     85 + 32,	/* RTDMAC1 - CH5 */
     86 + 32,	/* RTDMAC1 - CH6 */
     87 + 32,	/* RTDMAC1 - CH7 */
     88 + 32,	/* RTDMAC1 - CH8 */
     89 + 32,	/* RTDMAC1 - CH9 */
     90 + 32,	/* RTDMAC1 - CH10 */
     91 + 32,	/* RTDMAC1 - CH11 */
     92 + 32,	/* RTDMAC1 - CH12 */
     93 + 32,	/* RTDMAC1 - CH13 */
     94 + 32,	/* RTDMAC1 - CH14 */
     95 + 32,	/* RTDMAC1 - CH15 */   
     96 + 32,	/* RTDMAC2 - CH0 */
     97 + 32,	/* RTDMAC2 - CH1 */
     98 + 32,	/* RTDMAC2 - CH2 */
     99 + 32,	/* RTDMAC2 - CH3 */
    100 + 32,	/* RTDMAC2 - CH4 */
    101 + 32,	/* RTDMAC2 - CH5 */
    102 + 32,	/* RTDMAC2 - CH6 */
    103 + 32,	/* RTDMAC2 - CH7 */
    104 + 32,	/* RTDMAC2 - CH8 */
    105 + 32,	/* RTDMAC2 - CH9 */
    106 + 32,	/* RTDMAC2 - CH10 */
    107 + 32,	/* RTDMAC2 - CH11 */
    108 + 32,	/* RTDMAC2 - CH12 */
    109 + 32,	/* RTDMAC2 - CH13 */
    110 + 32,	/* RTDMAC2 - CH14 */
    111 + 32,	/* RTDMAC2 - CH15 */
    112 + 32,	/* RTDMAC3 - CH0 */
    113 + 32,	/* RTDMAC3 - CH1 */
    114 + 32,	/* RTDMAC3 - CH2 */
    115 + 32,	/* RTDMAC3 - CH3 */
    116 + 32,	/* RTDMAC3 - CH4 */
    117 + 32,	/* RTDMAC3 - CH5 */
    118 + 32,	/* RTDMAC3 - CH6 */
    119 + 32,	/* RTDMAC3 - CH7 */
    120 + 32,	/* RTDMAC3 - CH8 */
    121 + 32,	/* RTDMAC3 - CH9 */
    122 + 32,	/* RTDMAC3 - CH10 */
    123 + 32,	/* RTDMAC3 - CH11 */
    124 + 32,	/* RTDMAC3 - CH12 */
    125 + 32,	/* RTDMAC3 - CH13 */
    126 + 32,	/* RTDMAC3 - CH14 */
    127 + 32	/* RTDMAC3 - CH15 */
};

static sysdmac_ctrl_t  sysdmac[RCAR_MAX_DMAC_MMAP_GROUPS] = { {0, 0}, };

static int dma_alloc_buffer(void *handle, dma_addr_t *addr, unsigned size, unsigned flags);
static void dma_free_buffer(void *handle, dma_addr_t *addr);

static int
dma_init(const char *options)
{
    int i;
#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (options == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        options = cs_machine_str;
    }
#endif

    if (options) {
        if ( strstr( options, "H3") || strstr( options, "h3") ) {
            rcar_soc = PRODUCT_ID_RCAR_H3;
        } else if ( strstr( options, "M3N") || strstr( options, "m3n") ) {
            rcar_soc = PRODUCT_ID_RCAR_M3N;
        } else if ( strstr( options, "M3") || strstr( options, "m3") ) {
            rcar_soc = PRODUCT_ID_RCAR_M3W;
        } else if ( strstr( options, "D3") || strstr( options, "d3") ) {
            rcar_soc = PRODUCT_ID_RCAR_D3;
        } else if ( strstr( options, "E3") || strstr( options, "e3") ) {
            rcar_soc = PRODUCT_ID_RCAR_E3;
        } else if ( strstr( options, "V3M") || strstr( options, "v3m") ) {
            rcar_soc = PRODUCT_ID_RCAR_V3M;
        } else if ( strstr( options, "V3H") || strstr( options, "v3h") ) {
            rcar_soc = PRODUCT_ID_RCAR_V3H;
        } else if ( strstr( options, "V3U") || strstr( options, "v3u") ) {
            rcar_soc = PRODUCT_ID_RCAR_V3U;
        }
    }

    for (i = 0; dma_chan_info[i].soc != PRODUCT_ID_RCAR_UNKNOWN; i++) {
        if ( dma_chan_info[i].soc == rcar_soc ) {
            sys_channels             = dma_chan_info[i].sys_chans;
            sys_channels_per_group   = dma_chan_info[i].sys_chans_per_grp;
            if (sys_channels_per_group != 0)
                sys_channel_groups = sys_channels / sys_channels_per_group;
            sys_channel_groups += ((sys_channels % sys_channels_per_group) ? 1 : 0);

            audio_channels           = dma_chan_info[i].audio_chans;
            audio_channels_per_group = dma_chan_info[i].audio_chans_per_grp;
            if (audio_channels_per_group != 0)
                audio_channel_groups = audio_channels / audio_channels_per_group;
            audio_channel_groups += ((audio_channels % audio_channels_per_group) ? 1 : 0);

            rt_channels              = dma_chan_info[i].rt_chans;
            rt_channels_per_group    = dma_chan_info[i].rt_chans_per_grp;
            if (rt_channels_per_group != 0)
                rt_channel_groups = rt_channels / rt_channels_per_group;
            rt_channel_groups += ((rt_channels % rt_channels_per_group) ? 1 : 0);

            break;
        }
    }

    if (dma_chan_info[i].soc == PRODUCT_ID_RCAR_UNKNOWN) {
        fprintf(stderr, "%s: Unknown SoC %d (%s)\n", __FUNCTION__,
                            rcar_soc, options ? options : "Null");
        return EINVAL;
    }

    return EOK;
}

static void
dma_fini()
{
}

static void
dma_query_channel(void *handle, dma_channel_query_t *chinfo)
{
    dma_channel_t   *chan = handle;

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
    info->num_channels      = sys_channels;
    info->max_priority      = sys_channels - 1;

    return 0;
}

static int
dma_channel_info(unsigned channel, dma_channel_info_t *info)
{
    if (rcar_soc == PRODUCT_ID_RCAR_UNKNOWN) {
        if (dma_init(NULL) != EOK) {
            errno = EINVAL;
            return -1;
        }
    }

    if (channel > sys_channels) {
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
                                  DMA_CAP_DEVICE_TO_MEMORY | DMA_CAP_MEMORY_TO_DEVICE | DMA_CAP_EVENT_ON_COMPLETE;
    info->mem_lower_limit       = 0;
    info->mem_upper_limit       = 0xffffffff;
    info->mem_nocross_boundary  = 0;

    return 0;
}

#if _NTO_VERSION >= 700
static void query_hwinfo(dma_channel_t *chan)
{
    unsigned hwi_off, tag_idx;
    hwi_tag *tag;
    uint32_t chan_in_grp_idx = chan->chan_idx % chan->channels_per_group;

    hwi_off = hwi_find_device(chan->name, chan->grp_idx);
    if(hwi_off != HWI_NULL_OFF){
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag){
            chan->pbase = tag->location.base;
            chan->size = tag->location.len;
            
            if (rcar_soc == PRODUCT_ID_RCAR_V3U) {
                tag_idx = 1;
                tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, &tag_idx);
                if(tag) {
                    chan->pbase_grp = tag->location.base;
                    chan->size_grp = tag->location.len;
                }
            } else {
                chan->pbase_grp = tag->location.base;
                chan->size_grp = RCAR_SYSDMAC_GRP_REGSIZE;
            }

            tag_idx = 0;
            while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                if(strcmp(RCAR_HWI_DMA_REG,  __hwi_find_string(tag->regname.regname)) == 0) {
                    chan->pbase += tag->regname.offset;
                } else if(strcmp(RCAR_HWI_DMA_SIZE,  __hwi_find_string(tag->regname.regname)) == 0) {
                    chan->regs_size = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_DMA_DESC,  __hwi_find_string(tag->regname.regname)) == 0) {
                    chan->desc_offset = tag->regname.offset;
                }
            }

            chan->irq = hwitag_find_ivec(hwi_off, &chan_in_grp_idx);
        }
    }
}
#endif

static char *dma_opts[] = {
#define SYSDMAC_OPT_VER     0
    "ver",          // optional, R-car version, e.g. "h3", "m3", "e3", "m3n", "d3", "v3m", "v3h", "v3u"
#define SYSDMAC_OPT_DMATYPE 1
    "dma",          // dmac type, "sys" or "audio", default "sys"
#define SYSDMAC_OPT_DESCNUM 2
    "desc",         // number of internal descriptors required
#define SYSDMAC_OPT_DEBUG   3
    "debug",        // debug, 1=on, 0=off, default off
#define SYSDMAC_OPT_CAIE    4
    "cae_intr",   // Channel Address Error interrupt 1=enabled, 0=disabled, default=disabled
#if _NTO_VERSION >= 704
#define SYSDMAC_OPT_TMEM    5
    "typed_mem",    // typed mem name for buffer
#endif
    NULL
};

static int
sysdmac_parse_options(dma_channel_t *chan, char *options)
{
    char   *value;
    int     opt;

#if _NTO_VERSION >= 704
    chan->typed_mem_fd = NOFD;
#endif

    while (options && *options != '\0') {
        if ((opt = getsubopt(&options, dma_opts, &value)) == -1) {
            fprintf(stderr, "%s:%d - getsubopt()failed - %s\n", __func__, __LINE__, strerror(errno));
            return EINVAL;
        }
        switch (opt) {
            case SYSDMAC_OPT_VER:
                dma_init(value);
                break;
            case SYSDMAC_OPT_DMATYPE:
                strcpy(chan->name, value);
                break;
            case SYSDMAC_OPT_DESCNUM:
                chan->desc_num = strtoul(value, 0, 0);
                break;
            case SYSDMAC_OPT_DEBUG:
                chan->debug = strtoul(value, 0, 0);
                break;
#if _NTO_VERSION >= 704
            case SYSDMAC_OPT_TMEM:
                chan->typed_mem_fd = posix_typed_mem_open(value, O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
                if(chan->typed_mem_fd == NOFD) {
                    fprintf(stderr, "Unable to find typed mem %s: %s\n", value, strerror(errno));
                    return errno;
                }
                break;
#endif
            case SYSDMAC_OPT_CAIE:
                chan->caie = strtoul(value, 0, 0);
                break;
            default:
                fprintf(stderr, "%s:%d - Invalid option %s\n", __func__, __LINE__, dma_opts[opt]);
                return EINVAL;
        }
    }

    if (rcar_soc == PRODUCT_ID_RCAR_UNKNOWN) {
        if (dma_init(NULL) != EOK) {
            return EINVAL;
        }
    }

    if( sys_channels == 0 && audio_channels == 0 && rt_channels == 0) {
        fprintf(stderr, "%s:%d - Invalid channels\n", __func__, __LINE__);
        return EINVAL;
    }
    
    strcpy(chan->desc_name, strcmp(chan->name, "sys") ? "sysdesc" : !strcmp(chan->name, "audio") ? "audiodesc" : "rtdesc");

    // a number of descriptors larger than 128 is invalid
    if( chan->desc_num > 128 ) {
        chan->desc_num = 0;
    }

    if( strcmp(chan->name, "sys") == 0 ) {
        chan->channels = sys_channels;
        chan->channel_groups = sys_channel_groups;
        chan->channels_per_group = sys_channels_per_group;
    } else if( strcmp(chan->name, "audio") == 0 ) {
        chan->channels = audio_channels;
        chan->channel_groups = audio_channel_groups;
        chan->channels_per_group = audio_channels_per_group;
    } else if( strcmp(chan->name, "rt") == 0 ) {
        chan->channels = rt_channels;
        chan->channel_groups = rt_channel_groups;
        chan->channels_per_group = rt_channels_per_group;
    } else {
        fprintf(stderr, "Invalid DMAC type %s\n", chan->name);
        return EINVAL;
    }

    if (rcar_soc == PRODUCT_ID_RCAR_V3U) {
        chan->buffer_mbu_default = 0x80;
        chan->buffer_mbu_max = 512;
        chan->buffer_ulb_default = 0x400;
        chan->buffer_ulb_max = 1024;
    } else {
        chan->buffer_mbu_default = 0x80;
        chan->buffer_mbu_max = 256;
        chan->buffer_ulb_default = 0x100;
        chan->buffer_ulb_max = 512;
    }

#if _NTO_VERSION >= 704
    /* SYS-DMAC can only support 32-bit addresses when IPMMU is enabled, so
     * force the use of below4G type memory if an smmu object was provided
     * and no type memory was specified in the options.
     */
    if (chan->sop && chan->typed_mem_fd == NOFD)
    {
        chan->typed_mem_fd = posix_typed_mem_open("sysram&below4G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
        if(chan->typed_mem_fd == NOFD) {
            fprintf(stderr, "Failed to open typed mem sysram&below4G - %s\n", strerror(errno));
            return errno;
        }
    }
#endif

    return EOK;
}

static uintptr_t sysdmac_map_registers(dma_channel_t *chan)
{
    uint32_t mem_map_idx;
    uint32_t chan_per_grp = chan->channels_per_group;
    uint32_t chan_in_grp_idx = chan->chan_idx % chan_per_grp;
    uint32_t reg_offset = 0;

    chan->pbase = 0;
    chan->pbase_grp = 0;

#if _NTO_VERSION >= 700
    query_hwinfo(chan);

    if( chan->pbase == 0 ) {
        if (rcar_soc == PRODUCT_ID_RCAR_V3M || rcar_soc == PRODUCT_ID_RCAR_V3H) { /* Rcar V3M/V3H */
            if( strcmp(chan->name, "sys") == 0 ) {
                switch( chan->grp_idx ) {
                    case 0: chan->pbase = RCAR_GEN3_SYSDMAC1_BASE; break;
                    case 1: chan->pbase = RCAR_GEN3_SYSDMAC2_BASE; break;
                }
            } else if (strcmp(chan->name, "rt") == 0) {
                switch( chan->grp_idx ) {
                    case 0: chan->pbase = RCAR_GEN3_RTDMAC0_BASE; break;
                    case 1: chan->pbase = RCAR_GEN3_RTDMAC1_BASE; break;
                }
            }
        } else if ( rcar_soc == PRODUCT_ID_RCAR_V3U) {
            if( strcmp(chan->name, "sys") == 0 ) {
                switch( chan->grp_idx ) {
                    case 0: 
                        chan->pbase = RCAR_GEN3_SYSDMAC1_BASE;
                        chan->pbase_grp = RCAR_V3U_SYSDMAC1_COM_BASE;
                        break;
                    case 1: 
                        chan->pbase = RCAR_GEN3_SYSDMAC2_BASE;
                        chan->pbase_grp = RCAR_V3U_SYSDMAC2_COM_BASE;
                        break;
                }
            } else if (strcmp(chan->name, "rt") == 0) {
                switch( chan->grp_idx ) {
                    case 0: 
                        chan->pbase = RCAR_V3U_RTDMAC0_BASE;
                        chan->pbase_grp = RCAR_V3U_RTDMAC0_COM_BASE;
                        break;
                    case 1: 
                        chan->pbase = RCAR_V3U_RTDMAC1_BASE;
                        chan->pbase_grp = RCAR_V3U_RTDMAC1_COM_BASE;
                        break;
                    case 2: 
                        chan->pbase = RCAR_V3U_RTDMAC2_BASE;
                        chan->pbase_grp = RCAR_V3U_RTDMAC2_COM_BASE;
                        break;
                    case 3: 
                        chan->pbase = RCAR_V3U_RTDMAC3_BASE;
                        chan->pbase_grp = RCAR_V3U_RTDMAC3_COM_BASE;
                        break;
                }
            }
        } else {  /* Rcar H3/M3/D3/E3 */
            if( strcmp(chan->name, "sys") == 0 ) {
                switch( chan->grp_idx ) {
                    case 0: chan->pbase = RCAR_GEN3_SYSDMAC0_BASE; break;
                    case 1: chan->pbase = RCAR_GEN3_SYSDMAC1_BASE; break;
                    case 2: chan->pbase = RCAR_GEN3_SYSDMAC2_BASE; break;
                }
            } else if( strcmp(chan->name, "audio") == 0 ) {
                switch( chan->grp_idx ) {
                    case 0: chan->pbase = RCAR_GEN3_AUDIODMAC0_BASE; break;
                    case 1: chan->pbase = RCAR_GEN3_AUDIODMAC1_BASE; break;
                }
            } else if (strcmp(chan->name, "rt") == 0) {
                switch( chan->grp_idx ) {
                    case 0: chan->pbase = RCAR_GEN3_RTDMAC0_BASE; break;
                    case 1: chan->pbase = RCAR_GEN3_RTDMAC1_BASE; break;
                }
            }                
        }

        if ( rcar_soc == PRODUCT_ID_RCAR_V3U) {
            chan->size = RCAR_V3U_SYSDMAC_SIZE;
            chan->size_grp = RCAR_V3U_SYSDMAC_COM_SIZE;
            chan->regs_size = RCAR_V3U_SYSDMAC_CHREG_SIZE;
            chan->desc_offset = RCAR_V3U_SYSDMAC_DESC;
        } else {
            if( chan->pbase != 0 ) {
                chan->pbase_grp = chan->pbase;
                chan->pbase += RCAR_GEN3_SYSDMAC_REGS;
            }

            chan->size = RCAR_GEN3_SYSDMAC_SIZE;
            chan->size_grp = RCAR_SYSDMAC_GRP_REGSIZE;
            chan->regs_size = RCAR_GEN3_SYSDMAC_REGSIZE;
            chan->desc_offset = RCAR_GEN3_SYSDMAC_DESC;
        }
    }
#else
    /* only 2nd generation RCAR platforms supported */
    if( strcmp(chan->name, "sys") == 0 ) {
        switch( chan->grp_idx ) {
            case 0: chan->pbase = RCAR_SYSDMAC0_BASE; break;
            case 1: chan->pbase = RCAR_SYSDMAC1_BASE; break;
        }
    } else if( strcmp(chan->name, "audio") == 0 ) {
        switch( chan->grp_idx ) {
            case 0: chan->pbase = RCAR_AUDIODMAC0_BASE; break;
            case 1: chan->pbase = RCAR_AUDIODMAC1_BASE; break;
        }
    }
    if( chan->pbase != 0 ) {
        chan->pbase_grp = chan->pbase;
        chan->pbase += RCAR_SYSDMAC_REGS;
        chan->regs_size = RCAR_SYSDMAC_REGSIZE;
        chan->size = RCAR_SYSDMAC_SIZE;
        chan->desc_offset = RCAR_SYSDMAC_DESC;
    }
#endif

    if( chan->pbase == 0 ) {
        return (uintptr_t)MAP_FAILED;
    }

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
            return (uintptr_t)MAP_FAILED;
        }

        chan->vbase_grp = mmap_device_io(chan->size_grp, chan->pbase_grp);
        if (chan->vbase_grp == (uintptr_t)MAP_FAILED) {
            return (uintptr_t)MAP_FAILED;
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
        if ( rcar_soc == PRODUCT_ID_RCAR_V3U) {
            chan->desc.vaddr = (void *)chan->vbase_grp;
            chan->desc.paddr = chan->pbase_grp;
        } else {
            chan->desc.vaddr = (void *)chan->vbase;
            chan->desc.paddr = chan->pbase;
        }
    }

    /* Calculate register offset for the requested DMA channel */
    reg_offset = chan_in_grp_idx * (rcar_soc == PRODUCT_ID_RCAR_V3U ? RCAR_V3U_SYSDMAC_REGS : chan->regs_size);

    chan->regs = chan->vbase + reg_offset;

#if _NTO_VERSION >= 704
    if( chan->sop ) {
        /* Each channel is seen as a separate device by the IPMMU, so do a
         * smmu_device_add_mmio() on the specific channel and not the full controller
         */
        if (smmu_device_add_mmio( chan->sop, chan->pbase + reg_offset, chan->regs_size ) == -1) {
            fprintf(stderr, "%s: smmu_device_add_mmio() failed for 0x%"PRIxPTR":%d - %s\n", __func__, chan->pbase + reg_offset, chan->regs_size, strerror(errno));

            const unsigned prev_value = atomic_sub_value(&sysdmac[mem_map_idx].num, 1);
            if (prev_value == 1)
            {
                munmap_device_io(chan->vbase, chan->size);
                munmap_device_io(chan->vbase_grp, chan->size_grp);
                sysdmac[mem_map_idx].num = sysdmac[mem_map_idx].paddr = sysdmac[mem_map_idx].vaddr = sysdmac[mem_map_idx].paddr_grp = sysdmac[mem_map_idx].vaddr_grp = 0;
            }
            return (uintptr_t)MAP_FAILED;
        }
    }

#endif
    return (chan->vbase);
}

static void sysdmac_unmap_registers(dma_channel_t *chan)
{
    uint32_t mem_map_idx;

    // unmap memory if not in use for other DMA channels in same group
    for (mem_map_idx = 0; mem_map_idx < RCAR_MAX_DMAC_MMAP_GROUPS; mem_map_idx++) {
        if (sysdmac[mem_map_idx].paddr == chan->pbase) {
            const unsigned prev_value = atomic_sub_value(&sysdmac[mem_map_idx].num, 1);
            if (prev_value == 1) {
                munmap_device_io(sysdmac[mem_map_idx].vaddr, chan->size);
                sysdmac[mem_map_idx].num = sysdmac[mem_map_idx].paddr = sysdmac[mem_map_idx].vaddr = 0;

                munmap_device_io(sysdmac[mem_map_idx].vaddr_grp, chan->size_grp);
                sysdmac[mem_map_idx].paddr_grp = sysdmac[mem_map_idx].vaddr_grp = 0;                
            }
#if _NTO_VERSION >= 704
            if (chan->sop) {
                uint32_t chan_per_grp = chan->channels_per_group;
                uint32_t chan_in_grp_idx = chan->chan_idx % chan_per_grp;
                uint32_t reg_offset = chan_in_grp_idx * (rcar_soc == PRODUCT_ID_RCAR_V3U ? RCAR_V3U_SYSDMAC_REGS : chan->regs_size);
                smmu_device_add_mmio( NULL, chan->pbase + reg_offset, chan->regs_size );
            }
#endif
            chan->vbase = 0;
            chan->vbase_grp = 0;
        }
    }
}

static void sysdmac_dump_registers(dma_channel_t *chan)
{
    fprintf(stdout, "%s DMA chan %d reg dump: DMADPBASE=%x DMAFIXDPBASE=%x DMACHCRB=%x DMACHCR=%x DMARS=%x DMADPCR=%x\n", chan->name, chan->chan_idx,
        in32(chan->regs + SYSDMAC_DMADPBASE), in32(chan->regs + SYSDMAC_DMAFIXDPBASE),
        in32(chan->regs + SYSDMAC_DMACHCRB), in32(chan->regs + SYSDMAC_DMACHCR),
        in32(chan->regs + SYSDMAC_DMARS), in32(chan->regs + SYSDMAC_DMADPCR));
    fprintf(stdout, "%s DMA chan %d reg dump: DMASAR=%x DMAFIXSAR=%x DMADAR=%x DMAFIXDAR=%x DMATCR=%x DMATCRB=%x\n", chan->name, chan->chan_idx,
        in32(chan->regs + SYSDMAC_DMASAR), in32(chan->regs + SYSDMAC_DMAFIXSAR),
        in32(chan->regs + SYSDMAC_DMADAR), in32(chan->regs + SYSDMAC_DMAFIXDAR),
        in32(chan->regs + SYSDMAC_DMATCR), in32(chan->regs + SYSDMAC_DMATCRB));
}

static int sysdmac_create_resources(dma_channel_t *chan)
{
    rsrc_alloc_t rsrc_req;
    int dma_rsrc_ret = EOK;
    int mem_rsrc_ret = EOK;

    memset(&rsrc_req, 0, sizeof(rsrc_req));

    dma_rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -1, chan->name, RSRCDBMGR_DMA_CHANNEL);

    if (dma_rsrc_ret < 0) {
        dma_rsrc_ret = errno;
        fprintf(stderr, "%s: Failed querying %s dma resources\n", __FUNCTION__, chan->name);
        return dma_rsrc_ret;
    } else if (dma_rsrc_ret == 0) {
        rsrc_req.start = 0;
        rsrc_req.end = chan->channels - 1;
        rsrc_req.flags = RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name = chan->name;

        dma_rsrc_ret = rsrcdbmgr_create (&rsrc_req, 1);

        if( dma_rsrc_ret != EOK ) {
            dma_rsrc_ret = errno;
            fprintf (stderr, "%s: Failed creating resources: err %d\n", __FUNCTION__, dma_rsrc_ret );
            return dma_rsrc_ret;
        }
    } else {
        dma_rsrc_ret = EALREADY;
    }

    memset (&rsrc_req, 0, sizeof(rsrc_req));

    mem_rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -1, chan->desc_name, RSRCDBMGR_MEMORY);

    if (mem_rsrc_ret < 0) {
        mem_rsrc_ret = errno;
        fprintf(stderr, "%s: Failed querying %s memory resources\n", __FUNCTION__, chan->name);
        return mem_rsrc_ret;
    } else if (mem_rsrc_ret == 0) {
        rsrc_req.start  = 0;
        rsrc_req.end    = chan->channel_groups * 128 - 1;
        rsrc_req.flags  = RSRCDBMGR_MEMORY | RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name   = chan->desc_name;

        mem_rsrc_ret = rsrcdbmgr_create (&rsrc_req, 1);

        if (mem_rsrc_ret != EOK) {
            mem_rsrc_ret = errno;
            fprintf (stderr, "%s: Failed creating resources: err %d\n", __FUNCTION__, mem_rsrc_ret);
            return mem_rsrc_ret;
        }
    } else {
        mem_rsrc_ret = EALREADY;
    }

    if (dma_rsrc_ret == EALREADY && mem_rsrc_ret == EALREADY) {
        return EALREADY;
    }

    return EOK;
}

static void *
#if _NTO_VERSION >= 704
dma_channel_attach_smmu(const char *options, const struct sigevent *event, unsigned *channel, int priority, dma_attach_flags flags, struct smmu_object *sop)
#else
dma_channel_attach(const char *options, const struct sigevent *event, unsigned *channel, int priority, dma_attach_flags flags)
#endif
{
    dma_channel_t   *chan;
    char            *optstr = NULL;
    rsrc_request_t  req = { 0 };
    uint32_t chans_per_group;

    if (options) {
        optstr = strdup(options);
    } else {
        optstr = strdup("dma=sys");
    }

    if (optstr == NULL) {
        fprintf(stderr,  "%s: strdup failed\n", __FUNCTION__);
        return NULL;
    }

    chan = calloc(1, sizeof(*chan));
    if (chan == NULL) {
        fprintf(stderr, "%s: calloc failed\n", __FUNCTION__);
        goto fail0;
    }

#if _NTO_VERSION >= 704
    chan->sop = sop;
#endif

    if ((errno = sysdmac_parse_options(chan, optstr)) != EOK) {
        fprintf(stderr, "%s: parse options failed\n", __FUNCTION__);
        goto fail1;
    }

    sysdmac_create_resources(chan);

    req.length = 1;
    req.start  = 0;
    req.end    = chan->channels - 1;
    req.flags  = RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;

    //RCAR SYS DMAC has reversed priority, i.e., channel 0 has the highest priority.
    // select Any Channel; channel argument will be ignored
    if (flags & DMA_ATTACH_ANY_CHANNEL) {
        // Channel number is specified by the priority argument
        if (flags & DMA_ATTACH_PRIORITY_STRICT){
            if ((req.start <= priority) && (priority <= req.end)){
                req.start = req.end = priority;
            }
            else{
                fprintf (stderr, "%s: Invalid Priority number\n", __FUNCTION__);
                goto fail1;
            }
        }
        // The end of range is specified by the priority argument
        else if (flags & DMA_ATTACH_PRIORITY_ATLEAST) {
            if ((req.start <= priority) && (priority <= chan->channels - 1))
                req.end = priority;
            else {
                fprintf (stderr, "%s: Invalid Priority number\n", __FUNCTION__);
                goto fail1;
            }
        }
        // The lowest priority channel should be selected
        else if (!(flags & DMA_ATTACH_PRIORITY_HIGHEST)) {
            req.flags |= RSRCDBMGR_FLAG_TOPDOWN;
        }
    }
    else {
        if (channel == NULL){
            fprintf (stderr, "%s: NULL is an Invalid Channel number\n", __FUNCTION__);
            goto fail1;
        }
        else{
            // A specific (valid) channel is requested
            if ((*channel >= req.start) && (*channel <= chan->channels - 1)){
                req.start = req.end = *channel;
                if (flags & DMA_ATTACH_PRIORITY_STRICT){
                    if (*channel != priority){
                        fprintf (stderr, "%s: Invalid Priority/Channel number\n", __FUNCTION__);
                        fprintf(stderr,  "%s: priority level should be the same as channel number\n", __FUNCTION__);
                        goto fail1;
                    }
                }
                else if (flags & DMA_ATTACH_PRIORITY_ATLEAST) {
                    if((priority < req.start)|| (*channel > priority)){
                        fprintf (stderr, "%s: Invalid Priority/Channel number\n", __FUNCTION__);
                        goto fail1;
                    }
                }
                else if (!(flags & DMA_ATTACH_PRIORITY_HIGHEST)) {
                    req.flags |= RSRCDBMGR_FLAG_TOPDOWN;
                }
            }
            // An Invalid Channel is requested
            else {
                fprintf (stderr, "%s: Invalid Channel number\n", __FUNCTION__);
                goto fail1;
            }
        }
    }

    if (rsrcdbmgr_attach(&req, 1) == -1) {
        fprintf(stderr, "%s: %s DMA channel rsrcdbmgr_attach failed: %s\n", __FUNCTION__, chan->name, strerror(errno));
        goto fail1;
    }

    if (channel) {
        *channel = req.start;
    }

    chans_per_group = chan->channels_per_group;
    chan->chan_idx = req.start;
    chan->grp_idx = req.start / chans_per_group;

    // Request descriptor memory if necessary
    if (chan->desc_num > 0 && chan->desc_num <= 128) {
        req.length = chan->desc_num;
        req.start  = chan->grp_idx * 128;
        req.end    = req.start + 128 - 1;
        req.flags  = RSRCDBMGR_MEMORY | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = chan->desc_name;

        // If resource request fails, the library will try to allocate DMA descriptor from system memory later
        if (rsrcdbmgr_attach(&req, 1) == -1) {
            fprintf(stderr, "%s: %s DMA chan %d grp %d descriptor rsrcdbmgr_attach failed: %s\n", __FUNCTION__,
                chan->name, chan->chan_idx, chan->grp_idx, strerror(errno));
            chan->desc_num = 0;
        } else {
            chan->desc_idx = req.start;
        }
    }

    // map in DMA registers for DMA channel group, including descriptor memory
    if (sysdmac_map_registers(chan) == (uintptr_t)MAP_FAILED) {
        fprintf(stderr, "%s: %s DMA chan %d grp %d register map failed: %s\n", __FUNCTION__,
            chan->name, chan->chan_idx, chan->grp_idx, strerror(errno));
        goto fail2;
    }

    // Disable channel
    out32(chan->regs + SYSDMAC_DMACHCR, 0);

    chan->iid = -1;

    if (chan->irq == 0) {
        if ( rcar_soc == PRODUCT_ID_RCAR_V3U) {
            if (strcmp(chan->name, "sys") == 0) {
                chan->irq = SYSDMAC_TE_IRQS_V3U[chan->chan_idx];
            } else if ( !strcmp(chan->name, "rt") ) {
                chan->irq = RTDMAC_TE_IRQS_V3U[chan->chan_idx];
            }
        } else if (rcar_soc == PRODUCT_ID_RCAR_V3M || rcar_soc == PRODUCT_ID_RCAR_V3H) {
            if (strcmp(chan->name, "sys") == 0) {
                switch( chan->grp_idx ) {
                    case 0: chan->irq = SYSDMAC1_TE_IRQS_V3MH[chan->chan_idx]; break;
                    case 1: chan->irq = SYSDMAC2_TE_IRQS_V3MH[chan->chan_idx % chans_per_group]; break;
                }
            } else {
                chan->irq = RTDMAC_TE_IRQS_V3MH[chan->chan_idx];
            }
        } else {
            if (strcmp(chan->name, "sys") == 0) {
                switch( chan->grp_idx ) {
                    case 0: chan->irq = SYSDMAC0_TE_IRQS[chan->chan_idx]; break;
                    case 1: chan->irq = SYSDMAC1_TE_IRQS[chan->chan_idx % chans_per_group]; break;
                    case 2: chan->irq = SYSDMAC2_TE_IRQS[chan->chan_idx % chans_per_group]; break;
                }
            } else if ( !strcmp(chan->name, "rt") ) {
                switch( chan->grp_idx ) {
                    case 0: chan->irq = RTDMAC0_TE_IRQS[chan->chan_idx]; break;
                    case 1: chan->irq = RTDMAC1_TE_IRQS[chan->chan_idx % chans_per_group]; break;
                }
            } else {
                chan->irq = AUDIODMAC_TE_IRQS[chan->chan_idx];
            }
        }
    }

#ifdef DEBUG
    fprintf(stderr, "%s: (channels %d, groups %d) Name %s Channel %d grp %d irq %d\n",
        __FUNCTION__, chan->channels, chan->channel_groups,
        chan->name, chan->chan_idx, chan->grp_idx, chan->irq);
#endif

    if (flags & (DMA_ATTACH_EVENT_ON_COMPLETE | DMA_ATTACH_EVENT_PER_SEGMENT) && event != NULL) {
        chan->iid = InterruptAttachEvent(chan->irq, event, _NTO_INTR_FLAGS_TRK_MSK);

        if (chan->iid == -1) {
            fprintf(stderr, "%s: %s DMA chan %d grp %d interrupt attach failed: %s\n", __FUNCTION__,
                chan->name, chan->chan_idx, chan->grp_idx, strerror(errno));
            goto fail3;
        }
    }

    chan->aflags = flags;

    return chan;

fail3:
    sysdmac_unmap_registers(chan);
fail2:
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;
    rsrcdbmgr_detach(&req, 1);

    if (chan->desc_num > 0) {
        req.length = chan->desc_num;
        req.start  = chan->desc_idx;
        req.end    = req.start + chan->desc_num - 1;
        req.flags  = RSRCDBMGR_MEMORY | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = chan->name;
        rsrcdbmgr_detach(&req, 1);
    }

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
dma_channel_attach(const char *options, const struct sigevent *event, unsigned *channel, int priority, dma_attach_flags flags)
{
    return (dma_channel_attach_smmu(options, event, channel, priority, flags, NULL));
}
#endif

static void
dma_channel_release(void *handle)
{
    dma_channel_t   *chan = handle;
    rsrc_request_t  req = { 0 };

    // Disable the channel
    out32(chan->regs + SYSDMAC_DMACHCR, 0);

    // release descriptor memory
    if (chan->desc.len > 0) {
        dma_free_buffer(handle, &chan->desc);
    }

    // Detach interrupt
    if (chan->iid != -1) {
        InterruptDetach(chan->iid);
    }

    // release DMA resource
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;
    rsrcdbmgr_detach(&req, 1);

    if (chan->desc_num > 0) {
        req.length = chan->desc_num;
        req.start  = chan->desc_idx;
        req.end    = req.start + chan->desc_num - 1;
        req.flags  = RSRCDBMGR_MEMORY | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = chan->name;
        rsrcdbmgr_detach(&req, 1);
    }

    // unmap registers
    sysdmac_unmap_registers(chan);

#if _NTO_VERSION >= 704
    if(chan->typed_mem_fd != NOFD) {
        close(chan->typed_mem_fd);
        chan->typed_mem_fd = NOFD;
    }
#endif

    free(handle);
}

static int
dma_setup_descriptor(void *handle, const dma_transfer_t *tinfo, uint32_t chcr)
{
    dma_channel_t   *chan = handle;
    sysdmac_desc_t  *desc;
    off64_t         dpbase;     // descriptor physical address
    int             segs, sgi, desc_idx_in_grp;
    uint32_t        fixsar = 0, fixdar = 0;

    // We only support source and destination have same amount of segments
    if (tinfo->src_fragments <= 0 && tinfo->dst_fragments <= 0) {
        return -1;
    }

    segs = tinfo->src_flags & DMA_ADDR_FLAG_SEGMENTED ? tinfo->src_fragments : tinfo->dst_fragments;

    if (chan->desc_num == 0) {  // no internal descriptor memory
        if (segs > SYSDMAC_DESCRIPTORS_PER_GROUP) {
            fprintf(stderr, "SYSDMAC: Unsupported segment value %d", segs);
            return (-1);
        }
        if (chan->desc.len == 0) {
            if( dma_alloc_buffer( handle,
                                  &chan->desc,
                                  SYSDMAC_DESCRIPTORS_PER_GROUP * sizeof(sysdmac_desc_t),
                                  DMA_BUF_FLAG_NOCACHE) != 0 ) {
                fprintf(stderr, "SYSDMAC: Unable to allocate DMA descriptor buffer");
                fprintf(stderr, "%s: %s DMA chan %d grp %d buf alloc failed: %s\n", __FUNCTION__,
                    chan->name, chan->chan_idx, chan->grp_idx, strerror(errno));
                return (-1);
            }
        }
        dpbase = chan->desc.paddr;
        desc   = (sysdmac_desc_t *)chan->desc.vaddr;
    } else {
        if (segs > chan->desc_num) {
            fprintf(stderr, "SYSDMAC: Segment number %d larger than descriptor number %d", segs, chan->desc_num);
            return (-1);
        }
        desc_idx_in_grp = chan->desc_idx % 128;
        dpbase = chan->desc.paddr + chan->desc_offset + desc_idx_in_grp * sizeof(sysdmac_desc_t);
        desc   = (sysdmac_desc_t *)(chan->desc.vaddr + chan->desc_offset + desc_idx_in_grp * sizeof(sysdmac_desc_t));
    }
    
    chan->desc_seg_num = segs;

    out32(chan->regs + SYSDMAC_DMADPBASE, dpbase | 1);
    out32(chan->regs + SYSDMAC_DMAFIXDPBASE, dpbase >> 32);
    out32(chan->regs + SYSDMAC_DMACHCRB, SYSDMAC_CHCRB_DRST);  // Reset descriptor
    in32(chan->regs + SYSDMAC_DMACHCRB);
    out32(chan->regs + SYSDMAC_DMACHCRB, (segs - 1) << 24);

    fixsar = tinfo->src_addrs[0].paddr >> 32;
    fixdar = tinfo->dst_addrs[0].paddr >> 32;
#if _NTO_VERSION >= 704
    if (chan->sop) {
        if (fixsar || fixdar) {
            fprintf(stderr, "%s: above4G not supported when smmu is enabled\n", __FUNCTION__);
            return (-1);
        }
    }
#endif
    out32(chan->regs + SYSDMAC_DMAFIXSAR, fixsar);
    out32(chan->regs + SYSDMAC_DMAFIXDAR, fixdar);

    for (sgi = 0; sgi < segs; sgi++, desc++) {
        if ((tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT) == 0) {
            if ((tinfo->src_addrs[sgi].paddr >> 32) != fixsar) {
                fprintf(stderr, "%s: inconsistent MSB in source descriptors\n", __FUNCTION__);
                return (-1);
            }
        }
        desc->sar  = tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT ? tinfo->src_addrs[0].paddr : tinfo->src_addrs[sgi].paddr;
        if ((tinfo->dst_flags & DMA_ADDR_FLAG_NO_INCREMENT) == 0) {
            if ((tinfo->dst_addrs[sgi].paddr >> 32) != fixdar) {
                fprintf(stderr, "%s: inconsistent MSB in destination descriptors\n", __FUNCTION__);
                return (-1);
            }
        }
        desc->dar  = tinfo->dst_flags & DMA_ADDR_FLAG_NO_INCREMENT ? tinfo->dst_addrs[0].paddr : tinfo->dst_addrs[sgi].paddr;
        /* Determine the number of transfers based on the buffer length used on the side that uses DMA memory buffer(s),
         * that side is the one that does not use the DMA_ADDR_FLAG_NO_INCREMENT flag */
        desc->tcr  = tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT ? tinfo->dst_addrs[sgi].len : tinfo->src_addrs[sgi].len;
        desc->tcr /= tinfo->xfer_unit_size;

        if (desc->tcr > RCAR_DMA_XFER_COUNT_MAX) {
            fprintf(stderr, "%s: Transfer count %d is not exceeded 16,777,215\n", __FUNCTION__, desc->tcr);
            return -1;
        }
    }

    /* Enable Channel Address error interrupt */
    chcr |= SYSDMAC_CHCR_RPT | SYSDMAC_CHCR_DPB | (chan->caie ? SYSDMAC_CHCR_CAIE : 0);

    if (chan->aflags & DMA_ATTACH_EVENT_PER_SEGMENT) {
        chcr |= SYSDMAC_CHCR_DSIE;
    }
    if (chan->aflags & DMA_ATTACH_EVENT_ON_COMPLETE) {
        chcr |= SYSDMAC_CHCR_IE;
    }

    out32(chan->regs + SYSDMAC_DMACHCR, chcr);

    if (chan->debug) {
        sysdmac_dump_registers(chan);        
    }

    return 0;
}

static int
dma_setup_xfer(void *handle, const dma_transfer_t *tinfo)
{
    dma_channel_t   *chan = handle;
    uint32_t        chcr = 0, fixsar = 0, fixdar = 0;

    switch (tinfo->xfer_unit_size) {
        case 2:
            chcr |= SYSDMAC_CHCR_TS_WORD;
            break;
        case 4:
            chcr |= SYSDMAC_CHCR_TS_LONG;
            break;
        case 8:
            chcr |= SYSDMAC_CHCR_TS_8;
            break;
        case 16:
            chcr |= SYSDMAC_CHCR_TS_16;
            break;
        case 32:
            chcr |= SYSDMAC_CHCR_TS_32;
            break;
        case 64:
            chcr |= SYSDMAC_CHCR_TS_64;
            break;
        default:
            chcr |= SYSDMAC_CHCR_TS_BYTE;
            break;
    }

    if (tinfo->src_flags & DMA_ADDR_FLAG_DECREMENT) {
        chcr |= SYSDMAC_CHCR_SM_DECR;
    } else if (!(tinfo->src_flags & DMA_ADDR_FLAG_NO_INCREMENT)) {
        chcr |= SYSDMAC_CHCR_SM_INCR;
    }

    if (tinfo->dst_flags & DMA_ADDR_FLAG_DECREMENT) {
        chcr |= SYSDMAC_CHCR_DM_DECR;
    }
    if (!(tinfo->dst_flags & DMA_ADDR_FLAG_NO_INCREMENT)) {
        chcr |= SYSDMAC_CHCR_DM_INCR;
    }

    // Hardware triggered DMA transfer
    if ((tinfo->src_flags | tinfo->dst_flags) & DMA_ADDR_FLAG_DEVICE) {
        out16(chan->regs + SYSDMAC_DMARS, tinfo->req_id);
        chcr |= SYSDMAC_CHCR_RS_ONCHIP;
    } else {
        chcr |= SYSDMAC_CHCR_RS_AUTO;
    }

    // xfer_complete needs this
    chan->mflags = tinfo->mode_flags;
    // dma_bytes_left needs xfer_unit_size
    chan->xfer_count = tinfo->xfer_bytes / tinfo->xfer_unit_size;
    if (chan->xfer_count > RCAR_DMA_XFER_COUNT_MAX) {
        fprintf(stderr, "%s: Transfer count %d is not exceeded 16,777,215\n", __FUNCTION__, chan->xfer_count);
        return -1;
    }
    chan->xfer_unit_size = tinfo->xfer_unit_size;

    // set buffer control
    if (tinfo->mode_flags & DMA_MODE_FLAG_BUFFER_CONTROL) {
        uint32_t bufcr = 0;
        if(tinfo->buffer_mbu > chan->buffer_mbu_max) 
            bufcr |= chan->buffer_mbu_max << 16;
        else if(tinfo->buffer_mbu)
            bufcr |= tinfo->buffer_mbu << 16;
        else
            bufcr |= chan->buffer_mbu_default << 16;

        if(tinfo->buffer_ulb > chan->buffer_ulb_max) 
            bufcr |= chan->buffer_ulb_max;
        else if(tinfo->buffer_ulb)
            bufcr |= tinfo->buffer_ulb;
        else
            bufcr |= chan->buffer_ulb_default;

        out32(chan->regs + SYSDMAC_DMABUFCR,  bufcr);
    }

    if ((tinfo->src_flags | tinfo->dst_flags) & DMA_ADDR_FLAG_SEGMENTED) {
        if (tinfo->mode_flags & DMA_MODE_FLAG_INFI_REPEAT) {
            chcr |= SYSDMAC_CHCR_DPM_RPT_INF;
	    } else if (tinfo->mode_flags & DMA_MODE_FLAG_REPEAT) {
            chcr |= SYSDMAC_CHCR_DPM_RPT;
        } else {
            chcr |= SYSDMAC_CHCR_DPM_NORMAL;
        }

        return dma_setup_descriptor(handle, tinfo, chcr);
    }

    // segmented mode not in use
    chan->desc_seg_num = 0;

    if (chan->aflags & DMA_ATTACH_EVENT_ON_COMPLETE) {
        chcr |= SYSDMAC_CHCR_IE | (chan->caie ? SYSDMAC_CHCR_CAIE : 0);
    }

    out32(chan->regs + SYSDMAC_DMACHCR, chcr);

    // Single DMA buffer
    fixsar = tinfo->src_addrs[0].paddr >> 32;
    fixdar = tinfo->dst_addrs[0].paddr >> 32;
#if _NTO_VERSION >= 704
    if (chan->sop) {
        if (fixsar || fixdar) {
            fprintf(stderr, "%s: above4G not supported when smmu is enabled\n", __FUNCTION__);
            return (-1);
        }
    }
#endif
    out32(chan->regs + SYSDMAC_DMASAR, tinfo->src_addrs[0].paddr);
    out32(chan->regs + SYSDMAC_DMAFIXSAR, fixsar);
    out32(chan->regs + SYSDMAC_DMADAR, tinfo->dst_addrs[0].paddr);
    out32(chan->regs + SYSDMAC_DMAFIXDAR, fixdar);
    out32(chan->regs + SYSDMAC_DMATCR, chan->xfer_count);

    if (chan->debug) {
        sysdmac_dump_registers(chan);
    }

    return 0;
}

static int
dma_start(void *handle)
{
    dma_channel_t   *chan = handle;
    uint32_t        chcr;

    chcr = in32(chan->regs + SYSDMAC_DMACHCR) & ~(SYSDMAC_CHCR_TE|SYSDMAC_CHCR_DSE|SYSDMAC_CHCR_CAE);

    out32(chan->regs + SYSDMAC_DMACHCR, chcr | SYSDMAC_CHCR_DE);
    if (chan->debug) {
        sysdmac_dump_registers(chan);
    }

    return 0;
}

static int
dma_alloc_buffer(void *handle, dma_addr_t *addr, unsigned size, unsigned flags)
{
    dma_channel_t   *chan = handle;
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
    if (chan->typed_mem_fd != NOFD) {
        map_flags = MAP_SHARED;
    }
    addr->vaddr = mmap(NULL, size, prot_flags, map_flags, chan->typed_mem_fd, 0);
#else
    (void) chan;
    addr->vaddr = mmap(NULL, size, prot_flags, map_flags, NOFD, 0);
#endif
    if(addr->vaddr == MAP_FAILED) {
        fprintf(stderr, "%s failed: %s\n", __FUNCTION__, strerror(errno));
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
dma_free_buffer(void *handle, dma_addr_t *addr)
{
    dma_channel_t   *chan = handle;

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
dma_abort(void *handle)
{
    dma_channel_t   *chan = handle;
    int timeout = 1024; /* 1024 * 1 microseconds = ~1 miliseconds */
    int i = 0;

    /* Recover default setting */
    if (chan->mflags & DMA_MODE_FLAG_BUFFER_CONTROL) {
        out32(chan->regs + SYSDMAC_DMABUFCR,  chan->buffer_mbu_default << 16 | chan->buffer_ulb_default);
    }

    if ( (in32(chan->regs + SYSDMAC_DMACHCR) & SYSDMAC_CHCR_CAE) ) {
        int chan_in_grp_idx = chan->chan_idx % chan->channels_per_group;
        // Clear the DMA channel before proceeding
        // Since you can only write a 1 to each bit in the clear register
        // There isn't worry about a race condition with another channel that may be a
        // part of this global register.
        if (rcar_soc == PRODUCT_ID_RCAR_V3U) {
            out32(chan->regs + RCAR_GEN3_SYSDMAC_CHCLR, 1);
        } else {
            out32(chan->vbase_grp + RCAR_GEN3_SYSDMAC_DMACHCLR, 1 << chan_in_grp_idx);
        }
        fprintf(stderr, "%s: %s DMA chan %d: Channel Address Error\n",
            __FUNCTION__, chan->name, chan->chan_idx);
    } else {
        out32(chan->regs + SYSDMAC_DMACHCR,
                in32(chan->regs + SYSDMAC_DMACHCR) & ~(SYSDMAC_CHCR_DE | SYSDMAC_CHCR_DSIE | SYSDMAC_CHCR_IE | SYSDMAC_CHCR_CAIE | SYSDMAC_CHCR_DSE | SYSDMAC_CHCR_TE));
    }

    /* Even though we set the SYSDMAC_CHCR_DE bit to 0, it will read as 1 until
     * the current transfer is complete; wait until the current transfer is complete
     * by checking that the SYSDMAC_CHCR_DE bit reads 0 */
    for (i = 0; i < timeout; i++) {
        if ( !(in32(chan->regs + SYSDMAC_DMACHCR) & SYSDMAC_CHCR_DE) ) {
            break;
        }

        nanospin_ns(1000); /* 1 microseconds */
    }

    if (chan->iid != -1) {
        InterruptUnmask(chan->irq, chan->iid);
    }

    if (chan->debug) {
        sysdmac_dump_registers(chan);
    }

    if (i == timeout) {
        fprintf(stderr, "%s: %s DMA chan %d: wait for DE clear timed out\n",
            __FUNCTION__, chan->name, chan->chan_idx);
        return ETIMEDOUT;
    } else if (i > 0 && chan->debug) {
        fprintf(stdout, "%s: %s DMA chan %d: waited %d us for DE clear\n",
            __FUNCTION__, chan->name, chan->chan_idx, i);
    }

    return 0;
}

static unsigned
dma_bytes_left(void *handle)
{
    dma_channel_t *chan = handle;

    /* assign xfer_cnt to current TCR value for current segment */
    uint32_t xfer_cnt = in32(chan->regs + SYSDMAC_DMATCR);

    /* if segmented transfer is used, add the TCR values of all following segments */
    if (chan->desc_seg_num) {
        uint32_t next_seg = (in32(chan->regs + SYSDMAC_DMACHCRB) & 0x00FF0000) >> 16;
        /* if next_seg is 0, this was the last segment, nothing to add */
        if (next_seg) {
            sysdmac_desc_t  *desc;
            if (chan->desc_num == 0) {
                desc   = (sysdmac_desc_t *)chan->desc.vaddr;
            } else {
                uint32_t desc_idx_in_grp = chan->desc_idx % 128;
                desc   = (sysdmac_desc_t *)(chan->vbase + chan->desc_offset + desc_idx_in_grp * sizeof(sysdmac_desc_t));
            }
            while (next_seg < chan->desc_seg_num) {
                xfer_cnt += desc[next_seg++].tcr;
            }
        }
    }

    return xfer_cnt * chan->xfer_unit_size;
}

static int
dma_xfer_complete(void *handle)
{
    uint32_t status = 0;
    dma_channel_t *chan = handle;

    status = in32(chan->regs + SYSDMAC_DMACHCR);

    out32(chan->regs + SYSDMAC_DMACHCR, status & ~(SYSDMAC_CHCR_DSE | SYSDMAC_CHCR_TE));

    /* Return the error status code as an indication of whether an error occurred or not */
    status = (status & SYSDMAC_CHCR_CAE) ? (status & SYSDMAC_CHCR_CAE) : EOK;

    /* Abort the transfer if we are not in repeat mode or if we got an error */
    if (!(chan->mflags & (DMA_MODE_FLAG_REPEAT | DMA_MODE_FLAG_INFI_REPEAT)) || (status != EOK)) {
        dma_abort(handle);
        return (status);
    }

    /* Recover default setting */
    if (chan->mflags & DMA_MODE_FLAG_BUFFER_CONTROL) {
        out32(chan->regs + SYSDMAC_DMABUFCR,  chan->buffer_mbu_default << 16 | chan->buffer_ulb_default);
    }

    if (chan->iid != -1) {
        InterruptUnmask(chan->irq, chan->iid);
    }

    return (status);
}


static int
dma_check_xfer_complete(void *handle)
{
    dma_channel_t *chan = handle;

    return  (in32(chan->regs + RCAR_GEN3_SYSDMAC_DMACHCR) & SYSDMAC_CHCR_TE);
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

