/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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

#include "rtsn.h"
#include "m88q2110.h"

#define XMDIO_DEV     0
#define XMDIO_MMD_CTRL_REG      13
#define XMDIO_MMD_CTRL_REG_ADDR    0x0000
#define XMDIO_MMD_CTRL_REG_DATA    0x4000

#define XMDIO_MMD_ADDR_DATA_REG 14
#define XMDIO_MMD_ADDR_DATA_REG_MASK 0xFFFF

extern void rtsnes_mii_write(void *handle, uint8_t phy_id, uint8_t location,
						   uint16_t data);
extern uint16_t rtsnes_mii_read(void *handle, uint8_t phy_id, uint8_t location);

static m88q2110_rev_t phy_rev = M88Q2110_A2;
//static uint16_t post_op_mode = MRVL_88Q2112_OP_MODE_LEGACY;
static uint16_t post_op_mode = MRVL_88Q2112_OP_MODE_DEFAULT;

static char	*opts[] = {
	"opmode",
#define OP_MODE          0
	NULL};

/*
 * Use ":" as separator, other than the regular ","
 * @param optionp Option pointer
 * @param tokens Tokens
 * @param valuep Value pointer
 *
 * @retval -1 if index out of bounds.
 * @retval Index value.
 */
static int m88q2110_getsubopt( char **optionp, char * const *tokens, char **valuep )
{
	char    *p, *opt;
	int     len, index;
	const char *token;

	*valuep = 0;

	for( opt = p = *optionp, len = 0; *p && *p != ':'; p++, len++ ) {
		if( *p == '=' ) {
			for( *valuep = ++p; *p && *p != ':'; p++ ) {
				/* Nothing to do */
			}
			break;
		}
	}

	if( *p ) {
		*p++ = '\0';
	}
	*optionp = p;

	for( index = 0; ( token = *tokens++ ); index++ ) {
		if( *token == *opt && !strncmp( token, opt, len ) && token[len] == '\0' ) {
			return index;
		}
	}

	*valuep = opt;

	return -1;
}

static int parse_bs_options(char *options)
{
	char        *value;
	int         opt;
	unsigned    opmode_cnt = 0;

	while (options && *options != '\0') {
		if ((opt = m88q2110_getsubopt( &options, opts, &value)) == -1) {
			slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s: invalid BS options %s", __func__, options);
			return EINVAL;
		}

		switch (opt) {
			case OP_MODE:
				/* Only A2 PHY can change post op mode */
				if (phy_rev == M88Q2110_A2 && !opmode_cnt) {
					if (strcmp(value, "legacy") == 0) {
						post_op_mode = MRVL_88Q2112_OP_MODE_LEGACY;
					}
					else if (strcmp(value, "default") == 0) {
						post_op_mode = MRVL_88Q2112_OP_MODE_DEFAULT;
					}
					else {
						return EINVAL;
					}
					opmode_cnt++;
				}
				else if (phy_rev != M88Q2110_A2) {
					slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s: opmode can't be changed on A0 and A1 PHY ", __func__);
				}
				break;
			default:
				break;
		}
	}
	return EOK;
}

void mv_mii_write(void *handle, uint8_t dev_id,
				  uint16_t location, uint16_t data)
{
	uint8_t phy_idx = ((rtsnes_dev_t *)handle)->cfg.phy_addr;
    rtsnes_mii_write_cl45(handle, phy_idx, dev_id, location, data);
}

uint16_t mv_mii_read(void *handle, uint8_t dev_id,
							uint16_t location)
{
	uint8_t phy_idx = ((rtsnes_dev_t *)handle)->cfg.phy_addr;
	return rtsnes_mii_read_cl45(handle, phy_idx, dev_id, location);
}

static void setMasterSlave(rtsnes_dev_t *rtsnes, uint16_t forceMaster)
{
	uint16_t reg_data = 0;

	reg_data = mv_mii_read(rtsnes, 1, PMA_PMD_CTRL);

	if (forceMaster == 1){
		reg_data |= 1 << 14;
	}
	else{
		reg_data &= ~(1 << 14);
	}
	mv_mii_write(rtsnes, 1, PMA_PMD_CTRL, reg_data);
}

/* FIXME: Remove registers access that are not available on 88Q2110 */
void softReset(rtsnes_dev_t *rtsnes)
{
	uint16_t regData = 0;
    
    mv_mii_write(rtsnes, 3, 0xFFF3, 0x0024);
    regData = mv_mii_read(rtsnes, 1, 0);
    mv_mii_write(rtsnes, 1, 0, regData | 0x0800);
    mv_mii_write(rtsnes, 3, 0XFFF3, 0x020);
    mv_mii_write(rtsnes, 3, 0XFFE4, 0X0C);
    delay(1);
    mv_mii_write(rtsnes, 3, 0XFFE4, 0X06B6);
    mv_mii_write(rtsnes, 1, 0, regData & 0XF7FF);
    delay(1);
    mv_mii_write(rtsnes, 3, 0XFC47, 0X0030);
    mv_mii_write(rtsnes, 3, 0XFC47, 0X0031);
    mv_mii_write(rtsnes, 3, 0XFC47, 0X0030);
    mv_mii_write(rtsnes, 3, 0XFC47, 0X0000);
    mv_mii_write(rtsnes, 3, 0XFC47, 0X0001);
    mv_mii_write(rtsnes, 3, 0XFC47, 0X0000);
    mv_mii_write(rtsnes, 3, 0X0900, 0X8000);
    mv_mii_write(rtsnes, 1, 0X0900, 0X0000);
    mv_mii_write(rtsnes, 3, 0XFFE4, 0X000C);
}

static void getRevNum(rtsnes_dev_t *rtsnes)
{
	uint16_t dev_addr = 1;
	uint16_t reg_addr = 0x0003;
	uint16_t rev_num = 0;

	rev_num = mv_mii_read(rtsnes, dev_addr, reg_addr);
	rev_num = (rev_num & 0xf);
	slogf(_SLOGC_NETWORK, _SLOG_INFO, "rev_num=%#x", rev_num);
	phy_rev = rev_num;
	return;
}

static void initOperateModeSetting(rtsnes_dev_t *rtsnes)
{
	uint16_t init_op_mode = MRVL_88Q2112_OP_MODE_DEFAULT;

	// set op mode register
	mv_mii_write(rtsnes, 1, 0x0902, init_op_mode |= 1 << 1);

	return;
}

static void applyOperateModeConfig(rtsnes_dev_t *rtsnes)
{
    if (post_op_mode == MRVL_88Q2112_OP_MODE_LEGACY){
        // Enable 1000BT1 legacy mode support
        mv_mii_write(rtsnes, 3, 0xFDB8, 0x0001);
        mv_mii_write(rtsnes, 3, 0xFD3D, 0x0014);
        mv_mii_write(rtsnes, 1, 0x0902, post_op_mode |= 1 << 1);
    }
    else {
        // Set back to default compliant mode setting
        mv_mii_write(rtsnes, 3, 0xFDB8, 0x0000);
        mv_mii_write(rtsnes, 3, 0xFD3D, 0x0000);
        mv_mii_write(rtsnes, 1, 0x0902, 2);
    }

	return;
}

/* FIXME: Remove registers access that are not available on 88Q2110 */
void initQ2112A0Ge(rtsnes_dev_t *rtsnes)
{
	uint16_t reg_data = 0;

    mv_mii_write (rtsnes, 1, 0x0900, 0x4000);

    reg_data = mv_mii_read(rtsnes, 1, 0x0834);
    reg_data = (reg_data & 0xfff0) | 0x01;
    mv_mii_write (rtsnes, 1, 0x0834, reg_data);

    mv_mii_write (rtsnes, 3, 0xffe4, 0x07b5);
    mv_mii_write (rtsnes, 3, 0xffe4, 0x06b6);
    delay(5);

    mv_mii_write (rtsnes, 3, 0xffde, 0x402f);
    mv_mii_write (rtsnes, 3, 0xfe2a, 0x3c3d);
    mv_mii_write (rtsnes, 3, 0xfe34, 0x4040);
    mv_mii_write (rtsnes, 3, 0xfe4b, 0x9337);
    mv_mii_write (rtsnes, 3, 0xfe2a, 0x3c1d);
    mv_mii_write (rtsnes, 3, 0xfe34, 0x0040);
    mv_mii_write (rtsnes, 7, 0x8032, 0x0064);
    mv_mii_write (rtsnes, 7, 0x8031, 0x0a01);
    mv_mii_write (rtsnes, 7, 0x8031, 0x0c01);
    mv_mii_write (rtsnes, 3, 0xfe0f, 0x0000);
    mv_mii_write (rtsnes, 3, 0x800c, 0x0000);
    mv_mii_write (rtsnes, 3, 0x801d, 0x0800);
    mv_mii_write (rtsnes, 3, 0xfc00, 0x01c0);
    mv_mii_write (rtsnes, 3, 0xfc17, 0x0425);
    mv_mii_write (rtsnes, 3, 0xfc94, 0x5470);
    mv_mii_write (rtsnes, 3, 0xfc95, 0x0055);
    mv_mii_write (rtsnes, 3, 0xfc19, 0x08d8);
    mv_mii_write (rtsnes, 3, 0xfc1a, 0x0110);
    mv_mii_write (rtsnes, 3, 0xfc1b, 0x0a10);
    mv_mii_write (rtsnes, 3, 0xfc3a, 0x2725);
    mv_mii_write (rtsnes, 3, 0xfc61, 0x2627);
    mv_mii_write (rtsnes, 3, 0xfc3b, 0x1612);
    mv_mii_write (rtsnes, 3, 0xfc62, 0x1c12);
    mv_mii_write (rtsnes, 3, 0xfc9d, 0x6367);
    mv_mii_write (rtsnes, 3, 0xfc9e, 0x8060);
    mv_mii_write (rtsnes, 3, 0xfc00, 0x01c8);
    mv_mii_write (rtsnes, 3, 0x8000, 0x0000);
    mv_mii_write (rtsnes, 3, 0x8016, 0x0011);

    mv_mii_write (rtsnes, 3, 0xfda3, 0x1800);

    mv_mii_write (rtsnes, 3, 0xfe02, 0x00c0);
    mv_mii_write (rtsnes, 3, 0xffdb, 0x0010);
    mv_mii_write (rtsnes, 3, 0xfff3, 0x0020);
    mv_mii_write (rtsnes, 3, 0xfe40, 0x00a6);

    mv_mii_write (rtsnes, 3, 0xfe60, 0x0000);
    mv_mii_write (rtsnes, 3, 0xfe04, 0x0008);
    mv_mii_write (rtsnes, 3, 0xfe2a, 0x3c3d);
    mv_mii_write (rtsnes, 3, 0xfe4b, 0x9334);

    mv_mii_write (rtsnes, 3, 0xfc10, 0xf600);
    mv_mii_write (rtsnes, 3, 0xfc11, 0x073d);
    mv_mii_write (rtsnes, 3, 0xfc12, 0x000d);
    mv_mii_write (rtsnes, 3, 0xfc13, 0x0010);
}

static void rtsnes_m88q2110_phy_init(rtsnes_dev_t *rtsnes)
{
	initOperateModeSetting(rtsnes);
	setMasterSlave(rtsnes, 1); // Force master
	initQ2112A0Ge(rtsnes);
	applyOperateModeConfig(rtsnes);
	softReset(rtsnes);
}

int rtsnes_mv_phy_init (rtsnes_dev_t *rtsnes)
{
    uint32_t vid;
    int  rc, phy_addr = rtsnes->cfg.phy_addr;

	getRevNum(rtsnes);
	rc = parse_bs_options(rtsnes->bs_options);

	if (rc != EOK) {
		return rc;
	}

	rtsnes_m88q2110_phy_init(rtsnes);

	vid  = (mv_mii_read(rtsnes, 3, 2) << 16);
	vid |= (mv_mii_read(rtsnes, 3, 3));
    rtsnes->mdi->PhyData[phy_addr]->VendorOUI = PHYID_VENDOR(vid);
    rtsnes->mdi->PhyData[phy_addr]->Model = PHYID_MODEL(vid);

    mv_mii_write(rtsnes, 1, PMA_PMD_CTRL, 0xc001);

	return EOK;
}

int rtsnes_is_mv_phy(rtsnes_dev_t *rtsnes)
{
	uint32_t vid;

    if (rtsnes->is_mv_phy == -1) {
        vid  = (mv_mii_read(rtsnes, 3, 2) << 16);
        vid |= (mv_mii_read(rtsnes, 3, 3));
        switch (PHYID_VENDOR(vid)) {
            case MARVELLX:
                rtsnes->is_mv_phy = 1;
                break;
            default:
                rtsnes->is_mv_phy = 0;
                break;
        }
    }        

	return rtsnes->is_mv_phy;
}

void rtsnes_mv_phy_fini(rtsnes_dev_t *rtsnes)
{
	uint16_t reg;
	/* this register is self-clear, so no need to clear the reset bit */
	reg = mv_mii_read(rtsnes, 1, GIGE_T1_CONTROL_REG);
	reg |= GIGE_T1_CONTROL_REG_RESET;
	mv_mii_write(rtsnes, 1, GIGE_T1_CONTROL_REG, reg);
}
