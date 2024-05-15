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

#include <sys/neutrino.h>
#include <atomic.h>
#include "spi_slave.h"
#include "msiof.h"

/*******************************************************************************
* Outline      : Interrupt handler function
* Function Name: spi_irq
* Description  : This function will be called when an interrupt happen.
* Arguments    : area: argument passed to this funtion
*                id: interrupt id
* Return Value : sigevent
* Note         :
*******************************************************************************/
const struct sigevent * spi_slave_attach_intr(void * area, int id)
{
    spi_slave_t * dev = (spi_slave_t *)area;
    uint32_t tmp;
    uint32_t intr;

    volatile uint8_t  *buf_8;
    volatile uint16_t *buf_16;
    volatile uint32_t *buf_32;

    int      idx;
    int      fs;
    buf_8 = dev->rx_buff.data;
    buf_16 = dev->rx_buff.data;
    buf_32 = dev->rx_buff.data;

    fs   = SPI_FIFO_BIT_LEN - dev->cfg.bitlen;
    intr = in32(dev->vbase + RCAR_GEN3_MSIOF_STR);
    out32(dev->vbase + RCAR_GEN3_MSIOF_STR, intr);

    /* Receive data transfer request interrupt */
    if (SISTR_RDREQ & intr)
    {
        idx = dev->rx_buff.head % BUFF_MAX_SIZE;
        /* Data in hardware FIFO is stored at bitlen-bit MSB, so we need to
         * shift right (32 - bitlen) bit to read data  */
        if (SPI_BIT_LEN_8 == dev->cfg.bitlen)
        {
            buf_8[idx] = in32(dev->vbase + RCAR_GEN3_MSIOF_RFDR) >> fs;
        }
        else if (SPI_BIT_LEN_16 == dev->cfg.bitlen)
        {
            buf_16[idx] = in32(dev->vbase + RCAR_GEN3_MSIOF_RFDR) >> fs;
        }
        else
        {
            buf_32[idx] = in32(dev->vbase + RCAR_GEN3_MSIOF_RFDR) >> fs;
        }
        dev->rx_buff.head++;
    }
    /* Transmit data transfer request interrupt */
    if (SISTR_TDREQ & intr)
    {
        /* Write dummy 0 to transmit FIFO to prevent bus error on MOSI line */
        out32(dev->vbase + RCAR_GEN3_MSIOF_TFDR, 0x00 << fs);
    }
    /* Receive FIFO full */
    /* When there is a data arrives in recv FIFO, RDREQ interrupt will happen, and we read
     * this data to rx buffer. So when RFFUL interrupt happen, it can be an error. */
    if (SISTR_RFFUL & intr)
    {
        dev->status |= SPI_STATUS_RECV_FIFO_FULL;
    }

    /* Receive frame sync error */
    if (SISTR_RFSERR & intr)
    {
        /* Clear status bit */
        out32(dev->vbase + RCAR_GEN3_MSIOF_STR, SIIER_RFSERRE);
        dev->status |= SPI_STATUS_SYNC_ERR;
    }
    /* End of frame interrupt */
    if (SISTR_REOF & intr)
    {
        /* Disable interrupt */
        out32(dev->vbase + RCAR_GEN3_MSIOF_IER, 0);
        /* Clear status */
        out32(dev->vbase + RCAR_GEN3_MSIOF_STR,
               in32(dev->vbase + RCAR_GEN3_MSIOF_STR));
        /* Disable TX, RX to prevent receiving unexpected data can cause bus error */
        tmp = in32 (dev->vbase + RCAR_GEN3_MSIOF_CTR);
        tmp &= ~(SICTR_RXE | SICTR_TXE);
        out32 (dev->vbase + RCAR_GEN3_MSIOF_CTR, tmp);

        /* Reset RX, TX hardware FIFO to clear unused data */
        tmp = in32(dev->vbase + RCAR_GEN3_MSIOF_CTR);
        tmp |= SICTR_TXRST | SICTR_RXRST;
        out32 (dev->vbase + RCAR_GEN3_MSIOF_CTR, tmp);

        dev->status |= SPI_STATUS_TRANSFER_DONE;
    }
    return &dev->spievent;
}
