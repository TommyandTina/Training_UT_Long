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

#ifndef _PROTO_H_INCLUDED
#define _PROTO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/hwinfo.h>
#include <drvr/hwinfo.h>
#include <hw/inout.h>
#include <hw/i2c.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <aarch64/r-car.h>
#include <unistd.h>
#include <hw/dma.h>
#include <hw/rcar_cpg.h>
#include <hw/hwinfo_rcar.h>
#include <sys/mman.h>

//#define I2C_CLOCK_USING_EQUATION_3

#define	CMODE_SEND          0x00000001
#define	CMODE_RECV          0x00000002
#define	CMODE_DOSTOP        0x00000004
#define	CMODE_DODMA         0x00000008
#define	CMODE_DODMACONT     0x00000010

#define	VERBOSE_QUIET       0
#define	VERBOSE_LEVEL1      1
#define	VERBOSE_LEVEL2      2
#define	VERBOSE_LEVEL4      4
#define	VERBOSE_LEVEL8      8

#define I2C_DEFAULT_PCLK    133333333
#define I2C_STANDARD_BAUD   100000
#define I2C_FAST_BAUD       400000
#define I2C_FASTPLUS_BAUD   1000000

#define OUTPUT_MODE_OD           0      /* Open drain buffer */
#define OUTPUT_MODE_OD_FMP_18V   1      /* Open drain buffer Fast mode+ 1.8V */
#define OUTPUT_MODE_OD_FMP_33V   2      /* Open drain buffer Fast mode+ 3.3V */
#define OUTPUT_MODE_LVTTL        3      /* Low drive only buffer */

#define RCAR_I2C_BUSY_TIMEOUT        500000

#define RCAR_I2C_ICMCR_DISABLE_START (RCAR_ICMCR_MDBS | RCAR_ICMCR_MIE)
#define RCAR_I2C_ICMCR_PHASE_START   (RCAR_ICMCR_MDBS | RCAR_ICMCR_MIE | RCAR_ICMCR_ESG)
#define RCAR_I2C_ICMCR_PHASE_STOP    (RCAR_ICMCR_MDBS | RCAR_ICMCR_MIE | RCAR_ICMCR_FSB)
//#define RCAR_I2C_ICMCR_MASK_DATA   (~(RCAR_ICMCR_ESG | RCAR_ICMCR_FSB) & 0xFF)
#define RCAR_I2C_ICMCR_MASK_DATA     (~(RCAR_ICMCR_ESG) & 0xFF)

#define RCAR_I2C_ICMIER_ALL          (RCAR_ICMIER_MNRE | RCAR_ICMIER_MALE | RCAR_ICMIER_MSTE | RCAR_ICMIER_MATE)
#define RCAR_I2C_ICMIER_SEND         (RCAR_I2C_ICMIER_ALL | RCAR_ICMIER_MDEE)
#define RCAR_I2C_ICMIER_RECV         (RCAR_I2C_ICMIER_ALL | RCAR_ICMIER_MDRE)

/* For DMA transfer */
#define RCAR_I2C_DMA_PRI             15  /* (I2C3, I2C4, I2C5, I2C6) support DMA channel from 0 to 15 */
#define RCAR_I2C_DMA_LENGTH_REQ      8
#define RCAR_I2C_DMA_BUF_SIZE        256
#define RCAR_I2C_PRIORITY            21

#define RCAR_I2C_DEF_INTR_PRIORITY   21
#define RCAR_I2C_EVENT               (_PULSE_CODE_MINAVAIL+1)
#define RCAR_I2C_DMA_EVENT           (_PULSE_CODE_MINAVAIL+2)

/* I2C Controller Specific Registers */
#define RCAR_I2C_SIZE       0x100

#define TIMEOUT_MULTI               1000
#define LOOP_TIMEOUT                1000

extern int verbose;

#define rcar_i2c_slogf(dev, vl, msg, ...) \
            if (vl == VERBOSE_QUIET) \
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, msg, ##__VA_ARGS__); \
            else if (vl <= dev->verbose) \
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, msg, ##__VA_ARGS__)

typedef struct {
    int                 en;
    int                 coid;
    struct sigevent     event;
    dma_functions_t     funcs;
    int16_t             chan_min;
    int16_t             chan_max;
    int16_t             tx_reqid;
    int16_t             rx_reqid;
    void               *hndl;
    dma_addr_t          buf;
    int                 smmu;
    char               *typed_mem;
    struct smmu_object *smmu_obj;
} i2c_dma_t;

typedef struct {
    uintptr_t           regbase;
    paddr_t             physbase;
    int                 chan_idx;
    unsigned            slave_addr;
    i2c_addrfmt_t       slave_addr_fmt;

    int                 irq;
    int                 intr_count;
    int                 iid;
    int                 chid;
    int                 coid;
    struct sigevent     intrEvent;
    int                 bus_busy_timeout;

    uint8_t             *buf;
    int                 len;
    unsigned            mode;
    unsigned            status;
    int                 isr;
    int                 output_mode;
    int                 verbose;

    /* for I2C clock */
    uint32_t            busSpeed;
    unsigned            pck;        /* peripheral clock */
    unsigned            scl_freq;
    unsigned long       scl_period;    /* in ns */
    uint32_t            icccr;
    uint32_t            smd;
    uint32_t            schd;
    uint32_t            scld;

    /* SW Reset Variables */
    int                 reset_id;

    /* DMA */
    i2c_dma_t           dma;

    uint8_t             product_id;
} rcar_i2c_dev_t;

const struct sigevent* rcar_i2c_intr(void* arg);
int rcar_i2c_wait_complete(rcar_i2c_dev_t *dev);
int rcar_i2c_parse_options(rcar_i2c_dev_t *dev, int argc, char *argv[]);
void *rcar_i2c_init(int argc, char *argv[]);
void rcar_i2c_fini(void *hdl);
int rcar_i2c_reset(rcar_i2c_dev_t *dev);
int rcar_i2c_set_slave_addr(void *hdl, unsigned int addr, i2c_addrfmt_t fmt);
int rcar_i2c_set_bus_speed(void *hdl, unsigned int speed, unsigned int *ospeed);
int rcar_i2c_version_info(i2c_libversion_t *version);
int rcar_i2c_driver_info(void *hdl, i2c_driver_info_t *info);
i2c_status_t rcar_i2c_recv(void *hdl, void *buf, unsigned int len, unsigned int stop);
i2c_status_t rcar_i2c_send(void *hdl, void *buf, unsigned int len, unsigned int stop);
void rcar_i2c_clock_init(rcar_i2c_dev_t  *dev);

int rcar_i2c_dma_init(rcar_i2c_dev_t *dev);
void rcar_i2c_dma_deinit(rcar_i2c_dev_t *dev);
i2c_status_t rcar_i2c_dma_recv(rcar_i2c_dev_t *dev);
i2c_status_t rcar_i2c_dma_send(rcar_i2c_dev_t *dev);
#endif /* _PROTO_H_INCLUDED */
