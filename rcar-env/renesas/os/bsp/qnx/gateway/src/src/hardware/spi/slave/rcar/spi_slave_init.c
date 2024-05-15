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
#include <atomic.h>
#include <sys/procmgr.h>
#include <hw/hwinfo_rcar.h>
#include <drvr/hwinfo.h>
#include "proto.h"
#include "msiof.h"

spi_slave_t   *dev;
static void spi_dinit(spi_slave_t * dev);
static volatile unsigned running = 0;
int spi_create_buffer(spi_slave_t * dev,void ** buffer, uint8_t datasize);
static void sig_handler(int signo)
{
    atomic_set(&running, 1);
    dispatch_unblock(dev->ctp);
}

int get_product_id(spi_slave_t *dev)
{
    hwi_tag *tag;
    unsigned hwi_off;

    dev->product_id = PRODUCT_ID_RCAR_UNKNOWN;

    hwi_off = hwi_find_device("r-car", 0);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, NULL);
        if(tag) {
            dev->product_id = tag->hwversion.version;
        } else {
            return (-1);
        }
    } else {
        return (-1);
    }

    return EOK;
}
/*******************************************************************************
* Outline      : This function run as soon as start this driver.
* Function Name: main
* Description  : Initializing SPI Slave default parrameters, resource manager, parse options,
*                then creating SPI Slave device and handling clients.
* Arguments    : argc
*                *argv[]
* Return Value : EXIT_SUCCESS
                 EXIT_FAILURE
* Note         :
*******************************************************************************/
int main (int argc, char *argv[])
{
    struct sigaction act;

    if (NULL == (dev = (spi_slave_t * )calloc(1, sizeof(spi_slave_t)))) {
        fprintf (stderr, "SPI-SLAVE : calloc is failed\n");
        return EXIT_FAILURE;
    }

    /* Configure default parameter for driver */
    /* Slave mode, 8bit length, CPOL = 0 and CPHA = 0 by default */
    dev->pbase       = 0;
    dev->irq         = 0;
    dev->vbase       = 0;
    dev->cfg.cpha    = SPI_CPHA_0;
    dev->cfg.cpol    = SPI_CPOL_0;
    dev->cfg.wordlen = 1;
    dev->cfg.id      = 0;
    dev->cfg.bitlen  = SPI_BIT_LEN_8;
    dev->verbose     = VERBOSE_QUIET;
    dev->status      = 0;
    dev->blksize 	 = MSIOF_SPI_FIFO_SIZE_64;
    dev->cfg.lsb_first   = SPI_SLAVE_MSB_FIRST;

    /* Specify product */
    if (get_product_id(dev)) {
        fprintf (stderr, "SPI-SLAVE : get Product ID is failed\n");
        goto fail;
    }
    if (dev->product_id == PRODUCT_ID_RCAR_V4H) {
        dev->blksize = MSIOF_SPI_FIFO_SIZE_256;
    } else {
        dev->blksize = MSIOF_SPI_FIFO_SIZE_64;
    }
    if (spi_slave_option(dev, argc, argv)) {
        fprintf (stderr, "SPI-SLAVE : Unable to get product_id\n");
        goto fail;
    }

    if (spi_create_buffer(dev,(void *)&dev->rx_buff.data, dev->cfg.bitlen)) {
        fprintf (stderr, "SPI-SLAVE : create rx buffer is failed\n");
        goto fail;
    }

    if (spi_create_buffer(dev,(void *)&dev->tx_buff, dev->cfg.bitlen)) {
        fprintf (stderr, "SPI-SLAVE : create tx buffer is failed\n");
        goto fail1;
    }

    rcar_spi_slogf(dev, VERBOSE_LEVEL1, "[Slog INFO] device information\n"
                   "Channel = %d, CPOL = %d, CPHA = %d, bitlen = %d",
                   dev->channel, dev->cfg.cpol, dev->cfg.cpha, dev->cfg.bitlen);
    if (dev->pbase == 0 || dev->irq == 0) {
        fprintf (stderr, "SPI-SLAVE : HW_INFO() supply info is failed\n");
        goto fail2;
    }

    ThreadCtl(_NTO_TCTL_IO, 0);

    /* Map SPI registers */
    if ((uintptr_t)MAP_FAILED == (dev->vbase = mmap_device_io(SPI_REG_LEN, dev->pbase))) {
        fprintf (stderr, "SPI-SLAVE : mmap is failed\n");
        goto fail2;
    }

    if (spi_slave_config(dev)) {
        fprintf (stderr, "SPI-SLAVE : config is failed\n");
        goto fail3;
    }

    /* Attach interrupt */
    if(-1 == (dev->iid = InterruptAttach(dev->irq, (void *)spi_slave_intr, dev,
                               0, _NTO_INTR_FLAGS_TRK_MSK))) {
        fprintf (stderr, "SPI-SLAVE : InterruptAttach is failed\n");
        goto fail3;
    }

    // Register exit handler
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTERM);
    act.sa_handler = sig_handler;
    act.sa_flags = 0;
    sigaction(SIGTERM, &act, NULL);	

    if (spi_slave_create_instance(dev)) {
        goto fail4;
    }

    spi_lock_init(dev);

    /* Start the resource manager message loop */
    while (!running) {
        dev->ctp = dispatch_block (dev->ctp);
        if (dev->ctp != NULL) {
            dispatch_handler (dev->ctp);
        } else if (errno != EFAULT) {
            atomic_set(&running, 1);
        }
    }

    spi_dinit(dev);
    return EXIT_SUCCESS;

fail4:
    InterruptDetach(dev->iid);
fail3:
    munmap_device_io(dev->vbase, SPI_REG_LEN);
fail2:
    free(dev->tx_buff);
fail1:
    free((void *)dev->rx_buff.data);
fail:
    free(dev);
    dev = NULL;

    return EXIT_FAILURE;
}

/*******************************************************************************
* Outline      : This function creates buffer for transmitting and receiving
* Function Name: spi_create_buffer
* Arguments    : void ** buffer, uint8_t datasize
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_create_buffer(spi_slave_t * dev,void ** buffer, uint8_t datasize)
{
    int size;

    if (BIT_LEN_VALIDATE(datasize) == 0) {
        return -1;
    } else {
        switch (datasize) {
            case SPI_BIT_LEN_8: size = sizeof(uint8_t); break;
            case SPI_BIT_LEN_16: size = sizeof(uint16_t); break;
            default: size = sizeof(uint32_t); break;
        }

        if (NULL != (*buffer = calloc(dev->blksize, size))) {
            return EOK;
        }
        return -1;
    }
}

/*******************************************************************************
* Outline      : This function de-initialize SPI driver.
* Function Name: spi_dinit
* Description  :
* Arguments    : spi_slave_t * dev

* Note         :
*******************************************************************************/
static void spi_dinit(spi_slave_t * dev)
{
    resmgr_detach(dev->dpp, dev->res_id, _RESMGR_DETACH_ALL);
    spi_lock_fini(dev);
    InterruptDetach(dev->iid);
    munmap_device_io(dev->vbase, SPI_REG_LEN);

    dispatch_destroy(dev->dpp);
    free((void *)dev->rx_buff.data);
    free(dev->tx_buff);
    free(dev);
}
