/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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
 * notices, as well as the QNX baseelopment Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <stdio.h>
#include <atomic.h>
#include "proto.h"

#define MILI_CELSIUS        1000

/* ECM releated to Gen4 TSC */
#define RCAR_ECM_ERRCTLR     (dev->product_id == PRODUCT_ID_RCAR_V3U ? RCAR_V3U_ECM_ERRCTLR13 : RCAR_S4_ECM_ERRCTLR(16))
#define RCAR_ECM_ERRSTSR     (dev->product_id == PRODUCT_ID_RCAR_V3U ? RCAR_V3U_ECM_ERRSTSR13 : RCAR_S4_ECM_ERRSTSR(16))
#define RCAR_ECM_ERRTGTR     (dev->product_id == PRODUCT_ID_RCAR_V3U ? RCAR_V3U_ECM_ERRTGTR13 : RCAR_S4_ECM_ERRTGTR(16))
#define RCAR_ECM_WPCNTR      (dev->product_id == PRODUCT_ID_RCAR_V3U ? RCAR_V3U_ECM_WPCNTR    : RCAR_S4_ECM_WPCNTR)

/* Gen3 THCODE & PTAT value ref Gen3 HWM 10A.4(6)
 * S4 THCODE & PTAT value ref Form Linux v5.10.41/rcar-5.1.6.rc2
 */ 
int THCODE_TABLE[TSC_MAX_NUM][3] = {
    /* TSC0 */ {    0,    0,   0 },
    /* TSC1 */ { 3397, 2800, 2221 }, // For H3, M3, V3H, S4
    /* TSC2 */ { 3393, 2795, 2216 }, // For H3, M3, V3H, S4
    /* TSC3 */ { 3389, 2805, 2237 }, // For H3, M3, V3H, S4
    /* TSC4 */ {    0,    0,   0 },
};

int PTAT_TABLE[TSC_MAX_NUM][3] = {
    /* TSC0 */ {    0,    0,   0 }, 
    /* TSC1 */ { 2631, 1509, 435 }, // For H3, M3, V3H, S4
    /* TSC2 */ { 2631, 1509, 435 }, // For H3, M3, V3H, S4
    /* TSC3 */ { 2631, 1509, 435 }, // For H3, M3, V3H, S4
    /* TSC4 */ {    0,    0,   0 }, 
};

/* For V3U:
 *  There is no official values yet.
 *  So values in the following tables are just examples from section 10C.3.3.3 
 */
int GEN4_THCODE_TABLE[3] = {
    /* TSC0,1,2,3,4 */ 3415, 2694, 2195 // An example for V3U
};

int GEN4_PTAT_TABLE[3] = {
    /* TSC0,1,2,3,4 */ 2650, 1288,  347 // An example for V3U
};

int GEN4_PTAT0_TABLE[3] = {
    /* TSC0,1,2,3,4 */ 2659,   0,   357 // An example for V3U
};

/* Table of TSC constant data */
rcar_tsc_t tsc_data_table[] = {
    {   /* TSC0 Data */
        .phy_addr    = RCAR_TSC0_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_TSC_SIZE,
        .thcode      = THCODE_TABLE[0],
        .ptat        = PTAT_TABLE[0],
        .ptat0       = PTAT_TABLE[0],
        .has_hw_code = 0,
        .ths_chan    = 0,
    },
    {   /* TSC1 Data */
        .phy_addr    = RCAR_TSC1_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_TSC_SIZE,
        .thcode      = THCODE_TABLE[1],
        .ptat        = PTAT_TABLE[1],
        .ptat0       = PTAT_TABLE[1],
        .has_hw_code = 1,
        .ths_chan    = 1,
    },    
    {   /* TSC2 Data */
        .phy_addr    = RCAR_TSC2_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_TSC_SIZE,
        .thcode      = THCODE_TABLE[2],
        .ptat        = PTAT_TABLE[2],
        .ptat0       = PTAT_TABLE[2],
        .has_hw_code = 0,
        .ths_chan    = 2,
    },
    {   /* TSC3 Data */
        .phy_addr    = RCAR_TSC3_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_TSC_SIZE,
        .thcode      = THCODE_TABLE[3],
        .ptat        = PTAT_TABLE[3],
        .ptat0       = PTAT_TABLE[3],
        .has_hw_code = 0,
        .ths_chan    = 3,
    },
    {   /* TSC4 Data */
        .phy_addr    = RCAR_TSC4_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_TSC_SIZE,
        .thcode      = THCODE_TABLE[4],
        .ptat        = PTAT_TABLE[4],
        .ptat0       = PTAT_TABLE[4],
        .has_hw_code = 0,
        .ths_chan    = 4,
    }    
};

inline int _round_temp(float i)
{
    int tmp, tmp1, tmp2;
    int result = 0;
    
    tmp = (int)(i * 10.0);
    
    tmp1 = abs(tmp) % 10;
    tmp2 = abs(tmp) / 10;
    if (tmp1 < 5)
        result = tmp2;
    else
        result = tmp2 + 1;

    return ((tmp < 0) ? (result*(-1)) : result);
}

/*
 **********************************************************************************
 * Outline      : Convert temp_code to temperature and return temperature
 * Function Name: rcar_tsc_convert_code_to_temp
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 *              : int temp_code
 * Return Value : temperature in celsius
 * Note         :
 ***********************************************************************************
 */
static float rcar_tsc_convert_code_to_temp(rcar_tsc_t *tsc, int temp_code)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    float temp;

    if (dev->product_id == PRODUCT_ID_RCAR_V3U) {
        temp_code &= TEMP_CODE_MASK;
        if (temp_code < tsc->thcode[1]) {
            temp  = (float)(167 * ((tsc->ptat[1] - tsc->ptat[2]) * (temp_code - tsc->thcode[2]) + (tsc->ptat[2] - tsc->ptat0[2]) * (tsc->thcode[1]- tsc->thcode[2])));
            temp /= (float)((tsc->ptat0[0] - tsc->ptat0[2]) * (tsc->thcode[1] - tsc->thcode[2]));
            temp -= (float)41;
        } 
        else {
            temp  = (float)(167 * ((tsc->ptat[0] - tsc->ptat[1]) * (temp_code - tsc->thcode[1]) + (tsc->ptat[1] - tsc->ptat0[2]) * (tsc->thcode[0] - tsc->thcode[1])));
            temp /= (float)((tsc->ptat0[0] - tsc->ptat0[2]) * (tsc->thcode[0] - tsc->thcode[1]));
            temp -= (float)41;
        }
        //celsius = _round_temp(temp);
    }
    //else if (dev->product_id == PRODUCT_ID_RCAR_S4) {
    //    float a, b;
    //    float t1 = 126.0, t2 = -41.0, tj = tsc->tj_t / MILI_CELSIUS;
    //    /* To calculate temperature is based on Linux BSP v5.10.41/rcar-5.1.6.rc2 for S4 */
    //    temp_code &= TEMP_CODE_MASK;
    //    if (temp_code <= tsc->thcode[1]) {
    //        a = (float)(tsc->thcode[1] - tsc->thcode[2]) / (float)(tj - t2);
    //        b = (float)(tsc->thcode[2]) - (a * t2);
    //    }
    //    else {
    //        a = (float)(tsc->thcode[1] - tsc->thcode[0]) / (float)(tj - t1);
    //        b = (float)(tsc->thcode[0]) - (a * t1);
    //    }
    //    temp  = ((float)temp_code - b) / (float)a;
    //}
    else if (dev->type == TYPE_A) {
        int t1 = dev->product_id == PRODUCT_ID_RCAR_M3W ? 116 : 126;
        int t2 = dev->product_id == PRODUCT_ID_RCAR_M3W ? 157 : 167;
        temp_code &= TEMP_CODE_MASK;
        /* To calculate temperature is based on hardware user manual, section 10A.3.1.2 for Gen3
         * Rev.0.80, section 13.3.3.4 for S4 */
        if (temp_code < tsc->thcode[1]) {
            temp  = (float)(t2 * (tsc->ptat[2] - tsc->ptat[1]) * (tsc->thcode[2] - temp_code));
            temp /= (float)((tsc->ptat[2] - tsc->ptat[0]) * (tsc->thcode[2] - tsc->thcode[1]));
            temp -= (float)41;
        }
        else {
            temp  = (float)(t2 * (tsc->ptat[0] - tsc->ptat[1]) * (tsc->thcode[0] - temp_code));
            temp /= (float)((tsc->ptat[0] - tsc->ptat[2]) * (tsc->thcode[1] - tsc->thcode[0]));
            temp += (float)t1;
        }
        //celsius = _round_temp(temp);
    }
    else {
        temp_code &= RCAR_TSC_THSSR_CTEMP_MASK;
        /*
         * As hardware description, there are 2 formulas to
         * calculate temperature on E3 when CTEMP[5:0] is less than
         * and greater or equal to 24.
         */
        if (temp_code < 24)
            temp = (float)((temp_code * 55) - 720.0) / 10.0;
        else
            temp = (float)(temp_code * 5) - 60.0;
    }

    return temp;
}

/*
 **********************************************************************************
 * Outline      : Convert temperature in celsius to temp_code to write to IRQTEMP1-3
 * Function Name: rcar_tsc_convert_temp_to_code
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 *              : int celsius
 * Return Value : temp_code
 * Note         :
 ***********************************************************************************
 */
static int rcar_tsc_convert_temp_to_code(rcar_tsc_t *tsc, int celsius)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    int ctemp = 0;
    float tmp;

    if (dev->product_id == PRODUCT_ID_RCAR_V3U) {
        if ((celsius * MILI_CELSIUS) <= tsc->tj_t) {
            tmp  = (float)((celsius + 41) * (tsc->ptat0[0] - tsc->ptat0[2]) * (tsc->thcode[1] - tsc->thcode[2]));
            tmp -= (float)((126 - (-41)) * (tsc->ptat[2] - tsc->ptat0[2]) * (tsc->thcode[1] - tsc->thcode[2]));
            tmp /= (float)((126 - (-41)) * (tsc->ptat[1] - tsc->ptat[2]));
            ctemp = tsc->thcode[2] + _round_temp(tmp);
        }
        else {
            tmp  = (float)((celsius + 41) * (tsc->ptat0[0] - tsc->ptat0[2]) * (tsc->thcode[0] - tsc->thcode[1]));
            tmp -= (float)((126 - (-41)) * (tsc->ptat[1] - tsc->ptat0[2]) * (tsc->thcode[0] - tsc->thcode[1]));
            tmp /= (float)((126 - (-41)) * (tsc->ptat[0] - tsc->ptat[1]));
            ctemp = tsc->thcode[1] + _round_temp(tmp);
        }
    }
    //else if (dev->product_id == PRODUCT_ID_RCAR_S4) {
    //    float a, b;
    //    float t1 = 126.0, t2 = -41.0, tj = tsc->tj_t / MILI_CELSIUS;
    //    /* To calculate IRQTEMP is based on Linux BSP v5.10.41/rcar-5.1.6.rc2 for S4 */
    //    if ((celsius * MILI_CELSIUS) <= tsc->tj_t) {
    //        a = (float)(tsc->thcode[1] - tsc->thcode[2]) / (float)(tj - t2);
    //        b = (float)(tsc->thcode[2]) - (a * t2);
    //    }
    //    else {
    //        a = (float)(tsc->thcode[1] - tsc->thcode[0]) / (float)(tj - t1);
    //        b = (float)(tsc->thcode[0]) - (a * t1);
    //    }
    //    ctemp = celsius * a + b;
    //}
    else if (dev->type == TYPE_A) {
        int t1 = dev->product_id == PRODUCT_ID_RCAR_M3W ? 116 : 126;
        int t2 = dev->product_id == PRODUCT_ID_RCAR_M3W ? 157 : 167;
        /* To calculate IRQTEMP is based on the hardware user manual v2.0, section 10A.3.1.1 for Gen3, 
         * Rev.0.80, section 13.3.3.6 for S4 */
        if ((celsius * MILI_CELSIUS) <= tsc->tj_t) {
            tmp  = (float)((celsius + 41) * (tsc->ptat[2] - tsc->ptat[0]) * (tsc->thcode[2] - tsc->thcode[1]));
            tmp /= (float)(t2 * (tsc->ptat[2] - tsc->ptat[1]));
            ctemp = tsc->thcode[2] - _round_temp(tmp);
        }
        else {
            tmp  = (float)((celsius - t1) * (tsc->ptat[0] - tsc->ptat[2]) * (tsc->thcode[1] - tsc->thcode[0]));
            tmp /= (float)(t2 * (tsc->ptat[0] - tsc->ptat[1]));
            ctemp = tsc->thcode[0] - _round_temp(tmp);
        }
    }
    else {
        /*
         * Similarly, to calculate register CTEMP[5:0] for E3
         * there are 2 formulas to measure CTEMP[5:0] which is
         * depending on temperature changes from less than
         * to greater or equal to 60 degrees celsius.
         */
        if (celsius < 60)
            ctemp = (celsius + 72) * 10  / 55;
        else
            ctemp = (celsius + 60) / 5;
    }

    return ctemp;
}


/*
 **********************************************************************************
 * Outline      : Read TEMP_CODE and return temperature
 * Function Name: rcar_thermal_get_curren_temp
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : temperature in celsius
 * Note         :
 ***********************************************************************************
 */
static float rcar_tsc_get_current_temp(rcar_tsc_t *tsc)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    float celsius = 0, temp_code;

    if (dev->type != TYPE_B) {
        temp_code = in32(tsc->vaddr + RCAR_TSC_TEMP);
        celsius = rcar_tsc_convert_code_to_temp(tsc, temp_code);
    } else {
        int i;
        uint32_t old, new, reg;

        reg = 0;
        old = ~0;
        for (i = 0; i < 128; i++) {
            /*
             * As hardware description, it needs to wait 300us after
             * changing comparator offset to get stable temperature.
             */
            nanospin_ns(350 * 1000);
            new = in32(tsc->vaddr + RCAR_TSC_B_THSSR) & RCAR_TSC_THSSR_CTEMP_MASK;

            if (new == old) {
                reg = new;
                break;
            }
            old = new;
        }
        /*
         * As hardware description, there are 2 formulas to
         * calculate temperature on E3 when CTEMP[5:0] is less than
         * and greater or equal to 24.
         */
        if (reg < 24)
             celsius = (float)((reg * 55) - 720.0) / 10.0;
        else
            celsius = (float)(reg * 5) - 60.0;
    }

    return celsius;
}

/*
 **************************************************************************************************
 * Outline      : Set threshold temperature to raise interrupts
 * Function Name: rcar_thermal_set_irq_temp
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : -1
 *                 EOK
 * Note         : Assuming TEMP2 interrupt when temperature over than current temperature 2 Celsius
 *                and TEMPD1 interrupt when temperature lower than current tempperature 2 Celsius
 ****************************************************************************************************
 */
static int rcar_tsc_set_irq_temp(rcar_tsc_t *tsc)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    int temp;
    int ctemp_low = 0, ctemp_high = 0;

    if (dev->upper_thrd == 0 || dev->lower_thrd == 0) {
        temp = _round_temp(rcar_tsc_get_current_temp(tsc));
    }

    if (dev->lower_thrd == 0) dev->lower_thrd = temp - 2;
    if (dev->upper_thrd == 0) dev->upper_thrd = temp + 2;

    ctemp_low = rcar_tsc_convert_temp_to_code(tsc, dev->lower_thrd);
    ctemp_high = rcar_tsc_convert_temp_to_code(tsc, dev->upper_thrd);

    if (dev->type == TYPE_B) {
        uint32_t reg  = in32(tsc->vaddr + RCAR_TSC_B_INTCTLR);
        reg &= ~(INTCTLR_CTEMP1_MASK | INTCTLR_CTEMP0_MASK);
        reg |= INTCTLR_CTEMP1_VAL(ctemp_high) | INTCTLR_CTEMP0_VAL(ctemp_low);

        out32(tsc->vaddr + RCAR_TSC_B_INTCTLR, reg);

        /* Enable Interrupts */    
        out32(tsc->vaddr + RCAR_TSC_B_ENR, RCAR_TSC_B_Tj00INT | RCAR_TSC_B_Tj01INT);
    }
    else {
        out32(tsc->vaddr + RCAR_TSC_IRQTEMP1, ctemp_low);
        out32(tsc->vaddr + RCAR_TSC_IRQTEMP2, ctemp_high);

        /* Enable TEMPD1 and TEMP2 interrupts */
        out32(tsc->vaddr + RCAR_TSC_IRQEN, RCAR_TSC_IRQ_TEMPD1_EN | RCAR_TSC_IRQ_TEMP2_EN);
    }

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Fucntion use to initialize H3 1.1 A-type TSC
 * Function Name: rcar_tsc_init_a_h3_1_1
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value :  ENOMEM
 *                 EOK
 * Note         : Initialize A-type TSC workaround
 *                There is no document to mention about this difference. 
 *                Contact Renesas to get further information
 ***********************************************************************************
 */
static int rcar_tsc_init_a_h3_1_1(rcar_tsc_t *tsc)
{
    out32(tsc->vaddr + RCAR_TSC_THCTR,  1 << 5);
    out32(tsc->vaddr + RCAR_TSC_THCTR,  0x0);

    delay(1);

    out32(tsc->vaddr + RCAR_TSC_THCTR,  1 << 8);

    out32(tsc->vaddr + RCAR_TSC_IRQCTL, 0x3F);
    out32(tsc->vaddr + RCAR_TSC_IRQMSK, 0);
    out32(tsc->vaddr + RCAR_TSC_IRQEN, RCAR_TSC_IRQ_TEMPD1_EN | RCAR_TSC_IRQ_TEMP2_EN);

    out32(tsc->vaddr + RCAR_TSC_THCTR, (1 << 8) | (1 << 7) | (1 << 5) | (1 << 4));

    delay(1);

    out32(tsc->vaddr + RCAR_TSC_THCTR, (1 << 8) | (1 << 7) | (1 << 5) | (1 << 4) | (1 << 1) | (1 << 0));

    delay(1);

    if (tsc->has_hw_code && (COR_PARA_VLD == (in32(tsc->vaddr + RCAR_TSC_THSCP) & COR_PARA_VLD))) {
        tsc->thcode[0] = in32(tsc->vaddr + RCAR_TSC_THCODE1(((rcar_tsc_dev_t *)(tsc->dev))->product_id)) & RCAR_TSC_THCODE_MASK;
        tsc->thcode[1] = in32(tsc->vaddr + RCAR_TSC_THCODE2(((rcar_tsc_dev_t *)(tsc->dev))->product_id)) & RCAR_TSC_THCODE_MASK;
        tsc->thcode[2] = in32(tsc->vaddr + RCAR_TSC_THCODE3(((rcar_tsc_dev_t *)(tsc->dev))->product_id)) & RCAR_TSC_THCODE_MASK;
    }

    rcar_tsc_set_irq_temp(tsc);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Fucntion use to initialize A-type TSC
 * Function Name: rcar_tsc_init_a
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value :  ENOMEM
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_tsc_init_a(rcar_tsc_t *tsc)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    uint32_t reg_val = 0;
    float tmp;
    int timeout;

    if ((dev->product_id == PRODUCT_ID_RCAR_H3) &&
        ((dev->product_rev == PRODUCT_H3_REV_1_0) || (dev->product_rev == PRODUCT_H3_REV_1_1))) {
            return rcar_tsc_init_a_h3_1_1(tsc);
    }

    if (dev->product_id == PRODUCT_ID_RCAR_V3U) {
        timeout = 100; //Standby mode checking timeout (T.B.D)
        while ((--timeout > 0) && ((in32(tsc->vaddr + RCAR_TSC_THINITSTR) & RCAR_TSC_THINITSTR_THTMP_MASK) != RCAR_TSC_THINITSTR_THTMP_MASK)) {
            /* Can read THCODE_INT1, THCODE_INT2, THCODE_INT3 here */
            nanospin_ns(1000); 
        }
        timeout = 100; //Connection test mode checking timeout (T.B.D)
        while ((--timeout > 0) && (in32(tsc->vaddr + RCAR_TSC_CVM_CTRL_BK) & RCAR_TSC_CVM_CTRL_BK_THEN_BK)) {
            nanospin_ns(1000); 
        }    
    }

    /* Normal mode, both THS & CIVM are ON */
    out32(tsc->vaddr + RCAR_TSC_THCTR, 0);

    /* Initial Sequence Stop Setting */
    if (dev->product_id == PRODUCT_ID_RCAR_V3U) {
        timeout = 100; //Initial Sequence Stop Setting timeout
        while ((--timeout > 0) && !((in32(tsc->vaddr + RCAR_TSC_SEQ_ACT_MON) & RCAR_TSC_SEQ_ACT_MON_ACT_MASK))) {
            nanospin_ns(1000); 
        }    
    }

    /* Wait for initialization */
    delay(1);

    /* Read THCODE */
    if (tsc->has_hw_code) {
        tsc->thcode[0] = in32(tsc->vaddr + RCAR_TSC_THCODE1(dev->product_id)) & RCAR_TSC_THCODE_MASK;
        tsc->thcode[1] = in32(tsc->vaddr + RCAR_TSC_THCODE2(dev->product_id)) & RCAR_TSC_THCODE_MASK;
        tsc->thcode[2] = in32(tsc->vaddr + RCAR_TSC_THCODE3(dev->product_id)) & RCAR_TSC_THCODE_MASK;
    }

    /* Calculate Tj_T */
    if (dev->product_id == PRODUCT_ID_RCAR_V3U) {
        tmp  = (float)((tsc->ptat[1] - tsc->ptat0[2]) * 167);
        tmp /= (float)(tsc->ptat0[0] - tsc->ptat0[2]);
    }
    //else if(dev->product_id == PRODUCT_ID_RCAR_S4) {
    //    /* Calculate Tj_T following Linux BSP v5.10.41/rcar-5.1.6.rc2 for S4 */
    //    tmp  = (float)((tsc->ptat[1] - tsc->ptat[2]) * (126 - (-41)));
    //    tmp /= (float) (tsc->ptat[0] - tsc->ptat[2]);            
    //}
    else {
        /* Calculate Tj_T is based on the hardware user manual v2.0, section 10A.3.1.1 for Gen3, 
         * Rev.0.80, section 13.3.3.6 for S4 */
        tmp  = (float)((tsc->ptat[1] - tsc->ptat[2]) * (dev->product_id == PRODUCT_ID_RCAR_M3W ? 157 : 167));
        tmp /= (float)(tsc->ptat[0] - tsc->ptat[2]);            
    }
    //tsc->tj_t = _round_temp(tmp - 41.0);
    tsc->tj_t = (tmp - 41.0) * MILI_CELSIUS;

    /* Clear interrupt status */
    out32(tsc->vaddr + RCAR_TSC_IRQSTR, 0);

    /* Clear TEMPD1 and TEMP2 interrupt masks */
    out32(tsc->vaddr + RCAR_TSC_IRQMSK, RCAR_TSC_IRQ_TEMPD1_EN | RCAR_TSC_IRQ_TEMP2_EN);

    if (dev->ecm_vaddr != (uintptr_t)MAP_FAILED) {
        /* Setup ECM for receiving error from TSC1~4 and then interrupt to CPU */
        out32(dev->ecm_vaddr + RCAR_ECM_WPCNTR, (0xACCE << 16 ) | 0x1); /* Disable write protection */
        out32(dev->ecm_vaddr + RCAR_ECM_ERRCTLR, in32(dev->ecm_vaddr + RCAR_ECM_ERRCTLR) | (0xFFF << 14)); /* Enable error detection */
        out32(dev->ecm_vaddr + RCAR_ECM_ERRTGTR, in32(dev->ecm_vaddr + RCAR_ECM_ERRTGTR) | (0xFFF << 14)); /* Route error detection to INTC */
    }

    /* Set of higher and lower temperature thresholds to cause over/under interuppts */
    rcar_tsc_set_irq_temp(tsc);

    /* Enable the A/D converter */
    reg_val  = in32(tsc->vaddr + RCAR_TSC_THCTR);
    reg_val |= RCAR_TSC_THCTR_THSST;
    out32(tsc->vaddr + RCAR_TSC_THCTR, reg_val);

    delay(2);

    /* By reading out current TEMP_CODE to calculate curent temperature 
     * and then set thresholds by +/- 2 degrees to cause over/under interuppts.
     * If not to start TSC2, TEMP_CODE is always zero 
     * So we put set thresholds interrupt after starting TSC2 
     */ 
    //rcar_tsc_set_irq_temp(tsc);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Fucntion use to initialize B-type TSC
 * Function Name: rcar_tsc_init_b
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value :  ENOMEM
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_tsc_init_b(rcar_tsc_t *tsc)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    uint32_t reg_val = 0;

    if (dev->product_id == PRODUCT_ID_RCAR_V3M)
        nanospin_ns(3500 * 1000);

    /* Using 2 interrupts: Tj00 falling, Tj01 rising */
    out32(tsc->vaddr + RCAR_TSC_B_POSNEG, 0x1);

    /* Setting control register */
    reg_val = in32(tsc->vaddr + RCAR_TSC_B_THSCR);
    reg_val = RCAR_TSC_B_THSCR_CPCTL;
    out32(tsc->vaddr + RCAR_TSC_B_THSCR, reg_val);

    /* Wait for initialization */
    nanospin_ns(350 * 1000);

    /* Clear interrupt status */
    out32(tsc->vaddr + RCAR_TSC_B_STR, 0);

    /* Mask interrupt */
    out32(tsc->vaddr + RCAR_TSC_B_INT_MASK, 0x4 | 
        (~0x7 & in32(tsc->vaddr + RCAR_TSC_B_INT_MASK)));

    rcar_tsc_set_irq_temp(tsc);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : A-type TSC interrupt handler
 * Function Name: rcar_tsc_intr_a
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : -1
 *                 EOK
 * Note         :
 ***********************************************************************************
 */

static int rcar_tsc_intr_a(rcar_tsc_t * tsc)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    float temp;
    uint32_t irq_status;

    if (dev->ecm_vaddr != (uintptr_t)MAP_FAILED) {
        /* Clear ECM error status */
        out32(dev->ecm_vaddr + RCAR_ECM_ERRSTSR, (0xFFF << 14) & in32(dev->ecm_vaddr + RCAR_ECM_ERRSTSR)); 
    }

    /* Get interrupt status value */
    irq_status = in32(tsc->vaddr + RCAR_TSC_IRQSTR);

    if(irq_status == 0)
        return EINVAL; // No interrupt

    /* Clear interrupt status */
    out32(tsc->vaddr + RCAR_TSC_IRQSTR, 0);

    temp = rcar_tsc_convert_code_to_temp(tsc, in32(tsc->vaddr + RCAR_TSC_TEMP));

    if (irq_status & RCAR_TSC_IRQ_TEMP1_EN) {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : Current temp(%0.3f) >= threshold(%0.3f)", tsc->ths_chan,
                temp, rcar_tsc_convert_code_to_temp(tsc, in32(tsc->vaddr + RCAR_TSC_IRQTEMP1)));
    } 

    if (irq_status & RCAR_TSC_IRQ_TEMP2_EN) {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : Current temp(%0.3f) >= threshold(%0.3f)", tsc->ths_chan,
                temp, rcar_tsc_convert_code_to_temp(tsc, in32(tsc->vaddr + RCAR_TSC_IRQTEMP2)));
    }

    if (irq_status & RCAR_TSC_IRQ_TEMP3_EN) {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : Current temp(%0.3f) >= threshold(%0.3f)", tsc->ths_chan,
                temp, rcar_tsc_convert_code_to_temp(tsc, in32(tsc->vaddr + RCAR_TSC_IRQTEMP3)));
    }

    if(irq_status & RCAR_TSC_IRQ_TEMPD1_EN) {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : Current temp(%0.3f) <= threshold(%0.3f)", tsc->ths_chan, 
                temp, rcar_tsc_convert_code_to_temp(tsc, in32(tsc->vaddr + RCAR_TSC_IRQTEMP1)));
    }

    if(irq_status & RCAR_TSC_IRQ_TEMPD2_EN) {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : Current temp(%0.3f) <= threshold(%0.3f)", tsc->ths_chan,
                temp, rcar_tsc_convert_code_to_temp(tsc, in32(tsc->vaddr + RCAR_TSC_IRQTEMP2)));
    }

    if(irq_status & RCAR_TSC_IRQ_TEMPD3_EN) {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : Current temp(%0.3f) <= threshold(%0.3f)", tsc->ths_chan,
                temp, rcar_tsc_convert_code_to_temp(tsc, in32(tsc->vaddr + RCAR_TSC_IRQTEMP3)));
    }

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : A-/C-type TSC interrupt handler
 * Function Name: rcar_tsc_intr_b
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : -1
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_tsc_intr_b(rcar_tsc_t *tsc)
{
    uint32_t irq_status;

    /* Get interrupt status value */
    irq_status = in32(tsc->vaddr + RCAR_TSC_B_STR);

    /* Clear interrupt status value */
    out32(tsc->vaddr + RCAR_TSC_B_STR, 0);

    if (irq_status & RCAR_TSC_B_Tj00INT) {
        thermal_slogf(VERBOSE_LEVEL1, "Interrupt TJ00 occurs.");
    }

    if (irq_status & RCAR_TSC_B_Tj01INT) {
        thermal_slogf(VERBOSE_LEVEL1, "Interrupt TJ01 occurs.");
    }

    if (irq_status & RCAR_TSC_B_Tj02INT) {
        thermal_slogf(VERBOSE_LEVEL1, "Interrupt TJ02 occurs.");
    }

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Get operate voltage of A-type TSC
 * Function Name: rcar_tsc_get_mvolt_a
 * Description  :
 * Arguments    : rcar_tsc_t  *tsc
 * Return Value : mili-voltage value
 * Note         : We are using formula from Manual v2.00 for product H3, M3W, M3N, V3H
 *                and Rev.0.70 for S4
 ***********************************************************************************
 */
static int rcar_tsc_get_mvolt_a(rcar_tsc_t *tsc, int *mvolt)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    float vdd = 0.0;
    int volt_code = in32(tsc->vaddr + RCAR_TSC_VOLT) & VOLT_CODE_MASK;

    if(PRODUCT_ID_RCAR_S4 == dev->product_id) {
        /* Base on S4 HWM Rev.0.70, section 13.3.3.6 */
        vdd  = 1.32 * volt_code - 118.0;
    }
    else {
        /* Voltage calculation based on hardware user manual v1.5 section 10A.3.1.2 for Gen3 */
        int temp = _round_temp(rcar_tsc_get_current_temp(tsc));
        if(PRODUCT_ID_RCAR_M3W == dev->product_id) {
            vdd  = 360 * (float)(tsc->thcode[2] - 4 * volt_code);
            vdd /= (float)(tsc->thcode[2] - tsc->thcode[0]);
            vdd += 630.0 + (float)temp * 0.25;
        }
        else {
            vdd  = 390.0 * (float)(tsc->thcode[2] - 4 * volt_code);
            vdd /= (float)(tsc->thcode[2] - tsc->thcode[0]);
            vdd += 630.0 + (float)temp * 0.1;
        }
    }
    *mvolt = _round_temp(vdd);

    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d voltage(mV) = %d",__FUNCTION__, tsc->ths_chan + 1, *mvolt);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Get operate voltage of B-type TSC
 * Function Name: rcar_tsc_get_mvolt_b
 * Description  :
 * Arguments    : rcar_tsc_t  *tsc
 *              : int * mvolt 
 * Return Value : EOK 
 *              : EINVAL
 * Note         :
 ***********************************************************************************
 */
static int rcar_tsc_get_mvolt_b(rcar_tsc_t *tsc, int *mvolt)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    uint32_t volt_code;
    int i, avolt = 0;

    if (dev->product_id != PRODUCT_ID_RCAR_V3M)
        return EOPNOTSUPP;

    /* Read VOLT_CODE[5:0] */
    volt_code = in32(tsc->vaddr + RCAR_TSC_B_CIVM_SR) & RCAR_TSC_VOLT_CODE_B_MASK;

    /* The hardware user manual v2.0 section 10B.3.5 is saying:
     * When acquire a chip internal voltage, read VOLT_CODE[5:0] of CIVM_SR register 5 times and use an average value of them
     */
    for (i = 0; i < 5; i++) {
        nanospin_ns(1600 * 1000);
        avolt += VOLT_CONVERT(volt_code);
    }

    /* Voltage calculation based on hardware user manual v2.0 section 10B.2.8 */
    *mvolt = avolt + 855 ;

    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d voltage(mV) = %d", __FUNCTION__, tsc->ths_chan + 1, *mvolt);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Get operate voltage of V3U TSC
 * Function Name: rcar_v3u_tsc_get_mvolt
 * Description  :
 * Arguments    : rcar_tsc_t  *tsc
 * Return Value : mili-voltage value
 * Note         : We are using formula from Manual Rev0.5 for product V3U
 ***********************************************************************************
 */
static int rcar_v3u_tsc_get_mvolt(rcar_tsc_t *tsc, int *mvolt)
{
    float vdd = 0.0;
    int volt_code, temp_code;
    int vmcode3_l, vmcode3_h, vmcode2_l, vmcode2_h, vmcode1_l, vmcode1_h;
    int tmp;

    volt_code = in32(tsc->vaddr + RCAR_TSC_VOLT) & VOLT_CODE_MASK;
    temp_code = in32(tsc->vaddr + RCAR_TSC_TEMP) & TEMP_CODE_MASK;

    if(temp_code < tsc->thcode[1]) {
        tmp = in32(tsc->vaddr + RCAR_TSC_VMCODE3);
        vmcode3_l = tmp & RCAR_TSC_VMCODE_MASK;
        vmcode3_h = (tmp >> 16) & RCAR_TSC_VMCODE_MASK;

        tmp = in32(tsc->vaddr + RCAR_TSC_VMCODE2);
        vmcode2_l = tmp & RCAR_TSC_VMCODE_MASK;
        vmcode2_h = (tmp >> 16) & RCAR_TSC_VMCODE_MASK;

        vdd  = (temp_code - tsc->thcode[2]) * (700 + (volt_code - vmcode2_l) * (200 / (vmcode2_h - vmcode2_l)));
        vdd += (tsc->thcode[1] - temp_code) * (700 + (volt_code - vmcode3_l) * (200 / (vmcode3_h - vmcode3_l)));
        vdd /= (tsc->thcode[1] - tsc->thcode[2]);
    }
    else {
        tmp = in32(tsc->vaddr + RCAR_TSC_VMCODE2);
        vmcode2_l = tmp & RCAR_TSC_VMCODE_MASK;
        vmcode2_h = (tmp >> 16) & RCAR_TSC_VMCODE_MASK;

        tmp = in32(tsc->vaddr + RCAR_TSC_VMCODE1);
        vmcode1_l = tmp & RCAR_TSC_VMCODE_MASK;
        vmcode1_h = (tmp >> 16) & RCAR_TSC_VMCODE_MASK;

        vdd  = (temp_code - tsc->thcode[1]) * (700 + (volt_code - vmcode1_l) * (200 / (vmcode1_h - vmcode1_l)));
        vdd += (tsc->thcode[0] - temp_code) * (700 + (volt_code - vmcode2_l) * (200 / (vmcode2_h - vmcode2_l)));
        vdd /= (tsc->thcode[0] - tsc->thcode[1]);
    }

    *mvolt = _round_temp(vdd);

    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d voltage(mV) = %d",__FUNCTION__, tsc->ths_chan + 1, *mvolt);

    return EOK;
}

/*
 * Thermal Init
 */
static int rcar_tsc_init(rcar_tsc_t * tsc)
{    
    rcar_tsc_dev_t *dev ;

    if(!tsc)
        return -1;

    dev = (rcar_tsc_dev_t *)tsc->dev;

     if(TYPE_A == dev->type)
        return rcar_tsc_init_a(tsc);
    else
        return rcar_tsc_init_b(tsc);
}

/*
 * Thermal Deinit
 */
static void rcar_tsc_deinit(rcar_tsc_t * tsc)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)tsc->dev;
    if(!tsc)
        return;

    if ((uintptr_t)MAP_FAILED != tsc->vaddr) {
        /* Disable interrupts */
        if(TYPE_A == dev->type) {
            out32(tsc->vaddr + RCAR_TSC_THCTR, 0);
            delay(1);
            out32(tsc->vaddr + RCAR_TSC_IRQEN, 0);
            out32(tsc->vaddr + RCAR_TSC_IRQMSK, 0);
            out32(tsc->vaddr + RCAR_TSC_THCTR, 0);
            out32(tsc->vaddr + RCAR_TSC_IRQTEMP1, 0);
            out32(tsc->vaddr + RCAR_TSC_IRQTEMP2, 0);
            out32(tsc->vaddr + RCAR_TSC_IRQTEMP3, 0);
            delay(1);
        }
        else
            out32(tsc->vaddr + RCAR_TSC_B_ENR, 0);

        munmap_device_io(tsc->vaddr, tsc->reg_size);
    }

    tsc->vaddr = (uintptr_t)MAP_FAILED;
}

/*
 * Process Thermal Interrupt
 */
static int rcar_tsc_intr(rcar_tsc_t * tsc)
{
    rcar_tsc_dev_t *dev;

    if(!tsc)
        return -1;

    dev = (rcar_tsc_dev_t *)tsc->dev;

    if(TYPE_A == dev->type)
        return rcar_tsc_intr_a(tsc);
    else
        return rcar_tsc_intr_b(tsc);
}

static int intr_pulse_handler (message_context_t * ctp, int code, unsigned flags, void *handle)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)handle;
    int i, j, found = 0;

    for(i = 0; i < dev->intr_num; i++) {
        if(code == dev->intr_pulse_code[i]) {
            for(j = dev->chan_min; j <= dev->chan_max ; j++) {
                rcar_tsc_intr(dev->tsc[j]);
            }
            found = 1;            
            InterruptUnmask(dev->intr_irq[i], dev->intr_id[i]);
            break;
        }
    }

    if (!found) {
        thermal_slogf(VERBOSE_LEVEL1, "%s: Received an unknown pulse: %d",
                      __FUNCTION__, code);
    }

    return 0;
}

/*
 * Get voltage in Celsius
 */
int rcar_tsc_get_temp(rcar_tsc_t * tsc, float *temp)
{
    if(!tsc)
        return -1;

    *temp = rcar_tsc_get_current_temp(tsc);

    tsc->cur_temp = *temp;
    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d temperature = %0.3f", __FUNCTION__, tsc->ths_chan, *temp);

    return EOK;
}

/*
 * Get voltage in mili-vol
 */
int rcar_tsc_get_mvolt(rcar_tsc_t * tsc, int *mvolt)
{
    rcar_tsc_dev_t *dev;

    if(!tsc)
        return -1;

    dev = (rcar_tsc_dev_t *)tsc->dev;

    if (dev->product_id == PRODUCT_ID_RCAR_V3U)
        return(rcar_v3u_tsc_get_mvolt(tsc, mvolt));
    else if(TYPE_A == dev->type)
        return(rcar_tsc_get_mvolt_a(tsc, mvolt));
    else
        return(rcar_tsc_get_mvolt_b(tsc, mvolt));
}

void rcar_thermal_deinit(rcar_tsc_dev_t *dev)
{
    int i;

    for(i = 0; i < dev->intr_num; i++) {
        if (dev->intr_id[i] != -1) 
            InterruptDetach(dev->intr_id[i]);
    }

    if ((uintptr_t)MAP_FAILED != dev->ecm_vaddr) {
        munmap_device_io(dev->ecm_vaddr, dev->ecm_size);
    }

    for(i = dev->chan_min; i <= dev->chan_max; i++) {
        rcar_tsc_deinit(dev->tsc[i]);
        dev->tsc[i] = NULL;
    }
}

/*
 * Parse data for THS based on product ID and init thermal
 */
int rcar_thermal_init(rcar_tsc_dev_t *dev)
{
    unsigned   hwi_off, tag_idx;
    hwi_tag    *tag;
    rcar_tsc_t *tsc;
    uintptr_t  ptat_vbase;
    int        ptat[3] = { 0, 0, 0 };
    int        ptat0[3] = { 0, 0, 0 };
    int        has_hw_code = 0;
    int        i;

    hwi_off = hwi_find_device("r-car", 0);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, NULL);
        if(tag) {
            dev->product_id = tag->hwversion.version;
            dev->product_rev = tag->hwversion.hclass;
        } else {
            thermal_slogf(VERBOSE_QUIET, "%s: Unable to query product_id from HW info", __FUNCTION__);
            return EINVAL;
        }
    } else {
        thermal_slogf(VERBOSE_QUIET, "%s: Unable to find product version name from HW info\n", __FUNCTION__);
        return EINVAL;
    }

    hwi_off = hwi_find_device(RCAR_HWI_THERMAL_OPS, 0);
    tag_idx = 0;
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            dev->ecm_pbase = tag->location.base;
            dev->ecm_size = tag->location.len;
 
            while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                if(strcmp(RCAR_HWI_THERMAL_CHAN_MIN,  __hwi_find_string(tag->regname.regname)) == 0) {
                    dev->chan_min = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_THERMAL_CHAN_MAX,  __hwi_find_string(tag->regname.regname)) == 0) {
                    dev->chan_max = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_THERMAL_TYPE,  __hwi_find_string(tag->regname.regname)) == 0) {
                    dev->type = tag->regname.offset;
                } else if(strcmp(RCAR_HWI_THERMAL_IRQ_NUM,  __hwi_find_string(tag->regname.regname)) == 0) {
                    dev->intr_num = tag->regname.offset;
                }
            }
            if (dev->intr_num > TSC_INTR_NUM) dev->intr_num = TSC_INTR_NUM;
            for (i = 0 ; i < dev->intr_num; i++) {
                tag_idx = i;
                dev->intr_irq[i] = hwitag_find_ivec(hwi_off, &tag_idx);
            }
        } else {
            thermal_slogf(VERBOSE_QUIET, "%s: Unable to find RCAR_HWI_THERMAL_OPS from HW info\n", __FUNCTION__);
            return EINVAL;
        }
    } else {
        thermal_slogf(VERBOSE_QUIET, "%s: Unable to find RCAR_HWI_THERMAL_OPS from HW info\n", __FUNCTION__);
        return EINVAL;
    }

    dev->ecm_vaddr = (uintptr_t)MAP_FAILED;
    if ( dev->ecm_pbase ) {
        /* Mapping memory for ECM register */
        if ((uintptr_t)MAP_FAILED == (dev->ecm_vaddr = (uintptr_t)mmap_device_io(dev->ecm_size, dev->ecm_pbase))) {
            thermal_slogf(VERBOSE_QUIET, "%s: mmap_device_io(RCAR_V3U_ECM_BASE) failed", __FUNCTION__);
            return ENOMEM;
        }
    }

    /* Interrupt attachment */
    for (i = 0; i < dev->intr_num; i++) {
        dev->intr_id[i] = -1;

        dev->intr_pulse_code[i] = pulse_attach(dev->dpp, MSG_FLAG_ALLOC_PULSE, 0, intr_pulse_handler, dev);

        SIGEV_PULSE_INIT(&dev->event[i], dev->coid, 21, dev->intr_pulse_code[i], NULL);

        if ((dev->intr_id[i] = InterruptAttachEvent(dev->intr_irq[i], &dev->event[i],
                                            _NTO_INTR_FLAGS_TRK_MSK|_NTO_INTR_FLAGS_END)) == -1) {
            thermal_slogf(VERBOSE_QUIET, "%s: Interrupt attach failed:%s \n", __FUNCTION__, strerror(errno));
            goto failed;
        }
    }

    if ((ptat_vbase = (uintptr_t)mmap_device_io(PTAT_SIZE, THERMAL_PTAT_BASE)) != (uintptr_t)MAP_FAILED) {
        has_hw_code = (COR_PARA_VLD == (in32(ptat_vbase + RCAR_TSC_THSCP) & COR_PARA_VLD));
        /* Read PTAT */
        if (has_hw_code) {
            ptat[0] = in32(ptat_vbase + RCAR_TSC_PTAT1(dev->product_id)) & RCAR_TSC_PTAT_MASK;
            ptat[1] = in32(ptat_vbase + RCAR_TSC_PTAT2(dev->product_id)) & RCAR_TSC_PTAT_MASK;
            ptat[2] = in32(ptat_vbase + RCAR_TSC_PTAT3(dev->product_id)) & RCAR_TSC_PTAT_MASK;

            if ((dev->product_id == PRODUCT_ID_RCAR_V3U)) {
                ptat0[0] = (in32(ptat_vbase + RCAR_TSC_PTAT1(dev->product_id)) >> 16) & RCAR_TSC_PTAT_MASK;
                ptat0[2] = (in32(ptat_vbase + RCAR_TSC_PTAT3(dev->product_id)) >> 16) & RCAR_TSC_PTAT_MASK;
            }
        }
        munmap_device_io(ptat_vbase, PTAT_SIZE);
    }

    /* Initialize and start TSC */
    for (i = dev->chan_min; i <= dev->chan_max ; i++) {
        tsc = &tsc_data_table[i];
        if (dev->product_id == PRODUCT_ID_RCAR_V3U) {
            tsc->thcode = &GEN4_THCODE_TABLE[0];
            tsc->ptat   = &GEN4_PTAT_TABLE[0];
            tsc->ptat0  = &GEN4_PTAT0_TABLE[0];
        }
        tsc->has_hw_code = has_hw_code;
        if (tsc->has_hw_code) {
            tsc->ptat[0]  = ptat[0];
            tsc->ptat[1]  = ptat[1];
            tsc->ptat[2]  = ptat[2];
            tsc->ptat0[0] = ptat0[0];
            tsc->ptat0[2] = ptat0[2];
        }

        tsc->dev = (void *)dev;
        dev->tsc[i] = tsc;

        hwi_off = hwi_find_device(RCAR_HWI_THERMAL, i);
        if (hwi_off != HWI_NULL_OFF) {
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
            if (tag) {
                tsc->phy_addr = tag->location.base;
                tsc->reg_size = tag->location.len;
            }
        }

        /* Mapping memory for register */
        if (!tsc->phy_addr || ((uintptr_t)MAP_FAILED == (tsc->vaddr = (uintptr_t)mmap_device_io(tsc->reg_size, tsc->phy_addr)))) {
            thermal_slogf(VERBOSE_QUIET, "%s: mmap_device_io(0x%08) failed", __FUNCTION__, tsc->phy_addr);
            goto failed;
        }

        if(EOK != rcar_tsc_init(tsc)) {
            thermal_slogf(VERBOSE_QUIET, "%s: Failed to init TSC%d.", __FUNCTION__, i);
            goto failed;
        }
    }

    return EOK;
    
failed:
    rcar_thermal_deinit(dev);
    
    return -EIO;
}

