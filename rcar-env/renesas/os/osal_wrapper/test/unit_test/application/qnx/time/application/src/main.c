
#include "stub.h"
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "target/qnx/r_osal_os_type.h"

e_osal_return_t time_test_all(int func_id);
e_osal_return_t time01_01_Initialize(void);
e_osal_return_t time01_02_Deinitialize(void);
e_osal_return_t time01_03_GetVersion(void);
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
e_osal_return_t time09_18_MmngrInitializeMemoryManagerObj(void);
e_osal_return_t time09_19_MmngrInitializeMemoryBufferObj(void);
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
#define TEST_NUM (104)

const char api_list[TEST_NUM][120] = {
    "R_OSAL_Initialize    :  \0",
    "R_OSAL_ThreadCreate    :  \0",
    "R_OSAL_ThreadSleepUntilTimeStamp    :  \0",
    "R_OSAL_ThreadSleepForTimePeriod    :  \0",
    "R_OSAL_ThreadSelf    :  \0",
    "R_OSAL_ThreadEqual    :  \0",
    "R_OSAL_ThreadJoin    :  \0",
    "R_OSAL_ThreadInitializeThreadConfigSt    :  \0",
    "R_OSAL_ThsyncMutexCreate    :  \0",
    "R_OSAL_ThsyncMutexLockUntilTimeStamp    :  \0",
    "R_OSAL_ThsyncMutexUnlock    :  \0",
    "R_OSAL_ThsyncMutexLockForTimePeriod    :  \0",
    "R_OSAL_ThsyncMutexTryLock    :  \0",
    "R_OSAL_ThsyncCondCreate    :  \0",
    "R_OSAL_ThsyncCondWaitForTimePeriod    :  \0",
    "R_OSAL_ThsyncCondWaitUntilTimeStamp    :  \0",
    "R_OSAL_ThsyncCondBroadcast    :  \0",
    "R_OSAL_ThsyncCondSignal    :  \0",
    "R_OSAL_ThsyncCondDestroy    :  \0",
    "R_OSAL_ThsyncMutexDestroy    :  \0",
    "R_OSAL_ThsyncMemoryBarrier    :  \0",
    "R_OSAL_MqCreate    :  \0",
    "R_OSAL_MqSendUntilTimeStamp    :  \0",
    "R_OSAL_MqReceiveUntilTimeStamp    :  \0",
    "R_OSAL_MqSendForTimePeriod    :  \0",
    "R_OSAL_MqReceiveForTimePeriod    :  \0",
    "R_OSAL_MqIsFull    :  \0",
    "R_OSAL_MqIsEmpty    :  \0",
    "R_OSAL_MqGetConfig    :  \0",
    "R_OSAL_MqReset    :  \0",
    "R_OSAL_MqInitializeMqConfigSt    :  \0",
    "R_OSAL_MqDelete    :  \0",
    "R_OSAL_ClockTimeGetTimeStamp    :  \0",
    "R_OSAL_ClockTimeCalculateTimeDifference    :  \0",
    "R_OSAL_IoGetNumOfDevices    :  \0",
    "R_OSAL_IoGetDeviceList    :  \0",
    "R_OSAL_IoDeviceOpen    :  \0",
    "R_OSAL_IoRead8    :  \0",
    "R_OSAL_IoRead16    :  \0",
    "R_OSAL_IoRead32    :  \0",
    "R_OSAL_IoRead64    :  \0",
    "R_OSAL_IoWrite8    :  \0",
    "R_OSAL_IoWrite16    :  \0",
    "R_OSAL_IoWrite32    :  \0",
    "R_OSAL_IoWrite64    :  \0",
    "R_OSAL_IoBlockRead8    :  \0",
    "R_OSAL_IoBlockRead16    :  \0",
    "R_OSAL_IoBlockRead32    :  \0",
    "R_OSAL_IoBlockRead64    :  \0",
    "R_OSAL_IoBlockWrite8    :  \0",
    "R_OSAL_IoBlockWrite16    :  \0",
    "R_OSAL_IoBlockWrite32    :  \0",
    "R_OSAL_IoBlockWrite64    :  \0",
    "R_OSAL_IoGetDeviceAxiBusId    :  \0",
    "R_OSAL_IoGetAxiBusIdFromDeviceName    :  \0",
    "R_OSAL_IoGetAxiBusNameList    :  \0",
    "R_OSAL_IoGetAxiBusName    :  \0",
    "R_OSAL_IoGetAxiIdListForDeviceHdl    :  \0",
    "R_OSAL_IoGetAxiIdListForDeviceName    :  \0",
    "R_OSAL_IoGetAxiIdListForAxiBusName    :  \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceHdl    :  \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceName    :  \0",
    "R_OSAL_IoGetNumOfAxiIdsForAxiBusName    :  \0",
    "R_OSAL_InterruptGetNumOfIrqChannels    :  \0",
    "R_OSAL_InterruptRegisterIsr    :  \0",
    "R_OSAL_InterruptEnableIsr    :  \0",
    "R_OSAL_InterruptDisableIsr    :  \0",
    "R_OSAL_InterruptUnregisterIsr    :  \0",
    "R_OSAL_InterruptIsISRContext    :  \0",
    "R_OSAL_InterruptSetIsrPriority    :  \0",
    "R_OSAL_IoDeviceClose    :  \0",
    "R_OSAL_PmSetRequiredState    :  \0",
    "R_OSAL_PmGetRequiredState    :  \0",
    "R_OSAL_PmWaitForState    :  \0",
    "R_OSAL_PmGetState    :  \0",
    "R_OSAL_PmGetLowestIntermediatePowerState    :  \0",
    "R_OSAL_PmSetPolicy    :  \0",
    "R_OSAL_PmGetPolicy    :  \0",
    "R_OSAL_PmSetResetState    :  \0",
    "R_OSAL_PmGetResetState    :  \0",
    "R_OSAL_PmSetPostClock    :  \0",
    "R_OSAL_MmngrOpen    :  \0",
    "R_OSAL_MmngrAlloc    :  \0",
    "R_OSAL_MmngrGetConfig    :  \0",
    "R_OSAL_MmngrGetOsalMaxConfig    :  \0",
    "R_OSAL_MmngrInitializeMmngrConfigSt    :  \0",
    "R_OSAL_MmngrRegisterMonitor    :  \0",
    "R_OSAL_MmngrGetCpuPtr    :  \0",
    "R_OSAL_MmngrGetConstCpuPtr    :  \0",
    "R_OSAL_MmngrFlush    :  \0",
    "R_OSAL_MmngrInvalidate    :  \0",
    "R_OSAL_MmngrGetHwAddr    :  \0",
    "R_OSAL_MmngrGetSize    :  \0",
    "R_OSAL_MmngrCheckBuffer    :  \0",
    "R_OSAL_MmngrDebugMmngrPrint    :  \0",
    "R_OSAL_MmngrDebugBufferPrint    :  \0",
    "R_OSAL_MmngrDealloc    :  \0",
    "R_OSAL_MmngrClose    :  \0",
    "R_OSAL_MmngrOpenFromRegionIdx    :  \0",
    "R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx    :  \0",
    "R_OSAL_MmngrInitializeMemoryManagerObj    :  \0",
    "R_OSAL_MmngrInitializeMemoryBufferObj    :  \0",
    "R_OSAL_GetVersion    :  \0",
    "R_OSAL_Deinitialize    :  \0",
};

int func_count = 0;
int test_count = 0;
uint64_t elapsed_time[TEST_NUM];

#if defined(RCAR_S4)
uint64_t expected_time[TEST_NUM] = {
    3958175,        // time01_01_Initialize
    33333,          // time02_01_ThreadCreate
    8333,           // time02_02_ThreadSleepUntilTimeStamp
    8333,           // time02_03_ThreadSleepForTimePeriod
    8333,           // time02_04_ThreadSelf
    8333,           // time02_05_ThreadEqual
    8333,           // time02_06_ThreadJoin
    8333,           // time02_07_ThreadInitializeThreadConfigSt
    8333,           // time03_01_ThsyncMutexCreate
    8333,           // time03_03_ThsyncMutexLockUntilTimeStamp
    8333,           // time03_06_ThsyncMutexUnlock
    8333,           // time03_04_ThsyncMutexLockForTimePeriod
    8333,           // time03_05_ThsyncMutexTryLock
    8333,           // time03_07_ThsyncCondCreate
    8333,           // time03_09_ThsyncCondWaitForTimePeriod
    8333,           // time03_10_ThsyncCondWaitUntilTimeStamp
    8333,           // time03_11_ThsyncCondBroadcast
    8333,           // time03_12_ThsyncCondSignal
    8333,           // time03_08_ThsyncCondDestroy
    8333,           // time03_02_ThsyncMutexDestroy
    8333,           // time03_13_ThsyncMemoryBarrier
    8333,           // time04_01_MqCreate
    8333,           // time04_03_MqSendUntilTimeStamp
    8333,           // time04_05_MqReceiveUntilTimeStamp
    8333,           // time04_04_MqSendForTimePeriod
    8333,           // time04_06_MqReceiveForTimePeriod
    8333,           // time04_07_MqIsFull
    8333,           // time04_08_MqIsEmpty
    8333,           // time04_09_MqGetConfig
    8333,           // time04_10_MqReset
    8333,           // time04_11_MqInitializeMqConfigSt
    8333,           // time04_02_MqDelete
    8333,           // time05_01_ClockTimeGetTimeStamp
    8333,           // time05_02_ClockTimeCalculateTimeDifference
    891631,         // time07_01_IoGetNumOfDevices
    891631,         // time07_02_IoGetDeviceList
    8333,           // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    8333,           // time07_05_IoRead8
    8333,           // time07_06_IoRead16
    8333,           // time07_07_IoRead32
    8333,           // time07_08_IoRead64
    8333,           // time07_09_IoWrite8
    8333,           // time07_10_IoWrite16
    8333,           // time07_11_IoWrite32
    8333,           // time07_12_IoWrite64
    8333,           // time07_13_IoBlockRead8
    8333,           // time07_14_IoBlockRead16
    8333,           // time07_15_IoBlockRead32
    8333,           // time07_16_IoBlockRead64
    8333,           // time07_17_IoBlockWrite8
    8333,           // time07_18_IoBlockWrite16
    8333,           // time07_19_IoBlockWrite32
    8333,           // time07_20_IoBlockWrite64
    549978,         // time07_21_IoGetDeviceAxiBusId
    549978,         // time07_22_IoGetAxiBusIdFromDeviceName
    558311,         // time07_23_IoGetAxiBusNameList
    8333,           // time07_24_IoGetAxiBusName
    549978,         // time07_25_IoGetAxiIdListForDeviceHdl
    549978,         // time07_26_IoGetAxiIdListForDeviceName
    558311,         // time07_27_IoGetAxiIdListForAxiBusName
    8333,           // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    549978,         // time07_29_IoGetNumOfAxiIdsForDeviceName
    558311,         // time07_30_IoGetNumOfAxiIdsForAxiBusName
    8333,           // time06_01_InterruptGetNumOfIrqChannels
    8333,           // time06_02_InterruptRegisterIsr
    8333,           // time06_04_InterruptEnableIsr
    8333,           // time06_05_InterruptDisableIsr
    8333,           // time06_03_InterruptUnregisterIsr
    8333,           // time06_06_InterruptIsISRContext
    8333,           // time06_07_InterruptSetIsrPriority
    8333,           // time07_04_IoDeviceClose
    33333,          // time08_01_PmSetRequiredState
    8333,           // time08_02_PmGetRequiredState
    8333,           // time08_03_PmWaitForState
    8333,           // time08_04_PmGetState
    8333,           // time08_05_PmGetLowestIntermediatePowerState
    8333,           // time08_06_PmSetPolicy
    8333,           // time08_07_PmGetPolicy
    8333,           // time08_08_PmSetResetState
    8333,           // time08_09_PmGetResetState
    8333,           // time08_10_PmSetPostClock
    50000,          // time09_01_MmngrOpen
    50000,          // time09_03_MmngrAlloc
    8333,           // time09_05_MmngrGetConfig
    8333,           // time09_06_MmngrGetOsalMaxConfig
    8333,           // time09_07_MmngrInitializeMmngrConfigSt
    8333,           // time09_08_MmngrRegisterMonitor
    8333,           // time09_09_MmngrGetCpuPtr
    8333,           // time09_10_MmngrGetConstCpuPtr
    8333,           // time09_11_MmngrFlush
    8333,           // time09_12_MmngrInvalidate
    8333,           // time09_13_MmngrGetHwAddr
    8333,           // time09_14_MmngrGetSize
    50000,          // time09_15_MmngrCheckBuffer
    8333,           // time09_16_MmngrDebugMmngrPrint
    8333,           // time09_17_MmngrDebugBufferPrint
    8333,           // time09_04_MmngrDealloc
    8333,           // time09_02_MmngrClose
    50000,          // time09_20_MmngrOpenFromRegionIdx
    8333,           // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    8333,           // time09_18_MmngrInitializeMemoryManagerObj
    8333,           // time09_19_MmngrInitializeMemoryBufferObj
    8333,           // time01_03_GetVersion
    7841353         // time01_02_Deinitialize
};
#elif defined(RCAR_V4H)
uint64_t expected_time[TEST_NUM] = {
    3958175,        // time01_01_Initialize
    11765,          // time02_01_ThreadCreate
    5882,           // time02_02_ThreadSleepUntilTimeStamp
    5882,           // time02_03_ThreadSleepForTimePeriod
    5882,           // time02_04_ThreadSelf
    5882,           // time02_05_ThreadEqual
    5882,           // time02_06_ThreadJoin
    5882,           // time02_07_ThreadInitializeThreadConfigSt
    5882,           // time03_01_ThsyncMutexCreate
    5882,           // time03_03_ThsyncMutexLockUntilTimeStamp
    5882,           // time03_06_ThsyncMutexUnlock
    5882,           // time03_04_ThsyncMutexLockForTimePeriod
    5882,           // time03_05_ThsyncMutexTryLock
    5882,           // time03_07_ThsyncCondCreate
    5882,           // time03_09_ThsyncCondWaitForTimePeriod
    5882,           // time03_10_ThsyncCondWaitUntilTimeStamp
    5882,           // time03_11_ThsyncCondBroadcast
    5882,           // time03_12_ThsyncCondSignal
    5882,           // time03_08_ThsyncCondDestroy
    5882,           // time03_02_ThsyncMutexDestroy
    5882,           // time03_13_ThsyncMemoryBarrier
    5882,           // time04_01_MqCreate
    5882,           // time04_03_MqSendUntilTimeStamp
    5882,           // time04_05_MqReceiveUntilTimeStamp
    5882,           // time04_04_MqSendForTimePeriod
    5882,           // time04_06_MqReceiveForTimePeriod
    5882,           // time04_07_MqIsFull
    5882,           // time04_08_MqIsEmpty
    5882,           // time04_09_MqGetConfig
    5882,           // time04_10_MqReset
    5882,           // time04_11_MqInitializeMqConfigSt
    5882,           // time04_02_MqDelete
    5882,           // time05_01_ClockTimeGetTimeStamp
    5882,           // time05_02_ClockTimeCalculateTimeDifference
    1564612,        // time07_01_IoGetNumOfDevices
    1564612,        // time07_02_IoGetDeviceList
    5882,           // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    5882,           // time07_05_IoRead8
    5882,           // time07_06_IoRead16
    5882,           // time07_07_IoRead32
    5882,           // time07_08_IoRead64
    5882,           // time07_09_IoWrite8
    5882,           // time07_10_IoWrite16
    5882,           // time07_11_IoWrite32
    5882,           // time07_12_IoWrite64
    5882,           // time07_13_IoBlockRead8
    5882,           // time07_14_IoBlockRead16
    5882,           // time07_15_IoBlockRead32
    5882,           // time07_16_IoBlockRead64
    5882,           // time07_17_IoBlockWrite8
    5882,           // time07_18_IoBlockWrite16
    5882,           // time07_19_IoBlockWrite32
    5882,           // time07_20_IoBlockWrite64
    729368,         // time07_21_IoGetDeviceAxiBusId
    729368,         // time07_22_IoGetAxiBusIdFromDeviceName
    735250,         // time07_23_IoGetAxiBusNameList
    5882,           // time07_24_IoGetAxiBusName
    729368,         // time07_25_IoGetAxiIdListForDeviceHdl
    729368,         // time07_26_IoGetAxiIdListForDeviceName
    735250,         // time07_27_IoGetAxiIdListForAxiBusName
    5882,           // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    729368,         // time07_29_IoGetNumOfAxiIdsForDeviceName
    735250,         // time07_30_IoGetNumOfAxiIdsForAxiBusName
    5882,           // time06_01_InterruptGetNumOfIrqChannels
    5882,           // time06_02_InterruptRegisterIsr
    5882,           // time06_04_InterruptEnableIsr
    5882,           // time06_05_InterruptDisableIsr
    5882,           // time06_03_InterruptUnregisterIsr
    5882,           // time06_06_InterruptIsISRContext
    5882,           // time06_07_InterruptSetIsrPriority
    5882,           // time07_04_IoDeviceClose
    11765,          // time08_01_PmSetRequiredState
    5882,           // time08_02_PmGetRequiredState
    5882,           // time08_03_PmWaitForState
    5882,           // time08_04_PmGetState
    5882,           // time08_05_PmGetLowestIntermediatePowerState
    5882,           // time08_06_PmSetPolicy
    5882,           // time08_07_PmGetPolicy
    5882,           // time08_08_PmSetResetState
    5882,           // time08_09_PmGetResetState
    5882,           // time08_10_PmSetPostClock
    17647,          // time09_01_MmngrOpen
    17647,          // time09_03_MmngrAlloc
    5882,           // time09_05_MmngrGetConfig
    5882,           // time09_06_MmngrGetOsalMaxConfig
    5882,           // time09_07_MmngrInitializeMmngrConfigSt
    5882,           // time09_08_MmngrRegisterMonitor
    5882,           // time09_09_MmngrGetCpuPtr
    5882,           // time09_10_MmngrGetConstCpuPtr
    5882,           // time09_11_MmngrFlush
    5882,           // time09_12_MmngrInvalidate
    5882,           // time09_13_MmngrGetHwAddr
    5882,           // time09_14_MmngrGetSize
    17647,          // time09_15_MmngrCheckBuffer
    5882,           // time09_16_MmngrDebugMmngrPrint
    5882,           // time09_17_MmngrDebugBufferPrint
    5882,           // time09_04_MmngrDealloc
    5882,           // time09_02_MmngrClose
    17647,          // time09_20_MmngrOpenFromRegionIdx
    5882,           // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    5882,           // time09_18_MmngrInitializeMemoryManagerObj
    5882,           // time09_19_MmngrInitializeMemoryBufferObj
    5882,           // time01_03_GetVersion
    13216854        // time01_02_Deinitialize
};
#elif defined(RCAR_V4M)
uint64_t expected_time[TEST_NUM] = {
    9960000,        // time01_01_Initialize
    20001,          // time02_01_ThreadCreate
    10000,          // time02_02_ThreadSleepUntilTimeStamp
    10000,          // time02_03_ThreadSleepForTimePeriod
    10000,          // time02_04_ThreadSelf
    10000,          // time02_05_ThreadEqual
    10000,          // time02_06_ThreadJoin
    10000,          // time02_07_ThreadInitializeThreadConfigSt
    10000,          // time03_01_ThsyncMutexCreate
    10000,          // time03_03_ThsyncMutexLockUntilTimeStamp
    10000,          // time03_06_ThsyncMutexUnlock
    10000,          // time03_04_ThsyncMutexLockForTimePeriod
    10000,          // time03_05_ThsyncMutexTryLock
    10000,          // time03_07_ThsyncCondCreate
    10000,          // time03_09_ThsyncCondWaitForTimePeriod
    10000,          // time03_10_ThsyncCondWaitUntilTimeStamp
    10000,          // time03_11_ThsyncCondBroadcast
    10000,          // time03_12_ThsyncCondSignal
    10000,          // time03_08_ThsyncCondDestroy
    10000,          // time03_02_ThsyncMutexDestroy
    10000,          // time03_13_ThsyncMemoryBarrier
    10000,          // time04_01_MqCreate
    10000,          // time04_03_MqSendUntilTimeStamp
    10000,          // time04_05_MqReceiveUntilTimeStamp
    10000,          // time04_04_MqSendForTimePeriod
    10000,          // time04_06_MqReceiveForTimePeriod
    10000,          // time04_07_MqIsFull
    10000,          // time04_08_MqIsEmpty
    10000,          // time04_09_MqGetConfig
    10000,          // time04_10_MqReset
    10000,          // time04_11_MqInitializeMqConfigSt
    10000,          // time04_02_MqDelete
    10000,          // time05_01_ClockTimeGetTimeStamp
    10000,          // time05_02_ClockTimeCalculateTimeDifference
    2110000,        // time07_01_IoGetNumOfDevices
    2110000,        // time07_02_IoGetDeviceList
    10000,          // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    10000,          // time07_05_IoRead8
    10000,          // time07_06_IoRead16
    10000,          // time07_07_IoRead32
    10000,          // time07_08_IoRead64
    10000,          // time07_09_IoWrite8
    10000,          // time07_10_IoWrite16
    10000,          // time07_11_IoWrite32
    10000,          // time07_12_IoWrite64
    10000,          // time07_13_IoBlockRead8
    10000,          // time07_14_IoBlockRead16
    10000,          // time07_15_IoBlockRead32
    10000,          // time07_16_IoBlockRead64
    10000,          // time07_17_IoBlockWrite8
    10000,          // time07_18_IoBlockWrite16
    10000,          // time07_19_IoBlockWrite32
    10000,          // time07_20_IoBlockWrite64
    1130000,        // time07_21_IoGetDeviceAxiBusId
    1130000,        // time07_22_IoGetAxiBusIdFromDeviceName
    1140000,        // time07_23_IoGetAxiBusNameList
    10000,          // time07_24_IoGetAxiBusName
    1130000,        // time07_25_IoGetAxiIdListForDeviceHdl
    1130000,        // time07_26_IoGetAxiIdListForDeviceName
    1140000,        // time07_27_IoGetAxiIdListForAxiBusName
    10000,          // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    1130000,        // time07_29_IoGetNumOfAxiIdsForDeviceName
    1140000,        // time07_30_IoGetNumOfAxiIdsForAxiBusName
    10000,          // time06_01_InterruptGetNumOfIrqChannels
    10000,          // time06_02_InterruptRegisterIsr
    10000,          // time06_04_InterruptEnableIsr
    10000,          // time06_05_InterruptDisableIsr
    10000,          // time06_03_InterruptUnregisterIsr
    10000,          // time06_06_InterruptIsISRContext
    10000,          // time06_07_InterruptSetIsrPriority
    10000,          // time07_04_IoDeviceClose
    20001,          // time08_01_PmSetRequiredState
    10000,          // time08_02_PmGetRequiredState
    10000,          // time08_03_PmWaitForState
    10000,          // time08_04_PmGetState
    10000,          // time08_05_PmGetLowestIntermediatePowerState
    10000,          // time08_06_PmSetPolicy
    10000,          // time08_07_PmGetPolicy
    10000,          // time08_08_PmSetResetState
    10000,          // time08_09_PmGetResetState
    10000,          // time08_10_PmSetPostClock
    30000,          // time09_01_MmngrOpen
    30000,          // time09_03_MmngrAlloc
    10000,          // time09_05_MmngrGetConfig
    10000,          // time09_06_MmngrGetOsalMaxConfig
    10000,          // time09_07_MmngrInitializeMmngrConfigSt
    10000,          // time09_08_MmngrRegisterMonitor
    10000,          // time09_09_MmngrGetCpuPtr
    10000,          // time09_10_MmngrGetConstCpuPtr
    10000,          // time09_11_MmngrFlush
    10000,          // time09_12_MmngrInvalidate
    10000,          // time09_13_MmngrGetHwAddr
    10000,          // time09_14_MmngrGetSize
    30000,          // time09_15_MmngrCheckBuffer
    10000,          // time09_16_MmngrDebugMmngrPrint
    10000,          // time09_17_MmngrDebugBufferPrint
    10000,          // time09_04_MmngrDealloc
    10000,          // time09_02_MmngrClose
    30000,          // time09_20_MmngrOpenFromRegionIdx
    10000,          // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    10000,          // time09_18_MmngrInitializeMemoryManagerObj
    10000,          // time09_19_MmngrInitializeMemoryBufferObj
    10000,          // time01_03_GetVersion
    18530000,       // time01_02_Deinitialize
};
#else    /* V3M, V3Hx */
uint64_t expected_time[TEST_NUM] = {
    15000000,       // time01_01_Initialize
    15000000,       // time02_01_ThreadCreate
    15000000,       // time02_02_ThreadSleepUntilTimeStamp
    15000000,       // time02_03_ThreadSleepForTimePeriod
    15000000,       // time02_04_ThreadSelf
    15000000,       // time02_05_ThreadEqual
    15000000,       // time02_06_ThreadJoin
    15000000,       // time02_07_ThreadInitializeThreadConfigSt
    15000000,       // time03_01_ThsyncMutexCreate
    15000000,       // time03_03_ThsyncMutexLockUntilTimeStamp
    15000000,       // time03_06_ThsyncMutexUnlock
    15000000,       // time03_04_ThsyncMutexLockForTimePeriod
    15000000,       // time03_05_ThsyncMutexTryLock
    15000000,       // time03_07_ThsyncCondCreate
    15000000,       // time03_09_ThsyncCondWaitForTimePeriod
    15000000,       // time03_10_ThsyncCondWaitUntilTimeStamp
    15000000,       // time03_11_ThsyncCondBroadcast
    15000000,       // time03_12_ThsyncCondSignal
    15000000,       // time03_08_ThsyncCondDestroy
    15000000,       // time03_02_ThsyncMutexDestroy
    15000000,       // time03_13_ThsyncMemoryBarrier
    15000000,       // time04_01_MqCreate
    15000000,       // time04_03_MqSendUntilTimeStamp
    15000000,       // time04_05_MqReceiveUntilTimeStamp
    15000000,       // time04_04_MqSendForTimePeriod
    15000000,       // time04_06_MqReceiveForTimePeriod
    15000000,       // time04_07_MqIsFull
    15000000,       // time04_08_MqIsEmpty
    15000000,       // time04_09_MqGetConfig
    15000000,       // time04_10_MqReset
    15000000,       // time04_11_MqInitializeMqConfigSt
    15000000,       // time04_02_MqDelete
    15000000,       // time05_01_ClockTimeGetTimeStamp
    15000000,       // time05_02_ClockTimeCalculateTimeDifference
    15000000,       // time07_01_IoGetNumOfDevices
    15000000,       // time07_02_IoGetDeviceList
    15000000,       // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    15000000,       // time07_05_IoRead8
    15000000,       // time07_06_IoRead16
    15000000,       // time07_07_IoRead32
    15000000,       // time07_08_IoRead64
    15000000,       // time07_09_IoWrite8
    15000000,       // time07_10_IoWrite16
    15000000,       // time07_11_IoWrite32
    15000000,       // time07_12_IoWrite64
    15000000,       // time07_13_IoBlockRead8
    15000000,       // time07_14_IoBlockRead16
    15000000,       // time07_15_IoBlockRead32
    15000000,       // time07_16_IoBlockRead64
    15000000,       // time07_17_IoBlockWrite8
    15000000,       // time07_18_IoBlockWrite16
    15000000,       // time07_19_IoBlockWrite32
    15000000,       // time07_20_IoBlockWrite64
    15000000,       // time07_21_IoGetDeviceAxiBusId
    15000000,       // time07_22_IoGetAxiBusIdFromDeviceName
    15000000,       // time07_23_IoGetAxiBusNameList
    15000000,       // time07_24_IoGetAxiBusName
    15000000,       // time07_25_IoGetAxiIdListForDeviceHdl
    15000000,       // time07_26_IoGetAxiIdListForDeviceName
    15000000,       // time07_27_IoGetAxiIdListForAxiBusName
    15000000,       // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    15000000,       // time07_29_IoGetNumOfAxiIdsForDeviceName
    15000000,       // time07_30_IoGetNumOfAxiIdsForAxiBusName
    15000000,       // time06_01_InterruptGetNumOfIrqChannels
    15000000,       // time06_02_InterruptRegisterIsr
    15000000,       // time06_04_InterruptEnableIsr
    15000000,       // time06_05_InterruptDisableIsr
    15000000,       // time06_03_InterruptUnregisterIsr
    15000000,       // time06_06_InterruptIsISRContext
    15000000,       // time06_07_InterruptSetIsrPriority
    15000000,       // time07_04_IoDeviceClose
    15000000,       // time08_01_PmSetRequiredState
    15000000,       // time08_02_PmGetRequiredState
    15000000,       // time08_03_PmWaitForState
    15000000,       // time08_04_PmGetState
    15000000,       // time08_05_PmGetLowestIntermediatePowerState
    15000000,       // time08_06_PmSetPolicy
    15000000,       // time08_07_PmGetPolicy
    15000000,       // time08_08_PmSetResetState
    15000000,       // time08_09_PmGetResetState
    15000000,       // time08_10_PmSetPostClock
    15000000,       // time09_01_MmngrOpen
    15000000,       // time09_03_MmngrAlloc
    15000000,       // time09_05_MmngrGetConfig
    15000000,       // time09_06_MmngrGetOsalMaxConfig
    15000000,       // time09_07_MmngrInitializeMmngrConfigSt
    15000000,       // time09_08_MmngrRegisterMonitor
    15000000,       // time09_09_MmngrGetCpuPtr
    15000000,       // time09_10_MmngrGetConstCpuPtr
    15000000,       // time09_11_MmngrFlush
    15000000,       // time09_12_MmngrInvalidate
    15000000,       // time09_13_MmngrGetHwAddr
    15000000,       // time09_14_MmngrGetSize
    15000000,       // time09_15_MmngrCheckBuffer
    15000000,       // time09_16_MmngrDebugMmngrPrint
    15000000,       // time09_17_MmngrDebugBufferPrint
    15000000,       // time09_04_MmngrDealloc
    15000000,       // time09_02_MmngrClose
    15000000,       // time09_20_MmngrOpenFromRegionIdx
    15000000,       // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    15000000,       // time09_18_MmngrInitializeMemoryManagerObj
    15000000,       // time09_19_MmngrInitializeMemoryBufferObj
    15000000,       // time01_03_GetVersion
    15000000,       // time01_02_Deinitialize
};
#endif

int main(void)
{
    e_osal_return_t osal_ret;
    unsigned int test_pass   = 0;
    unsigned int test_fail   = 0;

    printf("Check API time\n");

#ifdef USE_STUB
    g_mutex_trylock_err = 0;
#endif
    osal_ret = time_test_all(101);  // time01_01_Initialize
    osal_ret = time_test_all(201);  // time02_01_ThreadCreate
    osal_ret = time_test_all(202);  // time02_02_ThreadSleepUntilTimeStamp
    osal_ret = time_test_all(203);  // time02_03_ThreadSleepForTimePeriod
    osal_ret = time_test_all(204);  // time02_04_ThreadSelf(void)     // xOS2
    osal_ret = time_test_all(205);  // time02_05_ThreadEqual
    osal_ret = time_test_all(206);  // time02_06_ThreadJoin
    osal_ret = time_test_all(207);  // time02_07_ThreadInitializeThreadConfigSt
    osal_ret = time_test_all(301);  // time03_01_ThsyncMutexCreate
    osal_ret = time_test_all(303);  // time03_03_ThsyncMutexLockUntilTimeStamp
    osal_ret = time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret = time_test_all(304);  // time03_04_ThsyncMutexLockForTimePeriod
    osal_ret = time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret = time_test_all(305);  // time03_05_ThsyncMutexTryLock
    osal_ret = time_test_all(307);  // time03_07_ThsyncCondCreate
#ifdef USE_STUB
    g_mutex_trylock_err = EBUSY;
#endif
    osal_ret = time_test_all(309);  // time03_09_ThsyncCondWaitForTimePeriod
    osal_ret = time_test_all(310);  // time03_10_ThsyncCondWaitUntilTimeStamp
#ifdef USE_STUB
    g_mutex_trylock_err = 0;
#endif
    osal_ret = time_test_all(311);  // time03_11_ThsyncCondBroadcast
    osal_ret = time_test_all(312);  // time03_12_ThsyncCondSignal
    osal_ret = time_test_all(308);  // time03_08_ThsyncCondDestroy
    osal_ret = time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret = time_test_all(302);  // time03_02_ThsyncMutexDestroy
    osal_ret = time_test_all(313);  // time03_13_ThsyncMemoryBarrier
    osal_ret = time_test_all(401);  // time04_01_MqCreate
    osal_ret = time_test_all(403);  // time04_03_MqSendUntilTimeStamp
    osal_ret = time_test_all(405);  // time04_05_MqReceiveUntilTimeStamp
    osal_ret = time_test_all(404);  // time04_04_MqSendForTimePeriod
    osal_ret = time_test_all(406);  // time04_06_MqReceiveForTimePeriod
    osal_ret = time_test_all(407);  // time04_07_MqIsFull
    osal_ret = time_test_all(408);  // time04_08_MqIsEmpty
    osal_ret = time_test_all(409);  // time04_09_MqGetConfig
    osal_ret = time_test_all(410);  // time04_10_MqReset
    osal_ret = time_test_all(411);  // time04_11_MqInitializeMqConfigSt
    osal_ret = time_test_all(402);  // time04_02_MqDelete
    osal_ret = time_test_all(501);  // time05_01_ClockTimeGetTimeStamp
    osal_ret = time_test_all(502);  // time05_02_ClockTimeCalculateTimeDifference
    osal_ret = time_test_all(701);  // time07_01_IoGetNumOfDevices
    osal_ret = time_test_all(702);  // time07_02_IoGetDeviceList
    osal_ret = time_test_all(703);  // time07_03_IoDeviceOpen(TEST_DEVICE_NAME)
    osal_ret = time_test_all(705);  // time07_05_IoRead8
    osal_ret = time_test_all(706);  // time07_06_IoRead16
    osal_ret = time_test_all(707);  // time07_07_IoRead32
    osal_ret = time_test_all(708);  // time07_08_IoRead64
    osal_ret = time_test_all(709);  // time07_09_IoWrite8
    osal_ret = time_test_all(710);  // time07_10_IoWrite16
    osal_ret = time_test_all(711);  // time07_11_IoWrite32
    osal_ret = time_test_all(712);  // time07_12_IoWrite64
    osal_ret = time_test_all(713);  // time07_13_IoBlockRead8
    osal_ret = time_test_all(714);  // time07_14_IoBlockRead16
    osal_ret = time_test_all(715);  // time07_15_IoBlockRead32
    osal_ret = time_test_all(716);  // time07_16_IoBlockRead64
    osal_ret = time_test_all(717);  // time07_17_IoBlockWrite8
    osal_ret = time_test_all(718);  // time07_18_IoBlockWrite16
    osal_ret = time_test_all(719);  // time07_19_IoBlockWrite32
    osal_ret = time_test_all(720);  // time07_20_IoBlockWrite64
    osal_ret = time_test_all(721);  // time07_21_IoGetDeviceAxiBusId
    osal_ret = time_test_all(722);  // time07_22_IoGetAxiBusIdFromDeviceName
    osal_ret = time_test_all(723);  // time07_23_IoGetAxiBusNameList
    osal_ret = time_test_all(724);  // time07_24_IoGetAxiBusName
    osal_ret = time_test_all(725);  // time07_25_IoGetAxiIdListForDeviceHdl
    osal_ret = time_test_all(726);  // time07_26_IoGetAxiIdListForDeviceName
    osal_ret = time_test_all(727);  // time07_27_IoGetAxiIdListForAxiBusName
    osal_ret = time_test_all(728);  // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    osal_ret = time_test_all(729);  // time07_29_IoGetNumOfAxiIdsForDeviceName
    osal_ret = time_test_all(730);  // time07_30_IoGetNumOfAxiIdsForAxiBusName
    osal_ret = time_test_all(601);  // time06_01_InterruptGetNumOfIrqChannels
    osal_ret = time_test_all(602);  // time06_02_InterruptRegisterIsr
    osal_ret = time_test_all(604);  // time06_04_InterruptEnableIsr
    osal_ret = time_test_all(605);  // time06_05_InterruptDisableIsr
    osal_ret = time_test_all(603);  // time06_03_InterruptUnregisterIsr
    osal_ret = time_test_all(606);  // time06_06_InterruptIsISRContext
    osal_ret = time_test_all(607);  // time06_07_InterruptSetIsrPriority
    osal_ret = time_test_all(704);  // time07_04_IoDeviceClose

    osal_ret = time_test_all(732);  // time07_03_IoDeviceOpen(TEST_DEVICE_NAME_PM)
    osal_ret = time_test_all(801);  // time08_01_PmSetRequiredState
    osal_ret = time_test_all(802);  // time08_02_PmGetRequiredState
    osal_ret = time_test_all(803);  // time08_03_PmWaitForState
    osal_ret = time_test_all(804);  // time08_04_PmGetState
    osal_ret = time_test_all(805);  // time08_05_PmGetLowestIntermediatePowerState
    osal_ret = time_test_all(806);  // time08_06_PmSetPolicy
    osal_ret = time_test_all(807);  // time08_07_PmGetPolicy
    osal_ret = time_test_all(808);  // time08_08_PmSetResetState
    osal_ret = time_test_all(809);  // time08_09_PmGetResetState
    osal_ret = time_test_all(704);  // time07_04_IoDeviceClose
    osal_ret = time_test_all(731);  // time07_03_IoDeviceOpen(fbc_00)
    osal_ret = time_test_all(810);  // time08_10_PmSetPostClock
    osal_ret = time_test_all(704);  // time07_04_IoDeviceClose
    osal_ret = time_test_all(901);  // time09_01_MmngrOpen
    osal_ret = time_test_all(903);  // time09_03_MmngrAlloc
    osal_ret = time_test_all(905);  // time09_05_MmngrGetConfig
    osal_ret = time_test_all(906);  // time09_06_MmngrGetOsalMaxConfig
    osal_ret = time_test_all(907);  // time09_07_MmngrInitializeMmngrConfigSt
    osal_ret = time_test_all(908);  // time09_08_MmngrRegisterMonitor
    osal_ret = time_test_all(909);  // time09_09_MmngrGetCpuPtr
    osal_ret = time_test_all(910);  // time09_10_MmngrGetConstCpuPtr
    osal_ret = time_test_all(911);  // time09_11_MmngrFlush
    osal_ret = time_test_all(912);  // time09_12_MmngrInvalidate
    osal_ret = time_test_all(913);  // time09_13_MmngrGetHwAddr
    osal_ret = time_test_all(914);  // time09_14_MmngrGetSize
    osal_ret = time_test_all(915);  // time09_15_MmngrCheckBuffer
    osal_ret = time_test_all(916);  // time09_16_MmngrDebugMmngrPrint
    osal_ret = time_test_all(917);  // time09_17_MmngrDebugBufferPrint
    osal_ret = time_test_all(904);  // time09_04_MmngrDealloc
    osal_ret = time_test_all(902);  // time09_02_MmngrClose
    osal_ret = time_test_all(920);  // time09_20_MmngrOpenFromRegionIdx
    osal_ret = time_test_all(921);  // time09_21_MmngrGetOsalMaxConfigFromRegionIdx
    osal_ret = time_test_all(902);  // time09_02_MmngrClose
    osal_ret = time_test_all(918);  // time09_18_MmngrInitializeMemoryManagerObj
    osal_ret = time_test_all(919);  // time09_19_MmngrInitializeMemoryBufferObj
    osal_ret = time_test_all(103);  // time01_03_GetVersion
    osal_ret = time_test_all(102);  // time01_02_Deinitialize

    sleep(10);

    printf("\n\nOSAL API execution times:\n");
    for (int i = 0; i < TEST_NUM; i++)
    {
        printf("%s Expect: %lu, Actual: %lu ns\n", api_list[i], expected_time[i], elapsed_time[i]);
        if (expected_time[i] < elapsed_time[i])
        {
            printf("%s has exceeded the expected execution time. \n", api_list[i]);
            test_fail++;
        }
        else
        {
            test_pass++;
        }
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, (TEST_NUM - (test_pass + test_fail)));

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
#define TEST_AXI_BUS_NAME   "vi0_01"
#define TEST_DEVICE_NAME    "ims_00"
#define TEST_DEVICE_NAME_PM "ims_00"
#define TEST_DEVICE_TYPE    "ims"
#elif defined (RCAR_V4H) || defined (RCAR_V4M)
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
#else    /* V3x, V3M, V4H, V4M */
st_osal_mq_config_t     mq_config = {1, 4};   // {max_num_msg, msg_size}
#endif
osal_device_handle_t    dev_handle = NULL;
osal_memory_manager_handle_t    mmgr_handle = NULL;
osal_memory_buffer_handle_t     buf_handle = NULL;

e_osal_return_t time_test_all(int func_id)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
#ifndef WINAMS_STUB
#define SEC2NSEC        (1000000000ULL)
#define TIME_BEFORE     clocktime_before = ClockCycles();
#define TIME_AFTER      clocktime_after  = ClockCycles();
#define TIME_DIFF       (clocktime_after - clocktime_before)

    uint64_t clocktime_before;
    uint64_t clocktime_after;

    /* initialize global variable for stub */
    init_stub_value();

    /* get current time */
    TIME_BEFORE
#endif /* WINAMS_STUB */

    switch (func_id)
    {
        case 101:  osal_ret = time01_01_Initialize(); break;
        case 102:  osal_ret = time01_02_Deinitialize(); break;
        case 103:  osal_ret = time01_03_GetVersion(); break;
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
        case 725:  osal_ret = time07_25_IoGetAxiIdListForDeviceHdl(); break;
        case 726:  osal_ret = time07_26_IoGetAxiIdListForDeviceName(); break;
        case 727:  osal_ret = time07_27_IoGetAxiIdListForAxiBusName(); break;
        case 728:  osal_ret = time07_28_IoGetNumOfAxiIdsForDeviceHdl(); break;
        case 729:  osal_ret = time07_29_IoGetNumOfAxiIdsForDeviceName(); break;
        case 730:  osal_ret = time07_30_IoGetNumOfAxiIdsForAxiBusName(); break;
        case 731:  osal_ret = time07_03_IoDeviceOpen("fbc_00"); break;
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
        case 918:  osal_ret = time09_18_MmngrInitializeMemoryManagerObj(); break;
        case 919:  osal_ret = time09_19_MmngrInitializeMemoryBufferObj(); break;
        case 920:  osal_ret = time09_20_MmngrOpenFromRegionIdx(); break;
        case 921:  osal_ret = time09_21_MmngrGetOsalMaxConfigFromRegionIdx(); break;
        default:   osal_ret = OSAL_RETURN_PAR; break;
    }

#ifndef WINAMS_STUB
    /* print elapsed time */
    TIME_AFTER
    switch(func_count)
    {
        case 12:      break;
        case 20:      break;
        case 73:      break;
        case 83:      break;
        case 84:      break;
        case 86:      break;
        case 106:     break;
        default:      elapsed_time[test_count] = ((TIME_DIFF - get_stub_time())*SEC2NSEC)/(uint64_t)SYSPAGE_ENTRY(qtime)->cycles_per_sec;
                      printf("\n\nfunc-id=\t%d\tresult=\t%4d\tExecution time:\t%ld\t (ns)\n", func_id, osal_ret, elapsed_time[test_count]);
                      test_count++;
#if 1
                      /* print pthread_create call count */
                      printf("pthread_create call count    :\t%ld\n", get_pthread_create_count());

                      /* print pthread_mutex_init call count */
                      printf("pthread_mutex_init call count:\t%ld\n", get_pthread_mutex_init_count());

                      /* print pthread_cond_init call count */
                      printf("pthread_cond_init call count :\t%ld\n", get_pthread_cond_init_count());

                      /* print malloc call count */
                      printf("malloc call count            :\t%ld\n", get_malloc_count());

                      /* print realloc call count */
                      printf("realloc call count           :\t%ld\n", get_realloc_count());

                      /* print calloc call count */
                      printf("calloc call count            :\t%ld\n", get_calloc_count());

                      /* print timer_create call count */
                      printf("timer_create call count      :\t%ld\n", get_timer_create_count());
#endif  /* Call count */
                      break;
    }
    func_count++;
#endif  /* WINAMS_STUB */

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

/* irq callback function */
void time_dummy_func(void)
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
    void* user_arg;

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

e_osal_return_t time09_18_MmngrInitializeMemoryManagerObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_manager_t tmp_manager;

    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&tmp_manager);

    return osal_ret;
}

e_osal_return_t time09_19_MmngrInitializeMemoryBufferObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_buffer_t tmp_buffer;

    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&tmp_buffer);

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

