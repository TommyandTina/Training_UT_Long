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
#include <time.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <math.h>
#include <string.h>

static cpg_mgr_funcs_t cpg_hwfuncs;

static int rcar_i2c_bus_barrier(rcar_i2c_dev_t *dev)
{
    int i;

    for (i = 0; i < LOOP_TIMEOUT; i++) {
        /* make sure that bus is not busy */
        if (!(in32(dev->regbase + RCAR_I2C_ICMCR) & RCAR_ICMCR_FSDA))
            return 0;
        nanospin_ns(1000);
    }

    /* Waiting did not help, try to recover */
    return EBUSY;
}

static void rcar_i2c_force_stop(rcar_i2c_dev_t *dev, int num)
{
    unsigned retry_count = 0x1000000;

    rcar_i2c_slogf(dev, VERBOSE_LEVEL8, "[chan %d]%s: Force stop I2C bus", dev->chan_idx, __FUNCTION__);

    if (!rcar_i2c_bus_barrier(dev))
        return;

    if (in32(dev->regbase + RCAR_I2C_ICMCR) & RCAR_ICMCR_FSDA) {
        out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_ICMCR_MIE | RCAR_ICMCR_FSB | RCAR_ICMCR_MDBS);
        out32(dev->regbase + RCAR_I2C_ICMSR, 0);

        while (in32(dev->regbase + RCAR_I2C_ICMSR) & RCAR_ICMSR_MDR) {
            in32(dev->regbase + RCAR_I2C_ICRXD);
            out32(dev->regbase + RCAR_I2C_ICMSR, 0);
        }

        while ((retry_count--) && !(in32(dev->regbase + RCAR_I2C_ICMSR) & RCAR_ICMSR_MST));

        out32(dev->regbase + RCAR_I2C_ICMSR, 0);
    }
}

void * rcar_i2c_init(int argc, char *argv[])
{
    rcar_i2c_dev_t  *dev;

    /* Set up privilege */
    if (-1 == ThreadCtl(_NTO_TCTL_IO_PRIV, 0)) {
        perror("ThreadCtl");
        return NULL;
    }

    /* Allocate driver struct */
    dev = calloc(sizeof(rcar_i2c_dev_t), 1);
    if ((!dev)) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "%s: fail malloc of dev", __FUNCTION__);
        return NULL;
    }

    /* Calibrate nanospin for delays */
    if (nanospin_calibrate(0) != EOK) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "%s: fail to calibrate nanospin", __FUNCTION__);
        goto fail;
    }

    /* Defaults */
    dev->iid = -1;
    dev->regbase = ( uintptr_t )MAP_FAILED;
    dev->irq = 0;
    dev->chid = -1;
    dev->coid = -1;
    dev->intr_count = 0;

    /* Parse command-line options */
    if (-1 == rcar_i2c_parse_options(dev, argc, argv)) {
        fprintf(stderr, "Incorrect arguments. Check the use message. \n");
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "%s: fail rcar_i2c_parse_options", __FUNCTION__);
        goto fail;
    }

    if (!dev->physbase || !dev->irq) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: missing physbase or irq", dev->chan_idx, __FUNCTION__);
        goto fail;
    }

    /* Map I2C register space */
    dev->regbase = mmap_device_io(RCAR_I2C_SIZE, dev->physbase);
    if (dev->regbase == (uintptr_t)MAP_FAILED) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: fail mmap_device_io %lx, %d", dev->chan_idx, __FUNCTION__, dev->physbase, RCAR_I2C_SIZE);
        goto fail;
    }

    if ((cpg_mgr_getfuncs(&cpg_hwfuncs, sizeof(cpg_mgr_funcs_t))) != EOK) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Unable to get cpg hw funcs", dev->chan_idx, __FUNCTION__);
        dev->reset_id = -1;
    }
    else {
        cpg_hwfuncs.init(0);
    }

    if ((dev->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1) {
        rcar_i2c_slogf( dev, VERBOSE_QUIET, "[chan %d]%s: ChannelCreate failed: %s(%d)", dev->chan_idx, __FUNCTION__, strerror(errno), errno );
        goto fail1;
    }

    if ((dev->coid = ConnectAttach(0, 0, dev->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
        rcar_i2c_slogf( dev, VERBOSE_QUIET, "[chan %d]%s: ConnectAttach failed: %s(%d)", dev->chan_idx, __FUNCTION__, strerror(errno), errno );
        goto fail2;
    }

    /* Initialize interrupt handler event */
    SIGEV_PULSE_INIT(&dev->intrEvent, dev->coid, RCAR_I2C_DEF_INTR_PRIORITY, RCAR_I2C_EVENT, NULL);

    /* initialize bus speed to default */
    rcar_i2c_set_bus_speed(dev, dev->busSpeed, NULL);

    /* Reset the I2C peripheral */
    rcar_i2c_reset(dev);

    /* reset slave interface */
    out32(dev->regbase + RCAR_I2C_ICSIER, 0);
    out32(dev->regbase + RCAR_I2C_ICSCR, 8);
    out32(dev->regbase + RCAR_I2C_ICSAR, 0);
    out32(dev->regbase + RCAR_I2C_ICSSR, 0);

    /* reset master interface */
    out32(dev->regbase + RCAR_I2C_ICMIER, 0);
    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_ICMCR_MDBS);
    out32(dev->regbase + RCAR_I2C_ICMAR, 0);
    out32(dev->regbase + RCAR_I2C_ICMSR, 0);

    if (dev->dma.en) {
        if (rcar_i2c_dma_init(dev) != EOK) {
            dev->dma.en = 0;
        } else {
            dev->isr = 0; // Always run in events mode with dma
        }
    }

    rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: I2C DMA support is %sabled", dev->chan_idx, __FUNCTION__,
            dev->dma.en ? "en" : "dis");

    if(dev->isr) { /* ISR Mode */
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: ISR Mode", dev->chan_idx, __FUNCTION__);
        dev->iid = InterruptAttach(dev->irq, (void *)rcar_i2c_intr, dev, 0, _NTO_INTR_FLAGS_TRK_MSK);
    } else { /* Events Mode */
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: Events Mode", dev->chan_idx, __FUNCTION__);
        dev->iid = InterruptAttachEvent (dev->irq, &dev->intrEvent, _NTO_INTR_FLAGS_TRK_MSK);
    }

    if (dev->iid == -1) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: InterruptAttach/Event failed (%s). isr mode %d irq %x",
            dev->chan_idx, __FUNCTION__, strerror(errno), dev->isr, dev->irq);
        goto fail3;
    }

    return dev;

fail3:
    ConnectDetach (dev->coid);
fail2:
     ChannelDestroy(dev->chid);
fail1:
    if(dev->reset_id != -1)
        cpg_hwfuncs.deinit();
     munmap_device_io(dev->regbase, RCAR_I2C_SIZE);
fail:
    if (dev->dma.typed_mem)
        free(dev->dma.typed_mem);
    free(dev);
    return NULL;
}

void rcar_i2c_fini(void *hdl)
{
    rcar_i2c_dev_t  *dev = hdl;

    rcar_i2c_slogf(dev, VERBOSE_LEVEL8, "[chan %d]: I2C deinitializing...", dev->chan_idx);

    out32(dev->regbase + RCAR_I2C_ICMCR, RCAR_ICMCR_MDBS);
    out32(dev->regbase + RCAR_I2C_ICSCR, 8);

    if(dev->reset_id != -1)
        cpg_hwfuncs.deinit();

    if(dev->dma.en)
        rcar_i2c_dma_deinit(dev);

    if (dev->coid != -1) {
        ConnectDetach (dev->coid);
    }

    if (dev->chid != -1) {
        ChannelDestroy(dev->chid);
    }

    if (dev->coid != -1) {
        ConnectDetach (dev->coid);
    }

    munmap_device_io(dev->regbase, RCAR_I2C_SIZE);

    InterruptDetach(dev->irq);

    free(hdl);
}

int rcar_i2c_reset(rcar_i2c_dev_t *dev)
{
    int ret = EIO;

    /* make sure any data transfer is complete before reset */
    rcar_i2c_force_stop(dev, 10);

    if(dev->reset_id !=-1) {
        ret = cpg_hwfuncs.mstp_reset(dev->reset_id);
        if (ret) { 
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Unable to CPG-reset", dev->chan_idx, __FUNCTION__);
        }
    }

    rcar_i2c_clock_init(dev);

    return ret;
}
