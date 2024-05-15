/*
 * Copyright (c) 2023, BlackBerry Limited.
 * Copyright (c) 2022, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ravb.h"
#include "m88q2110.h"

#define XMDIO_DEV     0
#define XMDIO_MMD_CTRL_REG      13
#define XMDIO_MMD_CTRL_REG_ADDR    0x0000
#define XMDIO_MMD_CTRL_REG_DATA    0x4000

#define XMDIO_MMD_ADDR_DATA_REG 14
#define XMDIO_MMD_ADDR_DATA_REG_MASK 0xFFFF

static m88q2110_rev_t phy_rev = M88Q2110_A2;
//static uint16_t post_op_mode = MRVL_88Q2112_OP_MODE_LEGACY;
static uint16_t post_op_mode = MRVL_88Q2112_OP_MODE_DEFAULT;

static char *opts[] = {
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

static void mv_mii_write(void *handle, uint8_t dev_id,
                  uint16_t location, uint16_t data)
{
    uint8_t phy_idx = ((ravb_dev_t *)handle)->options.cfg.phy_addr;
    ravb_cl45_write(handle, phy_idx, dev_id, location, data);
}

static uint16_t mv_mii_read(void *handle, uint8_t dev_id,
                            uint16_t location)
{
    uint8_t phy_idx = ((ravb_dev_t *)handle)->options.cfg.phy_addr;
    return ravb_cl45_read(handle, phy_idx, dev_id, location);
}

static void setMasterSlave(ravb_dev_t *ravb, uint16_t forceMaster)
{
    uint16_t reg_data = 0;

    reg_data = mv_mii_read(ravb, 1, PMA_PMD_CTRL);

    if (forceMaster == 1){
        reg_data |= 1 << 14;
    }
    else{
        reg_data &= ~(1 << 14);
    }
    mv_mii_write(ravb, 1, PMA_PMD_CTRL, reg_data);
}

/* FIXME: Remove registers access that are not available on 88Q2110 */
static void softReset(ravb_dev_t *ravb)
{
    uint16_t regData = 0;

    mv_mii_write(ravb, 3, 0xFFF3, 0x0024);
    regData = mv_mii_read(ravb, 1, 0);
    mv_mii_write(ravb, 1, 0, regData | 0x0800);
    mv_mii_write(ravb, 3, 0xFFF3, 0x020);
    mv_mii_write(ravb, 3, 0xFFE4, 0x0C);
    delay(1);
    mv_mii_write(ravb, 3, 0xFFE4, 0x06B6);
    mv_mii_write(ravb, 1, 0, regData & 0xF7FF);
    delay(1);
    mv_mii_write(ravb, 3, 0xFC47, 0x0030);
    mv_mii_write(ravb, 3, 0xFC47, 0x0031);
    mv_mii_write(ravb, 3, 0xFC47, 0x0030);
    mv_mii_write(ravb, 3, 0xFC47, 0x0000);
    mv_mii_write(ravb, 3, 0xFC47, 0x0001);
    mv_mii_write(ravb, 3, 0xFC47, 0x0000);
    mv_mii_write(ravb, 3, 0x0900, 0x8000); // Software reset
    mv_mii_write(ravb, 1, 0x0900, 0x0000); // Normal operation
    mv_mii_write(ravb, 3, 0xFFE4, 0x000C);
}

static void getRevNum(ravb_dev_t *ravb)
{
    uint16_t dev_addr = 1;
    uint16_t reg_addr = 0x0003;
    uint16_t rev_num = 0;

    rev_num = mv_mii_read(ravb, dev_addr, reg_addr);
    rev_num = (rev_num & 0xf);
    slogf(_SLOGC_NETWORK, _SLOG_INFO, "rev_num=%#x", rev_num);
    phy_rev = rev_num;
    return;
}

static void initOperateModeSetting(ravb_dev_t *ravb)
{
	uint16_t init_op_mode = MRVL_88Q2112_OP_MODE_DEFAULT;

	// set op mode register
	mv_mii_write(ravb, 1, 0x0902, init_op_mode |= 1 << 1);

	return;
}

static void applyOperateModeConfig(ravb_dev_t *ravb)
{
    if (post_op_mode == MRVL_88Q2112_OP_MODE_LEGACY){
        // Enable 1000BT1 legacy mode support
        mv_mii_write(ravb, 3, 0xFDB8, 0x0001);
        mv_mii_write(ravb, 3, 0xFD3D, 0x0014);
        mv_mii_write(ravb, 1, 0x0902, post_op_mode |= 1 << 1);
    }
    else {
        // Set back to default compliant mode setting
        mv_mii_write(ravb, 3, 0xFDB8, 0x0000);
        mv_mii_write(ravb, 3, 0xFD3D, 0x0000);
        mv_mii_write(ravb, 1, 0x0902, 2);
    }

    return;
}

/* FIXME: Remove registers access that are not available on 88Q2110 */
static void initQ2112A0Ge(ravb_dev_t *ravb)
{
    uint32_t reg_data = 0;
    
    mv_mii_write (ravb, 1, 0x0900, 0x4000);

    reg_data = mv_mii_read(ravb, 1, 0x0834);
    reg_data = (reg_data & 0xfff0) | 0x01;
    mv_mii_write (ravb, 1, 0x0834, reg_data);

    mv_mii_write (ravb, 3, 0xffe4, 0x07b5);
    mv_mii_write (ravb, 3, 0xffe4, 0x06b6);
    delay(5);

    mv_mii_write (ravb, 3, 0xffde, 0x402f);
    mv_mii_write (ravb, 3, 0xfe2a, 0x3c3d);
    mv_mii_write (ravb, 3, 0xfe34, 0x4040);
    mv_mii_write (ravb, 3, 0xfe4b, 0x9337);
    mv_mii_write (ravb, 3, 0xfe2a, 0x3c1d);
    mv_mii_write (ravb, 3, 0xfe34, 0x0040);
    mv_mii_write (ravb, 7, 0x8032, 0x0064);
    mv_mii_write (ravb, 7, 0x8031, 0x0a01);
    mv_mii_write (ravb, 7, 0x8031, 0x0c01);
    mv_mii_write (ravb, 3, 0xfe0f, 0x0000);
    mv_mii_write (ravb, 3, 0x800c, 0x0000);
    mv_mii_write (ravb, 3, 0x801d, 0x0800);
    mv_mii_write (ravb, 3, 0xfc00, 0x01c0);
    mv_mii_write (ravb, 3, 0xfc17, 0x0425);
    mv_mii_write (ravb, 3, 0xfc94, 0x5470);
    mv_mii_write (ravb, 3, 0xfc95, 0x0055);
    mv_mii_write (ravb, 3, 0xfc19, 0x08d8);
    mv_mii_write (ravb, 3, 0xfc1a, 0x0110);
    mv_mii_write (ravb, 3, 0xfc1b, 0x0a10);
    mv_mii_write (ravb, 3, 0xfc3a, 0x2725);
    mv_mii_write (ravb, 3, 0xfc61, 0x2627);
    mv_mii_write (ravb, 3, 0xfc3b, 0x1612);
    mv_mii_write (ravb, 3, 0xfc62, 0x1c12);
    mv_mii_write (ravb, 3, 0xfc9d, 0x6367);
    mv_mii_write (ravb, 3, 0xfc9e, 0x8060);
    mv_mii_write (ravb, 3, 0xfc00, 0x01c8);
    mv_mii_write (ravb, 3, 0x8000, 0x0000);
    mv_mii_write (ravb, 3, 0x8016, 0x0011);

    mv_mii_write (ravb, 3, 0xfda3, 0x1800);

    mv_mii_write (ravb, 3, 0xfe02, 0x00c0);
    mv_mii_write (ravb, 3, 0xffdb, 0x0010);
    mv_mii_write (ravb, 3, 0xfff3, 0x0020);
    mv_mii_write (ravb, 3, 0xfe40, 0x00a6);

    mv_mii_write (ravb, 3, 0xfe60, 0x0000);
    mv_mii_write (ravb, 3, 0xfe04, 0x0008);
    mv_mii_write (ravb, 3, 0xfe2a, 0x3c3d);
    mv_mii_write (ravb, 3, 0xfe4b, 0x9334);

    mv_mii_write (ravb, 3, 0xfc10, 0xf600);
    mv_mii_write (ravb, 3, 0xfc11, 0x073d);
    mv_mii_write (ravb, 3, 0xfc12, 0x000d);
    mv_mii_write (ravb, 3, 0xfc13, 0x0010);
}

static void ravb_m88q2110_phy_init(ravb_dev_t *ravb)
{
    initOperateModeSetting(ravb);
    initQ2112A0Ge(ravb);
    applyOperateModeConfig(ravb);
    setMasterSlave(ravb, 1); // Force master
    softReset(ravb);
}

int ravb_mv_phy_init (ravb_dev_t *ravb)
{
    int  rc;

    getRevNum(ravb);
    rc = parse_bs_options(ravb->options.m88q2110_opt);
    if (rc != EOK) {
        return rc;
    }

    ravb_m88q2110_phy_init(ravb);

    mv_mii_write(ravb, 1, PMA_PMD_CTRL, 0xc001);

    return EOK;
}

int ravb_is_mv_phy(ravb_dev_t *ravb)
{
    uint32_t vid;

    if (ravb->is_mv_phy == -1) {
        vid  = (mv_mii_read(ravb, 3, 2) << 16);
        vid |= (mv_mii_read(ravb, 3, 3));
        switch (PHYID_VENDOR(vid)) {
            case MARVELLX:
                ravb->mdi->PhyData[ravb->options.cfg.phy_addr]->VendorOUI = PHYID_VENDOR(vid);
                ravb->mdi->PhyData[ravb->options.cfg.phy_addr]->Model = PHYID_MODEL(vid);
                ravb->is_mv_phy = 1;
                break;
            default:
                ravb->is_mv_phy = 0;
                break;
        }
    }

    return ravb->is_mv_phy;
}

void ravb_mv_phy_fini(ravb_dev_t *ravb)
{
    uint16_t reg;
    /* this register is self-clear, so no need to clear the reset bit */
    reg = mv_mii_read(ravb, 1, GIGE_T1_CONTROL_REG);
    reg |= GIGE_T1_CONTROL_REG_RESET;
    mv_mii_write(ravb, 1, GIGE_T1_CONTROL_REG, reg);
}

/*
 * Check for link up/down on Marvell PHY
 */
void ravb_mv_phy_state(ravb_dev_t *ravb)
{
    int             linkup = 0;
    uint16_t        sts, ana;
    nic_config_t    *cfg;
    struct ifnet    *ifp;

    cfg = &ravb->options.cfg;
    ifp = &ravb->ecom.ec_if;

    if (ravb->mdi->PhyData[cfg->phy_addr]->Model != MV88Q2110) {
        return;
    }

    nic_mutex_lock(&ravb->mdi_mutex);
    sts = mv_mii_read (ravb, 3, GIGE_T1_STATUS_REG);
    ana = mv_mii_read (ravb, 7, AUTONEG_STATUS_REG);
    nic_mutex_unlock(&ravb->mdi_mutex);

    if(sts & GIGE_T1_STATUS_REG_LINK_STATUS) {
        if((ana & AUTONEG_STATUS_BOTH_RX_STATUS) == AUTONEG_STATUS_BOTH_RX_STATUS) {
            linkup = 1;
        }
    }

    if ((cfg->flags & NIC_FLAG_LINK_DOWN) && linkup) {
        uint32_t val, val1;
        /* Link was down and is now up */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(device index %d): link up", __FUNCTION__, ravb->options.cfg.device_index);
        }
        cfg->flags &= ~NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_UP);
        cfg->media_rate = PHY_SPEED_1000M;
        cfg->duplex = 1;

        /* Program speed/duplex setting in to the MAC */
        val = in32(ravb->base + ECMR);
        val &= ~(ECMR_RE | ECMR_TE);
        out32(ravb->base + ECMR, val);
        val1 = in32(ravb->base + GECMR) | GECMR_SPEED_1000;
        out32(ravb->base + GECMR, val1);
        val |= (ECMR_RE | ECMR_TE | ECMR_DM);
        out32(ravb->base + ECMR, val);
    } else if (((cfg->flags & NIC_FLAG_LINK_DOWN) == 0) && (linkup == 0)) {
        /* Link was up and is now down */
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_INFO, "%s(device index %d): link down", __FUNCTION__, ravb->options.cfg.device_index);
        }
        cfg->flags |= NIC_FLAG_LINK_DOWN;
        if_link_state_change(ifp, LINK_STATE_DOWN);
    }
}
