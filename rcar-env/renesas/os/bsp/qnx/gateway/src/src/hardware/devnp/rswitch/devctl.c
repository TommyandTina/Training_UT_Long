/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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

#include "rswitch.h"

#include <net/ifdrvcom.h>
#include <sys/sockio.h>
#include <hw/rswitch_devnp_ioctl.h>

int rswitch2_mii_op_ioctl(rswitch2_dev_t *rswitch2, struct ifdrv *ifd);

void rswitch2_update_stats (rswitch2_dev_t *rswitch2)
{
    //nic_ethernet_stats_t        *estats;

    //estats = &rswitch2->stats.un.estats;
    /*
     * HW counters do not clear on read and do not rollover.
     * We read the hardware and accumulate in to the software stat,
     * and then write back a 0 to reset the hardware stat. This has the
     * risk of missing counts that happen between the read and write back,
     * but it is the best we can do with this hardware design.
     */

    /*TODO: */

    /* MAFCR is multicast rx which is already counted in rswitch2_receive() */
}

void rswitch2_clear_stats (rswitch2_dev_t *rswitch2)
{
    /* Clear the counters in hw as part of reading them */
    rswitch2_update_stats(rswitch2);

    /* Now clear counters in our data structure */
    memset(&rswitch2->stats, 0, sizeof(rswitch2->stats));

    /* Reset stats info for devctl */
    rswitch2->stats.revision = NIC_STATS_REVISION;

    rswitch2->stats.valid_stats =
    NIC_STAT_TXED_MULTICAST | NIC_STAT_RXED_MULTICAST |
    NIC_STAT_TXED_BROADCAST | NIC_STAT_RXED_BROADCAST |
    NIC_STAT_TX_FAILED_ALLOCS | NIC_STAT_RX_FAILED_ALLOCS;

    rswitch2->stats.un.estats.valid_stats =
    NIC_ETHER_STAT_FCS_ERRORS |
    NIC_ETHER_STAT_SYMBOL_ERRORS |
    NIC_ETHER_STAT_OVERSIZED_PACKETS |
    NIC_ETHER_STAT_SHORT_PACKETS;
}


int rswitch2_mii_op_ioctl(rswitch2_dev_t *rswitch2, struct ifdrv *ifd)
{
    union phy_ops_t         mdi_op;
    uint32_t        length, mii_op_size = 0;
    nic_config_t	*cfg = &rswitch2->cfg;
    int             err = EOK;

    if (ifd->ifd_len <= sizeof(mdi_op)) {
        return EINVAL;
    }

    /* get the size of mii_op*/
    if (ISSTACK) {
        if (copyin((((uint8_t *)ifd) + sizeof(*ifd)),
            &mii_op_size, sizeof(uint32_t))) {
                return EINVAL;
            }
    } else {
        memcpy(&mii_op_size, (((uint8_t *)ifd) + sizeof(*ifd)), sizeof(uint32_t));
    }

    if (mii_op_size % sizeof(mdi_op) != 0) {
        return EINVAL;
    }
    if (mii_op_size == 0) {
        return EINVAL;
    }

    if (strcmp (ifd->ifd_name, rswitch2->dev.dv_xname)) {
        return EINVAL;
    }

    /*lock the mutex for atomic */
    length = 0;
    nic_mutex_lock(&rswitch2->mdi_mutex);
    while (length < mii_op_size) {

        /* get the mii_op data*/
        if (ISSTACK) {
            if (copyin((((uint8_t *)ifd) + sizeof(*ifd)) + sizeof(mii_op_size) + length,
                &mdi_op, sizeof(mdi_op))) {
                    nic_mutex_unlock(&rswitch2->mdi_mutex);
                    return EINVAL;
            }
        } else {
            memcpy(&mdi_op, (((uint8_t *)ifd) + sizeof(*ifd)) + sizeof(mii_op_size)+ length, sizeof(mdi_op));
        }

        switch(mdi_op.mii_op.op) {
            case SLEEP:
                if (cfg->verbose > 5) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s(): op SLEEP:  %d\n", __func__, mdi_op.mii_op.data);
                }
                nic_delay(mdi_op.mii_op.data);
                mdi_op.mii_op.err = 0;
                break;
            case READ:
                mdi_op.mii_op.data = rswitch2_mdi_read(rswitch2, mdi_op.mii_op.addr, mdi_op.mii_op.reg);
                mdi_op.mii_op.err  = 0;
                if (cfg->verbose > 5) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,  "%s(): op READ:%x,%x,0x%x\n", __func__, mdi_op.mii_op.addr, mdi_op.mii_op.reg, mdi_op.mii_op.data);
                }
                break;
            case WRITE:
                rswitch2_mdi_write(rswitch2, mdi_op.mii_op.addr, mdi_op.mii_op.reg, mdi_op.mii_op.data);
                mdi_op.mii_op.err = 0;
                if (cfg->verbose > 5) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,  "%s(): op WRITE:  %x,%x,0x%x\n", __func__, mdi_op.mii_op.addr, mdi_op.mii_op.reg, mdi_op.mii_op.data);
                }
                break;
            case CL45_READ:
                mdi_op.cl45_op.phy_data = rswitch2_mii_read_c45(rswitch2, mdi_op.cl45_op.phy_id, mdi_op.cl45_op.phy_dev, mdi_op.cl45_op.phy_reg);
                mdi_op.cl45_op.err = 0;
                if (cfg->verbose > 5) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,  "%s(): op CL45_READ:  %x,%x,%x,0x%x\n", __func__, mdi_op.cl45_op.phy_id, mdi_op.cl45_op.phy_reg, mdi_op.cl45_op.phy_dev, mdi_op.cl45_op.phy_data);
                }
                break;
            case CL45_WRITE:
                rswitch2_mii_write_c45(rswitch2, mdi_op.cl45_op.phy_id, mdi_op.cl45_op.phy_dev, mdi_op.cl45_op.phy_reg, mdi_op.cl45_op.phy_data);
                mdi_op.cl45_op.err = 0;
                if (cfg->verbose > 5) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,  "%s(): op CL45_WRITE:  %x,%x,%x,0x%x\n", __func__, mdi_op.cl45_op.phy_id, mdi_op.cl45_op.phy_reg, mdi_op.cl45_op.phy_dev, mdi_op.cl45_op.phy_data);
                }
                break;
            case SW_READ:
                mdi_op.mii_op.data = rswitch2_mdi_read(rswitch2, mdi_op.mii_op.addr, mdi_op.mii_op.reg);
                mdi_op.mii_op.err  = 0;
                if (cfg->verbose > 5) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,  "%s(): op SW_READ:%x,%x,0x%x\n", __func__, mdi_op.mii_op.addr, mdi_op.mii_op.reg, mdi_op.mii_op.data);
                }
                break;
            case SW_WRITE:
                rswitch2_mdi_write(rswitch2, mdi_op.mii_op.addr, mdi_op.mii_op.reg, mdi_op.mii_op.data);
                mdi_op.mii_op.err = 0;
                if (cfg->verbose > 5) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,  "%s(): op SW_WRITE:  %x,%x,0x%x\n", __func__, mdi_op.mii_op.addr, mdi_op.mii_op.reg, mdi_op.mii_op.data);
                }
                break;
            default:
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,  "%s(): unkown mii_op  %d\n", __func__, mdi_op.mii_op.op);
                mdi_op.mii_op.err = -1;
                nic_mutex_unlock(&rswitch2->drv_mutex);
                return EINVAL;
        }

        if (ISSTACK) {
            err = copyout(&mdi_op, (((uint8_t *)ifd) + sizeof(*ifd)) + sizeof(mii_op_size) + length, sizeof(mdi_op));
            if (err!=EOK) {
                nic_mutex_unlock(&rswitch2->mdi_mutex);
                return err;
            }
        } else {
            memcpy((((uint8_t *)ifd) + sizeof(*ifd)) + sizeof(mii_op_size) + length, &mdi_op, sizeof(mdi_op));
        }

        length += sizeof(mdi_op);
    }

    nic_mutex_unlock(&rswitch2->mdi_mutex);

    return EOK;

}



int rswitch2_ioctl (struct ifnet *ifp, unsigned long cmd, caddr_t data)
{
    rswitch2_dev_t           *rswitch2;
    int                     error;
    struct ifdrv_com        *ifdc;
    struct drvcom_config    *dcfgp;
    struct drvcom_stats     *dstp;
    struct ifreq            *ifr;
    struct ifdrv            *ifd;

    rswitch2 = ifp->if_softc;
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
                    memcpy(&dcfgp->dcom_config, &rswitch2->cfg, sizeof(rswitch2->cfg));
                    break;

                case DRVCOM_STATS:
                    dstp = (struct drvcom_stats *)ifdc;

                    if (ifdc->ifdc_len != sizeof(nic_stats_t)) {
                        error = EINVAL;
                        break;
                    }

                    rswitch2_update_stats(rswitch2);
                    memcpy(&dstp->dcom_stats, &rswitch2->stats,
                    sizeof(dstp->dcom_stats));
                    break;

                default:
                    error = EOPNOTSUPP;
                    break;
            }
            break;

        case SIOCSIFMEDIA:
            /* TODO: Set net media */
            if (rswitch2_is_br_phy(rswitch2)) {
                error = ENOTTY;
                break;
            }

        /* Fall through for normal PHY */
        case SIOCGIFMEDIA:

            ifr = (struct ifreq *)data;
            struct ifmediareq *ifmr = (struct ifmediareq *) ifr;

            if (rswitch2_is_br_phy(rswitch2)) {
                switch (rswitch2->cfg.media_rate) {
                    case    100 * 1000:
                        ifmr->ifm_current = IFM_ETHER | IFM_FDX | IFM_100_T1;
                        break;
                    case    1000 * 1000:
                        ifmr->ifm_current = IFM_ETHER | IFM_FDX | IFM_1000_T1;
                        break;
                    default:
                        ifmr->ifm_current = IFM_ETHER | IFM_NONE;
                        break;
                }
                ifmr->ifm_active = ifmr->ifm_current;
                ifmr->ifm_count = 0;
                ifmr->ifm_status = IFM_AVALID;
                if ((rswitch2->cfg.flags & NIC_FLAG_LINK_DOWN) == 0) {
                    ifmr->ifm_status |= IFM_ACTIVE;
                }
            } else {
                 error = ifmedia_ioctl(ifp, ifr, &rswitch2->bsd_mii.mii_media, cmd);
            }

            /* TODO: Set net media */
            break;

        case SIOCSIFMTU:
            ifr = (struct ifreq *)data;
            /* Hardware only supports 1500 MTU: see RFLR register for details */
            if (ifr->ifr_mtu == ETHERMTU) {
                error = EOK;
            } else {
                error = EINVAL;
            }
            break;

        case SIOCSDRVSPEC:
        case SIOCGDRVSPEC:
            ifd = (struct ifdrv *)data;
             switch (ifd->ifd_cmd) {
                case GENERIC_MII_OP:
                    error = rswitch2_mii_op_ioctl(rswitch2, ifd);
                    break;
                default:
                    error = rswitch2_ptp_ioctl(rswitch2, ifd);
                    break;
            }
            break;
        /*
        * Deliberate fallthrough for other flags
        * handling in ether_ioctl.
        */
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
