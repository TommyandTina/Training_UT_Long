#include "stub.h"
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "rcar-xos/osal_configuration/target/zephyr/r_osal_configuration.h"
#include "target/zephyr/r_osal_os_type.h"

#define LOOP_NUM                    (10)
#define API_NUM                     (111)
#define MAX_LEN                     (128)
#define TEST_SET(function)          {#function, performance_##function, {0}, 0, 0, 0, 0, 0, 0, 0, 0, 0}

/* Exclude multiple execution API from measurement */
#define EXCLUDE_MEASUREMENT(cnt)    {                                                       \
                                        if (( 12 == cnt) || /* R_OSAL_ThsyncMutexUnlock */  \
                                            ( 20 == cnt) || /* R_OSAL_ThsyncMutexUnlock */  \
                                            ( 73 == cnt) || /* R_OSAL_IoDeviceOpen      */  \
                                            ( 84 == cnt) || /* R_OSAL_IoDeviceOpen      */  \
                                            ( 83 == cnt) || /* R_OSAL_IoDeviceClose     */  \
                                            ( 86 == cnt) || /* R_OSAL_IoDeviceClose     */  \
                                            (106 == cnt) )  /* R_OSAL_MmngrClose        */  \
                                        {                                                   \
                                            continue;                                       \
                                        }                                                   \
                                    }

typedef struct
{
    char            func_name[MAX_LEN];
    e_osal_return_t (*check_func)(void);
    uint64_t        measurement_time[LOOP_NUM];
    uint64_t        total_time;
    uint64_t        max;
    uint32_t        pthread_create_count;
    uint32_t        pthread_mutex_init_count;
    uint32_t        pthread_cond_init_count;
    uint32_t        malloc_count;
    uint32_t        realloc_count;
    uint32_t        calloc_count;
    uint32_t        timer_create_count;
} st_peformance_test_t;

e_osal_return_t performance_R_OSAL_Initialize(void);
e_osal_return_t performance_R_OSAL_Deinitialize(void);
e_osal_return_t performance_R_OSAL_GetVersion(void);
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
e_osal_return_t performance_R_OSAL_InterruptGetNumOfIrqChannels(void);
e_osal_return_t performance_R_OSAL_InterruptRegisterIsr(void);
e_osal_return_t performance_R_OSAL_InterruptUnregisterIsr(void);
e_osal_return_t performance_R_OSAL_InterruptEnableIsr(void);
e_osal_return_t performance_R_OSAL_InterruptDisableIsr(void);
e_osal_return_t performance_R_OSAL_InterruptIsISRContext(void);
e_osal_return_t performance_R_OSAL_InterruptSetIsrPriority(void);
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
e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryManagerObj(void);
e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryBufferObj(void);
e_osal_return_t performance_R_OSAL_MmngrOpenFromRegionIdx(void);
e_osal_return_t performance_R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(void);

st_peformance_test_t performance_test[API_NUM] =
{
    TEST_SET(R_OSAL_Initialize),
    TEST_SET(R_OSAL_ThreadCreate),
    TEST_SET(R_OSAL_ThreadSleepUntilTimeStamp),
    TEST_SET(R_OSAL_ThreadSleepForTimePeriod),
    TEST_SET(R_OSAL_ThreadSelf),
    TEST_SET(R_OSAL_ThreadEqual),
    TEST_SET(R_OSAL_ThreadJoin),
    TEST_SET(R_OSAL_ThreadInitializeThreadConfigSt),
    TEST_SET(R_OSAL_ThsyncMutexCreate),
    TEST_SET(R_OSAL_ThsyncMutexLockUntilTimeStamp),
    TEST_SET(R_OSAL_ThsyncMutexUnlock),
    TEST_SET(R_OSAL_ThsyncMutexLockForTimePeriod),
    TEST_SET(R_OSAL_ThsyncMutexUnlock),
    TEST_SET(R_OSAL_ThsyncMutexTryLock),
    TEST_SET(R_OSAL_ThsyncCondCreate),
    TEST_SET(R_OSAL_ThsyncCondWaitForTimePeriod),
    TEST_SET(R_OSAL_ThsyncCondWaitUntilTimeStamp),
    TEST_SET(R_OSAL_ThsyncCondBroadcast),
    TEST_SET(R_OSAL_ThsyncCondSignal),
    TEST_SET(R_OSAL_ThsyncCondDestroy),
    TEST_SET(R_OSAL_ThsyncMutexUnlock),
    TEST_SET(R_OSAL_ThsyncMutexDestroy),
    TEST_SET(R_OSAL_ThsyncMemoryBarrier),
    TEST_SET(R_OSAL_MqCreate),
    TEST_SET(R_OSAL_MqSendUntilTimeStamp),
    TEST_SET(R_OSAL_MqReceiveUntilTimeStamp),
    TEST_SET(R_OSAL_MqSendForTimePeriod),
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
    TEST_SET(R_OSAL_InterruptUnregisterIsr),
    TEST_SET(R_OSAL_InterruptIsISRContext),
    TEST_SET(R_OSAL_InterruptSetIsrPriority),
    TEST_SET(R_OSAL_IoDeviceClose),
    TEST_SET(R_OSAL_IoDeviceOpen),
    TEST_SET(R_OSAL_PmSetRequiredState),
    TEST_SET(R_OSAL_PmGetRequiredState),
    TEST_SET(R_OSAL_PmWaitForState),
    TEST_SET(R_OSAL_PmGetState),
    TEST_SET(R_OSAL_PmGetLowestIntermediatePowerState),
    TEST_SET(R_OSAL_PmSetPolicy),
    TEST_SET(R_OSAL_PmGetPolicy),
    TEST_SET(R_OSAL_PmSetResetState),
    TEST_SET(R_OSAL_PmGetResetState),
    TEST_SET(R_OSAL_IoDeviceClose),
    TEST_SET(R_OSAL_IoDeviceOpen),
    TEST_SET(R_OSAL_PmSetPostClock),
    TEST_SET(R_OSAL_IoDeviceClose),
    TEST_SET(R_OSAL_MmngrOpen),
    TEST_SET(R_OSAL_MmngrAlloc),
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
    TEST_SET(R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx),
    TEST_SET(R_OSAL_MmngrClose),
    TEST_SET(R_OSAL_MmngrInitializeMemoryManagerObj),
    TEST_SET(R_OSAL_MmngrInitializeMemoryBufferObj),
    TEST_SET(R_OSAL_GetVersion),
    TEST_SET(R_OSAL_Deinitialize)
};

/* irq callback function */
void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg);

/* mmgr callback function */
void time_callback_mmgr(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl);

#define CPU_CLOCK       (1400000000ULL)
#define SEC2NSEC        (1000000000ULL)
#define PMCR_C          (1 << 2)
#define PMCR_E          (1 << 0)
#define PMCNTENSET_C    (1 << 31)

/* The Create ID of the configuration table */
#define OSAL_THREAD_CREATE_ID   (0xf000U)
#define OSAL_MUTEX_CREATE_ID    (0xf000U)
#define OSAL_COND_CREATE_ID     (0xf000U)
#define OSAL_MQ_CREATE_ID       (0xf000U)

#define TEST_DEVICE_NAME        "ut_dummy_1"
#define TEST_DEVICE_NAME_PM     "ut_dummy_1"
#define TEST_DEVICE_NAME_FBC    "fbc_ut_dummy_1"
#define TEST_DEVICE_TYPE        "ut_dummy"
#define TEST_AXI_BUS_NAME       "ut_axi_dummy_00"

static bool         is_initialized = false;
static bool         is_thread_created = false;
static bool         is_mutex_created = false;
static bool         is_cond_created = false;
static bool         is_mq_created = false;
static bool         is_io_numget = false;
static bool         is_io_devlistget = false;
static uint32_t     io_open_cnt = 0;
static const char*  io_device[] =
{
    TEST_DEVICE_NAME,
    TEST_DEVICE_NAME_PM,
    TEST_DEVICE_NAME_FBC
};

osal_thread_handle_t            th_handle = NULL;
osal_mutex_handle_t             mutex_handle = NULL;
osal_cond_handle_t              cond_handle = NULL;
osal_mq_handle_t                mq_handle = NULL;
st_osal_mq_config_t             mq_config = {1, 4};   // {max_num_msg, msg_size}
osal_device_handle_t            dev_handle = NULL;
osal_memory_manager_handle_t    mmgr_handle = NULL;
osal_memory_buffer_handle_t     buf_handle = NULL;

int main(int argc, char *argv[])
{
    e_osal_return_t osal_ret;
    uint32_t        loop_cnt;
    uint32_t        api_cnt;
    uint32_t        reg;
    uint32_t        cycle_before;
    uint32_t        cycle_after;

    printf("UT performance test start\n");

    reg = PMCNTENSET_C;
    __asm__ volatile("mcr   p15, 0, %0, c9, c12, 1" :: "r" (reg));

    reg = 0;
    __asm__ volatile("mrc   p15, 0, %0, c9, c12, 0" : "=r" (reg));
    reg |= (PMCR_C | PMCR_E);
    __asm__ volatile("mcr   p15, 0, %0, c9, c12, 0" :: "r" (reg));

    __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (reg));

    for (loop_cnt = 0; loop_cnt < LOOP_NUM; loop_cnt++)
    {
        printf("\n");
        printf("LOOP %02u\n", loop_cnt);

        for (api_cnt = 0; api_cnt < API_NUM; api_cnt++)
        {
            init_stub_value();

            __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (cycle_before));
            osal_ret = performance_test[api_cnt].check_func();
            __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (cycle_after));

            EXCLUDE_MEASUREMENT(api_cnt);

            performance_test[api_cnt].measurement_time[loop_cnt]    = ((cycle_after - cycle_before - get_stub_cycle()) * SEC2NSEC) / CPU_CLOCK;
            performance_test[api_cnt].total_time                    += performance_test[api_cnt].measurement_time[loop_cnt];

            if (performance_test[api_cnt].max < performance_test[api_cnt].measurement_time[loop_cnt])
            {
                performance_test[api_cnt].max = performance_test[api_cnt].measurement_time[loop_cnt];
            }
            printf("%-41s, result=%4d, Execution time: %7llu (ns)\n", performance_test[api_cnt].func_name, osal_ret, performance_test[api_cnt].measurement_time[loop_cnt]);

            performance_test[api_cnt].pthread_create_count      = get_pthread_create_count();
            performance_test[api_cnt].pthread_mutex_init_count  = get_pthread_mutex_init_count();
            performance_test[api_cnt].pthread_cond_init_count   = get_pthread_cond_init_count();
            performance_test[api_cnt].malloc_count              = get_malloc_count();
            performance_test[api_cnt].realloc_count             = get_realloc_count();
            performance_test[api_cnt].calloc_count              = get_calloc_count();
            performance_test[api_cnt].timer_create_count        = get_timer_create_count();
        }
    }

    printf("\n");
    printf("Output OS function count\n");

    for (api_cnt = 0; api_cnt < API_NUM; api_cnt++)
    {
        EXCLUDE_MEASUREMENT(api_cnt);
        printf("%-41s, pthread_create=%u, pthread_mutex_init=%u, pthread_cond_init=%u, malloc=%u, realloc=%u, calloc=%u, timer_create=%u\n", performance_test[api_cnt].func_name, performance_test[api_cnt].pthread_create_count, performance_test[api_cnt].pthread_mutex_init_count, performance_test[api_cnt].pthread_cond_init_count, performance_test[api_cnt].malloc_count, performance_test[api_cnt].realloc_count, performance_test[api_cnt].calloc_count, performance_test[api_cnt].timer_create_count);
    }

    printf("\n");
    printf("Output UT performance\n");
    printf("                                                       [ns]\n");
    printf("                                               ave,     max\n");

    for (api_cnt = 0; api_cnt < API_NUM; api_cnt++)
    {
        EXCLUDE_MEASUREMENT(api_cnt);
        printf("%-41s, %7llu, %7llu\n", performance_test[api_cnt].func_name, performance_test[api_cnt].total_time / LOOP_NUM, performance_test[api_cnt].max);
    }

    printf("\n");
    printf("UT performance test finish\n");

    return 0;
}

e_osal_return_t performance_R_OSAL_Initialize(void)
{
    e_osal_return_t                     osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_region_info_list_t*   p_osal_mmngr_region_info_list = (st_osal_mmngr_region_info_list_t *)&g_osal_memory_region_info_list;

    if (is_initialized == false)
    {
        osal_ret = R_OSAL_Initialize();

        if (osal_ret == OSAL_RETURN_OK)
        {
            is_initialized = true;

            strcpy(p_osal_mmngr_region_info_list->p_info->name, "osal_mmngr_00");
            p_osal_mmngr_region_info_list->num_of_memory_regions    = 1;
            p_osal_mmngr_region_info_list->default_memory_region    = 0;
            p_osal_mmngr_region_info_list->p_info->offset           = 0x60000000;
            p_osal_mmngr_region_info_list->p_info->range            = 0x20000000;
        }
    }
    return osal_ret;
}

e_osal_return_t performance_R_OSAL_Deinitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_Deinitialize();
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_initialized  = false;
        io_open_cnt     = 0;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_GetVersion(void)
{
    st_osal_version_t* version;

    version = R_OSAL_GetVersion();

    return OSAL_RETURN_OK;
}

void time_dummy_func(void)
{
    return;
}

e_osal_return_t performance_R_OSAL_ThreadCreate(void)
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
        if (osal_ret == OSAL_RETURN_OK)
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

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadSleepForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThreadSleepForTimePeriod(10);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadSelf(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_thread_handle_t handle;

    osal_ret = R_OSAL_ThreadSelf(&handle);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadEqual(void)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    bool                    result   = false;
    osal_thread_handle_t    th_handle2;

    th_handle2 = th_handle;
    osal_ret = R_OSAL_ThreadEqual(th_handle, th_handle2, &result);
    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadJoin(void)
{
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    int64_t         return_value = 0;

    osal_ret = R_OSAL_ThreadJoin(th_handle, &return_value);
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_thread_created = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThreadInitializeThreadConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;

    osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_mutex_created == false)
    {
        osal_ret = R_OSAL_ThsyncMutexCreate(OSAL_MUTEX_CREATE_ID, &mutex_handle);
        if (osal_ret == OSAL_RETURN_OK)
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
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_mutex_created = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexLockUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncMutexLockForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10);

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

    if (is_cond_created == false)
    {
        osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_CREATE_ID, &cond_handle);
        if (osal_ret == OSAL_RETURN_OK)
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
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_cond_created = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondWaitForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    g_mutex_trylock_err = EBUSY;
    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 10);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ThsyncCondWaitUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    g_mutex_trylock_err = EBUSY;
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

    if (is_mq_created == false)
    {
        osal_ret = R_OSAL_MqCreate(&mq_config, OSAL_MQ_CREATE_ID, &mq_handle);
        if (osal_ret == OSAL_RETURN_OK)
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
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_mq_created = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqSendUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;
    char            send_data[50] = {0};

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;
    osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, &absolute_time, send_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqSendForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            send_data[50] = {0};

    osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 10, send_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqReceiveUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;
    char            recv_data[50];

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &absolute_time, recv_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqReceiveForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            recv_data[50];

    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, recv_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqIsFull(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result   = false;

    osal_ret = R_OSAL_MqIsFull(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqIsEmpty(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result   = false;

    osal_ret = R_OSAL_MqIsEmpty(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqGetConfig(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t config;

    osal_ret = R_OSAL_MqGetConfig(mq_handle, &config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MqReset(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MqReset(mq_handle);

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
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_clock_type_t clock_type = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    st_osal_time_t      absolute_time;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_ClockTimeCalculateTimeDifference(void)
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

e_osal_return_t performance_R_OSAL_InterruptGetNumOfIrqChannels(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          num_channels;

    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);

    return osal_ret;
}

void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
    return;
}

e_osal_return_t performance_R_OSAL_InterruptRegisterIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        arg_data = 0;

    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                           (p_osal_isr_func_t)time_callback_func, (void*)&arg_data);

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

e_osal_return_t performance_R_OSAL_InterruptDisableIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_InterruptIsISRContext(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result;

    osal_ret = R_OSAL_InterruptIsISRContext(&result);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_InterruptSetIsrPriority(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetNumOfDevices(void)
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

e_osal_return_t performance_R_OSAL_IoGetDeviceList(void)
{
    e_osal_return_t osal_ret        = OSAL_RETURN_OK;
    char            *device_type    = TEST_DEVICE_TYPE;
    char            list[400]       = {0};
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

e_osal_return_t performance_R_OSAL_IoDeviceOpen(void)
{
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    st_osal_device_control_t*   p_control;
    st_osal_device_info_t*      dev_info;

    osal_ret = R_OSAL_IoDeviceOpen(io_device[io_open_cnt], &dev_handle);
    if (osal_ret == OSAL_RETURN_OK)
    {
        io_open_cnt++;;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoDeviceClose(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoDeviceClose(dev_handle);
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_io_numget        = false;
        is_io_devlistget    = false;
    }

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead8(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint8_t         read_data = 0;

    osal_ret = R_OSAL_IoRead8(dev_handle, 0x00C0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead16(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint16_t        read_data = 0;

    osal_ret = R_OSAL_IoRead16(dev_handle, 0x00C0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead32(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint32_t        read_data = 0;

    osal_ret = R_OSAL_IoRead32(dev_handle, 0x00C0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoRead64(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint64_t        read_data = 0;

    osal_ret = R_OSAL_IoRead64(dev_handle, 0x00C0, &read_data);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite8(dev_handle, 0x00C0, 0xff);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite16(dev_handle, 0x00C0, 0xff);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite32(dev_handle, 0x00C0, 0xff);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoWrite64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite64(dev_handle, 0x00C0, 0xff);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t         read_data[16];

    osal_ret = R_OSAL_IoBlockRead8(dev_handle, 0x0034, read_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t        read_data[8];

    osal_ret = R_OSAL_IoBlockRead16(dev_handle, 0x0034, read_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        read_data[4];

    osal_ret = R_OSAL_IoBlockRead32(dev_handle, 0x0034, read_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockRead64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t        read_data[2];

    osal_ret = R_OSAL_IoBlockRead64(dev_handle, 0x00C0, read_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite8(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint8_t         p_data[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    osal_ret = R_OSAL_IoBlockWrite8(dev_handle, 0x0034, p_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite16(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint16_t        p_data[8] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000, 0x0100, 0x0000};

    osal_ret = R_OSAL_IoBlockWrite16(dev_handle, 0x0034, p_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite32(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint32_t        p_data[4] = {0x00000000, 0x00000000, 0x00000040, 0x00000100};

    osal_ret = R_OSAL_IoBlockWrite32(dev_handle, 0x0034, p_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoBlockWrite64(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint64_t        p_data[2] = {0x000000FF000000FF, 0x0000000000000000};

    osal_ret = R_OSAL_IoBlockWrite64(dev_handle, 0x00C0, p_data, 16);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetDeviceAxiBusId(void)
{
    e_osal_return_t   osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetDeviceAxiBusId(dev_handle, &axi_id);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiBusIdFromDeviceName(void)
{
    e_osal_return_t   osal_ret = OSAL_RETURN_OK;
    char              *device  = TEST_DEVICE_NAME;
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName(device, &axi_id);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiBusNameList(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            list[400] = {0};
    size_t          num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusNameList(list, sizeof(list), &num_of_byte);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiBusName(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t   axi_id   = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    char                name[200];
    size_t              num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, name, sizeof(name), &num_of_byte);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiIdListForDeviceHdl(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t array[32];
    size_t array_size = 32;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl(dev_handle, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiIdListForDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            *device  = TEST_DEVICE_NAME;
    osal_axi_bus_id_t array[32];
    size_t array_size = 32;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(device, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetAxiIdListForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = TEST_AXI_BUS_NAME;
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
    char            *device  = TEST_DEVICE_NAME;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(device, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_IoGetNumOfAxiIdsForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = TEST_AXI_BUS_NAME;
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

    osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_CG, 10);

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

    osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);

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

    osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);

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
    bool     clock_enable = false;
    uint32_t clock_driver = 10;

    osal_ret = R_OSAL_PmSetPostClock(dev_handle, clock_enable, clock_driver);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrOpen(void)
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
    size_t size      = 4096;
    size_t alignment = 4096;

    if (buf_handle == NULL)
    {
        osal_ret = R_OSAL_MmngrAlloc(mmgr_handle, size, alignment, &buf_handle);
    }

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

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrGetConfig(mmgr_handle, &config);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetOsalMaxConfig(void)
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

e_osal_return_t performance_R_OSAL_MmngrInitializeMmngrConfigSt(void)
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

e_osal_return_t performance_R_OSAL_MmngrRegisterMonitor(void)
{
    e_osal_return_t      osal_ret        = OSAL_RETURN_OK;
    e_osal_mmngr_event_t monitored_event = OSAL_MMNGR_EVENT_ALLOCATE;
    void* user_arg;

    osal_ret = R_OSAL_MmngrRegisterMonitor(mmgr_handle, monitored_event, time_callback_mmgr, &user_arg);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    void            *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetConstCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    const void      *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrFlush(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          offset   = 0x00;
    size_t          size     = 0x40;

    osal_ret = R_OSAL_MmngrFlush(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrInvalidate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          offset   = 0x00;
    size_t          size     = 0x40;

    osal_ret = R_OSAL_MmngrInvalidate(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetHwAddr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    uintptr_t hw_addr;

    osal_ret = R_OSAL_MmngrGetHwAddr(buf_handle, axi_id, &hw_addr);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrGetSize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          size;

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

e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryManagerObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_manager_t tmp_manager;

    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&tmp_manager);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrInitializeMemoryBufferObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_buffer_t tmp_buffer;

    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&tmp_buffer);

    return osal_ret;
}

e_osal_return_t performance_R_OSAL_MmngrOpenFromRegionIdx(void)
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

e_osal_return_t performance_R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(void)
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
