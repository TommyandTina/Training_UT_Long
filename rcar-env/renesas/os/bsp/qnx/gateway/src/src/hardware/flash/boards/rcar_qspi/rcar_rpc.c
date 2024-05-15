/*
 * $QNXLicenseC:
 * Copyright 2016, 2017 QNX Software Systems.
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

#include "rcar_rpc.h"
#include <aarch64/r-car.h>
#include <hw/rcar_cpg.h>

static cpg_mgr_funcs_t cpg_hwfuncs;
static rcar_rpc_t  rcar_rpc = {0,};
char *bsargs = NULL;

#define HYPER_SMENR_4BIT_SIZES (RCAR_RPC_SMENR_CDB(2) | RCAR_RPC_SMENR_OCDB(2) | \
                               RCAR_RPC_SMENR_ADB(2) | RCAR_RPC_SMENR_OPDB(2) | \
                               RCAR_RPC_SMENR_SPIDB(2))

#define RPC_READ_CACHE_FLUSH  { \
                                out32(rpc->vbase + RCAR_RPC_DRCR, in32(rpc->vbase + RCAR_RPC_DRCR) | RCAR_RPC_DRCR_RCF); \
                                in32(rpc->vbase + RCAR_RPC_DRCR); \
                              } while(0)

/* 08/29/2022:
 * [defect]
 * RPC data cache hit logic has a problem.
 * Reading RPC registers with CacheON may hit the data cache by mistake.
 * Wrong data is read.
 *
 * [WA]
 * Set the HS bit of the PHYCNT register to 0 before accessing the register, and set the HS bit to 1 after the access.
 * (Cache is disabled during register access.)
 */

static void rpc_set_clock(rpc_dev_t *dev, uint32_t clock)
{
    rcar_rpc_t *rpc = dev->rpc;

    /* Is it OK to be HiZ for Flash Side data pins ?
     * Renesas board use Cypress flash, 
     * it is OK to be HiZ for Flash Side data pins
     */
    out32(rpc->vbase + RCAR_RPC_CMNCR, in32(rpc->vbase + RCAR_RPC_CMNCR)
                            | RCAR_RPC_CMNCR_MOIIO_HIZ);

    cpg_hwfuncs.clk_rate_set(CPG_CLK_RPC, 0, &clock);
}

static void rpc_qspi_config(rpc_dev_t *dev)
{
    rcar_rpc_t *rpc = dev->rpc;

    out32(rpc->vbase + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_CAL // PHY calibration
                            | RCAR_RPC_PHYCNT_PHYMEM(0)     // Serial flash in SDR mode
                            | RCAR_RPC_CMNCR_REV);

    out32(rpc->vbase + RCAR_RPC_CMNCR, RCAR_RPC_CMNCR_MD    // Manual mode
                            | RCAR_RPC_CMNCR_SFDE           // Undocumented bit, but they must be set (found out without this setting, must swap read/write data)
                            | RCAR_RPC_CMNCR_MOIIO_HIZ 
                            | RCAR_RPC_CMNCR_IOFV_HIZ 
                            | RCAR_RPC_CMNCR_BSZ(0));       // 1 Serial Flash chip

    out32(rpc->vbase + RCAR_RPC_SMDRENR, 0);
    out32(rpc->vbase + RCAR_RPC_DRDMCR, 0);
    out32(rpc->vbase + RCAR_RPC_SSLDR, 4 << 8); // Use default value
    out32(rpc->vbase + RCAR_RPC_DRCR, 0);
    out32(rpc->vbase + RCAR_RPC_DREAR, 0);
    out32(rpc->vbase + RCAR_RPC_DRENR, 0);
    out32(rpc->vbase + RCAR_RPC_SMCR, 0);
}

static void rpc_hyper_config(rpc_dev_t *dev)
{
    rcar_rpc_t *rpc = dev->rpc;

    out32(rpc->vbase + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_CAL // PHY calibration
                            //| RCAR_RPC_PHYCNT_HS
                            | RCAR_RPC_PHYCNT_STRTIM(rpc->strobe)
                            | RCAR_RPC_PHYCNT_PHYMEM(3)     // HyperFlash
                            | RCAR_RPC_CMNCR_REV);

    out32(rpc->vbase + RCAR_RPC_CMNCR, RCAR_RPC_CMNCR_MD    // Manual mode
                            | RCAR_RPC_CMNCR_SFDE           // Undocumented bit, but they must be set
                            | RCAR_RPC_CMNCR_MOIIO_HIZ 
                            | RCAR_RPC_CMNCR_IOFV_HIZ
                            | RCAR_RPC_CMNCR_BSZ(1));       // QSPI Flash x 2 or HyperFlash

    out32(rpc->vbase + RCAR_RPC_SMOPR, 0);

    out32(rpc->vbase + RCAR_RPC_SMDRENR,  RCAR_RPC_SMDRENR_HYPE(5) // HyperFlash or Octal-SPI flash in DDR mode
                            | RCAR_RPC_SMDRENR_ADDRE               // Enable Address DDR transfer
                            | RCAR_RPC_SMDRENR_SPIDRE
                            | RCAR_RPC_SMDRENR_OPDRE);             // Enable DATA DDR transfer

    out32(rpc->vbase + RCAR_RPC_SSLDR, 0x00010101);
}

static void rpc_xfer_init(rpc_dev_t *dev, int dummy)
{
    rcar_rpc_t *rpc = dev->rpc;

    // Disable fucntion
    out32(rpc->vbase + RCAR_RPC_SMCR, 0x00000000);

    // data dummy control
    if (dummy != 0) {
        out32(rpc->vbase + RCAR_RPC_SMDMCR, dummy - 1);
    }
}

static int rpc_wait_flag(rpc_dev_t *dev, int mask, int value, int timeout)
{
    rcar_rpc_t *rpc = dev->rpc;

    while(timeout-- > 0) {
        if((in32(rpc->vbase + RCAR_RPC_CMNSR) & mask) == value)
            break;
        nanospin_ns(10);
    }

    if (!timeout)
        return EBUSY;
    else
        return EOK;
}

static void rpc_hyper_ext_mode_init(rpc_dev_t *dev)
{
    rcar_rpc_t *rpc = dev->rpc;

    out32(rpc->vbase + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_CAL    // PHY calibration
                            //| RCAR_RPC_PHYCNT_HS               // High speed using DMA transfer
                            | RCAR_RPC_PHYCNT_STRTIM(rpc->strobe)
                            | RCAR_RPC_PHYCNT_PHYMEM(3)        // HyperFlash
                            | RCAR_RPC_CMNCR_REV );

    out32(rpc->vbase + RCAR_RPC_CMNCR, 0                       // External address space read mode
                            | RCAR_RPC_CMNCR_SFDE              // Undocumented bit, but they must be set
                            | RCAR_RPC_CMNCR_MOIIO_HIZ 
                            | RCAR_RPC_CMNCR_IOFV_HIZ 
                            | RCAR_RPC_CMNCR_BSZ(1));          // HyperFlash chip

    out32(rpc->vbase + RCAR_RPC_DRCR, 0);

    out32(rpc->vbase + RCAR_RPC_DRCMR, 0x00A00000);            // Command

    out32(rpc->vbase + RCAR_RPC_DRENR, RCAR_RPC_DRENR_CDB_4BIT
                            | RCAR_RPC_DRENR_OCDB_4BIT
                            | RCAR_RPC_DRENR_ADB_4BIT
                            | RCAR_RPC_DRENR_OPDP_4BIT
                            | RCAR_RPC_DRENR_DRDB_4BIT
                            | RCAR_RPC_DRENR_DME
                            | RCAR_RPC_DRENR_CDE
                            | RCAR_RPC_DRENR_OCDE
                            | RCAR_RPC_DRENR_ADE(4));

    out32(rpc->vbase + RCAR_RPC_DRDMCR, 0x0000000E);

    out32(rpc->vbase + RCAR_RPC_DRDRENR, RCAR_RPC_SMDRENR_SPIDRE // DDR transfer
                            | RCAR_RPC_SMDRENR_ADDRE             // DDR transfer
                            | RCAR_RPC_SMDRENR_HYPE(5));         // HyperFlash

    out32(rpc->vbase + RCAR_RPC_PHYOFFSET1, (in32(rpc->vbase + RCAR_RPC_PHYOFFSET1) & ~RCAR_RPC_PHYOFFSET1_DDRTMG(3))
                            | RCAR_RPC_PHYOFFSET1_DDRTMG(3)
                            | RCAR_RPC_PHYOFFSET1_REV);

    out32(rpc->vbase + RCAR_RPC_PHYOFFSET2, (in32(rpc->vbase + RCAR_RPC_PHYOFFSET2) & ~RCAR_RPC_PHYOFFSET2_OCTTMG(7))
                            | RCAR_RPC_PHYOFFSET2_OCTTMG(4)
                            | RCAR_RPC_PHYOFFSET2_REV);

    out32(rpc->vbase + RCAR_RPC_PHYINT, in32(rpc->vbase + RCAR_RPC_PHYINT) | 0x01070002);

    out32(rpc->vbase + RCAR_RPC_PHYCNT, in32(rpc->vbase + RCAR_RPC_PHYCNT)
                            | RCAR_RPC_PHYCNT_HS); // High Speed using DMA transfer
}

/* SPI flash interface calls */
static void rcar_write_protection_disable(rpc_dev_t *dev)
{
    rcar_rpc_t *rpc = dev->rpc;
    uint32_t dataL = 0;

    dataL = in32(rpc->vbase + RCAR_RPC_PHYINT);

    if(dataL & RCAR_RPC_PHYINT_WPVAL) {   //bit1:  WPVAL(0:RPC_WP#=H(Protect Disable), 1:RPC_WP#=L(Protect Enable))
        dataL &= ~RCAR_RPC_PHYINT_WPVAL;
        out32(rpc->vbase + RCAR_RPC_PHYINT, dataL);
    }
}

static int smenr_len_2_regbit(int len)
{
    switch (len) {
        case 0:
            return RCAR_RPC_SMENR_SPIDE(0x0);
        case 1:
            return RCAR_RPC_SMENR_SPIDE(0x8);
        case 2:
            return RCAR_RPC_SMENR_SPIDE(0xC);
        case 3:
        default:
            return RCAR_RPC_SMENR_SPIDE(0xF);
    }
}

static int rpc_wait_tx_end(rpc_dev_t *dev)
{
    return rpc_wait_flag(dev, RCAR_RPC_CMNSR_TEND, RCAR_RPC_CMNSR_TEND, 1000);
}

static inline void rpc_qspi_read_data(rpc_dev_t *dev, uint8_t *data, int len)
{
    rcar_rpc_t *rpc = dev->rpc;
    uint32_t read_data = in32(rpc->vbase + RCAR_RPC_SMRDR0);

#if RCAR_RPC_CMNCR_SFDE == 0
    for (int i = len; i > 0; i--) {
         *data++ = (uint8_t)(read_data >> 24);
         read_data <<= 8;
    }
#else
    memcpy(data, &read_data, len);
#endif
}

static inline void rpc_qspi_write_data(rpc_dev_t *dev, uint8_t *data, int len)
{
    rcar_rpc_t *rpc = dev->rpc;
    uint32_t write_data = 0;

    if (len == 1)
        memcpy((void *)rpc->vbase + RCAR_RPC_SMWDR0, data, 1);
    else if (len == 2)
        memcpy((void *)(rpc->vbase + RCAR_RPC_SMWDR0), data, 2);
    else {
        memcpy(&write_data, data, len);
        out32(rpc->vbase + RCAR_RPC_SMWDR0, write_data);
    }
}

static int rpc_qspi_ext_read(void *hdl, uint8_t cmd, int csize, uint32_t addr, int alen, int asize, void *data, int dlen, int dsize, int dummy)
{
    rpc_dev_t   *dev = hdl;
    rcar_rpc_t  *rpc = dev->rpc;
    uint32_t    drenr;
    int         len;
    int         read_burst = 0;

    /* This calculation is to make len divisible by 8 */
    len = (dlen + 7) & ~7;

    if (addr + len > RCAR_RPC_MAP_SIZE)
        return -1;

    if ((len % 256) == 0 && !(addr & 0xFF))
        read_burst = 1;

    /* 
     * Set External read Mode using DMA
     */
    out32(rpc->vbase + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_STRTIM(rpc->strobe) // Internal strobe delay
                            | RCAR_RPC_CMNCR_REV);
    out32(rpc->vbase + RCAR_RPC_PHYCNT, in32(rpc->vbase + RCAR_RPC_PHYCNT)
                            | RCAR_RPC_PHYCNT_CAL);               // PHY calibration

    out32(rpc->vbase + RCAR_RPC_CMNCR, RCAR_RPC_CMNCR_SFDE  // Undocumented bit, but they must be set (found out without this setting, must swap read/write data)
                            | RCAR_RPC_CMNCR_MOIIO_HIZ 
                            | RCAR_RPC_CMNCR_IOFV_HIZ 
                            | RCAR_RPC_CMNCR_BSZ(0));       // 1 Serial Flash chip

    out32(rpc->vbase + RCAR_RPC_DRCR, read_burst ?
                             RCAR_RPC_DRCR_RBE              // Read Burst 
                           | RCAR_RPC_DRCR_RBURST(32)       // 32 continuous data units
                           | RCAR_RPC_DRCR_SSLE
                           : 0);

    out32(rpc->vbase + RCAR_RPC_DREAR, 1);	 // External address bits[25:0]

    /* Set Data Read Enable */
    drenr = RCAR_RPC_DRENR_CDB(csize)   // Command Bit Size : 1 or 4 bits
         |  RCAR_RPC_DRENR_ADB(asize)   // Address Bit Size : 1 or 4 bits
         |  RCAR_RPC_DRENR_DRDB(dsize)  // Transfer Data Bit Size : 1 or 4 bits
         |  RCAR_RPC_DRENR_CDE;         // Command enable : Enable

    if(alen == 3)
        drenr |= RCAR_RPC_DRENR_ADE(0x7);    // Address Enable : ADR[23:0] is output
    else if (alen == 4)
        drenr |= RCAR_RPC_DRENR_ADE(0xF);    // Address Enable : ADR[31:0] is output 

    /* Set Command */
    out32(rpc->vbase + RCAR_RPC_DRCMR, cmd << 16);

    /* Set Dummy */
    if(dummy) {
        drenr |= RCAR_RPC_DRENR_DME; // Dummy enable : Enable
        out32(rpc->vbase + RCAR_RPC_DRDMCR, dummy - 1);
    }

    out32(rpc->vbase + RCAR_RPC_DRENR, drenr);

    out32(rpc->vbase + RCAR_RPC_DRDRENR, 0);

    /* High Speed for DMA transfer */
    if(read_burst == 1)
        out32(rpc->vbase + RCAR_RPC_PHYCNT, in32(rpc->vbase + RCAR_RPC_PHYCNT)
                            | RCAR_RPC_PHYCNT_HS);

    /* FIXME : If not using the cache, we need to flush the cache? */
    if(!read_burst)
        RPC_READ_CACHE_FLUSH;

    rpc_dma_setup(dev, len, RCAR_RPC_READ, addr);
    if (rpc_dma_xfer(dev, len, RCAR_RPC_READ) != EOK) {
        goto fail;
    }

    /* Copy data */
    memcpy(data, (void *)rpc->dbuf.vaddr, dlen); 

    if (read_burst) {
        /* FIXME : We don't know whether next access is continue or not, do we need to deassert QSPIn_SSL line here? */
        out32(rpc->vbase + RCAR_RPC_DRCR, in32(rpc->vbase + RCAR_RPC_DRCR) | RCAR_RPC_DRCR_SSLN);

        if (rpc_wait_flag(dev, RCAR_RPC_CMNSR_SSLF, 0, 1000) != EOK)
            goto fail;
    }

    return dlen;

fail:

    return -1;
}

static int rpc_qspi_read(void *hdl, uint8_t cmd, int csize, uint32_t addr, int alen, int asize, void *data, int dlen, int dsize, int dummy)
{
    rpc_dev_t  *dev = hdl;
    rcar_rpc_t *rpc = dev->rpc;
    int        nbytes, pos = 0;
    uint32_t   smenr, smcr;
    uint8_t    *rdata = (uint8_t *)data;

    /* Make sure previous tranfer was completed */
    if (rpc_wait_tx_end(dev))
        return (-1);

    if (cmd == QSPI_CMD_4QOR || cmd == QSPI_CMD_QOR || cmd == QSPI_CMD_4FREAD || cmd == QSPI_CMD_FREAD) {
        if ((rpc_qspi_ext_read(hdl, cmd, csize, addr, alen, asize, data, dlen, dsize, dummy)) > 0)
            return dlen;
    }

    /*
     * Set Manual Mode 
     */
    rpc_qspi_config(dev);

    smenr = RCAR_RPC_SMENR_CDB(csize)    // Command Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_ADB(asize)    // Address Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_SPIDB(dsize)  // Transfer Data Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_CDE           // Command enable : Enable
          | RCAR_RPC_SMENR_SPIDE(smenr_len_2_regbit(dlen));   // Transfer Data Enable : 8/16/32 bits

    if(alen == 3)
        smenr |= RCAR_RPC_SMENR_ADE(0x7);  // Address Enable : ADR[23:0] is output
    else if (alen == 4)
        smenr |= RCAR_RPC_SMENR_ADE(0xF);  // Address Enable : ADR[31:0] is output 

    /* Set Dummy */
    if(dummy) {
        smenr |= RCAR_RPC_SMENR_DME;
        out32(rpc->vbase + RCAR_RPC_SMDMCR, RCAR_RPC_DRDMCR_DMCYC(dummy - 1));
    }

    /* Set command */
    out32(rpc->vbase + RCAR_RPC_SMCMR, cmd << 16);

    /* Set Address */
    out32(rpc->vbase + RCAR_RPC_SMADR, addr);

    smcr = RCAR_RPC_SMCR_SPIRE | RCAR_RPC_SMCR_SPIE;

    if (alen == 0 && dlen > 4)
         smcr |= RCAR_RPC_SMCR_SSLKP;

    while (pos < dlen) {
        nbytes = dlen - pos;
        if (nbytes > 4)
            nbytes = 4;
        else
            smcr &= ~RCAR_RPC_SMCR_SSLKP;

        /* Start transfer */
        out32(rpc->vbase + RCAR_RPC_SMENR, smenr);
        out32(rpc->vbase + RCAR_RPC_SMCR,  smcr);

        /* Waiting for transfer end */
        if (rpc_wait_tx_end(dev))
            goto fail;

        /* Read data */
        rpc_qspi_read_data(dev, rdata, nbytes);

        if (!pos && alen == 0 && dlen > 4)
            smenr = RCAR_RPC_SMENR_SPIDE(0xF); // Disable CDE/ADE. For register transfer with multiple data, address increment is not needed

        if(alen) {
            /* Set new address */
            addr += 4;
            out32(rpc->vbase + RCAR_RPC_SMADR, addr);
        }

        pos += nbytes;
        rdata += nbytes;
    }

    return dlen;

fail:
    return (-1);
}

static int rpc_qspi_write_buffer(void *hdl, uint8_t cmd, int csize, uint32_t addr, int alen, int asize, uint8_t *data, int dlen, int dsize)
{
    rpc_dev_t   *dev = hdl;
    rcar_rpc_t  *rpc = dev->rpc;
    uint32_t    smenr, smcr;

    //out32(rpc->vbase + RCAR_RPC_PHYCNT, 0);
    rpc_qspi_config(dev);

    /*  Read Cache Clear */
    out32(rpc->vbase + RCAR_RPC_DRCR, RCAR_RPC_DRCR_RCF);
    //RPC_READ_CACHE_FLUSH;

    //rpc_qspi_config(dev);

    /* Set Manual Mode */
    smenr = RCAR_RPC_SMENR_CDB(csize)
          | RCAR_RPC_SMENR_ADB(asize)
          | RCAR_RPC_SMENR_SPIDB(dsize)
          | RCAR_RPC_SMENR_CDE            				     
          | RCAR_RPC_SMENR_SPIDE(smenr_len_2_regbit(dlen));
    if (alen == 3)
        smenr |= RCAR_RPC_SMENR_ADE(0x7);  // Enable Address Output : ADR[23:0]
    else if (alen == 4)
        smenr |= RCAR_RPC_SMENR_ADE(0xF);  // Enable Address Output : ADR[31:0]
    else
        goto  fail;

    out32(rpc->vbase + RCAR_RPC_PHYCNT, 0
                            | RCAR_RPC_PHYCNT_PHYMEM(0)   // Serial flash in SDR mode
                            | RCAR_RPC_PHYCNT_STRTIM(rpc->strobe)  // Internal strobe delay
                            | RCAR_RPC_PHYCNT_WBUF        // Write Buffer Enable
                            | RCAR_RPC_PHYCNT_WBUF2       // Write Buffer Enable 2
                            | RCAR_RPC_CMNCR_REV);

    out32(rpc->vbase + RCAR_RPC_PHYCNT, in32(rpc->vbase + RCAR_RPC_PHYCNT)
                              | RCAR_RPC_PHYCNT_CAL);     // PHY calibration

    memcpy((void *)(rpc->dbuf.vaddr + RCAR_TXDMA_OFF), data, dlen);

    rpc_dma_setup(dev, dlen, RCAR_RPC_WRITE, RCAR_TXDMA_OFF);

    if (rpc_dma_xfer(dev, dlen, RCAR_RPC_WRITE) != EOK)
        goto fail;

    /* Set command */
    out32(rpc->vbase + RCAR_RPC_SMCMR, cmd << 16);

     /* Set address */
    out32(rpc->vbase + RCAR_RPC_SMADR, addr);

    out32(rpc->vbase + RCAR_RPC_SMENR, smenr);

    smcr = RCAR_RPC_SMCR_SPIWE | RCAR_RPC_SMCR_SPIE;

    /* Start transfer */
    out32(rpc->vbase + RCAR_RPC_SMCR, smcr);

    /* Wait for transfer complete */
    if (rpc_wait_tx_end(dev))
        goto fail;

    RPC_READ_CACHE_FLUSH;
    rpc_qspi_config(dev);

    return EOK;

fail:
    return (-1);	
}

static int rpc_qspi_write(void *hdl, uint8_t cmd, int csize, uint32_t addr, int alen, int asize, void *data, int dlen, int dsize)
{
    rpc_dev_t   *dev = hdl;
    rcar_rpc_t  *rpc = dev->rpc;
    uint8_t     *buf = (uint8_t *)data;
    uint32_t    smenr, smcr;
    int         nbytes, pos = 0;

    /* Make previous tranfer was completed */
    if (rpc_wait_tx_end(dev))
        return (-1);

    if ((cmd == QSPI_CMD_4PP || cmd == QSPI_CMD_PP) && (rpc->qspi_wbuf == 1)) {
        while ((dlen - pos) >= 256) {
            if (rpc_qspi_write_buffer(hdl, cmd, csize, addr + pos, alen, asize, buf, 256, dsize))
                break;
            pos += 256;
            buf += 256;
        }
        if( dlen == pos) return dlen;
    }

    rpc_qspi_config(dev);

    smcr = RCAR_RPC_SMCR_SPIE;
    if ((dlen - pos) > 0)
        smcr |= RCAR_RPC_SMCR_SSLKP;

    /* Set Manual Mode */
    smenr = RCAR_RPC_SMENR_CDB(csize)    // Command Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_ADB(asize)    // Address Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_CDE;          // Command enable : Enable

    if (alen == 3)
        smenr |= RCAR_RPC_SMENR_ADE(0x7);  // Enable Address Output : ADR[23:0]
    else if (alen == 4)
        smenr |= RCAR_RPC_SMENR_ADE(0xF);  // Enable Address Output : ADR[31:0]

    /* Set command */
    out32(rpc->vbase + RCAR_RPC_SMCMR, cmd << 16);

    /* Set Address */
    out32(rpc->vbase + RCAR_RPC_SMADR, addr + pos);

    /* Start Address & command transfer */
    out32(rpc->vbase + RCAR_RPC_SMENR, smenr);
    out32(rpc->vbase + RCAR_RPC_SMCR, smcr);

    /* Waiting for stransfer end */
    if (rpc_wait_tx_end(dev))
        goto fail;

    if (dlen == 0) // For erase,
        return 1;

    smcr |= RCAR_RPC_SMCR_SPIWE;

    smenr = RCAR_RPC_SMENR_SPIDB(dsize);   // Transfer Data Bit Size : 1/4 bits
    if ((dlen - pos) >= 4)
        smenr |= RCAR_RPC_SMENR_SPIDE(0xF);              // Enable data transfer : 32 bits

    while (pos < dlen) {
        nbytes = dlen - pos;
        if (nbytes >= 4) {
            nbytes = 4;
        } else if (nbytes >= 2){
            nbytes = 2;
            smenr &= ~RCAR_RPC_SMENR_SPIDE(0xF);
            smenr |= RCAR_RPC_SMENR_SPIDE(0xC);  // Enable data transfer : 16 bits
        } else {
            nbytes = 1;
            smenr &= ~RCAR_RPC_SMENR_SPIDE(0xF);
            smenr |= RCAR_RPC_SMENR_SPIDE(0x8);  // Enable data transfer : 8 bits
        }
        if ((dlen - pos - nbytes) == 0) {
            smcr &= ~RCAR_RPC_SMCR_SSLKP;
        }

        /* Write data */
        rpc_qspi_write_data(dev, buf, nbytes);

        /* Start Data transfer */
        out32(rpc->vbase + RCAR_RPC_SMENR, smenr);
        out32(rpc->vbase + RCAR_RPC_SMCR, smcr);

        /* Waiting for stransfer end */
        if (rpc_wait_tx_end(dev))
            goto fail;

        pos += nbytes;
        buf += nbytes;
    }

    return dlen;

fail:
    return (-1);
}


static int rpc_hyper_write_4bytes(rpc_dev_t *dev, uint32_t addr, uint32_t data)
{
    rcar_rpc_t *rpc = dev->rpc;

    /* Set command */
    out32(rpc->vbase + RCAR_RPC_SMCMR, 0x0);

    /* Set address */
    out32(rpc->vbase + RCAR_RPC_SMADR, addr >> 1);

    /* Write data */
    out32(rpc->vbase + RCAR_RPC_SMWDR0, data);

    /* Set Manual Mode */
    out32(rpc->vbase + RCAR_RPC_SMENR, HYPER_SMENR_4BIT_SIZES 
                | RCAR_RPC_SMENR_CDE
                | RCAR_RPC_SMENR_OCDE
                | RCAR_RPC_SMENR_ADE(4)
                | RCAR_RPC_SMENR_SPIDE(0xC));

    /* Start transfer */
    out32(rpc->vbase + RCAR_RPC_SMCR, RCAR_RPC_SMCR_SPIWE | RCAR_RPC_SMCR_SPIE);

    /* Waiting for transfer complete */
    if (rpc_wait_tx_end(dev))
        return -1;

    return 0;
}

static uint32_t rpc_hyper_read_4bytes_reg(rpc_dev_t *dev, uint32_t addr, uint32_t *data)
{
    rcar_rpc_t *rpc = dev->rpc;

    rpc_hyper_config(dev);
    rpc_xfer_init(dev, 16);

    /* Set command */
    out32(rpc->vbase + RCAR_RPC_SMCMR, 0x00800000);

    /* Set address */
    out32(rpc->vbase + RCAR_RPC_SMADR, addr >> 1);

    /* Set dummy */
    // out32(rpc->vbase + RCAR_RPC_SMDMCR, 15); // 16 cycle dummy wait

    /* Set Manual Mode */
    out32(rpc->vbase + RCAR_RPC_SMENR, HYPER_SMENR_4BIT_SIZES 
                | RCAR_RPC_SMENR_DME 
                | RCAR_RPC_SMENR_CDE 
                | RCAR_RPC_SMENR_OCDE 
                | RCAR_RPC_SMENR_ADE(4) 
                | RCAR_RPC_SMENR_SPIDE(0xC));

    out32(rpc->vbase + RCAR_RPC_PHYCNT, in32(rpc->vbase + RCAR_RPC_PHYCNT)
                            | RCAR_RPC_PHYCNT_HS);

    /* Start stransfer */
    out32(rpc->vbase + RCAR_RPC_SMCR, RCAR_RPC_SMCR_SPIRE | RCAR_RPC_SMCR_SPIE);

    /* Waiting for transfer complete */
    if (rpc_wait_tx_end(dev))
        return (-1);

    /* Read data */
    *data = in32(rpc->vbase + RCAR_RPC_SMRDR0);

    return 0;
}

static int rpc_hyper_write_cmd(void *hdl, uint32_t addr, uint32_t data)
{
    rpc_dev_t  *dev = hdl;
    rcar_rpc_t *rpc = dev->rpc;

    rpc_hyper_config(dev);
    rpc_xfer_init(dev, 0);

    /* Set command */
    out32(rpc->vbase + RCAR_RPC_SMCMR, 0x0);

    /* Set address */
    out32(rpc->vbase + RCAR_RPC_SMADR, addr);

    /* Write data */
    out32(rpc->vbase + RCAR_RPC_SMWDR0, data);

    /* Set Manual Mode */
    out32(rpc->vbase + RCAR_RPC_SMENR, HYPER_SMENR_4BIT_SIZES 
                | RCAR_RPC_SMENR_CDE 
                | RCAR_RPC_SMENR_OCDE 
                | RCAR_RPC_SMENR_ADE(4)
                | RCAR_RPC_SMENR_SPIDE(8));

    out32(rpc->vbase + RCAR_RPC_PHYCNT, in32(rpc->vbase + RCAR_RPC_PHYCNT)
                            | RCAR_RPC_PHYCNT_HS);

    /* Start stransfer */
    out32(rpc->vbase + RCAR_RPC_SMCR, RCAR_RPC_SMCR_SPIWE | RCAR_RPC_SMCR_SPIE);

    /* Waiting for transfer complete */
    if (rpc_wait_tx_end(dev))
        return (-1);

    return 0;
}

static int rpc_hyper_read_reg(void *hdl, uint32_t addr, uint8_t *dbuf, int dlen)
{
    rpc_dev_t *dev = hdl;
    uint32_t  rddata;
    uint32_t  add = addr;

    //rpc_hyper_config(dev);

    if (dlen == 1) {
        if (rpc_hyper_read_4bytes_reg(dev, add << 1, &rddata) < 0)
            return (-1);

        *dbuf = (rddata >> 8) & 0xFF;
    } else {
        for (int i = 0; i < dlen; i += 2) {
            if (rpc_hyper_read_4bytes_reg(dev, add << 1, &rddata) < 0)
                return (-1);

            dbuf[i] = (rddata >> 8) & 0xFF;
            dbuf[i + 1] = (rddata >> 24) & 0xFF;
            add += 0x2;
        }
    }

    return dlen;
}

static int rpc_hyper_enable_write(void *hdl)
{
    if (rpc_hyper_write_cmd(hdl, HYPER_UNLOCK1_ADDR, HYPER_UNLOCK1_DATA) < 0)
        return (-1);
    if (rpc_hyper_write_cmd(hdl, HYPER_UNLOCK2_ADDR, HYPER_UNLOCK2_DATA) < 0)
        return (-1);
    return 0;
}

static int rpc_hyper_read_status(void *hdl, uint16_t *status)
{
    rpc_dev_t *dev = hdl;
    uint32_t  rdstatus;

    if (rpc_hyper_write_cmd(hdl, HYPER_READ_STATUS_ADDR, HYPER_READ_STATUS_DATA) < 0)
        return (-1);

    if (rpc_hyper_read_4bytes_reg(dev, 0x0, &rdstatus) < 0)
        return (-1);

    *status = ((rdstatus & 0xFF00) >> 8) | ((rdstatus & 0x00FF) << 8);

    return 0;
}

static int rpc_hyper_write_buffer(void *hdl, uint32_t addr, uint8_t *data, int dlen)
{
    rpc_dev_t   *dev = hdl;
    rcar_rpc_t *rpc = dev->rpc;
    uint16_t status;
    int rc = -1;
    int timeout = 1000;

    rpc_hyper_config(dev);

    if (rpc_hyper_enable_write(hdl) < 0)
        goto fail;

    if (rpc_hyper_write_cmd(hdl, HYPER_ENTRY_ADDR, HYPER_WORD_PROGRAM_DATA) < 0)
        goto fail;

    out32(rpc->vbase + RCAR_RPC_DRCR, RCAR_RPC_DRCR_SSLN
                            | RCAR_RPC_DRCR_RBURST(32)      // 32 continuous data units
                            | RCAR_RPC_DRCR_RCF             // Read Cache Flush
                            | RCAR_RPC_DRCR_RBE             // Read Burst
                            | RCAR_RPC_DRCR_SSLE);

    out32(rpc->vbase + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_CAL // PHY calibration
                            | RCAR_RPC_PHYCNT_WBUF          // Write Buffer Enable
                            | RCAR_RPC_PHYCNT_WBUF2         // Write Buffer Enable2
                            | RCAR_RPC_PHYCNT_PHYMEM(3)     // HyperFlash
                            | RCAR_RPC_CMNCR_REV);

    out32(rpc->vbase + RCAR_RPC_SMADR, addr >> 1);

    out32(rpc->vbase + RCAR_RPC_SMENR, HYPER_SMENR_4BIT_SIZES 
                            | RCAR_RPC_SMENR_CDE            // Command enable
                            | RCAR_RPC_SMENR_OCDE           // Option Command enable
                            | RCAR_RPC_SMENR_ADE(4)         // ADR[23:0] output (24 Bit Address)
                            | RCAR_RPC_SMENR_SPIDE(0xF));   // 64bit transfer

    /* High Speed using DMA transfer */
    out32(rpc->vbase + RCAR_RPC_PHYCNT, in32(rpc->vbase + RCAR_RPC_PHYCNT)
                            | RCAR_RPC_PHYCNT_HS);

    memcpy((void *)(rpc->dbuf.vaddr + RCAR_TXDMA_OFF), data, dlen);

    rpc_dma_setup(dev, dlen, RCAR_RPC_WRITE, RCAR_TXDMA_OFF);

    if (rpc_dma_xfer(dev, dlen, RCAR_RPC_WRITE) == EOK)
        rc = dlen;
    else
        goto fail;

    /* Start transfer */
    out32(rpc->vbase + RCAR_RPC_SMCR, RCAR_RPC_SMCR_SPIWE | RCAR_RPC_SMCR_SPIE);

    /* Wait for transfer complete */
    if (rpc_wait_tx_end(dev))
        goto fail;

    out32(rpc->vbase + RCAR_RPC_DRCR, RCAR_RPC_DRCR_SSLN
                            | RCAR_RPC_DRCR_RBURST(32)      // 32 continuous data units
                            | RCAR_RPC_DRCR_RCF             // Read Cache Flush
                            | RCAR_RPC_DRCR_RBE             // Read Burst
                            | RCAR_RPC_DRCR_SSLE);

    out32(rpc->vbase + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_CAL // PHY calibration
                            | RCAR_RPC_PHYCNT_PHYMEM(3)     // HyperFlash
                            | RCAR_RPC_PHYCNT_WBUF          // Write Buffer Enable
                            | RCAR_RPC_CMNCR_REV);
    while (timeout-- > 0) {
        if (rpc_hyper_read_status(dev, &status) < 0)
            goto fail;

        if (status & HYPER_DEVICE_READY)
            break;

        nanospin_ns(1000);
    }
    if (!timeout)
        goto fail;

    return rc;

fail:
    return (-1);
}

static int rpc_hyper_write_word(void *hdl, uint32_t addr, uint8_t *data, int dlen)
{
    rpc_dev_t   *dev = hdl;
    uint32_t    *writeData = (uint32_t *)data;
    uint32_t    add = addr;
    uint16_t    status;
    int         rc = -1;
    int         i;
    int         timeout = 1000;

    rpc_hyper_config(dev);

    for (i = 0; i < dlen; i += 4) {
        rpc_hyper_enable_write(hdl);
        
        rpc_hyper_write_cmd(hdl, HYPER_ENTRY_ADDR, HYPER_WORD_PROGRAM_DATA);

        if ((rpc_hyper_write_4bytes(dev, add, *writeData++)) < 0)
            goto done;

        while (timeout-- > 0) {
            if (rpc_hyper_read_status(dev, &status) < 0)
                goto done;
            if (status & HYPER_DEVICE_READY) {
                rc = dlen;
                break;
            }
        }
        if(!timeout)
            goto done;

        add += 0x4;
    }
done:
    return rc;
}

static int rpc_hyper_write(void *hdl, uint32_t addr, uint8_t *data, int dlen)
{
    rpc_dev_t *dev = hdl;
    int       pos = 0;
    int       len, buflen;
    uint32_t  clk = 40 * 1000000; // 40Mhz

    /* Set RPC clock to 40Mhz to safely write to hyperflash */
    rpc_set_clock(dev, clk);

    rpc_hyper_config(dev);
    rpc_xfer_init(dev, 0);
    rcar_write_protection_disable(dev);

    len  = (dlen >> 3) * 8;

    while(pos < len) {
        buflen = len - pos;
        if (buflen >= 256)
            buflen = 256;

        if (rpc_hyper_write_buffer(hdl, addr + pos, data + pos, buflen) < 0)
            return (-1);

        pos += buflen;
    }

    buflen = dlen - pos;

    if (buflen) {
        if (rpc_hyper_write_word(hdl, addr + pos, data + pos, buflen) < 0)
            return (-1);
    }

    /* Set RPC clock back to previous value now that the write is done */
    clk = dev->rpc->clock;
    rpc_set_clock(dev, clk);

    return (dlen);
}

static int rpc_hyper_read(void *hdl, uint32_t addr, uint8_t *data, int dlen)
{
    rpc_dev_t  *dev = hdl;
    rcar_rpc_t *rpc = dev->rpc;
    int         len;
    int         rc = -1;

    if (dlen > RCAR_RPC_DMABUF_SIZE) {
        /* The RCAR_RPC_DMABUF_SIZE constant is set to the max message size in the F3S library (F3S_DATA_MAX).
         * The length we get here should never be larger than F3S_DATA_MAX, so RCAR_RPC_DMABUF_SIZE is sized
         * to match it to best use the DMA buffer.  See JI:2525409 for more information
         */
        fprintf(stderr, "RCAR_RPC: Requested read is greater than DMA buffer size\n");
        return (-1);
    }

    /* This calculation is to make 'dlen' divisible by 4 */
    len = (dlen + 7) & ~7;

    rpc_hyper_ext_mode_init(dev);
    rpc_dma_setup(dev, len, RCAR_RPC_READ, addr);

    if (rpc_dma_xfer(dev, len, RCAR_RPC_READ) == EOK) {
        memcpy(data, (void *)rpc->dbuf.vaddr, dlen);
        rc = dlen;
    }

    return rc;
}

static int rpc_dinit(void *hdl)
{
    rpc_dev_t  *dev = hdl;
    rcar_rpc_t *rpc = dev->rpc;

    if (--rpc->ndev)
        goto fini;

    // Disable fucntion
    out32(rpc->vbase + RCAR_RPC_SMCR, 0x00000000);

    ConnectDetach(rpc->coid);
    ChannelDestroy(rpc->chid);
    InterruptDetach(rpc->iid);

    rpc->dmafuncs.free_buffer(rpc->txdma, &rpc->dbuf);
    rpc->dmafuncs.channel_release(rpc->txdma);
    rpc->dmafuncs.channel_release(rpc->rxdma);
    rpc->dmafuncs.fini();

    munmap_device_io(rpc->vbase, rpc->size);

    cpg_hwfuncs.deinit();

    rpc->vbase = 0;

fini:
    if (rpc->dma_ver)
        free(rpc->dma_ver);
    if (rpc->dma_typed_mem)
        free(rpc->dma_typed_mem);
    free(dev);

    return EOK;
}

static void query_hwinfo(rpc_dev_t *dev)
{
    unsigned hwi_off;
    hwi_tag *tag;

    rcar_rpc.pbase = RCAR_RPC_BASE;
    rcar_rpc.size = RCAR_RPC_SIZE;
    rcar_rpc.clock = dev->drate;

    hwi_off = hwi_find_bus(RCAR_HWI_SPI, 0);
    if(hwi_off != HWI_NULL_OFF){
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag){
            rcar_rpc.pbase = tag->location.base;
            rcar_rpc.size = tag->location.len;
        }

        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, 0);
        if(tag){
            rcar_rpc.clock = tag->inputclk.clk/tag->inputclk.div;
        }
    }

    hwi_off = hwi_find_device(RCAR_HWI_SPI_DEV, 0);
    if(hwi_off != HWI_NULL_OFF){
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag){
            rcar_rpc.address = tag->location.base;
            rcar_rpc.window_size = tag->location.len;
        }
    }
}

void* rpc_init(f3s_socket_t *socket)
{
    rpc_dev_t               *dev;
    int                     opt;
    int                     len;
    char                    *value;
    char                    *freeptr;
    char                    *options;
    static char             *opts[] = {"cs", "drate", "mode", "clock", "base", "size", "ver", "typed_mem", "smmu", "wbuf", "strobe", NULL};

    if ((dev = calloc(1, sizeof(rpc_dev_t))) == NULL) {
        fprintf(stderr, "RCAR_RPC: Could not allocate memory\n");
        return (NULL);
    }

    dev->bus   = 1;
    dev->csel  = 0;
    dev->drate = 160000000;

    dev->spic.hcap                      = SPI_CAP_QUAD;
    dev->spic.dinit                     = rpc_dinit;
    dev->spic.spi_read_flash            = rpc_qspi_read;
    dev->spic.spi_write_flash           = rpc_qspi_write;
    dev->spic.spi_hyper_read_registers  = rpc_hyper_read_reg;
    dev->spic.spi_hyper_read_memory     = rpc_hyper_read;
    dev->spic.spi_hyper_write_cmd       = rpc_hyper_write_cmd;
    dev->spic.spi_hyper_write_memory    = rpc_hyper_write;

    query_hwinfo(dev);

    /* Socket args */
    if(socket->address) {
        rcar_rpc.address = socket->address;
    }
    if(socket->window_size) {
        rcar_rpc.window_size = socket->window_size;
    }
    if(!rcar_rpc.address || !rcar_rpc.window_size) {
        fprintf(stderr, "RCAR_RPC: missing device parameters\n");
        goto fail0;
    }

    /* Board Specific args */
    rcar_rpc.qspi_wbuf = 0;
    rcar_rpc.strobe = 15; // For Gen4
    if(bsargs) {
        options = freeptr = strdup(bsargs);
        while (options && *options != '\0') {
            opt = getsubopt(&options, opts, &value);
            switch (opt) {
            case 0:     // chip select
                dev->csel  = strtoul(value, 0, 0);
                break;
            case 1:     // data rate
                dev->drate = strtoul(value, 0, 0);
                break;
            case 2:     // mode
                dev->mode  = strtoul(value, 0, 0);
                break;
            case 3:     // clock
                rcar_rpc.clock = strtoul(value, 0, 0);
                break;
            case 4:     // controller base
                rcar_rpc.pbase = strtoul(value, 0, 0);
                break;
            case 5:     // controller reg size
                rcar_rpc.size = strtoul(value, 0, 0);
                break;
            case 6:     // ver
                if(strlen(value)) {
                    len = 4 + strlen(value) + 1;
                    rcar_rpc.dma_ver = (char *)malloc(len);
                    snprintf(rcar_rpc.dma_ver, len, "ver=%s", value);
                }
                break;
            case 7:     // typed_mem
                if(strlen(value)) {
                    len = strlen(value) + 1;
                    rcar_rpc.dma_typed_mem = (char *)malloc(len);
                    snprintf(rcar_rpc.dma_typed_mem, len, "%s", value);
                }
                break;
            case 8:    // smmu
                if (value) {
                    if ((!strcmp(value, "on"))||(!strcmp(value, "1"))) {
                        if(smmu_init(0) == -1) {
                            fprintf(stderr, "RCAR_RPC: failed to connect to SMMU Manager\n");
                            goto fail0;
                        } else {
                            rcar_rpc.smmu = 1;
                        }
                    } else if ((!strcmp(value, "off"))||(!strcmp(value, "0"))) {
                        rcar_rpc.smmu = 0;
                    }
                }
                break;
            case 9:
                rcar_rpc.qspi_wbuf = 1;
                break;
            case 10:
                rcar_rpc.strobe = strlen(value);
                break;
            }
        }

        free(freeptr);
    }

    // First time called
    if (rcar_rpc.vbase == 0) {
        if(!rcar_rpc.pbase || !rcar_rpc.size) {
            fprintf(stderr, "RCAR_RPC: missing controller parameters\n");
            goto fail0;
        }

        if ((rcar_rpc.vbase = mmap_device_io(rcar_rpc.size, rcar_rpc.pbase)) == (uintptr_t)MAP_FAILED)
            goto fail0;

        /* attach interrupt */
        if ((rcar_rpc.chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1)
            goto fail1;

        if ((rcar_rpc.coid = ConnectAttach(0, 0, rcar_rpc.chid, _NTO_SIDE_CHANNEL, 0)) == -1)
            goto fail2;

        if (rcar_dma_init(&rcar_rpc) != EOK)
            goto fail3;
    }

    rcar_rpc.ndev++;
    dev->rpc = &rcar_rpc;

    if ((cpg_mgr_getfuncs(&cpg_hwfuncs, sizeof(cpg_mgr_funcs_t))) != EOK) {
        fprintf(stderr, "RCAR_RPC: Unable to get cpg hw funcs\n");
    }
    else {
        uint32_t clk = rcar_rpc.clock;
        cpg_hwfuncs.init(1);
        rpc_set_clock(dev, clk);
    }

    return (dev);

fail3:
    ConnectDetach(rcar_rpc.coid);
fail2:
    ChannelDestroy(rcar_rpc.chid);
fail1:
    munmap_device_io(rcar_rpc.vbase, rcar_rpc.size);
fail0:
    if (rcar_rpc.dma_ver)
        free(rcar_rpc.dma_ver);
    if (rcar_rpc.dma_typed_mem)
        free(rcar_rpc.dma_typed_mem);
    free(dev);

    return (NULL);
}
