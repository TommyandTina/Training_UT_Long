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
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Configurator for specific OS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal_configuration/target/qnx/r_osal_configuration.h"

/*======================================================================================================================
 Private macro definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @def RESORCE_ID
 * @brief Resource IDs
***********************************************************************************************************************/
/* PRQA S 3214 1 # Do not change because the user is a public source. */
#define RESORCE_ID_RESERVE  0x0000U
#define RESORCE_ID_IMR      0x1000U
#define RESORCE_ID_VIP      0x2000U
#define RESORCE_ID_ICCOM    0x3000U
#define RESORCE_ID_LDE      0x4000U
#define RESORCE_ID_CODEC    0x4000U
#define RESORCE_ID_IMP      0x5000U
#define RESORCE_ID_ISP      0x6000U
#define RESORCE_ID_CPURTT   0x7000U
#define RESORCE_ID_HWARTT   0xa000U
#define RESORCE_ID_PAP      0xb000U
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

#define    OSAL_THREAD_LDE_CAPUTRE_TASK              (RESORCE_ID_LDE   +  0U)
#define    OSAL_THREAD_LDE_OUTPUT_TASK               (RESORCE_ID_LDE   +  1U)
#define    OSAL_THREAD_LDE_CTX_TASK                  (RESORCE_ID_LDE   +  2U)
#define    OSAL_THREAD_CODEC_ENC_CAP_TASK            (RESORCE_ID_CODEC +  3U)
#define    OSAL_THREAD_CODEC_ENC_OUT_TASK            (RESORCE_ID_CODEC +  4U)
#define    OSAL_THREAD_CODEC_ENC_CTX_TASK            (RESORCE_ID_CODEC +  5U)
#define    OSAL_THREAD_CODEC_DEC_TASK                (RESORCE_ID_CODEC +  6U)
#define    OSAL_THREAD_CODEC_OUT_TASK                (RESORCE_ID_CODEC +  7U)

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

#define    OSAL_THREAD_PAP_TASK00                    (RESORCE_ID_PAP   +  0U)

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

#define    OSAL_MUTEX_LDE_LIB_00                     (RESORCE_ID_LDE   +  0U)
#define    OSAL_MUTEX_LDE_LIB_01                     (RESORCE_ID_LDE   +  1U)
#define    OSAL_MUTEX_LDE_LIB_02                     (RESORCE_ID_LDE   +  2U)
#define    OSAL_MUTEX_LDE_APP_00                     (RESORCE_ID_LDE   +  3U)
#define    OSAL_MUTEX_CODEC_LIB_00                   (RESORCE_ID_CODEC +  4U)
#define    OSAL_MUTEX_CODEC_LIB_01                   (RESORCE_ID_CODEC +  5U)
#define    OSAL_MUTEX_CODEC_LIB_02                   (RESORCE_ID_CODEC +  6U)
#define    OSAL_MUTEX_CODEC_LIB_03                   (RESORCE_ID_CODEC +  7U)
#define    OSAL_MUTEX_CODEC_APP_00                   (RESORCE_ID_CODEC +  8U)
#define    OSAL_MUTEX_CODEC_APP_01                   (RESORCE_ID_CODEC +  9U)
#define    OSAL_MUTEX_CODEC_APP_02                   (RESORCE_ID_CODEC + 10U)
#define    OSAL_MUTEX_CODEC_APP_03                   (RESORCE_ID_CODEC + 11U)
#define    OSAL_MUTEX_CODEC_APP_04                   (RESORCE_ID_CODEC + 12U)
#define    OSAL_MUTEX_CODEC_APP_05                   (RESORCE_ID_CODEC + 13U)
#define    OSAL_MUTEX_CODEC_APP_06                   (RESORCE_ID_CODEC + 14U)
#define    OSAL_MUTEX_CODEC_LOG_00                   (RESORCE_ID_CODEC + 15U)
#define    OSAL_MUTEX_CODEC_LOG_01                   (RESORCE_ID_CODEC + 16U)

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

#define    OSAL_MUTEX_PAP_00                         (RESORCE_ID_PAP   +  0U)
#define    OSAL_MUTEX_PAP_01                         (RESORCE_ID_PAP   +  1U)
#define    OSAL_MUTEX_PAP_02                         (RESORCE_ID_PAP   +  2U)
#define    OSAL_MUTEX_PAP_03                         (RESORCE_ID_PAP   +  3U)

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

#define    OSAL_COND_PAP_00                          (RESORCE_ID_PAP  +  0U)
#define    OSAL_COND_PAP_01                          (RESORCE_ID_PAP  +  1U)
#define    OSAL_COND_PAP_02                          (RESORCE_ID_PAP  +  2U)

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

#define    OSAL_MQ_LDE_APP_MAIN                      (RESORCE_ID_LDE   +  0U)
#define    OSAL_MQ_LDE_APP_OUT                       (RESORCE_ID_LDE   +  1U)
#define    OSAL_MQ_LDE_APP_CTX                       (RESORCE_ID_LDE   +  2U)
#define    OSAL_MQ_CODEC_ENC_APP_MAIN                (RESORCE_ID_CODEC +  3U)
#define    OSAL_MQ_CODEC_ENC_APP_OUT                 (RESORCE_ID_CODEC +  4U)
#define    OSAL_MQ_CODEC_ENC_APP_CTX                 (RESORCE_ID_CODEC +  5U)
#define    OSAL_MQ_CODEC_DEC_MAIN                    (RESORCE_ID_CODEC +  6U)
#define    OSAL_MQ_CODEC_DEC_OUT                     (RESORCE_ID_CODEC +  7U)
#define    OSAL_MQ_CODEC_DEC_SRM                     (RESORCE_ID_CODEC +  8U)
#define    OSAL_MQ_CODEC_DEC_PIC                     (RESORCE_ID_CODEC +  9U)

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

#define    OSAL_MQ_PAP_00                            (RESORCE_ID_PAP   +  0U)
#define    OSAL_MQ_PAP_01                            (RESORCE_ID_PAP   +  1U)
#define    OSAL_MQ_PAP_02                            (RESORCE_ID_PAP   +  2U)
#define    OSAL_MQ_PAP_03                            (RESORCE_ID_PAP   +  3U)

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

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var g_osal_device_soc_type
 * @brief osal device configuration : SoC type
***********************************************************************************************************************/
const st_osal_device_soc_type_t g_osal_device_soc_type =
{
#if defined(RCAR_V3M)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V3M
#elif defined(RCAR_V3H1)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V3H1
#elif defined(RCAR_V3H2)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V3H2
#elif defined(RCAR_V4H)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4H
#elif defined(RCAR_V4M)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4M
#elif defined(RCAR_S4)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_S4
#else
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_INVALID
#endif
};

/*******************************************************************************************************************//**
 * @var g_osal_memory_max_configration
 * @brief osal resource configuration : max memory configuration structure
***********************************************************************************************************************/
const st_osal_mmngr_config_t g_osal_memory_max_configration =
{
    .mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT,
    .memory_limit = (512U * 1024U * 1024U),
    .max_allowed_allocations = (4U * 1024U),
    .max_registered_monitors_cbs = (4U * 1024U)
};

/*******************************************************************************************************************//**
 * @var g_osal_memory_region_info
 * @brief osal resource configuration : memory region information structure
***********************************************************************************************************************/
const st_osal_mmngr_region_info_list_t g_osal_memory_region_info_list =
{
    .num_of_memory_regions = 2,
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
#if defined(RCAR_V4H) || defined(RCAR_V4M)
        /* Memory region 0(default) */
        {
            .name = "",
            .offset = 0x80000000,           /* The default region requires "0x80000000 - 0x9FFFFFFF" */
            .range = (512U * 1024U * 1024U)
        },
        /* Memory region 1 */ /* example */
        {
            .name = "",
            .offset = 0x600000000,          /* region 1, "0x600000000 - 0x604000000" */
            .range = (64U * 1024U * 1024U)
        }
#else
        /* Memory region 0(default) */
        {
            .name = "sysram&below4G",      /* The default region requires "below 4G area" */
            .offset = 0,
            .range = (512U * 1024U * 1024U)
        },
        /* Memory region 1 */ /* example */
        {
            .name = "",
            .offset = 0,                    /* if offset is 0, the allocated area is OS depenent */
            .range = (64U * 1024U * 1024U)
        }
#endif
    }
};

/*******************************************************************************************************************//**
 * @var g_osal_thread_priority_matrix
 * @brief osal resource configuration : Matrix of thread priorities
***********************************************************************************************************************/
const int32_t g_osal_thread_priority_matrix[15] =
{
        1,          /* OSAL_THREAD_PRIORITY_TYPE0  */
        2,          /* OSAL_THREAD_PRIORITY_TYPE1  */
        3,          /* OSAL_THREAD_PRIORITY_TYPE2  */
        4,          /* OSAL_THREAD_PRIORITY_TYPE3  */
        5,          /* OSAL_THREAD_PRIORITY_TYPE4  */
        6,          /* OSAL_THREAD_PRIORITY_TYPE5  */
        7,          /* OSAL_THREAD_PRIORITY_TYPE6  */
        8,          /* OSAL_THREAD_PRIORITY_TYPE7  */
        9,          /* OSAL_THREAD_PRIORITY_TYPE8  */
        10,         /* OSAL_THREAD_PRIORITY_TYPE9  */
        11,         /* OSAL_THREAD_PRIORITY_TYPE10 */
        12,         /* OSAL_THREAD_PRIORITY_TYPE11 */
        13,         /* OSAL_THREAD_PRIORITY_TYPE12 */
        14,         /* OSAL_THREAD_PRIORITY_TYPE13 */
        15          /* OSAL_THREAD_PRIORITY_TYPE14 */
};

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_priority_matrix
 * @brief osal device configuration : Matrix of interrupt thread priorities
***********************************************************************************************************************/
const int32_t g_osal_interrupt_thread_priority_matrix[15] =
{
        16,         /* OSAL_INTERRUPT_PRIORITY_TYPE0  */
        17,         /* OSAL_INTERRUPT_PRIORITY_TYPE1  */
        18,         /* OSAL_INTERRUPT_PRIORITY_TYPE2  */
        19,         /* OSAL_INTERRUPT_PRIORITY_TYPE3  */
        20,         /* OSAL_INTERRUPT_PRIORITY_TYPE4  */
        21,         /* OSAL_INTERRUPT_PRIORITY_TYPE5  */
        22,         /* OSAL_INTERRUPT_PRIORITY_TYPE6  */
        23,         /* OSAL_INTERRUPT_PRIORITY_TYPE7  */
        24,         /* OSAL_INTERRUPT_PRIORITY_TYPE8  */
        25,         /* OSAL_INTERRUPT_PRIORITY_TYPE9  */
        26,         /* OSAL_INTERRUPT_PRIORITY_TYPE10 */
        27,         /* OSAL_INTERRUPT_PRIORITY_TYPE11 */
        28,         /* OSAL_INTERRUPT_PRIORITY_TYPE12 */
        29,         /* OSAL_INTERRUPT_PRIORITY_TYPE13 */
        30          /* OSAL_INTERRUPT_PRIORITY_TYPE14 */
};

/*******************************************************************************************************************//**
 * @var g_osal_internal_thread_affinity
 * @brief osal resource configuration : CPU Affinity value for threads managed independently by OSAL
***********************************************************************************************************************/
const uint64_t g_osal_internal_thread_affinity = 0x0U;

/*******************************************************************************************************************//**
 * @var g_osal_thread_config
 * @brief osal resource configuration : thread configuration table
***********************************************************************************************************************/
const st_osal_rcfg_thread_config_t g_osal_thread_config[] =
{
    /* thread id,                   func, userarg, priority,                 task_name,          stack_size */
    {OSAL_THREAD_ICCOM_DEMO_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom demor task",    0x1000} },
    {OSAL_THREAD_ICCOM_MGR_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom manager task",  0x1000} },
    {OSAL_THREAD_ICCOM_TIMER_TASK,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom timer task",    0x1000} },
    {OSAL_THREAD_ICCOM_RCV_TASK0,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task0", 0x1800} },
    {OSAL_THREAD_ICCOM_RCV_TASK1,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task1", 0x1800} },
    {OSAL_THREAD_ICCOM_RCV_TASK2,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task2", 0x1800} },
    {OSAL_THREAD_ICCOM_RCV_TASK3,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task3", 0x1800} },
    {OSAL_THREAD_ICCOM_RCV_TASK4,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task4", 0x1800} },
    {OSAL_THREAD_ICCOM_RCV_TASK5,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task5", 0x1800} },
    {OSAL_THREAD_ICCOM_RCV_TASK6,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task6", 0x1800} },
    {OSAL_THREAD_ICCOM_RCV_TASK7,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task7", 0x1800} },
    {OSAL_THREAD_ICCOM_READ_TASK0,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task0",    0x1000} },
    {OSAL_THREAD_ICCOM_READ_TASK1,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task1",    0x1000} },
    {OSAL_THREAD_ICCOM_READ_TASK2,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task2",    0x1000} },
    {OSAL_THREAD_ICCOM_READ_TASK3,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task3",    0x1000} },
    {OSAL_THREAD_ICCOM_READ_TASK4,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task4",    0x1000} },
    {OSAL_THREAD_ICCOM_READ_TASK5,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task5",    0x1000} },
    {OSAL_THREAD_ICCOM_READ_TASK6,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task6",    0x1000} },
    {OSAL_THREAD_ICCOM_READ_TASK7,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task7",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK0,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task0",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK1,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task1",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK2,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task2",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK3,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task3",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK4,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task4",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK5,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task5",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK6,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task6",    0x1000} },
    {OSAL_THREAD_ICCOM_SEND_TASK7,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task7",    0x1000} },
    {OSAL_THREAD_IMP_RTT_DRV,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp rtt drv task",    0x2000} },
    {OSAL_THREAD_IMP_DRV,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp drv task",        0x2000} },
    {OSAL_THREAD_IMPFW_TASK00,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task00",        0x4000} },
    {OSAL_THREAD_IMPFW_TASK01,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task01",        0x4000} },
    {OSAL_THREAD_IMPFW_TASK02,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task02",        0x4000} },
    {OSAL_THREAD_IMPFW_TASK03,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task03",        0x4000} },
    {OSAL_THREAD_IMPFW_TASK04,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task04",        0x4000} },
    {OSAL_THREAD_IMPFW_TASK05,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task05",        0x4000} },
    {OSAL_THREAD_IMPFW_TASK06,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_task06",        0x4000} },
    {OSAL_THREAD_IMPFW_APP_TASK00,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_app_task00",    0x2000} },
    {OSAL_THREAD_IMPFW_APP_TASK01,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_app_task01",    0x2000} },
    {OSAL_THREAD_IMPFW_APP_TASK02,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_app_task02",    0x2000} },
    {OSAL_THREAD_IMPFW_APP_TASK03,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_app_task03",    0x2000} },
    {OSAL_THREAD_IMPFW_APP_TASK04,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_app_task04",    0x2000} },
    {OSAL_THREAD_IMPFW_APP_TASK05,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_app_task05",    0x2000} },
    {OSAL_THREAD_IMPFW_APP_TASK06,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "impfw_app_task06",    0x2000} },
    {OSAL_THREAD_LDE_CAPUTRE_TASK,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpLdeTaskCap",      0x2000} },
    {OSAL_THREAD_LDE_OUTPUT_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpLdeTaskOut",      0x2000} },
    {OSAL_THREAD_LDE_CTX_TASK,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpLdeTaskCtx",      0x2000} },
    {OSAL_THREAD_CODEC_ENC_CAP_TASK,{NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpEncTaskCap",      0x2000} },
    {OSAL_THREAD_CODEC_ENC_OUT_TASK,{NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpEncTaskOut",      0x2000} },
    {OSAL_THREAD_CODEC_ENC_CTX_TASK,{NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpEncTaskCtx",      0x2000} },
    {OSAL_THREAD_CODEC_DEC_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpTaskDec",         0x2000} },
    {OSAL_THREAD_CODEC_OUT_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpTaskOut",         0x2000} },
    {OSAL_THREAD_CPURTT_CB_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "callback_task",       0x1000} },
    {OSAL_THREAD_CPURTT_FBIST_TASK, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "fbist_fin_task",      0x1000} },
    {OSAL_THREAD_CPURTT_A1_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a1_task",      0x1000} },
    {OSAL_THREAD_CPURTT_A21_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a21_task",     0x1000} },
    {OSAL_THREAD_CPURTT_A22_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a22_task",     0x1000} },
    {OSAL_THREAD_CPURTT_A23_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a23_task",     0x1000} },
    {OSAL_THREAD_CPURTT_A24_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_a24_task",     0x1000} },
    {OSAL_THREAD_CPURTT_TP_TASK1,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_tp_task1",     0x1000} },
    {OSAL_THREAD_CPURTT_TP_TASK2,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "cpurtt_tp_task2",     0x1000} },
    {OSAL_THREAD_FBIST_TP_TASK1,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "fbist_tp_task1",      0x1000} },
    {OSAL_THREAD_FBIST_TP_TASK2,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "fbist_tp_task2",      0x1000} },
    {OSAL_THREAD_RFSO_TP_TASK1,     {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rfso_tp_task1",       0x1000} },
    {OSAL_THREAD_RFSO_TP_TASK2,     {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rfso_tp_task2",       0x1000} },
    {OSAL_THREAD_IMR_TASK00,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task00",          0x2000} },
    {OSAL_THREAD_IMR_TASK01,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task01",          0x2000} },
    {OSAL_THREAD_IMR_TASK02,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task02",          0x2000} },
    {OSAL_THREAD_IMR_TASK03,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task03",          0x2000} },
    {OSAL_THREAD_IMR_TASK04,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task04",          0x2000} },
    {OSAL_THREAD_IMR_TASK05,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imr_task05",          0x2000} },
    {OSAL_THREAD_VIP_TASK00,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task00",        0x200000} },
    {OSAL_THREAD_VIP_TASK01,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task01",        0x200000} },
    {OSAL_THREAD_VIP_TASK02,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task02",        0x200000} },
    {OSAL_THREAD_VIP_TASK03,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task03",        0x200000} },
    {OSAL_THREAD_VIP_TASK04,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "vip_task04",        0x200000} },
    {OSAL_THREAD_ISP_TASK00,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task00",        0x200000} },
    {OSAL_THREAD_ISP_TASK01,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task01",        0x200000} },
    {OSAL_THREAD_ISP_TASK02,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task02",        0x200000} },
    {OSAL_THREAD_ISP_TASK03,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "isp_task03",        0x200000} },
    {OSAL_THREAD_PAP_TASK00,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "pap_task00",        0x200000} },

    {OSAL_THREAD_OSAL_00,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task01",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_01,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task02",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_02,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task03",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_03,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task04",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_04,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task05",         2 * 1024 * 1024} },
    {OSAL_THREAD_OSAL_05,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task06",         0x2000} },
    {OSAL_THREAD_OSAL_06,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task07",         0x2000} },
    {OSAL_THREAD_OSAL_07,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task08",         0x2000} },
    {OSAL_THREAD_OSAL_08,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task09",         0x2000} },
    {OSAL_THREAD_OSAL_09,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task10",         0x2000} },

    {.id = OSAL_THREAD_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : CPU affinity value for thread
***********************************************************************************************************************/
const st_osal_thread_affinity_t g_osal_thread_affinity[] =
{
    /* All default CPU Affinity */
    {.id = OSAL_THREAD_AFFINITY_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_mutex_config
 * @brief osal user configuration : mutex configuration table
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_rcfg_mutex_config_t g_osal_mutex_config[] =
{
    /* mutex id               */
    /*
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
    {OSAL_MUTEX_LDE_LIB_00     },
    {OSAL_MUTEX_LDE_LIB_01     },
    {OSAL_MUTEX_LDE_LIB_02     },
    {OSAL_MUTEX_LDE_APP_00     },
    {OSAL_MUTEX_CODEC_LIB_00   },
    {OSAL_MUTEX_CODEC_LIB_01   },
    {OSAL_MUTEX_CODEC_LIB_02   },
    {OSAL_MUTEX_CODEC_LIB_03   },
    {OSAL_MUTEX_CODEC_APP_00   },
    {OSAL_MUTEX_CODEC_APP_01   },
    {OSAL_MUTEX_CODEC_APP_02   },
    {OSAL_MUTEX_CODEC_APP_03   },
    {OSAL_MUTEX_CODEC_APP_04   },
    {OSAL_MUTEX_CODEC_APP_05   },
    {OSAL_MUTEX_CODEC_APP_06   },
    {OSAL_MUTEX_CODEC_LOG_00   },
    {OSAL_MUTEX_CODEC_LOG_01   },
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
    {OSAL_MUTEX_PAP_00         },
    {OSAL_MUTEX_PAP_01         },
    {OSAL_MUTEX_PAP_02         },
    {OSAL_MUTEX_PAP_03         },
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
    */

    /* There is no configuration of mutex. */
    {.id = OSAL_MUTEX_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_cond_config
 * @brief osal user configuration : cond configuration table
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_rcfg_cond_config_t g_osal_cond_config[] =
{
    /* cond id               */
    {OSAL_COND_CPURTT_00    },
    {OSAL_COND_IMR_CB00     },
    {OSAL_COND_IMR_CB01     },
    {OSAL_COND_IMR_CB02     },
    {OSAL_COND_IMR_CB03     },
    {OSAL_COND_IMR_CB04     },
    {OSAL_COND_IMR_CB05     },
    {OSAL_COND_DOF_00       },
    {OSAL_COND_PAP_00       },
    {OSAL_COND_PAP_01       },
    {OSAL_COND_PAP_02       },
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

    {.id = OSAL_COND_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_mq_config
 * @brief osal user configuration : message queue configuration table
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_mq_rcfg_config_t g_osal_mq_config[] =
{
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
    {OSAL_MQ_ICCOM_RECEIVE_TASK_00,          {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_01,          {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_02,          {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_03,          {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_04,          {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_05,          {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_06,          {1, 16}   },
    {OSAL_MQ_ICCOM_RECEIVE_TASK_07,          {1, 16}   },
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
    {OSAL_MQ_LDE_APP_MAIN,                   {16, 64}  },
    {OSAL_MQ_LDE_APP_OUT,                    {16, 64}  },
    {OSAL_MQ_LDE_APP_CTX,                    {16, 196} },
    {OSAL_MQ_CODEC_ENC_APP_MAIN,             {16, 64}  },
    {OSAL_MQ_CODEC_ENC_APP_OUT,              {16, 64}  },
    {OSAL_MQ_CODEC_ENC_APP_CTX,              {16, 196} },
    {OSAL_MQ_CODEC_DEC_MAIN,                 {16, 64}  },
    {OSAL_MQ_CODEC_DEC_OUT,                  {16, 64}  },
    {OSAL_MQ_CODEC_DEC_SRM,                  {16, 64}  },
    {OSAL_MQ_CODEC_DEC_PIC,                  {16, 196} },
    {OSAL_MQ_CPURTT_TEST00,                  { 1,  1}  },
    {OSAL_MQ_CPURTT_TEST01,                  { 1, 16}  },
    {OSAL_MQ_CPURTT_RFSO,                    { 1, 16}  },
    {OSAL_MQ_CPURTT_TEST02,                  { 1, 16}  },
    {OSAL_MQ_CPURTT_TEST03,                  { 1, 16}  },
    {OSAL_MQ_CPURTT_TEST04,                  { 1, 16}  },
    {OSAL_MQ_FBIST_TEST00,                   { 1, 16}  },
    {OSAL_MQ_FBIST_TEST01,                   { 1, 16}  },
    {OSAL_MQ_RFSO_TEST00,                    { 1, 16}  },
    {OSAL_MQ_RFSO_TEST01,                    { 1, 16}  },
    {OSAL_MQ_HWARTT_SAMPLE,                  {48, 32}  },
    {OSAL_MQ_PAP_00,                         { 2,  4}  },
    {OSAL_MQ_PAP_01,                         { 4, 80}  },
    {OSAL_MQ_PAP_02,                         { 1,  4}  },
    {OSAL_MQ_PAP_03,                         { 1,128}  },
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

    {.id = OSAL_MQ_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_config
 * @brief osal user configuration : interrupt thread configuration table
***********************************************************************************************************************/
#if defined(RCAR_V3H1)  /* V3H1 */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task192",           0x0400,      60000000},
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task193",           0x0400,      60000000},
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task194",           0x0400,      60000000},
    {195, "ims_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task195",           0x0400,      60000000},
    {254, "imr_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task254",           0x0400,      60000000},
    {255, "imr_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task255",           0x0400,      60000000},
    {242, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task242",           0x0400,      60000000},
    {243, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task243",           0x0400,      60000000},
    {238, "stv_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task238",           0x0400,      60000000},
    {239, "stv_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task239",           0x0400,      60000000},
    { 71, "acf_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task071",           0x0400,      60000000},
    { 97, "acf_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task097",           0x0400,      60000000},
    { 98, "acf_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task098",           0x0400,      60000000},
    { 99, "acf_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task099",           0x0400,      60000000},
    {100, "acf_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task100",           0x0400,      60000000},
    {101, "acf_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task101",           0x0400,      60000000},
    {102, "acf_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task102",           0x0400,      60000000},
    {103, "acf_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task103",           0x0400,      60000000},
    {104, "acf_04",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task104",           0x0400,      60000000},
    {108, "acf_04",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task108",           0x0400,      60000000},
    {224, "iccom_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task224",           0x0400,      60000000},
    {225, "iccom_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task225",           0x0400,      60000000},
    {226, "iccom_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task226",           0x0400,      60000000},
    {227, "iccom_03",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task227",           0x0400,      60000000},
    {228, "iccom_04",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task228",           0x0400,      60000000},
    {229, "iccom_05",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task229",           0x0400,      60000000},
    {230, "iccom_06",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task230",           0x0400,      60000000},
    {231, "iccom_07",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task231",           0x0400,      60000000},
    {300, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task300",           0x0400,      60000000},
    {170, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task170",           0x0400,      60000000},
    {281, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task281",           0x2000,      60000000},
    {282, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task282",           0x2000,      60000000},
    {283, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task283",           0x2000,      60000000},
    {284, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task284",           0x2000,      60000000},
    {285, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task285",           0x2000,      60000000},
    { 26, "cisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task026",           0x1000,      60000000},
    {109, "cisp_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task109",           0x1000,      60000000},
    { 25, "tisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task025",           0x1000,      60000000},
    { 27, "tisp_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task027",           0x1000,      60000000},
    { 14, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task014",           0x1000,      60000000},
    { 15, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task015",           0x1000,      60000000},
    { 32, "rfso_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task032",           0x1000,      60000000},
    { 33, "rfso_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task033",           0x1000,      60000000},
    { 34, "rfso_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task034",           0x1000,      60000000},
    { 35, "rfso_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task035",           0x1000,      60000000},
    { 36, "rfso_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task036",           0x1000,      60000000},
    { 37, "rfso_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task037",           0x1000,      60000000},
    {221, "rfso_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task221",           0x1000,      60000000},
    {186, "rfso_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task186",           0x1000,      60000000},
    {187, "rfso_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task187",           0x1000,      60000000},
    {185, "rfso_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task185",           0x1000,      60000000},
    {247, "rfso_10",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task247",           0x1000,      60000000},
    {196, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task196",           0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V3H2)    /* V3H2 */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task192",           0x0400,      60000000},
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task193",           0x0400,      60000000},
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task194",           0x0400,      60000000},
    {254, "imr_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task254",           0x0400,      60000000},
    {255, "imr_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task255",           0x0400,      60000000},
    {242, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task242",           0x0400,      60000000},
    {243, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task243",           0x0400,      60000000},
    {238, "stv_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task238",           0x0400,      60000000},
    {239, "stv_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task239",           0x0400,      60000000},
    { 71, "acf_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task071",           0x0400,      60000000},
    { 97, "acf_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task097",           0x0400,      60000000},
    { 98, "acf_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task098",           0x0400,      60000000},
    { 99, "acf_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task099",           0x0400,      60000000},
    {100, "acf_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task100",           0x0400,      60000000},
    {101, "acf_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task101",           0x0400,      60000000},
    {102, "acf_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task102",           0x0400,      60000000},
    {103, "acf_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task103",           0x0400,      60000000},
    {104, "acf_04",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task104",           0x0400,      60000000},
    {108, "acf_04",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task108",           0x0400,      60000000},
    {224, "iccom_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task224",           0x0400,      60000000},
    {225, "iccom_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task225",           0x0400,      60000000},
    {226, "iccom_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task226",           0x0400,      60000000},
    {227, "iccom_03",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task227",           0x0400,      60000000},
    {228, "iccom_04",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task228",           0x0400,      60000000},
    {229, "iccom_05",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task229",           0x0400,      60000000},
    {230, "iccom_06",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task230",           0x0400,      60000000},
    {231, "iccom_07",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task231",           0x0400,      60000000},
    {300, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task300",           0x0400,      60000000},
    {170, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task170",           0x0400,      60000000},
    {281, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task281",           0x2000,      60000000},
    {282, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task282",           0x2000,      60000000},
    {283, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task283",           0x2000,      60000000},
    {284, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task284",           0x2000,      60000000},
    {285, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task285",           0x2000,      60000000},
    { 26, "cisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task026",           0x1000,      60000000},
    {109, "cisp_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task109",           0x1000,      60000000},
    { 25, "tisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task025",           0x1000,      60000000},
    { 27, "tisp_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task027",           0x1000,      60000000},
    { 14, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task014",           0x1000,      60000000},
    { 15, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task015",           0x1000,      60000000},
    { 32, "rfso_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task032",           0x1000,      60000000},
    { 33, "rfso_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task033",           0x1000,      60000000},
    { 34, "rfso_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task034",           0x1000,      60000000},
    { 35, "rfso_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task035",           0x1000,      60000000},
    { 36, "rfso_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task036",           0x1000,      60000000},
    { 37, "rfso_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task037",           0x1000,      60000000},
    {221, "rfso_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task221",           0x1000,      60000000},
    {186, "rfso_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task186",           0x1000,      60000000},
    {187, "rfso_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task187",           0x1000,      60000000},
    {185, "rfso_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task185",           0x1000,      60000000},
    {247, "rfso_10",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task247",           0x1000,      60000000},
    {196, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task196",           0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V3M)     /* V3M */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task192",           0x0400,      60000000},
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task193",           0x0400,      60000000},
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task194",           0x0400,      60000000},
    {195, "ims_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task195",           0x0400,      60000000},
    {224, "iccom_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task224",           0x0400,      60000000},
    {225, "iccom_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task225",           0x0400,      60000000},
    {226, "iccom_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task226",           0x0400,      60000000},
    {227, "iccom_03",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task227",           0x0400,      60000000},
    {228, "iccom_04",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task228",           0x0400,      60000000},
    {229, "iccom_05",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task229",           0x0400,      60000000},
    {230, "iccom_06",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task230",           0x0400,      60000000},
    {231, "iccom_07",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task231",           0x0400,      60000000},
    {300, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task300",           0x0400,      60000000},
    {170, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task170",           0x0400,      60000000},
    {281, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task281",           0x2000,      60000000},
    {282, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task282",           0x2000,      60000000},
    {283, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task283",           0x2000,      60000000},
    {284, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task284",           0x2000,      60000000},
    {285, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task285",           0x2000,      60000000},
    { 26, "cisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task026",           0x1000,      60000000},
    { 25, "tisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task025",           0x1000,      60000000},
    { 14, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task014",           0x1000,      60000000},
    { 15, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task015",           0x1000,      60000000},
    { 32, "rfso_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task032",           0x1000,      60000000},
    { 33, "rfso_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task033",           0x1000,      60000000},
    { 34, "rfso_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task034",           0x1000,      60000000},
    { 35, "rfso_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task035",           0x1000,      60000000},
    { 36, "rfso_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task036",           0x1000,      60000000},
    { 37, "rfso_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task037",           0x1000,      60000000},
    {221, "rfso_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task221",           0x1000,      60000000},
    {186, "rfso_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task186",           0x1000,      60000000},
    {187, "rfso_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task187",           0x1000,      60000000},
    {185, "rfso_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task185",           0x1000,      60000000},
    {247, "rfso_10",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task247",           0x1000,      60000000},
    {196, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task196",           0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V4H)     /* V4H */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {579, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task579",           0x0400,      60000000},
    {580, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task580",           0x0400,      60000000},
    {581, "imr_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task581",           0x0400,      60000000},
    {582, "imr_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task582",           0x0400,      60000000},
    {583, "imr_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task583",           0x0400,      60000000},
    {565, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task565",           0x2000,      60000000},
    {566, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task566",           0x2000,      60000000},
    {567, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task567",           0x2000,      60000000},
    {478, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task478",           0x2000,      60000000},
    {479, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task479",           0x2000,      60000000},
    {480, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task480",           0x2000,      60000000},
    {481, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task481",           0x2000,      60000000},
    {482, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task482",           0x2000,      60000000},
    {483, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task483",           0x2000,      60000000},
    {484, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task484",           0x2000,      60000000},
    {485, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task485",           0x2000,      60000000},
    {486, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task486",           0x2000,      60000000},
    {487, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task487",           0x2000,      60000000},
    {488, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task488",           0x2000,      60000000},
    {489, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task489",           0x2000,      60000000},
    {490, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task490",           0x2000,      60000000},
    {491, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task491",           0x2000,      60000000},
    {492, "vdsp02_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task492",           0x2000,      60000000},
    {493, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task493",           0x2000,      60000000},
    {494, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task494",           0x2000,      60000000},
    {495, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task495",           0x2000,      60000000},
    {496, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task496",           0x2000,      60000000},
    {497, "vdsp03_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task497",           0x2000,      60000000},
    {861, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task861",           0x0400,      60000000},
    {862, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task862",           0x0400,      60000000},
    {873, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task873",           0x0400,      60000000},
    {874, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task874",           0x0400,      60000000},
    {871, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task871",           0x0400,      60000000},
    {872, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task872",           0x0400,      60000000},
    {464, "gsx_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task464",           0x1000,      60000000},
    {123, "iccom_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task123",           0x0400,      60000000},
    {125, "iccom_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task125",           0x0400,      60000000},
    {127, "iccom_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task127",           0x0400,      60000000},
    {129, "iccom_03",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task129",           0x0400,      60000000},
    {131, "iccom_04",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task131",           0x0400,      60000000},
    {133, "iccom_05",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task133",           0x0400,      60000000},
    {135, "iccom_06",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task135",           0x0400,      60000000},
    {137, "iccom_07",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task137",           0x0400,      60000000},
    {916, "iccom_08",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task916",           0x0400,      60000000},
    {918, "iccom_09",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task918",           0x0400,      60000000},
    {920, "iccom_10",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task920",           0x0400,      60000000},
    {922, "iccom_11",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task922",           0x0400,      60000000},
    {855, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task855",           0x0400,      60000000},
    {856, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task856",           0x0400,      60000000},
    {858, "vcp4_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task858",           0x0400,      60000000},
    {859, "vcp4_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task859",           0x0400,      60000000},
    {475, "cisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task475",           0x1000,      60000000},
    {473, "tisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task473",           0x1000,      60000000},
    {476, "cisp_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task476",           0x1000,      60000000},
    {474, "tisp_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task474",           0x1000,      60000000},
    {556, "vspx_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task556",           0x1000,      60000000},
    {557, "vspx_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task557",           0x1000,      60000000},
    {852, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task852",           0x1000,      60000000},
    {695, "fba_ims0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task695",           0x1000,      60000000},
    {696, "fba_ims1",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task696",           0x1000,      60000000},
    {691, "fba_imr0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task691",           0x1000,      60000000},
    {692, "fba_imr1",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task692",           0x1000,      60000000},
    {693, "fba_imr2",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task693",           0x1000,      60000000},
    {713, "fba_imp0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task713",           0x1000,      60000000},
    {714, "fba_imp1",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task714",           0x1000,      60000000},
    {715, "fba_imp2",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task715",           0x1000,      60000000},
    {716, "fba_imp3",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task716",           0x1000,      60000000},
    {719, "fba_imp_ocv0",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task719",           0x1000,      60000000},
    {720, "fba_imp_ocv1",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task720",           0x1000,      60000000},
    {721, "fba_imp_ocv2",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task721",           0x1000,      60000000},
    {722, "fba_imp_ocv3",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task722",           0x1000,      60000000},
    {717, "fba_imp_dp0",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task717",           0x1000,      60000000},
    {718, "fba_imp_dp1",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task718",           0x1000,      60000000},
    {727, "fba_imp_cnn0",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task727",           0x1000,      60000000},
    {738, "fba_imp_dsp0",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task738",           0x1000,      60000000},
    {739, "fba_imp_dsp1",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task739",           0x1000,      60000000},
    {740, "fba_imp_dsp2",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task740",           0x1000,      60000000},
    {741, "fba_imp_dsp3",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task741",           0x1000,      60000000},
    {744, "fba_imp_cnram0",         OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task744",           0x1000,      60000000},
    {745, "fba_imp_slim0",          OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task745",           0x1000,      60000000},
    {746, "fba_imp_slim1",          OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task746",           0x1000,      60000000},
    {701, "fba_umfl0",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task701",           0x1000,      60000000},
    {703, "fba_smps0",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task703",           0x1000,      60000000},
    {707, "fba_smpo0",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task707",           0x1000,      60000000},
    {699, "fba_pap0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task699",           0x1000,      60000000},
    {686, "fba_isp0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task686",           0x1000,      60000000},
    {687, "fba_isp1",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task687",           0x1000,      60000000},
    {669, "fba_ca76d0",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task669",           0x1000,      60000000},
    {670, "fba_ca76d1",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task670",           0x1000,      60000000},
    {671, "fba_ca760",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task671",           0x1000,      60000000},
    {672, "fba_ca761",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task672",           0x1000,      60000000},
    {673, "fba_ca762",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task673",           0x1000,      60000000},
    {674, "fba_ca763",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task674",           0x1000,      60000000},
    {227, "rfso_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task227",           0x1000,      60000000},
    {228, "rfso_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task228",           0x1000,      60000000},
    {229, "rfso_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task229",           0x1000,      60000000},
    {230, "rfso_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task230",           0x1000,      60000000},
    {231, "rfso_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task231",           0x1000,      60000000},
    {232, "rfso_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task232",           0x1000,      60000000},
    {233, "rfso_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task233",           0x1000,      60000000},
    {234, "rfso_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task234",           0x1000,      60000000},
    {235, "rfso_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task235",           0x1000,      60000000},
    {236, "rfso_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task236",           0x1000,      60000000},
    {237, "rfso_10",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task237",           0x1000,      60000000},
    {210, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task210",           0x1000,      60000000},
    {211, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task211",           0x1000,      60000000},
    {900, "caiplite_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task900",           0x1000,      60000000},
    {902, "caiplite_01",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task902",           0x1000,      60000000},
    {904, "caiplite_02",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task904",           0x1000,      60000000},
    {906, "caiplite_03",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task906",           0x1000,      60000000},
    {908, "caiplite_04",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task908",           0x1000,      60000000},
    {910, "caiplite_05",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task910",           0x1000,      60000000},
    {912, "caiplite_06",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task912",           0x1000,      60000000},
    {914, "caiplite_07",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task914",           0x1000,      60000000},
    { 10, "rtdmac_control0_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task010",           0x0400,      60000000},
    { 11, "rtdmac_control0_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task011",           0x0400,      60000000},
    { 12, "rtdmac_control0_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task012",           0x0400,      60000000},
    { 13, "rtdmac_control0_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task013",           0x0400,      60000000},
    { 14, "rtdmac_control0_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task014",           0x0400,      60000000},
    { 15, "rtdmac_control0_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task015",           0x0400,      60000000},
    { 16, "rtdmac_control0_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task016",           0x0400,      60000000},
    { 17, "rtdmac_control0_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task017",           0x0400,      60000000},
    { 18, "rtdmac_control0_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task018",           0x0400,      60000000},
    { 19, "rtdmac_control0_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task019",           0x0400,      60000000},
    { 20, "rtdmac_control0_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task020",           0x0400,      60000000},
    { 21, "rtdmac_control0_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task021",           0x0400,      60000000},
    { 22, "rtdmac_control0_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task022",           0x0400,      60000000},
    { 23, "rtdmac_control0_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task023",           0x0400,      60000000},
    { 24, "rtdmac_control0_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task024",           0x0400,      60000000},
    { 25, "rtdmac_control0_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task025",           0x0400,      60000000},
    { 28, "rtdmac_control1_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task028",           0x0400,      60000000},
    { 29, "rtdmac_control1_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task029",           0x0400,      60000000},
    { 30, "rtdmac_control1_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task030",           0x0400,      60000000},
    { 31, "rtdmac_control1_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task031",           0x0400,      60000000},
    { 32, "rtdmac_control1_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task032",           0x0400,      60000000},
    { 33, "rtdmac_control1_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task033",           0x0400,      60000000},
    { 34, "rtdmac_control1_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task034",           0x0400,      60000000},
    { 35, "rtdmac_control1_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task035",           0x0400,      60000000},
    { 36, "rtdmac_control1_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task036",           0x0400,      60000000},
    { 37, "rtdmac_control1_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task037",           0x0400,      60000000},
    { 38, "rtdmac_control1_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task038",           0x0400,      60000000},
    { 39, "rtdmac_control1_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task039",           0x0400,      60000000},
    { 40, "rtdmac_control1_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task040",           0x0400,      60000000},
    { 41, "rtdmac_control1_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task041",           0x0400,      60000000},
    { 42, "rtdmac_control1_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task042",           0x0400,      60000000},
    { 43, "rtdmac_control1_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task043",           0x0400,      60000000},
    { 46, "rtdmac_control2_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task046",           0x0400,      60000000},
    { 47, "rtdmac_control2_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task047",           0x0400,      60000000},
    { 48, "rtdmac_control2_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task048",           0x0400,      60000000},
    { 49, "rtdmac_control2_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task049",           0x0400,      60000000},
    { 50, "rtdmac_control2_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task050",           0x0400,      60000000},
    { 51, "rtdmac_control2_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task051",           0x0400,      60000000},
    { 52, "rtdmac_control2_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task052",           0x0400,      60000000},
    { 53, "rtdmac_control2_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task053",           0x0400,      60000000},
    { 54, "rtdmac_control2_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task054",           0x0400,      60000000},
    { 55, "rtdmac_control2_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task055",           0x0400,      60000000},
    { 56, "rtdmac_control2_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task056",           0x0400,      60000000},
    { 57, "rtdmac_control2_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task057",           0x0400,      60000000},
    { 58, "rtdmac_control2_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task058",           0x0400,      60000000},
    { 59, "rtdmac_control2_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task059",           0x0400,      60000000},
    { 60, "rtdmac_control2_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task060",           0x0400,      60000000},
    { 61, "rtdmac_control2_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task061",           0x0400,      60000000},
    { 64, "rtdmac_control3_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task064",           0x0400,      60000000},
    { 65, "rtdmac_control3_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task065",           0x0400,      60000000},
    { 66, "rtdmac_control3_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task066",           0x0400,      60000000},
    { 67, "rtdmac_control3_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task067",           0x0400,      60000000},
    { 68, "rtdmac_control3_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task068",           0x0400,      60000000},
    { 69, "rtdmac_control3_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task069",           0x0400,      60000000},
    { 70, "rtdmac_control3_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task070",           0x0400,      60000000},
    { 71, "rtdmac_control3_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task071",           0x0400,      60000000},
    { 72, "rtdmac_control3_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task072",           0x0400,      60000000},
    { 73, "rtdmac_control3_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task073",           0x0400,      60000000},
    { 74, "rtdmac_control3_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task074",           0x0400,      60000000},
    { 75, "rtdmac_control3_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task075",           0x0400,      60000000},
    { 76, "rtdmac_control3_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task076",           0x0400,      60000000},
    { 77, "rtdmac_control3_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task077",           0x0400,      60000000},
    { 78, "rtdmac_control3_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task078",           0x0400,      60000000},
    { 79, "rtdmac_control3_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task079",           0x0400,      60000000},
    {529, "vin_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task529",           0x0400,      60000000},
    {530, "vin_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task530",           0x0400,      60000000},
    {531, "vin_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task531",           0x0400,      60000000},
    {532, "vin_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task532",           0x0400,      60000000},
    {533, "vin_04",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task533",           0x0400,      60000000},
    {534, "vin_05",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task534",           0x0400,      60000000},
    {535, "vin_06",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task535",           0x0400,      60000000},
    {536, "vin_07",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task536",           0x0400,      60000000},
    {537, "vin_10",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task537",           0x0400,      60000000},
    {538, "vin_11",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task538",           0x0400,      60000000},
    {539, "vin_12",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task539",           0x0400,      60000000},
    {540, "vin_13",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task540",           0x0400,      60000000},
    {541, "vin_14",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task541",           0x0400,      60000000},
    {542, "vin_15",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task542",           0x0400,      60000000},
    {543, "vin_16",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task543",           0x0400,      60000000},
    {544, "vin_17",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task544",           0x0400,      60000000},
    {546, "vsp_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task546",           0x0400,      60000000},
    {551, "vsp_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task551",           0x0400,      60000000},
    {499, "csi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task499",           0x0400,      60000000},
    {500, "csi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task500",           0x0400,      60000000},
    {523, "du_00",                  OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task523",           0x0400,      60000000},
    {524, "du_00",                  OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task524",           0x0400,      60000000},
    {502, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task502",           0x0400,      60000000},
    {503, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task503",           0x0400,      60000000},
    {504, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task504",           0x0400,      60000000},
    {505, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task505",           0x0400,      60000000},
    {506, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task506",           0x0400,      60000000},
    {507, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task507",           0x0400,      60000000},
    {508, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task508",           0x0400,      60000000},
    {509, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task509",           0x0400,      60000000},
    {510, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task510",           0x0400,      60000000},
    {511, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task511",           0x0400,      60000000},
    {512, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task512",           0x0400,      60000000},
    {513, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task513",           0x0400,      60000000},
    {514, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task514",           0x0400,      60000000},
    {515, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task515",           0x0400,      60000000},
    {516, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task516",           0x0400,      60000000},
    {517, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task517",           0x0400,      60000000},
    {518, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task518",           0x0400,      60000000},
    {519, "dsi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task519",           0x0400,      60000000},
    {310, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task310",           0x0400,      60000000},
    {311, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task311",           0x0400,      60000000},
    {312, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task312",           0x0400,      60000000},
    {313, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task313",           0x0400,      60000000},
    {314, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task314",           0x0400,      60000000},
    {315, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task315",           0x0400,      60000000},
    {316, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task316",           0x0400,      60000000},
    {317, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task317",           0x0400,      60000000},
    {318, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task318",           0x0400,      60000000},
    {319, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task319",           0x0400,      60000000},
    {320, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task320",           0x0400,      60000000},
    {321, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task321",           0x0400,      60000000},
    {322, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task322",           0x0400,      60000000},
    {323, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task323",           0x0400,      60000000},
    {324, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task324",           0x0400,      60000000},
    {325, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task325",           0x0400,      60000000},
    {326, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task326",           0x0400,      60000000},
    {327, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task327",           0x0400,      60000000},
    {328, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task328",           0x0400,      60000000},
    {329, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task329",           0x0400,      60000000},
    {526, "doc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task526",           0x0400,      60000000},
    {527, "doc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task527",           0x0400,      60000000},
    {559, "dsc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task559",           0x0400,      60000000},
    {213, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task213",           0x0400,      60000000},
    {214, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task214",           0x0400,      60000000},
    {223, "fbs_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task223",           0x0400,      60000000},
    {878, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task878",           0x0400,      60000000},
    {879, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task879",           0x0400,      60000000},
    {880, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task880",           0x0400,      60000000},
    {881, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task881",           0x0400,      60000000},
    {882, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task882",           0x0400,      60000000},
    {883, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task883",           0x0400,      60000000},
    {884, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task884",           0x0400,      60000000},
    {885, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task885",           0x0400,      60000000},
    {886, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task886",           0x0400,      60000000},
    {887, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task887",           0x0400,      60000000},
    {888, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task888",           0x0400,      60000000},
    {889, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task889",           0x0400,      60000000},
    {890, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task890",           0x0400,      60000000},
    {891, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task891",           0x0400,      60000000},
    {892, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task892",           0x0400,      60000000},
    {893, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task893",           0x0400,      60000000},
    {894, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task894",           0x0400,      60000000},
    {895, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task895",           0x0400,      60000000},
    {896, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task896",           0x0400,      60000000},
    {897, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task897",           0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V4M)     /* V4M */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {579, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task579",           0x0400,      60000000},
    {580, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task580",           0x0400,      60000000},
    {581, "imr_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task581",           0x0400,      60000000},
    {582, "imr_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task582",           0x0400,      60000000},
    {565, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task565",           0x2000,      60000000},
    {566, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task566",           0x2000,      60000000},
    {567, "imp_top_00",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task567",           0x2000,      60000000},
    {478, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task478",           0x2000,      60000000},
    {479, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task479",           0x2000,      60000000},
    {480, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task480",           0x2000,      60000000},
    {481, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task481",           0x2000,      60000000},
    {482, "vdsp00_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task482",           0x2000,      60000000},
    {483, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task483",           0x2000,      60000000},
    {484, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task484",           0x2000,      60000000},
    {485, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task485",           0x2000,      60000000},
    {486, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task486",           0x2000,      60000000},
    {487, "vdsp01_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task487",           0x2000,      60000000},
    {861, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task861",           0x0400,      60000000},
    {862, "dof_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task862",           0x0400,      60000000},
    {873, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task873",           0x0400,      60000000},
    {874, "smd_ps_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task874",           0x0400,      60000000},
    {871, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task871",           0x0400,      60000000},
    {872, "smd_post_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task872",           0x0400,      60000000},
    {464, "gsx_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task464",           0x1000,      60000000},
    {123, "iccom_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task123",           0x0400,      60000000},
    {125, "iccom_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task125",           0x0400,      60000000},
    {127, "iccom_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task127",           0x0400,      60000000},
    {129, "iccom_03",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task129",           0x0400,      60000000},
    {131, "iccom_04",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task131",           0x0400,      60000000},
    {133, "iccom_05",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task133",           0x0400,      60000000},
    {135, "iccom_06",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task135",           0x0400,      60000000},
    {137, "iccom_07",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task137",           0x0400,      60000000},
    {916, "iccom_08",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task916",           0x0400,      60000000},
    {918, "iccom_09",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task918",           0x0400,      60000000},
    {920, "iccom_10",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task920",           0x0400,      60000000},
    {922, "iccom_11",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task922",           0x0400,      60000000},
    {855, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task855",           0x0400,      60000000},
    {856, "ivcp1e_00",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task856",           0x0400,      60000000},
    {475, "cisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task475",           0x1000,      60000000},
    {473, "tisp_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task473",           0x1000,      60000000},
    {474, "tisp_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task474",           0x1000,      60000000},
    {556, "vspx_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task556",           0x1000,      60000000},
    {852, "fbc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task852",           0x1000,      60000000},
    {695, "fba_ims0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task695",           0x1000,      60000000},
    {696, "fba_ims1",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task696",           0x1000,      60000000},
    {691, "fba_imr0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task691",           0x1000,      60000000},
    {692, "fba_imr1",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task692",           0x1000,      60000000},
    {713, "fba_imp0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task713",           0x1000,      60000000},
    {714, "fba_imp1",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task714",           0x1000,      60000000},
    {719, "fba_imp_ocv0",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task719",           0x1000,      60000000},
    {720, "fba_imp_ocv1",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task720",           0x1000,      60000000},
    {721, "fba_imp_ocv2",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task721",           0x1000,      60000000},
    {722, "fba_imp_ocv3",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task722",           0x1000,      60000000},
    {717, "fba_imp_dp0",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task717",           0x1000,      60000000},
    {718, "fba_imp_dp1",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task718",           0x1000,      60000000},
    {727, "fba_imp_cnn0",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task727",           0x1000,      60000000},
    {738, "fba_imp_dsp0",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task738",           0x1000,      60000000},
    {739, "fba_imp_dsp1",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task739",           0x1000,      60000000},
    {744, "fba_imp_cnram0",         OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task744",           0x1000,      60000000},
    {701, "fba_umfl0",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task701",           0x1000,      60000000},
    {703, "fba_smps0",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task703",           0x1000,      60000000},
    {707, "fba_smpo0",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task707",           0x1000,      60000000},
    {699, "fba_pap0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task699",           0x1000,      60000000},
    {686, "fba_isp0",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task686",           0x1000,      60000000},
    {669, "fba_ca76d0",             OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task669",           0x1000,      60000000},
    {671, "fba_ca760",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task671",           0x1000,      60000000},
    {672, "fba_ca761",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task672",           0x1000,      60000000},
    {673, "fba_ca762",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task673",           0x1000,      60000000},
    {674, "fba_ca763",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task674",           0x1000,      60000000},
    {227, "rfso_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task227",           0x1000,      60000000},
    {228, "rfso_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task228",           0x1000,      60000000},
    {229, "rfso_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task229",           0x1000,      60000000},
    {230, "rfso_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task230",           0x1000,      60000000},
    {231, "rfso_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task231",           0x1000,      60000000},
    {232, "rfso_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task232",           0x1000,      60000000},
    {233, "rfso_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task233",           0x1000,      60000000},
    {234, "rfso_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task234",           0x1000,      60000000},
    {235, "rfso_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task235",           0x1000,      60000000},
    {236, "rfso_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task236",           0x1000,      60000000},
    {237, "rfso_10",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task237",           0x1000,      60000000},
    {210, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task210",           0x1000,      60000000},
    {211, "ipmmu_mm_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task211",           0x1000,      60000000},
    {900, "caiplite_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task900",           0x1000,      60000000},
    {902, "caiplite_01",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task902",           0x1000,      60000000},
    {904, "caiplite_02",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task904",           0x1000,      60000000},
    {906, "caiplite_03",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task906",           0x1000,      60000000},
    {908, "caiplite_04",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task908",           0x1000,      60000000},
    {910, "caiplite_05",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task910",           0x1000,      60000000},
    {912, "caiplite_06",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task912",           0x1000,      60000000},
    {914, "caiplite_07",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task914",           0x1000,      60000000},
    { 10, "rtdmac_control0_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task010",           0x0400,      60000000},
    { 11, "rtdmac_control0_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task011",           0x0400,      60000000},
    { 12, "rtdmac_control0_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task012",           0x0400,      60000000},
    { 13, "rtdmac_control0_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task013",           0x0400,      60000000},
    { 14, "rtdmac_control0_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task014",           0x0400,      60000000},
    { 15, "rtdmac_control0_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task015",           0x0400,      60000000},
    { 16, "rtdmac_control0_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task016",           0x0400,      60000000},
    { 17, "rtdmac_control0_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task017",           0x0400,      60000000},
    { 18, "rtdmac_control0_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task018",           0x0400,      60000000},
    { 19, "rtdmac_control0_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task019",           0x0400,      60000000},
    { 20, "rtdmac_control0_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task020",           0x0400,      60000000},
    { 21, "rtdmac_control0_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task021",           0x0400,      60000000},
    { 22, "rtdmac_control0_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task022",           0x0400,      60000000},
    { 23, "rtdmac_control0_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task023",           0x0400,      60000000},
    { 24, "rtdmac_control0_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task024",           0x0400,      60000000},
    { 25, "rtdmac_control0_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task025",           0x0400,      60000000},
    { 28, "rtdmac_control1_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task028",           0x0400,      60000000},
    { 29, "rtdmac_control1_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task029",           0x0400,      60000000},
    { 30, "rtdmac_control1_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task030",           0x0400,      60000000},
    { 31, "rtdmac_control1_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task031",           0x0400,      60000000},
    { 32, "rtdmac_control1_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task032",           0x0400,      60000000},
    { 33, "rtdmac_control1_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task033",           0x0400,      60000000},
    { 34, "rtdmac_control1_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task034",           0x0400,      60000000},
    { 35, "rtdmac_control1_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task035",           0x0400,      60000000},
    { 36, "rtdmac_control1_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task036",           0x0400,      60000000},
    { 37, "rtdmac_control1_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task037",           0x0400,      60000000},
    { 38, "rtdmac_control1_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task038",           0x0400,      60000000},
    { 39, "rtdmac_control1_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task039",           0x0400,      60000000},
    { 40, "rtdmac_control1_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task040",           0x0400,      60000000},
    { 41, "rtdmac_control1_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task041",           0x0400,      60000000},
    { 42, "rtdmac_control1_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task042",           0x0400,      60000000},
    { 43, "rtdmac_control1_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task043",           0x0400,      60000000},
    {529, "vin_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task529",           0x0400,      60000000},
    {530, "vin_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task530",           0x0400,      60000000},
    {531, "vin_02",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task531",           0x0400,      60000000},
    {532, "vin_03",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task532",           0x0400,      60000000},
    {533, "vin_04",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task533",           0x0400,      60000000},
    {534, "vin_05",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task534",           0x0400,      60000000},
    {535, "vin_06",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task535",           0x0400,      60000000},
    {536, "vin_07",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task536",           0x0400,      60000000},
    {537, "vin_10",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task537",           0x0400,      60000000},
    {538, "vin_11",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task538",           0x0400,      60000000},
    {539, "vin_12",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task539",           0x0400,      60000000},
    {540, "vin_13",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task540",           0x0400,      60000000},
    {541, "vin_14",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task541",           0x0400,      60000000},
    {542, "vin_15",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task542",           0x0400,      60000000},
    {543, "vin_16",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task543",           0x0400,      60000000},
    {544, "vin_17",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task544",           0x0400,      60000000},
    {546, "vsp_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task546",           0x0400,      60000000},
    {499, "csi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task499",           0x0400,      60000000},
    {500, "csi_01",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task500",           0x0400,      60000000},
    {523, "du_00",                  OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task523",           0x0400,      60000000},
    {524, "du_00",                  OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task524",           0x0400,      60000000},
    {502, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task502",           0x0400,      60000000},
    {503, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task503",           0x0400,      60000000},
    {504, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task504",           0x0400,      60000000},
    {505, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task505",           0x0400,      60000000},
    {506, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task506",           0x0400,      60000000},
    {507, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task507",           0x0400,      60000000},
    {508, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task508",           0x0400,      60000000},
    {509, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task509",           0x0400,      60000000},
    {510, "dsi_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task510",           0x0400,      60000000},
    {310, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task310",           0x0400,      60000000},
    {311, "wwdt_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task311",           0x0400,      60000000},
    {312, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task312",           0x0400,      60000000},
    {313, "wwdt_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task313",           0x0400,      60000000},
    {314, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task314",           0x0400,      60000000},
    {315, "wwdt_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task315",           0x0400,      60000000},
    {316, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task316",           0x0400,      60000000},
    {317, "wwdt_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task317",           0x0400,      60000000},
    {318, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task318",           0x0400,      60000000},
    {319, "wwdt_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task319",           0x0400,      60000000},
    {320, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task320",           0x0400,      60000000},
    {321, "wwdt_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task321",           0x0400,      60000000},
    {322, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task322",           0x0400,      60000000},
    {323, "wwdt_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task323",           0x0400,      60000000},
    {324, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task324",           0x0400,      60000000},
    {325, "wwdt_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task325",           0x0400,      60000000},
    {326, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task326",           0x0400,      60000000},
    {327, "wwdt_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task327",           0x0400,      60000000},
    {328, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task328",           0x0400,      60000000},
    {329, "wwdt_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task329",           0x0400,      60000000},
    {526, "doc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task526",           0x0400,      60000000},
    {527, "doc_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task527",           0x0400,      60000000},
    {213, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task213",           0x0400,      60000000},
    {214, "sdmac_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task214",           0x0400,      60000000},
    {223, "fbs_00",                 OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task223",           0x0400,      60000000},
    {878, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task878",           0x0400,      60000000},
    {879, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task879",           0x0400,      60000000},
    {880, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task880",           0x0400,      60000000},
    {881, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task881",           0x0400,      60000000},
    {882, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task882",           0x0400,      60000000},
    {883, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task883",           0x0400,      60000000},
    {884, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task884",           0x0400,      60000000},
    {885, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task885",           0x0400,      60000000},
    {886, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task886",           0x0400,      60000000},
    {887, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task887",           0x0400,      60000000},
    {888, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task888",           0x0400,      60000000},
    {889, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task889",           0x0400,      60000000},
    {890, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task890",           0x0400,      60000000},
    {891, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task891",           0x0400,      60000000},
    {892, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task892",           0x0400,      60000000},
    {893, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task893",           0x0400,      60000000},
    {894, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task894",           0x0400,      60000000},
    {895, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task895",           0x0400,      60000000},
    {896, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task896",           0x0400,      60000000},
    {897, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task897",           0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_S4) || defined(RCAR_S4N)  /* S4 & S4N */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {933, "caiplite_00",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task933",           0x0400,      60000000},
    {935, "caiplite_01",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task935",           0x0400,      60000000},
    {937, "caiplite_02",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task937",           0x0400,      60000000},
    {939, "caiplite_03",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task939",           0x0400,      60000000},
    {941, "caiplite_04",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task941",           0x0400,      60000000},
    {943, "caiplite_05",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task943",           0x0400,      60000000},
    {945, "caiplite_06",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task945",           0x0400,      60000000},
    {947, "caiplite_07",            OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task947",           0x0400,      60000000},
    { 30, "rtdmac_control0_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task030",           0x0400,      60000000},
    { 31, "rtdmac_control0_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task031",           0x0400,      60000000},
    { 32, "rtdmac_control0_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task032",           0x0400,      60000000},
    { 33, "rtdmac_control0_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task033",           0x0400,      60000000},
    { 34, "rtdmac_control0_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task034",           0x0400,      60000000},
    { 35, "rtdmac_control0_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task035",           0x0400,      60000000},
    { 36, "rtdmac_control0_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task036",           0x0400,      60000000},
    { 37, "rtdmac_control0_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task037",           0x0400,      60000000},
    { 38, "rtdmac_control0_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task038",           0x0400,      60000000},
    { 39, "rtdmac_control0_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task039",           0x0400,      60000000},
    { 40, "rtdmac_control0_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task040",           0x0400,      60000000},
    { 41, "rtdmac_control0_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task041",           0x0400,      60000000},
    { 42, "rtdmac_control0_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task042",           0x0400,      60000000},
    { 43, "rtdmac_control0_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task043",           0x0400,      60000000},
    { 44, "rtdmac_control0_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task044",           0x0400,      60000000},
    { 45, "rtdmac_control0_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task045",           0x0400,      60000000},
    { 48, "rtdmac_control1_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task048",           0x0400,      60000000},
    { 49, "rtdmac_control1_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task049",           0x0400,      60000000},
    { 50, "rtdmac_control1_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task050",           0x0400,      60000000},
    { 51, "rtdmac_control1_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task051",           0x0400,      60000000},
    { 52, "rtdmac_control1_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task052",           0x0400,      60000000},
    { 53, "rtdmac_control1_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task053",           0x0400,      60000000},
    { 54, "rtdmac_control1_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task054",           0x0400,      60000000},
    { 55, "rtdmac_control1_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task055",           0x0400,      60000000},
    { 56, "rtdmac_control1_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task056",           0x0400,      60000000},
    { 57, "rtdmac_control1_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task057",           0x0400,      60000000},
    { 58, "rtdmac_control1_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task058",           0x0400,      60000000},
    { 59, "rtdmac_control1_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task059",           0x0400,      60000000},
    { 60, "rtdmac_control1_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task060",           0x0400,      60000000},
    { 61, "rtdmac_control1_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task061",           0x0400,      60000000},
    { 62, "rtdmac_control1_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task062",           0x0400,      60000000},
    { 63, "rtdmac_control1_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task063",           0x0400,      60000000},
    { 66, "rtdmac_control2_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task066",           0x0400,      60000000},
    { 67, "rtdmac_control2_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task067",           0x0400,      60000000},
    { 68, "rtdmac_control2_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task068",           0x0400,      60000000},
    { 69, "rtdmac_control2_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task069",           0x0400,      60000000},
    { 70, "rtdmac_control2_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task070",           0x0400,      60000000},
    { 71, "rtdmac_control2_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task071",           0x0400,      60000000},
    { 72, "rtdmac_control2_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task072",           0x0400,      60000000},
    { 73, "rtdmac_control2_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task073",           0x0400,      60000000},
    { 74, "rtdmac_control2_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task074",           0x0400,      60000000},
    { 75, "rtdmac_control2_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task075",           0x0400,      60000000},
    { 76, "rtdmac_control2_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task076",           0x0400,      60000000},
    { 77, "rtdmac_control2_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task077",           0x0400,      60000000},
    { 78, "rtdmac_control2_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task078",           0x0400,      60000000},
    { 79, "rtdmac_control2_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task079",           0x0400,      60000000},
    { 80, "rtdmac_control2_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task080",           0x0400,      60000000},
    { 81, "rtdmac_control2_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task081",           0x0400,      60000000},
    { 84, "rtdmac_control3_00",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task084",           0x0400,      60000000},
    { 85, "rtdmac_control3_01",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task085",           0x0400,      60000000},
    { 86, "rtdmac_control3_02",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task086",           0x0400,      60000000},
    { 87, "rtdmac_control3_03",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task087",           0x0400,      60000000},
    { 88, "rtdmac_control3_04",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task088",           0x0400,      60000000},
    { 89, "rtdmac_control3_05",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task089",           0x0400,      60000000},
    { 90, "rtdmac_control3_06",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task090",           0x0400,      60000000},
    { 91, "rtdmac_control3_07",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task091",           0x0400,      60000000},
    { 92, "rtdmac_control3_08",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task092",           0x0400,      60000000},
    { 93, "rtdmac_control3_09",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task093",           0x0400,      60000000},
    { 94, "rtdmac_control3_10",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task094",           0x0400,      60000000},
    { 95, "rtdmac_control3_11",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task095",           0x0400,      60000000},
    { 96, "rtdmac_control3_12",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task096",           0x0400,      60000000},
    { 97, "rtdmac_control3_13",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task097",           0x0400,      60000000},
    { 98, "rtdmac_control3_14",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task098",           0x0400,      60000000},
    { 99, "rtdmac_control3_15",     OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task099",           0x0400,      60000000},
    {948, "ships_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task948",           0x0400,      60000000},
    {949, "ships_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task949",           0x0400,      60000000},
    {950, "ships_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task950",           0x0400,      60000000},
    {951, "ships_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task951",           0x0400,      60000000},
    {952, "ships_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task952",           0x0400,      60000000},
    {953, "ships_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task953",           0x0400,      60000000},
    {320, "iccom_00",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task320",           0x0400,      60000000},
    {322, "iccom_01",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task322",           0x0400,      60000000},
    {324, "iccom_02",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task324",           0x0400,      60000000},
    {326, "iccom_03",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task326",           0x0400,      60000000},
    {328, "iccom_04",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task328",           0x0400,      60000000},
    {330, "iccom_05",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task330",           0x0400,      60000000},
    {332, "iccom_06",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task332",           0x0400,      60000000},
    {334, "iccom_07",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task334",           0x0400,      60000000},
    {336, "iccom_08",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task336",           0x0400,      60000000},
    {338, "iccom_09",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task338",           0x0400,      60000000},
    {340, "iccom_10",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task340",           0x0400,      60000000},
    {342, "iccom_11",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task342",           0x0400,      60000000},
    {344, "iccom_12",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task344",           0x0400,      60000000},
    {346, "iccom_13",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task346",           0x0400,      60000000},
    {348, "iccom_14",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task348",           0x0400,      60000000},
    {350, "iccom_15",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task350",           0x0400,      60000000},
    {354, "iccom_16",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task354",           0x0400,      60000000},
    {356, "iccom_17",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task356",           0x0400,      60000000},
    {358, "iccom_18",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task358",           0x0400,      60000000},
    {360, "iccom_19",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task360",           0x0400,      60000000},
    {362, "iccom_20",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task362",           0x0400,      60000000},
    {364, "iccom_21",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task364",           0x0400,      60000000},
    {366, "iccom_22",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task366",           0x0400,      60000000},
    {368, "iccom_23",               OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task368",           0x0400,      60000000},
    {192, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task192",           0x0400,      60000000},
    {193, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task193",           0x0400,      60000000},
    {194, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task194",           0x0400,      60000000},
    {195, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task195",           0x0400,      60000000},
    {196, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task196",           0x0400,      60000000},
    {197, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task197",           0x0400,      60000000},
    {198, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task198",           0x0400,      60000000},
    {199, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task199",           0x0400,      60000000},
    {200, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task200",           0x0400,      60000000},
    {201, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task201",           0x0400,      60000000},
    {202, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task202",           0x0400,      60000000},
    {203, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task203",           0x0400,      60000000},
    {204, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task204",           0x0400,      60000000},
    {205, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task205",           0x0400,      60000000},
    {206, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task206",           0x0400,      60000000},
    {207, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task207",           0x0400,      60000000},
    {208, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task208",           0x0400,      60000000},
    {209, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task209",           0x0400,      60000000},
    {210, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task210",           0x0400,      60000000},
    {211, "icumx_cmd_00",           OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task211",           0x0400,      60000000},
    {218, "rfso_00",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task218",           0x1000,      60000000},
    {219, "rfso_01",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task219",           0x1000,      60000000},
    {220, "rfso_02",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task220",           0x1000,      60000000},
    {221, "rfso_03",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task221",           0x1000,      60000000},
    {222, "rfso_04",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task222",           0x1000,      60000000},
    {223, "rfso_05",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task223",           0x1000,      60000000},
    {224, "rfso_06",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task224",           0x1000,      60000000},
    {225, "rfso_07",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task225",           0x1000,      60000000},
    {226, "rfso_08",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task226",           0x1000,      60000000},
    {227, "rfso_09",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task227",           0x1000,      60000000},
    {228, "rfso_10",                OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task228",           0x1000,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#else
#error  Device is not supported
#endif

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_affinity
 * @brief osal device configuration : CPU affinity value for Interrupt thread
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* All default CPU Affinity */
    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};

/*======================================================================================================================
End of File
======================================================================================================================*/
