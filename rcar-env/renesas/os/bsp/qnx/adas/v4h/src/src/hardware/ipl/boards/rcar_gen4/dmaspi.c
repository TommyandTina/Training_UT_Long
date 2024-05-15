/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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
#include "ipl.h"
#include <stdint.h>
#include <hw/inout.h>
#include <aarch64/r-car.h>
#include "rpc.h"
#include "board.h"

#ifdef RCAR_IPL_SYSDMAC_BASE

/* SYSDMAC registers, offset regs */
#define RCAR_SYSDMAC_DMASAR     0x00
#define RCAR_SYSDMAC_DMADAR     0x04
#define RCAR_SYSDMAC_DMATCR     0x08
#define RCAR_SYSDMAC_DMACHCR    0x0C

#define RCAR_SYSDMAC_DMACHCR_CAE            (1 << 31)
#define RCAR_SYSDMAC_DMACHCR_TS2            (1 << 20)
#define RCAR_SYSDMAC_DMACHCR_DM_INCR        (1 << 14)
#define RCAR_SYSDMAC_DMACHCR_SM_INCR        (1 << 12)
#define RCAR_SYSDMAC_DMACHCR_RS_AUTO        (1 << 10)
#define RCAR_SYSDMAC_DMACHCR_TS_LONG        (1 << 3)
#define RCAR_SYSDMAC_DMACHCR_TE             (1 << 1)
#define RCAR_SYSDMAC_DMACHCR_DE             (1 << 0)

#define DMA_SPI_CHCR_SETTINGS               (RCAR_SYSDMAC_DMACHCR_TS2 | RCAR_SYSDMAC_DMACHCR_DM_INCR | \
                                             RCAR_SYSDMAC_DMACHCR_SM_INCR | RCAR_SYSDMAC_DMACHCR_RS_AUTO | \
                                             RCAR_SYSDMAC_DMACHCR_TS_LONG)

#define RCAR_SYSDMAC_DMAOR_DME              (1 << 0)
#define RCAR_SYSDMAC_DMAOR_PR_RR            (3 << 8)

#define DMA_SPI_XFER_COUNT_SIZE             (0x3f)
#define DMA_SPI_XFER_COUNT_SHIFT            (6)

static int chan_idx = 0;

void InitDma01_Data(uint32_t prgStartAd, uint32_t sector_Ad, uint32_t accessCount)
{
    uint32_t base = RCAR_IPL_SYSDMAC_BASE + RCAR_GEN4_SYSDMAC_REGS * chan_idx;

    /* DMA Setting */
    out32(base + RCAR_SYSDMAC_DMASAR,  sector_Ad);        /* RPC area */
    out32(base + RCAR_SYSDMAC_DMADAR,  prgStartAd);
    out32(base + RCAR_SYSDMAC_DMATCR,  accessCount);
    out32(base + RCAR_SYSDMAC_DMACHCR, DMA_SPI_CHCR_SETTINGS | RCAR_SYSDMAC_DMACHCR_DE);
}

static void StartDma01(void)
{
    out16(RCAR_GEN4_SYSDMAC1_COM_BASE + RCAR_DMAC_DMAOR, RCAR_SYSDMAC_DMAOR_DME | RCAR_SYSDMAC_DMAOR_PR_RR);
}

static uint32_t WaitDma01(void)
{
    uint32_t dataL = 0;
    uint32_t base = RCAR_IPL_SYSDMAC_BASE + RCAR_GEN4_SYSDMAC_REGS * chan_idx;
    /*
     * DMA transfer complite check
     */
    while (1) {
        dataL = in32(base + RCAR_SYSDMAC_DMACHCR);
        if (dataL & RCAR_SYSDMAC_DMACHCR_CAE) {
            out32(base + RCAR_SYSDMAC_DMACHCR, dataL & ~RCAR_SYSDMAC_DMACHCR_TE);
            return(1);
        }
        if (dataL & RCAR_SYSDMAC_DMACHCR_TE) {
            out32(base + RCAR_SYSDMAC_DMACHCR, dataL & ~RCAR_SYSDMAC_DMACHCR_TE);
            break;
        }
    }
    out16(RCAR_GEN4_SYSDMAC1_COM_BASE + RCAR_DMAC_DMAOR, 0x0000);                /* 0: Disables DMA transfers on all channels */

    return(0);
}

static void DisableDma01(void)
{
    uint32_t base = RCAR_IPL_SYSDMAC_BASE + RCAR_GEN4_SYSDMAC_REGS * chan_idx;

    out32(base + RCAR_SYSDMAC_DMACHCR, in32(base + RCAR_SYSDMAC_DMACHCR) & DMA_SPI_CHCR_SETTINGS);
}

static void ClearDmaCh01(void)
{
    out32(RCAR_IPL_SYSDMAC_BASE + RCAR_DMAC_CHCLR, 1);
}

int dma_mem_copy(uint64_t dst, uint32_t src, uint32_t len)
{
    int ret;
    uint32_t paddingOffset = 0;
    uint32_t accessCount = 0;
    /* calculate padding size */
    paddingOffset = (len + DMA_SPI_XFER_COUNT_SIZE ) & ~DMA_SPI_XFER_COUNT_SIZE;
    accessCount = paddingOffset >> DMA_SPI_XFER_COUNT_SHIFT;

    /* DMA Setting */
    ClearDmaCh01();
    InitDma01_Data(dst, src, accessCount);
    StartDma01();
    ret = WaitDma01();
    DisableDma01();
    ClearDmaCh01();
    return ret;
}

#endif // #ifdef RCAR_IPL_SYSDMAC_BASE

