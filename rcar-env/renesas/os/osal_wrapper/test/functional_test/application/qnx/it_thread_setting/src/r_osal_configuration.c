/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Test Configurator for specific OS
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
#elif defined(RCAR_V4H2)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4H2
#elif defined(RCAR_V4H)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4H
#elif defined(RCAR_V4M)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_V4M
#elif defined(RCAR_S4) || defined(RCAR_S4N)
    .device_soc_type = (uint64_t)OSAL_DEVICE_SOC_TYPE_S4
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
const uint64_t g_osal_internal_thread_affinity = 0x2U;

/*******************************************************************************************************************//**
 * @var g_osal_thread_config
 * @brief osal resource configuration : thread configuration table
***********************************************************************************************************************/
const st_osal_rcfg_thread_config_t g_osal_thread_config[] =
{
    /* thread id,                   func, userarg, priority,                 task_name,          stack_size */
    /* task name is 16 characters */
    {OSAL_THREAD_OSAL_00,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "16characters_thr",    0x2000} },
    /* task name is 15 characters */
    {OSAL_THREAD_OSAL_01,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "15characters_th",     0x2000} },
    /* task name is 3 characters */
    {OSAL_THREAD_OSAL_02,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "thr",                 0x2000} },
    /* CPU Affinity is 0x2 */
    {OSAL_THREAD_OSAL_03,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task",           0x2000} },

    {.id = OSAL_THREAD_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : CPU affinity value for thread
***********************************************************************************************************************/
const st_osal_thread_affinity_t g_osal_thread_affinity[] =
{
    /* thread id,                    CPU Affinity */
    /* CPU Affinity is 0x2 */
    {OSAL_THREAD_OSAL_03,               0x2U  },

    {.id = OSAL_THREAD_AFFINITY_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var g_osal_mutex_config
 * @brief osal user configuration : mutex configuration table
***********************************************************************************************************************/
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_rcfg_mutex_config_t g_osal_mutex_config[] =
{
#if defined(RCAR_V3H1) || defined(RCAR_V3H2) || defined(RCAR_V3M) || defined(RCAR_V4H) || defined(RCAR_V4M)
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
#elif defined(RCAR_S4)
    /* mutex id               */
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
#if defined(RCAR_V3H1) || defined(RCAR_V3H2) || defined(RCAR_V3M) || defined(RCAR_V4H) || defined(RCAR_V4M)
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
#elif defined(RCAR_S4)
    /* cond id               */
    {OSAL_COND_CPURTT_00    },
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
#if defined(RCAR_V3H1) || defined(RCAR_V3H2) || defined(RCAR_V3M) || defined(RCAR_V4H) || defined(RCAR_V4M)
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
    {OSAL_MQ_CPURTT_TEST01,                  {128, 16} },
    {OSAL_MQ_CPURTT_RFSO,                    {128, 16} },
    {OSAL_MQ_CPURTT_TEST02,                  {128, 16} },
    {OSAL_MQ_CPURTT_TEST03,                  {128, 16} },
    {OSAL_MQ_CPURTT_TEST04,                  {128, 16} },
    {OSAL_MQ_FBIST_TEST00,                   {128, 16} },
    {OSAL_MQ_FBIST_TEST01,                   {128, 16} },
    {OSAL_MQ_RFSO_TEST00,                    {128, 16} },
    {OSAL_MQ_RFSO_TEST01,                    {128, 16} },
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
#elif defined(RCAR_S4)
    /* mq id,                       max_num_msg, msg_size */
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
    {OSAL_MQ_CPURTT_TEST00,                  { 1,  1}  },
    {OSAL_MQ_CPURTT_TEST01,                  {128, 16} },
    {OSAL_MQ_CPURTT_RFSO,                    {128, 16} },
    {OSAL_MQ_CPURTT_TEST02,                  {128, 16} },
    {OSAL_MQ_CPURTT_TEST03,                  {128, 16} },
    {OSAL_MQ_CPURTT_TEST04,                  {128, 16} },
    {OSAL_MQ_FBIST_TEST00,                   {128, 16} },
    {OSAL_MQ_FBIST_TEST01,                   {128, 16} },
    {OSAL_MQ_RFSO_TEST00,                    {128, 16} },
    {OSAL_MQ_RFSO_TEST01,                    {128, 16} },
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

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_config
 * @brief osal user configuration : interrupt thread configuration table
***********************************************************************************************************************/
#if defined(RCAR_V3H1)  /* V3H1 */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    /* task name is 16 characters */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "16characters_int",     0x0400,      60000000},
    /* task name is 15 characters */
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "15characters_in",      0x0400,      60000000},
    /* task name is 3 characters */
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int",                  0x0400,      60000000},
    /* CPU Affinity is 0x2 */
    {195, "ims_03",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int_task",             0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V3H2)    /* V3H2 */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    /* task name is 16 characters */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "16characters_int",     0x0400,      60000000},
    /* task name is 15 characters */
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "15characters_in",      0x0400,      60000000},
    /* task name is 3 characters */
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int",                  0x0400,      60000000},
    /* CPU Affinity is 0x2 */
    {254, "imr_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int_task",             0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V3M)     /* V3M */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    /* task name is 16 characters */
    {192, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "16characters_int",     0x0400,      60000000},
    /* task name is 15 characters */
    {193, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "15characters_in",      0x0400,      60000000},
    /* task name is 3 characters */
    {194, "ims_02",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int",                  0x0400,      60000000},
    /* CPU Affinity is 0x2 */
    {195, "ims_03",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int_task",             0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V4H)     /* V4H */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    /* task name is 16 characters */
    {579, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "16characters_int",     0x0400,      60000000},
    /* task name is 15 characters */
    {580, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "15characters_in",      0x0400,      60000000},
    /* task name is 3 characters */
    {581, "imr_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int",                  0x0400,      60000000},
    /* CPU Affinity is 0x2 */
    {582, "imr_01",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int_task",             0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_V4M)     /* V4M */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    /* task name is 16 characters */
    {579, "ims_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "16characters_int",     0x0400,      60000000},
    /* task name is 15 characters */
    {580, "ims_01",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "15characters_in",      0x0400,      60000000},
    /* task name is 3 characters */
    {581, "imr_00",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int",                  0x0400,      60000000},
    /* CPU Affinity is 0x2 */
    {582, "imr_01",                 OSAL_INTERRUPT_PRIORITY_TYPE10, "int_task",             0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#elif defined(RCAR_S4) || defined(RCAR_S4N)  /* S4 & S4N */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    /* task name is 16 characters */
    {933, "caiplite_00",            OSAL_INTERRUPT_PRIORITY_TYPE10, "16characters_int",     0x0400,      60000000},
    /* task name is 15 characters */
    {935, "caiplite_01",            OSAL_INTERRUPT_PRIORITY_TYPE10, "15characters_in",      0x0400,      60000000},
    /* task name is 3 characters */
    {937, "caiplite_02",            OSAL_INTERRUPT_PRIORITY_TYPE10, "int",                  0x0400,      60000000},
    /* CPU Affinity is 0x2 */
    {939, "caiplite_03",            OSAL_INTERRUPT_PRIORITY_TYPE10, "int_task",             0x0400,      60000000},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
#else
#error  Device is not supported
#endif

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_thread_affinity
 * @brief osal device configuration : CPU affinity value for Interrupt thread
***********************************************************************************************************************/
#if defined(RCAR_V3H1)  /* V3H1 */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* irq, CPU Affinity */
    /* CPU Affinity is 0x2 */
    {195,   0x2U  },

    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#elif defined(RCAR_V3H2)    /* V3H2 */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* irq, CPU Affinity */
    /* CPU Affinity is 0x2 */
    {254,   0x2U  },

    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#elif defined(RCAR_V3M)     /* V3M */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* irq, CPU Affinity */
    /* CPU Affinity is 0x2 */
    {195,   0x2U  },

    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#elif defined(RCAR_V4H)     /* V4H */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* irq, CPU Affinity */
    /* CPU Affinity is 0x2 */
    {582,   0x2U  },

    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#elif defined(RCAR_V4M)     /* V4M */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* irq, CPU Affinity */
    /* CPU Affinity is 0x2 */
    {582,   0x2U  },

    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#elif defined(RCAR_S4) || defined(RCAR_S4N)  /* S4 & S4N */
/* PRQA S 1531 1 # It must be external linkage separately from the referenced file because of the object to be exposed to the user. */
const st_osal_interrupt_thread_affinity_t  g_osal_interrupt_thread_affinity[] =
{
    /* irq, CPU Affinity */
    /* CPU Affinity is 0x2 */
    {939,   0x0U  },

    {.irq_number = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
#else
#error  Device is not supported
#endif

/*======================================================================================================================
End of File
======================================================================================================================*/
