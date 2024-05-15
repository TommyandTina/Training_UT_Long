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

#include "rcar_rpc.h"

static int rpc_dma_unit_size(int len)
{
    if (!(len & 0x3F))
        return 32;
    else if (!(len & 0x1F))
        return 32;
    else if (!(len & 7))
        return 8;
    else if (!(len & 3))
        return 4;
    else if (!(len & 1))
        return 2;
    else
        return 1;
}

/*
 * setup DMA transfer
 */
int rpc_dma_setup(rpc_dev_t *dev, int len, int dir, int offset)
{
    rcar_rpc_t     *rpc = dev->rpc;
    dma_transfer_t  tinfo;
    dma_addr_t      saddr, daddr;

    memset(&tinfo, 0, sizeof (tinfo));

    tinfo.xfer_unit_size = rpc_dma_unit_size(len);

    tinfo.xfer_bytes     = len;

    if (dir == RCAR_RPC_WRITE) {
        saddr.paddr     = rpc->dbuf.paddr + offset;
        tinfo.src_addrs = &saddr;
        daddr.paddr     = rpc->pbase + RCAR_RPC_WRITE_BUF_OFF;
        tinfo.dst_addrs = &daddr;

        rpc->dmafuncs.setup_xfer(rpc->txdma, &tinfo);
    } else {
        saddr.paddr     = rpc->address + offset;
        tinfo.src_addrs = &saddr;
        daddr.paddr     = rpc->dbuf.paddr;
        tinfo.dst_addrs = &daddr;

        rpc->dmafuncs.setup_xfer(rpc->rxdma, &tinfo);
    }

    return (len);
}

static int rpc_wait(rpc_dev_t *dev, int len)
{
    rcar_rpc_t     *rpc = dev->rpc;
    struct _pulse   pulse;
    uint64_t        to = len;

    to *= 2048 * 2048;

    while (1) {
       TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_RECEIVE, NULL, &to, NULL);

        if (MsgReceivePulse(rpc->chid, &pulse, sizeof(pulse), NULL) == -1) {
            fprintf(stderr, "RCAR_RPC: XFER error! sts = %#x\n", in32(rpc->vbase + RCAR_RPC_CMNSR));
            return (errno = ETIMEDOUT);
        }

        switch (pulse.code) {
            case RCAR_RPC_RDMA_EVENT:
                if (rpc->dmafuncs.bytes_left(rpc->rxdma) != 0) {
                    fprintf(stderr, "RCAR_RPC: Rx DMA is not completed properly\n");
                    return (EIO);
                }
                rpc->dmafuncs.xfer_complete(rpc->rxdma);
                return (EOK);
            case RCAR_RPC_TDMA_EVENT:
                if (rpc->dmafuncs.bytes_left(rpc->txdma) != 0) {
                    fprintf(stderr, "RCAR_RPC: Tx DMA is not completed properly\n");
                    return (EIO);
                }
                rpc->dmafuncs.xfer_complete(rpc->txdma);
                return (EOK);
        }
    }
}

/*
 * DMA transfer
 */
int rpc_dma_xfer(rpc_dev_t *dev, int len, int dir)
{
    rcar_rpc_t *rpc = dev->rpc;
    int         rc;

    if (dir == RCAR_RPC_WRITE) {
        rpc->dmafuncs.xfer_start(rpc->txdma);

        if ((rc = rpc_wait(dev, len)) != EOK)
            rpc->dmafuncs.xfer_abort(rpc->txdma);
    } else {
        rpc->dmafuncs.xfer_start(rpc->rxdma);

        if ((rc = rpc_wait(dev, len)) != EOK)
            rpc->dmafuncs.xfer_abort(rpc->rxdma);
    }

    return (rc);
}

int rcar_dma_init(rcar_rpc_t *rpc)
{
    struct sigevent         event;
#ifdef USE_SMMU
    struct smmu_map_entry   entry[2];
#endif
    char                    attach_opt[100] = "dma=sys";

    if (get_dmafuncs(&rpc->dmafuncs, sizeof(rpc->dmafuncs)) == -1) {
        fprintf(stderr, "RCAR_RPC: Failed to get DMA lib functions\n");
        goto fail1;
    }
#ifdef USE_SMMU
    if (rpc->smmu) {
        rpc->smmu_obj = smmu_obj_create(SOCF_NONE);
        if (rpc->smmu_obj == NULL) {
            fprintf(stderr, "RCAR_RPC: Failed to create SMMU object\n");
            goto fail1;
        }
    }
#endif
    rpc->dmafuncs.init(rpc->dma_ver);

    if (rpc->dma_typed_mem) {
        int len = strlen(attach_opt);
        snprintf(attach_opt+len, sizeof(attach_opt)-len, ",typed_mem=%s", rpc->dma_typed_mem);
    }

    event.sigev_notify   = SIGEV_PULSE;
    event.sigev_coid     = rpc->coid;
    event.sigev_code     = RCAR_RPC_TDMA_EVENT;
    event.sigev_priority = RCAR_RPC_PRIORITY;
#ifdef USE_SMMU
    if ((rpc->txdma = rpc->dmafuncs.channel_attach_smmu(attach_opt, &event, NULL, 0,
                                    DMA_ATTACH_PRIORITY_HIGHEST | DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE, rpc->smmu_obj)) == NULL) {
        fprintf(stderr, "RCAR_RPC: Unable to attach to DMA Channel");
        goto fail1;
    }
#else
    if ((rpc->txdma = rpc->dmafuncs.channel_attach(attach_opt, &event, NULL, 0,
                                    DMA_ATTACH_PRIORITY_HIGHEST | DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE)) == NULL) {
        fprintf(stderr, "RCAR_RPC: Unable to attach to DMA Channel");
        goto fail1;
    }
#endif
    if (rpc->dmafuncs.alloc_buffer(rpc->txdma, &rpc->dbuf, RCAR_RPC_DMABUF_SIZE, DMA_BUF_FLAG_NOCACHE) != 0) {
        fprintf(stderr, "RCAR_RPC: Unable to allocate DMA buffer");
        goto fail2;
    }

    event.sigev_notify   = SIGEV_PULSE;
    event.sigev_coid     = rpc->coid;
    event.sigev_code     = RCAR_RPC_RDMA_EVENT;
    event.sigev_priority = RCAR_RPC_PRIORITY;
#ifdef USE_SMMU
    if ((rpc->rxdma = rpc->dmafuncs.channel_attach_smmu(attach_opt, &event, NULL, 1,
                                    DMA_ATTACH_PRIORITY_HIGHEST | DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE, rpc->smmu_obj)) == NULL) {
        fprintf(stderr, "RCAR_RPC: Unable to attach to DMA Channel");
        goto fail2;
    }
    if (rpc->smmu_obj != NULL) {
        /* Add SMMU mapping for write buffer and socket */
        entry[0].phys = rpc->address; // Address Map for external reading
        entry[0].len = rpc->window_size;
        entry[1].phys = rpc->pbase + RCAR_RPC_WRITE_BUF_OFF; // Write buffer
        entry[1].len = RCAR_RPC_WRITE_BUF_SIZE;
        smmu_mapping_add (rpc->smmu_obj, SMF_READ|SMF_WRITE, 0, 2, entry, 0);
    }
#else
    if ((rpc->rxdma = rpc->dmafuncs.channel_attach(attach_opt, &event, NULL, 1,
                                    DMA_ATTACH_PRIORITY_HIGHEST | DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE)) == NULL) {
        fprintf(stderr, "RCAR_RPC: Unable to attach to DMA Channel");
        goto fail2;
    }
#endif
    return (EOK);

fail2:
    if (rpc->rxdma) {
        rpc->dmafuncs.channel_release(rpc->rxdma);
    }
    if (rpc->txdma) {
        rpc->dmafuncs.free_buffer(rpc->txdma, &rpc->dbuf);
        rpc->dmafuncs.channel_release(rpc->txdma);
    }
    rpc->dmafuncs.fini();
fail1:
#ifdef USE_SMMU
    if (rpc->smmu_obj != NULL) {
        smmu_obj_destroy(rpc->smmu_obj);
    }
#endif
    return (-1);
}
