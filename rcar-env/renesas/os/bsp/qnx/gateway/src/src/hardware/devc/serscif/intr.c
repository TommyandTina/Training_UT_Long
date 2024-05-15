/*
 * $QNXLicenseC:
 * Copyright 2016,2021 QNX Software Systems.
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

#include "externs.h"

static uint16_t scfdr_rx( DEV_SCIF *dev )
{
    uint16_t count = in16(dev->port + dev->rx_fifo_count_reg);

    switch (dev->scif)
    {
        case UART_TYPE_SCIF:
            count = SCIF_SCFDR_RX(count);
            break;
        case UART_TYPE_HSCIF:
            count = HSCIF_HSFDR_RX(count);
            break;
        default:
            break;
    }

    return (count);
}

/*
 * error_handling()
 * Check for Overrun, Parity, and Framing errors
 */
static unsigned error_handling(DEV_SCIF *dev)
{
#define STATUS_MASK (SCIF_SCSSR_PER | SCIF_SCSSR_FER | SCIF_SCSSR_BRK | SCIF_SCSSR_ER)
    unsigned err = 0;
    uint16_t fsr = 0;
    uint16_t status_mask = STATUS_MASK;

    /* Read error status */
    fsr = in16(dev->port + dev->status_reg);

    /* The Overrun Error status is in different registers on the different SCIF devices types */
    switch (dev->scif)
    {
        case UART_TYPE_SCIF:
            if (in16(dev->port + SCIF_SCLSR_OFF) & SCIF_SCLSR_ORER)
            {
                err |= TTI_OVERRUN;
                dev->tty.oband_data |= _OBAND_SER_OE;
                /* Clear LSR status */
                set_port16(dev->port + SCIF_SCLSR_OFF, SCIF_SCLSR_ORER, 0);
            }
            break;
        case UART_TYPE_HSCIF:
            break;
        default:
            break;
    }
    /* Clear status */
    set_port16(dev->port + dev->status_reg, status_mask, 0);

    if (fsr & SCIF_SCSSR_BRK )
    {
        err |= TTI_BREAK;
        dev->tty.oband_data |= _OBAND_SER_BI;
    }
    else if (fsr & SCIF_SCSSR_FER)
    {
        switch (dev->scif)
        {
            case UART_TYPE_SCIF:
            case UART_TYPE_HSCIF:
                /* BRK can also be detected by reading RX pin directly when FER is detected */
                if(!(in16(dev->port + SCIF_SCSPTR_OFF) & SCIF_SCSPTR_SPB2DT))
                {
                    err |= TTI_BREAK;
                    dev->tty.oband_data |= _OBAND_SER_BI;
                }
                else
                {
                    err |= TTI_FRAME;
                    dev->tty.oband_data |= _OBAND_SER_FE;
                }
                break;
            default:
                break;
        }
    }
    else if (fsr & SCIF_SCSSR_PER)
    {
        err |= TTI_PARITY;
        dev->tty.oband_data |= _OBAND_SER_PE;
    }

    if(err) {
        /* Save the error as out-of-band data which can be retrieved via devctl(). */
        atomic_set (&dev->tty.flags, OBAND_DATA);
    }

    return (err);
}

static unsigned char read_char(DEV_SCIF *dev)
{
    unsigned char c = 0;

    /* Read the character from the buffer */
    c = in8(dev->port + dev->rx_reg);
    /* Clear interrupt status */
    set_port16(dev->port + dev->status_reg, SCIF_SCSSR_RDF | SCIF_SCSSR_DR, 0);
    set_port16(dev->port + SCIF_SCLSR_OFF, SCIF_SCLSR_TO, 0);

    return (c);
}

int dma_process_rx(DEV_SCIF *dev)
{
    uint16_t  error = 0;

    dev->dma.Rx.status = 0;

    if(dev->tty.verbose > EVENT)
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: byte_cnt=%d",__func__,dev->dma.Rx.byte_cnt);

    /* Handle errors */
    error = error_handling(dev);

    /* No enough space in ibuf. Return!
     * This is okay because auto-cts is enabled and another transfer will
     * get scheduled once we receive the signal in tto.c
     */
    if((dev->tty.ibuf.size - dev->tty.ibuf.cnt) < dev->dma.Rx.byte_cnt)
    {
        /* Set IHW_PAGED otherwise io-char flow control code will not call tto */
        atomic_set (&dev->tty.flags, IHW_PAGED);

        /* Force READ to make more room in io-char buffer */
        atomic_set (&dev->tty.flags, EVENT_READ);

        iochar_send_event (&dev->tty);

        dev->rts_flag = 1;

        /* Disable TO intr */
        set_port16(dev->port + dev->sc_reg, SCIF_SCSCR_TOIE, 0);

        return dev->dma.Rx.status;
    }

    /* Schedule next DMA RX transfer
     * Configure Rx DMA buffer address and transfer size
     */
    dma_transfer_t   tinfo;
    dma_addr_t       dma_saddr;
    dma_addr_t       dma_daddr;

    memset(&tinfo, 0, sizeof(tinfo));

    pthread_mutex_lock(&dev->dma.rx_mutex);
    if(dev->dma.Rx.buffer0)
    {
        /* Start new DMA transfer using RX DMA buffer 1 */
        dma_saddr.paddr      = dev->dma.Rx.fifo_addr;
        dma_daddr.paddr      = dev->dma.Rx.dbuf.paddr + dev->dma.Rx.xfer_size;
        tinfo.req_id         = dev->dma.Rx.dmac_rs;
        tinfo.src_addrs      = &dma_saddr;
        tinfo.dst_addrs      = &dma_daddr;
        tinfo.src_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
        tinfo.xfer_bytes     = dma_daddr.len = dev->dma.Rx.xfer_size;
        tinfo.xfer_unit_size = 1;

        dev->dma.dmafuncs.setup_xfer(dev->dma.Rx.dma_chn, &tinfo);
        dev->dma.dmafuncs.xfer_start(dev->dma.Rx.dma_chn);

        /* Transfer data from RX DMA buffer 0 to RX software fifo */
        dev->dma.Rx.status |= tti2(&dev->tty, (unsigned char *)dev->dma.Rx.dbuf.vaddr, dev->dma.Rx.byte_cnt, error);
    }
    else
    {
        /* Start new DMA transfer using RX DMA buffer 0 */
        dma_saddr.paddr      = dev->dma.Rx.fifo_addr;
        dma_daddr.paddr      = dev->dma.Rx.dbuf.paddr;
        tinfo.req_id         = dev->dma.Rx.dmac_rs;
        tinfo.src_addrs      = &dma_saddr;
        tinfo.dst_addrs      = &dma_daddr;
        tinfo.src_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
        tinfo.xfer_bytes     = dma_daddr.len = dev->dma.Rx.xfer_size;
        tinfo.xfer_unit_size = 1;

        dev->dma.dmafuncs.setup_xfer(dev->dma.Rx.dma_chn, &tinfo);
        dev->dma.dmafuncs.xfer_start(dev->dma.Rx.dma_chn);

        /* Transfer data from RX DMA buffer 1 to RX software fifo */
        dev->dma.Rx.status |= tti2(&dev->tty, (unsigned char *)dev->dma.Rx.dbuf.vaddr + dev->dma.Rx.xfer_size, dev->dma.Rx.byte_cnt, error);
    }

    dev->dma.Rx.buffer0 ^= 1;
    pthread_mutex_unlock(&dev->dma.rx_mutex);

    /* Clear interrupt status: DR, TO */
    set_port16(dev->port + dev->status_reg, SCIF_SCSSR_DR, 0);
    set_port16(dev->port + SCIF_SCLSR_OFF, SCIF_SCLSR_TO, 0);

    if (dev->dma.Rx.status)
    {
        iochar_send_event (&dev->tty);
    }

    return dev->dma.Rx.status;
}

static int process_tx(DEV_SCIF *dev)
{
    int status = EOK;
    uint16_t tx_status = 0;

    tx_status = in16 (dev->port + dev->status_reg) & SCIF_SCSSR_TDFE;
    if(tx_status & SCIF_SCSSR_TDFE)
    {
        /* Disable TX interrupt, don't bother trying to clear the interrupt
         * status until we write more data to the TX FIFO in tto().
         */
        set_port16(dev->port + dev->sc_reg, SCIF_SCSCR_TIE, 0);
        /* Clear timer */
        dev->tty.un.s.tx_tmr = 0;
        atomic_set(&dev->tty.flags, EVENT_TTO);
        status |= 1;
    }

    return (status);
}

static const struct sigevent * do_interrupt(void *area)
{
    DEV_SCIF        *dev;
    int             status = EOK;
    struct sigevent *event = NULL;
    unsigned        c;
    int             pending_interrupts;

    for (dev = ((struct dev_list*)area)->device; dev != NULL; dev = dev->next)
    {
        if (dev->dma.status==DMA_DISABLE)
        {
            /* Process RX and error interrupts */
            if (scfdr_rx(dev))
            {
                // If we are hw flow controlled, don't read the char's.
                // This causes the fifo to fill up, and the the scif chip
                // automatically handles hw flow control.
                if (!dev->rts_flag)
                {
                    do
                    {
                        c = error_handling(dev);
                        c |= read_char(dev);
                        status |= tti(&dev->tty, c);
                    } while (!status && scfdr_rx(dev) > 0);
                }
                else
                {
                    // Make sure that RX and RX error interrupts get disabled if we are
                    // HW flow controlled or we could lock up the board, stuck in the ISR.
                    set_port16(dev->port + dev->sc_reg, dev->rx_intr_mask, 0);
                }
            } else {
                /* Handle BRK/ORER intr, which does not populate rx fifo */
                c = error_handling(dev);
                if (c)
                    status |= tti(&dev->tty, c);
            }
            status |= process_tx(dev);

            /* Return the status if any to io-char library */
            if (status) {
                if (dev->mode==INTR_EVENT) {
                    /* Interrupt event handler does not return a tte event, we must issue tte pulse manually */
                    iochar_send_event (&dev->tty);
                } else {
                    if (!(dev->tty.flags & EVENT_QUEUED)) {
                        event = &ttyctrl.event;
                        dev_lock(&ttyctrl);
                        ttyctrl.event_queue[ttyctrl.num_events++] = &dev->tty;
                        atomic_set(&dev->tty.flags, EVENT_QUEUED);
                        dev_unlock(&ttyctrl);
                    }
                }
            }
        }
        else /* DMA enabled */
        {
            /* TO(Timeout) intr */
            if ((in16(dev->port + SCIF_SCLSR_OFF) & SCIF_SCLSR_TO))
            {
                /* If TO intr is enabled */
                if( in16(dev->port + dev->sc_reg) & SCIF_SCSCR_TOIE )
                {
                    event = &dev->dma.Rx.event;
                    status |= 1;

                    /* Disable TO intr */
                    set_port16(dev->port + dev->sc_reg, SCIF_SCSCR_TOIE, 0);
                }
            }

            /* Check interrupt source */
            pending_interrupts = in16(dev->port + dev->status_reg);

            /* TX */
            if( pending_interrupts & SCIF_SCSSR_TDFE)
            {
                /* Clear interrupt */
                set_port16(dev->port + dev->sc_reg, SCIF_SCSCR_TIE, 0);
            }

            /* Overrun error */
            if(in16(dev->port + SCIF_SCLSR_OFF) & SCIF_SCLSR_ORER)
            {
                set_port16(dev->port + SCIF_SCLSR_OFF, SCIF_SCLSR_ORER, 0);
            }

            /* Return the status if any to dma pulse handler */
            if (status) {
                if (dev->mode==INTR_EVENT) {
                    MsgSendPulse(event->sigev_coid, event->sigev_priority, event->sigev_code, 0);
                }
            }
        }
    }
    return (event);
}

/*
 * This routine will handle all of our interrupts and dispatch them as required
 */
const struct sigevent * ser_intr(void *arg, int iid)
{
    return do_interrupt (arg);
}

int interrupt_event_handler (message_context_t * msgctp, int code, unsigned flags, void *handle)
{
    struct dev_list  *list = handle;
    do_interrupt (handle);
    InterruptUnmask (list->device->sh_intr, list->iid);
    return (EOK);
}

