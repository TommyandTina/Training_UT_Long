
#include "stub.h"
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "target/qnx/r_osal_os_type.h"

e_osal_return_t time_test_all(int func_id);
e_osal_return_t time01_01_Initialize(void);
e_osal_return_t time01_02_Deinitialize(void);
e_osal_return_t time01_03_GetVersion(void);
#ifdef RESOURCE_CHECK
e_osal_return_t time01_04_ClockTimeInitialize(void);
e_osal_return_t time01_05_ThreadInitialize(void);
e_osal_return_t time01_06_rcfg_cond_control_init(void);
e_osal_return_t time01_07_rcfg_mutex_control_init(void);
e_osal_return_t time01_08_MqInitialize(void);
e_osal_return_t time01_09_IoInitialize(void);
e_osal_return_t time01_10_PmInitialize(void);
e_osal_return_t time01_11_InterruptInitialize(void);
e_osal_return_t time01_12_MmngrInitialize(void);
#endif  /* #ifdef RESOURCE_CHECK */
e_osal_return_t time02_01_ThreadCreate(void);
e_osal_return_t time02_02_ThreadSleepUntilTimeStamp(void);
e_osal_return_t time02_03_ThreadSleepForTimePeriod(void);
e_osal_return_t time02_04_ThreadSelf(void);     // xOS2
e_osal_return_t time02_05_ThreadEqual(void);
e_osal_return_t time02_06_ThreadJoin(void);
e_osal_return_t time02_07_ThreadInitializeThreadConfigSt(void); // xOS2
e_osal_return_t time03_01_ThsyncMutexCreate(void);
e_osal_return_t time03_02_ThsyncMutexDestroy(void);
e_osal_return_t time03_03_ThsyncMutexLockUntilTimeStamp(void);
e_osal_return_t time03_04_ThsyncMutexLockForTimePeriod(void);
e_osal_return_t time03_05_ThsyncMutexTryLock(void);
e_osal_return_t time03_06_ThsyncMutexUnlock(void);
e_osal_return_t time03_07_ThsyncCondCreate(void);
e_osal_return_t time03_08_ThsyncCondDestroy(void);
e_osal_return_t time03_09_ThsyncCondWaitForTimePeriod(void);
e_osal_return_t time03_10_ThsyncCondWaitUntilTimeStamp(void);
e_osal_return_t time03_11_ThsyncCondBroadcast(void);
e_osal_return_t time03_12_ThsyncCondSignal(void);
e_osal_return_t time03_13_ThsyncMemoryBarrier(void);
e_osal_return_t time04_01_MqCreate(void);
e_osal_return_t time04_02_MqDelete(void);
e_osal_return_t time04_03_MqSendUntilTimeStamp(void);
e_osal_return_t time04_04_MqSendForTimePeriod(void);
e_osal_return_t time04_05_MqReceiveUntilTimeStamp(void);
e_osal_return_t time04_06_MqReceiveForTimePeriod(void);
e_osal_return_t time04_07_MqIsFull(void);
e_osal_return_t time04_08_MqIsEmpty(void);
e_osal_return_t time04_09_MqGetConfig(void);
e_osal_return_t time04_10_MqReset(void);        // xOS2
e_osal_return_t time04_11_MqInitializeMqConfigSt(void); // xOS2
e_osal_return_t time05_01_ClockTimeGetTimeStamp(void);
e_osal_return_t time05_02_ClockTimeCalculateTimeDifference(void);
e_osal_return_t time06_01_InterruptGetNumOfIrqChannels(void);
e_osal_return_t time06_02_InterruptRegisterIsr(void);
e_osal_return_t time06_03_InterruptUnregisterIsr(void);
e_osal_return_t time06_04_InterruptEnableIsr(void);
e_osal_return_t time06_05_InterruptDisableIsr(void);
e_osal_return_t time06_06_InterruptIsISRContext(void);
e_osal_return_t time06_07_InterruptSetIsrPriority(void);
e_osal_return_t time07_01_IoGetNumOfDevices(void);
e_osal_return_t time07_02_IoGetDeviceList(void);
e_osal_return_t time07_03_IoDeviceOpen(char *device);
e_osal_return_t time07_04_IoDeviceClose(void);
e_osal_return_t time07_05_IoRead8(void);
e_osal_return_t time07_06_IoRead16(void);
e_osal_return_t time07_07_IoRead32(void);
e_osal_return_t time07_08_IoRead64(void);
e_osal_return_t time07_09_IoWrite8(void);
e_osal_return_t time07_10_IoWrite16(void);
e_osal_return_t time07_11_IoWrite32(void);
e_osal_return_t time07_12_IoWrite64(void);
e_osal_return_t time07_13_IoBlockRead8(void);
e_osal_return_t time07_14_IoBlockRead16(void);
e_osal_return_t time07_15_IoBlockRead32(void);
e_osal_return_t time07_16_IoBlockRead64(void);
e_osal_return_t time07_17_IoBlockWrite8(void);
e_osal_return_t time07_18_IoBlockWrite16(void);
e_osal_return_t time07_19_IoBlockWrite32(void);
e_osal_return_t time07_20_IoBlockWrite64(void);
e_osal_return_t time07_21_IoGetDeviceAxiBusId(void);
e_osal_return_t time07_22_IoGetAxiBusIdFromDeviceName(void);
e_osal_return_t time07_23_IoGetAxiBusNameList(void);
e_osal_return_t time07_24_IoGetAxiBusName(void);
e_osal_return_t time07_25_IoGetAxiIdListForDeviceHdl(void);
e_osal_return_t time07_26_IoGetAxiIdListForDeviceName(void);
e_osal_return_t time07_27_IoGetAxiIdListForAxiBusName(void);
e_osal_return_t time07_28_IoGetNumOfAxiIdsForDeviceHdl(void);
e_osal_return_t time07_29_IoGetNumOfAxiIdsForDeviceName(void);
e_osal_return_t time07_30_IoGetNumOfAxiIdsForAxiBusName(void);
e_osal_return_t time08_01_PmSetRequiredState(void);
e_osal_return_t time08_02_PmGetRequiredState(void);
e_osal_return_t time08_03_PmWaitForState(void);
e_osal_return_t time08_04_PmGetState(void);
e_osal_return_t time08_05_PmGetLowestIntermediatePowerState(void);
e_osal_return_t time08_06_PmSetPolicy(void);
e_osal_return_t time08_07_PmGetPolicy(void);
e_osal_return_t time08_08_PmSetResetState(void);
e_osal_return_t time08_09_PmGetResetState(void);
e_osal_return_t time08_10_PmSetPostClock(void);
e_osal_return_t time09_01_MmngrOpen(void);
e_osal_return_t time09_02_MmngrClose(void);
e_osal_return_t time09_03_MmngrAlloc(void);
e_osal_return_t time09_04_MmngrDealloc(void);
e_osal_return_t time09_05_MmngrGetConfig(void);
e_osal_return_t time09_06_MmngrGetOsalMaxConfig(void);
e_osal_return_t time09_07_MmngrInitializeMmngrConfigSt(void);
e_osal_return_t time09_08_MmngrRegisterMonitor(void);
e_osal_return_t time09_09_MmngrGetCpuPtr(void);
e_osal_return_t time09_10_MmngrGetConstCpuPtr(void);
e_osal_return_t time09_11_MmngrFlush(void);
e_osal_return_t time09_12_MmngrInvalidate(void);
e_osal_return_t time09_13_MmngrGetHwAddr(void);
e_osal_return_t time09_14_MmngrGetSize(void);
e_osal_return_t time09_15_MmngrCheckBuffer(void);
e_osal_return_t time09_16_MmngrDebugMmngrPrint(void);
e_osal_return_t time09_17_MmngrDebugBufferPrint(void);
e_osal_return_t time09_18_MmngrInitializeMemoryBufferObj(void);
e_osal_return_t time09_19_MmngrInitializeMemoryManagerObj(void);
e_osal_return_t time09_20_MmngrOpenFromRegionIdx(void);
e_osal_return_t time09_21_MmngrGetOsalMaxConfigFromRegionIdx(void);

/* irq callback function */
void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg);

/* mmgr callback function */
void time_callback_mmgr(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl);


void entry_point(void)
{
    /* entry point for WinAMS */
}

#define SEQUENCE_TEST_NUM (104)
#define RESOURCE_TEST_NUM (112)

/* expect value of return code for sequence test */
e_osal_return_t expected_ret[SEQUENCE_TEST_NUM] = {
    OSAL_RETURN_OK,        // time01_01_Initialize
    OSAL_RETURN_OK,        // time02_01_ThreadCreate
    OSAL_RETURN_OK,        // time02_02_ThreadSleepUntilTimeStamp
    OSAL_RETURN_OK,        // time02_03_ThreadSleepForTimePeriod
    OSAL_RETURN_FAIL,      // time02_04_ThreadSelf
    OSAL_RETURN_OK,        // time02_05_ThreadEqual
    OSAL_RETURN_OK,        // time02_06_ThreadJoin
    OSAL_RETURN_OK,        // time02_07_ThreadInitializeThreadConfigSt
    OSAL_RETURN_OK,        // time03_01_ThsyncMutexCreate
    OSAL_RETURN_OK,        // time03_03_ThsyncMutexLockUntilTimeStamp
    OSAL_RETURN_OK,        // time03_06_ThsyncMutexUnlock
    OSAL_RETURN_OK,        // time03_04_ThsyncMutexLockForTimePeriod
    OSAL_RETURN_OK,        // time03_05_ThsyncMutexTryLock
    OSAL_RETURN_OK,        // time03_07_ThsyncCondCreate
    OSAL_RETURN_OK,        // time03_09_ThsyncCondWaitForTimePeriod
    OSAL_RETURN_OK,        // time03_10_ThsyncCondWaitUntilTimeStamp
    OSAL_RETURN_OK,        // time03_11_ThsyncCondBroadcast
    OSAL_RETURN_OK,        // time03_12_ThsyncCondSignal
    OSAL_RETURN_OK,        // time03_08_ThsyncCondDestroy
    OSAL_RETURN_OK,        // time03_02_ThsyncMutexDestroy
    OSAL_RETURN_OK,        // time03_13_ThsyncMemoryBarrier
    OSAL_RETURN_OK,        // time04_01_MqCreate
    OSAL_RETURN_OK,        // time04_03_MqSendUntilTimeStamp
    OSAL_RETURN_OK,        // time04_05_MqReceiveUntilTimeStamp
    OSAL_RETURN_OK,        // time04_04_MqSendForTimePeriod
    OSAL_RETURN_OK,        // time04_06_MqReceiveForTimePeriod
    OSAL_RETURN_OK,        // time04_07_MqIsFull
    OSAL_RETURN_OK,        // time04_08_MqIsEmpty
    OSAL_RETURN_OK,        // time04_09_MqGetConfig
    OSAL_RETURN_OK,        // time04_10_MqReset
    OSAL_RETURN_OK,        // time04_11_MqInitializeMqConfigSt
    OSAL_RETURN_OK,        // time04_02_MqDelete
    OSAL_RETURN_OK,        // time05_01_ClockTimeGetTimeStamp
    OSAL_RETURN_OK,        // time05_02_ClockTimeCalculateTimeDifference
    OSAL_RETURN_OK,        // time07_01_IoGetNumOfDevices
    OSAL_RETURN_OK,        // time07_02_IoGetDeviceList
    OSAL_RETURN_OK,        // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    OSAL_RETURN_OK,        // time07_05_IoRead8
    OSAL_RETURN_OK,        // time07_06_IoRead16
    OSAL_RETURN_OK,        // time07_07_IoRead32
    OSAL_RETURN_OK,        // time07_08_IoRead64
    OSAL_RETURN_OK,        // time07_09_IoWrite8
    OSAL_RETURN_OK,        // time07_10_IoWrite16
    OSAL_RETURN_OK,        // time07_11_IoWrite32
    OSAL_RETURN_OK,        // time07_12_IoWrite64
    OSAL_RETURN_OK,        // time07_13_IoBlockRead8
    OSAL_RETURN_OK,        // time07_14_IoBlockRead16
    OSAL_RETURN_OK,        // time07_15_IoBlockRead32
    OSAL_RETURN_OK,        // time07_16_IoBlockRead64
    OSAL_RETURN_OK,        // time07_17_IoBlockWrite8
    OSAL_RETURN_OK,        // time07_18_IoBlockWrite16
    OSAL_RETURN_OK,        // time07_19_IoBlockWrite32
    OSAL_RETURN_OK,        // time07_20_IoBlockWrite64
    OSAL_RETURN_OK,        // time07_21_IoGetDeviceAxiBusId
    OSAL_RETURN_OK,        // time07_22_IoGetAxiBusIdFromDeviceName
    OSAL_RETURN_OK,        // time07_23_IoGetAxiBusNameList
    OSAL_RETURN_OK,        // time07_24_IoGetAxiBusName
    OSAL_RETURN_OK,        // time07_25_IoGetAxiIdListForDeviceHdl
    OSAL_RETURN_OK,        // time07_26_IoGetAxiIdListForDeviceName
    OSAL_RETURN_OK,        // time07_27_IoGetAxiIdListForAxiBusName
    OSAL_RETURN_OK,        // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    OSAL_RETURN_OK,        // time07_29_IoGetNumOfAxiIdsForDeviceName
    OSAL_RETURN_OK,        // time07_30_IoGetNumOfAxiIdsForAxiBusName
    OSAL_RETURN_OK,        // time06_01_InterruptGetNumOfIrqChannels
    OSAL_RETURN_OK,        // time06_02_InterruptRegisterIsr
    OSAL_RETURN_OK,        // time06_04_InterruptEnableIsr
    OSAL_RETURN_OK,        // time06_05_InterruptDisableIsr
    OSAL_RETURN_OK,        // time06_03_InterruptUnregisterIsr
    OSAL_RETURN_OK,        // time06_06_InterruptIsISRContext
    OSAL_RETURN_OK,        // time06_07_InterruptSetIsrPriority
    OSAL_RETURN_OK,        // time07_04_IoDeviceClose
    OSAL_RETURN_OK,        // time08_01_PmSetRequiredState
    OSAL_RETURN_OK,        // time08_02_PmGetRequiredState
    OSAL_RETURN_OK,        // time08_03_PmWaitForState
    OSAL_RETURN_OK,        // time08_04_PmGetState
    OSAL_RETURN_OK,        // time08_05_PmGetLowestIntermediatePowerState
    OSAL_RETURN_OK,        // time08_06_PmSetPolicy
    OSAL_RETURN_OK,        // time08_07_PmGetPolicy
    OSAL_RETURN_OK,        // time08_08_PmSetResetState
    OSAL_RETURN_OK,        // time08_09_PmGetResetState
    OSAL_RETURN_OK,        // time08_10_PmSetPostClock
    OSAL_RETURN_OK,        // time09_01_MmngrOpen
    OSAL_RETURN_OK,        // time09_03_MmngrAlloc
    OSAL_RETURN_OK,        // time09_05_MmngrGetConfig
    OSAL_RETURN_OK,        // time09_06_MmngrGetOsalMaxConfig
    OSAL_RETURN_OK,        // time09_07_MmngrInitializeMmngrConfigSt
    OSAL_RETURN_OK,        // time09_08_MmngrRegisterMonitor
    OSAL_RETURN_OK,        // time09_09_MmngrGetCpuPtr
    OSAL_RETURN_OK,        // time09_10_MmngrGetConstCpuPtr
    OSAL_RETURN_OK,        // time09_11_MmngrFlush
    OSAL_RETURN_OK,        // time09_12_MmngrInvalidate
    OSAL_RETURN_OK,        // time09_13_MmngrGetHwAddr
    OSAL_RETURN_OK,        // time09_14_MmngrGetSize
    OSAL_RETURN_OK,        // time09_15_MmngrCheckBuffer
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    OSAL_RETURN_OK,        // time09_16_MmngrDebugMmngrPrint
    OSAL_RETURN_OK,        // time09_17_MmngrDebugBufferPrint
#else
    OSAL_RETURN_HANDLE_NOT_SUPPORTED,         // time09_16_MmngrDebugMmngrPrint
    OSAL_RETURN_UNSUPPORTED_OPERATION,        // time09_17_MmngrDebugBufferPrint
#endif
    OSAL_RETURN_OK,        // time09_04_MmngrDealloc
    OSAL_RETURN_OK,        // time09_02_MmngrClose
    OSAL_RETURN_OK,        // time09_20_MmngrOpenFromRegionIdx
    OSAL_RETURN_OK,        // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    OSAL_RETURN_OK,        // time09_18_MmngrInitializeMemoryBufferObj
    OSAL_RETURN_OK,        // time09_19_MmngrInitializeMemoryManagerObj
    OSAL_RETURN_OK,        // time01_03_GetVersion
    OSAL_RETURN_FAIL,      // time01_02_Deinitialize
};

/* count for resource check test */
int func_count = 0;
int test_count = 0;

/* structure definition for expected each resource count */
typedef struct st_expected_resource_count
{
    uint64_t    thread_count;        /* pthread_create call count */
    uint64_t    mutex_count;         /* pthread_mutex_init call count */
    uint64_t    cond_count;          /* pthread_cond_init call count */
    bool        memory_use;          /* malloc, realloc, calloc call */
    bool        timer_use;           /* timer_create call */
} st_expected_resource_count;

/* actual value for resource check test */
st_expected_resource_count actual_value[RESOURCE_TEST_NUM];

/* expected value for resource check test */
st_expected_resource_count expected_value[RESOURCE_TEST_NUM] = {
#if defined (RCAR_V3H1)
    {0, 0, 0, false, false},        // time01_03_ClockTimeInitialize
    {126, 126, 252, true, false},   // time01_04_ThreadInitialize
    {0, 21, 21, true, false},       // time01_05_rcfg_cond_control_init
    {0, 153, 0, true, false},       // time01_06_rcfg_mutex_control_init
    {0, 310, 310, true, false},     // time01_07_MqInitialize
    {0, 122, 0, true, false},       // time01_08_IoInitialize
    {1, 2, 3, true, false},         // time01_09_PmInitialize
    {53, 106, 53, true, false},     // time01_10_InterruptInitialize
    {0, 6, 0, true, false},         // time01_11_MmngrInitialize
#elif defined (RCAR_V3H2)
    {0, 0, 0, false, false},        // time01_03_ClockTimeInitialize
    {126, 126, 252, true, false},   // time01_04_ThreadInitialize
    {0, 21, 21, true, false},       // time01_05_rcfg_cond_control_init
    {0, 153, 0, true, false},       // time01_06_rcfg_mutex_control_init
    {0, 310, 310, true, false},     // time01_07_MqInitialize
    {0, 116, 0, true, false},       // time01_08_IoInitialize
    {1, 2, 3, true, false},         // time01_09_PmInitialize
    {52, 104, 52, true, false},     // time01_10_InterruptInitialize
    {0, 6, 0, true, false},         // time01_11_MmngrInitialize
#elif defined (RCAR_V3M)
    {0, 0, 0, false, false},        // time01_03_ClockTimeInitialize
    {126, 126, 252, true, false},   // time01_04_ThreadInitialize
    {0, 21, 21, true, false},       // time01_05_rcfg_cond_control_init
    {0, 153, 0, true, false},       // time01_06_rcfg_mutex_control_init
    {0, 310, 310, true, false},     // time01_07_MqInitialize
    {0, 48, 0, true, false},        // time01_08_IoInitialize
    {1, 2, 3, true, false},         // time01_09_PmInitialize
    {35, 70, 35, true, false},      // time01_10_InterruptInitialize
    {0, 6, 0, true, false},         // time01_11_MmngrInitialize
#elif defined (RCAR_V4H)
    {0, 0, 0, false, false},        // time01_03_ClockTimeInitialize
    {126, 126, 252, true, false},   // time01_04_ThreadInitialize
    {0, 21, 21, true, false},       // time01_05_rcfg_cond_control_init
    {0, 153, 0, true, false},       // time01_06_rcfg_mutex_control_init
    {0, 310, 310, true, false},     // time01_07_MqInitialize
    {0, 278, 0, true, false},       // time01_08_IoInitialize
    {1, 2, 3, true, false},         // time01_09_PmInitialize
    {264, 528, 264, true, false},   // time01_10_InterruptInitialize
    {0, 6, 0, true, false},         // time01_11_MmngrInitialize
#elif defined (RCAR_V4M)
    {0, 0, 0, false, false},        // time01_03_ClockTimeInitialize
    {126, 126, 252, true, false},   // time01_04_ThreadInitialize
    {0, 21, 21, true, false},       // time01_05_rcfg_cond_control_init
    {0, 153, 0, true, false},       // time01_06_rcfg_mutex_control_init
    {0, 310, 310, true, false},     // time01_07_MqInitialize
    {0, 210, 0, true, false},       // time01_08_IoInitialize
    {1, 2, 3, true, false},         // time01_09_PmInitialize
    {197, 394, 197, true, false},   // time01_10_InterruptInitialize
    {0, 6, 0, true, false},         // time01_11_MmngrInitialize
#elif defined (RCAR_S4)
    {0, 0, 0, false, false},        // time01_03_ClockTimeInitialize
    {50, 50, 100, true, false},     // time01_04_ThreadInitialize
    {0, 11, 11, true, false},       // time01_05_rcfg_cond_control_init
    {0, 50, 0, true, false},        // time01_06_rcfg_mutex_control_init
    {0, 188, 188, true, false},     // time01_07_MqInitialize
    {0, 130, 0, true, false},       // time01_08_IoInitialize
    {1, 2, 3, true, false},         // time01_09_PmInitialize
    {133, 266, 133, true, false},   // time01_10_InterruptInitialize
    {0, 2, 0, true, false},         // time01_11_MmngrInitialize
#endif
    {0, 0, 0, false, false},        // time02_01_ThreadCreate
    {0, 0, 0, false, false},        // time02_02_ThreadSleepUntilTimeStamp
    {0, 0, 0, false, false},        // time02_03_ThreadSleepForTimePeriod
    {0, 0, 0, false, false},        // time02_04_ThreadSelf(void)     
    {0, 0, 0, false, false},        // time02_05_ThreadEqual
    {0, 0, 0, false, false},        // time02_06_ThreadJoin
    {0, 0, 0, false, false},        // time02_07_ThreadInitializeThreadConfigSt
    {0, 0, 0, false, false},        // time03_01_ThsyncMutexCreate
    {0, 0, 0, false, false},        // time03_03_ThsyncMutexLockUntilTimeStamp
    {0, 0, 0, false, false},        // time03_06_ThsyncMutexUnlock
    {0, 0, 0, false, false},        // time03_04_ThsyncMutexLockForTimePeriod
    {0, 0, 0, false, false},        // time03_05_ThsyncMutexTryLock
    {0, 0, 0, false, false},        // time03_07_ThsyncCondCreate
    {0, 0, 0, false, false},        // time03_09_ThsyncCondWaitForTimePeriod
    {0, 0, 0, false, false},        // time03_10_ThsyncCondWaitUntilTimeStamp
    {0, 0, 0, false, false},        // time03_11_ThsyncCondBroadcast
    {0, 0, 0, false, false},        // time03_12_ThsyncCondSignal
    {0, 0, 0, false, false},        // time03_08_ThsyncCondDestroy
    {0, 0, 0, false, false},        // time03_02_ThsyncMutexDestroy
    {0, 0, 0, false, false},        // time03_13_ThsyncMemoryBarrier
    {0, 0, 0, false, false},        // time04_01_MqCreate
    {0, 0, 0, false, false},        // time04_03_MqSendUntilTimeStamp
    {0, 0, 0, false, false},        // time04_05_MqReceiveUntilTimeStamp
    {0, 0, 0, false, false},        // time04_04_MqSendForTimePeriod
    {0, 0, 0, false, false},        // time04_06_MqReceiveForTimePeriod
    {0, 0, 0, false, false},        // time04_07_MqIsFull
    {0, 0, 0, false, false},        // time04_08_MqIsEmpty
    {0, 0, 0, false, false},        // time04_09_MqGetConfig
    {0, 0, 0, false, false},        // time04_10_MqReset
    {0, 0, 0, false, false},        // time04_11_MqInitializeMqConfigSt
    {0, 0, 0, false, false},        // time04_02_MqDelete
    {0, 0, 0, false, false},        // time05_01_ClockTimeGetTimeStamp
    {0, 0, 0, false, false},        // time05_02_ClockTimeCalculateTimeDifference
    {0, 0, 0, false, false},        // time07_01_IoGetNumOfDevices
    {0, 0, 0, false, false},        // time07_02_IoGetDeviceList
    {0, 0, 0, false, false},        // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    {0, 0, 0, false, false},        // time07_05_IoRead8
    {0, 0, 0, false, false},        // time07_06_IoRead16
    {0, 0, 0, false, false},        // time07_07_IoRead32
    {0, 0, 0, false, false},        // time07_08_IoRead64
    {0, 0, 0, false, false},        // time07_09_IoWrite8
    {0, 0, 0, false, false},        // time07_10_IoWrite16
    {0, 0, 0, false, false},        // time07_11_IoWrite32
    {0, 0, 0, false, false},        // time07_12_IoWrite64
    {0, 0, 0, false, false},        // time07_13_IoBlockRead8
    {0, 0, 0, false, false},        // time07_14_IoBlockRead16
    {0, 0, 0, false, false},        // time07_15_IoBlockRead32
    {0, 0, 0, false, false},        // time07_16_IoBlockRead64
    {0, 0, 0, false, false},        // time07_17_IoBlockWrite8
    {0, 0, 0, false, false},        // time07_18_IoBlockWrite16
    {0, 0, 0, false, false},        // time07_19_IoBlockWrite32
    {0, 0, 0, false, false},        // time07_20_IoBlockWrite64
    {0, 0, 0, false, false},        // time07_21_IoGetDeviceAxiBusId
    {0, 0, 0, false, false},        // time07_22_IoGetAxiBusIdFromDeviceName
    {0, 0, 0, false, false},        // time07_23_IoGetAxiBusNameList
    {0, 0, 0, false, false},        // time07_24_IoGetAxiBusName
    {0, 0, 0, false, false},        // time07_25_IoGetAxiIdListForDeviceHdl
    {0, 0, 0, false, false},        // time07_26_IoGetAxiIdListForDeviceName
    {0, 0, 0, false, false},        // time07_27_IoGetAxiIdListForAxiBusName
    {0, 0, 0, false, false},        // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    {0, 0, 0, false, false},        // time07_29_IoGetNumOfAxiIdsForDeviceName
    {0, 0, 0, false, false},        // time07_30_IoGetNumOfAxiIdsForAxiBusName
    {0, 0, 0, false, false},        // time06_01_InterruptGetNumOfIrqChannels
    {0, 0, 0, false, false},        // time06_02_InterruptRegisterIsr
    {0, 0, 0, false, false},        // time06_04_InterruptEnableIsr
    {0, 0, 0, false, false},        // time06_05_InterruptDisableIsr
    {0, 0, 0, false, false},        // time06_03_InterruptUnregisterIsr
    {0, 0, 0, false, false},        // time06_06_InterruptIsISRContext
    {0, 0, 0, false, false},        // time06_07_InterruptSetIsrPriority
    {0, 0, 0, false, false},        // time07_04_IoDeviceClose
    {0, 0, 0, false, false},        // time08_01_PmSetRequiredState
    {0, 0, 0, false, false},        // time08_02_PmGetRequiredState
    {0, 0, 0, false, false},        // time08_03_PmWaitForState
    {0, 0, 0, false, false},        // time08_04_PmGetState
    {0, 0, 0, false, false},        // time08_05_PmGetLowestIntermediatePowerState
    {0, 0, 0, false, false},        // time08_06_PmSetPolicy
    {0, 0, 0, false, false},        // time08_07_PmGetPolicy
    {0, 0, 0, false, false},        // time08_08_PmSetResetState
    {0, 0, 0, false, false},        // time08_09_PmGetResetState
    {0, 0, 0, false, false},        // time08_10_PmSetPostClock
    {0, 16, 0, true, false},        // time09_01_MmngrOpen
    {0, 0, 0, false, false},        // time09_03_MmngrAlloc
    {0, 0, 0, false, false},        // time09_05_MmngrGetConfig
    {0, 0, 0, false, false},        // time09_06_MmngrGetOsalMaxConfig
    {0, 0, 0, false, false},        // time09_07_MmngrInitializeMmngrConfigSt
    {0, 0, 0, false, false},        // time09_08_MmngrRegisterMonitor
    {0, 0, 0, false, false},        // time09_09_MmngrGetCpuPtr
    {0, 0, 0, false, false},        // time09_10_MmngrGetConstCpuPtr
    {0, 0, 0, false, false},        // time09_11_MmngrFlush
    {0, 0, 0, false, false},        // time09_12_MmngrInvalidate
    {0, 0, 0, false, false},        // time09_13_MmngrGetHwAddr
    {0, 0, 0, false, false},        // time09_14_MmngrGetSize
    {0, 0, 0, false, false},        // time09_15_MmngrCheckBuffer
    {0, 0, 0, false, false},        // time09_16_MmngrDebugMmngrPrint
    {0, 0, 0, false, false},        // time09_17_MmngrDebugBufferPrint
    {0, 0, 0, false, false},        // time09_04_MmngrDealloc
    {0, 0, 0, false, false},        // time09_02_MmngrClose
    {0, 16, 0, true, false},        // time09_20_MmngrOpenFromRegionIdx
    {0, 0, 0, false, false},        // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    {0, 0, 0, false, false},        // time09_18_MmngrInitializeMemoryBufferObj
    {0, 0, 0, false, false},        // time09_19_MmngrInitializeMemoryManagerObj
    {0, 0, 0, false, false},        // time01_03_GetVersion
    {0, 0, 0, false, false}         // time01_02_Deinitialize
};

int main(void)
{
    e_osal_return_t osal_ret[SEQUENCE_TEST_NUM];
    unsigned int test_pass   = 0;
    unsigned int test_fail   = 0;

#ifdef USE_STUB
    g_mutex_trylock_err = 0;
#endif
#ifndef RESOURCE_CHECK
    printf("Check API sequence\n");
    osal_ret[0] = time_test_all(101);  // time01_01_Initialize
#else
    printf("Check Software Resource\n");
    time_test_all(104);  // time01_03_ClockTimeInitialize
    time_test_all(105);  // time01_04_ThreadInitialize
    time_test_all(106);  // time01_05_rcfg_cond_control_init
    time_test_all(107);  // time01_06_rcfg_mutex_control_init
    time_test_all(108);  // time01_07_MqInitialize
    time_test_all(109);  // time01_08_IoInitialize
    time_test_all(110);  // time01_09_PmInitialize
    time_test_all(111);  // time01_10_InterruptInitialize
    time_test_all(112);  // time01_11_MmngrInitialize
#endif
    osal_ret[1] = time_test_all(201);  // time02_01_ThreadCreate
    osal_ret[2] = time_test_all(202);  // time02_02_ThreadSleepUntilTimeStamp
    osal_ret[3] = time_test_all(203);  // time02_03_ThreadSleepForTimePeriod
    osal_ret[4] = time_test_all(204);  // time02_04_ThreadSelf(void)     // xOS2
    osal_ret[5] = time_test_all(205);  // time02_05_ThreadEqual
    osal_ret[6] = time_test_all(206);  // time02_06_ThreadJoin
    osal_ret[7] = time_test_all(207);  // time02_07_ThreadInitializeThreadConfigSt
    osal_ret[8] = time_test_all(301);  // time03_01_ThsyncMutexCreate
    osal_ret[9] = time_test_all(303);  // time03_03_ThsyncMutexLockUntilTimeStamp
    osal_ret[10] = time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret[11] = time_test_all(304);  // time03_04_ThsyncMutexLockForTimePeriod
    time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret[12] = time_test_all(305);  // time03_05_ThsyncMutexTryLock
    osal_ret[13] = time_test_all(307);  // time03_07_ThsyncCondCreate
#ifdef USE_STUB
    g_mutex_trylock_err = EBUSY;
#endif
    osal_ret[14] = time_test_all(309);  // time03_09_ThsyncCondWaitForTimePeriod
    osal_ret[15] = time_test_all(310);  // time03_10_ThsyncCondWaitUntilTimeStamp
#ifdef USE_STUB
    g_mutex_trylock_err = 0;
#endif
    osal_ret[16] = time_test_all(311);  // time03_11_ThsyncCondBroadcast
    osal_ret[17] = time_test_all(312);  // time03_12_ThsyncCondSignal
    osal_ret[18] = time_test_all(308);  // time03_08_ThsyncCondDestroy
    time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret[19] = time_test_all(302);  // time03_02_ThsyncMutexDestroy
    osal_ret[20] = time_test_all(313);  // time03_13_ThsyncMemoryBarrier
    osal_ret[21] = time_test_all(401);  // time04_01_MqCreate
    osal_ret[22] = time_test_all(403);  // time04_03_MqSendUntilTimeStamp
    osal_ret[23] = time_test_all(405);  // time04_05_MqReceiveUntilTimeStamp
    osal_ret[24] = time_test_all(404);  // time04_04_MqSendForTimePeriod
    osal_ret[25] = time_test_all(406);  // time04_06_MqReceiveForTimePeriod
    osal_ret[26] = time_test_all(407);  // time04_07_MqIsFull
    osal_ret[27] = time_test_all(408);  // time04_08_MqIsEmpty
    osal_ret[28] = time_test_all(409);  // time04_09_MqGetConfig
    osal_ret[29] = time_test_all(410);  // time04_10_MqReset
    osal_ret[30] = time_test_all(411);  // time04_11_MqInitializeMqConfigSt
    osal_ret[31] = time_test_all(402);  // time04_02_MqDelete
    osal_ret[32] = time_test_all(501);  // time05_01_ClockTimeGetTimeStamp
    osal_ret[33] = time_test_all(502);  // time05_02_ClockTimeCalculateTimeDifference
    osal_ret[34] = time_test_all(701);  // time07_01_IoGetNumOfDevices
    osal_ret[35] = time_test_all(702);  // time07_02_IoGetDeviceList
    osal_ret[36] = time_test_all(703);  // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    osal_ret[37] = time_test_all(705);  // time07_05_IoRead8
    osal_ret[38] = time_test_all(706);  // time07_06_IoRead16
    osal_ret[39] = time_test_all(707);  // time07_07_IoRead32
    osal_ret[40] = time_test_all(708);  // time07_08_IoRead64
    osal_ret[41] = time_test_all(709);  // time07_09_IoWrite8
    osal_ret[42] = time_test_all(710);  // time07_10_IoWrite16
    osal_ret[43] = time_test_all(711);  // time07_11_IoWrite32
    osal_ret[44] = time_test_all(712);  // time07_12_IoWrite64
    osal_ret[45] = time_test_all(713);  // time07_13_IoBlockRead8
    osal_ret[46] = time_test_all(714);  // time07_14_IoBlockRead16
    osal_ret[47] = time_test_all(715);  // time07_15_IoBlockRead32
    osal_ret[48] = time_test_all(716);  // time07_16_IoBlockRead64
    osal_ret[49] = time_test_all(717);  // time07_17_IoBlockWrite8
    osal_ret[50] = time_test_all(718);  // time07_18_IoBlockWrite16
    osal_ret[51] = time_test_all(719);  // time07_19_IoBlockWrite32
    osal_ret[52] = time_test_all(720);  // time07_20_IoBlockWrite64
    osal_ret[53] = time_test_all(721);  // time07_21_IoGetDeviceAxiBusId
    osal_ret[54] = time_test_all(722);  // time07_22_IoGetAxiBusIdFromDeviceName
    osal_ret[55] = time_test_all(723);  // time07_23_IoGetAxiBusNameList
    osal_ret[56] = time_test_all(724);  // time07_24_IoGetAxiBusName
    osal_ret[57] = time_test_all(726);  // time07_25_IoGetAxiIdListForDeviceHdl
    osal_ret[58] = time_test_all(727);  // time07_26_IoGetAxiIdListForDeviceName
    osal_ret[59] = time_test_all(728);  // time07_27_IoGetAxiIdListForAxiBusName
    osal_ret[60] = time_test_all(729);  // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    osal_ret[61] = time_test_all(730);  // time07_29_IoGetNumOfAxiIdsForDeviceName
    osal_ret[62] = time_test_all(731);  // time07_30_IoGetNumOfAxiIdsForAxiBusName
    osal_ret[63] = time_test_all(601);  // time06_01_InterruptGetNumOfIrqChannels
    osal_ret[64] = time_test_all(602);  // time06_02_InterruptRegisterIsr
    osal_ret[65] = time_test_all(604);  // time06_04_InterruptEnableIsr
    osal_ret[66] = time_test_all(605);  // time06_05_InterruptDisableIsr
    osal_ret[67] = time_test_all(603);  // time06_03_InterruptUnregisterIsr
    osal_ret[68] = time_test_all(606);  // time06_06_InterruptIsISRContext
    osal_ret[69] = time_test_all(607);  // time06_07_InterruptSetIsrPriority
    osal_ret[70] = time_test_all(704);  // time07_04_IoDeviceClose

    time_test_all(732);  // time07_03_IoDeviceOpen(TEST_DEVICE_NAME_PM)
    osal_ret[71] = time_test_all(801);  // time08_01_PmSetRequiredState
    osal_ret[72] = time_test_all(802);  // time08_02_PmGetRequiredState
    osal_ret[73] = time_test_all(803);  // time08_03_PmWaitForState
    osal_ret[74] = time_test_all(804);  // time08_04_PmGetState
    osal_ret[75] = time_test_all(805);  // time08_05_PmGetLowestIntermediatePowerState
    osal_ret[76] = time_test_all(806);  // time08_06_PmSetPolicy
    osal_ret[77] = time_test_all(807);  // time08_07_PmGetPolicy
    osal_ret[78] = time_test_all(808);  // time08_08_PmSetResetState
    osal_ret[79] = time_test_all(809);  // time08_09_PmGetResetState
    time_test_all(704);  // time07_04_IoDeviceClose

    time_test_all(725);  // time07_03_IoDeviceOpen(fbc_00)
    osal_ret[80] = time_test_all(810);  // time08_10_PmSetPostClock
    time_test_all(704);  // time07_04_IoDeviceClose
    osal_ret[81] = time_test_all(901);  // time09_01_MmngrOpen
    osal_ret[82] = time_test_all(903);  // time09_03_MmngrAlloc
    osal_ret[83] = time_test_all(905);  // time09_05_MmngrGetConfig
    osal_ret[84] = time_test_all(906);  // time09_06_MmngrGetOsalMaxConfig
    osal_ret[85] = time_test_all(907);  // time09_07_MmngrInitializeMmngrConfigSt
    osal_ret[86] = time_test_all(908);  // time09_08_MmngrRegisterMonitor
    osal_ret[87] = time_test_all(909);  // time09_09_MmngrGetCpuPtr
    osal_ret[88] = time_test_all(910);  // time09_10_MmngrGetConstCpuPtr
    osal_ret[89] = time_test_all(911);  // time09_11_MmngrFlush
    osal_ret[90] = time_test_all(912);  // time09_12_MmngrInvalidate
    osal_ret[91] = time_test_all(913);  // time09_13_MmngrGetHwAddr
    osal_ret[92] = time_test_all(914);  // time09_14_MmngrGetSize
    osal_ret[93] = time_test_all(915);  // time09_15_MmngrCheckBuffer
    osal_ret[94] = time_test_all(916);  // time09_16_MmngrDebugMmngrPrint
    osal_ret[95] = time_test_all(917);  // time09_17_MmngrDebugBufferPrint
    osal_ret[96] = time_test_all(904);  // time09_04_MmngrDealloc
    osal_ret[97] = time_test_all(902);  // time09_02_MmngrClose
    osal_ret[98] = time_test_all(920);  // time09_20_MmngrOpenFromRegionIdx
    osal_ret[99] = time_test_all(921);  // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    time_test_all(902);  // time09_02_MmngrClose
    osal_ret[100] = time_test_all(918);  // time09_18_MmngrInitializeMemoryBufferObj
    osal_ret[101] = time_test_all(919);  // time09_19_MmngrInitializeMemoryManagerObj
    osal_ret[102] = time_test_all(103);  // time01_03_GetVersion
    osal_ret[103] = time_test_all(102);  // time01_02_Deinitialize

    sleep(10);

#ifdef RESOURCE_CHECK
    bool pass_flag;
    for (int i = 0; i < RESOURCE_TEST_NUM; i++)
    {
        pass_flag = true;
        if (expected_value[i].thread_count != actual_value[i].thread_count)
        {
            pass_flag = false;
        }
        if (expected_value[i].mutex_count != actual_value[i].mutex_count)
        {
            pass_flag = false;
        }
        if (expected_value[i].cond_count != actual_value[i].cond_count)
        {
            pass_flag = false;
        }
        if (expected_value[i].memory_use != actual_value[i].memory_use)
        {
            pass_flag = false;
        }
        if (expected_value[i].timer_use != actual_value[i].timer_use)
        {
            pass_flag = false;
        }

        if (28 == i)        /* test no.28 includes test item "osal internal memory barrier initialize" */
        {
            if (true != pass_flag)
            {
                test_fail += 2;
            }
            else
            {
                test_pass += 2;
            }
        }
        else
        {
            if (true != pass_flag)
            {
                test_fail++;
            }
            else
            {
                test_pass++;
            }
        }
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, (RESOURCE_TEST_NUM + 1 - (test_pass + test_fail)));
#elif SEQUENCE_CHECK
    /* result of time01_01_Initialize includes test items: R_OSAL_Initialize(), internal thread, interrupt thread */
    if (expected_ret[0] != osal_ret[0])
    {
        test_fail += 3;
    }
    else
    {
        test_pass += 3;
    }
    for (int i = 1; i < SEQUENCE_TEST_NUM; i++)
    {
        if (expected_ret[i] != osal_ret[i])
        {
            test_fail++;
        }
        else
        {
            test_pass++;
        }
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, (SEQUENCE_TEST_NUM + 2 - (test_pass + test_fail)));
#endif

    if (0 != test_fail)
    {
        return -1;
    }

    return 0;
}

/* The Create ID of the configuration table */
#define OSAL_THREAD_CREATE_ID   (0xf000U)
#define OSAL_MUTEX_CREATE_ID    (0xf000U)
#define OSAL_COND_CREATE_ID     (0xf000U)
#define OSAL_MQ_CREATE_ID       (0xf000U)

#if defined (RCAR_V3U)
#define TEST_AXI_BUS_NAME   "ir_16"
#define TEST_DEVICE_NAME    "ims_00"
#define TEST_DEVICE_NAME_PM "ims_00"
#define TEST_DEVICE_TYPE    "ims"
#elif defined (RCAR_V3H1)
#define TEST_AXI_BUS_NAME   "vc0_04"
#define TEST_DEVICE_NAME    "ims_00"
#define TEST_DEVICE_NAME_PM "ims_00"
#define TEST_DEVICE_TYPE    "ims"
#elif defined (RCAR_V3H2)
#define TEST_AXI_BUS_NAME   "vc0_04"
#define TEST_DEVICE_NAME    "ims_00"
#define TEST_DEVICE_NAME_PM "ims_00"
#define TEST_DEVICE_TYPE    "ims"
#elif defined (RCAR_V3M)
#define TEST_AXI_BUS_NAME   "vi0_02"
#define TEST_DEVICE_NAME    "ims_00"
#define TEST_DEVICE_NAME_PM "ims_00"
#define TEST_DEVICE_TYPE    "ims"
#elif defined (RCAR_V4H)
#define TEST_AXI_BUS_NAME   "vc_00_04"
#define TEST_DEVICE_NAME    "ims_00"
#define TEST_DEVICE_NAME_PM "ims_00"
#define TEST_DEVICE_TYPE    "ims"
#elif defined (RCAR_V4M)
#define TEST_AXI_BUS_NAME   "vc_00_04"
#define TEST_DEVICE_NAME    "ims_00"
#define TEST_DEVICE_NAME_PM "ims_00"
#define TEST_DEVICE_TYPE    "ims"
#elif defined (RCAR_S4)
#define TEST_AXI_BUS_NAME   "rt1_00"
#define TEST_DEVICE_NAME    "rtdmac_control0_00"
#define TEST_DEVICE_NAME_PM "caiplite_00"
#define TEST_DEVICE_TYPE    "rtdmac_control0"
#endif

static bool is_initialized = false;
static bool is_thread_created = false;
static bool is_mutex_created = false;
static bool is_cond_created = false;
static bool is_mq_created = false;
static bool is_io_numget = false;
static bool is_io_devlistget = false;
static bool is_io_opened = false;
osal_thread_handle_t    th_handle = NULL;
osal_mutex_handle_t     mutex_handle = NULL;
osal_cond_handle_t      cond_handle = NULL;
osal_mq_handle_t        mq_handle = NULL;
#if defined (RCAR_S4)
st_osal_mq_config_t     mq_config = {8, 16};   // {max_num_msg, msg_size}
#else
st_osal_mq_config_t     mq_config = {1, 4};   // {max_num_msg, msg_size}
#endif
osal_device_handle_t    dev_handle = NULL;
osal_memory_manager_handle_t    mmgr_handle = NULL;
osal_memory_buffer_handle_t     buf_handle = NULL;

/*
#ifdef RESOURCE_CHECK
extern bool gs_osal_threadsync_is_initialized;
extern e_osal_return_t osal_rcfg_cond_control_init(void);
extern e_osal_return_t osal_rcfg_mutex_control_init(void);
#endif
*/

e_osal_return_t time_test_all(int func_id)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    switch (func_id)
    {
#ifndef RESOURCE_CHECK
        case 101:  printf("\n<<func-id=%d>>R_OSAL_Initialize() sequence\n\n", func_id); break;
#endif
        case 102:  printf("\n<<func-id=%d>>R_OSAL_Deinitialize() sequence\n\n", func_id); break;
        case 103:  printf("\n<<func-id=%d>>R_OSAL_GetVersion() sequence\n\n", func_id); break;
#ifdef RESOURCE_CHECK
        case 104:  printf("\n<<func-id=%d>>R_OSAL_ClockTimeInitialize() sequence\n\n", func_id); break;
        case 105:  printf("\n<<func-id=%d>>R_OSAL_ThreadInitialize() sequence\n\n", func_id); break;
        case 106:  printf("\n<<func-id=%d>>r_osal_rcfg_cond_control_init() sequence\n\n", func_id); break;
        case 107:  printf("\n<<func-id=%d>>r_osal_rcfg_mutex_control_init() sequence\n\n", func_id); break;
        case 108:  printf("\n<<func-id=%d>>R_OSAL_MqInitialize() sequence\n\n", func_id); break;
        case 109:  printf("\n<<func-id=%d>>R_OSAL_IoInitialize() sequence\n\n", func_id); break;
        case 110:  printf("\n<<func-id=%d>>R_OSAL_PmInitialize() sequence\n\n", func_id); break;
        case 111:  printf("\n<<func-id=%d>>R_OSAL_InterruptInitialize() sequence\n\n", func_id); break;
        case 112:  printf("\n<<func-id=%d>>R_OSAL_MmngrInitialize() sequence\n\n", func_id); break;
#endif  /* #ifdef RESOURCE_CHECK */
        case 201:  printf("\n<<func-id=%d>>R_OSAL_ThreadCreate() sequence\n\n", func_id); break;
        case 202:  printf("\n<<func-id=%d>>R_OSAL_ThreadSleepUntilTimeStamp() sequence\n\n", func_id); break;
        case 203:  printf("\n<<func-id=%d>>R_OSAL_ThreadSleepForTimePeriod() sequence\n\n", func_id); break;
        case 204:  printf("\n<<func-id=%d>>R_OSAL_ThreadSelf() sequence\n\n", func_id); break;
        case 205:  printf("\n<<func-id=%d>>R_OSAL_ThreadEqual() sequence\n\n", func_id); break;
        case 206:  printf("\n<<func-id=%d>>R_OSAL_ThreadJoin() sequence\n\n", func_id); break;
        case 207:  printf("\n<<func-id=%d>>R_OSAL_ThreadInitializeThreadConfigSt() sequence\n\n", func_id); break;
        case 301:  printf("\n<<func-id=%d>>R_OSAL_ThsyncMutexCreate() sequence\n\n", func_id); break;
        case 302:  printf("\n<<func-id=%d>>R_OSAL_ThsyncMutexDestroy() sequence\n\n", func_id); break;
        case 303:  printf("\n<<func-id=%d>>R_OSAL_ThsyncMutexLockUntilTimeStamp() sequence\n\n", func_id); break;
        case 304:  printf("\n<<func-id=%d>>R_OSAL_ThsyncMutexLockForTimePeriod() sequence\n\n", func_id); break;
        case 305:  printf("\n<<func-id=%d>>R_OSAL_ThsyncMutexTryLock() sequence\n\n", func_id); break;
        case 306:  printf("\n<<func-id=%d>>R_OSAL_ThsyncMutexUnlock() sequence\n\n", func_id); break;
        case 307:  printf("\n<<func-id=%d>>R_OSAL_ThsyncCondCreate() sequence\n\n", func_id); break;
        case 308:  printf("\n<<func-id=%d>>R_OSAL_ThsyncCondDestroy() sequence\n\n", func_id); break;
        case 309:  printf("\n<<func-id=%d>>R_OSAL_ThsyncCondWaitForTimePeriod() sequence\n\n", func_id); break;
        case 310:  printf("\n<<func-id=%d>>R_OSAL_ThsyncCondWaitUntilTimeStamp() sequence\n\n", func_id); break;
        case 311:  printf("\n<<func-id=%d>>R_OSAL_ThsyncCondBroadcast() sequence\n\n", func_id); break;
        case 312:  printf("\n<<func-id=%d>>R_OSAL_ThsyncCondSignal() sequence\n\n", func_id); break;
        case 313:  printf("\n<<func-id=%d>>R_OSAL_ThsyncMemoryBarrier() sequence\n\n", func_id); break;
        case 401:  printf("\n<<func-id=%d>>R_OSAL_MqCreate() sequence\n\n", func_id); break;
        case 402:  printf("\n<<func-id=%d>>R_OSAL_MqDelete() sequence\n\n", func_id); break;
        case 403:  printf("\n<<func-id=%d>>R_OSAL_MqSendUntilTimeStamp() sequence\n\n", func_id); break;
        case 404:  printf("\n<<func-id=%d>>R_OSAL_MqSendForTimePeriod() sequence\n\n", func_id); break;
        case 405:  printf("\n<<func-id=%d>>R_OSAL_MqReceiveUntilTimeStamp() sequence\n\n", func_id); break;
        case 406:  printf("\n<<func-id=%d>>R_OSAL_MqReceiveForTimePeriod() sequence\n\n", func_id); break;
        case 407:  printf("\n<<func-id=%d>>R_OSAL_MqIsFull() sequence\n\n", func_id); break;
        case 408:  printf("\n<<func-id=%d>>R_OSAL_MqIsEmpty() sequence\n\n", func_id); break;
        case 409:  printf("\n<<func-id=%d>>R_OSAL_MqGetConfig() sequence\n\n", func_id); break;
        case 410:  printf("\n<<func-id=%d>>R_OSAL_MqReset() sequence\n\n", func_id); break;
        case 411:  printf("\n<<func-id=%d>>R_OSAL_MqInitializeMqConfigSt() sequence\n\n", func_id); break;
        case 501:  printf("\n<<func-id=%d>>R_OSAL_ClockTimeGetTimeStamp() sequence\n\n", func_id); break;
        case 502:  printf("\n<<func-id=%d>>R_OSAL_ClockTimeCalculateTimeDifference() sequence\n\n", func_id); break;
        case 601:  printf("\n<<func-id=%d>>R_OSAL_InterruptGetNumOfIrqChannels() sequence\n\n", func_id); break;
        case 602:  printf("\n<<func-id=%d>>R_OSAL_InterruptRegisterIsr() sequence\n\n", func_id); break;
        case 603:  printf("\n<<func-id=%d>>R_OSAL_InterruptUnregisterIsr() sequence\n\n", func_id); break;
        case 604:  printf("\n<<func-id=%d>>R_OSAL_InterruptEnableIsr() sequence\n\n", func_id); break;
        case 605:  printf("\n<<func-id=%d>>R_OSAL_InterruptDisableIsr() sequence\n\n", func_id); break;
        case 606:  printf("\n<<func-id=%d>>R_OSAL_InterruptIsISRContext() sequence\n\n", func_id); break;
        case 607:  printf("\n<<func-id=%d>>R_OSAL_InterruptSetIsrPriority() sequence\n\n", func_id); break;
        case 701:  printf("\n<<func-id=%d>>R_OSAL_IoGetNumOfDevices() sequence\n\n", func_id); break;
        case 702:  printf("\n<<func-id=%d>>R_OSAL_IoGetDeviceList() sequence\n\n", func_id); break;
        case 703:  printf("\n<<func-id=%d>>R_OSAL_IoDeviceOpen() sequence\n\n", func_id); break;
        case 704:  printf("\n<<func-id=%d>>R_OSAL_IoDeviceClose() sequence\n\n", func_id); break;
        case 705:  printf("\n<<func-id=%d>>R_OSAL_IoRead8() sequence\n\n", func_id); break;
        case 706:  printf("\n<<func-id=%d>>R_OSAL_IoRead16() sequence\n\n", func_id); break;
        case 707:  printf("\n<<func-id=%d>>R_OSAL_IoRead32() sequence\n\n", func_id); break;
        case 708:  printf("\n<<func-id=%d>>R_OSAL_IoRead64() sequence\n\n", func_id); break;
        case 709:  printf("\n<<func-id=%d>>R_OSAL_IoWrite8() sequence\n\n", func_id); break;
        case 710:  printf("\n<<func-id=%d>>R_OSAL_IoWrite16() sequence\n\n", func_id); break;
        case 711:  printf("\n<<func-id=%d>>R_OSAL_IoWrite32() sequence\n\n", func_id); break;
        case 712:  printf("\n<<func-id=%d>>R_OSAL_IoWrite64() sequence\n\n", func_id); break;
        case 713:  printf("\n<<func-id=%d>>R_OSAL_IoBlockRead8() sequence\n\n", func_id); break;
        case 714:  printf("\n<<func-id=%d>>R_OSAL_IoBlockRead16() sequence\n\n", func_id); break;
        case 715:  printf("\n<<func-id=%d>>R_OSAL_IoBlockRead32() sequence\n\n", func_id); break;
        case 716:  printf("\n<<func-id=%d>>R_OSAL_IoBlockRead64() sequence\n\n", func_id); break;
        case 717:  printf("\n<<func-id=%d>>R_OSAL_IoBlockWrite8() sequence\n\n", func_id); break;
        case 718:  printf("\n<<func-id=%d>>R_OSAL_IoBlockWrite16() sequence\n\n", func_id); break;
        case 719:  printf("\n<<func-id=%d>>R_OSAL_IoBlockWrite32() sequence\n\n", func_id); break;
        case 720:  printf("\n<<func-id=%d>>R_OSAL_IoBlockWrite64() sequence\n\n", func_id); break;
        case 721:  printf("\n<<func-id=%d>>R_OSAL_IoGetDeviceAxiBusId() sequence\n\n", func_id); break;
        case 722:  printf("\n<<func-id=%d>>R_OSAL_IoGetAxiBusIdFromDeviceName() sequence\n\n", func_id); break;
        case 723:  printf("\n<<func-id=%d>>R_OSAL_IoGetAxiBusNameList() sequence\n\n", func_id); break;
        case 724:  printf("\n<<func-id=%d>>R_OSAL_IoGetAxiBusName() sequence\n\n", func_id); break;
        case 725:  printf("\n<<func-id=%d>>R_OSAL_IoDeviceOpen() sequence\n\n", func_id); break;
        case 726:  printf("\n<<func-id=%d>>R_OSAL_IoGetAxiIdListForDeviceHdl() sequence\n\n", func_id); break;
        case 727:  printf("\n<<func-id=%d>>R_OSAL_IoGetAxiIdListForDeviceName() sequence\n\n", func_id); break;
        case 728:  printf("\n<<func-id=%d>>R_OSAL_IoGetAxiIdListForAxiBusName() sequence\n\n", func_id); break;
        case 729:  printf("\n<<func-id=%d>>R_OSAL_IoGetNumOfAxiIdsForDeviceHdl() sequence\n\n", func_id); break;
        case 730:  printf("\n<<func-id=%d>>R_OSAL_IoGetNumOfAxiIdsForDeviceName() sequence\n\n", func_id); break;
        case 731:  printf("\n<<func-id=%d>>R_OSAL_IoGetNumOfAxiIdsForAxiBusName() sequence\n\n", func_id); break;
        case 732:  printf("\n<<func-id=%d>>R_OSAL_IoDeviceOpen() sequence\n\n", func_id); break;
        case 801:  printf("\n<<func-id=%d>>R_OSAL_PmSetRequiredState() sequence\n\n", func_id); break;
        case 802:  printf("\n<<func-id=%d>>R_OSAL_PmGetRequiredState() sequence\n\n", func_id); break;
        case 803:  printf("\n<<func-id=%d>>R_OSAL_PmWaitForState() sequence\n\n", func_id); break;
        case 804:  printf("\n<<func-id=%d>>R_OSAL_PmGetState() sequence\n\n", func_id); break;
        case 805:  printf("\n<<func-id=%d>>R_OSAL_PmGetLowestIntermediatePowerState() sequence\n\n", func_id); break;
        case 806:  printf("\n<<func-id=%d>>R_OSAL_PmSetPolicy() sequence\n\n", func_id); break;
        case 807:  printf("\n<<func-id=%d>>R_OSAL_PmGetPolicy() sequence\n\n", func_id); break;
        case 808:  printf("\n<<func-id=%d>>R_OSAL_PmSetResetState() sequence\n\n", func_id); break;
        case 809:  printf("\n<<func-id=%d>>R_OSAL_PmGetResetState() sequence\n\n", func_id); break;
        case 810:  printf("\n<<func-id=%d>>R_OSAL_PmSetPostClock() sequence\n\n", func_id); break;
        case 901:  printf("\n<<func-id=%d>>R_OSAL_MmngrOpen() sequence\n\n", func_id); break;
        case 902:  printf("\n<<func-id=%d>>R_OSAL_MmngrClose() sequence\n\n", func_id); break;
        case 903:  printf("\n<<func-id=%d>>R_OSAL_MmngrAlloc() sequence\n\n", func_id); break;
        case 904:  printf("\n<<func-id=%d>>R_OSAL_MmngrDealloc() sequence\n\n", func_id); break;
        case 905:  printf("\n<<func-id=%d>>R_OSAL_MmngrGetConfig() sequence\n\n", func_id); break;
        case 906:  printf("\n<<func-id=%d>>R_OSAL_MmngrGetOsalMaxConfig() sequence\n\n", func_id); break;
        case 907:  printf("\n<<func-id=%d>>R_OSAL_MmngrInitializeMmngrConfigSt() sequence\n\n", func_id); break;
        case 908:  printf("\n<<func-id=%d>>R_OSAL_MmngrRegisterMonitor() sequence\n\n", func_id); break;
        case 909:  printf("\n<<func-id=%d>>R_OSAL_MmngrGetCpuPtr() sequence\n\n", func_id); break;
        case 910:  printf("\n<<func-id=%d>>R_OSAL_MmngrGetConstCpuPtr() sequence\n\n", func_id); break;
        case 911:  printf("\n<<func-id=%d>>R_OSAL_MmngrFlush() sequence\n\n", func_id); break;
        case 912:  printf("\n<<func-id=%d>>R_OSAL_MmngrInvalidate() sequence\n\n", func_id); break;
        case 913:  printf("\n<<func-id=%d>>R_OSAL_MmngrGetHwAddr() sequence\n\n", func_id); break;
        case 914:  printf("\n<<func-id=%d>>R_OSAL_MmngrGetSize() sequence\n\n", func_id); break;
        case 915:  printf("\n<<func-id=%d>>R_OSAL_MmngrCheckBuffer() sequence\n\n", func_id); break;
        case 916:  printf("\n<<func-id=%d>>R_OSAL_MmngrDebugMmngrPrint() sequence\n\n", func_id); break;
        case 917:  printf("\n<<func-id=%d>>R_OSAL_MmngrDebugBufferPrint() sequence\n\n", func_id); break;
        case 918:  printf("\n<<func-id=%d>>R_OSAL_MmngrInitializeMemoryBufferObj() sequence\n\n", func_id); break;
        case 919:  printf("\n<<func-id=%d>>R_OSAL_MmngrInitializeMemoryManagerObj() sequence\n\n", func_id); break;
        case 920:  printf("\n<<func-id=%d>>R_OSAL_MmngrOpenFromRegionIdx() sequence\n\n", func_id); break;
        case 921:  printf("\n<<func-id=%d>>R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx() sequence\n\n", func_id); break;
        default:   osal_ret = OSAL_RETURN_PAR; break;
    }

    /* initialize global variable for stub */
    init_stub_value();

    switch (func_id)
    {
#ifndef RESOURCE_CHECK
        printf("time01_01_Initialize api start");
        case 101:  osal_ret = time01_01_Initialize(); break;
#endif  /* #ifndef RESOURCE_CHECK */
        case 102:  osal_ret = time01_02_Deinitialize(); break;
        case 103:  osal_ret = time01_03_GetVersion(); break;
#ifdef RESOURCE_CHECK
        case 104:  osal_ret = time01_04_ClockTimeInitialize(); break;
        case 105:  osal_ret = time01_05_ThreadInitialize(); break;
        case 106:  osal_ret = time01_06_rcfg_cond_control_init(); break;
        case 107:  osal_ret = time01_07_rcfg_mutex_control_init(); break;
        case 108:  osal_ret = time01_08_MqInitialize(); break;
        case 109:  osal_ret = time01_09_IoInitialize(); break;
        case 110:  osal_ret = time01_10_PmInitialize(); break;
        case 111:  osal_ret = time01_11_InterruptInitialize(); break;
        case 112:  osal_ret = time01_12_MmngrInitialize(); break;
#endif  /* #ifdef RESOURCE_CHECK */
        case 201:  osal_ret = time02_01_ThreadCreate(); break;
        case 202:  osal_ret = time02_02_ThreadSleepUntilTimeStamp(); break;
        case 203:  osal_ret = time02_03_ThreadSleepForTimePeriod(); break;
        case 204:  osal_ret = time02_04_ThreadSelf(); break;
        case 205:  osal_ret = time02_05_ThreadEqual(); break;
        case 206:  osal_ret = time02_06_ThreadJoin(); break;
        case 207:  osal_ret = time02_07_ThreadInitializeThreadConfigSt(); break;
        case 301:  osal_ret = time03_01_ThsyncMutexCreate(); break;
        case 302:  osal_ret = time03_02_ThsyncMutexDestroy(); break;
        case 303:  osal_ret = time03_03_ThsyncMutexLockUntilTimeStamp(); break;
        case 304:  osal_ret = time03_04_ThsyncMutexLockForTimePeriod(); break;
        case 305:  osal_ret = time03_05_ThsyncMutexTryLock(); break;
        case 306:  osal_ret = time03_06_ThsyncMutexUnlock(); break;
        case 307:  osal_ret = time03_07_ThsyncCondCreate(); break;
        case 308:  osal_ret = time03_08_ThsyncCondDestroy(); break;
        case 309:  osal_ret = time03_09_ThsyncCondWaitForTimePeriod(); break;
        case 310:  osal_ret = time03_10_ThsyncCondWaitUntilTimeStamp(); break;
        case 311:  osal_ret = time03_11_ThsyncCondBroadcast(); break;
        case 312:  osal_ret = time03_12_ThsyncCondSignal(); break;
        case 313:  osal_ret = time03_13_ThsyncMemoryBarrier(); break;
        case 401:  osal_ret = time04_01_MqCreate(); break;
        case 402:  osal_ret = time04_02_MqDelete(); break;
        case 403:  osal_ret = time04_03_MqSendUntilTimeStamp(); break;
        case 404:  osal_ret = time04_04_MqSendForTimePeriod(); break;
        case 405:  osal_ret = time04_05_MqReceiveUntilTimeStamp(); break;
        case 406:  osal_ret = time04_06_MqReceiveForTimePeriod(); break;
        case 407:  osal_ret = time04_07_MqIsFull(); break;
        case 408:  osal_ret = time04_08_MqIsEmpty(); break;
        case 409:  osal_ret = time04_09_MqGetConfig(); break;
        case 410:  osal_ret = time04_10_MqReset(); break;
        case 411:  osal_ret = time04_11_MqInitializeMqConfigSt(); break;
        case 501:  osal_ret = time05_01_ClockTimeGetTimeStamp(); break;
        case 502:  osal_ret = time05_02_ClockTimeCalculateTimeDifference(); break;
        case 601:  osal_ret = time06_01_InterruptGetNumOfIrqChannels(); break;
        case 602:  osal_ret = time06_02_InterruptRegisterIsr(); break;
        case 603:  osal_ret = time06_03_InterruptUnregisterIsr(); break;
        case 604:  osal_ret = time06_04_InterruptEnableIsr(); break;
        case 605:  osal_ret = time06_05_InterruptDisableIsr(); break;
        case 606:  osal_ret = time06_06_InterruptIsISRContext(); break;
        case 607:  osal_ret = time06_07_InterruptSetIsrPriority(); break;
        case 701:  osal_ret = time07_01_IoGetNumOfDevices(); break;
        case 702:  osal_ret = time07_02_IoGetDeviceList(); break;
        case 703:  osal_ret = time07_03_IoDeviceOpen(TEST_DEVICE_NAME); break;
        case 704:  osal_ret = time07_04_IoDeviceClose(); break;
        case 705:  osal_ret = time07_05_IoRead8(); break;
        case 706:  osal_ret = time07_06_IoRead16(); break;
        case 707:  osal_ret = time07_07_IoRead32(); break;
        case 708:  osal_ret = time07_08_IoRead64(); break;
        case 709:  osal_ret = time07_09_IoWrite8(); break;
        case 710:  osal_ret = time07_10_IoWrite16(); break;
        case 711:  osal_ret = time07_11_IoWrite32(); break;
        case 712:  osal_ret = time07_12_IoWrite64(); break;
        case 713:  osal_ret = time07_13_IoBlockRead8(); break;
        case 714:  osal_ret = time07_14_IoBlockRead16(); break;
        case 715:  osal_ret = time07_15_IoBlockRead32(); break;
        case 716:  osal_ret = time07_16_IoBlockRead64(); break;
        case 717:  osal_ret = time07_17_IoBlockWrite8(); break;
        case 718:  osal_ret = time07_18_IoBlockWrite16(); break;
        case 719:  osal_ret = time07_19_IoBlockWrite32(); break;
        case 720:  osal_ret = time07_20_IoBlockWrite64(); break;
        case 721:  osal_ret = time07_21_IoGetDeviceAxiBusId(); break;
        case 722:  osal_ret = time07_22_IoGetAxiBusIdFromDeviceName(); break;
        case 723:  osal_ret = time07_23_IoGetAxiBusNameList(); break;
        case 724:  osal_ret = time07_24_IoGetAxiBusName(); break;
        case 725:  osal_ret = time07_03_IoDeviceOpen("fbc_00"); break;
        case 726:  osal_ret = time07_25_IoGetAxiIdListForDeviceHdl(); break;
        case 727:  osal_ret = time07_26_IoGetAxiIdListForDeviceName(); break;
        case 728:  osal_ret = time07_27_IoGetAxiIdListForAxiBusName(); break;
        case 729:  osal_ret = time07_28_IoGetNumOfAxiIdsForDeviceHdl(); break;
        case 730:  osal_ret = time07_29_IoGetNumOfAxiIdsForDeviceName(); break;
        case 731:  osal_ret = time07_30_IoGetNumOfAxiIdsForAxiBusName(); break;
        case 732:  osal_ret = time07_03_IoDeviceOpen(TEST_DEVICE_NAME_PM); break;
        case 801:  osal_ret = time08_01_PmSetRequiredState(); break;
        case 802:  osal_ret = time08_02_PmGetRequiredState(); break;
        case 803:  osal_ret = time08_03_PmWaitForState(); break;
        case 804:  osal_ret = time08_04_PmGetState(); break;
        case 805:  osal_ret = time08_05_PmGetLowestIntermediatePowerState(); break;
        case 806:  osal_ret = time08_06_PmSetPolicy(); break;
        case 807:  osal_ret = time08_07_PmGetPolicy(); break;
        case 808:  osal_ret = time08_08_PmSetResetState(); break;
        case 809:  osal_ret = time08_09_PmGetResetState(); break;
        case 810:  osal_ret = time08_10_PmSetPostClock(); break;
        case 901:  osal_ret = time09_01_MmngrOpen(); break;
        case 902:  osal_ret = time09_02_MmngrClose(); break;
        case 903:  osal_ret = time09_03_MmngrAlloc(); break;
        case 904:  osal_ret = time09_04_MmngrDealloc(); break;
        case 905:  osal_ret = time09_05_MmngrGetConfig(); break;
        case 906:  osal_ret = time09_06_MmngrGetOsalMaxConfig(); break;
        case 907:  osal_ret = time09_07_MmngrInitializeMmngrConfigSt(); break;
        case 908:  osal_ret = time09_08_MmngrRegisterMonitor(); break;
        case 909:  osal_ret = time09_09_MmngrGetCpuPtr(); break;
        case 910:  osal_ret = time09_10_MmngrGetConstCpuPtr(); break;
        case 911:  osal_ret = time09_11_MmngrFlush(); break;
        case 912:  osal_ret = time09_12_MmngrInvalidate(); break;
        case 913:  osal_ret = time09_13_MmngrGetHwAddr(); break;
        case 914:  osal_ret = time09_14_MmngrGetSize(); break;
        case 915:  osal_ret = time09_15_MmngrCheckBuffer(); break;
        case 916:  osal_ret = time09_16_MmngrDebugMmngrPrint(); break;
        case 917:  osal_ret = time09_17_MmngrDebugBufferPrint(); break;
        case 918:  osal_ret = time09_18_MmngrInitializeMemoryBufferObj(); break;
        case 919:  osal_ret = time09_19_MmngrInitializeMemoryManagerObj(); break;
        case 920:  osal_ret = time09_20_MmngrOpenFromRegionIdx(); break;
        case 921:  osal_ret = time09_21_MmngrGetOsalMaxConfigFromRegionIdx(); break;
        default:   osal_ret = OSAL_RETURN_PAR; break;
    }

#ifdef SEQUENCE_CHECK
    /* print call api */
    print_call_api();
    printf("<<func-id=%d>>\tresult=\t%4d\n\n", func_id, osal_ret);
#endif  /* SEQUENCE_CHECK */

#ifdef RESOURCE_CHECK
    /* don't get result from the second time onwards */
    
    switch(func_count)
    {
    case 20:    break;
    case 28:    break;
    case 81:    break;
    case 91:    break;
    case 92:    break;
    case 94:    break;
    case 114:   break;
    default:    printf("<<func-id=%d>>\tresult=\t%4d\n\n", func_id, osal_ret);
                /* print pthread_create call count */
                printf("\tpthread_create call count    :\t%ld\n", get_pthread_create_count());
                actual_value[test_count].thread_count = get_pthread_create_count();

                /* print pthread_mutex_init call count */
                printf("\tpthread_mutex_init call count:\t%ld\n", get_pthread_mutex_init_count());
                actual_value[test_count].mutex_count = get_pthread_mutex_init_count();

                /* print pthread_cond_init call count */
                printf("\tpthread_cond_init call count :\t%ld\n", get_pthread_cond_init_count());
                actual_value[test_count].cond_count = get_pthread_cond_init_count();

                /* print malloc call count */
                printf("\tmalloc call count            :\t%ld\n", get_malloc_count());

                /* print malloc total size */
                printf("\tmalloc total size            :\t%ld\n", get_malloc_size());

                /* print realloc call count */
                printf("\trealloc call count           :\t%ld\n", get_realloc_count());

                /* print calloc call count */
                printf("\tcalloc call count            :\t%ld\n", get_calloc_count());

                /* print calloc total size */
                printf("\tcalloc total size            :\t%ld\n", get_calloc_size());
                if (0 != (get_malloc_count() + get_realloc_count() + get_calloc_count()))
                {
                    actual_value[test_count].memory_use = true;
                }
                else
                {
                    actual_value[test_count].memory_use = false;
                }

                /* print timer_create call count */
                printf("\ttimer_create call count      :\t%ld\n", get_timer_create_count());
                if (0 != get_timer_create_count())
                {
                    actual_value[test_count].timer_use = true;
                }
                else
                {
                    actual_value[test_count].timer_use = false;
                }
                test_count++;
                break;
    }
    func_count++;
#endif  /* RESOURCE_CHECK */

    return osal_ret;
}

e_osal_return_t time01_01_Initialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if(is_initialized == false)
    {
        osal_ret = R_OSAL_Initialize();
        
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_initialized = true;
        }
    }
    return osal_ret;
}

e_osal_return_t time01_02_Deinitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_Deinitialize();
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_initialized = false;
    }

    return osal_ret;
}

e_osal_return_t time01_03_GetVersion(void)
{
    st_osal_version_t* version;

    version = R_OSAL_GetVersion();

    return OSAL_RETURN_OK;
}

#ifdef RESOURCE_CHECK
e_osal_return_t time01_04_ClockTimeInitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ClockTimeInitialize();

    return osal_ret;
}

e_osal_return_t time01_05_ThreadInitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThreadInitialize();

    return osal_ret;
}

e_osal_return_t time01_06_rcfg_cond_control_init(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    R_OSAL_RCFG_ThsyncSetInitStatus(OSAL_MANAGER_STATE_INITIALIZED);
    osal_ret = r_osal_rcfg_cond_control_init();

    return osal_ret;
}

e_osal_return_t time01_07_rcfg_mutex_control_init(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = r_osal_rcfg_mutex_control_init();

    return osal_ret;
}

e_osal_return_t time01_08_MqInitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MqInitialize();

    return osal_ret;
}

e_osal_return_t time01_09_IoInitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoInitialize();

    return osal_ret;
}

e_osal_return_t time01_10_PmInitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmInitialize();

    return osal_ret;
}

e_osal_return_t time01_11_InterruptInitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptInitialize();

    return osal_ret;
}

e_osal_return_t time01_12_MmngrInitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrInitialize();
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_initialized = true;
    }

    return osal_ret;
}
#endif  /* #ifdef RESOURCE_CHECK */


/* irq callback function */
static int64_t time_dummy_func(void *arg)
{
    return;
}

e_osal_return_t time02_01_ThreadCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;

    if (is_thread_created == false)
    {
        config.func       = time_dummy_func;
        config.userarg    = NULL;
        config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
        config.task_name  = "time_Thread";
        config.stack_size = 0x1000;

        osal_ret = R_OSAL_ThreadCreate(&config, OSAL_THREAD_CREATE_ID, &th_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_thread_created = true;
        }
    }
    
    return osal_ret;
}

e_osal_return_t time02_02_ThreadSleepUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);

    return osal_ret;
}

e_osal_return_t time02_03_ThreadSleepForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThreadSleepForTimePeriod(10);

    return osal_ret;
}

e_osal_return_t time02_04_ThreadSelf(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_thread_handle_t handle;

    osal_ret = R_OSAL_ThreadSelf(&handle);

    return osal_ret;
}

e_osal_return_t time02_05_ThreadEqual(void)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    bool                    result   = false;
    osal_thread_handle_t    th_handle2;

    th_handle2 = th_handle;
    osal_ret = R_OSAL_ThreadEqual(th_handle, th_handle2, &result);
    return osal_ret;
}

e_osal_return_t time02_06_ThreadJoin(void)
{
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    int64_t         return_value = 0;

    osal_ret = R_OSAL_ThreadJoin(th_handle, &return_value);

    return osal_ret;
}

e_osal_return_t time02_07_ThreadInitializeThreadConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;

    osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&config);

    return osal_ret;
}

e_osal_return_t time03_01_ThsyncMutexCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_mutex_created == false)
    {
        osal_ret = R_OSAL_ThsyncMutexCreate(OSAL_MUTEX_CREATE_ID, &mutex_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_mutex_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time03_02_ThsyncMutexDestroy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_mutex_created = false;
    }

    return osal_ret;
}

e_osal_return_t time03_03_ThsyncMutexLockUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time);

    return osal_ret;
}

e_osal_return_t time03_04_ThsyncMutexLockForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10);

    return osal_ret;
}

e_osal_return_t time03_05_ThsyncMutexTryLock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);

    return osal_ret;
}

e_osal_return_t time03_06_ThsyncMutexUnlock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    return osal_ret;
}

e_osal_return_t time03_07_ThsyncCondCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_cond_created == false)
    {
        osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_CREATE_ID, &cond_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_cond_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time03_08_ThsyncCondDestroy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_cond_created = false;
    }

    return osal_ret;
}

e_osal_return_t time03_09_ThsyncCondWaitForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 10);
    
    return osal_ret;
}

e_osal_return_t time03_10_ThsyncCondWaitUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;
    
    osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);

    return osal_ret;
}

e_osal_return_t time03_11_ThsyncCondBroadcast(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);

    return osal_ret;
}

e_osal_return_t time03_12_ThsyncCondSignal(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);

    return osal_ret;
}

e_osal_return_t time03_13_ThsyncMemoryBarrier(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMemoryBarrier();

    return osal_ret;
}

e_osal_return_t time04_01_MqCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_mq_created == false)
    {
        osal_ret = R_OSAL_MqCreate(&mq_config, OSAL_MQ_CREATE_ID, &mq_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_mq_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time04_02_MqDelete(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MqDelete(mq_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_mq_created = false;
    }

    return osal_ret;
}

e_osal_return_t time04_03_MqSendUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;
    char            send_data[50] = {0};

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;
    osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, &absolute_time, send_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t time04_04_MqSendForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            send_data[50] = {0};

    osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 10, send_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t time04_05_MqReceiveUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;
    char            recv_data[50];

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &absolute_time, recv_data, mq_config.msg_size);
    
    return osal_ret;
}

e_osal_return_t time04_06_MqReceiveForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            recv_data[50];

    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, recv_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t time04_07_MqIsFull(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result   = false;

    osal_ret = R_OSAL_MqIsFull(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t time04_08_MqIsEmpty(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result   = false;

    osal_ret = R_OSAL_MqIsEmpty(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t time04_09_MqGetConfig(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t config;

    osal_ret = R_OSAL_MqGetConfig(mq_handle, &config);

    return osal_ret;
}

e_osal_return_t time04_10_MqReset(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MqReset(mq_handle);

    return osal_ret;
}

e_osal_return_t time04_11_MqInitializeMqConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t config;

    osal_ret = R_OSAL_MqInitializeMqConfigSt(&config);

    return osal_ret;
}

e_osal_return_t time05_01_ClockTimeGetTimeStamp(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_clock_type_t clock_type = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    st_osal_time_t      absolute_time;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);

    return osal_ret;
}

e_osal_return_t time05_02_ClockTimeCalculateTimeDifference(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time1;
    st_osal_time_t  absolute_time2;
    osal_nano_sec_t different_time;

    absolute_time1.tv_sec = 1;
    absolute_time1.tv_nsec = 2000;
    absolute_time2.tv_sec = 2;
    absolute_time2.tv_nsec = 1000;

    osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time2, &absolute_time1, &different_time);

    return osal_ret;
}

e_osal_return_t time06_01_InterruptGetNumOfIrqChannels(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          num_channels;

    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);

    return osal_ret;
}

/* irq callback function */
void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
    return;
}

e_osal_return_t time06_02_InterruptRegisterIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        arg_data = 0;

    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                           (p_osal_isr_func_t)time_callback_func, (void*)&arg_data);

    return osal_ret;
}

e_osal_return_t time06_03_InterruptUnregisterIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)time_callback_func);

    return osal_ret;
}

e_osal_return_t time06_04_InterruptEnableIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);

    return osal_ret;
}

/* Note: Must be called just time06_04_InterruptEnableIsr */
e_osal_return_t time06_05_InterruptDisableIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);

    return osal_ret;
}

e_osal_return_t time06_06_InterruptIsISRContext(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result;

    osal_ret = R_OSAL_InterruptIsISRContext(&result);

    return osal_ret;
}

e_osal_return_t time06_07_InterruptSetIsrPriority(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);

    return osal_ret;
}

e_osal_return_t time07_01_IoGetNumOfDevices(void)
{
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    char            *device_type = TEST_DEVICE_TYPE;
    size_t          numOfDevices;

    if (is_io_numget == false)
    {
        osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
        if (osal_ret == OSAL_RETURN_OK)
        {
            is_io_numget = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time07_02_IoGetDeviceList(void)
{
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    char            *device_type = TEST_DEVICE_TYPE;
    char            list[400]     = {0};
    size_t          numOfLists;

    if (is_io_devlistget == false)
    {
        osal_ret  = R_OSAL_IoGetDeviceList(device_type, list, sizeof(list), &numOfLists);
        if (osal_ret == OSAL_RETURN_OK)
        {
            is_io_devlistget = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time07_03_IoDeviceOpen(char *device)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_io_opened == false)
    {
        osal_ret = R_OSAL_IoDeviceOpen(device, &dev_handle);
        if (osal_ret == OSAL_RETURN_OK)
        {
            is_io_opened = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time07_04_IoDeviceClose(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoDeviceClose(dev_handle);
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_io_numget = false;
        is_io_devlistget = false;
        is_io_opened = false;
    }

    return osal_ret;
}

e_osal_return_t time07_05_IoRead8(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint8_t         read_data = 0;

    osal_ret = R_OSAL_IoRead8(dev_handle, 0x00C0, &read_data);

    return osal_ret;
}

e_osal_return_t time07_06_IoRead16(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint16_t        read_data = 0;

    osal_ret = R_OSAL_IoRead16(dev_handle, 0x00C0, &read_data);

    return osal_ret;
}

e_osal_return_t time07_07_IoRead32(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint32_t        read_data = 0;

    osal_ret = R_OSAL_IoRead32(dev_handle, 0x00C0, &read_data);

    return osal_ret;
}

e_osal_return_t time07_08_IoRead64(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint64_t        read_data = 0;

    osal_ret = R_OSAL_IoRead64(dev_handle, 0x00C0, &read_data);
    
    return osal_ret;
}

e_osal_return_t time07_09_IoWrite8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite8(dev_handle, 0x00C0, 0xff);

    return osal_ret;
}

e_osal_return_t time07_10_IoWrite16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite16(dev_handle, 0x00C0, 0xff);
    
    return osal_ret;
}

e_osal_return_t time07_11_IoWrite32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite32(dev_handle, 0x00C0, 0xff);

    return osal_ret;
}

e_osal_return_t time07_12_IoWrite64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite64(dev_handle, 0x00C0, 0xff);

    return osal_ret;
}

e_osal_return_t time07_13_IoBlockRead8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t         read_data[16];

    osal_ret = R_OSAL_IoBlockRead8(dev_handle, 0x0034, read_data, 16);

    return osal_ret;
}

e_osal_return_t time07_14_IoBlockRead16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t        read_data[8];

    osal_ret = R_OSAL_IoBlockRead16(dev_handle, 0x0034, read_data, 16);

    return osal_ret;
}

e_osal_return_t time07_15_IoBlockRead32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        read_data[4];

    osal_ret = R_OSAL_IoBlockRead32(dev_handle, 0x0034, read_data, 16);

    return osal_ret;
}

e_osal_return_t time07_16_IoBlockRead64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t        read_data[2];

    osal_ret = R_OSAL_IoBlockRead64(dev_handle, 0x00C0, read_data, 16);
    
    return osal_ret;
}

e_osal_return_t time07_17_IoBlockWrite8(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint8_t         p_data[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    osal_ret = R_OSAL_IoBlockWrite8(dev_handle, 0x0034, p_data, 16);

    return osal_ret;
}

e_osal_return_t time07_18_IoBlockWrite16(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint16_t        p_data[8] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000, 0x0100, 0x0000};

    osal_ret = R_OSAL_IoBlockWrite16(dev_handle, 0x0034, p_data, 16);

    return osal_ret;
}

e_osal_return_t time07_19_IoBlockWrite32(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint32_t        p_data[4] = {0x00000000, 0x00000000, 0x00000040, 0x00000100};

    osal_ret = R_OSAL_IoBlockWrite32(dev_handle, 0x0034, p_data, 16);

    return osal_ret;
}

e_osal_return_t time07_20_IoBlockWrite64(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint64_t        p_data[2] = {0x000000FF000000FF, 0x0000000000000000};

    osal_ret = R_OSAL_IoBlockWrite64(dev_handle, 0x00C0, p_data, 16);
    
    return osal_ret;
}

e_osal_return_t time07_21_IoGetDeviceAxiBusId(void)
{
    e_osal_return_t   osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetDeviceAxiBusId(dev_handle, &axi_id);

    return osal_ret;
}

e_osal_return_t time07_22_IoGetAxiBusIdFromDeviceName(void)
{
    e_osal_return_t   osal_ret = OSAL_RETURN_OK;
    char              *device  = TEST_DEVICE_NAME;
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName(device, &axi_id);

    return osal_ret;
}

e_osal_return_t time07_23_IoGetAxiBusNameList(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            list[400] = {0};
    size_t          num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusNameList(list, sizeof(list), &num_of_byte);

    return osal_ret;
}

e_osal_return_t time07_24_IoGetAxiBusName(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t   axi_id   = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    char                name[200];
    size_t              num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, name, sizeof(name), &num_of_byte);

    return osal_ret;
}

e_osal_return_t time07_25_IoGetAxiIdListForDeviceHdl(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t array[32];
    size_t array_size = 32;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl(dev_handle, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_26_IoGetAxiIdListForDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            *device  = TEST_DEVICE_NAME;
    osal_axi_bus_id_t array[32];
    size_t array_size = 32;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(device, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_27_IoGetAxiIdListForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = TEST_AXI_BUS_NAME;
    osal_axi_bus_id_t array[10];
    size_t  array_size = 10;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_28_IoGetNumOfAxiIdsForDeviceHdl(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(dev_handle, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_29_IoGetNumOfAxiIdsForDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            *device  = TEST_DEVICE_NAME;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(device, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_30_IoGetNumOfAxiIdsForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = TEST_AXI_BUS_NAME;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name, &num_of_axi_ids);

    return osal_ret;
}
e_osal_return_t time08_01_PmSetRequiredState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

    return osal_ret;
}

e_osal_return_t time08_02_PmGetRequiredState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_required_state_t req_state;

    osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);

    return osal_ret;
}

e_osal_return_t time08_03_PmWaitForState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_CG, 10);

    return osal_ret;
}

e_osal_return_t time08_04_PmGetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t pm_state;

    osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);

    return osal_ret;
}

e_osal_return_t time08_05_PmGetLowestIntermediatePowerState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t pm_state;

    osal_ret = R_OSAL_PmGetLowestIntermediatePowerState(dev_handle, &pm_state);

    return osal_ret;
}

e_osal_return_t time08_06_PmSetPolicy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);

    return osal_ret;
}

e_osal_return_t time08_07_PmGetPolicy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_policy_t pm_policy;

    osal_ret = R_OSAL_PmGetPolicy(dev_handle, &pm_policy);

    return osal_ret;
}

e_osal_return_t time08_08_PmSetResetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);

    return osal_ret;
}

e_osal_return_t time08_09_PmGetResetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_reset_state_t reset_state;

    osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);

    return osal_ret;
}

e_osal_return_t time08_10_PmSetPostClock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool     clock_enable = false;
    uint32_t clock_driver = 10;

    osal_ret = R_OSAL_PmSetPostClock(dev_handle, clock_enable, clock_driver);

    return osal_ret;
}

e_osal_return_t time09_01_MmngrOpen(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    if (mmgr_handle == NULL)
    {
        config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
        config.memory_limit                = 4096;
        config.max_allowed_allocations     = 16;
        config.max_registered_monitors_cbs = 100;

        osal_ret = R_OSAL_MmngrOpen(&config, &mmgr_handle);
    }

    return osal_ret;
}

e_osal_return_t time09_02_MmngrClose(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrClose(mmgr_handle);
    mmgr_handle = NULL;

    return osal_ret;
}
    
e_osal_return_t time09_03_MmngrAlloc(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t size      = 4096;
    size_t alignment = 4096;

    if (buf_handle == NULL)
    {
        osal_ret = R_OSAL_MmngrAlloc(mmgr_handle, size, alignment, &buf_handle);
    }

    return osal_ret;
}

e_osal_return_t time09_04_MmngrDealloc(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDealloc(mmgr_handle, buf_handle);
    buf_handle = NULL;

    return osal_ret;
}

e_osal_return_t time09_05_MmngrGetConfig(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED; 
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrGetConfig(mmgr_handle, &config);

    return osal_ret;
}

e_osal_return_t time09_06_MmngrGetOsalMaxConfig(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED; 
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);

    return osal_ret;
}

e_osal_return_t time09_07_MmngrInitializeMmngrConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED; 
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(&config);

    return osal_ret;
}

void time_callback_mmgr(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    return;
}

e_osal_return_t time09_08_MmngrRegisterMonitor(void)
{
    e_osal_return_t      osal_ret        = OSAL_RETURN_OK;
    e_osal_mmngr_event_t monitored_event = OSAL_MMNGR_EVENT_ALLOCATE;
    void  *user_arg;

    osal_ret = R_OSAL_MmngrRegisterMonitor(mmgr_handle, monitored_event, time_callback_mmgr, &user_arg);

    return osal_ret;
}

e_osal_return_t time09_09_MmngrGetCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    void            *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t time09_10_MmngrGetConstCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    const void      *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t time09_11_MmngrFlush(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          offset   = 0x00;
    size_t          size     = 0x40;

    osal_ret = R_OSAL_MmngrFlush(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t time09_12_MmngrInvalidate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          offset   = 0x00;
    size_t          size     = 0x40;

    osal_ret = R_OSAL_MmngrInvalidate(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t time09_13_MmngrGetHwAddr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    uintptr_t hw_addr;

    osal_ret = R_OSAL_MmngrGetHwAddr(buf_handle, axi_id, &hw_addr);

    return osal_ret;
}

e_osal_return_t time09_14_MmngrGetSize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          size;

    osal_ret = R_OSAL_MmngrGetSize(buf_handle, &size);

    return osal_ret;
}

e_osal_return_t time09_15_MmngrCheckBuffer(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrCheckBuffer(buf_handle);

    return osal_ret;
}

e_osal_return_t time09_16_MmngrDebugMmngrPrint(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDebugMmngrPrint(mmgr_handle, stdout);

    return osal_ret;
}

e_osal_return_t time09_17_MmngrDebugBufferPrint(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDebugBufferPrint(buf_handle, stdout);

    return osal_ret;
}

e_osal_return_t time09_18_MmngrInitializeMemoryBufferObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_buffer_t buffer_obj;

    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&buffer_obj);

    return osal_ret;
}

e_osal_return_t time09_19_MmngrInitializeMemoryManagerObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_manager_t memory_obj;

    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&memory_obj);

    return osal_ret;
}

e_osal_return_t time09_20_MmngrOpenFromRegionIdx(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    if (mmgr_handle == NULL)
    {
        config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
        config.memory_limit                = 4096;
        config.max_allowed_allocations     = 16;
        config.max_registered_monitors_cbs = 100;

        osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, 0, &mmgr_handle);
    }

    return osal_ret;
}

e_osal_return_t time09_21_MmngrGetOsalMaxConfigFromRegionIdx(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED; 
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(0, &config);

    return osal_ret;
}
