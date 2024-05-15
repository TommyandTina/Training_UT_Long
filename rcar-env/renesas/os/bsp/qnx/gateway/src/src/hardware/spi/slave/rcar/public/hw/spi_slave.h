/*
* $QNXLicenseC:
* Copyright 2022, QNX Software Systems.
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
#ifndef _RCAR_SPISLAVE_DCMD_H
#define _RCAR_SPISLAVE_DCMD_H

#include <stdint.h>
#include <stdbool.h>
#include <devctl.h>

#define MSIOF_SPI_FIFO_SIZE_64      64
#define MSIOF_SPI_FIFO_SIZE_256     256

typedef struct
{
    volatile void * data;
    volatile int head;
    volatile int tail;
} buffer_t;

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
    uint16_t      wordlen;
    uint8_t       cpol;
    uint8_t       cpha;
    uint8_t       lsb_first;
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
    SPI_STATUS_RECV_FIFO_UDF        = (1 << 8),
    SPI_STATUS_RECV_FIFO_OVF        = (1 << 9),
} spi_status_t;

#include <devctl.h>
#define _DCMD_SPI                         _DCMD_MISC
#define DCMD_SPI_SLAVE_SET_CONFIG         __DIOT (_DCMD_SPI, 1, spi_slave_cfg_t)
#define DCMD_SPI_SLAVE_GET_CONFIG         __DIOF (_DCMD_SPI, 2, spi_slave_cfg_t)
#define DCMD_SPI_SLAVE_READ_BYTE          __DIOF (_DCMD_SPI, 3, spi_data_t)
#define DCMD_SPI_SLAVE_WRITE_BYTE         __DIOT (_DCMD_SPI, 4, spi_data_t)
#define DCMD_SPI_SLAVE_ENABLE_XCHANGE     __DION (_DCMD_SPI, 5)
#define DCMD_SPI_SLAVE_DISABLE_XCHANGE    __DION (_DCMD_SPI, 6)
#define DCMD_SPI_SLAVE_BUS_RESET          __DION (_DCMD_SPI, 7)
#define DCMD_SPI_SLAVE_GET_STATUS         __DIOF (_DCMD_SPI, 8, uint32_t)
#define DCMD_SPI_SLAVE_GET_FIFO_SIZE      __DIOF (_DCMD_SPI, 9, uint32_t)
#endif
