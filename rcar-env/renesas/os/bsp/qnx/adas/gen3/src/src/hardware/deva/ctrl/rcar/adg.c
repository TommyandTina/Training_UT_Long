/*
 * $QNXLicenseC:
 * Copyright 2015, 2016 QNX Software Systems.
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

#include "adg.h"
#include <errno.h>
#include <stdint.h>
#include <audio_driver.h>
#include "mem_map.h"
#include "rcar_support.h"

/* The audio clock generator (ADG) block supplies the necessary clocks for the
 * operation of the SSI, SCU and/or DTCP modules
 */

/* audio clock config for SSI 0 to 9: AUDIO_CLK_SEL0 register for SSI 0 to 3,
   AUDIO_CLK_SEL1 register for SSI 4 to 7,  AUDIO_CLK_SEL2 register for SSI 8 to 9 */
#define AUDIO_CLK_SEL_DIVSELSSI_MASK( ssi )          ( 0x19 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_DIV1( ssi )          0
#define AUDIO_CLK_SEL_DIVSELSSI_DIV2( ssi )          ( 0x1 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_DIV4( ssi )          ( 0x8 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_DIV8( ssi )          ( 0x9 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_DIV16( ssi )         ( 0x10 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_DIV32( ssi )         ( 0x11 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
/* AUDIO_CLK_SEL_DIVSELSSI_AVB_DIVn masks only applicable to Rcar-H3, used when bitmask
   AUDIO_CLK_SEL_ACLKSELSSI_MASK set to AUDIO_CLK_SEL_ACLKSELSSI_AVB */
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV0( ssi )      0
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV1( ssi )      ( 0x1 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV2( ssi )      ( 0x8 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV3( ssi )      ( 0x9 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV4( ssi )      ( 0x10 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV5( ssi )      ( 0x11 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV6( ssi )      ( 0x18 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )
#define AUDIO_CLK_SEL_DIVSELSSI_AVB_DIV7( ssi )      ( 0x19 << ( ( ( ssi & 0x3 ) << 3 ) + 3 ) )

#define AUDIO_CLK_SEL_ACLKSELSSI_MASK( ssi )         ( 0x3 << ( ( ( ssi & 0x3 ) << 3 ) + 4 ) )
#define AUDIO_CLK_SEL_ACLKSELSSI_DIVCLK( ssi )       0
#define AUDIO_CLK_SEL_ACLKSELSSI_BRGA( ssi )         ( 0x1 << ( ( ( ssi & 0x3 ) << 3 ) + 4 ) )
#define AUDIO_CLK_SEL_ACLKSELSSI_BRGB( ssi )         ( 0x2 << ( ( ( ssi & 0x3 ) << 3 ) + 4 ) )
/* AUDIO_CLK_SEL_ACLKSELSSI_AVB only applicable to Rcar-H3 */
#define AUDIO_CLK_SEL_ACLKSELSSI_AVB( ssi )          ( 0x3 << ( ( ( ssi & 0x3 ) << 3 ) + 4 ) )

#define AUDIO_CLK_SEL_DIVCLKSELSSI_MASK( ssi )       ( 0x7 << ( ( ssi & 0x3 ) << 3 ) )
#define AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKA( ssi ) ( 0x1 << ( ( ssi & 0x3 ) << 3 ) )
#define AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKB( ssi ) ( 0x2 << ( ( ssi & 0x3 ) << 3 ) )
#define AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKC( ssi ) ( 0x3 << ( ( ssi & 0x3 ) << 3 ) )
#define AUDIO_CLK_SEL_DIVCLKSELSSI_MLBCLK( ssi )     ( 0x4 << ( ( ssi & 0x3 ) << 3 ) )

/* SRC clock selection and div for SRC 0 to 9:
   SRCIN_TIMSEL0 for SRC 0,1, SRCIN_TIMSEL1 for SRC 2,3, SRCIN_TIMSEL2 for SRC 4,5,
   SRCIN_TIMSEL3 for SRC 6,7, SRCIN_TIMSEL4 for SRC 8,9
   SRCOUT_TIMSEL0 for SRC 0,1, SRCOUT_TIMSEL1 for SRC 2,3, SRCOUT_TIMSEL2 for SRC 4,5,
   SRCOUT_TIMSEL3 for SRC 6,7, SRCOUT_TIMSEL4 for SRC 8,9 */
#define SRC_DIVCLK_SEL_MASK( src )               ( 0x1F << ( ( ( src & 0x1 ) << 4 ) + 8 ) )
#define SRC_DIVCLK_SEL_AUDIO_CLKA( src )         0
#define SRC_DIVCLK_SEL_AUDIO_CLKB( src )         ( 0x1 << ( ( ( src & 0x1 ) << 4 ) + 8 ) )
#define SRC_DIVCLK_SEL_AUDIO_CLKC( src )         ( 0x2 << ( ( ( src & 0x1 ) << 4 ) + 8 ) )
#define SRC_DIVCLK_SEL_BRGA( src )               ( 0x3 << ( ( ( src & 0x1 ) << 4 ) + 8 ) )
#define SRC_DIVCLK_SEL_BRGB( src )               ( 0x4 << ( ( ( src & 0x1 ) << 4 ) + 8 ) )
#define SRC_DIVCLK_SEL_MLB( src )                ( 0x5 << ( ( ( src & 0x1 ) << 4 ) + 8 ) )
#define SRC_DIVCLK_SEL_SSI( src, ssi )           ( ( 0x6 + ssi ) << ( ( ( src & 0x1 ) << 4 ) + 8 ) )
#define SRC_DIVRATIO_SEL_MASK( src )             ( 0x1F << ( ( src & 0x1 ) << 4 ) )
#define SRC_DIVRATIO_SEL_DIV( src, div_idx )     ( div_idx << ( ( src & 0x1 ) << 4 ) )

/* CMD clock selection and div for CMD 0 to 1: CMDOUT_TIMSEL */
#define CMDOUT_DIVCLK_SEL_MASK( cmd )               ( 0x1F << ( ( ( cmd & 0x1 ) << 4 ) + 8 ) )
#define CMDOUT_DIVCLK_SEL_AUDIO_CLKA( cmd )         0
#define CMDOUT_DIVCLK_SEL_AUDIO_CLKB( cmd )         ( 0x1 << ( ( ( cmd & 0x1 ) << 4 ) + 8 ) )
#define CMDOUT_DIVCLK_SEL_AUDIO_CLKC( cmd )         ( 0x2 << ( ( ( cmd & 0x1 ) << 4 ) + 8 ) )
#define CMDOUT_DIVCLK_SEL_BRGA( cmd )               ( 0x3 << ( ( ( cmd & 0x1 ) << 4 ) + 8 ) )
#define CMDOUT_DIVCLK_SEL_BRGB( cmd )               ( 0x4 << ( ( ( cmd & 0x1 ) << 4 ) + 8 ) )
#define CMDOUT_DIVCLK_SEL_MLB( cmd )                ( 0x5 << ( ( ( cmd & 0x1 ) << 4 ) + 8 ) )
#define CMDOUT_DIVCLK_SEL_SSI( cmd, ssi )           ( ( 0x6 + ssi ) << ( ( ( cmd & 0x1 ) << 4 ) + 8 ) )
#define CMDOUT_DIVRATIO_SEL_MASK( cmd )             ( 0x1F << ( ( cmd & 0x1 ) << 4 ) )
#define CMDOUT_DIVRATIO_SEL_DIV( cmd, div_idx )     ( div_idx << ( ( cmd & 0x1 ) << 4 ) )

typedef struct {
    volatile uint32_t brra;            // aka bbra in Rcar-H2
    volatile uint32_t brrb;            // aka bbrb in Rcar-H2
    volatile uint32_t brgckr;          // aka ssickr in Rcar-H2
    volatile uint32_t audio_clk_sel0;
    volatile uint32_t audio_clk_sel1;
    volatile uint32_t audio_clk_sel2;
    volatile uint32_t dummy1[6];
    volatile uint32_t tim_en;          // aka div_en in Rcar-H2
    volatile uint32_t srcin_timsel0;
    volatile uint32_t srcin_timsel1;
    volatile uint32_t srcin_timsel2;
    volatile uint32_t srcin_timsel3;
    volatile uint32_t srcin_timsel4;
    volatile uint32_t srcout_timsel0;
    volatile uint32_t srcout_timsel1;
    volatile uint32_t srcout_timsel2;
    volatile uint32_t srcout_timsel3;
    volatile uint32_t srcout_timsel4;
    volatile uint32_t cmdout_timsel;
    volatile uint32_t adsp_timsel;     // new in Rcar-H3
    volatile uint32_t dtcp_timsel;
    volatile uint32_t dummy2[6];
    volatile uint32_t adsp_timsel1;    // new in Rcar-H3
    volatile uint32_t adsp_timsel2;    // new in Rcar-H3
    volatile uint32_t adsp_timsel3;    // new in Rcar-H3
    volatile uint32_t dummy3[29];
    volatile uint32_t avbckr;          // new in Rcar-H3
    volatile uint32_t avb_sync_sel0;   // new in Rcar-H3
    volatile uint32_t dummy4;
    volatile uint32_t avb_sync_sel1;   // new in Rcar-H3
    volatile uint32_t avb_sync_sel2;   // new in Rcar-H3
    volatile uint32_t avb_sync_div0;   // new in Rcar-H3
    volatile uint32_t avb_sync_div1;   // new in Rcar-H3
    volatile uint32_t avb_clk_div0;    // new in Rcar-H3
    volatile uint32_t avb_clk_div1;    // new in Rcar-H3
    volatile uint32_t avb_clk_div2;    // new in Rcar-H3
    volatile uint32_t avb_clk_div3;    // new in Rcar-H3
    volatile uint32_t avb_clk_div4;    // new in Rcar-H3
    volatile uint32_t avb_clk_div5;    // new in Rcar-H3
    volatile uint32_t avb_clk_div6;    // new in Rcar-H3
    volatile uint32_t avb_clk_div7;    // new in Rcar-H3
    volatile uint32_t avb_clk_config;  // new in Rcar-H3
    volatile uint32_t avb_clk_status;  // new in Rcar-H3
} adg_hw_reg_t;

/* the ADG register map is stored as a static global variable and
 * shared across multiple rcar deva instances if more than one mounted;
 * the only thing that is important is to not map the memory multiple
 * times, and to unmap it only when the last rcar deva instance is
 * unmounted; this is checked using the adg_ref_count, also a static
 * global variable, that counts how many driver instances are mounted;
 * additionally the ADG registers can be accessed from multiple driver
 * instances, as they typically have bit fields that refer to clock source
 * or clock settings for multiple SSI instances; use a mutex (also a static
 * global variable) to control concurrent access to these registers and
 * to adg_ref_count
 */
static unsigned long adg_base = 0;
static uint32_t adg_len = 0;
static adg_hw_reg_t *adg = MAP_FAILED;
static ado_mutex_t adg_lock = PTHREAD_MUTEX_INITIALIZER;
static uint32_t adg_ref_count = 0;

uint32_t scu_divratio_tbl[30] =
    { 2,4,6,8,12,16,24,32,48,64,96,128,192,256,384,512,768,1024,
      1536,2048,3072,4096,6144,8192,12288,16384,24576,32768,49152,98304 };

int adg_init()
{
    ado_mutex_lock (&adg_lock);

    /* at the first adg_init call populate base and len */
    if (!adg_base || !adg_len) {
#if _NTO_VERSION >= 700
        /* the hwinfo query works for all gen3 variants, if it fails, then
         * this is a gen2 variant, use the RCAR_GEN2_ADG_BASE as base and
         * as len the offset in adg_hw_reg_t of the first gen3 addition in
         * terms of ADG registers
         */
        if (rcar_query_hwinfo(RCAR_HWI_AUDIO_ADG, &adg_base, &adg_len) != EOK) {
            adg_base = RCAR_GEN2_ADG_BASE;
            adg_len = offsetof(adg_hw_reg_t, dummy2);
        }
#else
        adg_base = RCAR_ADG_BASE;
        adg_len = offsetof(adg_hw_reg_t, dummy2);
#endif
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

void adg_deinit()
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

int adg_set_ssi_clksrc(uint32_t ssi_channel, uint32_t clk) {

    volatile uint32_t* audio_clk_sel;
    int ret = EOK;

    ado_debug (DB_LVL_DRIVER, "Setting SSI%d Clock Select to %u", ssi_channel, clk);

    switch( ssi_channel ) {
        case 0:
        case 1:
        case 2:
        case 3:
            audio_clk_sel = &adg->audio_clk_sel0;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            audio_clk_sel = &adg->audio_clk_sel1;
            break;
        case 8:
        default:
            ado_error_fmt ("Invalid ssi_channel %d", ssi_channel);
            return EINVAL;
        case 9:
            audio_clk_sel = &adg->audio_clk_sel2;
            break;
    }

    ado_mutex_lock(&adg_lock);

    *audio_clk_sel &= ~(AUDIO_CLK_SEL_DIVCLKSELSSI_MASK( ssi_channel ) |
                        AUDIO_CLK_SEL_ACLKSELSSI_MASK( ssi_channel ));
    /* use the divided clock of one of AUDIO_CLKA, AUDIO_CLKB, AUDIO_CLKC
     * as the clock source for the SSI channels */
    *audio_clk_sel |= AUDIO_CLK_SEL_ACLKSELSSI_DIVCLK( ssi_channel );

    switch( clk ) {
        case AUDIO_CLKA:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKA(ssi_channel);
            break;
        case AUDIO_CLKB:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKB(ssi_channel);
            break;
        case AUDIO_CLKC:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVCLKSELSSI_AUDIO_CLKC(ssi_channel);
            break;
        default:
            ado_error_fmt ("Invalid clock source %d", clk);
            ret = EINVAL;
            break;
    }

    ado_mutex_unlock(&adg_lock);

    return ret;
}

int adg_set_src_clksrc(uint32_t src_channel, uint32_t clk, uint32_t playback)
{
    volatile uint32_t* src_timsel;
    int ret = EOK;

    switch ( src_channel ) {
        case 0:
        case 1:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel0;
            } else {
                src_timsel = &adg->srcin_timsel0;
            }
            break;
        case 2:
        case 3:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel1;
            } else {
                src_timsel = &adg->srcin_timsel1;
            }
            break;
        case 4:
        case 5:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel2;
            } else {
                src_timsel = &adg->srcin_timsel2;
            }
            break;
        case 6:
        case 7:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel3;
            } else {
                src_timsel = &adg->srcin_timsel3;
            }
            break;
        case 8:
        case 9:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel4;
            } else {
                src_timsel = &adg->srcin_timsel4;
            }
            break;
        default:
            return EINVAL;
    }

    ado_mutex_lock (&adg_lock);

    *src_timsel &= ~SRC_DIVCLK_SEL_MASK( src_channel );
    switch( clk ) {
        case AUDIO_CLKA:
            *src_timsel |= SRC_DIVCLK_SEL_AUDIO_CLKA (src_channel);
            break;
        case AUDIO_CLKB:
            *src_timsel |= SRC_DIVCLK_SEL_AUDIO_CLKB (src_channel);
            break;
        case AUDIO_CLKC:
            *src_timsel |= SRC_DIVCLK_SEL_AUDIO_CLKC (src_channel);
            break;
        default:
            ado_error_fmt ("Invalid clock source %d", clk);
            ret = EINVAL;
            break;
    }

    ado_mutex_unlock (&adg_lock);

    return ret;
}

int adg_set_src_clkdiv(uint32_t src_channel, uint32_t divisor, uint32_t playback)
{
    volatile uint32_t* src_timsel;
    uint32_t div_idx;

    switch ( src_channel ) {
        case 0:
        case 1:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel0;
            } else {
                src_timsel = &adg->srcin_timsel0;
            }
            break;
        case 2:
        case 3:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel1;
            } else {
                src_timsel = &adg->srcin_timsel1;
            }
            break;
        case 4:
        case 5:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel2;
            } else {
                src_timsel = &adg->srcin_timsel2;
            }
            break;
        case 6:
        case 7:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel3;
            } else {
                src_timsel = &adg->srcin_timsel3;
            }
            break;
        case 8:
        case 9:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel4;
            } else {
                src_timsel = &adg->srcin_timsel4;
            }
            break;
        default:
            return EINVAL;
    }

    for (div_idx = 0; div_idx < sizeof(scu_divratio_tbl)/sizeof(scu_divratio_tbl[0]); div_idx++) {
        if (scu_divratio_tbl[div_idx] == divisor) break;
    }

    if (div_idx == sizeof(scu_divratio_tbl)/sizeof(scu_divratio_tbl[0])) {
        return EINVAL;
    }

    ado_mutex_lock (&adg_lock);

    *src_timsel &= ~SRC_DIVRATIO_SEL_MASK( src_channel );
    *src_timsel |= SRC_DIVRATIO_SEL_DIV( src_channel, div_idx );

    ado_mutex_unlock (&adg_lock);

    return EOK;
}

/* use the WS signal of an SSI as SRC clock source */
/* Note: when the WS of an SSI is used as SRC clock source, it can't be divided */
int adg_set_src_clksrc_ssiws(uint32_t src_channel, uint32_t ssi_channel, uint32_t playback)
{
    volatile uint32_t* src_timsel;

    switch ( src_channel ) {
        case 0:
        case 1:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel0;
            } else {
                src_timsel = &adg->srcin_timsel0;
            }
            break;
        case 2:
        case 3:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel1;
            } else {
                src_timsel = &adg->srcin_timsel1;
            }
            break;
        case 4:
        case 5:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel2;
            } else {
                src_timsel = &adg->srcin_timsel2;
            }
            break;
        case 6:
        case 7:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel3;
            } else {
                src_timsel = &adg->srcin_timsel3;
            }
            break;
        case 8:
        case 9:
            if ( playback ) {
                src_timsel = &adg->srcout_timsel4;
            } else {
                src_timsel = &adg->srcin_timsel4;
            }
            break;
        default:
            return EINVAL;
    }

    ado_mutex_lock (&adg_lock);

    *src_timsel &= ~SRC_DIVCLK_SEL_MASK( src_channel );
    *src_timsel |= SRC_DIVCLK_SEL_SSI( src_channel, ssi_channel );

    ado_mutex_unlock (&adg_lock);

    return EOK;
}

int adg_set_cmd_clksrc(uint32_t cmd_channel, uint32_t clk)
{
    volatile uint32_t* cmd_timsel = &adg->cmdout_timsel;
    int ret = EOK;

    ado_mutex_lock (&adg_lock);

    *cmd_timsel &= ~CMDOUT_DIVCLK_SEL_MASK( cmd_channel );
    switch( clk ) {
        case AUDIO_CLKA:
            *cmd_timsel |= CMDOUT_DIVCLK_SEL_AUDIO_CLKA (cmd_channel);
            break;
        case AUDIO_CLKB:
            *cmd_timsel |= CMDOUT_DIVCLK_SEL_AUDIO_CLKB (cmd_channel);
            break;
        case AUDIO_CLKC:
            *cmd_timsel |= CMDOUT_DIVCLK_SEL_AUDIO_CLKC (cmd_channel);
            break;
        default:
            ado_error_fmt ("Invalid clock source %d", clk);
            ret = EINVAL;
            break;
    }

    ado_mutex_unlock (&adg_lock);

    return ret;
}

int adg_set_cmd_clkdiv(uint32_t cmd_channel, uint32_t divisor)
{
    volatile uint32_t* cmd_timsel = &adg->cmdout_timsel;
    uint32_t div_idx;

    for (div_idx = 0; div_idx < sizeof(scu_divratio_tbl)/sizeof(scu_divratio_tbl[0]); div_idx++) {
        if (scu_divratio_tbl[div_idx] == divisor) break;
    }

    if (div_idx == sizeof(scu_divratio_tbl)/sizeof(scu_divratio_tbl[0])) {
        return EINVAL;
    }

    ado_mutex_lock (&adg_lock);

    *cmd_timsel &= ~CMDOUT_DIVRATIO_SEL_MASK( cmd_channel );
    *cmd_timsel |= CMDOUT_DIVRATIO_SEL_DIV( cmd_channel, div_idx );

    ado_mutex_unlock (&adg_lock);

    return EOK;
}

/* use the WS signal of an SSI as CMD clock source */
/* Note: when the WS of an SSI is used as CMD clock source, it can't be divided */
int adg_set_cmd_clksrc_ssiws(uint32_t cmd_channel, uint32_t ssi_channel)
{
    volatile uint32_t* cmd_timsel = &adg->cmdout_timsel;

    ado_mutex_lock (&adg_lock);

    *cmd_timsel &= ~CMDOUT_DIVCLK_SEL_MASK( cmd_channel );
    *cmd_timsel |= CMDOUT_DIVCLK_SEL_SSI( cmd_channel, ssi_channel );

    ado_mutex_unlock (&adg_lock);

    return EOK;
}

int adg_set_ssi_clkdiv(uint32_t ssi_channel, uint32_t divisor)
{
    volatile uint32_t* audio_clk_sel;
    int ret = EOK;

    ado_debug (DB_LVL_DRIVER, "Setting SSI%d divisor to %u", ssi_channel, divisor);

    switch( ssi_channel ) {
        case 0:
        case 1:
        case 2:
        case 3:
            audio_clk_sel = &adg->audio_clk_sel0;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            audio_clk_sel = &adg->audio_clk_sel1;
            break;
        case 8:
        default:
            ado_error_fmt ("Invalid ssi_channel %d", ssi_channel);
            return EINVAL;
        case 9:
            audio_clk_sel = &adg->audio_clk_sel2;
            break;
    }

    ado_mutex_lock (&adg_lock);

    *audio_clk_sel &= ~AUDIO_CLK_SEL_DIVSELSSI_MASK( ssi_channel );

    switch (divisor) {
        case 1:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVSELSSI_DIV1( ssi_channel );
            break;

        case 2:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVSELSSI_DIV2( ssi_channel );
            break;

        case 4:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVSELSSI_DIV4( ssi_channel );
            break;

        case 8:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVSELSSI_DIV8( ssi_channel );
            break;

        case 16:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVSELSSI_DIV16( ssi_channel );
            break;

        case 32:
            *audio_clk_sel |= AUDIO_CLK_SEL_DIVSELSSI_DIV32( ssi_channel );
            break;

        default:
            ado_error_fmt ("Invalid divisor %d", divisor);
            ret = EINVAL;
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

    ado_debug( DB_LVL_DRIVER, "ADG reg dump: BRRA=0x%x BRRB=0x%x BRGCKR=0x%x TIM_EN=0x%x",
               adg->brra, adg->brrb, adg->brgckr, adg->tim_en );
    ado_debug( DB_LVL_DRIVER, "ADG reg dump: AUDIO_CLK_SEL[0:2]=0x%x 0x%x 0x%x",
               adg->audio_clk_sel0, adg->audio_clk_sel1, adg->audio_clk_sel2 );
    ado_debug( DB_LVL_DRIVER, "ADG reg dump: SRCIN_TIMSEL[0:4]=0x%x 0x%x 0x%x 0x%x 0x%x",
               adg->srcin_timsel0, adg->srcin_timsel1, adg->srcin_timsel2,
               adg->srcin_timsel3, adg->srcin_timsel4 );
    ado_debug( DB_LVL_DRIVER, "ADG reg dump: SRCOUT_TIMSEL[0:4]=0x%x 0x%x 0x%x 0x%x 0x%x",
               adg->srcout_timsel0, adg->srcout_timsel1, adg->srcout_timsel2,
               adg->srcout_timsel3, adg->srcout_timsel4 );
    ado_debug( DB_LVL_DRIVER, "ADG reg dump: CMDOUT_TIMSEL=0x%x DTCP_TIMSEL=0x%x",
               adg->cmdout_timsel, adg->dtcp_timsel );

    if ( rcar_adg_ext_reg_supported() ) {
        ado_debug( DB_LVL_DRIVER, "ADG reg dump: ADSP_TIMSEL=0x%x ADSP_TIMSEL[1:3]=0x%x 0x%x 0x%x",
                   adg->adsp_timsel, adg->adsp_timsel1, adg->adsp_timsel2, adg->adsp_timsel3 );
        ado_debug( DB_LVL_DRIVER, "ADG reg dump: AVBCKR=0x%x AVB_SYNC_SEL[0:2]=0x%x 0x%x 0x%x AVB_SYNC_DIV[0:1]=0x%x 0x%x",
                   adg->avbckr, adg->avb_sync_sel0, adg->avb_sync_sel1, adg->avb_sync_sel2,
                   adg->avb_sync_div0, adg->avb_sync_div1 );
        ado_debug( DB_LVL_DRIVER, "ADG reg dump: AVB_CLK_DIV[0:7]=0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
                   adg->avb_clk_div0, adg->avb_clk_div1, adg->avb_clk_div2, adg->avb_clk_div3,
                   adg->avb_clk_div4, adg->avb_clk_div5, adg->avb_clk_div6, adg->avb_clk_div7 );
        ado_debug( DB_LVL_DRIVER, "ADG reg dump: AVB_CLK_CONFIG=0x%x, AVB_CLK_STATUS=0x%x",
                   adg->avb_clk_config, adg->avb_clk_status );
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/adg.c $ $Rev: 911285 $")
#endif
