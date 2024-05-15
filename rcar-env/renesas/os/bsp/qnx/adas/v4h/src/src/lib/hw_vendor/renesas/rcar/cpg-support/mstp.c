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

/* Structure for CPG register */
typedef struct _cpg_mstp_offset {
    uint32_t srcr_offset;
    uint32_t srstclr_offset;
    uint32_t smstpcr_offset;
    uint32_t rmstpcr_offset;
    uint32_t mstpsr_offset;
    uint32_t rahsr_offset;
    uint32_t fsrchkra_offset;
    uint32_t fsrchkrb_offset;
    uint32_t fsrchksetr_offset;
    uint32_t fsrchkclrr_offset;
} cpg_reg_offset_t;

typedef struct {
    int clock;
    uint8_t stc;
} cpg_stc_t;

#define BITMASK(x,y)        (((2ULL << (y-x)) - 1) << x)

#define CPG_MSTP(_srcr_offset, _srstclr_offset, _smstpcr_offset, _rmstpcr_offset, _mstpsr_offset, _rahsr_offset, _fsrchkra_offset, _fsrchkrb_offset, _fsrchksetr_offset, _fsrchkclrr_offset) \
{ \
    .srcr_offset  = _srcr_offset, \
    .srstclr_offset = _srstclr_offset, \
    .smstpcr_offset = _smstpcr_offset, \
    .rmstpcr_offset = _rmstpcr_offset, \
    .mstpsr_offset = _mstpsr_offset, \
    .rahsr_offset = _rahsr_offset, \
    .fsrchkra_offset = _fsrchkra_offset, \
    .fsrchkrb_offset = _fsrchkrb_offset, \
    .fsrchksetr_offset = _fsrchksetr_offset, \
    .fsrchkclrr_offset = _fsrchkclrr_offset, \
}

static const struct _cpg_mstp_offset gen3_mstp_table[] = {
    /*        srcr_offset              srstclr_offset               smstpcr_offset                rmstpcr_offset                mstpsr_offset                         */
    CPG_MSTP( RCAR_GEN3_CPG_SRCR0,     RCAR_GEN3_CPG_SRSTCLR0,      RCAR_GEN3_CPG_SMSTPCR0,       RCAR_GEN3_CPG_RMSTPCR0,       RCAR_GEN3_CPG_MSTPSR0,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR1,     RCAR_GEN3_CPG_SRSTCLR1,      RCAR_GEN3_CPG_SMSTPCR1,       RCAR_GEN3_CPG_RMSTPCR1,       RCAR_GEN3_CPG_MSTPSR1,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR2,     RCAR_GEN3_CPG_SRSTCLR2,      RCAR_GEN3_CPG_SMSTPCR2,       RCAR_GEN3_CPG_RMSTPCR2,       RCAR_GEN3_CPG_MSTPSR2,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR3,     RCAR_GEN3_CPG_SRSTCLR3,      RCAR_GEN3_CPG_SMSTPCR3,       RCAR_GEN3_CPG_RMSTPCR3,       RCAR_GEN3_CPG_MSTPSR3,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR4,     RCAR_GEN3_CPG_SRSTCLR4,      RCAR_GEN3_CPG_SMSTPCR4,       RCAR_GEN3_CPG_RMSTPCR4,       RCAR_GEN3_CPG_MSTPSR4,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR5,     RCAR_GEN3_CPG_SRSTCLR5,      RCAR_GEN3_CPG_SMSTPCR5,       RCAR_GEN3_CPG_RMSTPCR5,       RCAR_GEN3_CPG_MSTPSR5,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR6,     RCAR_GEN3_CPG_SRSTCLR6,      RCAR_GEN3_CPG_SMSTPCR6,       RCAR_GEN3_CPG_RMSTPCR6,       RCAR_GEN3_CPG_MSTPSR6,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR7,     RCAR_GEN3_CPG_SRSTCLR7,      RCAR_GEN3_CPG_SMSTPCR7,       RCAR_GEN3_CPG_RMSTPCR7,       RCAR_GEN3_CPG_MSTPSR7,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR8,     RCAR_GEN3_CPG_SRSTCLR8,      RCAR_GEN3_CPG_SMSTPCR8,       RCAR_GEN3_CPG_RMSTPCR8,       RCAR_GEN3_CPG_MSTPSR8,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR9,     RCAR_GEN3_CPG_SRSTCLR9,      RCAR_GEN3_CPG_SMSTPCR9,       RCAR_GEN3_CPG_RMSTPCR9,       RCAR_GEN3_CPG_MSTPSR9,   0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR10,    RCAR_GEN3_CPG_SRSTCLR10,     RCAR_GEN3_CPG_SMSTPCR10,      RCAR_GEN3_CPG_RMSTPCR10,      RCAR_GEN3_CPG_MSTPSR10,  0, 0, 0, 0, 0 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR11,    RCAR_GEN3_CPG_SRSTCLR11,     RCAR_GEN3_CPG_SMSTPCR11,      RCAR_GEN3_CPG_RMSTPCR11,      RCAR_GEN3_CPG_MSTPSR11,  0, 0, 0, 0, 0 ),
};


static const struct _cpg_mstp_offset gen4_mstp_table[] = {
    /*        srcr_offset                srstclr_offset                 smstpcr_offset                       mstpsr_offset               rahsr_offset               fsrchkra_offset               fsrchkrb_offset               fsrchksetr_offset               fsrchkclrr_offset          */
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(0),     RCAR_GEN4_CPG_SRSTCLR(0),      RCAR_GEN4_CPG_MSTPCR(0),     0,      RCAR_GEN4_CPG_MSTPSR(0),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(0),    RCAR_GEN4_CPG_FSRCHKRB(0),    RCAR_GEN4_CPG_FSRCHKSETR(0),    RCAR_GEN4_CPG_FSRCHKCLRR(0) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(1),     RCAR_GEN4_CPG_SRSTCLR(1),      RCAR_GEN4_CPG_MSTPCR(1),     0,      RCAR_GEN4_CPG_MSTPSR(1),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(1),    RCAR_GEN4_CPG_FSRCHKRB(1),    RCAR_GEN4_CPG_FSRCHKSETR(1),    RCAR_GEN4_CPG_FSRCHKCLRR(1) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(2),     RCAR_GEN4_CPG_SRSTCLR(2),      RCAR_GEN4_CPG_MSTPCR(2),     0,      RCAR_GEN4_CPG_MSTPSR(2),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(2),    RCAR_GEN4_CPG_FSRCHKRB(2),    RCAR_GEN4_CPG_FSRCHKSETR(2),    RCAR_GEN4_CPG_FSRCHKCLRR(2) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(3),     RCAR_GEN4_CPG_SRSTCLR(3),      RCAR_GEN4_CPG_MSTPCR(3),     0,      RCAR_GEN4_CPG_MSTPSR(3),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(3),    RCAR_GEN4_CPG_FSRCHKRB(3),    RCAR_GEN4_CPG_FSRCHKSETR(3),    RCAR_GEN4_CPG_FSRCHKCLRR(3) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(4),     RCAR_GEN4_CPG_SRSTCLR(4),      RCAR_GEN4_CPG_MSTPCR(4),     0,      RCAR_GEN4_CPG_MSTPSR(4),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(4),    RCAR_GEN4_CPG_FSRCHKRB(4),    RCAR_GEN4_CPG_FSRCHKSETR(4),    RCAR_GEN4_CPG_FSRCHKCLRR(4) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(5),     RCAR_GEN4_CPG_SRSTCLR(5),      RCAR_GEN4_CPG_MSTPCR(5),     0,      RCAR_GEN4_CPG_MSTPSR(5),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(5),    RCAR_GEN4_CPG_FSRCHKRB(5),    RCAR_GEN4_CPG_FSRCHKSETR(5),    RCAR_GEN4_CPG_FSRCHKCLRR(5) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(6),     RCAR_GEN4_CPG_SRSTCLR(6),      RCAR_GEN4_CPG_MSTPCR(6),     0,      RCAR_GEN4_CPG_MSTPSR(6),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(6),    RCAR_GEN4_CPG_FSRCHKRB(6),    RCAR_GEN4_CPG_FSRCHKSETR(6),    RCAR_GEN4_CPG_FSRCHKCLRR(6) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(7),     RCAR_GEN4_CPG_SRSTCLR(7),      RCAR_GEN4_CPG_MSTPCR(7),     0,      RCAR_GEN4_CPG_MSTPSR(7),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(7),    RCAR_GEN4_CPG_FSRCHKRB(7),    RCAR_GEN4_CPG_FSRCHKSETR(7),    RCAR_GEN4_CPG_FSRCHKCLRR(7) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(8),     RCAR_GEN4_CPG_SRSTCLR(8),      RCAR_GEN4_CPG_MSTPCR(8),     0,      RCAR_GEN4_CPG_MSTPSR(8),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(8),    RCAR_GEN4_CPG_FSRCHKRB(8),    RCAR_GEN4_CPG_FSRCHKSETR(8),    RCAR_GEN4_CPG_FSRCHKCLRR(8) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(9),     RCAR_GEN4_CPG_SRSTCLR(9),      RCAR_GEN4_CPG_MSTPCR(9),     0,      RCAR_GEN4_CPG_MSTPSR(9),    RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(9),    RCAR_GEN4_CPG_FSRCHKRB(9),    RCAR_GEN4_CPG_FSRCHKSETR(9),    RCAR_GEN4_CPG_FSRCHKCLRR(9) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(10),    RCAR_GEN4_CPG_SRSTCLR(10),     RCAR_GEN4_CPG_MSTPCR(10),    0,      RCAR_GEN4_CPG_MSTPSR(10),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(10),   RCAR_GEN4_CPG_FSRCHKRB(10),   RCAR_GEN4_CPG_FSRCHKSETR(10),   RCAR_GEN4_CPG_FSRCHKCLRR(10) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(11),    RCAR_GEN4_CPG_SRSTCLR(11),     RCAR_GEN4_CPG_MSTPCR(11),    0,      RCAR_GEN4_CPG_MSTPSR(11),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(11),   RCAR_GEN4_CPG_FSRCHKRB(11),   RCAR_GEN4_CPG_FSRCHKSETR(11),   RCAR_GEN4_CPG_FSRCHKCLRR(11) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(12),    RCAR_GEN4_CPG_SRSTCLR(12),     RCAR_GEN4_CPG_MSTPCR(12),    0,      RCAR_GEN4_CPG_MSTPSR(12),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(12),   RCAR_GEN4_CPG_FSRCHKRB(12),   RCAR_GEN4_CPG_FSRCHKSETR(12),   RCAR_GEN4_CPG_FSRCHKCLRR(12) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(13),    RCAR_GEN4_CPG_SRSTCLR(13),     RCAR_GEN4_CPG_MSTPCR(13),    0,      RCAR_GEN4_CPG_MSTPSR(13),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(13),   RCAR_GEN4_CPG_FSRCHKRB(13),   RCAR_GEN4_CPG_FSRCHKSETR(13),   RCAR_GEN4_CPG_FSRCHKCLRR(13) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(14),    RCAR_GEN4_CPG_SRSTCLR(14),     RCAR_GEN4_CPG_MSTPCR(14),    0,      RCAR_GEN4_CPG_MSTPSR(14),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(14),   RCAR_GEN4_CPG_FSRCHKRB(14),   RCAR_GEN4_CPG_FSRCHKSETR(14),   RCAR_GEN4_CPG_FSRCHKCLRR(14) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(15),    RCAR_GEN4_CPG_SRSTCLR(15),     RCAR_GEN4_CPG_MSTPCR(15),    0,      RCAR_GEN4_CPG_MSTPSR(15),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(15),   RCAR_GEN4_CPG_FSRCHKRB(15),   RCAR_GEN4_CPG_FSRCHKSETR(15),   RCAR_GEN4_CPG_FSRCHKCLRR(15) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(16),    RCAR_GEN4_CPG_SRSTCLR(16),     RCAR_GEN4_CPG_MSTPCR(16),    0,      RCAR_GEN4_CPG_MSTPSR(16),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(16),   RCAR_GEN4_CPG_FSRCHKRB(16),   RCAR_GEN4_CPG_FSRCHKSETR(16),   RCAR_GEN4_CPG_FSRCHKCLRR(16) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(17),    RCAR_GEN4_CPG_SRSTCLR(17),     RCAR_GEN4_CPG_MSTPCR(17),    0,      RCAR_GEN4_CPG_MSTPSR(17),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(17),   RCAR_GEN4_CPG_FSRCHKRB(17),   RCAR_GEN4_CPG_FSRCHKSETR(17),   RCAR_GEN4_CPG_FSRCHKCLRR(17) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(18),    RCAR_GEN4_CPG_SRSTCLR(18),     RCAR_GEN4_CPG_MSTPCR(18),    0,      RCAR_GEN4_CPG_MSTPSR(18),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(18),   RCAR_GEN4_CPG_FSRCHKRB(18),   RCAR_GEN4_CPG_FSRCHKSETR(18),   RCAR_GEN4_CPG_FSRCHKCLRR(18) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(19),    RCAR_GEN4_CPG_SRSTCLR(19),     RCAR_GEN4_CPG_MSTPCR(19),    0,      RCAR_GEN4_CPG_MSTPSR(19),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(19),   RCAR_GEN4_CPG_FSRCHKRB(19),   RCAR_GEN4_CPG_FSRCHKSETR(19),   RCAR_GEN4_CPG_FSRCHKCLRR(19) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(20),    RCAR_GEN4_CPG_SRSTCLR(20),     RCAR_GEN4_CPG_MSTPCR(20),    0,      RCAR_GEN4_CPG_MSTPSR(20),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(20),   RCAR_GEN4_CPG_FSRCHKRB(20),   RCAR_GEN4_CPG_FSRCHKSETR(20),   RCAR_GEN4_CPG_FSRCHKCLRR(20) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(21),    RCAR_GEN4_CPG_SRSTCLR(21),     RCAR_GEN4_CPG_MSTPCR(21),    0,      RCAR_GEN4_CPG_MSTPSR(21),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(21),   RCAR_GEN4_CPG_FSRCHKRB(21),   RCAR_GEN4_CPG_FSRCHKSETR(21),   RCAR_GEN4_CPG_FSRCHKCLRR(21) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(22),    RCAR_GEN4_CPG_SRSTCLR(22),     RCAR_GEN4_CPG_MSTPCR(22),    0,      RCAR_GEN4_CPG_MSTPSR(22),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(22),   RCAR_GEN4_CPG_FSRCHKRB(22),   RCAR_GEN4_CPG_FSRCHKSETR(22),   RCAR_GEN4_CPG_FSRCHKCLRR(22) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(23),    RCAR_GEN4_CPG_SRSTCLR(23),     RCAR_GEN4_CPG_MSTPCR(23),    0,      RCAR_GEN4_CPG_MSTPSR(23),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(23),   RCAR_GEN4_CPG_FSRCHKRB(23),   RCAR_GEN4_CPG_FSRCHKSETR(23),   RCAR_GEN4_CPG_FSRCHKCLRR(23) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(24),    RCAR_GEN4_CPG_SRSTCLR(24),     RCAR_GEN4_CPG_MSTPCR(24),    0,      RCAR_GEN4_CPG_MSTPSR(24),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(24),   RCAR_GEN4_CPG_FSRCHKRB(24),   RCAR_GEN4_CPG_FSRCHKSETR(24),   RCAR_GEN4_CPG_FSRCHKCLRR(24) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(25),    RCAR_GEN4_CPG_SRSTCLR(25),     RCAR_GEN4_CPG_MSTPCR(25),    0,      RCAR_GEN4_CPG_MSTPSR(25),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(25),   RCAR_GEN4_CPG_FSRCHKRB(25),   RCAR_GEN4_CPG_FSRCHKSETR(25),   RCAR_GEN4_CPG_FSRCHKCLRR(25) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(26),    RCAR_GEN4_CPG_SRSTCLR(26),     RCAR_GEN4_CPG_MSTPCR(26),    0,      RCAR_GEN4_CPG_MSTPSR(26),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(26),   RCAR_GEN4_CPG_FSRCHKRB(26),   RCAR_GEN4_CPG_FSRCHKSETR(26),   RCAR_GEN4_CPG_FSRCHKCLRR(26) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(27),    RCAR_GEN4_CPG_SRSTCLR(27),     RCAR_GEN4_CPG_MSTPCR(27),    0,      RCAR_GEN4_CPG_MSTPSR(27),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(27),   RCAR_GEN4_CPG_FSRCHKRB(27),   RCAR_GEN4_CPG_FSRCHKSETR(27),   RCAR_GEN4_CPG_FSRCHKCLRR(27) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(28),    RCAR_GEN4_CPG_SRSTCLR(28),     RCAR_GEN4_CPG_MSTPCR(28),    0,      RCAR_GEN4_CPG_MSTPSR(28),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(28),   RCAR_GEN4_CPG_FSRCHKRB(28),   RCAR_GEN4_CPG_FSRCHKSETR(28),   RCAR_GEN4_CPG_FSRCHKCLRR(28) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(29),    RCAR_GEN4_CPG_SRSTCLR(29),     RCAR_GEN4_CPG_MSTPCR(29),    0,      RCAR_GEN4_CPG_MSTPSR(29),   RCAR_GEN4_CPG_RAHSR(0),    RCAR_GEN4_CPG_FSRCHKRA(29),   RCAR_GEN4_CPG_FSRCHKRB(29),   RCAR_GEN4_CPG_FSRCHKSETR(29),   RCAR_GEN4_CPG_FSRCHKCLRR(29) ),
};

static int reset_config(const cpg_reg_offset_t* const reg_offset,
                const uint32_t bitmask, const int assert)
{
    int status;
    uint32_t offset;

    if (assert) {
        /* Reset module */
        offset = reg_offset->srcr_offset;
    } else {
        /* Release reset module */
        offset = reg_offset->srstclr_offset;
    }

    cpg_write(offset, bitmask);

    status = cpg_waitfor(reg_offset->srcr_offset, bitmask, assert ? bitmask : 0);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Cannot reset assert/deassert, mask 0x%x (0x%08x)",
                    __func__, bitmask, cpg_read(reg_offset->srcr_offset));
    }
    return status;
}

static const cpg_reg_offset_t* mod_id_parse(const uint32_t soc_id, const int mod_id,
                                                uint32_t* const bitmask)
{
    const struct _cpg_mstp_offset* mstp_off_table;
    uint32_t mstp_off_num;
    const uint32_t reg_idx = CPG_MSTP_ID_TO_REG_OFFSET(mod_id);
    const uint32_t bit = CPG_MSTP_ID_TO_BIT(mod_id);

    if (bit > 31) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid bit %d, mod_id %d",__func__, bit, mod_id);
        return NULL;
    }

    *bitmask = BIT(bit);

    if (IS_RCAR_GEN4(soc_id)) {
        mstp_off_table = gen4_mstp_table;
        mstp_off_num   = ARRAY_SIZE(gen4_mstp_table);
    } else {
        mstp_off_table = gen3_mstp_table;
        mstp_off_num   = ARRAY_SIZE(gen3_mstp_table);
    }

    if (reg_idx >= mstp_off_num) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid reg_idx %d, mstp_off_num %d mod_id %d",
            __func__, reg_idx, mstp_off_num, mod_id);
        return NULL;
    }
    return &mstp_off_table[reg_idx];
}

int mstp_config(const int mod_id, const int enable)
{
    uint32_t tmp;
    uint32_t bitmask;
    int status;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    tmp  = cpg_read(reg_offset->smstpcr_offset);
    if (enable) {
        tmp &= ~bitmask;
    } else {
        tmp |=  bitmask;
    }
    cpg_write(reg_offset->smstpcr_offset, tmp);

    if (!enable)
        return CPG_RET_OK;

    /* Check if change is effected */
    status = cpg_waitfor(reg_offset->mstpsr_offset, bitmask, (enable ? 0 : bitmask));

    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Cannot %s MSTPCR%d mask 0x%x (0x%08x)",
                    __func__, enable ? "enable" : "disable", CPG_MSTP_ID_TO_REG_OFFSET(mod_id),
                    bitmask, cpg_read(reg_offset->smstpcr_offset));
    }

    return status;
}

int mstp_status(const int mod_id, int *status)
{
    uint32_t tmp;
    uint32_t bitmask;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    tmp = cpg_read(reg_offset->mstpsr_offset) & bitmask;
    *status = (tmp == 0);

    return CPG_RET_OK;
}

int mstp_reset(const int mod_id)
{
    int status;
    uint32_t bitmask;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    status = reset_config(reg_offset, bitmask, 1); // assert
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Failed to reset mod_id %d",__func__, mod_id);
        return status;
    }

    /* Wait for at least one cycle of the RCLK clock (32 kHz) */
    nanospin_ns(35000);

    status = reset_config(reg_offset, bitmask, 0); // deassert

    return status;
}

int mstp_reset_config(const int mod_id, const int assert)
{
    uint32_t bitmask;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    return reset_config(reg_offset, bitmask, assert);
}

int mstp_reset_state(const int mod_id, int *status)
{
    uint32_t bitmask, tmp;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    tmp = cpg_read(reg_offset->srcr_offset);
    *status = (tmp & bitmask) ? 1 : 0;

    return CPG_RET_OK;
}

static int check_intented_reset(const cpg_reg_offset_t* const reg_offset, uint32_t bitmask)
{
    uint32_t tmp;
    int retries;

    /* CLear Reset Status Monitor function */
    cpg_write(reg_offset->fsrchkclrr_offset, bitmask);
    for (retries = 1000; retries > 0; --retries) {
        tmp = cpg_read(reg_offset->fsrchkra_offset) & bitmask;
        tmp |= cpg_read(reg_offset->fsrchkrb_offset) & bitmask;
        if (tmp == 0) {
            break;
        }
        nanospin_ns(100);
    }

    if (retries <= 0) {
        return 0;
    }

    /* Issue intented reset */
    cpg_write(reg_offset->fsrchksetr_offset, bitmask);

    /* Check intented reset */
    for (retries = 1000; retries > 0; --retries) {
        tmp = cpg_read(reg_offset->fsrchkra_offset);
        if (tmp & bitmask) {
            tmp = cpg_read(reg_offset->fsrchkrb_offset);
            if (tmp & bitmask) {
                break;
            }
        }
        nanospin_ns(100);
    }

    cpg_write(reg_offset->fsrchkclrr_offset, bitmask);

    if (retries > 0) {
        return 1;
    } else {
        return 0;
    }
}

static void force_reset(const cpg_reg_offset_t* const reg_offset,
                const uint32_t bitmask)
{
    uint32_t tmp;

    tmp = ~bitmask;
    cpg_write(reg_offset->rahsr_offset, tmp);

    /* Assert a software reset */
    cpg_write(reg_offset->srcr_offset, bitmask);

    /* Wait for at least one cycle of the RCLK clock (32 kHz) */
    nanospin_ns(35000);

    /* FIXME : Need to de-assert a software reset here ? */
    //cpg_write(reg_offset->srstclr_offset, bitmask);
}

/*
 * Software Reset flow (A)
 * Base on Figure 9.3.1 of TRM Section 9
 */
int mstp_assert_gen4_a(const int mod_id)
{
    uint32_t bitmask;
    int status, restart = 0;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev", __func__);
        return CPG_RET_INVALPARA;
    }

    if (cpg_dev.soc_id != PRODUCT_ID_RCAR_V4H) {
        cpg_slogf(_SLOG_ERROR, "%s: Not supportting for SoC %x", __func__, cpg_dev.soc_id);
        return CPG_RET_INVALPARA;
    }

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    /* Assert Software reset */
    status = reset_config(reg_offset, bitmask, 1);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Cannot assert reset to mod_id %d", __func__, mod_id);
        return status;
    }

    /* Wait 1ms */
    delay(1);

    /* Intented Reset is OK */
    if (check_intented_reset(reg_offset, bitmask)) {
        /* Intented Reset is OK */
        goto done;
    }

    /* Stop module */
    status = mstp_config(mod_id, 0);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Cannot stop mod_id %d", __func__, mod_id);
        return status;
    }

    restart = 1;

    /* Set to 1 at Node and Slave Bus bit of SRCRn. See Table 9.3.1
     * FIXME : Not find Table 9.3.1
     *         Set to 1 at Node and Slave Bus bit of SRCRn is to assert software reset again?
     */
    status = reset_config(reg_offset, bitmask, 1);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Failed to reset mod_id %d",__func__, mod_id);
        return status;
    }

    /* Wait 1ms */
    delay(1);

    /* Intented Reset is OK ? */
    if (check_intented_reset(reg_offset, bitmask)) {
        /* Assert reset */
        status = reset_config(reg_offset, bitmask, 1);
        if (status != CPG_RET_OK) {
            cpg_slogf(_SLOG_ERROR, "%s: Cannot assert reset to mod_id %d", __func__, mod_id);
            return status;
        }

        /* Wait 1ms */
        delay(1);

        /* Intented Reset is not OK ? */
        if (!check_intented_reset(reg_offset, bitmask)) {
            /* Assert Force Reset */
            force_reset(reg_offset, bitmask);

            /* Wait 1ms */
            delay(1);
        }

        goto done;
    }

    /* Assert Force Reset */
    force_reset(reg_offset, bitmask);

    /* Wait 1ms */
    delay(1);

    /* Intented Reset is not OK ? */
    if (!check_intented_reset(reg_offset, bitmask)) {
        /* Issue reset failed */
        cpg_slogf(_SLOG_ERROR, "%s: Issue reset to mod_id %d failed", __func__, mod_id);
        return CPG_RET_HWFAIL;
    }

done:
    if (restart) {
        /* Restart module */
        status = mstp_config(mod_id, 1);
        if (status != CPG_RET_OK) {
            cpg_slogf(_SLOG_ERROR, "%s: Cannot start mod_id %d", __func__, mod_id);
            return status;
        }
    }

    return CPG_RET_OK;
}

/*
 * Software Reset flow (B)
 * Base on Figure 9.3.2 of TRM Section 9
 */
int mstp_assert_gen4_b(const int mod_id)
{
    uint32_t bitmask;
    int status;

    if (cpg_dev.inited == 0) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid cpg_dev",__func__);
        return CPG_RET_INVALPARA;
    }

    if (cpg_dev.soc_id != PRODUCT_ID_RCAR_V4H) {
        cpg_slogf(_SLOG_ERROR, "%s: Not supportting for SoC %x", __func__, cpg_dev.soc_id);
        return CPG_RET_INVALPARA;
    }

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    /* Assert Software reset */
    status = reset_config(reg_offset, bitmask, 1);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Cannot assert reset to mod_id %d", __func__, mod_id);
        return status;
    }

    /* Wait 1ms */
    delay(1);

    /* Intented Reset is not OK */
    if (!check_intented_reset(reg_offset, bitmask)) {
        /* Issue reset failed */
        cpg_slogf(_SLOG_ERROR, "%s: Issue reset to mod_id %d failed", __func__, mod_id);
        return CPG_RET_HWFAIL;
    }

    return CPG_RET_OK;
}

/*
 * Software Reset flow (C)
 * Base on Figure 9.3.3 of TRM Section 9
 */
int mstp_assert_gen4_c(const int mod_id)
{
    int status;

    if (cpg_dev.soc_id != PRODUCT_ID_RCAR_V4H) {
        cpg_slogf(_SLOG_ERROR, "%s: Not supportting for SoC %x", __func__, cpg_dev.soc_id);
        return CPG_RET_INVALPARA;
    }

    /* Stop module */
    status = mstp_config(mod_id, 0);
    if (status != CPG_RET_OK) {
        cpg_slogf(_SLOG_ERROR, "%s: Cannot stop mod_id %d", __func__, mod_id);
        return status;
    }

    /*
     * Remaining is same as B)
     */
    return mstp_assert_gen4_b(mod_id);
}

int mstp_deassert_gen4(const int mod_id)
{
    return mstp_reset_config(mod_id, 0); // deassert reset
}

/*
 * Software Reset flow (A)
 * Base on Figure 9.3.1 of TRM Section 9
 */
int mstp_reset_gen4_a(const int mod_id)
{
    int status = mstp_assert_gen4_a(mod_id);

    if (status == CPG_RET_OK) {
        /* Reset OK, clear reset state */
        status =  mstp_deassert_gen4(mod_id); // deassert reset
    }

    return status;
}

/*
 * Software Reset flow (B)
 * Base on Figure 9.3.2 of TRM Section 9
 */
int mstp_reset_gen4_b(const int mod_id)
{
    int status = mstp_assert_gen4_b(mod_id);

    if (status == CPG_RET_OK) {
        /* Reset OK, clear reset state */
        status =  mstp_deassert_gen4(mod_id); // deassert reset
    }

    return status;
}

/*
 * Software Reset flow (C)
 * Base on Figure 9.3.3 of TRM Section 9
 */
int mstp_reset_gen4_c(const int mod_id)
{
    int status = mstp_assert_gen4_c(mod_id);

    if (status == CPG_RET_OK) {
        /* Reset OK, clear reset state */
        status =  mstp_deassert_gen4(mod_id); // deassert reset
    }

    return status;
}
