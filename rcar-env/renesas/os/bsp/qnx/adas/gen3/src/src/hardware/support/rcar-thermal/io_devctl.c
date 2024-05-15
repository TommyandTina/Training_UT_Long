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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <sys/resmgr.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <assert.h>
#include <devctl.h>

#include "proto.h"

static pthread_mutex_t g_csIOCTL= PTHREAD_MUTEX_INITIALIZER;

int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
    thermal_packet_t *data;
    int  status = 0;
    int  nbytes = 0;

    /*
     *  Let common code handle DCMD_ALL_* cases.
     *  You can do this before or after you intercept devctl's depending
     *  on your intentions.  Here we aren't using any pre-defined values
     *  so let the system ones be handled first.
     */
    if ((status = iofunc_devctl_default(ctp, msg, ocb)) != _RESMGR_DEFAULT) {
        return(status);
    }

    /*
     *  Three examples of devctl operations.
     *  SET: Setting a value (int) in the server
     *  GET: Getting a value (int) from the server
     */
    if((status = pthread_mutex_lock(&g_csIOCTL)) != EOK)
        return status;

    /* Get the data from the message */
    data = _DEVCTL_DATA(msg->i);

    switch (msg->i.dcmd) {
        case DCMD_THSCIVM_GET_TEMPERATURE:
            status = thermal_get_temperature(data);
            if(status != EOK) {
                pthread_mutex_unlock(&g_csIOCTL);
                return status;
            }
            nbytes = sizeof(thermal_packet_t);
            break;
        case DCMD_THSCIVM_GET_VOLTAGE:
            status = thermal_get_voltage(data);
            if(status != EOK) {
                pthread_mutex_unlock(&g_csIOCTL);
                return status;
            }
            nbytes = sizeof(thermal_packet_t);
            break;
        default:
            pthread_mutex_unlock( &g_csIOCTL );
            return(ENOSYS);
    }

    if((status = pthread_mutex_unlock( &g_csIOCTL )) != EOK)
        return status;

    memset(&msg->o, 0, sizeof(msg->o));
    msg->o.ret_val = status;
    msg->o.nbytes = nbytes;
    
    return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o) + nbytes));
}
