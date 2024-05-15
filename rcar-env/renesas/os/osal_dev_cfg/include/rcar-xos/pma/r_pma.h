/***********************************************************************************************************************
* Copyright [2020] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* Version      : 0.8.0
* Description  :
***********************************************************************************************************************/

#ifndef R_PMA_H
/* Multiple inclusion protection macro */
#define R_PMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*======================================================================================================================
Public type definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup PMA API
 * @defgroup PMA_Types Type definitions
 *
 * @{
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @enum e_pma_return_t
 * PMA return code.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM001 */
typedef enum
{
    R_PMA_RESULT_OK,              /*!< OK */
    R_PMA_ERR_RESULT_NG,          /*!< fatal error */
    R_PMA_ERR_ALREADY_IN_USE,     /*!< another user occupy HWA */
    R_PMA_ERR_INVALID_HANDLE,     /*!< handle is invalid */
    R_PMA_ERR_NO_VACANT_HANDLE,   /*!< the number of user reached maximum */
    R_PMA_ERR_INVALID_ARGUMENT,   /*!< argument is invalid */
    R_PMA_ERR_BUS_IF_CHK_NG,      /*!< Internal Bus Interface Check NG */
    R_PMA_ERR_RESET_CHK_NG,       /*!< Reset status monitor NG */
    R_PMA_ERR_RESET_TOGGLE_CHK_NG /*!< Initial Toggle Check NG */
} e_pma_return_t;
/* Covers: V3X_XOS1_PMA_UD_EX_ENUM001 */

/*******************************************************************************************************************//**
 * @enum e_pma_hwa_id_t
 * Hardware accelerator id.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM002 */
typedef enum
{
    R_PMA_HWA_ID_IMP_TOP,           /*!< IMP TOP */
    R_PMA_HWA_ID_IMP_MASTER,        /*!< IMP LDMAC-MASTER */
    R_PMA_HWA_ID_IMP_CHECKER,       /*!< IMP LDMAC-CHECKER */
    R_PMA_HWA_ID_IMP_SPMI0_00,      /*!< IMP SPMI0 */
    R_PMA_HWA_ID_IMP_SPMI1_01,      /*!< IMP SPMI1 */
    R_PMA_HWA_ID_IMP_SPMC0_00,      /*!< IMP SPMC0 */
    R_PMA_HWA_ID_IMP_SPMC1_01,      /*!< IMP SPMC1 */
    R_PMA_HWA_ID_IMP_SPMC2_02,      /*!< IMP SPMC2 */
    R_PMA_HWA_ID_IMP_CORE0,         /*!< IMP0 */
    R_PMA_HWA_ID_IMP_CORE1,         /*!< IMP1 */
    R_PMA_HWA_ID_IMP_CORE2,         /*!< IMP2 */
    R_PMA_HWA_ID_IMP_CORE3,         /*!< IMP3 */
    R_PMA_HWA_ID_IMP_OCV_CORE0,     /*!< IMP-OCV0 */
    R_PMA_HWA_ID_IMP_OCV_CORE1,     /*!< IMP-OCV1 */
    R_PMA_HWA_ID_IMP_OCV_CORE2,     /*!< IMP-OCV2 */
    R_PMA_HWA_ID_IMP_OCV_CORE3,     /*!< IMP-OCV3 */
    R_PMA_HWA_ID_IMP_OCV_CORE4,     /*!< IMP-OCV4 */
    R_PMA_HWA_ID_IMP_OCV_CORE5,     /*!< IMP-OCV5 */
    R_PMA_HWA_ID_IMP_OCV_CORE6,     /*!< IMP-OCV6 */
    R_PMA_HWA_ID_IMP_OCV_CORE7,     /*!< IMP-OCV7 */
    R_PMA_HWA_ID_IMP_DMAC0,         /*!< IMP DMA0 */
    R_PMA_HWA_ID_IMP_DMAC1,         /*!< IMP DMA1 */
    R_PMA_HWA_ID_IMP_DMAC_SLIM0,    /*!< IMP DMAC SLIM0 */
    R_PMA_HWA_ID_IMP_PSC0,          /*!< IMP PSC0 */
    R_PMA_HWA_ID_IMP_PSC1,          /*!< IMP PSC1 */
    R_PMA_HWA_ID_IMP_CNN0,          /*!< IMP CNN0 */
    R_PMA_HWA_ID_IMP_CNN1,          /*!< IMP CNN1 */
    R_PMA_HWA_ID_IMP_CNN2,          /*!< IMP CNN2 */
    R_PMA_HWA_ID_IMP_SLIM0,         /*!< IMP SLIM0 */
    R_PMA_HWA_ID_IMP_SPM,           /*!< IMP SPM */
    R_PMA_HWA_ID_IMP_SPMI0,         /*!< IMP RAM0 */
    R_PMA_HWA_ID_IMP_SPMI1,         /*!< IMP RAM1 */
    R_PMA_HWA_ID_IMP_SPMC0,         /*!< IMP CNN RAM0 */
    R_PMA_HWA_ID_IMP_SPMC1,         /*!< IMP CNN RAM1 */
    R_PMA_HWA_ID_IMP_SPMC2,         /*!< IMP CNN RAM2 */
    R_PMA_HWA_ID_IMP_DTA,           /*!< IMP DTA */
    R_PMA_HWA_ID_IMP_RADSP0,        /*!< IMP RADSP0 */
    R_PMA_HWA_ID_IMP_RADSP1,        /*!< IMP RADSP1 */
    R_PMA_HWA_ID_IMP_RADSP_DMAC0,   /*!< IMP RADSP DMAC0 */
    R_PMA_HWA_ID_IMP_RADSP_DMAC1,   /*!< IMP RADSP DMAC1 */
    R_PMA_HWA_ID_VIP_UMFL0,         /*!< UMFL0  (DOF) */
    R_PMA_HWA_ID_VIP_UMFL1,         /*!< UMFL1  (DOF) */
    R_PMA_HWA_ID_VIP_DISP0,         /*!< DISP0 (STV) */
    R_PMA_HWA_ID_VIP_DISP1,         /*!< DISP1 (STV) */
    R_PMA_HWA_ID_VIP_ACF0,          /*!< ACF0(CLE0) */
    R_PMA_HWA_ID_VIP_ACF1,          /*!< ACF1(CLE1) */
    R_PMA_HWA_ID_VIP_ACF2,          /*!< ACF2(CLE2) */
    R_PMA_HWA_ID_VIP_ACF3,          /*!< ACF3(CLE3) */
    R_PMA_HWA_ID_VIP_ACF4,          /*!< ACF3(CLE4) */
    R_PMA_HWA_ID_ICCOM,             /*!< ICCOM */
    R_PMA_HWA_ID_VCP_VCP4,          /*!< VCP4 */
    R_PMA_HWA_ID_VCP_FCPC,          /*!< FCPC */
    R_PMA_HWA_ID_VCP_IVCP1E,        /*!< iVCP1E */
    R_PMA_HWA_ID_ISP_CORE0,         /*!< ISP Channel 0 */
    R_PMA_HWA_ID_ISP_CORE1,         /*!< ISP Channel 1 */
    R_PMA_HWA_ID_ISP_CORE2,         /*!< ISP Channel 2 */
    R_PMA_HWA_ID_ISP_CORE3,         /*!< ISP Channel 3 */
    R_PMA_HWA_ID_ISP_TISP0,         /*!< ISP TISP0 */
    R_PMA_HWA_ID_ISP_TISP1,         /*!< ISP TISP1 */
    R_PMA_HWA_ID_ISP_TISP2,         /*!< ISP TISP2 */
    R_PMA_HWA_ID_ISP_TISP3,         /*!< ISP TISP3 */
    R_PMA_HWA_ID_ISP_VSPX0,         /*!< ISP VSPX0 */
    R_PMA_HWA_ID_ISP_VSPX1,         /*!< ISP VSPX1 */
    R_PMA_HWA_ID_ISP_VSPX2,         /*!< ISP VSPX2 */
    R_PMA_HWA_ID_ISP_VSPX3,         /*!< ISP VSPX3 */
    R_PMA_HWA_ID_IMR_LX4_CH0,       /*!< IMR-LX4 ch0 */
    R_PMA_HWA_ID_IMR_LX4_CH1,       /*!< IMR-LX4 ch1 */
    R_PMA_HWA_ID_IMR_LX4_CH2,       /*!< IMR-LX4 ch2 */
    R_PMA_HWA_ID_IMR_LX4_CH3,       /*!< IMR-LX4 ch3 */
    R_PMA_HWA_ID_IMR_LX4_CH4,       /*!< IMR-LX4 ch4 */
    R_PMA_HWA_ID_IMR_LX4_CH5,       /*!< IMR-LX4 ch5 */
    R_PMA_HWA_ID_FBC,               /*!< FBC */
    R_PMA_HWA_ID_FBA_IMS0,          /*!< FBA IMS0 */
    R_PMA_HWA_ID_FBA_IMS1,          /*!< FBA IMS1 */
    R_PMA_HWA_ID_FBA_IMS2,          /*!< FBA IMS1 */
    R_PMA_HWA_ID_FBA_IMS3,          /*!< FBA IMS1 */
    R_PMA_HWA_ID_FBA_IMR0,          /*!< FBA IMR0 */
    R_PMA_HWA_ID_FBA_IMR1,          /*!< FBA IMR1 */
    R_PMA_HWA_ID_FBA_IMR2,          /*!< FBA IMR2 */
    R_PMA_HWA_ID_FBA_IMR3,          /*!< FBA IMR3 */
    R_PMA_HWA_ID_FBA_IMP0,          /*!< FBA IMP0 */
    R_PMA_HWA_ID_FBA_IMP1,          /*!< FBA IMP1 */
    R_PMA_HWA_ID_FBA_IMP2,          /*!< FBA IMP2 */
    R_PMA_HWA_ID_FBA_IMP3,          /*!< FBA IMP3 */
    R_PMA_HWA_ID_FBA_IMP4,          /*!< FBA IMP3 */
    R_PMA_HWA_ID_FBA_OCV0,          /*!< FBA IMP OCV0 */
    R_PMA_HWA_ID_FBA_OCV1,          /*!< FBA IMP OCV1 */
    R_PMA_HWA_ID_FBA_OCV2,          /*!< FBA IMP OCV2 */
    R_PMA_HWA_ID_FBA_OCV3,          /*!< FBA IMP OCV3 */
    R_PMA_HWA_ID_FBA_OCV4,          /*!< FBA IMP OCV4 */
    R_PMA_HWA_ID_FBA_OCV5,          /*!< FBA IMP OCV5 */
    R_PMA_HWA_ID_FBA_OCV6,          /*!< FBA IMP OCV6 */
    R_PMA_HWA_ID_FBA_OCV7,          /*!< FBA IMP OCV7 */
    R_PMA_HWA_ID_FBA_DP0,           /*!< FBA IMP DP0 */
    R_PMA_HWA_ID_FBA_DP1,           /*!< FBA IMP DP1 */
    R_PMA_HWA_ID_FBA_CNN0,          /*!< FBA IMP CNN0 MAIN */
    R_PMA_HWA_ID_FBA_CNN0_SUB0,     /*!< FBA IMP CNN0 SUB0 */
    R_PMA_HWA_ID_FBA_CNN0_SUB1,     /*!< FBA IMP CNN0 SUB1 */
    R_PMA_HWA_ID_FBA_CNN0_SUB2,     /*!< FBA IMP CNN0 SUB2 */
    R_PMA_HWA_ID_FBA_CNN0_SUB3,     /*!< FBA IMP CNN0 SUB3 */
    R_PMA_HWA_ID_FBA_CNN1,          /*!< FBA IMP CNN1 MAIN */
    R_PMA_HWA_ID_FBA_CNN1_SUB0,     /*!< FBA IMP CNN1 SUB0 */
    R_PMA_HWA_ID_FBA_CNN1_SUB1,     /*!< FBA IMP CNN1 SUB1 */
    R_PMA_HWA_ID_FBA_CNN1_SUB2,     /*!< FBA IMP CNN1 SUB2 */
    R_PMA_HWA_ID_FBA_CNN1_SUB3,     /*!< FBA IMP CNN1 SUB3 */
    R_PMA_HWA_ID_FBA_CNN2,          /*!< FBA IMP CNN2 MAIN */
    R_PMA_HWA_ID_FBA_CNN2_SUB0,     /*!< FBA IMP CNN2 SUB0 */
    R_PMA_HWA_ID_FBA_CNN2_SUB1,     /*!< FBA IMP CNN2 SUB1 */
    R_PMA_HWA_ID_FBA_CNN2_SUB2,     /*!< FBA IMP CNN2 SUB2 */
    R_PMA_HWA_ID_FBA_CNN2_SUB3,     /*!< FBA IMP CNN2 SUB3 */
    R_PMA_HWA_ID_FBA_DSP0,          /*!< FBA IMP DSP0 */
    R_PMA_HWA_ID_FBA_DSP1,          /*!< FBA IMP DSP1 */
    R_PMA_HWA_ID_FBA_CNRAM0,        /*!< FBA IMP CNRAM0 */
    R_PMA_HWA_ID_FBA_CNRAM1,        /*!< FBA IMP CNRAM1 */
    R_PMA_HWA_ID_FBA_CNRAM2,        /*!< FBA IMP CNRAM2 */
    R_PMA_HWA_ID_FBA_SLIM0,         /*!< FBA SLIM0 */
    R_PMA_HWA_ID_FBA_DOF0,          /*!< FBA DOF0 */
    R_PMA_HWA_ID_FBA_DOF1,          /*!< FBA DOF1 */
    R_PMA_HWA_ID_FBA_STV0,          /*!< FBA STV0 */
    R_PMA_HWA_ID_FBA_STV1,          /*!< FBA STV1 */
    R_PMA_HWA_ID_FBA_ACF0,          /*!< FBA ACF0 */
    R_PMA_HWA_ID_FBA_ACF1,          /*!< FBA ACF1 */
    R_PMA_HWA_ID_FBA_SMPS0,         /*!< FBA SMPS0 */
    R_PMA_HWA_ID_FBA_SMPS1,         /*!< FBA SMPS1 */
    R_PMA_HWA_ID_FBA_SMES0,         /*!< FBA SMES0 */
    R_PMA_HWA_ID_FBA_SMES1,         /*!< FBA SMES1 */
    R_PMA_HWA_ID_FBA_SMPO0,         /*!< FBA SMPO0 */
    R_PMA_HWA_ID_FBA_SMPO1,         /*!< FBA SMPO1 */
    R_PMA_HWA_ID_FBA_ISP0,          /*!< FBA ISP0 */
    R_PMA_HWA_ID_FBA_ISP1,          /*!< FBA ISP1 */
    R_PMA_HWA_ID_FBA_ISP2,          /*!< FBA ISP2 */
    R_PMA_HWA_ID_FBA_ISP3,          /*!< FBA ISP3 */
    R_PMA_HWA_ID_FBA_CL0,           /*!< FBA CA76 CL0 */
    R_PMA_HWA_ID_FBA_CL0_CPU0,      /*!< FBA CA76 CL0 CPU0 */
    R_PMA_HWA_ID_FBA_CL0_CPU1,      /*!< FBA CA76 CL0 CPU1 */
    R_PMA_HWA_ID_FBA_CL1,           /*!< FBA CA76 CL1 */
    R_PMA_HWA_ID_FBA_CL1_CPU2,      /*!< FBA CA76 CL1 CPU2 */
    R_PMA_HWA_ID_FBA_CL1_CPU3,      /*!< FBA CA76 CL1 CPU3 */
    R_PMA_HWA_ID_FBA_CL2,           /*!< FBA CA76 CL2 */
    R_PMA_HWA_ID_FBA_CL2_CPU4,      /*!< FBA CA76 CL2 CPU4 */
    R_PMA_HWA_ID_FBA_CL2_CPU5,      /*!< FBA CA76 CL2 CPU5 */
    R_PMA_HWA_ID_FBA_CL3,           /*!< FBA CA76 CL3 */
    R_PMA_HWA_ID_FBA_CL3_CPU6,      /*!< FBA CA76 CL3 CPU6 */
    R_PMA_HWA_ID_FBA_CL3_CPU7,      /*!< FBA CA76 CL3 CPU7 */
    R_PMA_HWA_ID_RFSO0,             /*!< RFSO0 */
    R_PMA_HWA_ID_RFSO1,             /*!< RFSO1 */
    R_PMA_HWA_ID_RFSO2,             /*!< RFSO2 */
    R_PMA_HWA_ID_RFSO3,             /*!< RFSO3 */
    R_PMA_HWA_ID_RFSO4,             /*!< RFSO4 */
    R_PMA_HWA_ID_RFSO5,             /*!< RFSO5 */
    R_PMA_HWA_ID_RFSO6,             /*!< RFSO6 */
    R_PMA_HWA_ID_RFSO7,             /*!< RFSO7 */
    R_PMA_HWA_ID_RFSO8,             /*!< RFSO8 */
    R_PMA_HWA_ID_RFSO9,             /*!< RFSO9 */
    R_PMA_HWA_ID_RFSO10,            /*!< RFSO10 */

    R_PMA_HWA_ID_IVDP1C,
    R_PMA_HWA_ID_FCPC,
    R_PMA_HWA_ID_DHD,
    R_PMA_HWA_ID_DU0,
    R_PMA_HWA_ID_DU1,
    R_PMA_HWA_ID_DU2,
    R_PMA_HWA_ID_DU3,
    R_PMA_HWA_ID_VSPD0,
    R_PMA_HWA_ID_VSPD1,
    R_PMA_HWA_ID_VSPD2,
    R_PMA_HWA_ID_VIN0,
    R_PMA_HWA_ID_VIN1,
    R_PMA_HWA_ID_VIN2,
    R_PMA_HWA_ID_VIN3,
    R_PMA_HWA_ID_VIN4,
    R_PMA_HWA_ID_VIN5,
    R_PMA_HWA_ID_VIN6,
    R_PMA_HWA_ID_VIN7,
    R_PMA_HWA_ID_CSI20,
    R_PMA_HWA_ID_CSI40,
    R_PMA_HWA_ID_CSI41,
    R_PMA_HWA_ID_DOC0,
    R_PMA_HWA_ID_DOC1,
    R_PMA_HWA_ID_HDMI0,
    R_PMA_HWA_ID_HDMI1,
    R_PMA_HWA_ID_LVDS,
    R_PMA_HWA_ID_CANFD,
    R_PMA_HWA_ID_ETHERNET,
    R_PMA_HWA_ID_RTDMAC0,
    R_PMA_HWA_ID_RTDMAC1,
    R_PMA_HWA_ID_NUM
} e_pma_hwa_id_t;

/* Covers: V3X_XOS1_PMA_UD_EX_ENUM002 */

/*******************************************************************************************************************//**
 * @enum e_pma_hwa_req_t
 * Requirement of Hardware accelerator.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM003 */
typedef enum
{
    R_PMA_HWA_RELEASED, /*!< release HWA */
    R_PMA_HWA_REQUESTED /*!< request HWA */
} e_pma_hwa_req_t;
/* Covers: V3X_XOS1_PMA_UD_EX_ENUM003 */

/*******************************************************************************************************************//**
 * @enum e_pma_power_policy_t
 * Power policy.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM004 */
typedef enum
{
    R_PMA_PG_MODE,     /*!< power gating */
    R_PMA_CG_MODE,     /*!< clock gating */
    R_PMA_HP_MODE      /*!< high performance */
} e_pma_power_policy_t;
/* Covers: V3X_XOS1_PMA_UD_EX_ENUM004 */

/*******************************************************************************************************************//**
 * @enum e_pma_reset_req_t
 * Requirement of software reset.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM005 */
typedef enum
{
    R_PMA_RELEASED_RESET, /*!< release software reset */
    R_PMA_APPLIED_RESET   /*!< apply software reset */
} e_pma_reset_req_t;
/* Covers: V3X_XOS1_PMA_UD_EX_ENUM005 */

/*******************************************************************************************************************//**
 * @enum e_pma_onoff_t
 * Status of power and clock.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM006 */
typedef enum
{
    R_PMA_STATE_OFF, /*!< not supplied */
    R_PMA_STATE_ON   /*!< supplied */
} e_pma_onoff_t;
/* Covers: V3X_XOS1_PMA_UD_EX_ENUM006 */

/*******************************************************************************************************************//**
 * @enum e_pma_cfg_reg_chk_result_t
 * Result of Configuration Register Check.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM007 */
typedef enum
{
    R_PMA_CFG_CHK_SUCCESS,         /*!< OK */
    R_PMA_CFG_CHK_ERROR_TRANSIENT, /*!< transient fault */
    R_PMA_CFG_CHK_ERROR_PERMANENT  /*!< permanent fault */
} e_pma_cfg_reg_chk_result_t;
/* Covers: V3X_XOS1_PMA_UD_EX_ENUM007 */

/*******************************************************************************************************************//**
 * @enum e_pma_reset_monitor_result_t
 * Result of Reset status monitor for each module.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_ENUM008 */
typedef enum
{
    R_PMA_RST_MON_CHK_SUCCESS, /*!< OK */
    R_PMA_RST_MON_CHK_ERROR    /*!< reset status monitor NG */
} e_pma_reset_monitor_result_t;
/* Covers: V3X_XOS1_PMA_UD_EX_ENUM008 */

/*******************************************************************************************************************//**
 * @struct st_pma_hwa_state_t
 * Status of hardware accelerator.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_STR002 */
typedef struct
{
    e_pma_hwa_req_t enReqStat;       /*!< user's requirement of HWA */
    e_pma_onoff_t   enActualPwrStat; /*!< actual status of SYSC register */
    e_pma_onoff_t   enActualClkStat; /*!< actual status of MSTP register */
} st_pma_hwa_state_t;
/* Covers: V3X_XOS1_PMA_UD_EX_DT003 */

/*******************************************************************************************************************//**
 * @struct st_pma_config_t
 * Configuration of PMA.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_STR003 */
typedef struct
{
    uint32_t task_priority;            /*!< Priority of PMA task */
    uint32_t timeout_ms;               /*!< wait time[ms] for return of OS API */
    uint32_t polling_max_num;          /*!< the number of times to check register */
    uint32_t loop_num_for_rclk_cycle;  /*!< the number of busy loop to wait for RCLK 1cycle until a software reset completion */
} st_pma_config_t;
/* Covers: V3X_XOS1_PMA_UD_EX_DT004 */

/*******************************************************************************************************************//**
 * @struct st_pma_handle_t
 * Handle of PMA.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_STR001 */
typedef struct st_pma_user_info_t * st_pma_handle_t;
/* Covers: V3X_XOS1_PMA_UD_EX_DT001 */

/*******************************************************************************************************************//**
 * @typedef R_PMA_CB 
 * Callback function.
***********************************************************************************************************************/
/* Traceability ID: V3X_XOS1_PMA_CD_IF_H01_DEF001 */
typedef void (* R_PMA_CB)(void);
/* Covers: V3X_XOS1_PMA_UD_EX_DT002 */

/** @} */

/*******************************************************************************************************************//**
 * @ingroup PMA_API
 * @defgroup PMA_Function Function Definitions
 *
 * @{
 **********************************************************************************************************************/
/*======================================================================================================================
Public global functions
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Get a handle and Initialize PMA.
* @param[in]        pConfig   PMA configuration parameter.
* @param[out]       pHandle   PMA handle.
* @return           Return value
* @retval           R_PMA_RESULT_OK                Success
* @retval           R_PMA_ERR_RESULT_NG            Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT     Arguments is invalid
* @retval           R_PMA_ERR_NO_VACANT_HANDLE     All Handles are already used
* @retval           R_PMA_ERR_RESET_TOGGLE_CHK_NG  Initial toggle check fail
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_Init(st_pma_handle_t *pHandle, const st_pma_config_t *pConfig)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Release the handle.
* @param[in]        handle   PMA handle.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_Quit(st_pma_handle_t handle)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Occupy the specified HWA.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
* @retval           R_PMA_ERR_ALREADY_IN_USE    Another user already occupies the HWA
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_GetLockHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Release the lock of the specified HWA.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_UnlockHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Switch status of power and clock of the specified HWA to ON.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
* @retval           R_PMA_ERR_BUS_IF_CHK_NG     Internal bus interface check fail
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_RequestHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Switch status of power and clock of the specified HWA according to power policy.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
* @retval           R_PMA_ERR_BUS_IF_CHK_NG     Internal bus interface check fail
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_ReleaseHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Get status of power and clock of the specified HWA from register.
* @param[in]        handle      PMA handle.
* @param[in]        hwaId       HWA identifier.
* @param[out]       pHwaState   HWA status.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_GetHwaState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Set power policy to the specified HWA.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @param[in]        policy   power policy.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_SetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Get power policy of the specified HWA.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @param[out]       pPolicy  power policy.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_GetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Execute a software reset to the specified HWA.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
* @retval           R_PMA_ERR_RESET_CHK_NG      reset monitor check fail
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_ApplyResetHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Release the specified HWA from a software reset.
* @param[in]        handle   PMA handle.
* @param[in]        hwaId    HWA identifier.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_ReleaseResetHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Get status of software reset to the specified HWA.
* @param[in]        handle     PMA handle.
* @param[in]        hwaId      HWA identifier.
* @param[out]       pRstStat   Software reset status.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_GetReset(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstStat)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Execute configuration register check.
* @param[in]        handle    PMA handle.
* @param[out]       pResult   Result of configuration register check.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_ConfigurationRegisterCheck(st_pma_handle_t handle, e_pma_cfg_reg_chk_result_t *pResult)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Set callback for internal bus interface chaek.
* @param[in]        handle     PMA handle.
* @param[in]        callback   callback function.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_InternalBusInterfaceCheck(st_pma_handle_t handle, R_PMA_CB callback)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Execute reset monitor check.
* @param[in]        handle    PMA handle.
* @param[out]       pResult   Result of reset monitor check.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_ResetStatusMonitor(st_pma_handle_t handle, e_pma_reset_monitor_result_t *pResult)
{
	return R_PMA_ERR_RESULT_NG;
}

/*******************************************************************************************************************//**
* @brief            Set callback for reset monitor check.
* @param[in]        handle     PMA handle.
* @param[in]        callback   Callback function.
* @return           Return value
* @retval           R_PMA_RESULT_OK             Success
* @retval           R_PMA_ERR_RESULT_NG         Fail
* @retval           R_PMA_ERR_INVALID_ARGUMENT  Arguments is invalid
* @retval           R_PMA_ERR_INVALID_HANDLE    Handle is invalid
***********************************************************************************************************************/
static inline e_pma_return_t R_PMA_SetResetStatusMonitorErrCallback(st_pma_handle_t handle, R_PMA_CB callback)
{
	return R_PMA_ERR_RESULT_NG;
}

/** @} */

#ifdef __cplusplus
}
#endif
#endif /* R_PMA_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
