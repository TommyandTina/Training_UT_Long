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

#if (_NTO_VERSION < 660)
#include <drvr/mdi.h>
#else
#include <netdrvr/mdi.h>
#endif

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

uint16_t m88e2110_link_status(rswitch2_dev_t *rswitch2);

uint16_t rswitch2_mdi_read(void *handle, uint8_t phyid, uint8_t location)
{
    rswitch2_dev_t   *rswitch2 = handle;
    rswitch2_etha_t  *etha = &rswitch2->etha;
    int              pop = MDIO_READ;
    uint16_t         val;
    int              i;

    pthread_mutex_lock(&etha->etha_mutex);

    /* Clear completion statuses */
    out32(etha->addr + MMIS1, 0xF);

    out32(etha->addr + MPSM, (pop << 13) | (location << 8) | (phyid << 3) | MPSM_PSME);

    i = RSWITCH2_TIMEOUT_MS;
    while ((in32(etha->addr + MMIS1) & MMIS1_PRACS) !=  MMIS1_PRACS) {
        if(--i == 0) {
            pthread_mutex_unlock(&etha->etha_mutex);
            return -ETIMEDOUT;
        }
        nanospin_ns(1000);
    }

    /* Read data */
    val = (in32(etha->addr + MPSM) & MPSM_PRD_MASK) >> 16;

    /* Clear Read access completed Status */
    out32(etha->addr + MMIS1, MMIS1_PRACS);

    pthread_mutex_unlock(&etha->etha_mutex);

    return val;
}

void rswitch2_mdi_write(void *handle, uint8_t phyid, uint8_t location, uint16_t data)
{
    rswitch2_dev_t   *rswitch2 = handle;
    rswitch2_etha_t  *etha = &rswitch2->etha;
    int              pop = MDIO_WRITE;
    int              i;

    pthread_mutex_lock(&etha->etha_mutex);

    /* Clear completion statuses */
    out32(etha->addr + MMIS1, 0xF);

    out32(etha->addr + MPSM, (data << 16) | (pop << 13) | (location << 8) | (phyid << 3) | MPSM_PSME);

    i = RSWITCH2_TIMEOUT_MS;
    while ((in32(etha->addr + MMIS1) & MMIS1_PWACS) !=  MMIS1_PWACS) {
        if(--i == 0) {
            pthread_mutex_unlock(&etha->etha_mutex);
            return;
        }
        nanospin_ns(1000);
    }

    pthread_mutex_unlock(&etha->etha_mutex);
}

uint16_t rswitch2_mii_read_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad)
{
    rswitch2_dev_t   *rswitch2 = handle;
    rswitch2_etha_t  *etha = &rswitch2->etha;
    int              pop = MDIO_READ_C45;
    uint16_t         val;
    int              i;

    pthread_mutex_lock(&etha->etha_mutex);

    /* Clear completion statuses */
    out32(etha->addr + MMIS1, 0xF);

    /* Submit address to PHY (MDIO_ADDR_C45 << 13) */
    val = MPSM_PSME | MPSM_MFF_C45;
    out32(etha->addr + MPSM, (regad << 16) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((in32(etha->addr + MMIS1) & MMIS1_PAACS) !=  MMIS1_PAACS) {
        if(--i == 0) {
            pthread_mutex_unlock(&etha->etha_mutex);
            return -ETIMEDOUT;
        }
        nanospin_ns(1000);
    }

    /* Clear Address access completed Status */
    out32(etha->addr + MMIS1, MMIS1_PAACS);

    out32(etha->addr + MPSM, (pop << 13) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((in32(etha->addr + MMIS1) & MMIS1_PRACS) !=  MMIS1_PRACS) {
        if(--i == 0) {
            pthread_mutex_unlock(&etha->etha_mutex);
            return -ETIMEDOUT;
        }
        nanospin_ns(1000);
    }

    /* Read data */
    val = (in32(etha->addr + MPSM) & MPSM_PRD_MASK) >> 16;

    /* Clear Read access completed Status */
    out32(etha->addr + MMIS1, MMIS1_PRACS);

    pthread_mutex_unlock(&etha->etha_mutex);

    return val;
}

void rswitch2_mii_write_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad, uint16_t data)
{
    rswitch2_dev_t   *rswitch2 = handle;
    rswitch2_etha_t  *etha = &rswitch2->etha;
    int              pop = MDIO_WRITE_C45;
    uint32_t         val;
    int              i;

    pthread_mutex_lock(&etha->etha_mutex);

    /* Clear completion statuses */
    out32(etha->addr + MMIS1, 0xF);

    /* Submit address to PHY (MDIO_ADDR_C45 << 13) */
    val = MPSM_PSME | MPSM_MFF_C45;
    out32(etha->addr + MPSM, (regad << 16) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((in32(etha->addr + MMIS1) & MMIS1_PAACS) !=  MMIS1_PAACS) {
        if(--i == 0) {
            pthread_mutex_unlock(&etha->etha_mutex);
            return;
        }
        nanospin_ns(1000);
    }

    /* Clear Address access completed Status */
    out32(etha->addr + MMIS1, MMIS1_PAACS);

    out32(etha->addr + MPSM, (data << 16) | (pop << 13) | (devad << 8) | (phyad << 3) | val);

    i = RSWITCH2_TIMEOUT_MS;
    while ((in32(etha->addr + MMIS1) & MMIS1_PWACS) !=  MMIS1_PWACS) {
        if(--i == 0) {
            pthread_mutex_unlock(&etha->etha_mutex);
            return;
        }
        nanospin_ns(1000);
    }

    pthread_mutex_unlock(&etha->etha_mutex);
}

static void rswitch2_mii_callback(void *handle, uint8_t phyaddr, uint8_t linkstate)
{
    rswitch2_dev_t  *rswitch2;
    struct ifnet    *ifp;
    nic_config_t    *cfg;
    char            *s, *f;
    int             i, mode;

    rswitch2 = (rswitch2_dev_t *)handle;
    cfg = &rswitch2->cfg;
    ifp = &rswitch2->ecom.ec_if;

    switch (linkstate) {
        case MDI_LINK_UP:
            /* Link came up find out what speed duplex etc */
#if 0
            i = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PCS, 0x8008);
            if ((i & PHY_LINK) == 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "rswitch2: Read(MDI_MMD_PCS, 0x8008) falied %x", i);
                mode = 0;
            } else {
                mode = (i & PHY_LINK_SPEED);
                if (mode == PHY_LINK_SPEED) mode |= (i & PHY_LINK_SPEED2);
                mode |= (i & PHY_DUPLEX);
            }
            switch (mode) {
                case PHY_LINK_10:
                    cfg->media_rate = 10000L;
                    cfg->duplex = 0;
                    s = "10baseTX";
                    break;
                case PHY_LINK_100:
                    cfg->media_rate = 100000L;
                    cfg->duplex = 0;
                    s = "100baseTX";
                    break;
                case PHY_LINK_1000:
                    cfg->media_rate = 1000000L;
                    cfg->duplex = 0;
                    s = "1000baseT";
                    break;
                case PHY_LINK_2500:
                    cfg->media_rate = 2500000L;
                    cfg->duplex = 0;
                    s = "2500baseT";
                    break;
                case PHY_LINK_10 | PHY_DUPLEX:
                    cfg->media_rate = 10000L;
                    cfg->duplex = 1;
                    s = "10baseTX full-duplex";
                    break;
                case PHY_LINK_100 | PHY_DUPLEX:
                    cfg->media_rate = 100000L;
                    cfg->duplex = 1;
                    s = "100baseTX full-duplex";
                    break;
                case PHY_LINK_1000 | PHY_DUPLEX:
                    cfg->media_rate = 1000000L;
                    cfg->duplex = 1;
                    s = "1000baseT full-duplex";
                    break;
                case PHY_LINK_2500 | PHY_DUPLEX:
                    cfg->media_rate = 2500000L;
                    cfg->duplex = 1;
                    s = "2500baseT full-duplex";
                    break;
                default:
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                          "rswitch2: Ignoring unknown media or rswitch does not supported");
                    return;
            }
#else
            i = MDI_GetActiveMedia(rswitch2->mdi, cfg->phy_addr, &mode);
            if (i != MDI_LINK_UP) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "rswitch2: MDI_GetActiveMedia() failed : %x", i);
                mode = 0;
            }

            switch (mode) {
                case MDI_10bT:
                    cfg->media_rate = 10000L;
                    cfg->duplex = 0;
                    s = "10baseTX";
                    break;

                case MDI_10bTFD:
                    cfg->media_rate = 10000L;
                    cfg->duplex = 0;
                    s = "10baseTX full-duplex";
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
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                          "rswitch2: Ignoring unknown media or rswitch does not supported");
                    return;
            }
#endif
            /* Sort out flow control */
            rswitch2->flow_status = 0;
            f = "";
            if (cfg->duplex == 1) {
                switch (rswitch2->set_flow) {
                    case -1:
                        break;

                    case 1:
                        rswitch2->flow_status = IFM_FLOW;
                        f = "flowcontrol";
                        break;

                    case 2:
                        rswitch2->flow_status = IFM_ETH_RXPAUSE;
                        f = "rxpause";
                        break;

                    case 3:
                        rswitch2->flow_status = IFM_ETH_TXPAUSE;
                        f = "txpause";
                        break;
                }
            }

            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: %s Link up %s %s",
                      ifp->if_xname, s, f);
            }

            cfg->flags &= ~NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_UP);

            /* Set speed (bps) */
            //rswitch2_set_speed(rswitch2, rswitch2->mii, cfg->media_rate);
            break;

        case MDI_LINK_DOWN:
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: %s Link down",
                      ifp->if_xname);
            }
            cfg->media_rate = cfg->duplex = -1;
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            MDI_AutoNegotiate(rswitch2->mdi, cfg->phy_addr, NoWait);
            if_link_state_change(ifp, LINK_STATE_DOWN);
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Unknown linkstate %d",
                  linkstate);
            break;
    }
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

uint16_t m88e2110_link_status(rswitch2_dev_t *rswitch2)
{
    nic_config_t *cfg = &rswitch2->cfg;
    uint16_t     reg;
    int          timeout = 0;

    while (1) {
        reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PCS, 0x8008);
        if (reg & PHY_LINK)
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

    nic_mutex_lock(&rswitch2->drv_mutex);

    /* Check Boot Status */
    reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_PMA_PMD, 0xC050);
    if (reg & BOOT_FATAL) {
         slogf(_SLOGC_NETWORK,_SLOG_INFO, "rswitch2: PHY failed to boot, rc=0x%x", reg);
         nic_mutex_unlock(&rswitch2->drv_mutex);
        return -EINVAL;
    }

    /* Confirm Port Status */
    reg = rswitch2_mii_read_c45(rswitch2, cfg->phy_addr, MDI_MMD_VEND_2, 0xF001);
    if (!reg) {
        slogf(_SLOGC_NETWORK,_SLOG_INFO, "rswitch2: Port Error, Port status = 0x%x ", reg);
         nic_mutex_unlock(&rswitch2->drv_mutex);
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
                slogf(_SLOGC_NETWORK,_SLOG_INFO, "rswitch: PHY init timeouw, reg=0x%x", reg);
                nic_mutex_unlock(&rswitch2->drv_mutex);
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

    nic_mutex_unlock(&rswitch2->drv_mutex);

    return EOK;
}


static void m88e2110_parse_status(rswitch2_dev_t *rswitch2)
{
    uint16_t        reg, spd;
    nic_config_t    *cfg = &rswitch2->cfg;
    struct ifnet    *ifp = &rswitch2->ecom.ec_if;
    rswitch2_etha_t *etha = &rswitch2->etha;
    int              set_to_phy = 0;

    reg = m88e2110_check_link(rswitch2);
    if ((cfg->flags & NIC_FLAG_LINK_DOWN) && (reg & PHY_LINK)) {
        /* Link was down and is now up */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK,_SLOG_INFO, "rswitch2: Port %d link up", rswitch2->port);
        }
        cfg->flags &= ~NIC_FLAG_LINK_DOWN;
        spd = (reg & PHY_LINK_SPEED);
        if (spd == PHY_LINK_SPEED)
            spd |= (reg & PHY_LINK_SPEED2);
        switch (spd) {
            case PHY_LINK_10:
                cfg->media_rate =    10000L;
                break;
            case PHY_LINK_100:
                cfg->media_rate =   100000L;
                break;
            case PHY_LINK_1000:
                cfg->media_rate =  1000000L;
                break;
            case PHY_LINK_2500:
                cfg->media_rate =  2500000L;
                break;
            case PHY_LINK_5000:
            case PHY_LINK_10000:
                /* RSwitch controller supports maximun of 2.5G */
                set_to_phy = 1;
                cfg->media_rate =  2500000L;
                break;
            default:
                cfg->media_rate =  1000000L;
                set_to_phy = 1;
                break;
        }
        if (etha->phy_speed != cfg->media_rate) {
            rswitch2_set_speed(rswitch2, rswitch2->mii, cfg->media_rate);
            if (set_to_phy) m88e2110_phy_set_speed(rswitch2, cfg->media_rate);
        }
        cfg->duplex = (reg & PHY_DUPLEX) ? 1 : 0;
        if_link_state_change(ifp, LINK_STATE_UP);
    } else if (((cfg->flags & NIC_FLAG_LINK_DOWN) == 0) && ((reg & PHY_LINK) == 0)) {
        /* Link was up and is now down */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "rswitch2: Port %d link down", rswitch2->port);
        }
        cfg->media_rate = -1;
        cfg->duplex = -1;
        cfg->flags |= NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_DOWN);
    }
}

static void rswitch2_do_mdi_monitor(void* arg)
{
    rswitch2_dev_t *rswitch2   = arg;
    MDI_MonitorPhy(rswitch2->mdi);
    m88e2110_parse_status(rswitch2);

    kthread_exit(0);
}

void rswitch2_MDI_MonitorPhy(void *arg)
{
    rswitch2_dev_t  *rswitch2;
    nic_config_t    *cfg;

    rswitch2 = arg;
    cfg = &rswitch2->cfg;

    if (((cfg->flags & NIC_FLAG_LINK_DOWN) != 0) || !rswitch2->pkts_received) {
        if (cfg->verbose & VERBOSE_PHY) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Check port %d link state", rswitch2->port);
        }
        kthread_create1(rswitch2_do_mdi_monitor, arg, NULL, NULL);
    }

    //rswitch2->pkts_received = 0;

    /*
     * With 32 bit counters and 100Mb/s max pps the stats max out in 8 hours.
     * Without an interrupt to warn of approaching max, we need to poll
     * faster than that. We already have a 3 second poller here and the
     * poll of the stats is lightweight, 16 reads and writes, so we poll
     * here rather than setting up a new poller.
     */
    rswitch2_update_stats(rswitch2);

    callout_msec(&rswitch2->mii_callout, 3 * 1000, rswitch2_MDI_MonitorPhy, arg);
}

static void rswitch2_mediastatus(struct ifnet *ifp, struct ifmediareq *ifmr)
{
    rswitch2_dev_t      *rswitch2;

    rswitch2 = ifp->if_softc;

    rswitch2->bsd_mii.mii_media_active = IFM_ETHER;
    rswitch2->bsd_mii.mii_media_status = IFM_AVALID;

    if ((rswitch2->cfg.flags & NIC_FLAG_LINK_DOWN) != 0) {
        rswitch2->bsd_mii.mii_media_active |= IFM_NONE;
    } else {
        rswitch2->bsd_mii.mii_media_status |= IFM_ACTIVE;

        switch (rswitch2->cfg.media_rate) {
            case 10000L:
                rswitch2->bsd_mii.mii_media_active |= IFM_10_T;
                break;

            case 100000L:
                rswitch2->bsd_mii.mii_media_active |= IFM_100_TX;
                break;

            case 1000000L:
                rswitch2->bsd_mii.mii_media_active |= IFM_1000_T;
                break;

            default:
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                      "rswitch2: Unknown media, forcing none");
            /* Fallthrough */
            case 0:
                rswitch2->bsd_mii.mii_media_active |= IFM_NONE;
                break;
        }

        if (rswitch2->cfg.duplex) {
            rswitch2->bsd_mii.mii_media_active |= IFM_FDX;
        }

        rswitch2->bsd_mii.mii_media_active |= rswitch2->flow_status;
    }

    /* Return the data */
    ifmr->ifm_status = rswitch2->bsd_mii.mii_media_status;
    ifmr->ifm_active = rswitch2->bsd_mii.mii_media_active;
}

int rswitch2_mediachange(struct ifnet *ifp)
{
    rswitch2_dev_t   *rswitch2;
    struct ifmedia  *ifm;
    nic_config_t    *cfg;
    int             an_media;

    if (!(ifp->if_flags & IFF_UP)) {
        return EOK;
    }

    rswitch2 = ifp->if_softc;
    ifm = &rswitch2->bsd_mii.mii_media;
    cfg = &rswitch2->cfg;

    /* Media is changing so link will be down until autoneg completes */
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    switch (ifm->ifm_media & IFM_TMASK) {
        case IFM_NONE:
            rswitch2->set_speed = 0;
            rswitch2->set_duplex = 0;
            rswitch2->set_flow = 0;

            /* Special case, shut down the PHY and bail out */
            callout_stop(&rswitch2->mii_callout);
            MDI_DisableMonitor(rswitch2->mdi);
            MDI_PowerdownPhy(rswitch2->mdi, cfg->phy_addr);
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_DOWN);
            return EOK;

        case IFM_AUTO:
            rswitch2->set_speed = -1;
            rswitch2->set_duplex = -1;
            rswitch2->set_flow = -1;

            MDI_GetMediaCapable(rswitch2->mdi, cfg->phy_addr, &an_media);
            an_media |= MDI_FLOW | MDI_FLOW_ASYM;
            break;

        case IFM_100_TX:
            rswitch2->set_speed = 100000L;
            rswitch2->set_duplex = 0;
            rswitch2->set_flow = 0;

            if ((ifm->ifm_media & IFM_FDX) == 0) {
                an_media = MDI_100bT;
            }
            else {
                rswitch2->set_duplex = 1;
                an_media = MDI_100bTFD;
                if (ifm->ifm_media & IFM_FLOW) {
                    rswitch2->set_flow = 1;
                    an_media |= MDI_FLOW;
                }
                else if (ifm->ifm_media & IFM_ETH_RXPAUSE) {
                    rswitch2->set_flow = 2;
                    an_media |= MDI_FLOW | MDI_FLOW_ASYM;
                }
                else if (ifm->ifm_media & IFM_ETH_TXPAUSE) {
                    rswitch2->set_flow = 3;
                    an_media |= MDI_FLOW_ASYM;
                }
            }
            break;

        case IFM_1000_T:
            rswitch2->set_speed = 1000000L;
            rswitch2->set_duplex = 0;
            rswitch2->set_flow = 0;

            if ((ifm->ifm_media & IFM_FDX) == 0) {
                an_media = MDI_1000bT;
            }
            else {
                rswitch2->set_duplex = 1;
                an_media = MDI_1000bTFD;
                if (ifm->ifm_media & IFM_FLOW) {
                    rswitch2->set_flow = 1;
                    an_media |= MDI_FLOW;
                }
                else if (ifm->ifm_media & IFM_ETH_RXPAUSE) {
                    rswitch2->set_flow = 2;
                    an_media |= MDI_FLOW | MDI_FLOW_ASYM;
                }
                else if (ifm->ifm_media & IFM_ETH_TXPAUSE) {
                    rswitch2->set_flow = 3;
                    an_media |= MDI_FLOW_ASYM;
                }
            }
            break;

        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Unknown media type");
            return 1;
    }
    MDI_PowerupPhy(rswitch2->mdi, cfg->phy_addr);
    if (rswitch2_is_br_phy(rswitch2)) {
        m88e2110_phy_init(rswitch2);
    }
    MDI_EnableMonitor(rswitch2->mdi, 0);
    MDI_SetAdvert(rswitch2->mdi, cfg->phy_addr, an_media);
    MDI_AutoNegotiate(rswitch2->mdi, cfg->phy_addr, NoWait);
    rswitch2_MDI_MonitorPhy (rswitch2);

    return EOK;
}

static void rswitch2_mediainit(rswitch2_dev_t *rswitch2)
{
    nic_config_t    *cfg;
    struct ifmedia  *ifm;

    cfg = &rswitch2->cfg;
    ifm = &rswitch2->bsd_mii.mii_media;

    rswitch2->bsd_mii.mii_ifp = &rswitch2->ecom.ec_if;

    ifmedia_init(ifm, IFM_IMASK, rswitch2_mediachange, rswitch2_mediastatus);
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
    rswitch2->set_speed = cfg->media_rate;
    rswitch2->set_duplex = cfg->duplex;
    cfg->media_rate = 0;
    cfg->duplex = 0;

    switch (rswitch2->set_speed) {
        case -1:
            ifm->ifm_media = IFM_ETHER | IFM_AUTO;
            break;

        case 100 *1000L:
            ifm->ifm_media = IFM_ETHER | IFM_100_TX;
            if (rswitch2->set_duplex != 0) {
                ifm->ifm_media |= IFM_FDX;
                switch (rswitch2->set_flow) {
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
            if (rswitch2->set_duplex != 0) {
                ifm->ifm_media |= IFM_FDX;
                switch (rswitch2->set_flow) {
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
                  "rswitch2: Unknown initial media, forcing none");
        /* Fallthrough */

        case 0:
            ifm->ifm_media = IFM_ETHER | IFM_NONE;
            break;
    }

    ifmedia_set(ifm, ifm->ifm_media);
}

int rswitch2_phy_init(rswitch2_dev_t *rswitch2)
{
    nic_config_t *cfg;
    struct ifnet *ifp;
    int          rc, phy_addr = -1, i;

    cfg = &rswitch2->cfg;
    ifp = &rswitch2->ecom.ec_if;

    rc = MDI_Register_ExtendedCl45(rswitch2, rswitch2_mii_write_c45,
                rswitch2_mii_read_c45, rswitch2_mii_callback, &rswitch2->mdi, NULL, 0, 0);
    if (rc != MDI_SUCCESS) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rswitch2: Failed to register with MDI");
        return (ENODEV);
    }

    for (i = 0; i < 32; i++) {
        if (MDI_FindPhy (rswitch2->mdi, i) == MDI_SUCCESS) {
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: PHY at address %d", i);
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

    rc = MDI_InitPhy(rswitch2->mdi, cfg->phy_addr);
    if (rc != MDI_SUCCESS) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Failed to init the PHY");
        return ENODEV;
    }

    if (rswitch2_is_br_phy(rswitch2)) {
        rc = MDI_ResetPhy(rswitch2->mdi, cfg->phy_addr, WaitBusy);
        if (rc != MDI_SUCCESS) {
            slogf(_SLOGC_NETWORK,_SLOG_ERROR, "%s(): Failed to reset the PHY: %d",
                __FUNCTION__, rc);
            return rc;
        }
        MDI_PowerdownPhy(rswitch2->mdi, cfg->phy_addr);
    } else {
        rc = m88e2110_phy_init(rswitch2);
        if (rc != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rswitch2: rswitch2_phy_init failed: %d", rc);
            return rc;
        }
    }

    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    rswitch2_mediainit(rswitch2);

    return EOK;
}

void rswitch2_phy_fini(rswitch2_dev_t *rswitch2)
{
    ifmedia_delete_instance(&rswitch2->bsd_mii.mii_media, IFM_INST_ANY);

    MDI_DeRegister(&rswitch2->mdi);
    rswitch2->mdi = NULL;
}

int rswitch2_is_br_phy(rswitch2_dev_t *rswitch2) 
{
    uint32_t PhyAddr;
    int      is_br = 0;

    PhyAddr = rswitch2->cfg.phy_addr;
    switch (rswitch2->mdi->PhyData[PhyAddr]->VendorOUI) {
    case MARVELLX:
        switch (rswitch2->mdi->PhyData[PhyAddr]->Model) {
            case MV88Q2110:
                is_br = 1;
                break;
        }
        break;
    }

    return is_br;
}
