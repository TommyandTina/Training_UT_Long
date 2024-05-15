/***********************************************************************************************************************
* Copyright [2023-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    main.c
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Check about Memory Usage
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           0.1.0     07.04.2023    Create New
 *           0.2.0     09.11.2023    Add definition for V4M
***********************************************************************************************************************/
#include "osal_test_memory.h"

/*** prototype ***/
static int Test_ThreadManager (void);
static int Test_ThreadSyncManager (void);
static int Test_MessageManager (void);
static int Test_ClockTimeManager (void);
static int Test_InterruptManager (void);
static int Test_IOManager (void);
static int Test_PowerManager (void);
static int Test_MemoryManager (void);
static int CheckVersion (void);

/* Test_ThreadManager */
static int64_t osal_test_thread (void *user_arg);

/* Test_ThreadSyncManager */
static int Test_ThreadSyncManager_Mutex (void);
static int Test_ThreadSyncManager_Cond (void);
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

/*** global variable ***/
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
    "R_OSAL_IoBlockRead8   : \0",
    "R_OSAL_IoBlockRead16   : \0",
    "R_OSAL_IoBlockRead32   : \0",
    "R_OSAL_IoBlockRead64   : \0",
    "R_OSAL_IoBlockWrite8   : \0",
    "R_OSAL_IoBlockWrite16   : \0",
    "R_OSAL_IoBlockWrite32   : \0",
    "R_OSAL_IoBlockWrite64   : \0",
    "R_OSAL_IoGetAxiBusIdFromDeviceName   : \0",
    "R_OSAL_IoGetAxiBusName   : \0",
    "R_OSAL_IoGetAxiBusNameList   : \0",
    "R_OSAL_IoGetDeviceAxiBusId   : \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceHdl   : \0",
    "R_OSAL_IoGetNumOfAxiIdsForDeviceName   : \0",
    "R_OSAL_IoGetNumOfAxiIdsForAxiBusName   : \0",
    "R_OSAL_IoGetAxiIdListForDeviceHdl   : \0",
    "R_OSAL_IoGetAxiIdListForDeviceName   : \0",
    "R_OSAL_IoGetAxiIdListForAxiBusName   : \0",
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
    "R_OSAL_MmngrOpenFromRegionIdx   : \0",
    "R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx   : \0",
};

const int64_t expected_value[TEST_NUM] = {
#if defined(RCAR_S4)
    215672,     /* R_OSAL_Initialize */
    -215672,    /* R_OSAL_Deinitialize */
#elif defined(RCAR_V4H)
    550209,     /* R_OSAL_Initialize */
    -550209,    /* R_OSAL_Deinitialize */
#else /* V4M */
    527793,     /* R_OSAL_Initialize */
    -527793,    /* R_OSAL_Deinitialize */
#endif
    0,          /* R_OSAL_GetVersion */
    0,          /* R_OSAL_ThreadCreate */
    0,          /* R_OSAL_ThreadSleepUntilTimeStamp */
    0,          /* R_OSAL_ThreadSleepForTimePeriod */
    0,          /* R_OSAL_ThreadSelf */
    0,          /* R_OSAL_ThreadEqual */
    0,          /* R_OSAL_ThreadInitializeThreadConfigSt */
    0,          /* R_OSAL_ThreadJoin */
    0,          /* R_OSAL_ThsyncMutexCreate */
    0,          /* R_OSAL_ThsyncMutexDestroy */
    0,          /* R_OSAL_ThsyncMutexLockUntilTimeStamp */
    0,          /* R_OSAL_ThsyncMutexLockForTimePeriod */
    0,          /* R_OSAL_ThsyncMutexTryLock */
    0,          /* R_OSAL_ThsyncMutexUnlock */
    0,          /* R_OSAL_ThsyncCondCreate */
    0,          /* R_OSAL_ThsyncCondDestroy */
    0,          /* R_OSAL_ThsyncCondWaitUntilTimeStamp */
    0,          /* R_OSAL_ThsyncCondWaitForTimePeriod */
    0,          /* R_OSAL_ThsyncCondBroadcast */
    0,          /* R_OSAL_ThsyncCondSignal */
    0,          /* R_OSAL_ThsyncMemoryBarrier */
    0,          /* R_OSAL_MqCreate */
    0,          /* R_OSAL_MqDelete */
    0,          /* R_OSAL_MqSendUntilTimeStamp */
    0,          /* R_OSAL_MqSendForTimePeriod */
    0,          /* R_OSAL_MqReceiveUntilTimeStamp */
    0,          /* R_OSAL_MqReceiveForTimePeriod */
    0,          /* R_OSAL_MqIsFull */
    0,          /* R_OSAL_MqIsEmpty */
    0,          /* R_OSAL_MqGetConfig */
    0,          /* R_OSAL_MqReset */
    0,          /* R_OSAL_MqInitializeMqConfigSt */
    0,          /* R_OSAL_ClockTimeGetTimeStamp */
    0,          /* R_OSAL_ClockTimeCalculateTimeDifference */
    0,          /* R_OSAL_InterruptGetNumOfIrqChannels */
    0,          /* R_OSAL_InterruptRegisterIsr */
    0,          /* R_OSAL_InterruptUnregisterIsr */
    0,          /* R_OSAL_InterruptEnableIsr */
    0,          /* R_OSAL_InterruptDisableIsr */
    0,          /* R_OSAL_InterruptIsISRContext */
    0,          /* R_OSAL_InterruptSetIsrPriority */
    0,          /* R_OSAL_IoGetNumOfDevices */
    0,          /* R_OSAL_IoGetDeviceList */
    0,          /* R_OSAL_IoDeviceOpen */
    0,          /* R_OSAL_IoDeviceClose */
    0,          /* R_OSAL_IoRead8 */
    0,          /* R_OSAL_IoRead16 */
    0,          /* R_OSAL_IoRead32 */
    0,          /* R_OSAL_IoRead64 */
    0,          /* R_OSAL_IoWrite8 */
    0,          /* R_OSAL_IoWrite16 */
    0,          /* R_OSAL_IoWrite32 */
    0,          /* R_OSAL_IoWrite64 */
    0,          /* R_OSAL_IoBlockRead8 */
    0,          /* R_OSAL_IoBlockRead16 */
    0,          /* R_OSAL_IoBlockRead32 */
    0,          /* R_OSAL_IoBlockRead64 */
    0,          /* R_OSAL_IoBlockWrite8 */
    0,          /* R_OSAL_IoBlockWrite16 */
    0,          /* R_OSAL_IoBlockWrite32 */
    0,          /* R_OSAL_IoBlockWrite64 */
    0,          /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    0,          /* R_OSAL_IoGetAxiBusName */
    0,          /* R_OSAL_IoGetAxiBusNameList */
    0,          /* R_OSAL_IoGetDeviceAxiBusId */
    0,          /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
    0,          /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    0,          /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    0,          /* R_OSAL_IoGetAxiIdListForDeviceHdl */
    0,          /* R_OSAL_IoGetAxiIdListForDeviceName */
    0,          /* R_OSAL_IoGetAxiIdListForAxiBusName */
    0,          /* R_OSAL_PmSetRequiredState */
    0,          /* R_OSAL_PmGetRequiredState */
    0,          /* R_OSAL_PmWaitForState */
    0,          /* R_OSAL_PmGetState */
    0,          /* R_OSAL_PmSetPolicy */
    0,          /* R_OSAL_PmGetPolicy */
    0,          /* R_OSAL_PmSetResetState */
    0,          /* R_OSAL_PmGetResetState */
    0,          /* R_OSAL_PmGetLowestIntermediatePowerState */
    0,          /* R_OSAL_PmSetPostClock */
    899584,     /* R_OSAL_MmngrOpen */
    -899584,    /* R_OSAL_MmngrClose */
    0,          /* R_OSAL_MmngrAlloc */
    0,          /* R_OSAL_MmngrDealloc */
    0,          /* R_OSAL_MmngrGetConfig */
    0,          /* R_OSAL_MmngrGetOsalMaxConfig */
    0,          /* R_OSAL_MmngrInitializeMmngrConfigSt */
    0,          /* R_OSAL_MmngrRegisterMonitor */
    0,          /* R_OSAL_MmngrGetCpuPtr */
    0,          /* R_OSAL_MmngrGetConstCpuPtr */
    0,          /* R_OSAL_MmngrFlush */
    0,          /* R_OSAL_MmngrInvalidate */
    0,          /* R_OSAL_MmngrGetHwAddr */
    0,          /* R_OSAL_MmngrGetSize */
    0,          /* R_OSAL_MmngrCheckBuffer */
    0,          /* R_OSAL_MmngrDebugMmngrPrint */
    0,          /* R_OSAL_MmngrDebugBufferPrint */
    0,          /* R_OSAL_MmngrInitializeMemoryBufferObj */
    0,          /* R_OSAL_MmngrInitializeMemoryManagerObj */
    899584,     /* R_OSAL_MmngrOpenFromRegionIdx */
    0,          /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
};

static osal_thread_handle_t     th_handle;
static osal_thread_handle_t     th_handle2;

int64_t memory_usage[TEST_NUM];

/*** Common Utils ***/
void Memory_Usage_Before (struct mallinfo *info)
{
    info[0] = mallinfo();
    return ;
}

void Memory_Usage_After (struct mallinfo *info)
{
    info[1] = mallinfo();
    return ;
}

int64_t Memory_Usage_Diff (struct mallinfo *info)
{
    return ((info[1].hblkhd + info[1].usmblks + info[1].uordblks) - (info[0].hblkhd + info[0].usmblks + info[0].uordblks));
}

void Memory_Usage_Print (struct mallinfo *info)
{
    printf("Before Memory Usage\n");
    printf("arena   :%d\n", info[0].arena);
    printf("ordblks :%d\n", info[0].ordblks);
    printf("smblks  :%d\n", info[0].smblks);
    printf("hblks   :%d\n", info[0].hblks);
    printf("hblkhd  :%d\n", info[0].hblkhd);
    printf("usmblks :%d\n", info[0].usmblks);
    printf("fsmblks :%d\n", info[0].fsmblks);
    printf("uordblks:%d\n", info[0].uordblks);
    printf("fordblks:%d\n", info[0].fordblks);
    printf("keepcost:%d\n", info[0].keepcost);
    printf("\nAfter Memory Usage\n");
    printf("arena   :%d\n", info[1].arena);
    printf("ordblks :%d\n", info[1].ordblks);
    printf("smblks  :%d\n", info[1].smblks);
    printf("hblks   :%d\n", info[1].hblks);
    printf("hblkhd  :%d\n", info[1].hblkhd);
    printf("usmblks :%d\n", info[1].usmblks);
    printf("fsmblks :%d\n", info[1].fsmblks);
    printf("uordblks:%d\n", info[1].uordblks);
    printf("fordblks:%d\n", info[1].fordblks);
    printf("keepcost:%d\n", info[1].keepcost);

    return;
}

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
* Start of function main()
***********************************************************************************************************************/
int main(int argc, char *argv[])
{
    (void)argv;
    (void)argc;

    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             ret;
    unsigned int    test_pass = 0;
    unsigned int    test_fail = 0;
    unsigned int    test_skip = 0;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
#if 1 /* IPL workaround */
    /* Required for mmap */
    /* #include <hw/inout.h> */
    /* Required for register address */
    /* #include "r-car-gen3.h" */
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

    OSAL_TEST_INFO("*** R_OSAL_Initialize ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_Initialize();
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_Initialize] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* Check Execution time of GetVersion */
    ret = CheckVersion();

    if (ret == 0)
    {
        /* Check Execution time of Clock & Time Manager */
        ret = Test_ClockTimeManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    if (ret == 0)
    {
        /* Check Execution time of Thread Manager */
        ret = Test_ThreadManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    if (ret == 0)
    {
        /* Check Execution time of ThreadSync Manager */
        ret = Test_ThreadSyncManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    if (ret == 0)
    {
        /* Check Execution time of Message Manager */
        ret = Test_MessageManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    if (ret == 0)
    {
        /* Check Execution time of IO Manager */
        ret = Test_IOManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    if (ret == 0)
    {
        /* Check Execution time of Interrupt Manager */
        ret = Test_InterruptManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    if (ret == 0)
    {
        /* Check Execution time of Power Manager */
        ret = Test_PowerManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    if (ret == 0)
    {
        /* Check Execution time of Memory Manager */
        ret = Test_MemoryManager();
    }
    else
    {
        (void)R_OSAL_Deinitialize();
        return -1;
    }

    OSAL_TEST_INFO("*** R_OSAL_Deinitialize ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_Deinitialize();
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_Deinitialize] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    printf("OSAL API Memory Usage:\n");
    for (int i = 0; i < TEST_NUM; i++)
    {
        OSAL_TEST_INFO("%s\tExpect: %ld [byte], Actual: %ld [byte]\n", api_list[i], expected_value[i], memory_usage[i]);
        if (0 < expected_value[i])
        {
            if (expected_value[i] < memory_usage[i])
            {
                OSAL_TEST_INFO("%s has exceeded the expected memory usage. \n", api_list[i]);
                test_fail++;
            }
            else
            {
                test_pass++;
            }
        }
        else if (0 > expected_value[i])
        {
            if (expected_value[i] > memory_usage[i])
            {
                OSAL_TEST_INFO("%s has exceeded the expected memory usage. \n", api_list[i]);
                test_fail++;
            }
            else
            {
                test_pass++;
            }
        }
        else
        {
            if (expected_value[i] != memory_usage[i])
            {
                OSAL_TEST_INFO("%s has exceeded the expected memory usage. \n", api_list[i]);
                test_fail++;
            }
            else
            {
                test_pass++;
            }
        }
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return 0;
}
/***********************************************************************************************************************
* End of function main()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_ThreadManager()
***********************************************************************************************************************/
static int Test_ThreadManager (void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    st_osal_thread_config_t config;

    int64_t                 arg = 1;
    int64_t                 return_value;
    bool                    result;
    st_osal_time_t          absolute_time;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&config, 0x00, sizeof(st_osal_thread_config_t));
    memset(&th_handle, 0x00, sizeof(osal_thread_handle_t));

    OSAL_TEST_INFO("*** R_OSAL_ThreadInitializeThreadConfigSt ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&config);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThreadInitializeThreadConfigSt] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadInitializeThreadConfigSt is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    config.func       = osal_test_thread;
    config.userarg    = (void *)&arg;
    config.priority = OSAL_THREAD_PRIORITY_LOWEST;
    config.task_name = (char *)"Execution time test thread";
    config.stack_size = 2 * 1024 * 1024;

    OSAL_TEST_INFO("*** R_OSAL_ThreadCreate ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThreadCreate(&config, THREAD_MAIN_TASK_ID, &th_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThreadCreate] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    usleep(10 * 1000 * 1000);

    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
    absolute_time.tv_nsec += WAIT_TIME_NS;
    if(MAX_NS <= absolute_time.tv_nsec)
    {
        absolute_time.tv_sec += 1;
        absolute_time.tv_nsec %= MAX_NS;
    }

    OSAL_TEST_INFO("*** R_OSAL_ThreadSleepUntilTimeStamp ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThreadSleepUntilTimeStamp] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSleepUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    OSAL_TEST_INFO("*** R_OSAL_ThreadSleepForTimePeriod ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(WAIT_TIME_MS);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThreadSleepForTimePeriod] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSleepForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    th_handle2 = th_handle;
    OSAL_TEST_INFO("*** R_OSAL_ThreadEqual ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThreadEqual(th_handle, th_handle2, &result);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThreadEqual] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadEqual is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    OSAL_TEST_INFO("*** R_OSAL_ThreadJoin ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThreadJoin(th_handle, &return_value);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThreadJoin] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadJoin is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

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
    bool                    result;
    st_osal_time_t          absolute_time;

    osal_thread_handle_t const* s_handle = (osal_thread_handle_t *)user_arg;

    /* Memory Usage */
    struct mallinfo info[2];

    usleep(2 * 1000 * 1000);
    OSAL_TEST_INFO("*** Start: sub thread.***\n");
    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(WAIT_TIME_MS);

    OSAL_TEST_INFO("*** R_OSAL_ThreadSelf ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThreadSelf(&self_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThreadSelf] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThreadSelf is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    OSAL_TEST_INFO("*** End: sub thread.***\n");
    return 0;
}
/***********************************************************************************************************************
* End of function osal_test_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_ThreadSyncManager()
***********************************************************************************************************************/
static int Test_ThreadSyncManager (void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int ret;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    ret = Test_ThreadSyncManager_Mutex();
    ret = Test_ThreadSyncManager_Cond();

    OSAL_TEST_INFO("*** R_OSAL_ThsyncMemoryBarrier ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncMemoryBarrier();
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncMemoryBarrier] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMemoryBarrier is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ThreadSyncManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_ThreadSyncManager_Mutex()
***********************************************************************************************************************/
static int Test_ThreadSyncManager_Mutex (void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;

    osal_mutex_id_t     mutex_id;
    osal_mutex_handle_t mutex_handle;

    st_osal_time_t      absolute_time;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&mutex_handle,  0x00, sizeof(osal_mutex_handle_t));
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    mutex_id   = MUTEX_ID_NO1;

    OSAL_TEST_INFO("*** R_OSAL_ThsyncMutexCreate ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncMutexCreate] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncMutexLockForTimePeriod */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** R_OSAL_ThsyncMutexLockForTimePeriod ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, WAIT_TIME_MS);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncMutexLockForTimePeriod] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncMutexUnlock */    /* requires R_OSAL_ThsyncMutexTryLock */
    OSAL_TEST_INFO("*** R_OSAL_ThsyncMutexUnlock ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncMutexUnlock] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncMutexLockUntilTimeStamp */    /* requires R_OSAL_ThsyncMutexCreate */
    GetAbsoluteTime(&absolute_time);

    OSAL_TEST_INFO("*** R_OSAL_ThsyncMutexLockUntilTimeStamp ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncMutexLockUntilTimeStamp] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    (void)R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexTryLock */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** R_OSAL_ThsyncMutexTryLock ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncMutexTryLock] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexTryLock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncMutexUnlock */    /* requires R_OSAL_ThsyncMutexTryLock */
    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexDestroy */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_TEST_INFO("*** R_OSAL_ThsyncMutexDestroy ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncMutexDestroy] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ThreadSyncManager_Mutex()
***********************************************************************************************************************/

/* TEST Thread R_OSAL_ThsyncCondWaitUntilTimeStamp */
int64_t osal_test_cond_waitunittimestamp(void *user_arg)
{
    e_osal_return_t     osal_ret;
    st_arg_cond_t*      arg_cond;

    /* Memory Usage */
    struct mallinfo info[2];

    arg_cond = (st_arg_cond_t *)user_arg;

    /* mutex lock */
    (void)R_OSAL_ThsyncMutexTryLock(arg_cond->mutex_handle);

    /* cond wait */
    OSAL_TEST_INFO("*** R_OSAL_ThsyncCondWaitUntilTimeStamp ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(arg_cond->cond_handle, arg_cond->mutex_handle, &arg_cond->abs_time);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncCondWaitUntilTimeStamp] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondWaitUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    (void)R_OSAL_ThsyncMutexUnlock(arg_cond->mutex_handle);

    return (int64_t)osal_ret;
}


/* TEST Thread R_OSAL_ThsyncCondWaitForTimePeriod */
int64_t osal_test_cond_waitforperiod(void *user_arg)
{
    e_osal_return_t     osal_ret;
    st_arg_cond_t*      arg_cond;

    /* Memory Usage */
    struct mallinfo info[2];

    /* cond wait */
    arg_cond = (st_arg_cond_t *)user_arg;

    /* mutex lock */
    (void)R_OSAL_ThsyncMutexTryLock(arg_cond->mutex_handle);

    OSAL_TEST_INFO("*** R_OSAL_ThsyncCondWaitForTimePeriod ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(arg_cond->cond_handle, arg_cond->mutex_handle, arg_cond->time);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncCondWaitForTimePeriod] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondWaitForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    (void)R_OSAL_ThsyncMutexUnlock(arg_cond->mutex_handle);

    return (int64_t)osal_ret;
}


/***********************************************************************************************************************
* Start of function Test_ThreadSyncManager_Cond()
***********************************************************************************************************************/
static int Test_ThreadSyncManager_Cond (void)
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

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    cond_id     = COND_ID_NO1;
    memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
    memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));

    /* R_OSAL_ThsyncCondCreate */
    OSAL_TEST_INFO("*** R_OSAL_ThsyncCondCreate ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncCondCreate] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_ThsyncMutexCreate */
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    memset(&arg_cond, 0x00, sizeof(st_arg_cond_t));
    memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
    memset(&handle1, 0x00, sizeof(osal_thread_handle_t));

    arg_cond.mutex_handle   = mutex_handle;
    arg_cond.cond_handle    = cond_handle;
    arg_cond.time           = WAIT_TIME_MS;

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
    OSAL_TEST_INFO("*** R_OSAL_ThsyncCondSignal ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncCondSignal] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondSignal is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

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
    config2.userarg     = (void *)&arg_cond;
    config2.priority    = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name   = "R_OSAL_ThsyncCondWaitUntilTimeStamp";
    config2.stack_size  = 1024;

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
    OSAL_TEST_INFO("*** R_OSAL_ThsyncCondBroadcast ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncCondBroadcast] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondBroadcast is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

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
    OSAL_TEST_INFO("*** R_OSAL_ThsyncCondDestroy ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ThsyncCondDestroy] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

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
static int Test_MessageManager (void)
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

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    /* R_OSAL_MqInitializeMqConfigSt */
    OSAL_TEST_INFO("*** R_OSAL_MqInitializeMqConfigSt ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqInitializeMqConfigSt(&mq_config);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqInitializeMqConfigSt] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqInitializeMqConfigSt is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    mq_id = MQ_ID_NO1;
    memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));


    /* R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** R_OSAL_MqCreate ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqCreate] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqCreate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MqGetConfig */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** R_OSAL_MqGetConfig ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqGetConfig(mq_handle, &mq_config);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqGetConfig] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqGetConfig is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MqSendUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);

    OSAL_TEST_INFO("*** R_OSAL_MqSendUntilTimeStamp ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqSendUntilTimeStamp] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqSendUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MqReceiveUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);

    OSAL_TEST_INFO("*** R_OSAL_MqReceiveUntilTimeStamp ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqReceiveUntilTimeStamp] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqReceiveUntilTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    time_period = WAIT_TIME_MS;
    OSAL_TEST_INFO("*** R_OSAL_MqSendForTimePeriod ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, time_period, (void *)&msg, sizeof(msg));
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqSendForTimePeriod] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    time_period = WAIT_TIME_MS;
    OSAL_TEST_INFO("*** R_OSAL_MqReceiveForTimePeriod ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, (void *)rcv_data, sizeof(msg));
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqReceiveForTimePeriod] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MqIsFull */    /* requires R_OSAL_MqCreate */
    is_full = false;
    OSAL_TEST_INFO("*** R_OSAL_MqIsFull ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqIsFull(mq_handle, &is_full);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqIsFull] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqIsFull is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MqIsEmpty */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** R_OSAL_MqIsEmpty ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqIsEmpty(mq_handle, &mq_result);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqIsEmpty] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqIsEmpty is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MqReset */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** R_OSAL_MqReset ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqReset(mq_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqReset] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqReset is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /*R_OSAL_MqDelete */    /* requires R_OSAL_MqCreate */
    OSAL_TEST_INFO("*** R_OSAL_MqDelete ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MqDelete(mq_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MqDelete] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MqDelete is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_MessageManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_ClockTimeManager()
***********************************************************************************************************************/
static int Test_ClockTimeManager (void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time = 0;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    OSAL_TEST_INFO("*** R_OSAL_ClockTimeGetTimeStamp ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ClockTimeGetTimeStamp] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    (void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd);

    OSAL_TEST_INFO("*** R_OSAL_ClockTimeCalculateTimeDifference ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_ClockTimeCalculateTimeDifference] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    return 0;
}
/***********************************************************************************************************************
* End of function Test_ClockTimeManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_IOManager()
***********************************************************************************************************************/
static int Test_IOManager (void)
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
    char*                   device_type = DEVICE_TYPE;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&handle, 0x00, sizeof(osal_device_handle_t));
    list_size = sizeof(list);

    /* R_OSAL_IoGetNumOfDevices */
    OSAL_TEST_INFO("*** R_OSAL_IoGetNumOfDevices ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetNumOfDevices] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfDevices is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetDeviceList */    /* requires R_OSAL_IoGetNumOfDevices */
    OSAL_TEST_INFO("*** R_OSAL_IoGetDeviceList ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetDeviceList] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetDeviceList is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    device = strtok(list, s);
    /* R_OSAL_IoDeviceOpen */    /* requires R_OSAL_IoGetDeviceList */
    OSAL_TEST_INFO("*** R_OSAL_IoDeviceOpen ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoDeviceOpen(device, &handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoDeviceOpen] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceOpen is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED */
    osal_ret = R_OSAL_PmSetRequiredState(handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("Set required state failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoWrite8 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** R_OSAL_IoWrite8 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoWrite8(handle, offset, 0xff);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoWrite8] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoRead8 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_IoRead8 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoRead8(handle, offset, &read_data8);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoRead8] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** R_OSAL_IoWrite16 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoWrite16(handle, offset, 0xff);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoWrite16] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoRead16 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_IoRead16 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoRead16(handle, offset, &read_data16);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoRead16] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** R_OSAL_IoWrite32 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoWrite32(handle, offset, 0xff);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoWrite32] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoRead32 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_IoRead32 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoRead32(handle, offset, &read_data32);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoRead32] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_TEST_INFO("*** R_OSAL_IoWrite64 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoWrite64(handle, offset, 0xff);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoWrite64] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoWrite64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_IoRead64 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoRead64(handle, offset, &read_data64);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoRead64] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoRead64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockWrite8 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 1;
    OSAL_TEST_INFO("*** R_OSAL_IoBlockWrite8 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockWrite8(handle, offset, register_data8, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockWrite8] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockRead8 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data8, 0x00, sizeof(read_block_data8));
    OSAL_TEST_INFO("*** R_OSAL_IoBlockRead8 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_block_data8, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockRead8] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead8 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 2;
    OSAL_TEST_INFO("*** R_OSAL_IoBlockWrite16 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockWrite16(handle, offset, register_data16, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockWrite16] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockRead16 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data16, 0x00, sizeof(read_block_data16));
    OSAL_TEST_INFO("*** R_OSAL_IoBlockRead16 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_block_data16, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockRead16] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead16 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_TEST_INFO("*** R_OSAL_IoBlockWrite32 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockWrite32(handle, offset, register_data32, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockWrite32] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockRead32 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data32, 0x00, sizeof(read_block_data32));
    OSAL_TEST_INFO("*** R_OSAL_IoBlockRead32 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_block_data32, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockRead32] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead32 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    size = 16;
    OSAL_TEST_INFO("*** R_OSAL_IoBlockWrite64 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockWrite64(handle, offset, register_data64, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockWrite64] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockWrite64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoBlockRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_IoBlockRead64 ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_block_data64, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoBlockRead64] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoBlockRead64 is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetDeviceAxiBusId */    /* requires R_OSAL_IoDeviceOpen? */
    OSAL_TEST_INFO("*** R_OSAL_IoGetDeviceAxiBusId ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetDeviceAxiBusId (handle, &axi_id);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetDeviceAxiBusId] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetDeviceAxiBusId is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoDeviceClose */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_IoDeviceClose ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoDeviceClose(handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoDeviceClose] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceClose is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    OSAL_TEST_INFO("*** R_OSAL_IoGetAxiBusIdFromDeviceName ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName(device, &axi_id);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetAxiBusIdFromDeviceName] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiBusIdFromDeviceName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetAxiBusName */
    OSAL_TEST_INFO("*** R_OSAL_IoGetAxiBusName ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, axi_name, sizeof(axi_name), &numOfName);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetAxiBusName] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiBusName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetAxiBusNameList */
    ax_list_size = sizeof(ax_list);
    OSAL_TEST_INFO("*** R_OSAL_IoGetAxiBusNameList ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetAxiBusNameList(ax_list, ax_list_size, &ax_numOfLists);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetAxiBusNameList] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiBusNameList is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* Setup AXI Bus */
    device_type        = DEVICE_TYPE_FOR_AXI;
    axi_bus_name       = AXI_BUS_NAME;

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
    OSAL_TEST_INFO("*** R_OSAL_IoGetNumOfAxiIdsForDeviceHdl ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl( handle, &numOfaxiids );
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetNumOfAxiIdsForDeviceHdl] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForDeviceHdl is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetAxiIdListForDeviceHdl */    /* requires R_OSAL_IoDeviceOpen */
    ax_list_size = sizeof(ax_list);
    OSAL_TEST_INFO("*** R_OSAL_IoGetAxiIdListForDeviceHdl ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl( handle, &ax_list, ax_list_size, &numOfaxiids );
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetAxiIdListForDeviceHdl] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiIdListForDeviceHdl is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    OSAL_TEST_INFO("*** R_OSAL_IoGetNumOfAxiIdsForDeviceName ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName( device, &numOfaxiids );
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetNumOfAxiIdsForDeviceName] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForDeviceName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetAxiIdListForDeviceName */
    ax_list_size = sizeof(ax_list);
    OSAL_TEST_INFO("*** R_OSAL_IoGetAxiIdListForDeviceName ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetAxiIdListForDeviceName( device, &ax_list, ax_list_size, &numOfaxiids );
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetAxiIdListForDeviceName] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiIdListForDeviceName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    OSAL_TEST_INFO("*** R_OSAL_IoGetNumOfAxiIdsForAxiBusName ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name, &numOfaxiids );
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetNumOfAxiIdsForAxiBusName] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetNumOfAxiIdsForAxiBusName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_IoGetAxiIdListForAxiBusName */
    OSAL_TEST_INFO("*** R_OSAL_IoGetAxiIdListForAxiBusName ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name, &ax_list, ax_list_size, &numOfaxiids );
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_IoGetAxiIdListForAxiBusName] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_IoGetAxiIdListForAxiBusName is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

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
* Start of function Test_InterruptManager()
***********************************************************************************************************************/
void osal_test_cb(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{
    return;
}
static int Test_InterruptManager (void)
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
    char*                   device_type = DEVICE_TYPE;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    list_size = sizeof(list);

    /* R_OSAL_InterruptIsISRContext */
    OSAL_TEST_INFO("*** R_OSAL_InterruptIsISRContext ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_InterruptIsISRContext(&result);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_InterruptIsISRContext] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptIsISRContext is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_InterruptGetNumOfIrqChannels */    /* requires R_OSAL_IoDeviceOpen */
    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
    osal_ret = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
    device = strtok(list, s);
    osal_ret = R_OSAL_IoDeviceOpen(device, &dev_handle);

    OSAL_TEST_INFO("*** R_OSAL_InterruptGetNumOfIrqChannels ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_InterruptGetNumOfIrqChannels] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptGetNumOfIrqChannels is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_InterruptRegisterIsr */    /* requires R_OSAL_InterruptGetNumOfIrqChannels */
    OSAL_TEST_INFO("*** R_OSAL_InterruptRegisterIsr ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                               (p_osal_isr_func_t)osal_test_cb, (void *)&arg);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_InterruptRegisterIsr] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_InterruptEnableIsr */        /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** R_OSAL_InterruptEnableIsr ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_InterruptEnableIsr] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_InterruptDisableIsr */    /* requires R_OSAL_InterruptEnableIsr */
    OSAL_TEST_INFO("*** R_OSAL_InterruptDisableIsr ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_InterruptDisableIsr] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_InterruptSetIsrPriority */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** R_OSAL_InterruptSetIsrPriority ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_InterruptSetIsrPriority] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptSetIsrPriority is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_InterruptUnregisterIsr */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_TEST_INFO("*** R_OSAL_InterruptUnregisterIsr ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_InterruptUnregisterIsr] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    (void)R_OSAL_IoDeviceClose(dev_handle);

    return 0;
}
/***********************************************************************************************************************
* End of function Test_InterruptManager()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function Test_PowerManager()
***********************************************************************************************************************/
static int Test_PowerManager (void)
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
    char*                       device_type  = DEVICE_TYPE_FOR_POWER;

    /* Memory Usage */
    struct mallinfo info[2];

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
    OSAL_TEST_INFO("*** R_OSAL_PmSetRequiredState ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmSetRequiredState (dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmSetRequiredState] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetRequiredState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmSetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmSetResetState ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmSetResetState (dev_handle, OSAL_PM_RESET_STATE_APPLIED);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmSetResetState] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetResetState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmGetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmGetState ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmGetState (dev_handle, &pm_state);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmGetState] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmWaitForState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmWaitForState ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmWaitForState (dev_handle, OSAL_PM_STATE_RESET, 1000);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmWaitForState] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmWaitForState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmSetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmSetPolicy ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmSetPolicy (dev_handle, OSAL_PM_POLICY_PG, true);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmSetPolicy] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetPolicy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmGetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmGetPolicy ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmGetPolicy (dev_handle, &policy);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmGetPolicy] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetPolicy is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmGetRequiredState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmGetRequiredState ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmGetRequiredState (dev_handle, &req_state);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmGetRequiredState] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetRequiredState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmGetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmGetResetState ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmGetResetState (dev_handle, &reset_state);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmGetResetState] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetResetState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_PmGetLowestIntermediatePowerState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_TEST_INFO("*** R_OSAL_PmGetLowestIntermediatePowerState ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmGetLowestIntermediatePowerState (dev_handle, &pm_state);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmGetLowestIntermediatePowerState] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmGetLowestIntermediatePowerState is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    (void)R_OSAL_IoDeviceClose(dev_handle);

    /* R_OSAL_PmSetPostClock */    /* requires R_OSAL_IoDeviceOpen */
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    R_OSAL_IoGetNumOfDevices("fba", &numOfDevices);
    R_OSAL_IoGetDeviceList("fba", list, list_size, &numOfLists);
    device = strtok(list, s);
    R_OSAL_IoDeviceOpen(device, &dev_handle);

    OSAL_TEST_INFO("*** R_OSAL_PmSetPostClock ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_PmSetPostClock (dev_handle, true, 63);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_PmSetPostClock] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_PmSetPostClock is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

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

static int Test_MemoryManager (void)
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

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&mm_handle, 0x00, sizeof(osal_memory_manager_handle_t));

    /* R_OSAL_MmngrGetOsalMaxConfig */    /* requires R_OSAL_MmngrOpen */ /* ??? need to be executed before R_OSAL_MmngrOpen()? */
    OSAL_TEST_INFO("*** R_OSAL_MmngrGetOsalMaxConfig ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrGetOsalMaxConfig] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfig is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrOpen ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrOpen(&config , &mm_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrOpen] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrAlloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrAlloc ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrAlloc(mm_handle, size, alignment, &buffer_obj);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrAlloc] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrGetConfig */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrGetConfig ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrGetConfig(mm_handle, &config);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrGetConfig] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetConfig is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrInitializeMmngrConfigSt */    /* requires R_OSAL_MmngrOpen */ /* ??? */
    OSAL_TEST_INFO("*** R_OSAL_MmngrInitializeMmngrConfigSt ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(&config);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrInitializeMmngrConfigSt] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInitializeMmngrConfigSt is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrRegisterMonitor */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrRegisterMonitor ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrRegisterMonitor(mm_handle, monitored_event, osal_memory_mmngr_cb, (void*)&user_arg);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrRegisterMonitor] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrRegisterMonitor is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrGetCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrGetCpuPtr ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrGetCpuPtr(buffer_obj, (void **)&cpu_ptr);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrGetCpuPtr] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetCpuPtr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrGetConstCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrGetConstCpuPtr ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buffer_obj, (void **)&cpu_ptr);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrGetConstCpuPtr] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetConstCpuPtr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrFlush */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrFlush ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrFlush(buffer_obj, offset, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrFlush] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrFlush is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrInvalidate */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrInvalidate ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrInvalidate(buffer_obj, offset, size);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrInvalidate] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInvalidate is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrGetHwAddr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrGetHwAddr ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrGetHwAddr(buffer_obj, axi_id, &hw_addr);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrGetHwAddr] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetHwAddr is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrGetSize */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrGetSize ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrGetSize(buffer_obj, &osize);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrGetSize] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetSize is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrCheckBuffer */    /* requires R_OSAL_MmngrRegisterMonitor, R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrCheckBuffer ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrCheckBuffer(buffer_obj);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrCheckBuffer] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrCheckBuffer is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrDebugMmngrPrint */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrDebugMmngrPrint ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrDebugMmngrPrint(mm_handle, stdout);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrDebugMmngrPrint] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_INFO("R_OSAL_MmngrDebugMmngrPrint is failed, osal_ret = %d\n", osal_ret);
    }

    /* R_OSAL_MmngrDebugBufferPrint */    /* requires R_OSAL_MmngrAlloc */
    OSAL_TEST_INFO("*** R_OSAL_MmngrDebugBufferPrint ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrDebugBufferPrint(buffer_obj, stdout);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrDebugBufferPrint] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_INFO("R_OSAL_MmngrDebugBufferPrint is failed, osal_ret = %d\n", osal_ret);
    }

    /* R_OSAL_MmngrInitializeMemoryBufferObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrInitializeMemoryBufferObj ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&memory_buffer);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrInitializeMemoryBufferObj] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInitializeMemoryBufferObj is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrInitializeMemoryManagerObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrInitializeMemoryManagerObj ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&memory_manager);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrInitializeMemoryManagerObj] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrInitializeMemoryManagerObj is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrDealloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrDealloc ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrDealloc(mm_handle, buffer_obj);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrDealloc] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrDealloc is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrClose */    /* requires R_OSAL_MmngrOpen */
    OSAL_TEST_INFO("*** R_OSAL_MmngrClose ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrClose(mm_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrClose] = Memory_Usage_Diff(&info);;
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    memset(&mm_handle, 0x00, sizeof(osal_memory_manager_handle_t));

    /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
    OSAL_TEST_INFO("*** R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(region_idx, &config);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrGetOsalMaxConfigFromRegionIdx] = Memory_Usage_Diff(&info);;
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

    /* R_OSAL_MmngrOpenFromRegionIdx */
    OSAL_TEST_INFO("*** R_OSAL_MmngrOpenFromRegionIdx ***\n");
    Memory_Usage_Before(&info);
    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config , region_idx, &mm_handle);
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_MmngrOpenFromRegionIdx] = Memory_Usage_Diff(&info);;
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpenFromRegionIdx is failed, osal_ret = %d\n", osal_ret);
        return -1;
    }

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
static int CheckVersion (void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    int             ret;
    int             i;

    /* Memory Usage */
    struct mallinfo info[2];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    OSAL_TEST_INFO("*** R_OSAL_GetVersion ***\n");
    Memory_Usage_Before(&info);
    (void)R_OSAL_GetVersion();
    Memory_Usage_After(&info);
    Memory_Usage_Print(&info);
    memory_usage[TEST_GetVersion] = Memory_Usage_Diff(&info);;

    return 0;
}
/***********************************************************************************************************************
* End of function CheckVersion()
***********************************************************************************************************************/
