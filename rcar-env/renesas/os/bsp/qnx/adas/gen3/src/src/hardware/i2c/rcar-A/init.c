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
        if (!(in32(dev->regbase + RCAR_GEN3_I2C_ICMCR) & RCAR_GEN3_ICMCR_FSDA))
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

    if (in32(dev->regbase + RCAR_GEN3_I2C_ICMCR) & RCAR_GEN3_ICMCR_FSDA) {
        out32(dev->regbase + RCAR_GEN3_I2C_ICMCR, RCAR_GEN3_ICMCR_MIE | RCAR_GEN3_ICMCR_FSB | RCAR_GEN3_ICMCR_MDBS);
        out32(dev->regbase + RCAR_GEN3_I2C_ICMSR, 0);

        while (in32(dev->regbase + RCAR_GEN3_I2C_ICMSR) & RCAR_GEN3_ICMSR_MDR) {
            in32(dev->regbase + RCAR_GEN3_I2C_ICRXD);
            out32(dev->regbase + RCAR_GEN3_I2C_ICMSR, 0);
        }

        while ((retry_count--) && !(in32(dev->regbase + RCAR_GEN3_I2C_ICMSR) & RCAR_GEN3_ICMSR_MST));

        out32(dev->regbase + RCAR_GEN3_I2C_ICMSR, 0);
    }
}

static int rcar_fastmodeplus_support_check(rcar_i2c_dev_t *dev)
{
    /* Fast mode plus is only available on R-Car V3U/V4H */
    if (dev->product_id == PRODUCT_ID_RCAR_V3U ) {
        return 1;
    } else {
        return 0;
    }
}

/* Should consider to put IO voltage (PFC POC) setting to 1.8V in this function 
 * into startup of BSP 
 */
static int rcar_i2c_change_io_voltage_18v(rcar_i2c_dev_t *dev)
{
    uint32_t  mask = 0, poc;
    int ret = EOK;

    if (!rcar_fastmodeplus_support_check(dev)) {
        return EINVAL;
    }

    rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: Change IO voltage to 1.8V for Fast mode+", dev->chan_idx, __FUNCTION__);

    if (dev->poc.pbase == 0) {
        return EINVAL;
    }

    dev->poc.vbase = mmap_device_io(dev->poc.size, dev->poc.pbase);
    if (dev->poc.vbase == (uintptr_t)MAP_FAILED) {
        return ENOMEM;
    }

    poc = in32(dev->poc.vbase + dev->poc.reg);
    mask = (dev->poc.mask << dev->poc.shift);
    if (!(poc & mask)) {
        goto done;
    }

    poc &= ~mask;

    out32(dev->poc.vbase + dev->poc.pmmr, ~poc);
    out32(dev->poc.vbase + dev->poc.reg, poc);

    poc = in32(dev->poc.vbase + dev->poc.reg);
    if (poc & mask) {
        ret = EAGAIN;
    }

done:
    munmap_device_io(dev->poc.vbase, dev->poc.size);
    return ret;
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
    dev->isr = 0;
    dev->smmu = 0;

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

    dev->regbase = mmap_device_io(RCAR_GEN3_I2C_SIZE, dev->physbase);
    if (dev->regbase == (uintptr_t)MAP_FAILED) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "%s: fail mmap_device_io %lx, %d", __FUNCTION__, dev->physbase, RCAR_GEN3_I2C_SIZE);
        perror("mmap_device_io");
        goto fail;
    }

    if (dev->fast_mode_plus) {
         if (rcar_i2c_change_io_voltage_18v(dev) != EOK) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET,
                "[chan %d]%s: Unable change IO voltage to 1.8V in Fast mode+, use normal mode", dev->chan_idx, __FUNCTION__);
            dev->fast_mode_plus = 0;
        }
    }

    if ((cpg_mgr_getfuncs(&cpg_hwfuncs, sizeof(cpg_mgr_funcs_t))) != EOK) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Unable to get cpg hw funcs", dev->chan_idx, __FUNCTION__);
        dev->reset_id = -1;
    }
    else {
        cpg_hwfuncs.init(0);
    }

    /* initialize bus speed to default */
    rcar_i2c_set_bus_speed(dev, dev->busSpeed, NULL);

    rcar_i2c_reset(dev);

    /* reset slave interface */
    out32(dev->regbase + RCAR_GEN3_I2C_ICSIER, 0);
    out32(dev->regbase + RCAR_GEN3_I2C_ICSCR, 8);
    out32(dev->regbase + RCAR_GEN3_I2C_ICSAR, 0);
    out32(dev->regbase + RCAR_GEN3_I2C_ICSSR, 0);

    /* reset master interface */
    out32(dev->regbase + RCAR_GEN3_I2C_ICMIER, 0);
    out32(dev->regbase + RCAR_GEN3_I2C_ICMCR, 0);
    out32(dev->regbase + RCAR_GEN3_I2C_ICMAR, 0);
    out32(dev->regbase + RCAR_GEN3_I2C_ICMSR, 0);

    /* clock */
    out32(dev->regbase + RCAR_GEN3_I2C_ICCCR, 0);
    out32(dev->regbase + RCAR_GEN3_I2C_ICCCR2, 0);

    /* ISR */
    if(dev->isr) { /* Interrupt Handler */
        SIGEV_INTR_INIT(&dev->intrEvent);
        dev->iid = InterruptAttach(dev->irq, (void *)rcar_i2c_intr, dev, 0, _NTO_INTR_FLAGS_TRK_MSK);
        if (dev->iid == -1) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: InterruptAttach failed (%s). isr mode %d irq %x",
                dev->chan_idx, __FUNCTION__, strerror(errno), dev->isr, dev->irq);
            goto fail1;
        }
    } else { /* Interrupt Event Handler */
        /* Create channel for interrupt handler */
        if ((dev->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1) {
            goto fail1;
        }

        if ((dev->coid = ConnectAttach(0, 0, dev->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
            goto fail2;
        }

        /* Initialized Event for event handler */
        SIGEV_PULSE_INIT(&dev->intrEvent, dev->coid, RCAR_I2C_PRIORITY, RCAR_I2C_EVENT, NULL);

        dev->iid = InterruptAttachEvent (dev->irq, &dev->intrEvent, _NTO_INTR_FLAGS_TRK_MSK);
        if(dev->iid == -1) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: InterruptAttachEvent failed (%s). isr mode %d irq %x",
                dev->chan_idx, __FUNCTION__, strerror(errno), dev->isr, dev->irq);
            goto fail3;
        }
    }

    if (dev->dma) {
        if (rcar_i2c_dma_init(dev))
            dev->dma = 0;
    }

    return dev;
fail3:
    ConnectDetach (dev->coid);
fail2:
     ChannelDestroy(dev->chid);
fail1:
    if(dev->reset_id != -1)
        cpg_hwfuncs.deinit();
     munmap_device_io(dev->regbase, RCAR_GEN3_I2C_SIZE);
fail:
    free(dev);
    return NULL;
}

void rcar_i2c_fini(void *hdl)
{
    rcar_i2c_dev_t  *dev = hdl;

    rcar_i2c_slogf(dev, VERBOSE_LEVEL8, "[chan %d]: I2C deinitializing...", dev->chan_idx);

    out32(dev->regbase + RCAR_GEN3_I2C_ICMCR, RCAR_GEN3_ICMCR_MDBS);
    out32(dev->regbase + RCAR_GEN3_I2C_ICSCR, 8);

    if(dev->reset_id != -1)
        cpg_hwfuncs.deinit();

    if(dev->dma)
        rcar_i2c_dma_deinit(dev);

    if (dev->coid != -1) {
        ConnectDetach (dev->coid);
    }

    if (dev->chid != -1) {
        ChannelDestroy(dev->chid);
    }

    munmap_device_io(dev->regbase, RCAR_GEN3_I2C_SIZE);

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
