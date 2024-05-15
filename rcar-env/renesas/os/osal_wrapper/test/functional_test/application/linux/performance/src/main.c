/***********************************************************************************************************************
* File Name :    main.c
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Performance Test program for OSAL Linux IT
***********************************************************************************************************************/

/*** include ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

/*** definition ***/
#define TEST_NUM 100
#define TEST_COUNT 10

#define WAIT_TIME_MS 10         // 10 milisecond
#define WAIT_TIME_NS 10000000   // 10 milisecond
#define MAX_NS 1000000000

#define USEC_TO_NSEC (1000)
#define SEC_TO_NSEC  (1000000000ULL)

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
#define THREAD_MAIN_TASK_ID_ALL 0xf000U + 2U
/* message queue ID */
#define MQ_ID_NO1               0xf000U
/* mutex ID */
#define MUTEX_ID_NO1            0xf000U
/* condition variable ID */
#define COND_ID_NO1             0xf000U

#if defined(NDEBUG)
static const bool               ndebug = true;
#else
static const bool               ndebug = false;
#endif

#define SET_TIME(api_name, api_index, loop_index, ret, time)                                            \
{                                                                                                       \
    OSAL_TEST_INFO("***exec_time[%d][%d]: %d ***\n", api_index, loop_index, time);                      \
                                                                                                        \
    if (OSAL_RETURN_OK == ret)                                                                          \
    {                                                                                                   \
        test_pass++;                                                                                    \
        elapsed_time[api_index][loop_index] = time;                                                     \
    }                                                                                                   \
    else                                                                                                \
    {                                                                                                   \
        if ((true == ndebug) &&                                                                         \
            ((TEST_MmngrDebugMmngrPrint == api_index) || (TEST_MmngrDebugBufferPrint == api_index)))    \
        {                                                                                               \
            test_pass++;                                                                                \
            elapsed_time[api_index][loop_index] = time;                                                 \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            OSAL_TEST_ERR("%s is failed, osal_ret = %d\n", api_name, ret);                              \
            test_fail++;                                                                                \
            elapsed_time[api_index][loop_index] = -1;                                                   \
            return -1;                                                                                  \
        }                                                                                               \
    }                                                                                                   \
    OSAL_TEST_INFO("*** End: %s ***\n", api_name);                                                      \
}                                                                                                       \

#define TEST_Initialize                         0
#define TEST_Deinitialize                       1
#define TEST_GetVersion                         2
#define TEST_ThreadCreate                       3
#define TEST_ThreadSleepUntilTimeStamp          4
#define TEST_ThreadSleepForTimePeriod           5
#define TEST_ThreadSelf                         6
#define TEST_ThreadEqual                        7
#define TEST_ThreadInitializeThreadConfigSt     8
#define TEST_ThreadJoin                         9
#define TEST_ThsyncMutexCreate                  10
#define TEST_ThsyncMutexDestroy                 11
#define TEST_ThsyncMutexLockUntilTimeStamp      12
#define TEST_ThsyncMutexLockForTimePeriod       13
#define TEST_ThsyncMutexTryLock                 14
#define TEST_ThsyncMutexUnlock                  15
#define TEST_ThsyncCondCreate                   16
#define TEST_ThsyncCondDestroy                  17
#define TEST_ThsyncCondWaitUntilTimeStamp       18
#define TEST_ThsyncCondWaitForTimePeriod        19
#define TEST_ThsyncCondBroadcast                20
#define TEST_ThsyncCondSignal                   21
#define TEST_ThsyncMemoryBarrier                22
#define TEST_MqCreate                           23
#define TEST_MqDelete                           24
#define TEST_MqSendUntilTimeStamp               25
#define TEST_MqSendForTimePeriod                26
#define TEST_MqReceiveUntilTimeStamp            27
#define TEST_MqReceiveForTimePeriod             28
#define TEST_MqIsFull                           29
#define TEST_MqIsEmpty                          30
#define TEST_MqGetConfig                        31
#define TEST_MqReset                            32
#define TEST_MqInitializeMqConfigSt             33
#define TEST_ClockTimeGetTimeStamp              34
#define TEST_ClockTimeCalculateTimeDifference   35
#define TEST_InterruptGetNumOfIrqChannels       36
#define TEST_InterruptRegisterIsr               37
#define TEST_InterruptUnregisterIsr             38
#define TEST_InterruptEnableIsr                 39
#define TEST_InterruptDisableIsr                40
#define TEST_InterruptIsISRContext              41
#define TEST_InterruptSetIsrPriority            42
#define TEST_IoGetNumOfDevices                  43
#define TEST_IoGetDeviceList                    44
#define TEST_IoDeviceOpen                       45
#define TEST_IoDeviceClose                      46
#define TEST_IoRead8                            47
#define TEST_IoRead16                           48
#define TEST_IoRead32                           49
#define TEST_IoRead64                           50
#define TEST_IoWrite8                           51
#define TEST_IoWrite16                          52
#define TEST_IoWrite32                          53
#define TEST_IoWrite64                          54
#define TEST_IoBlockRead32                      55
#define TEST_IoBlockRead64                      56
#define TEST_IoBlockWrite32                     57
#define TEST_IoBlockWrite64                     58
#define TEST_IoGetAxiBusIdFromDeviceName        59
#define TEST_IoGetAxiBusName                    60
#define TEST_IoGetAxiBusNameList                61
#define TEST_IoGetDeviceAxiBusId                62
#define TEST_IoGetNumOfAxiIdsForAxiBusName      63
#define TEST_IoGetAxiIdListForAxiBusName        64
#define TEST_IoGetNumOfAxiIdsForDeviceHdl       65
#define TEST_IoGetAxiIdListForDeviceHdl         66
#define TEST_IoGetNumOfAxiIdsForDeviceName      67
#define TEST_IoGetAxiIdListForDeviceName        68
#define TEST_PmSetRequiredState                 69
#define TEST_PmGetRequiredState                 70
#define TEST_PmWaitForState                     71
#define TEST_PmGetState                         72
#define TEST_PmSetPolicy                        73
#define TEST_PmGetPolicy                        74
#define TEST_PmSetResetState                    75
#define TEST_PmGetResetState                    76
#define TEST_PmGetLowestIntermediatePowerState  77
#define TEST_PmSetPostClock                     78
#define TEST_MmngrOpen                          79
#define TEST_MmngrClose                         80
#define TEST_MmngrAlloc                         81
#define TEST_MmngrDealloc                       82
#define TEST_MmngrGetConfig                     83
#define TEST_MmngrGetOsalMaxConfig              84
#define TEST_MmngrInitializeMmngrConfigSt       85
#define TEST_MmngrRegisterMonitor               86
#define TEST_MmngrGetCpuPtr                     87
#define TEST_MmngrGetConstCpuPtr                88
#define TEST_MmngrFlush                         89
#define TEST_MmngrInvalidate                    90
#define TEST_MmngrGetHwAddr                     91
#define TEST_MmngrGetSize                       92
#define TEST_MmngrCheckBuffer                   93
#define TEST_MmngrDebugMmngrPrint               94
#define TEST_MmngrDebugBufferPrint              95
#define TEST_MmngrInitializeMemoryBufferObj     96
#define TEST_MmngrInitializeMemoryManagerObj    97
#define TEST_MmngrGetOsalMaxConfigFromRegionIdx 98
#define TEST_MmngrOpenFromRegionIdx             99

/*** global variable ***/
static osal_thread_handle_t     th_handle;
static osal_thread_handle_t     th_handle2;
static int                      g_count = 0;
static unsigned int             test_pass;
static unsigned int             test_fail;
static unsigned int             test_skip;

/*** prototype ***/
static int64_t osal_all_test (void *user_arg);
static int Test_ThreadManager (int test_count);
static int Test_ThreadSyncManager (int test_count);
static int Test_MessageManager (int test_count);
static int Test_ClockTimeManager (int test_count);
static int Test_InterruptManager (int test_count);
static int Test_IOManager (int test_count);
static int Test_IOAxiBusName (int test_count);
static int Test_PowerManager (int test_count);
static int Test_MemoryManager (int test_count);
static int CheckVersion (int test_count);

/* Test_ThreadManager */
static int64_t osal_test_thread (void *user_arg);
/* Test_ThreadSyncManager */
static int Test_ThreadSyncManager_Mutex (int test_count);
static int Test_ThreadSyncManager_Cond (int test_count);

typedef struct st_arg_mq_send_timeperiod
{
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
    void *p_buffer;
    size_t buffer_size;
} st_arg_mq_send_timeperiod_t;

typedef struct st_arg_mq_receive_timeperiod
{
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
} st_arg_mq_receive_timeperiod_t;

/*** Common Utils ***/
/* GetAbsoluteTime */
static void GetAbsoluteTime(st_osal_time_t* iop_absolute_time);

typedef struct st_arg_cond
{
    osal_milli_sec_t time;
    st_osal_time_t abs_time;
    osal_cond_handle_t cond_handle;
    osal_mutex_handle_t mutex_handle;
} st_arg_cond_t;

typedef struct st_osal_axi_bus_name_test_data
{
    char *test_bus_name;
    size_t axi_ids;
} st_osal_axi_bus_name_test_data_t;

typedef struct st_osal_device_info_axi_bus_test_data
{
    char *test_device_name;
    osal_device_handle_t test_hndl;
    size_t axi_ids;
} st_osal_device_info_axi_bus_test_data_t;

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
    "R_OSAL_IoGetNumOfAxiIdsForAxiBusName   : \0",
    "R_OSAL_IoGetAxiIdListForAxiBusName    : \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceHdl    : \0",
    "R_OSAL_IoGetAxiIdListForDeviceHdl    : \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceName   : \0",
    "R_OSAL_IoGetAxiIdListForDeviceName   : \0",
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
    "R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx   : \0",
    "R_OSAL_MmngrOpenFromRegionIdx   : \0",
};

uint64_t expected_time[TEST_NUM] = {
    500000 * USEC_TO_NSEC,      /* R_OSAL_Initialize */
    40000 * USEC_TO_NSEC,       /* R_OSAL_Deinitialize */
    10 * USEC_TO_NSEC,          /* R_OSAL_GetVersion */
    5000 * USEC_TO_NSEC,        /* R_OSAL_ThreadCreate */
    12000 * USEC_TO_NSEC,       /* R_OSAL_ThreadSleepUntilTimeStamp */
    12000 * USEC_TO_NSEC,       /* R_OSAL_ThreadSleepForTimePeriod */
    40 * USEC_TO_NSEC,          /* R_OSAL_ThreadSelf */
    30 * USEC_TO_NSEC,          /* R_OSAL_ThreadEqual */
    10 * USEC_TO_NSEC,          /* R_OSAL_ThreadInitializeThreadConfigSt */
    100 * USEC_TO_NSEC,         /* R_OSAL_ThreadJoin */
    140 * USEC_TO_NSEC,         /* R_OSAL_ThsyncMutexCreate */
    70 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexDestroy */
    50 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockUntilTimeStamp */
    50 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexLockForTimePeriod */
    40 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexTryLock */
    40 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMutexUnlock */
    90 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondCreate */
    70 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondDestroy */
    12000 * USEC_TO_NSEC,       /* R_OSAL_ThsyncCondWaitUntilTimeStamp */
    12000 * USEC_TO_NSEC,       /* R_OSAL_ThsyncCondWaitForTimePeriod */
    40 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondBroadcast */
    40 * USEC_TO_NSEC,          /* R_OSAL_ThsyncCondSignal */
    20 * USEC_TO_NSEC,          /* R_OSAL_ThsyncMemoryBarrier */
    180 * USEC_TO_NSEC,         /* R_OSAL_MqCreate */
    130 * USEC_TO_NSEC,         /* R_OSAL_MqDelete */
    100 * USEC_TO_NSEC,         /* R_OSAL_MqSendUntilTimeStamp */
    100 * USEC_TO_NSEC,         /* R_OSAL_MqSendForTimePeriod */
    100 * USEC_TO_NSEC,         /* R_OSAL_MqReceiveUntilTimeStamp */
    100 * USEC_TO_NSEC,         /* R_OSAL_MqReceiveForTimePeriod */
    50 * USEC_TO_NSEC,          /* R_OSAL_MqIsFull */
    50 * USEC_TO_NSEC,          /* R_OSAL_MqIsEmpty */
    30 * USEC_TO_NSEC,          /* R_OSAL_MqGetConfig */
    70 * USEC_TO_NSEC,          /* R_OSAL_MqReset */
    10 * USEC_TO_NSEC,          /* R_OSAL_MqInitializeMqConfigSt */
    10 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeGetTimeStamp */
    10 * USEC_TO_NSEC,          /* R_OSAL_ClockTimeCalculateTimeDifference */
    30 * USEC_TO_NSEC,          /* R_OSAL_InterruptGetNumOfIrqChannels */
    5000 * USEC_TO_NSEC,        /* R_OSAL_InterruptRegisterIsr */
    5000 * USEC_TO_NSEC,        /* R_OSAL_InterruptUnregisterIsr */
    80 * USEC_TO_NSEC,          /* R_OSAL_InterruptEnableIsr */
    50 * USEC_TO_NSEC,          /* R_OSAL_InterruptDisableIsr */
    20 * USEC_TO_NSEC,          /* R_OSAL_InterruptIsISRContext */
    40 * USEC_TO_NSEC,          /* R_OSAL_InterruptSetIsrPriority */
    5000 * USEC_TO_NSEC,        /* R_OSAL_IoGetNumOfDevices */
    5000 * USEC_TO_NSEC,        /* R_OSAL_IoGetDeviceList */
    5000 * USEC_TO_NSEC,        /* R_OSAL_IoDeviceOpen */
    380 * USEC_TO_NSEC,         /* R_OSAL_IoDeviceClose */
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
    190 * USEC_TO_NSEC,         /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    50 * USEC_TO_NSEC,          /* R_OSAL_IoGetAxiBusName */
    40 * USEC_TO_NSEC,          /* R_OSAL_IoGetAxiBusNameList */
    190 * USEC_TO_NSEC,         /* R_OSAL_IoGetDeviceAxiBusId */
    330 * USEC_TO_NSEC,         /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    370 * USEC_TO_NSEC,         /* R_OSAL_IoGetAxiIdListForAxiBusName */
    160 * USEC_TO_NSEC,         /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
    200 * USEC_TO_NSEC,         /* R_OSAL_IoGetAxiIdListForDeviceHdl */
    140 * USEC_TO_NSEC,         /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    180 * USEC_TO_NSEC,         /* R_OSAL_IoGetAxiIdListForDeviceName */
    100 * USEC_TO_NSEC,         /* R_OSAL_PmSetRequiredState */
    40 * USEC_TO_NSEC,          /* R_OSAL_PmGetRequiredState */
    70 * USEC_TO_NSEC,          /* R_OSAL_PmWaitForState */
    50 * USEC_TO_NSEC,          /* R_OSAL_PmGetState */
    100 * USEC_TO_NSEC,         /* R_OSAL_PmSetPolicy */
    40 * USEC_TO_NSEC,          /* R_OSAL_PmGetPolicy */
    60 * USEC_TO_NSEC,          /* R_OSAL_PmSetResetState */
    50 * USEC_TO_NSEC,          /* R_OSAL_PmGetResetState */
    40 * USEC_TO_NSEC,          /* R_OSAL_PmGetLowestIntermediatePowerState */
    110 * USEC_TO_NSEC,         /* R_OSAL_PmSetPostClock */
    200000 * USEC_TO_NSEC,      /* R_OSAL_MmngrOpen */
    70000 * USEC_TO_NSEC,       /* R_OSAL_MmngrClose */
    5000 * USEC_TO_NSEC,        /* R_OSAL_MmngrAlloc */
    40 * USEC_TO_NSEC,          /* R_OSAL_MmngrDealloc */
    30 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConfig */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfig */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMngrConfigSt */
    30 * USEC_TO_NSEC,          /* R_OSAL_MmngrRegisterMonitor */
    30 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetCpuPtr */
    30 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetConstCpuPtr */
    40 * USEC_TO_NSEC,          /* R_OSAL_MmngrFlush */
    40 * USEC_TO_NSEC,          /* R_OSAL_MmngrInvalidate */
    40 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetHwAddr */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetSize */
    120 * USEC_TO_NSEC,         /* R_OSAL_MmngrCheckBuffer */
    90 * USEC_TO_NSEC,          /* R_OSAL_MmngrDebugMmngrPrint */
    40 * USEC_TO_NSEC,          /* R_OSAL_MmngrDebugBufferPrint */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryBufferObj */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrInitializeMemoryManagerObj */
    10 * USEC_TO_NSEC,          /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
    200000 * USEC_TO_NSEC,      /* R_OSAL_MmngrOpenFromRegionIdx */
};

int main()
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    st_osal_time_t          current_time;
    st_osal_time_t          current_time_1st, current_time_2nd;
    osal_nano_sec_t         exec_time;
    uint64_t                diff_time;
    int                     ret = 0;

    struct timespec         abstime_before;
    struct timespec         abstime_after;
    uint64_t                t;

    st_osal_thread_config_t config;
    int64_t                 arg;
    int64_t                 return_value;
    osal_thread_handle_t    osal_handle;

    test_pass = 0;
    test_fail = 0;
    test_skip = 0;

#define TIME_BEFORE     clock_gettime(CLOCK_REALTIME, &abstime_before);
#define TIME_AFTER      clock_gettime(CLOCK_REALTIME, &abstime_after);

    for (int i = 0; i < TEST_NUM; i++)
    {
        switch (i)
        {
            case TEST_ThreadSleepUntilTimeStamp :
            case TEST_ThreadSleepForTimePeriod :
                continue;

            default :
                break;
        }

#if ENABLE_V3U_SETTING
        // V3U : CPU clock:  1.8GHz
        expected_time[i] = expected_time[i] / 1.8;
#elif ENABLE_V3M2_SETTING
        // V3M : CPU clock: 800MHz
        expected_time[i] = expected_time[i] / 0.8;
#elif ENABLE_V4H_SETTING
        // V4H : CPU clock: 1.7GHz
        expected_time[i] = expected_time[i] / 1.7;
#elif ENABLE_V4M_SETTING
        // V4M : CPU clock: 1.0GHz
        expected_time[i] = expected_time[i] / 1.0;
#elif ENABLE_S4_SETTING
        // S4  : CPU clock: 1.2GHz
        expected_time[i] = expected_time[i] / 1.2;
#else
        // V3Hx: CPU clock: 1.0GHz
#endif
    }

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    for (g_count = 0; g_count < TEST_COUNT; g_count++)
    {
        OSAL_TEST_INFO("*** Start: R_OSAL_Initialize ***\n");
        TIME_BEFORE
        osal_ret = R_OSAL_Initialize();
        TIME_AFTER
        t = (abstime_after.tv_sec * SEC_TO_NSEC + abstime_after.tv_nsec) - (abstime_before.tv_sec * SEC_TO_NSEC + abstime_before.tv_nsec);
        SET_TIME("R_OSAL_Initialize", TEST_Initialize, g_count, osal_ret, t);

        arg = g_count;
        config.func       = osal_all_test;
        config.userarg    = (void *)&arg;
        config.priority   = OSAL_THREAD_PRIORITY_HIGHEST - 1;
        config.task_name  = (char *)"Execution of Each Manager Test";
        config.stack_size = 2 * 1024 * 1024;

        osal_ret = R_OSAL_ThreadCreate(&config, THREAD_MAIN_TASK_ID_ALL, &osal_handle);
        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadCreate is failed, osal_ret = %d\n", osal_ret);
            ret = -1;
            goto END;
        }

        osal_ret = R_OSAL_ThreadJoin(osal_handle, &return_value);
        if (osal_ret != OSAL_RETURN_OK || 0 != return_value)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadJoin is failed, osal_ret = %d, return_value = %d\n", osal_ret, return_value);
            ret = -1;
            goto END;
        }

        OSAL_TEST_INFO("*** Start: R_OSAL_Deinitialize ***\n");
        TIME_BEFORE
        osal_ret = R_OSAL_Deinitialize();
        TIME_AFTER
        t = (abstime_after.tv_sec * SEC_TO_NSEC + abstime_after.tv_nsec) - (abstime_before.tv_sec * SEC_TO_NSEC + abstime_before.tv_nsec);
        SET_TIME("R_OSAL_Deinitialize", TEST_Deinitialize, g_count, osal_ret, t);
    }

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

        if (expected_time[i] < elapsed_time_avg[i])
        {
            OSAL_TEST_INFO("%s has exceeded the expected execution time. \n", api_list[i]);
        }
    }

    test_pass /= TEST_COUNT;
    test_fail /= TEST_COUNT;

    // The following unsupported API does not run the test
    // R_OSAL_IoBlockRead8/R_OSAL_IoBlockRead16/R_OSAL_IoBlockWrite8/R_OSAL_IoBlockWrite16
    test_pass += 4;

END :
    printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);
    return ret;
}

/***********************************************************************************************************************
* Start of function osal_all_test()
***********************************************************************************************************************/
static int64_t osal_all_test (void *user_arg)
{
    int64_t ret;
    bool    result;
    (void)user_arg;

    /* Check Execution time of GetVersion */
    ret = CheckVersion(g_count);

    if (ret == 0)
    {
        /* Check Execution time of Clock & Time Manager */
        ret = Test_ClockTimeManager(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of Thread Manager */
        ret = Test_ThreadManager(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of ThreadSync Manager */
        ret = Test_ThreadSyncManager(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of Message Manager */
        ret = Test_MessageManager(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of IO Manager */
        ret = Test_IOManager(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of APIs related to Axi Bus */
        ret = Test_IOAxiBusName(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of Interrupt Manager */
        ret = Test_InterruptManager(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of Power Manager */
        ret = Test_PowerManager(g_count);
    }

    if (ret == 0)
    {
        /* Check Execution time of Memory Manager */
        ret = Test_MemoryManager(g_count);
    }

    if (ret != 0)
    {
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function osal_all_test()
***********************************************************************************************************************/


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
    SET_TIME("R_OSAL_ThreadInitializeThreadConfigSt", TEST_ThreadInitializeThreadConfigSt, test_count, osal_ret, exec_time);

    config.func       = osal_test_thread;
    config.userarg    = (void *)&arg;
    config.priority   = OSAL_THREAD_PRIORITY_HIGHEST;
    config.task_name  = (char *)"Execution time test thread";
    config.stack_size = 2 * 1024 * 1024;

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadCreate ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadCreate(&config, THREAD_MAIN_TASK_ID, &th_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThreadCreate", TEST_ThreadCreate, test_count, osal_ret, exec_time);

    usleep(10 * 1000 * 1000);

    th_handle2 = th_handle;
    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadEqual ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadEqual(th_handle, th_handle2, &result);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThreadEqual", TEST_ThreadEqual, test_count, osal_ret, exec_time);

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadJoin ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadJoin(th_handle, &return_value);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThreadJoin", TEST_ThreadJoin, test_count, osal_ret, exec_time);
    if (0 != return_value)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadJoin is failed, osal_ret = %d, return value = %d\n", osal_ret, return_value);
        return -1;
    }

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
    SET_TIME("R_OSAL_ThreadSleepUntilTimeStamp", TEST_ThreadSleepUntilTimeStamp, test_count, osal_ret, exec_time);

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadSleepForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(WAIT_TIME_MS);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThreadSleepForTimePeriod", TEST_ThreadSleepForTimePeriod, test_count, osal_ret, exec_time);

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

    OSAL_TEST_INFO("*** Start: R_OSAL_ThreadSelf ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThreadSelf(&self_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThreadSelf", TEST_ThreadSelf, g_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncMemoryBarrier", TEST_ThsyncMemoryBarrier, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncMutexCreate", TEST_ThsyncMutexCreate, test_count, osal_ret, exec_time);

    /* R_OSAL_ThsyncMutexLockForTimePeriod */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexLockForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, WAIT_TIME_MS);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThsyncMutexLockForTimePeriod", TEST_ThsyncMutexLockForTimePeriod, test_count, osal_ret, exec_time);

    /* R_OSAL_ThsyncMutexUnlock */    /* requires R_OSAL_ThsyncMutexTryLock */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexUnlock ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThsyncMutexUnlock", TEST_ThsyncMutexUnlock, test_count, osal_ret, exec_time);

    /* R_OSAL_ThsyncMutexLockUntilTimeStamp */    /* requires R_OSAL_ThsyncMutexCreate */
    GetAbsoluteTime(&absolute_time);

    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexLockUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThsyncMutexLockUntilTimeStamp", TEST_ThsyncMutexLockUntilTimeStamp, test_count, osal_ret, exec_time);

    R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexTryLock */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexTryLock ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThsyncMutexTryLock", TEST_ThsyncMutexTryLock, test_count, osal_ret, exec_time);

    /* R_OSAL_ThsyncMutexUnlock */    /* requires R_OSAL_ThsyncMutexTryLock */
    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexDestroy */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_ThsyncMutexDestroy ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ThsyncMutexDestroy", TEST_ThsyncMutexDestroy, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncCondWaitUntilTimeStamp", TEST_ThsyncCondWaitUntilTimeStamp, g_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncCondWaitForTimePeriod", TEST_ThsyncCondWaitForTimePeriod, g_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncCondCreate", TEST_ThsyncCondCreate, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncCondSignal", TEST_ThsyncCondSignal, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncCondBroadcast", TEST_ThsyncCondBroadcast, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ThsyncCondDestroy", TEST_ThsyncCondDestroy, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_MqInitializeMqConfigSt", TEST_MqInitializeMqConfigSt, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_MqCreate", TEST_MqCreate, test_count, osal_ret, exec_time);

    /* R_OSAL_MqGetConfig */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqGetConfig ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqGetConfig(mq_handle, &mq_config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqGetConfig", TEST_MqGetConfig, test_count, osal_ret, exec_time);

    /* R_OSAL_MqSendUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);

    OSAL_TEST_INFO("*** Start: R_OSAL_MqSendUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqSendUntilTimeStamp", TEST_MqSendUntilTimeStamp, test_count, osal_ret, exec_time);

    /* R_OSAL_MqReceiveUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);

    OSAL_TEST_INFO("*** Start: R_OSAL_MqReceiveUntilTimeStamp ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqReceiveUntilTimeStamp", TEST_MqReceiveUntilTimeStamp, test_count, osal_ret, exec_time);

    time_period = WAIT_TIME_MS;
    OSAL_TEST_INFO("*** Start: R_OSAL_MqSendForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, time_period, (void *)&msg, sizeof(msg));
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqSendForTimePeriod", TEST_MqSendForTimePeriod, test_count, osal_ret, exec_time);

    time_period = WAIT_TIME_MS;
    OSAL_TEST_INFO("*** Start: R_OSAL_MqReceiveForTimePeriod ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, (void *)rcv_data, sizeof(msg));
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqReceiveForTimePeriod", TEST_MqReceiveForTimePeriod, test_count, osal_ret, exec_time);

    /* R_OSAL_MqIsFull */    /* requires R_OSAL_MqCreate */
    is_full = false;
    OSAL_TEST_INFO("*** Start: R_OSAL_MqIsFull ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqIsFull(mq_handle, &is_full);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqIsFull", TEST_MqIsFull, test_count, osal_ret, exec_time);

    /* R_OSAL_MqIsEmpty */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqIsEmpty ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqIsEmpty(mq_handle, &mq_result);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqIsEmpty", TEST_MqIsEmpty, test_count, osal_ret, exec_time);

    /* R_OSAL_MqReset */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqReset ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqReset(mq_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqReset", TEST_MqReset, test_count, osal_ret, exec_time);

    /*R_OSAL_MqDelete */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** Start: R_OSAL_MqDelete ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MqDelete(mq_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MqDelete", TEST_MqDelete, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_ClockTimeGetTimeStamp", TEST_ClockTimeGetTimeStamp, test_count, osal_ret, exec_time);

    OSAL_TEST_INFO("*** Start: R_OSAL_ClockTimeCalculateTimeDifference ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_ClockTimeCalculateTimeDifference", TEST_ClockTimeCalculateTimeDifference, test_count, osal_ret, exec_time);

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
#if defined (ENABLE_V3U_SETTING)
    const char              device_type[] = "imp";
#elif defined (ENABLE_S4_SETTING)
    const char              device_type[] = "caiplite";
#else /* V3Hx, V3M2, V4H */
    const char              device_type[] = "ims";
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
    SET_TIME("R_OSAL_InterruptIsISRContext", TEST_InterruptIsISRContext, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_InterruptGetNumOfIrqChannels", TEST_InterruptGetNumOfIrqChannels, test_count, osal_ret, exec_time);

    /* R_OSAL_InterruptRegisterIsr */    /* requires R_OSAL_InterruptGetNumOfIrqChannels */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptRegisterIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                               (p_osal_isr_func_t)osal_test_cb, (void *)&arg);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_InterruptRegisterIsr", TEST_InterruptRegisterIsr, test_count, osal_ret, exec_time);

    /* R_OSAL_InterruptEnableIsr */        /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptEnableIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_InterruptEnableIsr", TEST_InterruptEnableIsr, test_count, osal_ret, exec_time);

    /* R_OSAL_InterruptDisableIsr */    /* requires R_OSAL_InterruptEnableIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptDisableIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_InterruptDisableIsr", TEST_InterruptDisableIsr, test_count, osal_ret, exec_time);

    /* R_OSAL_InterruptSetIsrPriority */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptSetIsrPriority ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_InterruptSetIsrPriority", TEST_InterruptSetIsrPriority, test_count, osal_ret, exec_time);

    /* R_OSAL_InterruptUnregisterIsr */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** Start: R_OSAL_InterruptUnregisterIsr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_InterruptUnregisterIsr", TEST_InterruptUnregisterIsr, test_count, osal_ret, exec_time);

    (void)R_OSAL_IoDeviceClose(dev_handle);

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
    size_t                  list_size = 0;
    size_t                  numOfLists = 0;

    osal_device_handle_t    handle = OSAL_DEVICE_HANDLE_INVALID;
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

    char                    ax_list[200];
    size_t                  ax_list_size;
    size_t                  ax_numOfLists;

    st_osal_time_t          current_time_1st, current_time_2nd;
    osal_nano_sec_t         exec_time;

#if defined (ENABLE_V3U_SETTING)
    const char              device_type[] = "imp";
#elif defined (ENABLE_S4_SETTING)
    const char              device_type[] = "caiplite";
#else /* V3Hx, V3M2, V4H */
    const char              device_type[] = "ims";
#endif

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* R_OSAL_IoGetNumOfDevices */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfDevices ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetNumOfDevices", TEST_IoGetNumOfDevices, test_count, osal_ret, exec_time);

    /* R_OSAL_IoGetDeviceList */    /* requires R_OSAL_IoGetNumOfDevices */
    osal_ret = R_OSAL_IoGetDeviceList(device_type, NULL, 0, &list_size);
    if (osal_ret != OSAL_RETURN_OK_RQST)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetDeviceList is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetDeviceList ***\n");
    char list[list_size];
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetDeviceList", TEST_IoGetDeviceList, test_count, osal_ret, exec_time);

    device = strtok(list, s);
    /* R_OSAL_IoDeviceOpen */    /* requires R_OSAL_IoGetDeviceList */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoDeviceOpen ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoDeviceOpen(device, &handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoDeviceOpen", TEST_IoDeviceOpen, test_count, osal_ret, exec_time);

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
    SET_TIME("R_OSAL_IoWrite8", TEST_IoWrite8, test_count, osal_ret, exec_time);

    /* R_OSAL_IoRead8 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead8 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead8(handle, offset, &read_data8);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoRead8", TEST_IoRead8, test_count, osal_ret, exec_time);

    /* R_OSAL_IoWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoWrite16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoWrite16(handle, offset, 0xff);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoWrite16", TEST_IoWrite16, test_count, osal_ret, exec_time);

    /* R_OSAL_IoRead16 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead16(handle, offset, &read_data16);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoRead16", TEST_IoRead16, test_count, osal_ret, exec_time);

    /* R_OSAL_IoWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoWrite32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoWrite32(handle, offset, 0xff);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoWrite32", TEST_IoWrite32, test_count, osal_ret, exec_time);

    /* R_OSAL_IoRead32 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead32(handle, offset, &read_data32);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoRead32", TEST_IoRead32, test_count, osal_ret, exec_time);

    /* R_OSAL_IoWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoWrite64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoWrite64(handle, offset, 0xff);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoWrite64", TEST_IoWrite64, test_count, osal_ret, exec_time);

    /* R_OSAL_IoRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoRead64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoRead64(handle, offset, &read_data64);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoRead64", TEST_IoRead64, test_count, osal_ret, exec_time);

#if 0 /* Access to registers in R-Car is only supported in 32-bit units.  */
    /* R_OSAL_IoBlockWrite8 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite8 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite8(handle, offset, register_data8, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockWrite8", TEST_IoBlockWrite8, test_count, osal_ret, exec_time);

    /* R_OSAL_IoBlockRead8 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data8, 0x00, sizeof(read_block_data8));
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead8 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_block_data8, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockRead8", TEST_IoBlockRead8, test_count, osal_ret, exec_time);

    /* R_OSAL_IoBlockWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite16(handle, offset, register_data16, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockWrite16", TEST_IoBlockWrite16, test_count, osal_ret, exec_time);

    /* R_OSAL_IoBlockRead16 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data16, 0x00, sizeof(read_block_data16));
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead16 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_block_data16, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockRead16", TEST_IoBlockRead16, test_count, osal_ret, exec_time);
#endif

    /* R_OSAL_IoBlockWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite32(handle, offset, register_data32, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockWrite32", TEST_IoBlockWrite32, test_count, osal_ret, exec_time);

    /* R_OSAL_IoBlockRead32 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data32, 0x00, sizeof(read_block_data32));
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead32 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_block_data32, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockRead32", TEST_IoBlockRead32, test_count, osal_ret, exec_time);

    /* R_OSAL_IoBlockWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    size = 16;
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockWrite64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockWrite64(handle, offset, register_data64, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockWrite64", TEST_IoBlockWrite64, test_count, osal_ret, exec_time);

    /* R_OSAL_IoBlockRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoBlockRead64 ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_block_data64, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoBlockRead64", TEST_IoBlockRead64, test_count, osal_ret, exec_time);

    /* R_OSAL_IoGetDeviceAxiBusId */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetDeviceAxiBusId ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetDeviceAxiBusId (handle, &axi_id);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetDeviceAxiBusId", TEST_IoGetDeviceAxiBusId, test_count, osal_ret, exec_time);

    osal_ret = R_OSAL_PmSetRequiredState(handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetRequiredState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoDeviceClose */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoDeviceClose ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoDeviceClose(handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoDeviceClose", TEST_IoDeviceClose, test_count, osal_ret, exec_time);

    /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiBusIdFromDeviceName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName(device, &axi_id);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetAxiBusIdFromDeviceName", TEST_IoGetAxiBusIdFromDeviceName, test_count, osal_ret, exec_time);

    /* R_OSAL_IoGetAxiBusName */
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiBusName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, axi_name, sizeof(axi_name), &numOfName);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetAxiBusName", TEST_IoGetAxiBusName, test_count, osal_ret, exec_time);

    /* R_OSAL_IoGetAxiBusNameList */
    ax_list_size = sizeof(ax_list);
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiBusNameList ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiBusNameList(ax_list, ax_list_size, &ax_numOfLists);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetAxiBusNameList", TEST_IoGetAxiBusNameList, test_count, osal_ret, exec_time);

    return 0;
}
/***********************************************************************************************************************
* End of function Test_IOManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_PowerManager()
***********************************************************************************************************************/
static int Test_IOAxiBusName (int test_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    int                     ret;

    st_osal_time_t          current_time_1st, current_time_2nd;
    osal_nano_sec_t         exec_time;

    osal_device_handle_t    ims_device_hndl = OSAL_DEVICE_HANDLE_INVALID;

    st_osal_axi_bus_name_test_data_t        axi_bus_name_test_data;
    st_osal_device_info_axi_bus_test_data_t device_info_test_data;

    size_t                  num_of_axi_ids = 0;
    osal_axi_bus_id_t       *buffer;
    size_t                  num_of_max_entry = 0;
    size_t                  num_of_written_axi_ids = 0;
    
#if defined (ENABLE_V3U_SETTING)
    axi_bus_name_test_data.test_bus_name = "vi0_10-11_14-15";
    axi_bus_name_test_data.axi_ids = 4;
#elif defined (ENABLE_V3M2_SETTING)
    axi_bus_name_test_data.test_bus_name = "vi0_02-04_06-07";
    axi_bus_name_test_data.axi_ids = 5;
#elif defined (ENABLE_V4H_SETTING)
    axi_bus_name_test_data.test_bus_name = "vc_08";
    axi_bus_name_test_data.axi_ids = 1;
#elif defined (ENABLE_V4M_SETTING)
    axi_bus_name_test_data.test_bus_name = "vc_08";
    axi_bus_name_test_data.axi_ids = 1;
#elif defined (ENABLE_S4_SETTING)
    axi_bus_name_test_data.test_bus_name = "rt1_00-15";
    axi_bus_name_test_data.axi_ids = 16;
#else /* V3Hx */
    axi_bus_name_test_data.test_bus_name = "vc0_00-06";
    axi_bus_name_test_data.axi_ids = 7;
#endif

#if defined (ENABLE_V3U_SETTING)
    device_info_test_data.test_device_name = "imp_00";
    device_info_test_data.axi_ids = 8;
#elif defined (ENABLE_V4H_SETTING)
    device_info_test_data.test_device_name = "imp_00";
    device_info_test_data.axi_ids = 8;
#elif defined (ENABLE_V4M_SETTING)
    device_info_test_data.test_device_name = "imp_00";
    device_info_test_data.axi_ids = 8;
#elif defined (ENABLE_S4_SETTING)
    device_info_test_data.test_device_name = "caiplite_00";
    device_info_test_data.axi_ids = 1;
#else /* V3Hx, V3M2 */
    device_info_test_data.test_device_name = "ims_00";
    device_info_test_data.axi_ids = 1;
#endif
    device_info_test_data.test_hndl = ims_device_hndl;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    osal_ret = R_OSAL_IoDeviceOpen(device_info_test_data.test_device_name, &device_info_test_data.test_hndl);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceOpen is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfAxiIdsForAxiBusName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data.test_bus_name, &num_of_axi_ids);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetNumOfAxiIdsForAxiBusName", TEST_IoGetNumOfAxiIdsForAxiBusName, test_count, osal_ret, exec_time);

    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfAxiIdsForDeviceHdl ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data.test_hndl, &num_of_axi_ids);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetNumOfAxiIdsForDeviceHdl", TEST_IoGetNumOfAxiIdsForDeviceHdl, test_count, osal_ret, exec_time);

    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetNumOfAxiIdsForDeviceName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data.test_device_name, &num_of_axi_ids);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetNumOfAxiIdsForDeviceName", TEST_IoGetNumOfAxiIdsForDeviceName, test_count, osal_ret, exec_time);

    /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName is required R_OSAL_IoGetAxiIdListForAxiBusName */
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data.test_bus_name, &num_of_max_entry);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForAxiBusName is failed, osal_ret = %d\n", osal_ret);
    }

    buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiIdListForAxiBusName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data.test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetAxiIdListForAxiBusName", TEST_IoGetAxiIdListForAxiBusName, test_count, osal_ret, exec_time);

    /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl is required R_OSAL_IoGetAxiIdListForDeviceHdl */
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data.test_hndl, &num_of_max_entry);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForDeviceHdl is failed, osal_ret = %d\n", osal_ret);
    }
    free(buffer);

    buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiIdListForDeviceHdl ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data.test_hndl, buffer, num_of_max_entry, &num_of_written_axi_ids);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetAxiIdListForDeviceHdl", TEST_IoGetAxiIdListForDeviceHdl, test_count, osal_ret, exec_time);
    free(buffer);

    /* R_OSAL_IoGetNumOfAxiIdsForDeviceName is required R_OSAL_IoGetAxiIdListForDeviceName */
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data.test_device_name, &num_of_max_entry);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForDeviceName is failed, osal_ret = %d\n", osal_ret);
    }

    buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);
    OSAL_TEST_INFO("*** Start: R_OSAL_IoGetAxiIdListForDeviceName ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data.test_device_name, buffer, num_of_max_entry, &num_of_written_axi_ids);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_IoGetAxiIdListForDeviceName", TEST_IoGetAxiIdListForDeviceName, test_count, osal_ret, exec_time);
    free(buffer);

    osal_ret = R_OSAL_IoDeviceClose(device_info_test_data.test_hndl);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceClose is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_IOAxiBusName()
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

#if defined (ENABLE_S4_SETTING)
    const char              device_type[] = "caiplite";
#else /* V3Hx, V3M2 */
    const char              device_type[] = "imp";
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
    SET_TIME("R_OSAL_PmSetRequiredState", TEST_PmSetRequiredState, test_count, osal_ret, exec_time);

    /* R_OSAL_PmSetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmSetResetState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmSetResetState (dev_handle, OSAL_PM_RESET_STATE_APPLIED);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmSetResetState", TEST_PmSetResetState, test_count, osal_ret, exec_time);

    /* R_OSAL_PmGetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetState (dev_handle, &pm_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmGetState", TEST_PmGetState, test_count, osal_ret, exec_time);

    /* R_OSAL_PmWaitForState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmWaitForState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmWaitForState (dev_handle, OSAL_PM_STATE_RESET, 1000);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmWaitForState", TEST_PmWaitForState, test_count, osal_ret, exec_time);

    /* R_OSAL_PmSetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmSetPolicy ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmSetPolicy (dev_handle, OSAL_PM_POLICY_PG, true);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmSetPolicy", TEST_PmSetPolicy, test_count, osal_ret, exec_time);

    /* R_OSAL_PmGetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetPolicy ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetPolicy (dev_handle, &policy);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmGetPolicy", TEST_PmGetPolicy, test_count, osal_ret, exec_time);

    /* R_OSAL_PmGetRequiredState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetRequiredState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetRequiredState (dev_handle, &req_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmGetRequiredState", TEST_PmGetRequiredState, test_count, osal_ret, exec_time);

    /* R_OSAL_PmGetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetResetState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetResetState (dev_handle, &reset_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmGetResetState", TEST_PmGetResetState, test_count, osal_ret, exec_time);

    /* R_OSAL_PmGetLowestIntermediatePowerState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_PmGetLowestIntermediatePowerState ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmGetLowestIntermediatePowerState (dev_handle, &pm_state);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmGetLowestIntermediatePowerState", TEST_PmGetLowestIntermediatePowerState, test_count, osal_ret, exec_time);

    (void)R_OSAL_IoDeviceClose(dev_handle);

    /* R_OSAL_PmSetPostClock */    /* requires R_OSAL_IoDeviceOpen */
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    R_OSAL_IoGetNumOfDevices("fba", &numOfDevices);
    R_OSAL_IoGetDeviceList("fba", list, list_size, &numOfLists);
    device = strtok(list, s);
    (void)R_OSAL_IoDeviceOpen(device, &dev_handle);

    OSAL_TEST_INFO("*** Start: R_OSAL_PmSetPostClock ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_PmSetPostClock (dev_handle, true, 63);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_PmSetPostClock", TEST_PmSetPostClock, test_count, osal_ret, exec_time);

    (void)R_OSAL_IoDeviceClose(dev_handle);

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

    /* R_OSAL_MmngrGetOsalMaxConfig */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetOsalMaxConfig ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrGetOsalMaxConfig", TEST_MmngrGetOsalMaxConfig, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrOpen ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrOpen(&config , &mm_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrOpen", TEST_MmngrOpen, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrAlloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrAlloc ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrAlloc(mm_handle, size, alignment, &buffer_obj);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrAlloc", TEST_MmngrAlloc, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrGetConfig */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetConfig ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetConfig(mm_handle, &config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrGetConfig", TEST_MmngrGetConfig, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrInitializeMmngrConfigSt */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInitializeMmngrConfigSt ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(&config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrInitializeMmngrConfigSt", TEST_MmngrInitializeMmngrConfigSt, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrRegisterMonitor */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrRegisterMonitor ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrRegisterMonitor(mm_handle, monitored_event, osal_memory_mmngr_cb, (void*)&user_arg);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrRegisterMonitor", TEST_MmngrRegisterMonitor, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrGetCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetCpuPtr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetCpuPtr(buffer_obj, (void **)&cpu_ptr);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrGetCpuPtr", TEST_MmngrGetCpuPtr, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrGetConstCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetConstCpuPtr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buffer_obj, (void **)&cpu_ptr);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrGetConstCpuPtr", TEST_MmngrGetConstCpuPtr, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrFlush */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrFlush ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrFlush(buffer_obj, offset, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrFlush", TEST_MmngrFlush, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrInvalidate */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInvalidate ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInvalidate(buffer_obj, offset, size);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrInvalidate", TEST_MmngrInvalidate, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrGetHwAddr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetHwAddr ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetHwAddr(buffer_obj, axi_id, &hw_addr);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrGetHwAddr", TEST_MmngrGetHwAddr, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrGetSize */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetSize ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetSize(buffer_obj, &osize);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrGetSize", TEST_MmngrGetSize, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrCheckBuffer */    /* requires R_OSAL_MmngrRegisterMonitor, R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrCheckBuffer ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrCheckBuffer(buffer_obj);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrCheckBuffer", TEST_MmngrCheckBuffer, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrDebugMmngrPrint */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrDebugMmngrPrint ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrDebugMmngrPrint(mm_handle, stdout);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrDebugMmngrPrint", TEST_MmngrDebugMmngrPrint, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrDebugBufferPrint */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrDebugBufferPrint ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrDebugBufferPrint(buffer_obj, stdout);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrDebugBufferPrint", TEST_MmngrDebugBufferPrint, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrInitializeMemoryBufferObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInitializeMemoryBufferObj ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&memory_buffer);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrInitializeMemoryBufferObj", TEST_MmngrInitializeMemoryBufferObj, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrInitializeMemoryManagerObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrInitializeMemoryManagerObj ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&memory_manager);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrInitializeMemoryManagerObj", TEST_MmngrInitializeMemoryManagerObj, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrDealloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrDealloc ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrDealloc(mm_handle, buffer_obj);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrDealloc", TEST_MmngrDealloc, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrClose */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrClose ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrClose(mm_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrClose", TEST_MmngrClose, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(0, &config);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx", TEST_MmngrGetOsalMaxConfigFromRegionIdx, test_count, osal_ret, exec_time);

    /* R_OSAL_MmngrOpenFromRegionIdx */
    OSAL_TEST_INFO("*** Start: R_OSAL_MmngrOpenFromRegionIdx ***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_1st);
    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config , 0, &mm_handle);
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION,&current_time_2nd);
    (void)R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &exec_time);
    SET_TIME("R_OSAL_MmngrOpenFromRegionIdx", TEST_MmngrOpenFromRegionIdx, test_count, osal_ret, exec_time);
    (void)R_OSAL_MmngrClose(mm_handle);

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
    SET_TIME("R_OSAL_GetVersion", TEST_GetVersion, test_count, OSAL_RETURN_OK, exec_time);

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
