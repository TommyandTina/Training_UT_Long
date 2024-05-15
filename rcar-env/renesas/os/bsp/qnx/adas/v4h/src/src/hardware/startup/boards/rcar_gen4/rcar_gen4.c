/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

#include "startup.h"
#include "rcar_startup.h"
#include "rcar_gen4.h"
#include <hw/rcar_cpg.h>
#include <hw/rcar_sysc.h>
#include <aarch64/r-car.h>

#define TMU_START0  (1 << 0)
#define TMU_UNF     (1 << 8)

static uint32_t tmu_base = RCAR_TMU_0_2_BASE;

static void tmu_delay(uint32_t cycles)
{
	uint8_t start_reg;

	/* Stop TMU0 before changing settings */
	start_reg = in8(tmu_base + RCAR_TMU_TSTR);
	out8(tmu_base + RCAR_TMU_TSTR, start_reg & ~RCAR_TMU_START0);

	out16(tmu_base + RCAR_TMU_TCR0, 0);
	out32(tmu_base + RCAR_TMU_TCOR0, cycles);
	out32(tmu_base + RCAR_TMU_TCNT0, cycles);

	/* Start TMU0 */
	out8(tmu_base + RCAR_TMU_TSTR, start_reg | RCAR_TMU_START0);

	/* Check the countdown is 0 */
	while (!(in16(tmu_base + RCAR_TMU_TCR0) & RCAR_TMU_UNF));

	/* stop count */
	out8(tmu_base + RCAR_TMU_TSTR, start_reg & ~RCAR_TMU_START0);
}

/*
 * TMU0 counts at 16.6667MHz/4:
 *  => 1 cycle  = 240 nseconds
 *  => 4 cycles = 960 nseconds ~ 1 useconds
 */
void usec_delay(uint32_t usec)
{
	tmu_delay(usec << 2);
}

/* Define override function for libcpg and libsysc */
void mynanospin_ns(unsigned long nsec)
{
	/* Cannot fully achieve nanospin since each TMU0 cycle is a minimum of 240 nsec */
	tmu_delay(nsec/240 + 1);
}


#if defined(CPG_CLK_DIRECT_ACCESS)
static void cpg_reg_wr(uint32_t domain, uint32_t reg, uint32_t val)
{
	/* if CPGWPCR.WPE == 1 */
	out32(domain + RCAR_GEN4_CPG_CPGWPR, ~val);
	out32(domain + reg, val);
}
#endif


void cpg_config(cpg_t *cfg)
{
	cpg_module_t *module;
	cpg_ctrl_t *ctrl;

	if (cfg->ctrl != NULL) {
		/* First disable related module clocks */
		if (cfg->module != NULL) {
			module = cfg->module;
			while (module->flags) {
				cpg_support_funcs.mstp_config(module->mod_id, 0);
				module++;
			}
		}

		/* Set clock control registers: avoid clock calculation in libcpg by setting registers directly */
		ctrl = cfg->ctrl;
#if defined(CPG_CLK_DIRECT_ACCESS)
		while (ctrl->reg) {
			cpg_reg_wr(cfg->domain, ctrl->reg, ctrl->val);
			RCAR_DEBUG_PRINT("%s CTRL domain=0x%x reg=0x%x val=0x%x\n", __FUNCTION__, cfg->domain, ctrl->reg, in32(cfg->domain, ctrl->reg));
			ctrl++;
		}
#else
		while (ctrl->freq) {
            cpg_support_funcs.clk_rate(ctrl->clk_id, ctrl->idx, &ctrl->freq, 1);
			RCAR_DEBUG_PRINT("%s CTRL clk_id=0x%x idx=0x%x ferq=%d\n", __FUNCTION__,
                    ctrl->clk_id, ctrl->idx, ctrl->freq);
			ctrl++;
		}
#endif
	}

	if (cfg->module != NULL) {
		module = cfg->module;
		while (module->flags) {
			/* Module Reset */
			if (module->flags & CPG_FLAGS_RST) {
				RCAR_DEBUG_PRINT("%s RST domain=0x%x module=%d start_bit=%d end_bit=%d\n", __FUNCTION__, cfg->domain, module->index, module->start_bit, module->end_bit);
				cpg_support_funcs.mstp_reset(module->mod_id);
			}

			/* Module Clock Enable/Disable */
			RCAR_DEBUG_PRINT("%s %s domain=0x%x module=%d start_bit=%d end_bit=%d\n", __FUNCTION__, (module->flags & CPG_FLAGS_EN)?"EN":"DIS", cfg->domain, module->index, module->start_bit, module->end_bit);
			cpg_support_funcs.mstp_config(module->mod_id, (module->flags & CPG_FLAGS_EN)?1:0);

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

	/* Based on R-Car S4/V4H HWM
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
			RCAR_DEBUG_PRINT("%s GPIO 0x%x:0x%x\n", __FUNCTION__, cfg->domain + cfg->gpio_gp + gpsr->reg, pfc_reg_rd(cfg->domain, cfg->gpio_gp, gpsr->reg));

			/* Set GPSR to PF (if necessary) */
			if (gpsr->val) {
				/* IPSR setting */
				if (cfg->ipsr != NULL) {
					ipsr = cfg->ipsr;
					while (ipsr->reg) {
						pfc_group_wr(cfg->domain, cfg->gpio_gp, ipsr->reg, ipsr->start_bit, ipsr->end_bit, ipsr->bitlen, ipsr->val);
						RCAR_DEBUG_PRINT("%s IPSR 0x%x:0x%x\n", __FUNCTION__, cfg->domain + cfg->gpio_gp + ipsr->reg, pfc_reg_rd(cfg->domain, cfg->gpio_gp, ipsr->reg));
						ipsr++;
					}
				}

				/* GPSR setting */
				tmp = pfc_reg_rd(cfg->domain, cfg->gpio_gp, gpsr->reg);
				tmp |= BITMASK(gpsr->start_bit, gpsr->end_bit);
				pfc_reg_wr(cfg->domain, cfg->gpio_gp, gpsr->reg, tmp);
				RCAR_DEBUG_PRINT("%s GPSR 0x%x:0x%x\n", __FUNCTION__, cfg->domain + cfg->gpio_gp + gpsr->reg, pfc_reg_rd(cfg->domain, cfg->gpio_gp, gpsr->reg));
			}
			gpsr++;
		}
	}

	/* DRVCTRL setting */
	if (cfg->drvctrl != NULL) {
		drvctrl = cfg->drvctrl;
		while (drvctrl->reg) {
			pfc_group_wr(cfg->domain, cfg->gpio_gp, drvctrl->reg, drvctrl->start_bit, drvctrl->end_bit, drvctrl->bitlen, drvctrl->val);
			RCAR_DEBUG_PRINT("%s DRVCTRL 0x%x=0x%x\n", __FUNCTION__, cfg->domain + cfg->gpio_gp + drvctrl->reg, pfc_reg_rd(cfg->domain, cfg->gpio_gp, drvctrl->reg));
			drvctrl++;
		}
	}

	/* Other PFC/gpio_gp setting */
	if (cfg->com != NULL) {
		com = cfg->com;
		while (com->reg) {
			pfc_group_wr(cfg->domain, cfg->gpio_gp, com->reg, com->start_bit, com->end_bit, com->bitlen, com->val);
			RCAR_DEBUG_PRINT("%s COM 0x%x=0x%x\n", __FUNCTION__, cfg->domain + cfg->gpio_gp + com->reg, pfc_reg_rd(cfg->domain, cfg->gpio_gp, com->reg));
			com++;
		}
	}
}

void sysc_config(sysc_t *cfg)
{
	sysc_pdr_t *pdr;

	if (cfg->pdr != NULL) {
		pdr = cfg->pdr;
		while (pdr->index) {
			RCAR_DEBUG_PRINT("%s domain=0x%x pdr=%d status=%d\n", __FUNCTION__, cfg->domain, pdr->index, pdr->status);
			sysc_support_funcs.power_set(pdr->index, pdr->status);

			pdr++;
		}
	}
}

void other_config(other_t *o)
{
	while (o->base) {
		RCAR_DEBUG_PRINT("%s base=0x%x reg=0x%x value=0x%x reg_len=%d\n", __FUNCTION__, o->base, o->reg, o->val, o->reg_len);
		if (o->reg_len == 16)  {
			out16(o->base + o->reg, o->val);
		} else  {
			out32(o->base + o->reg, o->val);
		}
		o++;
	}
}


void soc_config(ip_t *cfg)
{
	int i = 0;

	for (ip_t *ip = cfg; ip->sysc != (sysc_t *)~0; ip++, i++) {
		RCAR_DEBUG_PRINT("ip[%d]\n", i);
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
	}
}
