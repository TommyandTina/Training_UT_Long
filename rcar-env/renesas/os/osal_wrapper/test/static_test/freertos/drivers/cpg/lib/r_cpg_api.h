/*
============================================================================
DESCRIPTION
Contain the CPG modules types that the RCAR Drivers need.
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

#ifndef R_CPG_API_H_
#define R_CPG_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * @defgroup     CPGgroup CPG driver API
 *      CPG API (Clock Pulse Generator)
 */

/**
 * @enum    r_cpg_RegIdx_t
 * @brief   List of register index used in RMSTPCRn/MSTPSRn/SRCRn/SRSTCLRn
 */
typedef enum {
    R_CPG_REGIDX_0,
    R_CPG_REGIDX_1,
    R_CPG_REGIDX_2,
    R_CPG_REGIDX_3,
    R_CPG_REGIDX_4,
    R_CPG_REGIDX_5,
    R_CPG_REGIDX_6,
    R_CPG_REGIDX_7,
    R_CPG_REGIDX_8,
    R_CPG_REGIDX_9,
    R_CPG_REGIDX_10,
    R_CPG_REGIDX_11,
} r_cpg_RegIdx_t;

/**
 * @enum    r_cpg_BitPos_t
 * @brief   List of bit number
 */
typedef enum {
    R_CPG_BIT0,
    R_CPG_BIT1,
    R_CPG_BIT2,
    R_CPG_BIT3,
    R_CPG_BIT4,
    R_CPG_BIT5,
    R_CPG_BIT6,
    R_CPG_BIT7,
    R_CPG_BIT8,
    R_CPG_BIT9,
    R_CPG_BIT10,
    R_CPG_BIT11,
    R_CPG_BIT12,
    R_CPG_BIT13,
    R_CPG_BIT14,
    R_CPG_BIT15,
    R_CPG_BIT16,
    R_CPG_BIT17,
    R_CPG_BIT18,
    R_CPG_BIT19,
    R_CPG_BIT20,
    R_CPG_BIT21,
    R_CPG_BIT22,
    R_CPG_BIT23,
    R_CPG_BIT24,
    R_CPG_BIT25,
    R_CPG_BIT26,
    R_CPG_BIT27,
    R_CPG_BIT28,
    R_CPG_BIT29,
    R_CPG_BIT30,
    R_CPG_BIT31,
} r_cpg_BitPos_t;

/**
 * @enum    r_cpg_Error_t
 * @brief   list of errors CPG API can return
 */
typedef enum r_cpg_Error_st {
    R_CPG_ERR_SUCCESS,             /**< success */
    R_CPG_ERR_INVALID_PARAMETER,   /**< invalid parameter */
    R_CPG_ERR_FAILED,              /**< failed */
} r_cpg_Error_t;

/**
 * @enum    r_cpg_ClockName_t
 * @brief   List of clock name
 */
typedef enum {
    R_CPG_ZB3_CLOCK,      /* ZB3 Clock   */
    R_CPG_SD0H_CLOCK,     /* SD0H Clock   */
    R_CPG_SD0_CLOCK,      /* SD0 Clock    */
    R_CPG_SD1H_CLOCK,     /* SD1H Clock   */
    R_CPG_SD1_CLOCK,      /* SD1 Clock    */
    R_CPG_SD2H_CLOCK,     /* SD2H Clock   */
    R_CPG_SD2_CLOCK,      /* SD2 Clock    */
    R_CPG_SD3H_CLOCK,     /* SD3H Clock   */
    R_CPG_SD3_CLOCK,      /* SD3 Clock    */
    R_CPG_SSPSRC_CLOCK,   /* SSPSRC Clock */
    R_CPG_SSPRS_CLOCK,    /* SSPRS Clock  */
    R_CPG_RPC_CLOCK,      /* RPC Clock    */
    R_CPG_RPCD2_CLOCK,    /* RPCD2 Clock  */
    R_CPG_MSO_CLOCK,      /* MSO Clock    */
    R_CPG_CANFD_CLOCK,    /* CANFD Clock  */
    R_CPG_HDMI_CLOCK,     /* HDMI Clock   */
    R_CPG_CSI0_CLOCK,     /* CSI0 Clock   */
    R_CPG_CLOCK_LAST
} r_cpg_ClockName_t;

/**

 * Function R_PMA_CPG_Init
 * @brief Initialise of CPG
 */
void R_PMA_CPG_Init(void);

/**
 * Function R_PMA_CPG_Quit
 * @brief DeInitialise of CPG
 */
void R_PMA_CPG_Quit(void);

/**
 * Function R_PMA_CPG_SetModuleStopStat
 * @brief Set a state of the module stop for specified module
 *
 * @param[in]   RegIdx - RMSTPCR register index @ref r_cpg_RegIdx_t
 * @param[in]   BitPos - RMSTPCR register bit number @ref r_cpg_BitPos_t
 * @param[in]   enable - true: Enable Module Stop, false: Disable Module Stop
 * @retval      See  @ref r_cpg_Error_t
 */
r_cpg_Error_t R_PMA_CPG_SetModuleStopStat(r_cpg_RegIdx_t RegIdx, 
                                           r_cpg_BitPos_t BitPos,
                                           bool enable);

/**
 * Function R_PMA_CPG_GetModuleStopStat
 * @brief Get a state of the power for specified power domain
 *
 * @param[in]   RegIdx - RMSTPCR register index @ref r_cpg_RegIdx_t
 * @param[in]   BitPos - RMSTPCR register bit number @ref r_cpg_BitPos_t
 * @param[out]  enable - true: Module Stop is enabled, false: Module Stop is disabled
 * @retval      See  @ref r_cpg_Error_t
 */
r_cpg_Error_t R_PMA_CPG_GetModuleStopStat(r_cpg_RegIdx_t RegIdx,
                                           r_cpg_BitPos_t BitPos,
                                           bool* enable);

/**
 * Function R_PMA_CPG_SetResetStat
 * @brief Set a state of the CPG reset for specified module
 *
 * @param[in]   RegIdx - SRCR/SRSTCLR register index @ref r_cpg_RegIdx_t
 * @param[in]   BitPos - SRCR/SRSTCLR register bit number @ref r_cpg_BitPos_t
 * @param[in]   ResetStat - 1: Set SRCR target bit to 1.
 *     0: Set the SRSTCLR target bit to 1 and wait for the SRCR bit to become 0.
 *   It is necessary to wait for RCLK 1 cycle on the user side. 
 * @retval      See  @ref r_cpg_Error_t
 */
r_cpg_Error_t R_PMA_CPG_SetResetStat(r_cpg_RegIdx_t RegIdx,
                                      r_cpg_BitPos_t BitPos,
                                      uint32_t ResetStat);

/**
 * Function R_PMA_CPG_GetResetStat
 * @brief Get a state of the CPG reset for specified module
 *
 * @param[in]   RegIdx    - SRCR register index @ref r_cpg_RegIdx_t
 * @param[in]   BitPos    - SRCR register bit number @ref r_cpg_BitPos_t
 * @param[out]  ResetStat - Reset status, 0: SRCR target bit is 0
 *                                        1: SRCR target bit is 1 
 * @retval      See  @ref r_cpg_Error_t
 */
r_cpg_Error_t R_PMA_CPG_GetResetStat(r_cpg_RegIdx_t RegIdx,
                                      r_cpg_BitPos_t BitPos,
                                      uint32_t* ResetStat);

/**
 * Function R_PMA_CPG_SetClockStopStat
 * @brief Set a state of the clock stop for specified module
 *
 * @param[in]   ClockName - Clock name of CPG @ref r_cpg_ClockName_t
 * @param[in]   enable - true: Enable Clock Stop, false: Disable Clock Stop
 * @retval      See  @ref r_cpg_Error_t
 */
r_cpg_Error_t R_PMA_CPG_SetClockStopStat(r_cpg_ClockName_t ClockName,
                                          bool enable);

/**
 * Function R_PMA_CPG_GetClockStopStat
 * @brief Get a state of the clock stop for specified module
 *
 * @param[in]   ClockName - Clock name of CPG @ref r_cpg_ClockName_t
 * @param[out]  enable - true: Clock is stopped, false: Clock is running
 * @retval      See  @ref r_cpg_Error_t
 */
r_cpg_Error_t R_PMA_CPG_GetClockStopStat(r_cpg_ClockName_t ClockName,
                                          bool* enable);

/** @} */ /*end of group CPGgroup */

#ifdef __cplusplus
}
#endif

#endif /* R_CPG_API_H_ */
