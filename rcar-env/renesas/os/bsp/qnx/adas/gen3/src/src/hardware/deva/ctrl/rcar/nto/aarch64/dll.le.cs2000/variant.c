/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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

#include <audio_driver.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>
#include "i2c2.h"
#include "rcar.h"
#include "variant.h"

typedef struct variant_instance
{
   HW_CONTEXT_T*                    hw_context;
   uint32_t                         mclk1_rate;
   uint32_t                         mclk2_rate;
   TAILQ_ENTRY(variant_instance)    link;
} variant_instance_t;

static TAILQ_HEAD(instance_list, variant_instance) instances;

static ado_mutex_t cs2000_lock = PTHREAD_MUTEX_INITIALIZER;
extern void cs2000_set(uint8_t enable);
extern void cs2000_register_dump(void);

int variant_init ( HW_CONTEXT_T *rcar )
{
    variant_instance_t* instance;
    static int init_instances = 0;
    int  status;
    char devname[40];

    ado_mutex_lock (&cs2000_lock);

    if (!init_instances) {
        TAILQ_INIT(&instances);
        init_instances = 1;
    }

    if (TAILQ_EMPTY(&instances)) {

        ado_debug (DB_LVL_DRIVER, "RCAR: Variant init");

        if (get_i2c_fd() != -1) {
            ado_debug (DB_LVL_DRIVER, "RCAR: I2C already opened");
        } else {
            sprintf( devname, "/dev/i2c%d", rcar->i2c_dev );

            if ((status = open_i2c_fd(devname)) != EOK) {
                ado_mutex_unlock (&cs2000_lock);
                ado_error_fmt("RCAR: Failed to open I2C FD");
                return status;
            }
        }

        cs2000_set(0); /* default, CS2000 not enabled */

        if ( rcar->debug ) {
            cs2000_register_dump();
        }
    } else {
        ado_debug(DB_LVL_DRIVER, "RCAR: Variant already initialized");
    }

    instance = calloc (1, sizeof(variant_instance_t));
    instance->hw_context = rcar;
    instance->mclk1_rate = 22579200; /* default, CS2000 not enabled */
    instance->mclk2_rate = 0;
    TAILQ_INSERT_TAIL(&instances, instance, link);

    ado_mutex_unlock(&cs2000_lock);

    return EOK;
}

int variant_uninit ( HW_CONTEXT_T *rcar )
{
    variant_instance_t* instance;

    ado_mutex_lock(&cs2000_lock);

    for (instance = TAILQ_FIRST(&instances); instance != NULL; instance = TAILQ_NEXT(instance, link)) {
        if (instance->hw_context == rcar) {
            TAILQ_REMOVE(&instances, instance, link);
            free (instance);
            break;
        }
    }

    if (TAILQ_EMPTY(&instances)) {

        ado_debug (DB_LVL_DRIVER, "RCAR: Variant uninit");

        if (get_i2c_fd() == -1) {
            ado_debug (DB_LVL_DRIVER, "RCAR: Nothing to do, not initialized");
            ado_mutex_unlock(&cs2000_lock);
            return EOK;
        }

        cs2000_set(0);

        if ( rcar->debug ) {
            cs2000_register_dump();
        }

        close_i2c_fd();
    } else {
        ado_debug(DB_LVL_DRIVER, "RCAR: Variant can't be uninitialized yet");
    }

    ado_mutex_unlock(&cs2000_lock);

    return EOK;
}

int variant_set_clock_rate ( HW_CONTEXT_T* rcar, uint32_t mclk1_rate, uint32_t mclk2_rate )
{
    bool cs2000_enable = false;
    variant_instance_t* instance;

    ado_debug (DB_LVL_DRIVER, "RCAR: mclk1_rate %d, mclk2_rate %d", mclk1_rate, mclk2_rate);

    if (get_i2c_fd() == -1) {
        ado_error_fmt ("RCAR: Not initialized");
        return EBADFD;
    }

    ado_mutex_lock(&cs2000_lock);

    /* store the mclk1,2_rate for the current instance and cycle through all instances to check if
     * any instance uses mclk1,2_rate of 24576000, which requires the enabling of the CS2000 in order
     * to generate this rate
     */
    for (instance = TAILQ_FIRST(&instances); instance != NULL; instance = TAILQ_NEXT(instance, link)) {
        if (instance->hw_context == rcar) {
            if (mclk1_rate != 0) {
                instance->mclk1_rate = mclk1_rate;
            }
            if (mclk2_rate != 0) {
                instance->mclk2_rate = mclk2_rate;
            }
        }
        if (instance->mclk1_rate == 24576000 || instance->mclk2_rate == 24576000) {
            cs2000_enable = true;
        }
    }

    ado_mutex_unlock(&cs2000_lock);


    if (cs2000_enable) {
        ado_debug( DB_LVL_DRIVER, "RCAR: Enabling CS2000" );
        cs2000_set(1);
    } else {
        ado_debug( DB_LVL_DRIVER, "RCAR: Disabling CS2000" );
        cs2000_set(0);
    }

    return EOK;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/nto/aarch64/dll.le.cs2000/variant.c $ $Rev: 895184 $")
#endif
