/*
 * $QNXLicenseC:
 * Copyright (c) 2022, 2023, BlackBerry Limited.
 * Copyright (c) 2022, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Module Description:  board specific interface

#include <internal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <rcar.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>
#include <aarch64/r-car.h>

static void query_hwinfo(sdmmc_bs_ext *bsext, uint32_t idx)
{
    unsigned hwi_off, tag_idx;
    hwi_tag *tag;

    /* SD card VDD Power */
    hwi_off = hwi_find_device(RCAR_HWI_SDHI_VDD, idx);
    if(hwi_off != HWI_NULL_OFF) {
        tag_idx = 0;
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            bsext->pwr_vdd_pbase = tag->location.base;
            bsext->pwr_vdd_size = tag->location.len;

            while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                if(strcmp(RCAR_HWI_PFC_REG,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pwr_vdd_offset = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_PFC_SHIFT,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pwr_vdd_shift = tag->regname.offset;
                }
            }
        }
    }

    /* SD card IF Power */
    hwi_off = hwi_find_device(RCAR_HWI_SDHI_IF, idx);
    if(hwi_off != HWI_NULL_OFF) {
        tag_idx = 0;
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            bsext->pwr_if_pbase = tag->location.base;
            bsext->pwr_if_size = tag->location.len;

            while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                if(strcmp(RCAR_HWI_PFC_REG,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pwr_if_offset = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_PFC_SHIFT,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pwr_if_shift = tag->regname.offset;
                }
            }
        }
    }

    hwi_off = hwi_find_device(RCAR_HWI_SDHI_PFC, idx);
    if(hwi_off != HWI_NULL_OFF) {
        tag_idx = 0;
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            bsext->pfc_pbase = tag->location.base;
            bsext->pfc_size = tag->location.len;

            while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                if(strcmp(RCAR_HWI_PFC_REG,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pfc_offset = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_PFC_PMMR,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pfc_pmmr = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_PFC_MASK,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pfc_mask = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_PFC_SHIFT,  __hwi_find_string(tag->regname.regname)) == 0) {
                    bsext->pfc_mask <<= tag->regname.offset;
                }
            }
        }
    }
}

static int rcar_bs_cd(sdio_hc_t *hc)
{
    rcar_sdmmc_t    *sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;
    sdmmc_bs_ext    *bsext = (sdmmc_bs_ext *)hc->bs_hdl;
    int             cstate, pstate;

    cstate = CD_RMV;

    if(hc->flags & HC_FLAG_DEV_MMC) {
        cstate = (CD_INS);
    } else {
        pstate = sdmmc_read(sdmmc->vbase, MMC_SD_INFO1);

        hc->caps |= HC_CAP_CD_INTR;

        if ((pstate & SDH_INFO1_CD)) {
            cstate  |= CD_INS;
            if (!(pstate & SDH_INFO1_WP) && !(bsext->nowp)) {
                cstate |= CD_WP;
            }
        }
    }

    return cstate;
}

static int rcar_bs_signal_voltage(sdio_hc_t *hc, int signal_voltage)
{
    sdmmc_bs_ext    *bsext = (sdmmc_bs_ext *)hc->bs_hdl;
    uint32_t        pocctrl0;

    if (!bsext->pfc_base && !bsext->pwr_if_base) {
        /* No signal voltage line to use, just return EOK */
        return (EOK);
    }

    // Signal IO voltage level
    if (bsext->pfc_base) {
        pocctrl0  = in32(bsext->pfc_base + bsext->pfc_offset);
        if (signal_voltage == SIGNAL_VOLTAGE_3_3) {
            pocctrl0 |=  bsext->pfc_mask;
        } else {
            pocctrl0 &= ~bsext->pfc_mask;
        }
        // It's not safe to do a read modified write for common registers!
        out32(bsext->pfc_base + bsext->pfc_pmmr, ~pocctrl0);
        out32(bsext->pfc_base + bsext->pfc_offset, pocctrl0);

        if (in32(bsext->pfc_base + bsext->pfc_offset) != pocctrl0) {
            sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "%s: Failed to change signal level %d!", __func__, signal_voltage);
            return (EIO);
        }
    }

    if (bsext->pwr_if_base) {
        // Interface voltage control VDDQVA_SDx
        if (signal_voltage == SIGNAL_VOLTAGE_3_3) {
            out32(bsext->pwr_if_base + bsext->pwr_if_offset, in32(bsext->pwr_if_base + bsext->pwr_if_offset) | (1 << bsext->pwr_if_shift));
        } else {
            out32(bsext->pwr_if_base + bsext->pwr_if_offset, in32(bsext->pwr_if_base + bsext->pwr_if_offset) & ~(1 << bsext->pwr_if_shift));
        }
    } else if (bsext->pwr_cpld_ctrl) {
        /* Not support yet */
    }

    hc->signal_voltage = signal_voltage;

    return (EOK);
}

static int rcar_sdhi_signal_voltage(sdio_hc_t *hc, int signal_voltage)
{
    rcar_sdmmc_t     *sdmmc;
    uint16_t        clkctl;
    int             cnt;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    hc->signal_voltage = signal_voltage;

    if (signal_voltage == SIGNAL_VOLTAGE_3_3)
        return (rcar_bs_signal_voltage(hc, signal_voltage));

    /*
     * Change the electric potential of the bus signal to 1.8V after checking that the SD card supports 1.8 V.
     * following procedure is followed point (2) of section 70.3.1 of document ver.0027
     */

    clkctl = SDH_CLKCTRL_SCLKEN;
    if (sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) & SDH_CLKCTRL_SDCLKOFFEN)
        clkctl |= SDH_CLKCTRL_SDCLKOFFEN;

    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, ~(clkctl) & sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL));

    /* Check to see if DAT0 bit is 0  */
    if (sdmmc_read(sdmmc->vbase, MMC_SD_INFO2) & 0x0080) {
        sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1, "%s:  DAT0 is not LOW", __FUNCTION__);
        sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | clkctl);
        return (EIO);
    }

    rcar_bs_signal_voltage(hc, signal_voltage);

    /* Wait for at least 5ms and this depends on card manufacturer */
    delay(5);

    /* Start SDCLK */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | SDH_CLKCTRL_SCLKEN);
    cnt = 20;

    /* Check to see if DAT0 bit is 1  */
    do {
        delay(1); /* Wait for 1ms */
    } while (!(sdmmc_read(sdmmc->vbase, MMC_SD_INFO2) & 0x0080) && (--cnt != 0));

    if (cnt == 0) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1, "%s:  DAT0 is not HIGH", __FUNCTION__);
        return (EIO);
    }

    if (clkctl & SDH_CLKCTRL_SDCLKOFFEN) {
        sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | SDH_CLKCTRL_SDCLKOFFEN);
    }

    return (EOK);
}

static int rcar_bs_pwr(sdio_hc_t *hc, int vdd)
{
    sdmmc_bs_ext    *bsext = (sdmmc_bs_ext *)hc->bs_hdl;

    if (!(hc->flags & HC_FLAG_DEV_MMC)) {
        if (bsext->pwr_vdd_base) {
            if (vdd != 0) {
                out32(bsext->pwr_vdd_base + bsext->pwr_vdd_offset, in32(bsext->pwr_vdd_base + bsext->pwr_vdd_offset) | (1 << bsext->pwr_vdd_shift));
            }
            else {
                out32(bsext->pwr_vdd_base + bsext->pwr_vdd_offset, in32(bsext->pwr_vdd_base + bsext->pwr_vdd_offset) & ~(1 << bsext->pwr_vdd_shift));
            }
        } else if (bsext->pwr_cpld_ctrl) {
            /* Not support yet */
        }
        delay(10); //need delay to reset and power up stably
    }

    bsext->rcar_pwr(hc, vdd);

    return (EOK);
}

static int my_getsubopt(char **optionp, char * const *tokens, char **valuep)
{
    char        *p, *opt;
    int         len, index;
    const char  *token;

    *valuep = 0;

    for (opt = p = *optionp, len = 0; *p && *p != ':'; p++, len++) {
        if (*p == '=') {
            for (*valuep = ++p; *p && *p != ':'; p++) {
                /* Nothing to do */
            }
            break;
        }
    }
    if (*p) {
        *p++ = '\0';
    }
    *optionp = p;

    for (index = 0; (token = *tokens++); index++) {
        if (*token == *opt && !strncmp(token, opt, len) && token[len] == '\0') {
            return index;
        }
    }
    *valuep = opt;
    return -1;
}

static int rcar_bs_args(sdio_hc_t *hc, char *options)
{
    sdmmc_bs_ext    *bsext = (sdmmc_bs_ext *)hc->bs_hdl;
    char            *value, *cp;
    int             opt;

    static char     *opts[] = {
                        "nowp",      /* no write protected capability */
                        "vdd",       /* vdd pwr base addr, size, vdd offset, vdd shift */
                        "if",        /* if pwr base addr, size, if offset, if shift */
                        "pfc",       /* signal IO base addr, size, pmmr off, poc offset, poc mask, poc shift */
                        "cpld",      /* CPLD control signal voltage and SD/MMC Pin mux */ 
                        NULL
                    };

    bsext->nowp = 0;


    while (options && *options != '\0') {
        if ((opt = my_getsubopt(&options, opts, &value)) == -1) {
            sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0, "%s: invalid BS options %s", __func__, options);
            return EINVAL;
        }
        switch (opt) {
        case 0:
            bsext->nowp = 1;
            break;
        case 1: /* vdd pwr base_addr, size, vdd offset, vdd shift */
            if(value != NULL) {
                bsext->pwr_vdd_pbase = strtoul(value, NULL, 0);
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pwr_vdd_size = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pwr_vdd_offset = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pwr_vdd_shift = strtoul(value, NULL, 0);
                }
            }
            break;
        case 2: /* if pwr base_addr, size, if offset, if shift */
            if(value != NULL) {
                bsext->pwr_if_pbase = strtoul(value, NULL, 0);
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pwr_if_size = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pwr_if_offset = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pwr_if_shift = strtoul(value, NULL, 0);
                }
            }
            break;
        case 3: /* signal IO base_addr, size, pmmr off, pfc offset, pfc mask, pfc shift */
            if(value != NULL) {
                bsext->pfc_pbase = strtoul(value, NULL, 0);
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pfc_size = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pfc_pmmr = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pfc_offset = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pfc_mask = strtoul(value, NULL, 0);
                }
                if ((cp = strchr(value, '/'))) {
                    value = cp+1;
                    bsext->pfc_mask <<= strtoul(value, NULL, 0); /* mask shift */
                }
            }
            break;
        case 4: /* CPLD control signal voltage */
            /* Not support yet */
            bsext->pwr_cpld_ctrl = 0;
            break;
        default:
            break;
        } /* switch (opt) */
    } /* while (options && *options != '\0') */

    return (EOK);
}

static int rcar_bs_dinit(sdio_hc_t *hc)
{
    sdmmc_bs_ext *bsext = (sdmmc_bs_ext *)hc->bs_hdl;

    if (bsext == NULL) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0,
                    "%s: bsext is NULL", __func__);
        return EINVAL;
    }

    if ((bsext->pwr_if_base != (uintptr_t)MAP_FAILED) &&
        (bsext->pwr_if_pbase != bsext->pwr_vdd_pbase)) {
        munmap_device_io(bsext->pwr_if_base, bsext->pwr_if_size);
    }
    if (bsext->pwr_vdd_base != (uintptr_t)MAP_FAILED) {
        munmap_device_io(bsext->pwr_vdd_base, bsext->pwr_vdd_size);
    }
    if (bsext->pfc_base != (uintptr_t)MAP_FAILED) {
        munmap_device_io(bsext->pfc_base, bsext->pfc_size);
    }

    free(bsext);
    hc->bs_hdl = NULL;

    return rcar_sdmmc_dinit(hc);
}

static int rcar_bs_init(sdio_hc_t *hc)
{
    sdmmc_bs_ext    *bsext;
    sdio_hc_cfg_t   *cfg = &hc->cfg;

    if (!(bsext = calloc(1, sizeof(sdmmc_bs_ext)))) {
        return (ENOMEM);
    }

    hc->bs_hdl = (void *)bsext;

    bsext->pwr_vdd_pbase = 0;
    bsext->pwr_if_pbase = 0;
    bsext->pfc_pbase = 0;

    query_hwinfo(bsext, cfg->idx);
    if (rcar_bs_args(hc, cfg->options)) {
        return EINVAL;
    }

    if (bsext->pfc_pbase) {
        bsext->pfc_base = mmap_device_io(bsext->pfc_size, bsext->pfc_pbase);
        if (bsext->pfc_base == (uintptr_t)MAP_FAILED) {
            sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0,
                        "%s: mmap_device_io(0x%08x) failed (errno %d)",
                        __func__, bsext->pfc_pbase, errno);
            goto fail;
        }
    }

    if (hc->flags & HC_FLAG_DEV_MMC) {
        /* emmc */
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "%s: EMMC", __FUNCTION__);

        hc->caps |= (uint64_t)(HC_CAP_SV_1_8V);
        hc->caps |= (uint64_t)(HC_CAP_XPC_1_8V);
        hc->ocr   = OCR_VDD_32_33 | OCR_VDD_33_34;

        // Fix IO signal voltage level to 1.8V
        rcar_bs_signal_voltage(hc, SIGNAL_VOLTAGE_1_8);

        if (rcar_sdmmc_init(hc) != EOK) {
            sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0,
                        "%s: rcar_sdmmc_init() failed", __func__);
            goto fail1;
        }
    } else {
        /* sd */
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "%s: SD", __FUNCTION__);
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "%s: vdd pwr base=0x%x vdd offset=0x%x vdd shift=%d", __FUNCTION__, bsext->pwr_vdd_pbase, bsext->pwr_vdd_offset, bsext->pwr_vdd_shift);
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "%s: if pwr base=0x%x if offset=0x%x if shift=%d", __FUNCTION__, bsext->pwr_if_pbase, bsext->pwr_if_offset, bsext->pwr_if_shift);
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "%s: pfc base=0x%x offset=0x%x pmmr=0x%x mask=0x%x", __FUNCTION__, bsext->pfc_pbase, bsext->pfc_offset, bsext->pfc_pmmr, bsext->pfc_mask);

        if (bsext->pwr_vdd_pbase) {
            bsext->pwr_vdd_base = mmap_device_io(bsext->pwr_vdd_size, bsext->pwr_vdd_pbase);
            if (bsext->pwr_vdd_base == (uintptr_t)MAP_FAILED) {
                sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0,
                        "%s: mmap_device_io(0x%08x) failed (errno %d)",
                        __func__, bsext->pwr_vdd_pbase, errno);
                goto fail1;
            }
        }

        if (bsext->pwr_if_pbase) {
            if (bsext->pwr_if_pbase == bsext->pwr_vdd_pbase) {
                bsext->pwr_if_base = bsext->pwr_vdd_base;
            } else {
                bsext->pwr_if_base = mmap_device_io(bsext->pwr_if_size, bsext->pwr_if_pbase);
                if (bsext->pwr_if_base == (uintptr_t)MAP_FAILED) {
                sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0,
                        "%s: mmap_device_io(0x%08x) failed (errno %d)",
                        __func__, bsext->pwr_if_pbase, errno);
                    goto fail2;
                }
            }
        }

        // Initial IO signal voltage level to 3.3V
        rcar_bs_signal_voltage(hc, SIGNAL_VOLTAGE_3_3);

        hc->caps |= (uint64_t)(HC_CAP_SV_3_3V);
        hc->caps |= (uint64_t)(HC_CAP_XPC_3_3V);
        if (bsext->pwr_if_pbase || bsext->pwr_cpld_ctrl) {
            hc->caps |= (uint64_t)(HC_CAP_SV_1_8V);
            hc->caps |= (uint64_t)(HC_CAP_XPC_1_8V);
        }
        hc->ocr   = OCR_VDD_17_195 | OCR_VDD_32_33 | OCR_VDD_33_34;

        if (rcar_sdmmc_init(hc) != EOK) {
            sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0,
                        "%s: rcar_sdmmc_init() failed", __func__);
            goto fail3;
        }

        hc->entry.signal_voltage = rcar_sdhi_signal_voltage;
        hc->entry.dinit          = rcar_bs_dinit;
    }

    hc->entry.cd    = rcar_bs_cd;
    bsext->rcar_pwr = hc->entry.pwr;
    hc->entry.pwr   = rcar_bs_pwr;

    return EOK;

fail3:
    if ((bsext->pwr_if_pbase != bsext->pwr_vdd_pbase) &&
        (bsext->pwr_if_base != (uintptr_t)MAP_FAILED)) {
        munmap_device_io(bsext->pwr_if_base, bsext->pwr_if_size);
    }
fail2:
    if (bsext->pwr_vdd_base != (uintptr_t)MAP_FAILED) {
        munmap_device_io(bsext->pwr_vdd_base, bsext->pwr_vdd_size);
    }
fail1:
    if (bsext->pfc_base != (uintptr_t)MAP_FAILED) {
        munmap_device_io(bsext->pfc_base, bsext->pfc_size);
    }
fail:
    if (bsext != NULL) {
        free(bsext);
    }

    return (ENODEV);
}

sdio_product_t sdio_fs_products[] = {
    { .did = SDIO_DEVICE_ID_WILDCARD, .class = 0, .aflags = 0, .name = "rcar", .init = rcar_bs_init },
};

sdio_vendor_t sdio_vendors[] = {
    { .vid = SDIO_VENDOR_ID_WILDCARD, .name = "Renesas", .chipsets = sdio_fs_products },
    { .vid = 0, .name = NULL, .chipsets = NULL }
};

