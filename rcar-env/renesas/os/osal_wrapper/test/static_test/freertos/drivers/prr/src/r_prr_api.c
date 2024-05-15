/*
============================================================================
DESCRIPTION
PRR low level driver API
============================================================================
                            C O P Y R I G H T
============================================================================
                      Copyright (c) 2019 - 2020
                                  by
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: to be used as sample S/W

DISCLAIMER
This software is supplied by Renesas Electronics Corporation and is only
intended for use with Renesas products. No other uses are authorized. This
software is owned by Renesas Electronics Corporation and is protected under
all applicable laws, including copyright laws.
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
Renesas reserves the right, without notice, to make changes to this software
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the
following link:
http://www.renesas.com/disclaimer *
Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*/

#include "r_prr_api.h"

/* Product Register */
#define PRR                 (0xFFF00044U)
#define PRR_PRODUCT_MASK    (0x00007F00U)
#define PRR_CUT_MASK        (0x000000FFU)
#define PRR_PRODUCT_H3      (0x00004F00U)           /* R-Car H3   */
#define PRR_PRODUCT_M3      (0x00005200U)           /* R-Car M3-W */
#define PRR_PRODUCT_V3M     (0x00005400U)           /* R-Car V3M  */
#define PRR_PRODUCT_M3N     (0x00005500U)           /* R-Car M3-N */
#define PRR_PRODUCT_V3H     (0x00005600U)           /* R-Car V3H  */
#define PRR_PRODUCT_E3      (0x00005700U)           /* R-Car E3  */
#define PRR_PRODUCT_D3      (0x00005800U)           /* R-Car D3  */
#define PRR_PRODUCT_10      (0x00U)         /*   Ver.1.0  */
#define PRR_PRODUCT_11      (0x01U)         /*   Ver.1.1  */
#define PRR_PRODUCT_20      (0x10U)         /*   Ver.2.0  */
#define PRR_PRODUCT_30      (0x20U)         /*   Ver.3.0  */

static uint32_t loc_RegRead32(uintptr_t addr)
{
    return *((volatile uint32_t *)addr);
}

r_prr_DevName_t R_PRR_GetDevice(void)
{
    r_prr_DevName_t dev;
    uint32_t        reg_val;

    reg_val = loc_RegRead32(PRR);
    reg_val = reg_val & PRR_PRODUCT_MASK;

    switch (reg_val) {
    case PRR_PRODUCT_H3:
        dev = R_PRR_RCARH3;
        break;
    case PRR_PRODUCT_M3:
        dev = R_PRR_RCARM3W;
        break;
    case PRR_PRODUCT_M3N:
        dev = R_PRR_RCARM3N;
        break;
    case PRR_PRODUCT_E3:
        dev = R_PRR_RCARE3;
        break;
    case PRR_PRODUCT_D3:
        dev = R_PRR_RCARD3;
        break;
    default:
        dev = R_PRR_RCARLAST;
        break;
    }

    return dev;
}
