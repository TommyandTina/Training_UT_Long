/*
 * $QNXLicenseC: 
 * Copyright 2017, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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
#include <assert.h>
#include <atomic.h>
#include <string.h>
#include <time.h>

/*
 * Process interrupts.
 *
 * Returns:
 * EOK    Transaction is completed
 * -1     Transaction is not completed
 */
static inline int process_interrupts(rcar_i2c_dev_t *dev)
{
    uint32_t  msr;

    /* Clear START or STOP immediately, it is also Disable Start Generation */
    out32(dev->regbase + RCAR_I2C_ICMCR, in32(dev->regbase + RCAR_I2C_ICMCR) & RCAR_I2C_ICMCR_MASK_DATA);

    msr = in32(dev->regbase + RCAR_I2C_ICMSR);

    /* Just handle interrupts that are currently enabled */
    msr &= in32(dev->regbase + RCAR_I2C_ICMIER);

    if (!dev->isr) {
        rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: STS=0x%x", dev->chan_idx, __FUNCTION__, msr);
    }

    /* Arbitration Lost Interrupt */
    if (msr & RCAR_ICMSR_MAL) {
        if (!dev->isr) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4,
                "[chan %d]%s: MAL interrupt. Transmission not completed: Arbitration lost intr.", dev->chan_idx, __FUNCTION__);
        }
        out32(dev->regbase + RCAR_I2C_ICMIER, 0);
        out32(dev->regbase + RCAR_I2C_ICMCR,  0);
        out32(dev->regbase + RCAR_I2C_ICMSR,  0);

        atomic_set(&dev->status, I2C_STATUS_ARBL | I2C_STATUS_DONE);
        return EOK;
    }

    /* Master NACK Received Interrupt */
    if (msr & RCAR_ICMSR_MNR) {
        if (!dev->isr) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4,
                "[chan %d]%s: MNR interrupt. Transmission not completed: Slave NACK.", dev->chan_idx, __FUNCTION__);
        }
        out32(dev->regbase + RCAR_I2C_ICMSR, ~RCAR_ICMSR_MNR & 0x7F);

        out32(dev->regbase + RCAR_I2C_ICMIER, RCAR_ICMSR_MST);

        //atomic_set(&dev->status, I2C_STATUS_NACK | I2C_STATUS_DONE);
        //return EOK;
        atomic_set(&dev->status, I2C_STATUS_NACK);
        return -1;
    }

    /* Master Stop Transmitted Interrupt */
    if (msr & RCAR_ICMSR_MST) {
        if (!dev->isr) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MST interrupt.", dev->chan_idx, __FUNCTION__);
        }
        out32(dev->regbase + RCAR_I2C_ICMIER, 0);
        out32(dev->regbase + RCAR_I2C_ICMCR,  0);
        out32(dev->regbase + RCAR_I2C_ICMSR,  0);

        atomic_set(&dev->status, I2C_STATUS_DONE);

        return EOK;
    }

    /* Master Address Transmitted Interrupt, Check MAT flags first before check MDR or MDE flag */
    if (msr & RCAR_ICMSR_MAT) {
        uint32_t msr_clear = RCAR_ICMSR_MAT;

        if (msr & RCAR_ICMSR_MDR) {
            if (!dev->isr) {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MAT & MDR interrupts.", dev->chan_idx, __FUNCTION__);
            }
            if (dev->mode & CMODE_RECV) {
                if (dev->mode & CMODE_DODMA) {
                    /* Disable Start Generation */
                    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_DISABLE_START);
                    /* Enable DMA receive mode */
                    out32(dev->regbase + RCAR_I2C_ICDMAER, RCAR_ICDMAER_RMDMAE);
                    /* Start DMA transfer */
                    dev->dma.funcs.xfer_start(dev->dma.hndl);
                } else if ((dev->len == 1) && (dev->mode & CMODE_DOSTOP)) {
                    /* Enable STOP condition and Disable Start condition */
                    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_PHASE_STOP);
                } else {
                    /* Disable Start Generation */
                    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_DISABLE_START);
                }
            }
            /* Need to clear MDR, if not the data transmission suspended */
            msr_clear |= RCAR_ICMSR_MDR;
        } else if (msr & RCAR_ICMSR_MDE) {
            if (!dev->isr) {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MAT & MDE interrupts.", dev->chan_idx, __FUNCTION__);
            }

            if (dev->mode & CMODE_SEND) {
                if ((dev->len == 0) && (dev->mode & CMODE_DOSTOP)) {
                    /* Enable STOP condition */
                    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_PHASE_STOP);
                } else {
                    /* Disable Start Generation */
                    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_DISABLE_START);
                }
            }

            /* Need to clear MDE, if not the data transmission suspended */
            msr_clear |= RCAR_ICMSR_MDE;
        }

        out32(dev->regbase + RCAR_I2C_ICMSR, ~(msr_clear) & 0x7F);

        return -1;
    }

    /* Master Data Received Interrupt */
    if (msr & RCAR_ICMSR_MDR) {
        if (!dev->isr) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MDR interrupts, len %d", dev->chan_idx, __FUNCTION__, dev->len);
        }

        if (dev->len) {
            /* Read one data byte */
            *dev->buf = in32(dev->regbase + RCAR_I2C_ICRXD);
            dev->buf++; dev->len--;

            /* Check if need enable a STOP Generation */
            if ((dev->len == 1) && (dev->mode & CMODE_DOSTOP)) {
                out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_PHASE_STOP);
            }

            /* No Stop Generation and all data received ? */
            if (dev->len == 0  && !(dev->mode & CMODE_DOSTOP)) {
                out32(dev->regbase + RCAR_I2C_ICMIER, 0);
                out32(dev->regbase + RCAR_I2C_ICMCR,  0);
                out32(dev->regbase + RCAR_I2C_ICMSR,  0);

                atomic_set(&dev->status, I2C_STATUS_DONE);
                return EOK;
            }
        }

        /* Clear MDR */
        out32(dev->regbase + RCAR_I2C_ICMSR, ~RCAR_ICMSR_MDR & 0x7F);

        return -1;
    }

    /* Master Data Empty Interrupt */
    if (msr & RCAR_ICMSR_MDE) {
        if (dev->len) {
            if (!dev->isr) {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4,
                    "[chan %d]%s: Transmission not completed: MDE interrupt, but data left to be sent",
                        dev->chan_idx, __FUNCTION__);
            }
            if (dev->mode & CMODE_DODMA) {
                /* Enable DMA transmit mode */
                out32(dev->regbase + RCAR_I2C_ICDMAER, RCAR_ICDMAER_TMDMAE);
                /* Start DMA transfer */
                dev->dma.funcs.xfer_start(dev->dma.hndl);
            } else {
                /* Write one data byte to TXD register */
                out32(dev->regbase + RCAR_I2C_ICTXD, *dev->buf);
                dev->buf++;dev->len--;
            }
        } else {
            if (!dev->isr) {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4,
                    "[chan %d]%s: Transmission completed: MDE interrupt, but actually done.", dev->chan_idx, __FUNCTION__);
            }

            if (dev->mode & CMODE_DOSTOP) {
                /* Enable STOP condition */
                out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_PHASE_STOP);
            } else {
                /* This is for Transmitter – Repeated START - Receiver operation, 
                 * return done status to go to Receiver operation */
                out32(dev->regbase + RCAR_I2C_ICMIER, 0);
                atomic_set(&dev->status, I2C_STATUS_DONE);
                return EOK;
            }
        }
        /* Need to clear MDE, if not the data transmission suspended */
        out32(dev->regbase + RCAR_I2C_ICMSR, ~RCAR_ICMSR_MDE & 0x7F);

        return -1;
    }

    return -1;
}

const struct sigevent* rcar_i2c_intr(void* arg)
{
    rcar_i2c_dev_t  *dev = (rcar_i2c_dev_t*) arg;

    if (process_interrupts(dev) == EOK) {
        /* Transaction is completed. Unblock TX/RX functions */
        return &dev->intrEvent;
    } else {
        /* Transaction is not completed. Block TX/RX functions */
        return NULL;
    }
}

int rcar_i2c_wait_complete(rcar_i2c_dev_t *dev) {
    uint64_t        to = dev->timeout;
    int             status = EOK;
    struct _pulse   pulse;

    if (to == 0) {
        /* timeout depends on data length */
        to = (dev->len + 1) * dev->scl_period * TIMEOUT_MULTI;
    }

    /* Loop until ISR marks dev->status as being DONE or until we run into an error
       waiting on the interrupt such as a time out or system problem */
    while (!(dev->status & I2C_STATUS_DONE) && (dev->status != I2C_STATUS_ERROR)) {

        TimerTimeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_RECEIVE, NULL, &to, NULL);
        if (MsgReceivePulse(dev->chid, &pulse, sizeof(pulse), NULL) < 0) {
            status = errno;

            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Error waiting on pulse: %s(%d)", dev->chan_idx, __FUNCTION__,
                strerror(status), status);

            out32(dev->regbase + RCAR_I2C_ICMIER, 0);
            out32(dev->regbase + RCAR_I2C_ICMSR, 0);
            out32(dev->regbase + RCAR_I2C_ICMCR, 0);
            rcar_i2c_reset(dev);

            dev->status = I2C_STATUS_ERROR;
            break;
        }
        if (pulse.code == RCAR_I2C_EVENT) {
            if (dev->isr == 1) {
                // ISR mode, interrupt is processed in the isr and the transaction is completed
                status = EOK;
            } else {
                // Events mode, process here.
                status = process_interrupts( dev );
                InterruptUnmask( dev->irq, dev->iid );
            }
            continue; // Continue to wait if not done or error
        } else if (pulse.code == RCAR_I2C_DMA_EVENT) {
            if (dev->dma.funcs.bytes_left(dev->dma.hndl) != 0) {
                rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Rx DMA is not completed properly.",
                    dev->chan_idx, __FUNCTION__);
                status = EIO;
            } else {
                status = EOK;
            }
        } else {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Unknown pulse received: %d",
                        dev->chan_idx, __FUNCTION__, pulse.code);
            dev->status = I2C_STATUS_ERROR;
            status = EINVAL;
        }
        break;
    }

    /* If the I2C finished processing but encountered a bus error, handle it */
    if (dev->status & I2C_STATUS_DONE) {
        if (dev->status & I2C_STATUS_ARBL) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: I2C Done and Arbitration lost, slave_addr[%02x]",
                    dev->chan_idx, __FUNCTION__, dev->slave_addr);
            rcar_i2c_reset(dev);
        }
        if (dev->status & I2C_STATUS_NACK) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: I2C Done and Slave NACK, slave_addr[%02x]",
                dev->chan_idx, __FUNCTION__, dev->slave_addr);
        }
    }

    /* Will fall through to EOK unless a system error was encountered while waiting for a pulse */
    return status;
}

int rcar_i2c_interrupt(rcar_i2c_dev_t *dev)
{
    if (!dev->isr) {
        process_interrupts( dev );
        InterruptUnmask( dev->irq, dev->iid );
    }

    return EOK;
}
