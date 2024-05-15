/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_configuration.c
* Version :      0.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Configurator for specific OS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal_configuration/target/zephyr/r_osal_configuration.h"

#include <device.h>
#include <devicetree.h>

/*======================================================================================================================
 Private macro definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @def DT_MACRO
 * @brief DTS Macro
***********************************************************************************************************************/
#define DT_ADDR(node_id) COND_CODE_1(DT_NODE_HAS_PROP(node_id, reg), \
    (DT_REG_ADDR(node_id)),(0x00000000))

#define DT_SIZE(node_id) COND_CODE_1(DT_NODE_HAS_PROP(node_id, reg), \
    (DT_REG_SIZE(node_id)),(0x0000))

#define NODE_PROPERTIES(node_id) { DT_LABEL(node_id), DT_ADDR(node_id), DT_SIZE(node_id) },

/***********************************************************************************************************************
 * @def RESORCE_ID
 * @brief Resource IDs
***********************************************************************************************************************/
/* PRQA S 3214 1 # Do not change because the user is a public source. */
#define RESORCE_ID_RESERVE  0x0000U
#define RESORCE_ID_IMR      0x1000U
#define RESORCE_ID_VIP      0x2000U
#define RESORCE_ID_ICCOM    0x3000U
#define RESORCE_ID_IMP      0x5000U
#define RESORCE_ID_ISP      0x6000U
#define RESORCE_ID_CPURTT   0x7000U
#define RESORCE_ID_HWARTT   0xa000U
#define RESORCE_ID_OSAL     0xf000U

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_ID
 * @brief Thread IDs.
***********************************************************************************************************************/
#define    OSAL_THREAD_ICCOM_DEMO_TASK               (RESORCE_ID_ICCOM +  0U)
#define    OSAL_THREAD_ICCOM_MGR_TASK                (RESORCE_ID_ICCOM +  1U)
#define    OSAL_THREAD_ICCOM_TIMER_TASK              (RESORCE_ID_ICCOM +  2U)
#define    OSAL_THREAD_ICCOM_RCV_TASK0               (RESORCE_ID_ICCOM +  3U)
#define    OSAL_THREAD_ICCOM_RCV_TASK1               (RESORCE_ID_ICCOM +  4U)
#define    OSAL_THREAD_ICCOM_RCV_TASK2               (RESORCE_ID_ICCOM +  5U)
#define    OSAL_THREAD_ICCOM_RCV_TASK3               (RESORCE_ID_ICCOM +  6U)
#define    OSAL_THREAD_ICCOM_RCV_TASK4               (RESORCE_ID_ICCOM +  7U)
#define    OSAL_THREAD_ICCOM_RCV_TASK5               (RESORCE_ID_ICCOM +  8U)
#define    OSAL_THREAD_ICCOM_RCV_TASK6               (RESORCE_ID_ICCOM +  9U)
#define    OSAL_THREAD_ICCOM_RCV_TASK7               (RESORCE_ID_ICCOM + 10U)
#define    OSAL_THREAD_ICCOM_READ_TASK0              (RESORCE_ID_ICCOM + 11U)
#define    OSAL_THREAD_ICCOM_READ_TASK1              (RESORCE_ID_ICCOM + 12U)
#define    OSAL_THREAD_ICCOM_READ_TASK2              (RESORCE_ID_ICCOM + 13U)
#define    OSAL_THREAD_ICCOM_READ_TASK3              (RESORCE_ID_ICCOM + 14U)
#define    OSAL_THREAD_ICCOM_READ_TASK4              (RESORCE_ID_ICCOM + 15U)
#define    OSAL_THREAD_ICCOM_READ_TASK5              (RESORCE_ID_ICCOM + 16U)
#define    OSAL_THREAD_ICCOM_READ_TASK6              (RESORCE_ID_ICCOM + 17U)
#define    OSAL_THREAD_ICCOM_READ_TASK7              (RESORCE_ID_ICCOM + 18U)
#define    OSAL_THREAD_ICCOM_SEND_TASK0              (RESORCE_ID_ICCOM + 19U)
#define    OSAL_THREAD_ICCOM_SEND_TASK1              (RESORCE_ID_ICCOM + 20U)
#define    OSAL_THREAD_ICCOM_SEND_TASK2              (RESORCE_ID_ICCOM + 21U)
#define    OSAL_THREAD_ICCOM_SEND_TASK3              (RESORCE_ID_ICCOM + 22U)
#define    OSAL_THREAD_ICCOM_SEND_TASK4              (RESORCE_ID_ICCOM + 23U)
#define    OSAL_THREAD_ICCOM_SEND_TASK5              (RESORCE_ID_ICCOM + 24U)
#define    OSAL_THREAD_ICCOM_SEND_TASK6              (RESORCE_ID_ICCOM + 25U)
#define    OSAL_THREAD_ICCOM_SEND_TASK7              (RESORCE_ID_ICCOM + 26U)

#define    OSAL_THREAD_IMP_RTT_DRV                   (RESORCE_ID_IMP   +  0U)
#define    OSAL_THREAD_IMP_DRV                       (RESORCE_ID_IMP   +  1U)
#define    OSAL_THREAD_IMPFW_TASK00                  (RESORCE_ID_IMP   +  2U)
#define    OSAL_THREAD_IMPFW_TASK01                  (RESORCE_ID_IMP   +  3U)
#define    OSAL_THREAD_IMPFW_TASK02                  (RESORCE_ID_IMP   +  4U)
#define    OSAL_THREAD_IMPFW_TASK03                  (RESORCE_ID_IMP   +  5U)
#define    OSAL_THREAD_IMPFW_TASK04                  (RESORCE_ID_IMP   +  6U)
#define    OSAL_THREAD_IMPFW_TASK05                  (RESORCE_ID_IMP   +  7U)
#define    OSAL_THREAD_IMPFW_TASK06                  (RESORCE_ID_IMP   +  8U)
#define    OSAL_THREAD_IMPFW_APP_TASK00              (RESORCE_ID_IMP   +  9U)
#define    OSAL_THREAD_IMPFW_APP_TASK01              (RESORCE_ID_IMP   + 10U)
#define    OSAL_THREAD_IMPFW_APP_TASK02              (RESORCE_ID_IMP   + 11U)
#define    OSAL_THREAD_IMPFW_APP_TASK03              (RESORCE_ID_IMP   + 12U)
#define    OSAL_THREAD_IMPFW_APP_TASK04              (RESORCE_ID_IMP   + 13U)
#define    OSAL_THREAD_IMPFW_APP_TASK05              (RESORCE_ID_IMP   + 14U)
#define    OSAL_THREAD_IMPFW_APP_TASK06              (RESORCE_ID_IMP   + 15U)

#define    OSAL_THREAD_CPURTT_CB_TASK                (RESORCE_ID_CPURTT +  0U)
#define    OSAL_THREAD_CPURTT_FBIST_TASK             (RESORCE_ID_CPURTT +  1U)
#define    OSAL_THREAD_CPURTT_A1_TASK                (RESORCE_ID_CPURTT +  2U)
#define    OSAL_THREAD_CPURTT_A21_TASK               (RESORCE_ID_CPURTT +  3U)
#define    OSAL_THREAD_CPURTT_A22_TASK               (RESORCE_ID_CPURTT +  4U)
#define    OSAL_THREAD_CPURTT_A23_TASK               (RESORCE_ID_CPURTT +  5U)
#define    OSAL_THREAD_CPURTT_A24_TASK               (RESORCE_ID_CPURTT +  6U)
#define    OSAL_THREAD_CPURTT_TP_TASK1               (RESORCE_ID_CPURTT +  7U)
#define    OSAL_THREAD_CPURTT_TP_TASK2               (RESORCE_ID_CPURTT +  8U)
#define    OSAL_THREAD_FBIST_TP_TASK1                (RESORCE_ID_CPURTT +  9U)
#define    OSAL_THREAD_FBIST_TP_TASK2                (RESORCE_ID_CPURTT + 10U)
#define    OSAL_THREAD_RFSO_TP_TASK1                 (RESORCE_ID_CPURTT + 11U)
#define    OSAL_THREAD_RFSO_TP_TASK2                 (RESORCE_ID_CPURTT + 12U)

#define    OSAL_THREAD_IMR_TASK00                    (RESORCE_ID_IMR   +  0U)
#define    OSAL_THREAD_IMR_TASK01                    (RESORCE_ID_IMR   +  1U)
#define    OSAL_THREAD_IMR_TASK02                    (RESORCE_ID_IMR   +  2U)
#define    OSAL_THREAD_IMR_TASK03                    (RESORCE_ID_IMR   +  3U)
#define    OSAL_THREAD_IMR_TASK04                    (RESORCE_ID_IMR   +  4U)
#define    OSAL_THREAD_IMR_TASK05                    (RESORCE_ID_IMR   +  5U)

#define    OSAL_THREAD_VIP_TASK00                    (RESORCE_ID_VIP   +  0U)
#define    OSAL_THREAD_VIP_TASK01                    (RESORCE_ID_VIP   +  1U)
#define    OSAL_THREAD_VIP_TASK02                    (RESORCE_ID_VIP   +  2U)
#define    OSAL_THREAD_VIP_TASK03                    (RESORCE_ID_VIP   +  3U)
#define    OSAL_THREAD_VIP_TASK04                    (RESORCE_ID_VIP   +  4U)

#define    OSAL_THREAD_ISP_TASK00                    (RESORCE_ID_ISP   +  0U)
#define    OSAL_THREAD_ISP_TASK01                    (RESORCE_ID_ISP   +  1U)
#define    OSAL_THREAD_ISP_TASK02                    (RESORCE_ID_ISP   +  2U)
#define    OSAL_THREAD_ISP_TASK03                    (RESORCE_ID_ISP   +  3U)

#define    OSAL_THREAD_OSAL_00                       (RESORCE_ID_OSAL  +  0U)
#define    OSAL_THREAD_OSAL_01                       (RESORCE_ID_OSAL  +  1U)
#define    OSAL_THREAD_OSAL_02                       (RESORCE_ID_OSAL  +  2U)
#define    OSAL_THREAD_OSAL_03                       (RESORCE_ID_OSAL  +  3U)
#define    OSAL_THREAD_OSAL_04                       (RESORCE_ID_OSAL  +  4U)
#define    OSAL_THREAD_OSAL_05                       (RESORCE_ID_OSAL  +  5U)
#define    OSAL_THREAD_OSAL_06                       (RESORCE_ID_OSAL  +  6U)
#define    OSAL_THREAD_OSAL_07                       (RESORCE_ID_OSAL  +  7U)
#define    OSAL_THREAD_OSAL_08                       (RESORCE_ID_OSAL  +  8U)
#define    OSAL_THREAD_OSAL_09                       (RESORCE_ID_OSAL  +  9U)

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_ID
 * @brief Mutex IDs.
***********************************************************************************************************************/
#define    OSAL_MUTEX_IMR_00                         (RESORCE_ID_IMR   +  0U)
#define    OSAL_MUTEX_IMR_01                         (RESORCE_ID_IMR   +  1U)
#define    OSAL_MUTEX_IMR_02                         (RESORCE_ID_IMR   +  2U)
#define    OSAL_MUTEX_IMR_03                         (RESORCE_ID_IMR   +  3U)
#define    OSAL_MUTEX_IMR_04                         (RESORCE_ID_IMR   +  4U)
#define    OSAL_MUTEX_IMR_05                         (RESORCE_ID_IMR   +  5U)
#define    OSAL_MUTEX_IMR_CB00                       (RESORCE_ID_IMR   +  6U)
#define    OSAL_MUTEX_IMR_CB01                       (RESORCE_ID_IMR   +  7U)
#define    OSAL_MUTEX_IMR_CB02                       (RESORCE_ID_IMR   +  8U)
#define    OSAL_MUTEX_IMR_CB03                       (RESORCE_ID_IMR   +  9U)
#define    OSAL_MUTEX_IMR_CB04                       (RESORCE_ID_IMR   + 10U)
#define    OSAL_MUTEX_IMR_CB05                       (RESORCE_ID_IMR   + 11U)

#define    OSAL_MUTEX_DOF0_00                        (RESORCE_ID_VIP   +  0U)
#define    OSAL_MUTEX_DOF0_01                        (RESORCE_ID_VIP   +  1U)
#define    OSAL_MUTEX_STV0_00                        (RESORCE_ID_VIP   +  2U)
#define    OSAL_MUTEX_STV0_01                        (RESORCE_ID_VIP   +  3U)
#define    OSAL_MUTEX_ACF0_00                        (RESORCE_ID_VIP   +  4U)
#define    OSAL_MUTEX_ACF0_01                        (RESORCE_ID_VIP   +  5U)
#define    OSAL_MUTEX_ACF1_00                        (RESORCE_ID_VIP   +  6U)
#define    OSAL_MUTEX_ACF1_01                        (RESORCE_ID_VIP   +  7U)
#define    OSAL_MUTEX_ACF2_00                        (RESORCE_ID_VIP   +  8U)
#define    OSAL_MUTEX_ACF2_01                        (RESORCE_ID_VIP   +  9U)
#define    OSAL_MUTEX_ACF3_00                        (RESORCE_ID_VIP   + 10U)
#define    OSAL_MUTEX_ACF3_01                        (RESORCE_ID_VIP   + 11U)
#define    OSAL_MUTEX_ACF4_00                        (RESORCE_ID_VIP   + 12U)
#define    OSAL_MUTEX_ACF4_01                        (RESORCE_ID_VIP   + 13U)
#define    OSAL_MUTEX_VIPRTT_00                      (RESORCE_ID_VIP   + 14U)
#define    OSAL_MUTEX_VIPRTT_01                      (RESORCE_ID_VIP   + 15U)
#define    OSAL_MUTEX_VIPRTT_02                      (RESORCE_ID_VIP   + 16U)
#define    OSAL_MUTEX_VIPRTT_03                      (RESORCE_ID_VIP   + 17U)
#define    OSAL_MUTEX_SMD_PS0_00                     (RESORCE_ID_VIP   + 18U)
#define    OSAL_MUTEX_SMD_PS0_01                     (RESORCE_ID_VIP   + 19U)
#define    OSAL_MUTEX_SMD_POST_00                    (RESORCE_ID_VIP   + 20U)
#define    OSAL_MUTEX_SMD_POST_01                    (RESORCE_ID_VIP   + 21U)

#define    OSAL_MUTEX_ICCOM_WRITE_00                 (RESORCE_ID_ICCOM +  0U)
#define    OSAL_MUTEX_ICCOM_WRITE_01                 (RESORCE_ID_ICCOM +  1U)
#define    OSAL_MUTEX_ICCOM_WRITE_02                 (RESORCE_ID_ICCOM +  2U)
#define    OSAL_MUTEX_ICCOM_WRITE_03                 (RESORCE_ID_ICCOM +  3U)
#define    OSAL_MUTEX_ICCOM_WRITE_04                 (RESORCE_ID_ICCOM +  4U)
#define    OSAL_MUTEX_ICCOM_WRITE_05                 (RESORCE_ID_ICCOM +  5U)
#define    OSAL_MUTEX_ICCOM_WRITE_06                 (RESORCE_ID_ICCOM +  6U)
#define    OSAL_MUTEX_ICCOM_WRITE_07                 (RESORCE_ID_ICCOM +  7U)
#define    OSAL_MUTEX_ICCOM_READ_00                  (RESORCE_ID_ICCOM +  8U)
#define    OSAL_MUTEX_ICCOM_READ_01                  (RESORCE_ID_ICCOM +  9U)
#define    OSAL_MUTEX_ICCOM_READ_02                  (RESORCE_ID_ICCOM + 10U)
#define    OSAL_MUTEX_ICCOM_READ_03                  (RESORCE_ID_ICCOM + 11U)
#define    OSAL_MUTEX_ICCOM_READ_04                  (RESORCE_ID_ICCOM + 12U)
#define    OSAL_MUTEX_ICCOM_READ_05                  (RESORCE_ID_ICCOM + 13U)
#define    OSAL_MUTEX_ICCOM_READ_06                  (RESORCE_ID_ICCOM + 14U)
#define    OSAL_MUTEX_ICCOM_READ_07                  (RESORCE_ID_ICCOM + 15U)
#define    OSAL_MUTEX_ICCOM_TIMER                    (RESORCE_ID_ICCOM + 16U)
#define    OSAL_MUTEX_ICCOM_OPEN                     (RESORCE_ID_ICCOM + 17U)

#define    OSAL_MUTEX_IMP_FW_LOCK                    (RESORCE_ID_IMP   +  0U)
#define    OSAL_MUTEX_IMP_RTT_LOCK                   (RESORCE_ID_IMP   +  1U)
#define    OSAL_MUTEX_IMP_DRV_LOCK                   (RESORCE_ID_IMP   +  2U)
#define    OSAL_MUTEX_IMP_MEMORY_BARRIER_0           (RESORCE_ID_IMP   +  3U)
#define    OSAL_MUTEX_IMP_MEMORY_BARRIER_1           (RESORCE_ID_IMP   +  4U)
#define    OSAL_MUTEX_IMP_MEMORY_BARRIER_2           (RESORCE_ID_IMP   +  5U)
#define    OSAL_MUTEX_IMP_MEMORY_BARRIER_3           (RESORCE_ID_IMP   +  6U)
#define    OSAL_MUTEX_IMP_MEMORY_BARRIER_4           (RESORCE_ID_IMP   +  7U)
#define    OSAL_MUTEX_IMP_MEMORY_BARRIER_5           (RESORCE_ID_IMP   +  8U)
#define    OSAL_MUTEX_IMP_MEMORY_BARRIER_6           (RESORCE_ID_IMP   +  9U)
#define    OSAL_MUTEX_IMP_CORE_STATE_0               (RESORCE_ID_IMP   + 10U)
#define    OSAL_MUTEX_IMP_CORE_STATE_1               (RESORCE_ID_IMP   + 11U)
#define    OSAL_MUTEX_IMP_CORE_STATE_2               (RESORCE_ID_IMP   + 12U)
#define    OSAL_MUTEX_IMP_CORE_STATE_3               (RESORCE_ID_IMP   + 13U)
#define    OSAL_MUTEX_IMP_CORE_STATE_4               (RESORCE_ID_IMP   + 14U)
#define    OSAL_MUTEX_IMP_CORE_STATE_5               (RESORCE_ID_IMP   + 15U)
#define    OSAL_MUTEX_IMP_CORE_STATE_6               (RESORCE_ID_IMP   + 16U)
#define    OSAL_MUTEX_IMP_ATTR_STATE_0               (RESORCE_ID_IMP   + 17U)
#define    OSAL_MUTEX_IMP_ATTR_STATE_1               (RESORCE_ID_IMP   + 18U)
#define    OSAL_MUTEX_IMP_ATTR_STATE_2               (RESORCE_ID_IMP   + 19U)
#define    OSAL_MUTEX_IMP_ATTR_STATE_3               (RESORCE_ID_IMP   + 20U)
#define    OSAL_MUTEX_IMP_ATTR_STATE_4               (RESORCE_ID_IMP   + 21U)
#define    OSAL_MUTEX_IMP_ATTR_STATE_5               (RESORCE_ID_IMP   + 22U)
#define    OSAL_MUTEX_IMP_ATTR_STATE_6               (RESORCE_ID_IMP   + 23U)
#define    OSAL_MUTEX_IMP_IMPFW_API_0                (RESORCE_ID_IMP   + 24U)
#define    OSAL_MUTEX_IMP_IMPFW_API_1                (RESORCE_ID_IMP   + 25U)
#define    OSAL_MUTEX_IMP_IMPFW_API_2                (RESORCE_ID_IMP   + 26U)
#define    OSAL_MUTEX_IMP_IMPFW_API_3                (RESORCE_ID_IMP   + 27U)
#define    OSAL_MUTEX_IMP_IMPFW_API_4                (RESORCE_ID_IMP   + 28U)
#define    OSAL_MUTEX_IMP_IMPFW_API_5                (RESORCE_ID_IMP   + 29U)
#define    OSAL_MUTEX_IMP_IMPFW_API_6                (RESORCE_ID_IMP   + 30U)

#define    OSAL_MUTEX_CISP_00                        (RESORCE_ID_ISP   +  0U)
#define    OSAL_MUTEX_CISP_01                        (RESORCE_ID_ISP   +  1U)
#define    OSAL_MUTEX_TISP_00                        (RESORCE_ID_ISP   +  2U)
#define    OSAL_MUTEX_TISP_01                        (RESORCE_ID_ISP   +  3U)
#define    OSAL_MUTEX_CISP_02                        (RESORCE_ID_ISP   +  4U)
#define    OSAL_MUTEX_CISP_03                        (RESORCE_ID_ISP   +  5U)
#define    OSAL_MUTEX_TISP_02                        (RESORCE_ID_ISP   +  6U)
#define    OSAL_MUTEX_TISP_03                        (RESORCE_ID_ISP   +  7U)
#define    OSAL_MUTEX_VSPX_00                        (RESORCE_ID_ISP   +  8U)
#define    OSAL_MUTEX_VSPX_01                        (RESORCE_ID_ISP   +  9U)
#define    OSAL_MUTEX_VSPX_02                        (RESORCE_ID_ISP   + 10U)
#define    OSAL_MUTEX_VSPX_03                        (RESORCE_ID_ISP   + 11U)

#define    OSAL_MUTEX_CPURTT_00                      (RESORCE_ID_CPURTT +  0U)
#define    OSAL_MUTEX_CPURTT_01                      (RESORCE_ID_CPURTT +  1U)
#define    OSAL_MUTEX_CPURTT_02                      (RESORCE_ID_CPURTT +  2U)
#define    OSAL_MUTEX_CPURTT_03                      (RESORCE_ID_CPURTT +  3U)
#define    OSAL_MUTEX_CPURTT_04                      (RESORCE_ID_CPURTT +  4U)
#define    OSAL_MUTEX_CPURTT_05                      (RESORCE_ID_CPURTT +  5U)
#define    OSAL_MUTEX_CPURTT_06                      (RESORCE_ID_CPURTT +  6U)
#define    OSAL_MUTEX_CPURTT_07                      (RESORCE_ID_CPURTT +  7U)
#define    OSAL_MUTEX_CPURTT_08                      (RESORCE_ID_CPURTT +  8U)
#define    OSAL_MUTEX_CPURTT_09                      (RESORCE_ID_CPURTT +  9U)
#define    OSAL_MUTEX_CPURTT_10                      (RESORCE_ID_CPURTT + 10U)
#define    OSAL_MUTEX_CPURTT_11                      (RESORCE_ID_CPURTT + 11U)
#define    OSAL_MUTEX_CPURTT_12                      (RESORCE_ID_CPURTT + 12U)
#define    OSAL_MUTEX_CPURTT_13                      (RESORCE_ID_CPURTT + 13U)
#define    OSAL_MUTEX_CPURTT_14                      (RESORCE_ID_CPURTT + 14U)
#define    OSAL_MUTEX_CPURTT_15                      (RESORCE_ID_CPURTT + 15U)
#define    OSAL_MUTEX_CPURTT_16                      (RESORCE_ID_CPURTT + 16U)
#define    OSAL_MUTEX_CPURTT_17                      (RESORCE_ID_CPURTT + 17U)
#define    OSAL_MUTEX_CPURTT_18                      (RESORCE_ID_CPURTT + 18U)
#define    OSAL_MUTEX_CPURTT_19                      (RESORCE_ID_CPURTT + 19U)
#define    OSAL_MUTEX_CPURTT_20                      (RESORCE_ID_CPURTT + 20U)
#define    OSAL_MUTEX_CPURTT_21                      (RESORCE_ID_CPURTT + 21U)

#define    OSAL_MUTEX_OSAL_00                        (RESORCE_ID_OSAL  +  0U)
#define    OSAL_MUTEX_OSAL_01                        (RESORCE_ID_OSAL  +  1U)
#define    OSAL_MUTEX_OSAL_02                        (RESORCE_ID_OSAL  +  2U)
#define    OSAL_MUTEX_OSAL_03                        (RESORCE_ID_OSAL  +  3U)
#define    OSAL_MUTEX_OSAL_04                        (RESORCE_ID_OSAL  +  4U)
#define    OSAL_MUTEX_OSAL_05                        (RESORCE_ID_OSAL  +  5U)
#define    OSAL_MUTEX_OSAL_06                        (RESORCE_ID_OSAL  +  6U)
#define    OSAL_MUTEX_OSAL_07                        (RESORCE_ID_OSAL  +  7U)
#define    OSAL_MUTEX_OSAL_08                        (RESORCE_ID_OSAL  +  8U)
#define    OSAL_MUTEX_OSAL_09                        (RESORCE_ID_OSAL  +  9U)

/*******************************************************************************************************************//**
 * @def OSAL_COND_ID
 * @brief Condition variable IDs.
***********************************************************************************************************************/
#define    OSAL_COND_CPURTT_00                       (RESORCE_ID_CPURTT +  0U)

#define    OSAL_COND_IMR_CB00                        (RESORCE_ID_IMR  +  0U)
#define    OSAL_COND_IMR_CB01                        (RESORCE_ID_IMR  +  1U)
#define    OSAL_COND_IMR_CB02                        (RESORCE_ID_IMR  +  2U)
#define    OSAL_COND_IMR_CB03                        (RESORCE_ID_IMR  +  3U)
#define    OSAL_COND_IMR_CB04                        (RESORCE_ID_IMR  +  4U)
#define    OSAL_COND_IMR_CB05                        (RESORCE_ID_IMR  +  5U)

#define    OSAL_COND_DOF_00                          (RESORCE_ID_VIP   +  0U)

#define    OSAL_COND_OSAL_00                         (RESORCE_ID_OSAL +  0U)
#define    OSAL_COND_OSAL_01                         (RESORCE_ID_OSAL +  1U)
#define    OSAL_COND_OSAL_02                         (RESORCE_ID_OSAL +  2U)
#define    OSAL_COND_OSAL_03                         (RESORCE_ID_OSAL +  3U)
#define    OSAL_COND_OSAL_04                         (RESORCE_ID_OSAL +  4U)
#define    OSAL_COND_OSAL_05                         (RESORCE_ID_OSAL +  5U)
#define    OSAL_COND_OSAL_06                         (RESORCE_ID_OSAL +  6U)
#define    OSAL_COND_OSAL_07                         (RESORCE_ID_OSAL +  7U)
#define    OSAL_COND_OSAL_08                         (RESORCE_ID_OSAL +  8U)
#define    OSAL_COND_OSAL_09                         (RESORCE_ID_OSAL +  9U)

/*******************************************************************************************************************//**
 * @def OSAL_MQ_ID
 * @brief Message queue IDs.
***********************************************************************************************************************/
#define    OSAL_MQ_IMR_MSG_00                        (RESORCE_ID_IMR   +  0U)
#define    OSAL_MQ_IMR_MSG_01                        (RESORCE_ID_IMR   +  1U)
#define    OSAL_MQ_IMR_MSG_02                        (RESORCE_ID_IMR   +  2U)
#define    OSAL_MQ_IMR_MSG_03                        (RESORCE_ID_IMR   +  3U)
#define    OSAL_MQ_IMR_MSG_04                        (RESORCE_ID_IMR   +  4U)
#define    OSAL_MQ_IMR_MSG_05                        (RESORCE_ID_IMR   +  5U)
#define    OSAL_MQ_IMR_RTT_MSG_00                    (RESORCE_ID_IMR   +  6U)
#define    OSAL_MQ_IMR_RTT_MSG_01                    (RESORCE_ID_IMR   +  7U)
#define    OSAL_MQ_IMR_RTT_MSG_02                    (RESORCE_ID_IMR   +  8U)
#define    OSAL_MQ_IMR_RTT_MSG_03                    (RESORCE_ID_IMR   +  9U)
#define    OSAL_MQ_IMR_RTT_MSG_04                    (RESORCE_ID_IMR   + 10U)
#define    OSAL_MQ_IMR_RTT_MSG_05                    (RESORCE_ID_IMR   + 11U)

#define    OSAL_MQ_ICCOM_MGR_TASK                    (RESORCE_ID_ICCOM +  0U)
#define    OSAL_MQ_ICCOM_REQ_OPEN                    (RESORCE_ID_ICCOM +  1U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_00          (RESORCE_ID_ICCOM +  2U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_01          (RESORCE_ID_ICCOM +  3U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_02          (RESORCE_ID_ICCOM +  4U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_03          (RESORCE_ID_ICCOM +  5U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_04          (RESORCE_ID_ICCOM +  6U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_05          (RESORCE_ID_ICCOM +  7U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_06          (RESORCE_ID_ICCOM +  8U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_07          (RESORCE_ID_ICCOM +  9U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_00         (RESORCE_ID_ICCOM + 10U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_01         (RESORCE_ID_ICCOM + 11U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_02         (RESORCE_ID_ICCOM + 12U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_03         (RESORCE_ID_ICCOM + 13U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_04         (RESORCE_ID_ICCOM + 14U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_05         (RESORCE_ID_ICCOM + 15U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_06         (RESORCE_ID_ICCOM + 16U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_07         (RESORCE_ID_ICCOM + 17U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_00             (RESORCE_ID_ICCOM + 18U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_01             (RESORCE_ID_ICCOM + 19U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_02             (RESORCE_ID_ICCOM + 20U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_03             (RESORCE_ID_ICCOM + 21U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_04             (RESORCE_ID_ICCOM + 22U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_05             (RESORCE_ID_ICCOM + 23U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_06             (RESORCE_ID_ICCOM + 24U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_07             (RESORCE_ID_ICCOM + 25U)
#define    OSAL_MQ_ICCOM_SEND_TASK_00                (RESORCE_ID_ICCOM + 26U)
#define    OSAL_MQ_ICCOM_SEND_TASK_01                (RESORCE_ID_ICCOM + 27U)
#define    OSAL_MQ_ICCOM_SEND_TASK_02                (RESORCE_ID_ICCOM + 28U)
#define    OSAL_MQ_ICCOM_SEND_TASK_03                (RESORCE_ID_ICCOM + 29U)
#define    OSAL_MQ_ICCOM_SEND_TASK_04                (RESORCE_ID_ICCOM + 30U)
#define    OSAL_MQ_ICCOM_SEND_TASK_05                (RESORCE_ID_ICCOM + 31U)
#define    OSAL_MQ_ICCOM_SEND_TASK_06                (RESORCE_ID_ICCOM + 32U)
#define    OSAL_MQ_ICCOM_SEND_TASK_07                (RESORCE_ID_ICCOM + 33U)
#define    OSAL_MQ_ICCOM_READ_TASK_00                (RESORCE_ID_ICCOM + 34U)
#define    OSAL_MQ_ICCOM_READ_TASK_01                (RESORCE_ID_ICCOM + 35U)
#define    OSAL_MQ_ICCOM_READ_TASK_02                (RESORCE_ID_ICCOM + 36U)
#define    OSAL_MQ_ICCOM_READ_TASK_03                (RESORCE_ID_ICCOM + 37U)
#define    OSAL_MQ_ICCOM_READ_TASK_04                (RESORCE_ID_ICCOM + 38U)
#define    OSAL_MQ_ICCOM_READ_TASK_05                (RESORCE_ID_ICCOM + 39U)
#define    OSAL_MQ_ICCOM_READ_TASK_06                (RESORCE_ID_ICCOM + 40U)
#define    OSAL_MQ_ICCOM_READ_TASK_07                (RESORCE_ID_ICCOM + 41U)
#define    OSAL_MQ_ICCOM_SEND_DATA_00                (RESORCE_ID_ICCOM + 42U)
#define    OSAL_MQ_ICCOM_SEND_DATA_01                (RESORCE_ID_ICCOM + 43U)
#define    OSAL_MQ_ICCOM_SEND_DATA_02                (RESORCE_ID_ICCOM + 44U)
#define    OSAL_MQ_ICCOM_SEND_DATA_03                (RESORCE_ID_ICCOM + 45U)
#define    OSAL_MQ_ICCOM_SEND_DATA_04                (RESORCE_ID_ICCOM + 46U)
#define    OSAL_MQ_ICCOM_SEND_DATA_05                (RESORCE_ID_ICCOM + 47U)
#define    OSAL_MQ_ICCOM_SEND_DATA_06                (RESORCE_ID_ICCOM + 48U)
#define    OSAL_MQ_ICCOM_SEND_DATA_07                (RESORCE_ID_ICCOM + 49U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_00             (RESORCE_ID_ICCOM + 50U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_01             (RESORCE_ID_ICCOM + 51U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_02             (RESORCE_ID_ICCOM + 52U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_03             (RESORCE_ID_ICCOM + 53U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_04             (RESORCE_ID_ICCOM + 54U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_05             (RESORCE_ID_ICCOM + 55U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_06             (RESORCE_ID_ICCOM + 56U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_07             (RESORCE_ID_ICCOM + 57U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_00       (RESORCE_ID_ICCOM + 58U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_01       (RESORCE_ID_ICCOM + 59U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_02       (RESORCE_ID_ICCOM + 60U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_03       (RESORCE_ID_ICCOM + 61U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_04       (RESORCE_ID_ICCOM + 62U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_05       (RESORCE_ID_ICCOM + 63U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_06       (RESORCE_ID_ICCOM + 64U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_07       (RESORCE_ID_ICCOM + 65U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_00    (RESORCE_ID_ICCOM + 66U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_01    (RESORCE_ID_ICCOM + 67U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_02    (RESORCE_ID_ICCOM + 68U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_03    (RESORCE_ID_ICCOM + 69U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_04    (RESORCE_ID_ICCOM + 70U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_05    (RESORCE_ID_ICCOM + 71U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_06    (RESORCE_ID_ICCOM + 72U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_07    (RESORCE_ID_ICCOM + 73U)

#define    OSAL_MQ_IMP_RTT_TASK_REQ                  (RESORCE_ID_IMP   +  0U)
#define    OSAL_MQ_IMP_RTT_TASK_ACK                  (RESORCE_ID_IMP   +  1U)
#define    OSAL_MQ_IMP_RTT_DRVCB_00                  (RESORCE_ID_IMP   +  2U)
#define    OSAL_MQ_IMP_RTT_EXECCB                    (RESORCE_ID_IMP   +  3U)
#define    OSAL_MQ_IMP_DRV_TASK_CBFUNC               (RESORCE_ID_IMP   +  4U)
#define    OSAL_MQ_IMP_DRV_TASK_DELTASK              (RESORCE_ID_IMP   +  5U)
#define    OSAL_MQ_IMP_IMPFW_SEND_0                  (RESORCE_ID_IMP   +  6U)
#define    OSAL_MQ_IMP_IMPFW_SEND_1                  (RESORCE_ID_IMP   +  7U)
#define    OSAL_MQ_IMP_IMPFW_SEND_2                  (RESORCE_ID_IMP   +  8U)
#define    OSAL_MQ_IMP_IMPFW_SEND_3                  (RESORCE_ID_IMP   +  9U)
#define    OSAL_MQ_IMP_IMPFW_SEND_4                  (RESORCE_ID_IMP   + 10U)
#define    OSAL_MQ_IMP_IMPFW_SEND_5                  (RESORCE_ID_IMP   + 11U)
#define    OSAL_MQ_IMP_IMPFW_SEND_6                  (RESORCE_ID_IMP   + 12U)
#define    OSAL_MQ_IMP_IMPFW_RECEIVE_0               (RESORCE_ID_IMP   + 13U)
#define    OSAL_MQ_IMP_IMPFW_RECEIVE_1               (RESORCE_ID_IMP   + 14U)
#define    OSAL_MQ_IMP_IMPFW_RECEIVE_2               (RESORCE_ID_IMP   + 15U)
#define    OSAL_MQ_IMP_IMPFW_RECEIVE_3               (RESORCE_ID_IMP   + 16U)
#define    OSAL_MQ_IMP_IMPFW_RECEIVE_4               (RESORCE_ID_IMP   + 17U)
#define    OSAL_MQ_IMP_IMPFW_RECEIVE_5               (RESORCE_ID_IMP   + 18U)
#define    OSAL_MQ_IMP_IMPFW_RECEIVE_6               (RESORCE_ID_IMP   + 19U)
#define    OSAL_MQ_IMP_IMPFW_CBFUNC_0                (RESORCE_ID_IMP   + 20U)
#define    OSAL_MQ_IMP_IMPFW_CBFUNC_1                (RESORCE_ID_IMP   + 21U)
#define    OSAL_MQ_IMP_IMPFW_CBFUNC_2                (RESORCE_ID_IMP   + 22U)
#define    OSAL_MQ_IMP_IMPFW_CBFUNC_3                (RESORCE_ID_IMP   + 23U)
#define    OSAL_MQ_IMP_IMPFW_CBFUNC_4                (RESORCE_ID_IMP   + 24U)
#define    OSAL_MQ_IMP_IMPFW_CBFUNC_5                (RESORCE_ID_IMP   + 25U)
#define    OSAL_MQ_IMP_IMPFW_CBFUNC_6                (RESORCE_ID_IMP   + 26U)
#define    OSAL_MQ_IMP_IMPFW_RTT_SAMPLE              (RESORCE_ID_IMP   + 27U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_0           (RESORCE_ID_IMP   + 28U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_1           (RESORCE_ID_IMP   + 29U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_2           (RESORCE_ID_IMP   + 30U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_3           (RESORCE_ID_IMP   + 31U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_4           (RESORCE_ID_IMP   + 32U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_5           (RESORCE_ID_IMP   + 33U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_6           (RESORCE_ID_IMP   + 34U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_0        (RESORCE_ID_IMP   + 35U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_1        (RESORCE_ID_IMP   + 36U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_2        (RESORCE_ID_IMP   + 37U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_3        (RESORCE_ID_IMP   + 38U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_4        (RESORCE_ID_IMP   + 39U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_5        (RESORCE_ID_IMP   + 40U)
#define    OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_6        (RESORCE_ID_IMP   + 41U)

#define    OSAL_MQ_CPURTT_TEST00                     (RESORCE_ID_CPURTT + 0U)
#define    OSAL_MQ_CPURTT_TEST01                     (RESORCE_ID_CPURTT + 1U)
#define    OSAL_MQ_CPURTT_RFSO                       (RESORCE_ID_CPURTT + 2U)
#define    OSAL_MQ_CPURTT_TEST02                     (RESORCE_ID_CPURTT + 3U)
#define    OSAL_MQ_CPURTT_TEST03                     (RESORCE_ID_CPURTT + 4U)
#define    OSAL_MQ_CPURTT_TEST04                     (RESORCE_ID_CPURTT + 5U)
#define    OSAL_MQ_FBIST_TEST00                      (RESORCE_ID_CPURTT + 6U)
#define    OSAL_MQ_FBIST_TEST01                      (RESORCE_ID_CPURTT + 7U)
#define    OSAL_MQ_RFSO_TEST00                       (RESORCE_ID_CPURTT + 8U)
#define    OSAL_MQ_RFSO_TEST01                       (RESORCE_ID_CPURTT + 9U)

#define    OSAL_MQ_HWARTT_SAMPLE                     (RESORCE_ID_HWARTT + 2U)

#define    OSAL_MQ_ISP_00                            (RESORCE_ID_ISP   +  0U)
#define    OSAL_MQ_ISP_01                            (RESORCE_ID_ISP   +  1U)
#define    OSAL_MQ_ISP_02                            (RESORCE_ID_ISP   +  2U)
#define    OSAL_MQ_ISP_03                            (RESORCE_ID_ISP   +  3U)

#define    OSAL_MQ_OSAL_00                           (RESORCE_ID_OSAL  +  0U)
#define    OSAL_MQ_OSAL_01                           (RESORCE_ID_OSAL  +  1U)
#define    OSAL_MQ_OSAL_02                           (RESORCE_ID_OSAL  +  2U)
#define    OSAL_MQ_OSAL_03                           (RESORCE_ID_OSAL  +  3U)
#define    OSAL_MQ_OSAL_04                           (RESORCE_ID_OSAL  +  4U)
#define    OSAL_MQ_OSAL_05                           (RESORCE_ID_OSAL  +  5U)
#define    OSAL_MQ_OSAL_06                           (RESORCE_ID_OSAL  +  6U)
#define    OSAL_MQ_OSAL_07                           (RESORCE_ID_OSAL  +  7U)
#define    OSAL_MQ_OSAL_08                           (RESORCE_ID_OSAL  +  8U)
#define    OSAL_MQ_OSAL_09                           (RESORCE_ID_OSAL  +  9U)

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_STACK
 * Thread Starck size.
***********************************************************************************************************************/
/* stack size for Application core use-case */
#if defined(CONFIG_SMALL_STACKSIZE)
/* stack size should be minimized for Realtime core application due to reduce memory consumption */
#define    OSAL_THREAD_STACK_SIZE_ICCOM_DEMO         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_MGR          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_TIMER        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV0         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV1         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV2         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV3         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV4         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV5         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV6         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV7         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ0        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ1        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ2        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ3        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ4        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ5        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ6        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ7        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND0        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND1        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND2        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND3        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND4        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND5        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND6        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND7        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMP_RTT_DRV        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMP_DRV            (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW0             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW1             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW2             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW3             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW4             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW5             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW6             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP0         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP1         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP2         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP3         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP4         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP5         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP6         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_CB          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_FBIST       (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A1          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A21         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A22         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A23         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A24         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_TP1         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_TP2         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_FBIST_TP1          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_FBIST_TP2          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_RFSO_TP1           (0x1000)
#define    OSAL_THREAD_STACK_SIZE_RFSO_TP2           (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMR00              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMR01              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMR02              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMR03              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMR04              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMR05              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_VIP00              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_VIP01              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_VIP02              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_VIP03              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_VIP04              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ISP00              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ISP01              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ISP02              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ISP03              (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL00             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL01             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL02             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL03             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL04             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL05             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL06             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL07             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL08             (0x1000)
#define    OSAL_THREAD_STACK_SIZE_OSAL09             (0x1000)
#else /* defined(CONFIG_SMALL_STACKSIZE) */
#define    OSAL_THREAD_STACK_SIZE_ICCOM_DEMO         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_MGR          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_TIMER        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV0         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV1         (0x1800)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV2         (0x1800)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV3         (0x1800)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV4         (0x1800)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV5         (0x1800)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV6         (0x1800)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_RCV7         (0x1800)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ0        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ1        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ2        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ3        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ4        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ5        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ6        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_READ7        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND0        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND1        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND2        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND3        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND4        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND5        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND6        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_ICCOM_SEND7        (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMP_RTT_DRV        (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMP_DRV            (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW0             (0x4000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW1             (0x4000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW2             (0x4000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW3             (0x4000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW4             (0x4000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW5             (0x4000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW6             (0x4000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP0         (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP1         (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP2         (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP3         (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP4         (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP5         (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMPFW_APP6         (0x2000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_CB          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_FBIST       (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A1          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A21         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A22         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A23         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_A24         (0x1000)
#define    OSAL_THREAD_STACK_SIZE_CPURTT_TP          (0x1000)
#define    OSAL_THREAD_STACK_SIZE_IMR00              (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMR01              (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMR02              (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMR03              (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMR04              (0x2000)
#define    OSAL_THREAD_STACK_SIZE_IMR05              (0x2000)
#define    OSAL_THREAD_STACK_SIZE_VIP00              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_VIP01              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_VIP02              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_VIP03              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_VIP04              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_ISP00              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_ISP01              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_ISP02              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_ISP03              (0x200000)
#define    OSAL_THREAD_STACK_SIZE_OSAL00             (0x200000)
#define    OSAL_THREAD_STACK_SIZE_OSAL01             (0x200000)
#define    OSAL_THREAD_STACK_SIZE_OSAL02             (0x200000)
#define    OSAL_THREAD_STACK_SIZE_OSAL03             (0x200000)
#define    OSAL_THREAD_STACK_SIZE_OSAL04             (0x200000)
#define    OSAL_THREAD_STACK_SIZE_OSAL05             (0x2000)
#define    OSAL_THREAD_STACK_SIZE_OSAL06             (0x2000)
#define    OSAL_THREAD_STACK_SIZE_OSAL07             (0x2000)
#define    OSAL_THREAD_STACK_SIZE_OSAL08             (0x2000)
#define    OSAL_THREAD_STACK_SIZE_OSAL09             (0x2000)
#endif/* defined(CONFIG_SMALL_STACKSIZE) */

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_STACK_ADDR
 * Thread Starck address.
***********************************************************************************************************************/
/*********** ### thread stack must be allocated statically on Zephyr ### ***********/
#if defined(ENABLE_RESOURCE_ICCOM)
K_THREAD_STACK_DEFINE(addr_iccom_demo,      OSAL_THREAD_STACK_SIZE_ICCOM_DEMO);
K_THREAD_STACK_DEFINE(addr_iccom_mgr,       OSAL_THREAD_STACK_SIZE_ICCOM_MGR);
K_THREAD_STACK_DEFINE(addr_iccom_timer,     OSAL_THREAD_STACK_SIZE_ICCOM_TIMER);
K_THREAD_STACK_DEFINE(addr_iccom_rcv0,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV0);
K_THREAD_STACK_DEFINE(addr_iccom_rcv1,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV1);
K_THREAD_STACK_DEFINE(addr_iccom_rcv2,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV2);
K_THREAD_STACK_DEFINE(addr_iccom_rcv3,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV3);
K_THREAD_STACK_DEFINE(addr_iccom_rcv4,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV4);
K_THREAD_STACK_DEFINE(addr_iccom_rcv5,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV5);
K_THREAD_STACK_DEFINE(addr_iccom_rcv6,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV6);
K_THREAD_STACK_DEFINE(addr_iccom_rcv7,      OSAL_THREAD_STACK_SIZE_ICCOM_RCV7);
K_THREAD_STACK_DEFINE(addr_iccom_read0,     OSAL_THREAD_STACK_SIZE_ICCOM_READ0);
K_THREAD_STACK_DEFINE(addr_iccom_read1,     OSAL_THREAD_STACK_SIZE_ICCOM_READ1);
K_THREAD_STACK_DEFINE(addr_iccom_read2,     OSAL_THREAD_STACK_SIZE_ICCOM_READ2);
K_THREAD_STACK_DEFINE(addr_iccom_read3,     OSAL_THREAD_STACK_SIZE_ICCOM_READ3);
K_THREAD_STACK_DEFINE(addr_iccom_read4,     OSAL_THREAD_STACK_SIZE_ICCOM_READ4);
K_THREAD_STACK_DEFINE(addr_iccom_read5,     OSAL_THREAD_STACK_SIZE_ICCOM_READ5);
K_THREAD_STACK_DEFINE(addr_iccom_read6,     OSAL_THREAD_STACK_SIZE_ICCOM_READ6);
K_THREAD_STACK_DEFINE(addr_iccom_read7,     OSAL_THREAD_STACK_SIZE_ICCOM_READ7);
K_THREAD_STACK_DEFINE(addr_iccom_send0,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND0);
K_THREAD_STACK_DEFINE(addr_iccom_send1,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND1);
K_THREAD_STACK_DEFINE(addr_iccom_send2,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND2);
K_THREAD_STACK_DEFINE(addr_iccom_send3,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND3);
K_THREAD_STACK_DEFINE(addr_iccom_send4,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND4);
K_THREAD_STACK_DEFINE(addr_iccom_send5,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND5);
K_THREAD_STACK_DEFINE(addr_iccom_send6,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND6);
K_THREAD_STACK_DEFINE(addr_iccom_send7,     OSAL_THREAD_STACK_SIZE_ICCOM_SEND7);
#endif /* defined(ENABLE_RESOURCE_ICCOM) */
#if defined(ENABLE_RESOURCE_IMP)
K_THREAD_STACK_DEFINE(addr_imp_rtt_drv,     OSAL_THREAD_STACK_SIZE_IMP_RTT_DRV);
K_THREAD_STACK_DEFINE(addr_imp_drv,         OSAL_THREAD_STACK_SIZE_IMP_DRV);
K_THREAD_STACK_DEFINE(addr_impfw0,          OSAL_THREAD_STACK_SIZE_IMPFW0);
K_THREAD_STACK_DEFINE(addr_impfw1,          OSAL_THREAD_STACK_SIZE_IMPFW1);
K_THREAD_STACK_DEFINE(addr_impfw2,          OSAL_THREAD_STACK_SIZE_IMPFW2);
K_THREAD_STACK_DEFINE(addr_impfw3,          OSAL_THREAD_STACK_SIZE_IMPFW3);
K_THREAD_STACK_DEFINE(addr_impfw4,          OSAL_THREAD_STACK_SIZE_IMPFW4);
K_THREAD_STACK_DEFINE(addr_impfw5,          OSAL_THREAD_STACK_SIZE_IMPFW5);
K_THREAD_STACK_DEFINE(addr_impfw6,          OSAL_THREAD_STACK_SIZE_IMPFW6);
K_THREAD_STACK_DEFINE(addr_impfw_app0,      OSAL_THREAD_STACK_SIZE_IMPFW_APP0);
K_THREAD_STACK_DEFINE(addr_impfw_app1,      OSAL_THREAD_STACK_SIZE_IMPFW_APP1);
K_THREAD_STACK_DEFINE(addr_impfw_app2,      OSAL_THREAD_STACK_SIZE_IMPFW_APP2);
K_THREAD_STACK_DEFINE(addr_impfw_app3,      OSAL_THREAD_STACK_SIZE_IMPFW_APP3);
K_THREAD_STACK_DEFINE(addr_impfw_app4,      OSAL_THREAD_STACK_SIZE_IMPFW_APP4);
K_THREAD_STACK_DEFINE(addr_impfw_app5,      OSAL_THREAD_STACK_SIZE_IMPFW_APP5);
K_THREAD_STACK_DEFINE(addr_impfw_app6,      OSAL_THREAD_STACK_SIZE_IMPFW_APP6);
#endif /* defined(ENABLE_RESOURCE_IMP) */
#if defined(ENABLE_RESOURCE_RTT)
K_THREAD_STACK_DEFINE(addr_cpurtt_cb,       OSAL_THREAD_STACK_SIZE_CPURTT_CB);
K_THREAD_STACK_DEFINE(addr_cpurtt_fbist,    OSAL_THREAD_STACK_SIZE_CPURTT_FBIST);
K_THREAD_STACK_DEFINE(addr_cpurtt_a1,       OSAL_THREAD_STACK_SIZE_CPURTT_A1);
K_THREAD_STACK_DEFINE(addr_cpurtt_a21,      OSAL_THREAD_STACK_SIZE_CPURTT_A21);
K_THREAD_STACK_DEFINE(addr_cpurtt_a22,      OSAL_THREAD_STACK_SIZE_CPURTT_A22);
K_THREAD_STACK_DEFINE(addr_cpurtt_a23,      OSAL_THREAD_STACK_SIZE_CPURTT_A23);
K_THREAD_STACK_DEFINE(addr_cpurtt_a24,      OSAL_THREAD_STACK_SIZE_CPURTT_A24);
K_THREAD_STACK_DEFINE(addr_cpurtt_tp1,      OSAL_THREAD_STACK_SIZE_CPURTT_TP1);
K_THREAD_STACK_DEFINE(addr_cpurtt_tp2,      OSAL_THREAD_STACK_SIZE_CPURTT_TP2);
K_THREAD_STACK_DEFINE(addr_fbist_tp1,       OSAL_THREAD_STACK_SIZE_FBIST_TP1);
K_THREAD_STACK_DEFINE(addr_fbist_tp2,       OSAL_THREAD_STACK_SIZE_FBIST_TP2);
K_THREAD_STACK_DEFINE(addr_rfso_tp1,        OSAL_THREAD_STACK_SIZE_RFSO_TP1);
K_THREAD_STACK_DEFINE(addr_rfso_tp2,        OSAL_THREAD_STACK_SIZE_RFSO_TP2);
#endif /* defined(ENABLE_RESOURCE_RTT) */
#if defined(ENABLE_RESOURCE_IMR)
K_THREAD_STACK_DEFINE(addr_imr00,           OSAL_THREAD_STACK_SIZE_IMR00);
K_THREAD_STACK_DEFINE(addr_imr01,           OSAL_THREAD_STACK_SIZE_IMR01);
K_THREAD_STACK_DEFINE(addr_imr02,           OSAL_THREAD_STACK_SIZE_IMR02);
K_THREAD_STACK_DEFINE(addr_imr03,           OSAL_THREAD_STACK_SIZE_IMR03);
K_THREAD_STACK_DEFINE(addr_imr04,           OSAL_THREAD_STACK_SIZE_IMR04);
K_THREAD_STACK_DEFINE(addr_imr05,           OSAL_THREAD_STACK_SIZE_IMR05);
#endif /* defined(ENABLE_RESOURCE_IMR) */
#if defined(ENABLE_RESOURCE_VISIONIP)
K_THREAD_STACK_DEFINE(addr_vip00,           OSAL_THREAD_STACK_SIZE_VIP00);
K_THREAD_STACK_DEFINE(addr_vip01,           OSAL_THREAD_STACK_SIZE_VIP01);
K_THREAD_STACK_DEFINE(addr_vip02,           OSAL_THREAD_STACK_SIZE_VIP02);
K_THREAD_STACK_DEFINE(addr_vip03,           OSAL_THREAD_STACK_SIZE_VIP03);
K_THREAD_STACK_DEFINE(addr_vip04,           OSAL_THREAD_STACK_SIZE_VIP04);
#endif /* defined(ENABLE_RESOURCE_VISIONIP) */
#if defined(ENABLE_RESOURCE_ISP)
K_THREAD_STACK_DEFINE(addr_isp00,           OSAL_THREAD_STACK_SIZE_ISP00);
K_THREAD_STACK_DEFINE(addr_isp01,           OSAL_THREAD_STACK_SIZE_ISP01);
K_THREAD_STACK_DEFINE(addr_isp02,           OSAL_THREAD_STACK_SIZE_ISP02);
K_THREAD_STACK_DEFINE(addr_isp03,           OSAL_THREAD_STACK_SIZE_ISP03);
#endif /* defined(ENABLE_RESOURCE_ISP) */
#if defined(ENABLE_RESOURCE_OSAL)
K_THREAD_STACK_DEFINE(addr_osal00,          OSAL_THREAD_STACK_SIZE_OSAL00);
K_THREAD_STACK_DEFINE(addr_osal01,          OSAL_THREAD_STACK_SIZE_OSAL01);
K_THREAD_STACK_DEFINE(addr_osal02,          OSAL_THREAD_STACK_SIZE_OSAL02);
K_THREAD_STACK_DEFINE(addr_osal03,          OSAL_THREAD_STACK_SIZE_OSAL03);
K_THREAD_STACK_DEFINE(addr_osal04,          OSAL_THREAD_STACK_SIZE_OSAL04);
K_THREAD_STACK_DEFINE(addr_osal05,          OSAL_THREAD_STACK_SIZE_OSAL05);
K_THREAD_STACK_DEFINE(addr_osal06,          OSAL_THREAD_STACK_SIZE_OSAL06);
K_THREAD_STACK_DEFINE(addr_osal07,          OSAL_THREAD_STACK_SIZE_OSAL07);
K_THREAD_STACK_DEFINE(addr_osal08,          OSAL_THREAD_STACK_SIZE_OSAL08);
K_THREAD_STACK_DEFINE(addr_osal09,          OSAL_THREAD_STACK_SIZE_OSAL09);
#endif /* defined(ENABLE_RESOURCE_OSAL) */

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var g_osal_device_soc_type
 * @brief osal device configuration : SoC type
***********************************************************************************************************************/
const st_osal_device_soc_type_t g_osal_device_soc_type =
{
#if defined(RCAR_V4H2)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4H2
#elif defined(RCAR_V4H)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4H
#else
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_INVALID
#endif
};

/*******************************************************************************************************************//**
 * @var g_osal_memory_max_configration
 * @brief osal resource configuration : max memory configuration structure
 * @note The mode and the memory_limit are unused
***********************************************************************************************************************/
const st_osal_mmngr_config_t g_osal_memory_max_configration =
{
    .mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT,
    .memory_limit = (0U),
    .max_allowed_allocations = (1024U),
    .max_registered_monitors_cbs = (1024U)
};

/*******************************************************************************************************************//**
 * @var g_osal_memory_region_info
 * @brief osal resource configuration : memory region information structure
***********************************************************************************************************************/
const st_osal_mmngr_region_info_list_t g_osal_memory_region_info_list =
{
    .num_of_memory_regions = DT_PROP(DT_NODELABEL(osal_mmngr), num_of_memory_regions),
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
        DT_FOREACH_CHILD(DT_NODELABEL(osal_mmngr), NODE_PROPERTIES)
    }
};

/*******************************************************************************************************************//**
 * @var g_osal_thread_config
 * @brief osal resource configuration : thread configuration table
***********************************************************************************************************************/
const st_osal_rcfg_thread_config_t g_osal_thread_config[] =
{
#if defined(RCAR_V4H) || defined(RCAR_V4H2)
    /* thread id,                   func, userarg, priority,                  task_name,             stack_size */
#if defined(ENABLE_RESOURCE_ICCOM)
    {OSAL_THREAD_ICCOM_DEMO_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task00",        OSAL_THREAD_STACK_SIZE_ICCOM_DEMO},      },
    {OSAL_THREAD_ICCOM_MGR_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task01",        OSAL_THREAD_STACK_SIZE_ICCOM_MGR},       },
    {OSAL_THREAD_ICCOM_TIMER_TASK,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task02",        OSAL_THREAD_STACK_SIZE_ICCOM_TIMER},     },
    {OSAL_THREAD_ICCOM_RCV_TASK0,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task03",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV0},      },
    {OSAL_THREAD_ICCOM_RCV_TASK1,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task04",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV1},      },
    {OSAL_THREAD_ICCOM_RCV_TASK2,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task05",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV2},      },
    {OSAL_THREAD_ICCOM_RCV_TASK3,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task06",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV3},      },
    {OSAL_THREAD_ICCOM_RCV_TASK4,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task07",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV4},      },
    {OSAL_THREAD_ICCOM_RCV_TASK5,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task08",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV5},      },
    {OSAL_THREAD_ICCOM_RCV_TASK6,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task09",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV6},      },
    {OSAL_THREAD_ICCOM_RCV_TASK7,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task10",        OSAL_THREAD_STACK_SIZE_ICCOM_RCV7},      },
    {OSAL_THREAD_ICCOM_READ_TASK0,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task11",        OSAL_THREAD_STACK_SIZE_ICCOM_READ0},     },
    {OSAL_THREAD_ICCOM_READ_TASK1,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task12",        OSAL_THREAD_STACK_SIZE_ICCOM_READ1},     },
    {OSAL_THREAD_ICCOM_READ_TASK2,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task13",        OSAL_THREAD_STACK_SIZE_ICCOM_READ2},     },
    {OSAL_THREAD_ICCOM_READ_TASK3,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task14",        OSAL_THREAD_STACK_SIZE_ICCOM_READ3},     },
    {OSAL_THREAD_ICCOM_READ_TASK4,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task15",        OSAL_THREAD_STACK_SIZE_ICCOM_READ4},     },
    {OSAL_THREAD_ICCOM_READ_TASK5,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task16",        OSAL_THREAD_STACK_SIZE_ICCOM_READ5},     },
    {OSAL_THREAD_ICCOM_READ_TASK6,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task17",        OSAL_THREAD_STACK_SIZE_ICCOM_READ6},     },
    {OSAL_THREAD_ICCOM_READ_TASK7,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task18",        OSAL_THREAD_STACK_SIZE_ICCOM_READ7},     },
    {OSAL_THREAD_ICCOM_SEND_TASK0,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task19",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND0},     },
    {OSAL_THREAD_ICCOM_SEND_TASK1,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task20",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND1},     },
    {OSAL_THREAD_ICCOM_SEND_TASK2,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task21",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND2},     },
    {OSAL_THREAD_ICCOM_SEND_TASK3,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task22",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND3},     },
    {OSAL_THREAD_ICCOM_SEND_TASK4,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task23",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND4},     },
    {OSAL_THREAD_ICCOM_SEND_TASK5,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task24",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND5},     },
    {OSAL_THREAD_ICCOM_SEND_TASK6,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task25",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND6},     },
    {OSAL_THREAD_ICCOM_SEND_TASK7,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom_task26",        OSAL_THREAD_STACK_SIZE_ICCOM_SEND7},     },
#endif /* defined(ENABLE_RESOURCE_ICCOM) */
#if defined(ENABLE_RESOURCE_IMP)
    {OSAL_THREAD_IMP_RTT_DRV,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp_task00",          OSAL_THREAD_STACK_SIZE_IMP_RTT_DRV},     },
    {OSAL_THREAD_IMP_DRV,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp_task01",          OSAL_THREAD_STACK_SIZE_IMP_DRV},         },
    {OSAL_THREAD_IMPFW_TASK00,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task00",        OSAL_THREAD_STACK_SIZE_IMPFW0},          },
    {OSAL_THREAD_IMPFW_TASK01,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task01",        OSAL_THREAD_STACK_SIZE_IMPFW1},          },
    {OSAL_THREAD_IMPFW_TASK02,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task02",        OSAL_THREAD_STACK_SIZE_IMPFW2},          },
    {OSAL_THREAD_IMPFW_TASK03,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task03",        OSAL_THREAD_STACK_SIZE_IMPFW3},          },
    {OSAL_THREAD_IMPFW_TASK04,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task04",        OSAL_THREAD_STACK_SIZE_IMPFW4},          },
    {OSAL_THREAD_IMPFW_TASK05,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task05",        OSAL_THREAD_STACK_SIZE_IMPFW5},          },
    {OSAL_THREAD_IMPFW_TASK06,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task06",        OSAL_THREAD_STACK_SIZE_IMPFW6},          },
    {OSAL_THREAD_IMPFW_APP_TASK00,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfwapp_task00",     OSAL_THREAD_STACK_SIZE_IMPFW_APP0},      },
    {OSAL_THREAD_IMPFW_APP_TASK01,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfwapp_task01",     OSAL_THREAD_STACK_SIZE_IMPFW_APP1},      },
    {OSAL_THREAD_IMPFW_APP_TASK02,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfwapp_task02",     OSAL_THREAD_STACK_SIZE_IMPFW_APP2},      },
    {OSAL_THREAD_IMPFW_APP_TASK03,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfwapp_task03",     OSAL_THREAD_STACK_SIZE_IMPFW_APP3},      },
    {OSAL_THREAD_IMPFW_APP_TASK04,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfwapp_task04",     OSAL_THREAD_STACK_SIZE_IMPFW_APP4},      },
    {OSAL_THREAD_IMPFW_APP_TASK05,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfwapp_task05",     OSAL_THREAD_STACK_SIZE_IMPFW_APP5},      },
    {OSAL_THREAD_IMPFW_APP_TASK06,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfwapp_task06",     OSAL_THREAD_STACK_SIZE_IMPFW_APP6},      },
#endif /* defined(ENABLE_RESOURCE_IMP) */
#if defined(ENABLE_RESOURCE_RTT)
    {OSAL_THREAD_CPURTT_CB_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "callback_task",       OSAL_THREAD_STACK_SIZE_CPURTT_CB},       },
    {OSAL_THREAD_CPURTT_FBIST_TASK, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "fbist_fin_task",      OSAL_THREAD_STACK_SIZE_CPURTT_FBIST},    },
    {OSAL_THREAD_CPURTT_A1_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a1_task",      OSAL_THREAD_STACK_SIZE_CPURTT_A1},       },
    {OSAL_THREAD_CPURTT_A21_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a21_task",     OSAL_THREAD_STACK_SIZE_CPURTT_A21},      },
    {OSAL_THREAD_CPURTT_A22_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a22_task",     OSAL_THREAD_STACK_SIZE_CPURTT_A22},      },
    {OSAL_THREAD_CPURTT_A23_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a23_task",     OSAL_THREAD_STACK_SIZE_CPURTT_A23},      },
    {OSAL_THREAD_CPURTT_A24_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a24_task",     OSAL_THREAD_STACK_SIZE_CPURTT_A24},      },
    {OSAL_THREAD_CPURTT_TP_TASK1,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_tp_task1",     OSAL_THREAD_STACK_SIZE_CPURTT_TP1},      },
    {OSAL_THREAD_CPURTT_TP_TASK2,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_tp_task2",     OSAL_THREAD_STACK_SIZE_CPURTT_TP2},      },
    {OSAL_THREAD_FBIST_TP_TASK1,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "fbist_tp_task1",      OSAL_THREAD_STACK_SIZE_FBIST_TP1},       },
    {OSAL_THREAD_FBIST_TP_TASK2,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "fbist_tp_task2",      OSAL_THREAD_STACK_SIZE_FBIST_TP2},       },
    {OSAL_THREAD_RFSO_TP_TASK1,     {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rfso_tp_task1",       OSAL_THREAD_STACK_SIZE_RFSO_TP1},        },
    {OSAL_THREAD_RFSO_TP_TASK2,     {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rfso_tp_task2",       OSAL_THREAD_STACK_SIZE_RFSO_TP2},        },
#endif /* defined(ENABLE_RESOURCE_RTT) */
#if defined(ENABLE_RESOURCE_IMR)
    {OSAL_THREAD_IMR_TASK00,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task00",          OSAL_THREAD_STACK_SIZE_IMR00},           },
    {OSAL_THREAD_IMR_TASK01,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task01",          OSAL_THREAD_STACK_SIZE_IMR01},           },
    {OSAL_THREAD_IMR_TASK02,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task02",          OSAL_THREAD_STACK_SIZE_IMR02},           },
    {OSAL_THREAD_IMR_TASK03,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task03",          OSAL_THREAD_STACK_SIZE_IMR03},           },
    {OSAL_THREAD_IMR_TASK04,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task04",          OSAL_THREAD_STACK_SIZE_IMR04},           },
    {OSAL_THREAD_IMR_TASK05,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task05",          OSAL_THREAD_STACK_SIZE_IMR05},           },
#endif /* defined(ENABLE_RESOURCE_IMR) */
#if defined(ENABLE_RESOURCE_VISIONIP)
    {OSAL_THREAD_VIP_TASK00,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task00",          OSAL_THREAD_STACK_SIZE_VIP00},           },
    {OSAL_THREAD_VIP_TASK01,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task01",          OSAL_THREAD_STACK_SIZE_VIP01},           },
    {OSAL_THREAD_VIP_TASK02,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task02",          OSAL_THREAD_STACK_SIZE_VIP02},           },
    {OSAL_THREAD_VIP_TASK03,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task03",          OSAL_THREAD_STACK_SIZE_VIP03},           },
    {OSAL_THREAD_VIP_TASK04,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task04",          OSAL_THREAD_STACK_SIZE_VIP04},           },
#endif /* defined(ENABLE_RESOURCE_VISIONIP) */
#if defined(ENABLE_RESOURCE_ISP)
    {OSAL_THREAD_ISP_TASK00,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task00",          OSAL_THREAD_STACK_SIZE_ISP00},           },
    {OSAL_THREAD_ISP_TASK01,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task01",          OSAL_THREAD_STACK_SIZE_ISP01},           },
    {OSAL_THREAD_ISP_TASK02,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task02",          OSAL_THREAD_STACK_SIZE_ISP02},           },
    {OSAL_THREAD_ISP_TASK03,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task03",          OSAL_THREAD_STACK_SIZE_ISP03},           },
#endif /* defined(ENABLE_RESOURCE_ISP) */

#if defined(ENABLE_RESOURCE_OSAL)
    {OSAL_THREAD_OSAL_00,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task01",         OSAL_THREAD_STACK_SIZE_OSAL00},          },
    {OSAL_THREAD_OSAL_01,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task02",         OSAL_THREAD_STACK_SIZE_OSAL01},          },
    {OSAL_THREAD_OSAL_02,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task03",         OSAL_THREAD_STACK_SIZE_OSAL02},          },
    {OSAL_THREAD_OSAL_03,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task04",         OSAL_THREAD_STACK_SIZE_OSAL03},          },
    {OSAL_THREAD_OSAL_04,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task05",         OSAL_THREAD_STACK_SIZE_OSAL04},          },
    {OSAL_THREAD_OSAL_05,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task06",         OSAL_THREAD_STACK_SIZE_OSAL05},          },
    {OSAL_THREAD_OSAL_06,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task07",         OSAL_THREAD_STACK_SIZE_OSAL06},          },
    {OSAL_THREAD_OSAL_07,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task08",         OSAL_THREAD_STACK_SIZE_OSAL07},          },
    {OSAL_THREAD_OSAL_08,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task09",         OSAL_THREAD_STACK_SIZE_OSAL08},          },
    {OSAL_THREAD_OSAL_09,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task10",         OSAL_THREAD_STACK_SIZE_OSAL09},          },
#endif /* defined(ENABLE_RESOURCE_OSAL) */

#endif
    {.id = OSAL_THREAD_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_thread_stack_config
 * @brief osal resource configuration : thread stack address configuration table
***********************************************************************************************************************/
const st_osal_thread_stack_t g_osal_thread_stack_config[] = 
{
#if defined(RCAR_V4H) || defined(RCAR_V4H2)
    /* thread id,                   address of stack */
#if defined(ENABLE_RESOURCE_ICCOM)
    {OSAL_THREAD_ICCOM_DEMO_TASK,   addr_iccom_demo    },
    {OSAL_THREAD_ICCOM_MGR_TASK,    addr_iccom_mgr     },
    {OSAL_THREAD_ICCOM_TIMER_TASK,  addr_iccom_timer   },
    {OSAL_THREAD_ICCOM_RCV_TASK0,   addr_iccom_rcv0    },
    {OSAL_THREAD_ICCOM_RCV_TASK1,   addr_iccom_rcv1    },
    {OSAL_THREAD_ICCOM_RCV_TASK2,   addr_iccom_rcv2    },
    {OSAL_THREAD_ICCOM_RCV_TASK3,   addr_iccom_rcv3    },
    {OSAL_THREAD_ICCOM_RCV_TASK4,   addr_iccom_rcv4    },
    {OSAL_THREAD_ICCOM_RCV_TASK5,   addr_iccom_rcv5    },
    {OSAL_THREAD_ICCOM_RCV_TASK6,   addr_iccom_rcv6    },
    {OSAL_THREAD_ICCOM_RCV_TASK7,   addr_iccom_rcv7    },
    {OSAL_THREAD_ICCOM_READ_TASK0,  addr_iccom_read0   },
    {OSAL_THREAD_ICCOM_READ_TASK1,  addr_iccom_read1   },
    {OSAL_THREAD_ICCOM_READ_TASK2,  addr_iccom_read2   },
    {OSAL_THREAD_ICCOM_READ_TASK3,  addr_iccom_read3   },
    {OSAL_THREAD_ICCOM_READ_TASK4,  addr_iccom_read4   },
    {OSAL_THREAD_ICCOM_READ_TASK5,  addr_iccom_read5   },
    {OSAL_THREAD_ICCOM_READ_TASK6,  addr_iccom_read6   },
    {OSAL_THREAD_ICCOM_READ_TASK7,  addr_iccom_read7   },
    {OSAL_THREAD_ICCOM_SEND_TASK0,  addr_iccom_send0   },
    {OSAL_THREAD_ICCOM_SEND_TASK1,  addr_iccom_send1   },
    {OSAL_THREAD_ICCOM_SEND_TASK2,  addr_iccom_send2   },
    {OSAL_THREAD_ICCOM_SEND_TASK3,  addr_iccom_send3   },
    {OSAL_THREAD_ICCOM_SEND_TASK4,  addr_iccom_send4   },
    {OSAL_THREAD_ICCOM_SEND_TASK5,  addr_iccom_send5   },
    {OSAL_THREAD_ICCOM_SEND_TASK6,  addr_iccom_send6   },
    {OSAL_THREAD_ICCOM_SEND_TASK7,  addr_iccom_send7   },
#endif /* defined(ENABLE_RESOURCE_ICCOM) */
#if defined(ENABLE_RESOURCE_IMP)
    {OSAL_THREAD_IMP_RTT_DRV,       addr_imp_rtt_drv   },
    {OSAL_THREAD_IMP_DRV,           addr_imp_drv       },
    {OSAL_THREAD_IMPFW_TASK00,      addr_impfw0        },
    {OSAL_THREAD_IMPFW_TASK01,      addr_impfw1        },
    {OSAL_THREAD_IMPFW_TASK02,      addr_impfw2        },
    {OSAL_THREAD_IMPFW_TASK03,      addr_impfw3        },
    {OSAL_THREAD_IMPFW_TASK04,      addr_impfw4        },
    {OSAL_THREAD_IMPFW_TASK05,      addr_impfw5        },
    {OSAL_THREAD_IMPFW_TASK06,      addr_impfw6        },
    {OSAL_THREAD_IMPFW_APP_TASK00,  addr_impfw_app0    },
    {OSAL_THREAD_IMPFW_APP_TASK01,  addr_impfw_app1    },
    {OSAL_THREAD_IMPFW_APP_TASK02,  addr_impfw_app2    },
    {OSAL_THREAD_IMPFW_APP_TASK03,  addr_impfw_app3    },
    {OSAL_THREAD_IMPFW_APP_TASK04,  addr_impfw_app4    },
    {OSAL_THREAD_IMPFW_APP_TASK05,  addr_impfw_app5    },
    {OSAL_THREAD_IMPFW_APP_TASK06,  addr_impfw_app6    },
#endif /* defined(ENABLE_RESOURCE_IMP) */
#if defined(ENABLE_RESOURCE_RTT)
    {OSAL_THREAD_CPURTT_CB_TASK,    addr_cpurtt_cb     },
    {OSAL_THREAD_CPURTT_FBIST_TASK, addr_cpurtt_fbist  },
    {OSAL_THREAD_CPURTT_A1_TASK,    addr_cpurtt_a1     },
    {OSAL_THREAD_CPURTT_A21_TASK,   addr_cpurtt_a21    },
    {OSAL_THREAD_CPURTT_A22_TASK,   addr_cpurtt_a22    },
    {OSAL_THREAD_CPURTT_A23_TASK,   addr_cpurtt_a23    },
    {OSAL_THREAD_CPURTT_A24_TASK,   addr_cpurtt_a24    },
    {OSAL_THREAD_CPURTT_TP_TASK1,   addr_cpurtt_tp1    },
    {OSAL_THREAD_CPURTT_TP_TASK2,   addr_cpurtt_tp2    },
    {OSAL_THREAD_FBIST_TP_TASK1,    addr_fbist_tp1     },
    {OSAL_THREAD_FBIST_TP_TASK2,    addr_fbist_tp2     },
    {OSAL_THREAD_RFSO_TP_TASK1,     addr_rfso_tp1      },
    {OSAL_THREAD_RFSO_TP_TASK2,     addr_rfso_tp2      },
#endif /* defined(ENABLE_RESOURCE_RTT) */
#if defined(ENABLE_RESOURCE_IMR)
    {OSAL_THREAD_IMR_TASK00,        addr_imr00         },
    {OSAL_THREAD_IMR_TASK01,        addr_imr01         },
    {OSAL_THREAD_IMR_TASK02,        addr_imr02         },
    {OSAL_THREAD_IMR_TASK03,        addr_imr03         },
    {OSAL_THREAD_IMR_TASK04,        addr_imr04         },
    {OSAL_THREAD_IMR_TASK05,        addr_imr05         },
#endif /* defined(ENABLE_RESOURCE_IMR) */
#if defined(ENABLE_RESOURCE_VISIONIP)
    {OSAL_THREAD_VIP_TASK00,        addr_vip00         },
    {OSAL_THREAD_VIP_TASK01,        addr_vip01         },
    {OSAL_THREAD_VIP_TASK02,        addr_vip02         },
    {OSAL_THREAD_VIP_TASK03,        addr_vip03         },
    {OSAL_THREAD_VIP_TASK04,        addr_vip04         },
#endif /* defined(ENABLE_RESOURCE_VISIONIP) */
#if defined(ENABLE_RESOURCE_ISP)
    {OSAL_THREAD_ISP_TASK00,        addr_isp00         },
    {OSAL_THREAD_ISP_TASK01,        addr_isp01         },
    {OSAL_THREAD_ISP_TASK02,        addr_isp02         },
    {OSAL_THREAD_ISP_TASK03,        addr_isp03         },
#endif /* defined(ENABLE_RESOURCE_ISP) */

#if defined(ENABLE_RESOURCE_OSAL)
    {OSAL_THREAD_OSAL_00,           addr_osal00        },
    {OSAL_THREAD_OSAL_01,           addr_osal01        },
    {OSAL_THREAD_OSAL_02,           addr_osal02        },
    {OSAL_THREAD_OSAL_03,           addr_osal03        },
    {OSAL_THREAD_OSAL_04,           addr_osal04        },
    {OSAL_THREAD_OSAL_05,           addr_osal05        },
    {OSAL_THREAD_OSAL_06,           addr_osal06        },
    {OSAL_THREAD_OSAL_07,           addr_osal07        },
    {OSAL_THREAD_OSAL_08,           addr_osal08        },
    {OSAL_THREAD_OSAL_09,           addr_osal09        },
#endif /* defined(ENABLE_RESOURCE_OSAL) */

#endif /* (RCAR_V4H) */
    {.id = OSAL_THREAD_STACK_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : thread CPU affinity configuration table
***********************************************************************************************************************/
const st_osal_thread_affinity_t g_osal_thread_affinity[] =
{
    /* example:
      thread id,             CPU Affinity
    { OSAL_THREAD_OSAL_00,      0x5U },
    */

    {.id = OSAL_THREAD_AFFINITY_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_internal_thread_affinity
 * @brief osal resource configuration : thread CPU affinity information for PMA
***********************************************************************************************************************/
const uint8_t g_osal_internal_thread_affinity = 0x0;

/*******************************************************************************************************************//**
 * @var g_osal_mutex_config
 * @brief osal user configuration : mutex configuration table
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_rcfg_mutex_config_t g_osal_mutex_config[] =
{
#if defined(RCAR_V4H) || defined(RCAR_V4H2)
    /* mutex id               */
    {OSAL_MUTEX_IMR_00         },
    {OSAL_MUTEX_IMR_01         },
    {OSAL_MUTEX_IMR_02         },
    {OSAL_MUTEX_IMR_03         },
    {OSAL_MUTEX_IMR_04         },
    {OSAL_MUTEX_IMR_05         },
    {OSAL_MUTEX_IMR_CB00       },
    {OSAL_MUTEX_IMR_CB01       },
    {OSAL_MUTEX_IMR_CB02       },
    {OSAL_MUTEX_IMR_CB03       },
    {OSAL_MUTEX_IMR_CB04       },
    {OSAL_MUTEX_IMR_CB05       },
    {OSAL_MUTEX_DOF0_00        },
    {OSAL_MUTEX_DOF0_01        },
    {OSAL_MUTEX_STV0_00        },
    {OSAL_MUTEX_STV0_01        },
    {OSAL_MUTEX_ACF0_00        },
    {OSAL_MUTEX_ACF0_01        },
    {OSAL_MUTEX_ACF1_00        },
    {OSAL_MUTEX_ACF1_01        },
    {OSAL_MUTEX_ACF2_00        },
    {OSAL_MUTEX_ACF2_01        },
    {OSAL_MUTEX_ACF3_00        },
    {OSAL_MUTEX_ACF3_01        },
    {OSAL_MUTEX_ACF4_00        },
    {OSAL_MUTEX_ACF4_01        },
    {OSAL_MUTEX_VIPRTT_00      },
    {OSAL_MUTEX_VIPRTT_01      },
    {OSAL_MUTEX_VIPRTT_02      },
    {OSAL_MUTEX_VIPRTT_03      },
    {OSAL_MUTEX_SMD_PS0_00     },
    {OSAL_MUTEX_SMD_PS0_01     },
    {OSAL_MUTEX_SMD_POST_00    },
    {OSAL_MUTEX_SMD_POST_01    },
    {OSAL_MUTEX_ICCOM_WRITE_00 },
    {OSAL_MUTEX_ICCOM_WRITE_01 },
    {OSAL_MUTEX_ICCOM_WRITE_02 },
    {OSAL_MUTEX_ICCOM_WRITE_03 },
    {OSAL_MUTEX_ICCOM_WRITE_04 },
    {OSAL_MUTEX_ICCOM_WRITE_05 },
    {OSAL_MUTEX_ICCOM_WRITE_06 },
    {OSAL_MUTEX_ICCOM_WRITE_07 },
    {OSAL_MUTEX_ICCOM_READ_00  },
    {OSAL_MUTEX_ICCOM_READ_01  },
    {OSAL_MUTEX_ICCOM_READ_02  },
    {OSAL_MUTEX_ICCOM_READ_03  },
    {OSAL_MUTEX_ICCOM_READ_04  },
    {OSAL_MUTEX_ICCOM_READ_05  },
    {OSAL_MUTEX_ICCOM_READ_06  },
    {OSAL_MUTEX_ICCOM_READ_07  },
    {OSAL_MUTEX_ICCOM_TIMER    },
    {OSAL_MUTEX_ICCOM_OPEN     },
    {OSAL_MUTEX_IMP_FW_LOCK    },
    {OSAL_MUTEX_IMP_RTT_LOCK   },
    {OSAL_MUTEX_IMP_DRV_LOCK   },
    {OSAL_MUTEX_IMP_MEMORY_BARRIER_0   },
    {OSAL_MUTEX_IMP_MEMORY_BARRIER_1   },
    {OSAL_MUTEX_IMP_MEMORY_BARRIER_2   },
    {OSAL_MUTEX_IMP_MEMORY_BARRIER_3   },
    {OSAL_MUTEX_IMP_MEMORY_BARRIER_4   },
    {OSAL_MUTEX_IMP_MEMORY_BARRIER_5   },
    {OSAL_MUTEX_IMP_MEMORY_BARRIER_6   },
    {OSAL_MUTEX_IMP_CORE_STATE_0       },
    {OSAL_MUTEX_IMP_CORE_STATE_1       },
    {OSAL_MUTEX_IMP_CORE_STATE_2       },
    {OSAL_MUTEX_IMP_CORE_STATE_3       },
    {OSAL_MUTEX_IMP_CORE_STATE_4       },
    {OSAL_MUTEX_IMP_CORE_STATE_5       },
    {OSAL_MUTEX_IMP_CORE_STATE_6       },
    {OSAL_MUTEX_IMP_ATTR_STATE_0       },
    {OSAL_MUTEX_IMP_ATTR_STATE_1       },
    {OSAL_MUTEX_IMP_ATTR_STATE_2       },
    {OSAL_MUTEX_IMP_ATTR_STATE_3       },
    {OSAL_MUTEX_IMP_ATTR_STATE_4       },
    {OSAL_MUTEX_IMP_ATTR_STATE_5       },
    {OSAL_MUTEX_IMP_ATTR_STATE_6       },
    {OSAL_MUTEX_IMP_IMPFW_API_0        },
    {OSAL_MUTEX_IMP_IMPFW_API_1        },
    {OSAL_MUTEX_IMP_IMPFW_API_2        },
    {OSAL_MUTEX_IMP_IMPFW_API_3        },
    {OSAL_MUTEX_IMP_IMPFW_API_4        },
    {OSAL_MUTEX_IMP_IMPFW_API_5        },
    {OSAL_MUTEX_IMP_IMPFW_API_6        },
    {OSAL_MUTEX_CISP_00        },
    {OSAL_MUTEX_CISP_01        },
    {OSAL_MUTEX_TISP_00        },
    {OSAL_MUTEX_TISP_01        },
    {OSAL_MUTEX_CISP_02        },
    {OSAL_MUTEX_CISP_03        },
    {OSAL_MUTEX_TISP_02        },
    {OSAL_MUTEX_TISP_03        },
    {OSAL_MUTEX_VSPX_00        },
    {OSAL_MUTEX_VSPX_01        },
    {OSAL_MUTEX_VSPX_02        },
    {OSAL_MUTEX_VSPX_03        },
    {OSAL_MUTEX_CPURTT_00      },
    {OSAL_MUTEX_CPURTT_01      },
    {OSAL_MUTEX_CPURTT_02      },
    {OSAL_MUTEX_CPURTT_03      },
    {OSAL_MUTEX_CPURTT_04      },
    {OSAL_MUTEX_CPURTT_05      },
    {OSAL_MUTEX_CPURTT_06      },
    {OSAL_MUTEX_CPURTT_07      },
    {OSAL_MUTEX_CPURTT_08      },
    {OSAL_MUTEX_CPURTT_09      },
    {OSAL_MUTEX_CPURTT_10      },
    {OSAL_MUTEX_CPURTT_11      },
    {OSAL_MUTEX_CPURTT_12      },
    {OSAL_MUTEX_CPURTT_13      },
    {OSAL_MUTEX_CPURTT_14      },
    {OSAL_MUTEX_CPURTT_15      },
    {OSAL_MUTEX_CPURTT_16      },
    {OSAL_MUTEX_CPURTT_17      },
    {OSAL_MUTEX_CPURTT_18      },
    {OSAL_MUTEX_CPURTT_19      },
    {OSAL_MUTEX_CPURTT_20      },
    {OSAL_MUTEX_CPURTT_21      },
    {OSAL_MUTEX_OSAL_00        },
    {OSAL_MUTEX_OSAL_01        },
    {OSAL_MUTEX_OSAL_02        },
    {OSAL_MUTEX_OSAL_03        },
    {OSAL_MUTEX_OSAL_04        },
    {OSAL_MUTEX_OSAL_05        },
    {OSAL_MUTEX_OSAL_06        },
    {OSAL_MUTEX_OSAL_07        },
    {OSAL_MUTEX_OSAL_08        },
    {OSAL_MUTEX_OSAL_09        },
#endif
    {.id = OSAL_MUTEX_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_cond_config
 * @brief osal user configuration : cond configuration table
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_rcfg_cond_config_t g_osal_cond_config[] =
{
#if defined(RCAR_V4H) || defined(RCAR_V4H2)
    /* cond id               */
    {OSAL_COND_CPURTT_00    },
    {OSAL_COND_IMR_CB00     },
    {OSAL_COND_IMR_CB01     },
    {OSAL_COND_IMR_CB02     },
    {OSAL_COND_IMR_CB03     },
    {OSAL_COND_IMR_CB04     },
    {OSAL_COND_IMR_CB05     },
    {OSAL_COND_DOF_00       },
    {OSAL_COND_OSAL_00      },
    {OSAL_COND_OSAL_01      },
    {OSAL_COND_OSAL_02      },
    {OSAL_COND_OSAL_03      },
    {OSAL_COND_OSAL_04      },
    {OSAL_COND_OSAL_05      },
    {OSAL_COND_OSAL_06      },
    {OSAL_COND_OSAL_07      },
    {OSAL_COND_OSAL_08      },
    {OSAL_COND_OSAL_09      },
#endif
    {.id = OSAL_COND_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_mq_config
 * @brief osal user configuration : message queue configuration table
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_mq_rcfg_config_t g_osal_mq_config[] =
{
#if defined(RCAR_V4H) || defined(RCAR_V4H2)
    /* mq id,                       max_num_msg, msg_size */
    {OSAL_MQ_IMR_MSG_00,                     {1, 4}    },
    {OSAL_MQ_IMR_MSG_01,                     {1, 4}    },
    {OSAL_MQ_IMR_MSG_02,                     {1, 4}    },
    {OSAL_MQ_IMR_MSG_03,                     {1, 4}    },
    {OSAL_MQ_IMR_MSG_04,                     {1, 4}    },
    {OSAL_MQ_IMR_MSG_05,                     {1, 4}    },
    {OSAL_MQ_IMR_RTT_MSG_00,                 {1, 4}    },
    {OSAL_MQ_IMR_RTT_MSG_01,                 {1, 4}    },
    {OSAL_MQ_IMR_RTT_MSG_02,                 {1, 4}    },
    {OSAL_MQ_IMR_RTT_MSG_03,                 {1, 4}    },
    {OSAL_MQ_IMR_RTT_MSG_04,                 {1, 4}    },
    {OSAL_MQ_IMR_RTT_MSG_05,                 {1, 4}    },
    {OSAL_MQ_ICCOM_MGR_TASK,                 {8, 16}   },
    {OSAL_MQ_ICCOM_REQ_OPEN,                 {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_00,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_01,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_02,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_03,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_04,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_05,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_06,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_UPPER_07,       {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_00,      {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_01,      {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_02,      {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_03,      {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_04,      {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_05,      {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_06,      {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_07,      {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_00,          {3, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_01,          {3, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_02,          {3, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_03,          {3, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_04,          {3, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_05,          {3, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_06,          {3, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_07,          {3, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_00,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_01,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_02,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_03,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_04,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_05,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_06,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_TASK_07,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_00,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_01,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_02,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_03,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_04,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_05,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_06,             {1, 16}   },
    {OSAL_MQ_ICCOM_READ_TASK_07,             {1, 16}   },
    {OSAL_MQ_ICCOM_SEND_DATA_00,             {1, 2048} },
    {OSAL_MQ_ICCOM_SEND_DATA_01,             {1, 2048} },
    {OSAL_MQ_ICCOM_SEND_DATA_02,             {1, 2048} },
    {OSAL_MQ_ICCOM_SEND_DATA_03,             {1, 2048} },
    {OSAL_MQ_ICCOM_SEND_DATA_04,             {1, 2048} },
    {OSAL_MQ_ICCOM_SEND_DATA_05,             {1, 2048} },
    {OSAL_MQ_ICCOM_SEND_DATA_06,             {1, 2048} },
    {OSAL_MQ_ICCOM_SEND_DATA_07,             {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_00,          {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_01,          {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_02,          {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_03,          {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_04,          {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_05,          {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_06,          {1, 2048} },
    {OSAL_MQ_ICCOM_RECEIVE_DATA_07,          {1, 2048} },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_00,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_01,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_02,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_03,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_04,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_05,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_06,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_07,    {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_00, {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_01, {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_02, {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_03, {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_04, {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_05, {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_06, {1, 16}   },
    {OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_07, {1, 16}   },
    {OSAL_MQ_IMP_RTT_TASK_REQ,               {28, 8}   },
    {OSAL_MQ_IMP_RTT_TASK_ACK,               {2, 4}    },
    {OSAL_MQ_IMP_RTT_DRVCB_00,               {2, 4}    },
    {OSAL_MQ_IMP_RTT_EXECCB,                 {2, 4}    },
    {OSAL_MQ_IMP_DRV_TASK_CBFUNC,            {40, 24}  },
    {OSAL_MQ_IMP_DRV_TASK_DELTASK,           {2, 4}    },
    {OSAL_MQ_IMP_IMPFW_SEND_0,               {63, 272} },
    {OSAL_MQ_IMP_IMPFW_SEND_1,               {63, 272} },
    {OSAL_MQ_IMP_IMPFW_SEND_2,               {63, 272} },
    {OSAL_MQ_IMP_IMPFW_SEND_3,               {63, 272} },
    {OSAL_MQ_IMP_IMPFW_SEND_4,               {63, 272} },
    {OSAL_MQ_IMP_IMPFW_SEND_5,               {63, 272} },
    {OSAL_MQ_IMP_IMPFW_SEND_6,               {63, 272} },
    {OSAL_MQ_IMP_IMPFW_RECEIVE_0,            {63, 8}   },
    {OSAL_MQ_IMP_IMPFW_RECEIVE_1,            {63, 8}   },
    {OSAL_MQ_IMP_IMPFW_RECEIVE_2,            {63, 8}   },
    {OSAL_MQ_IMP_IMPFW_RECEIVE_3,            {63, 8}   },
    {OSAL_MQ_IMP_IMPFW_RECEIVE_4,            {63, 8}   },
    {OSAL_MQ_IMP_IMPFW_RECEIVE_5,            {63, 8}   },
    {OSAL_MQ_IMP_IMPFW_RECEIVE_6,            {63, 8}   },
    {OSAL_MQ_IMP_IMPFW_CBFUNC_0,             {63, 32}  },
    {OSAL_MQ_IMP_IMPFW_CBFUNC_1,             {63, 32}  },
    {OSAL_MQ_IMP_IMPFW_CBFUNC_2,             {63, 32}  },
    {OSAL_MQ_IMP_IMPFW_CBFUNC_3,             {63, 32}  },
    {OSAL_MQ_IMP_IMPFW_CBFUNC_4,             {63, 32}  },
    {OSAL_MQ_IMP_IMPFW_CBFUNC_5,             {63, 32}  },
    {OSAL_MQ_IMP_IMPFW_CBFUNC_6,             {63, 32}  },
    {OSAL_MQ_IMP_IMPFW_RTT_SAMPLE,           {396, 32} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_0,        {63, 264} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_1,        {63, 264} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_2,        {63, 264} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_3,        {63, 264} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_4,        {63, 264} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_5,        {63, 264} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_SEND_6,        {63, 264} },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_0,     {63, 2}   },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_1,     {63, 2}   },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_2,     {63, 2}   },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_3,     {63, 2}   },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_4,     {63, 2}   },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_5,     {63, 2}   },
    {OSAL_MQ_IMP_IMPFW_ZEPHYR_RECEIVE_6,     {63, 2}   },
    {OSAL_MQ_CPURTT_TEST00,                  { 1,  1}  },
    {OSAL_MQ_CPURTT_TEST01,                  {128,16}  },
    {OSAL_MQ_CPURTT_RFSO,                    {128,16}  },
    {OSAL_MQ_CPURTT_TEST02,                  {128,16}  },
    {OSAL_MQ_CPURTT_TEST03,                  {128,16}  },
    {OSAL_MQ_CPURTT_TEST04,                  {128,16}  },
    {OSAL_MQ_FBIST_TEST00,                   {128,16}  },
    {OSAL_MQ_FBIST_TEST01,                   {128,16}  },
    {OSAL_MQ_RFSO_TEST00,                    {128,16}  },
    {OSAL_MQ_RFSO_TEST01,                    {128,16}  },
    {OSAL_MQ_HWARTT_SAMPLE,                  {48, 32}  },
    {OSAL_MQ_ISP_00,                         {16, 64}  },
    {OSAL_MQ_ISP_01,                         {16, 64}  },
    {OSAL_MQ_ISP_02,                         {16, 64}  },
    {OSAL_MQ_ISP_03,                         {16, 64}  },
    {OSAL_MQ_OSAL_00,                        { 2,  4}  },
    {OSAL_MQ_OSAL_01,                        {16, 64}  },
    {OSAL_MQ_OSAL_02,                        {16, 64}  },
    {OSAL_MQ_OSAL_03,                        {16, 64}  },
    {OSAL_MQ_OSAL_04,                        {16, 64}  },
    {OSAL_MQ_OSAL_05,                        {16,196}  },
    {OSAL_MQ_OSAL_06,                        {16,196}  },
    {OSAL_MQ_OSAL_07,                        {16,196}  },
    {OSAL_MQ_OSAL_08,                        {16,196}  },
    {OSAL_MQ_OSAL_09,                        {16,196}  },
#endif
    {.id = OSAL_MQ_INVALID_ID}
};

/*======================================================================================================================
End of File
======================================================================================================================*/
