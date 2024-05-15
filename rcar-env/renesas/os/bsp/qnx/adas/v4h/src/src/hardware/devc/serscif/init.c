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
#include <sys/mman.h>

//This is the maximum of the SCI and SCIF register sizes
#define SCI_MEM_SIZE 36*8

void set_port8(uintptr_t port, uint8_t mask, uint8_t data)
{
    out8(port, (in8(port) & ~mask) | (data & mask));
}

void set_port16(uintptr_t port, uint16_t mask, uint16_t data)
{
    out16(port, (in16(port) & ~mask) | (data & mask));
}

void set_port32(uintptr_t port, uint32_t mask, uint32_t data)
{
    out32(port, (in32(port) & ~mask) | (data & mask));
}

static void log_settings(DEV_SCIF *dev, TTYINIT_SCIF *devinit)
{
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Port .......................... %s (0x%x)", dev->tty.name, (uint32_t)devinit->tty.port);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "IRQ ........................... 0x%x", devinit->tty.intr);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Input Clock ................... %d", dev->clk);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Clock Divisor ................. %d", dev->div);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "DMA reqid TX .................. 0x%x", dev->dma.Tx.dmac_rs);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "DMA reqid RX .................. 0x%x", dev->dma.Rx.dmac_rs);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "DMA Chan Range................. %d-%d", dev->dma.chan_min, dev->dma.chan_max);
    /* Setting FIFO trigger levels not supported for HSCIF.
     * For HSCIF, the default power-up reg values are not modified (TTRG=8, RTRG=1)
     */
    if (dev->scif == UART_TYPE_SCIF) 
    {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Tx fifo size (TTRG[1:0])....... 0x%x", ((devinit->fifo >> 4) & 0x03));
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Rx fifo trigger (RTRG[1:0]).... 0x%x", ((devinit->fifo >> 6) & 0x03));
    } 
    else  if (dev->scif == UART_TYPE_HSCIF) 
    {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Tx fifo size (TTRG[1:0])....... 0x%x", ((devinit->fifo >> 0) & 0x7F));
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Rx fifo trigger (RTRG[1:0]).... 0x%x", ((devinit->fifo >> 8) & 0x7F));
    }
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Input buffer size ............. %d", dev->tty.ibuf.size);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Input flow control highwater .. %d", dev->tty.highwater);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Output buffer size ............ %d", dev->tty.obuf.size);
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "Canonical buffer size ......... %d\n", dev->tty.cbuf.size);
}

static int ser_attach_intr(DEV_SCIF *dev)
{
    struct dev_list **owner;
    struct dev_list *curr;

    owner = &devices;
    for (;;) {
        curr = *owner;
        if (curr == NULL) {
            curr = malloc(sizeof(*curr));
            if (curr == NULL) {
                fprintf(stderr, "Init allocation failed\n");
                exit(EXIT_FAILURE);
            }
            *owner = curr;
            curr->next = NULL;
            curr->device = NULL;
            break;
        }
        if (curr->device->sh_intr == dev->sh_intr) {
            break;
        }
        owner = &curr->next;
    }

    dev->next = curr->device;
    curr->device = dev;

    if (curr->device->next == NULL) {
        if (dev->mode==INTR_EVENT) {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "InterruptAttachEvent!");
            struct sigevent event;

            /* Associate a pulse which will call the event handler. */
            if ((event.sigev_code =
                pulse_attach(ttyctrl.dpp, MSG_FLAG_ALLOC_PULSE, 0, &interrupt_event_handler, curr)) == -1) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s: Unable to attach event pulse: %s", __FUNCTION__, strerror(errno));
                return -1;
            }

            /* Init the pulse for interrupt event */
            event.sigev_notify = SIGEV_PULSE;
            event.sigev_coid = ttyctrl.coid;
            event.sigev_priority = ttyctrl.event.sigev_priority + 1; /* service the event at a higher priority than io-char event */
            event.sigev_value.sival_int = 0;
            if (dev->sh_intr != 0) {
                curr->iid = InterruptAttachEvent(dev->sh_intr, &event, _NTO_INTR_FLAGS_TRK_MSK);
                if (curr->iid == -1) {
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s: Unable to attach InterruptEvent: %s", __FUNCTION__, strerror(errno));
                    return -1;
                }
            } else {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Unable to attache Interrupt event, INTR==0", __FUNCTION__);
                return -1;
            }
        } else {
            if (dev->mode!=INTR_ISR) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Invalid Mode option, ISR will handle the interrupts", __FUNCTION__);
                dev->mode=INTR_ISR;
            }
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "InterruptAttach!");
            if (dev->sh_intr != 0) {
                curr->iid = InterruptAttach(dev->sh_intr, (void*) ser_intr, curr, 0, 0);
                if (curr->iid == -1) {
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s: Unable to attach Interrupt: %s", __FUNCTION__, strerror(errno));
                    return -1;
                }
            } else {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Unable to attache Interrupt, INTR==0", __FUNCTION__);
                return -1;
            }
        }
        return (EOK);
    } else {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Failed to Attach; Wrong handler for ser_attach_intr", __FUNCTION__);
        return -1;
    }
}

/*
 * create_device()
 * Set up data structures for the console serial device
 */
int create_device(TTYINIT_SCIF *dip, unsigned unit)
{
    DEV_SCIF *dev;

    // Get buffers and set the name of the device
    if ((dev = calloc(1, sizeof(*dev))) == NULL) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Device allocation failed", __FUNCTION__);
        goto fail;
    }

    strcpy(dev->tty.name,dip->tty.name);
    dev->scif = dip->scif;
    dev->rstrg = dip->rstrg;
    dev->fifo = dip->fifo;
    dev->sh_intr = dip->tty.intr;
    dev->clock_source = dip->clock_source;
    dev->mode = dip->mode;
    dev->dma = dip->dma;

    switch (dev->scif) {
        case UART_TYPE_SCIF:
            dev->tx_reg = SCIF_SCFTDR_OFF;
            dev->rx_reg = SCIF_SCFRDR_OFF;
            dev->sc_reg = SCIF_SCSCR_OFF;
            dev->fc_reg = SCIF_SCFCR_OFF;
            dev->status_reg = SCIF_SCFSR_OFF;
            dev->tx_fifo_count_reg = SCIF_SCFDR_OFF;
            dev->rx_fifo_count_reg = SCIF_SCFDR_OFF;

            dev->rx_intr_mask = SCIF_SCSCR_RIE | SCIF_SCSCR_REIE;
            break;
        case UART_TYPE_HSCIF:
            dev->tx_reg = HSCIF_HSFTDR_OFF;
            dev->rx_reg = HSCIF_HSFRDR_OFF;
            dev->sc_reg = HSCIF_HSSCR_OFF;
            dev->fc_reg = HSCIF_HSFCR_OFF;
            dev->status_reg = HSCIF_HSFSR_OFF;
            dev->tx_fifo_count_reg = HSCIF_HSFDR_OFF;
            dev->rx_fifo_count_reg = HSCIF_HSFDR_OFF;

            dev->rx_intr_mask = SCIF_SCSCR_RIE | SCIF_SCSCR_REIE;
            break;
        default:
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Unsupport SCIF type", __FUNCTION__);
            goto fail1;
    }

    /* Map the io devices registers into address space - offset 4 bytes */
    if ((dev->port = mmap_device_io(SCI_MEM_SIZE, dip->tty.port)) == (uintptr_t)MAP_FAILED) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : MAP FAILED", __FUNCTION__);
        goto fail1;
    }

    /* Allocate Input buffer */
    if ((dev->tty.ibuf.buff = malloc(dev->tty.ibuf.size = dip->tty.isize)) == NULL) {
       slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Unable to allocate %d byte input buffer", __FUNCTION__, dev->tty.ibuf.size);
        goto fail2;
    } else {
        dev->tty.ibuf.head = dev->tty.ibuf.tail = dev->tty.ibuf.buff;
    }
    /* Allocate Output buffer */
    if ((dev->tty.obuf.buff = malloc(dev->tty.obuf.size = dip->tty.osize)) == NULL) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Unable to allocate %d byte output buffer", __FUNCTION__, dev->tty.obuf.size);
        goto fail3;
    } else {
        dev->tty.obuf.head = dev->tty.obuf.tail = dev->tty.obuf.buff;
    }
    /* Allocate Canonical buffer */
    if ((dev->tty.cbuf.buff = malloc(dev->tty.cbuf.size = dip->tty.csize)) == NULL) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : Unable to allocate %d byte canonical buffer", __FUNCTION__, dev->tty.cbuf.size);
        goto fail4;
    } else {
        dev->tty.cbuf.head = dev->tty.cbuf.tail = dev->tty.cbuf.buff;
    }

    dev->clk = dip->tty.clk;
    dev->div = dip->tty.div;
    dev->tty.baud = dip->tty.baud;
    dev->tty.fifo = dip->tty.fifo;
    dev->tty.verbose = dip->tty.verbose;
    dev->rts_hw_disable = dip->rts_hw_disable;
    dev->tty.highwater = dev->tty.ibuf.size - (dev->tty.ibuf.size < 128 ? dev->tty.ibuf.size/4 : 32);

    dev->tty.flags = EDIT_INSERT | LOSES_TX_INTR;
    dev->tty.c_cflag = dip->tty.c_cflag;
    dev->tty.c_iflag = dip->tty.c_iflag;
    dev->tty.c_lflag = dip->tty.c_lflag;
    dev->tty.c_oflag = dip->tty.c_oflag;

    /* Initialize termios cc codes to an ANSI terminal. */
    ttc(TTC_INIT_CC, &dev->tty, 0);

    /* Init DMA */
    if (dip->dma.status == DMA_ENABLE) {
        dev->dma.status        = dip->dma.status;
        dev->dma.chan_min      = dip->dma.chan_min;
        dev->dma.chan_max      = dip->dma.chan_max;
        dev->dma.Rx.status     = 0;
        dev->dma.Rx.buffer0    = 1;
        dev->dma.Rx.xfer_size  = DMA_XFER_SIZE;
        dev->dma.Tx.xfer_size  = DMA_XFER_SIZE;
        dev->dma.Rx.dmac_rs    = dip->dma.Rx.dmac_rs;
        dev->dma.Tx.dmac_rs    = dip->dma.Tx.dmac_rs;
        dev->dma.Rx.fifo_addr  = dip->dma.Rx.fifo_addr;
        dev->dma.Tx.fifo_addr  = dip->dma.Tx.fifo_addr;
        dev->dma.Tx.byte_cnt   = 0;
        dev->dma.Rx.byte_cnt   = 0;
        dev->dma.Tx.dma_busy   = 0;
        dev->dma.Rx.dma_busy   = 0;

        if (scif_dma_init(dev) == -1) {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : scif_dma_init failed: dma not enabled", __FUNCTION__);
            dev->dma.status = DMA_DISABLE;
        }
        dev->tty.highwater = dev->tty.ibuf.size + 1;
        dev->tty.flags &= ~LOSES_TX_INTR;
    } else {
        dev->tty.highwater = dev->tty.ibuf.size - (dev->tty.ibuf.size < 128 ? dev->tty.ibuf.size/4 : 32);
    }

    /*
     * Initialize the device's name.
     * Assume that the basename is set in device name.  This will attach
     * to the path assigned by the unit number/minor number combination
     */
    unit = SET_NAME_NUMBER(unit) | NUMBER_DEV_FROM_USER;
    ttc(TTC_INIT_TTYNAME, &dev->tty, unit);

    /* Disable Transmitter, Receiver and all interrupts */
    out16 (dev->port + dev->sc_reg, 0x0);

    if (ser_attach_intr(dev) == -1) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s : ser_attach_intr function exits with failure; check the logs", __FUNCTION__);
        goto fail5;
    } else {
        dev->ready = 1;
    }

    /* Create resmgr namespace entry */
    ttc(TTC_INIT_ATTACH, &dev->tty, 0);

    /* enable the interrupts and TX */
    ser_stty_scif(dev);

    if (dev->tty.verbose) {
        log_settings(dev, dip);
    }

    return (EOK);

    fail5:
        free(dev->tty.cbuf.buff);
    fail4:
        free(dev->tty.obuf.buff);
    fail3:
        free(dev->tty.ibuf.buff);
    fail2:
        munmap_device_io(dev->port,SCI_MEM_SIZE);
    fail1:
        free (dev);
    fail:
        return -1;
}

void disable_uart(void)
{
    DEV_SCIF *dev;
    struct dev_list *list;

    list = devices;
    while (list)
    {
        dev = list->device;
        while (dev)
        {
            if (dev->dma.status == DMA_ENABLE)
            {
                dev->dma.dmafuncs.xfer_abort(dev->dma.Rx.dma_chn);
                dev->dma.dmafuncs.xfer_abort(dev->dma.Tx.dma_chn);
                dev->dma.dmafuncs.free_buffer(dev->dma.Tx.dma_chn, &dev->dma.Tx.dbuf);
                dev->dma.dmafuncs.free_buffer(dev->dma.Rx.dma_chn, &dev->dma.Rx.dbuf);
                dev->dma.dmafuncs.channel_release(dev->dma.Rx.dma_chn);
                dev->dma.dmafuncs.channel_release(dev->dma.Tx.dma_chn);
                dev->dma.dmafuncs.fini();

                if (dev->dma.smmu_obj != NULL) {
                    smmu_obj_destroy(dev->dma.smmu_obj);
                }
                ConnectDetach(dev->dma.coid);
                ChannelDestroy(dev->dma.chid);

                set_port16(dev->port + dev->sc_reg, SCIF_SCSCR_TOIE, 0);
            }
            if (dev->dma.typed_mem) {
                free((void *)dev->dma.typed_mem);
            }
            /* Disable Receiver,  Transmitter and all the Interrupts */
            set_port16(dev->port + dev->sc_reg, SCIF_SCSCR_RE | dev->rx_intr_mask | SCIF_SCSCR_TE | SCIF_SCSCR_TIE, 0);

            /* Clear Rx Fifo */
            set_port16(dev->port + dev->fc_reg, SCIF_SCFCR_RFRST, SCIF_SCFCR_RFRST);
            set_port16(dev->port + dev->fc_reg, SCIF_SCFCR_RFRST, 0);

            dev = dev->next;
        }
        list = list->next;
    }

    if (ttyctrl.flags & USE_SMMU) {
        smmu_fini();
    }
}
