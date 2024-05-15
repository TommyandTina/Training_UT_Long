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

/* Structure for CPG register */
typedef struct _cpg_mstp_offset {
    uint32_t srcr_offset;
    uint32_t srstclr_offset;
    uint32_t smstpcr_offset;
    uint32_t rmstpcr_offset;
    uint32_t mstpsr_offset;
} cpg_reg_offset_t;

typedef struct {
    int clock;
    uint8_t stc;
} cpg_stc_t;

#define BITMASK(x,y)        (((2ULL << (y-x)) - 1) << x)

#define CPG_MSTP(_srcr_offset, _srstclr_offset, _smstpcr_offset, _rmstpcr_offset, _mstpsr_offset) \
{ \
    .srcr_offset  = _srcr_offset, \
    .srstclr_offset = _srstclr_offset, \
    .smstpcr_offset = _smstpcr_offset, \
    .rmstpcr_offset = _rmstpcr_offset, \
    .mstpsr_offset = _mstpsr_offset, \
}


static const struct _cpg_mstp_offset gen3_mstp_table[] = {
    /*         srcr_offset                srstclr_offset               smstpcr_offset                rmstpcr_offset             mstpsr_offset    */
    CPG_MSTP( RCAR_GEN3_CPG_SRCR0,     RCAR_GEN3_CPG_SRSTCLR0,      RCAR_GEN3_CPG_SMSTPCR0,       RCAR_GEN3_CPG_RMSTPCR0,       RCAR_GEN3_CPG_MSTPSR0  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR1,     RCAR_GEN3_CPG_SRSTCLR1,      RCAR_GEN3_CPG_SMSTPCR1,       RCAR_GEN3_CPG_RMSTPCR1,       RCAR_GEN3_CPG_MSTPSR1  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR2,     RCAR_GEN3_CPG_SRSTCLR2,      RCAR_GEN3_CPG_SMSTPCR2,       RCAR_GEN3_CPG_RMSTPCR2,       RCAR_GEN3_CPG_MSTPSR2  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR3,     RCAR_GEN3_CPG_SRSTCLR3,      RCAR_GEN3_CPG_SMSTPCR3,       RCAR_GEN3_CPG_RMSTPCR3,       RCAR_GEN3_CPG_MSTPSR3  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR4,     RCAR_GEN3_CPG_SRSTCLR4,      RCAR_GEN3_CPG_SMSTPCR4,       RCAR_GEN3_CPG_RMSTPCR4,       RCAR_GEN3_CPG_MSTPSR4  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR5,     RCAR_GEN3_CPG_SRSTCLR5,      RCAR_GEN3_CPG_SMSTPCR5,       RCAR_GEN3_CPG_RMSTPCR5,       RCAR_GEN3_CPG_MSTPSR5  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR6,     RCAR_GEN3_CPG_SRSTCLR6,      RCAR_GEN3_CPG_SMSTPCR6,       RCAR_GEN3_CPG_RMSTPCR6,       RCAR_GEN3_CPG_MSTPSR6  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR7,     RCAR_GEN3_CPG_SRSTCLR7,      RCAR_GEN3_CPG_SMSTPCR7,       RCAR_GEN3_CPG_RMSTPCR7,       RCAR_GEN3_CPG_MSTPSR7  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR8,     RCAR_GEN3_CPG_SRSTCLR8,      RCAR_GEN3_CPG_SMSTPCR8,       RCAR_GEN3_CPG_RMSTPCR8,       RCAR_GEN3_CPG_MSTPSR8  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR9,     RCAR_GEN3_CPG_SRSTCLR9,      RCAR_GEN3_CPG_SMSTPCR9,       RCAR_GEN3_CPG_RMSTPCR9,       RCAR_GEN3_CPG_MSTPSR9  ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR10,    RCAR_GEN3_CPG_SRSTCLR10,     RCAR_GEN3_CPG_SMSTPCR10,      RCAR_GEN3_CPG_RMSTPCR10,      RCAR_GEN3_CPG_MSTPSR10 ),
    CPG_MSTP( RCAR_GEN3_CPG_SRCR11,    RCAR_GEN3_CPG_SRSTCLR11,     RCAR_GEN3_CPG_SMSTPCR11,      RCAR_GEN3_CPG_RMSTPCR11,      RCAR_GEN3_CPG_MSTPSR11 ),
};

static const struct _cpg_mstp_offset v3u_mstp_table[] = {
    /*                           srcr_offset            srstclr_offset               smstpcr_offset       N/A for V3U      mstpsr_offset    */
    CPG_MSTP( RCAR_V3U_CPG_SRCR0,     RCAR_V3U_CPG_SRSTCLR0,      RCAR_V3U_CPG_MSTPCR0,       0,      RCAR_V3U_CPG_MSTPSR0  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR1,     RCAR_V3U_CPG_SRSTCLR1,      RCAR_V3U_CPG_MSTPCR1,       0,      RCAR_V3U_CPG_MSTPSR1  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR2,     RCAR_V3U_CPG_SRSTCLR2,      RCAR_V3U_CPG_MSTPCR2,       0,      RCAR_V3U_CPG_MSTPSR2  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR3,     RCAR_V3U_CPG_SRSTCLR3,      RCAR_V3U_CPG_MSTPCR3,       0,      RCAR_V3U_CPG_MSTPSR3  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR4,     RCAR_V3U_CPG_SRSTCLR4,      RCAR_V3U_CPG_MSTPCR4,       0,      RCAR_V3U_CPG_MSTPSR4  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR5,     RCAR_V3U_CPG_SRSTCLR5,      RCAR_V3U_CPG_MSTPCR5,       0,      RCAR_V3U_CPG_MSTPSR5  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR6,     RCAR_V3U_CPG_SRSTCLR6,      RCAR_V3U_CPG_MSTPCR6,       0,      RCAR_V3U_CPG_MSTPSR6  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR7,     RCAR_V3U_CPG_SRSTCLR7,      RCAR_V3U_CPG_MSTPCR7,       0,      RCAR_V3U_CPG_MSTPSR7  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR8,     RCAR_V3U_CPG_SRSTCLR8,      RCAR_V3U_CPG_MSTPCR8,       0,      RCAR_V3U_CPG_MSTPSR8  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR9,     RCAR_V3U_CPG_SRSTCLR9,      RCAR_V3U_CPG_MSTPCR9,       0,      RCAR_V3U_CPG_MSTPSR9  ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR10,    RCAR_V3U_CPG_SRSTCLR10,     RCAR_V3U_CPG_MSTPCR10,      0,      RCAR_V3U_CPG_MSTPSR10 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR11,    RCAR_V3U_CPG_SRSTCLR11,     RCAR_V3U_CPG_MSTPCR11,      0,      RCAR_V3U_CPG_MSTPSR11 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR12,    RCAR_V3U_CPG_SRSTCLR12,     RCAR_V3U_CPG_MSTPCR12,      0,      RCAR_V3U_CPG_MSTPSR12 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR13,    RCAR_V3U_CPG_SRSTCLR13,     RCAR_V3U_CPG_MSTPCR13,      0,      RCAR_V3U_CPG_MSTPSR13 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR14,    RCAR_V3U_CPG_SRSTCLR14,     RCAR_V3U_CPG_MSTPCR14,      0,      RCAR_V3U_CPG_MSTPSR14 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR15,    RCAR_V3U_CPG_SRSTCLR15,     RCAR_V3U_CPG_MSTPCR15,      0,      RCAR_V3U_CPG_MSTPSR15 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR16,    RCAR_V3U_CPG_SRSTCLR16,     RCAR_V3U_CPG_MSTPCR16,      0,      RCAR_V3U_CPG_MSTPSR16 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR17,    RCAR_V3U_CPG_SRSTCLR17,     RCAR_V3U_CPG_MSTPCR17,      0,      RCAR_V3U_CPG_MSTPSR17 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR18,    RCAR_V3U_CPG_SRSTCLR18,     RCAR_V3U_CPG_MSTPCR18,      0,      RCAR_V3U_CPG_MSTPSR18 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR19,    RCAR_V3U_CPG_SRSTCLR19,     RCAR_V3U_CPG_MSTPCR19,      0,      RCAR_V3U_CPG_MSTPSR19 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR20,    RCAR_V3U_CPG_SRSTCLR20,     RCAR_V3U_CPG_MSTPCR20,      0,      RCAR_V3U_CPG_MSTPSR20 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR21,    RCAR_V3U_CPG_SRSTCLR21,     RCAR_V3U_CPG_MSTPCR21,      0,      RCAR_V3U_CPG_MSTPSR21 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR22,    RCAR_V3U_CPG_SRSTCLR22,     RCAR_V3U_CPG_MSTPCR22,      0,      RCAR_V3U_CPG_MSTPSR22 ),
    CPG_MSTP( RCAR_V3U_CPG_SRCR23,    RCAR_V3U_CPG_SRSTCLR23,     0,                          0,      0                     ),
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

    if(soc_id == PRODUCT_ID_RCAR_V3U) {
        mstp_off_table = v3u_mstp_table;
        mstp_off_num   = ARRAY_SIZE(v3u_mstp_table);
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

    cpg_slogf(_SLOG_DEBUG1, "%s: mod_id %d enable %d",__func__, mod_id, enable);

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

    cpg_slogf(_SLOG_DEBUG1, "%s: mod_id %d",__func__, mod_id);

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

    cpg_slogf(_SLOG_DEBUG1, "%s: mod_id %d",__func__, mod_id);

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    status = reset_config(reg_offset, bitmask, 1); // assert
    if (status != CPG_RET_OK) {
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

    cpg_slogf(_SLOG_DEBUG1, "%s: mod_id %d assert %d",__func__, mod_id, assert);

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

    cpg_slogf(_SLOG_DEBUG1, "%s: mod_id %d reset state",__func__, mod_id);

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(cpg_dev.soc_id, mod_id, &bitmask);
    if (reg_offset == NULL) {
        cpg_slogf(_SLOG_ERROR, "%s: Invalid mod_id %d",__func__, mod_id);
        return CPG_RET_INVALPARA;
    }

    tmp = cpg_read(reg_offset->srcr_offset);
    *status = (tmp & bitmask) ? 1 : 0;

    return CPG_RET_OK;
}

