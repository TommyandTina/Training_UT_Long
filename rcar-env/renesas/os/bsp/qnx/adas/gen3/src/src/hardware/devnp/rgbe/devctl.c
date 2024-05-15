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

#include <net/ifdrvcom.h>
#include <sys/sockio.h>

void gether_update_stats (gether_dev_t *gether)
{
    nic_ethernet_stats_t        *estats;

    estats = &gether->stats.un.estats;
    /*
     * HW counters do not clear on read and do not rollover.
     * We read the hardware and accumulate in to the software stat,
     * and then write back a 0 to reset the hardware stat. This has the
     * risk of missing counts that happen between the read and write back,
     * but it is the best we can do with this hardware design.
     */

    /* Transmit Retry Over Count */
    estats->xcoll_aborted += in32(gether->base + GETHER_TROCR);
    out32(gether->base + GETHER_TROCR, 0);
    /* Delayed Collision Detect Counter Register  */
    estats->late_collisions += in32(gether->base + GETHER_CDCR);
    out32(gether->base + GETHER_CDCR, 0);
    /* Lost Carrier Counter Register  */
    estats->no_carrier += in32(gether->base + GETHER_LCCR);
    out32(gether->base + GETHER_LCCR, 0);
    /* CRC Error Frame Receive Counter Register  */
    estats->fcs_errors += in32(gether->base + GETHER_CEFCR);
    out32(gether->base + GETHER_CEFCR, 0);
    /* Frame Receive Error Counter Register */
    estats->symbol_errors += in32(gether->base + GETHER_FRECR);
    out32(gether->base + GETHER_FRECR, 0);
    /* Too-Short Frame Receive Counter Register */
    estats->short_packets += in32(gether->base + GETHER_TSFRCR);
    out32(gether->base + GETHER_TSFRCR, 0);
    /* Too-Long Frame Receive Counter Register */
    estats->oversized_packets += in32(gether->base + GETHER_TLFRCR);
    out32(gether->base + GETHER_TLFRCR, 0);
}

void gether_clear_stats (gether_dev_t *gether)
{
    /* Clear the counters in hw as part of reading them */
    gether_update_stats(gether);

    /* Now clear counters in our data structure */
    memset(&gether->stats, 0, sizeof(gether->stats));

    /* Reset stats info for devctl */
    gether->stats.revision = NIC_STATS_REVISION;

    gether->stats.valid_stats =
    NIC_STAT_TXED_MULTICAST | NIC_STAT_RXED_MULTICAST |
    NIC_STAT_TXED_BROADCAST | NIC_STAT_RXED_BROADCAST |
    NIC_STAT_TX_FAILED_ALLOCS | NIC_STAT_RX_FAILED_ALLOCS;

    gether->stats.un.estats.valid_stats =
    NIC_ETHER_STAT_ALIGN_ERRORS |
    NIC_ETHER_STAT_FCS_ERRORS |
    NIC_ETHER_STAT_LATE_COLLISIONS |
    NIC_ETHER_STAT_XCOLL_ABORTED |
    NIC_ETHER_STAT_NO_CARRIER |
    NIC_ETHER_STAT_SYMBOL_ERRORS |
    NIC_ETHER_STAT_OVERSIZED_PACKETS |
    NIC_ETHER_STAT_SHORT_PACKETS;
}

int gether_ioctl (struct ifnet *ifp, unsigned long cmd, caddr_t data)
{
    gether_dev_t          *gether;
    int                    error;
    struct ifdrv_com      *ifdc;
    struct drvcom_config  *dcfgp;
    struct drvcom_stats   *dstp;
    struct ifreq          *ifr;

    gether = ifp->if_softc;
    error = EOK;

    switch (cmd) {
        case SIOCGDRVCOM:
            ifdc = (struct ifdrv_com *)data;
            switch (ifdc->ifdc_cmd) {
                case DRVCOM_CONFIG:
                    dcfgp = (struct drvcom_config *)ifdc;

                    if (ifdc->ifdc_len != sizeof(nic_config_t)) {
                        error = EINVAL;
                        break;
                    }
                    memcpy(&dcfgp->dcom_config, &gether->cfg, sizeof(gether->cfg));
                    break;

                case DRVCOM_STATS:
                    dstp = (struct drvcom_stats *)ifdc;

                    if (ifdc->ifdc_len != sizeof(nic_stats_t)) {
                        error = EINVAL;
                        break;
                    }

                    nic_mutex_lock(&gether->drv_mutex);
                    gether_update_stats(gether);
                    nic_mutex_unlock(&gether->drv_mutex);
                    memcpy(&dstp->dcom_stats, &gether->stats, sizeof(dstp->dcom_stats));
                    break;

                default:
                    error = EOPNOTSUPP;
                    break;
            }
            break;

        case SIOCSIFMEDIA:
            if (gether_is_mv_phy(gether)) {
                error = ENOTTY;
                break;
            }
        case SIOCGIFMEDIA:
            ifr = (struct ifreq *)data;
            struct ifmediareq *ifmr = (struct ifmediareq *) ifr;
            if (gether_is_mv_phy(gether)) {
                ifmr->ifm_current = IFM_ETHER | IFM_MANUAL;
                /* Assuming a master role (far-end phy is slave) */
                ifmr->ifm_current |= IFM_ETH_MASTER;
                ifmr->ifm_active = ifmr->ifm_current;
                ifmr->ifm_count = 0;
                ifmr->ifm_status = IFM_AVALID;
                if ((gether->cfg.flags & NIC_FLAG_LINK_DOWN) == 0) {
                    ifmr->ifm_status |= IFM_ACTIVE;
                }
            } else {
                nic_mutex_lock(&gether->drv_mutex);
                error = ifmedia_ioctl(ifp, ifr, &gether->bsd_mii.mii_media, cmd);
                nic_mutex_unlock(&gether->drv_mutex);
            }
            break;

        case SIOCSIFMTU:
            ifr = (struct ifreq *)data;
            /* Hardware only supports 1500 MTU */
            if (ifr->ifr_mtu == ETHERMTU) {
                error = EOK;
            } else {
                error = EINVAL;
            }
            break;
        default:
            error = ether_ioctl(ifp, cmd, data);
            if (error == ENETRESET) {
                /* No multicast filtering in hardware to update */
                error = EOK;
            }
            break;
    }
    return error;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/devctl.c $ $Rev: 869512 $")
#endif
