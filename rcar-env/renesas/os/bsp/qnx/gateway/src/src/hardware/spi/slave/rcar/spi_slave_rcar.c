/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#include "proto.h"
#include "msiof.h"

/*******************************************************************************
* Outline      : Configure SPI
* Function Name: spi_slave_config
* Description  : This function configure SPI mode, CPOL, CPHA, bitlen, wordlen
* Arguments    : spi_slave_t * dev
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_config (spi_slave_t *dev)
{
    uint32_t tmp;
    int ret = 0;

    /* Check parameter */
    if (0 != dev->cfg.id){
        return -1;
    }

    if( (dev->cfg.wordlen > 0) && (dev->cfg.wordlen <= dev->blksize) ){
        ret = 1;
    }
    ret &= BIT_LEN_VALIDATE(dev->cfg.bitlen);
    ret &= CPOL_VALIDATE(dev->cfg.cpol);
    ret &= CPHA_VALIDATE(dev->cfg.cpha);

    if (0 == ret) {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog Error] spi_slave_config: Invalid param");
        return -1;
    }

    /* Configure FIFO watermark
     * This configuration means MSIOF will set status bit in SISTR register
     * when 0 data in transmit FIFO or more than 1 data in receive FIFO */
    out32(dev->vbase + RCAR_MSIOF_FCTR, 0);
    /* Configure SITMDR1
     * BIT      Name        Value   Description
     * 31       TRMD        0       Slave mode
     * 30       PCON        1       MSIOF_SCK and MSIOF_SYNC are used as common
     *                              signals for transmission and reception
     * 29:28    SYNCMD[1:0] 10      Level mode/SPI
     * 24       BITLSB      1       MSB first
     * 25       SYNCAC      1       MSIOF_SYNC Polarity =  Active Low
     * 22:20    DTDL[2:0]   001     Number of clocks that SYNC active precede DATA
     * 18:16    SYNCDL[2:0] 001     Number of clocks that SYNC remain active after DATA completed
     * 3:2      FLD[1:0]    10      minimum idle time between frames
     *                              2-clock-cycle delay (these bits shall not be set to 00 in SPI mode)
     * 0        TXSTP       1       Transmission Stop
     */
    tmp = SITMDR1_PCON;
    tmp |= SITMDR1_TRANSFER_MODE(MODE_SLAVE);
    tmp |= SITMDR1_SYNCAC(ACTIVE_LOW);
    tmp |= SITMDR1_SYNCCH_SELECT(SYNCCH_MSIOF_SYNC);
    if (dev->cfg.lsb_first & SPI_SLAVE_MSB_FIRST) {
        tmp |= SITMDR1_BITLSB(MSB_FIRST);
    }
    tmp |= SITMDR1_SYNCMD_MODE(SPI_MODE);
    tmp |= SITMDR1_DTDL(DTDL_DELAY_0_CLK);
    tmp |= SITMDR1_SYNCDL(SYNCDL_DELAY_2_CLK);
    tmp |= SITMDR1_FLD(FLD_DELAY_2_CLK);

    out32(dev->vbase + RCAR_MSIOF_TMDR1, tmp);
    /* Set SYNCMD, BITLSB, SYNCAC to SIRMDR1 with the same value as SITMDR1 */
    out32(dev->vbase + RCAR_MSIOF_RMDR1, (tmp & SIRMDR1_MASK));
    /* Configure SPI mode (SICTR register)
     *    CPOL CPHA      TSCKIZ RSCKIZ TEDG REDG
     *    0    0         10     10     1    1
     *    0    1         10     10     0    0
     *    1    0         11     11     0    0
     *    1    1         11     11     1    1
    */
    /* Configure clock polarity */
    tmp = in32(dev->vbase + RCAR_MSIOF_CTR);
    tmp &= ~(SICTR_TSCKIZ_MASK | SICTR_RSCKIZ_MASK | SICTR_TEDG_MASK | SICTR_REDG_MASK);
    if (SPI_CPOL_0 == dev->cfg.cpol) {
        tmp |= SICTR_TSCKIZ(TSCKIZ_0) | SICTR_RSCKIZ(RSCKIZ_0);
    } else {
        tmp |= SICTR_TSCKIZ(TSCKIZ_1) | SICTR_RSCKIZ(RSCKIZ_1);
    }
    /* Configure Transmit and Receive timing */
    if ((dev->cfg.cpol) != (dev->cfg.cpha)) {
        tmp |= SICTR_TEDG(TRANS_AT_RISING_EDGE) | SICTR_REDG(RECV_AT_FALLING_EDGE);
    } else {
        tmp |= SICTR_TEDG(TRANS_AT_FALLING_EDGE) | SICTR_REDG(RECV_AT_RISING_EDGE);
    }
    out32(dev->vbase + RCAR_MSIOF_CTR, tmp);

    int bitlen = dev->cfg.bitlen;
    int word   = dev->cfg.wordlen;

    tmp = ((bitlen - 1) << 24) | ((word - 1) << 16);
    /* Configure bit length (bit per data) and word length (data per frame)
     * This configuration must be the same for RX and TX*/
    out32(dev->vbase + RCAR_MSIOF_TMDR2, tmp); // TX
    out32(dev->vbase + RCAR_MSIOF_RMDR2, tmp); // RX
    /* Clear status register */
    out32(dev->vbase + RCAR_MSIOF_STR,in32(dev->vbase + RCAR_MSIOF_STR));
    /* Clear all interrupt */
    out32(dev->vbase + RCAR_MSIOF_IER, 0);
    rcar_spi_slogf(dev, VERBOSE_LEVEL2, "[Slog INFO] SPI configuration\n"
                   "CPOL = %d, CPHA = %d, bitlen = %d, wordlen = %d\n",
                   dev->cfg.cpol, dev->cfg.cpha, dev->cfg.bitlen, dev->cfg.wordlen);
    rcar_spi_slogf(dev, VERBOSE_LEVEL2, "[Slog INFO] spi_slave_config function\n"
                   "MSIOF_SICTR   = %8X\n"
                   "MSIOF_SIIER   = %8X\n"
                   "MSIOF_SIFCTR  = %8X\n"
                   "MSIOF_SITMDR1 = %8X\n"
                   "MSIOF_SITMDR2 = %8X\n"
                   "MSIOF_SISTR   = %8X\n",
                   in32(dev->vbase + RCAR_MSIOF_CTR), in32(dev->vbase + RCAR_MSIOF_IER),
                   in32(dev->vbase + RCAR_MSIOF_FCTR), in32(dev->vbase + RCAR_MSIOF_TMDR1),
                   in32(dev->vbase + RCAR_MSIOF_TMDR2), in32(dev->vbase + RCAR_MSIOF_STR));
    return EOK;
}

/*******************************************************************************
* Outline      : Enable SPI for transmission and reception
* Function Name: spi_slave_enable_xchange
* Description  : This function prepares SPI module to transfer by configurating register,
*                enabling interrupt and tx, rx
* Arguments    : spi_slave_t * dev
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_enable_xchange (spi_slave_t * dev)
{
    int ret;

    /* Reset rx buffer */
    memset(&dev->rx_data, 0, sizeof(spi_data_t));
    spi_slave_clear_buffer(dev);
    dev->status = SPI_STATUS_NORMAL;
    /* Configure register before transfer */
    ret = spi_slave_config(dev);
    /* Enable interrupt */

    ret |= spi_slave_update_reg(dev->vbase + RCAR_MSIOF_IER, 0,
                               SIIER_REOF | SIIER_RDREQE | SIIER_TDREQE | SIIER_RFSERRE, RCAR_SPI_TIMEOUT);
    /* Enable TX and RX */
    ret |= spi_slave_update_reg(dev->vbase + RCAR_MSIOF_CTR, 0,
                                SICTR_TXE | SICTR_RXE, RCAR_SPI_TIMEOUT);

    if (!ret) {
        return EOK;
    } else {
        return -1;
    }
}

/*******************************************************************************
* Outline      : Disable SPI for transmission and reception
* Function Name: spi_slave_disable_xchange
* Description  : This function disable interrupt and rx/tx
* Arguments    : spi_slave_t * dev
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_disable_xchange (spi_slave_t * dev)
{
    int ret;

    dev->status = SPI_STATUS_NORMAL;
    /* Enable interrupt */
    out32(dev->vbase + RCAR_MSIOF_IER, 0);
    /* Disalbe TX and RX */
    ret = spi_slave_update_reg(dev->vbase + RCAR_MSIOF_CTR, SICTR_TXE | SICTR_RXE, 0,
                                RCAR_SPI_TIMEOUT);
    if (!ret) {
        return EOK;
    } else {
        return -1;
    }
}
/*******************************************************************************
* Outline      : Control SPI module to transfer data
* Function Name: spi_slave_xfer
* Description  : This function is blocking function, it writes data from tx buffer
*                to hardware FIFO, configures interrupt and waits until transmission completes.
* Arguments    : spi_slave_t * dev
*                int len: The word count of data
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_xfer(spi_slave_t * dev, int len)
{
    int ret;

    dev->status = SPI_STATUS_NORMAL;
    if (spi_slave_rst_txrx(dev)) {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] %s: failed to reset RX/TX", __func__);
        return -1;
    }

    dev->cfg.wordlen = len;

    if (EOK != spi_slave_config(dev)) {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] %s: failed to configure", __func__);
        return -1;
    }

    /* Disable interrupt */
    out32(dev->vbase + RCAR_MSIOF_IER, 0);

    /* Clear status */
    out32(dev->vbase + RCAR_MSIOF_STR,in32(dev->vbase + RCAR_MSIOF_STR));

    /* Reset rx buffer */
    spi_slave_clear_buffer(dev);

    memset(&dev->rx_data, 0, sizeof(spi_data_t));

    /* Write data from tx buffer to hardware FIFO */
    if (spi_slave_write_fifo(dev, dev->tx_buff, len) != len) {
        dev->status |= SPI_STATUS_TRANS_FIFO_OVF;
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] %s: failed to write to FIFO", __func__);
        return -1;
    }

    /* Enable interrupt: End of frame, Valid data in receive FIFO and empty transmit FIFO interrupt */
    ret = spi_slave_update_reg(dev->vbase + RCAR_MSIOF_IER, 0,
                               SIIER_REOF | SIIER_RDREQE | SIIER_TDREQE | SIIER_RFSERRE, RCAR_SPI_TIMEOUT);

    /* Enable TX and RX */
    ret |= spi_slave_update_reg(dev->vbase + RCAR_MSIOF_CTR, 0,
                                SICTR_TXE | SICTR_RXE, RCAR_SPI_TIMEOUT);
    if (EOK != ret) {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] %s: failed to enable TX/RX", __func__);
        return -1;
    }

    /* Wait until transfer done */
    if (EOK == spi_slave_wait(dev)) {
        rcar_spi_slogf(dev, VERBOSE_LEVEL2, "[Slog INFO] %s: transfer done", __func__);
        return EOK;
    } else {
        if(dev->status & SPI_STATUS_TRANSFER_TIMEOUT){
            return ETIMEDOUT;
        }
    }

    return -1;
}

/*******************************************************************************
* Outline      : Write data from tx buffer to hardware FIFO
* Function Name: spi_slave_write_fifo
* Description  : This function write data to hardware FIFO with specific bitlen
* Arguments    : spi_slave_t * dev
*                void * tx_buf
*                int len
* Return Value : number of data has been writen
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_write_fifo(spi_slave_t * dev, void * tx_buf, int len)
{
    const uint8_t *buf_8 = tx_buf;
    const uint16_t *buf_16 = tx_buf;
    const uint32_t *buf_32 = tx_buf;
    int bitlen = dev->cfg.bitlen;
    int k, fs;

    if (0 == BIT_LEN_VALIDATE(bitlen)) {
        return -1;
    }

    /* Because data in FIFO is stored at high-bit side, so we need to shift left
     * (32 - bitlen) bit when write to FIFO */
    fs = SPI_FIFO_BIT_LEN - bitlen;

    for (k = 0; k < len; k++) {
        /* check if transmit FIFO is full or not */
        if (!(in32(dev->vbase + RCAR_MSIOF_STR) & SISTR_TFOVF)) {
            switch (bitlen) {
            case SPI_BIT_LEN_8:
                out32(dev->vbase + RCAR_MSIOF_TFDR, buf_8[k] << fs);
                break;
            case SPI_BIT_LEN_16:
                out32(dev->vbase + RCAR_MSIOF_TFDR, buf_16[k] << fs);
                break;
            case SPI_BIT_LEN_32:
                out32(dev->vbase + RCAR_MSIOF_TFDR, buf_32[k] << fs);
                break;
            }
        } else {
            break;
        }
    }

    return k;
}

/*******************************************************************************
* Outline      : Waiting for completion frame
* Function Name: spi_slave_wait
* Description  : This function waits for interrupt and check status flag.
* Arguments    : spi_slave_t * dev
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_wait (spi_slave_t * dev)
{
    uint64_t timeout = RCAR_SPI_INTR_TIMEOUT; //1s
    int      intrerr = ETIMEDOUT, cnt_timeout = 10;

    while (intrerr == ETIMEDOUT){
        TimerTimeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_INTR, NULL, &timeout, NULL);
        //timer_timeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_INTR, NULL, &timeout, NULL);
        intrerr = InterruptWait_r(0, NULL);
        if (SPI_STATUS_TRANSFER_DONE & dev->status){
            return EOK;
        }
        if (SPI_STATUS_SYNC_ERR & dev->status){
            rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] %s : FRAME SYNC error", __func__);
            return -1;
        }
        if(--cnt_timeout == 0) break;
    }
    dev->status |= SPI_STATUS_TRANSFER_TIMEOUT;
    return -1;
}

/*******************************************************************************
* Outline      : Reset tx and rx
* Function Name: spi_slave_rst_txrx
* Description  : This function disable transmisstion and reception, reset pointer in
*                hardware FIFO.
* Arguments    : spi_slave_t * dev
* Note         :
*******************************************************************************/
int spi_slave_rst_txrx(spi_slave_t * dev)
{
    uint32_t tmp;
    uint8_t  timeout = RCAR_SPI_TIMEOUT; //timeout of reset rx/tx
    uint32_t mask = SICTR_TXRST | SICTR_RXRST;

    tmp = in32(dev->vbase + RCAR_MSIOF_CTR);
    tmp |= mask;
    out32(dev->vbase + RCAR_MSIOF_CTR, tmp);

    /* Wait until TXRST and RXRST bit are cleared to 0  */
    for (timeout = RCAR_SPI_TIMEOUT; timeout > 0; timeout--) {
        if (!(in32(dev->vbase + RCAR_MSIOF_CTR) & mask)){
            break;
        }
        nanospin_ns(1000);
    }
    if (!timeout){
        return ETIMEDOUT;
    } else {
        return EOK;
    }
}

/*******************************************************************************
* Outline      : Set or clear some bits in a register and wait for data update
* Function Name: spi_slave_update_reg
* Description  :
* Arguments    : uintptr_t reg
*                uint32_t  clr_bits
*                uint32_t  set_bits
*                uint32_t  timeout
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_update_reg(uintptr_t reg,uint32_t clr_bits, uint32_t set_bits, uint32_t timeout)
{
    uint32_t data;
    uint32_t mask = clr_bits | set_bits;
    data = in32(reg);
    data &= ~clr_bits;
    data |= set_bits;
    out32(reg, data);

    /* Check if register has been updated */
    if (!timeout) { //No timeout
        data = in32(reg) & mask;
        if (data == set_bits) {
            return EOK;
        } else {
            return -1;
        }
    } else {
        do {
            data = in32(reg) & mask;
            timeout--;
        }
        while ((data != set_bits) && (timeout));

        if (timeout) {
            return EOK;
        } else {
            return -1;
        }
    }
}

void spi_slave_clear_buffer(spi_slave_t * dev)
{
    dev->rx_buff.head = 0;
    dev->rx_buff.tail = 0;
    int size_in_byte = dev->blksize;
    if (dev->cfg.bitlen == SPI_BIT_LEN_16) {
        size_in_byte *= 2;
    } else if (dev->cfg.bitlen == SPI_BIT_LEN_32) {
        size_in_byte *= 4;
    }
    memset((void *)dev->rx_buff.data, 0, size_in_byte);
}
