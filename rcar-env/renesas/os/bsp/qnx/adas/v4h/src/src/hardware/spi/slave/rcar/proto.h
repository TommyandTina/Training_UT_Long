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

#ifndef RCAR_SPI_SLAVE_H_
#define RCAR_SPI_SLAVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <sys/types.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/platform.h>
#include <sys/resmgr.h>
#include <sys/procmgr.h>
#include <atomic.h>
#include <fcntl.h>
#include <aarch64/r-car.h>
#include <hw/spi_slave.h>
/******************************************************************************
 *                                FOR DRIVER                                  *
 ******************************************************************************/
#define SPI_RESMGR_NPARTS        (2)
#define SPI_RESMGR_MSGSIZE       (2048)

#define STOP_PROGRAM             (1)

#define VERBOSE_QUIET            (0)
#define VERBOSE_LEVEL1           (1)
#define VERBOSE_LEVEL2           (2)
#define VERBOSE_LEVEL3           (3)
#define VERBOSE_LEVEL4           (4)
#define VERBOSE_LEVEL5           (5)
#define VERBOSE_LEVEL6           (6)
#define VERBOSE_LEVEL7           (7)
#define VERBOSE_LEVEL8           (8)

#define RCAR_SPI_TIMEOUT         (100)
#define RCAR_SPI_INTR_TIMEOUT    (1000000000) //timeout 1s

#define CPOL_OPTION              (0)
#define CPHA_OPTION              (1)
#define BIT_LEN_OPTION           (2)
#define SPI_SLAVE_MSB_FIRST      (1<<0)

#define rcar_spi_slogf(dev, vl, msg, ...) \
            if (vl == VERBOSE_QUIET) \
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, msg, ##__VA_ARGS__); \
            else if (vl <= dev->verbose) \
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, msg, ##__VA_ARGS__)

#define BIT_LEN_VALIDATE(BIT_LEN)   (((BIT_LEN) == SPI_BIT_LEN_8) || \
                                ((BIT_LEN) == SPI_BIT_LEN_16) || ((BIT_LEN) == SPI_BIT_LEN_32))

#define CPOL_VALIDATE(CPOL)         (((CPOL) == SPI_CPOL_0) || ((CPOL) == SPI_CPOL_1))

#define CPHA_VALIDATE(CPHA)         (((CPHA) == SPI_CPHA_0) || ((CPHA) == SPI_CPHA_1))


/* Main structure, maintain all parameter of SPI module */
typedef struct
{
    iofunc_attr_t       attr;
    dispatch_t          *dpp;
    dispatch_context_t  *ctp;
    int                 res_id;
    uint8_t             channel;
    off64_t             pbase;
    uintptr_t           vbase;
    uint16_t            irq;
    spi_slave_cfg_t     cfg;
    int                 iid;         /* interrupt id */
    struct sigevent     spievent;
    void *              tx_buff;
    buffer_t            rx_buff;
    spi_data_t          tx_data;
    spi_data_t          rx_data;
    int                 verbose;
    volatile uint32_t   status;
    int                 rcvid;      /* Blocked client's rcvid */
    uint32_t	            product_id;
    uint16_t	            blksize;    /* FIFO size */
    uint8_t             lsb_first;
    pthread_mutex_t     slave_mutex;
} spi_slave_t;

#define spi_lock_init(d)     pthread_mutex_init(&d->slave_mutex, NULL)
#define spi_lock_fini(d)     pthread_mutex_destroy(&d->slave_mutex)
#define spi_lock(d)          pthread_mutex_lock(&d->slave_mutex)
#define spi_unlock(d)        pthread_mutex_unlock(&d->slave_mutex)

int spi_slave_option (spi_slave_t* dev, int argc, char* argv[]);
int spi_slave_update_reg(uintptr_t reg,uint32_t clr_bits, uint32_t set_bits,uint32_t timeout);
int spi_slave_create_instance(void * data);
int spi_slave_enable_xchange(spi_slave_t * dev);
int spi_slave_disable_xchange (spi_slave_t * dev);
int spi_slave_xfer(spi_slave_t * dev, int len);
int spi_slave_wait (spi_slave_t * dev);
int spi_slave_config(spi_slave_t *dev);
int spi_slave_write_fifo(spi_slave_t * dev, void * tx_buf, int len);
int spi_slave_rst_txrx(spi_slave_t * dev);
void spi_slave_clear_buffer(spi_slave_t * dev);
const struct sigevent* spi_slave_intr(void* area, int id);
int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
iofunc_ocb_t * ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *attr);
void ocb_free(iofunc_ocb_t *ocb);
int get_product_id(spi_slave_t *dev);
#endif /* RCAR_SPI_SLAVE_H_ */
