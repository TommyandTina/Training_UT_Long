/*
 * $QNXLicenseC:
 * Copyright 2016, 2017 QNX Software Systems.
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

#ifndef _RCAR_RPC_H
#define _RCAR_RPC_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <hw/dma.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>

#include <f3s_spi.h>

#define RCAR_MAX_DEVICE         4

#define RCAR_RPC_WRITE          0
#define RCAR_RPC_READ           1

#define RCAR_RPC_ADDR_MAP       0x08000000
#define RCAR_RPC_MAP_SIZE       0x4000000

#define RCAR_RPC_WRITE_BUF_OFF  0x8000
#define RCAR_RPC_WRITE_BUF_SIZE 0x100

#define RCAR_RPC_EVENT          0x55
#define RCAR_RPC_RDMA_EVENT     0x56
#define RCAR_RPC_TDMA_EVENT     0x57
#define RCAR_RPC_PRIORITY       21

#define RCAR_TxDMA_THRESHOLD    16
#define RCAR_ALIGN_RxBUF(clen)  (4 - (clen & 3))

/* RPC registers, offset from RCAR_RPC_BASE (pbase) */
#define RCAR_RPC_CMNCR          0x00    /* Common control register */
#define RCAR_RPC_SSLDR          0x04    /* SSL delay register  */
#define RCAR_RPC_DRCR           0x0C    /* Data read control register */
#define RCAR_RPC_DRCMR          0x10    /* Data read command setting register */
#define RCAR_RPC_DREAR          0x14    /* Data read extended address setting register */
#define RCAR_RPC_DROPR          0x18    /* Data read option setting register */
#define RCAR_RPC_DRENR          0x1C    /* Data read enable setting register */
#define RCAR_RPC_SMCR           0x20    /* Manual mode control register */
#define RCAR_RPC_SMCMR          0x24    /* Manual mode command setting register */
#define RCAR_RPC_SMADR          0x28    /* Manual mode address setting register */
#define RCAR_RPC_SMOPR          0x2C    /* Manual mode option setting register */
#define RCAR_RPC_SMENR          0x30    /* Manual mode enable setting register */
#define RCAR_RPC_SMRDR0         0x38    /* Manual mode read data register 0 */
#define RCAR_RPC_SMRDR1         0x3C    /* Manual mode read data register 1 */
#define RCAR_RPC_SMWDR0         0x40    /* Manual mode write data register 0 */
#define RCAR_RPC_SMWDR1         0x44    /* Manual mode write data register 1 */
#define RCAR_RPC_CMNSR          0x48    /* Common status register */
#define RCAR_RPC_DRDMCR         0x58    /* Data read dummy cycle setting register */
#define RCAR_RPC_DRDRENR        0x5C    /* Data read DDR enable register */
#define RCAR_RPC_SMDMCR         0x60    /* Manual mode dummy cycle setting register */
#define RCAR_RPC_SMDRENR        0x64    /* Manual mode DDR enable register */
#define RCAR_RPC_PHYCNT         0x7C    /* PHY control register */
#define RCAR_RPC_PHYOFFSET1     0x80    /* PHY offset register 1 */
#define RCAR_RPC_PHYOFFSET2     0x84    /* PHY offset register 2 */
#define RCAR_RPC_PHYINT         0x88    /* PHY interrupt register */
#define RCAR_RPC_DIV_REG        0xA8    /* PHY divider register */
#define	RCAR_RPC_SEC_CONF       0xB8    /* Secure configuration register */
#define	RCAR_RPC_WRBUF          0x8000U /* W	RPC Write buffer (Access size=4/8/16/32/64Byte) */


/* CMNCR bit definition  */
#define RCAR_RPC_CMNCR_BSZ(x)        (((x) & 0x3) << 0)
#define RCAR_RPC_CMNCR_IO0FV(x)      (((x) & 0x3) << 8) 
#define RCAR_RPC_CMNCR_IO2FV(x)      (((x) & 0x3) << 12) // undocumented
#define RCAR_RPC_CMNCR_IO3FV(x)      (((x) & 0x3) << 14) // undocumented
#define RCAR_RPC_CMNCR_IOFV_HIZ (RCAR_RPC_CMNCR_IO0FV(3) | RCAR_RPC_CMNCR_IO2FV(3) | \
                RCAR_RPC_CMNCR_IO3FV(3))
#define RCAR_RPC_CMNCR_MOIIO0(x)     (((x) & 0x3) << 16)
#define RCAR_RPC_CMNCR_MOIIO1(x)     (((x) & 0x3) << 18)
#define RCAR_RPC_CMNCR_MOIIO2(x)     (((x) & 0x3) << 20)
#define RCAR_RPC_CMNCR_MOIIO3(x)     (((x) & 0x3) << 22)
#define RCAR_RPC_CMNCR_MOIIO_0    (RCAR_RPC_CMNCR_MOIIO0(0) | RCAR_RPC_CMNCR_MOIIO1(0) | \
                RCAR_RPC_CMNCR_MOIIO2(0) | RCAR_RPC_CMNCR_MOIIO3(0))
#define RCAR_RPC_CMNCR_MOIIO_1    (RCAR_RPC_CMNCR_MOIIO0(1) | RCAR_RPC_CMNCR_MOIIO1(1) | \
                RCAR_RPC_CMNCR_MOIIO2(1) | RCAR_RPC_CMNCR_MOIIO3(1))
#define RCAR_RPC_CMNCR_MOIIO_LAST    (RCAR_RPC_CMNCR_MOIIO0(2) | RCAR_RPC_CMNCR_MOIIO1(2) | \
                RCAR_RPC_CMNCR_MOIIO2(2) | RCAR_RPC_CMNCR_MOIIO3(2))
#define RCAR_RPC_CMNCR_MOIIO_HIZ    (RCAR_RPC_CMNCR_MOIIO0(3) | RCAR_RPC_CMNCR_MOIIO1(3) | \
                RCAR_RPC_CMNCR_MOIIO2(3) | RCAR_RPC_CMNCR_MOIIO3(3))
#define RCAR_RPC_CMNCR_SFDE          (1 << 24)   // undocumented bit but must be set ?
#define RCAR_RPC_CMNCR_MD            (1 << 31)
#define RCAR_RPC_CMNCR_REV           (0x260)   // undocumented bit but must be set ?


/* SSLDR bit definition  */
#define RCAR_RPC_SSLDR_SCKDL(x)      (x << 0)
#define RCAR_RPC_SSLDR_SLNDL(x)      (x << 8)
#define RCAR_RPC_SSLDR_SPNDL(x)      (x << 16)

/* DRCR bit definition*/
#define RCAR_RPC_DRCR_SSLE           (1 << 0)
#define RCAR_RPC_DRCR_RBE            (1 << 8)
#define RCAR_RPC_DRCR_RCF            (1 << 9)
#define RCAR_RPC_DRCR_RBURST(x)      ((((x) - 1) & 0x1F) << 16)
#define RCAR_RPC_DRCR_SSLN           (1 << 24)

/* DRENR bit definition */
#define RCAR_RPC_DRENR_OPDE(x)       (((x) & 0xF) << 4)
#define RCAR_RPC_DRENR_ADE(x)        (((x) & 0xF) << 8)
#define RCAR_RPC_DRENR_OCDE          (1 << 12)
#define RCAR_RPC_DRENR_CDE           (1 << 14)
#define RCAR_RPC_DRENR_DME           (1 << 15)
#define RCAR_RPC_DRENR_DRDB(x)       (((x >> 1) & 0x3) << 16)
 #define RCAR_RPC_DRENR_DRDB_1BIT    RCAR_RPC_DRENR_DRDB(1)
 #define RCAR_RPC_DRENR_DRDB_4BIT    RCAR_RPC_DRENR_DRDB(4)
#define RCAR_RPC_DRENR_OPDP(x)       (((x >> 1) & 0x3) << 20)
 #define RCAR_RPC_DRENR_OPDP_1BIT    RCAR_RPC_DRENR_OPDP(1)
 #define RCAR_RPC_DRENR_OPDP_4BIT    RCAR_RPC_DRENR_OPDP(4)
#define RCAR_RPC_DRENR_ADB(x)        (((x >> 1) & 0x3) << 24)
 #define RCAR_RPC_DRENR_ADB_1BIT     RCAR_RPC_DRENR_ADB(1)
 #define RCAR_RPC_DRENR_ADB_4BIT     RCAR_RPC_DRENR_ADB(4)
#define RCAR_RPC_DRENR_OCDB(x)       (((x >> 1) & 0x3) << 28)
 #define RCAR_RPC_DRENR_OCDB_1BIT    RCAR_RPC_DRENR_OCDB(1)
 #define RCAR_RPC_DRENR_OCDB_4BIT    RCAR_RPC_DRENR_OCDB(4)
#define RCAR_RPC_DRENR_CDB(x)        (((x >> 1) & 0x3) << 30)
 #define RCAR_RPC_DRENR_CDB_1BIT     RCAR_RPC_DRENR_CDB(1)
 #define RCAR_RPC_DRENR_CDB_4BIT     RCAR_RPC_DRENR_CDB(4)

/* SMCR bit definition */
#define RCAR_RPC_SMCR_SPIE           (1 << 0)
#define RCAR_RPC_SMCR_SPIWE          (1 << 1)
#define RCAR_RPC_SMCR_SPIRE          (1 << 2)
#define RCAR_RPC_SMCR_SSLKP          (1 << 8)

/* SMDRENR bit definition */
#define RCAR_RPC_SMDRENR_SPIDRE       (1 << 0)
#define RCAR_RPC_SMDRENR_OPDRE        (1 << 4)
#define RCAR_RPC_SMDRENR_ADDRE        (1 << 8)
#define RCAR_RPC_SMDRENR_HYPE(x)      (((x) & 0x7) << 12)

/* SMENR bit definition */
#define RCAR_RPC_SMENR_SPIDE(x)       (((x) & 0xF) << 0)
#define RCAR_RPC_SMENR_OPDE(x)        (((x) & 0xF) << 4)
#define RCAR_RPC_SMENR_ADE(x)         (((x) & 0xF) << 8)
#define RCAR_RPC_SMENR_OCDE           (1 << 12)
#define RCAR_RPC_SMENR_CDE            (1 << 14)
#define RCAR_RPC_SMENR_DME            (1 << 15)
#define RCAR_RPC_SMENR_SPIDB(x)       (((x >> 1) & 0x3) << 16)
 #define RCAR_RPC_SMENR_SPIDB_1BIT     RCAR_RPC_SMENR_SPIDB(1)
 #define RCAR_RPC_SMENR_SPIDB_4BIT     RCAR_RPC_SMENR_SPIDB(4)
#define RCAR_RPC_SMENR_OPDB(x)        (((x >> 1) & 0x3) << 20)
 #define RCAR_RPC_SMENR_OPDB_1BIT     RCAR_RPC_SMENR_OPDB(1)
 #define RCAR_RPC_SMENR_OPDB_4BIT     RCAR_RPC_SMENR_OPDB(4)
#define RCAR_RPC_SMENR_ADB(x)         (((x >> 1) & 0x3) << 24)
 #define RCAR_RPC_SMENR_ADB_1BIT      RCAR_RPC_SMENR_ADB(1)
 #define RCAR_RPC_SMENR_ADB_4BIT      RCAR_RPC_SMENR_ADB(4)
#define RCAR_RPC_SMENR_OCDB(x)        (((x >> 1) & 0x3) << 28)
 #define RCAR_RPC_SMENR_OCDB_1BIT     RCAR_RPC_SMENR_OCDB(1)
 #define RCAR_RPC_SMENR_OCDB_4BIT     RCAR_RPC_SMENR_OCDB(4)
#define RCAR_RPC_SMENR_CDB(x)         (((x >> 1) & 0x3) << 30)
 #define RCAR_RPC_SMENR_CDB_1BIT      RCAR_RPC_SMENR_CDB(1)
 #define RCAR_RPC_SMENR_CDB_4BIT      RCAR_RPC_SMENR_CDB(4)

#define RCAR_RPC_SINGLE_NOXFER        0x0
#define RCAR_RPC_SINGLE_8BITS         0x8
#define RCAR_RPC_SINGLE_16BITS        0xC
#define RCAR_RPC_SINGLE_32BITS        0xF

/* CMNSR bit definition*/

#define RCAR_RPC_CMNSR_TEND           (1 << 0)
#define RCAR_RPC_CMNSR_SSLF           (1 << 1)

/* DRDMCR  bit definition */

#define RCAR_RPC_DRDMCR_DMCYC(x)      (((x) & 0x1F) << 0)
#define RCAR_RPC_DRDMCR_DMDB(x)       (x << 16)

/* DRDRENR bit definition */

#define RCAR_RPC_DRDRENR_DRDRE        (1 << 0)
#define RCAR_RPC_DRDRENR_OPDRE        (1 << 4)
#define RCAR_RPC_DRDRENR_ADDRE        (1 << 8)
#define RCAR_RPC_DRDRENR_HYPE(x)      (((x) & 7) << 12)

/* SMDMCR bit definition */

#define RCAR_RPC_SMDMCR_DCMYC(x)      (((x) & 0x1F) << 0)
#define RCAR_RPC_SMDMCR_DMDB(x)       (x << 16)

/* PHYCNT bit definition */

#define RCAR_RPC_PHYCNT_PHYMEM(x)     (((x) & 3) << 0)
#define RCAR_RPC_PHYCNT_WBUF          (1 << 2)
#define RCAR_RPC_PHYCNT_WBUF2         (1 << 4)
#define RCAR_RPC_PHYCNT_STRTIM(x)     ((((x) & 0x7) << 15) | (((x) & 0x8) << 24))
#define RCAR_RPC_PHYCNT_HS            (1 << 18)
#define RCAR_RPC_PHYCNT_OCT           (1 << 20)
#define RCAR_RPC_PHYCNT_EXDS          (1 << 21)
#define RCAR_RPC_PHYCNT_OCTA(x)       (((x) & 0x3) << 22)
#define RCAR_RPC_PHYCNT_CAL           (1 << 31)

/* PHYINT BIT Definitions */
#define RCAR_RPC_PHYINT_RSTEN         (1<<18)      // RPC_RESET# Pin Enable
#define RCAR_RPC_PHYINT_WPEN          (1<<17)      // RPC_WP# Pin Enable
#define RCAR_RPC_PHYINT_INTEN         (1<<16)      // RPC_INT# Pin Enable
#define RCAR_RPC_PHYINT_RSTVAL        (1<<2)       // RPC_RESET# Pin Output Value
#define RCAR_RPC_PHYINT_WPVAL         (1<<1)       // RPC_WP# Pin Output Value
#define RCAR_RPC_PHYINT_INT           (1<<0)       // Interrupt Status

/* PHYOFFSET1 BIT Definitions */
#define RCAR_RPC_PHYOFFSET1_DDRTMG(x) (((x) & 3) << 28)
#define RCAR_RPC_PHYOFFSET1_REV       0x1511144    // undocumented bits, but they must be set ?

/* PHYOFFSET2 BIT Definitions */
#define RCAR_RPC_PHYOFFSET2_OCTTMG(x) (((x) & 7) << 8)
#define RCAR_RPC_PHYOFFSET2_REV       0x31         // undocumented bits, but they must be set ?


/* SPI controller specific */
typedef struct {
    unsigned        pbase;
    unsigned        size;
    uintptr_t       vbase;
    unsigned        buffer;
    unsigned        buffer_size;
    paddr_t         address;
    uint32_t        window_size;
    int             chid, coid;
    int             irq, iid;
    uint32_t        strobe;
    uint32_t        clock;
    uint32_t        drate;
    int             dtime;
    struct sigevent spievent;

    dma_functions_t dmafuncs;
    void            *txdma;
    dma_addr_t      dbuf;
    void            *rxdma;
#ifdef USE_SMMU
    int             smmu;
    struct smmu_object *smmu_obj;
#endif
#define RCAR_RPC_DMABUF_SIZE        (16 * 1024)     /* DMA buffer size, set to F3S_DATA_MAX */
#define RCAR_TXDMA_OFF              ( 4 * 1024)     /* Tx DMA buffer offset */
    char            *dma_ver;
    char            *dma_typed_mem;

    pthread_mutex_t mutex;
    int             ndev;
    int             qspi_wbuf;  // Using Write Buffer mode for QSPI flash
} rcar_rpc_t;

/* device specific structure */
typedef struct {
    spi_flash_t     spic;
    uint32_t        mode;
    uint32_t        drate;
    uint32_t        csel;
    uint32_t        bus;
    uint32_t        flags;
#define RCAR_FLAG_MUTEX         (1 << 0)
    rcar_rpc_t      *rpc;
} rpc_dev_t;

int rpc_dma_setup(rpc_dev_t *dev, int len, int dir, int offset);
int rpc_dma_xfer(rpc_dev_t *dev, int len, int dir);
int rcar_dma_init(rcar_rpc_t *rpc);

#endif
