/*
 * $QNXLicenseC:
 * Copyright 2017 QNX Software Systems.
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

#include <sys/platform.h>
#include <sys/startup.h>
#include <stdint.h>

#define RCAR_RPC_MAP_SIZE       0x4000000

typedef enum rpc_flash_type_ {
    RPC_FLASH_TYPE_UNKOWN = 0,
    RPC_FLASH_TYPE_HYPER,
    RPC_FLASH_TYPE_QSPI,
} rpc_flash_type;

extern int dma_mem_copy(uint64_t dst, uint32_t src, uint32_t len);
extern int flash_copy(uint64_t dst, uint32_t addr_flash, int dlen);
extern void rpc_flash_init(rpc_flash_type type);

/* RPC registers, offset from RCAR_RPC_BASE */
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
#define RCAR_RPC_OFFSET1        0x80    /*  */
#define RCAR_RPC_PHYINT         0x88    /* PHY interrupt register */
#define RCAR_RPC_DIV_REG        0xA8    /* Divider clock register */

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
#define RCAR_RPC_CMNCR_SFDE          (1 << 24) // undocumented bit but must be set                
#define RCAR_RPC_CMNCR_MD            (1 << 31)


/* SSLDR bit definition  */
#define RCAR_RPC_SSLDR_SCKDL(x)     (x << 0)
#define RCAR_RPC_SSLDR_SLNDL(x)     (x << 8)
#define RCAR_RPC_SSLDR_SPNDL(x)     (x << 16)

/* DRCR bit definition*/
#define RCAR_RPC_DRCR_SSLE(x)           (x << 0)
#define RCAR_RPC_DRCR_RBE               (1 << 8)
#define RCAR_RPC_DRCR_RCF               (1 << 9)
#define RCAR_RPC_DRCR_RPURST(x)         (x << 16)
#define RCAR_RPC_DRCR_SSLN(x)           (x << 24)
#define RCAR_RPC_DRCR_RBURST(x)         ((((x) - 1) & 0x1F) << 16)

/* DRENR bit definition */
#define RCAR_RPC_DRENR_OPDE(x)          (x << 4)
#define RCAR_RPC_DRENR_ADE(x)           (x << 8)
#define RCAR_RPC_DRENR_OCDE1            (1 << 12)
#define RCAR_RPC_DRENR_CDE              (1 << 14)
#define RCAR_RPC_DRENR_DME(x)           (x << 15)
#define RCAR_RPC_DRENR_DRDB(x)          (x << 16)
#define RCAR_RPC_DRENR_OPDP(x)          (x << 20)
#define RCAR_RPC_DRENR_ADB(x)           (x << 24)
#define RCAR_RPC_DRENR_OCDB(x)          (x << 28)
#define RCAR_RPC_DRENR_CDB(x)           (x << 30)

/* SMCR bit definition */
#define RCAR_RPC_CMNCR_SPIE             (1 << 0)
#define RCAR_RPC_CMNCR_SPIWE            (1 << 1)
#define RCAR_RPC_CMNCR_SPIRE            (1 << 2)
#define RCAR_RPC_CMNCR_SSLKP            (1 << 8)

/*SMENR bit definition */
#define RCAR_RPC_SMDRENR_SPIDE(x)       (x << 0)
#define RCAR_RPC_SMDRENR_OPDE(x)        (x << 4)
#define RCAR_RPC_SMDRENR_ADE(x)         (x << 8)
#define RCAR_RPC_SMDRENR_OCDE           (1 << 12)
#define RCAR_RPC_SMDRENR_CDE            (1 << 14)
#define RCAR_RPC_SMDRENR_DME            (1 << 15)
#define RCAR_RPC_SMDRENR_SPIDB(x)       (x << 16)
#define RCAR_RPC_SMDRENR_OPDB(x)        (x << 20)
#define RCAR_RPC_SMDRENR_ADB(x)         (x << 24)
#define RCAR_RPC_SMDRENR_OCDB(x)        (x << 28)
#define RCAR_RPC_SMDRENR_CDB(x)         (x << 30)


/* SMENR bit definition */
#define RCAR_RPC_SMENR_SPIDE(x)       (((x) & 0xF) << 0)
#define RCAR_RPC_SMENR_OPDE(x)        (((x) & 0xF) << 4)
#define RCAR_RPC_SMENR_ADE(x)         (((x) & 0xF) << 8)
#define RCAR_RPC_SMENR_OCDE           (1 << 12)
#define RCAR_RPC_SMENR_CDE            (1 << 14)
#define RCAR_RPC_SMENR_DME            (1 << 15)
#define RCAR_RPC_SMENR_SPIDB(x)       (((x) & 0x3) << 16)
 #define RCAR_RPC_SMENR_SPIDB_1BIT     RCAR_RPC_SMENR_SPIDB(0)
 #define RCAR_RPC_SMENR_SPIDB_4BIT     RCAR_RPC_SMENR_SPIDB(2)
#define RCAR_RPC_SMENR_OPDB(x)        (((x) & 0x3) << 20)
 #define RCAR_RPC_SMENR_OPDB_1BIT     RCAR_RPC_SMENR_OPDB(0)
 #define RCAR_RPC_SMENR_OPDB_4BIT     RCAR_RPC_SMENR_OPDB(2)
#define RCAR_RPC_SMENR_ADB(x)         (((x) & 0x3) << 24)
 #define RCAR_RPC_SMENR_ADB_1BIT      RCAR_RPC_SMENR_ADB(0)
 #define RCAR_RPC_SMENR_ADB_4BIT      RCAR_RPC_SMENR_ADB(2)
#define RCAR_RPC_SMENR_OCDB(x)        (((x) & 0x3) << 28)
 #define RCAR_RPC_SMENR_OCDB_1BIT     RCAR_RPC_SMENR_OCDB(0)
 #define RCAR_RPC_SMENR_OCDB_4BIT     RCAR_RPC_SMENR_OCDB(2)
#define RCAR_RPC_SMENR_CDB(x)         (((x) & 0x3) << 30)
 #define RCAR_RPC_SMENR_CDB_1BIT      RCAR_RPC_SMENR_CDB(0)
 #define RCAR_RPC_SMENR_CDB_4BIT      RCAR_RPC_SMENR_CDB(2)

/* CMNSR bit definition*/
#define RCAR_RPC_CMNSR_TEND             (1 << 0)
#define RCAR_RPC_CMNSR_SSLF             (1 << 1)

/* DRDMCR  bit definition */
#define RCAR_RPC_DRDMCR_DMCYC(x)        ((x) << 0)
#define RCAR_RPC_DRDMCR_DMDB(x)         ((x) << 16)

/*DRDRENR bit definition */
#define RCAR_RPC_DRDRENR_DRDRE(x)       (x << 0)
#define RCAR_RPC_DRDRENR_OPDRE(x)       (x << 4)
#define RCAR_RPC_DRDRENR_ADDRE(x)       (x << 8)
#define RCAR_RPC_DRDRENR_HYPE(x)        (x << 12)

/* SMDMCR bit definition */
#define RCAR_RPC_SMDMCR_DCMYC(x)        (x << 0)
#define RCAR_RPC_SMDMCR_DMDB(x)         (x << 16)

/* SMDRENR bit definition*/
#define RCAR_RPC_SMDRENR_SPIDRE(x)      (x << 0)
#define RCAR_RPC_SMDRENR_OPDRE(x)       (x << 4)
#define RCAR_RPC_SMDRENR_ADDRE(x)       (x << 8)
#define RCAR_RPC_SMDRENR_HYPE(x)        (x << 12)

/*PHYCNT bit definition */
#define RCAR_RPC_PHYCNT_PHYMEM(x)     (((x) & 3) << 0)
#define RCAR_RPC_PHYCNT_WBUF          (1 << 2)
#define RCAR_RPC_PHYCNT_WBUF2         (1 << 4)
#define RCAR_RPC_PHYCNT_STRTIM(x)     (((x) & 0x7) << 15)
#define RCAR_RPC_PHYCNT_HS            (1 << 18)
//#define RCAR_RPC_PHYCNT_OCTA         (1 << 20)
#define RCAR_RPC_PHYCNT_EXDS          (1 << 21)
#define RCAR_RPC_PHYCNT_OCTA(x)       (((x) & 0x3) << 22)
#define RCAR_RPC_PHYCNT_CAL           (1 << 31)

/* PHYINT BIT Definitions */
#define RCAR_RPC_PHYINT_RSTEN         (1<<18)         /* RPC_RESET# Pin Enable */
#define RCAR_RPC_PHYINT_WPEN          (1<<17)         /* RPC_WP# Pin Enable */
#define RCAR_RPC_PHYINT_INTEN         (1<<16)         /* RPC_INT# Pin Enable */
#define RCAR_RPC_PHYINT_RSTVAL        (1<<2)         /* RPC_RESET# Pin Output Value */
#define RCAR_RPC_PHYINT_WPVAL         (1<<1)         /* RPC_WP# Pin Output Value */
#define RCAR_RPC_PHYINT_INT           (1<<0)         /* Interrupt Status */

/* QSPI S25FL */
#define QSPI_CMD_WRR     0x01    // Write Status Register
#define QSPI_CMD_PP      0x02    // Page programming
#define QSPI_CMD_READ    0x03    // Read memory
#define QSPI_CMD_WRDI    0x04    // Write disable
#define QSPI_CMD_RDSR    0x05    // Read Status Register
#define QSPI_CMD_WREN    0x06    // Write enable
#define QSPI_CMD_RDSR2   0x07    // Read Status Register 2
#define QSPI_CMD_FREAD   0x0B    // Fast read memory
#define QSPI_CMD_4FREAD  0x0C    // Fast read memory, 4 bytes address
#define QSPI_CMD_4PP     0x12    // page programming, 4 bytes address
#define QSPI_CMD_4READ   0x13    // Read memory, 4 byte address
#define QSPI_CMD_P4E     0x20    // 4K sector erase
#define QSPI_CMD_4P4E    0x21    // 4K sector erase, 4 bytes address
//#define QSPI_CMD_QPP     0x32    // Quad Page Program, 3 bytes address
//#define QSPI_CMD_4QPP    0x34    // Quad Page Program
#define QSPI_CMD_RDCR    0x35    // Read Configuration Register 1
#define QSPI_CMD_EWSR    0x50    // Enable write status register
#define QSPI_CMD_RDAR    0x65    // Read anny register
#define QSPI_CMD_QOR     0xEB    // Quad Read Out, 3 bytes address
#define QSPI_CMD_4QOR    0xEC    // Quad Read Out, 4 bytes address
#define QSPI_CMD_WRAR    0x71    // Write anny register
#define QSPI_CMD_ERSP    0x75    // Erase suspend
#define QSPI_CMD_ERRS    0x7A    // Erase resume
#define QSPI_CMD_RDID    0x9F    // JEDEC Read-ID
#define QSPI_CMD_AAIP    0xAD    // Auto address increment programming
#define QSPI_CMD_SE      0xD8    // 64K sector erase
#define QSPI_CMD_4SE     0xDC    // 64K sector erase, 4 bytes address
#define QSPI_CMD_DYBRD   0xE0    // Read DRB register
#define QSPI_CMD_DYBWR   0xE1    // Write DRB register
#define QSPI_CMD_PPBRD   0xE2    // Read PPB register
#define QSPI_CMD_PPBP    0xE3    // PPB program
#define QSPI_CMD_PPBE    0xE4    // PPB erase
#define QSPI_CMD_RSTEN   0x66
#define QSPI_CMD_RST     0x99
#define QSPI_CMD_RESET   0xF0    // Legacy software reset

#define SMCMR_CMD(_cmd)  ((_cmd) << 16)

#define FLASH_WIP       (1 << 0)    // Write in Progress (WIP) bit
#define FLASH_ES        (1 << 1)    // erase suspended

#define CR1NV           0x00000002
#define CR2NV           0x00000003
#define CR3NV           0x00000004

#define CR1V            0x00800002
#define CR2V            0x00800003
#define CR3V            0x00800004

