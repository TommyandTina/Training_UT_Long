/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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

#include "proto.h"

#ifdef VARIANT_dll
#include <stdio.h>
#include <fcntl.h>
#include <hw/i2c.h>
#endif

#define BD9571_REG_SETVID       (0x54)

typedef struct ca57_opp {
    uint32_t  avs_code;
    uint32_t  pmic_volt;
    uint32_t  cpu_freq_mhz;
} ca57_opp_t;

#define AVS_CODE(n)               ((n) ? (1 << (n - 1)) : 0)
#define OPP(_code, _volt, _freq)  { .avs_code = AVS_CODE(_code),  \
                                     .pmic_volt = (_volt),         \
                                     .cpu_freq_mhz = (_freq) }

static const ca57_opp_t rcarh3_ca57_opp[] = {
    /* Boost Mode 2.0GHz */
    OPP(0, 96,  2000),
    /* Boost Mode 1.7GHz */
    OPP(0, 96,  1700),
    OPP(1, 95,  1700),
    OPP(2, 93,  1700),
    OPP(3, 91,  1700),
    OPP(4, 89,  1700),
    OPP(5, 88,  1700),
    OPP(6, 87,  1700),
    OPP(7, 86,  1700),
    /* Boost Mode 1.6GHz */
    OPP(0, 90,  1600),
    OPP(1, 89,  1600),
    OPP(2, 88,  1600),
    OPP(3, 87,  1600),
    OPP(4, 86,  1600),
    OPP(5, 85,  1600),
    OPP(6, 84,  1600),
    OPP(7, 83,  1600),
    /* Normal Mode 1.5GHz */
    OPP(0, 83,  1500),
    OPP(1, 82,  1500),
    OPP(2, 81,  1500),
    OPP(3, 80,  1500),
    OPP(4, 79,  1500),
    OPP(5, 78,  1500),
    OPP(6, 77,  1500),
    OPP(7, 76,  1500),
    /* Normal Mode 1.0GHz */
    OPP(0, 83,  1000),
    OPP(1, 82,  1000),
    OPP(2, 81,  1000),
    OPP(3, 80,  1000),
    OPP(4, 79,  1000),
    OPP(5, 78,  1000),
    OPP(6, 77,  1000),
    OPP(7, 76,  1000),
    /* Normal Mode 0.5GHz */
    OPP(0, 83,   500),
    OPP(1, 82,   500),
    OPP(2, 81,   500),
    OPP(3, 80,   500),
    OPP(4, 79,   500),
    OPP(5, 78,   500),
    OPP(6, 77,   500),
    OPP(7, 76,   500)
};

static const ca57_opp_t rcarm3w_ca57_opp[] = {
    /* Boost Mode 2.0GHz */
    OPP(0, 96,  2000),
    /* Boost Mode 1.8GHz */
    OPP(0, 96,  1800),
    OPP(1, 95,  1800),
    OPP(2, 93,  1800),
    OPP(3, 91,  1800),
    OPP(4, 89,  1800),
    OPP(5, 88,  1800),
    OPP(6, 87,  1800),
    OPP(7, 86,  1800),
     /* Boost Mode 1.7GHz */
    OPP(0, 90,  1700),
    OPP(1, 89,  1700),
    OPP(2, 88,  1700),
    OPP(3, 87,  1700),
    OPP(4, 86,  1700),
    OPP(5, 85,  1700),
    OPP(6, 84,  1700),
    OPP(7, 83,  1700),
    /* Boost Mode 1.6GHz */
    OPP(0, 90,  1600),
    OPP(1, 89,  1600),
    OPP(2, 88,  1600),
    OPP(3, 87,  1600),
    OPP(4, 86,  1600),
    OPP(5, 85,  1600),
    OPP(6, 84,  1600),
    OPP(7, 83,  1600),
    /* Normal Mode 1.5GHz */
    OPP(0, 83,  1500),
    OPP(1, 82,  1500),
    OPP(2, 81,  1500),
    OPP(3, 80,  1500),
    OPP(4, 79,  1500),
    OPP(5, 78,  1500),
    OPP(6, 77,  1500),
    OPP(7, 76,  1500),
    /* Normal Mode 1.0GHz */
    OPP(0, 83,  1000),
    OPP(1, 82,  1000),
    OPP(2, 81,  1000),
    OPP(3, 80,  1000),
    OPP(4, 79,  1000),
    OPP(5, 78,  1000),
    OPP(6, 77,  1000),
    OPP(7, 76,  1000),
    /* Normal Mode 0.5GHz */
    OPP(0, 83,   500),
    OPP(1, 82,   500),
    OPP(2, 81,   500),
    OPP(3, 80,   500),
    OPP(4, 79,   500),
    OPP(5, 78,   500),
    OPP(6, 77,   500),
    OPP(7, 76,   500),
};

static const ca57_opp_t rcarm3n_ca57_opp[] = {
    /* Boost Mode 2.0GHz */
    OPP(0, 96,  2000),
    /* Boost Mode 1.8GHz */
    OPP(0, 96,  1800),
    /* Boost Mode 1.7GHz */
    OPP(0, 90,  1700),
    /* Boost Mode 1.6GHz */
    OPP(0, 90,  1600),
    /* Normal Mode 1.5GHz */
    OPP(0, 83,  1500),
    /* Normal Mode 1.0GHz */
    OPP(0, 83,  1000),
    /* Normal Mode 0.5GHz */
    OPP(0, 83,   500),
};

static int avs_i2c_write(const int fd, const uint8_t slave_addr,
                const uint8_t reg, const uint8_t val)
{
    int status;

#ifdef VARIANT_a
    (void) fd;
    status = iic_write_reg(slave_addr, reg, (uint8_t*) &val, 1);
#else
    struct {
        i2c_send_t hdr;
        unsigned char bytes[8];
    } omsg;

    if (cpg_dev.pmic.i2c_fd < 0) {
        char i2c_name[30];

        snprintf(i2c_name, sizeof(i2c_name), "/dev/i2c%d", cpg_dev.pmic.i2c_bus);
        cpg_dev.pmic.i2c_fd = open(i2c_name, O_RDWR);
        if (cpg_dev.pmic.i2c_fd < 0) {
            cpg_slogf(_SLOG_ERROR, "%s: open iic (%s) failed", __FUNCTION__, i2c_name);
            return CPG_RET_ERROR;
        }
    }

    omsg.hdr.slave.addr = slave_addr;
    omsg.hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    omsg.hdr.len = 2;
    omsg.hdr.stop = 1;
    omsg.bytes[0] = reg;
    omsg.bytes[1] = val;

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);
#endif

    if (status) {
        cpg_slogf(_SLOG_ERROR, "%s: Send failed, addr=%x, val=%x", __func__, reg, val);
    }

    return status ? CPG_RET_ERROR : CPG_RET_OK;
}

static const ca57_opp_t* avs_select_opp(const uint32_t cpufreq)
{
    const uint32_t   freq_mhz = cpufreq / 1000000;
    const ca57_opp_t *opp_table;
    uint32_t          opp_num;
    uint32_t          avs_code;

    switch(cpg_dev.soc_id) {
        case PRODUCT_ID_RCAR_H3:
            opp_table = rcarh3_ca57_opp;
            opp_num = ARRAY_SIZE(rcarh3_ca57_opp);
            break;
        case PRODUCT_ID_RCAR_M3W:
            opp_table = rcarm3w_ca57_opp;
            opp_num = ARRAY_SIZE(rcarm3w_ca57_opp);
            break;
        case PRODUCT_ID_RCAR_M3N:
            opp_table = rcarm3n_ca57_opp;
            opp_num = ARRAY_SIZE(rcarm3n_ca57_opp);
            break;
        default:
            return NULL; // No support
    }

    avs_code = in32(cpg_dev.avs.vbase + RCAR_GEN3_ADVADJP) & 0x1FF;

    if((cpg_dev.soc_id == PRODUCT_ID_RCAR_H3) || (cpg_dev.soc_id == PRODUCT_ID_RCAR_M3W)) {
        /*Check bit[7] and bit[8]*/
        if( (avs_code & 0x180) == 0x180) {
            avs_code = AVS_CODE(0);
        }
    }

    for (uint32_t i = 0; i < opp_num; i++) {
        if ((avs_code == opp_table[i].avs_code) && (opp_table[i].cpu_freq_mhz == freq_mhz)) {
            return &opp_table[i];
        }
    }

    return NULL;
}

int avs_cpu_freq_set(const uint32_t cpufreq)
{
    const ca57_opp_t* opp;
    int status;

    if ( (cpg_dev.avs.vbase == 0) || (cpg_dev.pmic.model == PMIC_MODEL_UNKNOWN) ||
         (cpg_dev.pmic.i2c_bus < 0) || (cpg_dev.pmic.i2c_slave_addr < 0) ) {
        cpg_slogf(_SLOG_ERROR, "%s: AVS/PMIC info are invalid", __func__);
        return CPG_RET_INVALPARA;
    }

    opp = avs_select_opp(cpufreq);
    if(!opp) {
        cpg_slogf(_SLOG_ERROR, "%s: Unsupported CPU frequency (%d MHz)", __func__,
                cpufreq / 1000000);
        return CPG_RET_INVALPARA;
    }

    if (cpg_dev.pmic.model == PMIC_MODEL_BOHM_BD9571MWF) {
        status = avs_i2c_write(cpg_dev.pmic.i2c_fd, (uint8_t)cpg_dev.pmic.i2c_slave_addr,
                                    BD9571_REG_SETVID, (uint8_t)opp->pmic_volt);
    } else {
        cpg_slogf(_SLOG_ERROR, "%s: Unknown PMIC model %d", __func__, cpg_dev.pmic.model);
        return CPG_RET_INVALPARA;
    }

    return status ? CPG_RET_ERROR : CPG_RET_OK;
}
