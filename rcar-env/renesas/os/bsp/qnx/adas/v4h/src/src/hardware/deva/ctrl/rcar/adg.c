/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#include "adg.h"
#include <errno.h>
#include <stdint.h>
#include <audio_driver.h>
#include "mem_map.h"
#include <drvr/hwinfo.h>
#include <aarch64/r-car.h>
#include <hw/hwinfo_rcar.h>

/* The audio clock generator (ADG) block supplies the necessary clocks for the
 * operation of the SSI module
 */

/* Audio clock config for SSI  */
#define AUDIO_CLK_SEL_DIVSELSSI_MASK               (0x19 << 3)
#define AUDIO_CLK_SEL_DIVSELSSI_DIV1               (0x00 << 3)
#define AUDIO_CLK_SEL_DIVSELSSI_DIV2               (0x01 << 3)
#define AUDIO_CLK_SEL_DIVSELSSI_DIV4               (0x08 << 3) 
#define AUDIO_CLK_SEL_DIVSELSSI_DIV8               (0x09 << 3)
#define AUDIO_CLK_SEL_DIVSELSSI_DIV16              (0x10 << 3)
#define AUDIO_CLK_SEL_DIVSELSSI_DIV32              (0x11 << 3)

#define AUDIO_CLK_SEL_ACLKSELSSI_MASK              (0x3 << 4)
#define AUDIO_CLK_SEL_ACLKSELSSI_DIVCLK            (0x0 << 4)
#define AUDIO_CLK_SEL_ACLKSELSSI_BRGA              (0x1 << 4)
#define AUDIO_CLK_SEL_ACLKSELSSI_BRGB              (0x2 << 4)

#define AUDIO_CLK_SEL_DIVCLKSELSSI_MASK            (0x7 << 0)
#define AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKIN     (0x1 << 0)

typedef struct {
    volatile uint32_t brra;
    volatile uint32_t brrb;
    volatile uint32_t brgckr;
    volatile uint32_t audio_clk_sel0;
} adg_hw_reg_t;

/* the ADG register map is stored as a static global variable and
 * shared across multiple rcar deva instances if more than one mounted;
 * the only thing that is important is to not map the memory multiple
 * times, and to unmap it only when the last rcar deva instance is
 * unmounted; this is checked using the adg_ref_count, also a static
 * global variable, that counts how many driver instances are mounted;
 * additionally the ADG registers can be accessed from multiple driver
 * instances, as they typically have bit fields that refer to clock source
 * use a mutex (also a static global variable) to control concurrent access 
 * to these registers and to adg_ref_count
 */
static unsigned long adg_base = 0;
static uint32_t adg_len = 0;
static adg_hw_reg_t *adg = MAP_FAILED;
static ado_mutex_t adg_lock = PTHREAD_MUTEX_INITIALIZER;
static uint32_t adg_ref_count = 0;

int adg_init(void)
{
    ado_mutex_lock (&adg_lock);

    /* at the first adg_init call populate base and len */
    if (!adg_base) {
        unsigned hwi_off = hwi_find_device(RCAR_HWI_AUDIO_ADG, 0);

        if(hwi_off != HWI_NULL_OFF) {
            hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
            if (tag) {
                adg_base = tag->location.base;
                adg_len = tag->location.len;
            }
        }
        if (!adg_base) {
            adg_base = RCAR_ADG_BASE;
            adg_len = RCAR_ADG_SIZE;
        }
    }

    if (adg_ref_count == 0) {
        /* We can pass NULL for the card pointer if we do not need to register the mapping with SMMU */
        adg = (adg_hw_reg_t *)ADO_DEVICE_MMAP (NULL, adg_base, adg_len);
        if (adg == MAP_FAILED ) {
            ado_mutex_unlock (&adg_lock);
            ado_error_fmt ("ADG register map failed");
            return ENOMEM;
        }
    }

    adg_ref_count++;

    ado_mutex_unlock (&adg_lock);

    return EOK;
}

void adg_deinit(void)
{
    ado_mutex_lock (&adg_lock);

    if (adg_ref_count > 0) {
        adg_ref_count--;
    }

    if (adg_ref_count == 0) {
        if (adg != MAP_FAILED) {
           ADO_DEVICE_MUNMAP(NULL, adg, adg_len);
           adg = MAP_FAILED;
        }
    }

    ado_mutex_unlock (&adg_lock);
}

int adg_set_ssi_clksrc(uint32_t clk_src) {

    volatile uint32_t* audio_clk_sel;
    uint32_t audio_clk_sel_val;
    int ret = EOK;

    audio_clk_sel = &adg->audio_clk_sel0;
    audio_clk_sel_val = *audio_clk_sel;

    ado_mutex_lock(&adg_lock);

    audio_clk_sel_val &= ~(AUDIO_CLK_SEL_DIVCLKSELSSI_MASK |
                        AUDIO_CLK_SEL_ACLKSELSSI_MASK);
    
    /* Use the divided clock of one of AUDIO_CLKIN
     * as the clock source for the SSI */
    switch (clk_src) {
    case AUDIO_CLKA:
        audio_clk_sel_val |= AUDIO_CLK_SEL_ACLKSELSSI_BRGA | AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKIN;
        break;
    case AUDIO_CLKB:
        audio_clk_sel_val |= AUDIO_CLK_SEL_ACLKSELSSI_BRGB | AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKIN;
        break;
    default:
        audio_clk_sel_val |= AUDIO_CLK_SEL_ACLKSELSSI_DIVCLK | AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKIN;
        break;
    }
    *audio_clk_sel = audio_clk_sel_val;

    ado_mutex_unlock(&adg_lock);

    return ret;
}

int adg_set_ssi_clkdiv(uint32_t divisor, uint32_t clk_src)
{
    volatile uint32_t* audio_clk_sel;
    uint32_t audio_clk_sel_val;
    int ret = EOK;

    ado_debug (DB_LVL_DRIVER, "Setting SSI divisor to %u", divisor);

    ado_mutex_lock (&adg_lock);

    switch (clk_src) {
    case AUDIO_CLKA:
        audio_clk_sel = &adg->brra;
        *audio_clk_sel = divisor / 2;
        break;
    case AUDIO_CLKB:
        audio_clk_sel = &adg->brrb;
        *audio_clk_sel = divisor / 2;
        break;
    default:
        audio_clk_sel = &adg->audio_clk_sel0;
        audio_clk_sel_val = *audio_clk_sel;
        audio_clk_sel_val &= ~AUDIO_CLK_SEL_DIVSELSSI_MASK;
        switch (divisor) {
            case 1:
                audio_clk_sel_val |= AUDIO_CLK_SEL_DIVSELSSI_DIV1;
                break;

            case 2:
                audio_clk_sel_val |= AUDIO_CLK_SEL_DIVSELSSI_DIV2;
                break;

            case 4:
                audio_clk_sel_val |= AUDIO_CLK_SEL_DIVSELSSI_DIV4;
                break;

            case 8:
                audio_clk_sel_val |= AUDIO_CLK_SEL_DIVSELSSI_DIV8;
                break;

            case 16:
                audio_clk_sel_val |= AUDIO_CLK_SEL_DIVSELSSI_DIV16;
                break;

            case 32:
                audio_clk_sel_val |= AUDIO_CLK_SEL_DIVSELSSI_DIV32;
                break;

            default:
                ado_error_fmt ("Invalid divisor %d", divisor);
                ret = EINVAL;
                break;
        }
        *audio_clk_sel = audio_clk_sel_val;
        break;
    }

    ado_mutex_unlock (&adg_lock);

    return ret;
}

void adg_register_dump()
{
    if( adg == MAP_FAILED ) {
        ado_error_fmt ("memory not mapped");
        return;
    }

    ado_debug( DB_LVL_DRIVER, "ADG reg dump: BRRA=0x%x BRRB=0x%x BRGCKR=0x%x",
               adg->brra, adg->brrb, adg->brgckr );
    ado_debug( DB_LVL_DRIVER, "ADG reg dump: AUDIO_CLK_SEL0=0x%x", adg->audio_clk_sel0 );
}
