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
* File Name :    osal_test_memory.h
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of Check Memory Usage
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           0.1.0     07.04.2023    Create New
 *           0.2.0     09.11.2023    Add definition for V4M
***********************************************************************************************************************/
#ifndef OSAL_TEST_IT_MEMORT_H
#define OSAL_TEST_IT_MEMORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <malloc.h>

#if 1 /* IPL workaround */
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <hw/inout.h>
#endif

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

/*** definition ***/
#define WAIT_TIME_MS    (10)         // 10 milisecond
#define WAIT_TIME_NS    (10000000)   // 10 milisecond
#define WAIT_TIME_SEC   (1)          // 1 sec
#define MAX_NS          (1000000000)

#define INTERVAL_TIME   (1 * 1000 * 1000)
#define USEC_TO_NSEC    (1000)
#define SEC_TO_NSEC     (1000000000ULL)

#define OSAL_TEST_INFO(...) {printf(__VA_ARGS__);}
#define OSAL_TEST_ERR(...)  {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}

/* thread IDs */
#define THREAD_MAIN_TASK_ID     (0xf000U)
#define THREAD_MAIN_TASK_ID2    (0xf000U + 1U)
/* message queue IDs */
#define MQ_ID_NO1               (0xf000U)
/* mutex IDs */
#define MUTEX_ID_NO1            (0xf000U)
/* condition variable IDs */
#define COND_ID_NO1             (0xf000U)

/* IO related definitions */
#if defined (RCAR_S4)
#define DEVICE_TYPE             "caiplite";
#define DEVICE_TYPE_FOR_AXI     "rtdmac_control0";
#define DEVICE_TYPE_FOR_POWER   "caiplite";
#define AXI_BUS_NAME            "rt1_00_04";
#elif defined (RCAR_V4H) || defined (RCAR_V4M)
#define DEVICE_TYPE             "ims";
#define DEVICE_TYPE_FOR_AXI     "ims";
#define DEVICE_TYPE_FOR_POWER   "ims";
#define AXI_BUS_NAME            "vc_00_04";
#elif defined (RCAR_V3M2)
#define DEVICE_TYPE             "ims";
#define DEVICE_TYPE_FOR_AXI     "ims";
#define DEVICE_TYPE_FOR_POWER   "imp";
#define AXI_BUS_NAME            "vi0_02";
#else /* V3Hx */
#define DEVICE_TYPE             "ims";
#define DEVICE_TYPE_FOR_AXI     "ims";
#define DEVICE_TYPE_FOR_POWER   "imp";
#define AXI_BUS_NAME            "vc0_04";
#endif

#define TEST_NUM                                (104)
#define TEST_Initialize                         (0)
#define TEST_Deinitialize                       (1)
#define TEST_GetVersion                         (2)
#define TEST_ThreadCreate                       (3)
#define TEST_ThreadSleepUntilTimeStamp          (4)
#define TEST_ThreadSleepForTimePeriod           (5)
#define TEST_ThreadSelf                         (6)
#define TEST_ThreadEqual                        (7)
#define TEST_ThreadInitializeThreadConfigSt     (8)
#define TEST_ThreadJoin                         (9)
#define TEST_ThsyncMutexCreate                  (10)
#define TEST_ThsyncMutexDestroy                 (11)
#define TEST_ThsyncMutexLockUntilTimeStamp      (12)
#define TEST_ThsyncMutexLockForTimePeriod       (13)
#define TEST_ThsyncMutexTryLock                 (14)
#define TEST_ThsyncMutexUnlock                  (15)
#define TEST_ThsyncCondCreate                   (16)
#define TEST_ThsyncCondDestroy                  (17)
#define TEST_ThsyncCondWaitUntilTimeStamp       (18)
#define TEST_ThsyncCondWaitForTimePeriod        (19)
#define TEST_ThsyncCondBroadcast                (20)
#define TEST_ThsyncCondSignal                   (21)
#define TEST_ThsyncMemoryBarrier                (22)
#define TEST_MqCreate                           (23)
#define TEST_MqDelete                           (24)
#define TEST_MqSendUntilTimeStamp               (25)
#define TEST_MqSendForTimePeriod                (26)
#define TEST_MqReceiveUntilTimeStamp            (27)
#define TEST_MqReceiveForTimePeriod             (28)
#define TEST_MqIsFull                           (29)
#define TEST_MqIsEmpty                          (30)
#define TEST_MqGetConfig                        (31)
#define TEST_MqReset                            (32)
#define TEST_MqInitializeMqConfigSt             (33)
#define TEST_ClockTimeGetTimeStamp              (34)
#define TEST_ClockTimeCalculateTimeDifference   (35)
#define TEST_InterruptGetNumOfIrqChannels       (36)
#define TEST_InterruptRegisterIsr               (37)
#define TEST_InterruptUnregisterIsr             (38)
#define TEST_InterruptEnableIsr                 (39)
#define TEST_InterruptDisableIsr                (40)
#define TEST_InterruptIsISRContext              (41)
#define TEST_InterruptSetIsrPriority            (42)
#define TEST_IoGetNumOfDevices                  (43)
#define TEST_IoGetDeviceList                    (44)
#define TEST_IoDeviceOpen                       (45)
#define TEST_IoDeviceClose                      (46)
#define TEST_IoRead8                            (47)
#define TEST_IoRead16                           (48)
#define TEST_IoRead32                           (49)
#define TEST_IoRead64                           (50)
#define TEST_IoWrite8                           (51)
#define TEST_IoWrite16                          (52)
#define TEST_IoWrite32                          (53)
#define TEST_IoWrite64                          (54)
#define TEST_IoBlockRead8                       (55)
#define TEST_IoBlockRead16                      (56)
#define TEST_IoBlockRead32                      (57)
#define TEST_IoBlockRead64                      (58)
#define TEST_IoBlockWrite8                      (59)
#define TEST_IoBlockWrite16                     (60)
#define TEST_IoBlockWrite32                     (61)
#define TEST_IoBlockWrite64                     (62)
#define TEST_IoGetAxiBusIdFromDeviceName        (63)
#define TEST_IoGetAxiBusName                    (64)
#define TEST_IoGetAxiBusNameList                (65)
#define TEST_IoGetDeviceAxiBusId                (66)
#define TEST_IoGetNumOfAxiIdsForDeviceHdl       (67)
#define TEST_IoGetNumOfAxiIdsForDeviceName      (68)
#define TEST_IoGetNumOfAxiIdsForAxiBusName      (69)
#define TEST_IoGetAxiIdListForDeviceHdl         (70)
#define TEST_IoGetAxiIdListForDeviceName        (71)
#define TEST_IoGetAxiIdListForAxiBusName        (72)
#define TEST_PmSetRequiredState                 (73)
#define TEST_PmGetRequiredState                 (74)
#define TEST_PmWaitForState                     (75)
#define TEST_PmGetState                         (76)
#define TEST_PmSetPolicy                        (77)
#define TEST_PmGetPolicy                        (78)
#define TEST_PmSetResetState                    (79)
#define TEST_PmGetResetState                    (80)
#define TEST_PmGetLowestIntermediatePowerState  (81)
#define TEST_PmSetPostClock                     (82)
#define TEST_MmngrOpen                          (83)
#define TEST_MmngrClose                         (84)
#define TEST_MmngrAlloc                         (85)
#define TEST_MmngrDealloc                       (86)
#define TEST_MmngrGetConfig                     (87)
#define TEST_MmngrGetOsalMaxConfig              (88)
#define TEST_MmngrInitializeMmngrConfigSt       (89)
#define TEST_MmngrRegisterMonitor               (90)
#define TEST_MmngrGetCpuPtr                     (91)
#define TEST_MmngrGetConstCpuPtr                (92)
#define TEST_MmngrFlush                         (93)
#define TEST_MmngrInvalidate                    (94)
#define TEST_MmngrGetHwAddr                     (95)
#define TEST_MmngrGetSize                       (96)
#define TEST_MmngrCheckBuffer                   (97)
#define TEST_MmngrDebugMmngrPrint               (98)
#define TEST_MmngrDebugBufferPrint              (99)
#define TEST_MmngrInitializeMemoryBufferObj     (100)
#define TEST_MmngrInitializeMemoryManagerObj    (101)
#define TEST_MmngrOpenFromRegionIdx             (102)
#define TEST_MmngrGetOsalMaxConfigFromRegionIdx (103)

#endif  /* OSAL_TEST_IT_MEMORT_H */
