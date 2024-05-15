/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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

#include "rtsn.h"

#if (_NTO_VERSION < 660)
#include <drvr/mdi.h>
#else
#include <netdrvr/mdi.h>
#endif

#include "m88q2110.h"

uint16_t rtsnes_mii_read(void *handle, uint8_t phy_id, uint8_t location)
{
    rtsnes_dev_t  *rtsnes = handle;
    uint16_t      reg_val;
    int           i;

    rtsnes_rmac_write(rtsnes, TSNRMAC_MPSM, TSNRMAC_MPSM_PSME | (phy_id << 3) | (location << 8));

    i = TSNES_TIMEOUT_MS;
    while ((rtsnes_rmac_read(rtsnes, TSNRMAC_MPSM) & TSNRMAC_MPSM_PSME)) { // Wait for finish PHY register access
        if(--i == 0) {
            return -1;
        }
        nanospin_ns(1000);
    }

    rtsnes_rmac_write(rtsnes, TSNRMAC_MMIS1, TSNRMAC_MMIS1_PRACS);

	reg_val = (rtsnes_rmac_read(rtsnes, TSNRMAC_MPSM) >> 16) ;

    return reg_val;
}

void rtsnes_mii_write(void *handle, uint8_t phy_id, uint8_t location, uint16_t data)
{
    rtsnes_dev_t  *rtsnes = handle;
    int           i;

    rtsnes_rmac_write(rtsnes, TSNRMAC_MPSM, TSNRMAC_MPSM_PSME | TSNRMAC_MPSM_PSMAD_WRITE 
                                     | (phy_id << 3) | (location << 8) | (data << 16));

	i = TSNES_TIMEOUT_MS;
    while ((rtsnes_rmac_read(rtsnes, TSNRMAC_MPSM) & TSNRMAC_MPSM_PSME)) { // Wait for finish PHY register access
        if(--i == 0) {
            return;
        }
        nanospin_ns(1000);
    }

    rtsnes_rmac_write(rtsnes, TSNRMAC_MMIS1, TSNRMAC_MMIS1_PWACS);
}

/*****************************************************************************/
/* Clause 45 MDI write                                                       */
/*****************************************************************************/
void rtsnes_mii_write_cl45 (void *handle, uint8_t phyad, uint16_t devad, uint16_t regad, uint16_t data)
{
    rtsnes_mii_write (handle, phyad, MMD_ACCESS_CTRL, devad);
    rtsnes_mii_write (handle, phyad, MMD_ADDR_DATA, regad);
    rtsnes_mii_write (handle, phyad, MMD_ACCESS_CTRL, MMD_CTRL_DATA | devad);
    rtsnes_mii_write (handle, phyad, MMD_ADDR_DATA, data);
}

/*****************************************************************************/
/* Clause 45 MDI read                                                        */
/*****************************************************************************/
uint16_t rtsnes_mii_read_cl45 (void *handle, uint8_t phyad, uint16_t devad, uint16_t regad)
{
    rtsnes_dev_t  *rtsnes = handle;
    nic_config_t  *cfg = &rtsnes->cfg;
    uint16_t val;

    rtsnes_mii_write (handle, phyad, MMD_ACCESS_CTRL, devad);
    rtsnes_mii_write (handle, phyad, MMD_ADDR_DATA, regad);
    rtsnes_mii_write (handle, phyad, MMD_ACCESS_CTRL, MMD_CTRL_DATA | devad);

    val = rtsnes_mii_read (handle, phyad, MMD_ADDR_DATA);

    if (cfg->verbose & VERBOSE_PHY) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: PHY dev 0x%02x reg 0x%04x = 0x%04x", devad, regad, val);
    }

    return val;
}

/*
 * Check for link up/down on Marvell PHY
 */
static void rtsnes_do_mv_monitor(rtsnes_dev_t *rtsnes)
{
    int             phy_idx = rtsnes->cfg.phy_addr;
    int             linkup = 0;
    uint16_t        sts, ana;
    nic_config_t    *cfg;
    struct ifnet    *ifp;

    cfg = &rtsnes->cfg;
    ifp = &rtsnes->ecom.ec_if;

    nic_mutex_lock(&rtsnes->mdi_mutex);
    sts = rtsnes_mii_read_cl45 (rtsnes, phy_idx, 3, GIGE_T1_STATUS_REG);
    ana = rtsnes_mii_read_cl45 (rtsnes, phy_idx, 7, AUTONEG_STATUS_REG);
    nic_mutex_unlock(&rtsnes->mdi_mutex);

    if(sts & GIGE_T1_STATUS_REG_LINK_STATUS) {
        if((ana & AUTONEG_STATUS_BOTH_RX_STATUS) == AUTONEG_STATUS_BOTH_RX_STATUS) {
            linkup = 1;
        }
    }

    linkup |= rtsnes->internal_loopback; // for internal_loopback debug

    if ((cfg->flags & NIC_FLAG_LINK_DOWN) && linkup) {
        /* Link was down and is now up */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(): link up", __FUNCTION__);
        }
        cfg->flags &= ~NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_UP);
        cfg->media_rate = PHY_SPEED_1000M;
        cfg->duplex = 1;
        rtsnes_set_speed(rtsnes, cfg->media_rate);
    } else if (((cfg->flags & NIC_FLAG_LINK_DOWN) == 0) && (linkup == 0)) {
        /* Link was up and is now down */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(): link down", __FUNCTION__);
        }
        cfg->flags |= NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_DOWN);
    }
}

static void rtsnes_mii_callback(void *handle, uint8_t phyaddr, uint8_t linkstate)
{
    rtsnes_dev_t  *rtsnes;
    struct ifnet    *ifp;
    nic_config_t    *cfg;
    char            *s, *f;
    int             i, mode;

    rtsnes = (rtsnes_dev_t *)handle;
    cfg = &rtsnes->cfg;
    ifp = &rtsnes->ecom.ec_if;

    switch (linkstate) {
        case MDI_LINK_UP:
            /* Link came up find out what speed duplex etc */
            i = MDI_GetActiveMedia(rtsnes->mdi, cfg->phy_addr, &mode);
            if (i != MDI_LINK_UP) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "rtsnes:Link up but unknown media, ignoring");
                return;
            }

            switch (mode) {
                case MDI_100bT:
                    cfg->media_rate = PHY_SPEED_100M;
                    cfg->duplex = 0;
                    s = "100baseTX";
                    break;

                case MDI_100bTFD:
                    cfg->media_rate = PHY_SPEED_100M;
                    cfg->duplex = 1;
                    s = "100baseTX full-duplex";
                    break;

                case MDI_1000bT:
                    cfg->media_rate = PHY_SPEED_1000M;
                    cfg->duplex = 0;
                    s = "1000baseT";
                    break;

                case MDI_1000bTFD:
                    cfg->media_rate = PHY_SPEED_1000M;
                    cfg->duplex = 1;
                    s = "1000baseT full-duplex";
                    break;

                default:
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                          "rtsnes: Ignoring unknown media or rtsnes does not supported");
                    return;
            }

            /* Sort out flow control */
            rtsnes->flow_status = 0;
            f = "";
            if (cfg->duplex == 1) {
                switch (rtsnes->set_flow) {
                    case -1:
                        break;

                    case 1:
                        rtsnes->flow_status = IFM_FLOW;
                        f = "flowcontrol";
                        break;

                    case 2:
                        rtsnes->flow_status = IFM_ETH_RXPAUSE;
                        f = "rxpause";
                        break;

                    case 3:
                        rtsnes->flow_status = IFM_ETH_TXPAUSE;
                        f = "txpause";
                        break;
                }
            }

            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: %s Link up %s %s",
                      ifp->if_xname, s, f);
            }

            cfg->flags &= ~NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_UP);
            
            /* Set speed (bps) */
            if (rtsnes_is_mv_phy(rtsnes) == 1) {
                rtsnes_set_speed(rtsnes, cfg->media_rate);
            }
            break;

        case MDI_LINK_DOWN:
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: %s Link down",
                      ifp->if_xname);
            }
            cfg->media_rate = cfg->duplex = -1;
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            MDI_AutoNegotiate(rtsnes->mdi, cfg->phy_addr, NoWait);
            if_link_state_change(ifp, LINK_STATE_DOWN);
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Unknown linkstate %d",
                  linkstate);
            break;
    }
}

static void monitor_switch_port_0 (rtsnes_dev_t *rtsnes)

{
    uint16_t        reg, spd;
    nic_config_t    *cfg = &rtsnes->cfg;
    struct ifnet    *ifp = &rtsnes->ecom.ec_if;

    reg = rtsnes_mii_read (rtsnes, 0, PORT_STATUS);
    if ((cfg->flags & NIC_FLAG_LINK_DOWN) && (reg & SW_LINK_STATUS)) {
        /* Link was down and is now up */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK,_SLOG_INFO, "%s(): link up", __FUNCTION__);
        }
        cfg->flags &= ~NIC_FLAG_LINK_DOWN;
        spd = (reg & SPEED_MASK) >> SPEED_SHIFT;
        switch (spd) {
            case    0:
                cfg->media_rate = PHY_SPEED_10M;
                break;
            case    1:
                cfg->media_rate = PHY_SPEED_100M;
                break;
            case    2:
                cfg->media_rate = PHY_SPEED_1000M;
                break;
            default:
                cfg->media_rate = -1;
                break;
        }
        cfg->duplex = (reg & DUPLEX_MODE) ? 1 : 0;
        if_link_state_change(ifp, LINK_STATE_UP);
        rtsnes->phy_regs[MDI_BMSR] |= (BMSR_LINK_STATUS | BMSR_AN_COMPLETE);
    } else if (((cfg->flags & NIC_FLAG_LINK_DOWN) == 0) && ((reg & SW_LINK_STATUS) == 0)) {
        /* Link was up and is now down */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(): link down", __FUNCTION__);
        }
        cfg->media_rate = -1;
        cfg->duplex = -1;
        cfg->flags |= NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_DOWN);
        rtsnes->phy_regs[MDI_BMSR] &= ~(BMSR_LINK_STATUS | BMSR_AN_COMPLETE);
    }
}

static void rtsnes_do_mdi_monitor(void* arg)
{
    rtsnes_dev_t *rtsnes = arg;

    MDI_MonitorPhy(rtsnes->mdi);

    if (rtsnes_is_mv_phy(rtsnes) == 1) {
        rtsnes_do_mv_monitor(rtsnes);
    }
    else {
       monitor_switch_port_0(rtsnes);
    }

    kthread_exit(0);
}

void rtsnes_MDI_MonitorPhy(void *arg)
{
    rtsnes_dev_t   *rtsnes;
    nic_config_t   *cfg;

    rtsnes = arg;
    cfg = &rtsnes->cfg;

    if (((cfg->flags & NIC_FLAG_LINK_DOWN) != 0) || !rtsnes->pkts_received) {
        if (cfg->verbose & VERBOSE_PHY) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Check port link state");
        }
        kthread_create1(rtsnes_do_mdi_monitor, arg, NULL, NULL);
    }

    rtsnes->pkts_received = 0;

    /*
     * With 32 bit counters and 100Mb/s max pps the stats max out in 8 hours.
     * Without an interrupt to warn of approaching max, we need to poll
     * faster than that. We already have a 3 second poller here and the
     * poll of the stats is lightweight, 16 reads and writes, so we poll
     * here rather than setting up a new poller.
     */
    rtsnes_update_stats(rtsnes);

    callout_msec(&rtsnes->mii_callout, 3 * 1000, rtsnes_MDI_MonitorPhy, arg);
}

static void rtsnes_mediastatus(struct ifnet *ifp, struct ifmediareq *ifmr)
{
    rtsnes_dev_t      *rtsnes;

    rtsnes = ifp->if_softc;

    rtsnes->bsd_mii.mii_media_active = IFM_ETHER;
    rtsnes->bsd_mii.mii_media_status = IFM_AVALID;

    if ((rtsnes->cfg.flags & NIC_FLAG_LINK_DOWN) != 0) {
        rtsnes->bsd_mii.mii_media_active |= IFM_NONE;
    } else {
        rtsnes->bsd_mii.mii_media_status |= IFM_ACTIVE;

        switch (rtsnes->cfg.media_rate) {
            case PHY_SPEED_10M:
                rtsnes->bsd_mii.mii_media_active |= IFM_10_T;
                break;

            case PHY_SPEED_100M:
                rtsnes->bsd_mii.mii_media_active |= IFM_100_TX;
                break;

            case PHY_SPEED_1000M:
                rtsnes->bsd_mii.mii_media_active |= IFM_1000_T;
                break;

            default:
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "rtsnes: Unknown media, forcing none");
            /* Fallthrough */
            case 0:
                rtsnes->bsd_mii.mii_media_active |= IFM_NONE;
                break;
        }

        if (rtsnes->cfg.duplex) {
            rtsnes->bsd_mii.mii_media_active |= IFM_FDX;
        }

        rtsnes->bsd_mii.mii_media_active |= rtsnes->flow_status;
    }

    /* Return the data */
    ifmr->ifm_status = rtsnes->bsd_mii.mii_media_status;
    ifmr->ifm_active = rtsnes->bsd_mii.mii_media_active;
}

int rtsnes_mediachange(struct ifnet *ifp)
{
    rtsnes_dev_t   *rtsnes;
    struct ifmedia  *ifm;
    nic_config_t    *cfg;
    int             an_media;

    if (!(ifp->if_flags & IFF_UP)) {
        return EOK;
    }

    rtsnes = ifp->if_softc;
    ifm = &rtsnes->bsd_mii.mii_media;
    cfg = &rtsnes->cfg;

    /* Media is changing so link will be down until autoneg completes */
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    switch (ifm->ifm_media & IFM_TMASK) {
        case IFM_NONE:
            rtsnes->set_speed = 0;
            rtsnes->set_duplex = 0;
            rtsnes->set_flow = 0;

            /* Special case, shut down the PHY and bail out */
            callout_stop(&rtsnes->mii_callout);
            MDI_DisableMonitor(rtsnes->mdi);
            MDI_PowerdownPhy(rtsnes->mdi, cfg->phy_addr);
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_DOWN);
            return EOK;

        case IFM_AUTO:
            rtsnes->set_speed = -1;
            rtsnes->set_duplex = -1;
            rtsnes->set_flow = -1;

            MDI_GetMediaCapable(rtsnes->mdi, cfg->phy_addr, &an_media);
            an_media |= MDI_FLOW | MDI_FLOW_ASYM;
            break;

        case IFM_100_TX:
            rtsnes->set_speed = PHY_SPEED_100M;
            rtsnes->set_duplex = 0;
            rtsnes->set_flow = 0;

            if ((ifm->ifm_media & IFM_FDX) == 0) {
                an_media = MDI_100bT;
            }
            else {
                rtsnes->set_duplex = 1;
                an_media = MDI_100bTFD;
                if (ifm->ifm_media & IFM_FLOW) {
                    rtsnes->set_flow = 1;
                    an_media |= MDI_FLOW;
                }
                else if (ifm->ifm_media & IFM_ETH_RXPAUSE) {
                    rtsnes->set_flow = 2;
                    an_media |= MDI_FLOW | MDI_FLOW_ASYM;
                }
                else if (ifm->ifm_media & IFM_ETH_TXPAUSE) {
                    rtsnes->set_flow = 3;
                    an_media |= MDI_FLOW_ASYM;
                }
            }
            break;

        case IFM_1000_T:
            rtsnes->set_speed = PHY_SPEED_1000M;
            rtsnes->set_duplex = 0;
            rtsnes->set_flow = 0;

            if ((ifm->ifm_media & IFM_FDX) == 0) {
                an_media = MDI_1000bT;
            }
            else {
                rtsnes->set_duplex = 1;
                an_media = MDI_1000bTFD;
                if (ifm->ifm_media & IFM_FLOW) {
                    rtsnes->set_flow = 1;
                    an_media |= MDI_FLOW;
                }
                else if (ifm->ifm_media & IFM_ETH_RXPAUSE) {
                    rtsnes->set_flow = 2;
                    an_media |= MDI_FLOW | MDI_FLOW_ASYM;
                }
                else if (ifm->ifm_media & IFM_ETH_TXPAUSE) {
                    rtsnes->set_flow = 3;
                    an_media |= MDI_FLOW_ASYM;
                }
            }
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Unknown media type");
            return 1;
    }

    MDI_PowerupPhy(rtsnes->mdi, cfg->phy_addr);
    MDI_EnableMonitor(rtsnes->mdi, 0);
    MDI_SetAdvert(rtsnes->mdi, cfg->phy_addr, an_media);
    MDI_AutoNegotiate(rtsnes->mdi, cfg->phy_addr, NoWait);
    rtsnes_MDI_MonitorPhy (rtsnes);

    return EOK;
}

static void rtsnes_mediainit(rtsnes_dev_t *rtsnes)
{
    nic_config_t    *cfg;
    struct ifmedia  *ifm;

    cfg = &rtsnes->cfg;
    ifm = &rtsnes->bsd_mii.mii_media;

    rtsnes->bsd_mii.mii_ifp = &rtsnes->ecom.ec_if;

    ifmedia_init(ifm, IFM_IMASK, rtsnes_mediachange, rtsnes_mediastatus);
    ifmedia_add(ifm, IFM_ETHER | IFM_NONE, 0, NULL);

    ifmedia_add(ifm, IFM_ETHER | IFM_AUTO, 0, NULL);

    ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX | IFM_ETH_TXPAUSE, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX | IFM_ETH_RXPAUSE, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX | IFM_FLOW, 0, NULL);

    ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX | IFM_FLOW, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX | IFM_ETH_TXPAUSE, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX | IFM_ETH_RXPAUSE, 0, NULL);

    /*
     * nic_parse_options() sets speed / duplex in cfg but those are for
     * reporting state. Copy them across to the right place.
     */
    rtsnes->set_speed = cfg->media_rate;
    rtsnes->set_duplex = cfg->duplex;
    cfg->media_rate = 0;
    cfg->duplex = 0;

    switch (rtsnes->set_speed) {
        case -1:
            ifm->ifm_media = IFM_ETHER | IFM_AUTO;
            break;

        case PHY_SPEED_100M:
            ifm->ifm_media = IFM_ETHER | IFM_100_TX;
            if (rtsnes->set_duplex != 0) {
                ifm->ifm_media |= IFM_FDX;
                switch (rtsnes->set_flow) {
                    case 1:
                        ifm->ifm_media |= IFM_FLOW;
                        break;
                    case 2:
                        ifm->ifm_media |= IFM_ETH_RXPAUSE;
                        break;
                    case 3:
                        ifm->ifm_media |= IFM_ETH_TXPAUSE;
                        break;
                    default:
                        break;
                }
            }
            break;

        case PHY_SPEED_1000M:
            ifm->ifm_media = IFM_ETHER | IFM_1000_T;
            if (rtsnes->set_duplex != 0) {
                ifm->ifm_media |= IFM_FDX;
                switch (rtsnes->set_flow) {
                    case 1:
                        ifm->ifm_media |= IFM_FLOW;
                        break;
                    case 2:
                        ifm->ifm_media |= IFM_ETH_RXPAUSE;
                        break;
                    case 3:
                        ifm->ifm_media |= IFM_ETH_TXPAUSE;
                        break;
                    default:
                        break;
                }
            }
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rtsnes: Unknown initial media, forcing none");
        /* Fallthrough */

        case 0:
            ifm->ifm_media = IFM_ETHER | IFM_NONE;
            break;
    }

    ifmedia_set(ifm, ifm->ifm_media);
}

int rtsnes_phy_init(rtsnes_dev_t *rtsnes)
{
    nic_config_t *cfg;
    struct ifnet *ifp;
    int          rc, i;
    int          phy_addr = -1, scan_cnt = 1;

    cfg = &rtsnes->cfg;
    ifp = &rtsnes->ecom.ec_if;

    /* Scan Phy */
    for (;;) {
        if (scan_cnt) {
            rc = MDI_Register_ExtendedCl45(rtsnes, rtsnes_mii_write_cl45,
                        rtsnes_mii_read_cl45, rtsnes_mii_callback, &rtsnes->mdi, NULL, 0, 0);
        } else {
            rc = MDI_Register_Extended(rtsnes, rtsnes_mii_write,
                    rtsnes_mii_read, rtsnes_mii_callback, &rtsnes->mdi, NULL, 0, 0);
        }

        if (rc != MDI_SUCCESS) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rtsnes: Failed to register with MDI");
            return (ENODEV);
        }

        for (i = 0; i < 32; i++) {
            if (MDI_FindPhy (rtsnes->mdi, i) == MDI_SUCCESS) {
                if (cfg->verbose) {
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: PHY at address %d", i);
                }
                phy_addr = i;
                break;
            }
        }

        if ((phy_addr != -1) || (--scan_cnt < 0)) break;
    }

    if (phy_addr == -1) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "Failed to find PHY");
        return ENODEV;
    }
    cfg->phy_addr = phy_addr;

    rc = MDI_InitPhy(rtsnes->mdi, cfg->phy_addr);
    if (rc != MDI_SUCCESS) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed to init the PHY");
        return ENODEV;
    }

    if (rtsnes_is_mv_phy(rtsnes) != 1) {
        rc = MDI_ResetPhy(rtsnes->mdi, cfg->phy_addr, WaitBusy);
        if (rc != MDI_SUCCESS) {
            slogf(_SLOGC_NETWORK,_SLOG_ERROR, "%s(): Failed to reset the PHY: %d",
                __FUNCTION__, rc);
            return rc;
        }
        MDI_PowerdownPhy(rtsnes->mdi, cfg->phy_addr);
    } else {
        if (cfg->verbose & VERBOSE_PHY)
            slogf(_SLOGC_NETWORK,_SLOG_INFO, "Detected Marvell 88Q2110 PHY");
        rtsnes_mv_phy_init(rtsnes);
    }

    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    rtsnes_mediainit(rtsnes);

    return EOK;
}

void rtsnes_phy_fini(rtsnes_dev_t *rtsnes)
{
    if (rtsnes_is_mv_phy(rtsnes) == 1) {
        rtsnes_mv_phy_fini(rtsnes);
    }

    ifmedia_delete_instance(&rtsnes->bsd_mii.mii_media, IFM_INST_ANY);

    MDI_DeRegister(&rtsnes->mdi);
    rtsnes->mdi = NULL;
}

int rtsnes_is_br_phy(rtsnes_dev_t *rtsnes)
{
    uint32_t PhyAddr;
    int		 is_br = 0;

    PhyAddr = rtsnes->cfg.phy_addr;
    switch (rtsnes->mdi->PhyData[PhyAddr]->VendorOUI) {
    case MARVELLX:
        switch (rtsnes->mdi->PhyData[PhyAddr]->Model) {
            case MV88Q2110:
                is_br = 1;
                break;
        }
        break;
    }

    return is_br;
}
