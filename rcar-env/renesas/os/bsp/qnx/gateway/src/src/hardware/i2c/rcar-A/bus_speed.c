/*
 * $QNXLicenseC: 
 * Copyright 2017, QNX Software Systems.
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

#define USING_EQUATION_3

static int find_best_clk(rcar_i2c_dev_t *dev, unsigned speed)
{
#ifdef USING_EQUATION_3
    /* As per HW Manual, Note 2 Equation 3: 
     * SCL Rate Computation (CDFD = 1, HLSE = 1, and SME = 1)
     * SCL freq = MOD_CLK/(8 + 2 × SMD + SCLD + SCHD + F[(tICF + tr + IntDelay) × MOD_CLK])
     */

    unsigned long scl = speed;
    unsigned      ticf, tr, intdelay = 0, round;
    unsigned      schd = 0, scld = 0, smd = 0;
    double        sum;
    int           found = 1;

    /* Check recommended bus speeds */
    if (speed == 100000) {
        if(dev->output_mode == OUTPUT_MODE_LVTTL) {
            smd  = 14;
            scld = 658;
            schd = 639;
        } else {
            smd  = 20;
            scld = 653;
            schd = 632;
        }
    } else if (speed == 400000) {
        if(dev->output_mode == OUTPUT_MODE_LVTTL) {
            smd  = 16;
            scld = 155;
            schd = 138;
        } else {
            smd  = 21;
            scld = 150;
            schd = 133;
        }
    } else {
        ticf = 20;
        tr   = 35;
        if(dev->output_mode == OUTPUT_MODE_LVTTL){
            intdelay = 6;
            smd    = 14;
        } else {
            intdelay = 110;
            smd    = 20;
        } 

        /* 
         * Equation 3: SCL rate calculation
         * see
         *	ICCCR2
         *
         * SCL = MOD_CLK/(8 + 2 × SMD + SCLD + SCHD + F[(tICF + tr + IntDelay) × MOD_CLK])
         * 
         * MOD_CLK : Module Clock Frequency
         * SMD     : SCL masked period 
         * SCLD    : I2C SCL low clock period 
         * SCHD    : I2C SCL high clock period 
         * tICF    : I2C SCL falling time 
         * tr      : I2C SCL rising time 
         * IntDelay: LSI internal delay 
         *
         * sum = F[(tICF + tr + IntDelay) × MOD_CLK]
         *
         */

        sum  = ticf + tr + intdelay;
        sum  = dev->pck * sum;
        sum /= 1000000000;
        round = (unsigned)sum;

        /* 
         * SCL = MOD_CLK/(8 + 2 × SMD + SCLD + SCHD + F[(tICF + tr + IntDelay) × MOD_CLK])
         *
         *
         * Calculation result (= SCL) should be less than
         * bus_speed for hardware safety
         *
         * We use scl = MOD_CLK/(8 + 2*smd + schd + scld + round)
         * and scld = schd + 1
         */
        found = 0;        
        for(schd = 0,scld = 1; scld < 0xFFFF; schd++,scld++){
            scl = dev->pck / (8 + (2 * smd) + schd + scld + round);
            if(scl <= speed){
                found = 1;
                break;
            }
        }
    }
    if (found) {
        dev->smd = smd;
        dev->scld= scld;
        dev->schd= schd;
    } else {
        scl = 0;
    }
#else
    /* For SCL rate calculation using Equation 2 */
    unsigned long scl = speed;
    unsigned      ticf, tr, intdelay = 0;
    unsigned      scgd, cdf, round, ick, cdf_width = 3;
    double        sum;    
    int           found = 1;

    /* Check recommended bus speeds */
    if (speed == 100000)  {
        cdf = 6;
        scgd = 21;
    } else if (speed == 400000) {
        cdf = 6;
        scgd = 3;
    } else {
        /*
         * Equation 2: SCL rate calculation
         * see
         *	ICCCR
         *
         * ick	= clkp / (1 + CDF)
         * SCL	= ick / (20 + SCGD * 8 + F[(ticf + tr + intd) * ick])
         *
         * ick  : I2C internal clock < 20 MHz
         * ticf : I2C SCL falling time
         * tr   : I2C SCL rising  time
         * intd : LSI internal delay
         * clkp : MOD_CLK: module clock frequency
         * F[]  : rounded down to an integer
         */
        ticf = 35;
        tr   = 200;
        intdelay = 50;
        cdf = dev->pck / 20000000;
        if (cdf >= 1U << cdf_width) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Input clock %d too high\n", dev->chan_idx, __FUNCTION__, dev->pck);
            return 0;
        }
        ick = dev->pck / (cdf + 1);

        /*
         *
         * F[(ticf + tr + intd) * ick] with sum = (ticf + tr + intd)
         *  = F[sum * ick / 1000000000]
         */
        sum  = ticf + tr + intdelay;
        sum  = (ick + 500000) * sum;
        sum /= 10000000;
        round = (unsigned)(sum + 500) / 1000;

        /*
         * SCL	= ick / (20 + SCGD * 8 + F[(ticf + tr + intd) * ick])
         *
         * Calculation result (= SCL) should be less than
         * bus_speed for hardware safety
         *
         * We use scl = ick / (20 + (scgd * 8) + round);
         */
        found = 0;
        for (scgd = 0; scgd < 0x3F; scgd++) {
            scl = ick / (20 + (scgd * 8) + round);
            if (scl <= speed) {
                found = 1;
                break;
            }
        }
    }
    if (found) {
        dev->icccr = scgd << 3 | cdf;
    } else {
        scl = 0;
    }
#endif
    if (scl == 0) {
        if (dev->verbose) {
            rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Can not find out best scl clock", dev->chan_idx, __FUNCTION__);
        }
    }

    return scl;
}

int rcar_i2c_set_bus_speed(void *hdl, unsigned int speed, unsigned int *ospeed)
{
    rcar_i2c_dev_t  *dev = hdl;
    unsigned scl;

    if (speed > I2C_FAST_BAUD) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: unsupported speed %dHz, low down to 400kHz", dev->chan_idx, __FUNCTION__, speed);
        speed = I2C_FAST_BAUD;
    }

    if (speed == dev->scl_freq) {
        if (ospeed)
            *ospeed = dev->scl_freq;
        return 0;
    }

    scl = find_best_clk(dev, speed);
    if (scl == 0) {
        errno = ERANGE;
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: unsupported speed %dHz", dev->chan_idx, __FUNCTION__, speed);
        return -1;
    }

    if (scl == dev->scl_freq) {
        if (ospeed) {
            *ospeed = dev->scl_freq;
        }
        return 0;
    }

    dev->scl_period = 1e9 / scl;
    dev->scl_freq = scl;
    dev->busSpeed = dev->scl_freq;

    if (ospeed) {
        *ospeed = scl;
    }

    return 0;
}


void rcar_i2c_clock_init(rcar_i2c_dev_t *dev)
{
#ifdef USING_EQUATION_3
    out32(dev->regbase + RCAR_I2C_ICCCR2, 0x7);    //CDFD = 1, HLSE = 1, and SME = 1
    out32(dev->regbase + RCAR_I2C_ICCCR, 0x6);     //Clock to filter glitches = 133.3/(1 + 6) = 19Mhz    
    out32(dev->regbase + RCAR_I2C_ICMPR, dev->smd);
    out32(dev->regbase + RCAR_I2C_ICHPR, dev->schd);
    out32(dev->regbase + RCAR_I2C_ICLPR, dev->scld);
#else
    /* For SCL rate calculation using Equation 2 */
    out32(dev->regbase + RCAR_I2C_ICCCR2, 0);
    out32(dev->regbase + RCAR_I2C_ICCCR, dev->icccr);
#endif
    /* 1st bit setup cycle */
    out32(dev->regbase + RCAR_I2C_ICFBSCR, 0x07);
}

