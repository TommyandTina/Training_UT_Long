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

#include "ravb.h"

#if (_NTO_VERSION < 660)
#include <drvr/mdi.h>
#else
#include <netdrvr/mdi.h>
#endif

/* MDIO bus is 2.5MHz so clock is 200ns high + 200ns low */
#define PHY_DELAY() nanospin_ns(200)

static inline void ravb_mdio_idle(ravb_dev_t *ravb)
{
    out32(ravb->base + PIR, 0);
}

static inline void ravb_mdio_release_bus(ravb_dev_t *ravb)
{
    out32(ravb->base + PIR, 0);
    out32(ravb->base + PIR, PIR_MDC);
    out32(ravb->base + PIR, 0);
}

static inline void ravb_mdio_write_bit(ravb_dev_t *ravb, uint8_t data)
{
    out32(ravb->base + PIR, PIR_MMD | (data ? PIR_MDO : 0));
    out32(ravb->base + PIR, PIR_MMD | PIR_MDC | (data ? PIR_MDO : 0));
    out32(ravb->base + PIR, PIR_MMD | (data ? PIR_MDO : 0));
}

static uint8_t ravb_mdio_read_bit(ravb_dev_t *ravb)
{
    uint32_t val;

    out32(ravb->base + PIR, PIR_MDC);
    PHY_DELAY();
    out32(ravb->base + PIR, 0);
    val = in32(ravb->base + PIR);

    return ((val & PIR_MDI) ? 1 : 0);
}

uint16_t ravb_mdi_read(void *handle, uint8_t phy_id, uint8_t location)
{
    ravb_dev_t  *ravb = (ravb_dev_t *) handle;
    int         i;
    uint16_t    val = 0;

    /* Preamble */
    for (i = 0; i < 32; i++) {
        ravb_mdio_write_bit(ravb, 1);
    }

    /* Start */
    ravb_mdio_write_bit(ravb, 0);
    ravb_mdio_write_bit(ravb, 1);

    /* Read */
    ravb_mdio_write_bit(ravb, 1);
    ravb_mdio_write_bit(ravb, 0);

    /* PHY Address */
    for (i = 4; i >=0; i--) {
        ravb_mdio_write_bit(ravb, (phy_id >> i) & 1);
    }

    /* Register */
    for (i = 4; i >=0; i--) {
        ravb_mdio_write_bit(ravb, (location >> i) & 1);
    }

    /* Turnaround */
    ravb_mdio_release_bus(ravb);

    /* Data */
    for (i = 15; i >=0; i--) {
        val |= (ravb_mdio_read_bit(ravb) << i);
    }

    if (ravb->options.cfg.verbose & VERBOSE_PHY) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: MDIO Read Phy %d Reg 0x%x Data 0x%x",
              phy_id, location, val);
    }

    return (val);
}

void ravb_mdi_write(void *handle, uint8_t phy_id, uint8_t location,
                            uint16_t data)
{
    ravb_dev_t      *ravb = (ravb_dev_t *) handle;
    int             i;

    /* Preamble */
    for (i = 0; i < 32; i++) {
        ravb_mdio_write_bit(ravb, 1);
    }

    /* Start */
    ravb_mdio_write_bit(ravb, 0);
    ravb_mdio_write_bit(ravb, 1);

    /* Write */
    ravb_mdio_write_bit(ravb, 0);
    ravb_mdio_write_bit(ravb, 1);

    /* PHY Address */
    for (i = 4; i >=0; i--) {
        ravb_mdio_write_bit(ravb, (phy_id >> i) & 1);
    }

    /* Register */
    for (i = 4; i >=0; i--) {
        ravb_mdio_write_bit(ravb, (location >> i) & 1);
    }

    /* Turnaround */
    ravb_mdio_write_bit(ravb, 1);
    ravb_mdio_write_bit(ravb, 0);

    /* Data */
    for (i = 15; i >=0; i--) {
        ravb_mdio_write_bit(ravb, (data >> i) & 1);
    }

    ravb_mdio_idle(ravb);

    if (ravb->options.cfg.verbose & VERBOSE_PHY) {
    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
          "ravb: MDIO Write Phy %d Reg 0x%x Data 0x%x",
          phy_id, location, data);
    }
}

uint16_t ravb_mii_read(void *handle, uint8_t phy_id, uint8_t location)

{
    ravb_dev_t      *ravb = (ravb_dev_t *) handle;

    if (ravb->options.emu_phy != -1 && ravb->options.emu_phy == ravb->options.cfg.device_index) {
        return (emu_phy_read (ravb, phy_id, location));
    }
    return ravb_mdi_read (ravb, phy_id, location);
}

void ravb_mii_write(void *handle, uint8_t phy_id, uint8_t location,
                            uint16_t data)
{
    ravb_dev_t      *ravb = (ravb_dev_t *) handle;

    if (ravb->options.emu_phy != -1 && ravb->options.emu_phy == ravb->options.cfg.device_index) {
        return;
    }
    ravb_mdi_write (ravb, phy_id, location, data);
}

/*****************************************************************************/
/* Clause 45 MDI write                                                       */
/*****************************************************************************/

void    ravb_cl45_write_reg (void *hdl, int phy_idx, int device, int reg, int val)

{
    ravb_dev_t  *ravb = (ravb_dev_t *) hdl;

    ravb_mdi_write (ravb, phy_idx, MMD_ACCESS_CTRL, device);
    ravb_mdi_write (ravb, phy_idx, MMD_ADDR_DATA, reg);
    ravb_mdi_write (ravb, phy_idx, MMD_ACCESS_CTRL, MMD_CTRL_DATA | device);
    ravb_mdi_write (ravb, phy_idx, MMD_ADDR_DATA, val);
}

/*****************************************************************************/
/* Clause 45 MDI read                                                        */
/*****************************************************************************/

uint16_t ravb_cl45_read_reg (void *hdl, int phy_idx, int device, int reg)

{
    ravb_dev_t  *ravb = (ravb_dev_t *) hdl;

    ravb_mdi_write (ravb, phy_idx, MMD_ACCESS_CTRL, device);
    ravb_mdi_write (ravb, phy_idx, MMD_ADDR_DATA, reg);
    ravb_mdi_write (ravb, phy_idx, MMD_ACCESS_CTRL, MMD_CTRL_DATA | device);
    return ravb_mdi_read (ravb, phy_idx, MMD_ADDR_DATA);
}

static void ravb_mii_callback(void *handle, uint8_t phyaddr, uint8_t linkstate)
{
    ravb_dev_t      *ravb;
    struct ifnet    *ifp;
    nic_config_t    *cfg;
    char            *s, *f;
    int             i, mode;
    uint16_t        lpadvert;
    uint32_t        val, val1;

    ravb = handle;
    cfg = &ravb->options.cfg;
    ifp = &ravb->ecom.ec_if;

    switch (linkstate) {
        case MDI_LINK_UP:
            /* Link came up find out what speed duplex etc */
            i = MDI_GetActiveMedia(ravb->mdi, cfg->phy_addr, &mode);
            if (i != MDI_LINK_UP) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "ravb:Link up but unknown media, ignoring");
                return;
            }

            switch (mode) {
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
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                          "ravb:Ignoring unknown media or RAVB does not supported");
                    return;
            }

            /* Sort out flow control */
            ravb->flow_status = 0;
            f = "";
            if (cfg->duplex == 1) {
                switch (ravb->options.set_flow) {
                    case -1:
                        /* Flow set to autoneg, check the partner */
                        lpadvert = ravb_mii_read(ravb, cfg->phy_addr,
                                                  MDI_ANLPAR);
                        if (lpadvert & MDI_FLOW) {
                            ravb->flow_status = IFM_FLOW;
                            f = "flowcontrol";
                        }
                        else if (lpadvert & MDI_FLOW_ASYM) {
                            ravb->flow_status = IFM_ETH_RXPAUSE;
                            f = "rxpause";
                        }
                        break;

                    case 1:
                        ravb->flow_status = IFM_FLOW;
                        f = "flowcontrol";
                        break;

                    case 2:
                        ravb->flow_status = IFM_ETH_RXPAUSE;
                        f = "rxpause";
                        break;

                    case 3:
                        ravb->flow_status = IFM_ETH_TXPAUSE;
                        f = "txpause";
                        break;
                }
            }

            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: %s Link up %s %s",
                      ifp->if_xname, s, f);
            }

            cfg->flags &= ~NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_UP);

            /* Program speed/duplex/flowcontrol settings in to the MAC */
            val = in32(ravb->base + ECMR);
            val &= ~(ECMR_RE | ECMR_TE);
            out32(ravb->base + ECMR, val);

            val1 = in32(ravb->base + GECMR);

            if (cfg->media_rate == 1000000L) {
                val1 |= GECMR_SPEED_1000;
            }
            else {
                val1 &= ~GECMR_SPEED_1000;
            }
            out32(ravb->base + GECMR, val1);

            if (cfg->duplex) {
                val |= ECMR_DM;
            }
            else {
                val &= ~ECMR_DM;
            }

            val &= ~(ECMR_RXF | ECMR_TXF);
            if (ravb->flow_status == IFM_FLOW) {
                val |= (ECMR_RXF | ECMR_TXF);
            }
            else if (ravb->flow_status == IFM_ETH_RXPAUSE) {
                val |= ECMR_TXF;
            }
            else if (ravb->flow_status == IFM_ETH_TXPAUSE) {
                val |= ECMR_RXF;
            }

            val |= (ECMR_RE | ECMR_TE);
            out32(ravb->base + ECMR, val);
            break;

        case MDI_LINK_DOWN:
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: %s Link down",
                      ifp->if_xname);
            }
            cfg->media_rate = cfg->duplex = -1;
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            MDI_AutoNegotiate(ravb->mdi, cfg->phy_addr, NoWait);
            if_link_state_change(ifp, LINK_STATE_DOWN);
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Unknown linkstate %d",
                  linkstate);
            break;
    }
}

static void ravb_ksz9031_phy_init(ravb_dev_t *ravb)
{
    uint16_t    reg;

    nic_mutex_lock(&ravb->drv_mutex);

    /*
     * After PHY goes from Power-down mode to Normal operation,
     * wait a minimum of 1ms before read/write access to the PHY registers.
     */
    nic_delay(1);

    /*
     * Errata: When device links in the 1000Base-T slave mode only, the optional
     * 128MHz reference output clock has wide duty cycle variation.
     * Solution: operate in master mode (when possible), 1000 Base-T capable
     * When 2 KSZ9031 PHYs are connected back-to-back, one needs to be set as
     * master and the other as slave.
     */
    reg = (ravb->options.slave) ? 0x1300 : 0x1e00;
    ravb_mii_write(ravb, ravb->options.cfg.phy_addr, 0x9, reg);

    nic_mutex_unlock(&ravb->drv_mutex);
}

/*
 * Check for link up/down on BroadR-Reach PHY
 */

static void ravb_br_phy_state(ravb_dev_t *ravb)
{
    uint16_t    val;
    nic_config_t    *cfg = &ravb->options.cfg;
    struct ifnet    *ifp = &ravb->ecom.ec_if;

    /* Link state latches low so double read to clear */
    nic_mutex_lock(&ravb->drv_mutex);
    val = ravb_mii_read(ravb,  cfg->phy_addr, 1);
    val = ravb_mii_read(ravb,  cfg->phy_addr, 1);
    nic_mutex_unlock(&ravb->drv_mutex);

    if ((cfg->flags & NIC_FLAG_LINK_DOWN) &&
        (val & 4)) {
        /* Link was down and is now up */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK,_SLOG_INFO, "%s(): link up", __FUNCTION__);
        }
        cfg->flags &= ~NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_UP);
    } else if (((cfg->flags & NIC_FLAG_LINK_DOWN) == 0) &&
                ((val & 4) == 0)) {
        /* Link was up and is now down */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(): link down", __FUNCTION__);
        }
        cfg->media_rate = -1;
        cfg->flags |= NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_DOWN);
    }
}

/*
 * Check to see whether this is a BroadR-Reach PHY
 */

int ravb_is_br_phy (ravb_dev_t *ravb)
{
    uint32_t       PhyAddr;
    int            is_br;

    PhyAddr = ravb->options.cfg.phy_addr;

    switch (ravb->mdi->PhyData[PhyAddr]->VendorOUI) {
    case BROADCOM3:
        switch(ravb->mdi->PhyData[PhyAddr]->Model) {
            case BCM89811:
                is_br = 1;
                break;

            default:
                is_br = 0;
                break;
        }
        break;

    default:
        is_br = 0;
        break;
    }

    return is_br;
}

static void monitor_switch_port_0 (ravb_dev_t *ravb)

{
    uint16_t        reg, spd;
    nic_config_t    *cfg = &ravb->options.cfg;
    struct ifnet    *ifp = &ravb->ecom.ec_if;

    reg = ravb_mdi_read (ravb, 0, PORT_STATUS);
    if ((cfg->flags & NIC_FLAG_LINK_DOWN) && (reg & SW_LINK_STATUS)) {
        /* Link was down and is now up */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK,_SLOG_INFO, "%s(): link up", __FUNCTION__);
        }
        cfg->flags &= ~NIC_FLAG_LINK_DOWN;
        spd = (reg & SPEED_MASK) >> SPEED_SHIFT;
        switch (spd) {
            case    0:
                cfg->media_rate = 10000L;
                break;
            case    1:
                cfg->media_rate = 100000L;
                break;
            case    2:
                cfg->media_rate = 1000000L;
                break;
            default:
                cfg->media_rate = -1;
                break;
        }
        cfg->duplex = (reg & DUPLEX_MODE) ? 1 : 0;
        if_link_state_change(ifp, LINK_STATE_UP);
        ravb->phy_regs[MDI_BMSR] |= (BMSR_LINK_STATUS | BMSR_AN_COMPLETE);
    } else if (((cfg->flags & NIC_FLAG_LINK_DOWN) == 0) && ((reg & SW_LINK_STATUS) == 0)) {
        /* Link was up and is now down */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(): link down", __FUNCTION__);
        }
        cfg->media_rate = -1;
        cfg->duplex = -1;
        cfg->flags |= NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_DOWN);
        ravb->phy_regs[MDI_BMSR] &= ~(BMSR_LINK_STATUS | BMSR_AN_COMPLETE);
    }
}

static void ravb_do_mdi_monitor(void* arg)
{
    ravb_dev_t *ravb   = arg;

    if (!ravb_is_br_phy(ravb)) {
        MDI_MonitorPhy(ravb->mdi);
        if (ravb->options.mv88q5072) {
            monitor_switch_port_0 (ravb);
        }
    } else {
        ravb_br_phy_state(ravb);
    }

    kthread_exit(0);
}


static void ravb_bcm89811_phy_init(ravb_dev_t *ravb)
{
    int phy_idx = ravb->options.cfg.phy_addr;

    /*
     * The following came from Broadcom as 89811_script_v0_93.vbs
     *
     * Broadcom refuse to document what exactly is going on.
     * They insist these register writes are correct despite the
     * way sometimes the same register is written back-to-back with
     * contradictory values and others are written with default values.
     * There is also much writing to undocumented registers and reserved
     * fields in documented registers.
     */

    nic_mutex_lock(&ravb->drv_mutex);

    /* begin EMI optimization */
    ravb_mii_write(ravb, phy_idx, 0, 0x8000); //reset
    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0028);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x0C00);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0312);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x030B);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x030A);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x34C0);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0166);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x0020);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x012D);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x9B52);
    ravb_mii_write(ravb, phy_idx, 0x1E, 0x012E);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0xA04D);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0123);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x00C0);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0154);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x81C4);


    //IOUT = &H2&    ' 10=4.0mA
    //SLEW = &H2&    ' 10=3xdefault_slew
    //MII_PAD_SETTING = SLEW + 4*IOUT
    //v = &H0000& Or MII_PAD_SETTING * 2048
    //App.WrMii PORT, &H001E&, &H0811&   '
    //App.WrMii PORT, &H001F&, v   '
    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0811);
    ravb_mii_write(ravb, phy_idx, 0x1F, (0x2+(0x2<<2))<<11);

    //v = &H0064&
    //App.WrMii PORT, &H001E&, &H01D3&   '
    //App.WrMii PORT, &H001F&, v   '
    ravb_mii_write(ravb, phy_idx, 0x1E, 0x01D3);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x0064);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x01C1);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0xA5F7);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0028);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x0400);

    /* End EMI optimization */

    /*begin LED setup portion*/
    ravb_mii_write(ravb, phy_idx, 0x1E, 0x001D);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x3411);

    ravb_mii_write(ravb, phy_idx, 0x1E, 0x0820);
    ravb_mii_write(ravb, phy_idx, 0x1F, 0x0401);
    /*end of LED setup*/

    /* MII or MII-Lite config*/
    if ((ravb->options.mii) || (ravb->options.miiLite)) {
        ravb_mii_write(ravb, phy_idx, 0x1E, 0x002F);
        ravb_mii_write(ravb, phy_idx, 0x1F, 0xF167);
        ravb_mii_write(ravb, phy_idx, 0x1E, 0x0045);
        ravb_mii_write(ravb, phy_idx, 0x1F, 0x0000);
    } else {
    /* ENET defaults to RGMII mode */
        ravb_mii_write(ravb, phy_idx, 0x1E, 0x0045);
        ravb_mii_write(ravb, phy_idx, 0x1F, 0x0000);
        ravb_mii_write(ravb, phy_idx, 0x1E, 0x002F);
        ravb_mii_write(ravb, phy_idx, 0x1F, 0xF1E7);
   }

    /* Additional expansion reg config for MII-Lite mode only */
    if (ravb->options.miiLite) {
        ravb_mii_write(ravb, phy_idx, 0x1E, 0x000E);
        ravb_mii_write(ravb, phy_idx, 0x1F, (ravb_mii_read(ravb, phy_idx, 0x1F) | (1<<11)));
    }

    /* Set phy to master mode */
    slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(): Setting BroadrReach phy to master", __FUNCTION__);
    ravb_mii_write(ravb, phy_idx, 0, 0x0208);

    nic_mutex_unlock(&ravb->drv_mutex);
}

static void ravb_br_phy_init (ravb_dev_t *ravb)
{
    nic_config_t *cfg;

    cfg = &ravb->options.cfg;

    switch (ravb->mdi->PhyData[cfg->phy_addr]->VendorOUI) {
    case BROADCOM3:
        switch(ravb->mdi->PhyData[cfg->phy_addr]->Model) {
            case BCM89811:
                if (cfg->verbose & VERBOSE_PHY) {
                    slogf(_SLOGC_NETWORK,_SLOG_INFO, "Detected BCM89811");
                }
                ravb_bcm89811_phy_init(ravb);
                break;

            default:
                break;
        }

    default:
        break;
    }
}

void ravb_MDI_MonitorPhy(void *arg)
{
    ravb_dev_t      *ravb;
    nic_config_t    *cfg;

    ravb = arg;
    cfg = &ravb->options.cfg;

    if (((cfg->flags & NIC_FLAG_LINK_DOWN) != 0) || !ravb->pkts_received) {
        if (cfg->verbose & VERBOSE_PHY) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Check link state");
        }
        kthread_create1(ravb_do_mdi_monitor, arg, NULL, NULL);
    }

    ravb->pkts_received = 0;

    /*
     * With 32 bit counters and 100Mb/s max pps the stats max out in 8 hours.
     * Without an interrupt to warn of approaching max, we need to poll
     * faster than that. We already have a 3 second poller here and the
     * poll of the stats is lightweight, 16 reads and writes, so we poll
     * here rather than setting up a new poller.
     */
    ravb_update_stats(ravb);

    callout_msec(&ravb->mii_callout, 3 * 1000, ravb_MDI_MonitorPhy, arg);
}

static void ravb_mediastatus(struct ifnet *ifp, struct ifmediareq *ifmr)
{
    ravb_dev_t      *ravb;

    ravb = ifp->if_softc;

    ravb->bsd_mii.mii_media_active = IFM_ETHER;
    ravb->bsd_mii.mii_media_status = IFM_AVALID;

    if ((ravb->options.cfg.flags & NIC_FLAG_LINK_DOWN) != 0) {
        ravb->bsd_mii.mii_media_active |= IFM_NONE;
    } else {
        ravb->bsd_mii.mii_media_status |= IFM_ACTIVE;

        switch (ravb->options.cfg.media_rate) {
            case 100000L:
                ravb->bsd_mii.mii_media_active |= IFM_100_TX;
                break;

            case 1000000L:
                ravb->bsd_mii.mii_media_active |= IFM_1000_T;
                break;

            default:
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "ravb: Unknown media, forcing none");
            /* Fallthrough */
            case 0:
                ravb->bsd_mii.mii_media_active |= IFM_NONE;
                break;
        }

        if (ravb->options.cfg.duplex) {
            ravb->bsd_mii.mii_media_active |= IFM_FDX;
        }

        ravb->bsd_mii.mii_media_active |= ravb->flow_status;
    }

    /* Return the data */
    ifmr->ifm_status = ravb->bsd_mii.mii_media_status;
    ifmr->ifm_active = ravb->bsd_mii.mii_media_active;
}

int ravb_mediachange(struct ifnet *ifp)
{
    ravb_dev_t      *ravb;
    struct ifmedia  *ifm;
    nic_config_t    *cfg;
    int             an_media;

    if (!(ifp->if_flags & IFF_UP)) {
        return 0;
    }

    ravb = ifp->if_softc;
    ifm = &ravb->bsd_mii.mii_media;
    cfg = &ravb->options.cfg;

    /* Media is changing so link will be down until autoneg completes */
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    switch (ifm->ifm_media & IFM_TMASK) {
        case IFM_NONE:
            ravb->set_speed = 0;
            ravb->set_duplex = 0;
            ravb->options.set_flow = 0;

            /* Special case, shut down the PHY and bail out */
            callout_stop(&ravb->mii_callout);
            MDI_DisableMonitor(ravb->mdi);
            MDI_PowerdownPhy(ravb->mdi, cfg->phy_addr);
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_DOWN);
            return 0;

        case IFM_AUTO:
            ravb->set_speed = -1;
            ravb->set_duplex = -1;
            ravb->options.set_flow = -1;

            MDI_GetMediaCapable(ravb->mdi, cfg->phy_addr, &an_media);
            /* Enable Pause in autoneg */
            if ((ravb->mdi->PhyData[ cfg->phy_addr]->VendorOUI == KENDIN) &&
            (ravb->mdi->PhyData[ cfg->phy_addr]->Model == KSZ9031)) {
                /* Bug in KSZ9031 PHY: Fails to autoneg with ASYM */
                an_media |= MDI_FLOW;
            } else {
                an_media |= MDI_FLOW | MDI_FLOW_ASYM;
            }
            break;

        case IFM_100_TX:
            ravb->set_speed = 100000L;
            ravb->set_duplex = 0;
            ravb->options.set_flow = 0;

            if ((ifm->ifm_media & IFM_FDX) == 0) {
                an_media = MDI_100bT;
            }
            else {
                ravb->set_duplex = 1;
                an_media = MDI_100bTFD;
                if (ifm->ifm_media & IFM_FLOW) {
                    ravb->options.set_flow = 1;
                    an_media |= MDI_FLOW;
                }
                else if (ifm->ifm_media & IFM_ETH_RXPAUSE) {
                    ravb->options.set_flow = 2;
                    an_media |= MDI_FLOW | MDI_FLOW_ASYM;
                }
                else if (ifm->ifm_media & IFM_ETH_TXPAUSE) {
                    ravb->options.set_flow = 3;
                    an_media |= MDI_FLOW_ASYM;
                }
            }
            break;

        case IFM_1000_T:
            ravb->set_speed = 1000000L;
            ravb->set_duplex = 0;
            ravb->options.set_flow = 0;

            if ((ifm->ifm_media & IFM_FDX) == 0) {
                an_media = MDI_1000bT;
            }
            else {
                ravb->set_duplex = 1;
                an_media = MDI_1000bTFD;
                if (ifm->ifm_media & IFM_FLOW) {
                    ravb->options.set_flow = 1;
                    an_media |= MDI_FLOW;
                }
                else if (ifm->ifm_media & IFM_ETH_RXPAUSE) {
                    ravb->options.set_flow = 2;
                    an_media |= MDI_FLOW | MDI_FLOW_ASYM;
                }
                else if (ifm->ifm_media & IFM_ETH_TXPAUSE) {
                    ravb->options.set_flow = 3;
                    an_media |= MDI_FLOW_ASYM;
                }
            }
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Unknown media type");
            return 1;
    }

    MDI_PowerupPhy(ravb->mdi, cfg->phy_addr);
   switch (ravb->mdi->PhyData[cfg->phy_addr]->VendorOUI) {
        case KENDIN:
            switch (ravb->mdi->PhyData[cfg->phy_addr]->Model) {
                case KSZ9031:
                    if (cfg->verbose)
                        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "Detected Micrel KSZ9031 PHY");
                    ravb_ksz9031_phy_init (ravb);
                    break;
                default:
                    break;
            }
            break;

    case BROADCOM3:
            switch (ravb->mdi->PhyData[cfg->phy_addr]->Model) {
                case BCM89811:
                    if (cfg->verbose)
                        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "Detected Broadcom BCM89811 PHY");
                    ravb_bcm89811_phy_init (ravb);
                    break;

                default:
                    break;
            }
            break;

       default:
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "ravb: Handling default Vendor OUI 0x%8X", ravb->mdi->PhyData[cfg->phy_addr]->VendorOUI);
            break;
    }

    MDI_EnableMonitor(ravb->mdi, 0);
    MDI_SetAdvert(ravb->mdi, cfg->phy_addr, an_media);
    MDI_AutoNegotiate(ravb->mdi, cfg->phy_addr, NoWait);
    ravb_MDI_MonitorPhy (ravb);

    return 0;
}

static void ravb_mediainit(ravb_dev_t *ravb)
{
    nic_config_t    *cfg;
    struct ifmedia  *ifm;

    cfg = &ravb->options.cfg;
    ifm = &ravb->bsd_mii.mii_media;

    ravb->bsd_mii.mii_ifp = &ravb->ecom.ec_if;

    ifmedia_init(ifm, IFM_IMASK, ravb_mediachange, ravb_mediastatus);
    ifmedia_add(ifm, IFM_ETHER | IFM_NONE, 0, NULL);

    if(ravb_is_br_phy(ravb)) {
        ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX, 0, NULL);
    } else
        if (ravb->options.emu_phy != -1) {
            ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX, 0, NULL);
        }
        else {

        /*
         * Based on Gen3 HW User Guide Section 50.1.1:
         * RAVB controller only supports 100/1000 Full-duplex mode
         */
        ifmedia_add(ifm, IFM_ETHER | IFM_AUTO, 0, NULL);

        ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX, 0, NULL);
        ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX | IFM_ETH_TXPAUSE, 0, NULL);
        ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX | IFM_ETH_RXPAUSE, 0, NULL);
        ifmedia_add(ifm, IFM_ETHER | IFM_100_TX | IFM_FDX | IFM_FLOW, 0, NULL);

        ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX, 0, NULL);
        ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX | IFM_FLOW, 0, NULL);
        ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX | IFM_ETH_TXPAUSE, 0, NULL);
        ifmedia_add(ifm, IFM_ETHER | IFM_1000_T | IFM_FDX | IFM_ETH_RXPAUSE, 0, NULL);
    }

    /*
     * nic_parse_options() sets speed / duplex in cfg but those are for
     * reporting state. Copy them across to the right place.
     */
    ravb->set_speed = cfg->media_rate;
    ravb->set_duplex = cfg->duplex;
    cfg->media_rate = 0;
    cfg->duplex = 0;

    switch (ravb->set_speed) {
        case -1:
            ifm->ifm_media = IFM_ETHER | IFM_AUTO;
            break;

        case 100 *1000L:
            ifm->ifm_media = IFM_ETHER | IFM_100_TX;
            if (ravb->set_duplex != 0) {
                ifm->ifm_media |= IFM_FDX;
                switch (ravb->options.set_flow) {
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

        case 1000 * 1000L:
            ifm->ifm_media = IFM_ETHER | IFM_1000_T;
            if (ravb->set_duplex != 0) {
                ifm->ifm_media |= IFM_FDX;
                switch (ravb->options.set_flow) {
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
                  "ravb: Unknown initial media, forcing none");
        /* Fallthrough */

        case 0:
            ifm->ifm_media = IFM_ETHER | IFM_NONE;
            break;
    }

    ifmedia_set(ifm, ifm->ifm_media);
}

int ravb_phy_init(ravb_dev_t *ravb)
{
    nic_config_t    *cfg;
    struct ifnet    *ifp;
    int             i, rc, phy_addr = -1;

    cfg = &ravb->options.cfg;
    ifp = &ravb->ecom.ec_if;

    rc = MDI_Register_Extended(ravb, ravb_mii_write,
                ravb_mii_read, ravb_mii_callback, &ravb->mdi, NULL, 0, 0);
    if (rc != MDI_SUCCESS) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Failed to register with MDI");
        return (ENODEV);
    }

    if (ravb->options.emu_phy != -1 && ravb->options.emu_phy == cfg->device_index) {
        emu_phy_init (ravb);
    }

    for (i = 0; i < 32; i++) {
        if (MDI_FindPhy (ravb->mdi, i) == MDI_SUCCESS) {
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: PHY at address %d", i);
            }
            phy_addr = i;
            break;
        }
    }
    if (phy_addr == -1) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "Failed to find PHY");
        return ENODEV;
    }
    cfg->phy_addr = phy_addr;

    rc = MDI_InitPhy(ravb->mdi, cfg->phy_addr);
    if (rc != MDI_SUCCESS) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed to init the PHY");
        return ENODEV;
    }

    if (ravb_is_br_phy(ravb)) {
       ravb_br_phy_init(ravb);
    }
    else {
       rc= MDI_ResetPhy(ravb->mdi, cfg->phy_addr, WaitBusy);
       if (rc != MDI_SUCCESS) {
            slogf(_SLOGC_NETWORK,_SLOG_ERROR, "%s(): Failed to reset the PHY: %d",
                __FUNCTION__, rc);
            return -1;
       }

       MDI_PowerdownPhy(ravb->mdi, cfg->phy_addr);
    }

    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    ravb_mediainit(ravb);

    return EOK;
}

void ravb_phy_fini(ravb_dev_t *ravb)
{
    ifmedia_delete_instance(&ravb->bsd_mii.mii_media, IFM_INST_ANY);

    MDI_DeRegister(&ravb->mdi);
    ravb->mdi = NULL;
    if (ravb->options.emu_phy != -1) {
        emu_phy_dinit (ravb);
    }
}
