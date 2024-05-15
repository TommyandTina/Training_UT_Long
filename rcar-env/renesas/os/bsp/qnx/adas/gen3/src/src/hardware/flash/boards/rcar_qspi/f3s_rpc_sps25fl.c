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

#include "f3s_spi.h"

const struct qspi_flash_params qspi_flash_params_table[] = {
 /* Cypress */
    {"s25fl008a",      INFO(0x010213, 0x0,       64  * 1024, 16,   0) },
    {"s25fl016a",      INFO(0x010214, 0x0,       64  * 1024, 32,   0) },
    {"s25fl032a",      INFO(0x010215, 0x0,       64  * 1024, 64,   0) },
    {"s25fl064a",      INFO(0x010216, 0x0,       64  * 1024, 128,  0) },
    {"s25fl116k",      INFO(0x014015, 0x0,       64  * 1024, 128,  0) },
    {"s25fl164k",      INFO(0x014017, 0x0140,    64  * 1024, 128,  0) },
    {"s25fl128p_256k", INFO(0x012018, 0x0300,    256 * 1024, 64,   RD_FULL | WR_QPP) },
    {"s25fl128p_64k",  INFO(0x012018, 0x0301,    64  * 1024, 256,  RD_FULL | WR_QPP) },
    {"s25fl032p",      INFO(0x010215, 0x4d00,    64  * 1024, 64,   RD_FULL | WR_QPP) },
    {"s25fl064p",      INFO(0x010216, 0x4d00,    64  * 1024, 128,  RD_FULL | WR_QPP) },
    {"s25fl128s_256k", INFO(0x012018, 0x4d00,    256 * 1024, 64,   RD_FULL | WR_QPP) },
    {"s25fl128s_64k",  INFO(0x012018, 0x4d01,    64  * 1024, 256,  RD_FULL | WR_QPP) },
    {"s25fl256s_256k", INFO(0x010219, 0x4d00,    256 * 1024, 128,  RD_FULL | WR_QPP) },
    {"s25fl256s_64k",  INFO(0x010219, 0x4d01,    64  * 1024, 512,  RD_FULL | WR_QPP) },
    {"s25fs064s",      INFO6(0x010217, 0x4d0081, 64 * 1024, 128,  RD_FULL | WR_QPP | SECT_4K) },
    {"s25fs256s_64k",  INFO6(0x010219, 0x4d0181, 64  * 1024, 512,  RD_FULL | WR_QPP | SECT_4K) },
    {"s25fs512s",      INFO6(0x010220, 0x4d0081, 256 * 1024, 256,  RD_FULL | WR_QPP | SECT_4K) },
    {"s25fl512s_256k", INFO(0x010220, 0x4d00,    256 * 1024, 256,  RD_FULL | WR_QPP) },
    {"s25fl512s_64k",  INFO(0x010220, 0x4d01,    64  * 1024, 1024, RD_FULL | WR_QPP) },
    {"s25fl512s_512k", INFO(0x010220, 0x4f00,    256 * 1024, 256,  RD_FULL | WR_QPP) },

    {}, /* Empty entry to terminate the list */
    /*
     * Note:
     * Below paired flash devices has similar spi_flash params.
     * (s25fl129p_64k, s25fl128s_64k)
     * (w25q80bl, w25q80bv)
     * (w25q16cl, w25q16dv)
     * (w25q32bv, w25q32fv_spi)
     * (w25q64cv, w25q64fv_spi)
     * (w25q128bv, w25q128fv_spi)
     * (w25q32dw, w25q32fv_qpi)
     * (w25q64dw, w25q64fv_qpi)
     * (w25q128fw, w25q128fv_qpi)
     */
};

static int sps25fl_read_registers(void *hdl, uint8_t cmd, uint8_t *data, int dlen)
{
    return qspi_nor_read_registers(hdl, cmd, 1, 0, 0, 0, data, dlen, 1);
}

/* Read a register with RDAR command */
static int sps25fl_read_any_registers(void *hdl, uint8_t cmd, uint32_t reg, int alen, uint8_t *data, int dlen)
{
    return qspi_nor_read_registers(hdl, cmd, 1, reg, alen, 1, data, dlen, 1);
}

static int sps25fl_read_memory(void *hdl, uint32_t addr, uint8_t *data, int dlen)
{
    spi_flash_t *spi = hdl;

    if (spi->mode & SPI_MODE_QUAD)        
        return qspi_nor_read_memory(hdl,
                    spi->dcap & SPI_CAP_ADDR4 ? QSPI_CMD_4QOR : QSPI_CMD_QOR, 1,
                    addr, spi->dcap & SPI_CAP_ADDR4 ? 4 : 3, 4,
                    data, dlen, 4, 10);
    else
        return qspi_nor_read_memory(hdl,
                    spi->dcap & SPI_CAP_ADDR4 ? QSPI_CMD_4FREAD : QSPI_CMD_FREAD, 1,
                    addr, spi->dcap & SPI_CAP_ADDR4 ? 4 : 3, 1,
                    data, dlen, 1, 8);
}

static int sps25fl_write_registers(void *hdl, uint8_t cmd, uint8_t *data, int dlen)
{
    return qspi_nor_write(hdl, cmd, 1, 0, 0, 0, data, dlen, 1);
}

static int sps25fl_write_memory(void *hdl, uint8_t cmd, uint32_t addr, int alen, uint8_t *data, int dlen)
{
    if (sps25fl_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (-1);
    }

    return qspi_nor_write(hdl, cmd, 1, addr, alen, 1, data, dlen, 1);
}

static int sps25fl_busy_wait(void *hdl, uint32_t tmo)
{
    uint8_t sts = 0;

    while (tmo--) {
        if (sps25fl_read_registers(hdl, QSPI_CMD_RDSR, &sts, 1) < 0) {
            return (EIO);
        }

        if (!(sts & FLASH_WIP)) {
            return (EOK);
        }

        nanospin_ns(1000);
    }

    return (EAGAIN);
}

static int sps25fl_program_page(void *hdl, uint32_t addr, uint8_t *data, int dlen)
{
    spi_flash_t *spi = hdl;
    int         alen;
    //int         asize;
    uint8_t     cmd;
    int         dsize;
    int         rc;

    if (sps25fl_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (-1);
    }

    alen = spi->dcap & SPI_CAP_ADDR4 ? 4 : 3;
    dsize = 1;
    //dsize = spi->mode & SPI_MODE_QUAD ? 4 : 1;
    //asize = spi->mode & SPI_MODE_QUAD ? 4 : 1;
    cmd = alen == 4 ? QSPI_CMD_4PP : QSPI_CMD_PP;
    //if (spi->mode & SPI_MODE_QUAD)
    //    cmd = alen == 4 ? QSPI_CMD_4QPP : QSPI_CMD_QPP;

    rc = qspi_nor_write(hdl, cmd, 1, addr, alen, 1, data, dlen, dsize);
    if (rc < 0) {
        fprintf(stderr, "Page program failed.\n");
        return (-1);
    }

    if (sps25fl_busy_wait(hdl, 4096 * dlen) != EOK) {
        return (-1);
    }

    return rc;
}

/*
 * This is the write callout for SPI serial NOR flash.
 */
int32_t sps25fl_write(f3s_dbase_t *dbase,
                         f3s_access_t *access,
                         uint32_t flags,
                         uint32_t offset,
                         int32_t size,
                         uint8_t *buffer)
{
    int         rc;
    uint8_t     *buf;
    int         pagesz = dbase->buffer_size;
    int         left;
    int         cursz;
    void        *hdl = access->socket.socket_handle;

    if (access->service->page(&access->socket, 0, offset, &size) == NULL)
        return -1;

    cursz = pagesz - (offset & (pagesz - 1));
    if (cursz > size)
        cursz = size;
        
    if ((rc = sps25fl_program_page(hdl, offset, buffer, cursz)) < 0) {
        errno = EIO;
        return -1;
    }

    if (!(flags & F3S_VERIFY_WRITE))
        return rc;

    /* verify data was written correctly */
    buf  = alloca(rc);
    if (buf == NULL)
        return -1;
    left = rc;
    while (left) {
        cursz = sps25fl_read_memory (hdl, offset, buf, left);
        if (cursz <= 0) {
            errno = EIO;
            return -1;
        }
        if (memcmp(buffer, buf, cursz)) {
            fprintf(stderr, "(devf  t%d::%s:%d) program verify error\n"
                        "between offset 0x%x and 0x%x, size = %d\n",
                        pthread_self(), __func__, __LINE__, offset, offset+cursz, cursz);
            errno = EIO;
            return -1;
        }
        /* adjust */
        left   -= cursz;
        offset += cursz;
        buffer += cursz;
    }

    return rc;
}
/*
 * Summary
 *
 * MTD Version:    2 only
 * Locking Method: Persistent
 *
 * Description
 *
 * Use this for Cypress S25FS SPI flash capable of block locking.
 */

int sps25fl_unlockall(f3s_dbase_t *dbase, f3s_access_t *access,
                 uint32_t flags, uint32_t offset)
{
    void    *hdl = access->socket.socket_handle;
    /* check that this chip supports Persistent protection. */
    if (!(dbase->flags & F3S_PROTECT_PERSISTENT))
        return (ENOTSUP);

    if (sps25fl_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (EIO);
    }

    if (sps25fl_write_registers(hdl, QSPI_CMD_PPBE, NULL, 0) < 0) {
        fprintf(stderr, "Send PPBE failed.\n");
        return (EIO);
    }

    if (sps25fl_busy_wait(hdl, 4096) == EOK)
        return (EOK);

    return (EIO);
}


/*
 * Summary
 *
 * MTD Version:    2 only
 * Locking Method: Non-persistent
 *
 * Description
 *
 * Use this for Cypress S25FS SPI flash capable of block locking.
 */

int sps25fl_unlock(f3s_dbase_t *dbase, f3s_access_t *access,
                 uint32_t flags, uint32_t offset)
{
    void        *hdl = access->socket.socket_handle;
    spi_flash_t *spi = hdl;
    uint8_t     data = 0xFF;
    /* check that this chip supports Dynamic protection. */
    if (!(dbase->flags & F3S_PROTECT_DYN))
        return (ENOTSUP);

    if (sps25fl_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (EIO);
    }

    if (sps25fl_write_memory(hdl, QSPI_CMD_DYBWR, offset, spi->dcap & SPI_CAP_ADDR4 ? 4 : 3, &data, 1) < 0) {
        fprintf(stderr, "Lock failed.\n");
        return (EIO);
    }

    if (sps25fl_busy_wait(hdl, 4096) == EOK)
        return (EOK);

    return (EIO);
}



/*
 * This is the sync callout for SPI serial NOR flash.
 */
int32_t sps25fl_sync(f3s_dbase_t *dbase,
                    f3s_access_t *access,
                    uint32_t flags,
                    uint32_t text_offset)
{
    if (access->service->page (&access->socket, 0, text_offset, NULL) == NULL)
        return (ERANGE);

    return sps25fl_busy_wait(access->socket.socket_handle, 1);
}

/*
 * This is the erase suspend callout for SPI serial NOR flash.
 */
int sps25fl_suspend(f3s_dbase_t *dbase,
                     f3s_access_t *access,
                     uint32_t flags,
                     uint32_t offset)
{
    int     loop;
    uint8_t sts = 0;
    void    *hdl = access->socket.socket_handle;
    if (access->service->page(&access->socket, 0, offset, NULL) == NULL)
        return (ERANGE);

    if (sps25fl_write_registers(hdl, QSPI_CMD_ERSP, NULL, 0) < 0)
        fprintf(stderr, "Software reset failed.\n");

    /* max 45us according to spec */
    for (loop = 64; loop > 0; loop--) {
        if (sps25fl_read_registers(hdl, QSPI_CMD_RDSR, &sts, 1) < 0) {
            fprintf(stderr, "Read status register failed.\n");
            return (EIO);
        }

        if (!(sts & FLASH_WIP)) {       // not in writing
            if (sps25fl_read_registers(hdl, QSPI_CMD_RDSR2, &sts, 1) < 0) {
                fprintf(stderr, "Read status register failed.\n");
                return (EIO);
            }

            if ((sts & FLASH_ES) == 0)  // no WIP, no suspend, so erase completed
                return (ECANCELED);

            /* erase is suspended */

            return (EOK);
        }

        nanospin_ns(1000);
    }

    return (EIO);
}


/*
 * This is the erase resume callout for SPI serial NOR flash.
 */
int sps25fl_resume(f3s_dbase_t *dbase,
					 f3s_access_t *access,
					 uint32_t flags,
					 uint32_t offset)
{
	if (access->service->page(&access->socket, 0, offset, NULL) == NULL)
		return (ERANGE);

	if (sps25fl_write_registers(access->socket.socket_handle, QSPI_CMD_ERRS, NULL, 0) < 0)
		fprintf(stderr, "Software reset failed.\n");

	nanospin_ns(100 * 1000);

	return (EOK);
}


/*
 * This is the sync callout for SPI serial NOR flash.
 */
void sps25fl_reset (f3s_dbase_t *dbase,
                    f3s_access_t *access,
                    uint32_t flags,
                    uint32_t offset)
{

    if (sps25fl_write_registers(access->socket.socket_handle, QSPI_CMD_RSTEN, NULL, 0) < 0)
        fprintf(stderr, "Software reset failed.\n");

    if (sps25fl_write_registers(access->socket.socket_handle, QSPI_CMD_RST, NULL, 0) < 0)
        fprintf(stderr, "Software reset failed.\n");
}

/*
 * This is the read callout for SPI serial NOR flash.
 */
int32_t sps25fl_read (f3s_dbase_t *dbase,
                      f3s_access_t *access,
                      uint32_t flags,
                      uint32_t text_offset,
                      int32_t buffer_size,
                      uint8_t *buffer)
{
    int rc;

    rc = sps25fl_read_memory(access->socket.socket_handle, text_offset, buffer, buffer_size);

    if (rc < 0) {
        errno = EIO;
        return -1;
    }

    return  rc;     // return number of bytes read
}


int sps25fl_dolock(void *hdl, uint8_t cmd, uint32_t offset, uint8_t *data)
{
    spi_flash_t *spi = hdl;
    if (sps25fl_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (EIO);
    }

    if (sps25fl_write_memory(hdl, cmd, offset, spi->dcap & SPI_CAP_ADDR4 ? 4 : 3, data, data ? 1 : 0) < 0) {
        fprintf(stderr, "Lock failed.\n");
        return (EIO);
    }

    if (sps25fl_busy_wait(hdl, 4096) == EOK)
        return (EOK);

    return (EIO);
}

/*
 * Summary
 *
 * MTD Version:    2 only
 * Locking Method: Persistent and Non-persistent
 *
 * Description
 *
 * Use this for Cypress S25FS SPI flash capable of block locking.
 */

int sps25fl_lock(f3s_dbase_t *dbase, f3s_access_t *access,
                 uint32_t flags, uint32_t offset)
{
    void    *hdl = access->socket.socket_handle;
    uint8_t data = 0;
    /* The DYB bits aren't locked, check the PPB bits */
    if (dbase->flags & F3S_PROTECT_PERSISTENT)
        return sps25fl_dolock(hdl, QSPI_CMD_PPBP, offset, NULL);

    /* check that this chip supports Dynamic protection. */
    if (dbase->flags & F3S_PROTECT_DYN)
        return sps25fl_dolock(hdl, QSPI_CMD_DYBWR, offset, &data);

    return (ENOTSUP);
}

/*
 * Summary
 *
 * MTD Version:    2 only
 * Locking Method: Persistent and Non-persistent
 *
 * Description
 *
 * Use this for Cypress S25FS SPI flash capable of block locking.
 */

int sps25fl_islock(f3s_dbase_t *dbase,
                   f3s_access_t *access,
                   uint32_t flags,
                   uint32_t offset)
{
    uint8_t     data, alen;
    spi_flash_t *spi = access->socket.socket_handle;

    alen = spi->dcap & SPI_CAP_ADDR4 ? 4 : 3;
    /* check that this chip supports Dynamic protection. */
    if (dbase->flags & F3S_PROTECT_DYN) {
        if (sps25fl_read_any_registers(access->socket.socket_handle, QSPI_CMD_DYBRD, offset, alen, &data, 1) < 0) {
            fprintf(stderr, " failed.\n");
            return (EIO);
        }

        if (!(data & 0x01))
            return (EROFS);
    }

    /* The DYB bits aren't locked, check the PPB bits */
    if (dbase->flags & F3S_PROTECT_PERSISTENT) {
        if (sps25fl_read_any_registers(access->socket.socket_handle, QSPI_CMD_PPBRD, offset, alen, &data, 1) < 0) {
            fprintf(stderr, " failed.\n");
            return (EIO);
        }

        if (!(data & 0x01))
            return (EROFS);
    }

    return (EOK);
}

int param_sectors_detect(spi_flash_t *spi, const struct qspi_flash_params *params)
{
#define S25FS_PARAM_SECT_SIZE   (4 * 1024)
#define S25FS_PARAM_SECT_NUM    (8)

    uint8_t cr1 = 0, cr3 = 0;
    uint32_t sector_size;
    uint32_t sector_num;

    /* the S25FS has 8 4KB paramaters sectors overlaid on top or buttom of the flash address space.
     * The sector (size 256KB) that is overlaid by the parameters sectors has 224k of usable
     * space left. However, that left space cannot be handled by the libfs-flash3 lib as a geometry.
     * Therfore, we mark the location of the parameters sectors and deal with them in the erase
     * function.
     */
    if (JEDEC_EXT(params) == 0x4d00 && JEDEC_FID(params) == 0x81) {
        sector_size = S25FS_PARAM_SECT_SIZE;
        sector_num = S25FS_PARAM_SECT_NUM;
    } else {
        return EOK;
    }

    spi->param_sect_num  = 0;

    if (sps25fl_read_any_registers(spi, QSPI_CMD_RDAR, CR1V, 3, &cr1, 1) < 0 ||
        sps25fl_read_any_registers(spi, QSPI_CMD_RDAR, CR3V, 3, &cr3, 1) < 0) {
        fprintf(stderr, "sps25fl_read_any_registers failed.\n");
        return (EIO);
    }

    if (verbose > 3) {
        fprintf(stderr, "(devf  t%d::%s:%d) CR1V 0x%x CR3V 0x%x\n",
            pthread_self(), __func__, __LINE__, cr1, cr3);
    }

    if ((cr3 & 0x08) == 0) {
        // 4-kB Erase enabled (Hybrid Sector Architecture)
        spi->param_sect_size = sector_size;
        spi->param_sect_num  = sector_num;
        if ((cr1 & 0x4) == 0) {
            //TBPARM = 0: Parameter sectors at top (lower address)
            spi->param_sect_addr = 0;
        } else {
            //TBPARM = 1: Parameter sectors at bottom (higher address)
            spi->param_sect_addr = (params->sector_size * params->n_sectors) -
                                    (spi->param_sect_size * spi->param_sect_num);
        }
    }

    if (verbose > 3 && spi->param_sect_num) {
        fprintf(stderr, "(devf  t%d::%s:%d) Param sectors %d, size %dB address 0x%x\n",
            pthread_self(), __func__, __LINE__,
            spi->param_sect_num, spi->param_sect_size, spi->param_sect_addr);
    }

    return EOK;
}

static int sps25fl_enable_quad(void *hdl, int enable)
{
    uint8_t reg[2];
    int quad_ena;

    quad_ena = (enable ? 1 : 0) << 1;
    
    if (sps25fl_read_registers(hdl, QSPI_CMD_RDCR, &reg[1], 1) < 0)
        return (EIO);

    if ((reg[1] & (1 << 1)) != quad_ena) {
        /* write enable */
        if (sps25fl_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
            fprintf(stderr, "Enable write failed.\n");
            return (EIO);
        }

        if (sps25fl_read_registers(hdl, QSPI_CMD_RDSR, &reg[0], 1) < 0) {
            return (EIO);
        }

        reg[1] ^= 1 << 1;

        // write registers
        if (sps25fl_write_registers(hdl, QSPI_CMD_WRR, reg, 2) < 0) {
            fprintf(stderr, "Quad enable failed.\n");
            return (EIO);
        }

        if (sps25fl_busy_wait(hdl, 1024 * 1600) != EOK) {
            return (EIO);
        }
    }

    return (EOK);
}

/*
 * This is the ident callout for Cypress S25FS SPI serial NOR flash.
 */
int32_t sps25fl_ident(f3s_dbase_t *dbase,
                       f3s_access_t *access,
                       uint32_t flags,
                       uint32_t offset)
{
    int32_t     unit_size;
    uint8_t     buf[88];
    int32_t     chip_size = 0;
    spi_flash_t *spi = access->socket.socket_handle;
    const struct qspi_flash_params *params;

    if (sps25fl_read_registers(spi, QSPI_CMD_RDID, buf, 81) < 0) {
        fprintf(stderr, "Read JEDEC-ID failed\n");
        return (ENOENT);
    }

    uint8_t manufacture_id = buf[0];
    uint16_t jedec = buf[1] << 8 | buf[2];
    uint16_t ext_jedec = buf[3] << 8 | buf[4];
    uint8_t family_id = buf[5];

    if (verbose > 3) {
        fprintf(stderr, "(devf  t%d::%s:%d) ID 0x%x JEDEC 0x%x EXT JEDEC 0x%x FAM ID 0x%x \n",
            pthread_self(), __func__, __LINE__, manufacture_id, jedec, ext_jedec,family_id);
    }

    /* Validate params from qspi_flash_params table */
    params = qspi_flash_params_table;
    for (; params->name != NULL; params++) {
        if (JEDEC_MFR(params) == manufacture_id) {
            if (JEDEC_ID(params) == jedec) {
                if (params->id_len - 3 == 0) {
                    break;
                } else if (params->id_len - 3 == 2) {
                    if (JEDEC_EXT(params) == ext_jedec) {
                        break;
                    }
                } else {
                    if ((JEDEC_EXT(params) == (ext_jedec & 0xFF00)) && (JEDEC_FID(params) == family_id)) {
                        break;
                    }
                }
            }
        }
    }

    if (params->name == NULL) {
        if (verbose > 2) {
            fprintf(stderr, "\n(devf  t%d::%s:%d) Identifed not supported \n", pthread_self(), __func__, __LINE__);
        }
        return (ENOTSUP);
    }

    /* Fill dbase entry */
    dbase->struct_size = sizeof(*dbase);
    dbase->jedec_hi    = buf[0];
    dbase->jedec_lo    = ((uint16_t)buf[1] << 8) | buf[2];
    dbase->name        = "Cypress MirrotBit Multi-I/O";

    /* Read buffer size information */
    dbase->buffer_size = buf[0x2b];
    dbase->buffer_size <<= 8;
    dbase->buffer_size += buf[0x2a];

    /* Value is 2^N bytes per chip */
    dbase->buffer_size = 1 << dbase->buffer_size;

    /* Read number of geometries */
    dbase->geo_num = 1;

    dbase->geo_vect[0].unit_num = params->n_sectors;
    unit_size = params->sector_size;

    chip_size += unit_size * dbase->geo_vect[0].unit_num;

    /* Convert size to power of 2 */
    dbase->geo_vect[0].unit_pow2 = 0;
    while (unit_size > 1) {
        unit_size >>= 1;
        dbase->geo_vect[0].unit_pow2++;
    }

    if (verbose > 2) {
        fprintf(stderr, "(devf  t%d::%s:%d) Identifed: %s: Page %dB Sector %dKB Sectors %d Total %dMB\n",
            pthread_self(), __func__, __LINE__, params->name, params->page_size,
            (params->sector_size/1024), params->n_sectors, chip_size/1024/1024);
    }

    access->socket.window_size = chip_size;

    if( (params->flags & RD_FULL) && (params->flags & WR_QPP)){
        spi->dcap |= SPI_CAP_QUAD | SPI_CAP_ADDR4;
    }

    /* Detect read / write suspend */
    if      (buf[0x46] == 1) dbase->flags = F3S_ERASE_FOR_READ;
    else if (buf[0x46] == 2) dbase->flags = F3S_ERASE_FOR_READ | F3S_ERASE_FOR_WRITE;
    else                     dbase->flags = 0;

    param_sectors_detect(spi, params);

    if( spi->dcap & SPI_CAP_QUAD && spi->hcap & SPI_CAP_QUAD) {
        if (sps25fl_enable_quad(spi, 1) == EOK) {
            spi->mode |= SPI_MODE_QUAD;
        }
    }

    return (EOK);
}


static int sps25fl_sector_erase(void *hdl, uint32_t addr, int alen, int sector_size)
{
    uint8_t cmd = QSPI_CMD_P4E;  // default command

    /* write enable */
    if (sps25fl_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Enable write failed.\n");
        return (EIO);
    }

    if (sector_size == 4096) {
        if (alen == 4)
            cmd = QSPI_CMD_4P4E;
    } else {
        if (alen == 4)
            cmd = QSPI_CMD_4SE;
        else
            cmd = QSPI_CMD_SE;
    }

    if (sps25fl_write_memory(hdl, cmd, addr, alen, NULL, 0) < 0) {
        fprintf(stderr, "Sector erase failed.\n");
        return (EIO);
    }

    if (sps25fl_busy_wait(hdl, 1024 * 4096) != EOK)
    {
        return (-1);
    }
    return (EOK);
}


/*
 * This is the erase callout for SPI serial NOR flash.
 */
int sps25fl_erase(f3s_dbase_t *dbase,
                     f3s_access_t *access,
                     uint32_t flags,
                     uint32_t offset)
{
    spi_flash_t *spi = access->socket.socket_handle;
    uint32_t     sector_size = 1 << dbase->geo_vect[0].unit_pow2;
    uint32_t     param_offset = 0;
    uint8_t      alen = spi->dcap & SPI_CAP_ADDR4 ? 4 : 3;
    int          rc = 0;

    if (access->service->page(&access->socket, 0, offset, NULL) == NULL)
        return (ERANGE);

    // If the parameters sectors are enabled, erase them with 4P4E command.
    if (spi->param_sect_num > 0) {
        // First or last sector
        if ((spi->param_sect_addr == 0 && offset == 0) ||
            (spi->param_sect_addr && offset >= (access->socket.window_size - sector_size))) {
            for (int i = 0; i < spi->param_sect_num; i++){
                param_offset = spi->param_sect_addr + (spi->param_sect_size * i);
                rc = sps25fl_sector_erase(spi, param_offset, alen, spi->param_sect_size);

                if (rc < 0)
                    return rc;
            }
        }
    }

    /* 4 bytes address cycle, variable sector size */
    return sps25fl_sector_erase(spi, offset, alen, sector_size);
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/flash/boards/rcar_qspi/f3s_rpc_sps25fl.c $ $Rev: 870476 $")
#endif
