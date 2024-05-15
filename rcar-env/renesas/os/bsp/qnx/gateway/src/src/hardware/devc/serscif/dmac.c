/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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
#include "smmu.h"


static void * my_pulse_thread (void *args)
{
    struct _pulse pulse;
    DEV_SCIF  *dev = args;
    unsigned   bytes_left;

    for (;;)
    {
        if (MsgReceivePulse (dev->dma.chid, &pulse, sizeof (pulse), NULL) == -1)
        {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: MsgReceivePulse() error %s",__func__, strerror (errno));
        }

        switch (pulse.code) {
            case RCAR_SCIF_RDMA_EVENT:
                bytes_left = dev->dma.dmafuncs.bytes_left(dev->dma.Rx.dma_chn);
                dev->dma.Rx.byte_cnt = dev->dma.Rx.xfer_size - bytes_left;

                if (bytes_left == 0) {
                    dev->dma.dmafuncs.xfer_complete(dev->dma.Rx.dma_chn);
                } else {
                    dev->dma.dmafuncs.xfer_abort(dev->dma.Rx.dma_chn);
                }
                dev->dma.Rx.status = 0;

                /* If we are hw flow controlled, don't read the char's.
                 * This causes the fifo to fill up, and the the scif chip
                 * automatically handles hw flow control.
                 */
                if (!dev->rts_flag)
                {
                    /* Send data in DMA RX buffer to io-char and schedule next RX transfer */
                    dev->dma.Rx.status = dma_process_rx(dev);
                    /* Enable TO intr */
                    set_port16(dev->port + dev->sc_reg, SCIF_SCSCR_TOIE, SCIF_SCSCR_TOIE);
                }
                break;

            case RCAR_SCIF_TDMA_EVENT:
                if (dev->dma.dmafuncs.bytes_left(dev->dma.Tx.dma_chn) != 0) {
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: Tx DMA is not completed properly",__func__);
                    return (NULL);
                }

                dev->dma.dmafuncs.xfer_complete(dev->dma.Tx.dma_chn);
                dev->tty.un.s.tx_tmr = 0;
                dev->dma.Tx.dma_busy = 0;

                /* Send event to io-char, tto() will be processed at thread time */
                atomic_set(&dev->tty.flags, EVENT_TTO);
                iochar_send_event (&dev->tty);
                break;
        }
    }

    return (NULL);
}

static void create_dma_attach_opt(DEV_SCIF *dev, char* const dma_attach_opt)
{
    if ((dev->dma.chan_min != -1) && (dev->dma.chan_max != -1)) {
        if (dev->dma.typed_mem) {
            sprintf(dma_attach_opt, "dma=sys,range_min=%d,range_max=%d,typed_mem=%s", dev->dma.chan_min, dev->dma.chan_max, dev->dma.typed_mem);
        }
        else {
            sprintf(dma_attach_opt, "dma=sys,range_min=%d,range_max=%d", dev->dma.chan_min, dev->dma.chan_max);
        }
    }
    else {
        if (dev->dma.typed_mem) {
            sprintf(dma_attach_opt, "dma=sys,typed_mem=%s", dev->dma.typed_mem);
        }
        else {
            strlcpy(dma_attach_opt, "dma=sys", sizeof(dma_attach_opt));
        }
    }
}

int scif_dma_init(DEV_SCIF *dev)
{
    pthread_attr_t attr;
    int     policy;
    struct sched_param param;
    int err;
    dma_channel_query_t chinfo;
    dma_attach_flags    attach_flags;
    struct smmu_map_entry   entry[2];
    char   dma_attach_opt[256];
    unsigned long  priority = 0;

    /* Channel */
    if ((dev->dma.chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1)
        goto fail;

    if ((dev->dma.coid = ConnectAttach(0, 0, dev->dma.chid, _NTO_SIDE_CHANNEL, 0)) == -1)
        goto fail1;

    if (get_dmafuncs(&dev->dma.dmafuncs, sizeof(dev->dma.dmafuncs)) == -1) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: failed to get DMA lib functions", __func__);
        goto fail2;
    }

    if(ttyctrl.flags & USE_SMMU) {
        dev->dma.smmu_obj = smmu_obj_create(SOCF_NONE);
        if(dev->dma.smmu_obj == NULL) {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: failed to create SMMU object");
            goto fail2;
        }
    }

    /* Init */
    if (dev->dma.dmafuncs.init(NULL)!=0)
    {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "error initializing libdma!");
        goto fail2;
    }

    attach_flags =  DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE;

    create_dma_attach_opt(dev, dma_attach_opt);

    /* TX */
    dev->dma.Tx.event.sigev_notify   = SIGEV_PULSE;
    dev->dma.Tx.event.sigev_coid     = dev->dma.coid;
    dev->dma.Tx.event.sigev_code     = RCAR_SCIF_TDMA_EVENT;
    dev->dma.Tx.event.sigev_priority = RCAR_SCIF_PRIORITY;

    dev->dma.Tx.dma_chn = dev->dma.dmafuncs.channel_attach_smmu(dma_attach_opt, &dev->dma.Tx.event, NULL, priority, attach_flags, dev->dma.smmu_obj);
    if (dev->dma.Tx.dma_chn == NULL) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: Unable to attach to DMA Channel", __func__);
        goto fail2;
    }

    dev->dma.dmafuncs.query_channel( dev->dma.Tx.dma_chn, &chinfo );
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "%s: TX irq = %d dmac_rs = 0x%x", __func__, chinfo.irq, dev->dma.Tx.dmac_rs);

    if (dev->dma.dmafuncs.alloc_buffer(dev->dma.Tx.dma_chn, &dev->dma.Tx.dbuf, dev->dma.Tx.xfer_size, DMA_BUF_FLAG_NOCACHE) != 0) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: Unable to allocate DMA buffer", __func__);
        goto fail3;
    }

    /* RX */
    dev->dma.Rx.event.sigev_notify   = SIGEV_PULSE;
    dev->dma.Rx.event.sigev_coid     = dev->dma.coid;
    dev->dma.Rx.event.sigev_code     = RCAR_SCIF_RDMA_EVENT;
    dev->dma.Rx.event.sigev_priority = RCAR_SCIF_PRIORITY;

    dev->dma.Rx.dma_chn = dev->dma.dmafuncs.channel_attach_smmu(dma_attach_opt, &dev->dma.Rx.event, NULL, priority, attach_flags, dev->dma.smmu_obj);
    if (dev->dma.Rx.dma_chn == NULL) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: Unable to attach to DMA Channel", __func__);
        goto fail3;
    }

    dev->dma.dmafuncs.query_channel( dev->dma.Rx.dma_chn, &chinfo );
    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "%s: RX irq = %d dmac_rs = 0x%x", __func__, chinfo.irq, dev->dma.Rx.dmac_rs);

    /* Allocate 2x the transfer size for ping-pong buffer (only required for recv size) */
    if (dev->dma.dmafuncs.alloc_buffer(dev->dma.Rx.dma_chn, &dev->dma.Rx.dbuf, dev->dma.Rx.xfer_size*2, DMA_BUF_FLAG_NOCACHE) != 0) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: Unable to allocate DMA buffer", __func__);
        goto fail4;
    }

    /* Create thread to wait for event */
    pthread_attr_init (&attr);
    pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_getschedparam (pthread_self (), &policy, &param);
    param.sched_priority = sched_get_priority_max(SCHED_RR);
    pthread_attr_setschedparam (&attr, &param);
    pthread_attr_setschedpolicy (&attr, SCHED_RR);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

    if ((err = pthread_create (&dev->dma.tid, &attr, my_pulse_thread, dev)) != EOK)
    {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "hscif: %s: unable to attach to pulse (%s)", __func__, strerror (err));
        goto fail4;
    }

    if(dev->dma.smmu_obj) {
        /* Add SMMU mapping for TX RX FIFO */
        entry[0].phys = dev->dma.Rx.fifo_addr;
        entry[0].len = 1;
        entry[1].phys = dev->dma.Tx.fifo_addr;
        entry[1].len = 1;
        smmu_mapping_add(dev->dma.smmu_obj, SMF_READ|SMF_WRITE, 0, 2, entry, 0);
    }

    return (EOK);

fail4:
    dev->dma.dmafuncs.channel_release(dev->dma.Rx.dma_chn);
fail3:
    dev->dma.dmafuncs.channel_release(dev->dma.Tx.dma_chn);
fail2:
    if(dev->dma.smmu_obj != NULL) {
        smmu_obj_destroy(dev->dma.smmu_obj);
    }
    ConnectDetach(dev->dma.coid);
fail1:
    ChannelDestroy(dev->dma.chid);
fail:
    return (-1);
}
