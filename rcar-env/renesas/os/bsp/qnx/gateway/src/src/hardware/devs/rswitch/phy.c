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

#define SPEED_MASK              BITS(3,6)
#define SPEED_10                0
#define SPEED_100               BIT(6)
#define SPEED_1000              BIT(7)
#define SPEED_2500              SPEED_MASK

#define SPEED2_MASK             BITS(3,0)
#define SPEED2_5000             BITS(2,0)
#define SPEED2_2500             BITS(3,0)
#define PORT_RS                 BIT(15)
#define MACTYPE_MASK            0x7
#define SXGMII                  0x0
#define SGMII_AN_ON             0x4
#define SGMII_AN_OFF            0x5
#define BOOT_FATAL              1

/* PCS Register */
#define PHY_LINK                BIT(10)
#define PHY_DUPLEX              BIT(13)
#define PHY_LINK_SPEED          BITS(3,14)
#define PHY_LINK_10             BITS(0,14)
#define PHY_LINK_100            BITS(1,14)
#define PHY_LINK_1000           BITS(2,14)
#define PHY_LINK_SPEED2         BITS(3,2)
#define PHY_LINK_2500          (BITS(1,2) | PHY_LINK_SPEED)
#define PHY_LINK_5000          (BITS(2,2) | PHY_LINK_SPEED)
#define PHY_LINK_10000         (PHY_LINK_SPEED)

/* Host Interface Register */
 #define SD_INIT                BIT(15)
 #define DIS_AUTO_INIT          BIT(13)

#define PHY_TIMEOUT             1000
#define LINK_UP                 1
#define LINK_DOWN               0

#define MDIO_READ               0x02
#define MDIO_WRITE              0x01

#define MDIO_READ_C45           0x03
#define MDIO_WRITE_C45          0x01
#define MDIO_ADDR_C45           0x00
    
/* MDIO bus is 2.5MHz so clock is 200ns high + 200ns low */
#define PHY_DELAY()             nanospin_ns(1000)

uint16_t rswitch2_mii_read_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad)
{
    rswitch2_dev_t   *rswitch2 = handle;
    rswitch2_etha_t  *etha = &rswitch2->etha;
    int              pop = MDIO_READ_C45;
    uint16_t         val;
    int              i;

    /* Clear completion statuses */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MMIS1, 0xF);

    /* Submit address to PHY (MDIO_ADDR_C45 << 13) */
    val = MPSM_PSME | MPSM_MFF_C45;
    bus_write_4(rswitch2->res[0], etha->addr_offset + MPSM, (regad << 16) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((bus_read_4(rswitch2->res[0], etha->addr_offset + MMIS1) & MMIS1_PAACS) !=  MMIS1_PAACS) {
        if(--i == 0) {
            return -ETIMEDOUT;
        }
        nanospin_ns(1000);
    }

    /* Clear Address access completed Status */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MMIS1, MMIS1_PAACS);

    bus_write_4(rswitch2->res[0], etha->addr_offset + MPSM, (pop << 13) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((bus_read_4(rswitch2->res[0], etha->addr_offset + MMIS1) & MMIS1_PRACS) !=  MMIS1_PRACS) {
        if(--i == 0) {
            return -ETIMEDOUT;
        }
        nanospin_ns(1000);
    }

    /* Read data */
    val = (bus_read_4(rswitch2->res[0], etha->addr_offset + MPSM) & MPSM_PRD_MASK) >> 16;

    /* Clear Read access completed Status */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MMIS1, MMIS1_PRACS);

    if (rswitch2->verbose & VERBOSE_PHY) {
        device_printf(rswitch2->dev,"rswitch2: MDIO Read Phy %d Devad %d Reg 0x%x Data 0x%x \n",phyad, devad, regad, val);
    }

    return val;
}

void rswitch2_mii_write_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad, uint16_t data)
{
    rswitch2_dev_t   *rswitch2 = handle;
    rswitch2_etha_t  *etha = &rswitch2->etha;
    int              pop = MDIO_WRITE_C45;
    uint32_t         val;
    int              i;

    /* Clear completion statuses */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MMIS1, 0xF);

    /* Submit address to PHY (MDIO_ADDR_C45 << 13) */
    val = MPSM_PSME | MPSM_MFF_C45;
    bus_write_4(rswitch2->res[0], etha->addr_offset + MPSM, (regad << 16) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((bus_read_4(rswitch2->res[0], etha->addr_offset + MMIS1) & MMIS1_PAACS) !=  MMIS1_PAACS) {
        if(--i == 0) {
            return;
        }
        nanospin_ns(1000);
    }

    /* Clear Address access completed Status */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MMIS1, MMIS1_PAACS);

    bus_write_4(rswitch2->res[0], etha->addr_offset + MPSM, (data << 16) | (pop << 13) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((bus_read_4(rswitch2->res[0], etha->addr_offset + MMIS1) & MMIS1_PWACS) !=  MMIS1_PWACS) {
        if(--i == 0) {
            return;
        }
        nanospin_ns(1000);
    }

    if (rswitch2->verbose & VERBOSE_PHY) {
        device_printf(rswitch2->dev,"rswitch2: MDIO Write Phy %d Devad %d Reg 0x%x Data 0x%x \n",phyad, devad, regad, data);
    }
}

int rswitch2_miibus_read_reg(device_t dev, int phy, int reg)
{
    rswitch2_dev_t *rswitch2 = device_get_softc(dev);
    nic_config_t *cfg = &rswitch2->cfg;
    uint16_t devad = 1;
    int ret = 0;

    /*
     * FIXME: C45 is not supported yet 
     * So we hardcode some returns here
     */
    if (reg==0)  return (BMCR_AUTOEN);
    if (reg==1) {
        reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PCS, 0x8008);
        if (reg & PHY_LINK) {
            return (BMSR_10TFDX | BMSR_100TXFDX | BMSR_EXTSTAT | BMSR_ACOMP | BMSR_ANEG | BMSR_LINK);
        } else {
            return ~(BMSR_10TFDX | BMSR_100TXFDX | BMSR_EXTSTAT | BMSR_ACOMP | BMSR_ANEG | BMSR_LINK);
        }
    }
    if (reg==4)  return ANAR_X_FD;
    if (reg==5)  return ANLPAR_X_FD;
    if (reg==9)  return GTCR_ADV_1000TFDX;
    if (reg==10) return GTSR_LP_1000TFDX;
    if (reg==15) return EXTSR_1000TFDX;
    if (reg==2 || reg==3) devad = 3;
    ret =  rswitch2_mii_read_c45 (rswitch2, phy, devad, reg);

    return ret;
}

int rswitch2_miibus_write_reg(device_t dev, int phy, int reg, int val)
{
    /* 
     * FIXME: C45 is not supported yet 
     * return without doing
     */

    return 0;
}

static uint16_t m88e2110_check_link(rswitch2_dev_t *rswitch2)
{
    nic_config_t *cfg = &rswitch2->cfg;
    uint16_t     reg, reg1;
    int          timeout = 0;

    while (1) {
        reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PCS, 0x8008);
        if (reg & PHY_LINK)
            break;

        if (timeout > PHY_TIMEOUT)
            return 0;

        timeout++;
        PHY_DELAY();
    }
    while (1) {
        reg1 = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PHY_XS, 0xA003);
        if (reg1 & PHY_LINK)
            return reg;

        if (timeout > PHY_TIMEOUT)
            return 0;

        timeout++;
        PHY_DELAY();
    }

    return 0;
}

static int m88e2110_phy_set_speed(rswitch2_dev_t *rswitch2, int speed)
{
    nic_config_t *cfg = &rswitch2->cfg;
    uint16_t     reg;

    reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF000);
    reg &= ~SPEED_MASK;
    if (speed == 2500*1000L) {
        reg |= SPEED_2500;
    } else if (speed == 1000*1000L) {
        reg |= SPEED_1000;
    } else {
        reg |= SPEED_100;
    }

    rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF000, reg);
    rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF000, reg);

     if ((speed == 2500*1000L) || (speed == 5000*1000L)) {
        reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF0A8);
        reg &= ~SPEED2_MASK;
        if (speed == 2500*1000L)
            reg |= SPEED2_2500;
        else
            reg |= SPEED2_5000;
        rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF0A8, reg);
        rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF0A8, reg);
    }

    return EOK;
}

static int m88e2110_phy_init(rswitch2_dev_t *rswitch2)
{
    uint32_t        reg;
    nic_config_t    *cfg;
    int             timeout = 0;

    cfg = &rswitch2->cfg;

    mtx_lock(&rswitch2->mtx);

    /* Check Boot Status */
    reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PMA_PMD, 0xC050);
    if (reg & BOOT_FATAL) {
         device_printf(rswitch2->dev, "rswitch2: PHY failed to boot, rc=0x%x", reg);
         mtx_unlock(&rswitch2->mtx);
        return -EINVAL;
    }

    /* Confirm Port Status */
    reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF001);
    if (!reg) {
        device_printf(rswitch2->dev, "rswitch2: Port Error, Port status = 0x%x ", reg);
        mtx_unlock(&rswitch2->mtx);
        return -EINVAL;
    }

    /* Set speed */
    m88e2110_phy_set_speed(rswitch2, (&rswitch2->etha)->phy_speed);

    /* HWMON */
    rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF08C, 0x9600);
    
    /* Port 0 was resetted already in bootloader ? */
    if ((rswitch2->port0_reset && rswitch2->port == 0) || (rswitch2->port)) {
        /* Port Reset */
        reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PMA_PMD, 0xC04A);
        reg |= PORT_RS;
        rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_PMA_PMD, 0xC04A, reg);
        rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_PMA_PMD, 0xC04A, reg);
    }

    reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PMA_PMD, 0xC04A);
    /* Port Control: Set MACTYPE = SGMII */
    if ((reg & MACTYPE_MASK) != SGMII_AN_ON) {
        reg &= ~MACTYPE_MASK;
        reg |= PORT_RS | SGMII_AN_ON;
        rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_PMA_PMD, 0xC04A, reg);

        /* SerDes Init */
        reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_AN, 0x800F);
        reg |= SD_INIT | DIS_AUTO_INIT;
        rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_AN, 0x800F, reg);
        do {
            reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_AN, 0x800F);
            if (timeout > PHY_TIMEOUT) {
                device_printf(rswitch2->dev, "rswitch: PHY init timeout, reg=0x%x", reg);
                mtx_unlock(&rswitch2->mtx);
                return -ETIMEDOUT;
            }
            timeout++;
            PHY_DELAY();
        } while (reg & SD_INIT);
        reg &= ~DIS_AUTO_INIT;
        rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_AN, 0x800F, reg);
    }

    /* Enable EDPD mode - saving 600mW */
    reg  = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF08A);
    reg &= ~0xC000;
    rswitch2_mii_write_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF08C, reg);

    mtx_unlock(&rswitch2->mtx);

    return EOK;
}

static void m88e2110_phy_state(rswitch2_dev_t  *rswitch2)
{
    nic_config_t     *cfg = &rswitch2->cfg;
    struct mii_data  *mii = rswitch2->mii_softc;
    uint16_t         reg, spd;

    reg = m88e2110_check_link(rswitch2);
    if (reg & PHY_LINK) {
        /* Link was down and is now up */
        if (cfg->verbose) {
            device_printf(rswitch2->dev, "rswitch2: Port %d link up\n", rswitch2->port);
        }
        mii->mii_media_active = IFM_ETHER | IFM_FDX;
        mii->mii_media_status = IFM_AVALID | IFM_ACTIVE;
        spd = (reg & PHY_LINK_SPEED);
        if (spd == PHY_LINK_SPEED)
            spd |= (reg & PHY_LINK_SPEED2);
        switch (spd) {
            case PHY_LINK_10:
                mii->mii_media_active |= IFM_10_T;
                break;
            case PHY_LINK_100:
                mii->mii_media_active |= IFM_100_T;
                break;
            case PHY_LINK_1000:
            default:
                mii->mii_media_active |= IFM_1000_T;
                break;
            case PHY_LINK_2500:
                mii->mii_media_active |= IFM_2500_T;
                break;
            case PHY_LINK_5000:
            case PHY_LINK_10000:
                /* RSwitch controller supports maximun of 2.5G */
                mii->mii_media_active |= IFM_2500_T;
                rswitch2->flags = LINK_STATE_DOWN;
                break;
        }
        if (reg & PHY_DUPLEX) mii->mii_media_active |= IFM_FDX;
    } else {
        /* Link was up and is now down */
        if (cfg->verbose) {
            device_printf(rswitch2->dev, "rswitch2: Port %d link down\n", rswitch2->port);
        }
        mii->mii_media_active = IFM_ETHER | IFM_NONE;
        mii->mii_media_status = IFM_AVALID;
    }
}

void rswitch2_media_status(struct ifnet * ifp, struct ifmediareq *ifmr)
{
    rswitch2_dev_t      *rswitch2;
    struct mii_data     *mii;

    rswitch2 = ifp->if_softc;
    mii = rswitch2->mii_softc;
    //mtx_lock(&rswitch2->mtx);

    mii_pollstat(mii);
    /* Return the data */
    ifmr->ifm_status = mii->mii_media_status;
    ifmr->ifm_active = mii->mii_media_active;
    //mtx_unlock(&rswitch2->mtx);
}

int rswitch2_media_change(struct ifnet * ifp)
{
    rswitch2_dev_t  *rswitch2;
    struct ifmedia  *ifm;
    nic_config_t    *cfg;
    int error;

    rswitch2 = ifp->if_softc;

    mtx_lock(&rswitch2->mtx);

    if (!(ifp->if_flags & IFF_UP)) {
        return EOK;
    }

    ifm = &rswitch2->bsd_mii.mii_media;
    cfg = &rswitch2->cfg;

    /* Media is changing so link will be down until autoneg completes */
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    error = mii_mediachg(rswitch2->mii_softc);
    mtx_unlock(&rswitch2->mtx);

    return EOK;
}

void rswitch2_miibus_statchg(device_t dev)
{
    rswitch2_dev_t  *rswitch2;
    struct mii_data *mii;
    nic_config_t    *cfg;

    /*
     * Called by the MII bus driver when the PHY establishes
     * link to set the MAC interface registers.
     */

    rswitch2 = device_get_softc(dev);
    mii = rswitch2->mii_softc;
    cfg = &rswitch2->cfg;

    //mtx_assert(&rswitch2->mtx, MA_OWNED);

    m88e2110_phy_state(rswitch2);

    if (mii->mii_media_status & IFM_ACTIVE) {
        rswitch2->link_is_up = true;
    }
    else {
        rswitch2->link_is_up = false;
    }

    switch (IFM_SUBTYPE(mii->mii_media_active)) {
    case IFM_2500_T:
        rswitch2->media_rate = PHY_SPEED_2500M;
        break;
    case IFM_1000_T:
        rswitch2->media_rate = PHY_SPEED_1000M;
        break;
   case IFM_100_T:
        rswitch2->media_rate = PHY_SPEED_100M;
        break;
    case IFM_10_T:
        rswitch2->media_rate = PHY_SPEED_10M;
        break;
    case IFM_NONE:
    default:
        rswitch2->link_is_up = false;
        //device_printf(dev, "%s: Unsupported media %u\n", __func__,
        //        IFM_SUBTYPE(mii->mii_media_active));
        break;
    }
    if (mii->mii_media_active & IFM_FDX) {
        cfg->duplex = 1;
    } else {
        cfg->duplex = 0;
    }

    /* Hardware-specific code here: implement media-specific hardware initialization */
    if (rswitch2->link_is_up && rswitch2->flags & LINK_STATE_DOWN) {
        rswitch2_set_speed(rswitch2, rswitch2->mii, cfg->media_rate);
        m88e2110_phy_set_speed(rswitch2, cfg->media_rate);
        rswitch2->flags &= ~LINK_STATE_DOWN;
    } else if (!rswitch2->link_is_up && !(rswitch2->flags & LINK_STATE_DOWN)) {
        rswitch2->flags |= LINK_STATE_DOWN;
    }
}

static int rswitch2_find_phy(rswitch2_dev_t *rswitch2)
{
    int         status;
    int         i = 0;
    int         dv = 1;
    int         j = 0;
    for (i = 0; i < MII_NPHY; i++) {
        if ( i > 2)  dv = 3;
        for (j = 1; j <= dv; j++) {
            status = rswitch2_mii_read_c45(rswitch2, i, j, MII_BMCR);
            if ((status == 0) || (status == 0xffff)) {
                /* No PHY at this address.*/
                continue;
            }
            return i;
        }
    }
    return -1;
}

int rswitch2_phy_init(rswitch2_dev_t *rswitch2)
{
    nic_config_t *cfg;
    struct ifnet *ifp;
    int          rc, phy_addr = -1;

    cfg = &rswitch2->cfg;
    ifp = rswitch2->ifp;

    phy_addr = rswitch2_find_phy(rswitch2);
    if (phy_addr == -1) {
        device_printf(rswitch2->dev, "Failed to find PHY");
        return ENODEV;
    }
    
    cfg->phy_addr = phy_addr;
    device_printf(rswitch2->dev, "rswitch2: PHY at address %d\n", phy_addr);

    rc = m88e2110_phy_init(rswitch2);
    if (rc != EOK) {
        device_printf(rswitch2->dev,
                "rswitch2: rswitch2_phy_init failed: %d", rc);
        return rc;
    }

    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    return EOK;
}
