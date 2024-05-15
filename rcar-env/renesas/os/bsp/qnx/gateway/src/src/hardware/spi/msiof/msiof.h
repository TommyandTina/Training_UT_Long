/*
 * $QNXLicenseC:
 * Copyright 2015, QNX Software Systems.
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

#ifndef __MSIOF_SPI_H_INCLUDED
#define __MSIOF_SPI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <hw/spi-master.h>
#include <hw/dma.h>
#include <aarch64/r-car.h>


/* SPI event define */
#define MSIOF_SPI_INTERRUPT        0
#define MSIOF_SPI_EVENT            1
#define MSIOF_SPI_RXDMA_EVENT      0x52
#define MSIOF_SPI_TXDMA_EVENT      0x53
#define MSIOF_SPI_PRIORITY         21
/*	Rcar-V3H:  	Support RTDMAC channel from 0 to 31
    Rcar-Gen3: 	MSIOF0, MSIOF1 support SYSDMAC channel from 16 to 47
                MSIOF2, MSIOF3 support SYSDMAC channel from 0 to 15
    Rcar-E3:   	MSIOF0 support SYSDMAC channel from 16 to 47
                MSIOF1, MSIOF2, MSIOF3 support SYSDMAC channel from 0 to 15
    Rcar-V3M:  	MSIOF0, MSIOF1,MSIOF2, MSIOF3 support SYSDMAC channel from 16 to 4 

    All MSIOF channels support SYSDMAC from 0 to 15 in Rcar V3U/S4/V4H 
*/
#define MSIOF_SPI_DMA_PRI15        15
#define MSIOF_SPI_DMA_PRI31        31
#define MSIOF_SPI_DMA_PRI47        47

#define MSIOF_SPI_FIFO_SIZE_64      64
#define MSIOF_SPI_FIFO_SIZE_256     256
#define CTR_DELAY_NS                (1000)
#define MSIOF_SPI_CLK_RATE          26000000 /* Frequency of SPI reference clock */
#define MSIOF_SPI_REGLEN            0x64

/* RCar SPI Device info */
#define NUM_OF_SPI_DEVS             3 /* num of spi device per a channel */

#define CTR_TSCKIZ(x)  (((1<<1) | (x & 1)) << 30)
#define CTR_RSCKIZ(x)  (((1<<1) | (x & 1)) << 28)
#define CTR_TEDG_BP    (27)
#define CTR_REDG_BP    (26)
#define CTR_TSCKE      (1 << 15)
#define CTR_TFSE       (1 << 14)
#define CTR_TXE        (1 << 9)
#define CTR_RXE        (1 << 8)
#define CTR_TXRST      (1 << 1)
#define CTR_RXRST      (1 << 0)

#define IER_TDMAE      (1 << 31)
#define IER_TFEMPE     (1 << 29)
#define IER_TDREQE     (1 << 28)
#define IER_TEOF       (1 << 23)
#define IER_TFSERR     (1 << 21)
#define IER_TFOVFE     (1 << 20)
#define IER_TFUDFE     (1 << 19)
#define IER_RDMAE      (1 << 15)
#define IER_RDREQE     (1 << 12)
#define IER_REOF       (1 << 7)
#define IER_RFSERRE    (1 << 5)

#define STR_TFEMP      (1 << 29)
#define STR_TDREQ      (1 << 28)
#define STR_TEOF       (1 << 23)
#define STR_TFSERR     (1 << 21)
#define STR_TFOVF      (1 << 20)
#define STR_TFUDF      (1 << 19)
#define STR_RFFUL      (1 << 13)
#define STR_RDREQ      (1 << 12)
#define STR_REOF       (1 << 7)
#define STR_RFSERR     (1 << 5)
#define STR_RFUDF      (1 << 4)
#define STR_RFOVF      (1 << 3)

#define FCTR_TFUA_BP   (20)
#define FCTR_TFUA_MSK  (0x1ff) /* Note: FCTR_TFUA_MSK in R-CarGen3/V3U/S4 is 0x7F, but value of SIFCTR[28:27] is always 0, so no problem. V4H is 0x1FF, */
#define FCTR_RFUA_BP   (4)
#define FCTR_RFUA_MSK  (0x1ff)
#define FCTR_TFWM_64   (0 << 29)
#define FCTR_TFWM_32   (1 << 29)
#define FCTR_TFWM_24   (2 << 29)
#define FCTR_TFWM_16   (3 << 29)
#define FCTR_TFWM_12   (4 << 29)
#define FCTR_TFWM_8    (5 << 29)
#define FCTR_TFWM_4    (6 << 29)
#define FCTR_TFWM_1    (7 << 29)
#define FCTR_TFWM_MSK  (7 << 29)
#define FCTR_RFWM_1    (0 << 13)
#define FCTR_RFWM_4    (1 << 13)
#define FCTR_RFWM_8    (2 << 13)
#define FCTR_RFWM_16   (3 << 13)
#define FCTR_RFWM_32   (4 << 13)
#define FCTR_RFWM_64   (5 << 13)
#define FCTR_RFWM_128  (6 << 13)
#define FCTR_RFWM_256  (7 << 13)
#define FCTR_RFWM_MSK  (7 << 13)

/* TMDR2 and RMD2 registers */
#define MDR2_BITLEN1(i)	(((i) - 1) << 24) /* Data Size (8-32 bits) */
#define MDR2_WDLEN1(i)	(((i) - 1) << 16) /* Word Count (1-64/256 (SH, A1))) */

typedef struct {
    unsigned short div;
    unsigned short scr;
} spi_clk_t;

typedef struct _buffer {
    char    *head;
    char    *tail;
    char    *buff;
    int     cnt;
    int     bufsize;
    int     recsize;
} buffer_t;

typedef struct {
    /* DMA transfer */
    int             status;
    dma_functions_t funcs;
    void            *tx_hdl;
    void            *rx_hdl;
    dma_addr_t      pbuf;
    int16_t         tx_req_id;
    int16_t         rx_req_id;
    struct sigevent tx_event;
    struct sigevent rx_event;
    int             chan_min;
    int             chan_max;
    int             smmu;
    char            *typed_mem;
    struct smmu_object *smmu_obj;
} spi_dma_t;

/* The structure which maintains the various parameters
 * of the SPI module.
 */
typedef struct {
    SPIDEV          spi; /* This has to be the first element */
    uint32_t        pbase;
    uintptr_t       vbase;
    int             irq_spi;
    int             chid, coid;
    int             iid_spi;
    uint32_t        clock;
    uint8_t         *pbuf;
    int             xlen;
    int             dtime; /* usec per data, for time out use */
    uint8_t         channel_num;
    int             num_cs;
    struct sigevent spievent;
    uint32_t        mode[NUM_OF_SPI_DEVS]; /* mode operation */
    uint16_t        blksize; /* FIFO size */
    /* for DMA transfer */
    spi_dma_t       dma;
    uint8_t         product_id;
    uint8_t         product_rev;
    uint8_t         data_size; /* Data size(8 to 32) */
} msiof_spi_t;

extern void *msiof_init(void *hdl, char *options);
extern void msiof_dinit(void *hdl);
extern int msiof_drvinfo(void *hdl, spi_drvinfo_t *info);
extern int msiof_devinfo(void *hdl, uint32_t device, spi_devinfo_t *info);
extern int msiof_setcfg(void *hdl, uint16_t device, spi_cfg_t *cfg);
extern void *msiof_xfer(void *hdl, uint32_t device, uint8_t *buf, int *len);
extern int msiof_cfg(void *hdl, spi_cfg_t *cfg, uint32_t *ctrl);
extern int msiof_dma_xfer(void *hdl, uint32_t device, spi_dma_paddr_t *paddr, int len);

extern void msiof_setup(msiof_spi_t *msiof, uint32_t device, uint32_t clock_rate, uint32_t mode);
extern int msiof_wait(msiof_spi_t *msiof, int len);
extern int msiof_attach_intr(msiof_spi_t *msiof);
extern void spi_dump(msiof_spi_t *msiof);
extern int msiof_spi_update_ctr(msiof_spi_t *msiof, uint32_t clr_bits, uint32_t set_bits);

extern int dma_init(msiof_spi_t *msiof);
extern void dma_deinit(msiof_spi_t *msiof);
extern int dma_xfer(msiof_spi_t *msiof, spi_dma_paddr_t *paddr, int len_done, int dlen, int len);

#endif

