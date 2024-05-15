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

#ifndef _R_Car_DMAC_H
#define _R_Car_DMAC_H

#include <hw/dma.h>
#include <aarch64/r-car.h>
#include "rcar.h"

typedef enum
{
    AUDIO_PERIPHERAL_SSI_BUSIF_0_0,
    AUDIO_PERIPHERAL_SSI_BUSIF_0_1,
    AUDIO_PERIPHERAL_SSI_BUSIF_0_2,
    AUDIO_PERIPHERAL_SSI_BUSIF_0_3,
    AUDIO_PERIPHERAL_SSI_BUSIF_0_4,
    AUDIO_PERIPHERAL_SSI_BUSIF_0_5,
    AUDIO_PERIPHERAL_SSI_BUSIF_0_6,
    AUDIO_PERIPHERAL_SSI_BUSIF_0_7,
    AUDIO_PERIPHERAL_NUM
} audio_peripheral_t;

typedef struct {
   uint32_t addr;
   uint32_t mid_rid; /* as defined as MID and RID bit fields of the DMARS register */
} dmac_config_t;

#define AUDIO_PERIPHERAL_SSI_NUM 1

#define AUDIO_PERIPHERAL_SSI_BUSIF(idx) AUDIO_PERIPHERAL_SSI_BUSIF_0_0 + idx

int audio_dmac_init(HW_CONTEXT_T* rcar);
void audio_dmac_init_cleanup(HW_CONTEXT_T* rcar);
void audio_dmac_deinit(HW_CONTEXT_T* rcar);

int audio_dmac_count_register_get(void *dma_chn, uint32_t * tc_val);

/* Memory to peripheral transfer */
int dma_tx_get_config(audio_peripheral_t dst, dmac_config_t* dmac_config);
int audio_dmac_tx_setup(void *dma_chn, audio_peripheral_t dst, off64_t mem_addr, int len);

/* Peripheral to memory transfer */
int dma_rx_get_config(audio_peripheral_t src, dmac_config_t* dmac_config);
int audio_dmac_rx_setup(void *dma_chn, audio_peripheral_t src, off64_t mem_addr, int len);

/* Peripheral to memory and memory to peripheral transfer start/stop/clean */
int audio_dmac_start(void *dma_chn);
int audio_dmac_stop(void *dma_chn);
int audio_dmac_cleanup(void *dma_chn);
int audio_dmac_alloc_buf(void *dma_chn, dma_addr_t *addr, unsigned size, unsigned flags);
int audio_dmac_free_buf(void *dma_chn, dma_addr_t *addr);

#endif /* _R_Car_DMAC_H */
