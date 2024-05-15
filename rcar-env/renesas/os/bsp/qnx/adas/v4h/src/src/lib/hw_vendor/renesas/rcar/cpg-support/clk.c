/*
 * Copyright (c) 2020, 2023, BlackBerry Limited. All rights reserved.
 * Copyright (c) 2022, Renesas Electronics Corporation.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

#include "proto.h"

#ifdef ABS
 #undef ABS
#endif
#define ABS(x)  (((int)(x) >= 0) ? (int)(x) : (-(int)(x)))

#define MHZ(x)  ((x) * 1000U * 1000U)

static uint32_t cpg_clock_get_offset(const uint32_t soc_id, const cpg_clk_id_t id, const int idx);

struct pll_offset_table {
    char     name[16];
    int      bit_idx;
    uint32_t ctlr_off;
    int      type;
#define CPG_PLL_ALWAYS_ON   1
};

#define PLL_OFF(_num, _idx, _cr, _type)   \
            [CPG_CLK_PLL##_num] = {       \
                .name     = "PLL##_num",  \
                .bit_idx  = (_idx),       \
                .ctlr_off = (_cr),        \
                .type     = (_type)       \
            }

/*
 * PLL Clock
 */
static struct pll_offset_table  gen3_pll_offset_table[] = {
    PLL_OFF(0, 0, RCAR_GEN3_CPG_PLL0CR, CPG_PLL_ALWAYS_ON),
    PLL_OFF(2, 2, RCAR_GEN3_CPG_PLL2CR, CPG_PLL_ALWAYS_ON),
    PLL_OFF(3, 3, RCAR_GEN3_CPG_PLL3CR, CPG_PLL_ALWAYS_ON),
    PLL_OFF(4, 4, RCAR_GEN3_CPG_PLL4CR, CPG_PLL_ALWAYS_ON),
};

static struct pll_offset_table  v3u_pll_offset_table[] = {
    PLL_OFF(1,  0, CPG_REG_OFF_UNKNOWN,   CPG_PLL_ALWAYS_ON),
    PLL_OFF(20, 1, RCAR_GEN4_CPG_PLL20CR, CPG_PLL_ALWAYS_ON),
    PLL_OFF(21, 2, RCAR_GEN4_CPG_PLL21CR, CPG_PLL_ALWAYS_ON),
    PLL_OFF(30, 3, RCAR_GEN4_CPG_PLL30CR, CPG_PLL_ALWAYS_ON),
    PLL_OFF(31, 4, RCAR_GEN4_CPG_PLL31CR, CPG_PLL_ALWAYS_ON),
    PLL_OFF(4,  5, RCAR_GEN4_CPG_PLL4CR,  CPG_PLL_ALWAYS_ON),
    PLL_OFF(5,  6, CPG_REG_OFF_UNKNOWN,   CPG_PLL_ALWAYS_ON),
};

static struct pll_offset_table  gen4_pll_offset_table[] = {
    PLL_OFF(1,  0, RCAR_GEN4_CPG_PLL1CR0, CPG_PLL_ALWAYS_ON),
    PLL_OFF(2,  1, RCAR_GEN4_CPG_PLL2CR0, CPG_PLL_ALWAYS_ON),
    PLL_OFF(3,  3, RCAR_GEN4_CPG_PLL3CR0, CPG_PLL_ALWAYS_ON),
    PLL_OFF(4,  5, RCAR_GEN4_CPG_PLL4CR,  CPG_PLL_ALWAYS_ON),
    PLL_OFF(5,  6, CPG_REG_OFF_UNKNOWN,   CPG_PLL_ALWAYS_ON),
    PLL_OFF(6,  7, RCAR_GEN4_CPG_PLL6CR0, CPG_PLL_ALWAYS_ON),
};

static struct pll_offset_table * pll_get_offset(const uint32_t soc_id, const int pll_id)
{
    switch (soc_id) {
        case PRODUCT_ID_RCAR_V4H:
            switch (pll_id) {
                case CPG_CLK_PLL1:
                case CPG_CLK_PLL2:
                case CPG_CLK_PLL3:
                case CPG_CLK_PLL4:
                case CPG_CLK_PLL5:
                case CPG_CLK_PLL6:
                    return &gen4_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        case PRODUCT_ID_RCAR_S4:
            switch (pll_id) {
                case CPG_CLK_PLL1:
                case CPG_CLK_PLL2:
                case CPG_CLK_PLL3:
                case CPG_CLK_PLL5:
                case CPG_CLK_PLL6:
                    return &gen4_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        case PRODUCT_ID_RCAR_V3U:
            switch (pll_id) {
                case CPG_CLK_PLL1:
                case CPG_CLK_PLL4:
                case CPG_CLK_PLL5:
                case CPG_CLK_PLL20:
                case CPG_CLK_PLL21:
                case CPG_CLK_PLL30:
                case CPG_CLK_PLL31:
                    return &v3u_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        case PRODUCT_ID_RCAR_H3:
        case PRODUCT_ID_RCAR_M3W:
            switch (pll_id) {
                case CPG_CLK_PLL0:
                case CPG_CLK_PLL2:
                case CPG_CLK_PLL3:
                case CPG_CLK_PLL4:
                    return &gen3_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        case PRODUCT_ID_RCAR_M3N:
            switch (pll_id) {
                case CPG_CLK_PLL0:
                case CPG_CLK_PLL3:
                case CPG_CLK_PLL4:
                    return &gen3_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        case PRODUCT_ID_RCAR_E3:
        case PRODUCT_ID_RCAR_D3:
            switch (pll_id) {
                case CPG_CLK_PLL0:
                case CPG_CLK_PLL3:
                    return &gen3_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        case PRODUCT_ID_RCAR_V3H:
            switch (pll_id) {
                case CPG_CLK_PLL2:
                case CPG_CLK_PLL3:
                    return &gen3_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        case PRODUCT_ID_RCAR_V3M:
            switch (pll_id) {
                case CPG_CLK_PLL3:
                    return &gen3_pll_offset_table[pll_id];
                default:
                    return NULL;
            }
        default:
            return NULL;
    }
}

static int cpg_pll_clock_enable(const int pll_id)
{
    uint32_t tmp;
    const struct pll_offset_table* const offset = pll_get_offset(cpg_dev.soc_id, pll_id);

    if (offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: No support PLL%d", __func__, pll_id);
        return CPG_RET_INVALPARA;
    }

    if (offset->type & CPG_PLL_ALWAYS_ON) {
        return CPG_RET_OK;
    }

    tmp  = cpg_read(cpg_dev.ecr_off);
    tmp |= BIT(offset->bit_idx);
    cpg_write(cpg_dev.ecr_off, tmp);

    const int status = cpg_waitfor(cpg_dev.ecr_off, BIT(offset->bit_idx), BIT(offset->bit_idx));
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "[CPG clock] %s: Unable to enable", __func__, offset->name);
    }

    return status;
}


static int cpg_pll_clock_disable(const int pll_id)
{
    uint32_t tmp;
    const struct pll_offset_table* const offset = pll_get_offset(cpg_dev.soc_id, pll_id);

    if (offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: No support PLL%d", __func__, pll_id);
        return CPG_RET_INVALPARA;
    }

    if (offset->type & CPG_PLL_ALWAYS_ON) {
        return CPG_RET_OK;
    }

    tmp  = cpg_read(cpg_dev.ecr_off);
    tmp &= ~BIT(offset->bit_idx);
    cpg_write(cpg_dev.ecr_off, tmp);

    const int status = cpg_waitfor(cpg_dev.ecr_off, BIT(offset->bit_idx), 0);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "[CPG clock] %s: Unable to disable", __func__, offset->name);
    }

    return status;
}


static int cpg_pll_clock_is_enabled(const int pll_id)
{
    const struct pll_offset_table* const offset = pll_get_offset(cpg_dev.soc_id, pll_id);

    if (offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: No support PLL%d", __func__, pll_id);
        return CPG_RET_ERROR;
    }

    return !!(cpg_read(cpg_dev.ecr_off) & BIT((offset->bit_idx + 8)));
}


static int cpg_pll_clock_rate_get(const int pll_id, uint32_t *pllvco)
{
    uint32_t tmp;
    const struct pll_offset_table* const offset = pll_get_offset(cpg_dev.soc_id, pll_id);

    if (offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: No support PLL%d", __func__, pll_id);
        return CPG_RET_INVALPARA;
    }

    if (offset->ctlr_off == CPG_REG_OFF_UNKNOWN) {
        if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3U) {
            uint32_t fixed_mul = 0;
            if (pll_id == CPG_CLK_PLL1) {
                fixed_mul = 128;
            } else if (pll_id == CPG_CLK_PLL5) {
                fixed_mul = 192;
            }
            *pllvco = cpg_dev.ext_clk * fixed_mul;
        } else {
            /* S4, V4H does not have PLLCR for PLL5. Freq is fixed to 3200Mhz */
            *pllvco = MHZ(3200);
        }
        return CPG_RET_OK;
    }

    if ((cpg_read(cpg_dev.ecr_off) & BIT((8 + offset->bit_idx)))) {
        tmp = cpg_read(offset->ctlr_off);
        if (cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) {
            const uint32_t ss_mode = (tmp >> 16) & 0x7;
            *pllvco = ((tmp >> 20) & 0x1FF) + 1;
            if (ss_mode == 0) {
                *pllvco *= 2;
            }
        } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V4H) {
            *pllvco = (((tmp >> 20) & 0x1FF) + 1) * 2;
        } else {
            *pllvco = (((tmp >> 24) & 0x7F) + 1) * 2;
        }
        *pllvco *= cpg_dev.ext_clk;
        return CPG_RET_OK;
    }

    cpg_slogf(_SLOG_ERROR, "%s: %s is not enabled", __func__, offset->name);

    return CPG_RET_ERROR;
}


static int cpg_pll_clock_rate_set(const int pll_id, uint32_t* const pllvco)
{
    uint32_t stc_val;
    uint32_t tmp, stc_msk = 0x7F;
    const struct pll_offset_table* const offset = pll_get_offset(cpg_dev.soc_id, pll_id);

    if (offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: No support PLL%d", __func__, pll_id);
        return CPG_RET_INVALPARA;
    }

    tmp = cpg_read(offset->ctlr_off);

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) {
        switch(pll_id) {
            case CPG_CLK_PLL2:
            case CPG_CLK_PLL3:
            case CPG_CLK_PLL6:
            {
                uint32_t ni_val;
                uint32_t ni_msk = 0x1FF;
                uint32_t ss_mode = (tmp << 16) & 0x7;
                if (ss_mode == 0) {
                    ni_val = *pllvco / (2 * cpg_dev.ext_clk);
                } else {
                    ni_val = *pllvco / cpg_dev.ext_clk;
                }

                tmp &= ~(ni_msk << 20);
                tmp |= ((ni_val - 1) & ni_msk) << 20;
                cpg_write(offset->ctlr_off, tmp);

                tmp |= (1 << 31);
                cpg_write(offset->ctlr_off, tmp);
                break;
            }
            default:
                cpg_slogf(_SLOG_ERROR, "%s: PLL%d doesn't support Freq change", __func__, pll_id);
                return CPG_RET_INVALPARA;
        }
    } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V4H) {
        switch(pll_id) {
            case CPG_CLK_PLL2:
            case CPG_CLK_PLL3:
            case CPG_CLK_PLL4:
            case CPG_CLK_PLL6:
            {
                uint32_t ni_val;
                uint32_t ni_msk = 0x1FF;
                ni_val = *pllvco / (2 * cpg_dev.ext_clk);
                tmp &= ~(ni_msk << 20);
                tmp |= ((ni_val - 1) & ni_msk) << 20;
                cpg_write(offset->ctlr_off, tmp);
                tmp |= (1 << 31);
                cpg_write(offset->ctlr_off, tmp);
            }
            default:
                cpg_slogf(_SLOG_ERROR, "%s: PLL%d doesn't support Freq change", __func__, pll_id);
                return CPG_RET_INVALPARA;
        }
    } else {
        stc_val = *pllvco / (2 * cpg_dev.ext_clk);

        tmp &= ~(stc_msk << 24);
        tmp |= ((stc_val - 1) & stc_msk) << 24;
        cpg_write(offset->ctlr_off, tmp);
    }

    stc_msk = BIT(8 + offset->bit_idx);

    const int status = cpg_waitfor(cpg_dev.ecr_off, stc_msk, stc_msk);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Unable to change %s to %dHz", __func__, offset->name, *pllvco);
        return status;
    }

    cpg_pll_clock_rate_get(pll_id, pllvco);

    return CPG_RET_OK;
}

/*
 * CPU Clock
 */

#define RCAR_PLL0VCO_MIN        (2000000000)
#define RCAR_PLL0VCO_MAX        (4000000000)
#define RCAR_H3_PLL0VCO_MAX     (3400000000)
#define RCAR_M3_PLL0VCO_MAX     (3600000000)

static int cpg_cpu_clock_rate_set(const int idx, const uint32_t* const rate)
{
    const uint32_t stc_msk = 0x7F, zfc_msk = 0x1F, kick_msk = BIT(31);
    uint32_t zfc_val = 0, stc_val = 0;
    uint32_t diff, min = ~0U;
    uint32_t zfc, stc, freq = 0;
    uint32_t stc_min, stc_max;
    uint32_t frqcrc, pll0cr, frqcrb;

    if ( (cpg_dev.soc_id != PRODUCT_ID_RCAR_H3) &&
         (cpg_dev.soc_id != PRODUCT_ID_RCAR_M3W) &&
         (cpg_dev.soc_id != PRODUCT_ID_RCAR_M3N) ) {
        return CPG_RET_OK;
    }

    if (idx != 1) { // Currently only frequency setting for the CA57 on H3/M3 is supported
        cpg_slogf(_SLOG_ERROR, "%s: Error: Invalid idx %d", __func__, idx);
        return CPG_RET_INVALPARA;
    }

    if (cpg_read(RCAR_GEN3_CPG_FRQCRB) & kick_msk) {
        return CPG_RET_TIMEOUT;
    }

    stc_min = RCAR_PLL0VCO_MIN / (cpg_dev.ext_clk * 2) - 1;
    stc_max = RCAR_PLL0VCO_MAX / (cpg_dev.ext_clk * 2) - 1;

    for (zfc = 0; zfc <= zfc_msk; zfc++) {
        for (stc = stc_min; stc <= stc_max; stc++) {
            freq  = (stc + 1) * (32 - zfc) / 32;
            freq *= cpg_dev.ext_clk;
            if (*rate > freq) {
                diff = *rate - freq;
            } else {
                diff = freq - *rate;
            }
            if (diff < min) {
                min = diff;
                zfc_val = zfc;
                stc_val = stc;
            }
        }
    }

    /* Change clock rate */
    frqcrc = cpg_read(RCAR_GEN3_CPG_FRQCRC) & ~(zfc_msk << 8);
    pll0cr = cpg_read(RCAR_GEN3_CPG_PLL0CR) & ~(stc_msk << 24);
    frqcrc |= (zfc_val << 8);
    pll0cr |= (stc_val << 24);
    cpg_write(RCAR_GEN3_CPG_PLL0CR, pll0cr);
    cpg_write(RCAR_GEN3_CPG_FRQCRC, frqcrc);
    /*
     * Set KICK bit in FRQCRB to update hardware setting and wait for
     * clock change completion.
     */
    frqcrb = cpg_read(RCAR_GEN3_CPG_FRQCRB);
    frqcrb |= kick_msk;
    cpg_write(RCAR_GEN3_CPG_FRQCRB, frqcrb);

    /*
     * Note: There is no HW information about the worst case latency.
     *
     * Using experimental measurements, it seems that no more than
     * ~10 iterations are needed, independently of the CPU rate.
     * Since this value might be dependent of external xtal rate, pll4
     * rate or even the other emulation clocks rate, use 1000 as a
     * "super" safe value.
     */
    const int status = cpg_waitfor(RCAR_GEN3_CPG_FRQCRB, kick_msk, 0);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Unable to change rate to %dHz", __func__, *rate);
    }

    return status;
}

static int cpg_cpu_clock_rate_get(const int idx, uint32_t* const rate)
{
    uint64_t freq = 0;

    // FRQCRC and PLLxCR are not available for V3M and D3.
    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        // V3M cpu frequency is 800MHz
        freq = MHZ(800);
    } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_D3) {
        // D3 cpu frequency is 1000MHz
        freq = MHZ(1000);
    } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_E3) {
        // E3 cpu frequency is 1200MHz
        freq = MHZ(1200);
    } else {
        if ( ((cpg_dev.soc_id == PRODUCT_ID_RCAR_H3)  ||
              (cpg_dev.soc_id == PRODUCT_ID_RCAR_M3W) ||
              (cpg_dev.soc_id == PRODUCT_ID_RCAR_M3N)) &&
              (idx == 1) ) { // CA57
            const uint32_t  frqcrc = cpg_read(RCAR_GEN3_CPG_FRQCRC);
            const uint32_t  pll0cr = cpg_read(RCAR_GEN3_CPG_PLL0CR);

            freq = ((pll0cr >> 24) & 0x7F);
            if (cpg_read(cpg_dev.ecr_off) & BIT(8)) {
                freq  = (freq + 1) * cpg_dev.ext_clk;
#ifdef VARIANT_pm
                freq *= (32 - ((frqcrc >> 8) & 0x1F));
                freq /= 32;
#else
                // following evaluates to 0 (freq) for frqcrc > 0 (bits 12:8)
                freq *= (32 - ((frqcrc >> 8) & 0x1F)) / 32;
#endif
            } else {
                freq *= cpg_dev.ext_clk;
            }
#ifdef VARIANT_pm
            cpg_slogf(_SLOG_DEBUG1, "%s: ext_clk %u", __func__, cpg_dev.ext_clk);
            cpg_slogf(_SLOG_DEBUG1, "%s: freq %u", __func__, freq);

        } else if ( ((cpg_dev.soc_id == PRODUCT_ID_RCAR_H3)  ||
                     (cpg_dev.soc_id == PRODUCT_ID_RCAR_M3W)) &&
                     (idx == 0) ) { // CA53
            const uint32_t  frqcrc = cpg_read(RCAR_GEN3_CPG_FRQCRC);
            const uint32_t  pll2cr = cpg_read(RCAR_GEN3_CPG_PLL2CR);

            freq = ((pll2cr >> 24) & 0x7F);
            if (cpg_read(cpg_dev.ecr_off) & BIT(8)) {
                freq  = (freq + 1) * cpg_dev.ext_clk;
                freq *= (32 - ((frqcrc >> 0 ) & 0x1F));
                freq /= 32;
            } else {
                freq *= cpg_dev.ext_clk;
            }
            cpg_slogf(_SLOG_DEBUG1, "%s: ext_clk %u", __func__, cpg_dev.ext_clk);
            cpg_slogf(_SLOG_DEBUG1, "%s: freq %u", __func__, freq);
#endif // VARIANT_pm
        } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3U) {
            const uint32_t frqcrc = cpg_read(RCAR_GEN4_CPG_FRQCRC);
            const uint32_t pll20cr = cpg_read(RCAR_GEN4_CPG_PLL20CR);

            if (cpg_read(cpg_dev.ecr_off) && BIT(9)) {
                freq = (32 - ((frqcrc >> 0 ) & 0x1F)) / 32 * (((pll20cr >> 24) & 0x7F) + 1);
                if ( ((pll20cr >> 7) & 0x1) == 0x1) {
                    freq *= cpg_dev.ext_clk / 2;
                } else {
                    freq *= cpg_dev.ext_clk;
                }
            } else {
                freq = cpg_dev.ext_clk;
            }
        } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) {
            const uint32_t frqcrc = cpg_read(RCAR_GEN4_CPG_FRQCRC);
            const uint32_t pll2cr0 = cpg_read(RCAR_GEN4_CPG_PLL2CR0);
            const uint8_t ssmode = (uint8_t) ((pll2cr0 >> 16) & 0x7);

            if (cpg_read(RCAR_GEN4_CPG_PLLECR) && (1 << 9)) {
                /*
                 * SSMODE[2:0] = 3’b000
                 *  ・ PLL2 multiplication ratio = (NI[8:0] + 1) x 2
                 *  ・ PLL2VCO = (PLL2 reference clock) x (PLL2 multiplication ratio)
                 * SSMODE[2:0] = 3’b100, 3’b110
                 *  ・ PLL2 multiplication ratio = (NI[8:0] + 1) + NF[23:0] / 2^24
                 *  ・ PLL2VCO = (PLL2 reference clock) x (PLL2 multiplication ratio)
                 */
                if (ssmode == 0) {
                    freq = (((pll2cr0 >> 20) & 0x1FF) + 1) * 2;
                } else if ( (ssmode == 4) || (ssmode == 6) ) {
                    freq = (((pll2cr0 >> 20) & 0x1FF) + 1);
                }
                freq *= cpg_dev.ext_clk;
                freq  = (freq / 2) * (32 - ((frqcrc >> 8 ) & 0x1F)) / 32;
            } else {
                freq  = cpg_dev.ext_clk;
            }
        } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V4H) {
            const uint32_t frqcrc = cpg_read(RCAR_GEN4_CPG_FRQCRC);
            const uint32_t pll2cr0 = cpg_read(RCAR_GEN4_CPG_PLL2CR0);

            if (cpg_read(RCAR_GEN4_CPG_PLLECR) && (1 << 9)) {
                /*
                 * SSMODE[2:0] = 3’b000
                 *  ・ PLL2 multiplication ratio = (NI[8:0] + 1) x 2
                 *  ・ PLL2VCO = (PLL2 reference clock) x (PLL2 multiplication ratio)
                 * SSMODE[2:0] = 3’b100, 3’b110
                 *  ・ PLL2 multiplication ratio = (NI[8:0] + 1)) x 2 + NF[23:0] / 2^24
                 *  ・ PLL2VCO = (PLL2 reference clock) x (PLL2 multiplication ratio)
                 */
                freq = (((pll2cr0 >> 20) & 0x1FF) + 1) * 2;
                freq *= cpg_dev.ext_clk;
                freq = (freq / 2) * (32 - (frqcrc & 0x1F)) / 32;
            } else {
                freq = cpg_dev.ext_clk;;
            }
        } else {
            uint32_t div;
            const uint32_t pll2cr = cpg_read(RCAR_GEN3_CPG_PLL2CR);
            if (cpg_read(cpg_dev.ecr_off) & BIT(10)) {
                if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3H) {
                    div = 16;
                } else {
                    const uint32_t frqcrc = cpg_read(RCAR_GEN3_CPG_FRQCRC);
                    div = 32 - (frqcrc & 0x1F);
                }
                freq = (((pll2cr >> 24) & 0x7F) + 1) * div / 32;
            } else {
                freq = ((pll2cr >> 24) & 0x7F);
            }
            freq *= cpg_dev.ext_clk;
        }
    }

    *rate = (uint32_t)freq;

    return CPG_RET_OK;
}

/*
 * RGX Clock
 */

static int cpg_rgx_clock_rate_get(const int idx, uint32_t* const rate)
{
    uint32_t clock;
    int ret = CPG_RET_OK;
    (void) idx;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) {
        cpg_slogf(_SLOG_ERROR, "%s: No support", __func__);
        return CPG_RET_INVALPARA;
    }

    if ((cpg_dev.soc_id == PRODUCT_ID_RCAR_D3) || 
        (cpg_dev.soc_id == PRODUCT_ID_RCAR_E3)) {
        clock = cpg_read(cpg_dev.frqcrb_off);
        clock >>= 24;
        clock &= 0x1F;
        if ((clock & 0x1C) == 0) {
            *rate = MHZ(600);
        } else if ((clock & 0x1C) == 4) {
            *rate = MHZ(300);
        } else if (clock == 0x9) {
            *rate = MHZ(400);
        } else if (clock == 0xA) {
            *rate = MHZ(200);
        } else if (clock == 0xC) {
            *rate = 533330000;
        } else if (clock == 0xD) {
            *rate = 266660000;
        } else if (clock == 0xE) {
            *rate = 133330000;
        } else {
            *rate = 0;
            ret = CPG_RET_ERROR;
            cpg_slogf(_SLOG_ERROR, "%s: Failed to get clock", __func__);
        }
        return ret;
    }

    ret = cpg_pll_clock_rate_get(CPG_CLK_PLL4, &clock);
    if (ret == CPG_RET_OK) {
        const uint32_t zgfc_msk = 0x1F;
        if (IS_RCAR_GEN4(cpg_dev.soc_id)) {
            /*
             * (3DGE clock) = pll4vco * (1 / 2) * ((32- zgfc_val) / 32)
             */
            clock /= (32 - ((cpg_read(cpg_dev.frqcrb_off) >> 24) & zgfc_msk)) / 32;
            clock /= 2;
        } else {
            /*
             * (3DGE clock) = pll4vco * (1 / 4)
             */
            clock /= 4;
        }
        *rate = clock;

        return CPG_RET_OK;
    }

    return ret;
}


static int cpg_rgx_clock_rate_set(const int idx, uint32_t* const rate)
{
    uint32_t   tmp, stc_val = 0, kick;
    const uint32_t   stc_msk = 0x1F, kick_msk = BIT(31);
    int ret = CPG_RET_OK;
    (void) idx;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) {
        cpg_slogf(_SLOG_ERROR, "%s: No support", __func__);
        return CPG_RET_INVALPARA;
    }

    if (cpg_read(cpg_dev.frqcrb_off) & kick_msk) {
        cpg_slogf(_SLOG_ERROR, "%s: FRQCRB setting is busy", __func__);
        return CPG_RET_TIMEOUT;
    }

    if ((cpg_dev.soc_id == PRODUCT_ID_RCAR_D3) || 
        (cpg_dev.soc_id == PRODUCT_ID_RCAR_E3)) {
        if (*rate >= MHZ(600)) {
            stc_val = 0;
            *rate = MHZ(600);
        } else if ( (*rate < MHZ(600)) && (*rate >= 533330000U) ) {
            *rate = 533330000U;
            stc_val = 0xC;
        } else if ( (*rate < 533330000U) && (*rate >= MHZ(400)) ) {
            *rate = MHZ(400);
            stc_val = 0x9;
        } else if ( (*rate < MHZ(400)) && (*rate >= 266660000U) ) {
            *rate = 266660000U;
            stc_val = 0xD;
        } else if ( (*rate < 266660000U) && (*rate >= MHZ(400)) ) {
            *rate = MHZ(200);
            stc_val = 0xC;
        } else if (*rate < MHZ(200)) {
            *rate = 133330000U;
            stc_val = 0xE;
        }
    } else if (IS_RCAR_GEN4(cpg_dev.soc_id)) {
        uint32_t   pll4vco = cpg_dev.ext_clk;
        /* Calculate stc_val to set to bit[24:30] of FRQCRB register:
         * (3DGE rate) = (pll4vco) * (1 / 2) * (32 - stc_val) / 32
         * ==> stc_val = 32 - (3DGE rate * 2 * 32) / pll4vco
         */
        ret = cpg_pll_clock_rate_get(CPG_CLK_PLL4, &pll4vco);
        if (ret != CPG_RET_OK) {
            return ret;
        }

        stc_val = 32 - (*rate * 64) / pll4vco;
        stc_val &= stc_msk;
    } else {
        /*
         * (3DGE clock) = pll4vco * (1 / 4)
         */
        *rate *= 4;
        return cpg_pll_clock_rate_set(CPG_CLK_PLL4, rate);
    }

    tmp = cpg_read(cpg_dev.frqcrb_off);

    /* If this rate was set already, return CPG_RET_OK without doing any thing. */
    if (stc_val == ((tmp >> 24) & stc_msk)) {
        return CPG_RET_OK;
    }

    tmp &= ~(stc_msk << 24);
    tmp |= stc_val << 24;

    /* Change clock rate */
    cpg_write(cpg_dev.frqcrb_off, tmp);

    /*
     * Set KICK bit in FRQCRB to update hardware setting and wait for
     * clock change completion.
     */
    kick = cpg_read(cpg_dev.frqcrb_off);
    kick |= kick_msk;
    cpg_write(cpg_dev.frqcrb_off, kick);

    /*
     * Note: There is no HW information about the worst case latency.
     *
     * Using experimental measurements, it seems that no more than
     * ~10 iterations are needed, independently of the CPU rate.
     * Since this value might be dependent of external xtal rate, pll4
     * rate or even the other emulation clocks rate, use 1000 as a
     * "super" safe value.
     */
    const int status = cpg_waitfor(cpg_dev.frqcrb_off, kick_msk, 0);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Unable to change rate to %dHz", __func__, *rate);
        return status;
    }

    cpg_rgx_clock_rate_get(idx, rate);
    return CPG_RET_OK;
}

/*
 * SD Clock divider
 */

#define CPG_PLLVCO5_CLK       MHZ(3200)
#define CPG_MOD_PARENT_CLK    MHZ(800)
#define CPG_MOD_CKSTP_MASK    BIT(8)
#define CPG_MOD_DIV_MASK      0x3F

#define CPG_SD_SDSRC_CLK(n)   ( ((n) == 2) ? 533333333 : \
                                ((n) == 1) ? MHZ(640)  : \
                                ((n) == 0) ? MHZ(800)  : 0 )
#define CPG_SD_STP_HCK        BIT(9)
#define CPG_SD_STP_CK         BIT(8)
#define CPG_SD_SRCSEL(n)      (n << 12)

#define CPG_SD_STP_MASK       (CPG_SD_STP_HCK | CPG_SD_STP_CK)
#define CPG_SD_FC_MASK        ((0x7 << 2) | (0x3 << 0))
#define CPG_SD_SRCSEL_MASK    (3 << 12)

#define CPG_SD_DIV_TABLE_DATA(stp_hck, stp_ck, sd_srcfc, sd_fc, sd_div) \
{ \
    .val = ((stp_hck) ? CPG_SD_STP_HCK : 0) | \
           ((stp_ck) ? CPG_SD_STP_CK : 0) | \
           ((sd_srcfc) << 2) | \
           ((sd_fc) << 0), \
    .div = (sd_div), \
}

struct sd_div_table {
    uint32_t val;
    uint32_t div;
};

#define CPG_SDSRC_DIV_MAX  3
static const uint32_t sdsrc_div_table[CPG_SDSRC_DIV_MAX] = {
    0x0, 0x1, 0x2
};

static const struct sd_div_table cpg_sd_div_table[] = {
/*    CPG_SD_DIV_TABLE_DATA(stp_hck,  stp_ck,   sd_srcfc,   sd_fc,  sd_div) */
    CPG_SD_DIV_TABLE_DATA(0,        0,        1,          0,        2), /* HS400(Use 4TAP) */
    CPG_SD_DIV_TABLE_DATA(0,        0,        0,          1,        4), /* SDR104 / HS200 / HS400(Use 8TAP) */
    CPG_SD_DIV_TABLE_DATA(0,        0,        1,          1,        8), /* SDR50 */
    CPG_SD_DIV_TABLE_DATA(1,        0,        2,          1,       16), /* HS / SDR25 */
    CPG_SD_DIV_TABLE_DATA(1,        0,        3,          1,       32), /* NS / SDR12 */
    CPG_SD_DIV_TABLE_DATA(0,        0,        0,          0,        2),
    CPG_SD_DIV_TABLE_DATA(1,        0,        2,          0,        8),
    CPG_SD_DIV_TABLE_DATA(1,        0,        3,          0,       16),
    CPG_SD_DIV_TABLE_DATA(1,        0,        4,          0,       32),
    CPG_SD_DIV_TABLE_DATA(1,        0,        4,          1,       64),
};

static uint32_t cpg_sd_clock_get_ctlr_offset(const uint32_t soc_id, const int idx)
{
    switch (soc_id) {
        case PRODUCT_ID_RCAR_V3U:
        case PRODUCT_ID_RCAR_S4:
        case PRODUCT_ID_RCAR_V4H:
            switch (idx) {
                case 0:  return RCAR_GEN4_CPG_SD0CKCR;
                default: return CPG_REG_OFF_UNKNOWN;
            }
        case PRODUCT_ID_RCAR_H3:
        case PRODUCT_ID_RCAR_M3W:
        case PRODUCT_ID_RCAR_M3N:
            switch (idx) {
                case 0:  return RCAR_GEN3_CPG_SD0CKCR;
                case 1:  return RCAR_GEN3_CPG_SD1CKCR;
                case 2:  return RCAR_GEN3_CPG_SD2CKCR;
                case 3:  return RCAR_GEN3_CPG_SD3CKCR;
                default: return CPG_REG_OFF_UNKNOWN;
            }
        case PRODUCT_ID_RCAR_E3:
            switch (idx) {
                case 0:  return RCAR_GEN3_CPG_SD0CKCR;
                case 1:  return RCAR_GEN3_CPG_SD1CKCR;
                case 3:  return RCAR_GEN3_CPG_SD3CKCR;
                default: return CPG_REG_OFF_UNKNOWN;
            }
        case PRODUCT_ID_RCAR_D3:
            switch (idx) {
                case 2:  return RCAR_GEN3_CPG_SD2CKCR;
                default: return CPG_REG_OFF_UNKNOWN;
            }
        case PRODUCT_ID_RCAR_V3H:
        case PRODUCT_ID_RCAR_V3M:
            switch (idx) {
                case 0:  return RCAR_GEN3_CPG_SD0CKCR;
                default: return CPG_REG_OFF_UNKNOWN;
            }
        default:
            return CPG_REG_OFF_UNKNOWN;
    }
}

static int cpg_sd_clock_config(const int idx, const int enable)
{
    uint32_t val;
    uint32_t offset;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return CPG_RET_INVALPARA;
    }

    offset = cpg_sd_clock_get_ctlr_offset(cpg_dev.soc_id, idx);
    if (offset == CPG_REG_OFF_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return CPG_RET_INVALPARA;
    }

    val = cpg_read(offset);
    if (enable) {
        val &= ~(CPG_SD_STP_MASK);
    } else {
        val |= CPG_SD_STP_MASK;
    }
    cpg_write(offset, val);

    return CPG_RET_OK;
}


static int cpg_sd_clock_is_enabled(const int idx)
{
    uint32_t offset;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return -1;
    }

    offset = cpg_sd_clock_get_ctlr_offset(cpg_dev.soc_id, idx);
    if (offset == CPG_REG_OFF_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return -1;
    }

    return !(cpg_read(offset) & CPG_SD_STP_MASK);
}


static int cpg_sd_clock_rate_get(const int idx, uint32_t* const rate)
{
    uint32_t val, clock;
    uint32_t sdsrcsel = 0;
    uint32_t offset;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return CPG_RET_INVALPARA;
    }

    offset = cpg_sd_clock_get_ctlr_offset(cpg_dev.soc_id, idx);
    if (offset == CPG_REG_OFF_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return CPG_RET_INVALPARA;
    }

    if ((cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) || (cpg_dev.soc_id == PRODUCT_ID_RCAR_V4H)) {
        sdsrcsel = (cpg_read(RCAR_GEN4_CPG_SD0CKCR1) >> 29) & 3;
    } else if ((cpg_dev.soc_id == PRODUCT_ID_RCAR_V3H) && (cpg_dev.soc_rev == PRODUCT_V3H_REV_2_0)) {
        sdsrcsel = (cpg_read(offset) >> 12) & 3;
    }

    val = cpg_read(offset);
    clock   = CPG_SD_SDSRC_CLK(sdsrcsel) >> ((val >> 2) & 7);
    clock >>= ((val & 3) + 1);

    *rate = clock;

    return CPG_RET_OK;
}


static int cpg_sd_clock_rate_set(const int idx, uint32_t* const rate)
{
    uint32_t val, tmp, diff = ~0U;
    int i, j, k = -1, len = 1, n = 0;
    uint32_t offset;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return CPG_RET_INVALPARA;
    }

    offset = cpg_sd_clock_get_ctlr_offset(cpg_dev.soc_id, idx);
    if (offset == CPG_REG_OFF_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s : SDHI%d Not supported", __func__, idx);
        return CPG_RET_INVALPARA;
    }

    if ( (cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) ||
         (cpg_dev.soc_id == PRODUCT_ID_RCAR_V4H) ||
         ((cpg_dev.soc_id == PRODUCT_ID_RCAR_V3H) && (cpg_dev.soc_rev == PRODUCT_V3H_REV_2_0)) ) {
        len = CPG_SDSRC_DIV_MAX;
    }

    for (j = 0; j < len; j++) {
        for (i = 0; i < ARRAY_SIZE(cpg_sd_div_table); i++) {
            tmp = CPG_SD_SDSRC_CLK(sdsrc_div_table[j]) / cpg_sd_div_table[i].div;
            tmp = (tmp > *rate) ? (tmp - *rate) : (*rate - tmp);
            if (tmp < diff) {
                diff = tmp;
                k = i;
                n = j;
            }
        }
    }

    if ( (k < 0) || (k == 255)) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid rate %dHz", __func__, *rate);
        return CPG_RET_INVALPARA;
    }

    if ((cpg_dev.soc_id == PRODUCT_ID_RCAR_S4) ||
         (cpg_dev.soc_id == PRODUCT_ID_RCAR_V4H)) {
        val  = cpg_read(RCAR_GEN4_CPG_SD0CKCR1);
        val &= ~(3 << 29);
        val |=  (sdsrc_div_table[n] << 29);
        cpg_write(RCAR_GEN4_CPG_SD0CKCR1, val);
    }

    val  = cpg_read(offset);
    val &= ~(CPG_SD_STP_MASK | CPG_SD_FC_MASK | CPG_SD_SRCSEL_MASK);
    val |= cpg_sd_div_table[k].val & (CPG_SD_STP_MASK | CPG_SD_FC_MASK);
    if ((cpg_dev.soc_id == PRODUCT_ID_RCAR_V3H) && (cpg_dev.soc_rev == PRODUCT_V3H_REV_2_0)) {
        val |= sdsrc_div_table[n];
    }
    cpg_write(offset, val);

    cpg_slogf(_SLOG_INFO, "%s: SD%dCKCR 0x%x", __func__, idx, cpg_read(offset));

    /* Read back clock rate */
    cpg_sd_clock_rate_get(idx, rate);

    return CPG_RET_OK;
}

/*
 * RPC-IF clock
 */

#define GEN3_CPG_PLL1VCO_CLK (3200000000)
#define E3_CPG_PLL0VCO_CLK   (4800000000)
#define D3_CPG_PLL0VCO_CLK   (3000000000)
#define CPG_RPC_CKSTP_MASK   (BIT(8) | BIT(9))
#define CPG_RPC_DIV_MASK     0x1F

struct rpc_div_table {
    uint64_t parent;
    uint8_t  val;
    uint8_t  div;
    uint32_t rate;
};

#define RPC_DIV(_parent, _val, _div, _rate)  \
            { .parent = (_parent),           \
                .val  = (_val),              \
                .div  = (_div),              \
                .rate = (_rate),             \
            }

/* This table is calculated with PLL1VCO=3200MHz, PLL5VCO(V3U/S4/V4H)=3200MHz */
static const struct rpc_div_table cpg_rpc_div_table[] = {
    /*      parent           val   div   rate(MHz) of RPCD2φ */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x01,  16,   200),     /* For V3U/S4/V4H only */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x03,  32,   100),     /* For V3U/S4/V4H only */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x07,  64,    50),     /* For V3U/S4/V4H only */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x11,  20,   160),     /* 3200 x 1/5 x 1/2 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x13,  40,    80),     /* 3200 x 1/5 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x15,  60,    53),     /* 3200 x 1/5 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x17,  80,    40),     /* 3200 x 1/5 x 1/8 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x19,  24,   133),     /* 3200 x 1/6 x 1/2 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x1B,  48,    66),     /* 3200 x 1/6 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x1D,  72,    44),     /* 3200 x 1/6 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK, 0x1F,  96,    33),     /* 3200 x 1/6 x 1/8 x 1/2 */
};

/* This table is calculated with PLL0VCO=4800MHz/PLL1VCO=1600MHz */
static const struct rpc_div_table cpg_rpc_e3_div_table[] = {
    /*      parent            val   div   rate(MHz) of RPCD2φ */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x01,  20,    80),   /* 1600 x 1/5 x 1/2 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x03,  40,    40),   /* 1600 x 1/5 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x05,  60,    26),   /* 1600 x 1/5 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x07,  80,    20),   /* 1600 x 1/5 x 1/8 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x09,  12,   133),   /* 1600 x 1/3 x 1/2 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x0B,  24,    66),   /* 1600 x 1/3 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x0D,  36,    44),   /* 1600 x 1/3 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x0F,  48,    33),   /* 1600 x 1/3 x 1/8 x 1/2 */
    RPC_DIV(E3_CPG_PLL0VCO_CLK,     0x11,  32,   150),   /* 4800 x 1/8 x 1/2 x 1/2 */
    RPC_DIV(E3_CPG_PLL0VCO_CLK,     0x13,  64,    75),   /* 4800 x 1/8 x 1/4 x 1/2 */
    RPC_DIV(E3_CPG_PLL0VCO_CLK,     0x15,  96,    50),   /* 4800 x 1/8 x 1/6 x 1/2 */
    RPC_DIV(E3_CPG_PLL0VCO_CLK,     0x17, 128,    37),   /* 4800 x 1/8 x 1/8 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x1B,  16,   100),   /* 1600 x 1/2 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x1D,  24,    66),   /* 1600 x 1/2 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x1F,  32,    50),   /* 1600 x 1/2 x 1/8 x 1/2 */
};

/* This table is calculated with PLL0VCO=3000MHz/PLL1VCO=1600MHz */
static const struct rpc_div_table cpg_rpc_d3_div_table[] = {
    /*      parent            val   div   rate(MHz) of RPCD2φ */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x01,  20,    80),   /* 1600 x 1/5 x 1/2 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x03,  40,    40),   /* 1600 x 1/5 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x05,  60,    26),   /* 1600 x 1/5 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x07,  80,    20),   /* 1600 x 1/5 x 1/8 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x09,  12,   133),   /* 1600 x 1/3 x 1/2 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x0B,  24,    66),   /* 1600 x 1/3 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x0D,  36,    44),   /* 1600 x 1/3 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x0F,  48,    33),   /* 1600 x 1/3 x 1/8 x 1/2 */
    RPC_DIV(D3_CPG_PLL0VCO_CLK,     0x11,  20,   150),   /* 3000 x 1/5 x 1/2 x 1/2 */
    RPC_DIV(D3_CPG_PLL0VCO_CLK,     0x13,  40,    75),   /* 3000 x 1/5 x 1/4 x 1/2 */
    RPC_DIV(D3_CPG_PLL0VCO_CLK,     0x15,  60,    50),   /* 3000 x 1/5 x 1/6 x 1/2 */
    RPC_DIV(D3_CPG_PLL0VCO_CLK,     0x17,  80,    37),   /* 3000 x 1/5 x 1/8 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x1B,  16,   100),   /* 1600 x 1/2 x 1/4 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x1D,  24,    66),   /* 1600 x 1/2 x 1/6 x 1/2 */
    RPC_DIV(GEN3_CPG_PLL1VCO_CLK/2, 0x1F,  32,    50),   /* 1600 x 1/2 x 1/8 x 1/2 */
};

static int cpg_rpc_clock_config(const int idx, const int enable)
{
    (void) idx;
    uint32_t offset;
    uint32_t val;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        cpg_slogf(_SLOG_ERROR, "%s : No support", __func__);
        return CPG_RET_INVALPARA;
    }

    offset = cpg_clock_get_offset(cpg_dev.soc_id, CPG_CLK_RPC, 0);
    val = cpg_read(offset);
    if (enable) {
        val &= ~(CPG_RPC_CKSTP_MASK);
    } else {
        val |= (CPG_RPC_CKSTP_MASK);
    }
    cpg_write(offset, val);

    return CPG_RET_OK;
}


static int cpg_rpc_clock_is_enabled(const int idx)
{
    (void) idx;
    uint32_t offset;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        cpg_slogf(_SLOG_ERROR, "%s : No support", __func__);
        return CPG_RET_INVALPARA;
    }

    offset = cpg_clock_get_offset(cpg_dev.soc_id, CPG_CLK_RPC, 0);

    return !(cpg_read(offset) & CPG_RPC_CKSTP_MASK);
}


static int cpg_rpc_clock_rate_get(const int idx, uint32_t* const rate)
{
    (void) idx;
    const uint32_t offset = cpg_clock_get_offset(cpg_dev.soc_id, CPG_CLK_RPC, 0);
    uint64_t parent = 0;
    uint8_t val, div = 0;
    const struct rpc_div_table *rpc_table = NULL;
    long rpc_table_size, i = 0;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        cpg_slogf(_SLOG_ERROR, "%s : No support", __func__);
        return CPG_RET_INVALPARA;
    }

    val = (uint8_t) (cpg_read(offset) & 0x1F);

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_D3) {
        rpc_table = cpg_rpc_d3_div_table;
        rpc_table_size = ARRAY_SIZE(cpg_rpc_d3_div_table);
    } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_E3) {
        rpc_table = cpg_rpc_e3_div_table;
        rpc_table_size = ARRAY_SIZE(cpg_rpc_e3_div_table);
    } else {
        if (!IS_RCAR_GEN4(cpg_dev.soc_id)) {
            i = 3;
        }
        rpc_table = cpg_rpc_div_table;
        rpc_table_size = ARRAY_SIZE(cpg_rpc_div_table);
    }

    if (rpc_table) {
        for (; i < rpc_table_size; i++) {
            if (val == rpc_table[i].val) {
                parent = rpc_table[i].parent;
                div = rpc_table[i].div;
                break;
            }
        }
    }

    if ( (parent == 0) || (div == 0) ) {
        cpg_slogf(_SLOG_ERROR, "%s: Unable to get clock rate", __func__);
        return CPG_RET_INVALPARA;
    }

    *rate = (uint32_t) (parent / div);

    return CPG_RET_OK;
}

static int cpg_rpc_clock_rate_set(const int idx, uint32_t* const rate)
{
    (void) idx;
    const uint32_t offset = cpg_clock_get_offset(cpg_dev.soc_id, CPG_CLK_RPC, 0);
    uint32_t val;
    const struct rpc_div_table *rpc_table = NULL;
    long rpc_table_size, i = 0, k = -1;

    if (cpg_dev.soc_id == PRODUCT_ID_RCAR_V3M) {
        return CPG_RET_OK;
    } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_D3) {
        rpc_table = cpg_rpc_d3_div_table;
        rpc_table_size = ARRAY_SIZE(cpg_rpc_d3_div_table);
    } else if (cpg_dev.soc_id == PRODUCT_ID_RCAR_E3) {
        rpc_table = cpg_rpc_e3_div_table;
        rpc_table_size = ARRAY_SIZE(cpg_rpc_e3_div_table);
    } else {
        if (!IS_RCAR_GEN4(cpg_dev.soc_id)) {
            i = 3;
        }
        rpc_table = cpg_rpc_div_table;
        rpc_table_size = ARRAY_SIZE(cpg_rpc_div_table);
    }

    if (rpc_table) {
        uint32_t tmp, diff = ~0U;
        for (; i < rpc_table_size; i++) {
            tmp = (uint32_t) (rpc_table[i].parent / rpc_table[i].div);
            tmp = (uint32_t) ABS(tmp - *rate);
            if (tmp < diff) {
                diff = tmp;
                k = i;
            }
        }
    }

    if (k < 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid rate %dHz", __func__, *rate);
        return CPG_RET_INVALPARA;
    }

    val  = cpg_read(offset);
    val &= ~(CPG_RPC_CKSTP_MASK | CPG_RPC_DIV_MASK);

    val |= (uint32_t) rpc_table[k].val;
    *rate = (uint32_t) (rpc_table[k].parent / rpc_table[k].div);
    cpg_write(offset, val);

    cpg_slogf(_SLOG_INFO, "%s: RPCCKCR 0x%x", __func__, cpg_read(offset));

    return CPG_RET_OK;
}

/*
 * POSTnCKCR clock
 */

static uint32_t cpg_clock_get_post_reg_offset(const uint32_t soc_id, const int idx)
{
    if (soc_id == PRODUCT_ID_RCAR_S4) {
        switch (idx) {
            case 0: return RCAR_GEN4_CPG_POSTCKCR;
            case 2: return RCAR_GEN4_CPG_POST2CKCR;
            default : return CPG_REG_OFF_UNKNOWN;
        }
    } else if ((soc_id == PRODUCT_ID_RCAR_V3U) ||
               (soc_id == PRODUCT_ID_RCAR_V4H)) {
        switch (idx) {
            case 0: return RCAR_GEN4_CPG_POSTCKCR;
            case 2: return RCAR_GEN4_CPG_POST2CKCR;
            case 3: return RCAR_GEN4_CPG_POST3CKCR;
            case 4: return RCAR_GEN4_CPG_POST4CKCR;
            default : return CPG_REG_OFF_UNKNOWN;
        }
    } else if (soc_id == PRODUCT_ID_RCAR_V3H) {
        switch (idx) {
            case 0: return RCAR_GEN3_CPG_POSTCKCR;
            case 2: return RCAR_GEN3_CPG_POST2CKCR;
            case 4: return RCAR_GEN3_CPG_POST4CKCR;
            default : return CPG_REG_OFF_UNKNOWN;
        }
    } else if ((soc_id == PRODUCT_ID_RCAR_V3M) ||
               (soc_id == PRODUCT_ID_RCAR_M3W)) {
        switch (idx) {
            case 0: return RCAR_GEN3_CPG_POSTCKCR;
            case 2: return RCAR_GEN3_CPG_POST2CKCR;
            default : return CPG_REG_OFF_UNKNOWN;
        }
    } else if ((soc_id == PRODUCT_ID_RCAR_H3) ||
               (soc_id == PRODUCT_ID_RCAR_M3N)||
               (soc_id == PRODUCT_ID_RCAR_E3) ||
               (soc_id == PRODUCT_ID_RCAR_D3)) {
        switch (idx) {
            case 0: return RCAR_GEN3_CPG_POSTCKCR;
            case 2: return RCAR_GEN3_CPG_POST2CKCR;
            case 3: return RCAR_GEN3_CPG_POST3CKCR;
            default : return CPG_REG_OFF_UNKNOWN;
        }
    }

    return CPG_REG_OFF_UNKNOWN;
}

/*
 * HDMI, CAN-FD, MSIOF, CSI,... clocks
 */

static uint32_t cpg_clock_get_offset(const uint32_t soc_id, const cpg_clk_id_t id, const int idx)
{
    if (soc_id == PRODUCT_ID_RCAR_V3U) {
        switch (id) {
            case CPG_CLK_CANFD: return RCAR_GEN4_CPG_CANFDCKCR;
            case CPG_CLK_MSIOF: return RCAR_GEN4_CPG_MSOCKCR;
            case CPG_CLK_CSI2:  return RCAR_GEN4_CPG_CSI0CKCR;
            case CPG_CLK_DSI:   return RCAR_GEN4_CPG_DSICKCR;
            case CPG_CLK_IPC:   return RCAR_GEN4_CPG_IPCCKCR;
            case CPG_CLK_FRAY:  return RCAR_GEN4_CPG_FRAYCKCR;
            case CPG_CLK_RPC:   return RCAR_GEN4_CPG_RPCCKCR;
            case CPG_CLK_SDHI:  return cpg_sd_clock_get_ctlr_offset(soc_id, idx);
            case CPG_CLK_POST:  return cpg_clock_get_post_reg_offset(soc_id, idx);
            default: return CPG_REG_OFF_UNKNOWN;
        }
    } else if (soc_id == PRODUCT_ID_RCAR_S4) {
        switch (id) {
            case CPG_CLK_MSIOF: return RCAR_GEN4_CPG_MSOCKCR;
            case CPG_CLK_RPC:   return RCAR_GEN4_CPG_RPCCKCR;
            case CPG_CLK_RSW2:  return RCAR_GEN4_CPG_RSW2CKCR;
            case CPG_CLK_SDHI:  return cpg_sd_clock_get_ctlr_offset(soc_id, idx);
            case CPG_CLK_POST:  return cpg_clock_get_post_reg_offset(soc_id, idx);
            default: return CPG_REG_OFF_UNKNOWN;
        }
    } else if (soc_id == PRODUCT_ID_RCAR_V4H) {
        switch (id) {
            case CPG_CLK_CANFD: return RCAR_GEN4_CPG_CANFDCKCR;
            case CPG_CLK_MSIOF: return RCAR_GEN4_CPG_MSOCKCR;
            case CPG_CLK_CSI2:  return RCAR_GEN4_CPG_CSI0CKCR;
            case CPG_CLK_DSI:   return RCAR_GEN4_CPG_DSICKCR;
            case CPG_CLK_RPC:   return RCAR_GEN4_CPG_RPCCKCR;
            case CPG_CLK_SDHI:  return cpg_sd_clock_get_ctlr_offset(soc_id, idx);
            case CPG_CLK_POST:  return cpg_clock_get_post_reg_offset(soc_id, idx);
            default: return CPG_REG_OFF_UNKNOWN;
        }
    } else {
        switch (id) {
            case CPG_CLK_HDMI:  return RCAR_GEN3_CPG_HDMICKCR;
            case CPG_CLK_CANFD: return RCAR_GEN3_CPG_CANFDCKCR;
            case CPG_CLK_MSIOF: return RCAR_GEN3_CPG_MSOCKCR;
            case CPG_CLK_CSI2:  return RCAR_GEN3_CPG_CSI0CKCR;
            case CPG_CLK_RPC:   return RCAR_GEN3_CPG_RPCCKCR;
            case CPG_CLK_SDHI:  return cpg_sd_clock_get_ctlr_offset(soc_id, idx);
            case CPG_CLK_POST:  return cpg_clock_get_post_reg_offset(soc_id, idx);
            default: return CPG_REG_OFF_UNKNOWN;
        }
    }
}

int clk_config(const cpg_clk_id_t clk_id, const int idx, const int enable)
{
    uint32_t reg_off;
    uint32_t val;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    if (clk_id >= CPG_CLK_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s: No support clock id %d", __func__, clk_id);
        return CPG_RET_INVALPARA;
    }

    cpg_slogf(_SLOG_DEBUG1, "%s: clk_id %d idx %d enable %d",__func__, clk_id, idx, enable);

    switch (clk_id) {
        case CPG_CLK_PLL:
            if (enable) {
                return cpg_pll_clock_enable(idx);
            } else {
                return cpg_pll_clock_disable(idx);
            }
        case CPG_CLK_SDHI:
            return cpg_sd_clock_config(idx, enable);
        case CPG_CLK_RPC:
            return cpg_rpc_clock_config(idx, enable);
        default:
            reg_off = cpg_clock_get_offset(cpg_dev.soc_id, clk_id, idx);
            if (reg_off == CPG_REG_OFF_UNKNOWN) {
                cpg_slogf(_SLOG_ERROR, "%s: No support clock id %d", __func__, clk_id);
                return CPG_RET_INVALPARA;
            }
            val  = cpg_read(reg_off);
            if (enable) {
                val &= ~(CPG_MOD_CKSTP_MASK);
            } else {
                val |= (CPG_MOD_CKSTP_MASK);
            }
            cpg_write(reg_off, val);
    }

    return CPG_RET_OK;
}

static const char cpg_mod_name[][16] = {
    [CPG_CLK_HDMI]  = "HDMI",
    [CPG_CLK_CANFD] = "CANFD",
    [CPG_CLK_MSIOF] = "MSIOF",
    [CPG_CLK_CSI2]  = "CSI2",
    [CPG_CLK_DSI]   = "DSI",
    [CPG_CLK_IPC]   = "IPC",
    [CPG_CLK_FRAY]  = "FRAY",
    [CPG_CLK_RGX]   = "RGX",
    [CPG_CLK_SDHI]  = "SDHI",
    [CPG_CLK_RPC]   = "RPC",
    [CPG_CLK_CPU]   = "CPU",
    [CPG_CLK_PLL]   = "PLL",
    [CPG_CLK_POST]  = "POST",
    [CPG_CLK_RSW2]  = "RSW2"
};

int clk_rate(const cpg_clk_id_t clk_id, const int idx, unsigned *rate, const int set)
{
    uint32_t val, diff = ~0U, tmp;
    uint32_t div = CPG_MOD_DIV_MASK;
    uint32_t reg_off;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    if (clk_id >= CPG_CLK_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s: No support clock id %d", __func__, clk_id);
        return CPG_RET_INVALPARA;
    }

    if (clk_id == CPG_CLK_RSW2) {
        cpg_slogf(_SLOG_ERROR, "%s : %s Clock is fixed, no support this feature", __func__, cpg_mod_name[clk_id]);
        return CPG_RET_INVALPARA;
    }

    if (!rate) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid parameter (rate is NULL)", __func__);
        return CPG_RET_INVALPARA;
    }

    cpg_slogf(_SLOG_DEBUG1, "%s: clk_id %d idx %d rate %d set %d",__func__,
            clk_id, idx, *rate, set);

    if (set == 0) {
        switch (clk_id) {
            case CPG_CLK_PLL:
                return cpg_pll_clock_rate_get(idx, rate);
            case CPG_CLK_CPU:
                return cpg_cpu_clock_rate_get(idx, rate);
            case CPG_CLK_RGX:
                return cpg_rgx_clock_rate_get(idx, rate);
            case CPG_CLK_SDHI:
                return cpg_sd_clock_rate_get(idx, rate);
            case CPG_CLK_RPC:
                return cpg_rpc_clock_rate_get(idx, rate);
            default:
                reg_off = cpg_clock_get_offset(cpg_dev.soc_id, clk_id, idx);
                if (reg_off == CPG_REG_OFF_UNKNOWN) {
                    cpg_slogf(_SLOG_ERROR, "%s: No support %s%d", __func__,
                        cpg_mod_name[clk_id], idx);
                    return CPG_RET_INVALPARA;
                }

                div = cpg_read(reg_off) & CPG_MOD_DIV_MASK;
                *rate = CPG_MOD_PARENT_CLK / (div + 1);
        }
    } else {
        switch (clk_id) {
            case CPG_CLK_PLL:
                return cpg_pll_clock_rate_set(idx, rate);
            case CPG_CLK_CPU:
                return cpg_cpu_clock_rate_set(idx, rate);
            case CPG_CLK_RGX:
                return cpg_rgx_clock_rate_set(idx, rate);
            case CPG_CLK_SDHI:
                return cpg_sd_clock_rate_set(idx, rate);
            case CPG_CLK_RPC:
                return cpg_rpc_clock_rate_set(idx, rate);
            default:
                reg_off = cpg_clock_get_offset(cpg_dev.soc_id, clk_id, idx);
                if (reg_off == CPG_REG_OFF_UNKNOWN) {
                    cpg_slogf(_SLOG_ERROR, "%s: No support %s%d", __func__,
                        cpg_mod_name[clk_id], idx);
                    return CPG_RET_INVALPARA;
                }

                for (uint32_t i = 0; i <= CPG_MOD_DIV_MASK; i++) {
                    tmp = CPG_MOD_PARENT_CLK / (i + 1);
                    tmp = (tmp > *rate) ? (tmp - *rate) : (*rate - tmp);
                    if (tmp < diff) {
                        diff = tmp;
                        div = i;
                    }
                }

                if (div & ~CPG_MOD_DIV_MASK) {
                    cpg_slogf(_SLOG_ERROR, "%s: Invalid rate %dHz", __func__, *rate);
                    return CPG_RET_INVALPARA;
                }

                val = cpg_read(reg_off);
                val &= ~(CPG_MOD_CKSTP_MASK | CPG_MOD_DIV_MASK);
                val |= div;
                cpg_write(reg_off, val);

                *rate = CPG_MOD_PARENT_CLK / (div + 1);

                cpg_slogf(_SLOG_INFO, "%s: %s%dCKCR 0x%x", __func__,
                    cpg_mod_name[clk_id], idx, cpg_read(reg_off));
        }
    }

    return CPG_RET_OK;
}

int clk_divider(const cpg_clk_id_t clk_id, const int idx, unsigned *clk_div, const int set)
{
    uint32_t val;
    uint32_t reg_off;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    if (clk_id >= CPG_CLK_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s: No support clock id %d", __func__, clk_id);
        return CPG_RET_INVALPARA;
    }

    if (clk_id == CPG_CLK_RSW2) {
        cpg_slogf(_SLOG_ERROR, "%s : %s Clock is fixed, no support this feature", __func__, cpg_mod_name[clk_id]);
        return CPG_RET_INVALPARA;
    }

    if (!clk_div) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid parameter (clk_div is NULL)", __func__);
        return CPG_RET_INVALPARA;
    }

    cpg_slogf(_SLOG_DEBUG1, "%s: clk_id %d idx %d clk_div %d set %d",__func__,
            clk_id, idx, *clk_div, set);

    reg_off = cpg_clock_get_offset(cpg_dev.soc_id, clk_id, idx);
    if (reg_off == CPG_REG_OFF_UNKNOWN) {
        cpg_slogf(_SLOG_ERROR, "%s: No support %s%d", __func__, cpg_mod_name[clk_id], idx);
        return CPG_RET_INVALPARA;
    }

    if (set == 0) {
        *clk_div = cpg_read(reg_off) & CPG_MOD_DIV_MASK;
    } else {
        val = cpg_read(reg_off);
        if (clk_id == CPG_CLK_RPC) {
            val &= ~(CPG_RPC_CKSTP_MASK | CPG_RPC_DIV_MASK);
            val |= (*clk_div & CPG_RPC_DIV_MASK);
        } else if (clk_id == CPG_CLK_SDHI) {
            val &= ~(CPG_SD_STP_MASK | CPG_SD_FC_MASK);
            val |= (*clk_div & CPG_SD_FC_MASK);
        } else {
            val &= ~(CPG_MOD_CKSTP_MASK | CPG_MOD_DIV_MASK);
            val |= (*clk_div & CPG_MOD_DIV_MASK);
        }
        cpg_write(reg_off, val);

        cpg_slogf(_SLOG_INFO, "%s: %s%dCKCR 0x%x", __func__,
                    cpg_mod_name[clk_id], idx, cpg_read(reg_off));
    }

    return CPG_RET_OK;
}

int clk_status(const cpg_clk_id_t clk_id, const int idx, int *status)
{
    uint32_t reg_off;
    int ret = 0;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    if (clk_id >= CPG_CLK_UNKNOWN ) {
        cpg_slogf(_SLOG_ERROR, "%s: No support clock id %d", __func__, clk_id);
        return CPG_RET_INVALPARA;
    }

    if (!status) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid parameter (status is NULL)", __func__);
        return CPG_RET_INVALPARA;
    }

    cpg_slogf(_SLOG_DEBUG1, "%s: clk_id %d idx %d",__func__, clk_id, idx);

    switch (clk_id) {
        case CPG_CLK_PLL:
            ret = cpg_pll_clock_is_enabled(idx);
            break;
        case CPG_CLK_SDHI:
            ret = cpg_sd_clock_is_enabled(idx);
            break;
        case CPG_CLK_RPC:
            ret = cpg_rpc_clock_is_enabled(idx);
            break;
        default:
            reg_off = cpg_clock_get_offset(cpg_dev.soc_id, clk_id, idx);
            if (reg_off == CPG_REG_OFF_UNKNOWN) {
                cpg_slogf(_SLOG_ERROR, "%s: No support %s%d", __func__, cpg_mod_name[clk_id], idx);
                return CPG_RET_INVALPARA;
            }
            ret = !(cpg_read(reg_off) & CPG_MOD_CKSTP_MASK);
            break;
    }
    if (ret < 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Failed to get clock id %d status", __func__, clk_id);
        return CPG_RET_ERROR;
    } else {
        *status = ret;
        return CPG_RET_OK;
    }
}

#ifdef VARIANT_pm
///////////////////////////////////////////////////////////////////////////////
// Ported from r-car-dvfs.c...  with modifications...
//
//
// References:
// Renesas R-Car Series, 3rd Generation User Manual: Hardware (r19uh0105ej0200-r-car-3rd-generation.pdf)
//
//
// -----------------------------------------------------------------------------
// This is the Power Manager(PM) for the R-Car Gen3.
//
//      The Operating Performance Points(OPPs) for the Renesas R-Car are
//      altered according to the following schema in this Power Manager (values obtained from r8a7795.dtsi:
//      https://github.com/torvalds/linux/blob/master/arch/arm64/boot/dts/renesas/r8a7795.dtsi)
//
//      OPP           Frequency           Voltage   PLL0_Multiplier   MPU Divider
//
//  Cluster0
//      OPP_LOW          500 MHz            0.83            90           11/32
//      OPP_MID          1000 MHz           0.83            90           21/32
//      OPP_NORM         1500 MHz           0.83            90           32/32
//      OPP_BOOST1       1600 MHz           0.90            96           32/32
//      OPP_BOOST2       1700 MHz           0.96            102          32/32
//      OPP_BOOST3       2000 MHz           0.96            119          32/32
//
//  Cluster1
//      OPP_LOW          800 MHz            0.82            72           21/32
//      OPP_MID          1000 MHz           0.82            72           27/32
//      OPP_NORM         1200 MHz           0.82            72           32/32
//
// -----------------------------------------------------------------------------
// This is the Power Manager(PM) for the R-Car Gen3 M3-W.
//
//      The Operating Performance Points(OPPs) for the Renesas R-Car are
//      altered according to the following schema in this Power Manager (values obtained from r8a7795.dtsi:
//      https://github.com/torvalds/linux/blob/master/arch/arm64/boot/dts/renesas/r8a7796.dtsi)
//
//      OPP           Frequency           Voltage   PLL0_Multiplier   MPU Divider
//
//  Cluster0
//      OPP_NORMAL1       500 MHz           0.83            90           11/32
//      OPP_NORMAL2      1000 MHz           0.83            90           21/32
//      OPP_NORMAL3      1500 MHz           0.83            90           32/32
//      OPP_BOOST1       1600 MHz           0.90            96           32/32
//      OPP_BOOST2       1700 MHz           0.90            102          32/32
//      OPP_BOOST3       1800 MHz           0.96            108          32/32
//      OPP_BOOST4       2000 MHz           0.96            119          32/32
//
//  Cluster1
//      OPP_LOW          800 MHz            0.82            72           21/32
//      OPP_MID          1000 MHz           0.82            72           27/32
//      OPP_NORM         1200 MHz           0.82            72           32/32
//      OPP_BOOST        1300 MHz           0.82            78           32/32
//
//      frequency = ((Base frequency) * 2 * (pll0MpuMultiplier + 1) * (((32 - pllMpuDivider) & 0x1F)/32)) / 2
//
//  Base Frequency for Salvator-XS is 16640000
//  (https://github.com/torvalds/linux/blob/master/arch/arm64/boot/dts/renesas/salvator-xs.dtsi)
// -----------------------------------------------------------------------------

// -------------------------------------------------------------
// Definitions for R-CAR Frequency Control
// -------------------------------------------------------------

// Define the Maximum loop iterations to wait for the kick bit and PLL settling time
#define MAX_WAIT_ITERATIONS              (1000U)        // TODO: Arbitrary value but recommended by Renesas
                                                        // as value used in Linux

//#define BOARD_FREQUENCY                  (16640000U)

#define PLL_CR_MULTIPLIER_BASE_MASK      (0x7FU)
#define PLL_CR_MULTIPLIER_SHIFT          (24)
#define PLL_CR_MULTIPLIER_MASK           (PLL_CR_MULTIPLIER_BASE_MASK << PLL_CR_MULTIPLIER_SHIFT)
#define PLL_CR_MULTIPLIER_MASK_OUT       (~(PLL_CR_MULTIPLIER_BASE_MASK << PLL_CR_MULTIPLIER_SHIFT))

#define PLL0_STATUS_BIT_BASE_MASK        (0x1U)
#define PLL0_STATUS_BIT_SHIFT            (8)
#define PLL0_STATUS_BIT_MASK             (PLL0_STATUS_BIT_BASE_MASK << PLL0_STATUS_BIT_SHIFT)

#define PLL2_STATUS_BIT_BASE_MASK        (0x1U)
#define PLL2_STATUS_BIT_SHIFT            (10)
#define PLL2_STATUS_BIT_MASK             (PLL2_STATUS_BIT_BASE_MASK << PLL2_STATUS_BIT_SHIFT)

#define A57_FRQCRC_DIVIDER_BASE_MASK     (0x1FU)
#define A57_FRQCRC_DIVIDER_SHIFT         (8)
#define A57_FRQCRC_DIVIDER_MASK          (A57_FRQCRC_DIVIDER_BASE_MASK << A57_FRQCRC_DIVIDER_SHIFT)
#define A57_FRQCRC_DIVIDER_MASK_OUT      (~(A57_FRQCRC_DIVIDER_BASE_MASK << A57_FRQCRC_DIVIDER_SHIFT))

#define A53_FRQCRC_DIVIDER_BASE_MASK     (0x1FU)
#define A53_FRQCRC_DIVIDER_SHIFT         (0)
#define A53_FRQCRC_DIVIDER_MASK          (A53_FRQCRC_DIVIDER_BASE_MASK << A53_FRQCRC_DIVIDER_SHIFT)
#define A53_FRQCRC_DIVIDER_MASK_OUT      (~(A53_FRQCRC_DIVIDER_BASE_MASK << A53_FRQCRC_DIVIDER_SHIFT))

#define FRQCRB_KICKBIT_BASE_MASK         (0x1U)
#define FRQCRB_KICKBIT_SHIFT             (31)
#define FRQCRB_KICKBIT_MASK              (FRQCRB_KICKBIT_BASE_MASK << FRQCRB_KICKBIT_SHIFT)

#define DIVIDE_BY_32_SHIFT               (5)   // X Shift right by 5 is X/32

#define CLUSTER_ID_A53                   (0xD03)    /**< cluster_id value is ARM A53 midr part number value */
#define CLUSTER_ID_A57                   (0xD07)    /**< cluster_id value is ARM A57 midr part number value */

#define CLUSTER_IDX                      (0U)
#define FREQ_IDX                         (1U)
#define PLL_MULT_IDX                     (3U)
#define PLL_DIV_IDX                      (4U)

// For some reason, initial PLL multiplier and divider values setup by Uboot code do not
// match the values provided in Renesas RCAR documentation. This results in "measurement"
// values which do not exactly match the listed OPP. This results in "measurement" errors
// that are bounded by +/- 45 MHz (as determined by experimentation).  Thus, a +/- 45 MHz
// error bound is used to select the "correct" starting AVS voltage from the table
#define CPG_FREQ_RANGE 45U

// NOTE: the following table needs to have pll_mult+1 as driver writes pll_mult-1 to register.
static const uint32_t rcar_h3_opp_table[][5] = {
//           cluster,      freq, voltage,  pll_mult,  pll_div
   {  CLUSTER_ID_A57,      2000,    96,       120,       32 },
   {  CLUSTER_ID_A57,      1700,    96,       102,       32 },
   {  CLUSTER_ID_A57,      1600,    90,        96,       32 },
   {  CLUSTER_ID_A57,      1500,    83,        90,       32 },
   {  CLUSTER_ID_A57,      1000,    83,        90,       21 },
   {  CLUSTER_ID_A57,       500,    83,        90,       11 },
   {  CLUSTER_ID_A53,      1200,    83,        72,       32 },
   {  CLUSTER_ID_A53,      1000,    83,        72,       27 },
   {  CLUSTER_ID_A53,       800,    83,        72,       21 },
   {  99,                     0,     0,         0,        0 }  // Marks end of table
};

// NOTE: the following table needs to have pll_mult+1 as driver writes pll_mult-1 to register.
static const uint32_t rcar_m3w_opp_table[][5] = {
//           cluster,      freq, voltage,  pll_mult,  pll_div
   {  CLUSTER_ID_A57,      2000,    96,       120,       32 },
   {  CLUSTER_ID_A57,      1800,    96,       108,       32 },
   {  CLUSTER_ID_A57,      1700,    90,       102,       32 },
   {  CLUSTER_ID_A57,      1600,    90,        96,       32 },
   {  CLUSTER_ID_A57,      1500,    83,        90,       32 },
   {  CLUSTER_ID_A57,      1000,    83,        90,       21 },
   {  CLUSTER_ID_A57,       500,    83,        90,       11 },
   {  CLUSTER_ID_A53,      1300,    83,        78,       32 },
   {  CLUSTER_ID_A53,      1200,    83,        72,       32 },
   {  CLUSTER_ID_A53,      1000,    83,        72,       27 },
   {  CLUSTER_ID_A53,       800,    83,        72,       21 },
   { 99,                      0,     0,         0,        0 }  // Marks end of table
};

static int get_current_cpg_parameters(const unsigned int cluster_id, uint32_t * const cpu_divider, uint32_t * const pll_multiplier,
                                      const uint32_t freq_control_cpu_divider_register, const uint32_t pll_control_register)
{
    if ((NULL == pll_multiplier) || (NULL == cpu_divider)) {
        cpg_slogf(_SLOG_ERROR,"%s: NULL parameters", __func__);
        return CPG_RET_INVALPARA;
    }

    if (CLUSTER_ID_A53 == cluster_id) {
        *cpu_divider = 32U - (((freq_control_cpu_divider_register & A53_FRQCRC_DIVIDER_MASK) >> A53_FRQCRC_DIVIDER_SHIFT) &
                             A53_FRQCRC_DIVIDER_BASE_MASK);
        *pll_multiplier = (((pll_control_register & PLL_CR_MULTIPLIER_MASK) >> PLL_CR_MULTIPLIER_SHIFT) &
                          PLL_CR_MULTIPLIER_BASE_MASK);
    } else if (CLUSTER_ID_A57 == cluster_id) {
        *cpu_divider = 32U - (((freq_control_cpu_divider_register & A57_FRQCRC_DIVIDER_MASK) >> A57_FRQCRC_DIVIDER_SHIFT) &
                             A57_FRQCRC_DIVIDER_BASE_MASK);
        *pll_multiplier = (((pll_control_register & PLL_CR_MULTIPLIER_MASK) >> PLL_CR_MULTIPLIER_SHIFT) &
                         PLL_CR_MULTIPLIER_BASE_MASK);
    } else {
        cpg_slogf(_SLOG_ERROR,"%s: Invalid cluster value!", __func__);
        *cpu_divider = 0;
        *pll_multiplier = 0;
        return CPG_RET_INVALPARA;
    }

    return CPG_RET_OK;
}

static int get_current_frequency(const unsigned int cluster_id, uint32_t * const freq, uint32_t * const cpu_divider, uint32_t * const pll_multiplier)
{
    uint32_t     freq_control_cpu_divider_register;
    uint32_t     pll_control_register;
    uint32_t     current_cpu_divider;
    uint32_t     current_pll_multiplier;

    // Read the current Multiplier and Divider Values for the MPU PLL
    freq_control_cpu_divider_register = in32( cpg_dev.vbase + RCAR_GEN3_CPG_FRQCRC);
    if (CLUSTER_ID_A53 == cluster_id) {
        pll_control_register = in32(cpg_dev.vbase + RCAR_GEN3_CPG_PLL2CR);
    } else if (CLUSTER_ID_A57 == cluster_id) {
        pll_control_register = in32(cpg_dev.vbase + RCAR_GEN3_CPG_PLL0CR);
    } else {
        cpg_slogf(_SLOG_ERROR,"%s: Invalid cluster value!", __func__);
        return CPG_RET_INVALPARA;
    }

    if (CPG_RET_OK != get_current_cpg_parameters(cluster_id, &current_cpu_divider, &current_pll_multiplier, freq_control_cpu_divider_register, pll_control_register)) {
        cpg_slogf(_SLOG_ERROR,"%s: Failed to get CPG parameters", __func__);
        return CPG_RET_ERROR;
    }

    if (CLUSTER_ID_A53 == cluster_id) {
        if (in32(cpg_dev.vbase + RCAR_GEN3_CPG_PLLECR) & (PLL0_STATUS_BIT_MASK)) {
            *freq = cpg_dev.ext_clk * ((current_cpu_divider * (current_pll_multiplier + 1)) >> DIVIDE_BY_32_SHIFT) / (1000 * 1000); // MHz
        } else {
            *freq = (((pll_control_register >> PLL_CR_MULTIPLIER_SHIFT) &
                      PLL_CR_MULTIPLIER_BASE_MASK) + 1) / (1000 * 1000);   // MHz
        }
    } else if (CLUSTER_ID_A57 == cluster_id) {
        if (in32(cpg_dev.vbase + RCAR_GEN3_CPG_PLLECR) & (PLL2_STATUS_BIT_MASK)) {
            *freq = cpg_dev.ext_clk * ((current_cpu_divider * (current_pll_multiplier + 1)) >> DIVIDE_BY_32_SHIFT) / (1000 * 1000); // MHz
        } else {
            *freq = (((pll_control_register >> PLL_CR_MULTIPLIER_SHIFT) &
                      PLL_CR_MULTIPLIER_BASE_MASK) + 1) / (1000 * 1000); // MHz
        }
    } else {
        cpg_slogf(_SLOG_ERROR,"%s: Invalid cluster_id! , %d", __func__, cluster_id);
        return CPG_RET_INVALPARA;
    }

    *cpu_divider = current_cpu_divider;
    *pll_multiplier = current_pll_multiplier;

    cpg_slogf(_SLOG_INFO, "%s: pll_multiplier %u , cpu_divider %u: freq %u", __func__, *pll_multiplier, *cpu_divider, *freq);

    return CPG_RET_OK;
}

//based on rcar_set_cluster_freq
static int cpg_cpu_freq_set_opp(const int idx, uint32_t* const freq_mhz)
{
    const uint32_t (*rcar_opp_table)[5];

    uint32_t        freq_control_cpu_divider_register;
    uint32_t        freq_control_kick_bit_register;
    uint32_t        pll_control_register;
    uint32_t        kick_bit = 1;
    uint32_t        max_wait_iterations = MAX_WAIT_ITERATIONS; // Maximum loop iterations to wait for the kick bit
    struct timespec kick_wait_time;                            // Time to wait before kick bit transition is verified
    uint32_t        pll_multiplier;                            // Current Multiplier value for the MPU DPLL
    uint32_t        cpu_divider;                               // Current Divider value for the MPU DPLL
    uint32_t        new_pll_multiplier;                        // New Multiplier value for the MPU DPLL
    uint32_t        new_cpu_divider;                           // New Divider value for the MPU DPLL
    uint32_t        new_freq_control_cpu_divider_register;
    uint32_t        new_pll_control_register;
    uint32_t        pll_status;
    uint8_t         new_opp;
    unsigned int    cluster_id;

    if (PRODUCT_ID_RCAR_H3 == cpg_dev.soc_id) {
        cpg_slogf(_SLOG_DEBUG1, "%s: RCAR H3", __func__);
        rcar_opp_table = rcar_h3_opp_table;
    } else if (PRODUCT_ID_RCAR_M3W == cpg_dev.soc_id) {
        cpg_slogf(_SLOG_DEBUG1, "%s: RCAR M3W", __func__);
        rcar_opp_table = rcar_m3w_opp_table;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Invalid soc_id %u", __func__, cpg_dev.soc_id);
        return CPG_RET_ERROR;
    }

    if (1 == idx) {
        cpg_slogf(_SLOG_DEBUG1, "%s: A57, requested freq %u MHz", __func__, *freq_mhz);
        cluster_id = CLUSTER_ID_A57;
    } else if (0 == idx) {
        cpg_slogf(_SLOG_DEBUG1, "%s: A53, requested freq %u MHz", __func__, *freq_mhz);
        cluster_id = CLUSTER_ID_A53;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Invalid idx %d", __func__, idx);
        return CPG_RET_ERROR;
    }

    if (NULL == freq_mhz) {
        cpg_slogf(_SLOG_ERROR, "%s: NULL parameter", __func__);
        return CPG_RET_ERROR;
    }

    uint32_t freq = *freq_mhz;

    uint8_t index = 0;
    do {
        if ((freq >= (rcar_opp_table[index][FREQ_IDX] - CPG_FREQ_RANGE)) &&
            (freq <= (rcar_opp_table[index][FREQ_IDX] + CPG_FREQ_RANGE)) &&
            (rcar_opp_table[index][CLUSTER_IDX] == cluster_id)) {
            break;
        } else {
            index++;
        }
    } while (rcar_opp_table[index][FREQ_IDX] != 0U);

    if (rcar_opp_table[index][FREQ_IDX] != 0U) {
        freq = rcar_opp_table[index][FREQ_IDX];
        cpg_slogf(_SLOG_INFO, "%s: freq = %u MHz", __func__, freq);
    } else {
        cpg_slogf(_SLOG_ERROR,"%s: Unsupported OPP for %u MHz", __func__, freq);
        return CPG_RET_INVALPARA;
    }

    new_cpu_divider = rcar_opp_table[index][PLL_DIV_IDX];
    new_pll_multiplier = rcar_opp_table[index][PLL_MULT_IDX];
    new_opp = index;

    *freq_mhz = freq;
    cpg_slogf(_SLOG_INFO,"%s: Requested OPP is %u, frequency is %u", __func__, new_opp,freq);

    // Read the current Multiplier and Divider Values for the MPU PLL
    freq_control_cpu_divider_register = in32( cpg_dev.vbase + RCAR_GEN3_CPG_FRQCRC);
    if (CLUSTER_ID_A53 == cluster_id) {
        pll_control_register = in32( cpg_dev.vbase +  RCAR_GEN3_CPG_PLL2CR);
    } else if (CLUSTER_ID_A57 == cluster_id) {
        pll_control_register = in32( cpg_dev.vbase +  RCAR_GEN3_CPG_PLL0CR);
    } else {
        cpg_slogf(_SLOG_ERROR,"%s: Invalid cluster value!",__func__);
        return CPG_RET_INVALPARA;
    }

    if (CPG_RET_OK != get_current_cpg_parameters(cluster_id, &cpu_divider, &pll_multiplier, freq_control_cpu_divider_register, pll_control_register)) {
        cpg_slogf(_SLOG_ERROR,"%s: Failed to get CPG parameters", __func__);
        return CPG_RET_ERROR;
    }

    // check pll_multiplier+1 as driver writes pll_multiplier-1 to register.
    // new_pll_multiplier is directly from table (table already has pll_multiplier+1)
    if ((pll_multiplier+1 == new_pll_multiplier) && (cpu_divider == new_cpu_divider)) {
        cpg_slogf(_SLOG_DEBUG1,"%s: Req freq is same as current freq - no change needed", __func__);
        return CPG_RET_OK;
    } else {
        cpg_slogf(_SLOG_DEBUG1,"%s: Current cpu_divider %u, pll_multiplier %u", __func__, cpu_divider, pll_multiplier);
        cpg_slogf(_SLOG_DEBUG1,"%s: Target cpu_divider %u, pll_multiplier %u", __func__, new_cpu_divider, (new_pll_multiplier - 1));
    }

    cpg_slogf(_SLOG_DEBUG2,"%s: opp = %u", __func__, new_opp);

    // Set the new MPU DPLL Multiplier and Divider Values based on the requested Operating Performance Point
    // ----------------------------------------------------------------------------
    // 1. Set the new Multiplier Values for the MPU PLL
    // ----------------------------------------------------------------------------

    // Create the new Multiplier and Divider Values
    if (pll_multiplier != new_pll_multiplier) {
        new_pll_control_register = ((pll_control_register & PLL_CR_MULTIPLIER_MASK_OUT)|
                                  (((new_pll_multiplier - 1) << PLL_CR_MULTIPLIER_SHIFT) & PLL_CR_MULTIPLIER_MASK));

        uint32_t max_iterations = 0;

        if (CLUSTER_ID_A53 == cluster_id) {
            // Should only do this at initialization for H3 A53 cluster, should never
            // change PLL multiplier for H3 A53 cluster after init.  M3 A53 cluster has a
            // boosted mode which will require change to PLL multiplier

            // Write the new DPLL Multiplier values to the CPG register (PLL2CR)
            out32(cpg_dev.vbase + RCAR_GEN3_CPG_CPGWPCR, ~new_pll_control_register);  // write inverse of new multiplier value
                                                                              // to unlock CPG writes
            out32(cpg_dev.vbase + RCAR_GEN3_CPG_PLL2CR, new_pll_control_register);  // write new multiplier value
            do  {
                pll_status = in32(cpg_dev.vbase + RCAR_GEN3_CPG_PLLECR) & PLL2_STATUS_BIT_MASK;
                max_iterations++;
            } while ((pll_status == 0) && (max_iterations < MAX_WAIT_ITERATIONS));
        } else if (CLUSTER_ID_A57 == cluster_id) {
            // Write the new DPLL Multiplier values to the CPG register (PLL0CR)
            out32(cpg_dev.vbase + RCAR_GEN3_CPG_CPGWPCR, ~new_pll_control_register);  // write inverse of new multiplier value
                                                                              // to unlock CPG writes
            out32(cpg_dev.vbase + RCAR_GEN3_CPG_PLL0CR, new_pll_control_register);  // write new multiplier value
            do  {
                pll_status = in32(cpg_dev.vbase + RCAR_GEN3_CPG_PLLECR) & PLL0_STATUS_BIT_MASK;
                max_iterations++;
            } while ((pll_status == 0) && (max_iterations < MAX_WAIT_ITERATIONS));
        } else {
            cpg_slogf(_SLOG_ERROR,"%s: Invalid cluster value!", __func__);
            return CPG_RET_INVALPARA;
        }

        if (max_iterations == MAX_WAIT_ITERATIONS) {
            cpg_slogf(_SLOG_ERROR,"%s: Oscillator did not settle", __func__);
        }
    }

    // ----------------------------------------------------------------------------
    // 2. Set the new Divider Values for the MPU PLL
    // ----------------------------------------------------------------------------

    if (cpu_divider != new_cpu_divider) {
        max_wait_iterations = 0;
        kick_wait_time.tv_sec = 0;
        kick_wait_time.tv_nsec = 250;  // 250 ns, TODO: arbitrary value

        // Cannot change the divider value until kick_bit = 0 so check first (although
        // it should be 0 at this point)
        // Do a nanosleep() between iterations to allow OS to reschedule other tasks if needed

        do {
            freq_control_kick_bit_register = in32( cpg_dev.vbase + RCAR_GEN3_CPG_FRQCRB);
            kick_bit = (freq_control_kick_bit_register >> FRQCRB_KICKBIT_SHIFT) & FRQCRB_KICKBIT_BASE_MASK;
            if (kick_bit != 0U) {
                nanosleep(&kick_wait_time, NULL);
            }
            max_wait_iterations++;
        } while ((kick_bit == 1U) && (MAX_WAIT_ITERATIONS > max_wait_iterations));

        if (kick_bit == 1) {
            cpg_slogf(_SLOG_ERROR,"%s: Unable to change divider value, kick_bit is not cleared!", __func__);
            return CPG_RET_ERROR;
        }

        if (CLUSTER_ID_A53 == cluster_id) {
            new_freq_control_cpu_divider_register = ((freq_control_cpu_divider_register & A53_FRQCRC_DIVIDER_MASK_OUT)|
                                               (((32 - new_cpu_divider) << A53_FRQCRC_DIVIDER_SHIFT) & A53_FRQCRC_DIVIDER_MASK));
        } else if (CLUSTER_ID_A57 == cluster_id) {
            new_freq_control_cpu_divider_register = ((freq_control_cpu_divider_register & A57_FRQCRC_DIVIDER_MASK_OUT)|
                                               (((32 - new_cpu_divider) << A57_FRQCRC_DIVIDER_SHIFT) & A57_FRQCRC_DIVIDER_MASK));
        } else {
            cpg_slogf(_SLOG_ERROR,"%s: Invalid cluster value!", __func__);
            return CPG_RET_INVALPARA;
        }

        // Write the new CPU Divider values to the CPG register
        out32(cpg_dev.vbase + RCAR_GEN3_CPG_CPGWPCR, ~new_freq_control_cpu_divider_register);  // write inverse of new divider value
                                                                                     // to unlock CPG writes
        out32(cpg_dev.vbase + RCAR_GEN3_CPG_FRQCRC, new_freq_control_cpu_divider_register);

        // And for the Kick!
        out32(cpg_dev.vbase + RCAR_GEN3_CPG_CPGWPCR, ~(FRQCRB_KICKBIT_MASK | freq_control_kick_bit_register));  // write inverse of FRQCRB value
                                                                                                           // to unlock CPG writes
        out32(cpg_dev.vbase + RCAR_GEN3_CPG_FRQCRB, (FRQCRB_KICKBIT_MASK | freq_control_kick_bit_register));

        // Check the KICK bit in FRQCRB register first, spin until kick bit = 0 or MAX_WAIT_ITERATIONS.
        // Do a nanosleep() between iterations to allow OS to reschedule other tasks if needed

        kick_wait_time.tv_sec = 0;
        kick_wait_time.tv_nsec = 250; // TODO: purely arbitrary value
        max_wait_iterations = 0;

        do {
            freq_control_kick_bit_register = in32( cpg_dev.vbase + RCAR_GEN3_CPG_FRQCRB);
            kick_bit = (freq_control_kick_bit_register >> FRQCRB_KICKBIT_SHIFT) & FRQCRB_KICKBIT_BASE_MASK;
            if (kick_bit != 0U) {
                nanosleep(&kick_wait_time, NULL);
            }
            max_wait_iterations++;
        } while ((kick_bit == 1U) && (MAX_WAIT_ITERATIONS > max_wait_iterations));

        if (kick_bit != 0) {
            cpg_slogf(_SLOG_ERROR,"%s: Unable to verify new frequency setting, kick_bit not cleared!", __func__);
            return CPG_RET_ERROR;
        }
    }

    cpg_slogf(_SLOG_INFO,"%s: Frequency changed to %u", __func__, freq);
    cpg_slogf(_SLOG_DEBUG1,"%s: opp = %u", __func__, new_opp);

    return CPG_RET_OK;
}

//based on rcar_get_cluster_freq
static int cpg_cpu_freq_get(const int idx, uint32_t* const freq_mhz)
{
    const uint32_t (*rcar_opp_table)[5];  // Will be assigned to either h3 or m3w tables

    uint32_t     pll_multiplier;
    uint32_t     cpu_divider;
    unsigned int cluster_id;

    if (PRODUCT_ID_RCAR_H3 == cpg_dev.soc_id) {
        cpg_slogf(_SLOG_DEBUG1, "%s: RCAR H3", __func__);
        rcar_opp_table = rcar_h3_opp_table;
    } else if (PRODUCT_ID_RCAR_M3W == cpg_dev.soc_id) {
        cpg_slogf(_SLOG_DEBUG1, "%s: RCAR M3W", __func__);
        rcar_opp_table = rcar_m3w_opp_table;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Invalid soc_id %u", __func__, cpg_dev.soc_id);
        return CPG_RET_ERROR;
    }

    if (1 == idx) {
        cpg_slogf(_SLOG_DEBUG1, "%s: A57, get freq in MHz", __func__);
        cluster_id = CLUSTER_ID_A57;
    } else if (0 == idx) {
        cpg_slogf(_SLOG_DEBUG1, "%s: A53, get freq in MHz", __func__);
        cluster_id = CLUSTER_ID_A53;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Invalid idx %d", __func__, idx);
        return CPG_RET_ERROR;
    }

    if (NULL == freq_mhz) {
        cpg_slogf(_SLOG_ERROR,"%s: Invalid input parameter value!", __func__);
        return CPG_RET_INVALPARA;
    }

    if (CPG_RET_OK != get_current_frequency(cluster_id, freq_mhz, &cpu_divider, &pll_multiplier)) {
        cpg_slogf(_SLOG_ERROR,"%s: Failed to get frequency parameters", __func__);
        return CPG_RET_INVALPARA;
    }

    uint8_t index = 0;

    do {
        if ((cluster_id == rcar_opp_table[index][CLUSTER_IDX]) &&
            (cpu_divider == rcar_opp_table[index][PLL_DIV_IDX]) &&
           (pll_multiplier == (rcar_opp_table[index][PLL_MULT_IDX] - 1))) {
            break;
        } else {
            index++;
        }
    } while (rcar_opp_table[index][FREQ_IDX] != 0U);

    if (rcar_opp_table[index][FREQ_IDX] != 0U) {
        *freq_mhz = rcar_opp_table[index][FREQ_IDX]; // Report in MHz (table is MHz)
        cpg_slogf(_SLOG_INFO, "%s: freq for opp %u is %u, cpu_divider %u, pll_multiplier %u", __func__, index, *freq_mhz,cpu_divider, pll_multiplier);
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Unsupported OPP - Mult: %u  Div: %u, freq calc'd %u", __func__, pll_multiplier, cpu_divider, *freq_mhz);
        cpg_slogf(_SLOG_ERROR,"%s: Will not be able to verify the voltage setting", __func__);
        return CPG_RET_OK;  // Not necessarily an error, just means settings do not match Renesas documentiation
                     // Seems to happen when uboot sets initial settings
    }

    return CPG_RET_OK;
}


// ...Ported from r-car-dvfs.c
///////////////////////////////////////////////////////////////////////////////


static int cpg_cpu_freq_get_opp(const int cluster_idx, const uint32_t freq, const uint32_t freq_rng, uint32_t* const opp)
{
    const uint32_t (*rcar_opp_table)[5];

    if (PRODUCT_ID_RCAR_H3 == cpg_dev.soc_id) {
        cpg_slogf(_SLOG_DEBUG1, "%s: RCAR H3", __func__);
        rcar_opp_table = rcar_h3_opp_table;
    } else if (PRODUCT_ID_RCAR_M3W == cpg_dev.soc_id) {
        cpg_slogf(_SLOG_DEBUG1, "%s: RCAR M3W", __func__);
        rcar_opp_table = rcar_m3w_opp_table;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Invalid soc_id %u", __func__, cpg_dev.soc_id);
        return CPG_RET_INVALPARA;
    }

    unsigned int cluster_id;

    if (1 == cluster_idx) {
        cpg_slogf(_SLOG_INFO, "%s: A57, get opp for freq %u (freq range +/- %u)", __func__, freq, freq_rng);
        cluster_id = CLUSTER_ID_A57;
    } else if (0 == cluster_idx) {
        cpg_slogf(_SLOG_INFO, "%s: A53, get opp for freq %u (freq range +/- %u)", __func__, freq, freq_rng);
        cluster_id = CLUSTER_ID_A53;
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Error: Invalid cluster_idx %d", __func__, cluster_idx);
        return CPG_RET_INVALPARA;
    }

    if (NULL == opp) {
        cpg_slogf(_SLOG_ERROR,"%s: Invalid input parameter value!", __func__);
        return CPG_RET_INVALPARA;
    }

    if (CPG_FREQ_RANGE < freq_rng) {
        cpg_slogf(_SLOG_ERROR, "%s: ERROR - requested freq range %u > %u (limit)", __func__, freq_rng, CPG_FREQ_RANGE);
        return CPG_RET_INVALPARA;
    }

    uint32_t idx = 0;
    do {
        if ((freq >= (rcar_opp_table[idx][FREQ_IDX] - freq_rng)) &&
            (freq <= (rcar_opp_table[idx][FREQ_IDX] + freq_rng)) &&
            (rcar_opp_table[idx][CLUSTER_IDX] == cluster_id)) {
            break;
        } else {
            idx++;
        }
    } while (rcar_opp_table[idx][FREQ_IDX] != 0U);

    if (rcar_opp_table[idx][FREQ_IDX] == 0U) {
        cpg_slogf(_SLOG_ERROR, "%s: Unsupported OPP: freq %u (freq range +/- %u)", __func__, freq, freq_rng);
        return CPG_RET_INVALPARA;
    }
    else {
        *opp = idx;
        const uint32_t freq_temp = rcar_opp_table[idx][FREQ_IDX];
        cpg_slogf(_SLOG_INFO, "%s: OPP %u: freq %u (freq range +/- %u)", __func__, *opp, freq_temp, freq_rng);
    }

    return CPG_RET_OK;
}


int clk_freq(const cpg_clk_id_t clk_id, const int idx, unsigned *freq_mhz, const int set)
{
    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev", __func__);
        return CPG_RET_INVALPARA;
    }

    if (CPG_CLK_CPU != clk_id) {
        cpg_slogf(_SLOG_ERROR, "%s: No support clock id %d", __func__, clk_id);
        return CPG_RET_INVALPARA;
    }

    if (NULL == freq_mhz) {
        cpg_slogf(_SLOG_ERROR, "%s: NULL parameter", __func__);
        return CPG_RET_INVALPARA;
    }

    int ret_int = CPG_RET_OK;
    if (set == 0) {
        ret_int = cpg_cpu_freq_get(idx, freq_mhz);
        cpg_slogf(_SLOG_INFO, "%s: cluster %u, get freq %u (MHz)", __func__, idx, *freq_mhz);
    } else {
        cpg_slogf(_SLOG_INFO, "%s: cluster %u, set freq %u (MHz)", __func__, idx, *freq_mhz);
        ret_int = cpg_cpu_freq_set_opp(idx, freq_mhz);
    }

    return ret_int;
}


int clk_opp(const cpg_clk_id_t clk_id, const int idx, const uint32_t freq, const uint32_t freq_rng, uint32_t* const opp)
{
    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev", __func__);
        return CPG_RET_INVALPARA;
    }

    if (CPG_CLK_CPU != clk_id) {
        cpg_slogf(_SLOG_ERROR, "%s: No support clock id %d", __func__, clk_id);
        return CPG_RET_INVALPARA;
    }

    if (NULL == opp) {
        cpg_slogf(_SLOG_ERROR, "%s: NULL parameter", __func__);
        return CPG_RET_INVALPARA;
    }

    const int ret_int = cpg_cpu_freq_get_opp(idx, freq, freq_rng, opp);
    if (CPG_RET_OK == ret_int) {
        cpg_slogf(_SLOG_INFO, "%s: cluster %u, OPP %u for freq %u", __func__, idx, *opp, freq);
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: cluster %u, get OPP error %d", __func__, idx, ret_int);
    }

    return ret_int;
}
#endif // VARIANT_pm

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/mainline/lib/hw_vendor/renesas/rcar/cpg-support/clk.c $ $Rev: 974071 $")
#endif
