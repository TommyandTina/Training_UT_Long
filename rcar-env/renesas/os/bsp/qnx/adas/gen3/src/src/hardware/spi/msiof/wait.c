/*
 * $QNXLicenseC:
 * Copyright 2015, QNX Software Systems.
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

#include "msiof.h"

extern void spi_dump(msiof_spi_t *msiof);

int msiof_wait(msiof_spi_t *dev, int len)
{
	struct _pulse	 pulse;

	while (1) {
		if (len) {
			uint64_t to = dev->dtime * 1000 * 50; /* 50 times for time out */
			to *= len ;
			TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_RECEIVE, NULL, &to, NULL);
		}
		if (MsgReceivePulse(dev->chid, &pulse, sizeof(pulse), NULL) == -1){
			fprintf(stderr, "msiof_wait: errono %s(%d)\n", strerror(errno), errno);
			spi_dump(dev);
			return -1;
		}

		switch (pulse.code) {
			case MSIOF_SPI_EVENT:
				out32(dev->vbase + RCAR_MSIOF_IER, in32(dev->vbase + RCAR_MSIOF_IER) & ~(IER_TEOF | IER_REOF));
				InterruptUnmask(dev->irq_spi, dev->iid_spi);
				return (EOK);
            case MSIOF_SPI_RXDMA_EVENT:
                if (dev->dmafuncs.bytes_left(dev->rx_dma) != 0) {
                    fprintf(stderr, "MSIOF : RX DMA is not completed properly\n");
                    return (-EIO);
                }
                dev->dmafuncs.xfer_complete(dev->rx_dma);
                out32(dev->vbase + RCAR_MSIOF_IER, in32(dev->vbase + RCAR_MSIOF_IER) & ~(IER_RDREQE | IER_RDMAE));
                return (EOK);
            case MSIOF_SPI_TXDMA_EVENT:
                if (dev->dmafuncs.bytes_left(dev->tx_dma) != 0) {
                    fprintf(stderr, "MSIOF : TX DMA is not completed properly\n");
                    return (-EIO);
                }
                dev->dmafuncs.xfer_complete(dev->tx_dma);
                out32(dev->vbase + RCAR_MSIOF_IER, in32(dev->vbase + RCAR_MSIOF_IER) & ~(IER_TDREQE | IER_TDMAE));
                return (EOK);
        }
	}
}
