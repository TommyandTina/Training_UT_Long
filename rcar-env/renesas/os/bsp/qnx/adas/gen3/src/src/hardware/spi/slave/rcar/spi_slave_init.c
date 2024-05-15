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

#include <sys/procmgr.h>
#include <pthread.h>
#include "spi_slave.h"
#include "msiof.h"

volatile sig_atomic_t status = 0;
int tid = 0;
int terminate = 0;
static void spi_dinit(spi_slave_t * dev);
//static void signal_handler(int signo);
int spi_create_buffer(void ** buffer, uint8_t datasize);
void * rsmgr_run(void * arg);
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
    pthread_attr_t attr;
    siginfo_t      info;
    spi_slave_t   *dev = NULL;
    sigset_t       set;

    if(NULL == (dev = (spi_slave_t * )calloc(1, sizeof(spi_slave_t))))
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE calloc is failed");
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

    if (EXIT_SUCCESS != spi_slave_option(dev, argc, argv))
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE parse options is failed");
        goto failed1;
    }

    if((EXIT_FAILURE == spi_create_buffer((void *)&dev->rx_buff.data, dev->cfg.bitlen)) ||
       (EXIT_FAILURE == spi_create_buffer((void *)&dev->tx_buff, dev->cfg.bitlen)))
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE create buffer is failed");
        goto failed1;
    }

    rcar_spi_slogf(dev, VERBOSE_LEVEL1, "[Slog INFO] device information\n"
                   "Channel = %d, CPOL = %d, CPHA = %d, bitlen = %d",
                   dev->channel, dev->cfg.cpol, dev->cfg.cpha, dev->cfg.bitlen);
    if(dev->pbase == 0 || dev->irq == 0)
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE HW_INFO() supply info is failed");
    }

    ThreadCtl(_NTO_TCTL_IO, 0);

    /* Map SPI registers */
    if ((uintptr_t)MAP_FAILED == (dev->vbase = mmap_device_io(SPI_REG_LEN, dev->pbase)))
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE mmap is failed");
        goto failed1;
    }

    if (EXIT_SUCCESS != spi_slave_config(dev))
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE config is failed");
        goto failed;
    }

    /* Initialize interrupt event and attach interrupt */
    SIGEV_INTR_INIT(&dev->spievent);
    if(-1 == (dev->iid = InterruptAttach(dev->irq, (void *)spi_slave_attach_intr, dev,
                               0, _NTO_INTR_FLAGS_TRK_MSK))) 
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE InterruptAttach is failed");
        goto failed;
    }

    /* Create a thread to run resource manager */
    pthread_attr_init(&attr);
    if(EOK != (tid = pthread_create(NULL, &attr, &rsmgr_run, dev)))
    {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR]SPI-SLAVE pthread_create is failed");
        goto failed;
    }

    /* Create signal handler to handle terminating signal */
    sigemptyset(&set);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGINT);

    while (1)
    {
        if (SignalWaitinfo (&set, &info) == -1)
        {
            continue;
        }
        if ((info.si_signo == SIGTERM) || (info.si_signo == SIGINT))
        {
            break;
        }
    }

    dispatch_unblock(dev->ctp);
    terminate = 1;
    pthread_join(tid, NULL);
    spi_dinit(dev);
    return EXIT_SUCCESS;

failed:
    munmap_device_io(dev->vbase, SPI_REG_LEN);
    InterruptDetach(dev->iid);
failed1:
    free((void *)dev->rx_buff.data);
    free(dev->tx_buff);
    free(dev);

    return EXIT_FAILURE;
}

/*******************************************************************************
* Outline      : This function creates buffer for transmitting and receiving
* Function Name: spi_create_buffer
* Arguments    : void ** buffer, uint8_t datasize
* Note         :
*******************************************************************************/
int spi_create_buffer(void ** buffer, uint8_t datasize)
{
    int size;

    if (BIT_LEN_VALIDATE(datasize) == 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        switch (datasize) {
            case SPI_BIT_LEN_8: size = sizeof(uint8_t); break;
            case SPI_BIT_LEN_16: size = sizeof(uint16_t); break;
            default: size = sizeof(uint32_t); break;
        }

        if(NULL != (*buffer = calloc(BUFF_MAX_SIZE, size)))
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
}

/*******************************************************************************
* Outline      : This function creates resource manager and waits for incoming message
* Function Name: rsmgr_run
* Arguments    : void * arg
* Note         :
*******************************************************************************/
void * rsmgr_run(void * arg)
{
    spi_slave_t * dev = (spi_slave_t *)arg;
    spi_slave_create_instance(dev);
    while (1)
    {
        /* Waiting for resource manager message */
        if (NULL != (dev->ctp = dispatch_block(dev->ctp)))
        {
            dispatch_handler(dev->ctp);
        }
        else
        {
            break;
        }
        if (terminate == 1)
            break;
    }
    pthread_exit(NULL);
   return 0;
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
    rcar_spi_slogf(dev, VERBOSE_LEVEL1, "[Slog INFO] spi_dinit");
    resmgr_detach(dev->dpp, dev->res_id, _RESMGR_DETACH_ALL);

    InterruptDetach(dev->iid);
    munmap_device_io(dev->vbase, SPI_REG_LEN);

    dispatch_destroy(dev->dpp);
    free((void *)dev->rx_buff.data);
    free(dev->tx_buff);
    free(dev);
}
