/*
 * $QNXLicenseC:
 * Copyright 2015, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

/*
 * We use the same buffer for transmit and receive
 * For exchange, that's exactly what we wanted
 * For Read, it doesn't matter what we write to SPI, so we are OK.
 * For transmit, the receive data is put at the buffer we just transmitted, we are still OK.
 */
int msiof_attach_intr(msiof_spi_t *msiof)
{
    if ((msiof->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1) {
        return -1;
    }

    if ((msiof->coid = ConnectAttach(0, 0, msiof->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
        goto fail;
    }

    msiof->spievent.sigev_notify   = SIGEV_PULSE;
    msiof->spievent.sigev_coid     = msiof->coid;
    msiof->spievent.sigev_code     = MSIOF_SPI_EVENT;
    msiof->spievent.sigev_priority = MSIOF_SPI_PRIORITY;

    /*
     * Attach SPI interrupt
     */
    msiof->iid_spi = InterruptAttachEvent(msiof->irq_spi, &msiof->spievent, _NTO_INTR_FLAGS_TRK_MSK);

    if (msiof->iid_spi != -1) {
        return 0;
    }

    ConnectDetach(msiof->coid);
fail:
    ChannelDestroy(msiof->chid);
    return -1;
}
