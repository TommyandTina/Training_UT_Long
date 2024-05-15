/***********************************************************************************************************************
* Copyright [2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_resource_info.c
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL resource configuration file
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_usr_cfg/freertos/r_osal_usr_cfg_info.h"

#define RESOURCE_ID_RESERVE  0x0000U
#define RESOURCE_ID_IMR      0x1000U
#define RESOURCE_ID_ICCOM    0x3000U
#define RESOURCE_ID_LDE      0x4000U
#define RESOURCE_ID_IMP      0x5000U
#define RESOURCE_ID_OSAL     0xf000U
#define RESOURCE_ID_BSP      0x8000U
#define RESOURCE_ID_2DG      0x9000U
#define RESOURCE_ID_CODEC    0x9800U

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_ID
 * Thread IDs.
***********************************************************************************************************************/
/* xOS1 resource information. This is the compatibility of the alpha version */
/* Will be removed in the future */
/*
#define    OSAL_THREAD_ICCOM_MGR_TASK                (0U)
#define    OSAL_THREAD_ICCOM_TIMER_TASK              (1U)
#define    OSAL_THREAD_ICCOM_RCV_TASK0               (2U)
#define    OSAL_THREAD_ICCOM_RCV_TASK1               (3U)
#define    OSAL_THREAD_ICCOM_RCV_TASK2               (4U)
#define    OSAL_THREAD_ICCOM_RCV_TASK3               (5U)
#define    OSAL_THREAD_ICCOM_RCV_TASK4               (6U)
#define    OSAL_THREAD_ICCOM_RCV_TASK5               (7U)
#define    OSAL_THREAD_ICCOM_RCV_TASK6               (8U)
#define    OSAL_THREAD_ICCOM_RCV_TASK7               (9U)
#define    OSAL_THREAD_ICCOM_READ_TASK0              (10U)
#define    OSAL_THREAD_ICCOM_READ_TASK1              (11U)
#define    OSAL_THREAD_ICCOM_READ_TASK2              (12U)
#define    OSAL_THREAD_ICCOM_READ_TASK3              (13U)
#define    OSAL_THREAD_ICCOM_READ_TASK4              (14U)
#define    OSAL_THREAD_ICCOM_READ_TASK5              (15U)
#define    OSAL_THREAD_ICCOM_READ_TASK6              (16U)
#define    OSAL_THREAD_ICCOM_READ_TASK7              (17U)
#define    OSAL_THREAD_ICCOM_SEND_TASK0              (18U)
#define    OSAL_THREAD_ICCOM_SEND_TASK1              (19U)
#define    OSAL_THREAD_ICCOM_SEND_TASK2              (20U)
#define    OSAL_THREAD_ICCOM_SEND_TASK3              (21U)
#define    OSAL_THREAD_ICCOM_SEND_TASK4              (22U)
#define    OSAL_THREAD_ICCOM_SEND_TASK5              (23U)
#define    OSAL_THREAD_ICCOM_SEND_TASK6              (24U)
#define    OSAL_THREAD_ICCOM_SEND_TASK7              (25U)
#define    OSAL_THREAD_IMP_RTT_DRV                   (26U)
#define    OSAL_THREAD_IMP_DRV                       (27U)

#define    OSAL_THREAD_RESERVE_00                    (1000U)
#define    OSAL_THREAD_RESERVE_01                    (1001U)
#define    OSAL_THREAD_RESERVE_02                    (1002U)
#define    OSAL_THREAD_RESERVE_03                    (1003U)
#define    OSAL_THREAD_RESERVE_04                    (1004U)
#define    OSAL_THREAD_RESERVE_05                    (1005U)
#define    OSAL_THREAD_RESERVE_06                    (1006U)
#define    OSAL_THREAD_RESERVE_07                    (1007U)
#define    OSAL_THREAD_RESERVE_08                    (1008U)
#define    OSAL_THREAD_RESERVE_09                    (1009U)
*/
/* xOS2 resource information */
#define    OSAL_THREAD_ICCOM_DEMO_TASK               (RESOURCE_ID_ICCOM +  0U)
#define    OSAL_THREAD_ICCOM_MGR_TASK                (RESOURCE_ID_ICCOM +  1U)
#define    OSAL_THREAD_ICCOM_TIMER_TASK              (RESOURCE_ID_ICCOM +  2U)
#define    OSAL_THREAD_ICCOM_RCV_TASK0               (RESOURCE_ID_ICCOM +  3U)
#define    OSAL_THREAD_ICCOM_RCV_TASK1               (RESOURCE_ID_ICCOM +  4U)
#define    OSAL_THREAD_ICCOM_RCV_TASK2               (RESOURCE_ID_ICCOM +  5U)
#define    OSAL_THREAD_ICCOM_RCV_TASK3               (RESOURCE_ID_ICCOM +  6U)
#define    OSAL_THREAD_ICCOM_RCV_TASK4               (RESOURCE_ID_ICCOM +  7U)
#define    OSAL_THREAD_ICCOM_RCV_TASK5               (RESOURCE_ID_ICCOM +  8U)
#define    OSAL_THREAD_ICCOM_RCV_TASK6               (RESOURCE_ID_ICCOM +  9U)
#define    OSAL_THREAD_ICCOM_RCV_TASK7               (RESOURCE_ID_ICCOM + 10U)
#define    OSAL_THREAD_ICCOM_READ_TASK0              (RESOURCE_ID_ICCOM + 11U)
#define    OSAL_THREAD_ICCOM_READ_TASK1              (RESOURCE_ID_ICCOM + 12U)
#define    OSAL_THREAD_ICCOM_READ_TASK2              (RESOURCE_ID_ICCOM + 13U)
#define    OSAL_THREAD_ICCOM_READ_TASK3              (RESOURCE_ID_ICCOM + 14U)
#define    OSAL_THREAD_ICCOM_READ_TASK4              (RESOURCE_ID_ICCOM + 15U)
#define    OSAL_THREAD_ICCOM_READ_TASK5              (RESOURCE_ID_ICCOM + 16U)
#define    OSAL_THREAD_ICCOM_READ_TASK6              (RESOURCE_ID_ICCOM + 17U)
#define    OSAL_THREAD_ICCOM_READ_TASK7              (RESOURCE_ID_ICCOM + 18U)
#define    OSAL_THREAD_ICCOM_SEND_TASK0              (RESOURCE_ID_ICCOM + 19U)
#define    OSAL_THREAD_ICCOM_SEND_TASK1              (RESOURCE_ID_ICCOM + 20U)
#define    OSAL_THREAD_ICCOM_SEND_TASK2              (RESOURCE_ID_ICCOM + 21U)
#define    OSAL_THREAD_ICCOM_SEND_TASK3              (RESOURCE_ID_ICCOM + 22U)
#define    OSAL_THREAD_ICCOM_SEND_TASK4              (RESOURCE_ID_ICCOM + 23U)
#define    OSAL_THREAD_ICCOM_SEND_TASK5              (RESOURCE_ID_ICCOM + 24U)
#define    OSAL_THREAD_ICCOM_SEND_TASK6              (RESOURCE_ID_ICCOM + 25U)
#define    OSAL_THREAD_ICCOM_SEND_TASK7              (RESOURCE_ID_ICCOM + 26U)

#define    OSAL_THREAD_IMP_RTT_DRV                   (RESOURCE_ID_IMP   +  0U)
#define    OSAL_THREAD_IMP_DRV                       (RESOURCE_ID_IMP   +  1U)

#define    OSAL_THREAD_LDE_CAPUTRE_TASK              (RESOURCE_ID_LDE   +  0U)
#define    OSAL_THREAD_LDE_OUTPUT_TASK               (RESOURCE_ID_LDE   +  1U)
#define    OSAL_THREAD_LDE_CTX_TASK                  (RESOURCE_ID_LDE   +  2U)

#define    OSAL_THREAD_OSAL_00                       (RESOURCE_ID_OSAL  +  0U)
#define    OSAL_THREAD_OSAL_01                       (RESOURCE_ID_OSAL  +  1U)
#define    OSAL_THREAD_OSAL_02                       (RESOURCE_ID_OSAL  +  2U)
#define    OSAL_THREAD_OSAL_03                       (RESOURCE_ID_OSAL  +  3U)
#define    OSAL_THREAD_OSAL_04                       (RESOURCE_ID_OSAL  +  4U)
#define    OSAL_THREAD_OSAL_05                       (RESOURCE_ID_OSAL  +  5U)
#define    OSAL_THREAD_OSAL_06                       (RESOURCE_ID_OSAL  +  6U)
#define    OSAL_THREAD_OSAL_07                       (RESOURCE_ID_OSAL  +  7U)
#define    OSAL_THREAD_OSAL_08                       (RESOURCE_ID_OSAL  +  8U)
#define    OSAL_THREAD_OSAL_09                       (RESOURCE_ID_OSAL  +  9U)

#define    OSAL_THREAD_LOGO_00                       (RESOURCE_ID_BSP   +  0U)
#define    OSAL_THREAD_LOGO_01                       (RESOURCE_ID_BSP   +  1U)
#define    OSAL_THREAD_LOGO_02                       (RESOURCE_ID_BSP   +  2U)
#define    OSAL_THREAD_REARVIEW_00                   (RESOURCE_ID_BSP   +  3U)
#define    OSAL_THREAD_REARVIEW_01                   (RESOURCE_ID_BSP   +  4U)
#define    OSAL_THREAD_REARVIEW_CIO_SERVER           (RESOURCE_ID_BSP   +  5U)
#define    OSAL_THREAD_REARVIEW_TAURUS               (RESOURCE_ID_BSP   +  6U)
#define    OSAL_THREAD_RVC_MAIN                      (RESOURCE_ID_BSP   +  7U)
#define    OSAL_THREAD_TAURUS_MAIN                   (RESOURCE_ID_BSP   +  8U)
#define    OSAL_THREAD_TAURUS_CAN_EVENT              (RESOURCE_ID_BSP   +  9U)
#define    OSAL_THREAD_TAURUS_COM_SERVER             (RESOURCE_ID_BSP   +  10U)
#define    OSAL_THREAD_TAURUS_RVGC                   (RESOURCE_ID_BSP   +  11U)
#define    OSAL_THREAD_CIO_MAIN                      (RESOURCE_ID_BSP   +  12U)
#define    OSAL_THREAD_CIO_CANFD01                   (RESOURCE_ID_BSP   +  13U)
#define    OSAL_THREAD_CIO_CANFD02                   (RESOURCE_ID_BSP   +  14U)
#define    OSAL_THREAD_CIO_CANFD03                   (RESOURCE_ID_BSP   +  15U)
#define    OSAL_THREAD_CIO_CANFD04                   (RESOURCE_ID_BSP   +  16U)
#define    OSAL_THREAD_CIO_CANFD05                   (RESOURCE_ID_BSP   +  17U)
#define    OSAL_THREAD_CIO_CANFD06                   (RESOURCE_ID_BSP   +  18U)
#define    OSAL_THREAD_CIO_CANFD07                   (RESOURCE_ID_BSP   +  19U)
#define    OSAL_THREAD_CIO_CANFD08                   (RESOURCE_ID_BSP   +  20U)
#define    OSAL_THREAD_WM01                          (RESOURCE_ID_BSP   +  21U)
#define    OSAL_THREAD_WM02                          (RESOURCE_ID_BSP   +  22U)
#define    OSAL_THREAD_WM03                          (RESOURCE_ID_BSP   +  23U)
#define    OSAL_THREAD_WM04                          (RESOURCE_ID_BSP   +  24U)
#define    OSAL_THREAD_WM05                          (RESOURCE_ID_BSP   +  25U)
#define    OSAL_THREAD_WM06                          (RESOURCE_ID_BSP   +  26U)
#define    OSAL_THREAD_WM07                          (RESOURCE_ID_BSP   +  27U)
#define    OSAL_THREAD_WM08                          (RESOURCE_ID_BSP   +  28U)
#define    OSAL_THREAD_VIN01                         (RESOURCE_ID_BSP   +  29U)
#define    OSAL_THREAD_VIN02                         (RESOURCE_ID_BSP   +  30U)
#define    OSAL_THREAD_VIN03                         (RESOURCE_ID_BSP   +  31U)
#define    OSAL_THREAD_VIN04                         (RESOURCE_ID_BSP   +  32U)
#define    OSAL_THREAD_VIN05                         (RESOURCE_ID_BSP   +  33U)
#define    OSAL_THREAD_VIN06                         (RESOURCE_ID_BSP   +  34U)
#define    OSAL_THREAD_VIN07                         (RESOURCE_ID_BSP   +  35U)
#define    OSAL_THREAD_VIN08                         (RESOURCE_ID_BSP   +  36U)
#define    OSAL_THREAD_CIO_DUMMY01                   (RESOURCE_ID_BSP   +  37U)
#define    OSAL_THREAD_CIO_DUMMY02                   (RESOURCE_ID_BSP   +  38U)
#define    OSAL_THREAD_CIO_DUMMY03                   (RESOURCE_ID_BSP   +  39U)
#define    OSAL_THREAD_CIO_DUMMY04                   (RESOURCE_ID_BSP   +  40U)
#define    OSAL_THREAD_CIO_DUMMY05                   (RESOURCE_ID_BSP   +  41U)
#define    OSAL_THREAD_CIO_DUMMY06                   (RESOURCE_ID_BSP   +  42U)
#define    OSAL_THREAD_CIO_DUMMY07                   (RESOURCE_ID_BSP   +  43U)
#define    OSAL_THREAD_CIO_DUMMY08                   (RESOURCE_ID_BSP   +  44U)
#define    OSAL_THREAD_TAURUS_PARACAMERA             (RESOURCE_ID_BSP   +  45U)
#define    OSAL_THREAD_DHD_KERNEL                    (RESOURCE_ID_2DG   +   0U)
#define    OSAL_THREAD_DRW2D_TASK00                  (RESOURCE_ID_2DG   +   1U)
#define    OSAL_THREAD_DRW2D_TASK01                  (RESOURCE_ID_2DG   +   2U)
#define    OSAL_THREAD_DRW2D_TASK02                  (RESOURCE_ID_2DG   +   3U)
#define    OSAL_THREAD_DRW2D_TASK03                  (RESOURCE_ID_2DG   +   4U)
#define    OSAL_THREAD_DRW2D_TASK04                  (RESOURCE_ID_2DG   +   5U)
#define    OSAL_THREAD_DRW2D_TASK05                  (RESOURCE_ID_2DG   +   6U)
#define    OSAL_THREAD_DRW2D_TASK06                  (RESOURCE_ID_2DG   +   7U)
#define    OSAL_THREAD_DRW2D_TASK07                  (RESOURCE_ID_2DG   +   8U)
#define    OSAL_THREAD_DRW2D_TASK08                  (RESOURCE_ID_2DG   +   9U)
#define    OSAL_THREAD_DRW2D_TASK09                  (RESOURCE_ID_2DG   +  10U)
#define    OSAL_THREAD_DRW2D_TASK10                  (RESOURCE_ID_2DG   +  11U)
#define    OSAL_THREAD_DRW2D_TASK11                  (RESOURCE_ID_2DG   +  12U)
#define    OSAL_THREAD_DRW2D_TASK12                  (RESOURCE_ID_2DG   +  13U)
#define    OSAL_THREAD_DRW2D_TASK13                  (RESOURCE_ID_2DG   +  14U)

#define    OSAL_THREAD_RIVP_SAMPLE_MAIN              (RESOURCE_ID_CODEC +   0U)
#define    OSAL_THREAD_RIVP_TASK_DEC                 (RESOURCE_ID_CODEC +   1U)
#define    OSAL_THREAD_RIVP_TASK_OUT                 (RESOURCE_ID_CODEC +   2U)

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_ID
 * Mutex IDs.
***********************************************************************************************************************/
/* xOS1 resource information. This is the compatibility of the alpha version */
/* Will be removed in the future */
/*
#define    OSAL_MUTEX_IMR_00                         (0U)
#define    OSAL_MUTEX_IMR_01                         (1U)
#define    OSAL_MUTEX_IMR_02                         (2U)
#define    OSAL_MUTEX_IMR_03                         (3U)
#define    OSAL_MUTEX_IMR_04                         (4U)
#define    OSAL_MUTEX_IMR_05                         (5U)
#define    OSAL_MUTEX_DOF0_00                        (6U)
#define    OSAL_MUTEX_DOF0_01                        (7U)
#define    OSAL_MUTEX_STV0_00                        (8U)
#define    OSAL_MUTEX_STV0_01                        (9U)
#define    OSAL_MUTEX_ACF0_00                        (10U)
#define    OSAL_MUTEX_ACF0_01                        (11U)
#define    OSAL_MUTEX_ACF1_00                        (12U)
#define    OSAL_MUTEX_ACF1_01                        (13U)
#define    OSAL_MUTEX_ACF2_00                        (14U)
#define    OSAL_MUTEX_ACF2_01                        (15U)
#define    OSAL_MUTEX_ACF3_00                        (16U)
#define    OSAL_MUTEX_ACF3_01                        (17U)
#define    OSAL_MUTEX_ACF4_00                        (18U)
#define    OSAL_MUTEX_ACF4_01                        (19U)
#define    OSAL_MUTEX_VIPRTT_00                      (20U)
#define    OSAL_MUTEX_VIPRTT_01                      (21U)
#define    OSAL_MUTEX_VIPRTT_02                      (22U)
#define    OSAL_MUTEX_VIPRTT_03                      (23U)
#define    OSAL_MUTEX_ICCOM_WRITE_00                 (24U)
#define    OSAL_MUTEX_ICCOM_WRITE_01                 (25U)
#define    OSAL_MUTEX_ICCOM_WRITE_02                 (26U)
#define    OSAL_MUTEX_ICCOM_WRITE_03                 (27U)
#define    OSAL_MUTEX_ICCOM_WRITE_04                 (28U)
#define    OSAL_MUTEX_ICCOM_WRITE_05                 (29U)
#define    OSAL_MUTEX_ICCOM_WRITE_06                 (30U)
#define    OSAL_MUTEX_ICCOM_WRITE_07                 (31U)
#define    OSAL_MUTEX_ICCOM_READ_00                  (32U)
#define    OSAL_MUTEX_ICCOM_READ_01                  (33U)
#define    OSAL_MUTEX_ICCOM_READ_02                  (34U)
#define    OSAL_MUTEX_ICCOM_READ_03                  (35U)
#define    OSAL_MUTEX_ICCOM_READ_04                  (36U)
#define    OSAL_MUTEX_ICCOM_READ_05                  (37U)
#define    OSAL_MUTEX_ICCOM_READ_06                  (38U)
#define    OSAL_MUTEX_ICCOM_READ_07                  (39U)
#define    OSAL_MUTEX_ICCOM_TIMER                    (40U)
#define    OSAL_MUTEX_IMP_FW_LOCK                    (41U)
#define    OSAL_MUTEX_IMP_RTT_LOCK                   (42U)
#define    OSAL_MUTEX_IMP_DRV_LOCK                   (43U)
#define    OSAL_MUTEX_ENCODER_00                     (44U)
#define    OSAL_MUTEX_ENCODER_01                     (45U)
#define    OSAL_MUTEX_ENCODER_02                     (46U)
#define    OSAL_MUTEX_CISP_00                        (47U)
#define    OSAL_MUTEX_CISP_01                        (48U)
#define    OSAL_MUTEX_TISP_00                        (49U)
#define    OSAL_MUTEX_TISP_01                        (50U)
#define    OSAL_MUTEX_ENCODER_APP_00                 (51U)
#define    OSAL_MUTEX_ENCODER_APP_01                 (52U)
#define    OSAL_MUTEX_ICCOM_OPEN                     (53U)

#define    OSAL_MUTEX_RESERVE_00                     (1000U)
#define    OSAL_MUTEX_RESERVE_01                     (1001U)
#define    OSAL_MUTEX_RESERVE_02                     (1002U)
#define    OSAL_MUTEX_RESERVE_03                     (1003U)
#define    OSAL_MUTEX_RESERVE_04                     (1004U)
#define    OSAL_MUTEX_RESERVE_05                     (1005U)
#define    OSAL_MUTEX_RESERVE_06                     (1006U)
#define    OSAL_MUTEX_RESERVE_07                     (1007U)
#define    OSAL_MUTEX_RESERVE_08                     (1008U)
#define    OSAL_MUTEX_RESERVE_09                     (1009U)
*/
/* xOS2 resource information */
#define    OSAL_MUTEX_IMR_00                         (RESOURCE_ID_IMR   +  0U)
#define    OSAL_MUTEX_IMR_01                         (RESOURCE_ID_IMR   +  1U)
#define    OSAL_MUTEX_IMR_02                         (RESOURCE_ID_IMR   +  2U)
#define    OSAL_MUTEX_IMR_03                         (RESOURCE_ID_IMR   +  3U)

#define    OSAL_MUTEX_ICCOM_WRITE_00                 (RESOURCE_ID_ICCOM +  0U)
#define    OSAL_MUTEX_ICCOM_WRITE_01                 (RESOURCE_ID_ICCOM +  1U)
#define    OSAL_MUTEX_ICCOM_WRITE_02                 (RESOURCE_ID_ICCOM +  2U)
#define    OSAL_MUTEX_ICCOM_WRITE_03                 (RESOURCE_ID_ICCOM +  3U)
#define    OSAL_MUTEX_ICCOM_WRITE_04                 (RESOURCE_ID_ICCOM +  4U)
#define    OSAL_MUTEX_ICCOM_WRITE_05                 (RESOURCE_ID_ICCOM +  5U)
#define    OSAL_MUTEX_ICCOM_WRITE_06                 (RESOURCE_ID_ICCOM +  6U)
#define    OSAL_MUTEX_ICCOM_WRITE_07                 (RESOURCE_ID_ICCOM +  7U)
#define    OSAL_MUTEX_ICCOM_READ_00                  (RESOURCE_ID_ICCOM +  8U)
#define    OSAL_MUTEX_ICCOM_READ_01                  (RESOURCE_ID_ICCOM +  9U)
#define    OSAL_MUTEX_ICCOM_READ_02                  (RESOURCE_ID_ICCOM + 10U)
#define    OSAL_MUTEX_ICCOM_READ_03                  (RESOURCE_ID_ICCOM + 11U)
#define    OSAL_MUTEX_ICCOM_READ_04                  (RESOURCE_ID_ICCOM + 12U)
#define    OSAL_MUTEX_ICCOM_READ_05                  (RESOURCE_ID_ICCOM + 13U)
#define    OSAL_MUTEX_ICCOM_READ_06                  (RESOURCE_ID_ICCOM + 14U)
#define    OSAL_MUTEX_ICCOM_READ_07                  (RESOURCE_ID_ICCOM + 15U)
#define    OSAL_MUTEX_ICCOM_TIMER                    (RESOURCE_ID_ICCOM + 16U)
#define    OSAL_MUTEX_ICCOM_OPEN                     (RESOURCE_ID_ICCOM + 17U)

#define    OSAL_MUTEX_LDE_LIB_00                     (RESOURCE_ID_LDE   +  0U)
#define    OSAL_MUTEX_LDE_LIB_01                     (RESOURCE_ID_LDE   +  1U)
#define    OSAL_MUTEX_LDE_LIB_02                     (RESOURCE_ID_LDE   +  2U)
#define    OSAL_MUTEX_LDE_APP_00                     (RESOURCE_ID_LDE   +  3U)

#define    OSAL_MUTEX_IMP_FW_LOCK                    (RESOURCE_ID_IMP   +  0U)
#define    OSAL_MUTEX_IMP_RTT_LOCK                   (RESOURCE_ID_IMP   +  1U)
#define    OSAL_MUTEX_IMP_DRV_LOCK                   (RESOURCE_ID_IMP   +  2U)

#define    OSAL_MUTEX_OSAL_00                        (RESOURCE_ID_OSAL  +  0U)
#define    OSAL_MUTEX_OSAL_01                        (RESOURCE_ID_OSAL  +  1U)
#define    OSAL_MUTEX_OSAL_02                        (RESOURCE_ID_OSAL  +  2U)
#define    OSAL_MUTEX_OSAL_03                        (RESOURCE_ID_OSAL  +  3U)
#define    OSAL_MUTEX_OSAL_04                        (RESOURCE_ID_OSAL  +  4U)
#define    OSAL_MUTEX_OSAL_05                        (RESOURCE_ID_OSAL  +  5U)
#define    OSAL_MUTEX_OSAL_06                        (RESOURCE_ID_OSAL  +  6U)
#define    OSAL_MUTEX_OSAL_07                        (RESOURCE_ID_OSAL  +  7U)
#define    OSAL_MUTEX_OSAL_08                        (RESOURCE_ID_OSAL  +  8U)
#define    OSAL_MUTEX_OSAL_09                        (RESOURCE_ID_OSAL  +  9U)

#define    OSAL_MUTEX_CIOLIB                         (RESOURCE_ID_BSP   +  0U)
#define    OSAL_MUTEX_CIOLIB2                        (RESOURCE_ID_BSP   +  1U)
#define    OSAL_MUTEX_CIOVIN_00                      (RESOURCE_ID_BSP   +  2U)
#define    OSAL_MUTEX_DRIVER_DMAC_00                 (RESOURCE_ID_BSP   +  3U)
#define    OSAL_MUTEX_WM1                            (RESOURCE_ID_BSP   +  4U)
#define    OSAL_MUTEX_WM2                            (RESOURCE_ID_BSP   +  5U)
#define    OSAL_MUTEX_REARVIEW1                      (RESOURCE_ID_BSP   +  6U)
#define    OSAL_MUTEX_REARVIEW2                      (RESOURCE_ID_BSP   +  7U)
#define    OSAL_MUTEX_TAURUS_CAN                     (RESOURCE_ID_BSP   +  8U)
#define    OSAL_MUTEX_TAURUS_RVGC                    (RESOURCE_ID_BSP   +  9U)
#define    OSAL_MUTEX_CIO_CANFD                      (RESOURCE_ID_BSP   + 10U)
#define    OSAL_MUTEX_CIO_CANFD2                     (RESOURCE_ID_BSP   + 11U)
#define    OSAL_MUTEX_CIO_SERVER_CH0                 (RESOURCE_ID_BSP   + 12U)
#define    OSAL_MUTEX_CIO_SERVER_CH1                 (RESOURCE_ID_BSP   + 13U)
#define    OSAL_MUTEX_CIO_SERVER_CH2                 (RESOURCE_ID_BSP   + 14U)
#define    OSAL_MUTEX_CIO_SERVER_CH3                 (RESOURCE_ID_BSP   + 15U)
#define    OSAL_MUTEX_CIO_SERVER_CH4                 (RESOURCE_ID_BSP   + 16U)
#define    OSAL_MUTEX_CIO_SERVER_CH5                 (RESOURCE_ID_BSP   + 17U)
#define    OSAL_MUTEX_CIO_SERVER_CH6                 (RESOURCE_ID_BSP   + 18U)
#define    OSAL_MUTEX_CIO_SERVER_CH7                 (RESOURCE_ID_BSP   + 19U)
#define    OSAL_MUTEX_CIOVIN_01                      (RESOURCE_ID_BSP   + 20U)
#define    OSAL_MUTEX_DRIVER_DMAC_01                 (RESOURCE_ID_BSP   + 21U)
#define    OSAL_MUTEX_DRIVER_DMAC2                   (RESOURCE_ID_BSP   + 22U)
#define    OSAL_MUTEX_DRIVER_DMAC3                   (RESOURCE_ID_BSP   + 23U)
#define    OSAL_MUTEX_DRIVER_DMAC4                   (RESOURCE_ID_BSP   + 24U)
#define    OSAL_MUTEX_DRIVER_DMAC5                   (RESOURCE_ID_BSP   + 25U)
#define    OSAL_MUTEX_WM1_01                         (RESOURCE_ID_BSP   + 26U)
#define    OSAL_MUTEX_WM1_02                         (RESOURCE_ID_BSP   + 27U)
#define    OSAL_MUTEX_WM1_03                         (RESOURCE_ID_BSP   + 28U)
#define    OSAL_MUTEX_WM1_04                         (RESOURCE_ID_BSP   + 29U)
#define    OSAL_MUTEX_WM2_01                         (RESOURCE_ID_BSP   + 30U)
#define    OSAL_MUTEX_WM2_02                         (RESOURCE_ID_BSP   + 31U)
#define    OSAL_MUTEX_WM2_03                         (RESOURCE_ID_BSP   + 32U)
#define    OSAL_MUTEX_WM2_04                         (RESOURCE_ID_BSP   + 33U)
#define    OSAL_MUTEX_WM2_05                         (RESOURCE_ID_BSP   + 34U)
#define    OSAL_MUTEX_WM2_06                         (RESOURCE_ID_BSP   + 35U)
#define    OSAL_MUTEX_WM2_07                         (RESOURCE_ID_BSP   + 36U)
#define    OSAL_MUTEX_WM2_08                         (RESOURCE_ID_BSP   + 37U)
#define    OSAL_MUTEX_WM2_09                         (RESOURCE_ID_BSP   + 38U)
#define    OSAL_MUTEX_WM2_10                         (RESOURCE_ID_BSP   + 39U)
#define    OSAL_MUTEX_WM2_11                         (RESOURCE_ID_BSP   + 40U)
#define    OSAL_MUTEX_WM2_12                         (RESOURCE_ID_BSP   + 41U)
#define    OSAL_MUTEX_WM2_13                         (RESOURCE_ID_BSP   + 42U)
#define    OSAL_MUTEX_WM2_14                         (RESOURCE_ID_BSP   + 43U)
#define    OSAL_MUTEX_WM2_15                         (RESOURCE_ID_BSP   + 44U)
#define    OSAL_MUTEX_WM2_16                         (RESOURCE_ID_BSP   + 45U)
#define    OSAL_MUTEX_WM2_17                         (RESOURCE_ID_BSP   + 46U)
#define    OSAL_MUTEX_WM2_18                         (RESOURCE_ID_BSP   + 47U)
#define    OSAL_MUTEX_WM2_19                         (RESOURCE_ID_BSP   + 48U)
#define    OSAL_MUTEX_WM2_21                         (RESOURCE_ID_BSP   + 49U)
#define    OSAL_MUTEX_WM2_22                         (RESOURCE_ID_BSP   + 50U)
#define    OSAL_MUTEX_WM2_23                         (RESOURCE_ID_BSP   + 51U)
#define    OSAL_MUTEX_WM2_24                         (RESOURCE_ID_BSP   + 52U)
#define    OSAL_MUTEX_WM2_25                         (RESOURCE_ID_BSP   + 53U)
#define    OSAL_MUTEX_WM2_26                         (RESOURCE_ID_BSP   + 54U)
#define    OSAL_MUTEX_WM2_27                         (RESOURCE_ID_BSP   + 55U)
#define    OSAL_MUTEX_WM2_28                         (RESOURCE_ID_BSP   + 56U)
#define    OSAL_MUTEX_WM2_29                         (RESOURCE_ID_BSP   + 57U)
#define    OSAL_MUTEX_CIO_LIB_CH0                    (RESOURCE_ID_BSP   + 58U)
#define    OSAL_MUTEX_CIO_LIB_CH1                    (RESOURCE_ID_BSP   + 59U)
#define    OSAL_MUTEX_CIO_LIB_CH2                    (RESOURCE_ID_BSP   + 60U)
#define    OSAL_MUTEX_CIO_LIB_CH3                    (RESOURCE_ID_BSP   + 61U)
#define    OSAL_MUTEX_CIO_LIB_CH4                    (RESOURCE_ID_BSP   + 62U)
#define    OSAL_MUTEX_CIO_LIB_CH5                    (RESOURCE_ID_BSP   + 63U)
#define    OSAL_MUTEX_CIO_LIB_CH6                    (RESOURCE_ID_BSP   + 64U)
#define    OSAL_MUTEX_CIO_LIB_CH7                    (RESOURCE_ID_BSP   + 65U)
#define    OSAL_MUTEX_CIO_LIB_CH8                    (RESOURCE_ID_BSP   + 66U)
#define    OSAL_MUTEX_CIO_LIB_CH9                    (RESOURCE_ID_BSP   + 67U)
#define    OSAL_MUTEX_WM1_05                         (RESOURCE_ID_BSP   + 68U)
#define    OSAL_MUTEX_WM1_06                         (RESOURCE_ID_BSP   + 69U)
#define    OSAL_MUTEX_WM1_07                         (RESOURCE_ID_BSP   + 70U)
#define    OSAL_MUTEX_TAURUS_PARACAMERA              (RESOURCE_ID_BSP   + 71U)
#define    OSAL_MUTEX_TAURUS_RPMSG                   (RESOURCE_ID_BSP   + 72U)
#define    OSAL_MUTEX_RTDMAC_CH00                    (RESOURCE_ID_BSP   + 73U)
#define    OSAL_MUTEX_RTDMAC_CH01                    (RESOURCE_ID_BSP   + 74U)
#define    OSAL_MUTEX_RTDMAC_CH02                    (RESOURCE_ID_BSP   + 75U)
#define    OSAL_MUTEX_RTDMAC_CH03                    (RESOURCE_ID_BSP   + 76U)
#define    OSAL_MUTEX_RTDMAC_CH04                    (RESOURCE_ID_BSP   + 77U)
#define    OSAL_MUTEX_RTDMAC_CH05                    (RESOURCE_ID_BSP   + 78U)
#define    OSAL_MUTEX_RTDMAC_CH06                    (RESOURCE_ID_BSP   + 79U)
#define    OSAL_MUTEX_RTDMAC_CH07                    (RESOURCE_ID_BSP   + 80U)
#define    OSAL_MUTEX_RTDMAC_CH08                    (RESOURCE_ID_BSP   + 81U)
#define    OSAL_MUTEX_RTDMAC_CH09                    (RESOURCE_ID_BSP   + 82U)
#define    OSAL_MUTEX_RTDMAC_CH10                    (RESOURCE_ID_BSP   + 83U)
#define    OSAL_MUTEX_RTDMAC_CH11                    (RESOURCE_ID_BSP   + 84U)
#define    OSAL_MUTEX_RTDMAC_CH12                    (RESOURCE_ID_BSP   + 85U)
#define    OSAL_MUTEX_RTDMAC_CH13                    (RESOURCE_ID_BSP   + 86U)
#define    OSAL_MUTEX_RTDMAC_CH14                    (RESOURCE_ID_BSP   + 87U)
#define    OSAL_MUTEX_RTDMAC_CH15                    (RESOURCE_ID_BSP   + 88U)

#define    OSAL_MUTEX_DHD_SYNC_INTERRUPT             (RESOURCE_ID_2DG   + 0U)
#define    OSAL_MUTEX_DHD_ERROR_INTERRUPT            (RESOURCE_ID_2DG   + 1U)
#define    OSAL_MUTEX_DHD_STOP_INTERRUPT             (RESOURCE_ID_2DG   + 2U)
#define    OSAL_MUTEX_DHD_WAKEUP_SERVER              (RESOURCE_ID_2DG   + 3U)
#define    OSAL_MUTEX_DHD_LOCK_DEVICE                (RESOURCE_ID_2DG   + 4U)
#define    OSAL_MUTEX_DHD_LOCK_ALLOCATOR             (RESOURCE_ID_2DG   + 5U)
#define    OSAL_MUTEX_DRW2D_LOCK_API_00              (RESOURCE_ID_2DG   + 6U)
#define    OSAL_MUTEX_DRW2D_LOCK_API_01              (RESOURCE_ID_2DG   + 7U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_00        (RESOURCE_ID_2DG   + 8U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_00        (RESOURCE_ID_2DG   + 8U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_01        (RESOURCE_ID_2DG   + 9U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_02        (RESOURCE_ID_2DG   + 10U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_03        (RESOURCE_ID_2DG   + 11U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_04        (RESOURCE_ID_2DG   + 12U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_05        (RESOURCE_ID_2DG   + 13U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_06        (RESOURCE_ID_2DG   + 14U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_07        (RESOURCE_ID_2DG   + 15U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_08        (RESOURCE_ID_2DG   + 16U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_09        (RESOURCE_ID_2DG   + 17U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_10        (RESOURCE_ID_2DG   + 18U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_11        (RESOURCE_ID_2DG   + 19U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_12        (RESOURCE_ID_2DG   + 20U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_13        (RESOURCE_ID_2DG   + 21U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_14        (RESOURCE_ID_2DG   + 22U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_15        (RESOURCE_ID_2DG   + 23U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_16        (RESOURCE_ID_2DG   + 24U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_17        (RESOURCE_ID_2DG   + 25U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_18        (RESOURCE_ID_2DG   + 26U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_19        (RESOURCE_ID_2DG   + 27U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_20        (RESOURCE_ID_2DG   + 28U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_21        (RESOURCE_ID_2DG   + 29U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_22        (RESOURCE_ID_2DG   + 30U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_23        (RESOURCE_ID_2DG   + 31U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_24        (RESOURCE_ID_2DG   + 32U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_25        (RESOURCE_ID_2DG   + 33U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_26        (RESOURCE_ID_2DG   + 34U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_27        (RESOURCE_ID_2DG   + 35U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_28        (RESOURCE_ID_2DG   + 36U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_29        (RESOURCE_ID_2DG   + 37U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_30        (RESOURCE_ID_2DG   + 38U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_31        (RESOURCE_ID_2DG   + 39U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_32        (RESOURCE_ID_2DG   + 40U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_33        (RESOURCE_ID_2DG   + 41U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_34        (RESOURCE_ID_2DG   + 42U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_35        (RESOURCE_ID_2DG   + 43U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_36        (RESOURCE_ID_2DG   + 44U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_37        (RESOURCE_ID_2DG   + 45U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_38        (RESOURCE_ID_2DG   + 46U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_39        (RESOURCE_ID_2DG   + 47U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_40        (RESOURCE_ID_2DG   + 48U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_41        (RESOURCE_ID_2DG   + 49U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_42        (RESOURCE_ID_2DG   + 50U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_43        (RESOURCE_ID_2DG   + 51U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_44        (RESOURCE_ID_2DG   + 52U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_45        (RESOURCE_ID_2DG   + 53U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_46        (RESOURCE_ID_2DG   + 54U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_47        (RESOURCE_ID_2DG   + 55U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_48        (RESOURCE_ID_2DG   + 56U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_49        (RESOURCE_ID_2DG   + 57U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_50        (RESOURCE_ID_2DG   + 58U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_51        (RESOURCE_ID_2DG   + 59U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_52        (RESOURCE_ID_2DG   + 60U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_53        (RESOURCE_ID_2DG   + 61U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_54        (RESOURCE_ID_2DG   + 62U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_55        (RESOURCE_ID_2DG   + 63U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_56        (RESOURCE_ID_2DG   + 64U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_57        (RESOURCE_ID_2DG   + 65U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_58        (RESOURCE_ID_2DG   + 66U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_59        (RESOURCE_ID_2DG   + 67U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_60        (RESOURCE_ID_2DG   + 68U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_61        (RESOURCE_ID_2DG   + 69U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_62        (RESOURCE_ID_2DG   + 70U)
#define    OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_63        (RESOURCE_ID_2DG   + 71U)

#define    OSAL_MUTEX_LDD_00                         (RESOURCE_ID_CODEC +  0U)
#define    OSAL_MUTEX_LDD_01                         (RESOURCE_ID_CODEC +  1U)
#define    OSAL_MUTEX_LDD_02                         (RESOURCE_ID_CODEC +  2U)
#define    OSAL_MUTEX_LDD_03                         (RESOURCE_ID_CODEC +  3U)
#define    OSAL_MUTEX_LDD_04                         (RESOURCE_ID_CODEC +  4U)
#define    OSAL_MUTEX_LDD_05                         (RESOURCE_ID_CODEC +  5U)
#define    OSAL_MUTEX_LDD_06                         (RESOURCE_ID_CODEC +  6U)
#define    OSAL_MUTEX_LDD_07                         (RESOURCE_ID_CODEC +  7U)
#define    OSAL_MUTEX_LDD_LOG_00                     (RESOURCE_ID_CODEC +  8U)
#define    OSAL_MUTEX_LDD_LOG_01                     (RESOURCE_ID_CODEC +  9U)

/*******************************************************************************************************************//**
 * @def OSAL_COND_ID
 * Condition variable IDs.
***********************************************************************************************************************/
/* xOS1 resource information. This is the compatibility of the alpha version */
/* Will be removed in the future */
/*
#define    OSAL_COND_RESERVE_00                      (1000U)
#define    OSAL_COND_RESERVE_01                      (1001U)
#define    OSAL_COND_RESERVE_02                      (1002U)
#define    OSAL_COND_RESERVE_03                      (1003U)
#define    OSAL_COND_RESERVE_04                      (1004U)
#define    OSAL_COND_RESERVE_05                      (1005U)
#define    OSAL_COND_RESERVE_06                      (1006U)
#define    OSAL_COND_RESERVE_07                      (1007U)
#define    OSAL_COND_RESERVE_08                      (1008U)
#define    OSAL_COND_RESERVE_09                      (1009U)
*/
/* xOS2 resource information */
#define    OSAL_COND_OSAL_00                         (RESOURCE_ID_OSAL +  0U)
#define    OSAL_COND_OSAL_01                         (RESOURCE_ID_OSAL +  1U)
#define    OSAL_COND_OSAL_02                         (RESOURCE_ID_OSAL +  2U)
#define    OSAL_COND_OSAL_03                         (RESOURCE_ID_OSAL +  3U)
#define    OSAL_COND_OSAL_04                         (RESOURCE_ID_OSAL +  4U)
#define    OSAL_COND_OSAL_05                         (RESOURCE_ID_OSAL +  5U)
#define    OSAL_COND_OSAL_06                         (RESOURCE_ID_OSAL +  6U)
#define    OSAL_COND_OSAL_07                         (RESOURCE_ID_OSAL +  7U)
#define    OSAL_COND_OSAL_08                         (RESOURCE_ID_OSAL +  8U)
#define    OSAL_COND_OSAL_09                         (RESOURCE_ID_OSAL +  9U)

#define    OSAL_COND_DHD_SYNC_INTERRUPT              (RESOURCE_ID_2DG  +  0U)
#define    OSAL_COND_DHD_ERROR_INTERRUPT             (RESOURCE_ID_2DG  +  1U)
#define    OSAL_COND_DHD_STOP_INTERRUPT              (RESOURCE_ID_2DG  +  2U)
#define    OSAL_COND_DHD_WAKEUP_SERVER               (RESOURCE_ID_2DG  +  3U)
#define    OSAL_COND_DRW2D_VSYNC_00                  (RESOURCE_ID_2DG  +  4U)
#define    OSAL_COND_DRW2D_VSYNC_01                  (RESOURCE_ID_2DG  +  5U)
#define    OSAL_COND_DRW2D_VSYNC_02                  (RESOURCE_ID_2DG  +  6U)
#define    OSAL_COND_DRW2D_VSYNC_03                  (RESOURCE_ID_2DG  +  7U)
#define    OSAL_COND_DRW2D_VSYNC_04                  (RESOURCE_ID_2DG  +  8U)
#define    OSAL_COND_DRW2D_VSYNC_05                  (RESOURCE_ID_2DG  +  9U)

#define    OSAL_COND_REARVIEW_CLUSTER                (RESOURCE_ID_BSP  +  0U)
#define    OSAL_COND_REARVIEW_CID                    (RESOURCE_ID_BSP  +  1U)
#define    OSAL_COND_CIO_CANFD_IRQ_GL                (RESOURCE_ID_BSP  +  2U)
#define    OSAL_COND_CIO_CANFD_IRQ_CH                (RESOURCE_ID_BSP  +  3U)
#define    OSAL_COND_TAURUS_CAN                      (RESOURCE_ID_BSP  +  4U)
#define    OSAL_COND_TAURUS_RVGC                     (RESOURCE_ID_BSP  +  5U)
#define    OSAL_COND_WM_00                           (RESOURCE_ID_BSP  +  6U)
#define    OSAL_COND_WM_01                           (RESOURCE_ID_BSP  +  7U)
#define    OSAL_COND_WM_02                           (RESOURCE_ID_BSP  +  8U)
#define    OSAL_COND_WM_03                           (RESOURCE_ID_BSP  +  9U)
#define    OSAL_COND_WM_04                           (RESOURCE_ID_BSP  + 10U)
#define    OSAL_COND_WM_05                           (RESOURCE_ID_BSP  + 11U)
#define    OSAL_COND_WM_06                           (RESOURCE_ID_BSP  + 12U)
#define    OSAL_COND_WM_07                           (RESOURCE_ID_BSP  + 13U)
#define    OSAL_COND_WM_08                           (RESOURCE_ID_BSP  + 14U)
#define    OSAL_COND_WM_09                           (RESOURCE_ID_BSP  + 15U)
#define    OSAL_COND_WM_10                           (RESOURCE_ID_BSP  + 16U)
#define    OSAL_COND_WM_11                           (RESOURCE_ID_BSP  + 17U)
#define    OSAL_COND_WM_12                           (RESOURCE_ID_BSP  + 18U)
#define    OSAL_COND_WM_13                           (RESOURCE_ID_BSP  + 19U)
#define    OSAL_COND_WM_14                           (RESOURCE_ID_BSP  + 20U)
#define    OSAL_COND_CANFD_GLOBAL                    (RESOURCE_ID_BSP  + 21U)
#define    OSAL_COND_CANFD_CHANNEL                   (RESOURCE_ID_BSP  + 22U)
#define    OSAL_COND_RTDMAC_CH00                     (RESOURCE_ID_BSP  + 23U)
#define    OSAL_COND_RTDMAC_CH01                     (RESOURCE_ID_BSP  + 24U)
#define    OSAL_COND_RTDMAC_CH02                     (RESOURCE_ID_BSP  + 25U)
#define    OSAL_COND_RTDMAC_CH03                     (RESOURCE_ID_BSP  + 26U)
#define    OSAL_COND_RTDMAC_CH04                     (RESOURCE_ID_BSP  + 27U)
#define    OSAL_COND_RTDMAC_CH05                     (RESOURCE_ID_BSP  + 28U)
#define    OSAL_COND_RTDMAC_CH06                     (RESOURCE_ID_BSP  + 29U)
#define    OSAL_COND_RTDMAC_CH07                     (RESOURCE_ID_BSP  + 30U)
#define    OSAL_COND_RTDMAC_CH08                     (RESOURCE_ID_BSP  + 31U)
#define    OSAL_COND_RTDMAC_CH09                     (RESOURCE_ID_BSP  + 32U)
#define    OSAL_COND_RTDMAC_CH10                     (RESOURCE_ID_BSP  + 33U)
#define    OSAL_COND_RTDMAC_CH11                     (RESOURCE_ID_BSP  + 34U)
#define    OSAL_COND_RTDMAC_CH12                     (RESOURCE_ID_BSP  + 35U)
#define    OSAL_COND_RTDMAC_CH13                     (RESOURCE_ID_BSP  + 36U)
#define    OSAL_COND_RTDMAC_CH14                     (RESOURCE_ID_BSP  + 37U)
#define    OSAL_COND_RTDMAC_CH15                     (RESOURCE_ID_BSP  + 38U)
#define    OSAL_COND_WM_15                           (RESOURCE_ID_BSP  + 39U)
#define    OSAL_COND_WM_16                           (RESOURCE_ID_BSP  + 40U)
#define    OSAL_COND_WM_17                           (RESOURCE_ID_BSP  + 41U)
#define    OSAL_COND_WM_18                           (RESOURCE_ID_BSP  + 42U)
#define    OSAL_COND_WM_19                           (RESOURCE_ID_BSP  + 43U)


/*******************************************************************************************************************//**
 * @def OSAL_MQ_ID
 * Message queue IDs.
***********************************************************************************************************************/
/* xOS1 resource information. This is the compatibility of the alpha version */
/* Will be removed in the future */
/*
#define    OSAL_MQ_IMR_MSG_00                        (0U)
#define    OSAL_MQ_IMR_MSG_01                        (1U)
#define    OSAL_MQ_IMR_MSG_02                        (2U)
#define    OSAL_MQ_IMR_MSG_03                        (3U)
#define    OSAL_MQ_IMR_MSG_04                        (4U)
#define    OSAL_MQ_IMR_MSG_05                        (5U)
#define    OSAL_MQ_IMR_RTT_MSG_00                    (6U)
#define    OSAL_MQ_IMR_RTT_MSG_01                    (7U)
#define    OSAL_MQ_IMR_RTT_MSG_02                    (8U)
#define    OSAL_MQ_IMR_RTT_MSG_03                    (9U)
#define    OSAL_MQ_IMR_RTT_MSG_04                    (10U)
#define    OSAL_MQ_IMR_RTT_MSG_05                    (11U)
#define    OSAL_MQ_ICCOM_MGR_TASK                    (12U)
#define    OSAL_MQ_ICCOM_REQ_OPEN                    (13U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_00          (14U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_01          (15U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_02          (16U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_03          (17U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_04          (18U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_05          (19U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_06          (20U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_07          (21U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_00         (22U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_01         (23U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_02         (24U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_03         (25U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_04         (26U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_05         (27U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_06         (28U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_07         (29U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_00             (30U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_01             (31U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_02             (32U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_03             (33U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_04             (34U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_05             (35U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_06             (36U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_07             (37U)
#define    OSAL_MQ_ICCOM_SEND_TASK_00                (38U)
#define    OSAL_MQ_ICCOM_SEND_TASK_01                (39U)
#define    OSAL_MQ_ICCOM_SEND_TASK_02                (40U)
#define    OSAL_MQ_ICCOM_SEND_TASK_03                (41U)
#define    OSAL_MQ_ICCOM_SEND_TASK_04                (42U)
#define    OSAL_MQ_ICCOM_SEND_TASK_05                (43U)
#define    OSAL_MQ_ICCOM_SEND_TASK_06                (44U)
#define    OSAL_MQ_ICCOM_SEND_TASK_07                (45U)
#define    OSAL_MQ_ICCOM_READ_TASK_00                (46U)
#define    OSAL_MQ_ICCOM_READ_TASK_01                (47U)
#define    OSAL_MQ_ICCOM_READ_TASK_02                (48U)
#define    OSAL_MQ_ICCOM_READ_TASK_03                (49U)
#define    OSAL_MQ_ICCOM_READ_TASK_04                (50U)
#define    OSAL_MQ_ICCOM_READ_TASK_05                (51U)
#define    OSAL_MQ_ICCOM_READ_TASK_06                (52U)
#define    OSAL_MQ_ICCOM_READ_TASK_07                (53U)
#define    OSAL_MQ_ICCOM_SEND_DATA_00                (54U)
#define    OSAL_MQ_ICCOM_SEND_DATA_01                (55U)
#define    OSAL_MQ_ICCOM_SEND_DATA_02                (56U)
#define    OSAL_MQ_ICCOM_SEND_DATA_03                (57U)
#define    OSAL_MQ_ICCOM_SEND_DATA_04                (58U)
#define    OSAL_MQ_ICCOM_SEND_DATA_05                (59U)
#define    OSAL_MQ_ICCOM_SEND_DATA_06                (60U)
#define    OSAL_MQ_ICCOM_SEND_DATA_07                (61U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_00             (62U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_01             (63U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_02             (64U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_03             (65U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_04             (66U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_05             (67U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_06             (68U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_07             (69U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_00       (70U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_01       (71U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_02       (72U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_03       (73U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_04       (74U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_05       (75U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_06       (76U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_07       (77U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_00    (78U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_01    (79U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_02    (80U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_03    (81U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_04    (82U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_05    (83U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_06    (84U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_07    (85U)
#define    OSAL_MQ_IMP_RTT_TASK_REQ                  (86U)
#define    OSAL_MQ_IMP_RTT_TASK_ACK                  (87U)
#define    OSAL_MQ_IMP_RTT_DRVCB_00                  (88U)
#define    OSAL_MQ_IMP_RTT_EXECCB                    (89U)
#define    OSAL_MQ_IMP_DRV_TASK_CBFUNC               (90U)
#define    OSAL_MQ_IMP_DRV_TASK_DELTASK              (91U)

#define    OSAL_MQ_RESERVE_00                        (1000U)
#define    OSAL_MQ_RESERVE_01                        (1001U)
#define    OSAL_MQ_RESERVE_02                        (1002U)
#define    OSAL_MQ_RESERVE_03                        (1003U)
#define    OSAL_MQ_RESERVE_04                        (1004U)
#define    OSAL_MQ_RESERVE_05                        (1005U)
#define    OSAL_MQ_RESERVE_06                        (1006U)
#define    OSAL_MQ_RESERVE_07                        (1007U)
#define    OSAL_MQ_RESERVE_08                        (1008U)
#define    OSAL_MQ_RESERVE_09                        (1009U)
*/
/* xOS2 resource information */
#define    OSAL_MQ_IMR_MSG_00                        (RESOURCE_ID_IMR   +  0U)
#define    OSAL_MQ_IMR_MSG_01                        (RESOURCE_ID_IMR   +  1U)
#define    OSAL_MQ_IMR_MSG_02                        (RESOURCE_ID_IMR   +  2U)
#define    OSAL_MQ_IMR_MSG_03                        (RESOURCE_ID_IMR   +  3U)
#define    OSAL_MQ_IMR_MSG_04                        (RESOURCE_ID_IMR   +  4U)
#define    OSAL_MQ_IMR_MSG_05                        (RESOURCE_ID_IMR   +  5U)
#define    OSAL_MQ_IMR_RTT_MSG_00                    (RESOURCE_ID_IMR   +  6U)
#define    OSAL_MQ_IMR_RTT_MSG_01                    (RESOURCE_ID_IMR   +  7U)
#define    OSAL_MQ_IMR_RTT_MSG_02                    (RESOURCE_ID_IMR   +  8U)
#define    OSAL_MQ_IMR_RTT_MSG_03                    (RESOURCE_ID_IMR   +  9U)
#define    OSAL_MQ_IMR_RTT_MSG_04                    (RESOURCE_ID_IMR   + 10U)
#define    OSAL_MQ_IMR_RTT_MSG_05                    (RESOURCE_ID_IMR   + 11U)

#define    OSAL_MQ_ICCOM_MGR_TASK                    (RESOURCE_ID_ICCOM +  0U)
#define    OSAL_MQ_ICCOM_REQ_OPEN                    (RESOURCE_ID_ICCOM +  1U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_00          (RESOURCE_ID_ICCOM +  2U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_01          (RESOURCE_ID_ICCOM +  3U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_02          (RESOURCE_ID_ICCOM +  4U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_03          (RESOURCE_ID_ICCOM +  5U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_04          (RESOURCE_ID_ICCOM +  6U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_05          (RESOURCE_ID_ICCOM +  7U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_06          (RESOURCE_ID_ICCOM +  8U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_07          (RESOURCE_ID_ICCOM +  9U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_00         (RESOURCE_ID_ICCOM + 10U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_01         (RESOURCE_ID_ICCOM + 11U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_02         (RESOURCE_ID_ICCOM + 12U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_03         (RESOURCE_ID_ICCOM + 13U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_04         (RESOURCE_ID_ICCOM + 14U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_05         (RESOURCE_ID_ICCOM + 15U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_06         (RESOURCE_ID_ICCOM + 16U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_07         (RESOURCE_ID_ICCOM + 17U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_00             (RESOURCE_ID_ICCOM + 18U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_01             (RESOURCE_ID_ICCOM + 19U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_02             (RESOURCE_ID_ICCOM + 20U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_03             (RESOURCE_ID_ICCOM + 21U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_04             (RESOURCE_ID_ICCOM + 22U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_05             (RESOURCE_ID_ICCOM + 23U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_06             (RESOURCE_ID_ICCOM + 24U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_07             (RESOURCE_ID_ICCOM + 25U)
#define    OSAL_MQ_ICCOM_SEND_TASK_00                (RESOURCE_ID_ICCOM + 26U)
#define    OSAL_MQ_ICCOM_SEND_TASK_01                (RESOURCE_ID_ICCOM + 27U)
#define    OSAL_MQ_ICCOM_SEND_TASK_02                (RESOURCE_ID_ICCOM + 28U)
#define    OSAL_MQ_ICCOM_SEND_TASK_03                (RESOURCE_ID_ICCOM + 29U)
#define    OSAL_MQ_ICCOM_SEND_TASK_04                (RESOURCE_ID_ICCOM + 30U)
#define    OSAL_MQ_ICCOM_SEND_TASK_05                (RESOURCE_ID_ICCOM + 31U)
#define    OSAL_MQ_ICCOM_SEND_TASK_06                (RESOURCE_ID_ICCOM + 32U)
#define    OSAL_MQ_ICCOM_SEND_TASK_07                (RESOURCE_ID_ICCOM + 33U)
#define    OSAL_MQ_ICCOM_READ_TASK_00                (RESOURCE_ID_ICCOM + 34U)
#define    OSAL_MQ_ICCOM_READ_TASK_01                (RESOURCE_ID_ICCOM + 35U)
#define    OSAL_MQ_ICCOM_READ_TASK_02                (RESOURCE_ID_ICCOM + 36U)
#define    OSAL_MQ_ICCOM_READ_TASK_03                (RESOURCE_ID_ICCOM + 37U)
#define    OSAL_MQ_ICCOM_READ_TASK_04                (RESOURCE_ID_ICCOM + 38U)
#define    OSAL_MQ_ICCOM_READ_TASK_05                (RESOURCE_ID_ICCOM + 39U)
#define    OSAL_MQ_ICCOM_READ_TASK_06                (RESOURCE_ID_ICCOM + 40U)
#define    OSAL_MQ_ICCOM_READ_TASK_07                (RESOURCE_ID_ICCOM + 41U)
#define    OSAL_MQ_ICCOM_SEND_DATA_00                (RESOURCE_ID_ICCOM + 42U)
#define    OSAL_MQ_ICCOM_SEND_DATA_01                (RESOURCE_ID_ICCOM + 43U)
#define    OSAL_MQ_ICCOM_SEND_DATA_02                (RESOURCE_ID_ICCOM + 44U)
#define    OSAL_MQ_ICCOM_SEND_DATA_03                (RESOURCE_ID_ICCOM + 45U)
#define    OSAL_MQ_ICCOM_SEND_DATA_04                (RESOURCE_ID_ICCOM + 46U)
#define    OSAL_MQ_ICCOM_SEND_DATA_05                (RESOURCE_ID_ICCOM + 47U)
#define    OSAL_MQ_ICCOM_SEND_DATA_06                (RESOURCE_ID_ICCOM + 48U)
#define    OSAL_MQ_ICCOM_SEND_DATA_07                (RESOURCE_ID_ICCOM + 49U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_00             (RESOURCE_ID_ICCOM + 50U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_01             (RESOURCE_ID_ICCOM + 51U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_02             (RESOURCE_ID_ICCOM + 52U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_03             (RESOURCE_ID_ICCOM + 53U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_04             (RESOURCE_ID_ICCOM + 54U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_05             (RESOURCE_ID_ICCOM + 55U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_06             (RESOURCE_ID_ICCOM + 56U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_07             (RESOURCE_ID_ICCOM + 57U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_00       (RESOURCE_ID_ICCOM + 58U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_01       (RESOURCE_ID_ICCOM + 59U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_02       (RESOURCE_ID_ICCOM + 60U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_03       (RESOURCE_ID_ICCOM + 61U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_04       (RESOURCE_ID_ICCOM + 62U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_05       (RESOURCE_ID_ICCOM + 63U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_06       (RESOURCE_ID_ICCOM + 64U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_07       (RESOURCE_ID_ICCOM + 65U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_00    (RESOURCE_ID_ICCOM + 66U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_01    (RESOURCE_ID_ICCOM + 67U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_02    (RESOURCE_ID_ICCOM + 68U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_03    (RESOURCE_ID_ICCOM + 69U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_04    (RESOURCE_ID_ICCOM + 70U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_05    (RESOURCE_ID_ICCOM + 71U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_06    (RESOURCE_ID_ICCOM + 72U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_07    (RESOURCE_ID_ICCOM + 73U)

#define    OSAL_MQ_IMP_RTT_TASK_REQ                  (RESOURCE_ID_IMP   +  0U)
#define    OSAL_MQ_IMP_RTT_TASK_ACK                  (RESOURCE_ID_IMP   +  1U)
#define    OSAL_MQ_IMP_RTT_DRVCB_00                  (RESOURCE_ID_IMP   +  2U)
#define    OSAL_MQ_IMP_RTT_EXECCB                    (RESOURCE_ID_IMP   +  3U)
#define    OSAL_MQ_IMP_DRV_TASK_CBFUNC               (RESOURCE_ID_IMP   +  4U)
#define    OSAL_MQ_IMP_DRV_TASK_DELTASK              (RESOURCE_ID_IMP   +  5U)

#define    OSAL_MQ_LDE_APP_MAIN                      (RESOURCE_ID_LDE   +  0U)
#define    OSAL_MQ_LDE_APP_OUT                       (RESOURCE_ID_LDE   +  1U)
#define    OSAL_MQ_LDE_APP_CTX                       (RESOURCE_ID_LDE   +  2U)

#define    OSAL_MQ_OSAL_00                           (RESOURCE_ID_OSAL  +  0U)
#define    OSAL_MQ_OSAL_01                           (RESOURCE_ID_OSAL  +  1U)
#define    OSAL_MQ_OSAL_02                           (RESOURCE_ID_OSAL  +  2U)
#define    OSAL_MQ_OSAL_03                           (RESOURCE_ID_OSAL  +  3U)
#define    OSAL_MQ_OSAL_04                           (RESOURCE_ID_OSAL  +  4U)
#define    OSAL_MQ_OSAL_05                           (RESOURCE_ID_OSAL  +  5U)
#define    OSAL_MQ_OSAL_06                           (RESOURCE_ID_OSAL  +  6U)
#define    OSAL_MQ_OSAL_07                           (RESOURCE_ID_OSAL  +  7U)
#define    OSAL_MQ_OSAL_08                           (RESOURCE_ID_OSAL  +  8U)
#define    OSAL_MQ_OSAL_09                           (RESOURCE_ID_OSAL  +  9U)

#define    OSAL_MQ_CIO_SERVER_00                     (RESOURCE_ID_BSP   +  0U)
#define    OSAL_MQ_CIO_SERVER_01                     (RESOURCE_ID_BSP   +  1U)
#define    OSAL_MQ_CIO_SERVER_02                     (RESOURCE_ID_BSP   +  2U)
#define    OSAL_MQ_CIO_SERVER_03                     (RESOURCE_ID_BSP   +  3U)
#define    OSAL_MQ_CIO_SERVER_04                     (RESOURCE_ID_BSP   +  4U)
#define    OSAL_MQ_CIO_SERVER_05                     (RESOURCE_ID_BSP   +  5U)
#define    OSAL_MQ_CIO_SERVER_06                     (RESOURCE_ID_BSP   +  6U)
#define    OSAL_MQ_CIO_SERVER_07                     (RESOURCE_ID_BSP   +  7U)
#define    OSAL_MQ_CIO_SERVER_08                     (RESOURCE_ID_BSP   +  8U)
#define    OSAL_MQ_CIO_SERVER_09                     (RESOURCE_ID_BSP   +  9U)
#define    OSAL_MQ_CIO_SERVER_10                     (RESOURCE_ID_BSP   + 10U)
#define    OSAL_MQ_CIO_SERVER_11                     (RESOURCE_ID_BSP   + 11U)
#define    OSAL_MQ_CIO_SERVER_12                     (RESOURCE_ID_BSP   + 12U)
#define    OSAL_MQ_CIO_SERVER_13                     (RESOURCE_ID_BSP   + 13U)
#define    OSAL_MQ_CIO_SERVER_14                     (RESOURCE_ID_BSP   + 14U)
#define    OSAL_MQ_CIO_SERVER_15                     (RESOURCE_ID_BSP   + 15U)
#define    OSAL_MQ_CIO_SERVER_16                     (RESOURCE_ID_BSP   + 16U)
#define    OSAL_MQ_CIO_SERVER_17                     (RESOURCE_ID_BSP   + 17U)
#define    OSAL_MQ_REARVIEW_CAMERA_APP               (RESOURCE_ID_BSP   + 18U)
#define    OSAL_MQ_TAURUS_PARACAN                    (RESOURCE_ID_BSP   + 19U)
#define    OSAL_MQ_TAURUS_PARARVGC                   (RESOURCE_ID_BSP   + 20U)
#define    OSAL_MQ_TAURUS_PARAVIN                    (RESOURCE_ID_BSP   + 21U)

#define    OSAL_MQ_DHD_SERVER                         (RESOURCE_ID_2DG  +  0U)
#define    OSAL_MQ_DHD_CLIENT_00                      (RESOURCE_ID_2DG  +  1U)
#define    OSAL_MQ_DHD_CLIENT_01                      (RESOURCE_ID_2DG  +  2U)
#define    OSAL_MQ_DHD_CLIENT_02                      (RESOURCE_ID_2DG  +  3U)
#define    OSAL_MQ_DHD_CLIENT_03                      (RESOURCE_ID_2DG  +  4U)
#define    OSAL_MQ_DHD_CLIENT_04                      (RESOURCE_ID_2DG  +  5U)
#define    OSAL_MQ_DHD_CLIENT_05                      (RESOURCE_ID_2DG  +  6U)
#define    OSAL_MQ_DHD_CLIENT_06                      (RESOURCE_ID_2DG  +  7U)
#define    OSAL_MQ_DHD_CLIENT_07                      (RESOURCE_ID_2DG  +  8U)
#define    OSAL_MQ_DHD_CLIENT_08                      (RESOURCE_ID_2DG  +  9U)
#define    OSAL_MQ_DHD_CLIENT_09                      (RESOURCE_ID_2DG  + 10U)
#define    OSAL_MQ_DHD_CLIENT_10                      (RESOURCE_ID_2DG  + 11U)
#define    OSAL_MQ_DHD_CLIENT_11                      (RESOURCE_ID_2DG  + 12U)
#define    OSAL_MQ_DHD_CLIENT_12                      (RESOURCE_ID_2DG  + 13U)
#define    OSAL_MQ_DHD_CLIENT_13                      (RESOURCE_ID_2DG  + 14U)
#define    OSAL_MQ_DHD_CLIENT_14                      (RESOURCE_ID_2DG  + 15U)
#define    OSAL_MQ_DHD_CLIENT_15                      (RESOURCE_ID_2DG  + 16U)
#define    OSAL_MQ_DHD_CLIENT_16                      (RESOURCE_ID_2DG  + 17U)
#define    OSAL_MQ_DHD_CLIENT_17                      (RESOURCE_ID_2DG  + 18U)
#define    OSAL_MQ_DHD_CLIENT_18                      (RESOURCE_ID_2DG  + 19U)
#define    OSAL_MQ_DHD_CLIENT_19                      (RESOURCE_ID_2DG  + 20U)
#define    OSAL_MQ_DHD_CLIENT_20                      (RESOURCE_ID_2DG  + 21U)
#define    OSAL_MQ_DHD_CLIENT_21                      (RESOURCE_ID_2DG  + 22U)
#define    OSAL_MQ_DHD_CLIENT_22                      (RESOURCE_ID_2DG  + 23U)
#define    OSAL_MQ_DHD_CLIENT_23                      (RESOURCE_ID_2DG  + 24U)
#define    OSAL_MQ_DHD_CLIENT_24                      (RESOURCE_ID_2DG  + 25U)
#define    OSAL_MQ_DHD_CLIENT_25                      (RESOURCE_ID_2DG  + 26U)
#define    OSAL_MQ_DHD_CLIENT_26                      (RESOURCE_ID_2DG  + 27U)
#define    OSAL_MQ_DHD_CLIENT_27                      (RESOURCE_ID_2DG  + 28U)
#define    OSAL_MQ_DHD_CLIENT_28                      (RESOURCE_ID_2DG  + 29U)
#define    OSAL_MQ_DHD_CLIENT_29                      (RESOURCE_ID_2DG  + 30U)
#define    OSAL_MQ_DHD_CLIENT_30                      (RESOURCE_ID_2DG  + 31U)
#define    OSAL_MQ_DHD_CLIENT_31                      (RESOURCE_ID_2DG  + 32U)
#define    OSAL_MQ_DHD_CLIENT_32                      (RESOURCE_ID_2DG  + 33U)
#define    OSAL_MQ_DHD_CLIENT_33                      (RESOURCE_ID_2DG  + 34U)
#define    OSAL_MQ_DHD_CLIENT_34                      (RESOURCE_ID_2DG  + 35U)
#define    OSAL_MQ_DHD_CLIENT_35                      (RESOURCE_ID_2DG  + 36U)
#define    OSAL_MQ_DHD_CLIENT_36                      (RESOURCE_ID_2DG  + 37U)
#define    OSAL_MQ_DHD_CLIENT_37                      (RESOURCE_ID_2DG  + 38U)
#define    OSAL_MQ_DHD_CLIENT_38                      (RESOURCE_ID_2DG  + 39U)
#define    OSAL_MQ_DHD_CLIENT_39                      (RESOURCE_ID_2DG  + 40U)
#define    OSAL_MQ_DHD_CLIENT_40                      (RESOURCE_ID_2DG  + 41U)
#define    OSAL_MQ_DHD_CLIENT_41                      (RESOURCE_ID_2DG  + 42U)
#define    OSAL_MQ_DHD_CLIENT_42                      (RESOURCE_ID_2DG  + 43U)
#define    OSAL_MQ_DHD_CLIENT_43                      (RESOURCE_ID_2DG  + 44U)
#define    OSAL_MQ_DHD_CLIENT_44                      (RESOURCE_ID_2DG  + 45U)
#define    OSAL_MQ_DHD_CLIENT_45                      (RESOURCE_ID_2DG  + 46U)
#define    OSAL_MQ_DHD_CLIENT_46                      (RESOURCE_ID_2DG  + 47U)
#define    OSAL_MQ_DHD_CLIENT_47                      (RESOURCE_ID_2DG  + 48U)
#define    OSAL_MQ_DHD_CLIENT_48                      (RESOURCE_ID_2DG  + 49U)
#define    OSAL_MQ_DHD_CLIENT_49                      (RESOURCE_ID_2DG  + 50U)
#define    OSAL_MQ_DHD_CLIENT_50                      (RESOURCE_ID_2DG  + 51U)
#define    OSAL_MQ_DHD_CLIENT_51                      (RESOURCE_ID_2DG  + 52U)
#define    OSAL_MQ_DHD_CLIENT_52                      (RESOURCE_ID_2DG  + 53U)
#define    OSAL_MQ_DHD_CLIENT_53                      (RESOURCE_ID_2DG  + 54U)
#define    OSAL_MQ_DHD_CLIENT_54                      (RESOURCE_ID_2DG  + 55U)
#define    OSAL_MQ_DHD_CLIENT_55                      (RESOURCE_ID_2DG  + 56U)
#define    OSAL_MQ_DHD_CLIENT_56                      (RESOURCE_ID_2DG  + 57U)
#define    OSAL_MQ_DHD_CLIENT_57                      (RESOURCE_ID_2DG  + 58U)
#define    OSAL_MQ_DHD_CLIENT_58                      (RESOURCE_ID_2DG  + 59U)
#define    OSAL_MQ_DHD_CLIENT_59                      (RESOURCE_ID_2DG  + 60U)
#define    OSAL_MQ_DHD_CLIENT_60                      (RESOURCE_ID_2DG  + 61U)
#define    OSAL_MQ_DHD_CLIENT_61                      (RESOURCE_ID_2DG  + 62U)
#define    OSAL_MQ_DHD_CLIENT_62                      (RESOURCE_ID_2DG  + 63U)
#define    OSAL_MQ_DHD_CLIENT_63                      (RESOURCE_ID_2DG  + 64U)

#define    OSAL_MQ_LDD_00                             (RESOURCE_ID_CODEC + 0U)
#define    OSAL_MQ_LDD_01                             (RESOURCE_ID_CODEC + 1U)
#define    OSAL_MQ_LDD_02                             (RESOURCE_ID_CODEC + 2U)
#define    OSAL_MQ_LDD_03                             (RESOURCE_ID_CODEC + 3U)

/*******************************************************************************************************************//**
 * @var gs_osal_thread_config
 * osal user configuration : thread configuration table
***********************************************************************************************************************/
st_osal_rcfg_thread_config_t gs_osal_thread_config[] =
{
    /* thread id,                   func, userarg, priority,                 task_name,          stack_size */

    /* xOS1 resource information. This is the compatibility of the alpha version */
    /* Will be removed in the future */
    { 0U /* OSAL_THREAD_ICCOM_MGR_TASK  */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom manager task",  0x1000} },
    { 1U /* OSAL_THREAD_ICCOM_TIMER_TASK*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom timer task",    0x1000} },
    { 2U /* OSAL_THREAD_ICCOM_RCV_TASK0 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task0", 0x1800} },
    { 3U /* OSAL_THREAD_ICCOM_RCV_TASK1 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task1", 0x1800} },
    { 4U /* OSAL_THREAD_ICCOM_RCV_TASK2 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task2", 0x1800} },
    { 5U /* OSAL_THREAD_ICCOM_RCV_TASK3 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task3", 0x1800} },
    { 6U /* OSAL_THREAD_ICCOM_RCV_TASK4 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task4", 0x1800} },
    { 7U /* OSAL_THREAD_ICCOM_RCV_TASK5 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task5", 0x1800} },
    { 8U /* OSAL_THREAD_ICCOM_RCV_TASK6 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task6", 0x1800} },
    { 9U /* OSAL_THREAD_ICCOM_RCV_TASK7 */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task7", 0x1800} },
    {10U /* OSAL_THREAD_ICCOM_READ_TASK0*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task0",    0x1000} },
    {11U /* OSAL_THREAD_ICCOM_READ_TASK1*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task1",    0x1000} },
    {12U /* OSAL_THREAD_ICCOM_READ_TASK2*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task2",    0x1000} },
    {13U /* OSAL_THREAD_ICCOM_READ_TASK3*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task3",    0x1000} },
    {14U /* OSAL_THREAD_ICCOM_READ_TASK4*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task4",    0x1000} },
    {15U /* OSAL_THREAD_ICCOM_READ_TASK5*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task5",    0x2000} },
    {16U /* OSAL_THREAD_ICCOM_READ_TASK6*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task6",    0x1000} },
    {17U /* OSAL_THREAD_ICCOM_READ_TASK7*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task7",    0x1000} },
    {18U /* OSAL_THREAD_ICCOM_SEND_TASK0*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task0",    0x1000} },
    {19U /* OSAL_THREAD_ICCOM_SEND_TASK1*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task1",    0x1000} },
    {20U /* OSAL_THREAD_ICCOM_SEND_TASK2*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task2",    0x1000} },
    {21U /* OSAL_THREAD_ICCOM_SEND_TASK3*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task3",    0x1000} },
    {22U /* OSAL_THREAD_ICCOM_SEND_TASK4*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task4",    0x1000} },
    {23U /* OSAL_THREAD_ICCOM_SEND_TASK5*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task5",    0x1000} },
    {24U /* OSAL_THREAD_ICCOM_SEND_TASK6*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task6",    0x1000} },
    {25U /* OSAL_THREAD_ICCOM_SEND_TASK7*/, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task7",    0x1000} },
    {26U /* OSAL_THREAD_IMP_RTT_DRV     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp rtt drv task",    0x2000} },
    {27U /* OSAL_THREAD_IMP_DRV         */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp drv task"    ,    0x2000} },

    {1000U /*OSAL_THREAD_RESERVE_00     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task00"  ,    0x2000} },
    {1001U /*OSAL_THREAD_RESERVE_01     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task01"  ,    0x2000} },
    {1002U /*OSAL_THREAD_RESERVE_02     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task02"  ,    0x2000} },
    {1003U /*OSAL_THREAD_RESERVE_03     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task03"  ,    0x2000} },
    {1004U /*OSAL_THREAD_RESERVE_04     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task04"  ,    0x2000} },
    {1005U /*OSAL_THREAD_RESERVE_05     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task05"  ,    0x2000} },
    {1006U /*OSAL_THREAD_RESERVE_06     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task06"  ,    0x2000} },
    {1007U /*OSAL_THREAD_RESERVE_07     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task07"  ,    0x2000} },
    {1008U /*OSAL_THREAD_RESERVE_08     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task08"  ,    0x2000} },
    {1009U /*OSAL_THREAD_RESERVE_09     */, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "reserve task09"  ,    0x2000} },

    /* xOS2 resource information */
    {OSAL_THREAD_ICCOM_DEMO_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom demor task",     0x400} },
    {OSAL_THREAD_ICCOM_MGR_TASK,    {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom manager task",   0x400} },
    {OSAL_THREAD_ICCOM_TIMER_TASK,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom timer task",     0x400} },
    {OSAL_THREAD_ICCOM_RCV_TASK0,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task0",  0x600} },
    {OSAL_THREAD_ICCOM_RCV_TASK1,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task1",  0x600} },
    {OSAL_THREAD_ICCOM_RCV_TASK2,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task2",  0x600} },
    {OSAL_THREAD_ICCOM_RCV_TASK3,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task3",  0x600} },
    {OSAL_THREAD_ICCOM_RCV_TASK4,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task4",  0x600} },
    {OSAL_THREAD_ICCOM_RCV_TASK5,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task5",  0x600} },
    {OSAL_THREAD_ICCOM_RCV_TASK6,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task6",  0x600} },
    {OSAL_THREAD_ICCOM_RCV_TASK7,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom receive task7",  0x600} },
    {OSAL_THREAD_ICCOM_READ_TASK0,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task0",     0x400} },
    {OSAL_THREAD_ICCOM_READ_TASK1,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task1",     0x400} },
    {OSAL_THREAD_ICCOM_READ_TASK2,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task2",     0x400} },
    {OSAL_THREAD_ICCOM_READ_TASK3,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task3",     0x400} },
    {OSAL_THREAD_ICCOM_READ_TASK4,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task4",     0x400} },
    {OSAL_THREAD_ICCOM_READ_TASK5,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task5",     0x400} },
    {OSAL_THREAD_ICCOM_READ_TASK6,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task6",     0x400} },
    {OSAL_THREAD_ICCOM_READ_TASK7,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom read task7",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK0,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task0",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK1,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task1",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK2,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task2",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK3,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task3",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK4,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task4",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK5,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task5",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK6,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task6",     0x400} },
    {OSAL_THREAD_ICCOM_SEND_TASK7,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "iccom send task7",     0x400} },
    {OSAL_THREAD_IMP_RTT_DRV,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp rtt drv task",     0x800} },
    {OSAL_THREAD_IMP_DRV,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "imp drv task",         0x800} },
    {OSAL_THREAD_LDE_CAPUTRE_TASK,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpLdeTaskCap",       0x800} },
    {OSAL_THREAD_LDE_OUTPUT_TASK,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpLdeTaskOut",       0x800} },
    {OSAL_THREAD_LDE_CTX_TASK,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "rivpLdeTaskCtx",       0x800} },
    {OSAL_THREAD_OSAL_00,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task01",          0x800} },
    {OSAL_THREAD_OSAL_01,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task02",          0x800} },
    {OSAL_THREAD_OSAL_02,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task03",          0x800} },
    {OSAL_THREAD_OSAL_03,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task04",          0x800} },
    {OSAL_THREAD_OSAL_04,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task05",          0x800} },
    {OSAL_THREAD_OSAL_05,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task06",          0x800} },
    {OSAL_THREAD_OSAL_06,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task07",          0x800} },
    {OSAL_THREAD_OSAL_07,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task08",          0x800} },
    {OSAL_THREAD_OSAL_08,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task09",          0x800} },
    {OSAL_THREAD_OSAL_09,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "osal_task10",          0x800} },
    {OSAL_THREAD_LOGO_00,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE9,   NULL,                  0x1800} },
    {OSAL_THREAD_LOGO_01,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE8,   NULL,                  0x1800} },
    {OSAL_THREAD_LOGO_02,           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE7,   NULL,                  0x1800} },
    {OSAL_THREAD_REARVIEW_00,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE7,   NULL,                  0x1800} },
    {OSAL_THREAD_REARVIEW_01,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE7,   NULL,                  0x1800} },
    {OSAL_THREAD_REARVIEW_CIO_SERVER,{NULL, NULL, OSAL_THREAD_PRIORITY_TYPE9,  "CioServer",           0x1800} },
    {OSAL_THREAD_REARVIEW_TAURUS,   {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE8,   "Taurus",              0x1800} },
    {OSAL_THREAD_RVC_MAIN,          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE7,   "RvcMain",             0x1800} },
    {OSAL_THREAD_TAURUS_MAIN,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE1,   "TaurusMain",          0x800} },
    {OSAL_THREAD_TAURUS_CAN_EVENT,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE1,   "Taurus-CanEvent",     0x800} },
    {OSAL_THREAD_TAURUS_COM_SERVER, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "ComServer",           0x800} },
    {OSAL_THREAD_TAURUS_RVGC,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE11,  "Taurus-RVGC",         0x800} },
    {OSAL_THREAD_CIO_MAIN,          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE13,  "CioMain",             0x800} },
    {OSAL_THREAD_CIO_CANFD01,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd01",   0x1800} },
    {OSAL_THREAD_CIO_CANFD02,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd02",   0x1800} },
    {OSAL_THREAD_CIO_CANFD03,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd03",   0x1800} },
    {OSAL_THREAD_CIO_CANFD04,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd04",   0x1800} },
    {OSAL_THREAD_CIO_CANFD05,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd05",   0x1800} },
    {OSAL_THREAD_CIO_CANFD06,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd06",   0x1800} },
    {OSAL_THREAD_CIO_CANFD07,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd07",   0x1800} },
    {OSAL_THREAD_CIO_CANFD08,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_cio_canfd08",   0x1800} },
    {OSAL_THREAD_WM01,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM01",          0x1800} },
    {OSAL_THREAD_WM02,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM02",          0x1800} },
    {OSAL_THREAD_WM03,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM03",          0x1800} },
    {OSAL_THREAD_WM04,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM04",          0x1800} },
    {OSAL_THREAD_WM05,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM05",          0x1800} },
    {OSAL_THREAD_WM06,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM06",          0x1800} },
    {OSAL_THREAD_WM07,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM07",          0x1800} },
    {OSAL_THREAD_WM08,              {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_WM08",          0x1800} },
    {OSAL_THREAD_VIN01,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN01",         0x1800} },
    {OSAL_THREAD_VIN02,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN02",         0x1800} },
    {OSAL_THREAD_VIN03,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN03",         0x1800} },
    {OSAL_THREAD_VIN04,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN04",         0x1800} },
    {OSAL_THREAD_VIN05,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN05",         0x1800} },
    {OSAL_THREAD_VIN06,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN06",         0x1800} },
    {OSAL_THREAD_VIN07,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN07",         0x1800} },
    {OSAL_THREAD_VIN08,             {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "R-CAR_VIN08",         0x1800} },
    {OSAL_THREAD_CIO_DUMMY01,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy01",             0x1800} },
    {OSAL_THREAD_CIO_DUMMY02,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy02",             0x1800} },
    {OSAL_THREAD_CIO_DUMMY03,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy03",             0x1800} },
    {OSAL_THREAD_CIO_DUMMY04,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy04",             0x1800} },
    {OSAL_THREAD_CIO_DUMMY05,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy05",             0x1800} },
    {OSAL_THREAD_CIO_DUMMY06,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy06",             0x1800} },
    {OSAL_THREAD_CIO_DUMMY07,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy07",             0x1800} },
    {OSAL_THREAD_CIO_DUMMY08,       {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE14,  "Dummy08",             0x1800} },
    {OSAL_THREAD_TAURUS_PARACAMERA, {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE11,  NULL,                  0x800} },
    {OSAL_THREAD_DHD_KERNEL,        {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE11,  "DHDKernel",           0x4000} },
    {OSAL_THREAD_DRW2D_TASK00,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task00",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK01,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task01",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK02,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task02",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK03,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task03",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK04,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task04",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK05,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task05",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK06,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task06",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK07,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task07",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK08,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task08",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK09,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task09",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK10,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task10",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK11,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task11",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK12,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task12",        0x4000} },
    {OSAL_THREAD_DRW2D_TASK13,      {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "drw2d_task13",        0x4000} },
    {OSAL_THREAD_RIVP_SAMPLE_MAIN,  {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "RIVP_SAMPLE_MAIN",    0x2000} },
    {OSAL_THREAD_RIVP_TASK_DEC,     {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "rivpTaskDec",         0x2000} },
    {OSAL_THREAD_RIVP_TASK_OUT,     {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10,  "rivpTaskOut",         0x2000} },

    {.id = OSAL_THREAD_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var gs_osal_mutex_config
 * osal user configuration : mutex configuration table
***********************************************************************************************************************/
st_osal_rcfg_mutex_config_t gs_osal_mutex_config[] =
{
    /* mutex id               */

    /* xOS1 resource information. This is the compatibility of the alpha version */
    /* Will be removed in the future */
    { 0U /* OSAL_MUTEX_IMR_00         */},
    { 1U /* OSAL_MUTEX_IMR_01         */},
    { 2U /* OSAL_MUTEX_IMR_02         */},
    { 3U /* OSAL_MUTEX_IMR_03         */},
    { 4U /* OSAL_MUTEX_IMR_04         */},
    { 5U /* OSAL_MUTEX_IMR_05         */},
    { 6U /* OSAL_MUTEX_DOF0_00        */},
    { 7U /* OSAL_MUTEX_DOF0_01        */},
    { 8U /* OSAL_MUTEX_STV0_00        */},
    { 9U /* OSAL_MUTEX_STV0_01        */},
    {10U /* OSAL_MUTEX_ACF0_00        */},
    {11U /* OSAL_MUTEX_ACF0_01        */},
    {12U /* OSAL_MUTEX_ACF1_00        */},
    {13U /* OSAL_MUTEX_ACF1_01        */},
    {14U /* OSAL_MUTEX_ACF2_00        */},
    {15U /* OSAL_MUTEX_ACF2_01        */},
    {16U /* OSAL_MUTEX_ACF3_00        */},
    {17U /* OSAL_MUTEX_ACF3_01        */},
    {18U /* OSAL_MUTEX_ACF4_00        */},
    {19U /* OSAL_MUTEX_ACF4_01        */},
    {20U /* OSAL_MUTEX_VIPRTT_00      */},
    {21U /* OSAL_MUTEX_VIPRTT_01      */},
    {22U /* OSAL_MUTEX_VIPRTT_02      */},
    {23U /* OSAL_MUTEX_VIPRTT_03      */},
    {24U /* OSAL_MUTEX_ICCOM_WRITE_00 */},
    {25U /* OSAL_MUTEX_ICCOM_WRITE_01 */},
    {26U /* OSAL_MUTEX_ICCOM_WRITE_02 */},
    {27U /* OSAL_MUTEX_ICCOM_WRITE_03 */},
    {28U /* OSAL_MUTEX_ICCOM_WRITE_04 */},
    {29U /* OSAL_MUTEX_ICCOM_WRITE_05 */},
    {30U /* OSAL_MUTEX_ICCOM_WRITE_06 */},
    {31U /* OSAL_MUTEX_ICCOM_WRITE_07 */},
    {32U /* OSAL_MUTEX_ICCOM_READ_00  */},
    {33U /* OSAL_MUTEX_ICCOM_READ_01  */},
    {34U /* OSAL_MUTEX_ICCOM_READ_02  */},
    {35U /* OSAL_MUTEX_ICCOM_READ_03  */},
    {36U /* OSAL_MUTEX_ICCOM_READ_04  */},
    {37U /* OSAL_MUTEX_ICCOM_READ_05  */},
    {38U /* OSAL_MUTEX_ICCOM_READ_06  */},
    {39U /* OSAL_MUTEX_ICCOM_READ_07  */},
    {40U /* OSAL_MUTEX_ICCOM_TIMER    */},
    {41U /* OSAL_MUTEX_IMP_FW_LOCK    */},
    {42U /* OSAL_MUTEX_IMP_RTT_LOCK   */},
    {43U /* OSAL_MUTEX_IMP_DRV_LOCK   */},
    {44U /* OSAL_MUTEX_ENCODER_00     */},
    {45U /* OSAL_MUTEX_ENCODER_01     */},
    {46U /* OSAL_MUTEX_ENCODER_02     */},
    {47U /* OSAL_MUTEX_CISP_00        */},
    {48U /* OSAL_MUTEX_CISP_01        */},
    {49U /* OSAL_MUTEX_TISP_00        */},
    {50U /* OSAL_MUTEX_TISP_01        */},
    {51U /* OSAL_MUTEX_ENCODER_APP_00 */},
    {52U /* OSAL_MUTEX_ENCODER_APP_01 */},
    {53U /* OSAL_MUTEX_ICCOM_OPEN     */},

    {1000U /*OSAL_MUTEX_RESERVE_00     */},
    {1001U /*OSAL_MUTEX_RESERVE_01     */},
    {1002U /*OSAL_MUTEX_RESERVE_02     */},
    {1003U /*OSAL_MUTEX_RESERVE_03     */},
    {1004U /*OSAL_MUTEX_RESERVE_04     */},
    {1005U /*OSAL_MUTEX_RESERVE_05     */},
    {1006U /*OSAL_MUTEX_RESERVE_06     */},
    {1007U /*OSAL_MUTEX_RESERVE_07     */},
    {1008U /*OSAL_MUTEX_RESERVE_08     */},
    {1009U /*OSAL_MUTEX_RESERVE_09     */},

    /* xOS2 resource information */
    {OSAL_MUTEX_IMR_00         },
    {OSAL_MUTEX_IMR_01         },
    {OSAL_MUTEX_IMR_02         },
    {OSAL_MUTEX_IMR_03         },
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
    {OSAL_MUTEX_IMP_FW_LOCK    },
    {OSAL_MUTEX_IMP_RTT_LOCK   },
    {OSAL_MUTEX_IMP_DRV_LOCK   },
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
    {OSAL_MUTEX_CIOLIB         },
    {OSAL_MUTEX_CIOLIB2        },
    {OSAL_MUTEX_CIOVIN_00      },
    {OSAL_MUTEX_DRIVER_DMAC_00 },
    {OSAL_MUTEX_WM1            },
    {OSAL_MUTEX_WM2            },
    {OSAL_MUTEX_REARVIEW1      },
    {OSAL_MUTEX_REARVIEW2      },
    {OSAL_MUTEX_TAURUS_CAN     },
    {OSAL_MUTEX_TAURUS_RVGC    },
    {OSAL_MUTEX_CIO_CANFD      },
    {OSAL_MUTEX_CIO_CANFD2     },
    {OSAL_MUTEX_CIO_SERVER_CH0 },
    {OSAL_MUTEX_CIO_SERVER_CH1 },
    {OSAL_MUTEX_CIO_SERVER_CH2 },
    {OSAL_MUTEX_CIO_SERVER_CH3 },
    {OSAL_MUTEX_CIO_SERVER_CH4 },
    {OSAL_MUTEX_CIO_SERVER_CH5 },
    {OSAL_MUTEX_CIO_SERVER_CH6 },
    {OSAL_MUTEX_CIO_SERVER_CH7 },
    {OSAL_MUTEX_CIOVIN_01      },
    {OSAL_MUTEX_DRIVER_DMAC_01 },
    {OSAL_MUTEX_DRIVER_DMAC2   },
    {OSAL_MUTEX_DRIVER_DMAC3   },
    {OSAL_MUTEX_DRIVER_DMAC4   },
    {OSAL_MUTEX_DRIVER_DMAC5   },
    {OSAL_MUTEX_WM1_01         },
    {OSAL_MUTEX_WM1_02         },
    {OSAL_MUTEX_WM1_03         },
    {OSAL_MUTEX_WM1_04         },
    {OSAL_MUTEX_WM2_01         },
    {OSAL_MUTEX_WM2_02         },
    {OSAL_MUTEX_WM2_03         },
    {OSAL_MUTEX_WM2_04         },
    {OSAL_MUTEX_WM2_05         },
    {OSAL_MUTEX_WM2_06         },
    {OSAL_MUTEX_WM2_07         },
    {OSAL_MUTEX_WM2_08         },
    {OSAL_MUTEX_WM2_09         },
    {OSAL_MUTEX_WM2_10         },
    {OSAL_MUTEX_WM2_11         },
    {OSAL_MUTEX_WM2_12         },
    {OSAL_MUTEX_WM2_13         },
    {OSAL_MUTEX_WM2_14         },
    {OSAL_MUTEX_WM2_15         },
    {OSAL_MUTEX_WM2_16         },
    {OSAL_MUTEX_WM2_17         },
    {OSAL_MUTEX_WM2_18         },
    {OSAL_MUTEX_WM2_19         },
    {OSAL_MUTEX_WM2_21         },
    {OSAL_MUTEX_WM2_22         },
    {OSAL_MUTEX_WM2_23         },
    {OSAL_MUTEX_WM2_24         },
    {OSAL_MUTEX_WM2_25         },
    {OSAL_MUTEX_WM2_26         },
    {OSAL_MUTEX_WM2_27         },
    {OSAL_MUTEX_WM2_28         },
    {OSAL_MUTEX_WM2_29         },
    {OSAL_MUTEX_CIO_LIB_CH0    },
    {OSAL_MUTEX_CIO_LIB_CH1    },
    {OSAL_MUTEX_CIO_LIB_CH2    },
    {OSAL_MUTEX_CIO_LIB_CH3    },
    {OSAL_MUTEX_CIO_LIB_CH4    },
    {OSAL_MUTEX_CIO_LIB_CH5    },
    {OSAL_MUTEX_CIO_LIB_CH6    },
    {OSAL_MUTEX_CIO_LIB_CH7    },
    {OSAL_MUTEX_CIO_LIB_CH8    },
    {OSAL_MUTEX_CIO_LIB_CH9    },
    {OSAL_MUTEX_WM1_05         },
    {OSAL_MUTEX_WM1_06         },
    {OSAL_MUTEX_WM1_07         },
    {OSAL_MUTEX_TAURUS_PARACAMERA    },
    {OSAL_MUTEX_TAURUS_RPMSG         },
    {OSAL_MUTEX_RTDMAC_CH00          },
    {OSAL_MUTEX_RTDMAC_CH01          },
    {OSAL_MUTEX_RTDMAC_CH02          },
    {OSAL_MUTEX_RTDMAC_CH03          },
    {OSAL_MUTEX_RTDMAC_CH04          },
    {OSAL_MUTEX_RTDMAC_CH05          },
    {OSAL_MUTEX_RTDMAC_CH06          },
    {OSAL_MUTEX_RTDMAC_CH07          },
    {OSAL_MUTEX_RTDMAC_CH08          },
    {OSAL_MUTEX_RTDMAC_CH09          },
    {OSAL_MUTEX_RTDMAC_CH10          },
    {OSAL_MUTEX_RTDMAC_CH11          },
    {OSAL_MUTEX_RTDMAC_CH12          },
    {OSAL_MUTEX_RTDMAC_CH13          },
    {OSAL_MUTEX_RTDMAC_CH14          },
    {OSAL_MUTEX_RTDMAC_CH15          },
    {OSAL_MUTEX_DHD_SYNC_INTERRUPT             },
    {OSAL_MUTEX_DHD_ERROR_INTERRUPT            },
    {OSAL_MUTEX_DHD_STOP_INTERRUPT             },
    {OSAL_MUTEX_DHD_WAKEUP_SERVER              },
    {OSAL_MUTEX_DHD_LOCK_DEVICE                },
    {OSAL_MUTEX_DHD_LOCK_ALLOCATOR             },
    {OSAL_MUTEX_DRW2D_LOCK_API_00              },
    {OSAL_MUTEX_DRW2D_LOCK_API_01              },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_00        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_00        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_01        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_02        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_03        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_04        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_05        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_06        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_07        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_08        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_09        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_10        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_11        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_12        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_13        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_14        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_15        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_16        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_17        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_18        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_19        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_20        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_21        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_22        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_23        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_24        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_25        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_26        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_27        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_28        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_29        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_30        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_31        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_32        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_33        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_34        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_35        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_36        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_37        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_38        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_39        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_40        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_41        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_42        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_43        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_44        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_45        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_46        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_47        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_48        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_49        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_50        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_51        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_52        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_53        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_54        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_55        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_56        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_57        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_58        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_59        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_60        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_61        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_62        },
    {OSAL_MUTEX_DRW2D_DEVICE_CONTEXT_63        },
    {OSAL_MUTEX_LDD_00                         },
    {OSAL_MUTEX_LDD_01                         },
    {OSAL_MUTEX_LDD_02                         },
    {OSAL_MUTEX_LDD_03                         },
    {OSAL_MUTEX_LDD_04                         },
    {OSAL_MUTEX_LDD_05                         },
    {OSAL_MUTEX_LDD_06                         },
    {OSAL_MUTEX_LDD_07                         },
    {OSAL_MUTEX_LDD_LOG_00                     },
    {OSAL_MUTEX_LDD_LOG_01                     },
    {.id = OSAL_MUTEX_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var gs_osal_cond_config
 * osal user configuration : cond configuration table
***********************************************************************************************************************/
st_osal_rcfg_cond_config_t gs_osal_cond_config[] =
{
    /* cond id               */

    /* xOS1 resource information. This is the compatibility of the alpha version */
    /* Will be removed in the future */
    {1000U /* OSAL_COND_RESERVE_00      */},
    {1001U /* OSAL_COND_RESERVE_01      */},
    {1002U /* OSAL_COND_RESERVE_02      */},
    {1003U /* OSAL_COND_RESERVE_03      */},
    {1004U /* OSAL_COND_RESERVE_04      */},
    {1005U /* OSAL_COND_RESERVE_05      */},
    {1006U /* OSAL_COND_RESERVE_06      */},
    {1007U /* OSAL_COND_RESERVE_07      */},
    {1008U /* OSAL_COND_RESERVE_08      */},
    {1009U /* OSAL_COND_RESERVE_09      */},

    /* xOS2 resource information */
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
    {OSAL_COND_DHD_SYNC_INTERRUPT  },
    {OSAL_COND_DHD_ERROR_INTERRUPT },
    {OSAL_COND_DHD_STOP_INTERRUPT  },
    {OSAL_COND_DHD_WAKEUP_SERVER   },
    {OSAL_COND_DRW2D_VSYNC_00      },
    {OSAL_COND_DRW2D_VSYNC_01      },
    {OSAL_COND_DRW2D_VSYNC_02      },
    {OSAL_COND_DRW2D_VSYNC_03      },
    {OSAL_COND_DRW2D_VSYNC_04      },
    {OSAL_COND_DRW2D_VSYNC_05      },
    {OSAL_COND_REARVIEW_CLUSTER    },
    {OSAL_COND_REARVIEW_CID        },
    {OSAL_COND_CIO_CANFD_IRQ_GL        },
    {OSAL_COND_CIO_CANFD_IRQ_CH        },
    {OSAL_COND_TAURUS_CAN              },
    {OSAL_COND_TAURUS_RVGC             },
    {OSAL_COND_WM_00                   },
    {OSAL_COND_WM_01                   },
    {OSAL_COND_WM_02                   },
    {OSAL_COND_WM_03                   },
    {OSAL_COND_WM_04                   },
    {OSAL_COND_WM_05                   },
    {OSAL_COND_WM_06                   },
    {OSAL_COND_WM_07                   },
    {OSAL_COND_WM_08                   },
    {OSAL_COND_WM_09                   },
    {OSAL_COND_WM_10                   },
    {OSAL_COND_WM_11                   },
    {OSAL_COND_WM_12                   },
    {OSAL_COND_WM_13                   },
    {OSAL_COND_WM_14                   },
    {OSAL_COND_CANFD_GLOBAL            },
    {OSAL_COND_CANFD_CHANNEL           },
    {OSAL_COND_RTDMAC_CH00             },
    {OSAL_COND_RTDMAC_CH01             },
    {OSAL_COND_RTDMAC_CH02             },
    {OSAL_COND_RTDMAC_CH03             },
    {OSAL_COND_RTDMAC_CH04             },
    {OSAL_COND_RTDMAC_CH05             },
    {OSAL_COND_RTDMAC_CH06             },
    {OSAL_COND_RTDMAC_CH07             },
    {OSAL_COND_RTDMAC_CH08             },
    {OSAL_COND_RTDMAC_CH09             },
    {OSAL_COND_RTDMAC_CH10             },
    {OSAL_COND_RTDMAC_CH11             },
    {OSAL_COND_RTDMAC_CH12             },
    {OSAL_COND_RTDMAC_CH13             },
    {OSAL_COND_RTDMAC_CH14             },
    {OSAL_COND_RTDMAC_CH15             },
    {OSAL_COND_WM_15                   },
    {OSAL_COND_WM_16                   },
    {OSAL_COND_WM_17                   },
    {OSAL_COND_WM_18                   },
    {OSAL_COND_WM_19                   },

    {.id = OSAL_COND_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var gs_osal_mq_config
 * osal user configuration : message queue configuration table
***********************************************************************************************************************/
st_osal_mq_rcfg_config_t gs_osal_mq_config[] =
{
    /* mq id,                       max_num_msg, msg_size */
    /* xOS1 resource information. This is the compatibility of the alpha version */
    /* Will be removed in the future */
    { 0U /* OSAL_MQ_IMR_MSG_00,                    */, {126, 12} },
    { 1U /* OSAL_MQ_IMR_MSG_01,                    */, {10, 4}   },
    { 2U /* OSAL_MQ_IMR_MSG_02,                    */, {1, 4}    },
    { 3U /* OSAL_MQ_IMR_MSG_03,                    */, {1, 4}    },
    { 4U /* OSAL_MQ_IMR_MSG_04,                    */, {1, 4}    },
    { 5U /* OSAL_MQ_IMR_MSG_05,                    */, {1, 4}    },
    { 6U /* OSAL_MQ_IMR_RTT_MSG_00,                */, {1, 4}    },
    { 7U /* OSAL_MQ_IMR_RTT_MSG_01,                */, {1, 4}    },
    { 8U /* OSAL_MQ_IMR_RTT_MSG_02,                */, {1, 4}    },
    { 9U /* OSAL_MQ_IMR_RTT_MSG_03,                */, {1, 4}    },
    {10U /* OSAL_MQ_IMR_RTT_MSG_04,                */, {126, 272}},
    {11U /* OSAL_MQ_IMR_RTT_MSG_05,                */, {126, 8}  },
    {12U /* OSAL_MQ_ICCOM_MGR_TASK,                */, {8, 16}   },
    {13U /* OSAL_MQ_ICCOM_REQ_OPEN,                */, {1, 16}   },
    {14U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_00,      */, {1, 16}   },
    {15U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_01,      */, {1, 16}   },
    {16U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_02,      */, {1, 16}   },
    {17U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_03,      */, {1, 16}   },
    {18U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_04,      */, {1, 16}   },
    {19U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_05,      */, {1, 16}   },
    {20U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_06,      */, {1, 16}   },
    {21U /* OSAL_MQ_ICCOM_SEND_DATA_UPPER_07,      */, {1, 16}   },
    {22U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_00,     */, {1, 16}   },
    {23U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_01,     */, {1, 16}   },
    {24U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_02,     */, {1, 16}   },
    {25U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_03,     */, {1, 16}   },
    {26U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_04,     */, {1, 16}   },
    {27U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_05,     */, {1, 16}   },
    {28U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_06,     */, {1, 16}   },
    {29U /* OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_07,     */, {1, 16}   },
    {30U /* OSAL_MQ_ICCOM_RECEIVE_TASK_00,         */, {1, 16}   },
    {31U /* OSAL_MQ_ICCOM_RECEIVE_TASK_01,         */, {1, 16}   },
    {32U /* OSAL_MQ_ICCOM_RECEIVE_TASK_02,         */, {1, 16}   },
    {33U /* OSAL_MQ_ICCOM_RECEIVE_TASK_03,         */, {1, 16}   },
    {34U /* OSAL_MQ_ICCOM_RECEIVE_TASK_04,         */, {1, 16}   },
    {35U /* OSAL_MQ_ICCOM_RECEIVE_TASK_05,         */, {1, 16}   },
    {36U /* OSAL_MQ_ICCOM_RECEIVE_TASK_06,         */, {1, 16}   },
    {37U /* OSAL_MQ_ICCOM_RECEIVE_TASK_07,         */, {1, 16}   },
    {38U /* OSAL_MQ_ICCOM_SEND_TASK_00,            */, {1, 16}   },
    {39U /* OSAL_MQ_ICCOM_SEND_TASK_01,            */, {1, 16}   },
    {40U /* OSAL_MQ_ICCOM_SEND_TASK_02,            */, {1, 16}   },
    {41U /* OSAL_MQ_ICCOM_SEND_TASK_03,            */, {1, 16}   },
    {42U /* OSAL_MQ_ICCOM_SEND_TASK_04,            */, {1, 16}   },
    {43U /* OSAL_MQ_ICCOM_SEND_TASK_05,            */, {1, 16}   },
    {44U /* OSAL_MQ_ICCOM_SEND_TASK_06,            */, {1, 16}   },
    {45U /* OSAL_MQ_ICCOM_SEND_TASK_07,            */, {1, 16}   },
    {46U /* OSAL_MQ_ICCOM_READ_TASK_00,            */, {1, 16}   },
    {47U /* OSAL_MQ_ICCOM_READ_TASK_01,            */, {1, 16}   },
    {48U /* OSAL_MQ_ICCOM_READ_TASK_02,            */, {1, 16}   },
    {49U /* OSAL_MQ_ICCOM_READ_TASK_03,            */, {1, 16}   },
    {50U /* OSAL_MQ_ICCOM_READ_TASK_04,            */, {1, 16}   },
    {51U /* OSAL_MQ_ICCOM_READ_TASK_05,            */, {1, 16}   },
    {52U /* OSAL_MQ_ICCOM_READ_TASK_06,            */, {1, 16}   },
    {53U /* OSAL_MQ_ICCOM_READ_TASK_07,            */, {1, 16}   },
    {54U /* OSAL_MQ_ICCOM_SEND_DATA_00,            */, {1, 2048} },
    {55U /* OSAL_MQ_ICCOM_SEND_DATA_01,            */, {1, 2048} },
    {56U /* OSAL_MQ_ICCOM_SEND_DATA_02,            */, {1, 2048} },
    {57U /* OSAL_MQ_ICCOM_SEND_DATA_03,            */, {1, 2048} },
    {58U /* OSAL_MQ_ICCOM_SEND_DATA_04,            */, {1, 2048} },
    {59U /* OSAL_MQ_ICCOM_SEND_DATA_05,            */, {1, 2048} },
    {60U /* OSAL_MQ_ICCOM_SEND_DATA_06,            */, {1, 2048} },
    {61U /* OSAL_MQ_ICCOM_SEND_DATA_07,            */, {1, 2048} },
    {62U /* OSAL_MQ_ICCOM_RECEIVE_DATA_00,         */, {1, 2048} },
    {63U /* OSAL_MQ_ICCOM_RECEIVE_DATA_01,         */, {1, 2048} },
    {64U /* OSAL_MQ_ICCOM_RECEIVE_DATA_02,         */, {1, 2048} },
    {65U /* OSAL_MQ_ICCOM_RECEIVE_DATA_03,         */, {1, 2048} },
    {66U /* OSAL_MQ_ICCOM_RECEIVE_DATA_04,         */, {1, 2048} },
    {67U /* OSAL_MQ_ICCOM_RECEIVE_DATA_05,         */, {1, 2048} },
    {68U /* OSAL_MQ_ICCOM_RECEIVE_DATA_06,         */, {1, 2048} },
    {69U /* OSAL_MQ_ICCOM_RECEIVE_DATA_07,         */, {1, 2048} },
    {70U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_00,   */, {1, 16}   },
    {71U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_01,   */, {1, 16}   },
    {72U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_02,   */, {1, 16}   },
    {73U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_03,   */, {1, 16}   },
    {74U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_04,   */, {1, 16}   },
    {75U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_05,   */, {1, 16}   },
    {76U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_06,   */, {1, 16}   },
    {77U /* OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_07,   */, {1, 16}   },
    {78U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_00,*/, {1, 16}   },
    {79U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_01,*/, {1, 16}   },
    {80U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_02,*/, {1, 16}   },
    {81U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_03,*/, {1, 16}   },
    {82U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_04,*/, {1, 16}   },
    {83U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_05,*/, {1, 16}   },
    {84U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_06,*/, {1, 16}   },
    {85U /* OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_07,*/, {1, 16}   },
    {86U /* OSAL_MQ_IMP_RTT_TASK_REQ,              */, {28, 8}   },
    {87U /* OSAL_MQ_IMP_RTT_TASK_ACK,              */, {2, 4}    },
    {88U /* OSAL_MQ_IMP_RTT_DRVCB_00,              */, {2, 4}    },
    {89U /* OSAL_MQ_IMP_RTT_EXECCB,                */, {2, 4}    },
    {90U /* OSAL_MQ_IMP_DRV_TASK_CBFUNC,           */, {40, 24}  },
    {91U /* OSAL_MQ_IMP_DRV_TASK_DELTASK,          */, {2, 4}    },

    {1000U /* OSAL_MQ_RESERVE_00,                  */, {16, 64}  },
    {1001U /* OSAL_MQ_RESERVE_01,                  */, {16, 64}  },
    {1002U /* OSAL_MQ_RESERVE_02,                  */, {16, 64}  },
    {1003U /* OSAL_MQ_RESERVE_03,                  */, {16, 64}  },
    {1004U /* OSAL_MQ_RESERVE_04,                  */, {16, 64}  },
    {1005U /* OSAL_MQ_RESERVE_05,                  */, {16,196}  },
    {1006U /* OSAL_MQ_RESERVE_06,                  */, {16,196}  },
    {1007U /* OSAL_MQ_RESERVE_07,                  */, {16,196}  },
    {1008U /* OSAL_MQ_RESERVE_08,                  */, {16,196}  },
    {1009U /* OSAL_MQ_RESERVE_09,                  */, {16,196}  },

    /* xOS2 resource information */
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
    {OSAL_MQ_LDE_APP_MAIN,                   {16, 64}  },
    {OSAL_MQ_LDE_APP_OUT,                    {16, 64}  },
    {OSAL_MQ_LDE_APP_CTX,                    {16, 196} },
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
    {OSAL_MQ_CIO_SERVER_00,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_01,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_02,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_03,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_04,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_05,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_06,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_07,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_08,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_09,                  {10, 20}  },
    {OSAL_MQ_CIO_SERVER_10,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_11,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_12,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_13,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_14,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_15,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_16,                  {10, 12}  },
    {OSAL_MQ_CIO_SERVER_17,                  {10, 12}  },
    {OSAL_MQ_REARVIEW_CAMERA_APP,            {10, 12}  },
    {OSAL_MQ_TAURUS_PARACAN,                 {10, 12}  },
    {OSAL_MQ_TAURUS_PARARVGC,                {10, 12}  },
    {OSAL_MQ_TAURUS_PARAVIN,                 {10, 12}  },
    {OSAL_MQ_DHD_SERVER,                     {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_00,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_01,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_02,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_03,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_04,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_05,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_06,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_07,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_08,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_09,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_10,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_11,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_12,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_13,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_14,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_15,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_16,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_17,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_18,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_19,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_20,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_21,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_22,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_23,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_24,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_25,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_26,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_27,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_28,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_29,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_30,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_31,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_32,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_33,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_34,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_35,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_36,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_37,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_38,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_39,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_40,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_41,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_42,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_43,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_44,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_45,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_46,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_47,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_48,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_49,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_50,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_51,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_52,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_53,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_54,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_55,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_56,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_57,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_58,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_59,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_60,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_61,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_62,                  {64, 64}  },
    {OSAL_MQ_DHD_CLIENT_63,                  {64, 64}  },
    {OSAL_MQ_LDD_00,                         {16, 64}  },
    {OSAL_MQ_LDD_01,                         {16, 64}  },
    {OSAL_MQ_LDD_02,                         {16, 64}  },
    {OSAL_MQ_LDD_03,                         {16,196}  },

    {.id = OSAL_MQ_INVALID_ID}
};

/*******************************************************************************************************************//**
 * @var gs_osal_interrupt_thread_config
 * osal user configuration : interrupt thread configuration table
***********************************************************************************************************************/
st_osal_interrupt_thread_config_t  gs_osal_interrupt_thread_config[] =
{
    /* irq, device_id               thread  priority  interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {192, "imr_00",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imr00",                 0x0800},
    {193, "imr_01",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imr01",                 0x0800},
    {194, "imr_02",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imr02",                 0x0800},
    {195, "imr_03",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imr03",                 0x0800},
    {224, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-1",            0x0800},
    {225, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-2",            0x0800},
    {226, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-3",            0x0800},
    {227, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-4",            0x0800},
    {228, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-5",            0x0800},
    {229, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-6",            0x0800},
    {230, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-7",            0x0800},
    {231, "iccom_00",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iccom_00-8",            0x0800},
    {300, "ivcp1e_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "ivcp1e_00-1",           0x0800},
    {170, "ivcp1e_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "ivcp1e_00-2",           0x0800},
    {281, "imp_top_00",             OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imp_top_00-1",          0x0800},
    {282, "imp_top_00",             OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imp_top_00-2",          0x0800},
    {283, "imp_top_00",             OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imp_top_00-3",          0x0800},
    {284, "imp_top_00",             OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imp_top_00-4",          0x0800},
    {285, "imp_top_00",             OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "imp_top_00-5",          0x0800},
    {14,  "fbc_00",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "fbc_00-01",             0x0800},
    {15,  "fbc_00",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "fbc_00-02",             0x0800},
    {32,  "rfso_000",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_000",              0x0800},
    {33,  "rfso_001",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_001",              0x0800},
    {34,  "rfso_002",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_002",              0x0800},
    {35,  "rfso_003",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_003",              0x0800},
    {36,  "rfso_004",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_004",              0x0800},
    {37,  "rfso_005",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_005",              0x0800},
    {221, "rfso_006",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_006",              0x0800},
    {186, "rfso_007",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_007",              0x0800},
    {187, "rfso_008",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_008",              0x0800},
    {185, "rfso_009",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_009",              0x0800},
    {247, "rfso_010",               OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "rfso_010",              0x0800},

    {30,  "canfd",                  OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE9, "CanFD-IrqGlob",          0x1800},
    {29,  "canfd",                  OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE10,"CanFD-IrqChan",          0x1800},
    {188, "vin_00",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:0",                 0x1800},
    {189, "vin_01",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:1",                 0x1800},
    {190, "vin_02",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:2",                 0x1800},
    {191, "vin_03",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:3",                 0x1800},
    {174, "vin_04",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:4",                 0x1800},
    {175, "vin_05",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:5",                 0x1800},
    {176, "vin_06",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:6",                 0x1800},
    {171, "vin_07",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE12, "VIN:7",                 0x1800},
    {466, "vspd_00",                OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE13,"WMVspdIrq:0",            0x1800},
    {467, "vspd_01",                OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE13,"WMVspdIrq:1",            0x1800},
    {468, "vspd_02",                OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE13,"WMVspdIrq:2",            0x1800},

    {99,   "dhd_00",                OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE11,"DHDSync",                0x400},
    {98,   "dhd_00",                OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE11,"DHDError",               0x400},
    {97,   "dhd_00",                OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE11,"DHDStop",                0x400},

    {380, "ivdp1c_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iVDP1CVLC",              0x400},
    {381, "ivdp1c_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iVDP1CCL",               0x400},
    {223, "ivdp1c_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE0, "iVDP1CCM",               0x400},

    {432, "doc_00",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "WMDocIrq:0-DOC",         0x1800},
    {433, "doc_00",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "WMDocIrq:0-ACT",         0x1800},
    {434, "doc_01",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "WMDocIrq:1-ACT",         0x1800},
    {435, "doc_01",                 OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "WMDocIrq:1-DOC",         0x1800},

    {449, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch00",             0x1800},
    {450, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch01",             0x1800},
    {451, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch02",             0x1800},
    {452, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch03",             0x1800},
    {453, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch04",             0x1800},
    {454, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch05",             0x1800},
    {455, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch06",             0x1800},
    {456, "rtdmac_00",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch07",             0x1800},
    {457, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch08",             0x1800},
    {458, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch09",             0x1800},
    {459, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch10",             0x1800},
    {460, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch11",             0x1800},
    {461, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch12",             0x1800},
    {462, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch13",             0x1800},
    {463, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch14",             0x1800},
    {464, "rtdmac_01",              OSAL_THREAD_PRIORITY_TYPE5, OSAL_INTERRUPT_PRIORITY_TYPE1, "rtdmac ch15",             0x1800},

    {.id = OSAL_INTERRUPT_INVALID_ID}
};
