/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#include <string.h>
#include "ipl.h"
#include <hw/inout.h>
#include <aarch64/r-car.h>
#include "rpc.h"
#include "board.h"

#if defined(RCAR_IPL_HYPERFLASH_SUPPORT) || defined(RCAR_IPL_QSPIFLASH_SUPPORT)


static rpc_flash_type flash_type = RPC_FLASH_TYPE_QSPI;

static void rpc_qspi_init()
{
    out32(RCAR_RPC_BASE + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_CAL // PHY calibration
                            | RCAR_RPC_PHYCNT_PHYMEM(0)     // Serial flash in SDR mode
                            | 0x260);                       // The 0x260 are undocumented bits, but they must be set

    out32(RCAR_RPC_BASE + RCAR_RPC_CMNCR, RCAR_RPC_CMNCR_MD    // Manual mode
                            | RCAR_RPC_CMNCR_SFDE           // Undocumented bit, but they must be set (found out without this setting, must swap read/write data)
                            | RCAR_RPC_CMNCR_MOIIO_HIZ 
                            | RCAR_RPC_CMNCR_IOFV_HIZ 
                            | RCAR_RPC_CMNCR_BSZ(0));       // 1 Serial Flash chip

    out32(RCAR_RPC_BASE + RCAR_RPC_SMDRENR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_DRDMCR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_SSLDR, 4 << 8); // Use default value
    out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_DREAR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_DRENR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_SMCR, 0);
}

static void rpc_wait_xfer_end()
{
    volatile uint32_t status = 0;
    while (1) {
        status = in32(RCAR_RPC_BASE + RCAR_RPC_CMNSR);
        if (status & RCAR_RPC_CMNSR_TEND) {
            break;
        }
    }
}

static void rpc_qspi_write_cmd(uint32_t command)
{
    out32(RCAR_RPC_BASE + RCAR_RPC_SMCMR, SMCMR_CMD(command));
        //bit23-16 CMD[7:0] : command

    out32(RCAR_RPC_BASE + RCAR_RPC_SMENR , 0x00004000);
        //bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
        //bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
        //bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
        //bit15    DME        =    0 : No dummy cycle
        //bit14    CDE        =    1 : Command enable
        //bit11-8  ADE[3:0]   = 0000 : Address output disable
        //bit3-0   SPIDE[3:0] = 0000 : No transfer
    out32(RCAR_RPC_BASE + RCAR_RPC_SMCR , 0x00000001);
        //bit2     SPIRE      = 0 : Data read disable
        //bit1     SPIWE      = 0 : Data write disable
        //bit0     SPIE       = 1 : SPI transfer start
    rpc_wait_xfer_end();
}

void rpc_qspi_write_anyreg(uint32_t addr, uint8_t * writeData, int len)
{
    //bit31  CAL         =  1 : PHY calibration
    //bit1-0 PHYMEM[1:0] = 00 : QSPI-SDR
	out32(RCAR_RPC_BASE + RCAR_RPC_CMNCR, 0x81FFF300);
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	out32(RCAR_RPC_BASE + RCAR_RPC_SMCMR, 0x00010000);
		//bit23-16 CMD[7:0] = 0x01 : Write (Status & Configuration) Register
//	*((volatile uint32_t*)RPC_SMADR)      = addr;
//	*((volatile uint32_t*)RPC_SMDMCR)     = 0x00000006;
	out32(RCAR_RPC_BASE + RCAR_RPC_SMDRENR, 0x00000000);
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	out32(RCAR_RPC_BASE + RCAR_RPC_SMENR, 0x00004008);
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0000 : ADR[23:0] is disable
		//bit3-0   SPIDE[3:0] = 1000 : 8bit transfer
    //out32(RCAR_RPC_BASE + RCAR_RPC_SMCMR, SMCMR_CMD(QSPI_CMD_WRAR));
    //out32(RCAR_RPC_BASE + RCAR_RPC_SMADR, addr);
    //out32(RCAR_RPC_BASE + RCAR_RPC_SMENR, 0x00004708);
        //bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
        //bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
        //bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
        //bit15    DME        =    0 : No dummy cycle
        //bit14    CDE        =    1 : Command enable
        //bit11-8  ADE[3:0]   = 0111 : ADR[24:0] is output
        //bit3-0   SPIDE[3:0] = 1000 : 8bit transfer

    for (int i = 0; len; len--) {
        out8(RCAR_RPC_BASE + RCAR_RPC_SMWDR0, writeData[i]);
        if (len == 1)
            out32(RCAR_RPC_BASE + RCAR_RPC_SMCR, 0x00000003);
        else
            out32(RCAR_RPC_BASE + RCAR_RPC_SMCR, 0x00000103);
            //bit2     SPIRE      = 0 : Data read disable
            //bit1     SPIWE      = 1 : Data write disable
            //bit0     SPIE       = 1 : SPI transfer start

        rpc_wait_xfer_end();
    }
}

uint8_t rpc_qspi_read_reg(uint8_t cmd)
{
    uint8_t readData = 0;

    out32(RCAR_RPC_BASE + RCAR_RPC_SMCMR, SMCMR_CMD(cmd));
        //bit23-16 CMD[7:0] = 0x35 : Read Configuration Register (CFG)
    out32(RCAR_RPC_BASE + RCAR_RPC_SMENR , 0x00004008);
        //bit15    DME        =    0 : No dummy cycle
        //bit14    CDE        =    1 : Command enable
        //bit11-8  ADE[3:0]   = 0000 : Address output disable
        //bit3-0   SPIDE[3:0] = 1000 : 8bit transfer
    out32(RCAR_RPC_BASE + RCAR_RPC_SMCR , 0x00000005);
        //bit2     SPIRE      = 1 : Data read enable
        //bit1     SPIWE      = 0 : Data write disable
        //bit0     SPIE       = 1 : SPI transfer start

    rpc_wait_xfer_end();

    readData = in8(RCAR_RPC_BASE + RCAR_RPC_SMRDR0);
    return readData;
}

void rpc_qspi_enable_quad_mode(void)
{
    uint8_t configReg[2] = {0,0};
    uint8_t statusReg = 0;

    configReg[0] = rpc_qspi_read_reg(QSPI_CMD_RDCR);  // CFG reg, CR1V

    if (!(configReg[0] & (1 << 1))) {
        rpc_qspi_write_cmd(QSPI_CMD_WREN);          //WRITE ENABLE
        configReg[1] = rpc_qspi_read_reg(QSPI_CMD_RDSR);
        configReg[0] |= (1<<1);
        rpc_qspi_write_anyreg(CR1V, configReg, 2);   //CR1V[1]=1 :0=Dual or Serial, 1=Quad
        while (1) {
            statusReg = rpc_qspi_read_reg(QSPI_CMD_RDSR);
            if ((statusReg & FLASH_WIP) == 0) {
                break;
            }
        }
    }
}

void rpc_qspi_set_clk_v3m()
{
    uint32_t divreg_ratio;

    /*
     * RPCCK on v3m is 320Mhz.
     * DIVREG RATIO[1:0] 00: RPCCK/1 = 320Mhz, 01: RPCCK/2 = 160Mhz, 10: RPCCK/4 = 80Mhz
     * As per the V3M SK HW manual: QSPI flash clock is 80Mhz
     */
    divreg_ratio = 0b10;

    if (in32(RCAR_RPC_BASE + RCAR_RPC_DIV_REG) != divreg_ratio){
        out32(RCAR_RPC_BASE + RCAR_RPC_DIV_REG , divreg_ratio);
        __asm__ __volatile__ ("dsb sy");
    }
}

void rpc_qspi_quad_ext_mode(void)
{
    uint32_t tmp;

    // Enable external mode
    tmp = in32(RCAR_RPC_BASE + RCAR_RPC_CMNCR);
    tmp &= ~RCAR_RPC_CMNCR_MD;
    out32(RCAR_RPC_BASE + RCAR_RPC_CMNCR, tmp);

    out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, 0x001F0100);
        //bit20-16 RBURST[4:0] = 11111 : 32 continuous data unit
        //bit8     RBE         =     1 : Burst read
    out32(RCAR_RPC_BASE + RCAR_RPC_DRCMR, 0x00EC0000);
        //bit23-16 CMD[7:0] = 0x0C : 4FAST_READ 0Ch Command 4-byte address command

    out32(RCAR_RPC_BASE + RCAR_RPC_DREAR, 0x00000001);
        //bit23-16 EAV[7:0]   = 0    : ADR[32:26] output set0
        //bit2-0   EAC[2:0]   = 001  : ADR[25:0 ] Enable

    out32(RCAR_RPC_BASE + RCAR_RPC_DRENR, 0x0202CF00);
        //bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
        //bit25-24 ADB[1:0]   =   10 : 4bit width address (QSPI0_MOSI)
        //bit17-16 DRDB[1:0]  =   10 : 4bit width transfer data (QSPI0_IO0)
        //bit15    DME        =    1 : dummy cycle enable
        //bit14    CDE        =    1 : Command enable
        //bit11-8  ADE[3:0]   = 1111 : ADR[23:0] output (24 Bit Address)
    out32(RCAR_RPC_BASE + RCAR_RPC_DRDMCR, 0x00000009);
        //bit2-0 DMCYC[2:0]   = 111 : 8 cycle dummy wait
    out32(RCAR_RPC_BASE + RCAR_RPC_DRDRENR, 0x00000000);
        //bit8 ADDRE  = 0 : Address SDR transfer
        //bit0 DRDRE  = 0 : DATA SDR transfer
}

void rpc_hyper_init_ext_mode(void)
{
    /* Hyperflash code adapted from http://community.qnx.com/sf/frs/do/viewRelease/projects.renesas_drivers/frs.renesas_r_car_h2_archives_and_do.renesas_r_car_m3_reference_bsp_0 */
    /* 160Mhz initial hyperflash changes adapted from http://community.qnx.com/sf/frs/do/viewRelease/projects.renesas_drivers/frs.dnjp_600b.renesas_r_car_h3_ilp_with_hyperf */

    /* Read mode bits to determine if we should use the 160 Mhz settings or not */
    // TODO Figure out with renesas and TRM how to better use these magic numbers and simplfy this logic
    if ((in32(RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR) & RCAR_GEN3_MODEMR_BOOT_MASK) ==RCAR_GEN3_MODEMR_BOOT_HYPERFLASH160) {
        out32(RCAR_RPC_BASE + RCAR_RPC_PHYCNT, 0x80070263);
        out32(RCAR_RPC_BASE + RCAR_RPC_CMNCR, 0x01FFF301);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, 0x001F0100);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRCMR, 0x00A00000);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRENR, 0xA222D400);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRDMCR, 0x0000000E);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRDRENR, 0x00005101);
        out32(RCAR_RPC_BASE + RCAR_RPC_OFFSET1, 0x21511144);
        out32(RCAR_RPC_BASE + RCAR_RPC_PHYINT, 0x07070002);
    } else {
        out32(RCAR_RPC_BASE + RCAR_RPC_PHYCNT, 0x80040263);
        out32(RCAR_RPC_BASE + RCAR_RPC_CMNCR, 0x01FFF301);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, 0x00000000);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRCMR, 0x00A00000);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRENR, 0xA222D400);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRDMCR, 0x0000000E);
        out32(RCAR_RPC_BASE + RCAR_RPC_DRDRENR, 0x00005101);
        out32(RCAR_RPC_BASE + RCAR_RPC_OFFSET1, 0x21511144);
        out32(RCAR_RPC_BASE + RCAR_RPC_PHYINT, 0x07070002);
    }
}

void rpc_flash_init(rpc_flash_type type)
{
    uint32_t val;
    uint32_t clk = 0;

    val = RCAR_PRODUCT_ID(in32(RCAR_PRODUCT_REGISTER));
    switch (val) {
        case PRODUCT_ID_RCAR_V3M:
            /*
             * The CPG RPCCKCR register is valid for all the Rcar gen3 variants except for the V3M.
             * The V3M uses the DIVREG in the RPC to control the clock.
             * For the other rcar gen3 variants, to control the clock, the RPCCKCR can be set in
             * the rcar_soc_board.h
             */
            if (type == RPC_FLASH_TYPE_QSPI) {
                rpc_qspi_set_clk_v3m();
            }
            break;
        case PRODUCT_ID_RCAR_D3:
            /* TODO */
            break;
        default:
            if (IS_RCAR_GEN4(val)) {
                val = in32(RCAR_GEN4_RST_DM0_BASE + RCAR_GEN4_RST_MODEMR0) & RCAR_GEN3_MODEMR_BOOT_MASK;
            } else {
                val = in32(RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR) & RCAR_GEN3_MODEMR_BOOT_MASK;
            }

            switch(val) {
                case RCAR_GEN3_MODEMR_BOOT_HYPERFLASH160:
                    clk = 0x00000011;
                    break;
                case RCAR_GEN3_MODEMR_BOOT_HYPERFLASH80:
                case RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH80:
                    clk = 0x00000013;
                    break;
                case RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH40:
                    clk = 0x00000017;
                    break;
                default:
                    ser_putstr("WARNING: Unsupported boot mode, RPCCKCR not initialized.\n");
                    break;
            }
            break;
    }
    if (clk) {
        if (IS_RCAR_GEN4(val)) {
            out32(RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_CPGWPR, ~clk);
            out32(RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_RPCCKCR, clk);
        } else {
            out32(RCAR_GEN3_CPG_BASE + RCAR_GEN3_CPG_CPGWPR, ~clk);
            out32(RCAR_GEN3_CPG_BASE + RCAR_GEN3_CPG_RPCCKCR, clk);
        }
    }
    if (type == RPC_FLASH_TYPE_HYPER) {
        rpc_hyper_init_ext_mode();
    } else if (type == RPC_FLASH_TYPE_QSPI) {
        rpc_qspi_init();
        //rpc_qspi_enable_quad_mode();
        //rpc_qspi_quad_ext_mode();
    }
    flash_type = type;
}

static void rpc_wait_flag(int mask, int value)
{
    while (1) {
        if ((in32(RCAR_RPC_BASE + RCAR_RPC_CMNSR) & mask) == value) {
            break;
        }
    }
}

#define RCAR_RPC_SINGLE_NOXFER        0x0
#define RCAR_RPC_SINGLE_8BITS         0x8
#define RCAR_RPC_SINGLE_16BITS        0xC
#define RCAR_RPC_SINGLE_32BITS        0xF


static int rpc_bytes_set(int len)
{
    switch (len) {
        case 0:
            return RCAR_RPC_SINGLE_NOXFER;
        case 1:
            return RCAR_RPC_SINGLE_8BITS;
        case 2:
            return RCAR_RPC_SINGLE_16BITS;
        case 3:
        default:
            return RCAR_RPC_SINGLE_32BITS;
    }
}

int rpc_qspi_quad_ext_read(uint64_t dst, uint32_t flashAddr, int dlen)
{
    int ret = 0;
    uint32_t drenr;
    int      len;
    int      read_burst = 0;
    int alen = 4;
    int dummy = 10;
    int csize = 1;
    int asize = 4;
    int dsize = 4;
    uint32_t  cmd = 0xEC;

    /* This calculation is to make len divisible by 8 */
    len = (dlen + 7) & ~7;

    if (RCAR_IPL_QSPI_IMAGE_ADDR + len > RCAR_RPC_MAP_SIZE) {
        ser_putstr("Image size is large\n");
        return -1;
    }

    if ((len % 256) == 0) {
        read_burst = 1;
    }

    rpc_qspi_enable_quad_mode();
    rpc_qspi_quad_ext_mode();

    /* 
     * Set External read Mode using DMA
     */
    out32(RCAR_RPC_BASE + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_STRTIM(7) | 0x260 );
    out32(RCAR_RPC_BASE + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_STRTIM(7) | 0x260 | RCAR_RPC_PHYCNT_CAL);      

    out32(RCAR_RPC_BASE + RCAR_RPC_CMNCR, RCAR_RPC_CMNCR_SFDE      // Undocumented bit, but they must be set (found out without this setting, must swap read/write data)
                        | RCAR_RPC_CMNCR_MOIIO_HIZ 
                        | RCAR_RPC_CMNCR_IOFV_HIZ 
                        | RCAR_RPC_CMNCR_BSZ(0));               // 1 Serial Flash chip

    out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, read_burst ?
                          (  RCAR_RPC_DRCR_RBE                // Read Burst 
                           | RCAR_RPC_DRCR_RBURST(32)         // 32 continuous data units
                           | RCAR_RPC_DRCR_SSLE(1) )
                        : 0);

    out32(RCAR_RPC_BASE + RCAR_RPC_DREAR, 1);			  // External address bits[25:0]

    /* Set Data Read Enable */
    drenr = RCAR_RPC_DRENR_CDB(csize >> 1)   // Command Bit Size : 1 or 4 bits
         |  RCAR_RPC_DRENR_ADB(asize >> 1)   // Address Bit Size : 1 or 4 bits
         |  RCAR_RPC_DRENR_DRDB(dsize >> 1)  // Transfer Data Bit Size : 1 or 4 bits
         |  RCAR_RPC_DRENR_CDE;              // Command enable : Enable

    if (alen == 3) {
        drenr |= RCAR_RPC_DRENR_ADE(0x7);  // Address Enable : ADR[23:0] is output
    } else if (alen == 4) {
        drenr |= RCAR_RPC_DRENR_ADE(0xF);  // Address Enable : ADR[31:0] is output 
    }

    /* Set Command */
    out32(RCAR_RPC_BASE + RCAR_RPC_DRCMR, cmd << 16);

    /* Set Dummy */
    if (dummy) {
        drenr |= RCAR_RPC_DRENR_DME(1); // Dummy enable : Enable
        out32(RCAR_RPC_BASE + RCAR_RPC_DRDMCR, dummy - 1);
    }

    out32(RCAR_RPC_BASE + RCAR_RPC_DRENR, drenr);

    out32(RCAR_RPC_BASE + RCAR_RPC_DRDRENR, 0);

    /* FIXME : If not using the cache, we need to flush the cache, is it right? */
    if (!read_burst) {
        out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, in32(RCAR_RPC_BASE + RCAR_RPC_DRCR) | RCAR_RPC_DRCR_RCF);
        in32(RCAR_RPC_BASE + RCAR_RPC_DRCR);
    }

#ifdef RCAR_IPL_SYSDMAC_BASE
    ret =  dma_mem_copy(dst, flashAddr, dlen);
#else
    copy(dst, flashAddr, dlen);
#endif

    if (read_burst) {
        /* FIXME : We don't know whether next access is continue or not, so do we need to deassert QSPIn_SSL line here? */
        out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, in32(RCAR_RPC_BASE + RCAR_RPC_DRCR) | RCAR_RPC_DRCR_SSLN(1));

        rpc_wait_flag(RCAR_RPC_CMNSR_SSLF, 0);
    }

    return ret;
}

static inline void rpc_qspi_read_data(uint8_t *data, int len)
{
    uint32_t read_data = in32(RCAR_RPC_BASE + RCAR_RPC_SMRDR0);

    memcpy(data, &read_data, len);
}

int rpc_qspi_read(uint64_t dst, uint32_t flashAddr, int dlen)
{
    int alen = 4;
    int dummy = 10;
    int csize = 1;
    int asize = 4;
    int dsize = 4;
    uint32_t cmd = 0xEC;
    uint32_t addr = flashAddr;//RCAR_IPL_QSPI_IMAGE_ADDR;

    int        nbytes, pos = 0;
    uint32_t   smenr, smcr;
    uint8_t    *rdata = (uint8_t *)dst;

    rpc_qspi_enable_quad_mode();
    
    /* 
     * Set Manual Mode 
     */

    out32(RCAR_RPC_BASE + RCAR_RPC_PHYCNT, RCAR_RPC_PHYCNT_CAL // PHY calibration
                            | RCAR_RPC_PHYCNT_PHYMEM(0)     // Serial flash in SDR mode
                            | 0x260);                       // The 0x260 are undocumented bits, but they must be set

    out32(RCAR_RPC_BASE + RCAR_RPC_CMNCR, RCAR_RPC_CMNCR_MD    // Manual mode
                            | RCAR_RPC_CMNCR_SFDE           // Undocumented bit, but they must be set (found out without this setting, must swap read/write data)
                            | RCAR_RPC_CMNCR_MOIIO_HIZ 
                            | RCAR_RPC_CMNCR_IOFV_HIZ 
                            | RCAR_RPC_CMNCR_BSZ(0));       // 1 Serial Flash chip

    out32(RCAR_RPC_BASE + RCAR_RPC_SMDRENR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_DRDMCR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_SSLDR, 4 << 8); // Use default value
    out32(RCAR_RPC_BASE + RCAR_RPC_DRCR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_DREAR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_DRENR, 0);
    out32(RCAR_RPC_BASE + RCAR_RPC_SMCR, 0);

    smenr = RCAR_RPC_SMENR_CDB(csize >> 1)    // Command Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_ADB(asize >> 1)    // Address Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_SPIDB(dsize >> 1)  // Transfer Data Bit Size : 1/4 bits
          | RCAR_RPC_SMENR_CDE                         // Command enable : Enable
          | RCAR_RPC_SMENR_SPIDE(rpc_bytes_set(dlen)); // Transfer Data Enable : 8/16/32 bits

    if (alen == 3) {
        smenr |= RCAR_RPC_SMENR_ADE(0x7);  // Address Enable : ADR[23:0] is output
    } else if (alen == 4) {
        smenr |= RCAR_RPC_SMENR_ADE(0xF);  // Address Enable : ADR[31:0] is output 
    }

    /* Set Dummy */
    if (dummy) {
        smenr |= RCAR_RPC_SMENR_DME;
        out32(RCAR_RPC_BASE + RCAR_RPC_SMDMCR, RCAR_RPC_DRDMCR_DMCYC(dummy - 1));
    }

    /* Set command */
    out32(RCAR_RPC_BASE + RCAR_RPC_SMCMR, cmd << 16);

    /* Set Address */
    out32(RCAR_RPC_BASE + RCAR_RPC_SMADR, addr);

    smcr = RCAR_RPC_CMNCR_SPIRE | RCAR_RPC_CMNCR_SPIE;

    if ((alen == 0) && (dlen > 4)) {
         smcr |= RCAR_RPC_CMNCR_SSLKP;
    }

    while (pos < dlen) {
        nbytes = dlen - pos;
        if (nbytes > 4) {
            nbytes = 4;
        } else {
            smcr &= ~RCAR_RPC_CMNCR_SSLKP;
        }

        /* Start transfer */
        out32(RCAR_RPC_BASE + RCAR_RPC_SMENR, smenr);
        out32(RCAR_RPC_BASE + RCAR_RPC_SMCR,  smcr);

        /* Waiting for transfer end */
        rpc_wait_xfer_end();

        /* Read data */
        rpc_qspi_read_data(rdata, nbytes);

        if (!pos && alen == 0 && dlen > 4)
            smenr = RCAR_RPC_SMENR_SPIDE(0xF); // Disable CDE/ADE. For register transfer with multiple data, address increment is not needed

        if (alen) {
            /* Set new address */
            addr += 4;
            out32(RCAR_RPC_BASE + RCAR_RPC_SMADR, addr);
        }

        pos += nbytes;
        rdata += nbytes;
    }

    return 0;
}


int flash_copy(uint64_t dst, uint32_t flashAddr, int dlen)
{
    int ret = 0;

    rpc_wait_xfer_end();

    if (flash_type == RPC_FLASH_TYPE_QSPI) {
        if (RCAR_IPL_QSPIFLASH_IMAGE_ADDR == flashAddr) {
            ret = rpc_qspi_quad_ext_read(dst, flashAddr, dlen);
        } else {
            ret = rpc_qspi_read(dst, flashAddr, dlen);
        }
    } else {
#ifdef RCAR_IPL_SYSDMAC_BASE
        ret =  dma_mem_copy(dst, flashAddr, dlen);
#else
        copy(dst, flashAddr, dlen);
#endif

    }

    return ret;
}

#endif  // #if defined(RCAR_IPL_HYPERFLASH_SUPPORT) || defined(RCAR_IPL_QSPIFLASH_SUPPORT)

