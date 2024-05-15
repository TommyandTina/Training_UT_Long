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

#include "msiof.h"

extern int msiof_spi_update_ctr(msiof_spi_t *msiof, uint32_t clr_bits, uint32_t set_bits);
void spi_dump(msiof_spi_t *msiof);
void * product_type();

/*
 * Request DMA transfer
 */
int msiof_spi_dma_init(void *hdl)
{
    msiof_spi_t             *dev = hdl;
    struct sigevent         event;
    struct smmu_map_entry   entry[2];
    char *dma_opts = NULL;
    uint32_t priority = 0;
    uint32_t flags = DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE;

    if((dma_opts = product_type()) == NULL) {
        return (-1);
    }

    if (get_dmafuncs(&dev->dmafuncs, sizeof(dev->dmafuncs)) == -1) {
        fprintf(stderr, "MSIOF : Failed to get DMA lib functions\n");
        return (-1);
    }

    if(dev->smmu) {
        dev->smmu_obj = smmu_obj_create(SOCF_NONE);
        if(dev->smmu_obj == NULL) {
            fprintf(stderr, "MSIOF : %s: failed to create SMMU object\n", __FUNCTION__);
            return -1;
        }
    }

    dev->dmafuncs.init(NULL);

    for (int i = 0; i < dev->num_cs; i++) {     
        switch (dev->pbase) {
            case RCAR_GEN3_MSIOF0_BASE:
                dev->tx_mid_rid = RCAR_GEN3_DREQ_MSIOF0_TXI;
                dev->rx_mid_rid = RCAR_GEN3_DREQ_MSIOF0_RXI;
                if(strstr(dma_opts, "v3u") != NULL){
                    flags |= DMA_ATTACH_PRIORITY_ATLEAST;
                    priority = MSIOF_SPI_DMA_PRI;
                }
                break;
            case RCAR_GEN3_MSIOF1_BASE:
                dev->tx_mid_rid = RCAR_GEN3_DREQ_MSIOF1_TXI;
                dev->rx_mid_rid = RCAR_GEN3_DREQ_MSIOF1_RXI;
                if((strstr(dma_opts, "v3u") != NULL) || 
                   (strstr(dma_opts, "e3") != NULL)){
                    flags |= DMA_ATTACH_PRIORITY_ATLEAST;
                    priority = MSIOF_SPI_DMA_PRI;
                }
                break;
            case RCAR_GEN3_MSIOF2_BASE:
                dev->tx_mid_rid = RCAR_GEN3_DREQ_MSIOF2_TXI;
                dev->rx_mid_rid = RCAR_GEN3_DREQ_MSIOF2_RXI;
                flags |= DMA_ATTACH_PRIORITY_ATLEAST;
                priority = MSIOF_SPI_DMA_PRI;
                break;
            case RCAR_GEN3_MSIOF3_BASE:
                dev->tx_mid_rid = RCAR_GEN3_DREQ_MSIOF3_TXI;
                dev->rx_mid_rid = RCAR_GEN3_DREQ_MSIOF3_RXI;
                flags |= DMA_ATTACH_PRIORITY_ATLEAST;
                priority = MSIOF_SPI_DMA_PRI;
                break;
            case RCAR_GEN3_MSIOF4_BASE:
                dev->tx_mid_rid = RCAR_GEN3_DREQ_MSIOF4_TXI;
                dev->rx_mid_rid = RCAR_GEN3_DREQ_MSIOF4_RXI;
                if(strstr(dma_opts, "v3u") != NULL){
                    flags |= DMA_ATTACH_PRIORITY_ATLEAST;
                    priority = MSIOF_SPI_DMA_PRI;
                }
                break;
            case RCAR_GEN3_MSIOF5_BASE:
                dev->tx_mid_rid = RCAR_GEN3_DREQ_MSIOF5_TXI;
                dev->rx_mid_rid = RCAR_GEN3_DREQ_MSIOF5_RXI;
                if(strstr(dma_opts, "v3u") != NULL){
                    flags |= DMA_ATTACH_PRIORITY_ATLEAST;
                    priority = MSIOF_SPI_DMA_PRI;
                }
                break;
        }
    }

    event.sigev_notify   = SIGEV_PULSE;
    event.sigev_coid     = dev->coid;
    event.sigev_code     = MSIOF_SPI_TXDMA_EVENT;
    event.sigev_priority = MSIOF_SPI_PRIORITY;

    if ((dev->tx_dma = dev->dmafuncs.channel_attach_smmu(dma_opts, &event, NULL, priority,
                                    flags, dev->smmu_obj)) == NULL) {
        fprintf(stderr, "MSIOF : Unable to attach to DMA Channel\n");
        goto fail1;
    }
    
    event.sigev_notify   = SIGEV_PULSE;
    event.sigev_coid     = dev->coid;
    event.sigev_code     = MSIOF_SPI_RXDMA_EVENT;
    event.sigev_priority = MSIOF_SPI_PRIORITY;

    if ((dev->rx_dma = dev->dmafuncs.channel_attach_smmu(dma_opts, &event, NULL, priority,
                                    flags, dev->smmu_obj)) == NULL) {
        fprintf(stderr, "MSIOF : Unable to attach to DMA Channel\n");
        dev->dmafuncs.channel_release(dev->tx_dma);
        goto fail2;
    }

    if (dev->dmafuncs.alloc_buffer(dev->tx_dma, &dev->pdmabuf, dev->blksize * 4, DMA_BUF_FLAG_NOCACHE) != 0) {
        fprintf(stderr, "MSIOF : Unable to allocate DMA buffer");
        dev->dmafuncs.channel_release(dev->tx_dma);
        dev->dmafuncs.channel_release(dev->rx_dma);
        goto fail3;
    }

    if(dev->smmu_obj) {
        /* Add SMMU mapping for TX RX FIFO */
        entry[0].phys = dev->pbase + RCAR_MSIOF_TFDR;
        entry[0].len = 1;
        entry[1].phys = dev->pbase + RCAR_MSIOF_RFDR;
        entry[1].len = 1;
        smmu_mapping_add(dev->smmu_obj, SMF_READ|SMF_WRITE, 0, 2, entry, 0);
    }

    return (EOK);

fail3:
    dev->dmafuncs.channel_release(dev->tx_dma);
    dev->dmafuncs.channel_release(dev->rx_dma);
fail2:
    dev->dmafuncs.channel_release(dev->tx_dma);
fail1:
    if(dev->smmu_obj != NULL) {
        smmu_obj_destroy(dev->smmu_obj);
    }
    return -1;
}


/*
 * setup DMA transfer
 */
int msiof_spi_dma_xfer(void *hdl, spi_dma_paddr_t *paddr, int len_done, int dlen, int len)
{
    msiof_spi_t    *dev = hdl;
    dma_transfer_t tinfo;
    dma_addr_t     saddr, daddr;
    uint32_t       ier_bits = 0, dr2;
    int            ret = 0;

    /* TX setup */
    ier_bits |= IER_TDREQE | IER_TDMAE;
    memset(&tinfo, 0, sizeof (tinfo));
    tinfo.xfer_unit_size = dlen;              /* 8/16/32 bit transfer size */
    tinfo.xfer_bytes     = len;               /* Total transfer bytes */
    tinfo.req_id         = dev->tx_mid_rid;
    if (paddr->wpaddr)
        saddr.paddr      = paddr->wpaddr + len_done;
    else
        saddr.paddr      = dev->pdmabuf.paddr;
    tinfo.src_addrs      = &saddr;
    daddr.paddr          = dev->pbase + RCAR_MSIOF_TFDR;
    tinfo.dst_addrs      = &daddr;
    tinfo.dst_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
    dev->dmafuncs.setup_xfer(dev->tx_dma, &tinfo);

    /* RX setup */
    ier_bits |= IER_RDREQE | IER_RDMAE;
    memset(&tinfo, 0, sizeof (tinfo));
    tinfo.xfer_unit_size = dlen;               /* 8/16/32 bit transfer size */
    tinfo.xfer_bytes     = len;                /* Total transfer bytes */
    tinfo.req_id         = dev->rx_mid_rid;
    saddr.paddr          = dev->pbase + RCAR_MSIOF_RFDR;
    tinfo.src_addrs      = &saddr;
    tinfo.src_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
    if (paddr->rpaddr)
        daddr.paddr      = paddr->rpaddr + len_done;
    else
        daddr.paddr      = dev->pdmabuf.paddr;
    tinfo.dst_addrs      = &daddr;
    dev->dmafuncs.setup_xfer(dev->rx_dma, &tinfo);

    /* 1 stage FIFO watermarks for DMA */
    out32(dev->vbase + RCAR_MSIOF_FCTR, FCTR_TFWM_1 | FCTR_RFWM_1);

    /* Setup msiof transfer mode registers (32-bit words) */
    dr2 = MDR2_BITLEN1(dlen*8) | MDR2_WDLEN1(len/dlen);
    out32(dev->vbase + RCAR_MSIOF_TMDR2, dr2);
    out32(dev->vbase + RCAR_MSIOF_RMDR2, dr2);

    /* Enable interrupt */
    out32(dev->vbase + RCAR_MSIOF_IER, ier_bits);

    /* Now start DMA */
    dev->dmafuncs.xfer_start(dev->rx_dma);
    dev->dmafuncs.xfer_start(dev->tx_dma);

    /* Enable TX/RX */
    ret = msiof_spi_update_ctr(dev, 0, CTR_TSCKE);
    ret = ret ? ret : msiof_spi_update_ctr(dev, 0, CTR_RXE);
    ret = ret ? ret : msiof_spi_update_ctr(dev, 0, CTR_TXE);
    ret = ret ? ret : msiof_spi_update_ctr(dev, 0, CTR_TFSE);

    if (ret) {
        fprintf(stderr, "MSIOF : Failed to start hardware\n");
        spi_dump(dev);
        goto fail;
    }

    /* Wait for tx DMA completion */
    ret = msiof_wait(dev, len);
    if (ret)
        goto fail;

    /* Wait for rx DMA completion */
    ret = msiof_wait(dev, len);
    if (ret)
        goto fail;

    /* clear status bits */
    out32(dev->vbase + RCAR_MSIOF_STR, in32(dev->vbase + RCAR_MSIOF_STR));

    /* Disable Tx/Rx */
    ret = msiof_spi_update_ctr(dev, CTR_TFSE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(dev, CTR_TXE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(dev, CTR_RXE, 0);    
    ret = ret ? ret : msiof_spi_update_ctr(dev, CTR_TSCKE, 0);
    if (ret) {
        fprintf(stderr, "MSIOF : Failed to shut down hardware\n");
        spi_dump(dev);
        goto fail;
    }

    out32(dev->vbase + RCAR_MSIOF_IER, 0);

	return len;

fail:
	out32(dev->vbase + RCAR_MSIOF_STR, in32(dev->vbase + RCAR_MSIOF_STR));

    dev->dmafuncs.xfer_abort(dev->tx_dma);
    dev->dmafuncs.xfer_abort(dev->rx_dma);

	out32(dev->vbase + RCAR_MSIOF_IER, 0);

	return ret;
}

void * product_type()
{
    const char *options = NULL;
    char *dma_opts = NULL;
#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (options == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        options = cs_machine_str;
    }
#endif
    if (options) {
        if ( strstr( options, "H3") || strstr( options, "h3") ) {
            dma_opts = "ver=h3,dma=sys";
        } else if ( strstr( options, "M3N") || strstr( options, "m3n") ) {
            dma_opts = "ver=m3n,dma=sys";
        } else if ( strstr( options, "M3") || strstr( options, "m3") ) {
            dma_opts = "ver=m3,dma=sys";
        } else if ( strstr( options, "D3") || strstr( options, "d3") ) {
            dma_opts = "ver=d3,dma=sys";
        } else if ( strstr( options, "E3") || strstr( options, "e3") ) {
            dma_opts = "ver=e3,dma=sys";
        } else if ( strstr( options, "V3M") || strstr( options, "v3m") ) {
            dma_opts = "ver=v3m,dma=sys";
        } else if ( strstr( options, "V3H") || strstr( options, "v3h") ) {
            fprintf(stderr, "MSIOF: Rcar-V3H does not support DMA mode\n");
            return NULL;
        } else if ( strstr( options, "V3U") || strstr( options, "v3u") ) {
            dma_opts = "ver=v3u,dma=sys";
        } else {
            fprintf(stderr,"%s : Unknown SoC\n", __func__);
            return NULL;       
        }
    }   

    return dma_opts;
}
