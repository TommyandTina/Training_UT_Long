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

#include "msiof.h"

void dma_deinit(msiof_spi_t *msiof)
{
    if (msiof->dma.pbuf.vaddr) {
        msiof->dma.funcs.free_buffer(msiof->dma.tx_hdl, &msiof->dma.pbuf);
    }

    if (msiof->dma.tx_hdl) {
        msiof->dma.funcs.channel_release(msiof->dma.tx_hdl);
    }

    if (msiof->dma.rx_hdl) {
        msiof->dma.funcs.channel_release(msiof->dma.rx_hdl);
    }

#ifdef USE_SMMU
    if (msiof->dma.smmu_obj != NULL) {
        smmu_obj_destroy(msiof->dma.smmu_obj);
        smmu_fini();
    }
#endif
    if (msiof->dma.typed_mem) {
        free((void *)msiof->dma.typed_mem);
    }
}

static void create_dma_attach_opt(msiof_spi_t *msiof, char* const dma_attach_opt)
{
    char dma_type_name[16];

    if (msiof->product_id == PRODUCT_ID_RCAR_V3H)
        strlcpy(dma_type_name, "dma=rt", sizeof(dma_type_name));
    else
        strlcpy(dma_type_name, "dma=sys", sizeof(dma_type_name));

    if ((msiof->dma.chan_min != -1) && (msiof->dma.chan_max != -1)) {
        if (msiof->dma.typed_mem) {
            sprintf(dma_attach_opt, "%s,range_min=%d,range_max=%d,typed_mem=%s", dma_type_name, msiof->dma.chan_min, msiof->dma.chan_max, msiof->dma.typed_mem);
        }
        else {
            sprintf(dma_attach_opt, "%s,range_min=%d,range_max=%d", dma_type_name, msiof->dma.chan_min, msiof->dma.chan_max);
        }
    }
    else {
        if (msiof->dma.typed_mem) {
            sprintf(dma_attach_opt, "%s,typed_mem=%s", dma_type_name, msiof->dma.typed_mem);
        }
        else {
            strlcpy(dma_attach_opt, dma_type_name, sizeof(dma_type_name));
        }
    }
}

/*
 * Request DMA transfer
 */
int dma_init(msiof_spi_t *msiof)
{
#ifdef USE_SMMU
    struct smmu_map_entry   entry[2];
#endif
    char   dma_attach_opt[256];
    uint32_t  priority = 0;
    uint32_t  flags = DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE;

    if (get_dmafuncs(&msiof->dma.funcs, sizeof(msiof->dma.funcs)) == -1) {
        fprintf(stderr, "MSIOF : Failed to get DMA lib functions\n");
        goto fail;
    }
#ifdef USE_SMMU
    if (msiof->dma.smmu) {
        msiof->dma.smmu_obj = smmu_obj_create(SOCF_NONE);
        if (msiof->dma.smmu_obj == NULL) {
            fprintf(stderr, "MSIOF : Failed to create SMMU object\n");
            goto fail;
        }
    }
#endif
    create_dma_attach_opt(msiof, dma_attach_opt);

    if (msiof->dma.funcs.init(NULL) != 0) {
        fprintf(stderr, "MSIOF : Error in initializing libdma!\n");
        goto fail;
    }

    SIGEV_PULSE_INIT (&msiof->dma.tx_event, msiof->coid, MSIOF_SPI_PRIORITY, MSIOF_SPI_TXDMA_EVENT, 0);
#ifdef USE_SMMU
    msiof->dma.tx_hdl = msiof->dma.funcs.channel_attach_smmu(dma_attach_opt, &msiof->dma.tx_event, NULL, priority,
                                flags, msiof->dma.smmu_obj);
#else
    msiof->dma.tx_hdl = msiof->dma.funcs.channel_attach(dma_attach_opt, &msiof->dma.tx_event, NULL, priority,
                                flags);
#endif
    if (msiof->dma.tx_hdl == NULL) {
        fprintf(stderr, "MSIOF : Unable to attach to TX DMA Channel\n");
        goto fail1;
    }

    SIGEV_PULSE_INIT (&msiof->dma.rx_event, msiof->coid, MSIOF_SPI_PRIORITY, MSIOF_SPI_RXDMA_EVENT, 0);
#ifdef USE_SMMU
    msiof->dma.rx_hdl = msiof->dma.funcs.channel_attach_smmu(dma_attach_opt, &msiof->dma.rx_event, NULL, priority,
                                    flags, msiof->dma.smmu_obj);
#else
    msiof->dma.rx_hdl = msiof->dma.funcs.channel_attach(dma_attach_opt, &msiof->dma.rx_event, NULL, priority,
                                    flags);
#endif
    if (msiof->dma.rx_hdl == NULL) {
        fprintf(stderr, "MSIOF : Unable to attach to RX DMA Channel\n");
        goto fail2;
    }

    if (msiof->dma.funcs.alloc_buffer(msiof->dma.tx_hdl, &msiof->dma.pbuf, msiof->blksize * 4, DMA_BUF_FLAG_NOCACHE) != 0) {
        fprintf(stderr, "MSIOF : Unable to allocate DMA buffer");
        goto fail3;
    }
#ifdef USE_SMMU
    if (msiof->dma.smmu_obj != NULL) {
        /* Add SMMU mapping for TX RX FIFO */
        entry[0].phys = msiof->pbase + RCAR_MSIOF_TFDR;
        entry[0].len = 1;
        entry[1].phys = msiof->pbase + RCAR_MSIOF_RFDR;
        entry[1].len = 1;
        smmu_mapping_add(msiof->dma.smmu_obj, SMF_READ|SMF_WRITE, 0, 2, entry, 0);
    }
#endif
    return (EOK);

fail3:
    msiof->dma.funcs.channel_release(msiof->dma.rx_hdl);
fail2:
    msiof->dma.funcs.channel_release(msiof->dma.tx_hdl);
fail1:
#ifdef USE_SMMU
    if (msiof->dma.smmu_obj != NULL) {
        smmu_obj_destroy(msiof->dma.smmu_obj);
    }
#endif
fail:
    return -1;
}


/*
 * setup DMA transfer
 */
int dma_xfer(msiof_spi_t *msiof, spi_dma_paddr_t *paddr, int len_done, int dlen, int len)
{
    dma_transfer_t tinfo;
    dma_addr_t     saddr, daddr;
    uint32_t       ier_bits = 0, dr2;
    int            ret = 0;
#ifdef USE_SMMU
    int entry_idx = 0;
    struct smmu_map_entry entry[2];

    if (msiof->dma.smmu_obj != NULL) {
        /* Need SMMU mapping for Read/Write Data Buffer ? */
        if (paddr->wpaddr) {
            entry[entry_idx].phys = paddr->wpaddr;
            entry[entry_idx].len = len;
            entry_idx++;
        }
        if (paddr->rpaddr) {
            entry[entry_idx].phys = paddr->rpaddr;
            entry[entry_idx].len = len;
            entry_idx++;
        }
        if (entry_idx > 0) smmu_mapping_add(msiof->dma.smmu_obj, SMF_READ|SMF_WRITE, 0, entry_idx, entry, 0);
    }
#endif
    /* TX setup */
    ier_bits |= IER_TDREQE | IER_TDMAE;
    memset(&tinfo, 0, sizeof (tinfo));
    tinfo.xfer_unit_size = dlen;              /* 8/16/32 bit transfer size */
    tinfo.xfer_bytes     = len;               /* Total transfer bytes */
    tinfo.req_id         = msiof->dma.tx_req_id;
    if (paddr->wpaddr) {
        saddr.paddr      = paddr->wpaddr + len_done;
    } else {
        saddr.paddr      = msiof->dma.pbuf.paddr;
    }
    tinfo.src_addrs      = &saddr;
    daddr.paddr          = msiof->pbase + RCAR_MSIOF_TFDR;
    tinfo.dst_addrs      = &daddr;
    tinfo.dst_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
    if (msiof->dma.funcs.setup_xfer(msiof->dma.tx_hdl, &tinfo) != 0) {
        fprintf(stderr, "MSIOF : Error in setting up TX DMA transfer!\n");
        goto fail;
    }

    /* RX setup */
    ier_bits |= IER_RDREQE | IER_RDMAE;
    memset(&tinfo, 0, sizeof (tinfo));
    tinfo.xfer_unit_size = dlen;               /* 8/16/32 bit transfer size */
    tinfo.xfer_bytes     = len;                /* Total transfer bytes */
    tinfo.req_id         = msiof->dma.rx_req_id;
    saddr.paddr          = msiof->pbase + RCAR_MSIOF_RFDR;
    tinfo.src_addrs      = &saddr;
    tinfo.src_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
    if (paddr->rpaddr) {
        daddr.paddr      = paddr->rpaddr + len_done;
    } else {
        daddr.paddr      = msiof->dma.pbuf.paddr;
    }
    tinfo.dst_addrs      = &daddr;
    if (msiof->dma.funcs.setup_xfer(msiof->dma.rx_hdl, &tinfo) != 0) {
        fprintf(stderr, "MSIOF : Error in setting up RX DMA transfer!\n");
        goto fail;
    }

    /* 1 stage FIFO watermarks for DMA */
    out32(msiof->vbase + RCAR_MSIOF_FCTR, FCTR_TFWM_1 | FCTR_RFWM_1);

    /* Setup msiof transfer mode registers (32-bit words) */
    dr2 = MDR2_BITLEN1(dlen*8) | MDR2_WDLEN1(len/dlen);
    out32(msiof->vbase + RCAR_MSIOF_TMDR2, dr2);
    out32(msiof->vbase + RCAR_MSIOF_RMDR2, dr2);

    /* Enable interrupt */
    out32(msiof->vbase + RCAR_MSIOF_IER, ier_bits);

    /* Now start DMA */
    if (msiof->dma.funcs.xfer_start(msiof->dma.rx_hdl) != 0) {
        fprintf(stderr, "MSIOF : Error in starting RX DMA transfer!\n");
        goto fail;
    }
    if (msiof->dma.funcs.xfer_start(msiof->dma.tx_hdl) != 0) {
        fprintf(stderr, "MSIOF : Error in starting TX DMA transfer!\n");
        goto fail;
    }

    /* Enable TX/RX */
    ret = msiof_spi_update_ctr(msiof, 0, CTR_TSCKE);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_RXE);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_TXE);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, 0, CTR_TFSE);

    if (ret) {
        fprintf(stderr, "MSIOF : Failed to start hardware\n");
        spi_dump(msiof);
        goto fail;
    }

    /* Wait for tx DMA completion */
    ret = msiof_wait(msiof, len);
    if (ret) {
        fprintf(stderr, "MSIOF : Failed to wait for TX DMA completion\n");
        goto fail;
    }

    /* Wait for rx DMA completion */
    ret = msiof_wait(msiof, len);
    if (ret) {
        fprintf(stderr, "MSIOF : Failed to wait for RX DMA completion\n");
        goto fail;
    }

    /* clear status bits */
    out32(msiof->vbase + RCAR_MSIOF_STR, in32(msiof->vbase + RCAR_MSIOF_STR));

    /* Disable Tx/Rx */
    ret = msiof_spi_update_ctr(msiof, CTR_TFSE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_TXE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_RXE, 0);
    ret = ret ? ret : msiof_spi_update_ctr(msiof, CTR_TSCKE, 0);
    if (ret) {
        fprintf(stderr, "MSIOF : Failed to disable hardware\n");
        spi_dump(msiof);
        goto fail;
    }

    out32(msiof->vbase + RCAR_MSIOF_IER, 0);
#ifdef USE_SMMU
    if (msiof->dma.smmu_obj != NULL) {
        smmu_mapping_add(msiof->dma.smmu_obj, SMF_NONE, 0, 1, entry, 0);
    }
#endif
    return len;

fail:
    out32(msiof->vbase + RCAR_MSIOF_STR, in32(msiof->vbase + RCAR_MSIOF_STR));

    msiof->dma.funcs.xfer_abort(msiof->dma.tx_hdl);
    msiof->dma.funcs.xfer_abort(msiof->dma.rx_hdl);

    out32(msiof->vbase + RCAR_MSIOF_IER, 0);
#ifdef USE_SMMU
    if (msiof->dma.smmu_obj != NULL) {
        if (entry_idx > 0) smmu_mapping_add(msiof->dma.smmu_obj, SMF_NONE, 0, entry_idx, entry, 0);
    }
#endif
    return ret;
}
