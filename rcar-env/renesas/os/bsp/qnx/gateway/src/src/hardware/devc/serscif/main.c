/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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
#include <stdio.h>

/* Called from io-char's cleanup thread */
/* To use cleanup thread, set ttyctrl flag CREATE_CLEANUP_THREAD */
int dev_cleanup(TTYDEV *ttydev)
{
    DEV_SCIF *dev = (DEV_SCIF *)ttydev;

    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "%s: Cleaning up device %s",
          __FUNCTION__, ttydev->name);

    if(dev->dma.status == DMA_ENABLE)
    {
        dev->dma.dmafuncs.xfer_abort(dev->dma.Rx.dma_chn);
        dev->dma.dmafuncs.xfer_abort(dev->dma.Tx.dma_chn);
        dev->dma.dmafuncs.channel_release(dev->dma.Rx.dma_chn);
        dev->dma.dmafuncs.channel_release(dev->dma.Tx.dma_chn);
        dev->dma.dmafuncs.fini();
        dev->dma.dmafuncs.free_buffer(dev->dma.Tx.dma_chn, &dev->dma.Tx.dbuf);
        dev->dma.dmafuncs.free_buffer(dev->dma.Rx.dma_chn, &dev->dma.Rx.dbuf);

        if(dev->dma.smmu_obj != NULL) {
            smmu_obj_destroy(dev->dma.smmu_obj);
        }
    }

    if (dev->dma.typed_mem) {
        free((void *)dev->dma.typed_mem);
    }

    return EOK;
}

static void *term_thread (void *arg)
{
    sigset_t signals;
    sigemptyset (&signals);
    sigaddset (&signals, SIGTERM);
    sigwaitinfo (&signals, NULL);

    disable_uart();

    _exit (0);
}


int main(int argc, char *argv[])
{
    pthread_attr_t attr;
    sigset_t signals;

    pthread_attr_init (&attr);

    sigfillset (&signals);
    pthread_sigmask (SIG_BLOCK, &signals, NULL);

    pthread_create (NULL, &attr, term_thread, NULL);

    /*
     * Set the max # of devices and the
     * priority that the driver runs at
     */
    ttyctrl.max_devs = 16;
    ttc(TTC_INIT_PROC, &ttyctrl, 24);

    if(options(argc, argv) == 0) {
        fprintf(stderr, "%s: No serial ports found\n", argv[0]);
        exit(0);
    }

    ttc(TTC_INIT_START, &ttyctrl, 0);

    return 0;
}

