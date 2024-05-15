/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#include "proto.h"

typedef struct sysc_power_domain_ {
    int     pwr_idx;
    char    name[16];       /* Power domain name */
    uint8_t chan_idx;       /* Index of PWR*R */
    uint8_t chan_bit;       /* Bit in PWR* (except for PWRUP in PWRSR) */
    uint8_t on_status;      /* Status PWRUP bit in PWRSR */
    uint8_t isr_idx;
    uint8_t isr_bit;        /* Bit in SYSCI*R */
    int     pdmode_1;       /* Point to parent domain in Power Domain Mode 1 for V3H 2.0/2.1 only (-1 value doesn't support)  */
    int     pdmode_3;       /* Point to parent domain in Power Domain Mode 3 for V3H 2.0/2.1 only (-1 value doesn't support) */
} sysc_power_domain_t;

#define SYSC_PD(_idx, _name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3) \
            { \
                .pwr_idx   = _idx,       \
                .name      =  #_name,    \
                .chan_idx  = _ch_idx,    \
                .chan_bit  = _ch_bit,    \
                .on_status = _sts,       \
                .isr_idx   = _isr_idx,   \
                .isr_bit   = _isr_bit,   \
                .pdmode_1  = _pdm1,      \
                .pdmode_3  = _pdm3,      \
            }
#define SYSC_PD_GEN3(_name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3) \
            SYSC_PD(SYSC_PDR_GEN3_##_name, _name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3)
#define SYSC_PD_V3U(_name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3) \
            SYSC_PD(SYSC_PDR_V3U_##_name, _name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3)


static const sysc_power_domain_t h3_sysc_power_domain[] = {
    /*            name    chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    SYSC_PD_GEN3(CA57_SCU,5,        0,        4,         0,       12,     -1,    -1),
    SYSC_PD_GEN3(CA53_SCU,3,        0,        4,         0,       21,     -1,    -1),
    SYSC_PD_GEN3(CR7,     7,        0,        4,         0,       13,     -1,    -1),
    SYSC_PD_GEN3(A3IR,    4,        0,        4,         0,       24,     -1,    -1),
    SYSC_PD_GEN3(A3VP,    8,        0,        4,         0,        9,     -1,    -1),
    SYSC_PD_GEN3(A3VC,    9,        0,        4,         0,       14,     -1,    -1),
    SYSC_PD_GEN3(A2VC1,  10,        1,        2,         0,       26,     -1,    -1),
    SYSC_PD_GEN3(3DGA,    2,        0,        5,         0,       17,     -1,    -1),
    SYSC_PD_GEN3(3DGB,    2,        1,        6,         0,       18,     -1,    -1),
    SYSC_PD_GEN3(3DGC,    2,        2,        7,         0,       19,     -1,    -1),
    SYSC_PD_GEN3(3DGD,    2,        3,        8,         0,       20,     -1,    -1),
    SYSC_PD_GEN3(3DGE,    2,        4,        9,         0,       22,     -1,    -1),

};

static const sysc_power_domain_t m3w_sysc_power_domain[] = {
    /*            name    chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    SYSC_PD_GEN3(CA57_SCU,5,        0,        4,         0,       12,     -1,    -1),
    SYSC_PD_GEN3(CA53_SCU,3,        0,        4,         0,       21,     -1,    -1),
    SYSC_PD_GEN3(CR7,     7,        0,        4,         0,       13,     -1,    -1),
    SYSC_PD_GEN3(A3IR,    4,        0,        4,         0,       24,     -1,    -1),
    SYSC_PD_GEN3(A3VC,    9,        0,        4,         0,       14,     -1,    -1),
    SYSC_PD_GEN3(A2VC0,  10,        0,        2,         0,       25,     -1,    -1),
    SYSC_PD_GEN3(A2VC1,  10,        1,        3,         0,       26,     -1,    -1),
    SYSC_PD_GEN3(3DGA,    2,        0,        5,         0,       17,     -1,    -1),
    SYSC_PD_GEN3(3DGB,    2,        1,        6,         0,       18,     -1,    -1),
};

static const sysc_power_domain_t m3n_sysc_power_domain[] = {
    /*            name    chan_idx  chan_bit  on_status  isr_idx  isr_bit */
    SYSC_PD_GEN3(CA57_SCU,5,        0,        4,         0,       12,     -1,    -1),
    SYSC_PD_GEN3(CR7,     7,        0,        4,         0,       13,     -1,    -1),
    SYSC_PD_GEN3(A3VP,    8,        0,        4,         0,       9 ,     -1,    -1),
    SYSC_PD_GEN3(A3VC,    9,        0,        4,         0,       14,     -1,    -1),
    SYSC_PD_GEN3(A2VC1,  10,        1,        2,         0,       26,     -1,    -1),
    SYSC_PD_GEN3(3DGA,    2,        0,        5,         0,       17,     -1,    -1),
    SYSC_PD_GEN3(3DGB,    2,        1,        6,         0,       18,     -1,    -1),
};

static const sysc_power_domain_t e3_sysc_power_domain[] = {
    /*           name     chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    SYSC_PD_GEN3(CA53_SCU,3,        0,        4,         0,       21,     -1,    -1),
    SYSC_PD_GEN3(CR7,     7,        0,        4,         0,       13,     -1,    -1),
    SYSC_PD_GEN3(A3VC,    9,        0,        4,         0,       14,     -1,    -1),
    SYSC_PD_GEN3(A2VC1,  10,        1,        2,         0,       26,     -1,    -1),
    SYSC_PD_GEN3(3DGA,    2,        0,        5,         0,       17,     -1,    -1),
    SYSC_PD_GEN3(3DGB,    2,        1,        6,         0,       18,     -1,    -1),
};

static const sysc_power_domain_t d3_sysc_power_domain[] = {
    /*           name     chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    SYSC_PD_GEN3(CA53_SCU,3,        0,        4,         0,       21,     -1,    -1),
};

static const sysc_power_domain_t v3m_sysc_power_domain[] = {
    /*           name     chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    SYSC_PD_GEN3(CA53_SCU,3,        0,        4,         0,       21,     -1,    -1),
    SYSC_PD_GEN3(CR7,     7,        0,        4,         0,       13,     -1,    -1),
    SYSC_PD_GEN3(A3IR,    4,        0,        4,         0,       24,     -1,    -1),
    SYSC_PD_GEN3(A2IR0,  11,        0,        6,         0,       23,     -1,    -1),
    SYSC_PD_GEN3(A2IR1,  11,        1,        7,         0,       27,     -1,    -1),
    SYSC_PD_GEN3(A2SC0,  11,        4,       10,         0,       30,     -1,    -1),
    SYSC_PD_GEN3(A2SC1,  11,        5,       11,         0,       31,     -1,    -1),
    SYSC_PD_GEN3(A2DP,   11,        2,        8,         0,       28,     -1,    -1),
    SYSC_PD_GEN3(A2CN0,  11,        3,        9,         0,       29,     -1,    -1),
};

static const sysc_power_domain_t v3h_sysc_power_domain[] = {
    /*           name     chan_idx  chan_bit  on_status  isr_idx  isr_bit       pdmode_1               pdmode_3  */
    SYSC_PD_GEN3(CA53_SCU,3,        0,        4,         0,       21,       -1,                        -1                 ),
    SYSC_PD_GEN3(CR7,     7,        0,        4,         0,       13,       -1,                        -1                 ),
    SYSC_PD_GEN3(A3IR,    4,        0,        4,         0,       24,       SYSC_PDR_GEN3_A3IR,        SYSC_PDR_GEN3_A3IR ),
    SYSC_PD_GEN3(A2IR0,   11,       0,       14,         0,       23,       SYSC_PDR_GEN3_A2IR0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2IR1,   11,       1,       15,         0,       27,       SYSC_PDR_GEN3_A2IR0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2IR2,   11,       2,       16,         0,       28,       SYSC_PDR_GEN3_A2IR0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2IR3,   11,       3,       17,         0,       29,       SYSC_PDR_GEN3_A2IR0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2IR4,   11,       4,       18,         0,       15,       SYSC_PDR_GEN3_A2IR0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2IR5,   11,       5,       19,         0,       12,       SYSC_PDR_GEN3_A2IR5,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2SC0,   11,       6,       20,         0,       30,       SYSC_PDR_GEN3_A2SC0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2SC1,   11,       7,       21,         0,       31,       SYSC_PDR_GEN3_A2SC0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2SC2,   11,       8,       22,         0,        0,       SYSC_PDR_GEN3_A2SC0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2SC3,   11,       9,       23,         0,        1,       SYSC_PDR_GEN3_A2SC0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2SC4,   11,      10,       24,         0,        2,       SYSC_PDR_GEN3_A2SC0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2DP0,   11,      11,       25,         0,        3,       SYSC_PDR_GEN3_A2DP0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2DP1,   11,      12,       26,         0,        4,       SYSC_PDR_GEN3_A2DP0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A2CN0,   11,      13,       27,         0,       10,       SYSC_PDR_GEN3_A2CN0,       SYSC_PDR_GEN3_A2IR0),
    SYSC_PD_GEN3(A3VIP0,  12,       0,        4,         0,       11,       -1,                        -1                 ),
    SYSC_PD_GEN3(A3VIP1,  13,       0,        4,         0,       25,       -1,                        -1                 ),
    SYSC_PD_GEN3(A3VIP2,  14,       0,        4,         0,       26,       -1,                        -1                 ),
};

static const sysc_power_domain_t v3u_sysc_power_domain[] = {
    /*           name      chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    SYSC_PD_V3U(A1E0D0C0,  0,       0,         4,        0,       0,     -1,    -1),
    SYSC_PD_V3U(A1E0D0C1,  1,       0,         4,        0,       1,     -1,    -1),
    SYSC_PD_V3U(A1E0D1C0,  2,       0,         4,        0,       2,     -1,    -1),
    SYSC_PD_V3U(A1E0D1C1,  3,       0,         4,        0,       3,     -1,    -1),
    SYSC_PD_V3U(A1E1D0C0,  4,       0,         4,        0,       4,     -1,    -1),
    SYSC_PD_V3U(A1E1D0C1,  5,       0,         4,        0,       5,     -1,    -1),
    SYSC_PD_V3U(A1E1D1C0,  6,       0,         4,        0,       6,     -1,    -1),
    SYSC_PD_V3U(A1E1D1C1,  7,       0,         4,        0,       7,     -1,    -1),
    SYSC_PD_V3U(A2E0D1,   17,       0,         4,        0,      17,     -1,    -1),
    SYSC_PD_V3U(A2E0D0,   16,       0,         4,        0,      16,     -1,    -1),
    SYSC_PD_V3U(A2E1D0,   18,       0,         4,        0,      18,     -1,    -1),
    SYSC_PD_V3U(A2E1D1,   19,       0,         4,        0,      19,     -1,    -1),
    SYSC_PD_V3U(A3E0,     20,       0,         4,        0,      20,     -1,    -1),
    SYSC_PD_V3U(A3E1,     21,       0,         4,        0,      21,     -1,    -1),
    SYSC_PD_V3U(3DGA,     24,       0,         4,        0,      24,     -1,    -1),
    SYSC_PD_V3U(3DGB,     25,       0,         4,        0,      25,     -1,    -1),
    SYSC_PD_V3U(A1CNN2,   32,       0,         4,        1,       0,     -1,    -1),
    SYSC_PD_V3U(A1DSP0,   33,       0,         4,        1,       1,     -1,    -1),
    SYSC_PD_V3U(A2IMP0,   34,       0,         4,        1,       2,     -1,    -1),
    SYSC_PD_V3U(A2DP0,    35,       0,         4,        1,       3,     -1,    -1),
    SYSC_PD_V3U(A2CV0,    36,       0,         4,        1,       4,     -1,    -1),
    SYSC_PD_V3U(A2CV1,    37,       0,         4,        1,       5,     -1,    -1),
    SYSC_PD_V3U(A2CV4,    38,       0,         4,        1,       6,     -1,    -1),
    SYSC_PD_V3U(A2CV6,    39,       0,         4,        1,       7,     -1,    -1),
    SYSC_PD_V3U(A2CN2,    40,       0,         4,        1,       8,     -1,    -1),
    SYSC_PD_V3U(A1CNN0,   41,       0,         4,        1,       9,     -1,    -1),
    SYSC_PD_V3U(A2CN0,    42,       0,         4,        1,      10,     -1,    -1),
    SYSC_PD_V3U(A3IR,     43,       0,         4,        1,      11,     -1,    -1),
    SYSC_PD_V3U(A1CNN1,   44,       0,         4,        1,      12,     -1,    -1),
    SYSC_PD_V3U(A1DSP1,   45,       0,         4,        1,      13,     -1,    -1),
    SYSC_PD_V3U(A2IMP2,   46,       0,         4,        1,      14,     -1,    -1),
    SYSC_PD_V3U(A2DP1,    47,       0,         4,        1,      15,     -1,    -1),
    SYSC_PD_V3U(A2CV2,    48,       0,         4,        1,      16,     -1,    -1),
    SYSC_PD_V3U(A2CV3,    49,       0,         4,        1,      17,     -1,    -1),
    SYSC_PD_V3U(A2CV5,    50,       0,         4,        1,      18,     -1,    -1),
    SYSC_PD_V3U(A2CV7,    51,       0,         4,        1,      19,     -1,    -1),
    SYSC_PD_V3U(A2CN1,    52,       0,         4,        1,      20,     -1,    -1),
    SYSC_PD_V3U(A3VIP0,   56,       0,         4,        1,      24,     -1,    -1),
    SYSC_PD_V3U(A3VIP1,   57,       0,         4,        1,      25,     -1,    -1),
    SYSC_PD_V3U(A3VIP2,   58,       0,         4,        1,      26,     -1,    -1),
    SYSC_PD_V3U(A3VIP3,   59,       0,         4,        1,      27,     -1,    -1),
    SYSC_PD_V3U(A3ISP01,  60,       0,         4,        1,      28,     -1,    -1),
    SYSC_PD_V3U(A3ISP23,  61,       0,         4,        1,      29,     -1,    -1),
};

/* For each SYSC domain */
#define SYSC_PWRSR            (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? \
                                          RCAR_V3U_SYSC_PDRSR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWRSRx(power_domain->chan_idx))    /* Power Status Register */
#define SYSC_PWROFFCR         (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? \
                                          RCAR_V3U_SYSC_PDROFFCR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWROFFCRx(power_domain->chan_idx)) /* Power Shutoff Control Register */
#define SYSC_PWRONCR          (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? \
                                          RCAR_V3U_SYSC_PDRONCR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWRONCRx(power_domain->chan_idx))  /* Power Resume Control Register */
#define SYSC_PWRER            (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? \
                                          RCAR_V3U_SYSC_PDRESR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWRERx(power_domain->chan_idx))    /* Power Shutoff/Resume Error */
/* For common */
#define SYSC_SR               (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? RCAR_V3U_SYSC_SR : RCAR_GEN3_SYSC_SR)
#define SYSC_IMR              (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? ((power_domain->isr_idx == 0) ? RCAR_V3U_SYSC_IMR0  : RCAR_V3U_SYSC_IMR1) : \
                                          RCAR_GEN3_SYSC_IMR)
#define SYSC_IER              (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? ((power_domain->isr_idx == 0) ? RCAR_V3U_SYSC_IER0  : RCAR_V3U_SYSC_IER1) : \
                                          RCAR_GEN3_SYSC_IER)
#define SYSC_ISR              (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? ((power_domain->isr_idx == 0) ? RCAR_V3U_SYSC_ISCR0 : RCAR_V3U_SYSC_ISCR1) : \
                                          RCAR_GEN3_SYSC_ISR)
#define SYSC_ISCR             (uint32_t)((sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) ? ((power_domain->isr_idx == 0) ? RCAR_V3U_SYSC_ISCR0 : RCAR_V3U_SYSC_ISCR1) : \
                                          RCAR_GEN3_SYSC_ISCR)

#define SYSCSR_TIMEOUT         1000
#define SYSCISR_TIMEOUT        1000
#define PWRER_RETRIES          1000
#define SYSC_DELAY_NS          10000

static uint32_t rcar_sysc_extmask_get(const uint32_t soc_id)
{
    uint32_t extmask;

    switch (soc_id) {
        case PRODUCT_ID_RCAR_V3H:
            extmask = RCAR_GEN3_SYSC_EXTMASK_V3H;
            break;
        case PRODUCT_ID_RCAR_V3M:
            extmask = RCAR_GEN3_SYSC_EXTMASK_V3M;
            break;
        case PRODUCT_ID_RCAR_H3:
        case PRODUCT_ID_RCAR_M3N:
        case PRODUCT_ID_RCAR_E3:
            extmask = RCAR_GEN3_SYSC_EXTMASK_H3M3NE3;
            break;
        default:
            extmask = SYSC_REG_OFF_UNKNOWN;
    }
    return extmask;
}

static const sysc_power_domain_t* rcar_sysc_get_power_domain_(const sysc_power_domain_t* domains, int dom_size, const int pdr_number)
{
    int i;
    for (i = 0; i < dom_size; i++) {
        if (pdr_number == domains[i].pwr_idx) {
            return &domains[i];
        }
    }
    return NULL;
}

static const sysc_power_domain_t* rcar_sysc_get_power_domain(const int pdr_number)
{
    switch (sysc_dev.soc_id) {
        case PRODUCT_ID_RCAR_H3:  return rcar_sysc_get_power_domain_(h3_sysc_power_domain,  sizeof(h3_sysc_power_domain)/sizeof(sysc_power_domain_t),  pdr_number);
        case PRODUCT_ID_RCAR_M3W: return rcar_sysc_get_power_domain_(m3w_sysc_power_domain, sizeof(m3w_sysc_power_domain)/sizeof(sysc_power_domain_t), pdr_number);
        case PRODUCT_ID_RCAR_M3N: return rcar_sysc_get_power_domain_(m3n_sysc_power_domain, sizeof(m3n_sysc_power_domain)/sizeof(sysc_power_domain_t), pdr_number);
        case PRODUCT_ID_RCAR_E3:  return rcar_sysc_get_power_domain_(e3_sysc_power_domain,  sizeof(e3_sysc_power_domain)/sizeof(sysc_power_domain_t),  pdr_number);
        case PRODUCT_ID_RCAR_D3:  return rcar_sysc_get_power_domain_(d3_sysc_power_domain,  sizeof(d3_sysc_power_domain)/sizeof(sysc_power_domain_t),  pdr_number);
        case PRODUCT_ID_RCAR_V3M: return rcar_sysc_get_power_domain_(v3m_sysc_power_domain, sizeof(v3m_sysc_power_domain)/sizeof(sysc_power_domain_t), pdr_number);
        case PRODUCT_ID_RCAR_V3H: return rcar_sysc_get_power_domain_(v3h_sysc_power_domain, sizeof(v3h_sysc_power_domain)/sizeof(sysc_power_domain_t), pdr_number);
        case PRODUCT_ID_RCAR_V3U: return rcar_sysc_get_power_domain_(v3u_sysc_power_domain, sizeof(v3u_sysc_power_domain)/sizeof(sysc_power_domain_t), pdr_number);
        default: return NULL;
    }
}

static int rcar_sysc_domain_power_status_check(const uint32_t reg, const uint32_t mask,
                    const uint32_t val, const int timeout)
{
    int i;

    for (i = timeout; i > 0; --i) {
        if ( (in32(sysc_dev.vbase + reg) & mask) == val ) {
            return SYSC_RET_OK;
        }

        nanospin_ns(SYSC_DELAY_NS);
    }

    return SYSC_RET_TIMEOUT;
}

static int rcar_sysc_clear_status_flags(const sysc_power_domain_t* const power_domain)
{
    out32(sysc_dev.vbase + SYSC_ISCR, BIT(power_domain->isr_bit));

    if (sysc_dev.soc_id == PRODUCT_ID_RCAR_V3U) {
        /* Check to make suare status bit is cleared */
        if (rcar_sysc_domain_power_status_check(SYSC_ISR, BIT(power_domain->isr_bit), 0, SYSCISR_TIMEOUT)) {
            return SYSC_RET_TIMEOUT;
        }
    }

    return SYSC_RET_OK;
}

static int rcar_v3h_sysc_pdmode_read(void)
{
    if ((sysc_dev.soc_rev == PRODUCT_V3H_REV_2_0) ||
        (sysc_dev.soc_rev == PRODUCT_V3H_REV_2_1)) {
        return (in32(sysc_dev.vbase + RCAR_GEN3_SYSC_CPDMD) & SYSC_PDMODE_MASK);
    } else {
        return -1;
    }
}

static int rcar_v3h_sysc_pdmode_switch_domain(const int pdr_number)
{
    const sysc_power_domain_t *power_domain;
    int pdmode;
    int pdr_no = -1;

    if (sysc_dev.soc_id == PRODUCT_ID_RCAR_V3H) {
        power_domain = &v3h_sysc_power_domain[pdr_number];
        if (power_domain != NULL) {
            pdmode = rcar_v3h_sysc_pdmode_read();
            if (pdmode == SYSC_PDMODE_1) {
                pdr_no =  power_domain->pdmode_1;
            } else if (pdmode == SYSC_PDMODE_3) {
                pdr_no = power_domain->pdmode_3;
            }
        }
    }

    return ((pdr_no == -1) ? pdr_number : pdr_no);
}

static int rcar_sysc_domain_is_off(const sysc_power_domain_t* const power_domain)
{
    /* Domain is in the Power-OFF state ? */
    if (in32(sysc_dev.vbase + SYSC_PWRSR) & BIT(power_domain->chan_bit)) {
        return 1;
    }

    /* Domain is in the Power-ON state ? */
    if (in32(sysc_dev.vbase + SYSC_PWRSR) & BIT(power_domain->on_status)) {
        return 0;
    }

    return SYSC_RET_UNKNOWNSTA;
}

static int rcar_sysc_domain_power_off(const int pdr_number)
{
    const sysc_power_domain_t *power_domain;
    int i, pdr_no;
    const uint32_t extmask = rcar_sysc_extmask_get(sysc_dev.soc_id);

    pdr_no = rcar_v3h_sysc_pdmode_switch_domain(pdr_number);

    power_domain = rcar_sysc_get_power_domain(pdr_no);
    if (!power_domain) {
        sysc_slogf(_SLOG_ERROR, "%s: Power Domain(%d) is not valid", __func__, pdr_no);
        return SYSC_RET_INVALPARA;
    }

    if(pdr_no != pdr_number) {
        sysc_slogf(_SLOG_INFO, "%s: Switch Power Domain %s to %s", __func__,
            rcar_sysc_get_power_domain(pdr_number)->name, power_domain->name);
    }
    sysc_slogf(_SLOG_INFO, "%s: Power Domain %s", __func__, power_domain->name);

    if (rcar_sysc_domain_is_off(power_domain) == 1) {
        /* This Power Domain is in the Power-OFF state already, return without doing anything */
        sysc_slogf(_SLOG_INFO, "%s: Power Domain %s is OFF already", __func__, power_domain->name);
        return SYSC_RET_OK;
    }

    /* Set bit in SYSCEXTMASK */
    if (extmask != SYSC_REG_OFF_UNKNOWN) {
        out32(sysc_dev.vbase + extmask, 1);
    }

    /* Mask interrupt */
    out32(sysc_dev.vbase + SYSC_IMR, in32(sysc_dev.vbase + SYSC_IMR) | BIT(power_domain->isr_bit));

    /* Enable status bit update */
    out32(sysc_dev.vbase + SYSC_IER, in32(sysc_dev.vbase + SYSC_IER) | BIT(power_domain->isr_bit));

    /* Clear status bit */
    if (rcar_sysc_clear_status_flags(power_domain)) {
        sysc_slogf(_SLOG_ERROR, "%s: SYSCISCR%d[%d] is not 0", __func__, power_domain->isr_idx, power_domain->isr_bit);
        goto fail;
    }

    for (i = 0; i < PWRER_RETRIES; i++) {
        /* Power-OFF is ready to start? */
        if (rcar_sysc_domain_power_status_check(SYSC_SR, 1, 1, SYSCSR_TIMEOUT)) {
            sysc_slogf(_SLOG_ERROR, "%s: Power-OFF is not ready to start", __func__);
            goto fail;
        }

        /* Submmit Power-OFF Request */
        out32(sysc_dev.vbase + SYSC_PWROFFCR, BIT(power_domain->chan_bit));

        /* The Power-OFF request have been accepted? */
        if ( !(in32(sysc_dev.vbase + SYSC_PWRER) & BIT(power_domain->chan_bit)) ) {
            break;
        }

        nanospin_ns(SYSC_DELAY_NS);
    }

    if (i == PWRER_RETRIES) {
        sysc_slogf(_SLOG_ERROR, "%s: Cannot power off %s domain (%d)", __func__, power_domain->name, pdr_number);
        goto fail;
    }

    /* The Power-OFF processing is completed? */
    if (rcar_sysc_domain_power_status_check(SYSC_ISR, BIT(power_domain->isr_bit), BIT(power_domain->isr_bit), SYSCISR_TIMEOUT)) {
        sysc_slogf(_SLOG_ERROR, "%s: The %s Power-OFF processing is not completed", __func__, power_domain->name);
        goto fail;
    }

    /* Clear bit in SYSCEXTMASK */
    if (extmask != SYSC_REG_OFF_UNKNOWN) {
        out32(sysc_dev.vbase + extmask, 0);
    }

    /* Clear status bit */
    if (rcar_sysc_clear_status_flags(power_domain)) {
        sysc_slogf(_SLOG_ERROR, "%s: %s Domain SYSCISCR%d[%d] is not 0", __func__, power_domain->name, power_domain->isr_idx, power_domain->isr_bit);
        goto fail;
    }

    return SYSC_RET_OK;

fail:
    return SYSC_RET_HWFAIL;
}

static int rcar_sysc_domain_power_on(const int pdr_number)
{
    const sysc_power_domain_t *power_domain;
    int i, pdr_no;
    const uint32_t extmask = rcar_sysc_extmask_get(sysc_dev.soc_id);

    pdr_no = rcar_v3h_sysc_pdmode_switch_domain(pdr_number);

    power_domain = rcar_sysc_get_power_domain(pdr_no);
    if (!power_domain) {
        sysc_slogf(_SLOG_ERROR, "%s: Power Domain(%d) is not valid", __func__, pdr_no);
        return SYSC_RET_INVALPARA;
    }

    if(pdr_number != pdr_no) {
        sysc_slogf(_SLOG_INFO, "%s: Switch Power Domain %s to %s", __func__,
            rcar_sysc_get_power_domain(pdr_number)->name, power_domain->name);
    }
    sysc_slogf(_SLOG_INFO, "%s: Power Domain %s", __func__, power_domain->name);

    if (!rcar_sysc_domain_is_off(power_domain)) {
        /* This Power Domain is not in the on Power-ON state, return without doing anything */
        sysc_slogf(_SLOG_INFO, "%s: Power Domain %s is ON already", __func__, power_domain->name);
        return SYSC_RET_OK;
    }

    /* Set bit in SYSCEXTMASK */
    if (extmask != SYSC_REG_OFF_UNKNOWN) {
        out32(sysc_dev.vbase + extmask, 1);
    }

    /* Mask interrupt */
    out32(sysc_dev.vbase + SYSC_IMR, in32(sysc_dev.vbase + SYSC_IMR) | BIT(power_domain->isr_bit));

    /* Enable status bits update */
    out32(sysc_dev.vbase + SYSC_IER, in32(sysc_dev.vbase + SYSC_IER) | BIT(power_domain->isr_bit));

    /* Clear status bit */
    if (rcar_sysc_clear_status_flags(power_domain)) {
        sysc_slogf(_SLOG_ERROR, "%s: SYSCISCR%d[%d] is not 0", __func__, power_domain->isr_idx, power_domain->isr_bit);
        goto fail;
    }

    for (i = 0; i < PWRER_RETRIES; i++) {
        /* Power resume is ready to start? */
        if (rcar_sysc_domain_power_status_check(SYSC_SR, 2, 2, SYSCSR_TIMEOUT))  {
            sysc_slogf(_SLOG_ERROR, "%s: Power resume is not ready to start", __func__);
            goto fail;
        }

        /* Submit Power-ON Request */
        out32 (sysc_dev.vbase + SYSC_PWRONCR, BIT(power_domain->chan_bit));

        /* The Power-ON request have been accepted? */
        if (!(in32(sysc_dev.vbase + SYSC_PWRER) & BIT(power_domain->chan_bit))) {
            break;
        }

        nanospin_ns(SYSC_DELAY_NS);
    }

    if (i == PWRER_RETRIES) {
        sysc_slogf(_SLOG_ERROR, "%s: Cannot power on %s domain (%d)", __func__, power_domain->name, pdr_no);
        goto fail;
    }

    /* The Power-ON processing is completed? */
    if (rcar_sysc_domain_power_status_check(SYSC_ISR, BIT(power_domain->isr_bit), BIT(power_domain->isr_bit), SYSCISR_TIMEOUT)) {
        sysc_slogf(_SLOG_ERROR, "%s: The %s Power-ON processing is not completed", __func__, power_domain->name);
        goto fail;
    }

    /* Clear bit in SYSCEXTMASK */
    if (extmask != SYSC_REG_OFF_UNKNOWN) {
        out32(sysc_dev.vbase + extmask, 0);
    }

    /* Clear status bit */
    if (rcar_sysc_clear_status_flags(power_domain)) {
        sysc_slogf(_SLOG_ERROR, "%s: %s Domain SYSCISCR%d[%d] is not 0 ", __func__, power_domain->name, power_domain->isr_idx, power_domain->isr_bit);
        goto fail;
    }

    return SYSC_RET_OK;

fail:
    return SYSC_RET_HWFAIL;
}

int rcar_sysc_power_is_off(const int pdr_number)
{
    const sysc_power_domain_t *power_domain;
    int pdr_no;

    pdr_no = rcar_v3h_sysc_pdmode_switch_domain(pdr_number);

    power_domain = rcar_sysc_get_power_domain(pdr_no);
    if (!power_domain) {
        sysc_slogf(_SLOG_ERROR, "%s: Power Domain(%d) is not valid", __func__, pdr_no);
        return SYSC_RET_ERROR;
    }

    return rcar_sysc_domain_is_off(power_domain);
}

int rcar_sysc_power_set(const int pdr_number, const int status)
{

    if (status) {
        return rcar_sysc_domain_power_off(pdr_number);
    } else {
        return rcar_sysc_domain_power_on(pdr_number);
    }
}

int rcar_sysc_pdmode_read(void)
{
    if( sysc_dev.soc_id != PRODUCT_ID_RCAR_V3H) {
        return -1;
    }

    return rcar_v3h_sysc_pdmode_read();
}
