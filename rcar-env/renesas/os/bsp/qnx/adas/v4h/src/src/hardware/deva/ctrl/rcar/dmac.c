/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#include <audio_driver.h>
#include <string.h>
#include "dmac.h"

#define DMA_OPT_LEN    64

uint8_t AUDIO_DMAC_MIDRID[AUDIO_PERIPHERAL_NUM][2] =
{
    /* tx,  rx */
    { RCAR_DREQ_SSI00_TXI, RCAR_DREQ_SSI00_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_0
    { RCAR_DREQ_SSI01_TXI, RCAR_DREQ_SSI01_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_1
    { RCAR_DREQ_SSI02_TXI, RCAR_DREQ_SSI02_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_2
    { RCAR_DREQ_SSI03_TXI, RCAR_DREQ_SSI03_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_3
    { RCAR_DREQ_SSI04_TXI, RCAR_DREQ_SSI04_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_4
    { RCAR_DREQ_SSI05_TXI, RCAR_DREQ_SSI05_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_5
    { RCAR_DREQ_SSI06_TXI, RCAR_DREQ_SSI06_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_6
    { RCAR_DREQ_SSI07_TXI, RCAR_DREQ_SSI07_RXI }, // AUDIO_PERIPHERAL_SSI_BUSIF_0_7
};

#define AUDIO_DMAC_ADDR_SSI_BUSIF_BASE     0xEC400000
#define AUDIO_DMAC_ADDR_SSI_BUSIF_SIZE     0x10000
#define AUDIO_DMAC_ADDR_SSI_BUSIF(idx)     (AUDIO_DMAC_ADDR_SSI_BUSIF_BASE + (idx * 0x8000))

int dma_tx_get_config(audio_peripheral_t dst, dmac_config_t* dmac_config)
{
    uint32_t dst_addr = 0;
    if( dmac_config == NULL ) {
        return EINVAL;
    }

    dst_addr = AUDIO_DMAC_ADDR_SSI_BUSIF(dst);

    if( dst_addr == 0 ) {
       return EINVAL;
    }

    dmac_config->addr = dst_addr;
    dmac_config->mid_rid = AUDIO_DMAC_MIDRID[dst][0];

    return EOK;
}

int dma_rx_get_config(audio_peripheral_t src, dmac_config_t* dmac_config)
{
    uint32_t src_addr = 0;
    
    if( dmac_config == NULL ) {
        return EINVAL;
    }

    src_addr = AUDIO_DMAC_ADDR_SSI_BUSIF(src);

    if( src_addr == 0 ) {
       return EINVAL;
    }

    dmac_config->addr = src_addr;
    dmac_config->mid_rid = AUDIO_DMAC_MIDRID[src][1];

    return EOK;
}

extern int get_dmafuncs(dma_functions_t *funcs, int tabsize);

/* In case of multiple rcar deva driver instances mounted by the same io-audio process
 * the DMA functions will be common across all instances, thus they can be stored in
 * static global variables; just make sure to load them only once, when the first
 * instance is mounted
 */
static dma_functions_t dmafuncs;
static ado_mutex_t dmac_global_lock = PTHREAD_MUTEX_INITIALIZER;
static int dmac_init_count = 0;

static int audio_dmac_funcs_init(void)
{
    /*
     * Sys DMA
     */
    if (get_dmafuncs(&dmafuncs, sizeof(dmafuncs)) == -1)
    {
        printf("get_dmafuncs failed.\n");
        return -1;
    }

    dmafuncs.init(NULL);

    return EOK;
}

#ifdef HAVE_SMMU_H
static int audio_dmac_smmu_mapping_add (HW_CONTEXT_T* rcar)
{
    if (ado_smmu_mapping_add (rcar->card, AUDIO_DMAC_ADDR_SSI_BUSIF_BASE, AUDIO_DMAC_ADDR_SSI_BUSIF_SIZE) != EOK) {
        ado_error_fmt ("Failed adding SMMU mapping for addr 0x%x, size %d",
            AUDIO_DMAC_ADDR_SSI_BUSIF_BASE, AUDIO_DMAC_ADDR_SSI_BUSIF_SIZE);
        return (EINVAL);
    }

    return EOK;
}
#endif

#ifdef HAVE_SMMU_H
static int audio_dmac_smmu_mapping_remove (HW_CONTEXT_T* rcar)
{
    ado_smmu_mapping_remove (rcar->card, AUDIO_DMAC_ADDR_SSI_BUSIF_BASE, AUDIO_DMAC_ADDR_SSI_BUSIF_SIZE);

    return EOK;
}
#endif

int audio_dmac_init(HW_CONTEXT_T* rcar)
{
    rcar_audio_channel_t* audio_chan;
    char attach_opt[DMA_OPT_LEN];
    dma_channel_query_t chinfo;
    int ret = EOK;
    ado_mutex_lock (&dmac_global_lock);

    if (dmac_init_count == 0) {
        ret = audio_dmac_funcs_init ();

        if (ret != EOK) {
            ado_mutex_unlock (&dmac_global_lock);
            return ret;
        }
    }
    dmac_init_count++;

#ifdef HAVE_SMMU_H
    ret = audio_dmac_smmu_mapping_add (rcar);
    if (ret != EOK) {
        ado_mutex_unlock (&dmac_global_lock);
        goto dmac_init_fail;
    }
#endif

    ado_mutex_unlock (&dmac_global_lock);

    /*
     * SYS-DMAC
     * Enable Channel Address error (CAE) interrupt
     */
    strlcpy ( attach_opt, "dma=sys,desc=2,cae_intr=1,", sizeof(attach_opt) );
    if ( rcar->debug ) {
        strlcat(attach_opt, "debug=1,", sizeof(attach_opt));
    }

    /* Audio DMA channel */
    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->state == RCAR_CHN_IDLE ) {
                continue;
            }
#ifdef HAVE_SMMU_H
            audio_chan->dma_context.dma_chn =
                dmafuncs.channel_attach_smmu ( "dma=sys", NULL, NULL, 0,
                    DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_PER_SEGMENT, ado_card_smmu(rcar->card) );
#else
            audio_chan->dma_context.dma_chn =
                dmafuncs.channel_attach ( "dma=sys", NULL, NULL, 0, DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_PER_SEGMENT );
#endif
            if ( !audio_chan->dma_context.dma_chn ) {
                ado_error_fmt ( "unable to attach to audio DMA channel" );
                ret = EAGAIN;
                goto dmac_init_fail;
            }

            dmafuncs.query_channel ( audio_chan->dma_context.dma_chn, &chinfo );
            audio_chan->dma_context.dma_irq = chinfo.irq;
        }
    }

    return EOK;

dmac_init_fail:
    audio_dmac_init_cleanup (rcar);

    return ret;
}

void audio_dmac_init_cleanup (HW_CONTEXT_T* rcar)
{
    rcar_audio_channel_t* audio_chan;

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            audio_chan = &rcar->devices[i].audio_channels[chan];
            if ( audio_chan->dma_context.dma_chn ) {
                dmafuncs.channel_release ( audio_chan->dma_context.dma_chn );
                audio_chan->dma_context.dma_chn = NULL;
            }
        }
    }

    ado_mutex_lock (&dmac_global_lock);

#ifdef HAVE_SMMU_H
    audio_dmac_smmu_mapping_remove (rcar);
#endif

    if (--dmac_init_count == 0)
    {
        dmafuncs.fini();
    }

    ado_mutex_unlock (&dmac_global_lock);
}

void audio_dmac_deinit (HW_CONTEXT_T* rcar)
{
    rcar_audio_channel_t* audio_chan;

    for ( uint32_t i = 0; i < RCAR_MAX_DEVICES; i++ ) {
        for ( uint32_t chan = ADO_PCM_CHANNEL_PLAYBACK; chan < ADO_PCM_CHANNEL_MAX; chan++ ) {
            audio_chan = &rcar->devices[i].audio_channels[chan];

            if ( audio_chan->dma_context.dma_chn ) {
                audio_dmac_stop ( audio_chan->dma_context.dma_chn );
            }
        }
    }

    audio_dmac_init_cleanup (rcar);
}

int audio_dmac_count_register_get (void *dma_chn, uint32_t* tc_val)
{
    if( !dma_chn ) {
        return EINVAL;
    }

    *tc_val = dmafuncs.bytes_left(dma_chn);

    return EOK;
}

/*  Memory to peripheral(SSI) */
int audio_dmac_tx_setup(void *dma_chn, audio_peripheral_t dst, off64_t mem_addr, int len)
{
    dma_transfer_t  tinfo;
    dma_addr_t      saddr[2], daddr[2];
    dmac_config_t   dmac_config;

    if (!dma_chn) {
        return EINVAL;
    }

    if( dma_tx_get_config(dst, &dmac_config) != EOK ) {
        ado_error_fmt( "failed getting the DMAC TX config for dst %d", dst );
        return EINVAL;
    }

    memset(&tinfo, 0, sizeof(tinfo));

    tinfo.mode_flags     = DMA_MODE_FLAG_REPEAT;
    tinfo.xfer_unit_size = 4;
    tinfo.xfer_bytes     = len;
    tinfo.src_flags      = DMA_ADDR_FLAG_SEGMENTED;
    tinfo.src_fragments  = 2;
    tinfo.dst_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;

    saddr[0].paddr       = mem_addr;
    saddr[1].paddr       = mem_addr + len / 2;
    saddr[0].len         = len / 2;
    saddr[1].len         = len / 2;
    tinfo.src_addrs      = &saddr[0];

    daddr[0].paddr       = daddr[1].paddr = dmac_config.addr;
    tinfo.dst_addrs      = &daddr[0];

    tinfo.req_id         = dmac_config.mid_rid;

    dmafuncs.setup_xfer(dma_chn, &tinfo);

    return EOK;
}

/*  Peripheral(SSI) to memory  */
int audio_dmac_rx_setup(void *dma_chn, audio_peripheral_t src, off64_t mem_addr, int len)
{
    dma_transfer_t  tinfo;
    dma_addr_t      saddr[2], daddr[2];
    dmac_config_t   dmac_config;

    if (!dma_chn) {
        return EINVAL;
    }

    if( dma_rx_get_config(src, &dmac_config) != EOK ) {
        ado_error_fmt( "Failed getting the DMAC PM config for src %d", src );
        return EINVAL;
    }

    memset(&tinfo, 0, sizeof(tinfo));

    tinfo.mode_flags     = DMA_MODE_FLAG_REPEAT;
    tinfo.xfer_unit_size = 4;
    tinfo.xfer_bytes     = len;
    tinfo.dst_flags      = DMA_ADDR_FLAG_SEGMENTED;
    tinfo.dst_fragments  = 2;
    tinfo.src_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;

    daddr[0].paddr       = mem_addr;
    daddr[1].paddr       = mem_addr + len / 2;
    daddr[0].len         = len / 2;
    daddr[1].len         = len / 2;
    tinfo.dst_addrs      = &daddr[0];

    saddr[0].paddr       = saddr[1].paddr = dmac_config.addr;
    tinfo.src_addrs      = &saddr[0];

    tinfo.req_id         = dmac_config.mid_rid;

    dmafuncs.setup_xfer(dma_chn, &tinfo);

    return EOK;
}

inline int audio_dmac_cleanup(void * dma_chn)
{
    if( !dma_chn ) {
        return EINVAL;
    }

    return (dmafuncs.xfer_complete(dma_chn));
}

inline int
audio_dmac_alloc_buf( void * dma_chn, dma_addr_t *addr, unsigned size, unsigned flags )
{
    if( !dma_chn ) {
        return EINVAL;
    }

    return(dmafuncs.alloc_buffer(dma_chn, addr, size, flags));
}

inline int
audio_dmac_free_buf( void * dma_chn, dma_addr_t *addr )
{
    if( !dma_chn ) {
        return EINVAL;
    }

    dmafuncs.free_buffer(dma_chn, addr);
    return EOK;
}

inline int audio_dmac_start(void *dma_chn)
{
    if( !dma_chn ) {
        return EINVAL;
    }

    /* Start SYS-DMAC */
    dmafuncs.xfer_start (dma_chn);

    return EOK;
}

inline int audio_dmac_stop(void *dma_chn)
{
    if( !dma_chn ) {
        return EINVAL;
    }

    /* Stop SYS-DMAC */
    dmafuncs.xfer_abort(dma_chn);

    return EOK;
}

