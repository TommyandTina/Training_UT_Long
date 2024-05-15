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


#ifdef DEFN
    #define EXT
    #define INIT1(a)            = { a }
#else
    #define EXT extern
    #define INIT1(a)
#endif

#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <termios.h>
#include <devctl.h>
#include <sys/dcmd_chr.h>
#include <sys/iomsg.h>
#include <atomic.h>
#include <hw/inout.h>
#include <sys/io-char.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <arm/scif.h>
#include <drvr/hwinfo.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <sys/iofunc.h>
#include <hw/dma.h>
#include <aarch64/r-car-gen3.h>
#include <hw/hwinfo_rcar.h>



#define UART_TYPE_SCIF          1
#define UART_TYPE_HSCIF         2

#define RTS_CTS_HW_DISABLE      0
#define RTS_CTS_HW_ENABLE       1

#define INTR_ISR                1
#define INTR_EVENT              0

#define DMA_ENABLE              1
#define DMA_DISABLE             0

#define DMA_XFER_SIZE           512

#define RCAR_SCIF_EVENT             0x55
#define RCAR_SCIF_RDMA_EVENT        0x56
#define RCAR_SCIF_TDMA_EVENT        0x57
#define RCAR_SCIF_PRIORITY          21

#define SCIF_SCLSR_ORER        (1<<0)
#define SCIF_SCLSR_TO          (1<<2)

#define RCAR_MAX_DMACH          48

/* DMARS: MID+RID. Table 17.5 of Gen3 TRM */
/* HSCIF (CIS) RECEPTION */
static const uint16_t RCAR_GEN3_HSCIF_RX_DMARS[] =
{
    0X30,
    0X32,
    0X34,
    0X36,
    0X38,
};

/* HSCIF (CIS) TRANSMISSION */
static const uint16_t RCAR_GEN3_HSCIF_TX_DMARS[] =
{
    0X31,
    0X33,
    0X35,
    0X37,
    0X39,
};

/* SCIF (CIS) RECEPTION */
static const uint16_t RCAR_GEN3_SCIF_RX_DMARS[] =
{
    0X50,
    0X52,
    0X12, /* TRM use H'18, but tested working value is H'12 (18) */
    0X56,
    0X58,
    0X5A,
};

/* HSCIF (CIS) TRAMSMISION */
static const uint16_t RCAR_GEN3_SCIF_TX_DMARS[] =
{
    0X51,
    0X53,
    0X13, /* TRM uses H'19, but tested working value is H'13 (19) */
    0X57,
    0X59,
    0X5B,
};




typedef struct
{
    dma_addr_t      dbuf;
    int             xfer_size;
    struct sigevent sdma_event;
    void            *dma_chn;
    int             status;
    uint8_t         dmac_rs;
    uint32_t        fifo_addr;
    uint32_t        byte_cnt;
    int             dma_busy;
    int             buffer0;
} dma_handle;

typedef struct
{
    int              status;             /* DMA status */
    unsigned int     R_chn;
    unsigned int     T_chn;
    dma_handle       Rx;
    dma_handle       Tx;
    dma_functions_t  dmafuncs;
    int              coid;
    int              chid;
    pthread_t        tid;
    struct smmu_object *smmu_obj;
    pthread_mutex_t  rx_mutex;
} DMAControl;


typedef struct dev_scif {
    TTYDEV          tty;
    struct dev_scif *next;
    unsigned        sh_intr;
    unsigned        clk;            //Our clock cycle
    unsigned        div;            //Our div value
    uintptr_t       port;           //Our port base value
    int             scif;           // > 0 if we are an scif type (scif - 1 for SH7750, scif = 2 for SH7760)
    uint16_t        rstrg;          //RTS trigger value for SCIF FCR register
    volatile int    rts_flag;       //1 if hw flow control should be on
    uint32_t        baud;           // store the last baud rate. Current baudrate is stored in tty.baud
    uint32_t        c_cflag;        // store the last serial mode. Current c_cflag is stored in tty.c_cflag
    uint8_t         clock_source;   // Clock source
    uint8_t         rts_hw_disable; //Flag to disable the RTS and CTS lines
    uint16_t        rx_intr_mask;
    /* Register offsets */
    uint8_t         tx_reg;
    uint8_t         rx_reg;
    uint8_t         sc_reg;
    uint8_t         fc_reg;
    uint8_t         status_reg;
    uint8_t         tx_fifo_count_reg;
    uint8_t         rx_fifo_count_reg;
    int             mode;
    DMAControl      dma;
    uint8_t         ready;
} DEV_SCIF;

struct dev_list {
    struct dev_list *next;
    DEV_SCIF        *device;
    int             iid;
};

typedef struct ttyinit_scif {
    TTYINIT     tty;
    int         scif;
    uint16_t    rstrg;              //RTS trigger value for SCIF FCR register
    uint8_t     rts_hw_disable;     //Flag to disable the RTS and CTS lines
#define  INTERNAL_CLOCK 0
#define  EXTERNAL_CLOCK 1
    uint8_t     clock_source;       //Clock Source
    int         mode;
    DMAControl  dma;
} TTYINIT_SCIF;

EXT TTYCTRL          ttyctrl;
EXT struct dev_list  *devices;


#include "proto.h"

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serscif/externs.h $ $Rev: 893164 $")
#endif
