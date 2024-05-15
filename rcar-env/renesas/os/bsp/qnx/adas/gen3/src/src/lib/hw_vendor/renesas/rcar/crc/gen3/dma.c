/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation
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

#include "rcar_crc_common.h"
#include "rcar_wcrc.h"
#include <errno.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <unistd.h>
#include <string.h>
#include <aarch64/r-car-gen3.h>

/***********************************************
 *	MACROS/DEFINES
 ***********************************************/

/***********************************************
 *	GLOBAL VARIABLE
 ***********************************************/

extern int rcar_crc_wait(CRC_DEVICE_Type *dev, int len);

/***********************************************
 *	APIs IMPLEMENATION
 ************************************************/

/*!
 *    @brief        Initialize DMA for CRC 
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int rcar_dma_init(CRC_DEVICE_Type* dev)
{
    struct sigevent  event;

    switch (dev->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_CRC0_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_CRC0_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC0; 
            break;
        case RCAR_GEN3_CRC1_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_CRC1_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_CRC1_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC1;
            break;
        case RCAR_GEN3_CRC2_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_CRC2_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_CRC2_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC2;
            break;
        case RCAR_GEN3_CRC3_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_CRC3_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_CRC3_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC3;
            break;
        }

    switch (dev->kcrc_base) {
        case RCAR_GEN3_KCRC0_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_KCRC0_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_KCRC0_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC0;
            break;
        case RCAR_GEN3_KCRC1_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_KCRC1_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_KCRC1_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC1;
            break;
        case RCAR_GEN3_KCRC2_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_KCRC2_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_KCRC2_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC2;
            break;
        case RCAR_GEN3_KCRC3_BASE:
            dev->tx_mid_rid = RCAR_GEN3_DREQ_KCRC3_TXI;
            dev->rx_mid_rid = RCAR_GEN3_DREQ_KCRC3_RXI;
            dev->irq_crc =  RCAR_V3U_INTCSYS_WCRC3;
            break;

        }


    if ((dev->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1) {
        fprintf(stderr, "%s: CRC ChannelCreate failed\n", __func__);
        return -1;
    }

    if ((dev->coid = ConnectAttach(0, 0, dev->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
        fprintf(stderr, "%s: CRC ConnectAttach failed\n", __func__);
        ChannelDestroy(dev->chid); 
        return -1;
    }

    dev->crcevent.sigev_notify   = SIGEV_PULSE;
    dev->crcevent.sigev_coid     = dev->coid;
    dev->crcevent.sigev_code     = CRC_EVENT;
    dev->crcevent.sigev_priority = CRC_PRIORITY;

    /*
     * Attach CRC interrupt
     */
    dev->iid_crc = InterruptAttachEvent(dev->irq_crc, &dev->crcevent, _NTO_INTR_FLAGS_TRK_MSK);
    if (dev->iid_crc < 0) {
        fprintf(stderr, "%s: CRC Interrupt attachment failed\n", __func__);
        ConnectDetach(dev->coid);
        ChannelDestroy(dev->chid);
        return -1;
    } 

    if (get_dmafuncs(&dev->dmafuncs, sizeof(dev->dmafuncs)) == -1) {
        fprintf(stderr, "%s: Failed to get DMA lib functions\n", __func__);
        return (-1);
    }

    dev->dmafuncs.init(NULL);

    event.sigev_notify   = SIGEV_PULSE;
    event.sigev_coid     = dev->coid;
    event.sigev_code     = CRC_TXDMA_EVENT;
    event.sigev_priority = CRC_PRIORITY;

    if ((dev->tx_dma = dev->dmafuncs.channel_attach("dma=rt", &event, NULL, 0,
                                    DMA_ATTACH_PRIORITY_STRICT | DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE)) == NULL) {
        fprintf(stderr, "%s: Unable to attach to DMA Channel\n", __func__);
        return (-1);
    }
    
    event.sigev_notify   = SIGEV_PULSE;
    event.sigev_coid     = dev->coid;
    event.sigev_code     = CRC_RXDMA_EVENT;
    event.sigev_priority = CRC_PRIORITY;

    if ((dev->rx_dma = dev->dmafuncs.channel_attach("dma=rt", &event, NULL, 1,
                                    DMA_ATTACH_PRIORITY_STRICT | DMA_ATTACH_ANY_CHANNEL | DMA_ATTACH_EVENT_ON_COMPLETE)) == NULL) {
        fprintf(stderr, "Unable to attach to DMA Channel\n");
        dev->dmafuncs.channel_release(dev->tx_dma);
        return (-1);
    }

    if (dev->dmafuncs.alloc_buffer(dev->rx_dma, &dev->pdmabuf, 4, DMA_BUF_FLAG_NOCACHE) != 0) {
        fprintf(stderr, "Unable to allocate DMA buffer");
        dev->dmafuncs.channel_release(dev->tx_dma);
        dev->dmafuncs.channel_release(dev->rx_dma);
        return (-1);
    }

    return (EOK);
}

/*!
 *    @brief       Setup and start DMA transfers include read/write from FIFO port 
 *    @param[in]   (void *hdl (CRC_DEVICE_Type* dev)
 *    @param[in]    bool device_dir
 *    @param[in]    uint32_t fifo_port_addr
 *    @param[inout] void* paddr (in/out depend on device_dir)
 *    @param[in]    int len_done
 *    @param[in]    int dlen
 *    @param[in]    int len
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int rcar_crc_dma_xfer(void *hdl, bool device_dir, uint32_t fifo_port_addr, void* paddr, int len_done, int dlen, int len)
{
    CRC_DEVICE_Type*      dev = hdl;
    dma_transfer_t tinfo;
    dma_addr_t     saddr, daddr;
    int            ret = 0;

    if (device_dir == DEVICE_IN) { // Write to FIFO port
        /* TX setup */
        memset(&tinfo, 0, sizeof (tinfo));
        tinfo.xfer_unit_size = 4;
        if (len == 1) {
            tinfo.xfer_unit_size = 1;
            tinfo.xfer_bytes     = 4;
        } else  if (len == 2) {
            tinfo.xfer_unit_size = 2;
            tinfo.xfer_bytes     = 8;
        } else if (len == 3) {
            tinfo.xfer_unit_size = 1;
            tinfo.xfer_bytes     = 16;
        }else {
            tinfo.xfer_bytes     = 16*(len/4);
        }
        tinfo.req_id         = dev->tx_mid_rid;

        if (fifo_port_addr == dev->wcrc_fifo_command_port) {
           tinfo.xfer_bytes     = 4;
        }     

        saddr.paddr          =  (off64_t)paddr; // PHY address of data    
        tinfo.src_addrs      = &saddr;
        daddr.paddr          = fifo_port_addr;
        tinfo.dst_addrs      = &daddr;
        tinfo.dst_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;

        dev->dmafuncs.setup_xfer(dev->tx_dma, &tinfo);
        /* Now start DMA */
        dev->dmafuncs.xfer_start(dev->tx_dma);
        /* Wait for tx DMA completion */
       ret = rcar_crc_wait(dev, len);
       if (ret) {
           goto fail;
       }
    } else { // Read from FIFO port
        memset(&tinfo, 0, sizeof (tinfo));
        tinfo.xfer_unit_size = 4;;
        tinfo.xfer_bytes     = 4;;
        tinfo.req_id         = dev->rx_mid_rid;

        saddr.paddr          = fifo_port_addr;
        tinfo.src_addrs      = &saddr;
        tinfo.src_flags      = DMA_ADDR_FLAG_NO_INCREMENT | DMA_ADDR_FLAG_DEVICE;
        daddr.paddr          = dev->pdmabuf.paddr;
        tinfo.dst_addrs      = &daddr;
        dev->dmafuncs.setup_xfer(dev->rx_dma, &tinfo);
        /* Now start DMA */
        dev->dmafuncs.xfer_start(dev->rx_dma);
        /* Wait for rx DMA completion */
        ret = rcar_crc_wait(dev, len);
        if (ret)
           goto fail;
    }

   return len;

fail:

    if (device_dir == DEVICE_IN) {
       dev->dmafuncs.xfer_abort(dev->tx_dma);
    } else {
        dev->dmafuncs.xfer_abort(dev->rx_dma);
    }

    return ret;
}
