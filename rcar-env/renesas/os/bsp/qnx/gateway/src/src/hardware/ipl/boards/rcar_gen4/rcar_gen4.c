/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#include "rcar_gen4.h"
#include <hw/rcar_cpg_support.h>
#include <hw/rcar_sysc_support.h>
#include <aarch64/r-car.h>
#include <hw/inout.h>
#include <private/rcar_delay.h>


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

#define ARRAY_SIZE(x)       ( (int)(sizeof(x) / sizeof((x)[0])) )
#define BITMASK(x,y)        (((2ULL << (y-x)) - 1) << x)

#define CPG_MSTP(_srcr_offset, _srstclr_offset, _smstpcr_offset, _rmstpcr_offset, _mstpsr_offset) \
{ \
    .srcr_offset  = _srcr_offset, \
    .srstclr_offset = _srstclr_offset, \
    .smstpcr_offset = _smstpcr_offset, \
    .rmstpcr_offset = _rmstpcr_offset, \
    .mstpsr_offset = _mstpsr_offset, \
}


static const struct _cpg_mstp_offset gen4_mstp_table[] = {
    /*              srcr_offset            srstclr_offset               smstpcr_offset         N/A for GEN4    mstpsr_offset    */
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(0),     RCAR_GEN4_CPG_SRSTCLR(0),      RCAR_GEN4_CPG_MSTPCR(0),       0,      RCAR_GEN4_CPG_MSTPSR(0)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(1),     RCAR_GEN4_CPG_SRSTCLR(1),      RCAR_GEN4_CPG_MSTPCR(1),       0,      RCAR_GEN4_CPG_MSTPSR(1)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(2),     RCAR_GEN4_CPG_SRSTCLR(2),      RCAR_GEN4_CPG_MSTPCR(2),       0,      RCAR_GEN4_CPG_MSTPSR(2)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(3),     RCAR_GEN4_CPG_SRSTCLR(3),      RCAR_GEN4_CPG_MSTPCR(3),       0,      RCAR_GEN4_CPG_MSTPSR(3)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(4),     RCAR_GEN4_CPG_SRSTCLR(4),      RCAR_GEN4_CPG_MSTPCR(4),       0,      RCAR_GEN4_CPG_MSTPSR(4)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(5),     RCAR_GEN4_CPG_SRSTCLR(5),      RCAR_GEN4_CPG_MSTPCR(5),       0,      RCAR_GEN4_CPG_MSTPSR(5)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(6),     RCAR_GEN4_CPG_SRSTCLR(6),      RCAR_GEN4_CPG_MSTPCR(6),       0,      RCAR_GEN4_CPG_MSTPSR(6)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(7),     RCAR_GEN4_CPG_SRSTCLR(7),      RCAR_GEN4_CPG_MSTPCR(7),       0,      RCAR_GEN4_CPG_MSTPSR(7)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(8),     RCAR_GEN4_CPG_SRSTCLR(8),      RCAR_GEN4_CPG_MSTPCR(8),       0,      RCAR_GEN4_CPG_MSTPSR(8)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(9),     RCAR_GEN4_CPG_SRSTCLR(9),      RCAR_GEN4_CPG_MSTPCR(9),       0,      RCAR_GEN4_CPG_MSTPSR(9)  ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(10),    RCAR_GEN4_CPG_SRSTCLR(10),     RCAR_GEN4_CPG_MSTPCR(10),      0,      RCAR_GEN4_CPG_MSTPSR(10) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(11),    RCAR_GEN4_CPG_SRSTCLR(11),     RCAR_GEN4_CPG_MSTPCR(11),      0,      RCAR_GEN4_CPG_MSTPSR(11) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(12),    RCAR_GEN4_CPG_SRSTCLR(12),     RCAR_GEN4_CPG_MSTPCR(12),      0,      RCAR_GEN4_CPG_MSTPSR(12) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(13),    RCAR_GEN4_CPG_SRSTCLR(13),     RCAR_GEN4_CPG_MSTPCR(13),      0,      RCAR_GEN4_CPG_MSTPSR(13) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(14),    RCAR_GEN4_CPG_SRSTCLR(14),     RCAR_GEN4_CPG_MSTPCR(14),      0,      RCAR_GEN4_CPG_MSTPSR(14) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(15),    RCAR_GEN4_CPG_SRSTCLR(15),     RCAR_GEN4_CPG_MSTPCR(15),      0,      RCAR_GEN4_CPG_MSTPSR(15) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(16),    RCAR_GEN4_CPG_SRSTCLR(16),     RCAR_GEN4_CPG_MSTPCR(16),      0,      RCAR_GEN4_CPG_MSTPSR(16) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(17),    RCAR_GEN4_CPG_SRSTCLR(17),     RCAR_GEN4_CPG_MSTPCR(17),      0,      RCAR_GEN4_CPG_MSTPSR(17) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(18),    RCAR_GEN4_CPG_SRSTCLR(18),     RCAR_GEN4_CPG_MSTPCR(18),      0,      RCAR_GEN4_CPG_MSTPSR(18) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(19),    RCAR_GEN4_CPG_SRSTCLR(19),     RCAR_GEN4_CPG_MSTPCR(19),      0,      RCAR_GEN4_CPG_MSTPSR(19) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(20),    RCAR_GEN4_CPG_SRSTCLR(20),     RCAR_GEN4_CPG_MSTPCR(20),      0,      RCAR_GEN4_CPG_MSTPSR(20) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(21),    RCAR_GEN4_CPG_SRSTCLR(21),     RCAR_GEN4_CPG_MSTPCR(21),      0,      RCAR_GEN4_CPG_MSTPSR(21) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(22),    RCAR_GEN4_CPG_SRSTCLR(22),     RCAR_GEN4_CPG_MSTPCR(22),      0,      RCAR_GEN4_CPG_MSTPSR(22) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(23),    RCAR_GEN4_CPG_SRSTCLR(23),     RCAR_GEN4_CPG_MSTPCR(23),      0,      RCAR_GEN4_CPG_MSTPSR(23) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(24),    RCAR_GEN4_CPG_SRSTCLR(24),     RCAR_GEN4_CPG_MSTPCR(24),      0,      RCAR_GEN4_CPG_MSTPSR(24) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(25),    RCAR_GEN4_CPG_SRSTCLR(25),     RCAR_GEN4_CPG_MSTPCR(25),      0,      RCAR_GEN4_CPG_MSTPSR(25) ),
    CPG_MSTP( RCAR_GEN4_CPG_SRCR(26),    RCAR_GEN4_CPG_SRSTCLR(26),     RCAR_GEN4_CPG_MSTPCR(26),      0,      RCAR_GEN4_CPG_MSTPSR(26) ),
};


int32_t cpg_read(const uint32_t base, const uint32_t reg)
{
    return in32(base + reg);
}

void cpg_write(const uint32_t base, const uint32_t reg, const uint32_t data)
{
    out32(base + RCAR_GEN4_CPG_CPGWPR, ~data);
    out32(base + reg, data);
}

int cpg_waitfor(const uint32_t base, const uint32_t reg, const uint32_t mask, const uint32_t val)
{
    int i = 1000;
    while(--i) {
        if ( (cpg_read(base, reg) & mask) == val) {
            return 0;
        }
        rcar_usec_delay(100);
    }
    return -1;
}

static const cpg_reg_offset_t* mod_id_parse(const int mod_id, uint32_t* const bitmask)
{
    const struct _cpg_mstp_offset* mstp_off_table;
    uint32_t mstp_off_num;
    const uint32_t reg_idx = CPG_MSTP_ID_TO_REG_OFFSET(mod_id);
    const uint32_t bit = CPG_MSTP_ID_TO_BIT(mod_id);

    if (bit > 31) {
        return NULL;
    }

    *bitmask = BIT(bit);

    mstp_off_table = gen4_mstp_table;
    mstp_off_num   = ARRAY_SIZE(gen4_mstp_table);

    if (reg_idx >= mstp_off_num) {
        return NULL;
    }
    return &mstp_off_table[reg_idx];
}


int mstp_config(uint32_t domain, const int mod_id, const int enable)
{
    uint32_t tmp;
    uint32_t bitmask;
    int status;

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(mod_id, &bitmask);
    if (reg_offset == NULL) {
        return -1;
    }

    tmp  = cpg_read(domain, reg_offset->smstpcr_offset);
    if (enable) {
        tmp &= ~bitmask;
    } else {
        tmp |=  bitmask;
    }
    cpg_write(domain, reg_offset->smstpcr_offset, tmp);

    /* Check if change is effected */
    status = cpg_waitfor(domain, reg_offset->mstpsr_offset, bitmask, (enable ? 0 : bitmask));

    return status;
}


static int reset_config(uint32_t domain, const cpg_reg_offset_t* const reg_offset,
                const uint32_t bitmask, const int assert)
{
    int status;
    uint32_t tmp;
    uint32_t offset;

    if (assert) {
        /* Reset module */
        offset = reg_offset->srcr_offset;
    } else {
        /* Release reset module */
        offset = reg_offset->srstclr_offset;
    }

    tmp  = cpg_read(domain, offset) | bitmask;
    cpg_write(domain, offset, tmp);

    status = cpg_waitfor(domain, reg_offset->srcr_offset, bitmask, assert ? bitmask : 0);

    return status;
}


int mstp_reset(uint32_t domain, const int mod_id)
{
    int status;
    uint32_t bitmask;

    const cpg_reg_offset_t* const reg_offset = mod_id_parse(mod_id, &bitmask);
    if (reg_offset == NULL) {
        return -1;
    }

    status = reset_config(domain, reg_offset, bitmask, 1); // assert
    if (status != 0) {
        return status;
    }

    /* Wait for at least one cycle of the RCLK clock (32 kHz) */
    rcar_usec_delay(35000);

    status = reset_config(domain, reg_offset, bitmask, 0); // deassert

    return status;
}

static void cpg_reg_wr(uint32_t domain, uint32_t reg, uint32_t val)
{
	/* if CPGWPCR.WPE == 1 */
	out32(domain + RCAR_GEN4_CPG_CPGWPR, ~val);
	out32(domain + reg, val);
}

void cpg_config(cpg_t *cfg)
{
	cpg_module_t *module;
    cpg_ctrl_t *ctrl;

	if (cfg->ctrl != NULL) {
		/* First disable related module clocks */
		if (cfg->module != NULL) {
			module = cfg->module;
			while (module->flags) {
				mstp_config(cfg->domain, module->mod_id, 0);
				module++;
			}
		}
		ctrl = cfg->ctrl;
		while (ctrl->reg) {
			cpg_reg_wr(cfg->domain, ctrl->reg, ctrl->val);
			ctrl++;
		}
    }

	if (cfg->module != NULL) {
		module = cfg->module;
		while (module->flags) {
			/* Module Reset */
			if (module->flags & CPG_FLAGS_RST) {
				mstp_reset(cfg->domain, module->mod_id);
			}

			/* Module Clock Enable/Disable */
			mstp_config(cfg->domain, module->mod_id, (module->flags & CPG_FLAGS_EN)?1:0);

			module++;
		}
	}
}

static uint32_t pfc_reg_rd(uint32_t domain, uint32_t gpio_gp, uint32_t reg)
{
	return in32(domain + gpio_gp + reg);
}

static void pfc_reg_wr(uint32_t domain, uint32_t gpio_gp, uint32_t reg, uint32_t val)
{
	//out32(domain + gpio_gp + RCAR_GEN4_GPIO_PMMER, 1);
	out32(domain + gpio_gp + RCAR_GEN4_GPIO_PMMR, ~val);
	out32(domain + gpio_gp + reg, val);
}

static void pfc_group_wr(uint32_t domain, uint32_t gpio_gp, uint32_t reg, uint32_t start_bit, uint32_t end_bit, uint32_t bitlen, uint32_t val)
{
	uint32_t tmp;
	uint32_t shift;

	tmp = pfc_reg_rd(domain, gpio_gp, reg);
	tmp &= ~BITMASK(start_bit, end_bit);
	if(val) {
		for (shift = start_bit; shift <= end_bit; shift += bitlen) {
			tmp |= val << shift;
		}
	}
	pfc_reg_wr(domain, gpio_gp, reg, tmp);
}

void pfc_config(pfc_t *cfg)
{
	uint32_t tmp;

	pfc_reg_t *gpsr;
	pfc_reg_t *ipsr;
	pfc_reg_t *drvctrl;
	pfc_reg_t *com;

    static uint32_t dom = 0;

    /* Enable write PMMR register */
    if (dom != cfg->domain + cfg->gpio_gp) {
        dom = cfg->domain + cfg->gpio_gp;
        out32(dom + RCAR_GEN4_GPIO_PMMER, 1);
    }

	/* Based on R-Car S4 HWM
	 * Section 7.3.2
	 *		gpio_gp->PF: set IPSR, set GPSR=PF
	 *		PF->gpio_gp: set GPSR=gpio_gp
	 *		PF->PF: set GPSR=gpio_gp, set IPSR, set GPSR=PF
	 */
	if (cfg->gpsr != NULL) {
		gpsr = cfg->gpsr;
		while (gpsr->reg) {
			/* Set GPSR to gpio_gp */
			tmp = pfc_reg_rd(cfg->domain, cfg->gpio_gp, gpsr->reg);
			tmp &= ~BITMASK(gpsr->start_bit, gpsr->end_bit);
			pfc_reg_wr(cfg->domain, cfg->gpio_gp, gpsr->reg, tmp);
			
			/* Set GPSR to PF (if necessary) */
			if (gpsr->val) {
				/* IPSR setting */
				if (cfg->ipsr != NULL) {
					ipsr = cfg->ipsr;
					while (ipsr->reg) {
						pfc_group_wr(cfg->domain, cfg->gpio_gp, ipsr->reg, ipsr->start_bit, ipsr->end_bit, ipsr->bitlen, ipsr->val);
						ipsr++;
					}
				}

				/* GPSR setting */
				tmp = pfc_reg_rd(cfg->domain, cfg->gpio_gp, gpsr->reg);
				tmp |= BITMASK(gpsr->start_bit, gpsr->end_bit);
				pfc_reg_wr(cfg->domain, cfg->gpio_gp, gpsr->reg, tmp);
            }
			gpsr++;
		}
	}

	/* DRVCTRL setting */
	if (cfg->drvctrl != NULL) {
		drvctrl = cfg->drvctrl;
		while (drvctrl->reg) {
			pfc_group_wr(cfg->domain, cfg->gpio_gp, drvctrl->reg, drvctrl->start_bit, drvctrl->end_bit, drvctrl->bitlen, drvctrl->val);
			drvctrl++;
		}
	}

	/* Other PFC/gpio_gp setting */
	if (cfg->com != NULL) {
		com = cfg->com;
		while (com->reg) {
			pfc_group_wr(cfg->domain, cfg->gpio_gp, com->reg, com->start_bit, com->end_bit, com->bitlen, com->val);
			com++;
		}
	}
}


void other_config(other_t *o)
{
    while (o->base) {
       if (o->reg_len == 16)  {
            out16(o->base + o->reg, o->val);
        } else  {
            out32(o->base + o->reg, o->val);
        }
        o++;
    }
}

typedef struct sysc_power_domain_ {
    uint8_t chan_idx;       /* Index of PWR*R */
    uint8_t chan_bit;       /* Bit in PWR* (except for PWRUP in PWRSR) */
    uint8_t on_status;      /* Status PWRUP bit in PWRSR */
    uint8_t isr_idx;
    uint8_t isr_bit;        /* Bit in SYSCI*R */
} sysc_power_domain_t;

sysc_power_domain_t s4_sysc_power_domain[] = {
    /*                           chan_idx  chan_bit  on_status  isr_idx  isr_bit  */
    [SYSC_PDR_GEN4_A1E0D0C0] = {   0,       0,         4,        0,       0,  },
    [SYSC_PDR_GEN4_A1E0D0C1] = {   1,       0,         4,        0,       1,  },
    [SYSC_PDR_GEN4_A1E0D1C0] = {   2,       0,         4,        0,       2,  },
    [SYSC_PDR_GEN4_A1E0D1C1] = {   3,       0,         4,        0,       3,  },
    [SYSC_PDR_GEN4_A1E1D0C0] = {   4,       0,         4,        0,       4,  },
    [SYSC_PDR_GEN4_A1E1D0C1] = {   5,       0,         4,        0,       5,  },
    [SYSC_PDR_GEN4_A1E1D1C0] = {   6,       0,         4,        0,       6,  },
    [SYSC_PDR_GEN4_A1E1D1C1] = {   7,       0,         4,        0,       7,  },
    [SYSC_PDR_GEN4_A2E0D0  ] = {  16,       0,         4,        0,      16,  },
    [SYSC_PDR_GEN4_A2E0D1  ] = {  17,       0,         4,        0,      17,  },
    [SYSC_PDR_GEN4_A2E1D0  ] = {  18,       0,         4,        0,      18,  },
    [SYSC_PDR_GEN4_A2E1D1  ] = {  19,       0,         4,        0,      19,  },
    [SYSC_PDR_GEN4_A3E0    ] = {  20,       0,         4,        0,      20,  },
    [SYSC_PDR_GEN4_A3E1    ] = {  21,       0,         4,        0,      21,  },
};

/* For each SYSC domain */
#define SYSC_PWRSR            (uint32_t)(RCAR_GEN4_SYSC_PDRSR(power_domain->chan_idx))       /* Power Status Register */
#define SYSC_PWROFFCR         (uint32_t)(RCAR_GEN4_SYSC_PDROFFCR(power_domain->chan_idx))    /* Power Shutoff Control Register */
#define SYSC_PWRONCR          (uint32_t)(RCAR_GEN4_SYSC_PDRONCR(power_domain->chan_idx))     /* Power Resume Control Register */
#define SYSC_PWRER            (uint32_t)(RCAR_GEN4_SYSC_PDRESR(power_domain->chan_idx))      /* Power Shutoff/Resume Error */
/* For common */
#define SYSC_SR               (uint32_t)(RCAR_GEN4_SYSC_SR)
#define SYSC_IMR              (uint32_t)(((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_IMR0  : RCAR_GEN4_SYSC_IMR1))
#define SYSC_IER              (uint32_t)(((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_IER0  : RCAR_GEN4_SYSC_IER1))
#define SYSC_ISR              (uint32_t)(((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_ISCR0 : RCAR_GEN4_SYSC_ISCR1))
#define SYSC_ISCR             (uint32_t)(((power_domain->isr_idx == 0) ? RCAR_GEN4_SYSC_ISCR0 : RCAR_GEN4_SYSC_ISCR1))


static void domain_power(uint32_t vbase, const int pdr_number,int status)
{
    sysc_power_domain_t *power_domain;
    int i;

    power_domain = &s4_sysc_power_domain[pdr_number];
    if (!power_domain) {
        return;
    }

    /* Mask interrupt */
    out32(vbase + SYSC_IMR, in32(vbase + SYSC_IMR) | BIT(power_domain->isr_bit));

    /* Enable status bits update */
    out32(vbase + SYSC_IER, in32(vbase + SYSC_IER) | BIT(power_domain->isr_bit));

    /* Clear status bit */
    out32(vbase + SYSC_ISCR, BIT(power_domain->isr_bit));

    for (i = 0; i < 100; i++) {
        if (status) {
            /* Submit Power-ON Request */
            out32 (vbase + SYSC_PWRONCR, BIT(power_domain->chan_bit));
        } else {
            /* Submit Power-OFF Request */
            out32 (vbase + SYSC_PWROFFCR, BIT(power_domain->chan_bit));
        }
        /* The Power-OFF request have been accepted? */
        if (!(in32(vbase + SYSC_PWRER) & BIT(power_domain->chan_bit))) {
            break;
        }

        rcar_usec_delay(10000);
    }

    /* Clear status bit */
    out32(vbase + SYSC_ISCR, BIT(power_domain->isr_bit));
}

void sysc_config(sysc_t *s)
{
	sysc_pdr_t *pdr;

	if (s->pdr != NULL) {
		pdr = s->pdr;
		while (pdr->index) {
            domain_power(s->domain, pdr->index, pdr->status);

			pdr++;
		}
	}
}

void soc_config(ip_t *cfg, int exit)
{
	int i = 0;

	for (ip_t *ip = cfg; ip->sysc != (sysc_t *)~0; ip++, i++) {

        if (ip->sysc) {
			/* Power setup */
			for(sysc_t *s = ip->sysc; s->domain; s++) {
				sysc_config(s);
			}
		}

		if (ip->pfc) {
			/* Pinmux setup */
			for(pfc_t *p = ip->pfc; p->domain; p++) {
				pfc_config(p);
			}
		}

		if (ip->cpg) {
			/* Clock setup */
			for(cpg_t *c = ip->cpg; c->domain; c++) {
				cpg_config(c);
			}
		}

        if (ip->other) {
			/* Other setup */
            other_config(ip->other);
		}

        if (exit) break;
	}
}
