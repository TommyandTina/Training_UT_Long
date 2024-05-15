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


/* THCODE value ref 10A.4(6) */
int THCODE_TABLE[TSC_MAX_NUM][3] = {
    /* TSC0 */ {    0,    0,   0 },
    /* TSC1 */ { 3397, 2800, 2221 }, // For H3, M3, V3H
    /* TSC2 */ { 3393, 2795, 2216 }, // For H3, M3, V3H
    /* TSC3 */ { 3389, 2805, 2237 }, // For H3, M3, V3H
    /* TSC4 */ {    0,    0,   0 },
};

int PTAT_TABLE[TSC_MAX_NUM][3] = {
    /* TSC0 */ {    0,    0,   0 }, 
    /* TSC1 */ { 2631, 1509, 435 }, // For H3, M3, V3H
    /* TSC2 */ { 2631, 1509, 435 }, // For H3, M3, V3H
    /* TSC3 */ { 2631, 1509, 435 }, // For H3, M3, V3H
    /* TSC4 */ {    0,    0,   0 }, 
};

/* For V3U:
 *  There is no official values yet.
 *  So values in the following tables are just examples from section 10C.3.3.3 
 */
int V3U_THCODE_TABLE[3] = {
    /* TSC0,1,2,3,4 */ 3415, 2694, 2195 // An example for V3U ( ? )
};

int V3U_PTAT_TABLE[3] = {
    /* TSC0,1,2,3,4 */ 2650, 1288,  347 // An example for V3U ( ? )
};

int V3U_PTAT0_TABLE[3] = {
    /* TSC0,1,2,3,4 */ 2659,   0,   357 // An example for V3U ( ? )
};

/* Table of TSC constant data */
rcar_tsc_t tsc_data_table[] = {
    {   /* TSC0 Data */
        .phy_addr    = RCAR_GEN3_THERMAL0_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_GEN3_THERMAL_SIZE,
        .thcode      = THCODE_TABLE[0],
        .ptat        = PTAT_TABLE[0],
        .ptat0       = PTAT_TABLE[0],
        .has_hw_code = 0,
        .ths_chan    = 0,
    },
    {   /* TSC1 Data */
        .phy_addr    = RCAR_GEN3_THERMAL1_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_GEN3_THERMAL_SIZE,
        .thcode      = THCODE_TABLE[1],
        .ptat        = PTAT_TABLE[1],
        .ptat0       = PTAT_TABLE[1],
        .has_hw_code = 1,
        .ths_chan    = 1,
    },    
    {   /* TSC2 Data */
        .phy_addr    = RCAR_GEN3_THERMAL2_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_GEN3_THERMAL_SIZE,
        .thcode      = THCODE_TABLE[2],
        .ptat        = PTAT_TABLE[2],
        .ptat0       = PTAT_TABLE[2],
        .has_hw_code = 0,
        .ths_chan    = 2,
    },
    {   /* TSC3 Data */
        .phy_addr    = RCAR_GEN3_THERMAL3_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_GEN3_THERMAL_SIZE,
        .thcode      = THCODE_TABLE[3],
        .ptat        = PTAT_TABLE[3],
        .ptat0       = PTAT_TABLE[3],
        .has_hw_code = 0,
        .ths_chan    = 3,
    },
    {   /* TSC4 Data */
        .phy_addr    = RCAR_GEN3_THERMAL4_BASE,
        .vaddr       = (uintptr_t)MAP_FAILED,
        .reg_size    = RCAR_GEN3_THERMAL_SIZE,
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
 * Outline      : Read TEMP_CODE and return temperature
 * Function Name: rcar_thermal_get_curren_temp
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : temperature in celsius
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_get_current_temp(rcar_tsc_t *tsc)
{
    int celsius = 0, temp_code;
    float temp;

    if (tsc->product_id == PRODUCT_ID_RCAR_V3U) {
        /* Read register and convert value to Celsius */
        temp_code = in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP) & TEMP_CODE_MASK;

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
        celsius = _round_temp(temp);
    }
    else {
        if (tsc->type == TYPE_A) {
            int t1 = 126, t2 = 167;
            /* Read register and convert value to Celsius */
            temp_code = in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP) & TEMP_CODE_MASK;
            /* Temperature calculation based on hardware user manual, section 10A.3.1.2 */
            if (tsc->product_id == PRODUCT_ID_RCAR_M3W) {
                t1 = 116;
                t2 = 157;
            }

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
            celsius = _round_temp(temp);
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
                new = in32(tsc->vaddr + RCAR_GEN3_THERMAL_B_THSSR) & RCAR_GEN3_THERMAL_THSSR_CTEMP_MASK;

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
                 celsius = ((reg * 55) - 720) / 10;
            else
                celsius = (reg * 5) - 60;
        }
    }

    return celsius;
}

/*
 **********************************************************************************
 * Outline      : Convert temperature in celsius to temp_code to write to IRQTEMP1-3
 * Function Name: rcar_thermal_convert_temp_to_code
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : temp_code
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_convert_temp_to_code(rcar_tsc_t *tsc, int celsius)
{
    int ctemp = 0;
    float tmp;

    if (tsc->product_id == PRODUCT_ID_RCAR_V3U) {
        if (celsius <= tsc->tj_t) {
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
    else {
        if (tsc->type == TYPE_A) {
            int t1 = 126, t2 = 167;

            /* Calculate IRQTEMP based on the hardware user manual v2.0, section 10A.3.1.1 */
            if (tsc->product_id == PRODUCT_ID_RCAR_M3W) {
                t1 = 116;
                t2 = 157;
            }
            if (celsius <= tsc->tj_t) {
                tmp  = (float)((celsius + 41) * (tsc->ptat[2] - tsc->ptat[0]) * (tsc->thcode[2] - tsc->thcode[1]));
                tmp /= (float)(t2 * (tsc->ptat[2] - tsc->ptat[1]));
                ctemp = tsc->thcode[2] - _round_temp(tmp);
            }
            else {
                tmp  = (float)((celsius - t1) * (tsc->ptat[0] - tsc->ptat[2]) * (tsc->thcode[1] - tsc->thcode[0]));
                tmp /= (float)(t2 * (tsc->ptat[0] - tsc->ptat[1]));
                ctemp = tsc->thcode[0] - _round_temp(tmp);
            }
        } else {
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
    }

    return ctemp;
}

/*
 **************************************************************************************************
 * Outline      : Set threadhold temperature to raise interrupts for A-/C-type TSC
 * Function Name: rcar_thermal_set_irq_temp_ac
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : -1
 *                 EOK
 * Note         : Assuming TEMP2 interrupt when temperature over than current temperature 2 Celsius
 *                and TEMPD1 interrupt when temperature lower than current tempperature 2 Celsius
 ****************************************************************************************************
 */
static int rcar_thermal_set_irq_temp_ac(rcar_tsc_t *tsc)
{
    int temp, thr_low, thr_high;
    int ctemp_low = 0, ctemp_high = 0;

    temp = rcar_thermal_get_current_temp(tsc);

    thr_low = temp - 2;
    thr_high = temp + 2;

    ctemp_low = rcar_thermal_convert_temp_to_code(tsc, thr_low);
    ctemp_high = rcar_thermal_convert_temp_to_code(tsc, thr_high);

    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP1, ctemp_low);
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP2, ctemp_high);

    /* Enable TEMPD1 and TEMP2 interrupts */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQEN, RCAR_GEN3_THERMAL_IRQ_TEMPD1_EN | RCAR_GEN3_THERMAL_IRQ_TEMP2_EN);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : SSet threahold temperature to raise interrupts for type-B TSC
 * Function Name: rcar_thermal_set_irq_temp_b
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : -1
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_set_irq_temp_b(rcar_tsc_t *tsc)
{
    int celsius, thr_low, thr_high;
    uint32_t reg, high, low;

    celsius = rcar_thermal_get_current_temp(tsc);

    thr_low  = celsius - 1;
    thr_high = celsius + 1;

    low  = rcar_thermal_convert_temp_to_code(tsc, thr_low);
    high = rcar_thermal_convert_temp_to_code(tsc, thr_high);

    reg  = in32(tsc->vaddr + RCAR_GEN3_THERMAL_B_INTCTLR);
    reg &= ~(INTCTLR_CTEMP1_MASK | INTCTLR_CTEMP0_MASK);
    reg |= INTCTLR_CTEMP1_VAL(high) | INTCTLR_CTEMP0_VAL(low);

    out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_INTCTLR, reg);

    /* Enable Interrupts */    
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_ENR, RCAR_GEN3_THERMAL_B_Tj00INT | RCAR_GEN3_THERMAL_B_Tj01INT);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Fucntion use to initialize H3 1.1 A-type TSC
 * Function Name: rcar_themal_init_a_h3_1_1
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value :  ENOMEM
 *                 EOK
 * Note         : Initialize A-type TSC workaround
 *                There is no document to mention about this difference. 
 *                Contact Renesas to get further information
 ***********************************************************************************
 */
static int rcar_themal_init_a_h3_1_1(rcar_tsc_t *tsc)
{
    /* Mapping memory for register */
    if (MAP_DEVICE_FAILED == (tsc->vaddr = (uintptr_t)mmap_device_io(tsc->reg_size, tsc->phy_addr)))
        return ENOMEM;

    out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR,  1 << 5);
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR,  0x0);

    delay(1);

    out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR,  1 << 8);

    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQCTL, 0x3F);
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQMSK, 0);
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQEN, RCAR_GEN3_THERMAL_IRQ_TEMPD1_EN | RCAR_GEN3_THERMAL_IRQ_TEMP2_EN);

    out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR, (1 << 8) | (1 << 7) | (1 << 5) | (1 << 4));

    delay(1);

    out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR, (1 << 8) | (1 << 7) | (1 << 5) | (1 << 4) | (1 << 1) | (1 << 0));

    delay(1);

    if (tsc->has_hw_code && (COR_PARA_VLD == (in32(tsc->vaddr + RCAR_GEN3_THERMAL_THSCP) & COR_PARA_VLD))) {
        tsc->thcode[0] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCODE1) & RCAR_GEN3_THERMAL_THCODE_MASK;
        tsc->thcode[1] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCODE1) & RCAR_GEN3_THERMAL_THCODE_MASK;
        tsc->thcode[2] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCODE1) & RCAR_GEN3_THERMAL_THCODE_MASK;
    }

    rcar_thermal_set_irq_temp_ac(tsc);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Fucntion use to initialize A-/C-type TSC
 * Function Name: rcar_thermal_init_ac
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value :  ENOMEM
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_init_ac(rcar_tsc_t *tsc)
{
    uint32_t reg_val = 0;
    float tmp;
    int timeout;

    if ((tsc->product_id == PRODUCT_ID_RCAR_H3) &&
        ((tsc->product_rev == PRODUCT_REV_1_0) || (tsc->product_rev == PRODUCT_REV_1_1))) {
            return rcar_themal_init_a_h3_1_1(tsc);
    }

    /* Mapping memory for register */
    if ((uintptr_t)MAP_FAILED == (tsc->vaddr = (uintptr_t)mmap_device_io(tsc->reg_size, tsc->phy_addr))) {
        thermal_slogf(VERBOSE_QUIET, "%s: mmap_device_io(RCAR_GEN3_TSC_BASE) failed", __FUNCTION__);
        return ENOMEM;
    }

    if (tsc->product_id == PRODUCT_ID_RCAR_V3U) {
        timeout = 100; //Standby mode checking timeout (T.B.D)
        while ((--timeout > 0) && ((in32(tsc->vaddr + RCAR_GEN3_THERMAL_THINITSTR) & RCAR_GEN3_THERMAL_THINITSTR_THTMP_MASK) != RCAR_GEN3_THERMAL_THINITSTR_THTMP_MASK)) {
            /* Can read THCODE_INT1, THCODE_INT2, THCODE_INT3 here */
            nanospin_ns(1000); 
        }

        timeout = 100; //Connection test mode checking timeout (T.B.D)
        while ((--timeout > 0) && (in32(tsc->vaddr + RCAR_GEN3_THERMAL_CVM_CTRL_BK) & RCAR_GEN3_THERMAL_CVM_CTRL_BK_THEN_BK)) {
            nanospin_ns(1000); 
        }    
    }

    reg_val  = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR);  
          
    /* Normal mode, both THS & CIVM are ON */
    reg_val &= ~RCAR_GEN3_THERMAL_THCTR_PONM;
    reg_val &= ~RCAR_GEN3_THERMAL_THCTR_SENSSEL;
    reg_val &= ~RCAR_GEN3_THERMAL_THCTR_THEN;
    reg_val &= ~RCAR_GEN3_THERMAL_THCTR_CIVMTST;
    reg_val &= ~RCAR_GEN3_THERMAL_THCTR_CTCTL;
    
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR, reg_val);
    
    /* Initial Sequence Stop Setting */
    if (tsc->product_id == PRODUCT_ID_RCAR_V3U) {
        timeout = 100; //Initial Sequence Stop Setting timeout
        while ((--timeout > 0) && !((in32(tsc->vaddr + RCAR_GEN3_THERMAL_SEQ_ACT_MON) & RCAR_GEN3_THERMAL_SEQ_ACT_MON_ACT_MASK))) {
            nanospin_ns(1000); 
        }    
    }

    /* Wait for initialization */
    delay(1);

    /* Read PTAT & THCODE */
    if (tsc->has_hw_code && (COR_PARA_VLD == (in32(tsc->vaddr + RCAR_GEN3_THERMAL_THSCP) & COR_PARA_VLD))) {
        tsc->ptat[0] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_PTAT1) & RCAR_GEN3_THERMAL_PTAT_MASK;
        tsc->ptat[1] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_PTAT2) & RCAR_GEN3_THERMAL_PTAT_MASK;
        tsc->ptat[2] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_PTAT3) & RCAR_GEN3_THERMAL_PTAT_MASK;

        if ((tsc->ths_chan == 1) && (tsc->product_id == PRODUCT_ID_RCAR_V3U)) {
            tsc->ptat0[0] = (in32(tsc->vaddr + RCAR_GEN3_THERMAL_PTAT1) >> 16) & RCAR_GEN3_THERMAL_PTAT_MASK;
            tsc->ptat0[2] = (in32(tsc->vaddr + RCAR_GEN3_THERMAL_PTAT3) >> 16) & RCAR_GEN3_THERMAL_PTAT_MASK;
        }

        tsc->thcode[0] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCODE1) & RCAR_GEN3_THERMAL_THCODE_MASK;
        tsc->thcode[1] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCODE1) & RCAR_GEN3_THERMAL_THCODE_MASK;
        tsc->thcode[2] = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCODE1) & RCAR_GEN3_THERMAL_THCODE_MASK;
    }

    /* Calculate Tj_T following the hardware user manual v2.0, section 10A.3.1.1 */
    if (tsc->product_id == PRODUCT_ID_RCAR_V3U) {
        tmp  = (float)((tsc->ptat[1] - tsc->ptat0[2]) * 167);
        tmp /= (float) (tsc->ptat0[0] - tsc->ptat0[2]);
    }
    else if (tsc->product_id == PRODUCT_ID_RCAR_M3W) {
        tmp  = (float)((tsc->ptat[1] - tsc->ptat[2]) * 157);
        tmp /= (float) (tsc->ptat[0] - tsc->ptat[2]);            
    }
    else {
        tmp  = (float)((tsc->ptat[1] - tsc->ptat[2]) * 167);
        tmp /= (float) (tsc->ptat[0] - tsc->ptat[2]);            
    }
    tsc->tj_t = _round_temp(tmp - 41.0);

    /* Clear interrupt status */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQSTR, 0);

    /* Clear TEMPD1 and TEMP2 interrupt masks */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQMSK, RCAR_GEN3_THERMAL_IRQ_TEMPD1_EN | RCAR_GEN3_THERMAL_IRQ_TEMP2_EN);

     if (tsc->product_id == PRODUCT_ID_RCAR_V3U) {
        /* Setup ECM for receiving error from TSC1~4 and then interrupt to CPU */
        out32(tsc->ecm_vaddr + RCAR_V3U_ECM_WPCNTR, (0xACCE << 16 ) | 0x1); /* Disable write protection */
        out32(tsc->ecm_vaddr + RCAR_V3U_ECM_ERRCTLR13, in32(tsc->ecm_vaddr + RCAR_V3U_ECM_ERRCTLR13) | (0xFFF << 14)); /* Enable error detection */
        out32(tsc->ecm_vaddr + RCAR_V3U_ECM_ERRTGTR13, in32(tsc->ecm_vaddr + RCAR_V3U_ECM_ERRTGTR13) | (0xFFF << 14)); /* Route error detection to INTC */
    }

    /* Set of threadholds cause over/under interuppts */
    //rcar_thermal_set_irq_temp_ac(tsc);

    /* Enable the A/D converter */
    reg_val  = in32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR);
    reg_val |= RCAR_GEN3_THERMAL_THCTR_THSST;
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR, reg_val);

    delay(1);
    
    /* By reading current TEMP_CODE to calculate curent temperature 
     * and then set threadholds by +/- 2 degrees to cause over/under interuppts.
     * If not to start TSC2, TEMP_CODE is always zero 
     * So we put set interrupt after starting TSC2 
     */ 
    rcar_thermal_set_irq_temp_ac(tsc);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Fucntion use to initialize B-type TSC
 * Function Name: rcar_thermal_init_b
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value :  ENOMEM
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_init_b(rcar_tsc_t *tsc)
{
    uint32_t reg_val = 0;

    /* Mapping memory for register */
    if ((uintptr_t)MAP_FAILED == (tsc->vaddr = (uintptr_t)mmap_device_io(tsc->reg_size, tsc->phy_addr)))
        return ENOMEM;

    if (tsc->product_id == PRODUCT_ID_RCAR_V3M)
        nanospin_ns(3500 * 1000);

    /* Using 2 interrupts: Tj00 falling, Tj01 rising */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_POSNEG, 0x1);

    /* Setting control register */
    reg_val = in32(tsc->vaddr + RCAR_GEN3_THERMAL_B_THSCR);
    reg_val = RCAR_GEN3_THERMAL_B_THSCR_CPCTL;
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_THSCR, reg_val);

    /* Wait for initialization */
    nanospin_ns(350 * 1000);

    /* Clear interrupt status */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_STR, 0);

    /* Mask interrupt */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_INT_MASK, 0x4 | 
        (~0x7 & in32(tsc->vaddr + RCAR_GEN3_THERMAL_B_INT_MASK)));

    rcar_thermal_set_irq_temp_b(tsc);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : A-/C-type TSC interrupt handler
 * Function Name: rcar_thermal_intr
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : -1
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_intr_ac(rcar_tsc_t * tsc)
{
    uint32_t irq_status;

    if (tsc->product_id == PRODUCT_ID_RCAR_V3U) {
        /* Clear ECM error status */
        out32(tsc->ecm_vaddr + RCAR_V3U_ECM_ERRSTSR13, (0xFFF << 14) & in32(tsc->ecm_vaddr + RCAR_V3U_ECM_ERRSTSR13)); 
    }

    /* Get interrupt status value */
    irq_status = in32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQSTR);

    if(irq_status == 0)
        return EINVAL; // No interrupt

    /* Clear interrupt status */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQSTR, 0);

    if (irq_status & RCAR_GEN3_THERMAL_IRQ_TEMP1_EN)
    {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : TEMP_CODE(%x) is over IRQTEMP1(5x)", tsc->ths_chan,
                in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP), in32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP1));
    } 

    if (irq_status & RCAR_GEN3_THERMAL_IRQ_TEMP2_EN)
    {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : TEMP_CODE(%x) is over IRQTEMP2(%x)", tsc->ths_chan, 
                in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP), in32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP2));
    }

    if (irq_status & RCAR_GEN3_THERMAL_IRQ_TEMP3_EN)
    {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : TEMP_CODE(%x) is over IRQTEMP3(%x)", tsc->ths_chan,
                in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP), in32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP3));
    }

    if(irq_status & RCAR_GEN3_THERMAL_IRQ_TEMPD1_EN)
    {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : TEMP_CODE(%x) is below IRQTEMP1(%x)", tsc->ths_chan, 
                in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP), in32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP1));
    }

    if(irq_status & RCAR_GEN3_THERMAL_IRQ_TEMPD2_EN)
    {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : TEMP_CODE(%x) is below IRQTEMP2(%x)", tsc->ths_chan,
                in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP), in32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP2));
    }

    if(irq_status & RCAR_GEN3_THERMAL_IRQ_TEMPD3_EN)
    {
        thermal_slogf(VERBOSE_LEVEL1, "TSC%d : TEMP_CODE(%x) is below IRQTEMP3(%x)", tsc->ths_chan,
                in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP), in32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP3));
    }

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : A-/C-type TSC interrupt handler
 * Function Name: rcar_thermal_intr_b
 * Description  :
 * Arguments    : rcar_tsc_t   *tsc
 * Return Value : -1
 *                 EOK
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_intr_b(rcar_tsc_t *tsc)
{
    uint32_t irq_status;

    /* Get interrupt status value */
    irq_status = in32(tsc->vaddr + RCAR_GEN3_THERMAL_B_STR);

    /* Clear interrupt status value */
    out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_STR, 0);

    if (irq_status & RCAR_GEN3_THERMAL_B_Tj00INT)
    {
        thermal_slogf(VERBOSE_LEVEL1, "Interrupt TJ00 occurs.");
    }

    if (irq_status & RCAR_GEN3_THERMAL_B_Tj01INT)
    {
        thermal_slogf(VERBOSE_LEVEL1, "Interrupt TJ01 occurs.");
    }

    if (irq_status & RCAR_GEN3_THERMAL_B_Tj02INT)
    {
        thermal_slogf(VERBOSE_LEVEL1, "Interrupt TJ02 occurs.");
    }

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Get temperature of A-/C-type TSC
 * Function Name: rcar_thermal_get_temp_ac
 * Description  :
 * Arguments    : rcar_tsc_t      *tsc
 * Return Value : temperature
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_get_temp_ac(rcar_tsc_t *tsc, int *temp)
{
    *temp = rcar_thermal_get_current_temp(tsc);

    tsc->cur_temp = *temp;
    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d temperature = %d", __FUNCTION__, tsc->ths_chan, *temp);

    return EOK;
}

/*
 * *********************************************************************************
 * Outline      : Get temperature of B-type TSC
 * Function Name: rcar_thermal_get_temp_b
 * Description  :
 * Arguments    : rcar_tsc_t      *tsc
 * Return Value : temperature
 * Note         :
 **********************************************************************************
 */
static int rcar_thermal_get_temp_b(rcar_tsc_t *tsc, int *temp)
{
    *temp = rcar_thermal_get_current_temp(tsc);

    tsc->cur_temp = *temp;
    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d temperature = %d", __FUNCTION__, tsc->ths_chan, *temp);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Get operate voltage of A-type TSC
 * Function Name: rcar_thermal_get_mvolt
 * Description  :
 * Arguments    : rcar_tsc_t  *tsc
 * Return Value : mili-voltage value
 * Note         : We are using formula from Manual v2.00 for product H3, M3W, M3N, V3H
 ***********************************************************************************
 */
static int rcar_thermal_get_mvolt_a(rcar_tsc_t *tsc, int *mvolt)
{
    float vdd = 0.0;
    int volt_code = in32(tsc->vaddr + RCAR_GEN3_THERMAL_VOLT) & VOLT_CODE_MASK;
    int temp = rcar_thermal_get_current_temp(tsc);

    /* Voltage calculation based on hardware user manual v1.5 section 10A.3.1.2 */
    if(PRODUCT_ID_RCAR_M3W == tsc->product_id) {
        vdd  = 360 * (float)(tsc->thcode[2] - 4 * volt_code);
        vdd /= (float)(tsc->thcode[2] - tsc->thcode[0]);
        vdd += 630.0 + (float)temp * 0.25;
    }
    else {
        vdd  = 390.0 * (float)(tsc->thcode[2] - 4 * volt_code);
        vdd /= (float)(tsc->thcode[2] - tsc->thcode[0]);
        vdd += 630.0 + (float)temp * 0.1;
    }
    *mvolt = _round_temp(vdd);

    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d voltage(mV) = %d",__FUNCTION__, tsc->ths_chan, *mvolt);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Get operate voltage of B-type TSC
 * Function Name: rcar_thermal_get_mvolt_b
 * Description  :
 * Arguments    : rcar_tsc_t  *tsc
 *              : int * mvolt 
 * Return Value : EOK 
 *              : EINVAL
 * Note         :
 ***********************************************************************************
 */
static int rcar_thermal_get_mvolt_b(rcar_tsc_t *tsc, int *mvolt)
{
    uint32_t volt_code;
    int i, avolt = 0;

    if (tsc->product_id != PRODUCT_ID_RCAR_V3M)
        return EOPNOTSUPP;

    /* Read VOLT_CODE[5:0] */
    volt_code = in32(tsc->vaddr + RCAR_GEN3_THERMAL_B_CIVM_SR) & RCAR_GEN3_THERMAL_VOLT_CODE_B_MASK;

    /* The hardware user manual v2.0 section 10B.3.5 is saying:
     * When acquire a chip internal voltage, read VOLT_CODE[5:0] of CIVM_SR register 5 times and use an average value of them
     */
    for (i = 0; i < 5; i++) {
        nanospin_ns(1600 * 1000);
        avolt += VOLT_CONVERT(volt_code);
    }

    /* Voltage calculation based on hardware user manual v2.0 section 10B.2.8 */
    *mvolt = avolt + 855 ;

    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d voltage(mV) = %d", __FUNCTION__, tsc->ths_chan, *mvolt);

    return EOK;
}

/*
 **********************************************************************************
 * Outline      : Get operate voltage of C-type TSC
 * Function Name: rcar_thermal_get_mvolt
 * Description  :
 * Arguments    : rcar_tsc_t  *tsc
 * Return Value : mili-voltage value
 * Note         : We are using formula from Manual Rev0.5 for product V3U
 ***********************************************************************************
 */
static int rcar_thermal_get_mvolt_c(rcar_tsc_t *tsc, int *mvolt)
{
    float vdd = 0.0;
    int volt_code, temp_code;
    int vmcode3_l, vmcode3_h, vmcode2_l, vmcode2_h, vmcode1_l, vmcode1_h;
    int tmp;

    volt_code = in32(tsc->vaddr + RCAR_GEN3_THERMAL_VOLT) & VOLT_CODE_MASK;
    temp_code = in32(tsc->vaddr + RCAR_GEN3_THERMAL_TEMP) & TEMP_CODE_MASK;

    if(temp_code < tsc->thcode[1]) {
        tmp = in32(tsc->vaddr + RCAR_GEN3_THERMAL_VMCODE3);
        vmcode3_l = tmp & RCAR_GEN3_THERMAL_VMCODE_MASK;
        vmcode3_h = (tmp >> 16) & RCAR_GEN3_THERMAL_VMCODE_MASK;

        tmp = in32(tsc->vaddr + RCAR_GEN3_THERMAL_VMCODE2);
        vmcode2_l = tmp & RCAR_GEN3_THERMAL_VMCODE_MASK;
        vmcode2_h = (tmp >> 16) & RCAR_GEN3_THERMAL_VMCODE_MASK;

        vdd  = (temp_code - tsc->thcode[2]) * (700 + (volt_code - vmcode2_l) * (200 / (vmcode2_h - vmcode2_l)));
        vdd += (tsc->thcode[1] - temp_code) * (700 + (volt_code - vmcode3_l) * (200 / (vmcode3_h - vmcode3_l)));
        vdd /= (tsc->thcode[1] - tsc->thcode[2]);
    }
    else {
        tmp = in32(tsc->vaddr + RCAR_GEN3_THERMAL_VMCODE2);
        vmcode2_l = tmp & RCAR_GEN3_THERMAL_VMCODE_MASK;
        vmcode2_h = (tmp >> 16) & RCAR_GEN3_THERMAL_VMCODE_MASK;

        tmp = in32(tsc->vaddr + RCAR_GEN3_THERMAL_VMCODE1);
        vmcode1_l = tmp & RCAR_GEN3_THERMAL_VMCODE_MASK;
        vmcode1_h = (tmp >> 16) & RCAR_GEN3_THERMAL_VMCODE_MASK;

        vdd  = (temp_code - tsc->thcode[1]) * (700 + (volt_code - vmcode1_l) * (200 / (vmcode1_h - vmcode1_l)));
        vdd += (tsc->thcode[0] - temp_code) * (700 + (volt_code - vmcode2_l) * (200 / (vmcode2_h - vmcode2_l)));
        vdd /= (tsc->thcode[0] - tsc->thcode[1]);
    }

    *mvolt = _round_temp(vdd);

    thermal_slogf(VERBOSE_LEVEL2, "%s: TSC%d voltage(mV) = %d",__FUNCTION__, tsc->ths_chan, *mvolt);

    return EOK;
}

/*
 * Thermal Init
 */
static int rcar_thermal_init(rcar_tsc_t * tsc)
{    
    if(!tsc)
        return -1;

     if(TYPE_A == tsc->type || TYPE_C == tsc->type)
        return rcar_thermal_init_ac(tsc);
    else
        return rcar_thermal_init_b(tsc);
}

/*
 * Thermal Deinit
 */
static void rcar_thermal_deinit(rcar_tsc_t * tsc)
{
    if(!tsc)
        return;

    if ((uintptr_t)MAP_FAILED != tsc->vaddr) {
        /* Disable interrupts */
        if(TYPE_A == tsc->type || TYPE_C == tsc->type) {
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR, 0);
            delay(1);
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQEN, 0);
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQMSK, 0);
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_THCTR, 0);
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP1, 0);
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP2, 0);
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_IRQTEMP3, 0);
            delay(1);
        }
        else
            out32(tsc->vaddr + RCAR_GEN3_THERMAL_B_ENR, 0);

        munmap_device_io(tsc->vaddr, tsc->reg_size);
    }

    tsc->vaddr = (uintptr_t)MAP_FAILED;
}

/*
 * Process Thermal Interrupt
 */
int rcar_thermal_irq(rcar_tsc_t * tsc)
{
    if(!tsc)
        return -1;

     if(TYPE_A == tsc->type || TYPE_C == tsc->type)
        return rcar_thermal_intr_ac(tsc);
    else
        return rcar_thermal_intr_b(tsc);
}

/*
 * Get voltage in Celsius
 */
int rcar_thermal_get_temp(rcar_tsc_t * tsc, int *temp)
{
    if(!tsc)
        return -1;

    if(TYPE_A == tsc->type || TYPE_C == tsc->type)
        return(rcar_thermal_get_temp_ac(tsc, temp));
    else
        return(rcar_thermal_get_temp_b(tsc, temp));
}

/*
 * Get voltage in mili-vol
 */
int rcar_thermal_get_vol(rcar_tsc_t * tsc, int *mvolt)
{
    if(!tsc)
        return -1;

    if(TYPE_A == tsc->type)
        return(rcar_thermal_get_mvolt_a(tsc, mvolt));
    else if(TYPE_B == tsc->type)
        return(rcar_thermal_get_mvolt_b(tsc, mvolt));
    else
        return(rcar_thermal_get_mvolt_c(tsc, mvolt));
}


static int intr_pulse_handler (message_context_t * ctp, int code, unsigned flags, void *handle)
{
    rcar_tsc_dev_t *dev = (rcar_tsc_dev_t *)handle;
    int i, j, found = 0;

    for(i = 0; i < TSC_INTR_NUM; i++) {
        if(code == dev->intr_pulse_code[i]) {
            for(j = dev->chan_min; j <= dev->chan_max ; j++) {
                rcar_thermal_irq(dev->tsc[j]);
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


void rcar_thermal_dev_deinit(rcar_tsc_dev_t *dev)
{
    int i;

    if ((uintptr_t)MAP_FAILED != dev->ecm_vaddr)
        munmap_device_io(dev->ecm_vaddr, RCAR_V3U_ECM_SIZE);

    for(i = dev->chan_min; i <= dev->chan_max; i++)
        rcar_thermal_deinit(dev->tsc[i]);
    
    for(i = 0; i < TSC_INTR_NUM; i++) {
        if (dev->intr_id[i] != -1) 
            InterruptDetach(dev->intr_id[i]);
    }
}

/*
 * Parse data for THS based on product ID and init thermal
 */
int rcar_thermal_dev_init(rcar_tsc_dev_t *dev)
{
    rcar_tsc_t      *tsc;
    uintptr_t       prr_base;
    uint32_t        prr_val;
    rcar_tsc_type_t type;
    int             product_id;
    int             product_rev; 
    int             i;

    /* Get product ID of device */
    prr_base = (uintptr_t)mmap_device_io(4, RCAR_GEN3_PRODUCT_REGISTER);
    if(MAP_DEVICE_FAILED == prr_base) {
        thermal_slogf(VERBOSE_QUIET, "%s: mmap_device_io(RCAR_GEN3_PRODUCT_REGISTER) failed",__FUNCTION__);
        return ENOMEM;
    }

    prr_val = in32(prr_base);

    munmap_device_io (prr_base, 4);

    product_id = RCAR_GEN3_PRODUCT_ID(prr_val);
    product_rev = RCAR_GEN3_PRODUCT_REV(prr_val);

    switch(product_id) {
        case (PRODUCT_ID_RCAR_H3):
        case (PRODUCT_ID_RCAR_M3W):
        case (PRODUCT_ID_RCAR_M3N):
            type = TYPE_A;
            dev->chan_min = 1;
            dev->chan_max = 3;
            break;
        case (PRODUCT_ID_RCAR_V3H):
            type = TYPE_A;
            dev->chan_min = 1;
            dev->chan_max = 2;
            break;
        case (PRODUCT_ID_RCAR_V3U):
            type = TYPE_C;
            dev->chan_min = 1;
            dev->chan_max = 4;
            break;            
        case (PRODUCT_ID_RCAR_D3):
        case (PRODUCT_ID_RCAR_V3M):
        case (PRODUCT_ID_RCAR_E3):
            type = TYPE_B;
            dev->chan_min = dev->chan_max = 0;
            break;
        default:
            return EIO;
    }

    dev->ecm_vaddr = (uintptr_t)MAP_FAILED;
    if (product_id == PRODUCT_ID_RCAR_V3U) {
        /* Mapping memory for ECM register */
        if ((uintptr_t)MAP_FAILED == (dev->ecm_vaddr = (uintptr_t)mmap_device_io(RCAR_V3U_ECM_SIZE, RCAR_V3U_ECM_BASE))) {
            thermal_slogf(VERBOSE_QUIET, "%s: mmap_device_io(RCAR_V3U_ECM_BASE) failed", __FUNCTION__);
            return ENOMEM;
        }
    }

    /* Interrupt attach */
    for (i = 0; i < TSC_INTR_NUM; i++) {
        dev->intr_id[i] = -1;
        if (product_id == PRODUCT_ID_RCAR_V3U) {
            if (i != 0) continue;
            dev->intr_irq[i] = RCAR_V3U_INTCSYS_ECM_ERR13;
        } 
        else {
            switch (i) {
                case 0:
                    dev->intr_irq[i] = RCAR_GEN3_INTCSYS_THERMAL0;
                    break;
                case 1:
                    dev->intr_irq[i] = RCAR_GEN3_INTCSYS_THERMAL1;
                    break;
                case 2:
                default:
                    dev->intr_irq[i] = RCAR_GEN3_INTCSYS_THERMAL2;
                    break;
            }
        }
        dev->intr_pulse_code[i] = pulse_attach(dev->dpp, MSG_FLAG_ALLOC_PULSE, 0, intr_pulse_handler, dev);

        SIGEV_PULSE_INIT(&dev->event[i], dev->coid, 21, dev->intr_pulse_code[i], NULL);

        if ((dev->intr_id[i] = InterruptAttachEvent(dev->intr_irq[i], &dev->event[i],
                                            _NTO_INTR_FLAGS_TRK_MSK|_NTO_INTR_FLAGS_END)) == -1) {
            thermal_slogf(VERBOSE_QUIET, "%s: Interrupt attach failed:%s \n", __FUNCTION__, strerror(errno));
            goto failed;
        }
    }

    /* Initialize and start TSC */
    for (i = dev->chan_min; i <= dev->chan_max ; i++) {
        tsc = &tsc_data_table[i];
        if (product_id == PRODUCT_ID_RCAR_V3U) {
            tsc->thcode = &V3U_THCODE_TABLE[0];
            tsc->ptat   = &V3U_PTAT_TABLE[0];
            tsc->ptat0  = &V3U_PTAT0_TABLE[0];
        }
        tsc->type = type;
        tsc->product_id = product_id;
        tsc->product_rev = product_rev;
        tsc->ecm_vaddr = dev->ecm_vaddr;
        tsc->dev = (void *)dev;
        dev->tsc[i] = tsc;

        if(EOK != rcar_thermal_init(tsc)) {
            thermal_slogf(VERBOSE_QUIET, "%s: Failed to init TSC%d.", __FUNCTION__, i);
            goto failed;
        }
    }

    return EOK;
    
failed:
    rcar_thermal_dev_deinit(dev);
    
    return -EIO;
}

