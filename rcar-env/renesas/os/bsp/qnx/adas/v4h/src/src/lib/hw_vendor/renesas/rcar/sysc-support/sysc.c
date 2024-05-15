/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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
    char    name[16];       /* Power domain name */
    uint8_t chan_idx;       /* Index of PWR*R */
    uint8_t chan_bit;       /* Bit in PWR* (except for PWRUP in PWRSR) */
    uint8_t on_status;      /* Status PWRUP bit in PWRSR */
    uint8_t isr_idx;
    uint8_t isr_bit;        /* Bit in SYSCI*R */
    int     pdmode_1;       /* Point to parent domain in Power Domain Mode 1 for V3H 2.0 only (-1 value doesn't support)  */
    int     pdmode_3;       /* Point to parent domain in Power Domain Mode 3 for V3H 2.0 only (-1 value doesn't support) */
} sysc_power_domain_t;

#define SYSC_PD(_idx, _name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3) \
            [_idx] = {                   \
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
#define SYSC_PD_GEN4(_name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3) \
            SYSC_PD(SYSC_PDR_GEN4_##_name, _name, _ch_idx, _ch_bit, _sts, _isr_idx, _isr_bit, _pdm1, _pdm3)


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
    SYSC_PD_GEN4(A1E0D0C0,  0,       0,         4,        0,       0,     -1,    -1),
    SYSC_PD_GEN4(A1E0D0C1,  1,       0,         4,        0,       1,     -1,    -1),
    SYSC_PD_GEN4(A1E0D1C0,  2,       0,         4,        0,       2,     -1,    -1),
    SYSC_PD_GEN4(A1E0D1C1,  3,       0,         4,        0,       3,     -1,    -1),
    SYSC_PD_GEN4(A1E1D0C0,  4,       0,         4,        0,       4,     -1,    -1),
    SYSC_PD_GEN4(A1E1D0C1,  5,       0,         4,        0,       5,     -1,    -1),
    SYSC_PD_GEN4(A1E1D1C0,  6,       0,         4,        0,       6,     -1,    -1),
    SYSC_PD_GEN4(A1E1D1C1,  7,       0,         4,        0,       7,     -1,    -1),
    SYSC_PD_GEN4(A2E0D1,   17,       0,         4,        0,      17,     -1,    -1),
    SYSC_PD_GEN4(A2E0D0,   16,       0,         4,        0,      16,     -1,    -1),
    SYSC_PD_GEN4(A2E1D0,   18,       0,         4,        0,      18,     -1,    -1),
    SYSC_PD_GEN4(A2E1D1,   19,       0,         4,        0,      19,     -1,    -1),
    SYSC_PD_GEN4(A3E0,     20,       0,         4,        0,      20,     -1,    -1),
    SYSC_PD_GEN4(A3E1,     21,       0,         4,        0,      21,     -1,    -1),
    SYSC_PD_GEN4(3DGA,     24,       0,         4,        0,      24,     -1,    -1),
    SYSC_PD_GEN4(3DGB,     25,       0,         4,        0,      25,     -1,    -1),
    SYSC_PD_GEN4(A1CNN2,   32,       0,         4,        1,       0,     -1,    -1),
    SYSC_PD_GEN4(A1DSP0,   33,       0,         4,        1,       1,     -1,    -1),
    SYSC_PD_GEN4(A2IMP0,   34,       0,         4,        1,       2,     -1,    -1),
    SYSC_PD_GEN4(A2DP0,    35,       0,         4,        1,       3,     -1,    -1),
    SYSC_PD_GEN4(A2CV0,    36,       0,         4,        1,       4,     -1,    -1),
    SYSC_PD_GEN4(A2CV1,    37,       0,         4,        1,       5,     -1,    -1),
    SYSC_PD_GEN4(A2CV4,    38,       0,         4,        1,       6,     -1,    -1),
    SYSC_PD_GEN4(A2CV6,    39,       0,         4,        1,       7,     -1,    -1),
    SYSC_PD_GEN4(A2CN2,    40,       0,         4,        1,       8,     -1,    -1),
    SYSC_PD_GEN4(A1CNN0,   41,       0,         4,        1,       9,     -1,    -1),
    SYSC_PD_GEN4(A2CN0,    42,       0,         4,        1,      10,     -1,    -1),
    SYSC_PD_GEN4(A3IR,     43,       0,         4,        1,      11,     -1,    -1),
    SYSC_PD_GEN4(A1CNN1,   44,       0,         4,        1,      12,     -1,    -1),
    SYSC_PD_GEN4(A1DSP1,   45,       0,         4,        1,      13,     -1,    -1),
    SYSC_PD_GEN4(A2IMP2,   46,       0,         4,        1,      14,     -1,    -1),
    SYSC_PD_GEN4(A2DP1,    47,       0,         4,        1,      15,     -1,    -1),
    SYSC_PD_GEN4(A2CV2,    48,       0,         4,        1,      16,     -1,    -1),
    SYSC_PD_GEN4(A2CV3,    49,       0,         4,        1,      17,     -1,    -1),
    SYSC_PD_GEN4(A2CV5,    50,       0,         4,        1,      18,     -1,    -1),
    SYSC_PD_GEN4(A2CV7,    51,       0,         4,        1,      19,     -1,    -1),
    SYSC_PD_GEN4(A2CN1,    52,       0,         4,        1,      20,     -1,    -1),
    SYSC_PD_GEN4(A3VIP0,   56,       0,         4,        1,      24,     -1,    -1),
    SYSC_PD_GEN4(A3VIP1,   57,       0,         4,        1,      25,     -1,    -1),
    SYSC_PD_GEN4(A3VIP2,   58,       0,         4,        1,      26,     -1,    -1),
    SYSC_PD_GEN4(A3VIP3,   59,       0,         4,        1,      27,     -1,    -1),
    SYSC_PD_GEN4(A3ISP0,   60,       0,         4,        1,      28,     -1,    -1),
    SYSC_PD_GEN4(A3ISP1,   61,       0,         4,        1,      29,     -1,    -1),
};

static const sysc_power_domain_t s4_sysc_power_domain[] = {
    /*           name      chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    SYSC_PD_GEN4(A1E0D0C0,  0,       0,         4,        0,       0,     -1,    -1),
    SYSC_PD_GEN4(A1E0D0C1,  1,       0,         4,        0,       1,     -1,    -1),
    SYSC_PD_GEN4(A1E0D1C0,  2,       0,         4,        0,       2,     -1,    -1),
    SYSC_PD_GEN4(A1E0D1C1,  3,       0,         4,        0,       3,     -1,    -1),
    SYSC_PD_GEN4(A1E1D0C0,  4,       0,         4,        0,       4,     -1,    -1),
    SYSC_PD_GEN4(A1E1D0C1,  5,       0,         4,        0,       5,     -1,    -1),
    SYSC_PD_GEN4(A1E1D1C0,  6,       0,         4,        0,       6,     -1,    -1),
    SYSC_PD_GEN4(A1E1D1C1,  7,       0,         4,        0,       7,     -1,    -1),
    SYSC_PD_GEN4(A2E0D0,   16,       0,         4,        0,      16,     -1,    -1),
    SYSC_PD_GEN4(A2E0D1,   17,       0,         4,        0,      17,     -1,    -1),
    SYSC_PD_GEN4(A2E1D0,   18,       0,         4,        0,      18,     -1,    -1),
    SYSC_PD_GEN4(A2E1D1,   19,       0,         4,        0,      19,     -1,    -1),
    SYSC_PD_GEN4(A3E0,     20,       0,         4,        0,      20,     -1,    -1),
    SYSC_PD_GEN4(A3E1,     21,       0,         4,        0,      21,     -1,    -1),
};

sysc_power_domain_t v4h_sysc_power_domain[] = {
    /*            name      chan_idx  chan_bit  on_status  isr_idx  isr_bit       */
    SYSC_PD_GEN4(A1E0D0C0,  0,       0,         4,        0,       0,    -1,    -1),
    SYSC_PD_GEN4(A1E0D0C1,  1,       0,         4,        0,       1,    -1,    -1),
    SYSC_PD_GEN4(A1E0D1C0,  2,       0,         4,        0,       2,    -1,    -1),
    SYSC_PD_GEN4(A1E0D1C1,  3,       0,         4,        0,       3,    -1,    -1),
    SYSC_PD_GEN4(A2E0D0,   16,       0,         4,        0,      16,    -1,    -1),
    SYSC_PD_GEN4(A2E0D1,   17,       0,         4,        0,      17,    -1,    -1),
    SYSC_PD_GEN4(A3E0,     20,       0,         4,        0,      20,    -1,    -1),
    SYSC_PD_GEN4(3DGA,     24,       0,         4,        0,      24,    -1,    -1),
    SYSC_PD_GEN4(3DGB,     25,       0,         4,        0,      25,    -1,    -1),
    SYSC_PD_GEN4(A1DSP0,   33,       0,         4,        1,       1,    -1,    -1),
    SYSC_PD_GEN4(A2IMP0,   34,       0,         4,        1,       2,    -1,    -1),
    SYSC_PD_GEN4(A2DP0,    35,       0,         4,        1,       3,    -1,    -1),
    SYSC_PD_GEN4(A2CV0,    36,       0,         4,        1,       4,    -1,    -1),
    SYSC_PD_GEN4(A2CV1,    37,       0,         4,        1,       5,    -1,    -1),
    SYSC_PD_GEN4(A1CNN0,   41,       0,         4,        1,       9,    -1,    -1),
    SYSC_PD_GEN4(A2CN0,    42,       0,         4,        1,      10,    -1,    -1),
    SYSC_PD_GEN4(A3IR,     43,       0,         4,        1,      11,    -1,    -1),
    SYSC_PD_GEN4(A1DSP1,   45,       0,         4,        1,      13,    -1,    -1),
    SYSC_PD_GEN4(A2IMP2,   46,       0,         4,        1,      14,    -1,    -1),
    SYSC_PD_GEN4(A2DP1,    47,       0,         4,        1,      15,    -1,    -1),
    SYSC_PD_GEN4(A2CV2,    48,       0,         4,        1,      16,    -1,    -1),
    SYSC_PD_GEN4(A2CV3,    49,       0,         4,        1,      17,    -1,    -1),
    SYSC_PD_GEN4(A1DSP2,   53,       0,         4,        1,      21,    -1,    -1),
    SYSC_PD_GEN4(A1DSP3,   54,       0,         4,        1,      22,    -1,    -1),
    SYSC_PD_GEN4(A3VIP0,   56,       0,         4,        1,      24,    -1,    -1),
    SYSC_PD_GEN4(A3VIP1,   57,       0,         4,        1,      25,    -1,    -1),
    SYSC_PD_GEN4(A3VIP2,   58,       0,         4,        1,      26,    -1,    -1),
    SYSC_PD_GEN4(A3ISP0,   60,       0,         4,        1,      28,    -1,    -1),
    SYSC_PD_GEN4(A3ISP1,   61,       0,         4,        1,      29,    -1,    -1),
    SYSC_PD_GEN4(A3DUL,    62,       0,         4,        1,      30,    -1,    -1),
};

/* For each SYSC domain */
#define SYSC_PWRSR            (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? \
                                          RCAR_GEN4_SYSC_PDRSR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWRSRx(power_domain->chan_idx))    /* Power Status Register */
#define SYSC_PWROFFCR         (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? \
                                          RCAR_GEN4_SYSC_PDROFFCR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWROFFCRx(power_domain->chan_idx)) /* Power Shutoff Control Register */
#define SYSC_PWRONCR          (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? \
                                          RCAR_GEN4_SYSC_PDRONCR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWRONCRx(power_domain->chan_idx))  /* Power Resume Control Register */
#define SYSC_PWRER            (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? \
                                          RCAR_GEN4_SYSC_PDRESR(power_domain->chan_idx) : \
                                          RCAR_GEN3_SYSC_PWRERx(power_domain->chan_idx))    /* Power Shutoff/Resume Error */
/* For common */
#define SYSC_SR               (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? RCAR_GEN4_SYSC_SR : RCAR_GEN3_SYSC_SR)
#define SYSC_IMR              (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? ((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_IMR0  : RCAR_GEN4_SYSC_IMR1) : \
                                          RCAR_GEN3_SYSC_IMR)
#define SYSC_IER              (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? ((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_IER0  : RCAR_GEN4_SYSC_IER1) : \
                                          RCAR_GEN3_SYSC_IER)
#define SYSC_ISR              (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? ((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_ISCR0 : RCAR_GEN4_SYSC_ISCR1) : \
                                          RCAR_GEN3_SYSC_ISR)
#define SYSC_ISCR             (uint32_t)(IS_RCAR_GEN4(sysc_dev.soc_id) ? ((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_ISCR0 : RCAR_GEN4_SYSC_ISCR1) : \
                                          RCAR_GEN3_SYSC_ISCR)

#define SYSCSR_RDY             3
#define SYSCSR_ON_RDY          2
#define SYSCSR_OFF_RDY         1
#define SYSCSR_TIMEOUT         1000
#define SYSCISR_TIMEOUT        1000
#define PWRER_RETRIES          1000
#define SYSC_DELAY_NS          10000
#define APMU_TIMEOUT           1000000


/* This SW W/A is only for V4H 2.0 or later:
 * -Due to leakage current, A3IR and A2CN0 and A1CNN0 needs to keep power ON
 * -A3IR and A2CN0 and A1CNN0 are turn on in IPL, so after booting BSP they are prohibited to turn off
 * -The following variables is used for return states */
#define STATE_OFF 0
#define STATE_ON  1
static uint8_t dummyA3IR  = STATE_ON;
static uint8_t dummyA2CN0 = STATE_ON;
static uint8_t dummyA1CNN0 = STATE_ON;

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

static const sysc_power_domain_t* rcar_sysc_get_power_domain(const int pdr_number)
{
    switch (sysc_dev.soc_id) {
        case PRODUCT_ID_RCAR_H3:  return &h3_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_M3W: return &m3w_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_M3N: return &m3n_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_E3:  return &e3_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_D3:  return &d3_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_V3M: return &v3m_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_V3H: return &v3h_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_V3U: return &v3u_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_S4:  return &s4_sysc_power_domain[pdr_number];
        case PRODUCT_ID_RCAR_V4H: return &v4h_sysc_power_domain[pdr_number];
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

    sysc_slogf(_SLOG_ERROR, "%s: Power domain status check timeout", __func__);
    return SYSC_RET_TIMEOUT;
}

static int rcar_sysc_clear_status_flags(const sysc_power_domain_t* const power_domain)
{
    out32(sysc_dev.vbase + SYSC_ISCR, BIT(power_domain->isr_bit));

    if (IS_RCAR_GEN4(sysc_dev.soc_id)) {
        /* Check to make suare status bit is cleared */
        if (rcar_sysc_domain_power_status_check(SYSC_ISR, BIT(power_domain->isr_bit), 0, SYSCISR_TIMEOUT)) {
            return SYSC_RET_TIMEOUT;
        }
    }

    return SYSC_RET_OK;
}

static int rcar_v3h_sysc_pdmode_read(void)
{
    if ((sysc_dev.soc_rev != PRODUCT_V3H_REV_2_0) && (sysc_dev.soc_rev != PRODUCT_V3H_REV_2_1)) {
        return -1;
    }

    return (in32(sysc_dev.vbase + RCAR_GEN3_SYSC_CPDMD) & SYSC_PDMODE_MASK);
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

static int rcar_sysc_domain_check_ready(const sysc_power_domain_t* const power_domain, int on)
{
    if (IS_RCAR_GEN4(sysc_dev.soc_id)) {
        if (rcar_sysc_domain_power_status_check(SYSC_SR, SYSCSR_RDY, SYSCSR_RDY, SYSCSR_TIMEOUT)) {
            return SYSC_RET_TIMEOUT;
        }
    } else {
        if (on) {
            if (rcar_sysc_domain_power_status_check(SYSC_SR, SYSCSR_ON_RDY, SYSCSR_ON_RDY, SYSCSR_TIMEOUT))
                return SYSC_RET_TIMEOUT;
        } else {
            if (rcar_sysc_domain_power_status_check(SYSC_SR, SYSCSR_OFF_RDY, SYSCSR_OFF_RDY, SYSCSR_TIMEOUT))
                return SYSC_RET_TIMEOUT;
        }
    }

    return SYSC_RET_OK;
}

static int rcar_sysc_domain_power_off(const int pdr_number)
{
    const sysc_power_domain_t *power_domain;
    int i, pdr_no = pdr_number;
    const uint32_t extmask = rcar_sysc_extmask_get(sysc_dev.soc_id);

    /* V4H 2.0 or later A3IR and A2CN0 and A1CNN0 are turned on in IPL, they are prohibited to turn off.
     * Just change variable not accessing to HW */
    if ((sysc_dev.soc_id == PRODUCT_ID_RCAR_V4H) &&
       ((sysc_dev.soc_rev == PRODUCT_V4H_REV_2_0) || (sysc_dev.soc_rev == PRODUCT_V4H_REV_2_1) || (sysc_dev.soc_rev == PRODUCT_V4H_REV_2_2))) {
        if (SYSC_PDR_GEN4_A3IR == pdr_number) {
            dummyA3IR  = STATE_OFF;
            return SYSC_RET_OK;
        } else if (SYSC_PDR_GEN4_A2CN0 == pdr_number) {
            dummyA2CN0 = STATE_OFF;
            return SYSC_RET_OK;
        } else if (SYSC_PDR_GEN4_A1CNN0 == pdr_number) {
            dummyA1CNN0 = STATE_OFF;
            return SYSC_RET_OK;
        }
    }

    power_domain = rcar_sysc_get_power_domain(pdr_no);
    if (!power_domain) {
        sysc_slogf(_SLOG_ERROR, "%s: Power Domain(%d) is not valid", __func__, pdr_no);
        return SYSC_RET_INVALPARA;
    }

    if (pdr_no != pdr_number) {
        sysc_slogf(_SLOG_INFO, "%s: Switch Power Domain %s to %s", __func__,
            rcar_sysc_get_power_domain(pdr_number)->name, power_domain->name);
    }

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
        if (rcar_sysc_domain_check_ready(power_domain, 0)) {
            sysc_slogf(_SLOG_ERROR, "%s: Power-OFF is not ready to start", __func__);
            goto fail;
        }

        /* Submmit Power-OFF Request */
        out32(sysc_dev.vbase + SYSC_PWROFFCR, BIT(power_domain->chan_bit));

        /* Gen4 dosen't check Error status register ? */
        //if (IS_RCAR_GEN4(sysc_dev.soc_id))
        //    break;

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
    int i, pdr_no = pdr_number;
    const uint32_t extmask = rcar_sysc_extmask_get(sysc_dev.soc_id);

    /* V4H 2.0 or later A3IR and A2CN0 and A1CNN0 are turned on in IPL already.
     * Just change variable no need to turn on */
    if ((sysc_dev.soc_id == PRODUCT_ID_RCAR_V4H) &&
       ((sysc_dev.soc_rev == PRODUCT_V4H_REV_2_0) || (sysc_dev.soc_rev == PRODUCT_V4H_REV_2_1) || (sysc_dev.soc_rev == PRODUCT_V4H_REV_2_2))) {
        if (SYSC_PDR_GEN4_A3IR == pdr_number) {
            dummyA3IR  = STATE_ON;
            return SYSC_RET_OK;
        } else if (SYSC_PDR_GEN4_A2CN0 == pdr_number) {
            dummyA2CN0 = STATE_ON;
            return SYSC_RET_OK;
        } else if (SYSC_PDR_GEN4_A1CNN0 == pdr_number) {
            dummyA1CNN0 = STATE_ON;
            return SYSC_RET_OK;
        }
    }

    power_domain = rcar_sysc_get_power_domain(pdr_no);
    if (!power_domain) {
        sysc_slogf(_SLOG_ERROR, "%s: Power Domain(%d) is not valid", __func__, pdr_no);
        return SYSC_RET_INVALPARA;
    }

    if (pdr_number != pdr_no) {
        sysc_slogf(_SLOG_INFO, "%s: Switch Power Domain %s to %s", __func__,
            rcar_sysc_get_power_domain(pdr_number)->name, power_domain->name);
    }

    if (rcar_sysc_domain_is_off(power_domain) == 0) {
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
        if (rcar_sysc_domain_check_ready(power_domain, 1)) {
            sysc_slogf(_SLOG_ERROR, "%s: Power resume is not ready to start", __func__);
            goto fail;
        }

        /* Submit Power-ON Request */
        out32 (sysc_dev.vbase + SYSC_PWRONCR, BIT(power_domain->chan_bit));

        /* Gen4 dosen't check Error status register ? */
        //if (IS_RCAR_GEN4(sysc_dev.soc_id))
        //    break;

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
    int pdr_no = pdr_number;

    /* V4H 2.0 or later A3IR and A2CN0 and A1CNN0 return state from variable's value */
    if ((sysc_dev.soc_id == PRODUCT_ID_RCAR_V4H) &&
       ((sysc_dev.soc_rev == PRODUCT_V4H_REV_2_0) || (sysc_dev.soc_rev == PRODUCT_V4H_REV_2_1) || (sysc_dev.soc_rev == PRODUCT_V4H_REV_2_2))) {
        if (SYSC_PDR_GEN4_A3IR == pdr_number) {
            return (dummyA3IR == STATE_OFF ? 1 : 0);
        } else if (SYSC_PDR_GEN4_A2CN0 == pdr_number) {
            return (dummyA2CN0 == STATE_OFF ? 1 : 0);
        } else if (SYSC_PDR_GEN4_A1CNN0 == pdr_number) {
            return (dummyA1CNN0 == STATE_OFF ? 1 : 0);
        }
    }

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
    if (sysc_dev.soc_id != PRODUCT_ID_RCAR_V3H) {
        return -1;
    }

    return rcar_v3h_sysc_pdmode_read();
}

static void rcar_apmu_domain_power_ctrl(uint32_t ctl_reg, uint32_t ctl_msk, uint32_t ctl_val)
{
    uint32_t tmp;

    tmp  =  in32(sysc_dev.apmu_vbase + ctl_reg);
    tmp &= ~ctl_msk;
    tmp |=  ctl_val;
    out32(sysc_dev.apmu_vbase + RCAR_GEN4_APMU_WPR, ~tmp);
    out32(sysc_dev.apmu_vbase + ctl_reg, tmp);
}

static int rcar_apmu_domain_power_sts(uint32_t sts_reg, uint32_t sts_msk, uint32_t sts_val)
{
    for (int i = 0; i < APMU_TIMEOUT; i++) {
        if ((in32(sysc_dev.apmu_vbase + sts_reg) & sts_msk) == sts_val) {
            return SYSC_RET_OK;
        }

        nanospin_ns(SYSC_DELAY_NS);
    }

    return SYSC_RET_TIMEOUT;
}

static int rcar_apmu_cluster_power(const int cluster, int on)
{
    uintptr_t cluster_off;
    int ret = SYSC_RET_OK;

    if (sysc_dev.apmu_vbase == 0) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid APMU vbase (0)", __func__);
        return SYSC_RET_INVALPARA;
    }

    cluster_off  = RCAR_GEN4_APMU_AP_CLUSTER0_GP;
    cluster_off += RCAR_GEN4_APMU_CLUSTER_SIZE * cluster;

    if (on) {
        // Clear Cluster OFF bit
        rcar_apmu_domain_power_ctrl(cluster_off + RCAR_GEN4_APMU_PWRCTRLCL,
                 RCAR_GEN4_APMU_PWRCTRLCL_PCHPDNEN, 0);

        // Wakeup Cluster
        rcar_apmu_domain_power_ctrl(cluster_off + RCAR_GEN4_APMU_PWRCTRLCL,
                 RCAR_GEN4_APMU_PWRCTRLCL_WUP_REQ,
                 RCAR_GEN4_APMU_PWRCTRLCL_WUP_REQ);
        // Wait until Cluster ON
        ret = rcar_apmu_domain_power_sts(cluster_off + RCAR_GEN4_APMU_FSMSTSRCL,
                 RCAR_GEN4_APMU_FSMSTSRCL_STATE_MSK,
                 RCAR_GEN4_APMU_FSMSTSRCL_STATE_ON);
    } else {
        // Enable PowerOFF
        rcar_apmu_domain_power_ctrl(cluster_off + RCAR_GEN4_APMU_PWRCTRLCL,
                 RCAR_GEN4_APMU_PWRCTRLCL_PCHPDNEN,
                 RCAR_GEN4_APMU_PWRCTRLCL_PCHPDNEN);
    }

    return ret;
}

static int rcar_apmu_l3cache_power(const int cluster, int on)
{
    uintptr_t cluster_off;

    if (sysc_dev.apmu_vbase == 0) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid APMU vbase (0)", __func__);
        return SYSC_RET_INVALPARA;
    }

    cluster_off  = RCAR_GEN4_APMU_AP_CLUSTER0_GP;
    cluster_off += RCAR_GEN4_APMU_CLUSTER_SIZE * cluster;

    // Power ClusterX L3 cache
    rcar_apmu_domain_power_ctrl(cluster_off + RCAR_GEN4_APMU_L3CTRLCL,
             RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_MSK,
             RCAR_GEN4_APMU_L3CTRLCL_L3CTRL_ALL_ON);

    return rcar_apmu_domain_power_sts(cluster_off + RCAR_GEN4_APMU_L3CTRLCL,
             RCAR_GEN4_APMU_L3CTRLCL_L3STS_MSK,
             RCAR_GEN4_APMU_L3CTRLCL_L3STS_ALL_ON);
}

static int rcar_apmu_a3_power(const int a3dom, int on)
{
    int ret = SYSC_RET_OK;

    if (sysc_dev.apmu_vbase == 0) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid APMU vbase (0)", __func__);
        return SYSC_RET_INVALPARA;
    }

    if (on) {
        // Clear A3domain OFF bit
        rcar_apmu_domain_power_ctrl(RCAR_GEN4_APMU_ADMIN_GP + (a3dom ? RCAR_GEN4_APMU_A3PWRCTRL1 : RCAR_GEN4_APMU_A3PWRCTRL0),
                 RCAR_GEN4_APMU_A3PWRCTRL_A3_PDN_EN,
                 0);

        // Request Wakup
        rcar_apmu_domain_power_ctrl(RCAR_GEN4_APMU_ADMIN_GP + (a3dom ? RCAR_GEN4_APMU_A3PWRCTRL1 : RCAR_GEN4_APMU_A3PWRCTRL0),
                 RCAR_GEN4_APMU_A3PWRCTRL_A3_WUP_REQ,
                 RCAR_GEN4_APMU_A3PWRCTRL_A3_WUP_REQ);

        ret = rcar_apmu_domain_power_sts(RCAR_GEN4_APMU_ADMIN_GP + (a3dom ? RCAR_GEN4_APMU_A3FSMSTSR1 : RCAR_GEN4_APMU_A3FSMSTSR0),
                 RCAR_GEN4_APMU_A3FSMSTSR_STATE_MSK,
                 RCAR_GEN4_APMU_A3FSMSTSR_STATE_ON);
    } else {
        // Enable PowerOFF
        rcar_apmu_domain_power_ctrl(RCAR_GEN4_APMU_ADMIN_GP + (a3dom ? RCAR_GEN4_APMU_A3PWRCTRL1 : RCAR_GEN4_APMU_A3PWRCTRL0),
                 RCAR_GEN4_APMU_A3PWRCTRL_A3_PDN_EN,
                 RCAR_GEN4_APMU_A3PWRCTRL_A3_PDN_EN);
    }

    return ret;
}

static int rcar_apmu_cpu_power(const int cpu, int on)
{
    uintptr_t corex_off, cluster_off;
    int ret = SYSC_RET_OK;

    if (sysc_dev.apmu_vbase == 0) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid APMU vbase (0)", __func__);
        return SYSC_RET_INVALPARA;
    }

    corex_off  = RCAR_GEN4_APMU_AP_CORE0_GP;
    corex_off += RCAR_GEN4_APMU_CLUSTER_OFFSET * (cpu / RCAR_GEN4_APMU_CORE_PER_CLUSTER);
    corex_off += RCAR_GEN4_APMU_CORE_IN_CLUSTER_SIZE * (cpu % RCAR_GEN4_APMU_CORE_PER_CLUSTER);

    cluster_off  = RCAR_GEN4_APMU_AP_CLUSTER0_GP;
    cluster_off += RCAR_GEN4_APMU_CLUSTER_SIZE * (cpu / RCAR_GEN4_APMU_CORE_PER_CLUSTER);

    if (on) {
        // Clear Cluster OFF bit
        rcar_apmu_domain_power_ctrl(cluster_off + RCAR_GEN4_APMU_PWRCTRLCL,
                 RCAR_GEN4_APMU_PWRCTRLCL_PCHPDNEN,
                 0);

        // Clear Core OFF bit
        rcar_apmu_domain_power_ctrl(corex_off + RCAR_GEN4_APMU_PWRCTRLC,
                RCAR_GEN4_APMU_PWRCTRLC_PCHPDNEN,
                0);

        // Wait until complete
        ret = rcar_apmu_domain_power_sts(corex_off + RCAR_GEN4_APMU_PWRCTRLC,
                 RCAR_GEN4_APMU_PWRCTRLC_PCHPDNEN,
                 0);

        if (ret == SYSC_RET_OK) {
            // Wakeup CPU
            rcar_apmu_domain_power_ctrl(corex_off + RCAR_GEN4_APMU_PWRCTRLC,
                     RCAR_GEN4_APMU_PWRCTRLC_WUP_REQ,
                     RCAR_GEN4_APMU_PWRCTRLC_WUP_REQ);

            // Wait until CPU ON
            ret = rcar_apmu_domain_power_sts(corex_off + RCAR_GEN4_APMU_FSMSTSRC,
                     RCAR_GEN4_APMU_FSMSTSRC_STATE_MSK,
                     RCAR_GEN4_APMU_FSMSTSRC_STATE_ON);
        }
    }
    else {
        // Clear DBGGEN_PPDN bit for core down to 'OFF' mode
        rcar_apmu_domain_power_ctrl(corex_off + RCAR_GEN4_APMU_SAFECTRLC,
             RCAR_GEN4_APMU_SAFECTRLC_DBGGEN_PPDN,
             0);

        // Enable PowerOFF
        rcar_apmu_domain_power_ctrl(corex_off + RCAR_GEN4_APMU_PWRCTRLC,
             RCAR_GEN4_APMU_PWRCTRLC_PCHPDNEN,
             RCAR_GEN4_APMU_PWRCTRLC_PCHPDNEN);
    }

    return ret;
}

static int rcar_apmu_domain_power_on(const int dom_id, uint32_t flag)
{
    int ret;

    if (sysc_dev.apmu_vbase == 0) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid APMU vbase (0)", __func__);
        return SYSC_RET_INVALPARA;
    }

    if (flag == APMU_POWER_CPU) {
        // Power ON CPU
        if ((ret = rcar_apmu_cpu_power(dom_id, 1))) {
            sysc_slogf(_SLOG_ERROR, "%s: Power ON CPU failed", __func__);
            return ret;
        }
    } else if (flag == APMU_POWER_CLUSTER) {
        // Power ON Cluster
        if ((ret = rcar_apmu_cluster_power(dom_id, 1))) {
            sysc_slogf(_SLOG_ERROR, "%s: Power ON cluster failed", __func__);
            return ret;
        }
    } else if (flag == APMU_POWER_L3CACHE) {
        // Power ON L3 cache
        if ((ret = rcar_apmu_l3cache_power(dom_id, 1))) {
            sysc_slogf(_SLOG_ERROR, "%s: Power ON L3 cache failed", __func__);
            return ret;
        }
    } else if (flag == APMU_POWER_A3DOMAIN) {
        // Power ON A3 domain
        if ((ret = rcar_apmu_a3_power(dom_id, 1))) {
            sysc_slogf(_SLOG_ERROR, "%s: Power ON A3 domain failed", __func__);
            return ret;
        }
    } else {
        sysc_slogf(_SLOG_ERROR, "%s: Unknown domain", __func__);
        return SYSC_RET_INVALPARA;
    }

    return SYSC_RET_OK;
}

static int rcar_apmu_domain_power_off(const int dom_id, uint32_t flag)
{
    int ret;
    volatile uint32_t core_pwrdn_en = 1;

    if (sysc_dev.apmu_vbase == 0) {
        sysc_slogf(_SLOG_ERROR, "%s: Invalid APMU vbase (0)", __func__);
        return SYSC_RET_INVALPARA;
    }

    if (flag == APMU_POWER_CPU) {
        // Power OFF CPU
        if ((ret = rcar_apmu_cpu_power(dom_id, 0))) {
            sysc_slogf(_SLOG_ERROR, "%s: Power OFF CPU failed", __func__);
            return ret;
        }
        asm volatile("msr S3_0_C15_C2_7,%0" :: "r" (core_pwrdn_en):);
    } else if (flag == APMU_POWER_CLUSTER) {
        // Power OFF Cluster
        if ((ret = rcar_apmu_cluster_power(dom_id, 0))) {
            sysc_slogf(_SLOG_ERROR, "%s: Power OFF cluster failed", __func__);
            return ret;
        }
    } else if (flag == APMU_POWER_A3DOMAIN) {
        // Power OFF A3 domain
        if ((ret = rcar_apmu_a3_power(dom_id, 0))) {
            sysc_slogf(_SLOG_ERROR, "%s: Power OFF A3 domain failed", __func__);
            return ret;
        }
    } else {
        sysc_slogf(_SLOG_ERROR, "%s: Unknown domain", __func__);
        return SYSC_RET_INVALPARA;
    }

    return SYSC_RET_OK;
}

int rcar_apmu_power_set(int dom_id, uint32_t flags, int status)
{
    if (status) {
        return rcar_apmu_domain_power_off(dom_id, flags);
    } else {
        return rcar_apmu_domain_power_on(dom_id, flags);
    }
}
