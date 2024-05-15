/***********************************************************************************************************************
* File Name :    main.c
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Performance Test program for OSAL QNX IT
***********************************************************************************************************************/

/*** include ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

#if 1 /* IPL workaround */
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <hw/inout.h>
#endif
/*** definition ***/
#define TEST_NUM   100
#define TEST_COUNT 10

#define WAIT_TIME_MS 10         // 10 milisecond
#define WAIT_TIME_NS 10000000   // 10 milisecond
#define WAIT_TIME_SEC 1         // 1 sec
#define MAX_NS 1000000000

#define TIME_BEFORE     clocktime_before = ClockCycles();
#define TIME_AFTER      clocktime_after  = ClockCycles();
#define TIME_DIFF       (clocktime_after - clocktime_before)

#define PRINT(str) printf("=====START:" #str "\n")
#define INTERVAL_TIME (1 * 1000 * 1000)
#define USEC_TO_NSEC (1000)
#define SEC_TO_NSEC  (1000000000ULL)

#define OSAL_FUNCTION_CALL(call, osal_ret) \
{ \
        PRINT(call); \
    osal_ret = call; \
    if (OSAL_RETURN_OK != osal_ret) \
    { \
        printf("%s(%d) osal_ret=%d\n", __func__, __LINE__, osal_ret); \
    } \
    usleep(INTERVAL_TIME); \
    printf("======END=====\n"); \
} \

#define OSAL_TEST_INFO(...) {printf(__VA_ARGS__);}
#define OSAL_TEST_ERR(...)  {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/* thread IDs */
#define THREAD_MAIN_TASK_ID     0xf000U
#define THREAD_MAIN_TASK_ID2    0xf000U + 1U
/* message queue IDs */
#define MQ_ID_NO1               0xf000U
/* mutex IDs */
#define MUTEX_ID_NO1            0xf000U
/* condition variable IDs */
#define COND_ID_NO1             0xf000U

#define TEST_Initialize                           0
#define TEST_Deinitialize                         1
#define TEST_GetVersion                           2
#define TEST_ThreadCreate                         3
#define TEST_ThreadSleepUntilTimeStamp            4
#define TEST_ThreadSleepForTimePeriod             5
#define TEST_ThreadSelf                           6
#define TEST_ThreadEqual                          7
#define TEST_ThreadInitializeThreadConfigSt       8
#define TEST_ThreadJoin                           9
#define TEST_ThsyncMutexCreate                   10
#define TEST_ThsyncMutexDestroy                  11
#define TEST_ThsyncMutexLockUntilTimeStamp       12
#define TEST_ThsyncMutexLockForTimePeriod        13
#define TEST_ThsyncMutexTryLock                  14
#define TEST_ThsyncMutexUnlock                   15
#define TEST_ThsyncCondCreate                    16
#define TEST_ThsyncCondDestroy                   17
#define TEST_ThsyncCondWaitUntilTimeStamp        18
#define TEST_ThsyncCondWaitForTimePeriod         19
#define TEST_ThsyncCondBroadcast                 20
#define TEST_ThsyncCondSignal                    21
#define TEST_ThsyncMemoryBarrier                 22
#define TEST_MqCreate                            23
#define TEST_MqDelete                            24
#define TEST_MqSendUntilTimeStamp                25
#define TEST_MqSendForTimePeriod                 26
#define TEST_MqReceiveUntilTimeStamp             27
#define TEST_MqReceiveForTimePeriod              28
#define TEST_MqIsFull                            29
#define TEST_MqIsEmpty                           30
#define TEST_MqGetConfig                         31
#define TEST_MqReset                             32
#define TEST_MqInitializeMqConfigSt              33
#define TEST_ClockTimeGetTimeStamp               34
#define TEST_ClockTimeCalculateTimeDifference    35
#define TEST_InterruptGetNumOfIrqChannels        36
#define TEST_InterruptRegisterIsr                37
#define TEST_InterruptUnregisterIsr              38
#define TEST_InterruptEnableIsr                  39
#define TEST_InterruptDisableIsr                 40
#define TEST_InterruptIsISRContext               41
#define TEST_InterruptSetIsrPriority             42
#define TEST_IoGetNumOfDevices                   43
#define TEST_IoGetDeviceList                     44
#define TEST_IoDeviceOpen                        45
#define TEST_IoDeviceClose                       46
#define TEST_IoRead8                             47
#define TEST_IoRead16                            48
#define TEST_IoRead32                            49
#define TEST_IoRead64                            50
#define TEST_IoWrite8                            51
#define TEST_IoWrite16                           52
#define TEST_IoWrite32                           53
#define TEST_IoWrite64                           54
#define TEST_IoBlockRead32                       55
#define TEST_IoBlockRead64                       56
#define TEST_IoBlockWrite32                      57
#define TEST_IoBlockWrite64                      58
#define TEST_IoGetAxiBusIdFromDeviceName         59
#define TEST_IoGetAxiBusName                     60
#define TEST_IoGetAxiBusNameList                 61
#define TEST_IoGetDeviceAxiBusId                 62
#define TEST_PmSetRequiredState                  63
#define TEST_PmGetRequiredState                  64
#define TEST_PmWaitForState                      65
#define TEST_PmGetState                          66
#define TEST_PmSetPolicy                         67
#define TEST_PmGetPolicy                         68
#define TEST_PmSetResetState                     69
#define TEST_PmGetResetState                     70
#define TEST_PmGetLowestIntermediatePowerState   71
#define TEST_PmSetPostClock                      72
#define TEST_MmngrOpen                           73
#define TEST_MmngrClose                          74
#define TEST_MmngrAlloc                          75
#define TEST_MmngrDealloc                        76
#define TEST_MmngrGetConfig                      77
#define TEST_MmngrGetOsalMaxConfig               78
#define TEST_MmngrInitializeMmngrConfigSt        79
#define TEST_MmngrRegisterMonitor                80
#define TEST_MmngrGetCpuPtr                      81
#define TEST_MmngrGetConstCpuPtr                 82
#define TEST_MmngrFlush                          83
#define TEST_MmngrInvalidate                     84
#define TEST_MmngrGetHwAddr                      85
#define TEST_MmngrGetSize                        86
#define TEST_MmngrCheckBuffer                    87
#define TEST_MmngrDebugMmngrPrint                88
#define TEST_MmngrDebugBufferPrint               89
#define TEST_MmngrInitializeMemoryBufferObj      90
#define TEST_MmngrInitializeMemoryManagerObj     91
#define TEST_IoGetNumOfAxiIdsForDeviceHdl        92
#define TEST_IoGetNumOfAxiIdsForDeviceName       93
#define TEST_IoGetNumOfAxiIdsForAxiBusName       94
#define TEST_IoGetAxiIdListForDeviceHdl          95
#define TEST_IoGetAxiIdListForDeviceName         96
#define TEST_IoGetAxiIdListForAxiBusName         97
#define TEST_MmngrOpenFromRegionIdx              98
#define TEST_MmngrGetOsalMaxConfigFromRegionIdx  99


/*** global variable ***/
static osal_thread_handle_t     th_handle;
static osal_thread_handle_t     th_handle2;
static int g_count = 0;


/*** prototype ***/
static int Test_ThreadManager (int test_count);
static int Test_ThreadSyncManager (int test_count);
static int Test_MessageManager (int test_count);
static int Test_ClockTimeManager (int test_count);
static int Test_InterruptManager (int test_count);
static int Test_IOManager (int test_count);
static int Test_PowerManager (int test_count);
static int Test_MemoryManager (int test_count);
static int CheckVersion (int test_count);

/* Test_ThreadManager */
static int64_t osal_test_thread (void *user_arg);
/* Test_ThreadSyncManager */
static int Test_ThreadSyncManager_Mutex (int test_count);
static int Test_ThreadSyncManager_Cond (int test_count);
typedef struct st_arg_mq_send_timeperiod {
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
    void *p_buffer;
    size_t buffer_size;
} st_arg_mq_send_timeperiod_t;

typedef struct st_arg_mq_receive_timeperiod {
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
} st_arg_mq_receive_timeperiod_t;

/*** Common Utils ***/
/* GetAbsoluteTime */
static void GetAbsoluteTime(st_osal_time_t* iop_absolute_time);

typedef struct st_arg_cond {
    osal_milli_sec_t time;
    st_osal_time_t abs_time;
    osal_cond_handle_t cond_handle;
    osal_mutex_handle_t mutex_handle;
} st_arg_cond_t;

/* function */
/***********************************************************************************************************************
* Test function to check API sequences.
* Note that this test should work with ltrace.
* The first argument of this test is the number of test case.
*                       0 : Test All
*                       1 : Test Thread Manager
*                       2 : Test ThreadSync Manager
*                       3 : Test Message Manager
*                       4 : Test Clock & Time Manager
*                       5 : Test Interrupt Manager
*                       6 : Test IO Manager
*                       7 : Test Power Manager
*                       8 : Test Memory Manager
*                       9 : Test Version Manager
***********************************************************************************************************************/
uint64_t elapsed_time[TEST_NUM][TEST_COUNT];
uint64_t elapsed_time_avg[TEST_NUM];
uint64_t elapsed_time_max[TEST_NUM];
uint64_t elapsed_time_min[TEST_NUM];
const char api_list[TEST_NUM][120] = {
    "R_OSAL_Initialize   : \0",
    "R_OSAL_Deinitialize   : \0",
    "R_OSAL_GetVersion   : \0",
    "R_OSAL_ThreadCreate   : \0",
    "R_OSAL_ThreadSleepUntilTimeStamp   : \0",
    "R_OSAL_ThreadSleepForTimePeriod   : \0",
    "R_OSAL_ThreadSelf   : \0",
    "R_OSAL_ThreadEqual   : \0",
    "R_OSAL_ThreadInitializeThreadConfigSt   : \0",
    "R_OSAL_ThreadJoin   : \0",
    "R_OSAL_ThsyncMutexCreate   : \0",
    "R_OSAL_ThsyncMutexDestroy   : \0",
    "R_OSAL_ThsyncMutexLockUntilTimeStamp   : \0",
    "R_OSAL_ThsyncMutexLockForTimePeriod   : \0",
    "R_OSAL_ThsyncMutexTryLock   : \0",
    "R_OSAL_ThsyncMutexUnlock   : \0",
    "R_OSAL_ThsyncCondCreate   : \0",
    "R_OSAL_ThsyncCondDestroy   : \0",
    "R_OSAL_ThsyncCondWaitUntilTimeStamp   : \0",
    "R_OSAL_ThsyncCondWaitForTimePeriod   : \0",
    "R_OSAL_ThsyncCondBroadcast   : \0",
    "R_OSAL_ThsyncCondSignal   : \0",
    "R_OSAL_ThsyncMemoryBarrier   : \0",
    "R_OSAL_MqCreate   : \0",
    "R_OSAL_MqDelete   : \0",
    "R_OSAL_MqSendUntilTimeStamp   : \0",
    "R_OSAL_MqSendForTimePeriod   : \0",
    "R_OSAL_MqReceiveUntilTimeStamp   : \0",
    "R_OSAL_MqReceiveForTimePeriod   : \0",
    "R_OSAL_MqIsFull   : \0",
    "R_OSAL_MqIsEmpty   : \0",
    "R_OSAL_MqGetConfig   : \0",
    "R_OSAL_MqReset   : \0",
    "R_OSAL_MqInitializeMqConfigSt   : \0",
    "R_OSAL_ClockTimeGetTimeStamp   : \0",
    "R_OSAL_ClockTimeCalculateTimeDifference   : \0",
    "R_OSAL_InterruptGetNumOfIrqChannels   : \0",
    "R_OSAL_InterruptRegisterIsr   : \0",
    "R_OSAL_InterruptUnregisterIsr   : \0",
    "R_OSAL_InterruptEnableIsr   : \0",
    "R_OSAL_InterruptDisableIsr   : \0",
    "R_OSAL_InterruptIsISRContext   : \0",
    "R_OSAL_InterruptSetIsrPriority   : \0",
    "R_OSAL_IoGetNumOfDevices   : \0",
    "R_OSAL_IoGetDeviceList   : \0",
    "R_OSAL_IoDeviceOpen   : \0",
    "R_OSAL_IoDeviceClose   : \0",
    "R_OSAL_IoRead8   : \0",
    "R_OSAL_IoRead16   : \0",
    "R_OSAL_IoRead32   : \0",
    "R_OSAL_IoRead64   : \0",
    "R_OSAL_IoWrite8   : \0",
    "R_OSAL_IoWrite16   : \0",
    "R_OSAL_IoWrite32   : \0",
    "R_OSAL_IoWrite64   : \0",
    "R_OSAL_IoBlockRead32   : \0",
    "R_OSAL_IoBlockRead64   : \0",
    "R_OSAL_IoBlockWrite32   : \0",
    "R_OSAL_IoBlockWrite64   : \0",
    "R_OSAL_IoGetAxiBusIdFromDeviceName   : \0",
    "R_OSAL_IoGetAxiBusName   : \0",
    "R_OSAL_IoGetAxiBusNameList   : \0",
    "R_OSAL_IoGetDeviceAxiBusId   : \0",
    "R_OSAL_PmSetRequiredState   : \0",
    "R_OSAL_PmGetRequiredState   : \0",
    "R_OSAL_PmWaitForState   : \0",
    "R_OSAL_PmGetState   : \0",
    "R_OSAL_PmSetPolicy   : \0",
    "R_OSAL_PmGetPolicy   : \0",
    "R_OSAL_PmSetResetState   : \0",
    "R_OSAL_PmGetResetState   : \0",
    "R_OSAL_PmGetLowestIntermediatePowerState   : \0",
    "R_OSAL_PmSetPostClock   : \0",
    "R_OSAL_MmngrOpen   : \0",
    "R_OSAL_MmngrClose   : \0",
    "R_OSAL_MmngrAlloc   : \0",
    "R_OSAL_MmngrDealloc   : \0",
    "R_OSAL_MmngrGetConfig   : \0",
    "R_OSAL_MmngrGetOsalMaxConfig   : \0",
    "R_OSAL_MmngrInitializeMmngrConfigSt   : \0",
    "R_OSAL_MmngrRegisterMonitor   : \0",
    "R_OSAL_MmngrGetCpuPtr   : \0",
    "R_OSAL_MmngrGetConstCpuPtr   : \0",
    "R_OSAL_MmngrFlush   : \0",
    "R_OSAL_MmngrInvalidate   : \0",
    "R_OSAL_MmngrGetHwAddr   : \0",
    "R_OSAL_MmngrGetSize   : \0",
    "R_OSAL_MmngrCheckBuffer   : \0",
    "R_OSAL_MmngrDebugMmngrPrint   : \0",
    "R_OSAL_MmngrDebugBufferPrint   : \0",
    "R_OSAL_MmngrInitializeMemoryBufferObj   : \0",
    "R_OSAL_MmngrInitializeMemoryManagerObj   : \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceHdl   : \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceName   : \0",
    "R_OSAL_IoGetNumOfAxiIdsForAxiBusName   : \0",
    "R_OSAL_IoGetAxiIdListForDeviceHdl   : \0",
    "R_OSAL_IoGetAxiIdListForDeviceName   : \0",
    "R_OSAL_IoGetAxiIdListForAxiBusName   : \0",
    "R_OSAL_MmngrOpenFromRegionIdx   : \0",
    "R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx   : \0",
};

#if defined(RCAR_S4)
uint64_t expected_time[TEST_NUM] = {
    21233 * USEC_TO_NSEC,      /* R_OSAL_Initialize */
    22136 * USEC_TO_NSEC,      /* R_OSAL_Deinitialize */
    8 * USEC_TO_NSEC,          /* R_OSAL_GetVersion */
    52 * USEC_TO_NSEC,         /* R_OSAL_ThreadCreate */
    11000 * USEC_TO_NSEC,      /* R_OSAL_ThreadSleepUntilTimeStamp */
    11000 * USEC_TO_NSEC,      /* R_OSAL_ThreadSleepForTimePeriod */
    11 * USEC_TO_NSEC,         /* R_OSAL_ThreadSelf */
    8 * USEC_TO_NSEC,          /* R_OSAL_ThreadEqual */
    8 * USEC_TO_NSEC,          /* R_OSAL_ThreadInitializeThreadConfigSt */
    13 * USEC_TO_NSEC,         /* R_OSAL_ThreadJoin */
    12 * USEC_TO_NSEC,         /* R_OSAL_ThsyncMutexCreate */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexDestroy */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockUntilTimeStamp */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockForTimePeriod */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexTryLock */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexUnlock */
    11 * USEC_TO_NSEC,         /* R_OSAL_ThsyncCondCreate */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondDestroy */
    5962 * USEC_TO_NSEC,       /* R_OSAL_ThsyncCondWaitUntilTimeStamp */
    5762 * USEC_TO_NSEC,       /* R_OSAL_ThsyncCondWaitForTimePeriod */
    18 * USEC_TO_NSEC,         /* R_OSAL_ThsyncCondBroadcast */
    18 * USEC_TO_NSEC,         /* R_OSAL_ThsyncCondSignal */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMemoryBarrier */
    12 * USEC_TO_NSEC,         /* R_OSAL_MqCreate */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqDelete */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqSendUntilTimeStamp */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqSendForTimePeriod */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqReceiveUntilTimeStamp */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqReceiveForTimePeriod */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqIsFull */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqIsEmpty */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqGetConfig */
    10 * USEC_TO_NSEC,         /* R_OSAL_MqReset */
    8 * USEC_TO_NSEC,          /* R_OSAL_MqInitializeMqConfigSt */
    9 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeGetTimeStamp */
    8 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeCalculateTimeDifference */
    8 * USEC_TO_NSEC,          /* R_OSAL_InterruptGetNumOfIrqChannels */
    25 * USEC_TO_NSEC,         /* R_OSAL_InterruptRegisterIsr */
    9 * USEC_TO_NSEC,          /* R_OSAL_InterruptUnregisterIsr */
    15 * USEC_TO_NSEC,         /* R_OSAL_InterruptEnableIsr */
    12 * USEC_TO_NSEC,         /* R_OSAL_InterruptDisableIsr */
    8 * USEC_TO_NSEC,          /* R_OSAL_InterruptIsISRContext */
    15 * USEC_TO_NSEC,         /* R_OSAL_InterruptSetIsrPriority */
    1109 * USEC_TO_NSEC,       /* R_OSAL_IoGetNumOfDevices */
    1107 * USEC_TO_NSEC,       /* R_OSAL_IoGetDeviceList */
    12 * USEC_TO_NSEC,         /* R_OSAL_IoDeviceOpen */
    10 * USEC_TO_NSEC,         /* R_OSAL_IoDeviceClose */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoRead8 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoRead16 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoRead32 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoRead64 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoWrite8 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoWrite16 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoWrite32 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoWrite64 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoBlockRead32 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoBlockRead64 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoBlockWrite32 */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoBlockWrite64 */
    558 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    9 * USEC_TO_NSEC,          /* R_OSAL_IoGetAxiBusName */
    568 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiBusNameList */
    557 * USEC_TO_NSEC,        /* R_OSAL_IoGetDeviceAxiBusId */
    92 * USEC_TO_NSEC,         /* R_OSAL_PmSetRequiredState */
    24 * USEC_TO_NSEC,         /* R_OSAL_PmGetRequiredState */
    24 * USEC_TO_NSEC,         /* R_OSAL_PmWaitForState */
    24 * USEC_TO_NSEC,         /* R_OSAL_PmGetState */
    1318 * USEC_TO_NSEC,       /* R_OSAL_PmSetPolicy */
    10 * USEC_TO_NSEC,         /* R_OSAL_PmGetPolicy */
    19 * USEC_TO_NSEC,         /* R_OSAL_PmSetResetState */
    10 * USEC_TO_NSEC,         /* R_OSAL_PmGetResetState */
    10 * USEC_TO_NSEC,         /* R_OSAL_PmGetLowestIntermediatePowerState */
    40 * USEC_TO_NSEC,         /* R_OSAL_PmSetPostClock */
    149068 * USEC_TO_NSEC,     /* R_OSAL_MmngrOpen */
    76629 * USEC_TO_NSEC,      /* R_OSAL_MmngrClose */
    72 * USEC_TO_NSEC,         /* R_OSAL_MmngrAlloc */
    10 * USEC_TO_NSEC,         /* R_OSAL_MmngrDealloc */
    9 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConfig */
    8 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfig */
    8 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMmngrConfigSt */
    9 * USEC_TO_NSEC,          /* R_OSAL_MmngrRegisterMonitor */
    9 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetCpuPtr */
    9 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConstCpuPtr */
    12 * USEC_TO_NSEC,         /* R_OSAL_MmngrFlush */
    13 * USEC_TO_NSEC,         /* R_OSAL_MmngrInvalidate */
    9 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetHwAddr */
    9 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetSize */
    51 * USEC_TO_NSEC,         /* R_OSAL_MmngrCheckBuffer */
    183 * USEC_TO_NSEC,        /* R_OSAL_MmngrDebugMmngrPrint */
    50 * USEC_TO_NSEC,         /* R_OSAL_MmngrDebugBufferPrint */
    8 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryBufferObj */
    8 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryManagerObj */
    8 * USEC_TO_NSEC,          /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
    566 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    577 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    570 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForDeviceHdl */
    566 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForDeviceName */
    578 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForAxiBusName */
    147840 * USEC_TO_NSEC,     /* R_OSAL_MmngrOpenFromRegionIdx */
    8 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
};
#elif defined(RCAR_V4H)
uint64_t expected_time[TEST_NUM] = {
    20711 * USEC_TO_NSEC,      /* R_OSAL_Initialize */
    25663 * USEC_TO_NSEC,      /* R_OSAL_Deinitialize */
    6 * USEC_TO_NSEC,          /* R_OSAL_GetVersion */
    22 * USEC_TO_NSEC,         /* R_OSAL_ThreadCreate */
    11000 * USEC_TO_NSEC,      /* R_OSAL_ThreadSleepUntilTimeStamp */
    11000 * USEC_TO_NSEC,      /* R_OSAL_ThreadSleepForTimePeriod */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThreadSelf */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThreadEqual */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThreadInitializeThreadConfigSt */
    8 * USEC_TO_NSEC,          /* R_OSAL_ThreadJoin */
    8 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexCreate */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexDestroy */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockUntilTimeStamp */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockForTimePeriod */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexTryLock */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexUnlock */
    7 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondCreate */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondDestroy */
    5957 * USEC_TO_NSEC,       /* R_OSAL_ThsyncCondWaitUntilTimeStamp */
    5739 * USEC_TO_NSEC,       /* R_OSAL_ThsyncCondWaitForTimePeriod */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondBroadcast */
    9 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondSignal */
    6 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMemoryBarrier */
    9 * USEC_TO_NSEC,          /* R_OSAL_MqCreate */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqDelete */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqSendUntilTimeStamp */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqSendForTimePeriod */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqReceiveUntilTimeStamp */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqReceiveForTimePeriod */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqIsFull */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqIsEmpty */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqGetConfig */
    7 * USEC_TO_NSEC,          /* R_OSAL_MqReset */
    6 * USEC_TO_NSEC,          /* R_OSAL_MqInitializeMqConfigSt */
    6 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeGetTimeStamp */
    6 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeCalculateTimeDifference */
    6 * USEC_TO_NSEC,          /* R_OSAL_InterruptGetNumOfIrqChannels */
    14 * USEC_TO_NSEC,         /* R_OSAL_InterruptRegisterIsr */
    6 * USEC_TO_NSEC,          /* R_OSAL_InterruptUnregisterIsr */
    8 * USEC_TO_NSEC,          /* R_OSAL_InterruptEnableIsr */
    7 * USEC_TO_NSEC,          /* R_OSAL_InterruptDisableIsr */
    6 * USEC_TO_NSEC,          /* R_OSAL_InterruptIsISRContext */
    8 * USEC_TO_NSEC,          /* R_OSAL_InterruptSetIsrPriority */
    1664 * USEC_TO_NSEC,       /* R_OSAL_IoGetNumOfDevices */
    1663 * USEC_TO_NSEC,       /* R_OSAL_IoGetDeviceList */
    7 * USEC_TO_NSEC,          /* R_OSAL_IoDeviceOpen */
    7 * USEC_TO_NSEC,          /* R_OSAL_IoDeviceClose */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoRead8 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoRead16 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoRead32 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoRead64 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoWrite8 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoWrite16 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoWrite32 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoWrite64 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoBlockRead32 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoBlockRead64 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoBlockWrite32 */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoBlockWrite64 */
    745 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoGetAxiBusName */
    756 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiBusNameList */
    763 * USEC_TO_NSEC,        /* R_OSAL_IoGetDeviceAxiBusId */
    25 * USEC_TO_NSEC,         /* R_OSAL_PmSetRequiredState */
    6 * USEC_TO_NSEC,          /* R_OSAL_PmGetRequiredState */
    8 * USEC_TO_NSEC,          /* R_OSAL_PmWaitForState */
    11 * USEC_TO_NSEC,         /* R_OSAL_PmGetState */
    1611 * USEC_TO_NSEC,       /* R_OSAL_PmSetPolicy */
    6 * USEC_TO_NSEC,          /* R_OSAL_PmGetPolicy */
    12 * USEC_TO_NSEC,         /* R_OSAL_PmSetResetState */
    6 * USEC_TO_NSEC,          /* R_OSAL_PmGetResetState */
    6 * USEC_TO_NSEC,          /* R_OSAL_PmGetLowestIntermediatePowerState */
    19 * USEC_TO_NSEC,         /* R_OSAL_PmSetPostClock */
    5897 * USEC_TO_NSEC,       /* R_OSAL_MmngrOpen */
    47538 * USEC_TO_NSEC,      /* R_OSAL_MmngrClose */
    33 * USEC_TO_NSEC,         /* R_OSAL_MmngrAlloc */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrDealloc */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConfig */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfig */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMmngrConfigSt */
    7 * USEC_TO_NSEC,          /* R_OSAL_MmngrRegisterMonitor */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetCpuPtr */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConstCpuPtr */
    7 * USEC_TO_NSEC,          /* R_OSAL_MmngrFlush */
    8 * USEC_TO_NSEC,          /* R_OSAL_MmngrInvalidate */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetHwAddr */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetSize */
    18 * USEC_TO_NSEC,         /* R_OSAL_MmngrCheckBuffer */
    58 * USEC_TO_NSEC,         /* R_OSAL_MmngrDebugMmngrPrint */
    17 * USEC_TO_NSEC,         /* R_OSAL_MmngrDebugBufferPrint */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryBufferObj */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryManagerObj */
    6 * USEC_TO_NSEC,          /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
    740 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    746 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    750 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForDeviceHdl */
    747 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForDeviceName */
    749 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForAxiBusName */
    5734 * USEC_TO_NSEC,       /* R_OSAL_MmngrOpenFromRegionIdx */
    6 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
};
#elif defined(RCAR_V4M)
uint64_t expected_time[TEST_NUM] = {
    30554 * USEC_TO_NSEC,       /* R_OSAL_Initialize */
    37215 * USEC_TO_NSEC,       /* R_OSAL_Deinitialize */
    10 * USEC_TO_NSEC,          /* R_OSAL_GetVersion */
    38 * USEC_TO_NSEC,          /* R_OSAL_ThreadCreate */
    18617 * USEC_TO_NSEC,       /* R_OSAL_ThreadSleepUntilTimeStamp */
    18536 * USEC_TO_NSEC,       /* R_OSAL_ThreadSleepForTimePeriod */
    15 * USEC_TO_NSEC,          /* R_OSAL_ThreadSelf */
    10 * USEC_TO_NSEC,          /* R_OSAL_ThreadEqual */
    10 * USEC_TO_NSEC,          /* R_OSAL_ThreadInitializeThreadConfigSt */
    15 * USEC_TO_NSEC,          /* R_OSAL_ThreadJoin */
    14 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexCreate */
    11 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexDestroy */
    11 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockUntilTimeStamp */
    11 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockForTimePeriod */
    11 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexTryLock */
    11 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexUnlock */
    13 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondCreate */
    11 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondDestroy */
    10127 * USEC_TO_NSEC,       /* R_OSAL_ThsyncCondWaitUntilTimeStamp */
    9757 * USEC_TO_NSEC,        /* R_OSAL_ThsyncCondWaitForTimePeriod */
    15 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondBroadcast */
    15 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondSignal */
    11 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMemoryBarrier */
    16 * USEC_TO_NSEC,          /* R_OSAL_MqCreate */
    11 * USEC_TO_NSEC,          /* R_OSAL_MqDelete */
    12 * USEC_TO_NSEC,          /* R_OSAL_MqSendUntilTimeStamp */
    11 * USEC_TO_NSEC,          /* R_OSAL_MqSendForTimePeriod */
    11 * USEC_TO_NSEC,          /* R_OSAL_MqReceiveUntilTimeStamp */
    11 * USEC_TO_NSEC,          /* R_OSAL_MqReceiveForTimePeriod */
    11 * USEC_TO_NSEC,          /* R_OSAL_MqIsFull */
    11 * USEC_TO_NSEC,          /* R_OSAL_MqIsEmpty */
    11 * USEC_TO_NSEC,          /* R_OSAL_MqGetConfig */
    12 * USEC_TO_NSEC,          /* R_OSAL_MqReset */
    10 * USEC_TO_NSEC,          /* R_OSAL_MqInitializeMqConfigSt */
    11 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeGetTimeStamp */
    10 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeCalculateTimeDifference */
    10 * USEC_TO_NSEC,          /* R_OSAL_InterruptGetNumOfIrqChannels */
    24 * USEC_TO_NSEC,          /* R_OSAL_InterruptRegisterIsr */
    11 * USEC_TO_NSEC,          /* R_OSAL_InterruptUnregisterIsr */
    14 * USEC_TO_NSEC,          /* R_OSAL_InterruptEnableIsr */
    13 * USEC_TO_NSEC,          /* R_OSAL_InterruptDisableIsr */
    11 * USEC_TO_NSEC,          /* R_OSAL_InterruptIsISRContext */
    13 * USEC_TO_NSEC,          /* R_OSAL_InterruptSetIsrPriority */
    2141 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfDevices */
    2139 * USEC_TO_NSEC,        /* R_OSAL_IoGetDeviceList */
    13 * USEC_TO_NSEC,          /* R_OSAL_IoDeviceOpen */
    12 * USEC_TO_NSEC,          /* R_OSAL_IoDeviceClose */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoRead8 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoRead16 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoRead32 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoRead64 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoWrite8 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoWrite16 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoWrite32 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoWrite64 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoBlockRead32 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoBlockRead64 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoBlockWrite32 */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoBlockWrite64 */
    1146 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    11 * USEC_TO_NSEC,          /* R_OSAL_IoGetAxiBusName */
    1164 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiBusNameList */
    1172 * USEC_TO_NSEC,        /* R_OSAL_IoGetDeviceAxiBusId */
    64 * USEC_TO_NSEC,          /* R_OSAL_PmSetRequiredState */
    17 * USEC_TO_NSEC,          /* R_OSAL_PmGetRequiredState */
    20 * USEC_TO_NSEC,          /* R_OSAL_PmWaitForState */
    30 * USEC_TO_NSEC,          /* R_OSAL_PmGetState */
    2740 * USEC_TO_NSEC,        /* R_OSAL_PmSetPolicy */
    11 * USEC_TO_NSEC,          /* R_OSAL_PmGetPolicy */
    21 * USEC_TO_NSEC,          /* R_OSAL_PmSetResetState */
    11 * USEC_TO_NSEC,          /* R_OSAL_PmGetResetState */
    11 * USEC_TO_NSEC,          /* R_OSAL_PmGetLowestIntermediatePowerState */
    33 * USEC_TO_NSEC,          /* R_OSAL_PmSetPostClock */
    10026 * USEC_TO_NSEC,       /* R_OSAL_MmngrOpen */
    80816 * USEC_TO_NSEC,       /* R_OSAL_MmngrClose */
    57 * USEC_TO_NSEC,          /* R_OSAL_MmngrAlloc */
    11 * USEC_TO_NSEC,          /* R_OSAL_MmngrDealloc */
    11 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConfig */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfig */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMmngrConfigSt */
    12 * USEC_TO_NSEC,          /* R_OSAL_MmngrRegisterMonitor */
    11 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetCpuPtr */
    11 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConstCpuPtr */
    12 * USEC_TO_NSEC,          /* R_OSAL_MmngrFlush */
    15 * USEC_TO_NSEC,          /* R_OSAL_MmngrInvalidate */
    11 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetHwAddr */
    11 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetSize */
    31 * USEC_TO_NSEC,          /* R_OSAL_MmngrCheckBuffer */
    99 * USEC_TO_NSEC,          /* R_OSAL_MmngrDebugMmngrPrint */
    29 * USEC_TO_NSEC,          /* R_OSAL_MmngrDebugBufferPrint */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryBufferObj */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryManagerObj */
    10 * USEC_TO_NSEC,          /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
    1138 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    1149 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    1153 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForDeviceHdl */
    1148 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForDeviceName */
    1154 * USEC_TO_NSEC,        /* R_OSAL_IoGetAxiIdListForAxiBusName */
    9749 * USEC_TO_NSEC,        /* R_OSAL_MmngrOpenFromRegionIdx */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
};
#else   /* V3M, V3Hx */
uint64_t expected_time[TEST_NUM] = {
    500000 * USEC_TO_NSEC,      /* R_OSAL_Initialize*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_Deinitialize*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_GetVersion*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThreadCreate*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThreadSleepUntilTimeStamp*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThreadSleepForTimePeriod*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThreadSelf*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThreadEqual*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThreadInitializeThreadConfigSt*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThreadJoin*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncMutexCreate*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncMutexDestroy*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncMutexLockUntilTimeStamp*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncMutexLockForTimePeriod*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncMutexTryLock*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncMutexUnlock*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncCondCreate*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncCondDestroy*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncCondWaitUntilTimeStamp*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncCondWaitForTimePeriod*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncCondBroadcast*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncCondSignal*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ThsyncMemoryBarrier*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqCreate*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqDelete*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqSendUntilTimeStamp*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqSendForTimePeriod*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqReceiveUntilTimeStamp*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqReceiveForTimePeriod*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqIsFull*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqIsEmpty*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqGetConfig*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqReset*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MqInitializeMqConfigSt*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ClockTimeGetTimeStamp*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_ClockTimeCalculateTimeDifference*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_InterruptGetNumOfIrqChannels*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_InterruptRegisterIsr*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_InterruptUnregisterIsr*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_InterruptEnableIsr*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_InterruptDisableIsr*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_InterruptIsISRContext*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_InterruptSetIsrPriority*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetNumOfDevices*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetDeviceList*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoDeviceOpen*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoDeviceClose*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoRead8*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoRead16*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoRead32*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoRead64*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoWrite8*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoWrite16*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoWrite32*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoWrite64*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoBlockRead32*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoBlockRead64*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoBlockWrite32*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoBlockWrite64*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetAxiBusIdFromDeviceName*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetAxiBusName*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetAxiBusNameList*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetDeviceAxiBusId*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmSetRequiredState*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmGetRequiredState*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmWaitForState*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmGetState*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmSetPolicy*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmGetPolicy*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmSetResetState*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmGetResetState*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmGetLowestIntermediatePowerState*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_PmSetPostClock*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrOpen*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrClose*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrAlloc*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrDealloc*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrGetConfig*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrGetOsalMaxConfig*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrInitializeMngrConfigSt*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrRegisterMonitor*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrGetCpuPtr*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrGetConstCpuPtr*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrFlush*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrInvalidate*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrGetHwAddr*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrGetSize*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrCheckBuffer*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrDebugMmngrPrint*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrDebugBufferPrint*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrInitializeMemoryBufferObj*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrInitializeMemoryManagerObj*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetNumOfAxiIdsForDeviceName*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetAxiIdListForDeviceHdl*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetAxiIdListForDeviceName*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_IoGetAxiIdListForAxiBusName*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrOpenFromRegionIdx*/
    500000 * USEC_TO_NSEC,      /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx*/
};
#endif

int main()
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  current_time;
    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_nano_sec_t exec_time;
    uint64_t diff_time;
    int             ret;
    bool            initialized_state = 0;

    uint64_t clocktime_before;
    uint64_t clocktime_after;

#define TIME_BEFORE     clocktime_before = ClockCycles();
#define TIME_AFTER      clocktime_after  = ClockCycles();
#define TIME_DIFF       (clocktime_after - clocktime_before)

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

#if 1 /* IPL workaround */
    // Required for mmap
    // #include <hw/inout.h>
    // Required for register address
    // #include "r-car-gen3.h"
    uintptr_t cpg_vbase = (uintptr_t)MAP_FAILED;
    uint32_t  reg_mstpcr8 = 0;

    cpg_vbase = (uintptr_t)mmap_device_io(0x1000, 0xE6150000);
    if (cpg_vbase  == (uintptr_t)MAP_FAILED) {
        printf("CPG - %s : Unable to map CPG registers 0x%x\n", __func__, 0xE6150000);
        return 0;
    }

    /* read register */
    reg_mstpcr8 = in32(cpg_vbase + 0x980);
    if((reg_mstpcr8 & 0x7ff00000 ) == 0x7ff00000)
    {
        printf("RMSTPCR initial value OK!!\n");
    }
    else
    {
        printf("RMSTPCR initial value NG!!\n");

        printf("Set initial RMSTPCR8 value\n");
        out32(cpg_vbase + 0x980, reg_mstpcr8 | 0x7ff00000);
    }
    munmap_device_io(cpg_vbase, 0x1000);
#endif

    initialized_state = 1;

    for (g_count = 0; g_count < TEST_COUNT; g_count++)
    {
        OSAL_TEST_INFO("*** Start: R_OSAL_Initialize ***\n");
        TIME_BEFORE
        osal_ret = R_OSAL_Initialize();
        TIME_AFTER
        diff_time = (TIME_DIFF*SEC_TO_NSEC)/(uint64_t)SYSPAGE_ENTRY(qtime)->cycles_per_sec;
        OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_Initialize, g_count, diff_time);
        elapsed_time[TEST_Initialize][g_count] = diff_time;

        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_Initialize is failed, osal_ret = %d\n", osal_ret);
            return -1;
        }
        initialized_state = 1;
        OSAL_TEST_INFO("*** End: R_OSAL_Initialize ***\n");

        /* Check Execution time of GetVersion */
        ret = CheckVersion(g_count);

        if (ret == 0)
        {
            /* Check Execution time of Clock & Time Manager */
            ret = Test_ClockTimeManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        if (ret == 0)
        {
            /* Check Execution time of Thread Manager */
            ret = Test_ThreadManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        if (ret == 0)
        {
            /* Check Execution time of ThreadSync Manager */
            ret = Test_ThreadSyncManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        if (ret == 0)
        {
            /* Check Execution time of Message Manager */
            ret = Test_MessageManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        if (ret == 0)
        {
            /* Check Execution time of IO Manager */
            ret = Test_IOManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        if (ret == 0)
        {
            /* Check Execution time of Interrupt Manager */
            ret = Test_InterruptManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        if (ret == 0)
        {
            /* Check Execution time of Power Manager */
            ret = Test_PowerManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        if (ret == 0)
        {
            /* Check Execution time of Memory Manager */
            ret = Test_MemoryManager(g_count);
        }
        else
        {
            if (initialized_state == 1)
            {
                initialized_state = 0;
                R_OSAL_Deinitialize();
            }
            return -1;
        }

        OSAL_TEST_INFO("*** Start: R_OSAL_Deinitialize ***\n");
        TIME_BEFORE
        osal_ret = R_OSAL_Deinitialize();
        TIME_AFTER
        diff_time = (TIME_DIFF*SEC_TO_NSEC)/(uint64_t)SYSPAGE_ENTRY(qtime)->cycles_per_sec;
        OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_Deinitialize, g_count, diff_time);
        elapsed_time[TEST_Deinitialize][g_count] = diff_time;

        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize is failed, osal_ret = %d\n", osal_ret);
            return -1;
        }
        initialized_state = 0;
        OSAL_TEST_INFO("*** End: R_OSAL_Deinitialize ***\n");
    }

    unsigned int test_pass = 0;
    unsigned int test_fail = 0;
    unsigned int test_skip = 0;

    uint64_t sum;
    uint64_t max;
    uint64_t min;
    OSAL_TEST_INFO("OSAL API execution times:\n");
    for (int i = 0; i < TEST_NUM; i++)
    {
        sum = 0u;
        max = 0u;
        min = ~0u;
        for (int j = 0; j < TEST_COUNT; j++)
        {
            sum += elapsed_time[i][j];
            max = MAX(max, elapsed_time[i][j]);
            min = MIN(min, elapsed_time[i][j]);
        }
        elapsed_time_avg[i] = sum / TEST_COUNT;
        elapsed_time_max[i] = max;
        elapsed_time_min[i] = min;
        OSAL_TEST_INFO("%s Expect: %lu, max: %lu ns, avg: %lu ns, min: %lu ns\n",
                            api_list[i], expected_time[i], elapsed_time_max[i], elapsed_time_avg[i], elapsed_time_min[i]);

        if (expected_time[i] < elapsed_time_min[i])
        {
            OSAL_TEST_INFO("%s has exceeded the expected execution time. \n", api_list[i]);
            test_fail++;
        }
        else
        {
            test_pass++;
        }
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return 0;
}

/***********************************************************************************************************************
* Start of function Test_ThreadManager()
***********************************************************************************************************************/
static int Test_ThreadManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    st_osal_thread_config_t config;

    int64_t                 arg;
    int64_t                 return_value;
    bool                    result;
    st_osal_time_t          absolute_time;
    st_osal_time_t          current_time_1st, current_time_2nd;
    osal_nano_sec_t         exec_time;
    osal_nano_sec_t         debug_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&config, 0x00, sizeof(st_osal_thread_config_t));
    memset(&th_handle, 0x00, sizeof(osal_thread_handle_t));
    
    arg = 1;

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadInitializeThreadConfigSt ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThreadInitializeThreadConfigSt, test_count, exec_time);
    elapsed_time[TEST_ThreadInitializeThreadConfigSt][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadInitializeThreadConfigSt is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThreadInitializeThreadConfigSt ***\n");

    config.func       = osal_test_thread;
    config.userarg    = (void *)&arg;
    config.priority = OSAL_THREAD_PRIORITY_LOWEST;
    config.task_name = (char *)"Execution time test thread";
    config.stack_size = 2 * 1024 * 1024;

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadCreate ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadCreate(&config, THREAD_MAIN_TASK_ID, &th_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThreadCreate, test_count, exec_time);
    elapsed_time[TEST_ThreadCreate][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThreadCreate ***\n");

    usleep(10 * 1000 * 1000);

    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
    absolute_time.tv_nsec += WAIT_TIME_NS;
    if(MAX_NS <= absolute_time.tv_nsec)
    {
        absolute_time.tv_sec += 1;
        absolute_time.tv_nsec %= MAX_NS;
    }

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadSleepUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThreadSleepUntilTimeStamp, test_count, exec_time);
    elapsed_time[TEST_ThreadSleepUntilTimeStamp][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSleepUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThreadSleepUntilTimeStamp ***\n");

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadSleepForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(WAIT_TIME_MS);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThreadSleepForTimePeriod, test_count, exec_time);
    elapsed_time[TEST_ThreadSleepForTimePeriod][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSleepForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThreadSleepForTimePeriod ***\n");

    th_handle2 = th_handle;
    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadEqual ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadEqual(th_handle, th_handle2, &result);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThreadEqual, test_count, exec_time);
    elapsed_time[TEST_ThreadEqual][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadEqual is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThreadEqual ***\n");

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadJoin ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadJoin(th_handle, &return_value);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThreadJoin, test_count, exec_time);
    elapsed_time[TEST_ThreadJoin][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadJoin is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThreadJoin ***\n");

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ThreadManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_test_thread()
***********************************************************************************************************************/
static int64_t osal_test_thread (void *user_arg)
{
    e_osal_return_t         osal_ret;

    osal_thread_handle_t    self_handle;
    st_osal_time_t          current_time_1st, current_time_2nd;
    osal_nano_sec_t         exec_time;
    st_osal_time_t          absolute_time;
    bool                    result;

    osal_thread_handle_t const* s_handle = (osal_thread_handle_t *)user_arg;

    usleep(2 * 1000 * 1000);
    OSAL_TEST_INFO("*** Start: sub thread.***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(10);

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadSelf ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadSelf(&self_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThreadSelf, g_count, exec_time);
    elapsed_time[TEST_ThreadSelf][g_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSelf is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThreadSelf ***\n");

    // osal_ret = R_OSAL_ThreadEqual(self_handle, *s_handle, &result);
    OSAL_TEST_INFO("*** End: sub thread.***\n");
    return 0;
}
/***********************************************************************************************************************
* End of function osal_test_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_ThreadSyncManager()
***********************************************************************************************************************/
static int Test_ThreadSyncManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int ret;
    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_nano_sec_t exec_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    ret = Test_ThreadSyncManager_Mutex(test_count);
    ret = Test_ThreadSyncManager_Cond(test_count);

    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMemoryBarrier ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMemoryBarrier();
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncMemoryBarrier, test_count, exec_time);
    elapsed_time[TEST_ThsyncMemoryBarrier][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMemoryBarrier is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncMemoryBarrier ***\n");

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ThreadSyncManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_ThreadSyncManager_Mutex()
***********************************************************************************************************************/
static int Test_ThreadSyncManager_Mutex (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    
    osal_mutex_id_t     mutex_id;
    osal_mutex_handle_t mutex_handle;
    
    // osal_milli_sec_t    time;
    st_osal_time_t      absolute_time;
    st_osal_time_t      current_time_1st, current_time_2nd;
    osal_nano_sec_t     exec_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&mutex_handle,  0x00, sizeof(osal_mutex_handle_t));
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    mutex_id   = MUTEX_ID_NO1;

    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexCreate ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncMutexCreate, test_count, exec_time);
    elapsed_time[TEST_ThsyncMutexCreate][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncMutexCreate ***\n");

    /* R_OSAL_ThsyncMutexLockForTimePeriod */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexLockForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, WAIT_TIME_MS);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncMutexLockForTimePeriod, test_count, exec_time);
    elapsed_time[TEST_ThsyncMutexLockForTimePeriod][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncMutexLockForTimePeriod ***\n");

    /* R_OSAL_ThsyncMutexUnlock */    /* requires R_OSAL_ThsyncMutexTryLock */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexUnlock ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncMutexUnlock, test_count, exec_time);
    elapsed_time[TEST_ThsyncMutexUnlock][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncMutexUnlock ***\n");

    /* R_OSAL_ThsyncMutexLockUntilTimeStamp */    /* requires R_OSAL_ThsyncMutexCreate */
    GetAbsoluteTime(&absolute_time);

    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexLockUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncMutexLockUntilTimeStamp, test_count, exec_time);
    elapsed_time[TEST_ThsyncMutexLockUntilTimeStamp][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncMutexLockUntilTimeStamp ***\n");

    R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexTryLock */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexTryLock ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncMutexTryLock, test_count, exec_time);
    elapsed_time[TEST_ThsyncMutexTryLock][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexTryLock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncMutexTryLock ***\n");

    /* R_OSAL_ThsyncMutexUnlock */    /* requires R_OSAL_ThsyncMutexTryLock */
    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexDestroy */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexDestroy ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncMutexDestroy, test_count, exec_time);
    elapsed_time[TEST_ThsyncMutexDestroy][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncMutexDestroy ***\n");

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ThreadSyncManager_Mutex()
***********************************************************************************************************************/

/* TEST Thread R_OSAL_ThsyncCondWaitUntilTimeStamp */
int64_t osal_test_cond_waitunittimestamp(void *user_arg, int test_count)
{
    e_osal_return_t     osal_ret;
    st_arg_cond_t*      arg_cond;
    st_osal_time_t      current_time_1st, current_time_2nd;
    osal_nano_sec_t     exec_time;

    arg_cond = (st_arg_cond_t *)user_arg;

    /* mutex lock */
    R_OSAL_ThsyncMutexTryLock(arg_cond->mutex_handle);

    /* cond wait */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncCondWaitUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(arg_cond->cond_handle, arg_cond->mutex_handle, &arg_cond->abs_time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncCondWaitUntilTimeStamp, g_count, exec_time);
    elapsed_time[TEST_ThsyncCondWaitUntilTimeStamp][g_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondWaitUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncCondWaitUntilTimeStamp ***\n");

    R_OSAL_ThsyncMutexUnlock(arg_cond->mutex_handle);

    return (int64_t)osal_ret;
}


/* TEST Thread R_OSAL_ThsyncCondWaitForTimePeriod */
int64_t osal_test_cond_waitforperiod(void *user_arg, int test_count)
{
    e_osal_return_t     osal_ret;
    st_arg_cond_t*      arg_cond;
    st_osal_time_t      current_time_1st, current_time_2nd;
    osal_nano_sec_t     exec_time;

    /* cond wait */
    arg_cond = (st_arg_cond_t *)user_arg;

    /* mutex lock */
    R_OSAL_ThsyncMutexTryLock(arg_cond->mutex_handle);

    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncCondWaitForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(arg_cond->cond_handle, arg_cond->mutex_handle, arg_cond->time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncCondWaitForTimePeriod, g_count, exec_time);
    elapsed_time[TEST_ThsyncCondWaitForTimePeriod][g_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondWaitForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncCondWaitForTimePeriod ***\n");

    R_OSAL_ThsyncMutexUnlock(arg_cond->mutex_handle);

    return (int64_t)osal_ret;
}


/***********************************************************************************************************************
* Start of function Test_ThreadSyncManager_Mutex()
***********************************************************************************************************************/
static int Test_ThreadSyncManager_Cond (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    osal_cond_id_t              cond_id;
    osal_cond_handle_t          cond_handle;
    osal_mutex_handle_t         mutex_handle;
    osal_milli_sec_t time;
    st_osal_time_t              absolute_time;
    osal_mutex_id_t             mutex_id = MUTEX_ID_NO1;

    /* Test Thread Parameter */
    st_arg_cond_t               arg_cond;
    st_osal_thread_config_t     config1;
    st_osal_thread_config_t     config2;
    osal_thread_handle_t        handle1;
    osal_thread_handle_t        handle2;
    osal_thread_id_t            thread_id;
    int64_t                     thread_ret  = 0;

    st_osal_time_t              current_time_1st, current_time_2nd;
    osal_nano_sec_t             exec_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    cond_id     = COND_ID_NO1;
    memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
    memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));

    /* R_OSAL_ThsyncCondCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncCondCreate ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncCondCreate, test_count, exec_time);
    elapsed_time[TEST_ThsyncCondCreate][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncCondCreate ***\n");

    //R_OSAL_ThsyncMutexCreate
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    memset(&arg_cond, 0x00, sizeof(st_arg_cond_t));
    memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
    memset(&handle1, 0x00, sizeof(osal_thread_handle_t));

    arg_cond.mutex_handle    = mutex_handle;
    arg_cond.cond_handle    = cond_handle;
    arg_cond.time            = WAIT_TIME_MS;

    config1.func       = osal_test_cond_waitforperiod;
    config1.userarg    = (void *)&arg_cond;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "R_OSAL_ThsyncCondWaitForTimePeriod";
    config1.stack_size = 1024;

    /* R_OSAL_ThreadCreate */
    thread_id = THREAD_MAIN_TASK_ID;
    osal_ret = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThreadSleepForTimePeriod */
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(5);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSleepForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* mutex lock */
    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexTryLock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncCondSignal */    /* requires R_OSAL_ThsyncCondCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncCondSignal ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncCondSignal, test_count, exec_time);
    elapsed_time[TEST_ThsyncCondSignal][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondSignal is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncCondSignal ***\n");

    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThreadJoin handle1 */
    osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadJoin is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
    memset(&handle2, 0x00, sizeof(osal_thread_handle_t));

    /* R_OSAL_ThsyncCondWaitUntilTimeStamp */    /* requires R_OSAL_ThsyncCondCreate */
    arg_cond.mutex_handle    = mutex_handle;
    arg_cond.cond_handle    = cond_handle;

    GetAbsoluteTime(&arg_cond.abs_time);

    config2.func        = osal_test_cond_waitunittimestamp;
    config2.userarg        = (void *)&arg_cond;
    config2.priority    = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name    = "R_OSAL_ThsyncCondWaitUntilTimeStamp";
    config2.stack_size    = 1024;

    /* R_OSAL_ThreadCreate */
    thread_id = THREAD_MAIN_TASK_ID2;
    osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThreadSleepForTimePeriod */
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(5);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSleepForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* mutex lock */
    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexTryLock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncCondBroadcast */    /* requires R_OSAL_ThsyncCondCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncCondBroadcast ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncCondBroadcast, test_count, exec_time);
    elapsed_time[TEST_ThsyncCondBroadcast][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondBroadcast is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncCondBroadcast ***\n");

    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThreadJoin handle2 */
    osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadJoin is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncCondDestroy */    /* requires R_OSAL_ThsyncCondCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncCondDestroy ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_ThsyncCondDestroy, test_count, exec_time);
    elapsed_time[TEST_ThsyncCondDestroy][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ThsyncCondDestroy ***\n");

    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ThreadSyncManager_Cond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_MessageManager()
***********************************************************************************************************************/
static int Test_MessageManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret;
    int                             ret;

    st_osal_mq_config_t             mq_config;
    osal_mq_id_t                    mq_id;
    osal_mq_handle_t                mq_handle;

    st_osal_time_t                  time_stamp;
    size_t                          buffer_size = sizeof(int);
    int                             p_buffer = 0;
    osal_milli_sec_t                time_period = 10;

    bool                            is_full;
    bool                            mq_result;

    uint32_t                        msg = 0xFFFF;
    char                            rcv_data[128];

    osal_thread_handle_t            thread_handle;
    st_osal_thread_config_t         config;
    int64_t                         return_value;
    st_arg_mq_send_timeperiod_t     send_timeperiod_arg;
    st_arg_mq_receive_timeperiod_t  receive_timeperiod_arg;

    st_osal_time_t                  current_time_1st, current_time_2nd;
    osal_nano_sec_t                 exec_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    /* R_OSAL_MqInitializeMqConfigSt */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqInitializeMqConfigSt ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqInitializeMqConfigSt(&mq_config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqInitializeMqConfigSt, test_count, exec_time);
    elapsed_time[TEST_MqInitializeMqConfigSt][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqInitializeMqConfigSt is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqInitializeMqConfigSt ***\n");

    memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    mq_id = MQ_ID_NO1;
    memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));


    /* R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqCreate ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqCreate, test_count, exec_time);
    elapsed_time[TEST_MqCreate][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqCreate ***\n");

    /* R_OSAL_MqGetConfig */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqGetConfig ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqGetConfig(mq_handle, &mq_config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqGetConfig, test_count, exec_time);
    elapsed_time[TEST_MqGetConfig][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqGetConfig is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqGetConfig ***\n");

    /* R_OSAL_MqSendUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);

    OSAL_TEST_INFO("*** Start: R_OSAL_MqSendUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqSendUntilTimeStamp, test_count, exec_time);
    elapsed_time[TEST_MqSendUntilTimeStamp][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqSendUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqSendUntilTimeStamp ***\n");

    /* R_OSAL_MqReceiveUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);

    OSAL_TEST_INFO("*** Start: R_OSAL_MqReceiveUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqReceiveUntilTimeStamp, test_count, exec_time);
    elapsed_time[TEST_MqReceiveUntilTimeStamp][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqReceiveUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqReceiveUntilTimeStamp ***\n");

    time_period = WAIT_TIME_MS;
    OSAL_TEST_INFO("*** Start: R_OSAL_MqSendForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, time_period, (void *)&msg, sizeof(msg));
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqSendForTimePeriod, test_count, exec_time);
    elapsed_time[TEST_MqSendForTimePeriod][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqSendForTimePeriod ***\n");

    time_period = WAIT_TIME_MS;
    OSAL_TEST_INFO("*** Start: R_OSAL_MqReceiveForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, (void *)rcv_data, sizeof(msg));
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqReceiveForTimePeriod, test_count, exec_time);
    elapsed_time[TEST_MqReceiveForTimePeriod][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqReceiveForTimePeriod ***\n");

    /* R_OSAL_MqIsFull */    /* requires R_OSAL_MqCreate */
    is_full = false;
    OSAL_TEST_INFO("*** Start: R_OSAL_MqIsFull ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqIsFull(mq_handle, &is_full);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqIsFull, test_count, exec_time);
    elapsed_time[TEST_MqIsFull][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqIsFull is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqIsFull ***\n");

    /* R_OSAL_MqIsEmpty */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqIsEmpty ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqIsEmpty(mq_handle, &mq_result);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqIsEmpty, test_count, exec_time);
    elapsed_time[TEST_MqIsEmpty][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqIsEmpty is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqIsEmpty ***\n");

    /* R_OSAL_MqReset */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqReset ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqReset(mq_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqReset, test_count, exec_time);
    elapsed_time[TEST_MqReset][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqReset is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqReset ***\n");

    /*R_OSAL_MqDelete */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqDelete ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqDelete(mq_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_MqDelete, test_count, exec_time);
    elapsed_time[TEST_MqDelete][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqDelete is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MqDelete ***\n");

    return 0;
}
/***********************************************************************************************************************
* End of function Test_MessageManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_ClockTimeManager()
***********************************************************************************************************************/
static int Test_ClockTimeManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             ret;

    int             i;
    st_osal_time_t  time;
    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_nano_sec_t exec_time = 0;
    osal_nano_sec_t diff_time = 0;
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    OSAL_TEST_INFO("*** Start: R_OSAL_ClockTimeGetTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_ClockTimeGetTimeStamp, test_count, exec_time);
    elapsed_time[TEST_ClockTimeGetTimeStamp][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ClockTimeGetTimeStamp ***\n");

    OSAL_TEST_INFO("*** Start: R_OSAL_ClockTimeCalculateTimeDifference ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_ClockTimeCalculateTimeDifference, test_count, exec_time);
    elapsed_time[TEST_ClockTimeCalculateTimeDifference][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_ClockTimeGetTimeStamp ***\n");

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ClockTimeManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_InterruptManager()
***********************************************************************************************************************/
void osal_test_cb(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{
    return;
}
static int Test_InterruptManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    int                     ret;

    bool                    result;

    size_t                  numOfDevices;
    char                    list[200];
    size_t                  list_size;
    size_t                  numOfLists;
    osal_device_handle_t    dev_handle;
    char*                   device;
    const char              s[2] = "\n";

    size_t                  num_channels = 0;

    int                     arg = 10;

    st_osal_time_t          current_time_1st, current_time_2nd;
    osal_nano_sec_t         exec_time;
#if defined (RCAR_S4)
    char*                   device_type = "caiplite";
#else /* V3Hx, V3M2, V4H, V4M */
    char*                   device_type = "ims";
#endif

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    list_size = sizeof(list);

    /* R_OSAL_InterruptIsISRContext */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptIsISRContext ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptIsISRContext(&result);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_InterruptIsISRContext, test_count, exec_time);
    elapsed_time[TEST_InterruptIsISRContext][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptIsISRContext is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_InterruptIsISRContext ***\n");

    /* R_OSAL_InterruptGetNumOfIrqChannels */    /* requires R_OSAL_IoDeviceOpen */
    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
    osal_ret = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
    device = strtok(list, s);
    osal_ret = R_OSAL_IoDeviceOpen(device, &dev_handle);

    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptGetNumOfIrqChannels ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_InterruptGetNumOfIrqChannels, test_count, exec_time);
    elapsed_time[TEST_InterruptGetNumOfIrqChannels][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptGetNumOfIrqChannels is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_InterruptGetNumOfIrqChannels ***\n");

    /* R_OSAL_InterruptRegisterIsr */    /* requires R_OSAL_InterruptGetNumOfIrqChannels */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptRegisterIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                               (p_osal_isr_func_t)osal_test_cb, (void *)&arg);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_InterruptRegisterIsr, test_count, exec_time);
    elapsed_time[TEST_InterruptRegisterIsr][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_InterruptRegisterIsr ***\n");

    /* R_OSAL_InterruptEnableIsr */        /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptEnableIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_InterruptEnableIsr, test_count, exec_time);
    elapsed_time[TEST_InterruptEnableIsr][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_InterruptEnableIsr ***\n");

    /* R_OSAL_InterruptDisableIsr */    /* requires R_OSAL_InterruptEnableIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptDisableIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_InterruptDisableIsr, test_count, exec_time);
    elapsed_time[TEST_InterruptDisableIsr][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_InterruptDisableIsr ***\n");

    /* R_OSAL_InterruptSetIsrPriority */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptSetIsrPriority ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_InterruptSetIsrPriority, test_count, exec_time);
    elapsed_time[TEST_InterruptSetIsrPriority][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptSetIsrPriority is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_InterruptSetIsrPriority ***\n");

    /* R_OSAL_InterruptUnregisterIsr */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptUnregisterIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_InterruptUnregisterIsr, test_count, exec_time);
    elapsed_time[TEST_InterruptUnregisterIsr][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_InterruptUnregisterIsr ***\n");

    osal_ret = R_OSAL_IoDeviceClose(dev_handle);

    return 0;
}
/***********************************************************************************************************************
* End of function Test_InterruptManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_IOManager()
***********************************************************************************************************************/
static int Test_IOManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    int                     ret;

    size_t                  numOfDevices;
    char                    list[400];
    size_t                  list_size;
    size_t                  numOfLists;

    osal_device_handle_t    handle;
    char*                   device;
    const char              s[2] = "\n";

    uintptr_t               offset;
    uint8_t                 read_data8 = 0;
    uint16_t                read_data16 = 0;
    uint32_t                read_data32 = 0;
    uint64_t                read_data64 = 0;

    size_t                  size;

    uint8_t                 read_block_data8[16];
    uint8_t                 register_data8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    uint16_t                read_block_data16[8];
    uint16_t                register_data16[8] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000, 0x0100, 0x0000};

    uint32_t                read_block_data32[4];
    uint32_t                register_data32[4] = {0x00000000, 0x00000000, 0x00000040, 0x00000100};

    uint64_t                read_block_data64[2];
    uint64_t                register_data64[2] = {0x000000FF000000FF, 0x0000000000000000};

    osal_axi_bus_id_t       axi_id = 0;
    char                    axi_name[100];
    size_t                  numOfName = 0;

    char                    ax_list[400];
    size_t                  ax_list_size;
    size_t                  ax_numOfLists;
    char*                   axi_bus_name;

    size_t                  numOfaxiids;

    st_osal_time_t          current_time_1st, current_time_2nd;
    osal_nano_sec_t         exec_time;

#if defined (RCAR_S4)
    char*                   device_type = "caiplite";
#else /* V3Hx, V3M2, V4H, V4M */
    char*                   device_type = "ims";
#endif

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&handle, 0x00, sizeof(osal_device_handle_t));
    list_size = sizeof(list);

    /* R_OSAL_IoGetNumOfDevices */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfDevices ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetNumOfDevices, test_count, exec_time);
    elapsed_time[TEST_IoGetNumOfDevices][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfDevices is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetNumOfDevices ***\n");

    /* R_OSAL_IoGetDeviceList */    /* requires R_OSAL_IoGetNumOfDevices */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetDeviceList ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetDeviceList, test_count, exec_time);
    elapsed_time[TEST_IoGetDeviceList][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetDeviceList is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetDeviceList ***\n");

    device = strtok(list, s);
    /* R_OSAL_IoDeviceOpen */    /* requires R_OSAL_IoGetDeviceList */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoDeviceOpen ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoDeviceOpen(device, &handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoDeviceOpen, test_count, exec_time);
    elapsed_time[TEST_IoDeviceOpen][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceOpen is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoDeviceOpen ***\n");

    /* set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED */
    osal_ret = R_OSAL_PmSetRequiredState(handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("Set required state failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoWrite8 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoWrite8 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoWrite8(handle, offset, 0xff);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoWrite8, test_count, exec_time);
    elapsed_time[TEST_IoWrite8][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoWrite8 ***\n");

    /* R_OSAL_IoRead8 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead8 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead8(handle, offset, &read_data8);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoRead8, test_count, exec_time);
    elapsed_time[TEST_IoRead8][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoRead8 ***\n");

    /* R_OSAL_IoWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoWrite16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoWrite16(handle, offset, 0xff);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoWrite16, test_count, exec_time);
    elapsed_time[TEST_IoWrite16][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoWrite16 ***\n");

    /* R_OSAL_IoRead16 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead16(handle, offset, &read_data16);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoRead16, test_count, exec_time);
    elapsed_time[TEST_IoRead16][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoRead16 ***\n");

    /* R_OSAL_IoWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoWrite32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoWrite32(handle, offset, 0xff);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoWrite32, test_count, exec_time);
    elapsed_time[TEST_IoWrite32][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoWrite32 ***\n");

    /* R_OSAL_IoRead32 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead32(handle, offset, &read_data32);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoRead32, test_count, exec_time);
    elapsed_time[TEST_IoRead32][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoRead32 ***\n");

    /* R_OSAL_IoWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoWrite64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoWrite64(handle, offset, 0xff);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoWrite64, test_count, exec_time);
    elapsed_time[TEST_IoWrite64][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoWrite64 ***\n");

    /* R_OSAL_IoRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead64(handle, offset, &read_data64);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoRead64, test_count, exec_time);
    elapsed_time[TEST_IoRead64][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoRead64 ***\n");

#if 0 /* Access to registers in R-Car is only supported in 32-bit units.  */
    /* R_OSAL_IoBlockWrite8 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite8 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite8(handle, offset, register_data8, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockWrite8, test_count, exec_time);
    elapsed_time[TEST_IoBlockWrite8][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockWrite8 ***\n");

    /* R_OSAL_IoBlockRead8 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data8, 0x00, sizeof(read_block_data8));
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead8 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_block_data8, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockRead8, test_count, exec_time);
    elapsed_time[TEST_IoBlockRead8][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockRead8 ***\n");

    /* R_OSAL_IoBlockWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite16(handle, offset, register_data16, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockWrite16, test_count, exec_time);
    elapsed_time[TEST_IoBlockWrite16][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockWrite16 ***\n");

    /* R_OSAL_IoBlockRead16 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data16, 0x00, sizeof(read_block_data16));
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_block_data16, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockRead16, test_count, exec_time);
    elapsed_time[TEST_IoBlockRead16][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockRead16 ***\n");
#endif

    /* R_OSAL_IoBlockWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite32(handle, offset, register_data32, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockWrite32, test_count, exec_time);
    elapsed_time[TEST_IoBlockWrite32][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockWrite32 ***\n");

    /* R_OSAL_IoBlockRead32 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data32, 0x00, sizeof(read_block_data32));
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_block_data32, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockRead32, test_count, exec_time);
    elapsed_time[TEST_IoBlockRead32][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockRead32 ***\n");

    /* R_OSAL_IoBlockWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite64(handle, offset, register_data64, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockWrite64, test_count, exec_time);
    elapsed_time[TEST_IoBlockWrite64][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockWrite64 ***\n");

    /* R_OSAL_IoBlockRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_block_data64, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoBlockRead64, test_count, exec_time);
    elapsed_time[TEST_IoBlockRead64][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoBlockRead64 ***\n");

    /* R_OSAL_IoGetDeviceAxiBusId */    /* requires R_OSAL_IoDeviceOpen? */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetDeviceAxiBusId ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetDeviceAxiBusId (handle, &axi_id);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetDeviceAxiBusId, test_count, exec_time);
    elapsed_time[TEST_IoGetDeviceAxiBusId][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetDeviceAxiBusId is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetDeviceAxiBusId ***\n");

    /* R_OSAL_IoDeviceClose */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoDeviceClose ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoDeviceClose(handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoDeviceClose, test_count, exec_time);
    elapsed_time[TEST_IoDeviceClose][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceClose is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoDeviceClose ***\n");

    /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiBusIdFromDeviceName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName(device, &axi_id);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetAxiBusIdFromDeviceName, test_count, exec_time);
    elapsed_time[TEST_IoGetAxiBusIdFromDeviceName][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiBusIdFromDeviceName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetAxiBusIdFromDeviceName ***\n");

    /* R_OSAL_IoGetAxiBusName */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiBusName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, axi_name, sizeof(axi_name), &numOfName);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetAxiBusName, test_count, exec_time);
    elapsed_time[TEST_IoGetAxiBusName][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiBusName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetAxiBusName ***\n");

    /* R_OSAL_IoGetAxiBusNameList */
    ax_list_size = sizeof(ax_list);
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiBusNameList ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiBusNameList(ax_list, ax_list_size, &ax_numOfLists);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetAxiBusNameList, test_count, exec_time);
    elapsed_time[TEST_IoGetAxiBusNameList][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiBusNameList is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetAxiBusNameList ***\n");


#if defined (RCAR_S4)
    device_type        = "rtdmac_control0";
    axi_bus_name       = "rt1_00_04";
#elif defined (RCAR_V4H) || defined (RCAR_V4M)
    device_type        = "ims";
    axi_bus_name       = "vc_00_04";
#elif defined (RCAR_V3M)
    device_type        = "ims";
    axi_bus_name       = "vi0_02";
#else /* V3Hx */
    device_type        = "ims";
    axi_bus_name       = "vc0_04";
#endif

    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("new api test R_OSAL_IoGetNumOfDevices is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    osal_ret = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("new api test R_OSAL_IoGetDeviceList is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    device = strtok(list, s);

    osal_ret = R_OSAL_IoDeviceOpen(device, &handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("new api test R_OSAL_IoDeviceOpen is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfAxiIdsForDeviceHdl ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl( handle, &numOfaxiids );
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetNumOfAxiIdsForDeviceHdl, test_count, exec_time);
    elapsed_time[TEST_IoGetNumOfAxiIdsForDeviceHdl][test_count] = exec_time;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForDeviceHdl is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetNumOfAxiIdsForDeviceHdl ***\n");

    /* R_OSAL_IoGetAxiIdListForDeviceHdl */    /* requires R_OSAL_IoDeviceOpen */
    ax_list_size = sizeof(ax_list);
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiIdListForDeviceHdl ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl( handle, &ax_list, ax_list_size, &numOfaxiids );
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetAxiIdListForDeviceHdl, test_count, exec_time);
    elapsed_time[TEST_IoGetAxiIdListForDeviceHdl][test_count] = exec_time;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiIdListForDeviceHdl is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetAxiIdListForDeviceHdl ***\n");

    /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfAxiIdsForDeviceName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName( device, &numOfaxiids );
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetNumOfAxiIdsForDeviceName, test_count, exec_time);
    elapsed_time[TEST_IoGetNumOfAxiIdsForDeviceName][test_count] = exec_time;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForDeviceName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetNumOfAxiIdsForDeviceName ***\n");

    /* R_OSAL_IoGetAxiIdListForDeviceName */
    ax_list_size = sizeof(ax_list);
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiIdListForDeviceName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiIdListForDeviceName( device, &ax_list, ax_list_size, &numOfaxiids );
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetAxiIdListForDeviceName, test_count, exec_time);
    elapsed_time[TEST_IoGetAxiIdListForDeviceName][test_count] = exec_time;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiIdListForDeviceName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetAxiIdListForDeviceName ***\n");

    /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfAxiIdsForAxiBusName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name, &numOfaxiids );
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetNumOfAxiIdsForAxiBusName, test_count, exec_time);
    elapsed_time[TEST_IoGetNumOfAxiIdsForAxiBusName][test_count] = exec_time;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForAxiBusName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetNumOfAxiIdsForAxiBusName ***\n");

    /* R_OSAL_IoGetAxiIdListForAxiBusName */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiIdListForAxiBusName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name, &ax_list, ax_list_size, &numOfaxiids );
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_IoGetAxiIdListForAxiBusName, test_count, exec_time);
    elapsed_time[TEST_IoGetAxiIdListForAxiBusName][test_count] = exec_time;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiIdListForAxiBusName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_IoGetAxiIdListForAxiBusName ***\n");

    osal_ret = R_OSAL_IoDeviceClose(handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("new api test R_OSAL_IoDeviceClose is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_IOManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_PowerManager()
***********************************************************************************************************************/
static int Test_PowerManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    int                         ret;

    size_t                      numOfDevices;
    char                        list[950];
    size_t                      list_size;
    size_t                      numOfLists;
    osal_device_handle_t        dev_handle = OSAL_DEVICE_HANDLE_INVALID;
    char*                       device;
    const char                  s[2] = "\n";

    e_osal_pm_required_state_t  req_state;
    e_osal_pm_state_t           pm_state;
    e_osal_pm_policy_t          policy;
    e_osal_pm_reset_state_t     reset_state;

    st_osal_time_t              current_time_1st, current_time_2nd;
    osal_nano_sec_t             exec_time;

#if defined (RCAR_S4)
    char*                       device_type  = "caiplite";
#elif defined (RCAR_V4H) || defined (RCAR_V4M)
    char*                       device_type  = "ims";
#else /* V3Hx, V3M2 */
    char*                       device_type  = "imp";
#endif

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    list_size = sizeof(list);

    /* requires R_OSAL_IoDeviceOpen */
    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfDevices is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    osal_ret = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetDeviceList is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    device = strtok(list, s);

    osal_ret = R_OSAL_IoDeviceOpen(device, &dev_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceOpen is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmSetRequiredState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmSetRequiredState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmSetRequiredState (dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmSetRequiredState, test_count, exec_time);
    elapsed_time[TEST_PmSetRequiredState][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetRequiredState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmSetRequiredState ***\n");

    /* R_OSAL_PmSetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmSetResetState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmSetResetState (dev_handle, OSAL_PM_RESET_STATE_APPLIED);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmSetResetState, test_count, exec_time);
    elapsed_time[TEST_PmSetResetState][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetResetState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmSetResetState ***\n");

    /* R_OSAL_PmGetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetState (dev_handle, &pm_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmGetState, test_count, exec_time);
    elapsed_time[TEST_PmGetState][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmGetState ***\n");

    /* R_OSAL_PmWaitForState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmWaitForState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmWaitForState (dev_handle, OSAL_PM_STATE_RESET, 1000);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmWaitForState, test_count, exec_time);
    elapsed_time[TEST_PmWaitForState][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmWaitForState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmWaitForState ***\n");

    /* R_OSAL_PmSetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmSetPolicy ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmSetPolicy (dev_handle, OSAL_PM_POLICY_PG, true);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmSetPolicy, test_count, exec_time);
    elapsed_time[TEST_PmSetPolicy][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetPolicy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmSetPolicy ***\n");

    /* R_OSAL_PmGetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetPolicy ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetPolicy (dev_handle, &policy);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmGetPolicy, test_count, exec_time);
    elapsed_time[TEST_PmGetPolicy][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetPolicy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmGetPolicy ***\n");

    /* R_OSAL_PmGetRequiredState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetRequiredState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetRequiredState (dev_handle, &req_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmGetRequiredState, test_count, exec_time);
    elapsed_time[TEST_PmGetRequiredState][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetRequiredState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmGetRequiredState ***\n");

    /* R_OSAL_PmGetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetResetState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetResetState (dev_handle, &reset_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmGetResetState, test_count, exec_time);
    elapsed_time[TEST_PmGetResetState][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetResetState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmGetResetState ***\n");

    /* R_OSAL_PmGetLowestIntermediatePowerState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetLowestIntermediatePowerState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetLowestIntermediatePowerState (dev_handle, &pm_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmGetLowestIntermediatePowerState, test_count, exec_time);
    elapsed_time[TEST_PmGetLowestIntermediatePowerState][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetLowestIntermediatePowerState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmGetLowestIntermediatePowerState ***\n");

    osal_ret = R_OSAL_IoDeviceClose(dev_handle);


    /* R_OSAL_PmSetPostClock */    /* requires R_OSAL_IoDeviceOpen */
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    R_OSAL_IoGetNumOfDevices("fba", &numOfDevices);
    R_OSAL_IoGetDeviceList("fba", list, list_size, &numOfLists);
    device = strtok(list, s);
    R_OSAL_IoDeviceOpen(device, &dev_handle);

    OSAL_TEST_INFO("*** Start: R_OSAL_PmSetPostClock ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmSetPostClock (dev_handle, true, 63);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_PmSetPostClock, test_count, exec_time);
    elapsed_time[TEST_PmSetPostClock][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetPostClock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_PmSetPostClock ***\n");

    R_OSAL_IoDeviceClose(dev_handle);

    return 0;
}
/***********************************************************************************************************************
* End of function Test_PowerManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_MemoryManager()
***********************************************************************************************************************/
/* Pseudo user callback functions */
void osal_memory_mmngr_cb(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error,
                         osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)error;
    (void)buffer_hndl;

    return;
}

static int Test_MemoryManager (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret;
    int                             ret;

    osal_memory_manager_handle_t    mm_handle;

    size_t                          size = 4096;
    size_t                          alignment = 4096;
    osal_memory_region_idx_t        region_idx = 0;
    osal_memory_buffer_handle_t     buffer_obj;

    st_osal_mmngr_config_t          config;
    e_osal_mmngr_event_t            monitored_event = OSAL_MMNGR_EVENT_ALLOCATE;
    int                             user_arg = 55;

    uintptr_t*                      cpu_ptr;

    size_t                          offset = 0;
    uintptr_t                       hw_addr = 0;

    osal_axi_bus_id_t               axi_id = 1;

    size_t                          osize     = 0;

    osal_memory_buffer_t            memory_buffer;
    osal_memory_manager_t           memory_manager;

    st_osal_time_t                  current_time_1st, current_time_2nd;
    osal_nano_sec_t                 exec_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&mm_handle, 0x00, sizeof(osal_memory_manager_handle_t));

    /* R_OSAL_MmngrGetOsalMaxConfig */    /* requires R_OSAL_MmngrOpen */ /* ??? need to be executed before R_OSAL_MmngrOpen()? */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetOsalMaxConfig ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrGetOsalMaxConfig, test_count, exec_time);
    elapsed_time[TEST_MmngrGetOsalMaxConfig][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfig is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrGetOsalMaxConfig ***\n");

    /* R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrOpen ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrOpen(&config , &mm_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrOpen, test_count, exec_time);
    elapsed_time[TEST_MmngrOpen][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrOpen ***\n");

    /* R_OSAL_MmngrAlloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrAlloc ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrAlloc(mm_handle, size, alignment, &buffer_obj);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrAlloc, test_count, exec_time);
    elapsed_time[TEST_MmngrAlloc][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrAlloc ***\n");

    /* R_OSAL_MmngrGetConfig */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetConfig ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetConfig(mm_handle, &config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrGetConfig, test_count, exec_time);
    elapsed_time[TEST_MmngrGetConfig][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetConfig is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrGetConfig ***\n");

    /* R_OSAL_MmngrInitializeMmngrConfigSt */    /* requires R_OSAL_MmngrOpen */ /* ??? */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInitializeMmngrConfigSt ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(&config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrInitializeMmngrConfigSt, test_count, exec_time);
    elapsed_time[TEST_MmngrInitializeMmngrConfigSt][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInitializeMmngrConfigSt is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrInitializeMmngrConfigSt ***\n");

    /* R_OSAL_MmngrRegisterMonitor */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrRegisterMonitor ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrRegisterMonitor(mm_handle, monitored_event, osal_memory_mmngr_cb, (void*)&user_arg);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrRegisterMonitor, test_count, exec_time);
    elapsed_time[TEST_MmngrRegisterMonitor][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrRegisterMonitor is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrRegisterMonitor ***\n");

    /* R_OSAL_MmngrGetCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetCpuPtr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetCpuPtr(buffer_obj, (void **)&cpu_ptr);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrGetCpuPtr, test_count, exec_time);
    elapsed_time[TEST_MmngrGetCpuPtr][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetCpuPtr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrGetCpuPtr ***\n");

    /* R_OSAL_MmngrGetConstCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetConstCpuPtr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buffer_obj, (void **)&cpu_ptr);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrGetConstCpuPtr, test_count, exec_time);
    elapsed_time[TEST_MmngrGetConstCpuPtr][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetConstCpuPtr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrGetConstCpuPtr ***\n");

    /* R_OSAL_MmngrFlush */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrFlush ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrFlush(buffer_obj, offset, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrFlush, test_count, exec_time);
    elapsed_time[TEST_MmngrFlush][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrFlush is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrFlush ***\n");

    /* R_OSAL_MmngrInvalidate */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInvalidate ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInvalidate(buffer_obj, offset, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrInvalidate, test_count, exec_time);
    elapsed_time[TEST_MmngrInvalidate][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInvalidate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrInvalidate ***\n");

    /* R_OSAL_MmngrGetHwAddr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetHwAddr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetHwAddr(buffer_obj, axi_id, &hw_addr);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrGetHwAddr, test_count, exec_time);
    elapsed_time[TEST_MmngrGetHwAddr][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetHwAddr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrGetHwAddr ***\n");

    /* R_OSAL_MmngrGetSize */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetSize ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetSize(buffer_obj, &osize);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrGetSize, test_count, exec_time);
    elapsed_time[TEST_MmngrGetSize][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetSize is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrGetSize ***\n");

    /* R_OSAL_MmngrCheckBuffer */    /* requires R_OSAL_MmngrRegisterMonitor, R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrCheckBuffer ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrCheckBuffer(buffer_obj);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrCheckBuffer, test_count, exec_time);
    elapsed_time[TEST_MmngrCheckBuffer][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrCheckBuffer is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrCheckBuffer ***\n");

    /* R_OSAL_MmngrDebugMmngrPrint */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrDebugMmngrPrint ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrDebugMmngrPrint(mm_handle, stdout);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrDebugMmngrPrint, test_count, exec_time);
    elapsed_time[TEST_MmngrDebugMmngrPrint][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_INFO("R_OSAL_MmngrDebugMmngrPrint is failed, osal_ret = %d\n", osal_ret);
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrDebugMmngrPrint ***\n");

    /* R_OSAL_MmngrDebugBufferPrint */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrDebugBufferPrint ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrDebugBufferPrint(buffer_obj, stdout);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrDebugBufferPrint, test_count, exec_time);
    elapsed_time[TEST_MmngrDebugBufferPrint][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_INFO("R_OSAL_MmngrDebugBufferPrint is failed, osal_ret = %d\n", osal_ret);
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrDebugBufferPrint ***\n");

    /* R_OSAL_MmngrInitializeMemoryBufferObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInitializeMemoryBufferObj ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&memory_buffer);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrInitializeMemoryBufferObj, test_count, exec_time);
    elapsed_time[TEST_MmngrInitializeMemoryBufferObj][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInitializeMemoryBufferObj is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrInitializeMemoryBufferObj ***\n");

    /* R_OSAL_MmngrInitializeMemoryManagerObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInitializeMemoryManagerObj ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&memory_manager);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrInitializeMemoryManagerObj, test_count, exec_time);
    elapsed_time[TEST_MmngrInitializeMemoryManagerObj][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInitializeMemoryManagerObj is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrInitializeMemoryManagerObj ***\n");

    /* R_OSAL_MmngrDealloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrDealloc ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrDealloc(mm_handle, buffer_obj);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrDealloc, test_count, exec_time);
    elapsed_time[TEST_MmngrDealloc][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrDealloc is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrDealloc ***\n");

    /* R_OSAL_MmngrClose */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrClose ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrClose(mm_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrClose, test_count, exec_time);
    elapsed_time[TEST_MmngrClose][test_count] = exec_time;

    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrClose ***\n");

    memset(&mm_handle, 0x00, sizeof(osal_memory_manager_handle_t));

    /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(region_idx, &config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrGetOsalMaxConfigFromRegionIdx, test_count, exec_time);
    elapsed_time[TEST_MmngrGetOsalMaxConfigFromRegionIdx][test_count] = exec_time;

    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx ***\n");

    /* R_OSAL_MmngrOpenFromRegionIdx */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrOpenFromRegionIdx ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config , region_idx, &mm_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", TEST_MmngrOpenFromRegionIdx, test_count, exec_time);
    elapsed_time[TEST_MmngrOpenFromRegionIdx][test_count] = exec_time;

    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpenFromRegionIdx is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }
    OSAL_TEST_INFO("*** End: R_OSAL_MmngrOpenFromRegionIdx ***\n");

    /* R_OSAL_MmngrClose */
    osal_ret = R_OSAL_MmngrClose(mm_handle);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_MemoryManager()
***********************************************************************************************************************/


/***********************************************************************************************************************
* Start of function CheckVersion()
***********************************************************************************************************************/
static int CheckVersion (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    int             ret;
    int             i;
    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_nano_sec_t exec_time = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    OSAL_TEST_INFO("*** Start: R_OSAL_GetVersion ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    (void)R_OSAL_GetVersion();
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    OSAL_TEST_INFO("*** exec_time[%d][%d]: %d ***\n", TEST_GetVersion, test_count, exec_time);
    elapsed_time[TEST_GetVersion][test_count] = exec_time;
    OSAL_TEST_INFO("*** End: R_OSAL_GetVersion ***\n");

    return 0;
}
/***********************************************************************************************************************
* End of function CheckVersion()
***********************************************************************************************************************/


/*** Common Utils ***/
/***********************************************************************************************************************
* Start of function GetAbsoluteTime()
***********************************************************************************************************************/
/* GetAbsoluteTime */
static void GetAbsoluteTime(st_osal_time_t* iop_absolute_time)
{
    e_osal_return_t osal_ret;
    st_osal_time_t current_time;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
    if(current_time.tv_nsec + WAIT_TIME_NS <= 999999999)
    {
        iop_absolute_time->tv_nsec = current_time.tv_nsec + WAIT_TIME_NS;
        iop_absolute_time->tv_sec = current_time.tv_sec + 5;
    }
    else
    {
        iop_absolute_time->tv_sec = current_time.tv_sec + 5;
        iop_absolute_time->tv_nsec = WAIT_TIME_NS - (999999999 - current_time.tv_nsec);
    }
}
/***********************************************************************************************************************
* End of function GetAbsoluteTime()
***********************************************************************************************************************/
