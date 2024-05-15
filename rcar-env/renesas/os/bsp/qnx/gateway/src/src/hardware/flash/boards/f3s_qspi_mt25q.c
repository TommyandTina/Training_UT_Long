/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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
#include <sys/slog.h>

/* TO DO: Read page-size from the Serial Flash Discovery Parameter (SFDP) Table */
#define MT25Q_PAGE_SIZE    256

const struct qspi_flash_params mt25q_qspi_flash_params_table[] = {
    /* Micron */
    {"mt25qu512a",   INFO(0x20BB20, 0x104C, 64 * 1024,    1024, 0) },
    {}, /* Empty entry to terminate the list */
};

static int mt25q_read_registers(void *hdl, uint8_t cmd, uint8_t *data, int dlen)
{
    spi_flash_t *spi = (spi_flash_t *)hdl;
    
    if (spi->mode & SPI_MODE_QUAD)
        return qspi_nor_read_registers(hdl, cmd, 4, 0, 0, 0, data, dlen, 4);
    else
        return (qspi_nor_read_registers(hdl, cmd, 1, 0, 0, 0, data, dlen, 1));
}

static int mt25q_read_memory(void *hdl, uint8_t cmd, uint32_t addr, uint8_t *data, int dlen, int dummy)
{
    spi_flash_t *spi = (spi_flash_t *)hdl;
    int alen;
    
    alen = spi->dcap & SPI_CAP_ADDR4 ? 4 : 3;    
    
    if (spi->mode & SPI_MODE_QUAD)
        return qspi_nor_read_memory(hdl, cmd, 4, addr, alen, 4, data, dlen, 4, dummy);
    else
        return qspi_nor_read_memory(hdl, cmd, 1, addr, alen, 1, data, dlen, 1, dummy);
}

static int mt25q_write_registers(void *hdl, uint8_t cmd, uint8_t *data, int dlen)
{
    spi_flash_t *spi = (spi_flash_t *)hdl;

    if (spi->mode & SPI_MODE_QUAD)
        return qspi_nor_write(hdl, cmd, 4, 0, 0, 0, data, dlen, 4);
    else
        return qspi_nor_write(hdl, cmd, 1, 0, 0, 0, data, dlen, 1);
}

static int mt25q_busy_wait(void *hdl, uint32_t tmo)
{
    uint8_t sts = 0;

    while (tmo--) {
        if (mt25q_read_registers(hdl, QSPI_CMD_RDSR, &sts, 1) < 0) {
            return (EIO);
        }

        if (!(sts & FLASH_WIP)) {
            return (EOK);
        }

        nanospin_ns(1000);
    }

    return (EAGAIN);
}

static int mt25q_write_memory(void *hdl, uint8_t cmd, uint32_t addr, uint8_t *data, int dlen)
{
    spi_flash_t *spi = (spi_flash_t *)hdl;
    int alen, rc;

    if (mt25q_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (-1);
    }
    
    alen = spi->dcap & SPI_CAP_ADDR4 ? 4 : 3;
    
    if (spi->mode & SPI_MODE_QUAD)
        rc = qspi_nor_write(hdl, cmd, 4, addr, alen, 4, data, dlen, 4);
    else
        rc = qspi_nor_write(hdl, cmd, 1, addr, alen, 1, data, dlen, 1);

    if (rc < 0) {
        fprintf(stderr, "Page program failed.\n");
        return (-1);
    }

    if (mt25q_busy_wait(hdl, 4096 * dlen) != EOK)
    {
        return (-1);
    }

    return rc;
}

/*
 * This is the write callout for SPI serial NOR flash.
 */
int32_t mt25q_write(f3s_dbase_t *dbase,
                         f3s_access_t *access,
                         uint32_t flags,
                         uint32_t offset,
                         int32_t size,
                         uint8_t *buffer)
{
    int         rc;
    uint8_t      *buf;
    int         pagesz = dbase->buffer_size;
    int         left;
    int         cursz;
    void        *hdl = access->socket.socket_handle;
    spi_flash_t *spi = hdl;
    uint8_t     cmd;

    if (access->service->page(&access->socket, 0, offset, &size) == NULL)
        return -1;

    cursz = pagesz - (offset & (pagesz - 1));
    if (cursz > size)
        cursz = size;

    if (spi->mode & SPI_MODE_QUAD) {
        cmd = spi->dcap & SPI_CAP_ADDR4 ? QSPI_CMD_4QPP : QSPI_CMD_PP;
    } else {
        cmd = spi->dcap & SPI_CAP_ADDR4 ? QSPI_CMD_4PP : QSPI_CMD_PP;
    }

    if ((rc = mt25q_write_memory(hdl, cmd, offset, buffer, cursz)) < 0) {
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
        cursz = mt25q_read(dbase, access, flags, offset, left, buf);

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
 * Use this for SPI flash capable of block locking.
 */

int mt25q_unlockall(f3s_dbase_t *dbase, f3s_access_t *access,
                 uint32_t flags, uint32_t offset)
{
    void    *hdl = access->socket.socket_handle;
    /* check that this chip supports Persistent protection. */
    if (!(dbase->flags & F3S_PROTECT_PERSISTENT))
        return (ENOTSUP);

    if (mt25q_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (EIO);
    }

    if (mt25q_write_registers(hdl, QSPI_CMD_PPBE, NULL, 0) < 0) {
        fprintf(stderr, "Send PPBE failed.\n");
        return (EIO);
    }

    if (mt25q_busy_wait(hdl, 4096) == EOK)
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
 * Use this for SPI flash capable of block locking.
 */

int mt25q_unlock(f3s_dbase_t *dbase, f3s_access_t *access,
                 uint32_t flags, uint32_t offset)
{
    void        *hdl = access->socket.socket_handle;
    uint8_t     data = 0xFF;
    /* check that this chip supports Dynamic protection. */
    if (!(dbase->flags & F3S_PROTECT_DYN))
        return (ENOTSUP);

    if (mt25q_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (EIO);
    }

    if (mt25q_write_memory(hdl, QSPI_CMD_DYBWR, offset, &data, 1) < 0) {
        fprintf(stderr, "Lock failed.\n");
        return (EIO);
    }

    if (mt25q_busy_wait(hdl, 4096) == EOK)
        return (EOK);

    return (EIO);
}



/*
 * This is the sync callout for SPI serial NOR flash.
 */
int32_t mt25q_sync (f3s_dbase_t *dbase,
                    f3s_access_t *access,
                    uint32_t flags,
                    uint32_t text_offset)
{
    if (access->service->page (&access->socket, 0, text_offset, NULL) == NULL)
        return (ERANGE);

    return mt25q_busy_wait(access->socket.socket_handle, 1);
}

#define MT25Q_FLAG_STATUS     0x70
#define MT25Q_FLAG_ERA_SUSP   (1 << 6)

/*
 * This is the erase suspend callout for SPI serial NOR flash.
 */
int mt25q_suspend(f3s_dbase_t *dbase,
                     f3s_access_t *access,
                     uint32_t flags,
                     uint32_t offset)
{
    int     loop;
    uint8_t sts = 0;
    void    *hdl = access->socket.socket_handle;
    if (access->service->page(&access->socket, 0, offset, NULL) == NULL)
        return (ERANGE);

    if (mt25q_write_registers(hdl, QSPI_CMD_ERSP, NULL, 0) < 0)
        fprintf(stderr, "Software reset failed.\n");

    /* max 45us according to spec */
    for (loop = 64; loop > 0; loop--) {
        if (mt25q_read_registers(hdl, QSPI_CMD_RDSR, &sts, 1) < 0) {
            fprintf(stderr, "Read status register failed.\n");
            return (EIO);
        }

        if (!(sts & FLASH_WIP)) {       // not in writing
            if (mt25q_read_registers(hdl, MT25Q_FLAG_STATUS, &sts, 1) < 0) {
                fprintf(stderr, "Read status register failed.\n");
                return (EIO);
            }

            if ((sts & MT25Q_FLAG_ERA_SUSP) == 0)  // no WIP, no suspend, so erase completed
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
int mt25q_resume(f3s_dbase_t *dbase,
					 f3s_access_t *access,
					 uint32_t flags,
					 uint32_t offset)
{
	if (access->service->page(&access->socket, 0, offset, NULL) == NULL)
		return (ERANGE);

	if (mt25q_write_registers(access->socket.socket_handle, QSPI_CMD_ERRS, NULL, 0) < 0)
		fprintf(stderr, "Software reset failed.\n");

	nanospin_ns(100 * 1000);

	return (EOK);
}


/*
 * This is the sync callout for SPI serial NOR flash.
 */
void mt25q_reset (f3s_dbase_t *dbase,
                    f3s_access_t *access,
                    uint32_t flags,
                    uint32_t offset)
{

    if (mt25q_write_registers(access->socket.socket_handle, QSPI_CMD_RSTEN, NULL, 0) < 0)
        fprintf(stderr, "Software reset failed.\n");

    if (mt25q_write_registers(access->socket.socket_handle, QSPI_CMD_RST, NULL, 0) < 0)
        fprintf(stderr, "Software reset failed.\n");
}

/*
 * This is the read callout for SPI serial NOR flash.
 */
int32_t mt25q_read (f3s_dbase_t *dbase,
                      f3s_access_t *access,
                      uint32_t flags,
                      uint32_t text_offset,
                      int32_t buffer_size,
                      uint8_t *buffer)
{
    int         rc;
    uint8_t     cmd;
    void        *hdl = access->socket.socket_handle;
    spi_flash_t *spi = hdl;

    /* check if offset does not fit in array */
    if (text_offset >= access->socket.window_size) {
        errno = ERANGE;
        return -1;
    }

    if (spi->mode & SPI_MODE_QUAD)
        cmd = spi->dcap & SPI_CAP_ADDR4 ? QSPI_CMD_4QOR : QSPI_CMD_QOR;
    else
        cmd = spi->dcap & SPI_CAP_ADDR4 ? QSPI_CMD_4FREAD : QSPI_CMD_FREAD;

    /* ensure that offset + size is not out of bounds */
    buffer_size = min(buffer_size, access->socket.window_size - text_offset);

    rc = mt25q_read_memory(hdl, cmd, text_offset, buffer, buffer_size, 10);
    if (-1 == rc) {
        fprintf(stderr,"\tOffset: 0x%X Size: %d\n",text_offset,buffer_size);
        errno = EIO;
        return -1;
    }
    return rc; /* return number of bytes read */
}

/*
 * Summary
 *
 * MTD Version:    2 only
 * Locking Method: Persistent and Non-persistent
 *
 * Description
 *
 * Use this for SPI flash capable of block locking.
 */

int mt25q_lock(f3s_dbase_t *dbase, f3s_access_t *access,
                 uint32_t flags, uint32_t offset)
{
    return (ENOTSUP);
}

int mt25q_dolock(void *hdl, uint8_t cmd, uint32_t offset, uint8_t *data)
{
    if (mt25q_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
        fprintf(stderr, "Send WREN failed.\n");
        return (EIO);
    }

    if (mt25q_write_memory(hdl, cmd, offset, data, data ? 1 : 0) < 0) {
        fprintf(stderr, "Lock failed.\n");
        return (EIO);
    }

    if (mt25q_busy_wait(hdl, 4096) == EOK)
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
 * Use this for SPI flash capable of block locking.
 */

int mt25q_islock(f3s_dbase_t *dbase,
                   f3s_access_t *access,
                   uint32_t flags,
                   uint32_t offset)
{
    uint8_t     data;

    /* check that this chip supports Dynamic protection. */
    if (dbase->flags & F3S_PROTECT_DYN) {
        if (mt25q_read_memory(access->socket.socket_handle, QSPI_CMD_DYBRD, offset, &data, 1, 0) < 0) {
            fprintf(stderr, " failed.\n");
            return (EIO);
        }

        if (!(data & 0x01))
            return (EROFS);
    }

    /* The DYB bits aren't locked, check the PPB bits */
    if (dbase->flags & F3S_PROTECT_PERSISTENT) {
        if (mt25q_read_memory(access->socket.socket_handle, QSPI_CMD_PPBRD, offset, &data, 1, 0) < 0) {
            fprintf(stderr, " failed.\n");
            return (EIO);
        }

        if (!(data & 0x01))
            return (EROFS);
    }

    return (EOK);
}

static int mt25q_enable_quad(void *hdl, int enable)
{
    spi_flash_t *spi = (spi_flash_t *)hdl;
    uint8_t st;

    /* write enable */
    if (enable & !(spi->mode & SPI_MODE_QUAD)) {
        //if (mt25q_write_registers(hdl, 0x35, NULL, 0) < 0) {
        //    fprintf(stderr, "Enter quad mode failed.\n");
        //    return (EIO);
        //}        
        if (mt25q_read_registers(hdl, 0x65, &st, 1) < 0) {
            fprintf(stderr, "Read 0x65 cmd failed\n");
            return (EIO);
        }
        if (mt25q_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
            fprintf(stderr, "Enable write failed.\n");
            return (EIO);
        }
        st &= ~(1 << 7);
        if (mt25q_write_registers(hdl, 0x61, &st, 1) < 0) {
            fprintf(stderr, "Write 0x61 cmd failed.\n");
            return (EIO);
        }
        spi->mode |= SPI_MODE_QUAD;
    } else if (!enable && (spi->mode & SPI_MODE_QUAD)) {
        //if (mt25q_write_registers(hdl, 0xF5, NULL, 0) < 0) {
        //    fprintf(stderr, "Reset quad mode failed.\n");
        //    return (EIO);
        //}        
        if (mt25q_read_registers(hdl, 0x65, &st, 1) < 0) {
            fprintf(stderr, "Read 0x65 cmd failed\n");
            return (EIO);
        }
        if (mt25q_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
            fprintf(stderr, "Enable write failed.\n");
            return (EIO);
        }
        st |= (1 << 7);
        if (mt25q_write_registers(hdl, 0x61, &st, 1) < 0) {
            fprintf(stderr, "Write 0x61 cmd failed.\n");
            return (EIO);
        }
        spi->mode &= ~SPI_MODE_QUAD;
    }

    return (EOK);
}

/*
 * This is the ident callout for Micron SPI serial NOR flash.
 */
int32_t mt25q_ident(f3s_dbase_t *dbase,
                       f3s_access_t *access,
                       uint32_t flags,
                       uint32_t offset)
{
    int32_t     unit_size;
    int32_t     geo_index = 0;
    uint8_t     buf[4];
    int32_t     chip_size = 0;
    spi_flash_t *spi = access->socket.socket_handle;
    const struct qspi_flash_params *params;

    if (mt25q_read_registers(access->socket.socket_handle, QSPI_CMD_RDID, buf, 4) < 0) {
        fprintf(stderr, "Read JEDEC-ID failed\n");
        return (ENOENT);
    }

    uint8_t manufacture_id = buf[0];
    uint16_t jedec = buf[1] << 8 | buf[2];

    /* Validate params from qspi_flash_params table */
    params = mt25q_qspi_flash_params_table;
    for (; params->name != NULL; params++) {
        if (JEDEC_MFR(params) == manufacture_id) {
            slogf(1000, 1, "(devf  t%d::%s:%d) Manufacture ID: 0x%x", pthread_self(), __func__,__LINE__, manufacture_id);
            if (JEDEC_ID(params) == jedec) {
                slogf(1000, 1, "(devf  t%d::%s:%d) JEDEC ID: 0x%x", pthread_self(), __func__,__LINE__, jedec);
                break;
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
    dbase->name        = "Micron MT25Q QSPI";

    /* Buffer size information */
    dbase->buffer_size = MT25Q_PAGE_SIZE;

    /* Read number of geometries */
    dbase->geo_num = 1;
    dbase->geo_vect[0].unit_num = 1024;
    unit_size = 64*1024;

    chip_size += unit_size * dbase->geo_vect[geo_index].unit_num;

    /* Convert size to power of 2 */
    dbase->geo_vect[0].unit_pow2 = 0;
    while (unit_size > 1) {
        unit_size >>= 1;
        dbase->geo_vect[0].unit_pow2++;
    }

    spi->dcap |= SPI_CAP_QUAD | SPI_CAP_ADDR4;

    if( spi->dcap & SPI_CAP_QUAD && spi->hcap & SPI_CAP_QUAD) {
        mt25q_enable_quad(spi, 1);
    }

    access->socket.window_size = chip_size;
    dbase->flags=0;

   return (EOK);
}


static int mt25q_sector_erase(void *hdl, uint32_t addr, int sector_size)
{
    spi_flash_t *spi = hdl;
    uint8_t cmd = QSPI_CMD_P4E;  // default command
    int alen = spi->dcap & SPI_CAP_ADDR4 ? 4 : 3;

    /* write enable */
    if (mt25q_write_registers(hdl, QSPI_CMD_WREN, NULL, 0) < 0) {
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

    if (mt25q_write_memory(hdl, cmd, addr, NULL, 0) < 0) {
        fprintf(stderr, "Sector erase failed.\n");
        return (EIO);
    }

    if (mt25q_busy_wait(hdl, 1024 * 4096) != EOK)
    {
        return (-1);
    }
    return (EOK);
}

/*
 * This is the erase callout for SPI serial NOR flash.
 */
int mt25q_erase(f3s_dbase_t *dbase,
                     f3s_access_t *access,
                     uint32_t flags,
                     uint32_t offset)
{
    int         geo_index;
    int         size = 0;
    if (access->service->page(&access->socket, 0, offset, NULL) == NULL)
        return (ERANGE);

    for (geo_index = 0; geo_index < dbase->geo_num; geo_index++) {
        size += (1 << dbase->geo_vect[geo_index].unit_pow2) * dbase->geo_vect[geo_index].unit_num;

        if (size > offset)
            break;
    }

    /* 4 bytes address cycle, variable sector size */
    return mt25q_sector_erase(access->socket.socket_handle,
                    offset, 1 << dbase->geo_vect[geo_index].unit_pow2);
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/flash/boards/rcar_qspi/f3s_qspi_mt25q.c $ $Rev: 872834 $")
#endif
