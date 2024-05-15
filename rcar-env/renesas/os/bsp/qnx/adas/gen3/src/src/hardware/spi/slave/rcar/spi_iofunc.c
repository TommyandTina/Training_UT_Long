/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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

#include "spi_slave.h"
#include "msiof.h"

/* Global variable */

resmgr_attr_t             rattr;
resmgr_connect_funcs_t    connect_funcs;
resmgr_io_funcs_t         io_funcs;
iofunc_funcs_t            ocb_funcs = { _IOFUNC_NFUNCS, ocb_calloc, ocb_free };
iofunc_mount_t            spi_mount = { 0, 0, 0, 0, &ocb_funcs };

/*******************************************************************************
* Outline      : This function creates resource manager for SPI driver
* Function Name: spi_slave_create_instance
* Description  : Create pathname, map io function and resource manager.
* Arguments    : void * data
* Return Value : EXIT_SUCCESS
                 EXIT_FAILURE
* Note         :
*******************************************************************************/
int spi_slave_create_instance (void * data)
{
    spi_slave_t * dev = data;
    char  devname[PATH_MAX + 1];
    /* Create dispatch layer */
    if (NULL == (dev->dpp = dispatch_create()))
    {
        perror("dispatch_create");
        goto failed0;
    }

    /* set up i/o handler functions */
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
            _RESMGR_IO_NFUNCS, &io_funcs);
    /* overwrite default handle with SPI driver handle function */
    io_funcs.devctl    = io_devctl;
    io_funcs.write     = io_write;
    io_funcs.read      = io_read;

    iofunc_attr_init(&dev->attr, S_IFCHR , NULL, NULL);
    /* mount our own OCB to resource manager
     * with this OCB, we can pass our device to io function */
    dev->attr.mount    = &spi_mount;

    /* Set resource manager attribute */
    memset(&rattr, 0, sizeof (rattr));
    rattr.nparts_max   = SPI_RESMGR_NPARTS;
    rattr.msg_max_size = SPI_RESMGR_MSGSIZE;

    /* register device name */
    snprintf(devname, PATH_MAX, "/dev/spi%d", dev->channel);
    if (-1 == (dev->res_id =  resmgr_attach(dev->dpp, &rattr, devname, _FTYPE_ANY, 0,
                    &connect_funcs, &io_funcs, &dev->attr)))
    {
        perror("resmgr_attach");
        goto failed1;
    }

    if ((dev->ctp = dispatch_context_alloc(dev->dpp)) == NULL)
        goto failed1;
    /* Return if there is no error */
    return EXIT_SUCCESS;

    failed1:
        dispatch_destroy(dev->dpp);
    failed0:
        return EXIT_FAILURE;
}

/*******************************************************************************
* Outline      : This function handle devctl function
* Function Name: io_devctl
* Description  :
* Arguments    : resmgr_context_t *ctp
*                io_devctl_t *msg
*                RESMGR_OCB_T *ocb
* Return Value : EXIT_SUCCESS
*                Specific error
* Note         :
*******************************************************************************/
int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
    spi_slave_t     * dev = (spi_slave_t *)(ocb->attr);
    spi_slave_cfg_t * cfg = NULL;
    spi_data_t      * data = NULL;
    uint8_t  *buf_8  = (uint8_t *)dev->rx_buff.data;
    uint16_t *buf_16 = (uint16_t *)dev->rx_buff.data;
    uint32_t *buf_32 = (uint32_t *)dev->rx_buff.data;
    unsigned status;
    int idx;
    int result = EOK;
    int ret = 0;

    status = iofunc_devctl_default(ctp, msg, ocb);
    if (_RESMGR_DEFAULT != status)
        return status;

    switch (msg->i.dcmd)
    {
        case DCMD_SPI_SLAVE_BUS_RESET:
        {
            status = spi_slave_rst_txrx(dev);
            if (EOK != status)
            {
                result = -1;
                break;
            }
            /* Set data output of message */
            memset(&msg->o, 0, sizeof(msg->o));
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o));

            break;
        }
        case DCMD_SPI_SLAVE_ENABLE_XCHANGE:
        {
            rcar_spi_slogf(dev, VERBOSE_LEVEL2, "[Slog DBG] devctl: ENABLE_XCHANGE CMD");
            status = spi_slave_enable_xchange(dev);
            if (EOK != status)
            {
                result = -1;
                break;
            }

            /* Set data output of message */
            memset(&msg->o, 0, sizeof(msg->o));
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o));
            break;
        }
        case DCMD_SPI_SLAVE_DISABLE_XCHANGE:
        {
            status = spi_slave_disable_xchange(dev);
            if (EOK != status)
            {
                result = -1;
                break;
            }
            /* Set data output of message */
            memset(&msg->o, 0, sizeof(msg->o));
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o));
            break;
        }
        case DCMD_SPI_SLAVE_GET_CONFIG:
        {
            rcar_spi_slogf(dev, VERBOSE_LEVEL2, "[Slog DBG] devctl: GET_CONFIG CMD");
            /* Check number of byte client */
            if (msg->o.nbytes < sizeof(spi_slave_cfg_t))
            {
                result = EINVAL;
                break;
            }
            cfg = (spi_slave_cfg_t *)_DEVCTL_DATA(msg->o);
            /* Set data output of message */
            memcpy(cfg, &dev->cfg, sizeof(spi_slave_t));
            msg->o.nbytes = sizeof(spi_slave_t);
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o) + sizeof(spi_slave_cfg_t));

            break;
        }
        case DCMD_SPI_SLAVE_SET_CONFIG:
        {
            rcar_spi_slogf(dev, VERBOSE_LEVEL2, "[Slog DBG] devctl: SET_CONFIG CMD");

            if (msg->i.nbytes < sizeof(spi_slave_cfg_t))
            {
                result = EINVAL;
                break;
            }

            cfg = (spi_slave_cfg_t *)_DEVCTL_DATA(msg->i);
            ret = BIT_LEN_VALIDATE(cfg->bitlen);
            ret &= WORD_LEN_VALIDATE(cfg->wordlen);
            ret &= CPOL_VALIDATE(cfg->cpol);
            ret &= CPHA_VALIDATE(cfg->cpha);
            if (0 == ret)
            {
                result = EINVAL;
                break;
            }

            memcpy(&dev->cfg, cfg, sizeof(spi_slave_cfg_t));
            status = spi_slave_config(dev);
            if (EOK != status)
            {
                result = EINVAL;
                break;
            }

            /* Set data output of message */
            memset(&msg->o, 0, sizeof(msg->o));
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o));
            break;
        }
        case DCMD_SPI_SLAVE_READ_BYTE:
        {
            if (msg->o.nbytes < sizeof(spi_data_t))
            {
                result = EINVAL;
                break;
            }

            data = (spi_data_t *)(_DEVCTL_DATA(msg->o));

            dev->rx_data.status = dev->status;
            if (dev->rx_buff.tail >= dev->rx_buff.head)
            {
                dev->rx_data.status |= SPI_STATUS_NO_DATA_AVAILABLE;
            }
            else
            {
                /* Get data and index from rx buffer */
                idx = dev->rx_buff.tail % BUFF_MAX_SIZE;

                if (dev->cfg.bitlen == SPI_BIT_LEN_16)
                {
                    dev->rx_data.byte = buf_16[idx];
                }
                else if (dev->cfg.bitlen == SPI_BIT_LEN_32)
                {
                    dev->rx_data.byte = buf_32[idx];
                }
                else
                {
                    dev->rx_data.byte = buf_8[idx];
                }

                dev->rx_data.idx  = idx;

                dev->rx_buff.tail++;
            }

            /* Set data output of message */
            memcpy(data, &dev->rx_data, sizeof(spi_data_t));
            msg->o.nbytes = sizeof(spi_data_t);
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o) + sizeof(spi_data_t));

            break;
        }
        case DCMD_SPI_SLAVE_WRITE_BYTE:
        {
            if (msg->i.nbytes < sizeof(spi_data_t))
            {
                result = EINVAL;
                break;
            }

            data = _DEVCTL_DATA(msg->i);
            if (1 != spi_slave_write_fifo(dev, (void *)&data->byte, 1))
            {
                result = -1;
                break;
            }

            /* Set data output of message */
            memset(&msg->o, 0, sizeof(msg->o));
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o));
            break;
        }
        default:
            result = ENOTTY;
            break;
    }
    return result;
}

/*******************************************************************************
* Outline      : This function handle write function
* Function Name: io_write
* Description  : This function read data from client message, then transmit data to master,
*                we don't care data read from master in the frame.
* Arguments    : resmgr_context_t *ctp
*                io_write_t *msg
*                RESMGR_OCB_T *ocb
* Return       : Number of writen bytes (if transmittion occured)
*                -1: No transmittion occured
*                -2: Invalid parameter
*******************************************************************************/
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
    int status;
    int nbytes;
    int data_number = 0;
    spi_slave_t * dev = (spi_slave_t *)ocb->attr;

    if ((status = iofunc_write_verify (ctp, msg, ocb, NULL)) != EOK)
        return -2;

    /* Get number of bytes that client want to write */
    nbytes = _IO_WRITE_GET_NBYTES(msg);

    resmgr_msgread(ctp, dev->tx_buff, nbytes, sizeof(msg->i));

    if (nbytes > 0)
    {
        /* Get number of transfer data */
        data_number = nbytes;
        if (dev->cfg.bitlen == SPI_BIT_LEN_16)
        {
            /* number of data (with datasize = 16bit) = number of bytes / 2 */
            if ((data_number % 2) != 0)
            {
                rcar_spi_slogf(dev, VERBOSE_QUIET,
                               "[Slog ERROR] io_write: for 16bit datasize, read length must be a multiples of 2");
                nbytes = -2;
            }
            else
            {
                data_number /= 2;
            }
        }
        else if (dev->cfg.bitlen == SPI_BIT_LEN_32)
        {
            /* number of data (with datasize = 16bit) = number of bytes / 4 */
            if ((data_number % 4) != 0)
            {
                rcar_spi_slogf(dev, VERBOSE_QUIET,
                               "[Slog ERROR] io_write: for 32bit datasize, read length must be a multiples of 4");
                nbytes = -2;
            }
            else
            {
                data_number /= 4;
            }
        }

        if (data_number > BUFF_MAX_SIZE)
        {
            rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] io_write: number of data exceed 64");
            nbytes = -2;
        }
        if (nbytes > 0)
        {
            status = spi_slave_xfer(dev, data_number);

            if (EXIT_FAILURE == status)
            {
                /* In case of SYNC ERR, we return number of read data */
                if (dev->status & SPI_STATUS_SYNC_ERR)
                {
                    /* Return number of read bytes */
                    nbytes = dev->rx_buff.head * (data_number / nbytes);
                }
                else
                {
                    nbytes = -1;
                }
            }
        }
    }
    else
    {
        nbytes = -2;
    }
    if (nbytes > 0)
    {
        ocb->attr->flags |= IOFUNC_ATTR_ATIME;
        iofunc_time_update(ocb->attr);
    }
    /* set up the number of bytes (returned by client's write()) */
    _IO_SET_READ_NBYTES(ctp, nbytes);
    return (_RESMGR_NPARTS(0));
}

/*******************************************************************************
* Outline      : This function handle read function
* Function Name: io_read
* Description  : This function wait and read data from master,
*                data to transmit to master will be all dummy 0.
* Arguments    : resmgr_context_t *ctp
*                io_read_t *msg
*                RESMGR_OCB_T *ocb
* Return       : Number of read bytes (if transmittion occured)
*                -1: No transmittion occured
*                -2: Invalid parameter
*******************************************************************************/
int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int status;
    int nbytes;
    int nparts = 0;
    int data_number = 0;
    spi_slave_t * dev = (spi_slave_t *)ocb->attr;

    if ((status = iofunc_read_verify (ctp, msg, ocb, NULL)) != EOK)
        return status;

    /* Get number of bytes that client want to read */
    nbytes = _IO_READ_GET_NBYTES(msg);

    if (nbytes > 0)
    {
        /* Get number of transfer data */
        data_number = nbytes;
        if (dev->cfg.bitlen == SPI_BIT_LEN_16)
        {
            if ((data_number % 2) != 0)
            {
                rcar_spi_slogf(dev, VERBOSE_QUIET,
                               "[Slog ERROR] io_read: for 16bit datasize, read length must be a multiples of 2");
                nbytes = -2;
            }
            else
            {
                data_number /= 2;
            }
        }
        else if (dev->cfg.bitlen == SPI_BIT_LEN_32)
        {
            if ((data_number % 4) != 0)
            {
                rcar_spi_slogf(dev, VERBOSE_QUIET,
                               "[Slog ERROR] io_read: for 32bit datasize, read length must be a multiples of 4");
                nbytes = -2;
            }
            else
            {
                data_number /= 4;
            }
        }

        if (data_number > BUFF_MAX_SIZE)
        {
            rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] io_read: number of data exceed 64");
            nbytes = -2;
        }
        if (nbytes > 0)
        {
            /* All datas transmitted to master are dummy 0 */
            status = spi_slave_xfer(dev, data_number);

            if (EXIT_FAILURE == status)
            {
                /* In case of SYNC ERR, we return number of read data */
                if (dev->status & SPI_STATUS_SYNC_ERR)
                {
                    /* Return number of read bytes */
                    nbytes = dev->rx_buff.head * (data_number / nbytes);
                }
                else
                {
                    nbytes = -1;
                }
            }
        }
    }
    else
    {
        nbytes = -2;
    }
    if (nbytes > 0)
    {
        nparts = 1;
        /* set up the return data IOV */
        if (dev->cfg.bitlen == SPI_BIT_LEN_8)
        {
            SETIOV (ctp->iov, dev->rx_buff.data, nbytes);
        }
        else if (dev->cfg.bitlen == SPI_BIT_LEN_16)
        {
            SETIOV (ctp->iov, dev->rx_buff.data, nbytes);
        }
        else
        {
            SETIOV (ctp->iov, dev->rx_buff.data, nbytes * 4);
        }
        ocb->attr->flags |= IOFUNC_ATTR_ATIME;
        iofunc_time_update(ocb->attr);
    }
    /* set up the number of bytes (returned by client's read()) */
    _IO_SET_READ_NBYTES(ctp, nbytes);
    return (_RESMGR_NPARTS(nparts));
}

/*******************************************************************************
* Outline      : This function allocates a block of memory for OCB
* Function Name: ocb_calloc
* Arguments    : resmgr_context_t *ctp
*                IOFUNC_ATTR_T *attr
* Note         :
*******************************************************************************/
iofunc_ocb_t * ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *attr)
{
    iofunc_ocb_t * ocb;

    if (NULL == (ocb = calloc(1, sizeof(iofunc_ocb_t))))
        return NULL;

    return ocb;
}

/*******************************************************************************
* Outline      : This function free memory that contains OCB
* Function Name: ocb_free
* Arguments    : iofunc_ocb_t *ocb
* Note         :
*******************************************************************************/
void ocb_free(iofunc_ocb_t *ocb)
{
    free(ocb);
}
