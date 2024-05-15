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
#include <stdlib.h>
#include <fcntl.h>
#include <drvr/hwinfo.h>
#include <net/ifdrvcom.h>
#include <sys/sockio.h>
#include <netdrvr/smmu.h>
#include <net/if_vlanvar.h>
#include <netdrvr/avb.h>
#include <netinet/ip.h>
#include <bpfilter.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>

static int  single = 0; 
static int  instance = 0;
static int  tsn_port_num = 1;
static uintptr_t  rswitch2_base = (uintptr_t)MAP_FAILED;
static uintptr_t  bat_desc_base = (uintptr_t)MAP_FAILED;
static off64_t    bat_desc_paddr = 0;

int rswitch2_etha_mode_change(rswitch2_dev_t *rswitch2, int mode);
int rswitch2_gwca_mode_change(rswitch2_dev_t *rswitch2, int mode);

int rswitch2_rsrcdb_create_resources();
int rswitch2_rsrcdb_attach_resources(rswitch2_dev_t *rswitch2, const rsrcdb_type_t type, const int length);
int rswitch2_rsrcdb_detach_resources(rswitch2_dev_t *rswitch2, const rsrcdb_type_t type, const int start, const int length);
int rswitch2_rsrcdb_query_gwca_resources(rswitch2_dev_t *rswitch2, int for_reset);
int rswitch2_rsrcdb_check_parallel_mode(rswitch2_dev_t *rswitch2);
int rswitch2_rsrcdb_check_gwca_used(rswitch2_dev_t *rswitch2);

static void rswitch2_serdes_write(uintptr_t addr, uint32_t reg, uint32_t bank, uint32_t data)
{
    out32(addr + RSWITCH2_SERDES_BANK_OFFSET, bank);
    out32(addr + reg, data);
}

static uint32_t rswitch2_serdes_read(uintptr_t addr, uint32_t reg, uint32_t bank)
{
    out32(addr + RSWITCH2_SERDES_BANK_OFFSET, bank);
    return in32(addr + reg);
}

static int rswitch2_serdes_reg_wait(uintptr_t addr, uint32_t reg, uint32_t bank, uint32_t mask, uint32_t val)
{
    int i = 0;

    out32(addr + RSWITCH2_SERDES_BANK_OFFSET, bank);
    delay(1);
    while (i < RSWITCH2_TIMEOUT_MS) {
        if ((in32(addr + reg) & mask) == val)
            return EOK;
        delay(1);
        i++;
    }

    return -ETIMEDOUT;
}

static int rswitch2_serdes_common_init_ram(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t     *etha = &rswitch2->etha;
    rswitch2_serdes_t   *serdes = &rswitch2->serdes;
    uintptr_t            addr = serdes->addr - etha->index * RSWITCH2_SERDES_OFFSET;
    int                  rc;

    rc = rswitch2_serdes_reg_wait(serdes->addr, 0x026C, 0x180, BIT(0), 0x01);
    if (rc)
        return rc;

    rswitch2_serdes_write(addr, 0x026C, 0x180, 0x03);

    rc = rswitch2_serdes_reg_wait(addr, 0x0000, 0x300, BIT(15), 0);

    return rc;
}

static int rswitch2_serdes_common_setting(rswitch2_dev_t *rswitch2, int mode)
{
    rswitch2_etha_t   *etha = &rswitch2->etha;
    rswitch2_serdes_t *serdes = &rswitch2->serdes;
    uintptr_t         addr = serdes->addr - etha->index * RSWITCH2_SERDES_OFFSET;

    if (mode == SERDES_SGMII) {
        rswitch2_serdes_write(addr, 0x0244, 0x180, 0x0097);
        rswitch2_serdes_write(addr, 0x01D0, 0x180, 0x0060);
        rswitch2_serdes_write(addr, 0x01D8, 0x180, 0x2200);
        rswitch2_serdes_write(addr, 0x01D4, 0x180, 0x0000);
        rswitch2_serdes_write(addr, 0x01E0, 0x180, 0x003d);
    } else if (mode == SERDES_USXGMII) {
        rswitch2_serdes_write(addr, 0x0244, 0x180, 0x0057);
        rswitch2_serdes_write(addr, 0x01CC, 0x180, 0xC200);
        rswitch2_serdes_write(addr, 0x01C4, 0x180, 0x0042);
        rswitch2_serdes_write(addr, 0x01C8, 0x180, 0x0000);
        rswitch2_serdes_write(addr, 0x01DC, 0x180, 0x002F);
    } else {
        return -EINVAL;
    }

    return EOK;
}

static int rswitch2_serdes_channel_settings(rswitch2_dev_t *rswitch2, int mode)
{
    int         rc;
    rswitch2_serdes_t   *serdes = &rswitch2->serdes;

    if (mode == SERDES_SGMII) {
        //rswitch2_serdes_write(serdes->addr, 0x001C, 0x300, 0x0001);
        rswitch2_serdes_write(serdes->addr, 0x0000, 0x380, 0x2000);

        rswitch2_serdes_write(serdes->addr, 0x01C0, 0x180, 0x0011);
        rswitch2_serdes_write(serdes->addr, 0x0248, 0x180, 0x0540);
        rswitch2_serdes_write(serdes->addr, 0x0258, 0x180, 0x0015);
        rswitch2_serdes_write(serdes->addr, 0x0144, 0x180, 0x0100);
        rswitch2_serdes_write(serdes->addr, 0x01A0, 0x180, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x00D0, 0x180, 0x0002);
        rswitch2_serdes_write(serdes->addr, 0x0150, 0x180, 0x0003);
        rswitch2_serdes_write(serdes->addr, 0x00C8, 0x180, 0x0100);
        rswitch2_serdes_write(serdes->addr, 0x0148, 0x180, 0x0100);
        rswitch2_serdes_write(serdes->addr, 0x0174, 0x180, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x0160, 0x180, 0x0007);
        rswitch2_serdes_write(serdes->addr, 0x01AC, 0x180, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x00C4, 0x180, 0x0310);
        if (rswitch2->port != 0) {
            rswitch2_serdes_write(serdes->addr, 0x00C8, 0x180, 0x0101);
            rc = rswitch2_serdes_reg_wait(serdes->addr, 0x00C8, 0x180, BIT(0), 0);
            if (rc) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes check bit0 of register C8 failed");
                return rc;
            }

            rswitch2_serdes_write(serdes->addr, 0x0148, 0x180, 0x101);
            rc = rswitch2_serdes_reg_wait(serdes->addr, 0x0148, 0x180, BIT(0), 0);
            if (rc) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes bit0 of register 148 failed");
                return rc;
            }
        } else {
            rswitch2_serdes_write(serdes->addr, 0x0000, 0x380, 0xa000);
            rc = rswitch2_serdes_reg_wait(serdes->addr, 0x0000, 0x380, BIT(15), 0);
            if (rc) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes check bit15 of register 0 failed");
                return rc;
            }
        }
        
        rswitch2_serdes_write(serdes->addr, 0x00C4, 0x180, 0x1310);
        rswitch2_serdes_write(serdes->addr, 0x00D8, 0x180, 0x1800);
        rswitch2_serdes_write(serdes->addr, 0x00DC, 0x180, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x001C, 0x300, 0x0001);
        rswitch2_serdes_write(serdes->addr, 0x0000, 0x380, 0x2100);
        rc = rswitch2_serdes_reg_wait(serdes->addr, 0x0000, 0x380, BIT(8), 0x0000);
        if (rc) {
            return rc;
        }

        uint32_t val = rswitch2_serdes_read(serdes->addr, 0x0004, 0x1F80);
        rswitch2_serdes_write(serdes->addr, 0x0004, 0x1F80, val | 0x100);

    } else if (mode == SERDES_USXGMII) {
        rswitch2_serdes_write(serdes->addr, 0x001C, 0x380, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x0000, 0x380, 0x2200);
        rswitch2_serdes_write(serdes->addr, 0x001C, 0x380, 0x0800);

        rswitch2_serdes_write(serdes->addr, 0x001C, 0x180, 0x0800);
        rswitch2_serdes_write(serdes->addr, 0x01C0, 0x180, 0x0001);
        rswitch2_serdes_write(serdes->addr, 0x0248, 0x180, 0x056A);
        rswitch2_serdes_write(serdes->addr, 0x0258, 0x180, 0x0015);
        rswitch2_serdes_write(serdes->addr, 0x0144, 0x180, 0x1100);
        rswitch2_serdes_write(serdes->addr, 0x01A0, 0x180, 0x0001);
        rswitch2_serdes_write(serdes->addr, 0x00D0, 0x180, 0x0002);
        rswitch2_serdes_write(serdes->addr, 0x0150, 0x180, 0x0002);
        rswitch2_serdes_write(serdes->addr, 0x00C8, 0x180, 0x0300);
        rswitch2_serdes_write(serdes->addr, 0x0148, 0x180, 0x0300);
        rswitch2_serdes_write(serdes->addr, 0x0174, 0x180, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x0160, 0x180, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x01AC, 0x180, 0x0000);
        rswitch2_serdes_write(serdes->addr, 0x00C4, 0x180, 0x0310);
        rswitch2_serdes_write(serdes->addr, 0x00C8, 0x180, 0x0301);
        rc = rswitch2_serdes_reg_wait(serdes->addr, 0x00C8, 0x180, BIT(0), 0);
        if (rc) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes check bit0 of register C8 failed");
            return rc;
        }
        rswitch2_serdes_write(serdes->addr, 0x0148, 0x180, 0x0301);
        rc = rswitch2_serdes_reg_wait(serdes->addr, 0x0148, 0x180, BIT(0), 0);
        if (rc) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes check bit0 of register 0x148 failed");
            return rc;
        }
        rswitch2_serdes_write(serdes->addr, 0x00C4, 0x180, 1310);
        rswitch2_serdes_write(serdes->addr, 0x00D8, 0x180, 1800);
        rswitch2_serdes_write(serdes->addr, 0x00DC, 0x180, 0000);
        rswitch2_serdes_write(serdes->addr, 0x0000, 0x380, 2300);
        rc = rswitch2_serdes_reg_wait(serdes->addr, 0x0000, 0x380, BIT(0), 0);
        if (rc) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes check bit0 of register 0 failed");
            return rc;
        }
    } else {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Unknown SerDes mode (Only support SGMII or USXGMII)");
        return -ENOTSUP;
    }
    
    return EOK;
}

static int rswitch2_serdes_init(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t     *etha = &rswitch2->etha;
    rswitch2_serdes_t   *serdes = &rswitch2->serdes;
    uintptr_t           addr;
    int                 rc;

    serdes->addr = mmap_device_io(RSWITCH2_SERDES_SIZE, RSWITCH2_SERDES_BASE + serdes->index * RSWITCH2_SERDES_OFFSET);
    if (serdes->addr == (uintptr_t)MAP_FAILED) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: mmap Serdes device failed.");
        return rc;
    }

    serdes->bank_off = RSWITCH2_SERDES_BANK_OFFSET;
    addr = serdes->addr - etha->index * RSWITCH2_SERDES_OFFSET;

    /* Initialize SRAM */
    rc = rswitch2_serdes_common_init_ram(rswitch2);
    if (rc) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Initialize SerDes SRAM failed.");
        return rc;
    }

    rc = rswitch2_serdes_reg_wait(serdes->addr, 0x0000, 0x300, BIT(15), 0);
    if (rc) {
        return rc;
    }

    rswitch2_serdes_write(serdes->addr, 0x03D4, 0x380, 0x0443);

    /* Set common setting */
    rc = rswitch2_serdes_common_setting(rswitch2, rswitch2->mii);
    if (rc) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Set SerDes common setting failed.");
        return rc;
    }

    rswitch2_serdes_write(serdes->addr, 0x03D0, 0x380, 0x0001);

    /* Assert softreset for PHY */
    rswitch2_serdes_write(addr, 0x0000, 0x0380, 0x8000);

    /* Initialize SRAM */
    rc = rswitch2_serdes_common_init_ram(rswitch2);
    if (rc) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Re-initialize SerDes SRAM failed.");
        return rc;
    }

    /* Set channel settings */
    rc = rswitch2_serdes_channel_settings(rswitch2, rswitch2->mii);
    if (rc) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Set SerDes channel settings failed.");
        return rc;
    }

    /* Select speed (bps) */
    rc = rswitch2_set_speed(rswitch2, rswitch2->mii, (&rswitch2->etha)->phy_speed);
    if (rc) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Select Serdes speed (bps)  failed.");
        return rc;
    }

    rswitch2_serdes_write(serdes->addr, 0x03C0, 0x380, 0x0000);
    rswitch2_serdes_write(serdes->addr, 0x03D0, 0x380, 0x0000);

    rc = rswitch2_serdes_reg_wait(rswitch2->serdes.addr, 0x0004, 0x300, BIT(2), BIT(2));
    if (rc) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes Link up failed");
        return rc;
    }

    return EOK;
}

int rswitch2_set_speed(rswitch2_dev_t *rswitch2, int mode, uint32_t speed)
{
    rswitch2_etha_t     *etha = &rswitch2->etha;
    rswitch2_serdes_t   *serdes = &rswitch2->serdes;
    int                 rc = EOK;
    uint32_t            val = 0;
    int                 etha_mode = etha->mode;

    if (mode == SERDES_SGMII) {
        switch (speed) {
        case 1000*1000:
            rswitch2_serdes_write(serdes->addr, 0x0000, 0x1F00, 0x0140);
            val |= MPIC_LSC_1000;
            break;
        case 100*1000:
            rswitch2_serdes_write(serdes->addr, 0x0000, 0x1F00, 0x2100);
             val |= MPIC_LSC_100;
            break;
        case 10*1000:
            rswitch2_serdes_write(serdes->addr, 0x0000, 0x1F00, 0x0100);
            break;
        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Don't support this speed(%d mbps)", speed/1000);
            return -ENOTSUP;
        }
    } else if (mode == SERDES_USXGMII) {
        switch (speed) {
        case 2500000:
            rswitch2_serdes_write(serdes->addr, 0x0000, 0x1F00, 0x0120);
            val |= MPIC_LSC_2500;
            break;
        case 100000:
            rswitch2_serdes_write(serdes->addr, 0x0000, 0x1F00, 0x0140);
            val |= MPIC_LSC_1000;
            break;
        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Don't support this speed(%d mbps) in USXGMII mode", speed/1000);
            return -ENOTSUP;
        }
        delay(1);
        rswitch2_serdes_write(serdes->addr, 0x0000, 0x380, 0x0000);
        rc = rswitch2_serdes_reg_wait(serdes->addr, 0x0148, 0x380, BIT(10), 0);
        if (rc) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: SerDes sets speed to %dMHz failed", speed/1000);
            return rc;
        }
    } else {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Unknown SerDes mode (Only support SGMII or USXGMII mode)");
        return -ENOTSUP;
    }

    pthread_mutex_lock(&etha->etha_mutex);
    if (etha->mode != EAMC_OPC_CONFIG) {
        rc = rswitch2_etha_mode_change(rswitch2, EAMC_OPC_CONFIG);
        if (rc) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: %s cannot change mode to EAMC_OPC_CONFIG", __func__);
            pthread_mutex_unlock(&etha->etha_mutex);
            return rc;
        }
    }

    val = (in32(etha->addr + MPIC) & ~MPIC_LSC_MASK) | val;
    /* Set RMAC speed */
    out32(etha->addr + MPIC, val);
    etha->phy_speed = speed;

    if (etha_mode != EAMC_OPC_CONFIG) {
        rc = rswitch2_etha_mode_change(rswitch2, etha_mode);
        if (rc) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: %s cannot recover mode to EAMC_OPC_OPERATION", __func__);
            pthread_mutex_unlock(&etha->etha_mutex);
            return rc;
        }
    }
    pthread_mutex_unlock(&etha->etha_mutex);

    return EOK;
}

int rswitch2_etha_mode_change(rswitch2_dev_t *rswitch2, int mode)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    int             rc;

    /* Change to Disable mode first */
    out32(etha->addr + EAMC, EAMC_OPC_DISABLE);
    rc = rswitch2_reg_wait(etha->addr, EAMS, EAMS_OPS_MASK, EAMC_OPC_DISABLE);
    if (rc) return rc;

    etha->mode = EAMC_OPC_DISABLE;

    /* Then change to expected mode */
    if (mode != EAMC_OPC_DISABLE) {
        out32(etha->addr + EAMC, mode);
        rc = rswitch2_reg_wait(etha->addr, EAMS, EAMS_OPS_MASK, mode);
        if (rc) return rc;
    }

    etha->mode = mode;

    return rc;
}

int rswitch2_gwca_mode_change(rswitch2_dev_t *rswitch2, int mode)
{
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             rc;

    /* Change to Disable mode first */
    out32(gwca->addr + GWMC, GWMC_OPC_DISABLE);
    rc = rswitch2_reg_wait(gwca->addr, GWMS, GWMS_OPS_MASK, GWMC_OPC_DISABLE);
    if (rc)
        return rc;

    /* Change expected mode */
    if (mode != GWMC_OPC_DISABLE) {
        out32(gwca->addr + GWMC, mode);
        rc = rswitch2_reg_wait(gwca->addr, GWMS, GWMS_OPS_MASK, mode);
    }

    gwca->mode = mode;

    return rc;
}

static int rswitch2_check_link(rswitch2_etha_t *etha)
{
    int rc;
    int timeout = 1000;

    /* Request Link Verification */
    out32(etha->addr + MLVC, MLVC_PLV);

    for(; timeout; --timeout) {
        /* Complete Link Verification */
        if ((rc = rswitch2_reg_wait(etha->addr, MLVC, MLVC_PLV, 0)) == 0) {
            break;
        }
    }

    if (timeout == 0) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Link verification timeout!");
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
        val = in32(rswitch2->base + RCEC);
        out32(rswitch2->base + RCEC, val | BIT(etha->index) | BIT(rswitch2_gwca_hw_index(gwca)) | RCEC_RCE);
    }
    else {
        val = in32(rswitch2->base + RCDC);
        out32(rswitch2->base + RCDC, val | BIT(etha->index) | BIT(rswitch2_gwca_hw_index(gwca)));
    }
}

static void rswitch2_clock_agent_etha(rswitch2_dev_t *rswitch2, int enable)
{
    rswitch2_etha_t *etha = &rswitch2->etha;

    if (enable)
        out32(rswitch2->base + RCEC, BIT(etha->index) | RCEC_RCE);
    else
        out32(rswitch2->base + RCDC, BIT(etha->index));
}

static void rswitch2_reset(rswitch2_dev_t *rswitch2)
{
    if (rswitch2->parallel_mode)
        return;

    out32(rswitch2->base + RRC, RRC_RR);
    out32(rswitch2->base + RRC, 0);
}

static int rswitch2_bpool_init(rswitch2_dev_t *rswitch2)
{
    uint32_t val;

    if (rswitch2->parallel_mode) {
        return 0;
    }

    val = in32(rswitch2->base + CABPIRM);
    if (val & CABPIRM_BPR)
        return 0;

    out32(rswitch2->base + CABPIRM, CABPIRM_BPIOG);
    return rswitch2_reg_wait(rswitch2->base, CABPIRM, CABPIRM_BPR, CABPIRM_BPR);
}

static void rswitch2_mfwd_init(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int gwca_hw_idx = rswitch2_gwca_hw_index(gwca);

    if (!rswitch2->parallel_mode && !rswitch2->instance) {
        for (int i = 0; i < 5; i++) {
            out32(rswitch2->base + FWPC0(i), FWPC0_DEFAULT);
            out32(rswitch2->base + FWPBFC(i), 0);
        }
    }

    if (!rswitch2->gwca.used && !rswitch2->instance) {
        out32(rswitch2->base + FWPC1(gwca_hw_idx), BIT(1) | BIT(0));
    }

    out32(rswitch2->base + FWPBFCSDC(gwca->index, etha->index), 
            rswitch2->rx_queue_index);

    out32(rswitch2->base + FWPBFC(etha->index), BIT(gwca_hw_idx));

    out32(rswitch2->base + FWPBFC(gwca_hw_idx),
            in32(rswitch2->base + FWPBFC(gwca_hw_idx)) | BIT(etha->index));
}

static void rswitch2_gwca_set_rate_limit(rswitch2_gwca_t *gwca)
{
    uint32_t gwgrlulc, gwgrlc;

    switch (gwca->speed) {
    case 1000*1000:
        gwgrlulc = 0x0000005f;
        gwgrlc = 0x00010260;
        break;
    default:
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: This rate is not supported (%d)\n", gwca->speed);
        return;
    }

    out32(gwca->addr + GWGRLULC, gwgrlulc);
    out32(gwca->addr + GWGRLC, gwgrlc);
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
        out32(gwca->addr + GWMTIRM, GWMTIRM_MTIOG);
        if ((rc = rswitch2_reg_wait(gwca->addr, GWMTIRM, GWMTIRM_MTR, GWMTIRM_MTR)))
            return rc;

        /* Reset AXI RAM */
        out32(gwca->addr + GWARIRM, GWARIRM_ARIOG);
        if ((rc = rswitch2_reg_wait(gwca->addr, GWARIRM, GWARIRM_ARR, GWARIRM_ARR)))
            return rc;

        /* Setting flow */
        out32(gwca->addr + GWVCC, GWVCC_VEM_SC_TAG);
        out32(gwca->addr + GWTTFC, 0);

        out32(gwca->addr + GWDCBAC0, (bat_desc_paddr >> 32) & GWDCBAC0_DCBAUP);
        out32(gwca->addr + GWDCBAC1, bat_desc_paddr);
        out32(gwca->addr + GWMDNC, GWMDNC_TSDMN(0x1) | GWMDNC_TXDMN(0xF) | GWMDNC_RXDMN(0x1));

        gwca->speed = 1000*1000;
        rswitch2_gwca_set_rate_limit(gwca);
    
        /* Set interrupt delay.
         * Base on RTM: Interrupt delay function aims at delaying data interrupts 
         * to accumulate data during a certain among of time before requesting software 
         * to process the data. As a result, software load is diminished, and data latency 
         * is still guaranteed.
         * (Without delay, TX and RX performance is poor)
         * FIXME : How much is best for interrupt delay ?
         */
        out32(rswitch2->gwca.addr + GWIDPC, 1023);
        out32(rswitch2->gwca.addr + GWIDC(rswitch2->rx_queue_index), 32 * 8);

        /* IPV remapping configuration */
        //out32(gwca->addr + GWIRC, 0x76543210);
        //out32(gwca->addr + GWIRC, 0x00000000);
    } else {
        out32(rswitch2->gwca.addr + GWIDC(rswitch2->rx_queue_index), 32 * 8);
    }

    for (int i = 0; i < NUM_RX_QUEUES; i++) {
        int idx = i + rswitch2->rx_queue_index;
        out32(gwca->addr + GWDCC(idx), GWDCC_BALR | GWDCC_ETS | GWDCC_EDE);
        //out32(gwca->addr +  GWRDQDC(idx), NUM_RX_DESC);
    }

    for (int i = 0; i < NUM_TX_QUEUES; i++) {
        int idx = i + rswitch2->tx_queue_index;
        out32(gwca->addr + GWDCC(idx), GWDCC_BALR | GWDCC_DQT | GWDCC_EDE);
    }

    return EOK;
}

static int rswitch2_gwca_start(rswitch2_dev_t *rswitch2)
{
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             mode;
    int             rc;

    mode = in32(gwca->addr + GWMC) & GWMS_OPS_MASK;

    if (mode == GWMC_OPC_OPERATION) {
        return EOK;
    }

    rc = rswitch2_gwca_mode_change(rswitch2, GWMC_OPC_OPERATION);
    if (rc) {
        return rc;
    }

    return EOK;
}

static void rswitch2_rmac_mdc_clock(struct rswitch2_etha *etha, int enable)
{
    uint32_t val;

    val = in32(etha->addr + MPIC);
    if (enable) {
        val &= ~(MPIC_PSMCS_MASK | MPIC_PSMHT_MASK);
        val |= MPIC_PSMCS(0x05) | MPIC_PSMHT(0x06);
    } else {
        val &= ~(MPIC_PSMCS_MASK);
    }
    out32(etha->addr + MPIC, val);
}

static int rswitch2_etha_init(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    nic_config_t    *cfg = &rswitch2->cfg;
    unsigned char   *mac = cfg->current_address;
    int             rc = EOK;
    uint32_t        val;

    rc = rswitch2_etha_mode_change(rswitch2, EAMC_OPC_CONFIG);
    if (rc)
        goto done;
#if 0
    /* Reset TAS RAM */;
    out32(etha->addr + EATASRIRM, EATASRIRM_TASRIOG);
    if ((rc = rswitch2_reg_wait(etha->addr, EATASRIRM, EATASRIRM_TASRR, EATASRIRM_TASRR)))
        goto done;

    /* Setting flow */
    out32(etha->addr + EATTFC, 0);

    for (int i = 0; i < NUM_TX_QUEUES; i++) {
        out32(etha->addr + EATDQDC(i + TX_QUEUE_OFFSET), EATDQDC_DQD);
    }
#endif
    /* Set MAC address */
    out32(etha->addr + MRMAC1, mac[5] | (mac[4] << 8) | (mac[3] << 16)  | (mac[2] << 24));
    out32(etha->addr + MRMAC0, mac[1] | (mac[0] << 8));

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

    out32(etha->addr + MPIC, val);

    /* IPV remapping configuration */
    //out32(etha->addr +  EAIRC, 0x76543210);

    rswitch2_rmac_mdc_clock(etha, 1);

done:
    return rc;
}

static int rswitch2_etha_start(rswitch2_dev_t *rswitch2)
{
    rswitch2_etha_t *etha = &rswitch2->etha;
    int             rc = EOK;

    pthread_mutex_lock(&etha->etha_mutex);
    if (etha->mode == EAMC_OPC_OPERATION) {
        pthread_mutex_unlock(&etha->etha_mutex);
        return rc;
    }

    rc = rswitch2_etha_mode_change(rswitch2, EAMC_OPC_OPERATION);
    if (rc) {
        pthread_mutex_unlock(&etha->etha_mutex);
        return rc;
    }

    pthread_mutex_unlock(&etha->etha_mutex);

    /* Check link */
    rc = rswitch2_check_link(etha);

    return rc;
}

static int rswitch2_bat_desc_init(rswitch2_dev_t *rswitch2)
{
    size_t          size;
    RSWITCH2_DESC   *desc;
    uint32_t        bat_desc_off;
    int             idx, queue_index, i;

    if ((queue_index = rswitch2_rsrcdb_attach_resources(rswitch2, RSRCDB_TYPE_QUEUE, DBAT_ENTRY_NUM)) == -1) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rswitch2_rsrcdb_attach_resources for queue failed, errno %d", errno);
        return errno;
    }

    if (queue_index + DBAT_ENTRY_NUM > DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_MAX) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: queue number exceeds the limits %d", queue_index);
        return errno;
    }

    /* The first time allocate for all ports with assuming all ports connecting to one GWCA.
     * Allocate descriptor base address table. They should be aligned
     * to size of struct rswitch2_desc.  */
    if (!rswitch2->gwca.used && !rswitch2->instance) {
        size = sizeof(RSWITCH2_DESC) * DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_MAX;
        bat_desc_base = (uintptr_t)mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED | MAP_PHYS | MAP_ANON, NOFD, 0);
        if (bat_desc_base == (uintptr_t)MAP_FAILED) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: bat_desc map failed, errno %d", errno);
            return errno;
        }
        if (mem_offset64((void *)bat_desc_base, NOFD, 1, &bat_desc_paddr, 0) == -1) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: bat_desc mem_offset64 failed, errno %d", errno);
            return errno;
        }
        if (smmu_map_driver_memory((_Uint64t)bat_desc_paddr, size) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: bat_desc smmu_map failed, errno %d", errno);
            return errno;
        }
        /* Initialize all descriptors */
        memset((void *)bat_desc_base, 0, size);
        for (i = 0; i < DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_MAX; i++) {
            ((RSWITCH2_DESC *)bat_desc_base)[i].die_dt = DT_EOS;
        }
    } else if (bat_desc_paddr == 0) {
        rswitch2_gwca_t *gwca = &rswitch2->gwca;
        bat_desc_paddr = (uint64_t)(in32(gwca->addr + GWDCBAC0) & GWDCBAC0_DCBAUP) << 32;
        bat_desc_paddr|= (uint64_t)(in32(gwca->addr + GWDCBAC1));
    }

    size = sizeof(RSWITCH2_DESC) * DBAT_ENTRY_NUM;
    bat_desc_off = queue_index * sizeof(RSWITCH2_DESC);

    if (bat_desc_base == (uintptr_t)MAP_FAILED) {
        rswitch2->bat_desc_addr = bat_desc_paddr + bat_desc_off;
        rswitch2->bat_desc = (RSWITCH2_DESC *)mmap_device_memory(NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, 
                                0, rswitch2->bat_desc_addr);
    } else {
        rswitch2->bat_desc_addr = bat_desc_paddr + bat_desc_off;
        rswitch2->bat_desc = (RSWITCH2_DESC *)(bat_desc_base + bat_desc_off);
    }

    rswitch2->tx_queue_index = queue_index + TX_QUEUE_OFFSET;
    rswitch2->rx_queue_index = queue_index + RX_QUEUE_OFFSET;

    /* RX descriptor base address for all queues */
    for(i = 0; i < NUM_RX_QUEUES; i++) {
        desc = &rswitch2->bat_desc[RX_QUEUE_OFFSET + i];
        desc->die_dt = DT_LINKFIX; /* type */
        desc->dptrh = rswitch2->rx_bd[i][NUM_RX_DESC].dptrh;
        desc->dptrl = rswitch2->rx_bd[i][NUM_RX_DESC].dptrl;

        idx = rswitch2->rx_queue_index + i;
        rswitch2->rx_irq_mask[idx / 32] |= BIT(idx % 32);
    }

    /* TX descriptor base address for all queues */
    for(i = 0; i < NUM_TX_QUEUES; i++) {
        desc = &rswitch2->bat_desc[TX_QUEUE_OFFSET + i];
        desc->die_dt = DT_LINKFIX; /* type */
        desc->dptrh = rswitch2->tx_bd[i][NUM_TX_DESC].dptrh;
        desc->dptrl = rswitch2->tx_bd[i][NUM_TX_DESC].dptrl;

        idx = rswitch2->tx_queue_index + i;
        rswitch2->tx_irq_mask[idx / 32] |= BIT(idx % 32);
    }

    CACHE_FLUSH(&rswitch2->cachectl, (void *)rswitch2->bat_desc, rswitch2->bat_desc_addr, size);

    return EOK;
}

static int rswitch2_tx_desc_init(rswitch2_dev_t *rswitch2)
{
    size_t            size;
    off64_t           offset;
    uint32_t          i, queue;
    RSWITCH2_TX_DESC  *tx_desc;

    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        /* mbuf pointer array, corresponding to tx descriptor ring */
        size = sizeof(struct mbuf *) * NUM_TX_DESC;
        rswitch2->tx_pkts[queue] = malloc(size, M_DEVBUF, M_NOWAIT);
        if (rswitch2->tx_pkts[queue] == NULL) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: tx_pkts malloc failed, queue %d, errno %d", queue, errno);
            return errno;
        }

        memset(rswitch2->tx_pkts[queue], 0x00, size);

        /* Allocate all TX descriptors. */
        size = sizeof(RSWITCH2_TX_DESC) * (NUM_TX_DESC_WITH_LINK);
        rswitch2->tx_bd[queue] = mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED | MAP_PHYS | MAP_ANON, NOFD, 0);
        if (rswitch2->tx_bd[queue] == MAP_FAILED) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: tx_bd mmap failed, queue %d, errno %d", queue, errno);
            return errno;
        }
        if (mem_offset64((void *)rswitch2->tx_bd[queue], NOFD, 1, &offset, 0) == -1) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: tx_bd mem_offset64 failed, queue %d, errno %d", queue, errno);
            return errno;
        }
        if (smmu_map_driver_memory((_Uint64t)offset, size) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: tx_bd smmu map failed, queue %d, errno %d", queue, errno);
            return errno;
        }

        memset((void *)rswitch2->tx_bd[queue], 0x00, size);

        /* Build TX ring buffer */
        for (i = 0; i < NUM_TX_DESC; i++) {
            tx_desc = &rswitch2->tx_bd[queue][i];
            tx_desc->die_dt = DT_FEMPTY | DIE;
        }
        tx_desc = &rswitch2->tx_bd[queue][NUM_TX_DESC];
        tx_desc->dptrl = (uint32_t)offset;
        tx_desc->dptrh = (uint8_t)(offset >> 32);
        tx_desc->die_dt = DT_LINKFIX; /* type */

        CACHE_FLUSH(&rswitch2->cachectl, (void *)rswitch2->tx_bd[queue], offset, size);
    }

    return EOK;
}

static int rswitch2_rx_desc_init(rswitch2_dev_t *rswitch2)
{
    size_t               size;
    off64_t              offset, m_phys;
    uint32_t             i, queue;
    struct mbuf          *m;
    RSWITCH2_RX_DESC     *rx_desc;

    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        /* mbuf pointer array, corresponding to rx descriptor ring */
        size = sizeof(struct mbuf *) * NUM_RX_DESC;
        rswitch2->rx_pkts[queue] = malloc(size, M_DEVBUF, M_NOWAIT);
        if (rswitch2->rx_pkts[queue] == NULL) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rx_pkts malloc failed, queue %d, errno %d", queue, errno);
            return errno;
        }
        memset(rswitch2->rx_pkts[queue], 0, size);

        /* Allocate all RX descriptors. */
        size = sizeof(RSWITCH2_RX_DESC) * (NUM_RX_DESC_WITH_LINK);
        rswitch2->rx_bd[queue] = mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED | MAP_PHYS | MAP_ANON, NOFD, 0);
        if (rswitch2->rx_bd[queue] == MAP_FAILED) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rx_bd mmap failed, queue %d, errno %d", queue, errno);
            return errno;
        }
        if (mem_offset64((void *)rswitch2->rx_bd[queue], NOFD, 1, &offset, 0) == -1) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rx_bd mem_offset64 failed, queue %d, errno %d", queue, errno);
            return errno;
        }

        if (smmu_map_driver_memory((_Uint64t)offset, size) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rx_bd smmu map failed, queue %d, errno %d", queue, errno);
            return errno;
        }

        memset((void *)rswitch2->rx_bd[queue], 0, size);

        /* Build RX ring buffer */
        for (i = 0; i < NUM_RX_DESC; i++) {
            m = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
            if (m == NULL) {
                return ENOMEM;
            }
            m_phys = mbuf_phys(m);
            rswitch2->rx_pkts[queue][i] = m;
            /* RX descriptor */
            rx_desc = &rswitch2->rx_bd[queue][i];
            /* The size of the buffer should be on 4-byte boundary. */
            rx_desc->info_ds = ALIGN(PKT_BUF_SZ, DPTR_ALIGN);
            rx_desc->dptrl = (uint32_t)m_phys;
            rx_desc->dptrh = (uint8_t)(m_phys >> 32);
            rx_desc->die_dt =  DT_FEMPTY | DIE;

            CACHE_FLUSH(&rswitch2->cachectl, m->m_data, ((off64_t)rx_desc->dptrl | ((off64_t)rx_desc->dptrh << 32)), m->m_ext.ext_size);
        }
        rx_desc = &rswitch2->rx_bd[queue][NUM_RX_DESC];
        rx_desc->dptrl = (uint32_t)offset;
        rx_desc->dptrh = (uint8_t)(offset >> 32);
        rx_desc->die_dt = DT_LINKFIX; /* type */

        CACHE_FLUSH(&rswitch2->cachectl, (void *)rswitch2->rx_bd[queue], offset, size);
    }

    return EOK;
}

static void rswitch2_shutdown (void *arg)
{
    rswitch2_dev_t *rswitch2 = arg;
    if (!rswitch2_rsrcdb_check_parallel_mode(rswitch2)) {
        rswitch2_reset(arg);
        rswitch2_clock_enable(arg, 0);
    }
}

static void rswitch2_cleanup (rswitch2_dev_t *rswitch2)
{
    nic_config_t *cfg = &rswitch2->cfg;
    struct mbuf  *m;
    uint32_t     i, queue;
    off64_t      offset;

    /* Disable and Detach all interrupts */
    rswitch2_enadis_data_irq(rswitch2, 0, 0);
    rswitch2_enadis_data_irq(rswitch2, 1, 0);

    for (i = 0; i < cfg->num_irqs; i++) {
        InterruptDetach(rswitch2->iid[i]);
    }

#if 0
    InterruptDetach(rswitch2->etha_iid);
#endif

    /* Checks and returns EINVAL if not in the interrupt_entry list */
    interrupt_entry_remove(&rswitch2->ient_rx, NULL);
    interrupt_entry_remove(&rswitch2->ient_tx, NULL);

    shutdownhook_disestablish(rswitch2->sdhook);

    /* Disable descriptor chains */
    for (i = 0; i < DBAT_ENTRY_NUM; i++) {
        rswitch2->bat_desc[i].die_dt = DT_EOS;
    }

    /* Cleanup Tx allocations */
    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        if (rswitch2->tx_pkts[queue]) {
            for (i = 0; i < NUM_TX_DESC; i++) {
                m = rswitch2->tx_pkts[queue][i];
                if (m != NULL) {
                    m_freem(m);
                }
            }
            free(rswitch2->tx_pkts[queue], M_DEVBUF);
        }
        if (rswitch2->tx_bd[queue]) {
            offset = (off64_t)rswitch2->tx_bd[queue][NUM_TX_DESC].dptrh << 32 | rswitch2->tx_bd[queue][NUM_TX_DESC].dptrl;
            if (smmu_unmap_driver_memory((_Uint64t)offset,
                                         sizeof(RSWITCH2_TX_DESC) *
                                         NUM_TX_DESC_WITH_LINK) != 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: unmap tx_bd failed, queue %d, errno %d", queue, errno);
            }

            munmap((void *)rswitch2->tx_bd[queue], sizeof(RSWITCH2_TX_DESC) * NUM_TX_DESC_WITH_LINK);
            rswitch2->tx_bd[queue] = NULL;
        }
    }

    /* Cleanup Rx allocations */
    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        if (rswitch2->rx_pkts[queue]) {
            for (i = 0; i < NUM_RX_DESC; i++) {
                m = rswitch2->rx_pkts[queue][i];
                if (m != NULL) {
                    m_freem(m);
                }
            }
            free(rswitch2->rx_pkts[queue], M_DEVBUF);
        }
        if (rswitch2->rx_bd[queue]) {
            offset = (off64_t)rswitch2->rx_bd[queue][NUM_RX_DESC].dptrh << 32 | rswitch2->rx_bd[queue][NUM_RX_DESC].dptrl;
            if (smmu_unmap_driver_memory((_Uint64t)offset,
                                         sizeof(RSWITCH2_RX_DESC) *
                                         NUM_RX_DESC_WITH_LINK) != 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: unmap rx_bd failed, queue %d, errno %d", queue, errno);
            }

            munmap((void *)rswitch2->rx_bd[queue], sizeof(RSWITCH2_RX_DESC) * NUM_RX_DESC_WITH_LINK);
            rswitch2->rx_bd[queue] = NULL;
        }
    }

    if (instance == 0) {
        if (rswitch2->base) {
            if (smmu_unregister_mmio_device(rswitch2->cfg.io_window_base[0],
                rswitch2->cfg.io_window_size[0]) != 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: unregister mmio device failed, errno %d", errno);
            }

            munmap_device_io(rswitch2->base, rswitch2->cfg.io_window_size[0]);
            rswitch2->base = (uintptr_t)MAP_FAILED;
        }

        if (bat_desc_base) {
            if (smmu_unmap_driver_memory((_Uint64t)bat_desc_paddr, sizeof(RSWITCH2_DESC) * DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_MAX) != 0) {
                   slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: unmap bat_desc failed, errno %d", errno);
            }

            munmap((void *)bat_desc_base, sizeof(RSWITCH2_DESC) * DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_MAX);
        }
    } else if (bat_desc_base == 0) {
        munmap((void *)rswitch2->bat_desc, sizeof(RSWITCH2_DESC) * DBAT_ENTRY_NUM);
    }

    rswitch2->bat_desc = NULL;

    rswitch2_rsrcdb_detach_resources(rswitch2, RSRCDB_TYPE_QUEUE, rswitch2->rx_queue_index - RX_QUEUE_OFFSET, DBAT_ENTRY_NUM);
    rswitch2_rsrcdb_detach_resources(rswitch2, RSRCDB_TYPE_ETHA, 0, 0);
    rswitch2_rsrcdb_detach_resources(rswitch2, RSRCDB_TYPE_GWCA, 0, 0);

    if (rswitch2->avb) {
        nw_pthread_reap (rswitch2->tid);
    }

    cache_fini(&rswitch2->cachectl);
}

void rswitch2_hw_stop (rswitch2_dev_t *rswitch2)
{
    struct mbuf *m;
    uint32_t    i, queue;

    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        /* Clear any pending Tx buffers */
        for (i = 0; i < NUM_TX_DESC; i++) {
            m = rswitch2->tx_pkts[queue][i];
            if (m != NULL) {
                m_freem(m);
            }
            rswitch2->tx_bd[queue][i].die_dt = DT_EEMPTY;
        }
    }
}

static void rswitch2_stop (struct ifnet *ifp, int disable)
{
    rswitch2_dev_t          *rswitch2;
    nic_config_t            *cfg;
    struct nw_work_thread   *wtp;

    rswitch2 = ifp->if_softc;
    cfg = &rswitch2->cfg;
    wtp = WTP;

    NW_SIGLOCK_P(&ifp->if_snd_ex, rswitch2->iopkt, wtp);
    callout_stop(&rswitch2->mii_callout);
    MDI_DisableMonitor(rswitch2->mdi);
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);
    ifp->if_flags &= ~(IFF_RUNNING | IFF_OACTIVE);
    NW_SIGUNLOCK_P(&ifp->if_snd_ex, rswitch2->iopkt, wtp);

    MDI_PowerdownPhy(rswitch2->mdi, cfg->phy_addr);
    rswitch2_hw_stop(rswitch2);

    rswitch2_clear_stats(rswitch2);
}

static int rswitch2_init (struct ifnet *ifp)
{
    rswitch2_dev_t          *rswitch2;
    rswitch2_etha_t         *etha;
    nic_config_t            *cfg;
    struct nw_work_thread   *wtp;
    uint32_t                mac_change;
    int rc;

    rswitch2 = ifp->if_softc;
    etha = &rswitch2->etha;
    cfg = &rswitch2->cfg;
    wtp = WTP;

    /* check if configuration is required */
    mac_change = memcmp(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen);
    if (mac_change) {
        memcpy(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen);
        {
            unsigned char * mac = cfg->current_address;
            out32(etha->addr + MRMAC1, mac[5] | (mac[4] << 8) | (mac[3] << 16) | (mac[2] << 24));
            out32(etha->addr + MRMAC0, mac[1] | (mac[0] << 8));
        }
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: MAC address updated");
        }
    }

    if ((ifp->if_flags & IFF_RUNNING) == 0) {
        NW_SIGLOCK_P(&ifp->if_snd_ex, rswitch2->iopkt, wtp);
        ifp->if_flags |= IFF_RUNNING;
        NW_SIGUNLOCK_P(&ifp->if_snd_ex, rswitch2->iopkt, wtp);
        rswitch2_rmac_mdc_clock(etha, 1);
        rswitch2_MDI_MonitorPhy(rswitch2);
    }

    rc = rswitch2_etha_start(rswitch2);
    if (rc) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2:rswitch2_etha_start failed");
        return rc;
    }

    rc = rswitch2_gwca_start(rswitch2);
    if (rc) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2:rswitch2_gwca_start failed");
        return rc;
    }

    /* Enable RX */
    /* FIXME: GWTRC is set for TX only */
    rswitch2_modify(rswitch2->gwca.addr, GWTRC((rswitch2->rx_queue_index)/32), 0, 
                    BIT((rswitch2->rx_queue_index)%32));

    return EOK;
}

static void *rswitch2_rx_thread (void *arg)
{
    rswitch2_dev_t  *rswitch2 = arg;
    int             rc, queue, mask;
    struct _pulse   pulse;
    int             idx, i;

    while (1) {
        rc = MsgReceivePulse(rswitch2->chid, &pulse, sizeof(pulse), NULL);
        if (rc == EOK) {
            switch (pulse.code) {
                case RSWITCH2_RX_PULSE:
                    queue = pulse.value.sival_int;
                    for (i = 0; i < NUM_RX_QUEUES; i++) {
                        idx = rswitch2->rx_queue_index + i;
                        mask = rswitch2->dis[idx / 32] & rswitch2->rx_irq_mask[idx / 32];
                        if ((queue == i) && (mask & (BIT(idx % 32)))) {
                            rswitch2_receive(rswitch2, WTP, i); // For queue i
                        }
                    }
                    break;
                case RSWITCH2_QUIESCE_PULSE:
                    quiesce_block(pulse.value.sival_int);
                    break;
                default:
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch Rx: Unknown pulse %d received", pulse.code);
                    break;
            }
        } else {
          slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch Rx: MsgReceivePulse: %s", strerror(rc));
        }
    }
    return NULL;
}

static void rswitch2_rx_thread_quiesce (void *arg, int die)
{
    rswitch2_dev_t   *rswitch2 = arg;

    MsgSendPulse(rswitch2->coid, SIGEV_PULSE_PRIO_INHERIT, RSWITCH2_QUIESCE_PULSE, die);
    return;
}

static int rswitch2_rx_thread_init (void *arg)
{
    rswitch2_dev_t          *rswitch2 = arg;
    struct nw_work_thread   *wtp = WTP;

    pthread_setname_np(gettid(), RSWITCH2_RX_THREAD_NAME);

    wtp->quiesce_callout = rswitch2_rx_thread_quiesce;
    wtp->quiesce_arg = rswitch2;
    return EOK;
}

int rswitch2_interrupt_attach (rswitch2_dev_t *rswitch2)
{
    nic_config_t    *cfg = &rswitch2->cfg;
    rswitch2_gwca_t *gwca = &rswitch2->gwca;
    int             i, rc;

    if (cfg->num_irqs == 0) {
        for (i = 0; i < RSWITCH2_IRQ_NUM; i++) {
            if (gwca->index == 0) {
                cfg->irq[i] = GWCA0_RXTX_CPU(i);
            } else {
                cfg->irq[i] = GWCA1_RXTX_CPU(i);
            }
            cfg->num_irqs++;
        }
    }

    if (rswitch2->avb) {
        for (i = 0; i < cfg->num_irqs; i++) {
            rc = InterruptAttach_r(cfg->irq[i], rswitch2_gwca_isr_avb, rswitch2, sizeof(rswitch2),
                _NTO_INTR_FLAGS_TRK_MSK | _NTO_INTR_FLAGS_END);
            if (rc < 0) {
                rc = -rc;
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "rswitch2: Failed to attach to the interrupt: %d", rc);
                rswitch2_cleanup(rswitch2);
                return rc;
            }
            rswitch2->iid[i] = rc;
        }
    } else {
        for (i = 0; i < cfg->num_irqs; i++) {
            rc = InterruptAttachArray_r(cfg->irq[i], rswitch2_gwca_isr, rswitch2, sizeof(rswitch2),
                _NTO_INTR_FLAGS_TRK_MSK | _NTO_INTR_FLAGS_END);
            if (rc < 0) {
                rc = -rc;
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "rswitch2: Failed to attach to the interrupt: %d", rc);
                rswitch2_cleanup(rswitch2);
                return rc;
            }
            rswitch2->iid[i] = rc;
        }
    }
#if 0
    rc = InterruptAttach_r(ETHA_STATUS((&rswitch2->etha)->index), rswitch2_etha_isr, rswitch2, sizeof(rswitch2),
            _NTO_INTR_FLAGS_TRK_MSK);
    if (rc < 0) {
        rc = -rc;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
            "rswitch2: Failed to attach to the interrupt: %d", rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }
    rswitch2->etha_iid = rc;
#endif

    return EOK;
}

int rswitch2_detach (struct device *dev, int flags)
{
    rswitch2_dev_t      *rswitch2;
    struct ifnet        *ifp;

    instance--;
    rswitch2 = (rswitch2_dev_t *)dev;
    ifp = &rswitch2->ecom.ec_if;

    ether_ifdetach(ifp);
    if_detach(ifp);
    rswitch2_phy_fini(rswitch2);

    rswitch2_cleanup(rswitch2);

    rswitch2_rmac_mdc_clock(&rswitch2->etha, 0);
    rswitch2_etha_mode_change(rswitch2, EAMC_OPC_DISABLE);
    rswitch2_clock_agent_etha(rswitch2, 0);

    return EOK;
}

int rswitch2_attach (struct device *parent, struct device *self, void *aux)
{
    rswitch2_dev_t      *rswitch2;
    rswitch2_etha_t     *etha;
    rswitch2_gwca_t     *gwca;
    rswitch2_serdes_t   *serdes;
    attach_args_t       *attach_args;
    struct  ifnet       *ifp;
    nic_config_t        *cfg;
    int                 rc;

    rswitch2 = (rswitch2_dev_t*)self;
    etha = &rswitch2->etha;
    gwca = &rswitch2->gwca;
    serdes = &rswitch2->serdes;
    attach_args = aux;

    /* Copy the config across */
    cfg = &rswitch2->cfg;
    memcpy(cfg, &attach_args->cfg, sizeof(*cfg));

    rswitch2->drv_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    rswitch2->ptp_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    rswitch2->mdi_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    rswitch2->set_flow = attach_args->set_flow;
    rswitch2->avb = attach_args->avb;
    rswitch2->tx_cleanup_needed = 0;
    rswitch2->port = cfg->device_index;
    rswitch2->port0_reset = attach_args->port0_reset;
    rswitch2->instance = instance;
    rswitch2->mii = attach_args->mii;
    rswitch2->parallel_mode = attach_args->parallel_mode;
    cfg->connector = NIC_CONNECTOR_MII;
    rswitch2->iopkt = attach_args->iopkt;
    rswitch2->dev.dv_dll_hdl = attach_args->dll_hdl;

    callout_init(&rswitch2->mii_callout);
    rswitch2->cachectl.fd = NOFD;
    if (cache_init(0, &rswitch2->cachectl, NULL) == -1) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init cache failed");
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    rswitch2->base = rswitch2_base;
    pthread_mutex_init(&etha->etha_mutex, NULL);

    /*  Use (forward to) GWCA0/GWCA1 */
    gwca->index = attach_args->gwca_idx;
    gwca->addr  = rswitch2->base + RSWITCH2_GWCA_OFFSET + gwca->index * RSWITCH2_GWCA_SIZE;

    rswitch2->parallel_mode = rswitch2_rsrcdb_check_parallel_mode(rswitch2);
    rswitch2->gwca.used = rswitch2_rsrcdb_check_gwca_used(rswitch2);

    if ((rc = rswitch2_rsrcdb_attach_resources(rswitch2, RSRCDB_TYPE_GWCA, 1) == -1)) {
        rc = errno;
        slogf (_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rswitch2_rsrcdb_attach_resources for GWCA%d failed %s", gwca->index, strerror (errno));
        rswitch2_cleanup (rswitch2);
        return rc;
    }

    rswitch2->gwca.rsrc_idx = rc;

    etha->phy_speed = 1000*1000;
    switch(cfg->media_rate) {
        case 10*1000:
        case 100*1000:
            etha->phy_speed = cfg->media_rate;
            break;
        default:
            etha->phy_speed = 1000*1000;
            break;
    }

    etha->index = cfg->device_index;
    etha->addr = rswitch2->base + RSWITCH2_ETHA_OFFSET + etha->index * RSWITCH2_ETHA_SIZE;

    if ((rc = rswitch2_rsrcdb_attach_resources(rswitch2, RSRCDB_TYPE_ETHA, 1) == -1)) {
        rc = errno;
        slogf (_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rswitch2_rsrcdb_attach_resources for ETHA%d failed %s", etha->index, strerror (errno));
        rswitch2_cleanup (rswitch2);
        return rc;
    }

    if (rswitch2->avb) {
        if ((rswitch2->chid = ChannelCreate(0)) == -1) {
            rc = errno;
            slogf (_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: ChannelCreate failed %s", strerror (errno));
            rswitch2_cleanup (rswitch2);
            return rc;
        }
        if ((rswitch2->coid = ConnectAttach(ND_LOCAL_NODE, 0, rswitch2->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
            rc = errno;
            slogf (_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: ConnectAttach failed %s", strerror (errno));
            rswitch2_cleanup (rswitch2);
            return rc;
        }
    }

    rc = rswitch2_tx_desc_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init TX Descriptors failed: %d", rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    rc = rswitch2_rx_desc_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init RX Descriptors failed: %d", rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    rc = rswitch2_bat_desc_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init BAT Descriptors failed: %d", rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    rswitch2->sdhook = shutdownhook_establish(rswitch2_shutdown, rswitch2);

    rswitch2_reset(rswitch2);

    rswitch2_clock_enable(rswitch2, 1);

    rc = rswitch2_bpool_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init bpool failed: %d", rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    rswitch2_mfwd_init(rswitch2);

    rc = rswitch2_gwca_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init gwca%d failed: %d", gwca->index, rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    rc = rswitch2_etha_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init etha%d failed: %d", etha->index, rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    /* Initialize PHY M88E2110 */
    rc = rswitch2_phy_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rswitch2_phy_init failed: %d", rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    serdes->index = cfg->device_index;
    rc = rswitch2_serdes_init(rswitch2);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Init serder%d failed: %d", etha->index, rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    /* Initialize gPTP */
    rswitch2_ptp_init(rswitch2);

    rswitch2->ient_tx.func = rswitch2_gwca_process_interrupt_tx;
    rswitch2->ient_tx.enable = rswitch2_gwca_enable_interrupt_tx;
    rswitch2->ient_tx.arg = rswitch2;

    if ((rc = interrupt_entry_init(&rswitch2->ient_tx, 0, NULL,
        cfg->priority)) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: interrupt_entry_init tx failed: %d", rc);
        rswitch2_cleanup(rswitch2);
        return rc;
    }

    if (!rswitch2->avb) {
        rswitch2->ient_rx.func = rswitch2_gwca_process_interrupt_rx;
        rswitch2->ient_rx.enable = rswitch2_gwca_enable_interrupt_rx;
        rswitch2->ient_rx.arg = rswitch2;

        if ((rc = interrupt_entry_init(&rswitch2->ient_rx, 0, NULL,
            cfg->priority)) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: interrupt_entry_init rx failed: %d", rc);
            rswitch2_cleanup(rswitch2);
            return rc;
        }
    } else {
        struct nw_stk_ctl *sctlp = &stk_ctl;
        for (int queue = 0; queue < NUM_RX_QUEUES; queue++) {
            /*
             * Create a pulse for each class of traffic. Start at the
             * default rx_prio but increment by 2 for each class to
             * allow a process to run at (class - 1) receiving and still
             * have priority over the lower class traffic without
             * impacting the dequeueing of packets from the limited Rx
             * descriptors.
             */

            SIGEV_PULSE_INIT(&rswitch2->isr_event[queue], rswitch2->coid, sctlp->rx_prio + (2 * queue),
                 RSWITCH2_RX_PULSE, queue);
        }

        /* pseudo interrupt for Rx queue */
        rswitch2->inter_queue.func = rswitch2_process_queue;
        rswitch2->inter_queue.enable = rswitch2_enable_queue;
        rswitch2->inter_queue.arg = rswitch2;

        if ((rc = interrupt_entry_init(&rswitch2->inter_queue, 0, NULL, cfg->priority)) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s(): interrupt_entry_init(rx) failed: %d", __func__, rc);
            rswitch2_cleanup(rswitch2);
            return rc;
        }

        if ((rc = pthread_mutex_init(&rswitch2->rx_mutex, NULL)) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s(): rx_mutex init failed: %d", __func__, rc);
            rswitch2_cleanup(rswitch2);
            return rc;
        }

        IFQ_SET_MAXLEN(&rswitch2->rx_queue, IFQ_MAXLEN);
        nw_pthread_create(&rswitch2->tid, NULL, rswitch2_rx_thread, rswitch2, 0,
            rswitch2_rx_thread_init, rswitch2);
    }

    memset(&rswitch2->spinlock, 0, sizeof(rswitch2->spinlock));

    /* Attach interrupts handler to an interrupt source */
    rswitch2_interrupt_attach(rswitch2);

    rswitch2_clear_stats(rswitch2);

    switch (cfg->device_index) {
        case 0:
            strcpy((char *)cfg->device_description, "Renesas RSwitch2 Network Device tsn0");
            strcpy(rswitch2->dev.dv_xname, "tsn0");
            break;
        case 1:
            strcpy((char *)cfg->device_description, "Renesas RSwitch2 Network Device tsn1");
            strcpy(rswitch2->dev.dv_xname, "tsn1");
            break;
        case 2:
            strcpy((char *)cfg->device_description, "Renesas RSwitch2 Network Device tsn2");
            strcpy(rswitch2->dev.dv_xname, "tsn2");
            break;
        default:
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "Don't support device index= %d", cfg->device_index);
            return ENODEV;
    }

    ifp = &rswitch2->ecom.ec_if;
    strcpy(ifp->if_xname, rswitch2->dev.dv_xname);
    ifp->if_softc = rswitch2;
    /* No multicast filtering in hardware so set IFF_ALLMULTI */
    ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST | IFF_ALLMULTI;
    ifp->if_ioctl = rswitch2_ioctl;
    ifp->if_start = rswitch2_start;
    ifp->if_init  = rswitch2_init;
    ifp->if_stop  = rswitch2_stop;
    IFQ_SET_READY(&ifp->if_snd);
    rswitch2->ecom.ec_capabilities |= ETHERCAP_VLAN_MTU;

    if (cfg->verbose) {
        nic_dump_config(cfg);
    }

    if_attach(ifp);
    ether_ifattach(ifp, cfg->current_address);

    if (rswitch2->avb) {
        /* Intercept if_output for pulling off AVB packets */
        rswitch2->stack_output = rswitch2->ecom.ec_if.if_output;
        rswitch2->ecom.ec_if.if_output = rswitch2_output;
    }

    rswitch2_gwca_enable_interrupt_tx(rswitch2);
    rswitch2_gwca_enable_interrupt_rx(rswitch2);

    return EOK;
}

static char *rswitch2_opts[] = {
    "flow",
#define OPT_FLOW            0
    "xgmii",
#define OPT_XGMII           1
    "avb",  
#define OPT_AVB             2
    "deviceidx",
#define OPT_TSN_PORT        3
    "portnum",
#define OPT_TSN_PORT_NUM    4
    "port0_reset",
#define OPT_PORT0_RESET     5
    "gwcaidx",
#define OPT_GWCA            6
    "parallel_mode",
#define OPT_PARALLEL_MODE   7
    NULL
};

static int rswitch2_parse_options (const char *optstring,
                 attach_args_t *attach_args)
{
    nic_config_t    *cfg;
    char            *options, *freeptr, *value;
    int             opt;

    if (optstring == NULL) {
        return EOK;
    }

    cfg = &attach_args->cfg;

    /* getsubopt() is destructive */
    freeptr = options = strdup(optstring);

    while (options && *options != '\0') {
        opt = getsubopt(&options, rswitch2_opts, &value);
        switch (opt) {

        case OPT_FLOW:
            attach_args->set_flow = strtoul(value, 0, 0);
            if (attach_args->set_flow > 3) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rswitch2: Unknown flow value %d, forcing off",
                  attach_args->set_flow);
            attach_args->set_flow = 0;
            }
            break;

        case OPT_XGMII:
            attach_args->mii = SERDES_USXGMII;
            break;

        case OPT_AVB:
            attach_args->avb = 1;
            break;

        case OPT_TSN_PORT:
            attach_args->cfg.device_index = strtoul(value, 0, 0);
            break;

        case OPT_TSN_PORT_NUM:
            tsn_port_num = strtoul(value, 0, 0);
            if (tsn_port_num > RSWITCH2_TSN_PORT_MAX)
                tsn_port_num = RSWITCH2_TSN_PORT_MAX;
            break;
        case OPT_PORT0_RESET:
           attach_args->port0_reset = 1;
           break;

        case OPT_GWCA:
           attach_args->gwca_idx = strtoul(value, 0, 0);
           break;

        case OPT_PARALLEL_MODE:
           attach_args->parallel_mode = 1;
           break;

        default:
            if (nic_parse_options(cfg, value) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rswitch2: Unknown option %s", value);
            (free)(freeptr);
            return EINVAL;
            }
        }
    }
    (free)(freeptr);
    return EOK;
}

CFATTACH_DECL(rswitch2,
    sizeof(rswitch2_dev_t),
    NULL,
    rswitch2_attach,
    rswitch2_detach,
    NULL);

#if 0
static void rswitch2_hwinfo_find_nicadd (nic_config_t *cfg, unsigned hwi_off)
{
    unsigned  nicaddr_idx;

    if (hwi_off == HWI_NULL_OFF) {
        /* Find network hardware information. */
        hwi_off = hwi_find_device("rswitch", 0);
    }

    if (hwi_off != HWI_NULL_OFF) {
        nicaddr_idx = cfg->device_index == -1 ? 0 : cfg->device_index;
        /* Find MAC address */
        hwitag_find_nicaddr(hwi_off, &nicaddr_idx, cfg->current_address);
    }
}

static void rswitch2_parse_syspage (nic_config_t *cfg)
{
    hwi_tag     *tag;
    unsigned    item;
    unsigned    irq_idx = 0;
    int         i;

    /* Find network hardware information. */
    item = hwi_find_device("rswitch", 0);
    if (item == HWI_NULL_OFF) {
        return;
    }

    if (cfg->num_io_windows == 0) {
        /* Find base address */
        if ((tag = hwi_tag_find(item, HWI_TAG_NAME_location, NULL)) != NULL) {
            cfg->io_window_base[0] = (paddr_t)tag->location.base;
            cfg->io_window_size[0] = (size_t)tag->location.len;
            cfg->num_io_windows = 1;
        }
    }

    if (cfg->num_irqs == 0) {
        /* Find IRQ */
        for (i = 0; i < 8; i++) {
            cfg->irq[i] = hwitag_find_ivec(item, &irq_idx);
            if (cfg->irq[i] == HWI_ILLEGAL_VECTOR) {
                cfg->irq[i] = -1;
                break;
            }
            cfg->num_irqs++;
            irq_idx++;
        }
    }

    if ((memcmp(cfg->current_address, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0)) {
        rswitch2_hwinfo_find_nicadd(cfg, item);
    }
}
#endif

static int rswitch2_entry (void *dll_hdl, struct _iopkt_self *iopkt, char *options)
{
    attach_args_t         *attach_args;
    nic_config_t          *cfg;
    int                   rc, port;
    struct ifnet          *ifp;
    struct drvcom_config  dcon;
    int		              done;
    struct device	      *dev;

    /* Check if it is already mounted by doing a "nicinfo" on each interface */
    IFNET_FOREACH(ifp) {
        dcon.dcom_cmd.ifdc_cmd = DRVCOM_CONFIG;
        dcon.dcom_cmd.ifdc_len = sizeof(dcon.dcom_config);
        rc = ifp->if_ioctl(ifp, SIOCGDRVCOM, (caddr_t)&dcon);
        if ((rc == EOK) && (dcon.dcom_config.num_io_windows > 0) &&
                (dcon.dcom_config.io_window_base[0] == RCAR_RSWITCH2_REG_BASE) ) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "%s:%d - Driver already loaded for %s",
                  __FUNCTION__, __LINE__, ifp->if_xname);
            return EBUSY;
        }
    }

    attach_args = calloc(1,sizeof(*attach_args));
    attach_args->cfg.device_index = -1;
    attach_args->iopkt = iopkt;
    attach_args->dll_hdl = dll_hdl;
    attach_args->mii = SERDES_SGMII;
    attach_args->gwca_idx = 1;

    /* Set defaults before parsing the command line */
    cfg = &attach_args->cfg;
    cfg->media_rate = -1;
    cfg->duplex = -1;
    attach_args->set_flow = -1;
    cfg->mtu = ETHERMTU;
    cfg->mru = ETHERMTU;
    cfg->flags = NIC_FLAG_MULTICAST;
    cfg->mac_length = ETHER_ADDR_LEN;
    cfg->priority = stk_ctl.rx_prio;

    if (rswitch2_parse_options(options, attach_args) != EOK) {
        (free)(attach_args);
        return EINVAL;
    }

    /* If setting speed, must set duplex and vice versa */
    if (((cfg->media_rate != -1) && (cfg->duplex == -1)) ||
        ((cfg->media_rate == -1) && (cfg->duplex != -1))) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rswitch2: Must set both speed and duplex if setting either");
        return EINVAL;
    }

    /* If speed set then turn flow off if not set */
    if ((cfg->media_rate != -1) && (attach_args->set_flow == -1)) {
        attach_args->set_flow = 0;
    }

    rswitch2_rsrcdb_create_resources();

    /*
     * If base address, IRQ, MAC address are not set from the command line,
     * set them from the syspage info and if not there then set to
     * default values.
     */
    //rswitch2_parse_syspage(cfg);

    if (cfg->num_io_windows == 0) {
        cfg->io_window_base[0] = RCAR_RSWITCH2_REG_BASE;
        cfg->io_window_size[0] = RCAR_RSWITCH2_REG_SIZE;
        cfg->num_io_windows = 1;
    }

    rswitch2_base = mmap_device_io(cfg->io_window_size[0], cfg->io_window_base[0]);
    if (rswitch2_base == (uintptr_t)MAP_FAILED) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: mmap device falied");
        return rc;
    }

    if (smmu_register_mmio_device(cfg->io_window_base[0], cfg->io_window_size[0]) != 0) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: mmio device smmu register failed, errno %d", rc);
        return rc;
    }

    if (cfg->num_irqs == 0) {
    }

    if ((memcmp(cfg->current_address, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0) ||
        ETHER_IS_MULTICAST(cfg->current_address)) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rswitch2: Invalid MAC address %02x%02x%02x%02x%02x%02x",
              cfg->current_address[0], cfg->current_address[1],
              cfg->current_address[2], cfg->current_address[3],
              cfg->current_address[4], cfg->current_address[5]);
        return EINVAL;
    }

    port = (cfg->device_index  == -1) ? 0 : cfg->device_index;
    done = tsn_port_num;
    instance = 0;

    for (; port < RSWITCH2_TSN_PORT_MAX; port++) {
        cfg->device_index = port;
        if (instance) {
            //rswitch2_hwinfo_find_nicadd(cfg, HWI_NULL_OFF);
            cfg->current_address[5] += 1;
        }
        memcpy(cfg->permanent_address, cfg->current_address, ETHER_ADDR_LEN);
        dev = NULL; /* No Parent */
        rc = dev_attach("tsn", options, &rswitch2_ca, attach_args, &single, &dev, NULL);
        if (rc == EOK) instance++;
        dev->dv_dll_hdl = dll_hdl;
        /* we're done */
        if (--done == 0) break;
    }

    (free)(attach_args);

    if (instance > 0) {
      return EOK;
    }
    return ENODEV;
}

struct _iopkt_drvr_entry IOPKT_DRVR_ENTRY_SYM(rswitch2) = IOPKT_DRVR_ENTRY_SYM_INIT(rswitch2_entry);

#ifdef VARIANT_a
#include <nw_dl.h>
/* This is what gets specified in the stack's dl.c */
struct nw_dll_syms rswitch2_syms[] = {
    {"iopkt_drvr_entry", &IOPKT_DRVR_ENTRY_SYM(rswitch2)},
    {NULL, NULL}
};
#endif
