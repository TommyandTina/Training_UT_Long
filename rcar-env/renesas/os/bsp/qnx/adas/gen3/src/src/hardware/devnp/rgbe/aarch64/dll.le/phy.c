/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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

static void gether_mii_callback(void *handle, uint8_t phyaddr, uint8_t linkstate)
{
    gether_dev_t      *gether;
    struct ifnet    *ifp;
    nic_config_t    *cfg;
    char            *s, *f;
    int             i, mode;
    uint16_t        lpadvert;
    uint32_t        val;

    gether = handle;
    cfg = &gether->cfg;
    ifp = &gether->ecom.ec_if;

    switch (linkstate) {
        case MDI_LINK_UP:
            /* Link came up find out what speed duplex etc */
            i = MDI_GetActiveMedia(gether->mdi, cfg->phy_addr, &mode);
            if (i != MDI_LINK_UP) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "gether:Link up but unknown media, ignoring");
                return;
            }

            switch (mode) {
                case MDI_10bT:
                    cfg->media_rate = 10000L;
                    cfg->duplex = 0;
                    s = "10baseT";
                    break;

                case MDI_10bTFD:
                    cfg->media_rate = 10000L;
                    cfg->duplex = 1;
                    s = "10baseT full-duplex";
                    break;
                case MDI_100bT:
                    cfg->media_rate = 100000L;
                    cfg->duplex = 0;
                    s = "100baseTX";
                    break;

                case MDI_100bTFD:
                    cfg->media_rate = 100000L;
                    cfg->duplex = 1;
                    s = "100baseTX full-duplex";
                    break;

                case MDI_1000bT:
                    cfg->media_rate = 1000000L;
                    cfg->duplex = 0;
                    s = "1000baseT";
                    break;

                case MDI_1000bTFD:
                    cfg->media_rate = 1000000L;
                    cfg->duplex = 1;
                    s = "1000baseT full-duplex";
                    break;

                default:
                    cfg->duplex = 0;
                    cfg->media_rate = 0L;
                    s = "Unknown";
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                          "gether:Ignoring unknown media or gether does not supported");
                    return;
            }

            /* Sort out flow control */
            gether->flow_status = 0;
            f = "";

            switch (gether->set_flow) {
                case FLOW_AUTO:
                    /* Flow set to autoneg, check the partner */
                    lpadvert = gether_mii_read(gether, cfg->phy_addr, MDI_ANLPAR);
                    if (lpadvert & MDI_FLOW) {
                        gether->flow_status = IFM_FLOW;
                        f = "flowcontrol";
                    }
                    else if (lpadvert & MDI_FLOW_ASYM) {
                        gether->flow_status = IFM_ETH_RXPAUSE;
                        f = "rxpause";
                    }
                    break;

                case FLOW_BOTH:
                    gether->flow_status = IFM_FLOW;
                    f = "flowcontrol";
                    break;

                case FLOW_RX:
                    gether->flow_status = IFM_ETH_RXPAUSE;
                    f = "rxpause";
                    break;
                case FLOW_TX:
                    gether->flow_status = IFM_ETH_TXPAUSE;
                    f = "txpause";
                    break;
                default:
                    break;
            }

            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: %s Link up %s %s", ifp->if_xname, s, f);
            }

            cfg->flags &= ~NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_UP);

            /* Program speed/duplex/flowcontrol settings in to the MAC */
            out32(gether->base + GETHER_ECMR,
                    in32(gether->base + GETHER_ECMR) & ~(GETHER_ECMR_RE | GETHER_ECMR_TE));

            /* wait for the operation to complete */
            while(in32(gether->base + GETHER_ECMR) & (GETHER_ECMR_TE | GETHER_ECMR_RE)) {}

            val = in32(gether->base + GETHER_GECMR) & ~0x5;
            if (cfg->media_rate == 1000000L){
                val |= 0x1;
            } else if (cfg->media_rate == 100000L){
                val |= 0x4;
            }
            out32(gether->base + GETHER_GECMR, val);


            val = in32(gether->base + GETHER_ECMR);
            if (cfg->duplex) {
                val |= GETHER_ECMR_DM;
            } else {
                val &= ~GETHER_ECMR_DM;
            }
            val &= ~(GETHER_ECMR_RXF | GETHER_ECMR_TXF);
            if (gether->flow_status == IFM_FLOW) {
                val |= (GETHER_ECMR_RXF | GETHER_ECMR_TXF);
            } else if (gether->flow_status == IFM_ETH_RXPAUSE) {
                val |= GETHER_ECMR_TXF;
            } else if (gether->flow_status == IFM_ETH_TXPAUSE) {
                val |= GETHER_ECMR_RXF;
            }
            val |= (GETHER_ECMR_RE | GETHER_ECMR_TE | GETHER_ECMR_CHG_DM);
            out32(gether->base + GETHER_ECMR, val);

            break;

        case MDI_LINK_DOWN:
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: %s Link down", ifp->if_xname);
            }
            cfg->media_rate = cfg->duplex = -1;
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            MDI_AutoNegotiate(gether->mdi, cfg->phy_addr, NoWait);
            if_link_state_change(ifp, LINK_STATE_DOWN);
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Unknown linkstate %d", linkstate);
            break;
    }
}

static void gether_monphy(void *arg)
{
    gether_dev_t      *gether;
    nic_config_t    *cfg;

    gether = arg;
    cfg = &gether->cfg;

    nic_mutex_lock(&gether->drv_mutex);

    if (((cfg->flags & NIC_FLAG_LINK_DOWN) != 0) || !gether->pkts_received) {
        if (cfg->verbose & VERBOSE_PHY) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Check link state");
        }
        MDI_MonitorPhy(gether->mdi);
    }

    gether->pkts_received = 0;

    /*
     * With 32 bit counters and 1000Mb/s max pps the stats max out in 48 minutes.
     * Without an interrupt to warn of approaching max, we need to poll
     * faster than that. We already have a 3 second poller here and the
     * poll of the stats is lightweight, 14 reads and writes, so we poll
     * here rather than setting up a new poller.
     */
    gether_update_stats(gether);

    nic_mutex_unlock(&gether->drv_mutex);

    atomic_clr(&gether->flags, FLAG_KTHREAD);
    kthread_exit(0);
}

void gether_MDI_MonitorPhy(void *arg)
{
    gether_dev_t *gether = arg;

    if (gether->flags & FLAG_DYING) {
        /* Dont schedule a monitor thread if the interface is getting deatched */
        return;
    }

    atomic_set(&gether->flags, FLAG_KTHREAD);
    kthread_create1(gether_monphy, arg, NULL, NULL);

    callout_msec(&gether->mii_callout, 3 * 1000, gether_MDI_MonitorPhy, arg);
}

static void gether_mediastatus(struct ifnet *ifp, struct ifmediareq *ifmr)
{
    gether_dev_t      *gether;

    gether = ifp->if_softc;

    gether->bsd_mii.mii_media_active = IFM_ETHER;
    gether->bsd_mii.mii_media_status = IFM_AVALID;

    if ((gether->cfg.flags & NIC_FLAG_LINK_DOWN) != 0) {
        gether->bsd_mii.mii_media_active |= IFM_NONE;
    } else {
        gether->bsd_mii.mii_media_status |= IFM_ACTIVE;

        switch (gether->cfg.media_rate) {
            case 10000L:
                gether->bsd_mii.mii_media_active |= IFM_10_T;
                break;

            case 100000L:
                gether->bsd_mii.mii_media_active |= IFM_100_TX;
                break;

            case 1000000L:
                gether->bsd_mii.mii_media_active |= IFM_1000_T;
                break;

            default:
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Unknown media, forcing none");
            /* Fallthrough */
            case 0:
                gether->bsd_mii.mii_media_active |= IFM_NONE;
                break;
        }

        if (gether->cfg.duplex) {
            gether->bsd_mii.mii_media_active |= IFM_FDX;
        }

        gether->bsd_mii.mii_media_active |= gether->flow_status;
    }

    /* Return the data */
    ifmr->ifm_status = gether->bsd_mii.mii_media_status;
    ifmr->ifm_active = gether->bsd_mii.mii_media_active;
}

int gether_mediachange(struct ifnet *ifp)
{
    gether_dev_t      *gether;
    struct ifmedia  *ifm;
    nic_config_t    *cfg;
    int             an_media;

    if (!(ifp->if_flags & IFF_UP)) {
        return 0;
    }

    gether = ifp->if_softc;
    ifm = &gether->bsd_mii.mii_media;
    cfg = &gether->cfg;

    /* Media is changing so link will be down until autoneg completes */
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    gether->set_duplex = 0;
    gether->set_flow = FLOW_NONE;

    switch (ifm->ifm_media & IFM_TMASK) {
        case IFM_NONE:
            gether->set_speed = 0;

            /* Special case, shut down the PHY and bail out */
            callout_stop(&gether->mii_callout);
            MDI_DisableMonitor(gether->mdi);
            MDI_PowerdownPhy(gether->mdi, cfg->phy_addr);
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_DOWN);
            return 0;

        case IFM_AUTO:
            gether->set_speed = -1;
            gether->set_duplex = -1;
            gether->set_flow = FLOW_AUTO;

            MDI_GetMediaCapable(gether->mdi, cfg->phy_addr, &an_media);
            /* Enable Pause in autoneg */
            if ((gether->mdi->PhyData[cfg->phy_addr]->VendorOUI == KENDIN) &&
            (gether->mdi->PhyData[cfg->phy_addr]->Model == KSZ9031)) {
                /* Bug in KSZ9031 PHY */
                an_media |= MDI_FLOW;
            } else {
                an_media |= MDI_FLOW | MDI_FLOW_ASYM;
            }
            break;
        case IFM_10_T:
            gether->set_speed = 10000L;
            if ((ifm->ifm_media & IFM_FDX) == 0) {
                gether->set_duplex = 0;
                an_media = MDI_10bT;
            } else {
                gether->set_duplex = 1;
                an_media = MDI_10bTFD;
            }
            break;

        case IFM_100_TX:
            gether->set_speed = 100000L;
            if ((ifm->ifm_media & IFM_FDX) == 0) {
                gether->set_duplex = 0;
                an_media = MDI_100bT;
            } else {
                gether->set_duplex = 1;
                an_media = MDI_100bTFD;
            }
            break;

        case IFM_1000_T:
            gether->set_speed = 1000000L;
            gether->set_duplex = 1;
            an_media = MDI_1000bTFD;
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Unknown media type");
            return 1;
    }

    if (ifm->ifm_media & IFM_FLOW) {
        gether->set_flow = FLOW_BOTH;
        an_media |= MDI_FLOW;
    } else if (ifm->ifm_media & IFM_ETH_TXPAUSE) {
        gether->set_flow = FLOW_TX;
        an_media |= MDI_FLOW_ASYM;
    } else if (ifm->ifm_media & IFM_ETH_RXPAUSE) {
        gether->set_flow = FLOW_RX;
        an_media |= MDI_FLOW_ASYM;
    }

    MDI_PowerupPhy(gether->mdi, cfg->phy_addr);

    if( (gether->mdi->PhyData[cfg->phy_addr]->VendorOUI == KENDIN) &&
        (gether->mdi->PhyData[cfg->phy_addr]->Model == KSZ9031)) {

        /*
         * After PHY goes from Power-down mode to Normal operation,
         * wait a minimum of 1ms before read/write access to the PHY registers.
         */
        nic_delay(1);

        /*
         * Errata: When device links in the 1000Base-T slave mode only, the optional
         * 128MHz reference output clock has wide duty cycle variation.
         * Solution: operate in master mode (when possible), 1000 Base-T capable
         */
        gether_mii_write(gether, gether->cfg.phy_addr, MDI_MSCR, (MSCR_ADV_1000bT |
            MSCR_ADV_1000bTFD | MSCR_PORT_TYPE | MSCR_CONFIG_VALUE | MSCR_MANUAL_CONFIG));
    }

    MDI_EnableMonitor(gether->mdi, 0);
    MDI_SetAdvert(gether->mdi, cfg->phy_addr, an_media);
    MDI_AutoNegotiate(gether->mdi, cfg->phy_addr, NoWait);
    gether_MDI_MonitorPhy(gether);

    return 0;
}

static void gether_mediainit(gether_dev_t *gether)
{
    nic_config_t    *cfg;
    struct ifmedia  *ifm;

    cfg = &gether->cfg;
    ifm = &gether->bsd_mii.mii_media;

    gether->bsd_mii.mii_ifp = &gether->ecom.ec_if;

    ifmedia_init(ifm, IFM_IMASK, gether_mediachange, gether_mediastatus);

    ifmedia_add(ifm, IFM_ETHER | IFM_NONE, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_AUTO, 0, NULL);

    ifmedia_add(ifm, IFM_ETHER | IFM_10_T, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_10_T | IFM_FDX, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_10_T | IFM_FDX | IFM_ETH_TXPAUSE, 0, NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_10_T | IFM_FDX | IFM_ETH_RXPAUSE, 0,NULL);
    ifmedia_add(ifm, IFM_ETHER | IFM_10_T | IFM_FDX | IFM_FLOW, 0, NULL);

    ifmedia_add(ifm, IFM_ETHER | IFM_100_TX, 0, NULL);
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
    gether->set_speed = cfg->media_rate;
    gether->set_duplex = cfg->duplex;
    cfg->media_rate = 0;
    cfg->duplex = 0;

    switch (gether->set_speed) {
        case -1:
            ifm->ifm_media = IFM_ETHER | IFM_AUTO;
            break;

        case 10 *1000L:
            ifm->ifm_media = IFM_ETHER | IFM_10_T;
            break;

        case 100 *1000L:
            ifm->ifm_media = IFM_ETHER | IFM_100_TX;
            break;

        case 1000 * 1000L:
            ifm->ifm_media = IFM_ETHER | IFM_1000_T;
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Unknown initial media, forcing none");
        /* Fallthrough */

        case 0:
            ifm->ifm_media = IFM_ETHER | IFM_NONE;
            break;
    }

    if (gether->set_speed > 0 && gether->set_duplex != 0) {
        ifm->ifm_media |= IFM_FDX;
        switch (gether->set_flow) {
            case FLOW_BOTH:
                ifm->ifm_media |= IFM_FLOW;
                break;
            case FLOW_RX:
                ifm->ifm_media |= IFM_ETH_RXPAUSE;
                break;
            case FLOW_TX:
                ifm->ifm_media |= IFM_ETH_TXPAUSE;
                break;
            default:
                break;
        }
    }

    ifmedia_set(ifm, ifm->ifm_media);
}

int gether_phy_init(gether_dev_t *gether)
{
    nic_config_t    *cfg;
    struct ifnet    *ifp;
    int             rc;

    cfg = &gether->cfg;
    ifp = &gether->ecom.ec_if;

    rc = MDI_Register_Extended(gether, gether_mii_write,
                gether_mii_read, gether_mii_callback, &gether->mdi, NULL, 0, 0);
    if (rc != MDI_SUCCESS) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to register with MDI");
        return (ENODEV);
    }

    for (cfg->phy_addr = 0; cfg->phy_addr < 32; cfg->phy_addr++) {
        if (MDI_FindPhy (gether->mdi, cfg->phy_addr) == MDI_SUCCESS) {
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: PHY at address %d",
                      cfg->phy_addr);
            }
            break;
        }
    }
    if (cfg->phy_addr == 32) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "Failed to find PHY");
        return ENODEV;
    }

    rc = MDI_InitPhy(gether->mdi, cfg->phy_addr);
    if (rc != MDI_SUCCESS) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to init the PHY");
        return ENODEV;
    }
    MDI_ResetPhy(gether->mdi, cfg->phy_addr, WaitBusy);
    MDI_PowerdownPhy(gether->mdi, cfg->phy_addr);
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    gether_mediainit(gether);

    return EOK;
}

void gether_phy_fini(gether_dev_t *gether)
{
    ifmedia_delete_instance(&gether->bsd_mii.mii_media, IFM_INST_ANY);

    MDI_DeRegister(&gether->mdi);
    gether->mdi = NULL;
}
void gether_MonitorPhy(void *arg)
{}

int gether_is_mv_phy (gether_dev_t *gether)
{
    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/aarch64/dll.le/phy.c $ $Rev: 870364 $")
#endif
