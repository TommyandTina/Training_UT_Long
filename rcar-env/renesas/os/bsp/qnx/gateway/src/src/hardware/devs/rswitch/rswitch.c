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

#include "rswitch.h"
#include <unistd.h>
#include <stdlib.h>

static int rswitch2_probe(device_t dev);
static int rswitch2_attach (device_t dev);
static int rswitch2_detach (device_t dev);
static int rswitch2_shutdown(device_t dev);

int drvr_ver = IOSOCK_VERSION_CUR;
SYSCTL_INT(_qnx_driver, OID_AUTO, rswitch2, CTLFLAG_RD, &drvr_ver, 0,
           "Version");

static device_method_t rswitch2_methods[] = {
        DEVMETHOD(device_probe,         rswitch2_probe),
        DEVMETHOD(device_attach,        rswitch2_attach),
        DEVMETHOD(device_detach,        rswitch2_detach),
        DEVMETHOD(device_shutdown,      rswitch2_shutdown),

        /* MII Interface Callback*/  
        DEVMETHOD(miibus_readreg,       rswitch2_miibus_read_reg),
        DEVMETHOD(miibus_writereg,      rswitch2_miibus_write_reg),
        DEVMETHOD(miibus_statchg,       rswitch2_miibus_statchg),
        DEVMETHOD_END
};

driver_t rswitch2_driver ={       
    "tsn",       
    rswitch2_methods,       
    sizeof(rswitch2_dev_t),
};

static devclass_t rswitch2_devclass;

DRIVER_MODULE(tsn, simplebus, rswitch2_driver, rswitch2_devclass, 0, 0);
DRIVER_MODULE(miibus, tsn, miibus_driver, miibus_devclass, 0, 0);

MODULE_DEPEND(tsn, ether, 1, 1, 1);
MODULE_DEPEND(tsn, miibus, 1, 1, 1);

struct _iosock_module_version iosock_module_version = IOSOCK_MODULE_VER_SYM_INIT;
static void rswitch2_uninit(void *arg)
{
}
SYSUNINIT(rswitch2_uninit, SI_SUB_DUMMY, SI_ORDER_ANY, rswitch2_uninit, NULL);

static struct resource_spec rswitch2_res_spec[] = {
    { SYS_RES_MEMORY, 0, RF_ACTIVE },
    { SYS_RES_MEMORY, 1, RF_ACTIVE },
    { -1, 0 }
};

static struct resource_spec irq_res_spec[] = {
    { SYS_RES_IRQ, 0, RF_ACTIVE },
    { SYS_RES_IRQ, 1, RF_ACTIVE },
    { -1, 0 }
};

static struct {
    void (*rswitch2_intr_cb)(void *);
} rswitch2_intrs [] = {
    { rswitch2_gwca_isr },
    { rswitch2_gwca_isr },
};

/* Declares are common for 3 ports simultaneously
 * and they are initialized at first port
 * then re-used for later other ports
 */
static struct resource *irq_res[RSWITCH2_IRQ_NUM];
static struct resource *base_res[RSWITCH2_BASE_NUM];
static bus_dma_tag_t desc_bat_tag;
static bus_dmamap_t  desc_bat_map;
static bus_addr_t    desc_bat_paddr;
static RSWITCH2_DESC * desc_bat_base;
static int  instance = 0;

static int rswitch2_etha_mode_change(rswitch2_dev_t *rswitch2, int mode);
static int rswitch2_gwca_mode_change(rswitch2_dev_t *rswitch2, int mode);

int rswitch2_wait(rswitch2_dev_t *rswitch2, int reg, uint32_t mask, uint32_t value)
{
    int i;

    for (i = 0; i < 10000; i++) {
        if ((bus_read_4(rswitch2->res[0],  reg) & mask) == value)
            return EOK;
        delay(10);
    }
    return ETIMEDOUT;
}

static void rswitch2_serdes_write(struct resource *res, uint32_t off, uint32_t reg, uint32_t bank, uint32_t data)
{
    bus_write_4(res, off + RSWITCH2_SERDES_BANK_OFFSET, bank);
    bus_write_4(res, off + reg, data);
}

static uint32_t rswitch2_serdes_read(struct resource *res, uint32_t off, uint32_t reg, uint32_t bank)
{
    bus_write_4(res, off + RSWITCH2_SERDES_BANK_OFFSET, bank);
    return bus_read_4(res, off + reg);
}

static int rswitch2_serdes_reg_wait(struct resource *res, uint32_t off, uint32_t reg, uint32_t bank, uint32_t mask, uint32_t val)
{
    int i = 0;

    bus_write_4(res, off + RSWITCH2_SERDES_BANK_OFFSET, bank);
    delay(1);
    while (i < RSWITCH2_TIMEOUT_MS) {
        if ((bus_read_4(res, off + reg) & mask) == val)
            return EOK;
        delay(1);
        i++;
    }

    return -ETIMEDOUT;
}

static int rswitch2_serdes_common_init_ram(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t     *etha = &rswitch2->etha;
    uint32_t            off = etha->index * RSWITCH2_SERDES_OFFSET;
    int                 rc;

    /* Serdes channel n */
    rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x026C, 0x180, BIT(0), 0x01);
    if (rc)
        return rc;

    /* Serdes channel 0 */
    rswitch2_serdes_write(etha->serdes_res, 0, 0x026C, 0x180, 0x03);
    rc = rswitch2_serdes_reg_wait(etha->serdes_res, 0, 0x0000, 0x300, BIT(15), 0);

    return rc;
}

static int rswitch2_serdes_common_setting(rswitch2_dev_t *rswitch2, int mode)
{
    rswitch2_etha_t     *etha = &rswitch2->etha;

    /* Serdes channel 0 */
    if (mode == SERDES_SGMII) {
        rswitch2_serdes_write(etha->serdes_res, 0, 0x0244, 0x180, 0x0097);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01D0, 0x180, 0x0060);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01D8, 0x180, 0x2200);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01D4, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01E0, 0x180, 0x003d);
    } else if (mode == SERDES_USXGMII) {
        rswitch2_serdes_write(etha->serdes_res, 0, 0x0244, 0x180, 0x0057);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01CC, 0x180, 0xC200);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01C4, 0x180, 0x0042);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01C8, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, 0, 0x01DC, 0x180, 0x002F);
    } else {
        return -EINVAL;
    }

    return EOK;
}

static int rswitch2_serdes_channel_settings(rswitch2_dev_t *rswitch2, int mode)
{
    rswitch2_etha_t    *etha = &rswitch2->etha;
    uint32_t           off  = etha->index * RSWITCH2_SERDES_OFFSET;
    int                rc;

    /* Serdes channel n */
    if (mode == SERDES_SGMII) {
        rswitch2_serdes_write(etha->serdes_res, off, 0x001C, 0x300, 0x0001);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x380, 0x2000);

        rswitch2_serdes_write(etha->serdes_res, off, 0x01C0, 0x180, 0x0011);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0248, 0x180, 0x0540);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0258, 0x180, 0x0015);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0144, 0x180, 0x0100);
        rswitch2_serdes_write(etha->serdes_res, off, 0x01A0, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00D0, 0x180, 0x0002);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0150, 0x180, 0x0003);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00C8, 0x180, 0x0100);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0148, 0x180, 0x0100);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0174, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0160, 0x180, 0x0007);
        rswitch2_serdes_write(etha->serdes_res, off, 0x01AC, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00C4, 0x180, 0x0310);
        if (rswitch2->port != 0) {
            rswitch2_serdes_write(etha->serdes_res, off, 0x00C8, 0x180, 0x0101);
            rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x00C8, 0x180, BIT(0), 0);
            if (rc) {
                device_printf(rswitch2->dev, "rswitch2: SerDes check bit0 of register C8 failed\n");
                return rc;
            }

            rswitch2_serdes_write(etha->serdes_res, off, 0x0148, 0x180, 0x101);
            rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0148, 0x180, BIT(0), 0);
            if (rc) {
                device_printf(rswitch2->dev, "rswitch2: SerDes bit0 of register 148 failed\n");
                return rc;
            }
        } else {
            rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x380, 0xa000);
            rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0000, 0x380, BIT(15), 0);
            if (rc) {
                device_printf(rswitch2->dev, "rswitch2: SerDes check bit15 of register 0 failed\n");
                return rc;
            }
        }
        
        rswitch2_serdes_write(etha->serdes_res, off, 0x00C4, 0x180, 0x1310);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00D8, 0x180, 0x1800);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00DC, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x001C, 0x300, 0x0001);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x380, 0x2100);
        rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0000, 0x380, BIT(8), 0x0000);
        if (rc) {
            return rc;
        }

        uint32_t val = rswitch2_serdes_read(etha->serdes_res, off, 0x0004, 0x1F80);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0004, 0x1F80, val | 0x100);

    } else if (mode == SERDES_USXGMII) {
        rswitch2_serdes_write(etha->serdes_res, off, 0x001C, 0x380, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x380, 0x2200);
        rswitch2_serdes_write(etha->serdes_res, off, 0x001C, 0x380, 0x0800);

        rswitch2_serdes_write(etha->serdes_res, off, 0x001C, 0x180, 0x0800);
        rswitch2_serdes_write(etha->serdes_res, off, 0x01C0, 0x180, 0x0001);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0248, 0x180, 0x056A);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0258, 0x180, 0x0015);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0144, 0x180, 0x1100);
        rswitch2_serdes_write(etha->serdes_res, off, 0x01A0, 0x180, 0x0001);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00D0, 0x180, 0x0002);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0150, 0x180, 0x0002);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00C8, 0x180, 0x0300);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0148, 0x180, 0x0300);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0174, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0160, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x01AC, 0x180, 0x0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00C4, 0x180, 0x0310);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00C8, 0x180, 0x0301);
        rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x00C8, 0x180, BIT(0), 0);
        if (rc) {
            device_printf(rswitch2->dev, "rswitch2: SerDes check bit0 of register C8 failed\n");
            return rc;
        }
        rswitch2_serdes_write(etha->serdes_res, off, 0x0148, 0x180, 0x0301);
        rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0148, 0x180, BIT(0), 0);
        if (rc) {
            device_printf(rswitch2->dev, "rswitch2: SerDes check bit0 of register 0x148 failed\n");
            return rc;
        }
        rswitch2_serdes_write(etha->serdes_res, off, 0x00C4, 0x180, 1310);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00D8, 0x180, 1800);
        rswitch2_serdes_write(etha->serdes_res, off, 0x00DC, 0x180, 0000);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x380, 2300);
        rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0000, 0x380, BIT(0), 0);
        if (rc) {
            device_printf(rswitch2->dev, "rswitch2: SerDes check bit0 of register 0 failed\n");
            return rc;
        }
    } else {
        device_printf(rswitch2->dev, "rswitch2: Unknown SerDes mode (Only support SGMII or USXGMII)\n");
        return -ENOTSUP;
    }
    
    return EOK;
}

int rswitch2_set_speed(rswitch2_dev_t *rswitch2, int mode, uint32_t speed)
{
    rswitch2_etha_t     *etha = &rswitch2->etha;
    uint32_t            off  = etha->index * RSWITCH2_SERDES_OFFSET;
    uint32_t            val = 0;
    int                 etha_mode = etha->mode;
    int                 rc = EOK;

    if (mode == SERDES_SGMII) {
        switch (speed) {
        case 1000*1000:
            rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x1F00, 0x0140);
            val |= MPIC_LSC_1000;
            break;
        case 100*1000:
            rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x1F00, 0x2100);
             val |= MPIC_LSC_100;
            break;
        case 10*1000:
            rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x1F00, 0x0100);
            break;
        default:
            device_printf(rswitch2->dev, "rswitch2: Don't support this speed(%d mbps)", speed/1000);
            return -ENOTSUP;
        }
    } else if (mode == SERDES_USXGMII) {
        switch (speed) {
        case 2500000:
            rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x1F00, 0x0120);
            val |= MPIC_LSC_2500;
            break;
        case 100000:
            rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x1F00, 0x0140);
            val |= MPIC_LSC_1000;
            break;
        default:
            device_printf(rswitch2->dev, "rswitch2: Don't support this speed(%d mbps) in USXGMII mode\n", speed/1000);
            return -ENOTSUP;
        }
        delay(1);
        rswitch2_serdes_write(etha->serdes_res, off, 0x0000, 0x380, 0x0000);
        rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0148, 0x380, BIT(10), 0);
        if (rc) {
            device_printf(rswitch2->dev, "rswitch2: SerDes sets speed to %dMHz failed", speed/1000);
            return rc;
        }
    } else {
       device_printf(rswitch2->dev, "rswitch2: Unknown SerDes mode (Only support SGMII or USXGMII mode)\n");
        return -ENOTSUP;
    }

    mtx_lock(&rswitch2->mtx);
    if (etha->mode != EAMC_OPC_CONFIG) {
        rc = rswitch2_etha_mode_change(rswitch2, EAMC_OPC_CONFIG);
        if (rc) {
            device_printf(rswitch2->dev, "rswitch2: %s cannot change mode to EAMC_OPC_CONFIG", __func__);
            mtx_unlock(&rswitch2->mtx);
            return rc;
        }
    }

    val = (bus_read_4(rswitch2->res[0], etha->addr_offset + MPIC) & ~MPIC_LSC_MASK) | val;
    /* Set RMAC speed */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MPIC, val);
    etha->phy_speed = speed;

    if (etha_mode != EAMC_OPC_CONFIG) {
        rc = rswitch2_etha_mode_change(rswitch2, etha_mode);
        if (rc) {
            device_printf(rswitch2->dev, "rswitch2: %s cannot recover mode to EAMC_OPC_OPERATION", __func__);
            mtx_unlock(&rswitch2->mtx);
            return rc;
        }
    }
    mtx_unlock(&rswitch2->mtx);

    return EOK;
}

static int rswitch2_serdes_init(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t     *etha = &rswitch2->etha;
    uint32_t            off  = etha->index * RSWITCH2_SERDES_OFFSET;
    int                 rc;

    /* Initialize SRAM */
    rc = rswitch2_serdes_common_init_ram(rswitch2);
    if (rc) {
        rc = errno;
        device_printf(rswitch2->dev, "rswitch2: Initialize SerDes SRAM failed.\n");
        return rc;
    }

    rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0000, 0x300, BIT(15), 0);
    if (rc) {
        return rc;
    }

    rswitch2_serdes_write(etha->serdes_res, off, 0x03D4, 0x380, 0x0443);

    /* Set common setting */
    rc = rswitch2_serdes_common_setting(rswitch2, rswitch2->mii);
    if (rc) {
        rc = errno;
        device_printf(rswitch2->dev, "rswitch2: Set SerDes common setting failed.\n");
        return rc;
    }

    rswitch2_serdes_write(etha->serdes_res, off, 0x03D0, 0x380, 0x0001);

    /* Assert softreset for PHY */
    if (rswitch2->port == 0) {
        rswitch2_serdes_write(etha->serdes_res, 0, 0x0000, 0x0380, 0x8000);
    }

    /* Initialize SRAM */
    rc = rswitch2_serdes_common_init_ram(rswitch2);
    if (rc) {
        rc = errno;
        device_printf(rswitch2->dev, "rswitch2: Re-initialize SerDes SRAM failed.\n");
        return rc;
    }

    /* Check softreset done */
    if (rswitch2->port == 0) {
        rswitch2_serdes_reg_wait(etha->serdes_res, 0, 0x0000, 0x380, BIT(15), 0);
    }

    /* Set channel settings */
    rc = rswitch2_serdes_channel_settings(rswitch2, rswitch2->mii);
    if (rc) {
        rc = errno;
        device_printf(rswitch2->dev, "rswitch2: Set SerDes channel settings failed.\n");
        return rc;
    }

    /* Select speed (bps) */
    rc = rswitch2_set_speed(rswitch2, rswitch2->mii, (&rswitch2->etha)->phy_speed);
    if (rc) {
        rc = errno;
        device_printf(rswitch2->dev, "rswitch2: Select Serdes speed (bps)  failed.\n");
        return rc;
    }

    rswitch2_serdes_write(etha->serdes_res, off, 0x03C0, 0x380, 0x0000);
    rswitch2_serdes_write(etha->serdes_res, off, 0x03D0, 0x380, 0x0000);

    rc = rswitch2_serdes_reg_wait(etha->serdes_res, off, 0x0004, 0x300, BIT(2), BIT(2));
    if (rc) {
        device_printf(rswitch2->dev, "rswitch2: SerDes Link up failed\n");
        return rc;
    }

    return EOK;
}

static int rswitch2_etha_mode_change(rswitch2_dev_t *rswitch2, int mode)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    int             rc;

    /* Change to Disable mode first */
    bus_write_4(rswitch2->res[0], etha->addr_offset + EAMC, EAMC_OPC_DISABLE);
    rc = rswitch2_reg_wait(rswitch2->res[0], etha->addr_offset + EAMS, EAMS_OPS_MASK, EAMC_OPC_DISABLE);
    if (rc) return rc;

    etha->mode = EAMC_OPC_DISABLE;

    /* Then change to expected mode */
    if (mode != EAMC_OPC_DISABLE) {
        bus_write_4(rswitch2->res[0], etha->addr_offset + EAMC, mode);
        rc = rswitch2_reg_wait(rswitch2->res[0], etha->addr_offset + EAMS, EAMS_OPS_MASK, mode);
        if (rc) return rc;
    }

    etha->mode = mode;

    return rc;
}

static int rswitch2_gwca_mode_change(rswitch2_dev_t *rswitch2, int mode)
{
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             rc;

    /* Change to Disable mode first */
    bus_write_4(rswitch2->res[0], gwca->addr_offset + GWMC, GWMC_OPC_DISABLE);
    rc = rswitch2_reg_wait(rswitch2->res[0], gwca->addr_offset + GWMS, GWMS_OPS_MASK, GWMC_OPC_DISABLE);
    if (rc)
        return rc;

    /* Change expected mode */
    if (mode != GWMC_OPC_DISABLE) {
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWMC, mode);
        rc = rswitch2_reg_wait(rswitch2->res[0], gwca->addr_offset + GWMS, GWMS_OPS_MASK, mode);
    }

    gwca->mode = mode;

    return rc;
}

static int rswitch2_check_link(rswitch2_dev_t *rswitch2, rswitch2_etha_t *etha)
{
    int rc;
    int timeout = 1000;

    /* Request Link Verification */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MLVC, MLVC_PLV);

    for(; timeout; --timeout) {
        /* Complete Link Verification */
        if ((rc = rswitch2_reg_wait(rswitch2->res[0], etha->addr_offset + MLVC, MLVC_PLV, 0)) == 0) {
            break;
        }
    }

    if (timeout == 0) {
        device_printf(rswitch2->dev, "rswitch2: Link verification timeout!\n");
        return rc;
    }

    return EOK;
}

static int rswitch2_gwca_hw_index(rswitch2_gwca_t *gwca)
{
    if (gwca->index == 0)
        return RSWITCH2_GWCA0_HW_INDEX;
    else
        return RSWITCH2_GWCA1_HW_INDEX;
}

static void rswitch2_clock_enable(rswitch2_dev_t *rswitch2, int enable)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    uint32_t val;

    if (enable) {
        val = bus_read_4(rswitch2->res[0], RCEC);
        bus_write_4(rswitch2->res[0], RCEC, val | BIT(etha->index) | BIT(rswitch2_gwca_hw_index(gwca)) | RCEC_RCE);
    }
    else {
        val = bus_read_4(rswitch2->res[0], RCDC);
        bus_write_4(rswitch2->res[0], RCDC, val | BIT(etha->index) | BIT(rswitch2_gwca_hw_index(gwca)));
    }
}

static void rswitch2_clock_agent_etha(rswitch2_dev_t *rswitch2, int enable)
{
    rswitch2_etha_t *etha = &rswitch2->etha;

    if (enable)
        bus_write_4(rswitch2->res[0], RCEC, BIT(etha->index) | RCEC_RCE);
    else
        bus_write_4(rswitch2->res[0], RCDC, BIT(etha->index));
}

static void rswitch2_reset(rswitch2_dev_t *rswitch2)
{
    if (rswitch2->parallel_mode)
        return;

    bus_write_4(rswitch2->res[0], RRC, RRC_RR);
    bus_write_4(rswitch2->res[0], RRC, 0);
}

static int rswitch2_bpool_init(rswitch2_dev_t *rswitch2)
{
    uint32_t val;

    if (rswitch2->parallel_mode) {
        return 0;
    }

    val = bus_read_4(rswitch2->res[0], CABPIRM);
    if (val & CABPIRM_BPR)
        return 0;

    bus_write_4(rswitch2->res[0], CABPIRM, CABPIRM_BPIOG);
    return rswitch2_reg_wait(rswitch2->res[0], CABPIRM, CABPIRM_BPR, CABPIRM_BPR);
}

static void rswitch2_mfwd_init(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int gwca_hw_idx = rswitch2_gwca_hw_index(gwca);

    if (!rswitch2->parallel_mode && !rswitch2->instance) {
        for (int i = 0; i < 5; i++) {
            bus_write_4(rswitch2->res[0], FWPC0(i), FWPC0_DEFAULT);
            bus_write_4(rswitch2->res[0], FWPBFC(i), 0);
        }
    }

    if (!rswitch2->gwca.used && !rswitch2->instance) {
        bus_write_4(rswitch2->res[0], FWPC1(gwca_hw_idx), BIT(1) | BIT(0));
    }

    bus_write_4(rswitch2->res[0], FWPBFCSDC(gwca->index, etha->index), 
            rswitch2->rx_queue_index);

    bus_write_4(rswitch2->res[0], FWPBFC(etha->index), BIT(gwca_hw_idx));

    bus_write_4(rswitch2->res[0], FWPBFC(gwca_hw_idx),
            bus_read_4(rswitch2->res[0], FWPBFC(gwca_hw_idx)) | BIT(etha->index));
}

static void rswitch2_gwca_set_rate_limit(rswitch2_dev_t *rswitch2, rswitch2_gwca_t *gwca)
{
    uint32_t gwgrlulc, gwgrlc;

    switch (gwca->speed) {
    case 1000*1000:
        gwgrlulc = 0x0000005f;
        gwgrlc = 0x00010260;
        break;
    default:
        device_printf(rswitch2->dev, "rswitch2: This rate is not supported (%d)\n", gwca->speed);
        return;
    }

    bus_write_4(rswitch2->res[0], gwca->addr_offset + GWGRLULC, gwgrlulc);
    bus_write_4(rswitch2->res[0], gwca->addr_offset + GWGRLC, gwgrlc);
}

static int rswitch2_gwca_init(rswitch2_dev_t *rswitch2)
{
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             rc = 0;

    /* This is first time to use GWCA0 or GWCA1 */
    if (!rswitch2->gwca.used && !rswitch2->instance) {
        /* Change config mode to configure GWCA */
        rc = rswitch2_gwca_mode_change(rswitch2, GWMC_OPC_CONFIG);
        if (rc)
            return rc;
        /* Reset multicast table */
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWMTIRM, GWMTIRM_MTIOG);
        rc = rswitch2_reg_wait(rswitch2->res[0], gwca->addr_offset + GWMTIRM, GWMTIRM_MTR, GWMTIRM_MTR);
        if (rc)
            return rc;

        /* Reset AXI RAM */
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWARIRM, GWARIRM_ARIOG);
        rc = rswitch2_reg_wait(rswitch2->res[0], gwca->addr_offset + GWARIRM, GWARIRM_ARR, GWARIRM_ARR);
        if (rc)
            return rc;

        /* Setting flow */
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWVCC, GWVCC_VEM_SC_TAG);
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWTTFC, 0);
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWDCBAC0, (uint32_t)(desc_bat_paddr >> 32) & GWDCBAC0_DCBAUP);
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWDCBAC1, (uint32_t)desc_bat_paddr);
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWMDNC, GWMDNC_TSDMN(0x1) | GWMDNC_TXDMN(0xF) | GWMDNC_RXDMN(0x1));
        gwca->speed = 1000*1000;
        rswitch2_gwca_set_rate_limit(rswitch2, gwca);
    
        /* Set interrupt delay.
         * Base on RTM: Interrupt delay function aims at delaying data interrupts 
         * to accumulate data during a certain among of time before requesting software 
         * to process the data. As a result, software load is diminished, and data latency 
         * is still guaranteed.
         * (Without delay, TX and RX performance is poor)
         * FIXME : How much is best for interrupt delay ?
         */
        //bus_write_4(rswitch2->res[0], gwca->addr_offset + GWIDPC, 1023);
        //bus_write_4(rswitch2->res[0], gwca->addr_offset + GWIDC(rswitch2->rx_queue_index), 32 * 8);

        /* IPV remapping configuration */
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWIRC, 0x76543210);
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWIRC, 0x00000000);
    } else {
        //bus_write_4(rswitch2->res[0], gwca->addr_offset + GWIDC(rswitch2->rx_queue_index), 32 * 8);
    }

    for (int i = 0; i < NUM_RX_QUEUES; i++) {
        int idx = i + rswitch2->rx_queue_index;
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWDCC(idx), GWDCC_BALR | GWDCC_ETS | GWDCC_EDE);
        //bus_write_4(rswitch2->res[0], gwca->addr_offset +  GWRDQDC(idx), NUM_RX_DESC);
    }

    for (int i = 0; i < NUM_TX_QUEUES; i++) {
        int idx = i + rswitch2->tx_queue_index;
        bus_write_4(rswitch2->res[0], gwca->addr_offset + GWDCC(idx), GWDCC_BALR | GWDCC_DQT | GWDCC_EDE);
    }

    return EOK;
}

static int rswitch2_gwca_start(rswitch2_dev_t *rswitch2)
{
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             mode;
    int             rc;

    mode = bus_read_4(rswitch2->res[0], gwca->addr_offset + GWMC) & GWMS_OPS_MASK;

    if (mode == GWMC_OPC_OPERATION) {
        return EOK;
    }

    rc = rswitch2_gwca_mode_change(rswitch2, GWMC_OPC_OPERATION);
    if (rc) {
        return rc;
    }

    return EOK;
}

static void rswitch2_rmac_mdc_clock(rswitch2_dev_t *rswitch2, int enable)
{
    uint32_t val;
    rswitch2_etha_t *etha = &rswitch2->etha;
    
    val = bus_read_4(rswitch2->res[0], etha->addr_offset + MPIC);
    if (enable) {
        val &= ~(MPIC_PSMCS_MASK | MPIC_PSMHT_MASK);
        val |= MPIC_PSMCS(0x05) | MPIC_PSMHT(0x06);
        //val |= MPIC_PSMCS(0x3F) | MPIC_PSMHT(0x06);
    } else {
        val &= ~(MPIC_PSMCS_MASK);
    }
    bus_write_4(rswitch2->res[0], etha->addr_offset + MPIC, val);
}

static int rswitch2_etha_init(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    unsigned char   *mac = rswitch2->current_address;
    int             rc = EOK;
    uint32_t        val;

    rc = rswitch2_etha_mode_change(rswitch2, EAMC_OPC_CONFIG);
    if (rc)
        goto done;

    /* Set MAC address */
    bus_write_4(rswitch2->res[0], etha->addr_offset + MRMAC1, mac[5] | (mac[4] << 8) | (mac[3] << 16)  | (mac[2] << 24));
    bus_write_4(rswitch2->res[0], etha->addr_offset + MRMAC0, mac[1] | (mac[0] << 8));

    /* Set MIIx */
    if (rswitch2->mii == SERDES_SGMII)
        val = MPIC_PIS_GMII;
    else
        val = MPIC_PIS_XGMII;

    if (etha->phy_speed == 100 * 1000)
        val |= MPIC_LSC_100;
    else if (etha->phy_speed == 1000 * 1000)
        val |= MPIC_LSC_1000;
    else if (etha->phy_speed == 2500 * 1000)
        val |= MPIC_LSC_2500;

    bus_write_4(rswitch2->res[0], etha->addr_offset + MPIC, val);

    /* IPV remapping configuration */
    //bus_write_4(rswitch2->res[0], etha->addr_offset +  EAIRC, 0x76543210);

    rswitch2_rmac_mdc_clock(rswitch2, 1);

done:
    return rc;
}

static int rswitch2_etha_start(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    int             rc = EOK;

    if (etha->mode == EAMC_OPC_OPERATION) {
        return rc;
    }

    rc = rswitch2_etha_mode_change(rswitch2, EAMC_OPC_OPERATION);
    if (rc) {
        return rc;
    }

    /* Check link */
    rc = rswitch2_check_link(rswitch2, etha);

    return rc;
}

static void rswitch2_getpaddr(void *arg, bus_dma_segment_t *segs, int nsegs, int error)
{
    if (error) {
        *((bus_addr_t *)arg) = 0;
        printf("%s: bus_dmamap_load failed (%d)\n", __func__, error);
        return;
    }
    *((bus_addr_t *)arg) = segs[0].ds_addr;
}

static int rswitch2_desc_bat_init(rswitch2_dev_t *rswitch2)
{
    size_t          dbat_size;
    int             idx, i; 
    int             queue_idx = 0;
    RSWITCH2_DESC   *desc_bat;
    int             error;

    if ((queue_idx = rswitch2_rsrcdb_attach_resources(rswitch2, RSRCDB_TYPE_QUEUE, DBAT_ENTRY_NUM)) == -1) {
        device_printf(rswitch2->dev, "%s: rswitch2_rsrcdb_attach_resources for queue failed, errno %d\n", __func__, errno);
        return errno;
    }

    dbat_size = sizeof(RSWITCH2_DESC) * DBAT_ENTRY_NUM;

    if (!rswitch2->gwca.used && !rswitch2->instance) {
        error = bus_dma_tag_create(
            bus_get_dma_tag(rswitch2->dev),     /* Parent tag */
            1, 0,                               /* alignment, boundary limit */
            BUS_SPACE_MAXADDR,                  /* lowaddr */
            BUS_SPACE_MAXADDR,                  /* highaddr */
            NULL, NULL,                         /* filter, filterarg */
            dbat_size,                          /* maxsize */
            RSWITCH2_TSN_PORT_NUM,              /* nsegments */
            dbat_size,                          /* maxsegsize */
            BUS_DMA_COHERENT,                   /* flags */
            NULL, NULL,                         /* lockfunc, lockarg */
            &desc_bat_tag);
        if (error != 0) {
            device_printf(rswitch2->dev, "%s could not create desctiption tag.\n", __func__);
            goto out;
        }

        error = bus_dmamem_alloc(desc_bat_tag, (void**)&desc_bat_base,
            BUS_DMA_COHERENT | BUS_DMA_NOWAIT | BUS_DMA_NOCACHE | BUS_DMA_ZERO,
            &desc_bat_map);
        if (error != 0) {
            device_printf(rswitch2->dev, "%s: could not allocate descriptor ring.\n", __func__);
            goto out;
        }

        error = bus_dmamap_load(desc_bat_tag, desc_bat_map,
            desc_bat_base, dbat_size, rswitch2_getpaddr, &desc_bat_paddr, BUS_DMA_NOWAIT);
        if (error || !desc_bat_paddr) {
            device_printf(rswitch2->dev, "%s: could not load descriptor ring map.\n", __func__);
            bus_dma_tag_destroy(desc_bat_tag);
            bus_dmamem_free(desc_bat_tag, desc_bat_base, desc_bat_map);
            goto out;
        }

        /* Initialize all descriptors */
        for (idx = 0; idx < DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_NUM; idx++) {
            desc_bat = &desc_bat_base[idx];
            desc_bat->die_dt = DT_EOS;
        }
    } else if (rswitch2->gwca.used && !rswitch2->instance) {
        rswitch2_gwca_t *gwca = &rswitch2->gwca;
        desc_bat_paddr = (bus_addr_t)(bus_read_4(rswitch2->res[0], gwca->addr_offset + GWDCBAC0) & GWDCBAC0_DCBAUP) << 32;
        desc_bat_paddr|= (bus_addr_t)(bus_read_4(rswitch2->res[0], gwca->addr_offset + GWDCBAC1));

        desc_bat_base = (RSWITCH2_DESC *)mmap (NULL, dbat_size * RSWITCH2_TSN_PORT_NUM, PROT_READ | PROT_WRITE | PROT_NOCACHE, 
                                MAP_SHARED | MAP_PHYS, NOFD, desc_bat_paddr);
    }

    rswitch2->tx_queue_index = queue_idx + TX_QUEUE_OFFSET;
    rswitch2->rx_queue_index = queue_idx + RX_QUEUE_OFFSET;

    /* Register the descriptor base address table */
    for(i = 0; i < NUM_RX_QUEUES; i++) {
        /* RX descriptor base address for all queues */
        idx = rswitch2->rx_queue_index + i;
        desc_bat = &desc_bat_base[idx];
        desc_bat->die_dt = DT_LINKFIX; /* type */
        desc_bat->dptrh = rswitch2->rx_ring[i][RX_DESC_COUNT].dptrh;
        desc_bat->dptrl = rswitch2->rx_ring[i][RX_DESC_COUNT].dptrl;
        desc_bat->info_ds = PKT_BUF_SIZE;
        rswitch2->rx_irq_mask[idx/32] |= BIT(idx%32);
    }

    for(i = 0; i < NUM_TX_QUEUES; i++) {
        /* TX descriptor base address for all queues */
        idx = rswitch2->tx_queue_index + i;
        desc_bat = &desc_bat_base[idx];
        desc_bat->die_dt = DT_LINKFIX; /* type */
        desc_bat->dptrh = rswitch2->tx_ring[i][TX_DESC_COUNT].dptrh;
        desc_bat->dptrl = rswitch2->tx_ring[i][TX_DESC_COUNT].dptrl;
        desc_bat->info_ds = PKT_BUF_SIZE;
        rswitch2->tx_irq_mask[idx/32] |= BIT(idx%32);
    }

out:
    if (error != 0)
        return (ENXIO);

    return (0);
}

static int rswitch2_tx_desc_init(rswitch2_dev_t *rswitch2)
{
    int error = 0;
    int idx = 0;
    int queue = 0;
    RSWITCH2_TX_DESC *tx_desc = NULL;

    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        /*
         * Set up Tx descriptor ring, descriptors, and DMA maps
         */
        error = bus_dma_tag_create(
            bus_get_dma_tag(rswitch2->dev), /* Parent tag. */
            1, 0,                       /* alignment, boundary limit */
            BUS_SPACE_MAXADDR,          /* lowaddr */
            BUS_SPACE_MAXADDR,          /* highaddr */
            NULL, NULL,                 /* filter, filterarg */
            TX_DESC_SIZE,               /* maxsize */
            1,                          /* nsegments */
            TX_DESC_SIZE,               /* maxsegsize */
            BUS_DMA_COHERENT,           /* flags */
            NULL, NULL,                 /* lockfunc, lockarg */
            &rswitch2->tx_desc_tag[queue]);
        if (error != 0) {
            device_printf(rswitch2->dev,"could not create TX ring DMA tag.\n");
            goto out;
        }

        error = bus_dmamem_alloc(rswitch2->tx_desc_tag[queue], (void**)&rswitch2->tx_ring[queue],
            BUS_DMA_COHERENT | BUS_DMA_NOWAIT | BUS_DMA_NOCACHE | BUS_DMA_ZERO,
            &rswitch2->tx_desc_map[queue]);
        if (error != 0) {
            device_printf(rswitch2->dev,"could not allocate TX descriptor ring.\n");
            goto out;
        }

        error = bus_dmamap_load(rswitch2->tx_desc_tag[queue], rswitch2->tx_desc_map[queue],
            rswitch2->tx_ring[queue], TX_DESC_SIZE, rswitch2_getpaddr,
            &rswitch2->tx_ring_paddr[queue], BUS_DMA_NOCACHE);
        if (error != 0) {
            device_printf(rswitch2->dev,"could not load TX descriptor ring map.\n");
            goto out;
        }

        for (idx = 0; idx < TX_DESC_COUNT; idx++) {
            tx_desc = &rswitch2->tx_ring[queue][idx];
            tx_desc->die_dt = DT_FEMPTY | DIE;
        }
        tx_desc = &rswitch2->tx_ring[queue][idx];
        tx_desc->dptrh = (uint32_t)(rswitch2->tx_ring_paddr[queue] >> 32);
        tx_desc->dptrl = (uint32_t)rswitch2->tx_ring_paddr[queue];
        tx_desc->die_dt = DT_LINKFIX;   /* type */

        error = bus_dma_tag_create(
            rswitch2->tx_desc_tag[queue],
            1, 0,                       /* alignment, boundary limit */
            BUS_SPACE_MAXADDR,          /* lowaddr */
            BUS_SPACE_MAXADDR,          /* highaddr */
            NULL, NULL,                 /* filter, filterarg */
            MCLBYTES,                   /* maxsize */
            1,                          /* nsegments */
            MCLBYTES,                   /* maxsegsize */
            BUS_DMA_COHERENT,           /* flags */
            NULL, NULL,                 /* lockfunc, lockarg */
            &rswitch2->tx_buf_tag[queue]);
        if (error != 0) {
            device_printf(rswitch2->dev,"could not create TX ring DMA tag.\n");
            goto out;
        }

        for (idx = 0; idx < TX_DESC_COUNT; idx++) {
            error = bus_dmamap_create(rswitch2->tx_buf_tag[queue], BUS_DMA_COHERENT | BUS_DMA_NOWAIT,
                &rswitch2->tx_buf_map[queue][idx].map);
            if (error != 0) {
                device_printf(rswitch2->dev,"could not create TX buffer DMA map.\n");
                goto out;
            }
        }
    }

out:
    if (error != 0)
        return (ENXIO);

    return (0);    
}

int rswitch2_setup_rxbuf(rswitch2_dev_t *rswitch2, int queue, int idx)
{
    struct bus_dma_segment segs[1];
    int error = 0, nsegs;
    struct mbuf *m;

    m = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
    if (m == NULL) {
        device_printf(rswitch2->dev,"Could not alloc Rx mbuf\n");
        if_inc_counter(rswitch2->ifp, IFCOUNTER_IQDROPS, 1);
        error = ENOMEM;
        return (error);
    }
    
    m->m_pkthdr.len =  m->m_len = PKT_BUF_SIZE;

    error = bus_dmamap_load_mbuf_sg(rswitch2->rx_buf_tag[queue], 
        rswitch2->rx_buf_map[queue][idx].map, m, segs, &nsegs, 0);
    if (error != 0){
        device_printf(rswitch2->dev,"%s: could not load Rx mbuf DMA map\n",__func__);
        return (error);
    }
    rswitch2->rx_buf_map[queue][idx].m = m;
    rswitch2->rx_ring[queue][idx].info_ds = PKT_BUF_SIZE;
    rswitch2->rx_ring[queue][idx].dptrh = (uint32_t)(segs[0].ds_addr >> 32);
    rswitch2->rx_ring[queue][idx].dptrl = (uint32_t)segs[0].ds_addr;
    rswitch2->rx_ring[queue][idx].die_dt = DT_FEMPTY | DIE;

    bus_dmamap_sync(rswitch2->rx_buf_tag[queue], rswitch2->rx_buf_map[queue][idx].map, 
        BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);

    return (error);
}

static int rswitch2_rx_desc_init(rswitch2_dev_t *rswitch2)
{
    int error = 0;
    int idx = 0;
    int queue = 0;

    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        /*
         * Set up RX descriptor ring, descriptors, dma maps, and mbufs.
         */
        error = bus_dma_tag_create(
            bus_get_dma_tag(rswitch2->dev),     /* Parent tag. */
            1, 0,                               /* alignment, boundary limit */
            BUS_SPACE_MAXADDR,                  /* lowaddr */
            BUS_SPACE_MAXADDR,		            /* highaddr */
            NULL, NULL,                         /* filter, filterarg */
            RX_DESC_SIZE,                       /* maxsize */
            1,                                  /* nsegments */
            RX_DESC_SIZE,                       /* maxsegsize */
            BUS_DMA_COHERENT,                   /* flags */
            NULL, NULL,                         /* lockfunc, lockarg */
            &rswitch2->rx_desc_tag[queue]);
        if (error != 0) {
            device_printf(rswitch2->dev,"Could not create RX ring DMA tag.\n");
            goto out;
        }

        error = bus_dmamem_alloc(rswitch2->rx_desc_tag[queue], (void **)&rswitch2->rx_ring[queue],
            BUS_DMA_COHERENT | BUS_DMA_NOWAIT | BUS_DMA_NOCACHE | BUS_DMA_ZERO,
            &rswitch2->rx_desc_map[queue]);
        if (error != 0) {
            device_printf(rswitch2->dev,"Could not allocate RX descriptor ring.\n");
            goto out;
        }

        error = bus_dmamap_load(rswitch2->rx_desc_tag[queue], rswitch2->rx_desc_map[queue],
            rswitch2->rx_ring[queue], RX_DESC_SIZE, rswitch2_getpaddr,
            &rswitch2->rx_ring_paddr[queue], BUS_DMA_NOCACHE);
        if (error != 0) {
            device_printf(rswitch2->dev,"Could not load RX descriptor ring map.\n");
            goto out;
        }

        error = bus_dma_tag_create(
            rswitch2->rx_desc_tag[queue],
            1, 0,                       /* alignment, boundary limit */
            BUS_SPACE_MAXADDR,          /* lowaddr */
            BUS_SPACE_MAXADDR,          /* highaddr */
            NULL, NULL,                 /* filter, filterarg */
            MCLBYTES,                   /* maxsize */
            1,                          /* nsegments */
            MCLBYTES,                   /* maxsegsize */
            BUS_DMA_COHERENT,           /* flags */
            NULL, NULL,                 /* lockfunc, lockarg */
            &rswitch2->rx_buf_tag[queue]);
        if (error != 0) {
            device_printf(rswitch2->dev,"Could not create RX buf DMA tag.\n");
            goto out;
        }

        for (idx = 0; idx < RX_DESC_COUNT; idx++) {
            error = bus_dmamap_create(rswitch2->rx_buf_tag[queue], BUS_DMA_COHERENT | BUS_DMA_NOWAIT | BUS_DMA_NOCACHE,
                        &rswitch2->rx_buf_map[queue][idx].map);
            if (error != 0) {
                device_printf(rswitch2->dev,"Could not create RX buffer DMA map.\n");
                goto out;
            }

            error = rswitch2_setup_rxbuf(rswitch2, queue, idx);
            if (error != 0) {
                device_printf(rswitch2->dev,"Could not create  RX buffer.\n");
                goto out;
            }
        }

        rswitch2->rx_ring[queue][idx].dptrh = rswitch2->rx_ring_paddr[queue] >> 32;
        rswitch2->rx_ring[queue][idx].dptrl = (uint32_t)rswitch2->rx_ring_paddr[queue];
        rswitch2->rx_ring[queue][idx].die_dt = DT_LINKFIX; /* type */
        bus_dmamap_sync(rswitch2->rx_desc_tag[queue], rswitch2->rx_desc_map[queue],
            BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);
    }

out:
    if (error != 0)
        return (ENXIO);

    return (0);
}

static int rswitch2_setup_dma(rswitch2_dev_t *rswitch2)
{
    int error = 0;

    error = rswitch2_tx_desc_init(rswitch2);
    if (error != 0)
        return (ENXIO);

    error = rswitch2_rx_desc_init(rswitch2);
    if (error != 0)
        return (ENXIO);

    error = rswitch2_desc_bat_init(rswitch2);
    if (error != 0)
        return (ENXIO);

    return (0);
}

void rswitch2_stop_locked(rswitch2_dev_t *rswitch2)
{
    struct ifnet *ifp;

    //mtx_assert(&rswitch2->mtx, MA_OWNED);

    ifp = rswitch2->ifp;
    ifp->if_drv_flags &= ~(IFF_DRV_RUNNING);

    /* Hardware-specific code here: stop hardware */
    rswitch2_hw_stop(rswitch2);
}

void rswitch2_setup_rxfilter(rswitch2_dev_t *rswitch2)
{
    /* Hardware-specific code here: enable hardware filtering */
}

static void rswitch2_tick(void *arg)
{
    rswitch2_dev_t *rswitch2;
    struct ifnet *ifp;
    int link_was_up;

    rswitch2 = arg;

    //mtx_assert(&rswitch2->mtx, MA_OWNED);
    ifp = rswitch2->ifp;

    if (!(ifp->if_drv_flags & IFF_DRV_RUNNING))
        return;
    /*
     * Typical Tx watchdog.  If this fires, it indicates that we enqueued
     * packets for output and never got a txdone interrupt for them.
     */
    if (rswitch2->tx_watchdog_count > 0) {
        if (--rswitch2->tx_watchdog_count == 0) {
            taskqueue_enqueue(rswitch2->tq, &rswitch2->tx);
        }
    }
    /*
     * Gather statistics from hardware counters. Depending on the frequency of
     * sample_tick, the developer may consider reducing how often this function
     * executes.
     */
    //rswitch2_harvest_stats(sc);

    /* Check the media status. */
    link_was_up = rswitch2->link_is_up;
    mii_tick(rswitch2->mii_softc);
    if (rswitch2->link_is_up && !link_was_up)
        taskqueue_enqueue(rswitch2->tq, &rswitch2->tx);

    /* Schedule another check one second from now. */
    callout_reset(&rswitch2->mii_callout, hz, rswitch2_tick, rswitch2);
}

void rswitch2_init_locked(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t         *etha =  &rswitch2->etha;
    rswitch2_gwca_t         *gwca = &rswitch2->gwca;
    struct ifnet            *ifp = rswitch2->ifp;
    uint32_t                mac_change;
    int                     rc;

    //mtx_assert(&rswitch2->mtx, MA_OWNED);
    /* check if configuration is required */
    mac_change = memcmp(rswitch2->current_address, IF_LLADDR(ifp), ifp->if_addrlen);
    if (mac_change) {
        memcpy(rswitch2->current_address, IF_LLADDR(ifp), ifp->if_addrlen);
        {
            unsigned char * mac = rswitch2->current_address;
            bus_write_4(rswitch2->res[0], etha->addr_offset + MRMAC1, mac[5] | (mac[4] << 8) | (mac[3] << 16) | (mac[2] << 24));
            bus_write_4(rswitch2->res[0], etha->addr_offset + MRMAC0, mac[1] | (mac[0] << 8));
        }
        if (rswitch2->verbose) {
            device_printf(rswitch2->dev, "rswitch2: MAC address updated");
        }
    }

    if (ifp->if_drv_flags & IFF_DRV_RUNNING)
        return;
    ifp->if_drv_flags |= IFF_DRV_RUNNING;
    
    rswitch2_rmac_mdc_clock(rswitch2, 1);
    rc = rswitch2_etha_start(rswitch2);
    if (rc) {
        device_printf(rswitch2->dev, "rswitch2:rswitch2_etha_start failed");
        return;
    }
    rc = rswitch2_gwca_start(rswitch2);
    if (rc) {
        device_printf(rswitch2->dev, "rswitch2:rswitch2_gwca_start failed");
        return;
    }
    /* Enable RX */
    /* FIXME: GWTRC is set for TX only */
    rswitch2_modify(rswitch2->res[0], gwca->addr_offset + GWTRC((rswitch2->rx_queue_index)/32), 0, 
                    BIT((rswitch2->rx_queue_index)%32));

    /* Hardware-specific code here: enable the DMA Rx/Tx interrupts */
    /* Hardware-specific code here: enable transmitters */

    for (int i = 0; i < NUM_TX_QUEUES; i++) {
        rswitch2->tx_pidx[i] = 0;
        rswitch2->tx_cidx[i] = 0;
    }
    for (int i = 0; i < NUM_RX_QUEUES; i++) {
        rswitch2->rx_idx[i] = 0;
    }
    rswitch2->tx_mtx_is_up = 1;
    if_setdrvflagbits(ifp, IFF_DRV_RUNNING, IFF_DRV_OACTIVE);
    /*
     * Call mii_mediachg(), which will call back into rswitch2_miibus_statchg()
     * to set up the remaining configuration registers based on current media.
     */
    mii_mediachg(rswitch2->mii_softc);

    callout_reset(&rswitch2->mii_callout, hz, rswitch2_tick, rswitch2);

}

static void rswitch2_init(void *if_softc)
{
    rswitch2_dev_t *rswitch2 = if_softc;
    mtx_lock(&rswitch2->mtx);
    rswitch2_init_locked(rswitch2);
    mtx_unlock(&rswitch2->mtx);
}

void rswitch2_hw_stop (rswitch2_dev_t *rswitch2)
{

    callout_stop(&rswitch2->mii_callout);
}

static int rswitch2_shutdown(device_t dev)
{
    rswitch2_dev_t *rswitch2 = device_get_softc(dev);

    /*
    * Hardware-specific code here: shutdown is called when io-sock is
    * terminated or crashes. It is not intended to be a graceful stop
    * (that is what detach is for). Therefore, the code should disable hardware
    * DMA using the simplest method possible.
    */

    if (!rswitch2_rsrcdb_check_parallel_mode(rswitch2)) {
        rswitch2 = device_get_softc(dev);
        rswitch2_reset(rswitch2);
        rswitch2_clock_enable(rswitch2, 0);
    }

    return 0;
}

static int rswitch2_interrupt_attach(rswitch2_dev_t *rswitch2)
{
    if (bus_setup_intr(rswitch2->dev, rswitch2->irq_res[0], INTR_TYPE_NET | INTR_MPSAFE, NULL,
        rswitch2_intrs[0].rswitch2_intr_cb, rswitch2, &rswitch2->intr_cookie[0]) != 0) {
        return (-1);
    }

#if 0
    if (bus_setup_intr(rswitch2->dev, rswitch2->irq_res[0], INTR_TYPE_NET | INTR_MPSAFE, NULL,
        rswitch2_intrs[1].rswitch2_intr_cb, rswitch2, &rswitch2->intr_cookie[1]) != 0) {
        return (-1);
    }
#endif

    return (0);
}

static void rswitch2_cleanup(rswitch2_dev_t *rswitch2)
{
    device_t dev = rswitch2->dev;
    bus_dmamap_t map;
    int idx, queue;

    rswitch2 = device_get_softc(dev);

    /* Disable and Detach all interrupts */
    rswitch2_enadis_data_irq(rswitch2, 0, 0);
    rswitch2_enadis_data_irq(rswitch2, 1, 0);

    if (rswitch2->is_attached) {
        mtx_lock(&rswitch2->mtx);
        rswitch2->is_detaching = true;
        rswitch2_stop_locked(rswitch2);
        mtx_unlock(&rswitch2->mtx);
        callout_drain(&rswitch2->mii_callout);
        ether_ifdetach(rswitch2->ifp);
    }

    /* Detach the miibus */
    if (rswitch2->miibus != NULL) {
        device_delete_child(dev, rswitch2->miibus);
        rswitch2->miibus = NULL;
    }
    bus_generic_detach(dev);

    if (rswitch2->ifp != NULL) {
        if_free(rswitch2->ifp);
    }

    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        /* Clean up Rx DMA resources and free mbufs. */
        for (idx = 0; idx < RX_DESC_COUNT; ++idx) {
            if ((map = rswitch2->rx_buf_map[queue][idx].map) != NULL) {
                bus_dmamap_unload(rswitch2->rx_buf_tag[queue], map);
                bus_dmamap_destroy(rswitch2->rx_buf_tag[queue], map);
                m_freem(rswitch2->rx_buf_map[queue][idx].m);
            }
        }
        if (rswitch2->rx_buf_tag[queue] != NULL) {
            bus_dma_tag_destroy(rswitch2->rx_buf_tag[queue]);
        }
        if (rswitch2->rx_desc_map[queue] != NULL) {
            bus_dmamap_unload(rswitch2->rx_desc_tag[queue], rswitch2->rx_desc_map[queue]);
            bus_dmamem_free(rswitch2->rx_desc_tag[queue], rswitch2->rx_ring[queue], rswitch2->rx_desc_map[queue]);
        }
        if (rswitch2->rx_desc_tag[queue] != NULL) {
            bus_dma_tag_destroy(rswitch2->rx_desc_tag[queue]);
        }
    }

    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        /* Clean up Tx DMA resources. */
        for (idx = 0; idx < TX_DESC_COUNT; ++idx) {
            if ((map = rswitch2->tx_buf_map[queue][idx].map) != NULL) {
                /* TX maps are already unloaded. */
                bus_dmamap_unload(rswitch2->tx_buf_tag[queue], map);
                bus_dmamap_destroy(rswitch2->tx_buf_tag[queue], map);
            }
        }

        if (rswitch2->tx_buf_tag[queue] != NULL) {
            bus_dma_tag_destroy(rswitch2->tx_buf_tag[queue]);
        }
        if (rswitch2->tx_desc_map[queue] != NULL) {
            bus_dmamap_unload(rswitch2->tx_desc_tag[queue], rswitch2->tx_desc_map[queue]);
            bus_dmamem_free(rswitch2->tx_desc_tag[queue], rswitch2->tx_ring[queue],rswitch2->tx_desc_map[queue]);
        }
        if (rswitch2->tx_desc_tag[queue] != NULL) {
            bus_dma_tag_destroy(rswitch2->tx_desc_tag[queue]);
        }
    }

    if (!rswitch2->gwca.used && instance == 0) {
        if (desc_bat_map != NULL) {
            bus_dmamap_unload(desc_bat_tag, desc_bat_map);
            bus_dmamem_free(desc_bat_tag, desc_bat_base, desc_bat_map);
        }
        if (desc_bat_tag != NULL) {
            bus_dma_tag_destroy(desc_bat_tag);
        }

        /* Release bus resources. */
        bus_release_resources(dev, rswitch2_res_spec, base_res);
    }
    else if (rswitch2->gwca.used && instance == 0) {
        munmap(desc_bat_base, sizeof(RSWITCH2_DESC) * DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_NUM);
    }

    if (rswitch2->tx_mtx_is_up) {
        mtx_lock(&rswitch2->tx_mtx);

        if (rswitch2->tq) {
            taskqueue_drain(rswitch2->tq, &rswitch2->tx);
            taskqueue_free(rswitch2->tq);
            rswitch2->tq = NULL;
        }

        if (rswitch2->br) {
            buf_ring_free(rswitch2->br, M_DEVBUF);
            rswitch2->br = NULL;
        }
        mtx_destroy(&rswitch2->tx_mtx);
    }

    for (idx = 0; idx < RSWITCH2_IRQ_NUM; idx++) {
        if (rswitch2->intr_cookie[idx] != NULL) {
            bus_teardown_intr(dev, rswitch2->irq_res[idx], rswitch2->intr_cookie[idx]);
            rswitch2->intr_cookie[idx] = NULL;
        }
    }

    if (instance == 0) {
        bus_release_resources(dev, irq_res_spec, irq_res);
    }
    
    mtx_destroy(&rswitch2->mtx);
    mtx_destroy(&rswitch2->tx_mtx);

    rswitch2_rmac_mdc_clock(rswitch2, 0);
    rswitch2_etha_mode_change(rswitch2, EAMC_OPC_DISABLE);
    rswitch2_clock_agent_etha(rswitch2, 0);

    rswitch2_rsrcdb_detach_resources(rswitch2, RSRCDB_TYPE_QUEUE, rswitch2->rx_queue_index - RX_QUEUE_OFFSET, DBAT_ENTRY_NUM);
    rswitch2_rsrcdb_detach_resources(rswitch2, RSRCDB_TYPE_ETHA, 0, 0);
    rswitch2_rsrcdb_detach_resources(rswitch2, RSRCDB_TYPE_GWCA, 0, 0);
}


static int rswitch2_detach (device_t dev)
{
    rswitch2_dev_t *rswitch2;

    rswitch2 = device_get_softc(dev);

    --instance;

    rswitch2_cleanup (rswitch2);

    return (0);
}

static int rswitch2_attach (device_t dev)
{
    uint8_t             macaddr[ETHER_ADDR_LEN];
    rswitch2_dev_t      *rswitch2;
    struct ifnet        *ifp;
    int                 error;
    char                *node_prop;
    phandle_t           node;
    boolean_t           is_cleanup = false;
    char	            tq_name[32];
    rswitch2_etha_t     *etha;
    rswitch2_gwca_t     *gwca;
    int                 rc;

    rswitch2 = device_get_softc(dev);
    rswitch2->dev = dev;

    etha = &rswitch2->etha;
    gwca = &rswitch2->gwca;

    rswitch2->is_attached = false;
    rswitch2->flags = 0;
    rswitch2->media_rate = 0;
    rswitch2->mii = SERDES_SGMII;

    rswitch2->port0_reset = 0;
    rswitch2->instance = instance;
    instance++;

    if ((node = ofw_bus_get_node(dev)) == -1) {
        device_printf(dev, "Impossible: Can't find ofw bus node\n");
        error = ENXIO;
        goto fail;
    }
    /*
     * Hardware-specific code here: an example of how to read some
     * property from a DTB file
     */

    if (OF_getprop_alloc(node, "gwca-index", (void **)&node_prop)) {
        gwca->index = atoi(node_prop);
        OF_prop_free(node_prop);
    }
    else {
        device_printf(dev, "Unable to get gwca-index prop, use gwca 0\n");
        gwca->index = 0;
    }

    rswitch2_rsrcdb_create_resources(rswitch2);

    gwca->addr_offset = RSWITCH2_GWCA_OFFSET + gwca->index * RSWITCH2_GWCA_SIZE;
    rswitch2->parallel_mode = rswitch2_rsrcdb_check_parallel_mode(rswitch2);
    rswitch2->gwca.used = rswitch2_rsrcdb_check_gwca_used(rswitch2);

    if ((error = rswitch2_rsrcdb_attach_resources(rswitch2, RSRCDB_TYPE_GWCA, 1) == -1)) {
        device_printf(dev, "rswitch2_rsrcdb_attach_resources for GWCA%d failed %s\n", gwca->index, strerror(errno));
        error = ENXIO;
        goto fail;
    }
    rswitch2->gwca.rsrc_idx = error;

    if (OF_getprop_alloc(node, "etha-index", (void **)&node_prop)) {
        etha->index = atoi(node_prop);
        OF_prop_free(node_prop);
    }
    else {
        device_printf(dev, "Unable to get etha-index prop, use etha 0\n");
        etha->index = 0;
    }

    etha->addr_offset = RSWITCH2_ETHA_OFFSET + etha->index * RSWITCH2_ETHA_SIZE;
    etha->phy_speed = 1000*1000;
    rswitch2->port = etha->index;

    if ((error = rswitch2_rsrcdb_attach_resources(rswitch2, RSRCDB_TYPE_ETHA, 1) == -1)) {
        device_printf(dev, "rswitch2_rsrcdb_attach_resources for ETHA%d failed\n", etha->index);
        error = ENXIO;
        goto fail;
    }

    if (OF_getprop_alloc(node, "phy-mode", (void **)&node_prop)) {
        if (strcmp(node_prop, "rgmii") == 0) {
            rswitch2->phy_mode = PHY_MODE_RGMII;
        }
        else if (strcmp(node_prop, "rmii") == 0) {
            rswitch2->phy_mode = PHY_MODE_RMII;
        }
        else if (strcmp(node_prop, "mii") == 0) {
            rswitch2->phy_mode = PHY_MODE_MII;
        }
        else if (strcmp(node_prop, "gmii") == 0) {
            rswitch2->phy_mode = PHY_MODE_GMII;
        }
        OF_prop_free(node_prop);
    }

    error = OF_getprop(node, "mac-address", macaddr, ETHER_ADDR_LEN);
    if (error != ETHER_ADDR_LEN) {
        device_printf(dev, "OF_getprop getting MAC address failed\n");
        error = ENOMEM;
        goto fail;
    }

    if (!rswitch2->instance) {
        /* Request the IRQ resources */
        if (bus_alloc_resources(dev, irq_res_spec, irq_res)) {
            device_printf(dev, "Could not allocate irq resources\n");
            error = ENXIO;
            goto fail;
        }

        /* Request the memory resources */
        if (bus_alloc_resources(dev, rswitch2_res_spec, base_res)) {
            device_printf(dev, "Could not allocate memory resources\n");
            bus_release_resources(dev, irq_res_spec, irq_res);
            error = ENXIO;
            goto fail;
        }
    }

    rswitch2->irq_res[0] = irq_res[0];
    rswitch2->res[0] = base_res[0];
    etha->serdes_res = base_res[1];

    rswitch2_clear_stats(rswitch2);

    /* Set RSWITCH2 reset mode */
    rswitch2_reset(rswitch2);

    /* Set up descriptors, DMA maps and mbufs */
    error = rswitch2_setup_dma(rswitch2);
    if (error) {
        bus_release_resources(dev, rswitch2_res_spec, base_res);
        bus_release_resources(dev, irq_res_spec, irq_res);
        device_printf(dev, "rswitch2_setup_dma failed\n");
        error = ENXIO;
        goto fail;
    }

    is_cleanup = true;

    /* Enable clock */
    rswitch2_clock_enable(rswitch2, 1);

    rc = rswitch2_bpool_init(rswitch2);
    if (rc != EOK) {
        device_printf(dev, "rswitch2: Init bpool failed: %d\n", rc);
        error = ENXIO;
        goto fail;
    }

    /* Initialize Fordwarding between GWCA and ETHA(TSN) */
    rswitch2_mfwd_init(rswitch2);

    /* Initialize GWCA */
    rc = rswitch2_gwca_init(rswitch2);
    if (rc != EOK) {
        device_printf(dev, "rswitch2: Init gwca%d failed: %d\n", gwca->index, rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    /* Initialize ETHA(TSN) */
    rc = rswitch2_etha_init(rswitch2);
    if (rc != EOK) {
        device_printf(dev, "rswitch2: Init etha%d failed: %d\n", etha->index, rc);
        error = ENXIO;
        goto fail;
    }

    /* Initialize PTP */
    rswitch2_ptp_init (rswitch2);

    callout_init(&rswitch2->mii_callout, 0);

    bzero(tq_name, sizeof (tq_name));
    snprintf(tq_name, sizeof (tq_name), "%s_tx_%d",device_get_nameunit(rswitch2->dev), rswitch2->port);
    
    /* Initialize mutex */
    mtx_init(&rswitch2->tx_mtx, tq_name, NULL, MTX_DEF);
    mtx_init(&rswitch2->mtx, device_get_nameunit(rswitch2->dev), MTX_NETWORK_LOCK, MTX_DEF);
    callout_init_mtx(&rswitch2->mii_callout, &rswitch2->mtx, 0);

    rswitch2->br = buf_ring_alloc(TX_QUEUE_LEN, M_DEVBUF, M_NOWAIT, &rswitch2->tx_mtx);
    if (rswitch2->br == NULL) {
        device_printf(dev, "buf_ring_alloc failed\n");
        error = ENOMEM;
        goto fail;
    }

    TASK_INIT(&rswitch2->tx, 0, rswitch2_tq_tx, rswitch2);

    bzero(tq_name, sizeof (tq_name));
    snprintf(tq_name, sizeof (tq_name), "%s%d",device_get_nameunit(rswitch2->dev), rswitch2->port);//device_get_unit(dev));
    rswitch2->tq = taskqueue_create(tq_name, M_NOWAIT, taskqueue_thread_enqueue, &rswitch2->tq);
    if (rswitch2->tq == NULL) {
        device_printf(dev, "taskqueue_create failed\n");
        error = ENOMEM;
        goto fail;
    }
    taskqueue_start_threads(&rswitch2->tq, 1, PI_NET, "%s", tq_name);

    /* Set up interrupt handler. */
    if (rswitch2_interrupt_attach(rswitch2) == -1) {
        device_printf(dev, "could not setup interrupt handler.\n");
        error = ENXIO;
        goto fail;
    }
    /* Set up the Ethernet interface. */
    rswitch2->ifp = ifp = if_alloc(IFT_ETHER);
    ifp->if_softc = rswitch2;
    if_initname(ifp, device_get_name(dev), rswitch2->port);
    ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST | IFF_ALLMULTI;
    ifp->if_capabilities |= IFCAP_VLAN_MTU | IFCAP_HWCSUM;
    ifp->if_capenable |= IFCAP_VLAN_MTU;
    ifp->if_transmit = rswitch2_transmit;
    ifp->if_qflush = rswitch2_qflush;
    ifp->if_ioctl = rswitch2_ioctl;
    ifp->if_init = rswitch2_init;
    IFQ_SET_MAXLEN(&ifp->if_snd, TX_DESC_COUNT - 1);
    ifp->if_snd.ifq_drv_maxlen = TX_DESC_COUNT - 1 ;
    IFQ_SET_READY(&ifp->if_snd);
    ifp->if_hdrlen = sizeof(struct ether_vlan_header);
    ifp->if_mtu = 2048 - (ETHER_HDR_LEN + ETHER_VLAN_ENCAP_LEN + ETHER_CRC_LEN);

    /* Initialize PHY M88E2110 */
    rswitch2_phy_init(rswitch2);

    rc = rswitch2_serdes_init(rswitch2);
    if (rc != EOK) {
        device_printf(dev, "rswitch2: Init serder%d failed: %d\n", etha->index, rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    /* Attach the mii driver. */
    error = mii_attach(dev, &rswitch2->miibus, ifp, rswitch2_media_change,
        rswitch2_media_status, BMSR_DEFCAPMASK, rswitch2->cfg.phy_addr,
        MII_OFFSET_ANY, MIIF_FORCEANEG);
    if (error != 0) {
        device_printf(dev, "PHY attach failed\n");
        error = ENXIO;
        is_cleanup = true;
        goto fail;
    }

    rswitch2->mii_softc = device_get_softc(rswitch2->miibus);

    /* All ready to run, attach the Ethernet interface. */
    ether_ifattach(ifp, macaddr);

    /* Initialize interrupt for TX/RX */
    rswitch2_enadis_data_irq(rswitch2, 0, 1);
    rswitch2_enadis_data_irq(rswitch2, 1, 1);

    device_printf(dev, "rswitch2: attached done.\n");
    rswitch2->is_attached = true;
    return 0;

fail:
    if (is_cleanup) {
        rswitch2_cleanup(rswitch2);
    }
    if (error != 0) {
        device_printf(dev, "%s failed with error %d\n",__func__, error);
        rswitch2_detach(dev);
    }
    return error;
}

static int rswitch2_probe(device_t dev)
{
    if (!ofw_bus_status_okay(dev)) {
        return (ENXIO);
    }
    if (!ofw_bus_is_compatible(dev, "renesas,etherswitch-r8a779f0")) {
        return (ENXIO);
    }

    device_set_desc(dev, "Renesas RSWITCH Network Device");

    return (BUS_PROBE_DEFAULT);
}
