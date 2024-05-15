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

#include "gether.h"
#include <sys/kthread.h>

#if (_NTO_VERSION < 660)
#include <drvr/mdi.h>
#else
#include <netdrvr/mdi.h>
#endif

/* MDIO bus is 2.5MHz so clock is 200ns high + 200ns low */
#define PHY_DELAY() nanospin_ns(200)

static inline void gether_mdio_idle(gether_dev_t *gether)
{
    out32(gether->base + GETHER_PIR, 0);
    PHY_DELAY();
}

static inline void gether_mdio_release_bus(gether_dev_t *gether)
{
    out32(gether->base + GETHER_PIR, 0);
    out32(gether->base + GETHER_PIR, GETHER_PIR_MDC);
    PHY_DELAY();
    out32(gether->base + GETHER_PIR, 0);
    PHY_DELAY();
}

static inline void gether_mdio_write_bit(gether_dev_t *gether, uint8_t data)
{
    out32(gether->base + GETHER_PIR, GETHER_PIR_MMD | (data ? GETHER_PIR_MDO : 0));
    nanospin_ns(10); // mdio Setup time
    out32(gether->base + GETHER_PIR, GETHER_PIR_MMD | GETHER_PIR_MDC | (data ? GETHER_PIR_MDO : 0));
    PHY_DELAY();
    out32(gether->base + GETHER_PIR, GETHER_PIR_MMD | (data ? GETHER_PIR_MDO : 0));
    PHY_DELAY();
}

static uint8_t gether_mdio_read_bit(gether_dev_t *gether)
{
    uint32_t val;

    out32(gether->base + GETHER_PIR, GETHER_PIR_MDC);
    PHY_DELAY();
    out32(gether->base + GETHER_PIR, 0);
    PHY_DELAY();
    val = in32(gether->base + GETHER_PIR);

    return ((val & GETHER_PIR_MDI) ? 1 : 0);
}

uint16_t gether_mii_read(void *handle, uint8_t phy_id, uint8_t location)
{
    gether_dev_t      *gether;
    int             i;
    uint16_t        val;
    int             retries;

    gether = handle;

    for (retries = 0; retries < 3; retries++) {
        /* Preamble */
        for (i = 0; i < 32; i++) {
            gether_mdio_write_bit(gether, 1);
        }

        /* Start */
        gether_mdio_write_bit(gether, 0);
        gether_mdio_write_bit(gether, 1);

        /* Read */
        gether_mdio_write_bit(gether, 1);
        gether_mdio_write_bit(gether, 0);

        /* PHY Address */
        for (i = 4; i >=0; i--) {
            gether_mdio_write_bit(gether, (phy_id >> i) & 1);
        }

        /* Register */
        for (i = 4; i >=0; i--) {
            gether_mdio_write_bit(gether, (location >> i) & 1);
        }

        /* Turnaround */
        gether_mdio_release_bus(gether);

        /* Data */
        val = 0;
        for (i = 15; i >=0; i--) {
            val |= (gether_mdio_read_bit(gether) << i);
        }

        if (val != 0xffff) {
            break;
        }
    }

    if (gether->cfg.verbose & VERBOSE_PHY) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "gether: MDIO Read Phy %d Reg 0x%x Data 0x%x",
              phy_id, location, val);
    }

    return (val);
}

void gether_mii_write(void *handle, uint8_t phy_id, uint8_t location,
                            uint16_t data)
{
    gether_dev_t      *gether;
    int             i;

    gether = handle;

    /* Preamble */
    for (i = 0; i < 32; i++) {
        gether_mdio_write_bit(gether, 1);
    }

    /* Start */
    gether_mdio_write_bit(gether, 0);
    gether_mdio_write_bit(gether, 1);

    /* Write */
    gether_mdio_write_bit(gether, 0);
    gether_mdio_write_bit(gether, 1);

    /* PHY Address */
    for (i = 4; i >=0; i--) {
        gether_mdio_write_bit(gether, (phy_id >> i) & 1);
    }

    /* Register */
    for (i = 4; i >=0; i--) {
        gether_mdio_write_bit(gether, (location >> i) & 1);
    }

    /* Turnaround */
    gether_mdio_write_bit(gether, 1);
    gether_mdio_write_bit(gether, 0);

    /* Data */
    for (i = 15; i >=0; i--) {
        gether_mdio_write_bit(gether, (data >> i) & 1);
    }

    gether_mdio_idle(gether);

    if (gether->cfg.verbose & VERBOSE_PHY) {
    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
          "gether: MDIO Write Phy %d Reg 0x%x Data 0x%x",
          phy_id, location, data);
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/phy_common.c $ $Rev: 869541 $")
#endif
