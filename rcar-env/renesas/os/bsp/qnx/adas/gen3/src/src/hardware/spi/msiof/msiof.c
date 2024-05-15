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
#include <stdint.h>
#include <hw/sysinfo.h>
#include <hw/hwinfo_rcar.h>
#include <drvr/hwinfo.h>
#include "msiof.h"

extern int product_id;
extern int product_rev;

static char *msiof_opts[] = {
    "base",     /* Base address for this MSIOFx controller */
    "clock",    /* Clock source of the SPI */
    "channel",  /* Channel in the HWInfo to use */
    "irq",      /* IRQ number for this MSIOFx intereface */
    "dma",      /* Use DMAC for Tx and Rx */
    "num_cs",   /* Number of supported devices */
    "blksize",  /* Transceiver block size */
    "smmu",     /* Connnect to SMMU */
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
	{
	0x00,
	"MSIFO-DEV0",
	{	(8 | SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF | SPI_MODE_BODER_MSB),
		400000
		}
	},
	{
	0x01,
	"MSIFO-DEV1",
	{	(8 | SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF | SPI_MODE_BODER_MSB),
		400000
		}
	},
	{
	0x02,
	"MSIFO-DEV2",
	{	(8 | SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF | SPI_MODE_BODER_MSB),
		400000
		}
	}
};

/*
 *   basic function
 */

void spi_dump(msiof_spi_t *msiof)
{
	fprintf(stderr, "RCAR_MSIOF_TMDR1 = %x\n",in32(msiof->vbase + RCAR_MSIOF_TMDR1));
	fprintf(stderr, "RCAR_MSIOF_TMDR2 = %x\n",in32(msiof->vbase + RCAR_MSIOF_TMDR2));
	fprintf(stderr, "RCAR_MSIOF_TMDR3 = %x\n",in32(msiof->vbase + RCAR_MSIOF_TMDR3));
	fprintf(stderr, "RCAR_MSIOF_RMDR1 = %x\n",in32(msiof->vbase + RCAR_MSIOF_RMDR1));
	fprintf(stderr, "RCAR_MSIOF_RMDR2 = %x\n",in32(msiof->vbase + RCAR_MSIOF_RMDR2));
	fprintf(stderr, "RCAR_MSIOF_RMDR3 = %x\n",in32(msiof->vbase + RCAR_MSIOF_RMDR3));
	fprintf(stderr, "RCAR_MSIOF_TSCR = %x\n",in16(msiof->vbase + RCAR_MSIOF_TSCR));
	fprintf(stderr, "RCAR_MSIOF_CTR = %x\n",in32(msiof->vbase + RCAR_MSIOF_CTR));
	fprintf(stderr, "RCAR_MSIOF_STR = %x\n",in32(msiof->vbase + RCAR_MSIOF_STR));
	fprintf(stderr, "RCAR_MSIOF_IER = %x\n",in32(msiof->vbase + RCAR_MSIOF_IER));
	fprintf(stderr, "RCAR_MSIOF_FCTR = %x\n",in32(msiof->vbase + RCAR_MSIOF_FCTR));
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

static void msiof_spi_write_fifo(msiof_spi_t *msiof, int device,
				      const void *tx_buf, int len)
{
	const uint8_t *buf_8 = tx_buf;
	const uint16_t *buf_16 = tx_buf;
	const uint32_t *buf_32 = tx_buf;
	int bitlen = devlist[device].cfg.mode & SPI_MODE_CHAR_LEN_MASK;
	int k, fs = 32 - bitlen;

        size_t word_count = (in32(msiof->vbase + RCAR_MSIOF_FCTR) >> FCTR_TFUA_BP) & FCTR_TFUA_MSK;
        if (word_count < len) {
            len = word_count ;
        }
        for (k = 0; k < len; k++){
            switch (bitlen){
                case 8:
                    out32(msiof->vbase + RCAR_MSIOF_TFDR, buf_8[k] << fs);
                    break;
                case 16:
                    if (devlist[device].cfg.mode & SPI_MODE_BODER_MSB) {
                        out32(msiof->vbase + RCAR_MSIOF_TFDR, ENDIAN_BE16(buf_16[k]) << fs);
                    } else {
                        out32(msiof->vbase + RCAR_MSIOF_TFDR, buf_16[k] << fs);
                    }
                    break;
                case 32:
                default:
                    if (devlist[device].cfg.mode & SPI_MODE_BODER_MSB) {
                        out32(msiof->vbase + RCAR_MSIOF_TFDR, ENDIAN_BE32(buf_32[k]) << fs);
                    } else {
                        out32(msiof->vbase + RCAR_MSIOF_TFDR, buf_32[k] << fs);
                    }
                    break;
            }
        }
}

static void msiof_spi_read_fifo(msiof_spi_t *msiof, int device,
				     void *rx_buf, int len)
{
	uint8_t *buf_8 = rx_buf;
	uint16_t *buf_16 = rx_buf;
	uint32_t *buf_32 = rx_buf;
	int bitlen = devlist[device].cfg.mode & SPI_MODE_CHAR_LEN_MASK;
	int k, fs = 32 - bitlen;

	for (k = 0; k < len;){
		if (in32(msiof->vbase + RCAR_MSIOF_STR) & STR_RDREQ){
			switch (bitlen){
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
}

static int msiof_spi_xfer(msiof_spi_t *msiof, int device,
				  const void *tx_buf, void *rx_buf,
				  int len)
{
	int ret, dr2;
	int bitlen = devlist[device].cfg.mode & SPI_MODE_CHAR_LEN_MASK;
	uint32_t words = 0;

	if (len >= msiof->blksize)
		len = msiof->blksize;

	/* Configure SPI Tx/Rx mode */
	words = len * 8 / bitlen;
	dr2 = ((bitlen - 1) << 24) | ((words - 1) << 16);
	out32(msiof->vbase + RCAR_MSIOF_TMDR2, dr2);
	out32(msiof->vbase + RCAR_MSIOF_RMDR2, dr2);
	/* Enable interrupts */
	out32(msiof->vbase + RCAR_MSIOF_IER, IER_TEOF | IER_REOF);

	/* setup clock and rx/tx signals */
	/* Enable TX CLK */
	ret = msiof_spi_update_ctr(msiof, 0, CTR_TSCKE);
	/* write tx_buf to fifo */
	msiof_spi_write_fifo(msiof, device, tx_buf, len);

	ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_TXE);
	ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_RXE);
	ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_TFSE);
	if (ret) {
		fprintf(stderr, "Failed to start hardware\n");
		spi_dump(msiof);
		goto fail;
	}
	/* wait for tx fifo to be emptied and rx fifo to be filled */
	msiof_wait(msiof, len);

	/* read rx_buf from fifo */
	msiof_spi_read_fifo(msiof, device, rx_buf, len);

	/* clear status bits */
	out32(msiof->vbase + RCAR_MSIOF_STR, in32(msiof->vbase + RCAR_MSIOF_STR));

	/* Disable Tx/Rx */
	ret = msiof_spi_update_ctr(msiof, CTR_TFSE, 0);
	ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_TXE, 0);
	ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_RXE, 0);
	ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_TSCKE, 0);
	if (ret) {
		fprintf(stderr, "Failed to shutdown hardware\n");
		spi_dump(msiof);
		goto fail;
	}

	return len;

fail:
	/* Clear interrupts */
	out32(msiof->vbase + RCAR_MSIOF_IER, 0);
	return ret;
}

static void query_hwi_device(msiof_spi_t *msiof, unsigned interface)
{
    hwi_tag *tag;
    unsigned hwi_off = hwi_find_bus(RCAR_HWI_MSIOF, interface);
    if(hwi_off != HWI_NULL_OFF){
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag){
            msiof->pbase = tag->location.base;
        }
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, 0);
        if(tag){
            msiof->irq_spi = tag->irq.vector;
        }
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, 0);
        if(tag){
            msiof->clock = tag->inputclk.clk/tag->inputclk.div;
        }
    }
}

static int msiof_spi_options(msiof_spi_t *msiof, char *optstring) {
	int opt, rc = 0;
	char *options, *freeptr, *c, *value;

	if (optstring == NULL)
		return 0;

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
                msiof->dma = 1;
                continue;                
		case 5:
			msiof->num_cs = strtoul(value, 0, 0);
			continue;
		case 6:
			msiof->blksize = strtoul(value, 0, 0);
			continue;
		case 7:    // smmu
                if (value) {
                    if ((!strcmp(value, "on"))||(!strcmp(value, "1"))) {
                        if(smmu_init(0) == -1) {
                            fprintf(stderr, "MSIOF: failed to connect to SMMU Manager\n");
                        } else {
                            msiof->smmu = 1;
                        }
                    } else if ((!strcmp(value, "off"))||(!strcmp(value, "0"))) {
                        msiof->smmu = 0;
                    }
                }
                continue;
		}
		error: fprintf(stderr, "msiof-spi: unknown option %s", c);
		rc = -1;
	}

	free(freeptr);

	return rc;
}


static int get_product_id()
{
    const char *options = NULL;
#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (options == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        options = cs_machine_str;
    }
#endif
    product_id = PRODUCT_ID_RCAR_UNKNOWN;
    product_rev = PRODUCT_REV_UNKNOWN;
    if (options) {
        if ( strstr( options, "H3") || strstr( options, "h3") ) {
            product_id = PRODUCT_ID_RCAR_H3;
            if(strstr( options, "1.0")) {
                product_rev = PRODUCT_REV_1_0;
            } else if(strstr( options, "1.1")) {
            	product_rev = PRODUCT_REV_1_1;
            } else if(strstr( options, "2.0")) {
            	product_rev = PRODUCT_REV_2_0;
            } else if(strstr( options, "3.0")) {
            	product_rev = PRODUCT_REV_3_0;
            }
        } else if ( strstr( options, "M3N") || strstr( options, "m3n") ) {
            product_id = PRODUCT_ID_RCAR_M3N;
        } else if ( strstr( options, "M3") || strstr( options, "m3") ) {
            product_id = PRODUCT_ID_RCAR_M3W;
        } else if ( strstr( options, "D3") || strstr( options, "d3") ) {
            product_id = PRODUCT_ID_RCAR_D3;
        } else if ( strstr( options, "E3") || strstr( options, "e3") ) {
            product_id = PRODUCT_ID_RCAR_E3;
        } else if ( strstr( options, "V3M") || strstr( options, "v3m") ) {
            product_id = PRODUCT_ID_RCAR_V3M;
        } else if ( strstr( options, "V3H") || strstr( options, "v3h") ) {
            product_id = PRODUCT_ID_RCAR_V3H;
        } else if ( strstr( options, "V3U") || strstr( options, "v3u") ) {
            product_id = PRODUCT_ID_RCAR_V3U;
        } else {
            fprintf(stderr,"%s : Unknown SoC\n", __func__);
            return -EIO;       
        }
    }

    return EOK;
}

void *msiof_init(void *hdl, char *options) {
	msiof_spi_t *dev;
	uintptr_t base;
	int i;
    int status;

    /* Calibrate nanospin for delays */
    while ((status = nanospin_calibrate(0)) == EINTR);
    if (status != EOK) {
        fprintf (stderr, "MSIOF SPI: nanospin_calibrate failed (error = %d)\n", status);
    }

	dev = calloc(1, sizeof(msiof_spi_t));
	if (dev == NULL)
		return NULL;
	/* Set defaults */
	dev->pbase = -1;
	dev->irq_spi = -1;
	dev->clock = MSIOF_SPI_CLK_RATE;
	dev->channel_num = -1;
	dev->num_cs = sizeof(devlist)/sizeof(spi_devinfo_t);
	dev->spi.hdl = hdl;
	dev->blksize = 1;
	dev->smmu = 0;

	ThreadCtl(_NTO_TCTL_IO, 0);

	if (msiof_spi_options(dev, options)) {
        fprintf(stderr, "bad options\n");
		goto fail0;
    }

	if (dev->pbase == -1 || dev->irq_spi == -1) {
		fprintf(stderr, "MSIOF SPI: Can't attach MSIOF channel %d !!!\n", dev->channel_num);
		goto fail0;
	}

	/*
	 * Map in SPI registers
	 */
	if ((base = mmap_device_io(MSIOF_SPI_REGLEN, dev->pbase)) == (uintptr_t) MAP_FAILED) {
        fprintf(stderr, "failed map\n");
		goto fail0;
    }
	dev->vbase = base;

	/*
	 * Calculate all device configuration here
	 */
	for (i = 0; i < dev->num_cs; i++) {
		msiof_setcfg(dev, i, &devlist[i].cfg);
	}

	/* disable interrupts */
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
    if (dev->dma) {
        if (msiof_spi_dma_init(dev)) {
            goto fail2;
        }
    }

    if (get_product_id()) {
        goto fail2;
    }

    return dev;
    
fail2:
    if (dev->dma) {
        dev->dmafuncs.free_buffer(dev->tx_dma, &dev->pdmabuf);
        dev->dmafuncs.channel_release(dev->rx_dma);
        dev->dmafuncs.channel_release(dev->tx_dma);
    }
    if(dev->smmu_obj != NULL) {
        smmu_obj_destroy(dev->smmu_obj);
    }
fail1:    
    munmap_device_io(dev->vbase, MSIOF_SPI_REGLEN);
fail0:
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
    if (dev->dma) {
        dev->dmafuncs.free_buffer(dev->tx_dma, &dev->pdmabuf);
        dev->dmafuncs.channel_release(dev->rx_dma);
        dev->dmafuncs.channel_release(dev->tx_dma);
    }
    if(dev->smmu_obj != NULL) {
        smmu_obj_destroy(dev->smmu_obj);
    }    
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
	uint32_t	control;

	if (device >= dev->num_cs)
		return (EINVAL);

	memcpy(&devlist[device].cfg, cfg, sizeof(spi_cfg_t));

	control = msiof_cfg(hdl, cfg);
	if (control < 0) {
		return (EINVAL);
	}

	dev->mode[device] = control;

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
		if (id >= 0 && id < dev->num_cs)
			memcpy(info, &devlist[id], sizeof(spi_devinfo_t));
		else
			return (EINVAL);
	} else {
		/*
		 * Info of next device
		 */
		if (id == SPI_DEV_ID_NONE)
			id = -1;
		if (id < (dev->num_cs - 1))
			memcpy(info, &devlist[id + 1], sizeof(spi_devinfo_t));
		else
			return (EINVAL);
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

	id = device & SPI_DEV_ID_MASK;
	if (id >= dev->num_cs) {
		*len = -1;
		fprintf(stderr, "MSIOF SPI: No support device = %d !!!", device);
		id = 0;
	}
	dev->xlen = *len;
	dev->pbuf = buf;
	/* Reset registers */
	mask = CTR_TXRST | CTR_RXRST;
	out32(dev->vbase + RCAR_MSIOF_CTR, in32(dev->vbase + RCAR_MSIOF_CTR) | mask);
	for (int k = 100; k > 0; k--) {
		if (!(in32(dev->vbase + RCAR_MSIOF_CTR) & mask))
			break;

		nanospin_ns(CTR_DELAY_NS);
	}
	// Cannot set more than 64KB of data at one time
	if (dev->xlen > (64 * 1024)) {
		*len = -1;
		return buf;
	}

	// Estimate transfer time in us... The calculated dtime is only used for
	// the timeout, so it doesn't have to be that accurate.  At higher clock
	// rates, a calculated dtime of 0 would mess-up the timeout calculation, so
	// round up to 1 us
	dev->dtime = dev->xlen * 1000 * 1000 / devlist[id].cfg.clock_rate;
	if (dev->dtime == 0)
		dev->dtime = 1;

	/* start the data transmit.....this happens by writing data to
	 * the corresponding transmit register. This module has been
	 * designed for Transmit/Recieve Mode. This part will change
	 * according to the design.
	 */
	msiof_setup(dev, id, devlist[id].cfg.clock_rate, devlist[id].cfg.mode);

	/* transfer in fifo sized chunks */
	len_tx_rx_once = dev->xlen;
	bytes_done = 0;

	while (bytes_done < dev->xlen) {
		void *rx_buf = dev->pbuf ? dev->pbuf + bytes_done : NULL;
		const void *tx_buf = dev->pbuf ? dev->pbuf + bytes_done : NULL;

		n = msiof_spi_xfer(dev, id, tx_buf, rx_buf, len_tx_rx_once);
		if (n < 0)
			break;

		bytes_done += n;
		len_tx_rx_once -= n;
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
		fprintf(stderr, "MSIOF : No support device %d", device);
		return -1;
	}

    /* Check DMA enable */
    if (dev->dma == 0) {
		fprintf(stderr, "MSIOF : No DMA transfer specificcation\n");
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
		if (!(in32(dev->vbase + RCAR_MSIOF_CTR) & mask))
			break;

		nanospin_ns(CTR_DELAY_NS);
	}

    dlen = ((devlist[device].cfg.mode & SPI_MODE_CHAR_LEN_MASK) + 7) >> 3;
	dev->xlen = len;

	// Estimate transfer time in us... The calculated dtime is only used for
	// the timeout, so it doesn't have to be that accurate.  At higher clock
	// rates, a calculated dtime of 0 would mess-up the timeout calculation, so
	// round up to 1 us
	dev->dtime = dev->xlen * 1000 * 1000 / devlist[id].cfg.clock_rate;
	if (dev->dtime == 0)
		dev->dtime = 1;

	/* start the data transmit.....this happens by writing data to
	 * the corresponding transmit register. This module has been
	 * designed for Transmit/Recieve Mode. This part will change
	 * according to the design.
	 */
	msiof_setup(dev, id, devlist[id].cfg.clock_rate, devlist[id].cfg.mode);

    len_left = len;

    while (len_left > 0) {
        len_tx_rx_once = min(len_left, dev->blksize * 4);

        n = msiof_spi_dma_xfer(dev, paddr, len - len_left, dlen, len_tx_rx_once);
        
        if (n < 0) return  -1;
        
        len_left -= n;
    }

	return len;
}
