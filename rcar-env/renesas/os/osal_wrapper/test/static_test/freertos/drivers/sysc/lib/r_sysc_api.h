/*
============================================================================
DESCRIPTION
SYSC Interface
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

#ifndef R_SYSC_API_H_
#define R_SYSC_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * @enum    r_sysc_Error_t
 * @brief   list of errors SYSC API can return
 */
typedef enum r_sysc_Error_st {
    R_SYSC_ERR_SUCCESS,             /**< success */
    R_SYSC_ERR_INVALID_PARAMETER,   /**< invalid parameter */
    R_SYSC_ERR_FAILED,              /**< failed */
} r_sysc_Error_t;

/**
 * @enum    r_sysc_PowerName_t
 * @brief   List of names that can control the power supply
 */
typedef enum {                    
    R_SYSC_CA57_CPU0, /* Cortex-A57 CPU0 */
    R_SYSC_CA57_CPU1, /* Cortex-A57 CPU1 */
    R_SYSC_CA57_CPU2, /* Cortex-A57 CPU2 */
    R_SYSC_CA57_CPU3, /* Cortex-A57 CPU3 */
    R_SYSC_CA57_SCU,  /* Cortex-A57 SCU  */
    R_SYSC_CA53_CPU0, /* Cortex-A53 CPU0 */
    R_SYSC_CA53_CPU1, /* Cortex-A53 CPU1 */
    R_SYSC_CA53_CPU2, /* Cortex-A53 CPU2 */
    R_SYSC_CA53_CPU3, /* Cortex-A53 CPU3 */
    R_SYSC_CA53_SCU,  /* Cortex-A53 SCU  */
    R_SYSC_CR7,       /* Cortex-R7       */
    R_SYSC_3DG_A,     /* 3DG-A           */
    R_SYSC_3DG_B,     /* 3DG-B           */
    R_SYSC_3DG_C,     /* 3DG-C           */
    R_SYSC_3DG_D,     /* 3DG-D           */
    R_SYSC_3DG_E,     /* 3DG-E           */
    R_SYSC_IMP_A3IR,  /* IMP/A3IR        */
    R_SYSC_A3VP,      /* A3VP            */
    R_SYSC_A3VC,      /* A3VC            */
    R_SYSC_A2VC1,     /* A2VC1           */
    R_SYSC_LAST
} r_sysc_PowerName_t;

/**
 * Function R_PMA_SYSC_Init
 * @brief Initialise of SYSC
 */
void R_PMA_SYSC_Init(void);

/**
 * Function R_PMA_SYSC_Quit
 * @brief DeInitialise of SYSC
 */
void R_PMA_SYSC_Quit(void);

/**
 * Function R_PMA_SYSC_SetPowerStat
 * @brief Set a state of the power for specified power domain
 *
 * @param[in]   PowerName - Power name of SYSC @ref r_sysc_PowerName_t
 * @param[in]   power_on  - true: power on, false:  power off
 * @retval      See  @ref r_sysc_Error_t
 */
r_sysc_Error_t R_PMA_SYSC_SetPowerStat(r_sysc_PowerName_t PowerName,
                                       bool power_on);

/**
 * Function R_PMA_SYSC_GetPowerStat
 * @brief Get a state of the power for specified power domain
 *
 * @param[in]   PowerName  - Power name of SYSC @ref r_sysc_PowerName_t
 * @param[in]   power_on   - true: power on, false:  power off
 * @param[out]  PowerStat  - Power status
 * @retval      See  @ref r_sysc_Error_t
 */
r_sysc_Error_t R_PMA_SYSC_GetPowerStat(r_sysc_PowerName_t PowerName,
                                       bool power_on,
                                       uint32_t* PowerStat);

#ifdef __cplusplus
}
#endif

#endif /* R_SYSC_API_H_ */
