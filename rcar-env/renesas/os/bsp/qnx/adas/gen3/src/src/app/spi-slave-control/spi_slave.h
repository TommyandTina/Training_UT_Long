/*
 * $QNXLicenseC:
 * Copyright 2019, Renesas Electronics Corporation
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

/******************************************************************************
 *                                FOR FRAMEWORK                               *
 ******************************************************************************/

/* Select SPI clock polarity */
enum
{
    SPI_CPOL_0,
    SPI_CPOL_1,
    NUMBER_OF_CPOL,
} spi_cpol_t;

/* Select SPI clock phase */
enum
{
    SPI_CPHA_0,
    SPI_CPHA_1,
    NUMBER_OF_CPHA,
} spi_cpha_t;

enum
{
    SPI_BIT_LEN_8 = 8,
    SPI_BIT_LEN_16 = 16,
    SPI_BIT_LEN_32 = 32,
} spi_bitlen_t;

/* This structure maintains configuration parameter of SPI module */
typedef struct
{
    uint8_t       id;        //Select pin to be SS (SYNC or SS1/2), in Slave mode, only SYNC can be used
    uint32_t      speed;
    uint8_t       bitlen;    //Data length in bit
    uint8_t       wordlen;
    uint8_t       cpol;
    uint8_t       cpha;
} spi_slave_cfg_t;

/* This structure contains transfer data and its index,
 * used when read and write one byte
 */
typedef struct
{
    volatile uint32_t    byte;
    volatile int        idx;
    int                 status;
} spi_data_t;

enum
{
    SPI_STATUS_NORMAL               = 0,
    SPI_STATUS_TRANSFER_DONE        = (1 << 0),
    SPI_STATUS_RECV_FIFO_FULL       = (1 << 1),
    SPI_STATUS_SYNC_ERR             = (1 << 2),
    SPI_STATUS_MASTER_TIMEOUT_ERR   = (1 << 3), //this error happen when master and slave transfer
                                              //some data but master don't set SYNC pin to 1
    SPI_STATUS_TRANSFER_TIMEOUT     = (1 << 4),
    SPI_STATUS_TRANS_FIFO_OVF       = (1 << 5),
    SPI_STATUS_FAILED_TO_START_XFER = (1 << 6),
    SPI_STATUS_NO_DATA_AVAILABLE    = (1 << 7),
} spi_status_t;

#include <devctl.h>
#define _DCMD_SPI                      _DCMD_MISC
#define DCMD_SPI_SLAVE_SET_CONFIG      __DIOT (_DCMD_SPI, 1, spi_slave_cfg_t)
#define DCMD_SPI_SLAVE_GET_CONFIG      __DIOF (_DCMD_SPI, 2, spi_slave_cfg_t)
#define DCMD_SPI_SLAVE_READ_BYTE       __DIOF (_DCMD_SPI, 3, spi_data_t)
#define DCMD_SPI_SLAVE_WRITE_BYTE      __DIOT (_DCMD_SPI, 4, spi_data_t)
#define DCMD_SPI_SLAVE_ENABLE_XCHANGE  __DION (_DCMD_SPI, 5)
#define DCMD_SPI_SLAVE_DISABLE_XCHANGE __DION (_DCMD_SPI, 6)
#define DCMD_SPI_SLAVE_BUS_RESET       __DION (_DCMD_SPI, 7)

/******************************************************************************
 *                                FOR DRIVER                                  *
 ******************************************************************************/
#define SPI_RESMGR_NPARTS        (2)
#define SPI_RESMGR_MSGSIZE       (2048)

#define BUFF_MAX_SIZE            (64)

#define SPI_WORD_LEN_MAX         (64)

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

#define TIMEOUT                  (100)
#define TIMEOUT_NS               (1000000000) //timeout in nano second

#define CPOL_OPTION              (0)
#define CPHA_OPTION              (1)
#define BIT_LEN_OPTION           (2)

#define rcar_spi_slogf(dev, vl, msg, ...) \
            if (vl == VERBOSE_QUIET) \
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, msg, ##__VA_ARGS__); \
            else if (vl <= dev->verbose) \
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, msg, ##__VA_ARGS__)

#define BIT_LEN_VALIDATE(BIT_LEN)   (((BIT_LEN) == SPI_BIT_LEN_8) || \
                                ((BIT_LEN) == SPI_BIT_LEN_16) || ((BIT_LEN) == SPI_BIT_LEN_32))

#define WORD_LEN_VALIDATE(WORD_LEN) (((WORD_LEN) > 0) && ((WORD_LEN) <= SPI_WORD_LEN_MAX))

#define CPOL_VALIDATE(CPOL)         (((CPOL) == SPI_CPOL_0) || ((CPOL) == SPI_CPOL_1))

#define CPHA_VALIDATE(CPHA)         (((CPHA) == SPI_CPHA_0) || ((CPHA) == SPI_CPHA_1))


/* SPI channel from 0 to 3 corresponding to MSIOF0 to MSIOF3 */
enum
{
    SPI_CHANNEL_0,
    SPI_CHANNEL_1,
    SPI_CHANNEL_2,
    SPI_CHANNEL_3,
} spi_channel_t;

typedef struct
{
    volatile void * data;
    //volatile uint8_t data[64];
    volatile int head;
    volatile int tail;
} buffer_t;

/* Main structure, maintain all parameter of SPI module */
typedef struct
{
    iofunc_attr_t       attr;
    dispatch_t          *dpp;
    dispatch_context_t  *ctp;
    int                 res_id;
    uint8_t             channel;
    uintptr_t           vbase;
    uint16_t            irq_base;
    spi_slave_cfg_t     cfg;
    int                 iid;         //interrupt id
    struct sigevent     spievent;
    void *             tx_buff;
    buffer_t            rx_buff;
    spi_data_t          tx_data;
    spi_data_t          rx_data;
    int                 verbose;
    volatile uint32_t   status;
} spi_slave_t;


int spi_slave_option (spi_slave_t* dev, int argc, char* argv[]);
int spi_slave_update_reg(uintptr_t reg,uint32_t clr_bits, uint32_t set_bits,uint32_t timeout);
int spi_slave_create_instance(void * data);
int spi_slave_enable_xchange(spi_slave_t * dev);
int spi_slave_disable_xchange (spi_slave_t * dev);
int spi_slave_xfer(spi_slave_t * dev, int len);
int spi_slave_wait (spi_slave_t * dev);
int spi_slave_config(spi_slave_t *dev);
int spi_slave_write_fifo(spi_slave_t * dev, void * tx_buf, int len);
int spi_slave_read_fifo(spi_slave_t *dev, void *rx_buf, int len);
int spi_slave_rst_txrx(spi_slave_t * dev);
void spi_slave_clear_buffer(spi_slave_t * dev);
const struct sigevent* spi_irq(void* area, int id);
int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
iofunc_ocb_t * ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *attr);
void ocb_free(iofunc_ocb_t *ocb);
#endif /* RCAR_SPI_SLAVE_H_ */
