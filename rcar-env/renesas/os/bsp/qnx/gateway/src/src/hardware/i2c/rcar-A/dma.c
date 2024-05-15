/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#include "proto.h"
#include <atomic.h>
#include <string.h>

int rcar_i2c_interrupt(rcar_i2c_dev_t *dev);

#define RCAR_I2C_ICDMAER_MDMACTSZ(x)    ((((x)-1)&0xFF) << 24)
#define RCAR_I2C_ICDMAER_RMDMATSZ(x)    (((x==256?0:x)&0xFF) << 16)
#define RCAR_I2C_ICDMAER_TMDMATSZ(x)    (((x==256?0:x)&0xFF) << 8)
#define RCAR_I2C_ICDMAER_TMDMACE        (1 << 7)
#define RCAR_I2C_ICDMAER_RMDMACE        (1 << 6)

static int rcar_dma_wait_complete(rcar_i2c_dev_t *dev)
{
    uint64_t    	to = dev->len + 1;
    struct _pulse   pulse;

    to *= dev->scl_period * 1000;

    while (1) {
        TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_RECEIVE, NULL, &to, NULL);

        if (MsgReceivePulse(dev->chid, &pulse, sizeof(pulse), NULL) == -1) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: %s", dev->chan_idx, __FUNCTION__, strerror(errno));
            return EIO;
        }

        switch (pulse.code) {
            case RCAR_I2C_EVENT:
                rcar_i2c_interrupt(dev);
                break;
            case RCAR_I2C_DMA_EVENT:
                if (dev->dma.funcs.bytes_left(dev->dma.hndl) != 0) {
                    rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Rx/Tx DMA is not completed properly.", dev->chan_idx, __FUNCTION__);
                    return EIO;
                }
                return EOK;
        }
    }

    return EIO;
}

static int rcar_dma_cont_wait_complete(rcar_i2c_dev_t *dev)
{
    uint8_t sta, dsta, msr, ier;
    int timeout = TIMEOUT_MULTI;

    if (dev->mode & CMODE_RECV) {
        sta = RCAR_ICMSR_MAT | RCAR_ICMSR_MDR;
        dsta = RCAR_I2C_ICDMAER_RMDMACE;
    }
    else {
        sta = RCAR_ICMSR_MAT | RCAR_ICMSR_MDE;
        dsta = RCAR_I2C_ICDMAER_TMDMACE;
    }

    ier = in32(dev->regbase + RCAR_I2C_ICMIER) & 0x7F;

    while (in32(dev->regbase + RCAR_I2C_ICDMAER) & dsta) {
        msr = in32(dev->regbase + RCAR_I2C_ICMSR) & 0x7F;
        msr &= ier;

        if ((msr & sta) == (RCAR_ICMSR_MAT | RCAR_ICMSR_MDR)) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MAT & MDR interrupts.", dev->chan_idx, __FUNCTION__);
            sta &= ~RCAR_ICMSR_MAT;
        }

        if ((msr & sta) == (RCAR_ICMSR_MAT | RCAR_ICMSR_MDE)) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MAT & MDE interrupts.", dev->chan_idx, __FUNCTION__);
            sta &= ~RCAR_ICMSR_MAT;
        }

        if ((msr & sta) == RCAR_ICMSR_MDR) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MDR interrupts.", dev->chan_idx, __FUNCTION__);
            sta &= ~RCAR_ICMSR_MDR;
            dev->dma.funcs.xfer_start(dev->dma.hndl); // Start DMA
            if (rcar_i2c_wait_complete(dev) == EOK) {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: DMA Completed.", dev->chan_idx, __FUNCTION__);
                atomic_set(&dev->status, I2C_STATUS_DONE);
            }
            else {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: DMA Failed.", dev->chan_idx, __FUNCTION__);
                atomic_set(&dev->status, I2C_STATUS_ERROR);
            }
        }

        if ((msr & sta) == RCAR_ICMSR_MDE) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MDE interrupts.", dev->chan_idx, __FUNCTION__);
            sta &= ~RCAR_ICMSR_MDE;
            dev->dma.funcs.xfer_start(dev->dma.hndl); // Start DMA
            if (rcar_i2c_wait_complete(dev) == EOK) {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: DMA Completed.", dev->chan_idx, __FUNCTION__);
                atomic_set(&dev->status, I2C_STATUS_DONE);
            }
            else {
                rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: DMA Failed.", dev->chan_idx, __FUNCTION__);
                atomic_set(&dev->status, I2C_STATUS_ERROR);
            }
        }

        /* Arbitration Lost Interrupt */
        if (msr & RCAR_ICMSR_MAL) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MAL interrupts.", dev->chan_idx, __FUNCTION__);
            atomic_set(&dev->status, I2C_STATUS_ARBL | I2C_STATUS_DONE);
            break;
        }

        /* No ACK Interrupt */
        if (msr & RCAR_ICMSR_MNR) {
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: MNR interrupts.", dev->chan_idx, __FUNCTION__);
            atomic_set(&dev->status, I2C_STATUS_NACK | I2C_STATUS_DONE);
            break;
        }

        if(timeout-- < 0) {
            atomic_set(&dev->status, I2C_STATUS_ERROR);
            rcar_i2c_slogf(dev, VERBOSE_LEVEL4, "[chan %d]%s: Waiting for event failed.", dev->chan_idx, __FUNCTION__);
            break;
        }

        nanospin_ns(dev->scl_period);
    }

    if (dev->status == I2C_STATUS_DONE)
        return EOK;
    else
        return EIO;
}

i2c_status_t
rcar_i2c_dma_recv(rcar_i2c_dev_t *dev)
{
    dma_transfer_t  tinfo;
    dma_addr_t      saddr[2], daddr[2];
    int             i;
    int             desc_count = 1;

    //dev->mode |= CMODE_DODMACONT;

    rcar_i2c_slogf(dev, VERBOSE_LEVEL8, "[chan %d]%s: %s ", dev->chan_idx, __FUNCTION__,
        (dev->mode & CMODE_DODMACONT) ? "DMA Master Continuous mode." : "");

    memset(&tinfo, 0, sizeof (tinfo));

    memset(dev->dma.buf.vaddr, 0, dev->len);

    if(dev->mode & CMODE_DODMACONT) {
        for(i = 0; i < desc_count; i++) {
            daddr[i].len    = dev->len / desc_count;
            daddr[i].paddr  = dev->dma.buf.paddr + ((daddr[i].len) * i);
        }
        tinfo.dst_flags     = DMA_ADDR_FLAG_SEGMENTED;
        tinfo.dst_fragments = desc_count;
    } else {
        daddr[0].paddr      = dev->dma.buf.paddr;
    }

    saddr[0].paddr       = dev->physbase + RCAR_I2C_ICRXD;

    tinfo.xfer_unit_size = 1;
    tinfo.xfer_bytes     = dev->mode & CMODE_DODMACONT ? dev->len : dev->len - 2; /* Last two bytes needs to use PIO in order for the STOP phase to work */
    tinfo.src_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
    tinfo.dst_addrs      = &daddr[0];
    tinfo.src_addrs      = &saddr[0];
    tinfo.req_id         = dev->dma.rx_reqid;

    dev->dma.funcs.setup_xfer(dev->dma.hndl, &tinfo);

    if (dev->mode & CMODE_DODMACONT) {
        out32(dev->regbase + RCAR_I2C_ICDMAER, RCAR_I2C_ICDMAER_MDMACTSZ(desc_count) // Number of unit
                    | RCAR_I2C_ICDMAER_RMDMATSZ(dev->len / desc_count) // Unit size
                    | RCAR_I2C_ICDMAER_RMDMACE | RCAR_ICDMAER_RMDMAE);
    }

    out32(dev->regbase + RCAR_I2C_ICMAR, (dev->slave_addr << 1) | 1);
    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_PHASE_START);
    out32(dev->regbase + RCAR_I2C_ICMSR, 0);
    out32(dev->regbase + RCAR_I2C_ICMIER, RCAR_I2C_ICMIER_RECV & ((dev->mode & CMODE_DODMACONT) ? ~RCAR_ICMSR_MST : 0x7F));

    if (dev->mode & CMODE_DODMACONT) {
        if (rcar_dma_cont_wait_complete(dev) == EOK) {
            memcpy(dev->buf, (void *)(dev->dma.buf.vaddr), dev->len);
        }

        dev->dma.funcs.xfer_complete(dev->dma.hndl);

        out32(dev->regbase + RCAR_I2C_ICDMAER, 0);
        out32(dev->regbase + RCAR_I2C_ICMIER, 0);
        out32(dev->regbase + RCAR_I2C_ICMSR, 0);
        out32(dev->regbase + RCAR_I2C_ICMCR, 0);
    } else {
        if (rcar_dma_wait_complete(dev) == EOK) {
            memcpy(dev->buf, (void *)(dev->dma.buf.vaddr), dev->len - 2);
            dev->buf += dev->len - 2;
            dev->len = 2;
        }

        dev->dma.funcs.xfer_complete(dev->dma.hndl);
        dev->mode &= ~CMODE_DODMA;
        out32(dev->regbase + RCAR_I2C_ICDMAER, 0);

        rcar_i2c_wait_complete(dev);
    }

    return dev->status;
}

i2c_status_t
rcar_i2c_dma_send(rcar_i2c_dev_t *dev)
{
    dma_transfer_t  tinfo = { 0 };
    dma_addr_t      saddr[2], daddr[2];
    int             desc_count = 1, i;

    //dev->mode |= CMODE_DODMACONT;

    rcar_i2c_slogf(dev, VERBOSE_LEVEL8, "[chan %d]%s: %s ", dev->chan_idx, __FUNCTION__,
        (dev->mode & CMODE_DODMACONT) ? "DMA Master Continuous mode." : "");

    // The first byte is written to ICTXD register by PIO
    memcpy((void *)dev->dma.buf.vaddr, dev->buf + 1, dev->len - 1);

    if (dev->mode & CMODE_DODMACONT) {
        for (i = 0; i < desc_count; i++) {
            saddr[i].len     = (dev->len - 1) / desc_count;
            saddr[i].paddr   = dev->dma.buf.paddr + saddr[i].len * i;
        }
        tinfo.src_flags      = DMA_ADDR_FLAG_SEGMENTED;
        tinfo.src_fragments  = desc_count;
    } else {
        saddr[0].paddr       = dev->dma.buf.paddr;
    }

    daddr[0].paddr           = dev->physbase + RCAR_I2C_ICTXD;

    tinfo.xfer_unit_size = 1;
    tinfo.xfer_bytes     = dev->len - 1;
    tinfo.src_addrs      = &saddr[0];
    tinfo.dst_addrs      = &daddr[0];
    tinfo.dst_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
    tinfo.req_id         = dev->dma.tx_reqid;

    dev->dma.funcs.setup_xfer(dev->dma.hndl, &tinfo);

    if (dev->mode & CMODE_DODMACONT)
        out32(dev->regbase + RCAR_I2C_ICDMAER, RCAR_I2C_ICDMAER_MDMACTSZ(desc_count) // 2 units
                    | RCAR_I2C_ICDMAER_TMDMATSZ((dev->len - 1) / desc_count) // Unit size
                    | RCAR_I2C_ICDMAER_TMDMACE | RCAR_ICDMAER_TMDMAE);

    out32(dev->regbase + RCAR_I2C_ICMSR, 0);
    out32(dev->regbase + RCAR_I2C_ICMIER, RCAR_I2C_ICMIER_SEND & ((dev->mode & CMODE_DODMACONT) ? ~RCAR_ICMSR_MST : 0x7F));
    out32(dev->regbase + RCAR_I2C_ICMAR, (dev->slave_addr << 1) | 0);
    out32(dev->regbase + RCAR_I2C_ICTXD, *dev->buf); // The first byte is written to ICTXD register by PIO

    ++dev->buf; --dev->len;

    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_I2C_ICMCR_PHASE_START);

    if (dev->mode & CMODE_DODMACONT) {
        if (rcar_dma_cont_wait_complete(dev) == EOK) {
        }
        dev->dma.funcs.xfer_complete(dev->dma.hndl);

        out32(dev->regbase + RCAR_I2C_ICDMAER, 0);
        out32(dev->regbase + RCAR_I2C_ICMIER, 0);
        out32(dev->regbase + RCAR_I2C_ICMSR, 0);
        out32(dev->regbase + RCAR_I2C_ICMCR, 0);
    } else {
        if(rcar_dma_wait_complete(dev) == EOK) {
            dev->buf += dev->len;
            dev->len = 0;
        }

        dev->dma.funcs.xfer_complete(dev->dma.hndl);
        dev->mode &= ~CMODE_DODMA;
        out32(dev->regbase + RCAR_I2C_ICDMAER, 0);

        rcar_i2c_wait_complete(dev);
    }

    return dev->status;
}
static void create_dma_attach_opt(rcar_i2c_dev_t *dev, char* const dma_attach_opt)
{
    char dma_type_name[16];

    if ((dev->product_id == PRODUCT_ID_RCAR_V3H) &&
        ((dev->physbase == RCAR_I2C3_BASE) ||
         (dev->physbase == RCAR_I2C4_BASE))) {
        strlcpy(dma_type_name, "dma=rt", sizeof(dma_type_name));
    } 
    else {
        strlcpy(dma_type_name, "dma=sys", sizeof(dma_type_name));
    }

    if ((dev->dma.chan_min != -1) && (dev->dma.chan_max != -1)) {
        if (dev->dma.typed_mem) {
            sprintf(dma_attach_opt, "%s,range_min=%d,range_max=%d,typed_mem=%s", dma_type_name, dev->dma.chan_min, dev->dma.chan_max, dev->dma.typed_mem);
        }
        else {
            sprintf(dma_attach_opt, "%s,range_min=%d,range_max=%d", dma_type_name, dev->dma.chan_min, dev->dma.chan_max);
        }
    }
    else {
        if (dev->dma.typed_mem) {
            sprintf(dma_attach_opt, "%s,typed_mem=%s", dma_type_name, dev->dma.typed_mem);
        }
        else {
            strlcpy(dma_attach_opt, dma_type_name, sizeof(dma_type_name));
        }
    }
}

int rcar_i2c_dma_init(rcar_i2c_dev_t *dev)
{
    char   dma_attach_opt[256];
    int    flags = DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE;
    int    pri = 0;
    struct smmu_map_entry   entry[2];

    if ((dev->dma.coid = ConnectAttach(0, 0, dev->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
        return -1;
    }

    SIGEV_PULSE_INIT(&dev->dma.event, dev->dma.coid, RCAR_I2C_PRIORITY, RCAR_I2C_DMA_EVENT, NULL);

    if (get_dmafuncs(&dev->dma.funcs, sizeof(dev->dma.funcs)) == -1) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Failed to get DMA lib functions", dev->chan_idx, __FUNCTION__);
        return -1;
    }

    if(dev->dma.smmu) {
        dev->dma.smmu_obj = smmu_obj_create(SOCF_NONE);
        if(dev->dma.smmu_obj == NULL) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Failed to create SMMU object\n", dev->chan_idx, __FUNCTION__);
            return -1;
        }
    }

    dev->dma.funcs.init(NULL);

    create_dma_attach_opt(dev, dma_attach_opt);

    dev->dma.hndl = dev->dma.funcs.channel_attach_smmu(dma_attach_opt,
                        &dev->dma.event, NULL, pri, flags, dev->dma.smmu_obj);
    if (dev->dma.hndl == NULL) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Unable to attach to DMA Channel", dev->chan_idx, __FUNCTION__);
        goto fail1;
    }

    if (dev->dma.funcs.alloc_buffer(dev->dma.hndl, &dev->dma.buf, RCAR_I2C_DMA_BUF_SIZE, DMA_BUF_FLAG_NOCACHE) != 0) {
        dev->dma.funcs.channel_release(dev->dma.hndl);
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Unable to allocate DMA buffer\n", dev->chan_idx, __FUNCTION__);
        goto fail2;
    }

    if(dev->dma.smmu_obj) {
        /* Add SMMU mapping for TX RX FIFO */
        entry[0].phys = dev->physbase + RCAR_I2C_ICTXD;
        entry[0].len = 1;
        entry[1].phys = dev->physbase + RCAR_I2C_ICRXD;
        entry[1].len = 1;
        smmu_mapping_add(dev->dma.smmu_obj, SMF_READ|SMF_WRITE, 0, 2, entry, 0);
    }

    return EOK;

fail2:
    dev->dma.funcs.channel_release(dev->dma.hndl);
    dev->dma.funcs.fini();
fail1:
    if(dev->dma.smmu_obj != NULL) {
        smmu_obj_destroy(dev->dma.smmu_obj);
    }
    return (-1);
}

void rcar_i2c_dma_deinit(rcar_i2c_dev_t *dev)
{
    dev->dma.funcs.channel_release(dev->dma.hndl);
    dev->dma.funcs.free_buffer(dev->dma.hndl, &dev->dma.buf);
    dev->dma.funcs.fini();
    if(dev->dma.smmu_obj != NULL) {
        smmu_obj_destroy(dev->dma.smmu_obj);
    }
    if (dev->dma.typed_mem) {
        free((void *)dev->dma.typed_mem);
    }
}

