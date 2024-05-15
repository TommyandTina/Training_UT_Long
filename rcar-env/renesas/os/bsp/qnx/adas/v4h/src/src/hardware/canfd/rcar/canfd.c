/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#include "canfd.h"
/***************************** Private functions ******************************/

static void canfd_send_msg(CANDEV  *cdev,canmsg_t *canmsg);
static void setting_channel_interrupt(candev_info_t *candev_info);
static void setting_global_interrupt(candev_info_t *candev_info);
static void setting_buffer(candev_info_t *candev_info);
static void setting_receive_rules(candev_info_t *candev_info);
static void setting_baudrate(candev_info_t *candev_info);
/************************ End of private functions *****************************/

/*******************************************************************************
* Outline      : Handling a transmit interrupt
* Function Name: canfd_tx_irq
* Description  : Checking interrupt error flags then hanling them or transmiting
*                next message if it is existing in FIFO
* Arguments    : void *area
*                int id
* Return Value : sigevent
* Note         :
*******************************************************************************/
const struct sigevent *canfd_tx_irq(void *area, int id)
{
    struct sigevent            *event = NULL;
    candev_exp_t               *candev_list = (candev_exp_t *)area;
    candev_info_t              *candev_info = candev_list->candev_info;
    canmsg_list_t              *msglist = NULL;
    canmsg_t                   *nextmsg = NULL;
    uint32_t                   val = 0;
    uint32_t                   k = 0;
    uint32_t                   mbxid = 0;
    uint32_t                   channel = 0;
    uint32_t                   tx_fifo_index = 0;
    uint32_t                   i = 0;
    uint32_t                   total_fifo = CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER + CANFD_RECEIVE_FIFO_BUFFER;

    for (i = 0; i < (CANFD_TOTAL_CHANNELS * CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER); i++)
    {
        /* Determine channel and TX index of channel */
        channel = (uint32_t)(i / CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER);
        tx_fifo_index = i % CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER;

        candev_info->devctl_stat[channel].total_interrupts++;
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmERFL(channel));
        if (0x7FFF & val)
        {
           /* Clear error flags */
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmERFL(channel), 0x7FFF & val); 
            atomic_set(&candev_info->canfd_stat[channel], 0x7FFF & val);
            if (EWF & val)
            {
                candev_info->devctl_stat[channel].error_warning_state_count++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (EPF & val)
            {
                candev_info->devctl_stat[channel].error_passive_state_count++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (BOEF & val)
            {
                candev_info->devctl_stat[channel].bus_off_state_count++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (SERR & val)
            {
                candev_info->devctl_stat[channel].stuff_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (FERR & val)
            {
                candev_info->devctl_stat[channel].form_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (AERR & val)
            {
                candev_info->devctl_stat[channel].missing_ack++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            } 
            if (CERR & val)
            {
                candev_info->devctl_stat[channel].crc_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            } 
            if (B1ERR & val)
            {
                candev_info->devctl_stat[channel].recess_bit_dom_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (B0ERR & val)
            {
                candev_info->devctl_stat[channel].dom_bit_recess_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
        }

        /* Clear interrupt flags */
        k = tx_fifo_index + (channel * 3);
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFSTS(k));
        if (CFTXIF & val)
        {
            val &= ~CFTXIF;
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFSTS(k), val);
        }
        else
        {
            continue;
        }

        /* Determine candev_list index of corresponding TX FIFO(x) */
        mbxid = (CANFD_RECEIVE_FIFO_BUFFER + tx_fifo_index) + (total_fifo * channel);

        msglist = candev_list[mbxid].candev.msg_queue;

        candev_info->devctl_stat[channel].transmitted_frames++;

        /* Transmit next message if there is existing messages */
        nextmsg = canmsg_dequeue_element(msglist);
        if (nextmsg)
        {
            /* Transmit next message */
            canfd_send_msg((CANDEV *)&candev_list[mbxid], nextmsg);
            /* Free queue */
            canmsg_queue_element(candev_list[mbxid].candev.free_queue, nextmsg);
        }
        else
        {
            /* No more transmit messages, end transmission */
            atomic_clr(&candev_list[mbxid].dflags, CANDEV_TX_ENABLED);
        }

        /* Block clients if need */
        if (candev_list[mbxid].candev.wait_client_queue->cnt)
        {
            event = &candev_list[mbxid].candev.event;
        }

        if (event)
        {
            break;
        }
    }

    return (event);
}

/*******************************************************************************
* Outline      : CAN Interrupt Handler Globle
* Function Name: canfd_rx_irq
* Description  : Checking interrupt error flags then hanling them and receive new
*                messages.
* Arguments    : void *area
*                int id
* Return Value : sigevent
* Note         :
*******************************************************************************/
const struct sigevent *canfd_rx_irq(void *area, int id)
{
    struct sigevent            *event = NULL;
    candev_exp_t               *candev_list = area;
    candev_info_t              *candev_info = candev_list->candev_info;
    candev_init_t              *candev_init = candev_info->candev_init;
    canmsg_list_t              *msglist = NULL;
    canmsg_t                   *rxmsg = NULL;
    uint32_t                   val = 0;
    uint32_t                   reg = 0;
    uint32_t                   i = 0;
    uint32_t                   channel = 0;
    uint32_t                   mbxid = 0;
    uint32_t                   tx_fifo_index = 0;
    uint32_t                   rx_fifo_index = 0;
    uint32_t                   rx_index = 0;
    uint32_t                   total_fifo = CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER + CANFD_RECEIVE_FIFO_BUFFER;

    for (rx_fifo_index = 0; rx_fifo_index < (CANFD_TOTAL_CHANNELS * CANFD_RECEIVE_FIFO_BUFFER); rx_fifo_index++)
    {
        /* Determine channel and RX index of channel */
        channel  = (uint32_t)(rx_fifo_index / CANFD_RECEIVE_FIFO_BUFFER);
        rx_index = rx_fifo_index % CANFD_RECEIVE_FIFO_BUFFER;
        candev_info->devctl_stat[channel].total_interrupts++;
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmERFL(channel));
        if (0x7FFF & val)
        {
            /* Clear error flags */
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmERFL(channel), 0x7FFF & val); 
            atomic_set(&candev_info->canfd_stat[channel], 0x7FFF & val);
            if (EWF & val)
            {
                candev_info->devctl_stat[channel].error_warning_state_count++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (EPF & val)
            {
                candev_info->devctl_stat[channel].error_passive_state_count++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (BOEF & val)
            {
                candev_info->devctl_stat[channel].bus_off_state_count++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (SERR & val)
            {
                candev_info->devctl_stat[channel].stuff_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (FERR & val)
            {
                candev_info->devctl_stat[channel].form_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (AERR & val)
            {
                candev_info->devctl_stat[channel].missing_ack++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            } 
            if (CERR & val)
            {
                candev_info->devctl_stat[channel].crc_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            } 
            if (B1ERR & val)
            {
                candev_info->devctl_stat[channel].recess_bit_dom_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
            if (B0ERR & val)
            {
                candev_info->devctl_stat[channel].dom_bit_recess_errors++;
                candev_info->devctl_stat[channel].total_frame_errors++;
            }
        }

        /* Clear all global error interrupts */
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGERFL);
        if (CANFD_GERFL_ERR(candev_init) & val)
        {
            if (MES & val)
            {
                /* Clear RFMLT bit in RSCFD0CFDRFSTSx */
                for (i = 0; i < (CANFD_RECEIVE_FIFO_BUFFER * CANFD_TOTAL_CHANNELS); i++)
                {
                    reg = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFSTS(i));
                    if (RFMLT & reg)
                    {
                        reg &= ~RFMLT;
                        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFSTS(i),reg);
                    }
                }

                /* Clear bit CFMLT in RSCFDnCFDCFSTSk */
                for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
                {
                    for (i = 0; i < CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER; i++)
                    {
                        tx_fifo_index = (3 * channel) + i;
                        reg = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFSTS(tx_fifo_index));
                        if (CFMLT & reg)
                        {
                            reg &= ~CFMLT;
                            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFSTS(tx_fifo_index), reg);
                        }
                    }
                }
            }
            /* THLELT bit in the RSCFDnCFDTHLSTSm */
            else if (THLES & val)
            {
                for (i = 0; i < CANFD_TOTAL_CHANNELS; i++)
                {
                    reg = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDTHLSTS(i));
                    if (THLELT & reg)
                    {
                        reg &= ~THLELT;
                        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDTHLSTS(i), reg);
                    }
                }
            }

            /* Clear DEF, CMPOF, EEF0~7 bits */
            val &= ~DEF;
            val &= ~CMPOF;
            val &= ~(EEF0 | EEF1 | EEF2 | EEF3 | EEF4 | EEF5 | EEF6 | EEF7);
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGERFL,val);
            break;
        }

        /* Clear interrupt bit */
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFSTS(rx_fifo_index));
        if (RFIF & val)
        {
            val &= ~RFIF;
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFSTS(rx_fifo_index), val);
        }

        /* check buffer empty */
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFSTS(rx_fifo_index));
        if (RFEMP & val)
        {
            continue;
        }

        /* Determine candev_list index of corresponding RX FIFO(x) */
        mbxid = rx_index + (channel * total_fifo);
        msglist = candev_list[mbxid].candev.free_queue;
        rxmsg = canmsg_dequeue_element(msglist);
        if (!rxmsg)
        {
            rxmsg = canmsg_dequeue_element(candev_list[mbxid].candev.msg_queue);
            if (!rxmsg) {
                break;
            }
        }

        /* Receive FIFO interrupt is disabled */
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFCC(rx_fifo_index));
        if (RFIE & val)
        {
            val &= ~RFIE;
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFCC(rx_fifo_index), val);
        }

        /*mid*/
        if (CLASSIC_CAN_MODE != candev_init->interface)
        {
            /* CANFD interface */
            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFID(rx_fifo_index));
        }
        else
        {
            /* Classic CAN interface */
            if (IS_RCAR_GEN4(product_id))
                val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFID(rx_fifo_index));
            else
                val = in32(candev_info->base + RCAR_CAN_RSCFD0RFID(rx_fifo_index));
        }
        /* Store message ID */
        rxmsg->cmsg.mid = RFID(val);
        /* Check RTR bit */
        if (!(RFRTR & val))
        {
            /* Get DLC */
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFPTR(rx_fifo_index));
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFPTR(rx_fifo_index));
                else
                    val = in32(candev_info->base + RCAR_CAN_RSCFD0RFPTR(rx_fifo_index));
            }
            rxmsg->cmsg.len = RFDLC(val);

            /* Read timestamp */
            rxmsg->cmsg.ext.timestamp = val & RFTS;

            /* Get Data */
            for (i = 0; i < rxmsg->cmsg.len; i+=sizeof(uint32_t))
            {
                if (CLASSIC_CAN_MODE != candev_init->interface)
                {
                    /* CANFD interface */
                    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFDF(i>>2, rx_fifo_index));
                }
                else
                {
                    /* Classic CAN interface */
                    if (IS_RCAR_GEN4(product_id))
                        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFDF(i>>2, rx_fifo_index));
                    else 
                    {
                        if (0 < i)
                        {
                            val = in32(candev_info->base + RCAR_CAN_RSCFD0RFDF1(rx_fifo_index));
                        }
                        else
                        {
                            val = in32(candev_info->base + RCAR_CAN_RSCFD0RFDF0(rx_fifo_index));
                        }
                    }
                }
                memcpy(&rxmsg->cmsg.dat[i], &val, sizeof(uint32_t));
            }
        }
        /* The read pointer moves to the next unread message in
           the receive FIFO buffer */
        val = RFPC(0xFF);
        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFPCTR(rx_fifo_index), val);

        /* Add populated element to the receive queue */
        canmsg_queue_element(candev_list[mbxid].candev.msg_queue, rxmsg);

        if (candev_list[mbxid].candev.wait_client_queue->cnt)
        {
            event = &candev_list[mbxid].candev.event;
        }

        candev_info->devctl_stat[channel].received_frames++;

        /* Re-enable Rx FIFO interrupts */
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFCC(rx_fifo_index));
        val |= RFIE;
        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFCC(rx_fifo_index), val);

        if (event)
        {
            break;
        }
    }

    return (event);
}

/*******************************************************************************
* Outline      : Re-configuring orget mid, ide and filter
* Function Name: can_control_cb
* Description  : This function re-configure or get mid, ide and filter
* Arguments    : CANDEV *cdev
*                int dcmd
*                DCMD_DATA *data
* Return Value : EOK
* Note         :
*******************************************************************************/
int canfd_control_cb(CANDEV *cdev, int dcmd, DCMD_DATA *data)
{
    candev_exp_t           *candev_exp = (candev_exp_t *)cdev;
    candev_info_t          *candev_info = candev_exp->candev_info;
    candev_init_t          *candev_init = candev_info->candev_init;
    uint32_t               val = 0;
    uint8_t                channel = 0;
    uint8_t                fifo_index = 0;
    uint8_t                index = 0;

    /* Get channel and tx/rx index */
    channel = candev_exp->channel;
    index = cdev->dev_unit;

    /* Select Receive Rule j index corresponding to RX FIFO(x)
     * or select TX FIFO(k) depending on device type (RX or TX) */
    fifo_index = (CANDEV_TYPE_RX == cdev->devtype) ? (index + (CANFD_RECEIVE_FIFO_BUFFER * channel))
                                                   : (index + (3 * channel));

    /* Handling command */
    switch (dcmd)
    {
        case CAN_DEVCTL_SET_MID:
            switch (cdev->devtype)
            {
                case CANDEV_TYPE_RX: /* rx */
                    /* Enable writing data to the receive rule table */
                    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR);
                    val |= AFLDAE;
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR,val);
                    candev_init->channel[channel].rxid[index] = data->mid;
                    val = GAFLID(data->mid);                       /* Set rx ID */
                    if (MAX_STANDARD_ID < val)
                    {
                        val |= GAFLIDE;                                 /* Use IDE bit */
                    }
                    else
                    {
                        val &= ~GAFLIDE;                                /* No IDE bit */
                    }
                    val &= ~GAFLLB;                                     /* When a message transmitted from another CAN node is received */
                    val &= ~GAFLRTR;                                    /* Data frame */

                    if (CLASSIC_CAN_MODE != candev_init->interface)
                    {
                        /* CANFD interface */
                        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLID(fifo_index), val);
                    }
                    else
                    {
                        /* Classic CAN interface */
                        if (IS_RCAR_GEN4(product_id))
                            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLID(fifo_index), val);
                        else
                            out32(candev_info->base + RCAR_CAN_RSCFD0GAFLID(fifo_index), val);
                    }

                    /* Disable writing data to the receive rule table */
                    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR);
                    val &= ~AFLDAE;
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR, val);
                break;

                case CANDEV_TYPE_TX: /* tx */
                    candev_init->channel[channel].txid[index] = data->mid;
                break;

                default:
                    /* Do nothing */
                break;
            }
        break;

        case CAN_DEVCTL_GET_MID:
            switch (cdev->devtype)
            {
                case CANDEV_TYPE_RX: /* rx */
                    if (CLASSIC_CAN_MODE != candev_init->interface)
                    {
                        /* CANFD interface */
                        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLID(fifo_index));
                    }
                    else
                    {
                        /* Classic CAN interface */
                        if (IS_RCAR_GEN4(product_id))
                            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLID(fifo_index));
                        else
                            val = in32(candev_info->base + RCAR_CAN_RSCFD0GAFLID(fifo_index));
                    }
                    data->mid = GAFLID(val);
                break;

                case CANDEV_TYPE_TX: /* tx */
                    data->mid = candev_init->channel[channel].txid[index];
                break;

                default:
                    /* Do nothing */
                break;
            }
        break;

        case CAN_DEVCTL_SET_MFILTER:
            /* Enable writing data to the receive rule table */
            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR);
            val |= AFLDAE;
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR, val);

            /* Set receive Rule Mask Register */
            val = data->mfilter;
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLM(fifo_index), val);
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLM(fifo_index), val);
                else
                    out32(candev_info->base + RCAR_CAN_RSCFD0GAFLM(fifo_index), val);
            }
            /* Disable writing data to the receive rule table */
            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR);
            val &= ~AFLDAE;
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR, val);
        break;

        case CAN_DEVCTL_GET_MFILTER:
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLM(fifo_index));
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLM(fifo_index));
                else
                    val = in32(candev_info->base + RCAR_CAN_RSCFD0GAFLM(fifo_index));
            }
            data->mfilter = GAFLIDM(val);
        break;

        case CAN_DEVCTL_GET_TIMESTAMP:
            val = in32(candev_info->base +  RCAR_CANFD_RSCFD0CFDGTSC);
            data->timestamp = TS(val);
        break;

        case CAN_DEVCTL_SET_TIMESTAMP:
            if(data->timestamp == 0)
            {
                val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR);
                val |= TSRST;
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR, val);
            }
            else
            {
                LOG_ERROR("Invalid timestamp value, RCAN only support reset timestamp value by write 0\n");
            }
        break;

        case CAN_DEVCTL_READ_CANMSG_EXT:
        case CAN_DEVCTL_WRITE_CANMSG_EXT:
            break;

        case CAN_DEVCTL_GET_STATS:
            memcpy(&data->stats, &candev_info->devctl_stat[channel], sizeof(CAN_DEVCTL_STATS));
            break;

        case CAN_DEVCTL_ERROR:
            // Read Channel n Error Flag Registers
            data->error.drvr1 = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmERFL(channel));
            data->error.drvr1 &= 0x7FFF;
            // Read and clear devctl info
            data->error.drvr2 = atomic_clr_value(&candev_info->canfd_stat[channel], 0xffffffff);
            // Read Reception Error Count
            data->error.drvr3 = (REC(in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmSTS(channel))) >> 16);
            // Read Transmission Error Count
            data->error.drvr4 = (TEC(in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmSTS(channel))) >> 24);

            break;
#if _NTO_VERSION <= 700
        case CAN_DEVCTL_ENABLE_CLASSICAL_CAN_FRAME:
            candev_init->channel[channel].frameType = CLASSICAL_CAN_FRAME;              /* Classical frame */
        break;

        case CAN_DEVCTL_DISABLE_CLASSICAL_CAN_FRAME:
            candev_init->channel[channel].frameType = CANFD_FRAME;                      /* CANFD frame */
        break;
#endif

        default:
            /* Driver does not support this DEVCTL */
            LOG_ERROR("Not support yet\n");
            return (ENOTSUP);
        break;
    }

    return (EOK);
}

/*******************************************************************************
* Outline      : Setting baudrate at initializing CAN module
* Function Name: setting_baudrate
* Description  : This function will set normal bit timing and data bit timing
*                as well as set internal timer prescaler, time stamp or select
*                clock source.
* Arguments    : candev_info_t
* Return Value : none
* Note         :
*******************************************************************************/
static void setting_baudrate(candev_info_t *candev_info)
{
    candev_init_t   *candev_init = candev_info->candev_init;
    uint32_t        val = 0;
    uint32_t        M = 0;
    float           temp = 0.0f;
    uint32_t        t = CANFD_DEFAULT_TX_INTERVAL;

    if (IS_RCAR_GEN4(product_id))
    {
        // RCAR_CANFD_RSCFD0CFDGCFG.CFITR = 0 and RCAR_CANFD_RSCFD0CFDGCFG.CFITSS = 0
        temp = (float)((t * pclk)/(N));               /* R-CarV4H/R-CarV3U: Interval time = (M x N) / pclk */
    }
    else
    {
        // RCAR_CANFD_RSCFD0CFDGCFG.CFITR = 0 and RCAR_CANFD_RSCFD0CFDGCFG.CFITSS = 0
        temp = (float)((t * pclk)/(2 * N));           /* R-CarGen3: Interval time = (2 x M x N) / pclk */
    }

    M = (uint32_t)roundf(temp);
    val  = ITRCP(M);                                  /* Interval Timer Prescaler Set */
    val |= TSBTCS(0);                                 /* Timestamp Clock Source Select */
    val &= ~TSSS;                                     /* Timestamp Source Select (R-CarGen3: pclk/2, R-CarGen4: pclk) */
    val |= TSP(3);                                    /* Timestamp Clock Source Division */
    val |= EEFE;                                      /* ECC Error Flag Enable */
    if (CLASSIC_CAN_MODE != candev_init->interface)
        val |= CMPOC;                                 /* Payload Overflow Mode Select,
                                                         Messages are stored and the payload which exceeds
                                                         the buffer size is truncated.*/
    if (EXTERNAL_CLOCK_MODE == candev_init->clks)     /* CAN Clock Source Select clkc */
        val |= DCS;                                   /* External clock */
    else
        val &= ~DCS;                                  /* Internal clock */

    val &= ~MME;                                      /* Mirror function is disabled. */
    val &= ~DRE;                                      /* DLC replacement is disabled. */
    val &= ~DCE;                                      /* DLC Check Disable */
    val &= ~TPRI;                                     /* Transmit Priority Select ID priority */
    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCFG, val);

    if (CLASSIC_CAN_MODE != candev_init->interface)
    {
        /* CANFD interface */
        /* Normal bit timing */
        candev_init->nbrp -= 1;
        candev_init->nsjw -= 1;
        candev_init->ntseg1 -= 1;
        candev_init->ntseg2 -= 1;
        val = NBRP(candev_init->nbrp);
        val |= NSJW(candev_init->nsjw);
        val |= NTSEG1(candev_init->ntseg1);
        val |= NTSEG2(candev_init->ntseg2);
        for (int channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++) {
            out32(candev_info->base + RCAR_CANFD_RSCFD0CmNCFG(channel), val);
        }

        /* Data bit timing */
        candev_init->dbrp -= 1;
        candev_init->dsjw -= 1;
        candev_init->dtseg1 -= 1;
        candev_init->dtseg2 -= 1;
        val = DBRP(candev_init->dbrp);
        val |= DSJW(candev_init->dsjw);
        val |= DTSEG1(candev_init->dtseg1);
        val |= DTSEG2(candev_init->dtseg2);
        for (int channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++) {
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmDCFG(channel), val);
        }

    }
    else
    {
        /* Classic CAN interface */
        candev_init->brp -= 1;
        candev_init->sjw -= 1;
        candev_init->tseg1 -= 1;
        candev_init->tseg2 -= 1;
        if (IS_RCAR_GEN4(product_id)) {
            val = NBRP(candev_init->brp);
            val |= NSJW(candev_init->sjw);
            val |= NTSEG1(candev_init->tseg1);
            val |= NTSEG2(candev_init->tseg2); 
        }
        else {
            val = BRP(candev_init->brp);
            val |= CSJW(candev_init->sjw);
            val |= TSEG1(candev_init->tseg1);
            val |= TSEG2(candev_init->tseg2);
        }
        for (int channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++) {
            out32(candev_info->base + RCAR_CANFD_RSCFD0CmNCFG(channel), val);
        }
    }
}

/*******************************************************************************
* Outline      : Configuring receive rules
* Function Name: setting_receive_rules
* Description  : This function uses to configure receive rules. One receive
*                rule to support each channel only at here.
* Arguments    : candev_info_t
* Return Value : none
* Note         :
*******************************************************************************/
static void setting_receive_rules(candev_info_t *candev_info)
{
    candev_init_t      *candev_init = candev_info->candev_init;
    uint8_t            channel = 0;
    uint32_t           val = 0;
    uint32_t           rxid = 0;
    uint32_t           rx_fifo_index = 0;
    uint32_t           i = 0;

    /* Enable writing data to the receive rule table */
    val = AFLDAE;
    val &= ~AFLPN(0x1F);     /* Receive Rule Table Page Number: 0 */
    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR, val);

    /* Set number of rule receive */
    val  = RNC(CHANNEL_0, CANFD_RECEIVE_FIFO_BUFFER);        /* only one rule per channel */
    val |= RNC(CHANNEL_1,CANFD_RECEIVE_FIFO_BUFFER);        /* only one rule per channel */
    if (product_id == PRODUCT_ID_RCAR_V3H && product_rev == PRODUCT_REV_2_0)
        val |= RNC(CHANNEL_2,CANFD_RECEIVE_FIFO_BUFFER);    /* 1 rule for channel 2 in V3H20*/ 
    for (channel = 0; channel < (CANFD_TOTAL_CHANNELS -1) ; channel += 2) {
        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLCFG0(channel), val);
    }

    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        for (i = 0; i < CANFD_RECEIVE_FIFO_BUFFER; i++)
        {
            /* Determine Receive Rule j and RX ID corresponding to RX FIFO(x) */
            rx_fifo_index = i + (CANFD_RECEIVE_FIFO_BUFFER * channel);
            rxid = candev_init->channel[channel].rxid[i];

            /* Receive Rule ID Register */
            val = GAFLID(rxid);                                /* Set rx ID */
            if (MAX_STANDARD_ID < val)
            {
                val |= GAFLIDE;                                /* Use IDE bit */
            }
            else
            {
                val &= ~GAFLIDE;                                /* No IDE bit */
            }
            val &= ~GAFLLB;                                     /* When a message transmitted from another CAN node is received */
            val &= ~GAFLRTR;                                    /* Data frame */
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLID(rx_fifo_index), val);
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLID(rx_fifo_index), val);
                else
                    out32(candev_info->base + RCAR_CAN_RSCFD0GAFLID(rx_fifo_index), val);
            }

            /* Receive Rule Mask Register */
            val = GAFLIDM(0x1FFFFFFF);                        /* Enable to compare message ID */
            val &= ~GAFLRTRM;                                  /* Not check RTR bit */
            val &= ~GAFLIDEM;                                  /* Not check IDE bit */
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLM(rx_fifo_index), val);
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLM(rx_fifo_index), val);
                else
                    out32(candev_info->base + RCAR_CAN_RSCFD0GAFLM(rx_fifo_index), val);
            }

            /* Receive Rule Pointer 0 Register */
            val = GAFLRMDP(0);                                   /* Set the receive buffer number to store receive messages. */
            val &= ~GAFLDLC(0x0F);                               /* Disables the DLC check function allowing messages with
                                                                any data length to pass the DLC check. */
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLP0(rx_fifo_index), val);
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLP0(rx_fifo_index), val);
                else
                    out32(candev_info->base + RCAR_CAN_RSCFD0GAFLP0(rx_fifo_index), val);
            }

            /* Receive Rule Pointer 1 Register */
            val = GAFLFDP0(1 << rx_fifo_index);                        /* receive FIFO buffer is selected. */
            val |= GAFLFDP1(0);                                   /* Transmit/receive FIFO buffer is not selected. */
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLP1(rx_fifo_index), val);
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLP1(rx_fifo_index), val);
                else
                    out32(candev_info->base + RCAR_CAN_RSCFD0GAFLP1(rx_fifo_index), val);
            }
        }
    }

    /* Disable writing data to the receive rule table */
    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR);
    val &= ~AFLDAE;
    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGAFLECTR, val);
}

/*******************************************************************************
* Outline      : Configuring receive FIFO buffer and transmit/receive FIFO buffer
* Function Name: setting_buffer
* Description  : This function uses to configuring receive FIFO buffer and transmit/receive
*                FIFO buffer.
*                For receive FIFO buffer: Interrupt if FIFO is 1/8 full, FIFO Buffer Depth
*                                         is 8 messages FIFO Buffer Payload Storing Size is
*                                         64 bytes.
*                For transmit/receive FIFO buffer: Interrupt if FIFO is 1/8 full, Transmit/Receive
*                                         FIFO Buffer Depth is 8 messages, FIFO Buffer Payload Storing
*                                         Size is 64 bytes.
* Arguments    : candev_info_t
* Return Value : none
* Note         :
*******************************************************************************/
static void setting_buffer(candev_info_t *candev_info)
{
    uint32_t           val = 0;
    uint32_t           tx_fifo_index = 0;
    uint32_t           rx_fifo_index = 0;
    uint32_t           channel = 0;
    uint32_t           i = 0;

    /* Receive FIFO buffer */
    val = RFIGCV(0);            /* Receive FIFO Interrupt Request Timing Select: When FIFO is 1/8 full. */
    val |= RFIM;                /* An interrupt occurs each time a message has been received. */
    val |= RFDC(0x02);          /* Receive FIFO Buffer Depth Configuration: 8 messages */
    if (candev_info->candev_init->interface != CLASSIC_CAN_MODE)
        val |= RFPLS(0x07);     /* Receive FIFO Buffer Payload Storing Size Select: 64 bytes */
    else
        val |= RFPLS(0x00);     /* Receive FIFO Buffer Payload Storing Size Select: 8 bytes */

    val |= RFIE;                /* Receive FIFO interrupt is enabled. */
    val &= ~RFE;                /* Receive FIFO buffers are unused. */
    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        for (i = 0; i < CANFD_RECEIVE_FIFO_BUFFER; i++)
        {
            /* Find fifo_index */
            rx_fifo_index = i + (CANFD_RECEIVE_FIFO_BUFFER * channel);
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFCC(rx_fifo_index), val);
        }
    }

    /* Transmit/Receive FIFO buffer */
    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        for (i = 0; i < CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER; i++)
        {
            tx_fifo_index = i + (3 * channel);
            val = CFITT(N);                                         /* Set a message transmission interval (N = 15) */
            val &= ~CFITSS;                                         /* R-CarGen3: Interval time = (2 x M x N) / pclk
                                                                       R-CarV4H/R-CarV3U: Interval time = (M x N) / pclk
                                                                    */
            val &= ~CFITR;
            val |= CFTML(i);                                        /* Link TX FIFOs to transmit buffers */
            val |= CFM(0x01);                                       /* Transmit mode */
            val |= CFIM;                                            /* A FIFO transmit interrupt request is generated each time a message has been transmitted.*/                                     
            val |= CFIGCV(0);                                       /* Transmit/Receive FIFO Receive Interrupt Request Timing Select:  When FIFO is 1/8 full */                           
            val |= CFDC(0x02);                                      /* Transmit/Receive FIFO Buffer Depth Configuration: 8 messages */
            if (candev_info->candev_init->interface != CLASSIC_CAN_MODE)
                val |= CFPLS(0x07);                                 /* Transmit/Receive FIFO Buffer Payload Storing Size Select 64 bytes */
            else
                val |= CFPLS(0x00);                                 /* Transmit/Receive FIFO Buffer Payload Storing Size Select 8 bytes */   
            val |= CFTXIE;                                          /* Transmit/receive FIFO transmit interrupt is enabled. */
            val &= ~CFE;                                            /* Transmit/receive FIFO buffers are unused. */

            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFCC(tx_fifo_index), val);
            if(candev_info->candev_init->interface != CLASSIC_CAN_MODE)
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFFDCSTS(tx_fifo_index), 0);
        }
    }
}

/*******************************************************************************
* Outline      : Configuring global interrupt
* Function Name: setting_global_interrupt
* Description  : This function uses to configure global mode, FIFO message lost
*                interrupt is enabled, Payload overflow interrupt is enabled and
*                global operating mode.
* Arguments    : candev_info_t
* Return Value : none
* Note         :
*******************************************************************************/
static void setting_global_interrupt(candev_info_t *candev_info)
{
    uint32_t      val = 0;

    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR);
    val &= ~GSLPR;                         /* Not in global stop mode */
    val |= MEIE;
    if (candev_info->candev_init->interface != CLASSIC_CAN_MODE)
        val |= CMPOFIE;                        /* Payload overflow interrupt is enabled. */
    val &= ~GMDC(CANFD_BIT_MASK);          /* Global operating mode */
    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR, val);
}

/*******************************************************************************
* Outline      : Configuring channel interrupt
* Function Name: setting_channel_interrupt
* Description  : This function uses to configure channel interrupt inlcude:
*                Bus error interrupt is enabled,
*                Error warning interrupt is enabled,
*                     Bus off entry interrupt is enabled,
*                Bus off recovery interrupt is enabled,
*                Overload frame transmit interrupt is enabled,
*                Bus lock interrupt is enabled,
*                Arbitration lost interrupt is enabled,
*                Transmit abort interrupt is enabled
*                Bus Off Recovery Mode Select: ENTRT_CHANNEL_HALT_MODE_AT_BUS_OFF_ENTRY,
*                Error flags for all error information are displayed
* Arguments    : candev_info_t
* Return Value : none
* Note         :
*******************************************************************************/
static void setting_channel_interrupt(candev_info_t *candev_info)
{
        uint32_t        val = 0;

        val &= ~CSLPR;                        /* Release channel stop mode */
        val &= ~RTBO;                         /* Forcible Return from Bus-off */
        val |= BEIE;                          /* Bus error interrupt is enabled. */
        val |= EWIE;                          /* Error warning interrupt is enabled. */
        val |= EPIE;                          /* Error passive interrupt is enabled. */
        val |= BOEIE;                         /* Bus off entry interrupt is enabled. */
        val |= BORIE;                         /* Bus off recovery interrupt is enabled. */
        val |= OLIE;                          /* Overload frame transmit interrupt is enabled. */
        val |= BLIE;                          /* Bus lock interrupt is enabled. */
        val |= ALIE;                          /* Arbitration lost interrupt is enabled. */
        val |= TAIE;                          /* Transmit abort interrupt is enabled. */
        val &= ~EOCOIE;                       /* Error occurrence counter overflow interrupt is disabled. */
        val &= ~SOCOIE;                       /* Communication success occurrence counter overflow interrupt is disabled.*/
        val &= ~TDCVFIE;                      /* Transmit delay correction violation interrupt is disabled. */
        val &= ~BOM(CANFD_BIT_MASK);
        val |= BOM(ENTRT_CHANNEL_HALT_MODE_AT_BUS_OFF_ENTRY); /* Bus Off Recovery Mode Select */
        val |= ERRD;                          /* Error flags for all error information are displayed. */
        val &= ~CTME;                         /* Communication test mode is disabled. */
        val &= ~CTMS(CANFD_BIT_MASK);         /* Mode Select */
        val &= ~CRCT;                         /* The first bit in the receive ID field is not reversed. */
        val &= ~ROM;                          /* Limited operation mode is disabled. */
        for (int channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++) {
            /* Clear error flags */
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmERFL(channel), 0);
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel), val);
    }
}

/*******************************************************************************
* Outline      : Initialize CAN device registers
* Function Name: canfd_init_hw
* Description  : This function used to initialize CAN device registers by calling
*                other functions: set baudrate, set receive rules, set buffers, set
*                interrupts and change global modes or channel modes.
* Arguments    : candev_init_t *candev_init
*                candev_info_t *candev_info
* Return Value : none
* Note         :
*******************************************************************************/
void canfd_init_hw(candev_init_t *candev_init, candev_info_t *candev_info)
{
    uint32_t         timeout = CANFD_TIMEOUT;
    uint32_t         val = 0;
    uint8_t          channel = 0;
    uint8_t          mode = 0;
    uint32_t         i = 0;
    uint32_t         tx_fifo_index = 0;
    uint32_t         rx_fifo_index = 0;

    /* Check the GRAMINIT flag to make sure RAM initialized completely */
    while (GRAMINIT & in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGSTS))
    {
        if (!timeout--)
        {
            fprintf(stderr,"CANFD: RAM initialization is ongoing\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Transition from global stop mode to global reset mode */    
    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR);
    val &= ~GSLPR;
    val &= ~GMDC(CANFD_BIT_MASK);
    val |= GMDC(GLOBAL_RESET_MODE);
    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR, val);

    /* Wait for entering to global reset mode */
    timeout = CANFD_TIMEOUT;
    while (!(GRSTSTS & in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGSTS)))
    {
        if (!timeout--)
        {
            fprintf(stderr,"CANFD: enter to global reset mode failed\n");
            exit(EXIT_FAILURE);
        }
    } 

    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGERFL, 0);

    /* Selecting CANFD mode */
    if (!IS_RCAR_GEN4(product_id)) 
    {
        val = RCMC(candev_init->interface);
        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGRMCFG, val);
    }

    /* Transition from channel stop mode to channel reset mode */
    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel));
        val &= ~CSLPR;
        val &= ~CHMDC(CANFD_BIT_MASK);
        val |= CHMDC(CHANNEL_RESET_MODE & CANFD_BIT_MASK);
        val |= ERRD;          /* Error flags for all error information are displayed. */
        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel), val);
        /* Wait for entering to channel reset mode */
        timeout = CANFD_TIMEOUT;
        while (!(CRSTSTS & in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmSTS(channel))))
        {
            if (!timeout--)
            {
                fprintf(stderr,"Channel %d reset mode failed !\n", channel);
                exit(EXIT_FAILURE);
            }
        }
        if (IS_RCAR_GEN4(product_id)) 
        {
            if (IS_RCAR_GEN4(product_id)) 
            {
                /* Selecting CANFD mode */
                if (CLASSIC_CAN_MODE == candev_init->interface) // Classical CAN only mode
                {
                    val = 0;
                    val &= ~(RCAR_CFDCnFDCFG_CLOE);
                    val &= ~(RCAR_CFDCnFDCFG_FDOE);
                    val |= RCAR_CFDCnFDCFG_CLOE;
                    out32(candev_info->base + RCAR_CANFD_CFDCnFDCFG(channel), val);
                    val |= RCAR_CFDCnFDCFG_CLOE;
                    if (candev_init->classic_tol)
                    {
                        val |= RCAR_CFDCnFDCFG_CFDTE;
                    }
                    out32(candev_info->base + RCAR_CANFD_CFDCnFDCFG(channel), val);
                } 
                else if (CANFD_MODE == candev_init->interface) // CAN-FD mode
                {
                    val = 0;
                    val &= ~(RCAR_CFDCnFDCFG_CLOE);
                    val &= ~(RCAR_CFDCnFDCFG_FDOE);
                    out32(candev_info->base + RCAR_CANFD_CFDCnFDCFG(channel), val);
                }
                else // FD only mode
                {
                    val = 0;
                    val &= ~(RCAR_CFDCnFDCFG_CLOE);
                    val &= ~(RCAR_CFDCnFDCFG_FDOE);
                    val |= RCAR_CFDCnFDCFG_FDOE;
                    out32(candev_info->base + RCAR_CANFD_CFDCnFDCFG(channel), val);
                }
            }
        }
    }

    /* Clear global error flags*/
    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGERFL, 0);

    /* Setting
        • Clock
        • Bit timing
        • Communication speed
        • Timestamp mirror function
        • DLC filter
        • Transmit priority
    */
    setting_baudrate(candev_info);
    /* Setting receive rules */
    setting_receive_rules(candev_info);
    /* Setting buffer */
    setting_buffer(candev_info);
    /* Setting global interrupt */
    setting_global_interrupt(candev_info);
    /* Setting channel interrupt */
    setting_channel_interrupt(candev_info);
   /* Entering to global operating mode */
    val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR);
    val &= ~GMDC((GLOBAL_OPERAING_MODE | CANFD_BIT_MASK));
    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDGCTR, val);
    timeout = CANFD_TIMEOUT;
    while ((GRAMINIT | GSLPSTS | GHLTSTS | GRSTSTS) &
            in32(candev_info->base + RCAR_CANFD_RSCFD0CFDGSTS))
    {
        if (!timeout--)
        {
            fprintf(stderr,"global operating failed !\n");
            exit(EXIT_FAILURE);
        }
    }
   /* Selecting operating mode */
    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        mode = candev_init->channel[channel].mode;
        if (CHANNEL_COMMUNICATION_MODE != mode)
        {
            //Enter to channel halt mode
            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel));
            val &= ~CHMDC(CANFD_BIT_MASK);
            val |= CHMDC(CHANNEL_HALT_MODE & CANFD_BIT_MASK);
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel), val);

            /* Waiting for entering channel halt mode */
            timeout = CANFD_TIMEOUT;
            while (!(CHLTSTS & in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmSTS(channel))))
            {
                if (!timeout--)
                {
                    fprintf(stderr,"Enter channel %d halt mode failed.\n", channel);
                    exit(EXIT_FAILURE);
                }
            }

            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel));
            val |= CTME;                                          /* Communication test mode is enabled. */
            val &= ~CTMS(CANFD_BIT_MASK);
            val |= CTMS(mode & CANFD_BIT_MASK);                   /* Set mode */
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel), val);
        }
    }
    /* Entering to channel communication mode */
    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel));
        val &= ~CHMDC(CANFD_BIT_MASK);
        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmCTR(channel), val);
        /* Waiting for entering to channel communication mode */
       timeout = CANFD_TIMEOUT;
       while (!(COMSTS & in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCmSTS(channel))))
       {
           if (!timeout--)
           {
               fprintf(stderr,"Enter channel %d communication failed.\n", channel);
               exit(EXIT_FAILURE);
           }
       }
    }
    /* Enable all RX FIFO in the global operating mode only*/
    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        for (i = 0; i < CANFD_RECEIVE_FIFO_BUFFER; i++)
        {
            rx_fifo_index = i + (CANFD_RECEIVE_FIFO_BUFFER * channel);
            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFCC(rx_fifo_index));
            val |= RFE;
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDRFCC(rx_fifo_index),val);
        }
    }
    /* Enable all TX FIFO in the global operating mode only*/
    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        for (i = 0;i < CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER; i++)
        {
            tx_fifo_index = i + (3 * channel);
            val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFCC(tx_fifo_index));
            val |= CFE;
            out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFCC(tx_fifo_index),val);
        }
    }
}

/*******************************************************************************
* Outline      : Attach interrupt handler for rx interrupt
* Function Name: canfd_init_tx_irq
* Description  : This function uses to attach interrupt handler for channel interrupt.
*                If there is an transmit interrupt, the function canfd_tx_irq() will
*                be called with input parameter is candev_exp.
* Arguments    : candev_exp_t
* Return Value : none
* Note         :
*******************************************************************************/
void canfd_init_tx_irq(candev_exp_t *candev_exp)
{
    int r = 0;

    /* Attach interrupt handler for channel interrupt */
    r = InterruptAttach(candev_exp->candev_info->candev_init->canfd_hwi.irqvector[1], canfd_tx_irq,
                                                            candev_exp, 0, _NTO_INTR_FLAGS_TRK_MSK);
    if (-1 == r)
    {
        fprintf(stderr,"InterruptAttach irqsys\n");
        exit(EXIT_FAILURE);
    }
}

/*******************************************************************************
* Outline      : Attach interrupt handler for tx interrupt
* Function Name: canfd_init_rx_irq
* Description  : This function uses to attach interrupt handler for global interrupt.
*                If there is an global interrupt, the function canfd_rx_interrupt() will
*                be called with input parameter is candev_exp.
* Arguments    : candev_exp_t
* Return Value : none
* Note         :
*******************************************************************************/
void canfd_init_rx_irq(candev_exp_t *candev_exp)
{
    int r = 0;

    /* Attach interrupt handler for global interrupt */
    r = InterruptAttach(candev_exp->candev_info->candev_init->canfd_hwi.irqvector[0], canfd_rx_irq,
                                                            candev_exp, 0, _NTO_INTR_FLAGS_TRK_MSK);
    if (-1 == r)
    {
        fprintf(stderr,"InterruptAttach irqsys\n");
        exit(EXIT_FAILURE);
    }
}

/*******************************************************************************
* Outline      : Building a CAN message to send out.
* Function Name: canfd_send_msg
* Description  : This function uses to building a CAN message to send out.
* Arguments    : CANDEV  *cdev
*                canmsg_t *canmsg
* Return Value : none
* Note         :
*******************************************************************************/
static void  canfd_send_msg(CANDEV  *cdev,canmsg_t *canmsg)
{
    candev_exp_t           *candev_list = (candev_exp_t *)cdev;
    candev_info_t          *candev_info = candev_list->candev_info;
    candev_init_t          *candev_init = candev_info->candev_init;
    uint8_t                 k = 0;
    uint8_t                 i = 0;
    uint32_t                timeout = 10;
    uint32_t                val = 0;
    uint32_t                payload = 0;
    uint32_t                txid = 0;
    uint8_t                 txbrs = 0;
    uint8_t                 channel = 0;
    uint8_t                 index = 0;
    uint8_t                 typeFrame = candev_init->channel[candev_list->channel].frameType;

    /* Select ID, bitrate switch mode and TX FIFO(k) depend on channel and channel's tx index */
    channel = candev_list->channel;
    index   = cdev->dev_unit;
    txid    = (~canmsg->cmsg.mid)? canmsg->cmsg.mid : candev_init->channel[channel].txid[index];
    txbrs   = candev_init->channel[channel].txbrs;
    k       = index + (3 * channel);
    while (timeout--)
    {
        val = in32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFSTS(k));
        if (!(CFFLL & val))                      /* The transmit/receive FIFO buffer is not full. */
        {
            val = CFID(txid);                    /* Transmit/Receive FIFO Buffer ID Data */
            if (MAX_STANDARD_ID < val)
            {
                val |= CFIDE;                    /* Use IDE bit */
            }
            else
            {
                val &= ~CFIDE;                   /* No IDE bit */
            }
            val &= ~THLEN;                       /* Transmit history data is not stored in the buffer. */
            val &= ~CFRTR;                       /* Data frame */

            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFID(k), val);
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFID(k), val);
                else
                    out32(candev_info->base + RCAR_CAN_RSCFD0CFID(k), val);
            }

            val = CFDLC(canmsg->cmsg.len);     /* DLC */
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFPTR(k), val);
                val = typeFrame;                 /* Select type of CAN frame */
                if (txbrs)
                {
                    val |= CFBRS;                /* The bit rate in the data area is changed. */
                }
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFFDCSTS(k), val);
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFPTR(k), val);
                else
                {
                    out32(candev_info->base + RCAR_CAN_RSCFD0CFPTR(k), val);
                }
            }

                /* Copy data to payload */
            for (i = 0; i < canmsg->cmsg.len; i+=sizeof(uint32_t))
            {
                memcpy(&payload,&canmsg->cmsg.dat[i],sizeof(uint32_t));
                if (CLASSIC_CAN_MODE != candev_init->interface)
                {
                    /* CANFD interface */
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFDF(i >> 2, k), payload);
                }
                else
                {
                    /* Classic CAN interface */
                    if (IS_RCAR_GEN4(product_id))
                        out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFDF(i >> 2, k), payload);
                    else
                        {
                        if (0 < i)
                        {
                            out32(candev_info->base + RCAR_CAN_RSCFD0CFDF1(k), payload);          /* 4 low bytes */
                        }
                        else
                        {
                            out32(candev_info->base + RCAR_CAN_RSCFD0CFDF0(k), payload);          /* 4 high bytes */
                        }
                    }
                }
            }

            /* Requesting a tx interrupt */
            if (CLASSIC_CAN_MODE != candev_init->interface)
            {
                /* CANFD interface */
                out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFPCTR(k), CFPC(0xFF));
            }
            else
            {
                /* Classic CAN interface */
                if (IS_RCAR_GEN4(product_id))
                    out32(candev_info->base + RCAR_CANFD_RSCFD0CFDCFPCTR(k), CFPC(0xFF));
                else
                    out32(candev_info->base + RCAR_CAN_RSCFD0CFPCTR(k), CFPC(0xFF));
            }

            break;
        }
    }
}

/*******************************************************************************
* Outline      : Be called by application
* Function Name: can_transmit_cb
* Description  : This function will be called by application then it sends parametes
*                to the function canfd_send_msg to build a CAN message and send out.
* Arguments    : CANDEV *cdev
* Return Value : none
* Note         :
*******************************************************************************/
void canfd_transmit_cb(CANDEV *cdev)
{
    candev_exp_t            *dev    = (candev_exp_t *)cdev;
    canmsg_t                *canmsg;

    if (!(dev->dflags & CANDEV_TX_ENABLED) && (canmsg = canmsg_dequeue_element(cdev->msg_queue)))
    {
        /* Indicate transmit is in progress */
        atomic_set(&dev->dflags, CANDEV_TX_ENABLED);
        /* Start transmission */
        canfd_send_msg(cdev,canmsg);
        /* Free queue */
        canmsg_queue_element(cdev->free_queue,canmsg);
    }
}
