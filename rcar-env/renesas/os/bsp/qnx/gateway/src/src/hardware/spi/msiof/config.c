/*
 * $QNXLicenseC:
 * Copyright 2015, QNX Software Systems.
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

#include "msiof.h"

#define NUM_SPI_CLK_RATES		6

static spi_clk_t const msiof_spi_clk_table[NUM_SPI_CLK_RATES] = {
    { 1,    0x0007 },
    { 2,    0x0000 },
    { 4,    0x0001 },
    { 8,    0x0002 },
    { 16,   0x0003 },
    { 32,   0x0004 },
};

static void msiof_spi_set_clk(msiof_spi_t *msiof, unsigned long rate)
{
    int i, brps;
    uint32_t div;

    div = msiof->clock / rate;
 
    if (div > 1024) {
        i = 5, brps = 32;
    } else {
        i = 1;
        for (brps = (div + 1) >> i; brps > 32; i++){
            brps = (brps + 1) >> 1;
        }

    }

    out16(msiof->vbase + RCAR_MSIOF_TSCR, msiof_spi_clk_table[i].scr | ((brps - 1) << 8));
}

void msiof_setup(msiof_spi_t *msiof, uint32_t device, uint32_t clock_rate, uint32_t mode)
{
    unsigned int tmp = 0;
    unsigned int tmdr1 = 0;
    unsigned int rmdr1 = 0;
    uint32_t fifo_setting;

    /* Setup clocks */
    msiof_spi_set_clk(msiof, clock_rate);

    out32(msiof->vbase + RCAR_MSIOF_CTR, msiof->mode[device]);

    /* Configure FIFO */
    /* The default value of SCIFCTR_TFUA[26:20] is 0x40 in R-CarGen3/V3U/S4.
        The default value of SCIFCTR_TFUA[28:2] is 0x100 in R-CarV4H */
    /* Set TFWM to B'111 as recommended by TRM (R-CarGen3/V3U section 59.3.5.3, R-CarGen4 section 109.3.5.3) for DMA setting */
    fifo_setting = in32(msiof->vbase + RCAR_MSIOF_FCTR);
    fifo_setting &= ~(FCTR_TFWM_MSK | FCTR_RFWM_MSK);
    fifo_setting |= (FCTR_TFWM_1 | FCTR_RFWM_1);
    out32(msiof->vbase + RCAR_MSIOF_FCTR, fifo_setting);

    if (msiof->dma.status == 0) {
        /* Set default FIFO watermarks for PIO */
        out32(msiof->vbase + RCAR_MSIOF_FCTR, 0);
    }
    /* Configure LSB/MSB
     * BITLSB = 0, MSB; BITLSB = 1, LSB
     */
    tmp = ((mode & SPI_MODE_BODER_MSB) ? 0 : 1) << 24;

    /* Configure SYNC Mode (bits 28 and 29) */
    /* SYNCMD[1:0] = 10 Level mode/SPI is the only acceptable setting for SPI drivers */
    /* Rest of the bits are set to 0 to comply with RMDR1 register settings on TRM */
    tmp |= 0x20000000;

    /* Configure CS */
    if (!(mode & SPI_MODE_CSSTAT_HIGH))
        tmp |= 1 << 25;

    /* Configure TMDR1 - 0xC020000D:
     * set TRMD[31] to choose SPI Master mode for Transfer Mode
     * set PCON[30] this is the only acceptable setting for Transfer Signal Connection(TRM)
     * set SITMDR1 DTDL[2:0] (bits 22 to 20) to 000: 0-clock-cycle delay for H3 1.0
                                                001: 1-clock-cycle delay for H3 1.1
                                                010: 2-clock-cycle delay for H3 3.0 and for other products
     * set SIRMDR1 DTDL[2:0] (bits 22 to 20) to 000: 2-clock-cycle delay for H3 1.0
                                                001: 1-clock-cycle delay for H3 1.1
                                                010: 2-clock-cycle delay for H3 1.0, H3 3.0 and for other products	  
     * set FLD [1:0] (bits 3 to 2) to 11: 3-clock-cycle delay; as TRM  asks for minumum 3 clock cycle delay on the usage notes.
     * set TXSTP[0] This is the only valid value for Transmission Stop - TRM */
     if (msiof->product_id == PRODUCT_ID_RCAR_H3) {
        switch (msiof->product_rev) {
            case PRODUCT_REV_1_0:
                tmdr1 = tmp | (0 << 20);
                rmdr1 = tmp | (2 << 20);
            break;
            case PRODUCT_REV_1_1:
                tmdr1 = tmp | (1 << 20);
                rmdr1 = tmp | (1 << 20); 
            break;
            case PRODUCT_REV_3_0:
            default:
                tmdr1 = tmp | (2 << 20);
                rmdr1 = tmp | (2 << 20);
            break;
        }
    } else {
        tmdr1 = tmp | (2 << 20);
        rmdr1 = tmp | (2 << 20);
    }

    out32(msiof->vbase + RCAR_MSIOF_TMDR1, 0xC000000D | tmdr1 | ((device & 0x03) << 26));
    out32(msiof->vbase + RCAR_MSIOF_RMDR1, rmdr1);
    out32(msiof->vbase + RCAR_MSIOF_TMDR2, 0);
    out32(msiof->vbase + RCAR_MSIOF_RMDR2, 0);

    /* Disable interrupts */
    out32(msiof->vbase + RCAR_MSIOF_IER, 0);
}

int msiof_cfg(void *hdl, spi_cfg_t *cfg, uint32_t *ctrl)
{
    (void) hdl;
    uint32_t tmp, edge;

    if (cfg == NULL) {
        return EINVAL;
    }
    /* Configure SPI mode (SICTR register)
     *    CPOL CPHA      TSCKIZ RSCKIZ TEDG REDG
     *    0    0         10     10     1    1
     *    0    1         10     10     0    0
     *    1    0         11     11     0    0
     *    1    1         11     11     1    1
    */
    tmp = CTR_TSCKIZ(!!(cfg->mode & SPI_MODE_CKPOL_HIGH));
    tmp |= CTR_RSCKIZ(!!(cfg->mode & SPI_MODE_CKPOL_HIGH));

    edge = (!!(cfg->mode & SPI_MODE_CKPOL_HIGH));
    edge ^= !(!!(cfg->mode & SPI_MODE_CKPHASE_HALF));
    tmp |= edge << CTR_TEDG_BP;
    tmp |= edge << CTR_REDG_BP;
    *ctrl = tmp;

    return EOK;
}
