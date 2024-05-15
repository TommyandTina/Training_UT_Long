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

#include "proto.h"
#include "msiof.h"

/* Global variable */

resmgr_attr_t             rattr;
resmgr_connect_funcs_t    connect_funcs;
resmgr_io_funcs_t         io_funcs;
iofunc_funcs_t            ocb_funcs = { _IOFUNC_NFUNCS, ocb_calloc, ocb_free };
iofunc_mount_t            spi_mount = { 0, 0, 0, 0, &ocb_funcs };

int event_pulse_handler(message_context_t * ctp,int code,unsigned flags,void *handle)
{
    spi_slave_t *dev = (spi_slave_t *)handle;
    if(dev->status & SPI_STATUS_TRANSFER_DONE){
        resmgr_msg_again(ctp, dev->rcvid);	
    }
    return 0;
}

int  io_unblock(resmgr_context_t *ctp, io_pulse_t *msg, RESMGR_OCB_T *ocb)
{
    return EINTR;
}
/*******************************************************************************
* Outline      : This function creates resource manager for SPI driver
* Function Name: spi_slave_create_instance
* Description  : Create pathname, map io function and resource manager.
* Arguments    : void * data
* Return Value : EOK: success
*                 -1: error
* Note         :
*******************************************************************************/
int spi_slave_create_instance (void * data)
{
    int coid = 0;
    short code = 0;
    spi_slave_t * dev = data;
    char  devname[NAME_MAX];
    /* Create dispatch layer */
    if (NULL == (dev->dpp = dispatch_create())) {
        perror("dispatch_create");
        goto fail;
    }

    /* set up i/o handler functions */
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,_RESMGR_IO_NFUNCS, &io_funcs);
    /* overwrite default handle with SPI driver handle function */
    io_funcs.devctl    = io_devctl;
    io_funcs.write     = io_write;
    io_funcs.read      = io_read;
    io_funcs.unblock   = io_unblock;
    iofunc_attr_init(&dev->attr, S_IFNAM | 0666, NULL, NULL);
    /* mount our own OCB to resource manager
     * with this OCB, we can pass our device to io function */
    dev->attr.mount    = &spi_mount;

    /* Set resource manager attribute */
    memset(&rattr, 0, sizeof (rattr));
    rattr.nparts_max   = SPI_RESMGR_NPARTS;
    rattr.msg_max_size = SPI_RESMGR_MSGSIZE;

    code = (short)pulse_attach(dev->dpp, MSG_FLAG_ALLOC_PULSE, 0, event_pulse_handler, dev);
    dev->spievent.sigev_code = code;
    if (code == -1) {
        fprintf(stderr,"Unable to attach pulse event.\n");
        goto fail1;
    }
    coid = message_connect(dev->dpp, MSG_FLAG_SIDE_CHANNEL);
    if (coid == -1) {
        fprintf(stderr,"Unable to attach channel and connection.\n");
        goto fail1;
    }
    dev->spievent.sigev_notify = SIGEV_PULSE;
    dev->spievent.sigev_coid = coid;
    dev->spievent.sigev_value.sival_ptr = NULL;
    /* register device name */
    snprintf(devname, NAME_MAX, "/dev/spi%d", dev->channel);
    if (-1 == (dev->res_id =  resmgr_attach(dev->dpp, &rattr, devname, _FTYPE_ANY, 0,
                    &connect_funcs, &io_funcs, &dev->attr))) {
        fprintf(stderr,"resmgr_attach failed:  %s.\n", strerror(errno));
        goto fail1;
    }
    resmgr_devino(dev->res_id, &dev->attr.mount->dev, &dev->attr.inode);
    dev->ctp = dispatch_context_alloc(dev->dpp);
    if(dev->ctp == NULL) {
        goto fail1;
    }
    procmgr_daemon( 0,PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL);
    /* Return if there is no error */
    return EOK;

fail1:
    dispatch_destroy(dev->dpp);
fail:
    return -1;
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
    if (_RESMGR_DEFAULT != status) {
        return status;
    }
    switch (msg->i.dcmd){
        case DCMD_SPI_SLAVE_BUS_RESET:
        {
            spi_lock(dev);
            status = spi_slave_rst_txrx(dev);
            spi_unlock(dev);
            if (EOK != status) {
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
            spi_lock(dev);
            status = spi_slave_enable_xchange(dev);
            spi_unlock(dev);
            if (EOK != status) {
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
            spi_lock(dev);
            status = spi_slave_disable_xchange(dev);
            spi_unlock(dev);
            if (EOK != status) {
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
            if (msg->o.nbytes < sizeof(spi_slave_cfg_t)) {
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

            if (msg->i.nbytes < sizeof(spi_slave_cfg_t)) {
                result = EINVAL;
                break;
            }

            cfg = (spi_slave_cfg_t *)_DEVCTL_DATA(msg->i);
            ret = BIT_LEN_VALIDATE(cfg->bitlen);
            if( (cfg->wordlen > dev->blksize) || (cfg->wordlen < 1) ) {
                result = EINVAL;
                break;
            }
            ret &= CPOL_VALIDATE(cfg->cpol);
            ret &= CPHA_VALIDATE(cfg->cpha);
            if (0 == ret) {
                result = EINVAL;
                break;
            }
            memcpy(&dev->cfg, cfg, sizeof(spi_slave_cfg_t));
            spi_lock(dev);
            status = spi_slave_config(dev);
            spi_unlock(dev);
            if (EOK != status) {
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
            if (msg->o.nbytes < sizeof(spi_data_t)) {
                result = EINVAL;
                break;
            }
            spi_lock(dev);
            data = (spi_data_t *)(_DEVCTL_DATA(msg->o));
            dev->rx_data.status = dev->status;
            if (dev->rx_buff.tail >= dev->rx_buff.head) {
                dev->rx_data.status |= SPI_STATUS_NO_DATA_AVAILABLE;
            } else {
                /* Get data and index from rx buffer */
                idx = dev->rx_buff.tail % dev->blksize;

                if (dev->cfg.bitlen == SPI_BIT_LEN_16){
                    dev->rx_data.byte = buf_16[idx];
                }
                else if (dev->cfg.bitlen == SPI_BIT_LEN_32){
                    dev->rx_data.byte = buf_32[idx];
                } else {
                    dev->rx_data.byte = buf_8[idx];
                }
                dev->rx_data.idx  = idx;
                dev->rx_buff.tail++;
            }

            /* Set data output of message */
            memcpy(data, &dev->rx_data, sizeof(spi_data_t));
            msg->o.nbytes = sizeof(spi_data_t);
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o) + sizeof(spi_data_t));
            spi_unlock(dev);
            break;
        }
        case DCMD_SPI_SLAVE_WRITE_BYTE:
        {
            if (msg->i.nbytes < sizeof(spi_data_t)) {
                result = EINVAL;
                break;
            }
            spi_lock(dev);
            data = _DEVCTL_DATA(msg->i);
            if (1 != spi_slave_write_fifo(dev, (void *)&data->byte, 1)) {
                result = -1;
                spi_unlock(dev);
                break;
            }
            spi_unlock(dev);
            /* Set data output of message */
            memset(&msg->o, 0, sizeof(msg->o));
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o));
            break;
        }
        case DCMD_SPI_SLAVE_GET_STATUS:
        {
            if (msg->o.nbytes < sizeof(uint32_t)) {
                result = EINVAL;
                break;
            }
            uint32_t *dev_status = (uint32_t *)(_DEVCTL_DATA(msg->o));
            *dev_status = dev->status;
            msg->o.nbytes = sizeof(uint32_t);
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o) + sizeof(uint32_t));
            break;
        }
        case DCMD_SPI_SLAVE_GET_FIFO_SIZE:
        {
            if (msg->o.nbytes < sizeof(uint32_t)) {
                result = EINVAL;
                break;
            }
            uint32_t *fifo_size = (uint32_t *)(_DEVCTL_DATA(msg->o));
            if (dev->product_id==PRODUCT_ID_RCAR_V4H) {
                *fifo_size = MSIOF_SPI_FIFO_SIZE_256;
            } else {
                *fifo_size = MSIOF_SPI_FIFO_SIZE_64;
            }

            msg->o.nbytes = sizeof(uint32_t);
            result = _RESMGR_PTR(ctp, msg, sizeof(msg->o) + sizeof(uint32_t));
            break;
        }
        default:
            result = ENOTTY;
            break;
    }
    return result;
}

int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
    int status = 0;
    int nbytes = 0;
    int word_number = 0;
    int	nonblock = 0;
    spi_slave_t * dev = (spi_slave_t *)ocb->attr;

    if ((status = iofunc_write_verify(ctp, msg, ocb, &nonblock)) != EOK) {
        return status;	
    }

    if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
        return ENOSYS;
    }
    /* Get number of bytes that client want to write */
    nbytes = _IO_WRITE_GET_NBYTES(msg);
    resmgr_msgread(ctp, dev->tx_buff, nbytes, sizeof(msg->i));

    if (nbytes <= 0) {
        goto failed;
    }

    /* Get number of transfer data */
    word_number = nbytes;
    if (dev->cfg.bitlen == SPI_BIT_LEN_16) {
            /* number of data (with datasize = 16bit) = number of bytes / 2 */
        if ((word_number % 2) != 0) {
            rcar_spi_slogf(dev, VERBOSE_QUIET,
                   "[Slog ERROR] %s: for 16bit datasize, read length must be a multiples of 2",__func__);
            goto failed;
        }
        word_number /= 2;
    } else if (dev->cfg.bitlen == SPI_BIT_LEN_32) {
        /* number of data (with datasize = 16bit) = number of bytes / 4 */
        if ((word_number % 4) != 0) {
        rcar_spi_slogf(dev, VERBOSE_QUIET,
            "[Slog ERROR] %s: for 32bit datasize, read length must be a multiples of 4",__func__);
            goto failed;
        }
        word_number /= 4;
    }

    if (word_number > dev->blksize){
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] %s: number of word data exceed > %d", __func__, dev->blksize);
        goto failed;
    }

    spi_unlock(dev);
    status = spi_slave_xfer(dev, word_number);
    spi_unlock(dev);
    if (status) {
        /* In case of SYNC ERR, we return number of read data */
        if (dev->status & SPI_STATUS_SYNC_ERR){
            /* Return number of read bytes */
            nbytes = dev->rx_buff.head * (dev->cfg.bitlen/SPI_BIT_LEN_8);
        } else {
            nbytes = 0;
        }
    }

    if (nbytes == 0)
        return (EAGAIN);

    ocb->attr->flags |= IOFUNC_ATTR_ATIME;
    /* set up the number of bytes (returned by client's write()) */
    _IO_SET_READ_NBYTES(ctp, nbytes);
    return (_RESMGR_NPARTS(0));

failed:
    return (EINVAL);
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
    int status;
    int nbytes;
    int nparts = 0;
    int word_number = 0;
    spi_slave_t * dev = (spi_slave_t *)ocb->attr;

    if ((status = iofunc_read_verify (ctp, msg, ocb, NULL)) != EOK) {
        return (status);
    }

    if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE){
        return (ENOSYS);
    }

    nbytes = _IO_READ_GET_NBYTES(msg);
    if(nbytes == 0) {
        _IO_SET_READ_NBYTES (ctp, 0);
        return _RESMGR_NPARTS(0);
    }

    dev->rcvid = ctp->rcvid;

    /* Get number of transfer data */
    word_number = nbytes;
    if (dev->cfg.bitlen == SPI_BIT_LEN_16) {
        if ((word_number % 2) != 0) {
            rcar_spi_slogf(dev, VERBOSE_QUIET,
                           "[Slog ERROR] %s: for 16bit datasize, read length must be a multiples of 2", __func__);
            goto failed;
        }
        word_number /= 2;
    } else if (dev->cfg.bitlen == SPI_BIT_LEN_32) {
        if ((word_number % 4) != 0){
            rcar_spi_slogf(dev, VERBOSE_QUIET,
                           "[Slog ERROR] %s: for 32bit datasize, read length must be a multiples of 4", __func__);
            goto failed;
        }
        word_number /= 4;
    }

    if (word_number > dev->blksize) {
        rcar_spi_slogf(dev, VERBOSE_QUIET, "[Slog ERROR] %s: number of data word > %d", __func__, dev->blksize);
        goto failed;
    }

    spi_lock(dev);
    /* All datas transmitted to master are dummy 0 */
    status = spi_slave_xfer(dev, word_number);
    spi_unlock(dev);
    if ((status == EOK) && (dev->status & SPI_STATUS_TRANSFER_DONE)) {
        nparts = 1;
        /* set up the return data IOV */
        SETIOV (ctp->iov, dev->rx_buff.data + ocb->offset, nbytes);
        /* mark the access time as invalid (we just accessed it) */
        ocb->attr->flags |= IOFUNC_ATTR_ATIME;
    } else if (dev->status & SPI_STATUS_SYNC_ERR) {
        /* Return number of read bytes */
        nbytes = dev->rx_buff.head * (dev->cfg.bitlen / SPI_BIT_LEN_8);
    } else {
        nbytes = 0;
    }

    _IO_SET_READ_NBYTES(ctp, nbytes);
    return (_RESMGR_NPARTS(nparts));

failed:

    return (EINVAL);
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

    if (NULL == (ocb = calloc(1, sizeof(iofunc_ocb_t))) ) {
        return NULL;
    }
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
