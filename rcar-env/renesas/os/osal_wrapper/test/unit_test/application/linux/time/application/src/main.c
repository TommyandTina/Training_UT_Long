#include "stub.h"
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

extern int stub_fd_count;
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
st_osal_mq_config_t     mq_config = {10, 50};   // {max_num_msg, msg_size}
osal_device_handle_t    dev_handle = NULL;
osal_memory_manager_handle_t    mmgr_handle = NULL;
osal_memory_buffer_handle_t     buf_handle = NULL;
bool in_io_device_open = false;

#define MAX_LEN     (128)
#define LOOP_NUM    (11)
#define API_NUM     (104)
#define TEST_SET(function) {#function, performance_##function, {0}, 0, 0, 0xFFFFFFFFFFFFFFFF}

typedef struct
{
    char                        func_name[MAX_LEN];
    e_osal_return_t (*check_func)(void);
    uint64_t                    measurement_time[LOOP_NUM];
    uint64_t                    total_time;
    uint64_t                    max;
    uint64_t                    min;
} st_peformance_test_t;

e_osal_return_t performance_R_OSAL_Initialize(void);
e_osal_return_t performance_R_OSAL_Deinitialize(void);
e_osal_return_t performance_R_OSAL_ThreadCreate(void);
e_osal_return_t performance_R_OSAL_ThreadSleepUntilTimeStamp(void);
e_osal_return_t performance_R_OSAL_ThreadSleepForTimePeriod(void);
e_osal_return_t performance_R_OSAL_ThreadSelf(void);
e_osal_return_t performance_R_OSAL_ThreadEqual(void);
e_osal_return_t performance_R_OSAL_ThreadJoin(void);
e_osal_return_t performance_R_OSAL_ThreadInitializeThreadConfigSt(void);
e_osal_return_t performance_R_OSAL_ThsyncMutexCreate(void);
e_osal_return_t performance_R_OSAL_ThsyncMutexDestroy(void);
e_osal_return_t performance_R_OSAL_ThsyncMutexLockUntilTimeStamp(void);
e_osal_return_t performance_R_OSAL_ThsyncMutexLockForTimePeriod(void);
e_osal_return_t performance_R_OSAL_ThsyncMutexTryLock(void);
e_osal_return_t performance_R_OSAL_ThsyncMutexUnlock(void);
e_osal_return_t performance_R_OSAL_ThsyncCondCreate(void);
e_osal_return_t performance_R_OSAL_ThsyncCondDestroy(void);
e_osal_return_t performance_R_OSAL_ThsyncCondWaitForTimePeriod(void);
e_osal_return_t performance_R_OSAL_ThsyncCondWaitUntilTimeStamp(void);
e_osal_return_t performance_R_OSAL_ThsyncCondBroadcast(void);
e_osal_return_t performance_R_OSAL_ThsyncCondSignal(void);
e_osal_return_t performance_R_OSAL_ThsyncMemoryBarrier(void);
e_osal_return_t performance_R_OSAL_MqCreate(void);
e_osal_return_t performance_R_OSAL_MqDelete(void);
e_osal_return_t performance_R_OSAL_MqSendUntilTimeStamp(void);
e_osal_return_t performance_R_OSAL_MqSendForTimePeriod(void);
e_osal_return_t performance_R_OSAL_MqReceiveUntilTimeStamp(void);
e_osal_return_t performance_R_OSAL_MqReceiveForTimePeriod(void);
e_osal_return_t performance_R_OSAL_MqIsFull(void);
e_osal_return_t performance_R_OSAL_MqIsEmpty(void);
e_osal_return_t performance_R_OSAL_MqGetConfig(void);
e_osal_return_t performance_R_OSAL_MqReset(void);
e_osal_return_t performance_R_OSAL_MqInitializeMqConfigSt(void);
e_osal_return_t performance_R_OSAL_ClockTimeGetTimeStamp(void);
e_osal_return_t performance_R_OSAL_ClockTimeCalculateTimeDifference(void);
e_osal_return_t performance_R_OSAL_IoGetNumOfDevices(void);
e_osal_return_t performance_R_OSAL_IoGetDeviceList(void);
e_osal_return_t performance_R_OSAL_IoDeviceOpen(void);
e_osal_return_t performance_R_OSAL_IoDeviceClose(void);
e_osal_return_t performance_R_OSAL_IoRead8(void);
e_osal_return_t performance_R_OSAL_IoRead16(void);
e_osal_return_t performance_R_OSAL_IoRead32(void);
e_osal_return_t performance_R_OSAL_IoRead64(void);
e_osal_return_t performance_R_OSAL_IoWrite8(void);
e_osal_return_t performance_R_OSAL_IoWrite16(void);
e_osal_return_t performance_R_OSAL_IoWrite32(void);
e_osal_return_t performance_R_OSAL_IoWrite64(void);
e_osal_return_t performance_R_OSAL_IoBlockRead8(void);
e_osal_return_t performance_R_OSAL_IoBlockRead16(void);
e_osal_return_t performance_R_OSAL_IoBlockRead32(void);
e_osal_return_t performance_R_OSAL_IoBlockRead64(void);
e_osal_return_t performance_R_OSAL_IoBlockWrite8(void);
e_osal_return_t performance_R_OSAL_IoBlockWrite16(void);
e_osal_return_t performance_R_OSAL_IoBlockWrite32(void);
e_osal_return_t performance_R_OSAL_IoBlockWrite64(void);
e_osal_return_t performance_R_OSAL_IoGetDeviceAxiBusId(void);
e_osal_return_t performance_R_OSAL_IoGetAxiBusIdFromDeviceName(void);
e_osal_return_t performance_R_OSAL_IoGetAxiBusNameList(void);
e_osal_return_t performance_R_OSAL_IoGetAxiBusName(void);
e_osal_return_t performance_R_OSAL_IoGetAxiIdListForDeviceHdl(void);
e_osal_return_t performance_R_OSAL_IoGetAxiIdListForDeviceName(void);
e_osal_return_t performance_R_OSAL_IoGetAxiIdListForAxiBusName(void);
e_osal_return_t performance_R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(void);
e_osal_return_t performance_R_OSAL_IoGetNumOfAxiIdsForDeviceName(void);
e_osal_return_t performance_R_OSAL_IoGetNumOfAxiIdsForAxiBusName(void);
e_osal_return_t performance_R_OSAL_InterruptGetNumOfIrqChannels(void);
e_osal_return_t performance_R_OSAL_InterruptRegisterIsr(void);
e_osal_return_t performance_R_OSAL_InterruptUnregisterIsr(void);
e_osal_return_t performance_R_OSAL_InterruptEnableIsr(void);
e_osal_return_t performance_R_OSAL_InterruptDisableIsr(void);
e_osal_return_t performance_R_OSAL_InterruptIsISRContext(void);
e_osal_return_t performance_R_OSAL_InterruptSetIsrPriority(void);
e_osal_return_t performance_R_OSAL_PmSetRequiredState(void);
e_osal_return_t performance_R_OSAL_PmGetRequiredState(void);
e_osal_return_t performance_R_OSAL_PmWaitForState(void);
e_osal_return_t performance_R_OSAL_PmGetState(void);
e_osal_return_t performance_R_OSAL_PmGetLowestIntermediatePowerState(void);
e_osal_return_t performance_R_OSAL_PmSetPolicy(void);
e_osal_return_t performance_R_OSAL_PmGetPolicy(void);
e_osal_return_t performance_R_OSAL_PmSetResetState(void);
e_osal_return_t performance_R_OSAL_PmGetResetState(void);
e_osal_return_t performance_R_OSAL_PmSetPostClock(void);
e_osal_return_t performance_R_OSAL_MmngrOpen(void);
e_osal_return_t performance_R_OSAL_MmngrClose(void);
e_osal_return_t performance_R_OSAL_MmngrAlloc(void);
e_osal_return_t performance_R_OSAL_MmngrDealloc(void);
e_osal_return_t performance_R_OSAL_MmngrGetConfig(void);
e_osal_return_t performance_R_OSAL_MmngrGetOsalMaxConfig(void);
e_osal_return_t performance_R_OSAL_MmngrInitializeMmngrConfigSt(void);
e_osal_return_t performance_R_OSAL_MmngrRegisterMonitor(void);
e_osal_return_t performance_R_OSAL_MmngrGetCpuPtr(void);
e_osal_return_t performance_R_OSAL_MmngrGetConstCpuPtr(void);
e_osal_return_t performance_R_OSAL_MmngrFlush(void);
e_osal_return_t performance_R_OSAL_MmngrInvalidate(void);
e_osal_return_t performance_R_OSAL_MmngrGetHwAddr(void);
e_osal_return_t performance_R_OSAL_MmngrGetSize(void);
e_osal_return_t performance_R_OSAL_MmngrCheckBuffer(void);
e_osal_return_t performance_R_OSAL_MmngrDebugMmngrPrint(void);
e_osal_return_t performance_R_OSAL_MmngrDebugBufferPrint(void);
e_osal_return_t performance_R_OSAL_GetVersion(void);
e_osal_return_t performance_R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(void);
e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryBufferObj(void);
e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryManagerObj(void);
e_osal_return_t performance_R_OSAL_MmngrOpenFromRegionIdx(void);

/* irq callback function */
void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg);

/* mmgr callback function */
void time_callback_mmgr(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl);

st_peformance_test_t performance_test[API_NUM] = {
    TEST_SET(R_OSAL_Initialize),
    TEST_SET(R_OSAL_GetVersion),
    TEST_SET(R_OSAL_ThreadCreate),
    TEST_SET(R_OSAL_ThreadSleepUntilTimeStamp),
    TEST_SET(R_OSAL_ThreadSleepForTimePeriod),
    TEST_SET(R_OSAL_ThreadSelf),
    TEST_SET(R_OSAL_ThreadEqual),
    TEST_SET(R_OSAL_ThreadJoin),
    TEST_SET(R_OSAL_ThreadInitializeThreadConfigSt),
    TEST_SET(R_OSAL_ThsyncMutexCreate),
    TEST_SET(R_OSAL_ThsyncMutexLockUntilTimeStamp),
    TEST_SET(R_OSAL_ThsyncMutexLockForTimePeriod),
    TEST_SET(R_OSAL_ThsyncMutexTryLock),
    TEST_SET(R_OSAL_ThsyncMutexUnlock),
    TEST_SET(R_OSAL_ThsyncCondCreate),
    TEST_SET(R_OSAL_ThsyncCondWaitForTimePeriod),
    TEST_SET(R_OSAL_ThsyncCondWaitUntilTimeStamp),
    TEST_SET(R_OSAL_ThsyncCondBroadcast),
    TEST_SET(R_OSAL_ThsyncCondSignal),
    TEST_SET(R_OSAL_ThsyncCondDestroy),
    TEST_SET(R_OSAL_ThsyncMutexDestroy),
    TEST_SET(R_OSAL_ThsyncMemoryBarrier),
    TEST_SET(R_OSAL_MqCreate),
    TEST_SET(R_OSAL_MqSendUntilTimeStamp),
    TEST_SET(R_OSAL_MqSendForTimePeriod),
    TEST_SET(R_OSAL_MqReceiveUntilTimeStamp),
    TEST_SET(R_OSAL_MqReceiveForTimePeriod),
    TEST_SET(R_OSAL_MqIsFull),
    TEST_SET(R_OSAL_MqIsEmpty),
    TEST_SET(R_OSAL_MqGetConfig),
    TEST_SET(R_OSAL_MqReset),
    TEST_SET(R_OSAL_MqInitializeMqConfigSt),
    TEST_SET(R_OSAL_MqDelete),
    TEST_SET(R_OSAL_ClockTimeGetTimeStamp),
    TEST_SET(R_OSAL_ClockTimeCalculateTimeDifference),
    TEST_SET(R_OSAL_IoGetNumOfDevices),
    TEST_SET(R_OSAL_IoGetDeviceList),
    TEST_SET(R_OSAL_IoDeviceOpen),
    TEST_SET(R_OSAL_IoRead8),
    TEST_SET(R_OSAL_IoRead16),
    TEST_SET(R_OSAL_IoRead32),
    TEST_SET(R_OSAL_IoRead64),
    TEST_SET(R_OSAL_IoWrite8),
    TEST_SET(R_OSAL_IoWrite16),
    TEST_SET(R_OSAL_IoWrite32),
    TEST_SET(R_OSAL_IoWrite64),
    TEST_SET(R_OSAL_IoBlockRead8),
    TEST_SET(R_OSAL_IoBlockRead16),
    TEST_SET(R_OSAL_IoBlockRead32),
    TEST_SET(R_OSAL_IoBlockRead64),
    TEST_SET(R_OSAL_IoBlockWrite8),
    TEST_SET(R_OSAL_IoBlockWrite16),
    TEST_SET(R_OSAL_IoBlockWrite32),
    TEST_SET(R_OSAL_IoBlockWrite64),
    TEST_SET(R_OSAL_IoGetDeviceAxiBusId),
    TEST_SET(R_OSAL_IoGetAxiBusIdFromDeviceName),
    TEST_SET(R_OSAL_IoGetAxiBusNameList),
    TEST_SET(R_OSAL_IoGetAxiBusName),
    TEST_SET(R_OSAL_IoGetAxiIdListForDeviceHdl),
    TEST_SET(R_OSAL_IoGetAxiIdListForDeviceName),
    TEST_SET(R_OSAL_IoGetAxiIdListForAxiBusName),
    TEST_SET(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl),
    TEST_SET(R_OSAL_IoGetNumOfAxiIdsForDeviceName),
    TEST_SET(R_OSAL_IoGetNumOfAxiIdsForAxiBusName),
    TEST_SET(R_OSAL_InterruptGetNumOfIrqChannels),
    TEST_SET(R_OSAL_InterruptRegisterIsr),
    TEST_SET(R_OSAL_InterruptEnableIsr),
    TEST_SET(R_OSAL_InterruptDisableIsr),
    TEST_SET(R_OSAL_InterruptSetIsrPriority),
    TEST_SET(R_OSAL_InterruptUnregisterIsr),
    TEST_SET(R_OSAL_InterruptIsISRContext),
    TEST_SET(R_OSAL_PmSetRequiredState),
    TEST_SET(R_OSAL_PmGetRequiredState),
    TEST_SET(R_OSAL_PmWaitForState),
    TEST_SET(R_OSAL_PmGetState),
    TEST_SET(R_OSAL_PmGetLowestIntermediatePowerState),
    TEST_SET(R_OSAL_PmSetPolicy),
    TEST_SET(R_OSAL_PmGetPolicy),
    TEST_SET(R_OSAL_PmSetResetState),
    TEST_SET(R_OSAL_PmGetResetState),
    TEST_SET(R_OSAL_PmSetPostClock),
    TEST_SET(R_OSAL_IoDeviceClose),
    TEST_SET(R_OSAL_MmngrOpen),
    TEST_SET(R_OSAL_MmngrAlloc),
    TEST_SET(R_OSAL_MmngrInitializeMemoryManagerObj),
    TEST_SET(R_OSAL_MmngrInitializeMemoryBufferObj),
    TEST_SET(R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx),
    TEST_SET(R_OSAL_MmngrGetConfig),
    TEST_SET(R_OSAL_MmngrGetOsalMaxConfig),
    TEST_SET(R_OSAL_MmngrInitializeMmngrConfigSt),
    TEST_SET(R_OSAL_MmngrRegisterMonitor),
    TEST_SET(R_OSAL_MmngrGetCpuPtr),
    TEST_SET(R_OSAL_MmngrGetConstCpuPtr),
    TEST_SET(R_OSAL_MmngrFlush),
    TEST_SET(R_OSAL_MmngrInvalidate),
    TEST_SET(R_OSAL_MmngrGetHwAddr),
    TEST_SET(R_OSAL_MmngrGetSize),
    TEST_SET(R_OSAL_MmngrCheckBuffer),
    TEST_SET(R_OSAL_MmngrDebugMmngrPrint),
    TEST_SET(R_OSAL_MmngrDebugBufferPrint),
    TEST_SET(R_OSAL_MmngrDealloc),
    TEST_SET(R_OSAL_MmngrClose),
    TEST_SET(R_OSAL_MmngrOpenFromRegionIdx),
    TEST_SET(R_OSAL_Deinitialize),
};

void entry_point(void)
{
    // entry point for WinAMS
}

int main(void)
{
    e_osal_return_t osal_ret;
    int             i, j;
    unsigned int    test_pass = 0;
    unsigned int    test_fail = 0;
    unsigned int    test_skip = 0;

#ifndef WINAMS_STUB
#define sec_to_nsec (1000000000)
    struct timespec abstime_before;
    struct timespec abstime_after;
#endif /* WINAMS_STUB */

    for (i = 0; i < LOOP_NUM; i++)
    {
        stub_fd_count = 0;
        for (j = 0; j < API_NUM; j++)
        {
#ifndef WINAMS_STUB
             // get current time
            init_stub_time();
            clock_gettime(CLOCK_REALTIME, &abstime_before);
#endif /* WINAMS_STUB */

            osal_ret = performance_test[j].check_func();

#ifndef WINAMS_STUB
            // print elapsed time
            clock_gettime(CLOCK_REALTIME, &abstime_after);

            performance_test[j].measurement_time[i] = (abstime_after.tv_sec * sec_to_nsec + abstime_after.tv_nsec) - (abstime_before.tv_sec * sec_to_nsec + abstime_before.tv_nsec) - get_stub_time();
            if (i > 0)
            {
                performance_test[j].max = (performance_test[j].measurement_time[i] > performance_test[j].max ) ? performance_test[j].measurement_time[i] : performance_test[j].max;
                performance_test[j].min = (performance_test[j].measurement_time[i] < performance_test[j].min ) ? performance_test[j].measurement_time[i] : performance_test[j].min;
                performance_test[j].total_time += performance_test[j].measurement_time[i];
            }

            printf("%-41s, result=%4d, Execution time: %7ld (ns)\n", performance_test[j].func_name, osal_ret, performance_test[j].measurement_time[i]);
#endif /* WINAMS_STUB */

            if (OSAL_RETURN_OK == osal_ret)
            {
                test_pass++;
            }
            else
            {
                if ((5 == j) && (OSAL_RETURN_FAIL == osal_ret))
                {
                    // R_OSAL_ThreadSelf returns OSAL_RETURN_FAIL.
                    // However, It can be ignored because all paths to be measured is done.
                    test_pass++;
                }
#if defined(NDEBUG)
                else if (((98 == j) && (OSAL_RETURN_HANDLE_NOT_SUPPORTED == osal_ret)) ||
                         ((99 == j) && (OSAL_RETURN_UNSUPPORTED_OPERATION == osal_ret)))
                {
                    // R_OSAL_MmngrDebugMmngrPrint and R_OSAL_MmngrDebugBufferPrint returns UNSUPPORTED.
                    // These API are measured only RELEASE build in UT phase.
                    test_pass++;
                }
#endif
                else
                {
                    test_fail++;
                }
            }
        }
    }

#ifndef WINAMS_STUB
    printf("\n");
    printf("                                                                        [ns]\n");
    printf("                                               ave,     max,    min,    boot\n");
    for (i = 0; i < API_NUM; i++)
    {
        printf("%-41s, %7ld, %7ld, %6ld, %7ld\n", performance_test[i].func_name, performance_test[i].total_time / (LOOP_NUM - 1), performance_test[i].max, performance_test[i].min, performance_test[i].measurement_time[0]);
    }
#endif /* WINAMS_STUB */

    test_pass /= LOOP_NUM;
    test_fail /= LOOP_NUM;

    printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);
    return 0;
}

e_osal_return_t performance_R_OSAL_Initialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    in_io_device_open = true;

    if(is_initialized == false)
    {
        osal_ret = R_OSAL_Initialize();

        if(osal_ret == OSAL_RETURN_OK)
        {
            is_initialized = true;
        }
    }
    in_io_device_open = false;

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_Deinitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_Deinitialize();
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_initialized = false;
    }

    return osal_ret;
}

/* irq callback function */
void time_dummy_func(void)
{
    return;
}

e_osal_return_t performance_R_OSAL_ThreadCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;

    is_thread_created = false;
    if (is_thread_created == false)
    {
        config.func       = time_dummy_func;
        config.userarg    = NULL;
        config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
        config.task_name  = "time_Thread";
        config.stack_size = 0x1000;

        osal_ret = R_OSAL_ThreadCreate(&config, 2, &th_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_thread_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadSleepUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;

    absolute_time.tv_sec = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadSleepForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;

    osal_ret = R_OSAL_ThreadSleepForTimePeriod(2500);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadSelf(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_thread_handle_t    th_handle2;

    th_handle2 = th_handle;
    osal_ret = R_OSAL_ThreadSelf(&th_handle2);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadEqual(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool                    result = false;
    osal_thread_handle_t    th_handle2;

    th_handle2 = th_handle;
    osal_ret = R_OSAL_ThreadEqual(th_handle, th_handle2, &result);
    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadJoin(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t return_value = 0;

    osal_ret = R_OSAL_ThreadJoin(th_handle, &return_value);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadInitializeThreadConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;
    int64_t return_value = 0;

    osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_mutex_created == false)
    {
        osal_ret = R_OSAL_ThsyncMutexCreate(2, &mutex_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_mutex_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexDestroy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_mutex_created = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexLockUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;

    absolute_time.tv_sec = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexLockForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;


    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 2500);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexTryLock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexUnlock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;


    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_cond_id_t              cond_id = 1001U;

    if (is_cond_created == false)
    {
        osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_cond_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondDestroy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_cond_created = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondWaitForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 2500);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondWaitUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;

    absolute_time.tv_sec = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondBroadcast(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondSignal(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMemoryBarrier(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMemoryBarrier();

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_mq_id_t            mq_id = 0;

    if (is_mq_created == false)
    {
        osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_mq_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqDelete(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MqDelete(mq_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_mq_created = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqSendUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;
    char                send_data[50];

    absolute_time.tv_sec = 1;
    absolute_time.tv_nsec = 2000;
    osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, &absolute_time, send_data, sizeof(send_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqSendForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char                send_data[50];

    osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 100, send_data, sizeof(send_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqReceiveUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;
    char                recv_data[50];

    absolute_time.tv_sec = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &absolute_time, recv_data, sizeof(recv_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqReceiveForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char                recv_data[50];


    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 100, recv_data, sizeof(recv_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqIsFull(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool                    result = false;

    osal_ret = R_OSAL_MqIsFull(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqIsEmpty(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool                    result = false;


    osal_ret = R_OSAL_MqIsEmpty(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqGetConfig(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t     config;   // {max_num_msg, msg_size}

    osal_ret = R_OSAL_MqGetConfig(mq_handle, &config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqReset(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    //osal_ret = performance_R_OSAL_MqCreate();     /* R_OSAL_MqCreate(); */
    if (osal_ret == OSAL_RETURN_OK)
    {
        osal_ret = R_OSAL_MqReset(mq_handle);
    }
    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqInitializeMqConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t config;

    osal_ret = R_OSAL_MqInitializeMqConfigSt(&config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ClockTimeGetTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_clock_type_t     clock_type = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    st_osal_time_t          absolute_time;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ClockTimeCalculateTimeDifference(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t          absolute_time1;
    st_osal_time_t          absolute_time2;
    osal_nano_sec_t         different_time;

    absolute_time1.tv_sec = 1;
    absolute_time1.tv_nsec = 2000;
    absolute_time2.tv_sec = 2;
    absolute_time2.tv_nsec = 1000;

    osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time2, &absolute_time1, &different_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_InterruptGetNumOfIrqChannels(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t  num_channels;

    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);

    return osal_ret;
}

/* irq callback function */
void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
    return;
}

e_osal_return_t performance_R_OSAL_InterruptRegisterIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                           (p_osal_isr_func_t)time_callback_func, NULL);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_InterruptUnregisterIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)time_callback_func);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_InterruptEnableIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);

    return osal_ret;
}

/* Note: Must be called just performance_R_OSAL_InterruptEnableIsr */
e_osal_return_t performance_R_OSAL_InterruptDisableIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
    return osal_ret;
}

e_osal_return_t performance_R_OSAL_InterruptIsISRContext(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool                    result;

    osal_ret = R_OSAL_InterruptIsISRContext(&result);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_InterruptSetIsrPriority(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool                    result;

    osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetNumOfDevices(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char                    *device_type = "fba";
    size_t                  numOfDevices;

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

e_osal_return_t performance_R_OSAL_IoGetDeviceList(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char                    *device_type = "fba";
    char                    list[200];
    size_t                  numOfLists;

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

e_osal_return_t performance_R_OSAL_IoDeviceOpen(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char                    *device = "fba_imr1";

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

extern int g_pattern;
e_osal_return_t performance_R_OSAL_IoDeviceClose(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoDeviceClose(dev_handle);
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_io_numget = false;
        is_io_devlistget = false;
        is_io_opened = false;
    }

    g_pattern = -2;
    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t                 read_data = 0;

    osal_ret = R_OSAL_IoRead8(dev_handle, 0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t                read_data = 0;

    osal_ret = R_OSAL_IoRead16(dev_handle, 0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t                read_data = 0;

    osal_ret = R_OSAL_IoRead32(dev_handle, 0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t                read_data = 0;

    osal_ret = R_OSAL_IoRead64(dev_handle, 0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite8(dev_handle, 0, 0xFF);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite16(dev_handle, 0, 0x00FF);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;


    osal_ret = R_OSAL_IoWrite32(dev_handle, 0, 0x000000FF);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite64(dev_handle, 0, 0x00000000000000FF);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t                 read_data[4];

    osal_ret = R_OSAL_IoBlockRead8(dev_handle, 0, read_data, sizeof(read_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t                read_data[4];

    osal_ret = R_OSAL_IoBlockRead16(dev_handle, 0, read_data, sizeof(read_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t                read_data[4];

    osal_ret = R_OSAL_IoBlockRead32(dev_handle, 0, read_data, sizeof(read_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t                read_data[4];

    osal_ret = R_OSAL_IoBlockRead64(dev_handle, 0, read_data, sizeof(read_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t                 write_data[2] = {0x0f, 0xff};

    osal_ret = R_OSAL_IoBlockWrite8(dev_handle, 0, write_data, sizeof(write_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t                write_data[2] = {0x0f, 0xff};

    osal_ret = R_OSAL_IoBlockWrite16(dev_handle, 0, write_data, sizeof(write_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t                write_data[2] = {0x0000000f, 0x000000ff};

    osal_ret = R_OSAL_IoBlockWrite32(dev_handle, 0, write_data, sizeof(write_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t                write_data[2] = {0x000000000000000f, 0x00000000000000ff};

    osal_ret = R_OSAL_IoBlockWrite64(dev_handle, 0, write_data, sizeof(write_data));

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetDeviceAxiBusId(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetDeviceAxiBusId(dev_handle, &axi_id);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiBusIdFromDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName("fba_imr1", &axi_id);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiBusNameList(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char                    list[200];
    size_t                  num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusNameList(list, sizeof(list), &num_of_byte);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t       axi_id=1;
    char                    name[200];
    size_t                  num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, name, sizeof(name), &num_of_byte);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiIdListForDeviceHdl(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t array[10];
    size_t  array_size = 10;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl(dev_handle, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiIdListForDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char device_name[] = "fba_imr1";
    osal_axi_bus_id_t array[10];
    size_t  array_size = 10;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(device_name, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiIdListForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = "vi0_02";
    osal_axi_bus_id_t array[10];
    size_t  array_size = 10;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(dev_handle, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetNumOfAxiIdsForDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char device_name[] = "fba_imr1";
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_name, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetNumOfAxiIdsForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = "vi0_02";
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmSetRequiredState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmGetRequiredState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_required_state_t req_state;

    osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmWaitForState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_CG, 1000);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmGetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t pm_state;

    osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmGetLowestIntermediatePowerState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t pm_state;

    osal_ret = R_OSAL_PmGetLowestIntermediatePowerState(dev_handle, &pm_state);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmSetPolicy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmGetPolicy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_policy_t pm_policy;

    osal_ret = R_OSAL_PmGetPolicy(dev_handle, &pm_policy);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmSetResetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmGetResetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_reset_state_t reset_state;

    osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_PmSetPostClock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool clock_enable = true;
    uint32_t clock_driver = 1;

    osal_ret = R_OSAL_PmSetPostClock(dev_handle, clock_enable, clock_driver);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrOpen(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit = 4096;
    config.max_allowed_allocations = 16;

    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrOpen(&config, &mmgr_handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrClose(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrClose(mmgr_handle);
    mmgr_handle = NULL;

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrAlloc(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t size = 0x80;
    size_t alignment = 0x40;

    osal_ret = R_OSAL_MmngrAlloc(mmgr_handle, size, alignment, &buf_handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrDealloc(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDealloc(mmgr_handle, buf_handle);
    buf_handle = NULL;

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetConfig(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    osal_ret = R_OSAL_MmngrGetConfig(mmgr_handle, &config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetOsalMaxConfig(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrInitializeMmngrConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(&config);

    return osal_ret;
}

void time_callback_mmgr(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    return;
}

e_osal_return_t performance_R_OSAL_MmngrRegisterMonitor(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_mmngr_event_t monitored_event = OSAL_MMNGR_EVENT_ALLOCATE;
    void* user_arg;

    osal_ret = R_OSAL_MmngrRegisterMonitor(mmgr_handle, monitored_event, time_callback_mmgr, &user_arg);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    void *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetConstCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    void *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrFlush(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t offset = 0x0;
    size_t size = 0x40;

    osal_ret = R_OSAL_MmngrFlush(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrInvalidate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t offset = 0x0;
    size_t size = 0x40;

    osal_ret = R_OSAL_MmngrInvalidate(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetHwAddr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id = 1;
#ifdef _USE_STUB
    uintptr_t hw_addr = (uintptr_t)get_cmem_area();
    osal_ret = R_OSAL_MmngrGetHwAddr(buf_handle, axi_id, hw_addr);
#else
    uintptr_t hw_addr;
    osal_ret = R_OSAL_MmngrGetHwAddr(buf_handle, axi_id, &hw_addr);
#endif
    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetSize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t size = 1;

    osal_ret = R_OSAL_MmngrGetSize(buf_handle, &size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrCheckBuffer(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrCheckBuffer(buf_handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrDebugMmngrPrint(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDebugMmngrPrint(mmgr_handle, stdout);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrDebugBufferPrint(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDebugBufferPrint(buf_handle, stdout);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_GetVersion(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_version_t* version;

    version = R_OSAL_GetVersion();

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_region_idx_t region_idx = 0;
    st_osal_mmngr_config_t config;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(region_idx, &config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryBufferObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    struct st_osal_memory_buffer_obj obj;

    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&obj);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryManagerObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    struct st_osal_memory_manager_obj obj;

    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&obj);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrOpenFromRegionIdx(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;
    osal_memory_region_idx_t region_idx = 0;

    config.max_allowed_allocations     = 16;
    config.max_registered_monitors_cbs = 100;
    config.memory_limit = 4096;
    config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;

    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, region_idx, &mmgr_handle);

    return osal_ret;
}
