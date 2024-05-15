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

#include <hw/sysinfo.h>
#include <hw/hwinfo_rcar.h>
#include <drvr/hwinfo.h>
#include <gulliver.h>
#include <inttypes.h>
#include <hw/rcar_cpg.h>
#include "msiof.h"

static cpg_mgr_funcs_t cpg_hwfuncs;

static char *msiof_opts[] = {
    "base",     /* Base address for this MSIOFx controller */
    "clock",    /* defined the value of the clock source of the SPI */
    "channel",  /* defines the channel in the HWInfo to use */
    "irq",      /* IRQ for this MSIOFx intereface */
    "dma",      /* Use DMAC for Tx and Rx */
    "num_cs",   /* number of devices supported */
    "blksize",  /* Transceiver's block size */
    "typed_mem", /* Memory type for DMA transfer buffer */
#ifdef USE_SMMU
    "smmu",     /* Connnect to SMMU */
#endif
    NULL
};

spi_funcs_t spi_drv_entry = {
    sizeof(spi_funcs_t),
    msiof_init,        /* init() */
    msiof_dinit,       /* fini() */
    msiof_drvinfo,     /* drvinfo() */
    msiof_devinfo,     /* devinfo() */
    msiof_setcfg,      /* setcfg() */
    msiof_xfer,        /* xfer() */
    msiof_dma_xfer,    /* dma_xfer() */
};

/*
 * Note:
 * The devices listed are just examples, users should change
 * this according to their own hardware spec.
 */
static spi_devinfo_t devlist[NUM_OF_SPI_DEVS] = {
    { // Connect to SYNC pin
        0x00,
        "MSIOF-DEV0",
        {	(8 | SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF | SPI_MODE_BODER_MSB),
            1000000
        }
    },
    { // Connect to SS1 pin
        0x01,
        "MSIOF-DEV1",
        {	(8 | SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF | SPI_MODE_BODER_MSB),
            1000000
        }
    },
    { // Connect to SS2 pin
        0x02,
        "MSIOF-DEV2",
        {	(8 | SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF | SPI_MODE_BODER_MSB),
            1000000
        }
    },
};

/*
 *   basic function
 */

void spi_dump(msiof_spi_t *msiof)
{
    fprintf(stderr, "RCAR_MSIOF_TMDR1 = %x\n", in32(msiof->vbase + RCAR_MSIOF_TMDR1));
    fprintf(stderr, "RCAR_MSIOF_TMDR2 = %x\n", in32(msiof->vbase + RCAR_MSIOF_TMDR2));
    fprintf(stderr, "RCAR_MSIOF_TMDR3 = %x\n", in32(msiof->vbase + RCAR_MSIOF_TMDR3));
    fprintf(stderr, "RCAR_MSIOF_RMDR1 = %x\n", in32(msiof->vbase + RCAR_MSIOF_RMDR1));
    fprintf(stderr, "RCAR_MSIOF_RMDR2 = %x\n", in32(msiof->vbase + RCAR_MSIOF_RMDR2));
    fprintf(stderr, "RCAR_MSIOF_RMDR3 = %x\n", in32(msiof->vbase + RCAR_MSIOF_RMDR3));
    fprintf(stderr, "RCAR_MSIOF_TSCR  = %x\n", in16(msiof->vbase + RCAR_MSIOF_TSCR));
    fprintf(stderr, "RCAR_MSIOF_CTR   = %x\n", in32(msiof->vbase + RCAR_MSIOF_CTR));
    fprintf(stderr, "RCAR_MSIOF_STR   = %x\n", in32(msiof->vbase + RCAR_MSIOF_STR));
    fprintf(stderr, "RCAR_MSIOF_IER   = %x\n", in32(msiof->vbase + RCAR_MSIOF_IER));
    fprintf(stderr, "RCAR_MSIOF_FCTR  = %x\n", in32(msiof->vbase + RCAR_MSIOF_FCTR));
}

int msiof_spi_update_ctr(msiof_spi_t *msiof, uint32_t clr_bits, uint32_t set_bits) {
    uint32_t mask = clr_bits | set_bits;
    uint32_t data;
    int k;

    data = in32(msiof->vbase + RCAR_MSIOF_CTR);
    data &= ~clr_bits;
    data |= set_bits;
    out32(msiof->vbase + RCAR_MSIOF_CTR, data);
    for (k = 100; k > 0; k--) {
        if ((in32(msiof->vbase + RCAR_MSIOF_CTR) & mask) == set_bits)
            break;

        nanospin_ns(CTR_DELAY_NS);
    }

    return k > 0 ? 0 : -ETIMEDOUT;
}

static void msiof_spi_write_fifo(msiof_spi_t *msiof, const void *tx_buf, int len, int bitlen)
{
    const uint8_t *buf_8 = tx_buf;
    const uint16_t *buf_16 = tx_buf;
    const uint32_t *buf_32 = tx_buf;
    int k, fs = 32 - bitlen;

    size_t word_count = (in32(msiof->vbase + RCAR_MSIOF_FCTR) >> FCTR_TFUA_BP) & FCTR_TFUA_MSK;
    if (word_count < len) {
        len = word_count ;
    }

    for (k = 0; k < len; k++) {
        switch (bitlen) {
            case 8:
                out32(msiof->vbase + RCAR_MSIOF_TFDR, buf_8[k] << fs);
                break;
            case 16:
                out32(msiof->vbase + RCAR_MSIOF_TFDR, buf_16[k] << fs);
                break;
            case 32:
            default:
                out32(msiof->vbase + RCAR_MSIOF_TFDR, buf_32[k] << fs);
                break;
        }
    }
}

static int msiof_spi_read_fifo(msiof_spi_t *msiof, void *rx_buf, int len, int bitlen)
{
    uint8_t *buf_8 = rx_buf;
    uint16_t *buf_16 = rx_buf;
    uint32_t *buf_32 = rx_buf;
    int k, fs = 32 - bitlen;

    for (k = 0; k < len;) {
        if (in32(msiof->vbase + RCAR_MSIOF_STR) & STR_RDREQ) {
            switch (bitlen) {
                case 8:
                    buf_8[k] = in32(msiof->vbase + RCAR_MSIOF_RFDR) >> fs;
                    break;
                case 16:
                    buf_16[k] = in32(msiof->vbase + RCAR_MSIOF_RFDR) >> fs;
                    break;
                case 32:
                default:
                    buf_32[k] = in32(msiof->vbase + RCAR_MSIOF_RFDR) >> fs;
                    break;
            }
            k++;
        }
    }
    return k;
}

static int msiof_spi_xfer(msiof_spi_t *msiof, int device,
                  const void *tx_buf, void *rx_buf, int len)
{
    int ret = 0;
    uint32_t dr2 = 0;
    int bitlen = devlist[device].cfg.mode & SPI_MODE_CHAR_LEN_MASK;
    uint32_t word_count = 0;

    /* Configure Word Count, Data Size, Group Count */
    word_count = len * 8 / bitlen;
    dr2 = ((bitlen - 1) << 24) | ((word_count - 1) << 16);
    out32(msiof->vbase + RCAR_MSIOF_TMDR2, dr2);
    out32(msiof->vbase + RCAR_MSIOF_RMDR2, dr2);
    /* Enable interrupts */
    out32(msiof->vbase + RCAR_MSIOF_IER, IER_TEOF | IER_REOF);

    /* Setup clock and rx/tx signals */
    /* Enable TX CLK */
    ret = msiof_spi_update_ctr(msiof, 0, CTR_TSCKE);
    /* Write tx_buf to fifo */
    msiof_spi_write_fifo(msiof, tx_buf, word_count, bitlen);

    ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_TXE);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_RXE);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_TFSE);
    if (ret) {
        fprintf(stderr, "MSIOF : Failed to start hardware\n");
        spi_dump(msiof);
        goto fail;
    }
    /* Wait for tx fifo to be emptied and rx fifo to be filled */
    if (msiof_wait(msiof,len )) {
        len = -1;
    } else { /* read rx_buf from fifo */
        word_count = msiof_spi_read_fifo(msiof, rx_buf, word_count, bitlen);
        len = (bitlen / 8) * word_count;
    }
    /* Clear status bits */
    out32(msiof->vbase + RCAR_MSIOF_STR, in32(msiof->vbase + RCAR_MSIOF_STR));

    /* Disable Tx/Rx */
    ret = msiof_spi_update_ctr(msiof, CTR_TFSE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_TXE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_RXE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_TSCKE, 0);
    if (ret) {
        fprintf(stderr, "MSIOF : Failed to shutdown hardware\n");
        spi_dump(msiof);
        goto fail;
    }

    return len;

fail:
    /* Dsiable interrupts */
    out32(msiof->vbase + RCAR_MSIOF_IER, 0);
    return -1;
}

/*
 * Specify parameters for default devices from hwi_info tags.
 */
static void query_hwi_device(msiof_spi_t *msiof, unsigned unit)
{
    unsigned hwi_off;
    unsigned tag_idx;
    hwi_tag  *tag;

    hwi_off = hwi_find_bus(RCAR_HWI_MSIOF, unit);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            msiof->pbase = tag->location.base;
        }
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, 0);
        if (tag) {
            if (!msiof->irq_spi)
                msiof->irq_spi = tag->irq.vector;
        }
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, 0);
        if (tag) {
            msiof->clock = tag->inputclk.clk/tag->inputclk.div;
        }
        if (!msiof->dma.rx_req_id || !msiof->dma.tx_req_id) {
            tag_idx = 0;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                msiof->dma.tx_req_id = (uint8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                msiof->dma.rx_req_id = (uint8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                msiof->dma.chan_min = (int8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                msiof->dma.chan_max = (int8_t) tag->dma.chnl;
            }
        }
    }
}

static int msiof_spi_options(msiof_spi_t *msiof, char *optstring) {
    int opt, rc = 0;
    char *options, *freeptr, *c, *value, *cp;
    int len;

    if (optstring == NULL) {
        return 0;
    }

    msiof->dma.rx_req_id = 0;
    msiof->dma.tx_req_id = 0;
    msiof->dma.chan_min = -1;
    msiof->dma.chan_max = -1;
    msiof->dma.typed_mem = NULL;

    freeptr = options = strdup(optstring);

    while (options && *options != '\0') {
        c = options;
        if ((opt = getsubopt(&options, msiof_opts, &value)) == -1)
            goto error;

        switch (opt) {
        case 0:
            msiof->pbase = strtoull(value, 0, 0);
            continue;
        case 1:
            msiof->clock = strtoul(value, NULL, 0);
            continue;
        case 2:
            msiof->channel_num = strtoul(value, NULL, 0);
            query_hwi_device(msiof, msiof->channel_num);
            continue;
        case 3:
            msiof->irq_spi = strtoul(value, 0, 0);
            continue;
        case 4:
            msiof->dma.status = 1;
            if (value == NULL) continue;
            msiof->dma.tx_req_id = (uint8_t) strtoul(value, NULL, 0);
            cp = strchr(value, ',');
            if (cp == NULL) continue;
            cp++;
            msiof->dma.rx_req_id = (uint8_t) strtoul(cp, NULL, 0);
            cp = strchr(cp, ',');
            if (cp == NULL) continue;
            cp++;
            msiof->dma.chan_min = (int8_t) strtoul(cp, NULL, 0);
            cp = strchr(cp, ',');
            if (cp == NULL) continue;
            cp++;
            msiof->dma.chan_max = (int8_t) strtoul(cp, NULL, 0);
            continue;
        case 5:
            msiof->num_cs = strtoul(value, 0, 0);
            if (msiof->num_cs > NUM_OF_SPI_DEVS) {
                    msiof->num_cs = NUM_OF_SPI_DEVS;
                    fprintf(stderr, "MSIOF : number of SPI device > %d, use %d\n",
                        (int)NUM_OF_SPI_DEVS,(int)NUM_OF_SPI_DEVS);
            }
            continue;
        case 6:
            msiof->blksize = strtoul(value, 0, 0);
            if (msiof->product_id == PRODUCT_ID_RCAR_V4H) {
                if (msiof->blksize > MSIOF_SPI_FIFO_SIZE_256) {
                    msiof->blksize = MSIOF_SPI_FIFO_SIZE_256;
                    fprintf(stderr, "MSIOF : block size > %d, use block size %d\n",
                        (int)MSIOF_SPI_FIFO_SIZE_256,(int)MSIOF_SPI_FIFO_SIZE_256);
                }
            } else {
                if (msiof->blksize > MSIOF_SPI_FIFO_SIZE_64) {
                    msiof->blksize = MSIOF_SPI_FIFO_SIZE_64;
                    fprintf(stderr, "MSIOF : block size > %d, use block size %d\n",
                        (int)MSIOF_SPI_FIFO_SIZE_64,(int)MSIOF_SPI_FIFO_SIZE_64);
                }
            }
            continue;
        case 7:
            if (value) {
                len = strlen(value) + 1;
                msiof->dma.typed_mem = (char *)malloc(len);
                snprintf(msiof->dma.typed_mem, len, "%s", value);
            }
            continue;
#ifdef USE_SMMU
        case 8:    // smmu
            if (value) {
                if ((!strcmp(value, "on"))||(!strcmp(value, "1"))) {
                    if (smmu_init(0) == -1) {
                        fprintf(stderr, "MSIOF: failed to connect to SMMU Manager\n");
                    } else {
                        msiof->dma.smmu = 1;
                    }
                } else if ((!strcmp(value, "off"))||(!strcmp(value, "0"))) {
                    msiof->dma.smmu = 0;
                }
            }
            continue;
#endif
        }

        error: fprintf(stderr, "MSIOF : unknown option %s", c);
        rc = -1;
    }

    if (msiof->dma.status && (!msiof->dma.tx_req_id || !msiof->dma.rx_req_id))  {
        msiof->dma.status = 0; /* use PIO to read/write data */
    }

    free(freeptr);

    return rc;
}

static int get_product_id(msiof_spi_t *dev)
{
    hwi_tag *tag;
    unsigned hwi_off;

    dev->product_id = PRODUCT_ID_RCAR_UNKNOWN;
    dev->product_rev = PRODUCT_REV_UNKNOWN;

    hwi_off = hwi_find_device("r-car", 0);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, NULL);
        if (tag) {
            dev->product_id = tag->hwversion.version;
            dev->product_rev = tag->hwversion.hclass;
        } else {
            fprintf (stderr, "MSIOF : Unable to query product_id from HW info\n");
            return (-1);
        }
    } else {
        fprintf (stderr, "MSIOF : Unable to find product version name from HW info\n");
        return (-1);
    }

    return EOK;
}

void *msiof_init(void *hdl, char *options)
{
    msiof_spi_t *dev;
    uintptr_t base;
    int i;
    
    dev = calloc(1, sizeof(msiof_spi_t));
    if (dev == NULL) {
        fprintf(stderr, "MSIOF : Calloc failed\n");
        return NULL;
    }

   /* Specify product */
    if (get_product_id(dev)) {
        fprintf(stderr, "MSIOF : Unable to get product ID\n");
        goto fail0;
    }

    ThreadCtl(_NTO_TCTL_IO, 0);

    /* Set defaults */
    dev->pbase = 0;
    dev->irq_spi = 0;
    dev->channel_num = -1;
    dev->num_cs = sizeof(devlist)/sizeof(spi_devinfo_t);
    dev->spi.hdl = hdl;
#ifdef USE_SMMU
    dev->dma.smmu = 0;
#endif
    dev->dma.status = 0;

    if (dev->product_id == PRODUCT_ID_RCAR_V4H) {
        dev->blksize = MSIOF_SPI_FIFO_SIZE_256;
    } else {
        dev->blksize = MSIOF_SPI_FIFO_SIZE_64;
    }

    if (msiof_spi_options(dev, options)) {
        goto fail0;
    }

    if (!dev->pbase || !dev->irq_spi) {
        fprintf(stderr, "MSIOF : Unable to attach MSIOF channel %d\n", dev->channel_num);
        goto fail0;
    }

    cpg_mgr_getfuncs(&cpg_hwfuncs, sizeof(cpg_mgr_funcs_t));
    cpg_hwfuncs.init(0);
    if (dev->clock) {
        cpg_hwfuncs.clk_rate_set(CPG_CLK_MSIOF, 0, &dev->clock);
    } else {
        cpg_hwfuncs.clk_rate_get(CPG_CLK_MSIOF, 0, &dev->clock);
    }
    cpg_hwfuncs.deinit();

    /*
     * Map in SPI registers
     */
    if ((base = mmap_device_io(MSIOF_SPI_REGLEN, dev->pbase)) == (uintptr_t) MAP_FAILED) {
        fprintf(stderr, "MSIOF : mmap_device_io for msiof registers failed\n");
        goto fail0;
    }
    dev->vbase = base;
    /*
     * Calculate all device configuration here
     */
    for (i = 0; i < dev->num_cs; i++) {
        msiof_setcfg(dev, i, &devlist[i].cfg);
    }

    /* Disable interrupts */
    out32(dev->vbase + RCAR_MSIOF_IER, 0);
    /*
     * Attach MSIOF interrupt
     */
    if (msiof_attach_intr(dev)) {
        goto fail1;
    }

    /*
     * Request DMAC channels for TX and RX
     */
    if (dev->dma.status) {
        if (dma_init(dev)) {
            goto fail1;
        }
    }

    return dev;

fail1:
    munmap_device_io(dev->vbase, MSIOF_SPI_REGLEN);
fail0:
    if (dev->dma.typed_mem) {
        free(dev->dma.typed_mem);
    }
    free(dev);

    return NULL;
}

void msiof_dinit(void *hdl) {
    msiof_spi_t *dev = hdl;
    /*
     * unmap the register, detach the interrupt
     */
    InterruptDetach(dev->iid_spi);
    munmap_device_io(dev->vbase, MSIOF_SPI_REGLEN);
    ConnectDetach(dev->coid);
    ChannelDestroy(dev->chid);
    if (dev->dma.status) {
        dev->dma.funcs.free_buffer(dev->dma.tx_hdl, &dev->dma.pbuf);
        dev->dma.funcs.channel_release(dev->dma.tx_hdl);
        dev->dma.funcs.channel_release(dev->dma.rx_hdl);
    }
#ifdef USE_SMMU
    if (dev->dma.smmu_obj != NULL) {
        smmu_obj_destroy(dev->dma.smmu_obj);
        smmu_fini();
    }
#endif
    if (dev->dma.typed_mem)
        free(dev->dma.typed_mem);
    free(hdl);
}

int msiof_drvinfo(void *hdl, spi_drvinfo_t *info) {
    info->version = (SPI_VERSION_MAJOR << SPI_VERMAJOR_SHIFT)
            | (SPI_VERSION_MINOR << SPI_VERMINOR_SHIFT) | (SPI_REVISION
            << SPI_VERREV_SHIFT);
    info->feature = SPI_FEATURE_DMA;
    strcpy(info->name, "MSIOF SPI");
    return (EOK);
}

int msiof_setcfg(void *hdl, uint16_t device, spi_cfg_t *cfg) {
    msiof_spi_t *dev = hdl;

    if (device >= dev->num_cs) {
        return (EINVAL);
    }

    memcpy(&devlist[device].cfg, cfg, sizeof(spi_cfg_t));

    if (msiof_cfg(hdl, cfg, &dev->mode[device]) != EOK) {
        return (EINVAL);
    }

    /* Call msiof_setup so that any requested line polarity changes happen now instead of later */
    msiof_setup(dev, device, devlist[device].cfg.clock_rate, devlist[device].cfg.mode);

    return (EOK);
}

int msiof_devinfo(void *hdl, uint32_t device, spi_devinfo_t *info) {
    msiof_spi_t *dev = hdl;
    int id = device & SPI_DEV_ID_MASK;
    if (device & SPI_DEV_DEFAULT) {
        /*
         * Info of this device
         */
        if (id >= 0 && id < dev->num_cs) {
            memcpy(info, &devlist[id], sizeof(spi_devinfo_t));
        } else {
            return (EINVAL);
        }
    } else {
        /*
         * Info of next device
         */
        if (id == SPI_DEV_ID_NONE) {
            id = -1;
        }
        if (id < (dev->num_cs - 1)) {
            memcpy(info, &devlist[id + 1], sizeof(spi_devinfo_t));
        } else {
            return (EINVAL);
        }
    }

    return (EOK);
}

void *msiof_xfer(void *hdl, uint32_t device, uint8_t *buf, int *len) {
    msiof_spi_t *dev = hdl;
    uint32_t id;
    uint32_t mask;
    int bytes_done;
    int n;
    int len_tx_rx_once;
    int bytes_left;

    id = device & SPI_DEV_ID_MASK;
    if (id >= dev->num_cs) {
        *len = -1;
        fprintf(stderr, "MSIOF : No support device = %d\n", device);
        id = 0;
    }
    dev->xlen = *len;
    dev->pbuf = buf;
    /* Reset registers */
    mask = CTR_TXRST | CTR_RXRST;
    out32(dev->vbase + RCAR_MSIOF_CTR, in32(dev->vbase + RCAR_MSIOF_CTR) | mask);
    for (int k = 100; k > 0; k--) {
        if (!(in32(dev->vbase + RCAR_MSIOF_CTR) & mask)) {
            break;
        }

        nanospin_ns(CTR_DELAY_NS);
    }
    // Estimate transfer time in us... The calculated dtime is only used for
    // the timeout, so it doesn't have to be that accurate.  At higher clock
    // rates, a calculated dtime of 0 would mess-up the timeout calculation, so
    // round up to 1 us
    dev->dtime = dev->xlen * 1000 * 1000 / devlist[id].cfg.clock_rate;
    if (dev->dtime == 0) {
        dev->dtime = 1;
    }

    /* start the data transmit.....this happens by writing data to
     * the corresponding transmit register. This module has been
     * designed for Transmit/Recieve Mode. This part will change
     * according to the design.
     */
    msiof_setup(dev, id, devlist[id].cfg.clock_rate, devlist[id].cfg.mode);

    bytes_left = dev->xlen;
    bytes_done = 0;

    while (bytes_done < dev->xlen) {
        void *rx_buf       = dev->pbuf ? dev->pbuf + bytes_done : NULL;
        const void *tx_buf = dev->pbuf ? dev->pbuf + bytes_done : NULL;

        /* transfer in fifo sized chunks */
        len_tx_rx_once = min(bytes_left, dev->blksize * 4);

        n = msiof_spi_xfer(dev, id, tx_buf, rx_buf, len_tx_rx_once);
        if (n < 0) {
            *len = -1;
            break;
        }

        bytes_done += n;
        bytes_left -= n;
    }

    return buf;
}

/*
 * setup DMA transfer
 */
int msiof_dma_xfer(void *hdl, uint32_t device, spi_dma_paddr_t *paddr, int len)
{
    msiof_spi_t    *dev = hdl;
    uint32_t       mask, id;
    int            n, dlen, len_left, len_tx_rx_once;

    /* Check correct CS index */
    id = device & SPI_DEV_ID_MASK;
    if (id >= dev->num_cs) {
        fprintf(stderr, "MSIOF : No support device %d\n", device);
        return -1;
    }

    /* Check DMA enable */
    if (dev->dma.status == 0) {
        fprintf(stderr, "MSIOF : DMA is not enabled!\n");
        return -1;
    }

    if (len < 16) {
        fprintf(stderr, "MSIOF : Length %d no support for DMA transfer\n", len);
        return -1;
    }

    /* Check valid data to send/receive */
    if ((paddr->wpaddr == 0) && (paddr->rpaddr == 0)) {
        fprintf(stderr, "MSIOF : No defined data buffer to send/receive\n");
        return -1;
    }

    /* Reset registers */
    mask = CTR_TXRST | CTR_RXRST;
    out32(dev->vbase + RCAR_MSIOF_CTR, in32(dev->vbase + RCAR_MSIOF_CTR) | mask);
    for (int k = 100; k > 0; k--) {
        if (!(in32(dev->vbase + RCAR_MSIOF_CTR) & mask)) {
            break;
        }

        nanospin_ns(CTR_DELAY_NS);
    }

    dlen = ((devlist[device].cfg.mode & SPI_MODE_CHAR_LEN_MASK) + 7) >> 3;
    dev->xlen = len;

    // Estimate transfer time in us... The calculated dtime is only used for
    // the timeout, so it doesn't have to be that accurate.  At higher clock
    // rates, a calculated dtime of 0 would mess-up the timeout calculation, so
    // round up to 1 us
    dev->dtime = dev->xlen * 1000 * 1000 / devlist[id].cfg.clock_rate;
    if (dev->dtime == 0) {
        dev->dtime = 1;
    }

    /* start the data transmit.....this happens by writing data to
     * the corresponding transmit register. This module has been
     * designed for Transmit/Recieve Mode. This part will change
     * according to the design.
     */
    msiof_setup(dev, id, devlist[id].cfg.clock_rate, devlist[id].cfg.mode);

    len_left = len;

    while (len_left > 0) {
        len_tx_rx_once = min(len_left, dev->blksize * 4);

        n = dma_xfer(dev, paddr, len - len_left, dlen, len_tx_rx_once);

        if (n < 0) {
            return  -1;
        }

        len_left -= n;
    }

    return len;
}
