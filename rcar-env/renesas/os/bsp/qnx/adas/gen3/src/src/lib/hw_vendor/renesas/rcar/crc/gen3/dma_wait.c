/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#include <errno.h>
#include <sys/neutrino.h>
#include <sched.h>
#include <string.h>
#include <sys/siginfo.h>
#include "rcar_crc_common.h"

/***********************************************
 *	MACROS/DEFINES
 ***********************************************/
#define TIMEOUT_NS               (100000)                  // Timeout in nano second
#define PULSE_CODE_TIMEOUT       _PULSE_CODE_MINAVAIL      // Pulse from timer

/***********************************************
 *	GLOBAL VARIABLE
 ***********************************************/

/***********************************************
 *      FUNCTION DECLARATION
 ***********************************************/
int rcar_crc_wait(CRC_DEVICE_Type *dev, int len);

/***********************************************
 *	APIs IMPLEMENATION
 ***********************************************/

/*!
 *    @brief        Wait for DMA transfer is finished
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    int len
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int rcar_crc_wait(CRC_DEVICE_Type *dev, int len)
{
    struct _pulse	pulse;
    struct itimerspec   timeout;
    int                 timer_id;
    struct sigevent     event;
    
    memset( &timeout, 0, sizeof(timeout) );
    timeout.it_value.tv_sec = 5; // Set timeout is 5 seconds

    SIGEV_PULSE_INIT( &event, dev->coid, (short)0, (short)PULSE_CODE_TIMEOUT, (void *) NULL ); 
    timer_create( CLOCK_REALTIME, &event, &timer_id );

    timer_settime( timer_id, 0, &timeout, NULL );
    for (;;){
        if (MsgReceivePulse(dev->chid, &pulse, sizeof(pulse), NULL) == -1) {
            fprintf(stderr, "%s  errono %s(%d)\n", __func__,strerror(errno), errno);
            return (-EIO);
        }

        switch (pulse.code) {
            case CRC_RXDMA_EVENT:
                if (dev->dmafuncs.bytes_left(dev->rx_dma) != 0) {
                    fprintf(stderr, "RX DMA is not completed properly\n");
                    return (-EIO);
                }
                dev->dmafuncs.xfer_complete(dev->rx_dma);
                /* Unmask the Interrupt */
                InterruptUnmask(dev->irq_crc, dev->iid_crc);
                return (EOK);
            case CRC_TXDMA_EVENT:
                if (dev->dmafuncs.bytes_left(dev->tx_dma) != 0) {
                    fprintf(stderr, "TX DMA is not completed properly\n");
                    return (-EIO);
                }
                dev->dmafuncs.xfer_complete(dev->tx_dma);
                /* Unmask the Interrupt */
                InterruptUnmask(dev->irq_crc, dev->iid_crc);
                return (EOK);
            case PULSE_CODE_TIMEOUT:
                fprintf(stderr, "Wait for Pulse Timeout\n");
                return (-EIO);
        }
    }
}
